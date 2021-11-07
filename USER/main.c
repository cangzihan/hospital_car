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
#include "openmv.h"

#define Enable_12864 0
#define Enable_TIM4_PWM 0
#define Enable_UAMART (1 - Enable_TIM4_PWM)

#define yaw_0 30000
#define yaw_90 39000
#define yaw_90r 21000
#define yaw_180_1 48000
#define yaw_180_2 12000

#define ALLOWED_SLOPE 500

u32 time = 0;
u32 time0;

u8 work=1;
u16 mode=0;
u8 test_mode=2;
u8 current_loc = 0;
u8 aim=7;

//俯仰，偏航，滚转
u16 pitch,yaw = 30000,roll;

void run_line(u16 yaw_keep)
{
	u16 yaw_now;    //用局部变量为了防止全局变量被中断修改
	u8 openmv_data;    //Openmv数据
	if((yaw_keep==yaw_180_1)&&(yaw<yaw_0))
		yaw_now = yaw + 36000;
	else if((yaw_keep==yaw_180_2)&&(yaw>yaw_0))
		yaw_now = yaw - 36000;
	else
		yaw_now = yaw;
	// Show 陀螺仪数据
	if(USART_RX_STA&0x8000)
	{
		USART_RX_STA=0; //清除串口标志位
		OLED_ShowNum(48,3,yaw,5);
	}
	// Get Openmv数据
	openmv_data = OpenMV_IO_Scan();
	OLED_ShowChar(72,6,'0'+openmv_data/10, 16);
	OLED_ShowChar(80,6,'0'+openmv_data%10, 16);
	
	if(yaw_now<yaw_keep-ALLOWED_SLOPE||openmv_data==2)
	{
		car_run(0x02);
		OLED_ShowChar(64,6,'L', 16);
		delay_ms(20); 
	}
	else if(yaw_now>yaw_keep+ALLOWED_SLOPE||openmv_data==3)
	{
		car_run(0x04); 
		OLED_ShowChar(64,6,'R', 16);
		delay_ms(20); 
	}
	else
	{
		car_run(0x01);
		OLED_ShowChar(72,6,'G', 16);
		delay_ms(20); 
	}
}

void back_line(u16 yaw_keep)
{
	u8 openmv_data;    //Openmv数据
	// Show 陀螺仪数据
	if(USART_RX_STA&0x8000)
	{
		USART_RX_STA=0; //清除串口标志位
		OLED_ShowNum(48,3,yaw,5);
	}
	
	// Get Openmv数据
	openmv_data = OpenMV_IO_Scan();
	if(yaw<yaw_keep-ALLOWED_SLOPE||openmv_data==2)
	{
		car_run(0x12);
		delay_ms(20); 
	}
	else if(yaw>yaw_keep+ALLOWED_SLOPE||openmv_data==3)
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
			if((yaw<yaw_180_1-200)&&(yaw>yaw_0))
			{
				car_run(0X06);
				delay_ms(10);
			}
			else
			{
				break;
			}
		}
		else
		{
			if((yaw_180_2<yaw-200)&&(yaw<yaw_0))
			{
				car_run(0X07);
				delay_ms(10);
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

u8 find_goal(void)
{
	u8 openmv_data;    //Openmv数据
	// Get Openmv数据
	openmv_data = OpenMV_IO_Scan();
	if(openmv_data>3&&openmv_data<12)
	{
		aim = openmv_data-=3;
		return 1;
	}
	else
	{
		OLED_ShowChar(72,6,'0'+openmv_data/10, 16);
		OLED_ShowChar(80,6,'0'+openmv_data%10, 16);
	}
	return 0;
}

// 跳转mode
void mode_change()
{
	if(mode==0)
	{
		if(find_goal())
		{
			mode=1;
			OLED_ShowChar(36,6,'0'+aim, 16);
		}
		else
		{
			OLED_ShowChar(36,6,'0', 16);
		}
	}
	else if(mode==1)
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
					mode=121;
					time0=time;
				}
			}
			else
			{
				mode+=1;
				time0=time;
			}
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
		if(time>=time0+35)
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
		if(time>=time0+56)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(1000); 
		}
	}
	else if(mode==121)
	{
		if(time>=time0+35)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(1000); 
			time0=time;
		}
	}
	else if(mode==122)
	{
		if(time>=time0+35)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
			turn_180(0);
			time0=time;
		}
	}
	else if(mode==123)
	{
		if(time>=time0+56)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(1000); 
		}
	}
	else if(mode==2)
	{
		if(time>=time0+30)
		{
			if(aim==3||aim==4)
			{
				car_run(0x03);
				delay_ms(500); 
				if(aim==3)
				{
					mode=210;
					turn_90(yaw_90);
					mode=211;
					time0=time;
				}
				else
				{
					mode=220;
					turn_90(yaw_90r);
					mode=221;
					time0=time;
				}
			}
			else
			{
				mode+=1;
				time0=time;
			}
		}
	}
	else if(mode==211)
	{
		if(time>=time0+35)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(1000); 
			time0=time;		
		}
	}
	else if(mode==212)
	{
		if(time>=time0+35)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
			turn_180(1);
			time0=time;
		}
	}
	else if(mode==213)
	{
		if(time>=time0+120)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
		}
	}
	else if(mode==221)
	{
		if(time>=time0+35)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(1000); 
			time0=time;
		}
	}
	else if(mode==222)
	{
		if(time>=time0+35)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
			turn_180(0);
			time0=time;
		}
	}
	else if(mode==223)
	{
		if(time>=time0+120)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(1000); 
		}
	}
	else if(mode==3)
	{
		if(time>=time0+30)
		{
			if(aim==5||aim==6||aim==7||aim==8)
			{
				car_run(0x03);
				delay_ms(500); 
				if(aim==5)
				{
					mode=3110;
					turn_90(yaw_90);
					mode=3111;
					time0=time;
				}
				else if(aim==7)
				{
					mode=3120;
					turn_90(yaw_90);
					mode=3121;
					time0=time;
				}
				
				else if(aim==6)
				{
					mode=3210;
					turn_90(yaw_90r);
					mode=3211;
					time0=time;
				}else if(aim==8)
				{
					mode=3220;
					turn_90(yaw_90r);
					mode=3221;
					time0=time;
				}
			}
			else
			{
				mode+=1;
				time0=time;
			}
		}
	}
	else if(mode==3111)//远端第一次左转后前进再右转
	{
		if(time>=time0+55)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(1000); 
			turn_90(yaw_0);			
			time0=time;		
		}
	}
	else if(mode==3112)//远端右转后前进
	{
		if(time>=time0+35)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100);
			time0=time;
		}
	}
	else if(mode==3113)//远端后退然后转至90度
	{
		if(time>=time0+35)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
			turn_90(yaw_90);
			time0=time;
		}
	}
	else if(mode==3114)//再后退
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
	else if(mode==3115)//后退至起点
	{
		if(time>=time0+160)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
			time0=time;
		}
	}
	else if(mode==3121)//远端第一次左转后前进再右转
	{
		if(time>=time0+45)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(1000); 
			turn_180(1);		
			time0=time;		
		}
	}
	else if(mode==3122)//远端右转后前进
	{
		if(time>=time0+30)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100);
			time0=time;
		}
	}
	else if(mode==3123)//远端后退然后转至90度
	{
		if(time>=time0+30)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
			turn_90(yaw_90r);
			time0=time;
		}
	}
	
	else if(mode==3124)//再后退
	{
		if(time>=time0+45)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
			turn_180(0);
			time0=time;
		}
	}
	else if(mode==3125)//后退至起点
	{
		if(time>=time0+130)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
			time0=time;
		}
	}
	else if(mode==3211)//远端第一次右转后前进再右转
	{
		if(time>=time0+55)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(1000); 
			turn_180(0);		
			time0=time;		
		}
	}
	else if(mode==3212)//远端右转后前进
	{
		if(time>=time0+35)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100);
			time0=time;
		}
	}
	
	else if(mode==3213)//远端后退然后转至90度
	{
		if(time>=time0+35)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
			turn_90(yaw_90r);
			time0=time;
		}
	}
	
	else if(mode==3214)//再后退
	{
		if(time>=time0+55)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
			turn_180(0);
			time0=time;
		}
	}
	else if(mode==3215)//后退至起点
	{
		if(time>=time0+160)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
			time0=time;
		}
	}
