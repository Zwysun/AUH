#include "bmp085.h"
/*********���̿���ֲ����*******/
short ac1;
short ac2; 
short ac3; 
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1; 
short b2;
short mb;
short mc;
short md;

#define sda(a) if(a) GPIO_SetBits(GPIOB,GPIO_Pin_11);else GPIO_ResetBits(GPIOB,GPIO_Pin_11)
#define scl(a) if(a) GPIO_SetBits(GPIOB,GPIO_Pin_10);else GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define read_sda GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)
void i2c_INIT(void)//���ģ��i2c stm32������i2c���������
{
	GPIO_InitTypeDef a;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	a.GPIO_Mode=GPIO_Mode_Out_OD;
	a.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	a.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&a);
}
void out_sda(void)//�ı�sda���ŵ��������ģʽ
{
	GPIO_InitTypeDef a;
	a.GPIO_Mode=GPIO_Mode_Out_OD;
	a.GPIO_Speed=GPIO_Speed_50MHz;
	a.GPIO_Pin=GPIO_Pin_8;
	GPIO_Init(GPIOB,&a);
}
void in_sda(void)
{
	GPIO_InitTypeDef a;
	a.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	a.GPIO_Pin=GPIO_Pin_8;
	GPIO_Init(GPIOB,&a);
}
void start(void)//���к�����βͳһ����scl����ʱ �������к�����ͷ������Ҫȷ��scl��ƽ ����ʱ��������Ӷ�����ʱ
{
	sda(1);
	delay_us(1);
	scl(1);
	delay_us(5);
	sda(0);//��sclΪ�ߵ�ƽʱ����sdaΪ��ʼ�ź�
	delay_us(5);
	scl(0);
	delay_us(1);
}
void stop(void)
{
	sda(0);
	delay_us(1);
	scl(1);
	delay_us(5);
	sda(1);//��sclΪ�͵�ƽʱ����sdaΪֹͣ�ź�
	delay_us(5);
	scl(0);
}
void send_data(u8 data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(data&0x80) sda(1);
		else sda(0);
		data<<=1;
		delay_us(1);
		scl(1); 
		delay_us(5);
		scl(0);
		delay_us(5);
	}
}
u8 receive_data(void)
{
	u8 i,data;
	sda(1);
	in_sda();//�ڽ�������ǰ�Ƚ�sda����Ϊ���� �����������Ϊ���
	for(i=0;i<8;i++)
	{
	  data<<=1;
		scl(1);
		delay_us(5);
		if(read_sda!=0) data|=0x01;
		delay_us(1);
		scl(0);
		delay_us(5);
	}  
	out_sda();
	return data;
}
void ack(void)//sdaΪ0ʱ��ack
{
	sda(0);
	delay_us(1);
	scl(1);
	delay_us(5);
	scl(0);
  delay_us(5);
}
void nack(void)//sdaΪ1ʱ��no ack
{
	sda(1);
	delay_us(1);
	scl(1);
	delay_us(5);
	scl(0);
  delay_us(5);
}
u8 wait_ack(void)//�˺���Ϊ�����Ӧ���ź�
{ 
	u8 data;
	sda(1);
	in_sda();
	scl(1);
	delay_us(5);
	data=read_sda;
	scl(0);
  delay_us(5);
	out_sda();
	while(data);
	return data;
}
/*
//���ֽ�д��BMP085�ڲ�����*******************************

void Single_Write(u8 SlaveAddress,u8 REG_Address,u8 REG_data)
{
  start();                  //��ʼ�ź�
  send_data(SlaveAddress);   //�����豸��ַ+д�ź�
  ack();
  send_data(REG_Address);    //�ڲ��Ĵ�����ַ
  ack();
  send_data(REG_data);       //�ڲ��Ĵ�������
  ack();
  stop();                   //����ֹͣ�ź�
}
*/
/*
//���ֽڶ�ȡBMP085�ڲ�����********************************
u8 Single_Read(u8 REG_Address)
{  
  u8 REG_data;
  start();                          //��ʼ�ź�
  send_data(BMP085_SlaveAddress);           //�����豸��ַ+д�ź�
	ack();
  send_data(REG_Address);            //���ʹ洢��Ԫ��ַ	
	ack();
  start();                          //��ʼ�ź�
  send_data(BMP085_SlaveAddress+1);         //�����豸��ַ+���ź�
	ack();
  REG_data=receive_data();              //�����Ĵ�������
	nack();   
	stop();                           //ֹͣ�ź�
  return REG_data; 
}
*/
//*********************************************************
//����BMP085�ڲ�����,��������
//*********************************************************
short Multiple_read(u8 ST_Address)
{   
	u8 msb, lsb;
	short _data;
  start();                          //��ʼ�ź�
  send_data(BMP085_SlaveAddress);    //�����豸��ַ+д�ź�
	wait_ack();
  send_data(ST_Address);             //���ʹ洢��Ԫ��ַ
	wait_ack();
  start();                          //��ʼ�ź�
  send_data(BMP085_SlaveAddress+1);         //�����豸��ַ+���ź�
	wait_ack();
  msb = receive_data();                 //BUF[0]�洢
  ack();                       //��ӦACK
  lsb = receive_data();     
	nack();                       //���һ��������Ҫ��NOACK
  stop();                           //ֹͣ�ź�
  delay_ms(5);
  _data = msb << 8;
	_data |= lsb;	
	return _data;
}
//********************************************************************
long bmp085ReadTemp(void)
{
  start();                  //��ʼ�ź�
  send_data(BMP085_SlaveAddress);   //�����豸��ַ+д�ź�
	wait_ack();
  send_data(0xF4);	          // write register address
	wait_ack();
  send_data(0x2E);       	// write register data for temp
	wait_ack();
  stop();                   //����ֹͣ�ź�
	delay_ms(5);	// max time is 4.5ms
	return (long) Multiple_read(0xF6);
}
//*************************************************************
long bmp085ReadPressure(void)
{
	long pressure = 0;
  start();                   //��ʼ�ź�
  send_data(BMP085_SlaveAddress);   //�����豸��ַ+д�ź�
	ack();
  send_data(0xF4);	          // write register address
	ack();
  send_data(0x34);       	  // write register data for pressure
	ack();
  stop();                    //����ֹͣ�ź�
	delay_ms(5);    	                  // max time is 4.5ms	
  pressure = Multiple_read(0xF6);
	pressure &= 0x0000FFFF;
	return pressure;	
	//return (long) bmp085ReadShort(0xF6);
}

