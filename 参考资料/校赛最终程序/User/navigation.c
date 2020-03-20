#include <navigation.h>
#include "relay.h"
#include "stm32f10x_usart.h" //stm32串口函数库


#include "bsp_usart1.h"
#include <stdarg.h>
#include "float_ctrl.h"


//水声导航

unsigned char CTRL_CRC16_High,CTRL_CRC16_Low;

unsigned char ctrl_message_txd[13]={start_frame,0,0,0,0,0,0,0,0,0,0,0,0}; 
unsigned char ctrl_message_rxd[13]={0,0,0,0,0,0,0,0,0,0,0,0,0};

unsigned char traget_address;

unsigned char PID_en_flag=1;

extern unsigned char yaw_high,yaw_low,pitch_high,pitch_low,roll_high,roll_low;




extern float roll; 
extern float pitch; 
extern float yaw; 


float traget_yaw=0;  
float traget_pitch=0; 
float traget_roll=0;  

float traget_x_speed=0;
float traget_y_speed=0; 
float traget_z_speed=0; 

float traget_speed_sys_x=0;

float sys_yaw=0;
float sys_pitch=0;
float sys_roll=0;

float sys_x_speed=0;
float sys_y_speed=0;
float sys_z_speed=0;

float KP=14.1;
float KI=3.31; 
float KD=0.046875;

float traget_depth=0;
float sys_depth=0;
float depth=0; 

unsigned char motor_sleep_state[4]={0};

extern unsigned char relay_state[4];

extern unsigned char message_rxd[13];//usbl的东西，我也不想说啥，困

extern unsigned char distance_high,distance_low,bearing_high,bearing_low,depth_high,depth_low;


unsigned char float_state;

int16_t T_kp,T_ki,T_kd;
int16_t motor_1_speed_set,motor_2_speed_set,motor_3_speed_set,motor_4_speed_set;


unsigned char rec_motive_flag=1;
unsigned char rec_motive_mode=5;


extern int X_angle,Y_angle;//定义XY轴声线
extern float x_angle_f,y_angle_f;//定义xy轴声线
 
unsigned char u1_u3_t_flag=0;

float match_depth_set=0;//100; 
 
float depth_p_set=10;
float depth_d_set=0;

float match_pitch_set=0;

float pitch_p_set=30;
float pitch_d_set=0.046875;

float  match_yaw_set=0;
float yaw_p_set=0.4;
float yaw_d_set=0.046875;

float match_speed_set=200;

