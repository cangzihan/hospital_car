#ifndef  __LCD12864_H__		  //Òª×¢ÒâÊÇ¡°_¡±¡°_¡±
#define  __LCD12864_H__
#include "sys.h"

#define uchar unsigned char
#define uint unsigned int

#define LCD_CS PBout(12)
#define LCD_SID PBout(13)
#define LCD_SCLK PBout(14)

void LCD12864_Init(void);
void LCD_Write_date(unsigned char RW, unsigned char RS, unsigned char W_data);
void serial_write_byte(unsigned char dat);

void LCD_Init(void);
void gotoxy(unsigned char x,unsigned char y);
void LCD_Write_String(uchar * str);
void Test(void);


#endif
