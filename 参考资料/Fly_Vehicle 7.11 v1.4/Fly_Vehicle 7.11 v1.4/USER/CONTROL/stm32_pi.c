#include "stm32_pi.h"
extern float navi_center_to_line_angle;
extern void LED_SW(u8 num,u8 sta);
extern ControlMsg_t control_data;//��������������������ļ��еľ�̬ȫ�ֱ��������ǲ��Ե�
//��̬ȫ�ֱ������������ǵ�ǰ.c�ļ��������˵��
//��ʹ����.c�ļ���ͬ���ľ�̬ȫ�ֱ���������Ҳ�ǲ�ͬ�ı��������������ڴ��еĵ�ַ��ͬ
//֮ǰ�����Ĵ�����Control_task.c�н��䶨��Ϊ��ȫ�ֱ���
//����ǰ.c�ļ���Message_Task.c�ֽ��䶨��Ϊ�˾�̬ȫ�ֱ���
//��ˣ�����Ҫ�ڵ�ǰ.c�ļ���Message_Task.c�� extern �������Ƕ���Ϊ��̬ȫ�ֱ���

char* itoa(int value, char *string, int radix);
PI_UsartRec_t stm32_pi_Rec;
void Stm32_Pi_Init(void)
{
  HAL_UART_Receive_DMA(&PI_USART, stm32_pi_Rec.RX_pData, PI_RX_LEN);  //����Ϊʲô��ʼ����ʱ
	//��ҲҪִ��һ���������Ӧ���ǰ�USART2��û������ȶ����꣬���������˼
	//Ӧ��������DMA����˼
	__HAL_UART_DISABLE_IT(&PI_USART, UART_IT_IDLE); //#define PI_USART  huart2
	//ʹ��IDLE�жϣ�����ж��ǵ����յ�һ֡����(��һ����8���ֽ�һ֡������ֽ�1֡Ҳ�У�ÿ��֡�ĳ���Ҳ��һ��һ��)ʱ�����ж�
	//��Ƭ����ô֪��һ֡�����ж��٣� ��һ֡������һ�𷢵ģ�����һ֮֡���п���
	//ͨ���������жϵģ��������ж�IDLE�����ϵ���˼�ǿ����ж�
	stm32_pi_Rec.RX_flag=0;
}
 
void HAL_PI_To_UART_Receive_IDLE(UART_HandleTypeDef *huart)
{//�������ÿ�δ���IDLE�жϾͻᱻ���ã�Ҳ����˵��ÿ���յ�һ֡���ݾͻ�����������
  	uint32_t temp;//�����ǣ����յ�һ֡��Ȼ��DMA������ɺ�Ž����������
 
  if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))//��� IDLE�жϱ�־Ϊ1 
    {   
        __HAL_UART_CLEAR_IDLEFLAG(huart);  //����жϱ�־
        HAL_UART_DMAStop(huart);  
        temp = huart->hdmarx->Instance->CNDTR;  //CNDTR��DMAÿ��Ҫ����ĸ�����ÿ������һ��-1,����0�ͽ�������(����ģʽ)
        stm32_pi_Rec.RX_Size =  PI_RX_LEN - temp;   //������ѭ��ģʽ�£�����0֮���Զ��ظ�����ֵ
        stm32_pi_Rec.RX_flag=1;  									//���PI_RX_LEN - temp�ʹ����յ��˶��ٸ�
 			  Pi_Stm32_unpackData(stm32_pi_Rec.RX_pData,stm32_pi_Rec.RX_Size);
		   	//memset(stm32_pi_Rec.RX_pData,'\0',PI_RX_LEN);//���д����������ڣ�����
			// void *memset(void *s, char ch, unsigned n);//��������Ĺ����ǽ�*s��ָ����ڴ�鸳ֵΪch�����ascii��
        HAL_UART_Receive_DMA(huart,stm32_pi_Rec.RX_pData,sizeof(stm32_pi_Rec.RX_pData)); 
			//�������д��������ѭ��ģʽ���ܣ�����Stm32_Pi_Init�Ѿ����ù�һ���ˣ����ﲻ�õ���Ҳ�У�
			//HAL_UART_Receive_DMA(huart,stm32_pi_Rec.RX_pData,stm32_pi_Rec.RX_Size);
			//sizeof(stm32_pi_Rec.RX_pData)=PI_RX_LEN
			//HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
			//����˵USART2���յ������ݣ�Ҳ������ݮ�ɷ��������ݣ�����DMAת�浽stm32_pi_Rec.RX_pData��
			//ÿ��ת������ݳ��Ȳ�Ӧ����sizeof(stm32_pi_Rec.RX_pData)����Ϊsizeof(stm32_pi_Rec.RX_pData)==PI_RX_LEN
			//Ӧ����stm32_pi_Rec.RX_Size
			//char b[20]='abc',sizeof(b)==20,
    }
		/*	PI_UsartRec_t stm32_pi_Rec;
		#define PI_RX_LEN 256  
		typedef struct  
		{  
			uint8_t  RX_flag:1;           //IDLE receive flag
			uint16_t RX_Size;             //receive length
			uint8_t  RX_pData[PI_RX_LEN]; //DMA receive buffer
		}PI_UsartRec_t;
		*/
}