/********************************************/
//命令发送程序，需要调用本地串口发送代码
//该函数可拓展含参数命令，故予以保留
/********************************************/
void CTRL_command_send(unsigned char command_num) // 
{   
	 // unsigned int CRC_result=0;
	  unsigned char txd_admission=1;
	  int16_t temp=0;	
	  
	
		switch(command_num)  
			{                  //见名知其意，故不注释
			//case MOTIVE_REPORT://
			//		ctrl_message_txd[2]=MOTIVE_REPORT;
			    
			//	break;
	    case DELAY_REPORT:
					ctrl_message_txd[2]=DELAY_REPORT;
			    ctrl_message_txd[3]=relay_state[0];
					ctrl_message_txd[4]=relay_state[1];
					ctrl_message_txd[5]=relay_state[2];
					ctrl_message_txd[6]=relay_state[3];
			    ctrl_message_txd[7]=0;
			    ctrl_message_txd[8]=0;
				break;
			case PID_REPORT:
					ctrl_message_txd[2]=PID_REPORT;
					
					 temp=KP*256;
				
			    ctrl_message_txd[4]=0;
					ctrl_message_txd[4]|=temp;
					ctrl_message_txd[3]=temp>>8;
			    
			    
			    
			    temp=KI*256;
					
					ctrl_message_txd[6]=0;
					ctrl_message_txd[6]|=temp;
					ctrl_message_txd[5]=temp>>8;
					
			    temp=KD*256;
					
					ctrl_message_txd[8]=0;
					ctrl_message_txd[8]|=temp;
					ctrl_message_txd[7]=temp>>8;
			  break;
			case MOTIVE_SET_REPORT:
				ctrl_message_txd[2]=MOTIVE_SET_REPORT;
			  temp=traget_yaw/180*32768;
			  ctrl_message_txd[4]=0;
				ctrl_message_txd[4]|=temp;
				ctrl_message_txd[3]=temp>>8;
			   
			  temp=traget_speed_sys_x;
			  ctrl_message_txd[6]=0;
				ctrl_message_txd[6]|=temp;
				ctrl_message_txd[5]=temp>>8;
				ctrl_message_txd[7]=0;
			  ctrl_message_txd[8]=0;
				break;
			case PID_STATE_REPORT:
				ctrl_message_txd[2]=PID_STATE_REPORT;
			  ctrl_message_txd[3]=PID_en_flag;
				ctrl_message_txd[4]=0;
			  ctrl_message_txd[5]=0;
				ctrl_message_txd[6]=0;
			  ctrl_message_txd[7]=0;
				ctrl_message_txd[8]=0;
			  
				break;
			case MOTIVE_TRUE_REPORT:
				ctrl_message_txd[2]=MOTIVE_TRUE_REPORT;
			  ctrl_message_txd[3]= yaw_high;
				ctrl_message_txd[4]= yaw_low;
				ctrl_message_txd[5]= pitch_high;
			  ctrl_message_txd[6]= pitch_low;
				ctrl_message_txd[7]= roll_high;
			  ctrl_message_txd[8]= roll_low;
				break;
			case MOTOR_REPORT:
				ctrl_message_txd[2]=MOTOR_REPORT;
				temp=motor_1_speed_set;
			  ctrl_message_txd[4]=0;
				ctrl_message_txd[4]|=temp;
				ctrl_message_txd[3]=temp>>8;
				temp=motor_2_speed_set;
			  ctrl_message_txd[6]=0;
				ctrl_message_txd[6]|=temp;
				ctrl_message_txd[5]=temp>>8;
				temp=motor_3_speed_set;
			  ctrl_message_txd[8]=0;
				ctrl_message_txd[8]|=temp;
				ctrl_message_txd[7]=temp>>8;
			  temp=motor_4_speed_set;
			  ctrl_message_txd[10]=0;
				ctrl_message_txd[10]|=temp;
				ctrl_message_txd[9]=temp>>8;
				break;
			
			case FLOAT_CTRL_SET_REPORT:
				
				ctrl_message_txd[2]=FLOAT_CTRL_SET_REPORT;
				ctrl_message_txd[3]=float_state;
				break;
			case USBL_PEPORT   	:  // 读取USBL的值
			 	ctrl_message_txd[2]=USBL_PEPORT;
			 // ctrl_message_txd[3]=distance_high;
			//	ctrl_message_txd[4]=distance_low;
			//	ctrl_message_txd[5]=bearing_high;
			//	ctrl_message_txd[6]=bearing_low;
			//	ctrl_message_txd[7]=depth_high;
				//ctrl_message_txd[8]=depth_low;
			
				temp=X_angle;
				ctrl_message_txd[6]=0;
				ctrl_message_txd[6]|=temp;
				ctrl_message_txd[5]=temp>>8;
			
				temp=Y_angle;
				ctrl_message_txd[10]=0;
				ctrl_message_txd[10]|=temp;
				ctrl_message_txd[9]=temp>>8;
			
			
					break;
			
			case MOTIVE_RELATIVE_SET_REPORT   	:  
				
			  ctrl_message_txd[2]=MOTIVE_RELATIVE_SET_REPORT;
			  temp=traget_yaw/180*32768;
			  ctrl_message_txd[4]=0;
				ctrl_message_txd[4]|=temp;
				ctrl_message_txd[3]=temp>>8;
			   
			  temp=traget_speed_sys_x;
			  ctrl_message_txd[6]=0;
				ctrl_message_txd[6]|=temp;
				ctrl_message_txd[5]=temp>>8;
				ctrl_message_txd[7]=0;
			  ctrl_message_txd[8]=0;
			
					break;
			case MOTIVE_TEST_REPORT:
				
			  ctrl_message_txd[2]=MOTIVE_TEST_REPORT;
				ctrl_message_txd[3]=rec_motive_flag;
			  ctrl_message_txd[4]=rec_motive_mode;
				ctrl_message_txd[5]=0;
				ctrl_message_txd[6]=0;
				ctrl_message_txd[7]=0;
				ctrl_message_txd[8]=0;
			
			
			
			 break;
			case SINGLE_MOTOR_SET_READ:
				
			
				break;
			
			case MATCH_TRAGET_DEPTH_REPORT:
				ctrl_message_txd[2]=MATCH_TRAGET_DEPTH_REPORT;
				ctrl_message_txd[3]=match_depth_set/256;
			  ctrl_message_txd[4]=(int)match_depth_set%256;
				ctrl_message_txd[5]=0;
				ctrl_message_txd[6]=0;
				ctrl_message_txd[7]=0;
				ctrl_message_txd[8]=0;
				break;	
			case MATCH_TRAGET_SPEED_REPORT:
				ctrl_message_txd[2]=MATCH_TRAGET_SPEED_REPORT;
				ctrl_message_txd[3]=match_speed_set/256;
			  ctrl_message_txd[4]=(int)match_speed_set%256;
				ctrl_message_txd[5]=0;
				ctrl_message_txd[6]=0;
				ctrl_message_txd[7]=0;
				ctrl_message_txd[8]=0;
				break;	
			case MATCH_TRAGET_PITCH_REPORT:
				ctrl_message_txd[2]=MATCH_TRAGET_PITCH_REPORT;
				ctrl_message_txd[3]=match_pitch_set/256;
			  ctrl_message_txd[4]=(int)match_pitch_set%256;
				ctrl_message_txd[5]=0;
				ctrl_message_txd[6]=0;
				ctrl_message_txd[7]=0;
				ctrl_message_txd[8]=0;			  			
				break;	
			case MATCH_DEPTH_PID_REPORT:
				ctrl_message_txd[2]=MATCH_DEPTH_PID_REPORT;
				ctrl_message_txd[3]=depth_p_set;
			  ctrl_message_txd[4]=(int)(depth_p_set*256)%256;
				ctrl_message_txd[5]=depth_d_set;
				ctrl_message_txd[6]=(int)(depth_d_set*256)%256;;
				ctrl_message_txd[7]=0;
				ctrl_message_txd[8]=0;
				break;	
			case MATCH_PITCH_PID_REPORT:
				ctrl_message_txd[2]=MATCH_PITCH_PID_REPORT;
				ctrl_message_txd[3]=pitch_p_set;
			  ctrl_message_txd[4]=(int)(pitch_p_set*256)%256;
				ctrl_message_txd[5]=pitch_d_set;
				ctrl_message_txd[6]=(int)(pitch_d_set*256)%256;;
				ctrl_message_txd[7]=0;
				ctrl_message_txd[8]=0;			
				break;	
			case MATCH_TRAGET_YAW_REPORT:
				ctrl_message_txd[2]=MATCH_TRAGET_YAW_REPORT;
				ctrl_message_txd[3]=match_yaw_set/256;
			  ctrl_message_txd[4]=(int)match_yaw_set%256;
				ctrl_message_txd[5]=0;
				ctrl_message_txd[6]=0;
				ctrl_message_txd[7]=0;
				ctrl_message_txd[8]=0;	
				break;	
			case MATCH_YAW_PID_REPORT:
				ctrl_message_txd[2]=MATCH_YAW_PID_REPORT;
				ctrl_message_txd[3]=yaw_p_set;
			  ctrl_message_txd[4]=(int)(yaw_p_set*256)%256;
				ctrl_message_txd[5]=yaw_d_set;
				ctrl_message_txd[6]=(int)(yaw_d_set*256)%256;;
				ctrl_message_txd[7]=0;
				ctrl_message_txd[8]=0;			
				break;	
			
			
			default:
				//检测到未知命令，禁止发送  
					txd_admission=0;
					;
      }
		//统一填充CRC16校检
    CTRL_CRC16Table( ctrl_message_txd,11);   	//CRC校检                     

    ctrl_message_txd[11]=CTRL_CRC16_High; 
	  ctrl_message_txd[12]=CTRL_CRC16_Low;
		
		
		if(txd_admission)
			CTRL_message_send();
		
}

