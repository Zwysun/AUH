#ifndef __HP206C_H
#define __HP206C_H
#include "sys.h"

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				        //发送IIC开始信号
void IIC_Stop(void);	  			      //发送IIC停止信号
void IIC_Send_Byte(u8 txd);			    //IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				      //IIC等待ACK信号
void IIC_Ack(void);					        //IIC发送ACK信号
void IIC_NAck(void);				        //IIC不发送ACK信号

//IO方向设置
void SDA_In(void);
void SDA_Out(void);


#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 

typedef unsigned int  uint;
typedef unsigned char uchar;
typedef unsigned long ulong;
typedef struct{
	int  temperture;
	int  pressure;
}HP206C_t;

typedef struct
{
	float Temperature;
	int32_t init_value;
	int32_t depth;
}PressureMsg_t;

#define HP206C_W_ADDRESS 0xec
#define HP206C_R_ADDRESS 0xed
#define HP20X_I2C_DEV_ID       0xEC//CSB PIN is VDD level(address is 0x76)
#define HP20X_I2C_DEV_ID2      0XEE //CSB PIN is GND level(address is 0x77)
#define HP20X_SOFT_RST         0x06
#define HP20X_WR_CONVERT_CMD   0x40
#define HP20X_CONVERT_OSR4096  0<<2
#define HP20X_CONVERT_OSR2048  1<<2
#define HP20X_CONVERT_OSR1024  2<<2  
#define HP20X_CONVERT_OSR512   3<<2
#define HP20X_CONVERT_OSR256   4<<2
#define HP20X_CONVERT_OSR128   5<<2

#define HP20X_READ_P           0x30  //read_p command
#define HP20X_READ_A           0x31  //read_a command
#define HP20X_READ_T           0x32  //read_t command
#define HP20X_READ_PT          0x10  //read_pt command
#define HP20X_READ_AT          0x11  //read_at command
#define HP20X_READ_CAL		     0X28	 //RE-CAL ANALOG

#define HP20X_WR_REG_MODE     0xC0
#define HP20X_RD_REG_MODE     0x80

#define ERR_WR_DEVID_NACK       0x01    
#define ERR_RD_DEVID_NACK       0x02    
#define ERR_WR_REGADD_NACK      0x04   
#define ERR_WR_REGCMD_NACK      0x08   
#define ERR_WR_DATA_NACK        0x10     
#define ERR_RD_DATA_MISMATCH    0x20 

#define I2C_DID_WR_MASK         0xFE
#define I2C_DID_RD_MASK         0x01

#define T_WIN_EN                0X01
#define PA_WIN_EN               0X02
#define T_TRAV_EN               0X04
#define PA_TRAV_EN              0X08
#define PA_RDY_EN               0X20
#define T_RDY_EN                0X10

#define T_WIN_CFG               0X01
#define PA_WIN_CFG              0X02
#define PA_MODE_P               0X00
#define PA_MODE_A               0X40

#define T_TRAV_CFG              0X04

#define HP206C_IIC    hi2c1


/*Table 5: Conversion Time VS OSR
OSR     Conversion Time (ms)
         Temperature |Temperature and Pressure (or Altitude)
128         2.1      |      4.1 
256         4.1      |      8.2 
512         8.2      |      16.4  
1024        16.4     |      32.8
2048        32.8     |      65.6   
4096        65.6     |      131.1 
*/
#define  HP206C_BUFFER_LEN       ( 100 )//压传数组总个数
#define  HP206C_MINUS_NUM				( 10  )//左右各滤除个数
 
void HP206C_Init(void);
void GetHp206cData(PressureMsg_t *dev);

extern PressureMsg_t  Hydrosnapper_hp206c;

#endif

