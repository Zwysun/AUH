#include "stm32_pi.h"
extern float navi_center_to_line_angle;
extern void LED_SW(u8 num,u8 sta);
extern ControlMsg_t control_data;//本来将这个定义成了这个文件中的静态全局变量，这是不对的
//静态全局变量的作用域是当前.c文件，这就是说，
//即使两个.c文件有同名的静态全局变量，他们也是不同的变量，即他们在内存中的地址不同
//之前发生的错误是Control_task.c中将其定义为了全局变量
//而当前.c文件和Message_Task.c又将其定义为了静态全局变量
//因此，我们要在当前.c文件和Message_Task.c中 extern 它，而非定义为静态全局变量

char* itoa(int value, char *string, int radix);
PI_UsartRec_t stm32_pi_Rec;
void Stm32_Pi_Init(void)
{
  HAL_UART_Receive_DMA(&PI_USART, stm32_pi_Rec.RX_pData, PI_RX_LEN);  //？？为什么初始化的时
	//候也要执行一次这个？，应该是把USART2上没读完的先读读完，不是这个意思
	//应该是启动DMA的意思
	__HAL_UART_DISABLE_IT(&PI_USART, UART_IT_IDLE); //#define PI_USART  huart2
	//使能IDLE中断，这个中断是当接收到一帧数据(不一定是8个字节一帧，多个字节1帧也行，每两帧的长度也不一定一样)时进入中断
	//单片机怎么知道一帧数据有多少？ 答：一帧数据是一起发的，和下一帧之间有空闲
	//通过这样来判断的，因此这个中断IDLE字面上的意思是空闲中断
	stm32_pi_Rec.RX_flag=0;
}
 
