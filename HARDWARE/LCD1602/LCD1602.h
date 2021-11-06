#ifndef _LCD1602_H_
#define _LCD1602_H_
#include "sys.h"

#define uchar unsigned char
#define uint unsigned int

#define RS PBout(14)
#define RW PBout(13)
#define EN PBout(12)

#define GPIO_Pin_0_7                 ((uint16_t)0x00FF)

void LCD1602_Init(void);
void LCD12864_Init(void);
void LCD_Write_String(uchar *p);
void gotoxy(uchar x,uchar y);//����x�У�y����ʾ����
void LCD_Write_date(uchar date);
void LCD_ShowxNum1602(u16 x,u16 y,u32 num,u8 len);
    
#endif
