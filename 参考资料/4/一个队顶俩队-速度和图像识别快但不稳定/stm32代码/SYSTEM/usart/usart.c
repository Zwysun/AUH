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

//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART3_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART3_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
    USART3->DR = (u8) ch;      
	return ch;
}
#endif 
 
//����3�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART3_REC_LEN���ֽ�.Ŀǰ��200���ֽ�
u8 USART2_RX_BUF[USART2_REC_LEN];     //���ջ���,���USART2_REC_LEN���ֽ�.Ŀǰ��200���ֽ�


u16 USART3_RX_STA=0;       //����״̬���
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART2_RX_STA=0;       //����״̬���
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0xfe
//bit13��	���յ���һ��0x0a
//bit12~0��	���յ�����Ч�ֽ���Ŀ


u8 USART1_RX_BUF[USART1_REC_LEN];
__IO u8 USART1_Rx_CNT = 0;


//JY901ʹ�ô���1
void IMU_Usart1_Init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//����������ߴ��ڵ�ʵʱָ�Ӧ������ߵ����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}
void Raspberry_Usart2_Init(u32 bound)
{
//GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART2��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
	//USART2_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART2_RX	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //USART2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//����������ߴ��ڵ�ʵʱָ�Ӧ������ߵ����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART2, &USART_InitStructure); //��ʼ������2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���2 

}




//���ߴ���ʹ�ô���3
void Lora_Usart3_Init(u32 bound){
	
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOD, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//ģ�鹦����������
	//PD10 AUX, PD11 M0, PD12 M1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);//��Loraģ������Ϊһ��ģʽ

  //Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//����������ߴ��ڵ�ʵʱָ�Ӧ������ߵ����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
  
   //USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure); //��ʼ������3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж� 
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);//���ڷ����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3

}

void USART3_IRQHandler(void)
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)������XCOM���Զ����ϻس��ͻ��У��Ǿ�OK��
	{
		Res =USART_ReceiveData(USART3);	//��ȡ���յ�������	
		if((USART3_RX_STA&0x8000)==0)//����δ���
		{
			if(USART3_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART3_RX_STA=0;//���մ���,���¿�ʼ
				else USART3_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART3_RX_STA|=0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res;//�ɼ����֧��2^14���ֽ����ݵĴ�ţ������Ҫ������ֽڣ��Ǿ�Ҫ��BUF�������ڶ���ʱ����
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//�������ݴ������,���¿�ʼ����	  
				}		 
			}
		}  		
  }

		//���ڽ���
		if(USART3_RX_STA&0x8000)
		{				  
			int commandtype = USART3_RX_BUF[0]-48;
			printf("commandtype=%d \n",commandtype);
			
			switch (commandtype)
			{	
				
				case 1: 
								incPIDinit(&PID_pitch);incPIDinit(&PID_depth);
								isposeloopclosed=1-isposeloopclosed;
								break;
				case 7://����ָ�ʶ����ĸ
								USART_SendData(USART2,CMD_ALPHABET_ID);
								break;
				
				case 30://��Ӧ��ĸNǰ׺
								startswitch=1;
								break;
				default:break;
		
			}	
				USART3_RX_STA=0;
		}
	  
  if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)                   //�����Ϊ�˱���STM32 USART ��һ���ֽڷ�����ȥ��BUG 
  { 
     USART_ITConfig(USART3, USART_IT_TXE, DISABLE);					     //��ֹ�����������жϣ� 
  }
		
}






void USART2_IRQHandler(void)      //����2�жϷ������
{ 
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		Res =USART_ReceiveData(USART2);	//��ȡ���յ�������
		if((USART2_RX_STA&0x8000)==0)//����δ���
		{
			if(USART2_RX_STA&0x4000)//���յ���0xfe
			{
				if(Res!=0x0a)
				{
					USART2_RX_BUF[USART2_RX_STA&0X1FFF]=Res;
					USART2_RX_STA++;
					USART2_RX_STA&=(~0x2000);//������յĲ���������0x0a,��֮ǰ�Ǹ������ı�Ǹ����
					if((USART2_RX_STA&0X1FFF)>(USART2_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  					
				}
				else 
				{
					if(USART2_RX_STA&0x2000)
						USART2_RX_STA|=0x8000;	//��������� 
					else					
					{
						USART2_RX_STA|=0x2000;
						USART2_RX_BUF[USART2_RX_STA&0X1FFF]=Res;
						USART2_RX_STA++;
						if((USART2_RX_STA&0X1FFF)>(USART2_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  					
					}
				}
			}
			else //��û�յ�0XFE
			{	
				if(Res==0xfe)
				{
					USART2_RX_STA|=0x4000;
					USART2_RX_BUF[0]=Res;
					USART2_RX_STA++;
				}
				else	USART2_RX_STA=0;//���մ���,���¿�ʼ			
			}
		}
  }
	if(USART2_RX_STA&0x8000)
	{				  
		Pi_Stm32_unpackData(USART2_RX_BUF,(USART2_RX_STA&0X1FFF));
		USART2_RX_STA=0;
	}
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)                   //�����Ϊ�˱���STM32 USART ��һ���ֽڷ�����ȥ��BUG 
     USART_ITConfig(USART2, USART_IT_TXE, DISABLE);					     //��ֹ�����������жϣ� 
}


void USART1_IRQHandler(void)      //����1 �жϷ������
{ 
  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)	   
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)                   
	{	
		CopeSerial1Data(USART_ReceiveData(USART1)); //��������	     
	}
}

