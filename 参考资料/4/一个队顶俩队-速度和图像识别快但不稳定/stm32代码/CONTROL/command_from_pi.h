#include "sys.h"
#include "stdbool.h"

#define CONSTRAIN(x,max,min) (x>max?max:(x<min?min:x))
#define MAX(a,b)             (a>b?a:b)
#define MIN(a,b)             (a<b?a:b)


#define CMD_NAV_ID					0x00
#define CMD_ALPHABET_ID			0x01
#define CMD_END_PI_THREAD_ID			0x02
#define CMD_LOCALIZATION_ID			0x03


#define MSG_NAVI_ID         		0X23
#define MSG_REGISTER_ID         0X24
#define MSG_ALPHABET_ID         0X25
#define MSG_LOCALIZATION_ID			0X26

extern void Pi_Stm32_unpackData(uint8_t *buf,u16 len);
extern const float pi;
extern long quan_navi_angle;
extern int navi_angle;
extern int navi_distance;
extern int navi_angle_compensation;
extern int brake_button;
extern int x_error,y_error;
extern int islocalization_enabled;
extern int letter_cnt;
extern int pre_y_error;
void TIM4_LocSettlement_Init(u16 arr,u16 psc);
