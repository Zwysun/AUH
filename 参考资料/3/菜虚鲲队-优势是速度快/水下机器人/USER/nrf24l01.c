#include "nrf24l01.h"
#include "delay.h"
u8 rx_add[5]={'a','b','c','d','e'};//���͵�ַ
u8 nrf_data[32]={0};//���߷�������
u8 rx_ok=0;
static void spi_init(void)//����spi2������ģ��ͨ��
{
	SPI_InitTypeDef a;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	a.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;
	a.SPI_CPHA=SPI_CPHA_1Edge;
	a.SPI_CPOL=SPI_CPOL_Low;
	a.SPI_DataSize=SPI_DataSize_8b;
	a.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	a.SPI_FirstBit=SPI_FirstBit_MSB;
	a.SPI_Mode=SPI_Mode_Master;
	a.SPI_NSS=SPI_NSS_Soft;
	a.SPI_CRCPolynomial =7; 
	SPI_Init(SPI2,&a);
	SPI_Cmd(SPI2, ENABLE);
}
static void spi_gpio(void)
{
	GPIO_InitTypeDef a;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);
	a.GPIO_Mode=GPIO_Mode_AF_PP;
	a.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_15;
	a.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&a);
	a.GPIO_Mode=GPIO_Mode_Out_PP;
	a.GPIO_Pin=GPIO_Pin_12;
	GPIO_Init(GPIOB,&a);
	a.GPIO_Pin=GPIO_Pin_12;
	GPIO_Init(GPIOA,&a);
	a.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	a.GPIO_Pin=GPIO_Pin_14;
	GPIO_Init(GPIOB,&a);
	a.GPIO_Mode=GPIO_Mode_IPU;
	a.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOA,&a);
}
static void spi_INIT(void)
{
	spi_init();
	spi_gpio();
}
u8 spi_read_write(u8 data)//��д����
{
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI2,data);
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
	return SPI_I2S_ReceiveData(SPI2);
}
static void exti_configuration(void)//�½����ж�
{
	EXTI_InitTypeDef a;   
	a.EXTI_Line=EXTI_Line11;
	a.EXTI_LineCmd=ENABLE;
	a.EXTI_Mode=EXTI_Mode_Interrupt; 
	a.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&a);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);
}
static void exti_INIT(void)
{
	exti_configuration();
}

