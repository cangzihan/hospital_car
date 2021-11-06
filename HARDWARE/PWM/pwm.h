#ifndef _pwm_H
#define _pwm_H
#include "stm32f10x.h"

void pwm_init(void);
void pwm_init2(void);
void TIM3_PWM(u8 channel,u16 value);
void TIM3_3CH(u16 value1,u16 value2,u16 value3);
void STEP_3CH(u16 value1,u16 value2,u16 value3);
void TIM8_PWM(u8 channel,u16 value);



#endif