else if(mode==3221)//远端第一次左转后前进再右转
	{
		if(time>=time0+55)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(1000); 
			turn_90(yaw_0);
			time0=time;		
		}
	}
	else if(mode==3222)//远端右转后前进
	{
		if(time>=time0+35)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100);
			time0=time;
		}
	}
	else if(mode==3223)//远端后退然后转至90度
	{
		if(time>=time0+35)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
			turn_90(yaw_90r);
			time0=time;
		}
	}
	else if(mode==3224)//再后退
	{
		if(time>=time0+55)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
			turn_180(0);
			time0=time;
		}
	}
	else if(mode==3225)//后退至起点
	{
		if(time>=time0+160)
		{
			mode+=1;
			car_run(0x03);
			delay_ms(100); 
			time0=time;
		}
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
//	uart_init(115200);
	uart2_init(115200);
	driver_Init();
	pwm_init();
	OpenMV_IO_Init();
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
	u8 openmv_data;
	system_initial();
	display_initial();
	delay_ms(1000); 
	delay_ms(1000); 
	while(!(USART_RX_STA&0x8000));
	USART_RX_STA=0; //清除串口标志位
	delay_ms(1000); 
	mode = 0;

	while (1)
	{
		if(work==1)
		{
			switch(mode)
			{
				case 3222:
				case 3112:
				case 3:
				case 2:
				case 1:run_line(yaw_0);break;
				case 3121:
				case 3111:
				case 211:
				case 111:run_line(yaw_90);break;
				case 3114:
				case 212:
				case 112:back_line(yaw_90);break;
				case 3122:
				case 3115:
				case 213:
				case 113:run_line(yaw_180_1);break;
				case 3221:
				case 3211:
				case 3124:
				case 221:
				case 121:run_line(yaw_90r);break;
				case 3224:
				case 3214:
				case 222:
				case 122:back_line(yaw_90r);break;
				case 3225:
				case 3215:
				case 3212:
				case 3125:
				case 223:
				case 123:run_line(yaw_180_2);break;
				case 3223:
				case 3123:
				case 3113:back_line(yaw_0);break;
				case 3213:back_line(yaw_180_2);break;
			}
			mode_change();
		}
		else if(test_mode==1)
		{
			driver_test();
		}
		else if(test_mode==2)
		{		
			openmv_data = OpenMV_IO_Scan();
			OLED_ShowChar(72,6,'0'+openmv_data/10, 16);
			OLED_ShowChar(80,6,'0'+openmv_data%10, 16);
			delay_ms(100);
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
