#ifndef __DJ_DRV_H__
#define __DJ_DRV_H__

#define XIFEN 16    //����ϸ��

#define CW_1 PBout(13)// ���1�����������    PCout(14)  
#define CW_2 PBout(14)// ���2�����������    PCout(15)
#define CW_3 PBout(15) // ���3�����������   PAout(0)     
#include "sys.h"
extern  char MOTOR_busy;    //��MOTOR_busy=1,��˵���������ת��

extern  void MOTOR_Init(char num);//�����ʼ��������ͬʱת���ĵ������

//*********����1,�Ƕ�1,��������(��������ú���)*********//
void MOTOR_drv1(char fx,double jiaodu,char sudu);

//*********����1,�Ƕ�1,����2,�Ƕ�2,��������(˫������ú���)*********//
void MOTOR_drv2(char fx_1,double jiaodu_1,char fx_2,double jiaodu_2,char sudu);	

//*********����1,�Ƕ�1,����2,�Ƕ�2,����3,�Ƕ�3,��������(��������ú���)*********//
void MOTOR_drv3(char fx_1,u16 jiaodu_1,char fx_2,u16 jiaodu_2,char fx_3,u16 jiaodu_3,char sudu);

void GO_3CH(u8 fx_1,u16 jiaodu_1,u8 fx_2,u16 jiaodu_2,u8 fx_3,u16 jiaodu_3);
void GOTO_3CH(u16 jiaodu_1,u16 jiaodu_2,u16 jiaodu_3);
void GOGO_3CH(u16 jiaodu_1,u16 jiaodu_2,u16 jiaodu_3);
void STEP_3CH_B(u16 value1,u16 value2,u16 value3);

#endif

