#include "stm32f10x.h"
#include "openmv.h"
#include "sys.h" 
#include "delay.h"

								    
//按键初始化函数
void OpenMV_IO_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
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
