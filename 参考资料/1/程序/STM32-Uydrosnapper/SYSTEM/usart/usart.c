#include "sys.h"
#include "usart.h"
#include "pid_for_posture.h"
#include "command_from_pi.h"
#include "HP206C.h"
#include "JY901.h"
#include "linefollow.h"

int pitch_P_gain=1,pitch_I_gain=1,pitch_D_gain=1;
int depth_P_gain=1,depth_I_gain=1,depth_D_gain=1;
int brake_lagtime=12;
int brake_angle=-20;
extern int brake_switch;

//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART3_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART3_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
    USART3->DR = (u8) ch;      
	return ch;
}
#endif 
 
//串口3中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART3_REC_LEN个字节.目前是200个字节
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART2_REC_LEN个字节.目前是200个字节


u16 USART3_RX_STA=0;       //接收状态标记
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA=0;       //接收状态标记
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0xfe
//bit13，	接收到第一个0x0a
//bit12~0，	接收到的有效字节数目


u8 USART1_RX_BUF[USART1_REC_LEN];
__IO u8 USART1_Rx_CNT = 0;


//JY901使用串口1
void IMU_Usart1_Init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//负责接收无线串口的实时指令，应具有最高的优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 
}
void Raspberry_Usart2_Init(u32 bound)
{
//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2，GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
	//USART2_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART2_RX	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //USART2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//负责接收无线串口的实时指令，应具有最高的优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); //初始化串口2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART2, ENABLE);                    //使能串口2 

}




//无线串口使用串口3
void Lora_Usart3_Init(u32 bound){
	
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOD, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//模块功能引脚配置
	//PD10 AUX, PD11 M0, PD12 M1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);//将Lora模块配置为一般模式

  //Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//负责接收无线串口的实时指令，应具有最高的优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
  
   //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART3, &USART_InitStructure); //初始化串口3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断 
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);//串口发送中断
	USART_Cmd(USART3, ENABLE);                    //使能串口3

}

