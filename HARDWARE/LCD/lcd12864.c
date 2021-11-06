#include "lcd12864.h"
#include "delay.h"

#define CS PBout(12)   //RS
#define SID PBout(13)  //RW
#define SCLK PBout(14) //E

void LCD12864_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_ResetBits(GPIOB, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);						 //输出高
	
	LCD_Init();
}

void sendbyte(unsigned char zdata)
{
    unsigned int i;
    for(i=0; i<8; i++)
    {
      if((zdata << i) & 0x80)    // 11101100
      {
       SID = 1;
      }
      else
      {
       SID = 0;
      }
      SCLK = 0;
      SCLK = 1;
    }
}

void write_com(unsigned char cmdcode)    // 11101100
{
    CS = 1;
    sendbyte(0xf8);
    sendbyte(cmdcode & 0xf0);            // 1110
    sendbyte((cmdcode << 4) & 0xf0);     //  1100
		delay_us(2);
}
/********************************************************************
* ?? : write_data()
* ?? : ?????
* ?? : cmdcode
* ?? : ?
***********************************************************************/
void write_data(unsigned char Dispdata)
{
	CS = 1;
	sendbyte(0xfa);
	sendbyte(Dispdata & 0xf0);
	sendbyte((Dispdata << 4) & 0xf0);
	delay_us(2);
}
/********************************************************************
* ?? : hzkdis()
* ?? : ?????
* ?? : *s
* ?? : ?
***********************************************************************/
void LCD_Write_String(uchar *str)
{  
    while(*str > 0)
    {
      write_data(*str);
      str++;
			delay_us(50);
    }
}

/*********************************************************** 
函数名称：LCD_setxy  
函数功能：设置显示汉字位置    X(1~16),Y(1~4) 
入口参数：X(1~16),Y(1~4)  
出口参数：无 
备 注：                x坐标
Line1 |  80H  81H  82H  83H  84H  85H  86H  87H 
Line2 |  90H  91H  92H  93H  94H  95H  96H  97H
Line3 |  88H  89H  8AH  8BH  8CH  8DH  8EH  8FH
Line4 |  98H  99H  9AH  9BH  9CH  9DH  9EH  9FH
***********************************************************/ 
void gotoxy(unsigned char y,unsigned char x)
{
	switch(y)
	{
		case 1:
			write_com(0X7F+x);break;		//Line1
		case 2:
			write_com(0X8F+x);break;		//Line2
		case 3:
			write_com(0X87+x);break;		//Line3
		case 4:
			write_com(0X97+x);break;		//Line4
		default:break;
	}
}


/*********************************************************** 
函数名称：LCD_init
函数功能：12864液晶初始化
入口参数：dat 8bit 
出口参数：无 
备 注： 
***********************************************************/
void LCD_Init(void)
{
  delay_us(200);
	write_com(0x30);
	delay_us(50);
	write_com(0x0c);
	delay_us(50);
}


void Test(void)
{  
	write_com(0x03);
	delay_us(50);
	gotoxy(1,1);
	LCD_Write_String("赵");
	gotoxy(2,1);
	LCD_Write_String("爽");
	gotoxy(3,1);
	LCD_Write_String("傻");
	gotoxy(4,1);
	LCD_Write_String("逼");
}

