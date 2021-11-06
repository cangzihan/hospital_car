#include "pwm.h"
#include "delay.h"
#include "stm32f10x_tim.h"

u16 pwm1_mode;
u16 pwm2_mode;
u16 pwm3_mode;
u16 pwm4_mode;
/*******************************************************************************
* 函 数 名         : pwm_init
* 函数功能		   : IO端口及TIM3初始化函数	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void pwm_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;   //声明一个结构体变量，用来初始化GPIO

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//声明一个结构体变量，用来初始化定时器

	TIM_OCInitTypeDef TIM_OCInitStructure;//根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	/*  配置GPIO的模式和IO口 */
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_Init(GPIOB,&GPIO_InitStructure);


	//TIM3定时器初始化
	TIM_TimeBaseInitStructure.TIM_Period = 7199;	   //PWM 频率=72000000/(7200*200)=    Hz//设置自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 199;//设置用来作为TIMx时钟频率预分频值，100Khz计数频率
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIM3, & TIM_TimeBaseInitStructure);

//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);//改变指定管脚的映射

	//PWM初始化	  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM输出使能
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;

	TIM_OC1Init(TIM3,&TIM_OCInitStructure);                          //A6
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR1上的预装载寄存器
	
//	TIM_OC2Init(TIM3,&TIM_OCInitStructure);                          //A7
//	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR2上的预装载寄存器
//	
//	TIM_OC3Init(TIM3,&TIM_OCInitStructure);                          //B0
//	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR3上的预装载寄存器
	
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);                          //B1
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR3上的预装载寄存器
	
	TIM_Cmd(TIM3,ENABLE);//使能或者失能TIMx外设
}

void pwm_init2()
{
	GPIO_InitTypeDef GPIO_InitStructure;   //声明一个结构体变量，用来初始化GPIO

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//声明一个结构体变量，用来初始化定时器

	TIM_OCInitTypeDef TIM_OCInitStructure;//根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);


	//TIM3定时器初始化
	TIM_TimeBaseInitStructure.TIM_Period = 7199;	   //PWM 频率=72000000/(7200*200)=    Hz//设置自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 199;//设置用来作为TIMx时钟频率预分频值，100Khz计数频率
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIM4, & TIM_TimeBaseInitStructure);

	//PWM初始化	  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM输出使能
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;

	TIM_OC1Init(TIM4,&TIM_OCInitStructure);                          //B6
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR1上的预装载寄存器
	
	TIM_OC2Init(TIM4,&TIM_OCInitStructure);                          //B7
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR2上的预装载寄存器
	
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);                          //B8
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR3上的预装载寄存器
	
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);                          //B9
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR3上的预装载寄存器
	
	TIM_Cmd(TIM4,ENABLE);//使能或者失能TIMx外设
}


void TIM3_PWM(u8 channel,u16 value)
{
	u8 c=channel;
	u16 v=value;
	switch(c)
	{
		case 1:
			TIM_SetCompare1(TIM3,v);
//			LCD_ShowNum(50,56,v,4,12);	
		  pwm1_mode=v;
		break;
		case 2:
			TIM_SetCompare2(TIM3,v);
//			LCD_ShowNum(50,69,v,4,12);	
		  pwm2_mode=v;
		break;
		case 3:
			TIM_SetCompare3(TIM3,v);
//			LCD_ShowNum(50,82,v,4,12);	
		  pwm3_mode=v;
		break;
		case 4:
			TIM_SetCompare4(TIM3,v);
//			LCD_ShowNum(50,82,v,4,12);	
		  pwm4_mode=v;
		break;
	}
}


void TIM3_3CH(u16 value1,u16 value2,u16 value3)
{
			TIM_SetCompare1(TIM3,value1);
//			LCD_ShowNum(50,56,value1,4,12);	
			TIM_SetCompare2(TIM3,value2);
//			LCD_ShowNum(50,69,value2,4,12);	
			TIM_SetCompare3(TIM3,value3);
//			LCD_ShowNum(50,82,value3,4,12);	
		  pwm1_mode=value1;
		  pwm2_mode=value2;
		  pwm3_mode=value3;
}

void STEP_3CH(u16 value1,u16 value2,u16 value3)
{
	while(value1!=pwm1_mode||value2!=pwm2_mode||value3!=pwm3_mode)
	{
		 if(value1!=pwm1_mode)
		 {
			 if(value1>pwm1_mode)
			 {
				 if(value1-pwm1_mode>25)
				 {
				   pwm1_mode+=25;
					 TIM_SetCompare1(TIM3,pwm1_mode);
				 }
				 else
				 {
					 pwm1_mode=value1;
					 TIM_SetCompare1(TIM3,pwm1_mode);
				 }
			 }
			 else  //pwm1_mode>value1
			 {
				 if(pwm1_mode-value1>25)
				 {
				   pwm1_mode-=25;
					 TIM_SetCompare1(TIM3,pwm1_mode);
				 }
				 else
				 {
					 pwm1_mode=value1;
					 TIM_SetCompare1(TIM3,pwm1_mode);
				 }
			 }
		 }
		 if(value2!=pwm2_mode)   //2
		 {
			 if(value2>pwm2_mode)
			 {
				 if(value2-pwm2_mode>25)
				 {
				   pwm2_mode+=25;
					 TIM_SetCompare2(TIM3,pwm2_mode);
				 }
				 else
				 {
					 pwm2_mode=value2;
					 TIM_SetCompare2(TIM3,pwm2_mode);
				 }
			 }
			 else  //pwm2_mode>value2
			 {
				 if(pwm2_mode-value2>25)
				 {
				   pwm2_mode-=25;
					 TIM_SetCompare2(TIM3,pwm2_mode);
				 }
				 else
				 {
					 pwm2_mode=value2;
					 TIM_SetCompare2(TIM3,pwm2_mode);
				 }
			 }
			 
		 }
		 if(value3!=pwm3_mode)    //3
		 {
			 if(value3>pwm3_mode)
			 {
				 if(value3-pwm3_mode>25)
				 {
				   pwm3_mode+=25;
					 TIM_SetCompare3(TIM3,pwm3_mode);
				 }
				 else
				 {
					 pwm3_mode=value3;
					 TIM_SetCompare3(TIM3,pwm3_mode);
				 }
			 }
			 else  //pwm3_mode>value3
			 {
				 if(pwm3_mode-value3>25)
				 {
				   pwm3_mode-=25;
					 TIM_SetCompare3(TIM3,pwm3_mode);
				 }
				 else
				 {
					 pwm3_mode=value3;
					 TIM_SetCompare3(TIM3,pwm3_mode);
				 }
			 }
		 }
		 delay_ms(100);
	}	 
}