u8 Read_Reg(u8 RegAddr)//���Ĵ���
{
  u8 btmp;
	CSN(0);
  spi_read_write(RegAddr);	
  btmp=spi_read_write(0xFF);			
  CSN(1);		
  return btmp;
}
void Read_RX(u8 pBuf[],u8 len)//����������
{
	u8 btmp;
	CSN(0);
	spi_read_write(0x61);
	for(btmp=0;btmp<len;btmp++)
  {
    pBuf[btmp]=spi_read_write(0xFF);	
  }
	CSN(1);
}
void Write_Reg(u8 RegAddr,u8 Value)//д�Ĵ���
{
  CSN(0);
  spi_read_write(0x20+RegAddr);
  spi_read_write(Value);			
	CSN(1);
}
void Write_TX(u8 pBuf[],u8 len)//д��������
{
  u8 i;
  CSN(0);
  spi_read_write(0xa0);	
  for(i=0;i<len;i++)
  {
    spi_read_write(pBuf[i]);		
  }
  CSN(1);
}
void Clean_TXFifo(void)//�����������
{
	CSN(0);
	spi_read_write(0xe1);	
	CSN(1);
}
void Clean_RXFifo(void)//�����������
{
	CSN(0);
	spi_read_write(0xe2);	
	CSN(1);
}
void Set_RxAddrP0(u8 pAddr[],u8 len)//����0ͨ�����ܵ�ַ
{
	u8 i;
  CSN(0);
	spi_read_write(0x20+0x0a);
	for(i=0;i<len;i++)
  {
    spi_read_write(pAddr[i]);		
  }
  CSN(1);
}
void Set_TxAddr(u8 pAddr[],u8 len)//���÷��͵�ַ
{
	u8 i;
  CSN(0);
	spi_read_write(0x20+0x10);
	for(i=0;i<len;i++)
  {
    spi_read_write(pAddr[i]);		
  }
  CSN(1);
}
void Read_TxAddr(u8 pAddr[],u8 len)//�����͵�ַ
{
	u8 i;
  CSN(0);
	spi_read_write(0x00+0x10);
	for(i=0;i<len;i++)
  {
    pAddr[i]=spi_read_write(0xff);		
  }
  CSN(1);
}
u8 Read_Status(void)//��״̬������ж�
{
  u8 status,i;
  status=Read_Reg(0x07);
	Write_Reg(0x07,status);//��������дһ���������ж� д1���жϣ����⣩
  for(i=4;i<7;i++)
	{
		if(status&(1<<i)) break;
	}
  return i;
}
void NRF24L01_check()//��������ģ���Ƿ�����
{
	u8 i;
	u8 buf[5]={'i','l','o','v','e'};
	u8 read_buf[5]={0}; 
	spi_INIT();
	while(1)
	{
		Set_TxAddr(buf,5);			
		Read_TxAddr(read_buf,5);
		for(i=0;i<5;i++)
		{
			if(buf[i]!=read_buf[i])
			{
				i=0;
				Set_TxAddr(buf,5);			
		    Read_TxAddr(read_buf,5);
			}	
		} 
		if(5==i)
		{
			break;				
		}
	}
}
void NRF24L01_Init(void)//����ģ���ʼ�� ���ܳ���Ҫ�뷢������ܳ�����ͬ ��ȡ�������� д��������ʱҲҪ��ͬ���� ���һ�������Զ�Ӧ�� ��һ��Ҳ���뿪�Զ�Ӧ����ܽ��� �ܽ�һ�� ȫ��һ��
{
	CE(0);
	Write_Reg(0x11,0x20);//��������32�ֽ� ��д����30��ʱ��˾�һ��Ҫ����д���͵�ַǰ �����͵�ַ�Ḵλ �²�ӦΪbug
	Set_TxAddr(rx_add,5);//д���͵�ַ 
	Set_RxAddrP0(rx_add,5);//д����ͨ��0��ַ
	Write_Reg(0x00,rx_mode);//���÷���ģʽ
	Write_Reg(0x01,0xff);//��������ͨ���Զ�Ӧ��
	Write_Reg(0x02,0x01);//����p0ͨ������
	Write_Reg(0x04,0x07);//�Զ��ط�ʱ��336us ����5
	Write_Reg(0x05,0x02);//����ͨ��Ƶ��2
	Write_Reg(0x06,0x0f);//����2m �������
	Read_Status();
	Clean_RXFifo();
	Clean_TXFifo();
	exti_INIT();
	CE(1);
}
void nrf_rx(void)
{
	CE(0);
	Read_Status();
	Clean_RXFifo();
	Write_Reg(0x00,rx_mode);//���ý���ģʽ
}
void nrf_tx(void)
{
	CE(0);
	Read_Status();
	Clean_TXFifo();
	Write_Reg(0x00,tx_mode);//���÷���ģʽ
}
void Delay_us(u16 time)
{    
   u8 i=0;  
   while(time--)
   {
      i=10;  
      while(i--);    
   }
}
void tx_data_launch(void)//�������ݷ���
{
	CE(1);
	Delay_us(11);//CE�ø�10us��������
	CE(0);
}
void deal_with_interrupt(void)
{
	CE(0);
	switch(Read_Status())
	{
		case RX_DR://���յ�����
		Read_RX(nrf_data,32);
		duty_all=nrf_data[0]+(nrf_data[1]<<8);
		nrf_rx();
		CE(1);
//		nrf_data[0]=(u8)(pitch*100+18000.5);
//		nrf_data[1]=(u16)(pitch*100+18000.5)>>8;
//		nrf_data[2]=(u8)(roll*100+18000.5);
//		nrf_data[3]=(u16)(roll*100+18000.5)>>8;
//		nrf_data[4]=(u8)(angular_speed_z*100+50000.5);
//		nrf_data[5]=(u16)(angular_speed_z*100+50000.5)>>8;
//		nrf_data[6]=(u8)(pitch_pid.uk*100+100000.5);
//		nrf_data[7]=(u16)(pitch_pid.uk*100+100000.5)>>8;
//		nrf_data[8]=(u8)(roll_pid.uk*100+100000.5);
//		nrf_data[9]=(u16)(roll_pid.uk*100+100000.5)>>8;
//		nrf_data[10]=(u8)(angular_speed_z_pid.uk*100+100000.5);
//		nrf_data[11]=(u16)(angular_speed_z_pid.uk*100+100000.5)>>8;
//		nrf_tx();  
//		Write_TX(nrf_data,32);
//		tx_data_launch();
		
		break;
		
		case TX_DS://����ɹ�
//		rx_ok=1;
//		nrf_rx();
//		CE(1);
		break;
		
		case MAX_RT://����ʧ��
		break;
	}
}
