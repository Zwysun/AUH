#ifndef __GY6500_H
#define __GY6500_H
#include "stm32f10x.h"
#include "delay.h"
#include "algorithm.h"
#include "math.h"
//****************************************
// ����MPU6050�ڲ���ַ
//****************************************
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
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
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I		0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	SlaveAddress	0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ

//�ڶ�������������
#define accel_LSB     8192.0
#define gyro_LSB      65.5
#define R_A           57.2958//������ת��
#define dt            0.01
#define gyro_biso_x   0.465
#define gyro_biso_y   -0.92
#define gyro_biso_z   -0.025


extern double pitch,roll,yaw;//ŷ����
extern double angular_speed_z;
extern double angle_x_accel,angle_y_accel;

void InitMPU6050(void);//��ʼ��
u8 Single_Read(u8 REG_Address);
int GetData(u8 REG_Address);//��ȡ����
//void calculate_angle_by_accel(void);
//void calculate_angle_by_gyro(void);

void attitude_algorithm(void);
#endif