void PI_STM32_Test_Demo(void)//���������BSP_Init�б�����
{
	Stm32_Pi_Init();
	
	LED_SW(5,1);
	char string[16] = {0};
	//uint8_t RXpdata[13]="qwertyuiopvka";
	//HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
	while(1)
	{// ��Ӳ��Թ��ܣ���ݮ�ɸ�stm32�����ݣ�stm32�ӵ����ٷ�����ݮ��
		if(stm32_pi_Rec.RX_flag)
		{
			
			HAL_UART_Transmit(&PI_USART, stm32_pi_Rec.RX_pData,stm32_pi_Rec.RX_Size, 0x100);//
			HAL_UART_Transmit(&PI_USART, (uint8_t*)&(control_data.navi_angle),4, 0x100);//һ��float4�ֽ�
			if((uint8_t)navi_center_to_line_angle == 0x00)//������ǿ��ת��ֻ������λ
			{
				HAL_UART_Transmit(&PI_USART, (uint8_t*)"avg", 3, 0x100);
			}
			
			itoa(stm32_pi_Rec.RX_Size, string, 10);
			HAL_UART_Transmit(&PI_USART, string, 2, 0x100);//���������������Ҫ�ַ����м����ֽھ��Ǽ�
			//HAL_UART_Transmit(&PI_USART, (uint8_t*)stm32_pi_Rec.RX_Size, 2, 0x100);
			stm32_pi_Rec.RX_flag=0;
		}
		HAL_UART_Transmit(&PI_USART, (uint8_t*)"out", 3, 0x100);//#define PI_USART              huart2
		//HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
		//����ֵ����������״̬�����ͳɹ� ʱ����HAL_OK
		//����1��ѡ��USART������2������ָ��(�����׵�ַ)������3�����ݳ���
		//����4�����ʱ�䣬��������ʱ��û����Ͳ����ˣ�ʱ�����ʱ�򷵻�HAL_TIMEOUT
		delay_ms(2000);
		
		// HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
		//����һ�е��������Ӧ���ǲ�ѯ��ʽ���ܣ�һ�㲻��
	}
}
void PI_stm32_Lora_test()
{
	//Lora(SX1278) ��ʼ��
	SX1278_Init();
	HAL_Delay(500);
	HAL_GPIO_WritePin(LORA_M0_GPIO_Port,LORA_M0_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LORA_M1_GPIO_Port,LORA_M1_Pin,GPIO_PIN_RESET);//һ��ģʽ
	HAL_Delay(500);
	
	//stm32_PI��ʼ��
	Stm32_Pi_Init();
	
	LED_SW(6,1);//��ʼ����ɺ�����
	
	char string[20];
	uint8_t len_str;
	while(1)
	{// ��Ӳ��Թ��ܣ���ݮ�ɸ�stm32�����ݣ�stm32�ӵ�����lora��������
		if(stm32_pi_Rec.RX_flag)
		{
			
			//HAL_UART_Transmit(&PI_USART, stm32_pi_Rec.RX_pData,stm32_pi_Rec.RX_Size, 0x100);//
			//HAL_UART_Transmit(&PI_USART, (uint8_t*)"inf", 3, 0x100);
			//�����������ֽڷ�����ݮ�ɺ�lora���Զ�
			//HAL_UART_Transmit(&PI_USART, (uint8_t*)&(navi_center_to_line_angle),4, 0x100);//һ��float4�ֽ�
			//HAL_UART_Transmit(&PI_USART, (uint8_t*)&(control_data.navi_distance),4, 0x100);
			//HAL_UART_Transmit(&SX1278_USART, (uint8_t*)&(navi_center_to_line_angle),4, 0x100);
			
			//��control_data.navi_angleתΪ�ַ�����Ȼ���ô��ڷֱ𷢸���ݮ�ɺ�lora���Զ�
			len_str=sprintf(string,"dis=%3.3f\r\n",control_data.navi_distance);
			HAL_UART_Transmit(&PI_USART, (uint8_t*)string, len_str, 0x100);//���������������Ҫ�ַ����м����ֽھ��Ǽ�
			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
			
			len_str=sprintf(string,"ang=%3.3f\r\n",control_data.navi_angle);
			HAL_UART_Transmit(&PI_USART, (uint8_t*)string, len_str, 0x100);//���������������Ҫ�ַ����м����ֽھ��Ǽ�
			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
			
			stm32_pi_Rec.RX_flag=0;
		}
		HAL_UART_Transmit(&PI_USART, (uint8_t*)"out", 3, 0x100);//#define PI_USART              huart2	
		HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"out", 3, 0x100);//#define PI_USART              huart2	
		delay_ms(500);
	}
}

//stm32 -> Pi      �����ϴ�����    		         
uint8_t send_buf[30]={0};
void SendToPi(uint8_t id,uint8_t* data,uint8_t len)//����UART4
{
    static uint8_t count = 0;
	  send_buf[0] = 0xfe;
    send_buf[1] = 7+len;
    send_buf[2] = count;
    send_buf[3] = id;
    for(uint8_t i=0;i<len;i++)
    {
        send_buf[4+i] = *(data+i);
    }
    send_buf[len+6] = 0x0a;
//		HAL_UART_Transmit(&PI_USART, (uint8_t*)"123", 3, 0x100);
		//Send_string_xlen(USART2,send_buf, send_buf[1]);
}
/******************************************************
		��������ת�ַ�������
        char *itoa(int value, char *string, int radix)
		radix=10 ��ʾ��10����	��ʮ���ƣ�ת�����Ϊ0;  

	    ����d=-379;
		ִ��	itoa(d, buf, 10); ��
		
		buf="-379"							   			  
**********************************************************/
char* itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

}







