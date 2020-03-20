#include "mpu6050.h"
double pitch,roll,yaw;//欧拉角
double angular_speed_z;
double angle_x_accel,angle_y_accel;
static void i2c1_init(void)//b8 b9
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;

	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
}
//ZRX          
//单字节写入*******************************************

void Single_Write(u8 REG_Address,u8 REG_data)		    
{
  I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C_SendData(I2C1, REG_Address);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  I2C_SendData(I2C1, REG_data); 
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  I2C_GenerateSTOP(I2C1, ENABLE);
}

//单字节读取*****************************************
u8 Single_Read(u8 REG_Address)
{   
	u8 REG_data;     	
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C_SendData(I2C1, REG_Address);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Receiver);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)==0);
	REG_data=I2C_ReceiveData(I2C1);
  I2C_AcknowledgeConfig(I2C1, ENABLE);
	return REG_data;
}		
//******************************************************************************************************
//初始化MPU6050
//******************************************************************************************************
void InitMPU6050(void)
{
	i2c1_init();
	Single_Write(PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_Write(SMPLRT_DIV, 0x00);
	Single_Write(CONFIG, 0x06);
	Single_Write(GYRO_CONFIG, 0x08);//500
	Single_Write(ACCEL_CONFIG, 0x08);//4g
//	Single_Write(FIFO_ENABLE,0x00);
}
//******************************************************************************************************
//合成数据
//******************************************************************************************************
signed int GetData(u8 REG_Address)
{
	u8 H,L;
	int data;
	H=Single_Read(REG_Address);
	L=Single_Read(REG_Address+1);
	data=(H<<8)+L;
	if(data>32767) data-=65536;
	return data;   //合成数据
}
void attitude_algorithm(void)
{
	double ax,ay,az,gx,gy,gz,g;
	
	ax=GetData(ACCEL_XOUT_H)/accel_LSB; 
	KalmanFilter(&ax_filter,0,ax);
	
	ay=GetData(ACCEL_YOUT_H)/accel_LSB;
	KalmanFilter(&ay_filter,0,ay);
	
	az=GetData(ACCEL_ZOUT_H)/accel_LSB;
	KalmanFilter(&az_filter,0,az);
	
	g=sqrt(ax_filter.x_k_k*ax_filter.x_k_k+ay_filter.x_k_k*ay_filter.x_k_k+az_filter.x_k_k*az_filter.x_k_k);
	
	angle_x_accel=asin(ax_filter.x_k_k/g)*R_A;
	angle_y_accel=asin(ay_filter.x_k_k/g)*R_A;
//	az=asin(az_filter.x_k_k/g)*R_A;
	
	gx=GetData(GYRO_XOUT_H)/gyro_LSB; 
	KalmanFilter(&gx_filter,0,gx);
	
	gy=GetData(GYRO_YOUT_H)/gyro_LSB;
	KalmanFilter(&gy_filter,0,gy);
	
	gz=GetData(GYRO_ZOUT_H)/gyro_LSB;
	KalmanFilter(&gz_filter,0,gz);
	
	Position_PID(&x_correction_pid,angle_x_accel-roll);
	Position_PID(&y_correction_pid,angle_y_accel-pitch);
	
	pitch+=(gx_filter.x_k_k-gyro_biso_x)*dt-y_correction_pid.uk;
	roll-=(gy_filter.x_k_k+gyro_biso_y)*dt+x_correction_pid.uk;
	angular_speed_z=gz_filter.x_k_k+gyro_biso_z; 
	yaw-=angular_speed_z*dt;
}