//需要自己补充底层代码

int CTRL_message_send(void) 
{
	unsigned char i;//发送计数
  for(i=0;i<13;i++)
		{
			
			USART_SendData(USART3, ctrl_message_txd[i]); //由串口发送出去
			/* 等待发送完毕 */
			while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	
			//此处需要换上符合底层的发送函数
		//	USART_SendData(USART1, message_txd[i]); //由串口发送出去
			/* 等待发送完毕 */
		//	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
		}

}


/********************************************/
//命令解析程序
//调用该函数前需要把接收缓冲区装填完毕
/********************************************/
unsigned char CTRL_command_handle(void)
{
	//unsigned int CRC_result=0;//定义CRC校检计算结果
	unsigned char CRC_high;
	unsigned char CRC_low;  
	int16_t temp=0;
  unsigned char i;
	unsigned char byte_high,byte_low;
	
	float yaw_temp;
	
	static int motive_command_num=0;
	
	//uint16_t temp_0;//临时变量，我也不知道该干嘛
	/*
	// 发送一个字节数据到USART1//		
	USART_SendData(USART3, local_id);			
	///等待发送完毕 /
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	
	
	//发送一个字节数据到USART1 //		
	USART_SendData(USART3, message_rxd[1]);			
	// 等待发送完毕 // 
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	 */
	
	if(ctrl_message_rxd[1]==local_address)//若地址匹配
		{	
		
			CTRL_CRC16Table( ctrl_message_rxd,11);     
      CRC_low=CTRL_CRC16_Low; 
			
			CRC_high=CTRL_CRC16_High;
			
		/*	for(i=0;i<13;i++)
						{
							//发送一个字节数据到USART1 
								USART_SendData(USART3, message_rxd[i]);
		
								// 等待发送完毕 
								while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	
						}*/
			/*
				//发送一个字节数据到USART1 //		
					USART_SendData(USART3, CRC_high);			
					// 等待发送完毕 //  
					while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	
					
					USART_SendData(USART3, CRC_low);			
					// 等待发送完毕//  
					while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	 */
			
			//进行CRC验证
			if((1))//ctrl_message_rxd[11]==CRC_high)&&(ctrl_message_rxd[12]==CRC_low))
			//	{ //当且仅当CRC通过后才会进行命令解算
					switch(ctrl_message_rxd[2])
						{
							//****************************************//
							case EMERGENCY_STOP :
								sw_ctrl(MOTOR_0_EN,sw_switch_off);
								sw_ctrl(MOTOR_1_EN,sw_switch_off);
								sw_ctrl(MOTOR_2_EN,sw_switch_off);
								sw_ctrl(MOTOR_3_EN,sw_switch_off);
								CTRL_command_send(DELAY_REPORT);
								break;
							
							case DELAY_OPERAN :
								sw_ctrl(MOTOR_0_EN,sw_switch_on);
								sw_ctrl(MOTOR_1_EN,sw_switch_on);
								sw_ctrl(MOTOR_2_EN,sw_switch_on);
								sw_ctrl(MOTOR_3_EN,sw_switch_on);
								CTRL_command_send(DELAY_REPORT); 
								break;
							
							
							
							case MOTOR_SLEEP :
							 			  
							 
								break;
							
							case MOTOR_WAKE:
							
								break;
							
							case MOTOR_SET :
							  temp=0;
							  temp|=((ctrl_message_rxd[3])<<8)	;
								temp|=ctrl_message_rxd[4]	;
							  motor_1_speed_set=temp;
								temp=0;
							  temp|=((ctrl_message_rxd[5])<<8)	;
								temp|=ctrl_message_rxd[6]	;
							  motor_2_speed_set=temp;
							  temp=0;
							  temp|=((ctrl_message_rxd[7])<<8)	;
								temp|=ctrl_message_rxd[8]	;
							  motor_3_speed_set=temp;
							  temp=0;
							  temp|=((ctrl_message_rxd[9])<<8)	;
								temp|=ctrl_message_rxd[10]	;
							  motor_4_speed_set=temp;
							
								CTRL_command_send(MOTOR_REPORT );
								break;
							case MOTOR_SET_READ :
							  						
								CTRL_command_send(MOTOR_REPORT );
								break;
							
							case PID_EN_SET :
							  if(ctrl_message_rxd[3])
									PID_en_flag=1;
								else
									PID_en_flag=0;
								CTRL_command_send(PID_STATE_REPORT);
								break;
							
							case PID_SET:
								
							  temp=0;
							  temp|=((ctrl_message_rxd[3])<<8)	;
								temp|=ctrl_message_rxd[4]	;
							  T_kp=temp;
							  KP=((float)temp)/256;
							
							  temp=0;
								temp|=ctrl_message_rxd[5]<<8	;
							  temp|=ctrl_message_rxd[6]	;		  
							  T_ki=temp;
						  	KI=((float)temp)/256;
						
							  temp=0;
							  temp|=ctrl_message_rxd[7]<<8	;
							  temp|=ctrl_message_rxd[8]	;
							  T_kd=temp;
								KD=((float)temp)/256;
							 							 
								CTRL_command_send(PID_REPORT);
								break;
								
								
							case MOTIVE_SET:
								
								temp=ctrl_message_rxd[3]<<8	;
								temp|=ctrl_message_rxd[4]	;
								traget_yaw=((float)temp)/32768*180; 
							  
							  temp=ctrl_message_rxd[5]<<8	;
								temp|=ctrl_message_rxd[6]	;
								traget_speed_sys_x=((float)temp); 
							
							  
								CTRL_command_send(MOTIVE_SET_REPORT);
							
								case MOTIVE_SET_READ	:	
								
								CTRL_command_send(MOTIVE_SET_REPORT);
								break;
							case MOTIVE_TRUE_READ:
								
								CTRL_command_send(MOTIVE_TRUE_REPORT );
							
								break;
							
							case FLOAT_CTRL_SET :
							 	float_state=ctrl_message_rxd[3];
								float_ctrl_pr(float_state);
							  CTRL_command_send(FLOAT_CTRL_SET_REPORT );
								break;
							case FLOAT_CTRL_SET_READ :
							 	//float_state=ctrl_message_rxd[3];
								//float_ctrl(float_state);
							   CTRL_command_send(FLOAT_CTRL_SET_REPORT );
								break;
							
							case USBL_READ	 ://读usbl
								
							   if(ctrl_message_rxd[3])
									 u1_u3_t_flag=1;
								 else
									 u1_u3_t_flag=0;
							 	//float_state=ctrl_message_rxd[3];
								//float_ctrl(float_state);
							   CTRL_command_send(USBL_PEPORT  );
								break;  
							
							case MOTIVE_RELATIVE_SET	 :
									//temp=ctrl_message_rxd[3]<<8	;
								//	temp|=ctrl_message_rxd[4]	;
									
									
						
							
									if(motive_command_num!=ctrl_message_rxd[7])
									
										if(ctrl_message_rxd[3]==1)
											{
												//temp=yaw_high<<8;					
									      //temp|=yaw_low;   
																		
									     // yaw_temp=sys_yaw;//((float)temp)/32768*180; 
											 // traget_yaw=yaw_temp+ctrl_message_rxd[4]; 
												
												traget_yaw=yaw+(float)(ctrl_message_rxd[4]); 
												
												
												
												
												temp=ctrl_message_rxd[5]<<8	;
												temp|=ctrl_message_rxd[6]	;
												traget_speed_sys_x=((float)temp); 
												
											}
										else if(ctrl_message_rxd[3]==2) 
											{
											
												traget_yaw=yaw-(float)(ctrl_message_rxd[4]);
												
											
												temp=ctrl_message_rxd[5]<<8	;
												temp|=ctrl_message_rxd[6]	;
												traget_speed_sys_x=((float)temp); 
											}
										else
											{
											
											  	traget_yaw=yaw; 
												
												temp=ctrl_message_rxd[5]<<8	;
									      temp|=ctrl_message_rxd[6]	;
												traget_speed_sys_x=((float)temp); 
											}
										
									motive_command_num=ctrl_message_rxd[7];
							  
									
									CTRL_command_send(MOTIVE_RELATIVE_SET_REPORT  );
								break;  
							case MOTIVE_TEST: 
								 if(ctrl_message_rxd[3])
									{
										rec_motive_flag=1;
										 rec_motive_mode=ctrl_message_rxd[4];
									}
								else
									rec_motive_flag=0;
								CTRL_command_send(MOTIVE_TEST_REPORT);
							
							
								break;
							case SINGLE_MOTOR_SET:
								switch(ctrl_message_rxd[3])
									{
											case 0:
												if(ctrl_message_rxd[4])
													motor_1_speed_set=ctrl_message_rxd[5]*256+ctrl_message_rxd[6];
												else
												  motor_1_speed_set=0-ctrl_message_rxd[5]*256-ctrl_message_rxd[6];
												break;
											case 1:
												if(ctrl_message_rxd[4])
													motor_2_speed_set=ctrl_message_rxd[5]*256+ctrl_message_rxd[6];
												else
												  motor_2_speed_set=0-ctrl_message_rxd[5]*256-ctrl_message_rxd[6];
												break;
											case 2:
												if(ctrl_message_rxd[4])//
													motor_3_speed_set=ctrl_message_rxd[5]*256+ctrl_message_rxd[6];
												else
												  motor_4_speed_set=0-ctrl_message_rxd[5]*256-ctrl_message_rxd[6];
												break;
											case 3:
												if(ctrl_message_rxd[4])
													motor_4_speed_set=ctrl_message_rxd[5]*256+ctrl_message_rxd[6];
												else
												  motor_4_speed_set=0-ctrl_message_rxd[5]*256-ctrl_message_rxd[6];
												break;
												
											default:
												;
											



									}
									
									CTRL_command_send(SINGLE_MOTOR_SET_READ);
									
								break;
							case MATCH_TRAGET_DEPTH_SET://深度配置
							
								match_depth_set=ctrl_message_rxd[3]*256+ctrl_message_rxd[4];
							
								CTRL_command_send(MATCH_TRAGET_DEPTH_REPORT);
								break;
							case MATCH_TRAGET_SPEED_SET :
								
						
								if(ctrl_message_rxd[3])
										match_speed_set=ctrl_message_rxd[4]*256+ctrl_message_rxd[5];
								else
										match_speed_set=0-ctrl_message_rxd[4]*256+ctrl_message_rxd[5];
							
								CTRL_command_send(MATCH_TRAGET_PITCH_REPORT);
								
							
								CTRL_command_send(MATCH_TRAGET_SPEED_REPORT);
								break;
							case MATCH_TRAGET_PITCH_SET:
								if(ctrl_message_rxd[3])
									match_pitch_set=ctrl_message_rxd[4]*256+ctrl_message_rxd[5];
								else
									match_pitch_set=0-ctrl_message_rxd[4]*256+ctrl_message_rxd[5];
							
								CTRL_command_send(MATCH_TRAGET_PITCH_REPORT);
								break;
							case MATCH_DEPTH_PID_SET:
								
								depth_p_set=ctrl_message_rxd[3]+(float)ctrl_message_rxd[4]/256;
							  depth_d_set=ctrl_message_rxd[5]+(float)ctrl_message_rxd[6]/256;
							
								CTRL_command_send(MATCH_DEPTH_PID_REPORT);
								break;
							case MATCH_PITCH_PID_SET:
								
							  pitch_p_set=ctrl_message_rxd[3]+(float)ctrl_message_rxd[4]/256;
							  pitch_d_set=ctrl_message_rxd[5]+(float)ctrl_message_rxd[6]/256;
								
								CTRL_command_send(MATCH_PITCH_PID_REPORT);
								break;
							case  MATCH_TRAGET_YAW_SET:
								
							
								if(ctrl_message_rxd[3])
									match_yaw_set=ctrl_message_rxd[4]*256+ctrl_message_rxd[5];
								else
									match_yaw_set=0-ctrl_message_rxd[4]*256+ctrl_message_rxd[5];
							
								CTRL_command_send(MATCH_TRAGET_YAW_REPORT);
								break;
							case MATCH_YAW_PID_SET:
								
							  yaw_p_set=ctrl_message_rxd[3]+(float)ctrl_message_rxd[4]/256;
							  yaw_d_set=ctrl_message_rxd[5]+(float)ctrl_message_rxd[6]/256;
								CTRL_command_send(MATCH_YAW_PID_REPORT);
							
								break;
							case MATCH_TRAGET_DEPTH_REPORT:
								CTRL_command_send(MATCH_TRAGET_DEPTH_REPORT);
								break;
							case MATCH_TRAGET_SPEED_REPORT:
								CTRL_command_send(MATCH_TRAGET_SPEED_REPORT);
								break;
							case MATCH_TRAGET_PITCH_REPORT:
								CTRL_command_send(MATCH_TRAGET_PITCH_REPORT);
								break;
							case MATCH_DEPTH_PID_REPORT:
								CTRL_command_send(MATCH_DEPTH_PID_REPORT);
								break;
							case MATCH_PITCH_PID_REPORT:
								CTRL_command_send(MATCH_PITCH_PID_REPORT);
								break;
							case MATCH_TRAGET_YAW_REPORT:
								CTRL_command_send(MATCH_TRAGET_YAW_REPORT);
								break;
							case MATCH_YAW_PID_REPORT:
								CTRL_command_send(MATCH_YAW_PID_REPORT);
								break;
							
							
							/*******************************************************/
							default:
								;
							
						}
			//	}
		//	else //CRC校检未通过
			//	USBL_command_send(warning);//发送错误信息
			
		//	return 1;
		}
	else//地址不匹配
		return 0;
	


}









