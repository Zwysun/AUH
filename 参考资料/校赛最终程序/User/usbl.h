#define start_frame 0xff //������ʼ֡

//#define servant 0 //����Ϊ�ӻ�
#define master 1 //����Ϊ����

#ifdef master  
   #define local_id 0x00 
	 #define opp_id 0x01
#else          
   #define local_id 0x01 
	 #define opp_id 0x00
#endif

#define test 0x00     

#define roll_read 0x01   
#define pitch_read 0x02  
#define yaw_read 0x03    

#define distance_read 0x04 
#define bearing_read 0x05  
#define depth_read 0x06  

#define roll_answer 0x07 
#define pitch_answer 0x08  
#define yaw_answer 0x09    

#define distance_answer 0x0A 
#define bearing_answer 0x0B  
#define depth_answer 0x0C   

#define RPY_read 0x0d 
#define DBD_read 0x0f 

#define RPY_answer 0xe 
#define DBD_answer 0x10



//�����泬�̻��ߵľ��뷽λ��ȶ�ȡ����

#define warning 0xfe 

//#define POLY        0x1021//CRCУ����






/************************************************************************************/
//ϵͳ�������������������
void CRC16Table( unsigned char *p, int usLen);//CRCУ�캯��
int message_send(void); //��Ϣ���ͺ���
/************************************************************************************/
//�û�������
void USBL_command_send(unsigned char command_num); // ����ͺ���
/*   ������ʽ
		USBL_command_send(roll_read);
		USBL_command_send(pitch_read);
		USBL_command_send(yaw_read);
		USBL_command_send(distance_read);
		USBL_command_send(bearing_read);
		USBL_command_send(depth_read);
		USBL_command_send(test);
    USBL_command_send(warning);
    USBL_command_send(RPY_read);
    USBL_command_send(DBD_read);
*/
unsigned char rxd_command_handle(void);//�����������