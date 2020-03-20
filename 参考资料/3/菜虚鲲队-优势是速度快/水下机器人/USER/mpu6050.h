#ifndef __GY6500_H
#define __GY6500_H
#include "stm32f10x.h"
#include "delay.h"
#include "algorithm.h"
#include "math.h"
//****************************************
// 定义MPU6050内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define FIFO_ENABLE   0x23
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据，+1为读取

//第二个陀螺仪数据
#define accel_LSB     8192.0
#define gyro_LSB      65.5
#define R_A           57.2958//弧度制转换
#define dt            0.01
#define gyro_biso_x   0.465
#define gyro_biso_y   -0.92
#define gyro_biso_z   -0.025


extern double pitch,roll,yaw;//欧拉角
extern double angular_speed_z;
extern double angle_x_accel,angle_y_accel;

void InitMPU6050(void);//初始化
u8 Single_Read(u8 REG_Address);
int GetData(u8 REG_Address);//获取数据
//void calculate_angle_by_accel(void);
//void calculate_angle_by_gyro(void);

void attitude_algorithm(void);
#endif