void HAL_PI_To_UART_Receive_IDLE(UART_HandleTypeDef *huart)
{//这个函数每次触发IDLE中断就会被调用，也就是说，每接收到一帧数据就会进入这个函数
  	uint32_t temp;//并且是，接收到一帧，然后DMA传输完成后才进入这个函数
 
  if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))//如果 IDLE中断标志为1 
    {   
        __HAL_UART_CLEAR_IDLEFLAG(huart);  //清除中断标志
        HAL_UART_DMAStop(huart);  
        temp = huart->hdmarx->Instance->CNDTR;  //CNDTR是DMA每次要传输的个数，每传输完一次-1,减到0就结束传输(正常模式)
        stm32_pi_Rec.RX_Size =  PI_RX_LEN - temp;   //但是在循环模式下，减到0之后自动回复到初值
        stm32_pi_Rec.RX_flag=1;  									//因此PI_RX_LEN - temp就代表收到了多少个
 			  Pi_Stm32_unpackData(stm32_pi_Rec.RX_pData,stm32_pi_Rec.RX_Size);
		   	//memset(stm32_pi_Rec.RX_pData,'\0',PI_RX_LEN);//这行代码的意义何在？？？
			// void *memset(void *s, char ch, unsigned n);//这个函数的功能是将*s所指向的内存块赋值为ch代表的ascii码
        HAL_UART_Receive_DMA(huart,stm32_pi_Rec.RX_pData,sizeof(stm32_pi_Rec.RX_pData)); 
			//上面那行代码如果是循环模式接受，那在Stm32_Pi_Init已经调用过一次了，这里不用调用也行？
			//HAL_UART_Receive_DMA(huart,stm32_pi_Rec.RX_pData,stm32_pi_Rec.RX_Size);
			//sizeof(stm32_pi_Rec.RX_pData)=PI_RX_LEN
			//HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
			//就是说USART2接收到的数据，也就是树莓派发来的数据，会由DMA转存到stm32_pi_Rec.RX_pData中
			//每次转存的数据长度不应该是sizeof(stm32_pi_Rec.RX_pData)，因为sizeof(stm32_pi_Rec.RX_pData)==PI_RX_LEN
			//应该用stm32_pi_Rec.RX_Size
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

void PI_STM32_Test_Demo(void)//这个函数在BSP_Init中被调用
{
	Stm32_Pi_Init();
	
	LED_SW(5,1);
	char string[16] = {0};
	//uint8_t RXpdata[13]="qwertyuiopvka";
	//HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
	while(1)
	{// 添加测试功能：树莓派给stm32发数据，stm32接到后再发给树莓派
		if(stm32_pi_Rec.RX_flag)
		{
			
			HAL_UART_Transmit(&PI_USART, stm32_pi_Rec.RX_pData,stm32_pi_Rec.RX_Size, 0x100);//
			HAL_UART_Transmit(&PI_USART, (uint8_t*)&(control_data.navi_angle),4, 0x100);//一个float4字节
			if((uint8_t)navi_center_to_line_angle == 0x00)//这样的强制转换只保留低位
			{
				HAL_UART_Transmit(&PI_USART, (uint8_t*)"avg", 3, 0x100);
			}
			
			itoa(stm32_pi_Rec.RX_Size, string, 10);
			HAL_UART_Transmit(&PI_USART, string, 2, 0x100);//这里第三个参数是要字符串有几个字节就是几
			//HAL_UART_Transmit(&PI_USART, (uint8_t*)stm32_pi_Rec.RX_Size, 2, 0x100);
			stm32_pi_Rec.RX_flag=0;
		}
		HAL_UART_Transmit(&PI_USART, (uint8_t*)"out", 3, 0x100);//#define PI_USART              huart2
		//HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
		//返回值，表征发送状态，发送成功 时返回HAL_OK
		//参数1，选择USART，参数2：数据指针(数据首地址)，参数3：数据长度
		//参数4，溢出时间，超过多少时间没发完就不发了，时间溢出时候返回HAL_TIMEOUT
		delay_ms(2000);
		
		// HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
		//上面一行的这个函数应该是查询方式接受，一般不用
	}
}
void PI_stm32_Lora_test()
{
	//Lora(SX1278) 初始化
	SX1278_Init();
	HAL_Delay(500);
	HAL_GPIO_WritePin(LORA_M0_GPIO_Port,LORA_M0_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LORA_M1_GPIO_Port,LORA_M1_Pin,GPIO_PIN_RESET);//一般模式
	HAL_Delay(500);
	
	//stm32_PI初始化
	Stm32_Pi_Init();
	
	LED_SW(6,1);//初始化完成后亮灯
	
	char string[20];
	uint8_t len_str;
	while(1)
	{// 添加测试功能：树莓派给stm32发数据，stm32接到后用lora传给电脑
		if(stm32_pi_Rec.RX_flag)
		{
			
			//HAL_UART_Transmit(&PI_USART, stm32_pi_Rec.RX_pData,stm32_pi_Rec.RX_Size, 0x100);//
			//HAL_UART_Transmit(&PI_USART, (uint8_t*)"inf", 3, 0x100);
			//将浮点数按字节发给树莓派和lora电脑端
			//HAL_UART_Transmit(&PI_USART, (uint8_t*)&(navi_center_to_line_angle),4, 0x100);//一个float4字节
			//HAL_UART_Transmit(&PI_USART, (uint8_t*)&(control_data.navi_distance),4, 0x100);
			//HAL_UART_Transmit(&SX1278_USART, (uint8_t*)&(navi_center_to_line_angle),4, 0x100);
			
			//将control_data.navi_angle转为字符串，然后用串口分别发给树莓派和lora电脑端
			len_str=sprintf(string,"dis=%3.3f\r\n",control_data.navi_distance);
			HAL_UART_Transmit(&PI_USART, (uint8_t*)string, len_str, 0x100);//这里第三个参数是要字符串有几个字节就是几
			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
			
			len_str=sprintf(string,"ang=%3.3f\r\n",control_data.navi_angle);
			HAL_UART_Transmit(&PI_USART, (uint8_t*)string, len_str, 0x100);//这里第三个参数是要字符串有几个字节就是几
			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
			
			stm32_pi_Rec.RX_flag=0;
		}
		HAL_UART_Transmit(&PI_USART, (uint8_t*)"out", 3, 0x100);//#define PI_USART              huart2	
		HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"out", 3, 0x100);//#define PI_USART              huart2	
		delay_ms(500);
	}
}

//stm32 -> Pi      数据上传函数    		         
uint8_t send_buf[30]={0};
void SendToPi(uint8_t id,uint8_t* data,uint8_t len)//发往UART4
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
		整形数据转字符串函数
        char *itoa(int value, char *string, int radix)
		radix=10 标示是10进制	非十进制，转换结果为0;  

	    例：d=-379;
		执行	itoa(d, buf, 10); 后
		
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







