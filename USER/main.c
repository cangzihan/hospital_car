#include "led.h"
#include "stm32f10x.h"
#include "GPIOLIKE51.h"
#include "delay.h"
#include "LCD1602.h"
#include "myiic.h"
#include "oled.h"
#include "key.h"
#include "exti.h"
#include "usart.h"
#include "usmart.h"	
#include "pwm.h"	
#include "driver.h"
#include "stm32f10x_tim.h"

#define Enable_12864 0
#define Enable_TIM4_PWM 0
#define Enable_UAMART (1 - Enable_TIM4_PWM)

#define yaw_0 30000
#define yaw_90 39000
#define yaw_90r 21000
#define yaw_180_1 48000
#define yaw_180_2 12000


u32 time = 0;
u32 time0;

u8 work=1;
u8 mode=0;
u8 test_mode=0;
u8 current_loc = 0;
u8 aim=1;

//俯仰，偏航，滚转
u16 pitch,yaw = 30000,roll;


void run_line(u16 yaw_keep)
{
	u16 yaw_now;    //用局部变量为了防止全局变量被中断修改
	if((yaw_keep==yaw_180_1)&&(yaw<yaw_0))
		yaw_now = yaw + 36000;
	else
		yaw_now = yaw;
	// Show 陀螺仪数据
	if(USART_RX_STA&0x8000)
	{
		USART_RX_STA=0; //清除串口标志位
		OLED_ShowNum(48,3,yaw,5);
	}
	if(yaw_now<yaw_keep-250)
	{
		car_run(0x02);
		delay_ms(20); 
	}
	else if(yaw_now>yaw_keep+250)
	{
		car_run(0x04); 
		delay_ms(20); 
	}
	else
	{
		car_run(0x01);
		delay_ms(20); 
	}
}

void back_line(u16 yaw_keep)
{
	// Show 陀螺仪数据
	if(USART_RX_STA&0x8000)
	{
		USART_RX_STA=0; //清除串口标志位
		OLED_ShowNum(48,3,yaw,5);
	}
	if(yaw<yaw_keep-250)
	{
		car_run(0x12);
		delay_ms(20); 
	}
	else if(yaw>yaw_keep+250)
	{
		car_run(0x14); 
		delay_ms(20); 
	}
	else
	{
		car_run(0x05);
		delay_ms(20); 
	}
}

void run_next_point()
{
	car_run(0x01);
	delay_ms(2000);
	car_run(0x03);
}

//小车旋转到绝对角度函数
void turn_90(u16 yaw_goal)
{
	u8 dir;
	if(yaw<yaw_goal)
		dir = 1;
	else
		dir = 0;
	
	while(1)
	{
		// Show 陀螺仪数据
		if(USART_RX_STA&0x8000)
		{
			USART_RX_STA=0; //清除串口标志位
			OLED_ShowNum(48,3,yaw,5);
		}
		if(dir == 1)
		{
			if(yaw<yaw_goal+120)
			{
				car_run(0X06);
				delay_ms(50);
			}
			else
			{
				break;
			}
		}
		else
		{
			if(yaw_goal<yaw+100)
			{
				car_run(0X07);
				delay_ms(50);
			}
			else
			{
				break;
			}
		}
	}
	//Stop
	car_run(0x03);
	delay_ms(300); 
}


//小车旋转到180度函数
//1-逆時针转到180
//0-顺时针转到180
void turn_180(u8 dir)
{
	while(1)
	{
		// Show 陀螺仪数据
		if(USART_RX_STA&0x8000)
		{
			USART_RX_STA=0; //清除串口标志位
			OLED_ShowNum(48,3,yaw,5);
		}
		if(dir == 1)
		{
			if((yaw<yaw_180_1+100)&&(yaw>yaw_0))
			{
				car_run(0X06);
				delay_ms(50);
			}
			else
			{
				break;
			}
		}
		else
		{
			if((yaw_180_2<yaw+100)&&(yaw<yaw_0))
			{
				car_run(0X07);
				delay_ms(50);
			}
			else
			{
				break;
			}
		}
	}
	//Stop
	car_run(0x03);
	delay_ms(300); 
}


// 跳转mode
void mode_change()
{
	if(mode==1)
	{
		if(time>=56)
		{
			if(aim==1||aim==2)
			{
				car_run(0x03);
				delay_ms(500); 
				if(aim==1)
				{
					mode=110;
					turn_90(yaw_90);
					mode=111;
					time0=time;
				}
				else
				{
					mode=120;
					turn_90(yaw_90r);
				}
			}
			else
			{
				mode+=1;
				time0=time;
			}
		}
	}
	else if(mode==2)
	{
		if(time>=time0+50)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(1000); 
			time0=time;
		}
	}
	else if(mode==3)
	{
		if(time>=time0+60)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(1000); 
			time0=time;
		}
	}
	else if(mode==111)
	{
		if(time>=time0+35)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(1000); 
			time0=time;
		}
	}
	else if(mode==112)
	{
		if(time>=time0+55)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
			turn_180(1);
			time0=time;
		}
	}
	else if(mode==113)
	{
		if(time>=time0+50)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
		}
	}
	else
	{
	}
}


// Initial the text of the OLED display
void display_initial(void)
{
	delay_ms(100);
	OLED_Init();
	OLED_Clear();
	delay_ms(100);
	OLED_ShowString(34,0,(u8 *)("Running"), 16);
	OLED_ShowString(12,3,(u8 *)("Yaw:"), 16);
}

void system_initial()
{
	delay_init();
	IIC_Init();
	uart_init(115200);
	uart2_init(115200);
	driver_Init();
	pwm_init();
	TIM_SetCompare1(TIM3,100); 
	TIM_SetCompare4(TIM3,100); 
}

//=============================================================================
//文件名称：main
//功能概要：主函数
//参数说明：无
//函数返回：int
//=============================================================================
int main(void)
{
	system_initial();
	display_initial();
	delay_ms(1000); 
	delay_ms(1000); 
	delay_ms(1000);  
	while(!(USART_RX_STA&0x8000));
	USART_RX_STA=0; //清除串口标志位
	delay_ms(1000); 
	mode = 1;

	while (1)
	{
		if(work==1)
		{
			if(mode==1)
				run_line(yaw_0);
			else if(mode==2)
				run_line(yaw_0);
			else if(mode==3)
				run_line(yaw_0);
			else if(mode==111)
				run_line(yaw_90);
			else if(mode==112)
				back_line(yaw_90);
			else if(mode==113)
				run_line(yaw_180_1);

			mode_change();
		}
		else if(test_mode==1)
		{
			driver_test();
		}
		time += 1;
		delay_ms(50); 
	}
}



/*
|  5 |                               |  8  |
|    |-------------------------------|     |          
|    			                                 |
|    |------|    |-------------------|     |  
|    |      |    |                   |     |
|   7|      |    |                   |  6  |
            |    |
------------|    |-------------------          
   3				|		 |				4
------------|    |-------------------  
            |    |
            |    |
            |    |
            |    |
------------|    |-------------------          
   1				|		 |				2
------------|    |-------------------  


*/