void USART3_IRQHandler(void)
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)，好像XCOM会自动加上回车和换行，那就OK了
	{
		Res =USART_ReceiveData(USART3);	//读取接收到的数据	
		if((USART3_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART3_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART3_RX_STA=0;//接收错误,重新开始
				else USART3_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART3_RX_STA|=0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res;//可见最大支持2^14个字节数据的存放，如果需要更多的字节，那就要把BUF的容量在定义时扩大
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//接收数据错误溢出,重新开始接收	  
				}		 
			}
		}  		
  }

		//串口解码
		if(USART3_RX_STA&0x8000)
		{				  
			int commandtype = USART3_RX_BUF[0]-48;
			printf("commandtype=%d \n",commandtype);
			
			switch (commandtype)
			{	
				case 0: 
								depth_P_gain= 100*(USART3_RX_BUF[2]-48)+10*(USART3_RX_BUF[3]-48)+(USART3_RX_BUF[4]-48);
								depth_I_gain= 100*(USART3_RX_BUF[6]-48)+10*(USART3_RX_BUF[7]-48)+(USART3_RX_BUF[8]-48);
								depth_D_gain= 100*(USART3_RX_BUF[10]-48)+10*(USART3_RX_BUF[11]-48)+(USART3_RX_BUF[12]-48);								
								PID_depth.proportion=PID_depth.proportion*depth_P_gain;
								PID_depth.integral=PID_depth.integral*depth_I_gain;
								PID_depth.derivative=PID_depth.derivative*depth_D_gain;
								printf("For depth: P=%.2f,I=%.2f,D=%.2f\n",PID_depth.proportion,PID_depth.integral,PID_depth.derivative);
								break;
				case 1: 
								incPIDinit(&PID_pitch);incPIDinit(&PID_depth);
								isposeloopclosed=1-isposeloopclosed;
								break;
				case 2: 
//								reset_PID_line();
//								nav_gain=100*(USART3_RX_BUF[2]-48)+10*(USART3_RX_BUF[3]-48)+(USART3_RX_BUF[4]-48);
//								nav_pro=100*(USART3_RX_BUF[6]-48)+10*(USART3_RX_BUF[7]-48)+(USART3_RX_BUF[8]-48);
//								islineloopclosed=1-islineloopclosed;
								break;
				case 3: 
								navi_angle=100*(USART3_RX_BUF[3]-48)+10*(USART3_RX_BUF[4]-48)+(USART3_RX_BUF[5]-48);
								navi_distance=100*(USART3_RX_BUF[8]-48)+10*(USART3_RX_BUF[9]-48)+(USART3_RX_BUF[10]-48);
								navi_angle*=((USART3_RX_BUF[2]=='+')-0.5)*2;
								navi_distance*=((USART3_RX_BUF[7]=='+')-0.5)*2;
								printf("Given angle = %d, Given distance = %d\n",navi_angle,navi_distance);
								navi_angle_compensation=navi_angle-yaw;
								break;
				case 4:	
								Target_depth = 100*(USART3_RX_BUF[2]-48)+10*(USART3_RX_BUF[3]-48)+(USART3_RX_BUF[4]-48);
								Target_pitch = 10*(USART3_RX_BUF[7]-48)+(USART3_RX_BUF[8]-48);
								Target_pitch*=((USART3_RX_BUF[6]=='+')-0.5)*2;
								break;
				case 5:
								pitch_P_gain= 100*(USART3_RX_BUF[2]-48)+10*(USART3_RX_BUF[3]-48)+(USART3_RX_BUF[4]-48);
								pitch_I_gain= 100*(USART3_RX_BUF[6]-48)+10*(USART3_RX_BUF[7]-48)+(USART3_RX_BUF[8]-48);
								pitch_D_gain= 100*(USART3_RX_BUF[10]-48)+10*(USART3_RX_BUF[11]-48)+(USART3_RX_BUF[12]-48);								
								PID_pitch.proportion=PID_pitch.proportion*pitch_P_gain;
								PID_pitch.integral=PID_pitch.integral*pitch_I_gain;
								PID_pitch.derivative=PID_pitch.derivative*pitch_D_gain;
								printf("For pitch: P=%.2f,I=%.2f,D=%.2f\n",PID_pitch.proportion,PID_pitch.integral,PID_pitch.derivative);
								break;
				case 6://发送指令，识别灯带
								USART_SendData(USART2,CMD_NAV_ID);
								break;
				case 7://发送指令，识别字母
								USART_SendData(USART2,CMD_ALPHABET_ID);
								break;
				case 8: 

				//这个用来打印小球坐标
				
				
				
				
				
				
				
								break;
				case 9: 
								left= 100*(USART3_RX_BUF[3]-48)+10*(USART3_RX_BUF[4]-48)+(USART3_RX_BUF[5]-48);
								right= 100*(USART3_RX_BUF[8]-48)+10*(USART3_RX_BUF[9]-48)+(USART3_RX_BUF[10]-48);
								forward= 100*(USART3_RX_BUF[13]-48)+10*(USART3_RX_BUF[14]-48)+(USART3_RX_BUF[15]-48);
								behind= 100*(USART3_RX_BUF[18]-48)+10*(USART3_RX_BUF[19]-48)+(USART3_RX_BUF[20]-48);
								left*=((USART3_RX_BUF[2]=='+')-0.5)*2;
								right*=((USART3_RX_BUF[7]=='+')-0.5)*2;
								forward*=((USART3_RX_BUF[12]=='+')-0.5)*2;
								behind*=((USART3_RX_BUF[17]=='+')-0.5)*2;
								Setpwm_NoCompensation();
								break;
				case 17://对应字母A前缀 
								printf("%d %d %d\n",roll,pitch,yaw);
								break;
				case 18://对应字母B前缀 
								printf("pressure=%d mbar depth= %d cm\n",(int)Hp206c_pressure,(int)Hydrosnapper_hp206c.depth);
								break;
				case 19://对应字母C前缀				
								printf("angle=%d distance=%d\n",navi_angle,navi_distance);
								break;
				case 20://对应字母D前缀			
								USART_SendData(USART2,CMD_END_PI_THREAD_ID);
								printf("End Pi thread\n");
								break;
				case 21://对应字母E前缀，开启定航向角闭环
								isyawloopclosed=1-isyawloopclosed;
								break;
				case 22://对应字母F前缀
								brake_angle=left= 100*(USART3_RX_BUF[3]-48)+10*(USART3_RX_BUF[4]-48)+(USART3_RX_BUF[5]-48);
								brake_angle*=((USART3_RX_BUF[2]=='+')-0.5)*2;
								brake_lagtime=100*(USART3_RX_BUF[7]-48)+10*(USART3_RX_BUF[8]-48)+(USART3_RX_BUF[9]-48);
//								Brake(brake_angle,brake_lagtime);
								break;
				case 23://对应字母G前缀
								left= 100*(USART3_RX_BUF[3]-48)+10*(USART3_RX_BUF[4]-48)+(USART3_RX_BUF[5]-48);
								right= 100*(USART3_RX_BUF[8]-48)+10*(USART3_RX_BUF[9]-48)+(USART3_RX_BUF[10]-48);
								left*=((USART3_RX_BUF[2]=='+')-0.5)*2;
								right*=((USART3_RX_BUF[7]=='+')-0.5)*2;
								Setpwm_LR();
								break;
				case 24://对应字母H前缀
								printf("lef=%d,right=%d\n",left,right);
								break;
				case 25://对应字母I前缀
								test_turn_angle=100*(USART3_RX_BUF[3]-48)+10*(USART3_RX_BUF[4]-48)+(USART3_RX_BUF[5]-48);
								test_turn_angle*=((USART3_RX_BUF[2]=='+')-0.5)*2;
								test_lag_cnt=10*(USART3_RX_BUF[7]-48)+(USART3_RX_BUF[8]-48);
								test_speed = 10*(USART3_RX_BUF[10]-48)+(USART3_RX_BUF[11]-48);
								Turn_leftright(test_turn_angle,test_lag_cnt,test_speed);
								break;
				case 26://对应字母J前缀
								printf("nav=%d distance=%d\n",yaw+navi_angle_compensation,navi_distance);
								break;
				case 27://对应字母K前缀
								yaw_Reset();
								while(yaw!=0){};
								break;
				case 28://对应字母L前缀
								mov_lagtime=100*(USART3_RX_BUF[2]-48)+10*(USART3_RX_BUF[3]-48)+(USART3_RX_BUF[4]-48);
								mov_lagpro=100*(USART3_RX_BUF[6]-48)+10*(USART3_RX_BUF[7]-48)+(USART3_RX_BUF[8]-48);
								Slight_MoveForward(mov_lagtime,mov_lagpro);
								break;
				case 29://对应字母M前缀
								mov_lagtime=100*(USART3_RX_BUF[2]-48)+10*(USART3_RX_BUF[3]-48)+(USART3_RX_BUF[4]-48);
								mov_lagpro=100*(USART3_RX_BUF[6]-48)+10*(USART3_RX_BUF[7]-48)+(USART3_RX_BUF[8]-48);
								Slight_MoveBehind(mov_lagtime,mov_lagpro);
								break;
				case 30://对应字母N前缀
								startswitch=1;
								break;
				case 32://对应字母P前缀
								test_turn_angle=100*(USART3_RX_BUF[3]-48)+10*(USART3_RX_BUF[4]-48)+(USART3_RX_BUF[5]-48);
								test_turn_angle*=((USART3_RX_BUF[2]=='+')-0.5)*2;
								test_lag_cnt=10*(USART3_RX_BUF[7]-48)+(USART3_RX_BUF[8]-48);
								Macroturn_leftright(test_turn_angle,test_lag_cnt);								
								break;
				case 33://对应字母Q前缀
								printf("yaw_speed=%.2f\n",yaw_speed);
								break;
				case 34://对应字母R，发送指令，识别黄球位置
								USART_SendData(USART2,CMD_LOCALIZATION_ID);
								printf("x_error=%d,y_error=%d\n",x_error,y_error);
								break;
				case 35://对应字母S，发送指令，开始寻球
								islocalization_enabled=1-islocalization_enabled;
								break;
				case 36://对应字母T，发送指令，开启寻求闭环
								printf("%d\n",letter_cnt);

				default:break;
		
			}	
				USART3_RX_STA=0;
		}
	  
  if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)                   //这段是为了避免STM32 USART 第一个字节发不出去的BUG 
  { 
     USART_ITConfig(USART3, USART_IT_TXE, DISABLE);					     //禁止发缓冲器空中断， 
  }
		
}






