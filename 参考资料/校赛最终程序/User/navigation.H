//导航文件

#define start_frame 0xff 

#define master_address 0x00 

#define servant_1_address 0x01  
#define servant_2_address 0x02
#define servant_3_address 0x03

#define local_address servant_1_address  



#define USBL_EXIST //定义usbl存在


#define EMERGENCY_STOP	  			  0x00   

#define DELAY_REPORT	     				0X01    

#define DELAY_OPERAN              0X19   


#define MOTOR_SLEEP       				0x02    
#define MOTOR_WAKE        				0x03    
#define MOTOR_SET         				0X04  
#define MOTOR_SET_READ     				0X05    

#define MOTOR_STATE_REPORT				0X06		
#define MOTOR_REPORT      				0x07		




#define PID_EN_SET       				 	0x08   
#define PID_SET          					0X09    

#define PID_STATE_REPORT 				 	0X0A		
#define PID_REPORT       				 	0X0B    




#define MOTIVE_SET        				0X0C    
#define MOTIVE_SET_READ						0X0D		
#define MOTIVE_TRUE_READ					0X0E		
#define MOTIVE_SET_CON						0X0F	  //我觉得玄乎//我没时间写
#define MOTIVE_SET_CON_R					0X10		
#define MOTVIE_SET_CON_T					0X11		

#define MOTIVE_SET_REPORT  				0X12    
#define MOTIVE_TRUE_REPORT 				0X13    
#define MOTIVE_SET_CON_R_REPORT  	0X14   
#define MOTIVE_SET_CON_T_REPORT  	0X15    

#define FLOAT_CTRL_SET						0X16    
#define FLOAT_CTRL_SET_REPORT     0X17		
#define FLOAT_CTRL_SET_READ				0X18		

#define USBL_READ				          0X20		
#define USBL_PEPORT               0X21    


#define MOTIVE_RELATIVE_SET       0X22   
#define MOTIVE_RELATIVE_SET_REPORT 0X23    

#define MOTIVE_TEST 0X24 
#define MOTIVE_TEST_REPORT 0X25 
#define SINGLE_MOTOR_SET   0X26 
#define SINGLE_MOTOR_SET_READ 0X27 


#define MATCH_TRAGET_DEPTH_SET    0x28
#define MATCH_TRAGET_DEPTH_REPORT 0x29

#define MATCH_TRAGET_SPEED_SET    0x2a     
#define MATCH_TRAGET_SPEED_REPORT 0X2b  

#define MATCH_TRAGET_PITCH_SET 0X2c 
#define MATCH_TRAGET_PITCH_REPORT 0X2d 

#define MATCH_DEPTH_PID_SET    0x2e
#define MATCH_DEPTH_PID_REPORT 0x2f

#define MATCH_PITCH_PID_SET    0x30
#define MATCH_PITCH_PID_REPORT 0x31

#define MATCH_TRAGET_YAW_SET    0x32
#define MATCH_TRAGET_YAW_REPORT 0x33

#define MATCH_YAW_PID_SET    0x34
#define MATCH_YAW_PID_REPORT 0x35

/************************************************************************************/
//系统级函数，请勿随意调用
void CTRL_CRC16Table( unsigned char *p, int usLen);//CRC校检函数


int CTRL_message_send(void); //信息发送函数
/************************************************************************************/
//用户级函数
void CTRL_command_send(unsigned char command_num); // 命令发送函数

/*   调用形式
		
*/
unsigned char CTRL_command_handle(void);//命令解析程序