/********************************************/
//16位CRC校检
//缓冲区数组地址，数组数量，校检码
/********************************************/

unsigned char CTRL_aucCRCHi[] = {      0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                                  0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                  0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40
                                  };

unsigned char CTRL_aucCRCLo[] =      { 0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
                                  0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
                                  0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
                                  0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
                                  0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
                                  0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
                                  0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
                                  0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
                                  0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
                                  0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
                                  0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
                                  0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
                                  0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
                                  0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
                                  0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
                                  0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
                                  0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
                                  0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
                                  0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
                                  0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
                                  0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
                                  0x41, 0x81, 0x80, 0x40
                                };





void CTRL_CRC16Table( unsigned char *p, int usLen)
{
    unsigned char ucCRCHi = 0xFF;
    unsigned char ucCRCLo = 0xFF;
    int iIndex;
    while( usLen-- )
    {
        iIndex  = ucCRCLo ^ *(p++);
        ucCRCLo = ( ucCRCHi ^ CTRL_aucCRCHi[iIndex] );
        ucCRCHi = CTRL_aucCRCLo[iIndex];
    }
    
		CTRL_CRC16_High=ucCRCHi; //CRC校检输出
		CTRL_CRC16_Low=ucCRCLo;//全局变量形式输出
    

    ucCRCHi = 0xFF;
    ucCRCLo = 0xFF;
}