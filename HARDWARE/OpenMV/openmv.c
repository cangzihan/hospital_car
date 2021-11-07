#include "stm32f10x.h"
#include "openmv.h"
#include "sys.h" 
#include "delay.h"

								    
//������ʼ������
void OpenMV_IO_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��ʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
}


u8 OpenMV_IO_Scan()
{
	u8 class_mv = 0;
	if(OpenMV_P0==0)class_mv += 1;
	if(OpenMV_P1==0)class_mv += 2;
	if(OpenMV_P2==0)class_mv += 4;
	if(OpenMV_P3==0)class_mv += 8;
//	if(OpenMV_P4==0)class_mv += 16;
	return class_mv;
}
