#ifndef __DJ_DRV_H__
#define __DJ_DRV_H__

#define XIFEN 16    //设置细分

#define CW_1 PBout(13)// 电机1方向控制引脚    PCout(14)  
#define CW_2 PBout(14)// 电机2方向控制引脚    PCout(15)
#define CW_3 PBout(15) // 电机3方向控制引脚   PAout(0)     
#include "sys.h"
extern  char MOTOR_busy;    //若MOTOR_busy=1,则说明电机正在转动

extern  void MOTOR_Init(char num);//电机初始化，设置同时转动的电机个数

//*********方向1,角度1,脉冲周期(单电机调用函数)*********//
void MOTOR_drv1(char fx,double jiaodu,char sudu);

//*********方向1,角度1,方向2,角度2,脉冲周期(双电机调用函数)*********//
void MOTOR_drv2(char fx_1,double jiaodu_1,char fx_2,double jiaodu_2,char sudu);	

//*********方向1,角度1,方向2,角度2,方向3,角度3,脉冲周期(三电机调用函数)*********//
void MOTOR_drv3(char fx_1,u16 jiaodu_1,char fx_2,u16 jiaodu_2,char fx_3,u16 jiaodu_3,char sudu);

void GO_3CH(u8 fx_1,u16 jiaodu_1,u8 fx_2,u16 jiaodu_2,u8 fx_3,u16 jiaodu_3);
void GOTO_3CH(u16 jiaodu_1,u16 jiaodu_2,u16 jiaodu_3);
void GOGO_3CH(u16 jiaodu_1,u16 jiaodu_2,u16 jiaodu_3);
void STEP_3CH_B(u16 value1,u16 value2,u16 value3);

#endif

