#include "exti.h"
#include "led.h"
#include "key.h"
#include "oled.h"
#include "delay.h"

//�ⲿ�ж�0�������
void EXTIX_Init(void)
{
   	EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	�����˿ڳ�ʼ��

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��


   //GPIOC.13	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

   //GPIOC.14	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource14);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line14;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

   //GPIOC.15	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY2
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource15);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


   //GPIOA.1	  �ж����Լ��жϳ�ʼ������ �½��ش��� PA1  WK_UP
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1); 

  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;	//��ռ���ȼ�
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;	//��ռ���ȼ�
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
}

//�ⲿ�ж�1������� 
void EXTI1_IRQHandler(void)
{
	delay_ms(10);//����
	if(WK_UP==0)	 	 //WK_UP����
	{
		OLED_Clear();
		delay_ms(10);
		OLED_ShowString(34,6,(u8 *)("Key: "), 16);
	}
	EXTI_ClearITPendingBit(EXTI_Line1); //���LINE0�ϵ��жϱ�־λ  
}
 

//�ⲿ�ж�2�������
void EXTI15_10_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY2==0)	 //����KEY1
	{				 
		OLED_ShowString(72,6,(u8 *)("3"), 16);
	}
	else if(KEY1==0)	 //����KEY1
	{
		OLED_ShowString(72,6,(u8 *)("2"), 16);
	}
	else if(KEY0==0)	 //����KEY0
	{
		OLED_ShowString(72,6,(u8 *)("1"), 16);
	}
	EXTI_ClearITPendingBit(EXTI_Line13);  //���LINE13�ϵ��жϱ�־λ  
	EXTI_ClearITPendingBit(EXTI_Line14);  //���LINE14�ϵ��жϱ�־λ  
	EXTI_ClearITPendingBit(EXTI_Line15);  //���LINE15�ϵ��жϱ�־λ  
}

