#include "exti.h"
#include "led.h"
#include "key.h"
#include "oled.h"
#include "delay.h"

//外部中断0服务程序
void EXTIX_Init(void)
{
   	EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟


   //GPIOC.13	  中断线以及中断初始化配置 下降沿触发 //KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   //GPIOC.14	  中断线以及中断初始化配置  下降沿触发	//KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource14);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line14;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   //GPIOC.15	  中断线以及中断初始化配置  下降沿触发	//KEY2
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource15);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


   //GPIOA.1	  中断线以及中断初始化配置 下降沿触发 PA1  WK_UP
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1); 

  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;	//抢占优先级
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;	//抢占优先级
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
}

//外部中断1服务程序 
void EXTI1_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(WK_UP==0)	 	 //WK_UP按键
	{
		OLED_Clear();
		delay_ms(10);
		OLED_ShowString(34,6,(u8 *)("Key: "), 16);
	}
	EXTI_ClearITPendingBit(EXTI_Line1); //清除LINE0上的中断标志位  
}
 

//外部中断2服务程序
void EXTI15_10_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY2==0)	 //按键KEY1
	{				 
		OLED_ShowString(72,6,(u8 *)("3"), 16);
	}
	else if(KEY1==0)	 //按键KEY1
	{
		OLED_ShowString(72,6,(u8 *)("2"), 16);
	}
	else if(KEY0==0)	 //按键KEY0
	{
		OLED_ShowString(72,6,(u8 *)("1"), 16);
	}
	EXTI_ClearITPendingBit(EXTI_Line13);  //清除LINE13上的中断标志位  
	EXTI_ClearITPendingBit(EXTI_Line14);  //清除LINE14上的中断标志位  
	EXTI_ClearITPendingBit(EXTI_Line15);  //清除LINE15上的中断标志位  
}

