#include "stm32f10x.h"//B������Ӧ������ͷ�ļ�

void motor_init(void); // ����̵�����ʼ��
void usbl_init(void);//   �泬�̻��߼̵�����ʼ��
void sys_init(void);//    ϵͳ��ʼ��
void sw_ctrl(unsigned char sw_ch,unsigned char sw_stage);//����״̬

#define MOTOR_0_EN  0  //�궨����0����
#define MOTOR_1_EN  1
#define MOTOR_2_EN  2
#define MOTOR_3_EN  3
#define COIL_EN 		4

#define USBL_EN     4 //�泬�̻���ʹ��


#define sw_switch_off 0  //���عر�
#define sw_switch_on  1  //���ش�

