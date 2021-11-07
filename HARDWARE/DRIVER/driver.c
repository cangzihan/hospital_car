#include "driver.h"
#include "delay.h"
#include "stm32f10x_tim.h"

#define Speed_UP 3000
#define Speed_LEFT_UP 3200
#define Speed_LEFT_DOWN 1900
#define Speed_ADD 100
#define Speed_DEC 250

void driver_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	    	
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_ResetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9); 						 //输出高 
}

void car_run(u8 run_mode)
{
	switch(run_mode)
	{
			case 0X01:
						LEFT_UP=1;
						LEFT_DOWN=0;
						RIGHT_UP=1;
						RIGHT_DOWN=0;
						TIM_SetCompare1(TIM3,Speed_UP); 
						TIM_SetCompare4(TIM3,Speed_UP+Speed_ADD); 
						break;
			case 0x02:
						LEFT_UP=1;
						LEFT_DOWN=0;
						RIGHT_UP=1;
						RIGHT_DOWN=0;
						TIM_SetCompare1(TIM3,Speed_LEFT_UP); 
						TIM_SetCompare4(TIM3,Speed_LEFT_DOWN); 
						break;
			case 0X04:
						LEFT_UP=1;
						LEFT_DOWN=0;
						RIGHT_UP=1;
						RIGHT_DOWN=0;
						TIM_SetCompare1(TIM3,Speed_LEFT_DOWN); 
						TIM_SetCompare4(TIM3,Speed_LEFT_UP); 
						break;
			// Back
			case 0x05:
						LEFT_UP=0;
						LEFT_DOWN=1;
						RIGHT_UP=0;
						RIGHT_DOWN=1;
						TIM_SetCompare1(TIM3,Speed_UP); 
						TIM_SetCompare4(TIM3,Speed_UP+Speed_ADD); 
						break;
			case 0x03:
						LEFT_UP=1;
						LEFT_DOWN=1;
						RIGHT_UP=1;
						RIGHT_DOWN=1;
						TIM_SetCompare1(TIM3,Speed_UP); 
						TIM_SetCompare4(TIM3,Speed_UP+Speed_ADD); 
						break;
			// 左转大转 0x06
			case 0x06:
						LEFT_UP=1;
						LEFT_DOWN=0;
						RIGHT_UP=0;
						RIGHT_DOWN=1;
						TIM_SetCompare1(TIM3,Speed_LEFT_UP-Speed_DEC); 
						TIM_SetCompare4(TIM3,Speed_LEFT_DOWN); 
						break;
			// 右转大转 0x07
			case 0x07:
						LEFT_UP=0;
						LEFT_DOWN=1;
						RIGHT_UP=1;
						RIGHT_DOWN=0;
						TIM_SetCompare1(TIM3,Speed_LEFT_DOWN); 
						TIM_SetCompare4(TIM3,Speed_LEFT_UP-Speed_DEC); 
						break;
			// 加速前进0x08
			case 0X08: 
						LEFT_UP=1;
						LEFT_DOWN=0;
						RIGHT_UP=1;
						RIGHT_DOWN=0;
						TIM_SetCompare1(TIM3,Speed_UP+550); 
						TIM_SetCompare4(TIM3,Speed_UP+600); 
						break;
			case 0x12:
						LEFT_UP=0;
						LEFT_DOWN=1;
						RIGHT_UP=0;
						RIGHT_DOWN=1;
						TIM_SetCompare1(TIM3,Speed_LEFT_DOWN); 
						TIM_SetCompare4(TIM3,Speed_LEFT_UP); 
						break;
			case 0X14:
						LEFT_UP=0;
						LEFT_DOWN=1;
						RIGHT_UP=0;
						RIGHT_DOWN=1;
						TIM_SetCompare1(TIM3,Speed_LEFT_UP); 
						TIM_SetCompare4(TIM3,Speed_LEFT_DOWN); 
						break;
	}
}


// 调试用
void driver_test(void)
{
	car_run(0x01);
	delay_ms(1000);
	delay_ms(1000);
	car_run(0x02);
	delay_ms(1000);
	delay_ms(1000);
	car_run(0x04);
	delay_ms(1000);
	delay_ms(1000);
	car_run(0x05);
	delay_ms(1000);
	delay_ms(1000);
	car_run(0x03);
	delay_ms(1000);
	delay_ms(1000);
}