//**************************************************************

//��ʼ��BMP085��������Ҫ��ο�pdf�����޸�**************
void Init_BMP085(void)
{
	ac1 = Multiple_read(0xAA);
	ac2 = Multiple_read(0xAC);
	ac3 = Multiple_read(0xAE);
	ac4 = Multiple_read(0xB0);
	ac5 = Multiple_read(0xB2);
	ac6 = Multiple_read(0xB4);
	b1 =  Multiple_read(0xB6);
	b2 =  Multiple_read(0xB8);
	mb =  Multiple_read(0xBA);
	mc =  Multiple_read(0xBC);
	md =  Multiple_read(0xBE);
}
//***********************************************************************
void bmp085Convert()
{
	long ut;
	long up;
	long x1, x2, b5, b6, x3, b3, p;
	unsigned long b4, b7;
	long  temperature;
	long  pressure;	
	ut = bmp085ReadTemp();
	ut = bmp085ReadTemp();	   // ��ȡ�¶�
	up = bmp085ReadPressure();
	up = bmp085ReadPressure();  // ��ȡѹǿ
	x1 = ((long)ut - ac6) * ac5 >> 15;
	x2 = ((long) mc << 11) / (x1 + md);
	b5 = x1 + x2;
	temperature = (b5 + 8) >> 4;
	b6 = b5 - 4000;
	x1 = (b2 * (b6 * b6 >> 12)) >> 11;
	x2 = ac2 * b6 >> 11;
	x3 = x1 + x2;
	b3 = (((long)ac1 * 4 + x3) + 2)/4;
	x1 = ac3 * b6 >> 13;
	x2 = (b1 * (b6 * b6 >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (ac4 * (unsigned long) (x3 + 32768)) >> 15;
	b7 = ((unsigned long) up - b3) * (50000 >> OSS);
	if( b7 < 0x80000000) p = (b7 * 2) / b4 ;
  else p = (b7 / b4) * 2;
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
  pressure = p + ((x1 + x2 + 3791) >> 4);
}
