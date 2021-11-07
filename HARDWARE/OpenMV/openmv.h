#ifndef __OPENMV_H
#define __OPENMV_H	 
#include "sys.h"

//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define WK_UP PAin(0)	//PA0  WK_UP

#define OpenMV_P0  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
#define OpenMV_P1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)
#define OpenMV_P2  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define OpenMV_P3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)

void OpenMV_IO_Init(void);//IO³õÊ¼»¯
u8 OpenMV_IO_Scan(void);  	//É¨Ãèº¯Êý					    
#endif
