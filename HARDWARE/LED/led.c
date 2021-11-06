#include "led.h"

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_8;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 GPIO_ResetBits(GPIOA, GPIO_Pin_8|GPIO_Pin_12|GPIO_Pin_11);						 //�����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	    	
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_ResetBits(GPIOB, GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9); 						 //����� 
}

void led_output(u8 cmd)
{
	if(cmd&0x01)
		LED0 = 1;
	else
		LED0 = 0;
	
	if(cmd&0x02)
		LED1 = 1;
	else
		LED1 = 0;
	
	if(cmd&0x04)
		LED2 = 1;
	else
		LED2 = 0;
	
	if(cmd&0x08)
		LED3 = 1;
	else
		LED3 = 0;
	
	if(cmd&0x10)
		LED4 = 1;
	else
		LED4 = 0;
	
	if(cmd&0x20)
		LED5 = 1;
	else
		LED5 = 0;
	
	if(cmd&0x40)
		LED6 = 1;
	else
		LED6 = 0;
	
	if(cmd&0x80)
		LED7 = 1;
	else
		LED7 = 0;
}