void USART2_IRQHandler(void)      //串口2中断服务程序
{ 
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		Res =USART_ReceiveData(USART2);	//读取接收到的数据
		if((USART2_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART2_RX_STA&0x4000)//接收到了0xfe
			{
				if(Res!=0x0a)
				{
					USART2_RX_BUF[USART2_RX_STA&0X1FFF]=Res;
					USART2_RX_STA++;
					USART2_RX_STA&=(~0x2000);//如果接收的不是连续的0x0a,把之前那个产生的标记给清掉
					if((USART2_RX_STA&0X1FFF)>(USART2_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  					
				}
				else 
				{
					if(USART2_RX_STA&0x2000)
						USART2_RX_STA|=0x8000;	//接收完成了 
					else					
					{
						USART2_RX_STA|=0x2000;
						USART2_RX_BUF[USART2_RX_STA&0X1FFF]=Res;
						USART2_RX_STA++;
						if((USART2_RX_STA&0X1FFF)>(USART2_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  					
					}
				}
			}
			else //还没收到0XFE
			{	
				if(Res==0xfe)
				{
					USART2_RX_STA|=0x4000;
					USART2_RX_BUF[0]=Res;
					USART2_RX_STA++;
				}
				else	USART2_RX_STA=0;//接收错误,重新开始			
			}
		}
  }
	if(USART2_RX_STA&0x8000)
	{				  
		Pi_Stm32_unpackData(USART2_RX_BUF,(USART2_RX_STA&0X1FFF));
		USART2_RX_STA=0;
	}
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)                   //这段是为了避免STM32 USART 第一个字节发不出去的BUG 
     USART_ITConfig(USART2, USART_IT_TXE, DISABLE);					     //禁止发缓冲器空中断， 
}


void USART1_IRQHandler(void)      //串口1 中断服务程序
{ 
  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)	   
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)                   
	{	
		CopeSerial1Data(USART_ReceiveData(USART1)); //处理数据	     
	}
}

