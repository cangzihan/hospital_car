#include "dj_drv.h"
#include "usart.h"
#include "delay.h"
#include "stm32f10x_tim.h"

/************************************************
基于STM32F103C8T6最小系统板（蓝）的电机驱动测试程序
电机驱动芯片为TB6560,采用共阴极接法
本驱动占用资源:GPIOA.1 A.2 A.3分别为电机1,2,3的CLK输入
               GPIOB.5 B.6 B.7分别为电机1,2,3的方向引脚（可用：B5,6,7,13,14,15）
							 定时器TIM2

注意事项：
     1:在电机初始化时，将会产生一个中断的更新标志位，需要在定时器使能前清除。(如果不清除，则在开启定时器的一瞬间直接进入定时器中断)
		 2:在定时器PWM模式下，需要在初始化时配置好输出比较值，原32例程并没有配置。
		 3:角度类型若为float，则1.8传进来变成了1.79999995，即丢了一个脉冲。
		 4:想要任意时刻改变比较输出的波形,则应该失能预装载寄存器。
************************************************/


int count_1=0,count_2=0,count_3=0;  //电机1，2，3走的脉冲个数
char MOTOR_busy=0;    //若MOTOR_busy=1,则说明电机正在转动

u8 pc=0;
char a1;
char a2;
char a3;
u16 b1;
u16 b2;
u16 b3;


extern u16 pwm1_mode;
extern u16 pwm2_mode;
extern u16 pwm3_mode;




void MOTOR_drv1(char fx_1,double jiaodu_1,char sudu)//单电机操作函数(方向 角度 速度)
{
	 //计算电机准备走的脉冲个数
  count_1=(int)(jiaodu_1/1.8*XIFEN); 
	 //设置电机的方向
	switch(fx_1)  
	{
		case 0:    //逆时针
			CW_1=0;
	  	break;
		case 1:    //顺时针
			CW_1=1;
  		break;
			
	}
	 //设置速度改变定时器周期和比较值  选择1，2，3，4档
	switch(sudu)    
	{
		case 1:
      TIM2->ARR=1000;     //设置转动周期1ms, 即每毫秒一个脉冲
		  TIM2->CCR2=500;	    //比较值为周期的一半, 即占空比为百分之50
	  	break;
		case 2:
			TIM2->ARR=500;      //周期0.5ms
	  	TIM2->CCR2=250;
	  	break;
		case 3:
			TIM2->ARR=240;      //周期0.24ms
	    TIM2->CCR2=120;
	  	break;
		case 4:
			TIM2->ARR=100;      //周期0.1ms
		  TIM2->CCR2=50;
	  	break;
					
	}
	if(count_1!=0)
	{
		TIM2->CCER|=0X0010;  //OC2比较输出使能
	}

		//*******清除中断挂起标志，防止中断使能直接进入中断*******//
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		TIM_ClearFlag(TIM2,TIM_IT_Update); //清TIM2中断标志
	  //使能定时器TIM2
	  if(count_1!=0)
		{
			MOTOR_busy=1;
	  	TIM_Cmd(TIM2, ENABLE);
		}
}

void MOTOR_drv2(char fx_1,double jiaodu_1,char fx_2,double jiaodu_2,char sudu)//
{
   //计算电机准备走的脉冲个数
	count_1=(int)(jiaodu_1/1.8*XIFEN);  
	count_2=(int)(jiaodu_2/1.8*XIFEN);
	
	//设置电机的方向
	switch(fx_1)       //电机1的方向
	{
		case 0:    //逆时针
			CW_1=0;
		  break;
		case 1:    //顺时针
			CW_1=1;
	  	break;		
	}
	switch(fx_2)      //电机2的方向
	{
		case 0:    //逆时针
			CW_2=0;
	  	break;
		case 1:    //顺时针
			CW_2=1;
	  	break;		
	}
	
	 //设置速度改变定时器周期和比较值  选择1，2，3，4档
	switch(sudu)     
	{
		case 1:
      TIM2->ARR=1000;  //设置转动周期1ms, 即每毫秒一个脉冲
		  TIM2->CCR2=500;	 //比较值为周期的一半, 即占空比为百分之50
      TIM2->CCR3=500;				
  		break;
		case 2:
			TIM2->ARR=500;    //周期0.5ms
	  	TIM2->CCR2=250;
		  TIM2->CCR3=250;		
  		break;
		case 3:
			TIM2->ARR=240;    //周期0.24ms
	    TIM2->CCR2=120;
		  TIM2->CCR3=120;		
	  	break;
		case 4:
			TIM2->ARR=100;    //周期0.1ms
		  TIM2->CCR2=50;
		  TIM2->CCR3=50;		
		  break;		
	}
	
	if(count_2!=0)
	{
		TIM2->CCER|=0X0100; //OC3比较输出使能
	}
	if(count_1!=0)
	{
		TIM2->CCER|=0X0010; //OC2比较输出使能
	}
	
		//*******清除中断挂起标志，防止中断使能直接进入中断*******//
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		TIM_ClearFlag(TIM2,TIM_IT_Update); //清TIM2中断标志
	  //使能定时器TIM2
	if((count_2!=0)||(count_1!=0))
	{
		MOTOR_busy=1;
	  TIM_Cmd(TIM2, ENABLE);
	}
	
}

void MOTOR_drv3(char fx_1,u16 jiaodu_1,char fx_2,u16 jiaodu_2,char fx_3,u16 jiaodu_3,char sudu)//
{
    //计算电机准备走的脉冲个数
	  count_1=(int)(jiaodu_1/1.8*XIFEN);  
	  count_2=(int)(jiaodu_2/1.8*XIFEN);
	  count_3=(int)(jiaodu_3/1.8*XIFEN);
	 //设置电机的方向
	  switch(fx_1)    //电机1的方向
	{
		case 0:    
			CW_1=0;     //逆时针
	  	break;
		case 1:  
			CW_1=1;     //顺时针
  		break;
			
	}
		switch(fx_2)   //电机2的方向
	{
		case 0:    
			CW_2=0;     //逆时针
	  	break;
	 	case 1:    
			CW_2=1;     //顺时针
	  	break; 
			
	}

		switch(fx_3)   //电机3的方向
	{
		case 0:  
			CW_3=0;    //逆时针
	  	break;
		case 1:    
			CW_3=1;    //顺时针
		  break; 
			
	}	
	  //设置速度改变定时器周期和比较值  选择1，2，3，4档
		switch(sudu)   
	{
		case 1:
      TIM2->ARR=1000;   //设置转动周期1ms, 即每毫秒一个脉冲
		  TIM2->CCR2=500;	  //比较值为周期的一半, 即占空比为百分之50
      TIM2->CCR3=500;
      TIM2->CCR4=500;		
		  break;
		case 2:
			TIM2->ARR=500;    //周期0.5ms
	  	TIM2->CCR2=250;
		  TIM2->CCR3=250;	
		  TIM2->CCR4=250;			
	  	break;
		case 3:
			TIM2->ARR=240;    //周期0.24ms
	    TIM2->CCR2=120;
		  TIM2->CCR3=120;		
		  TIM2->CCR4=120;
	  	break;
		case 4:
			TIM2->ARR=100;    //周期0.1ms
		  TIM2->CCR2=50;
		  TIM2->CCR3=50;	
		  TIM2->CCR4=50;		
		  break;
					
	}
	
	if(count_3!=0)
	{
		TIM2->CCER|=0X1000;  //OC4比较输出使能
	}
	if(count_2!=0)
	{
		TIM2->CCER|=0X0100;  //OC3比较输出使能
	}
	if(count_1!=0)
	{
		TIM2->CCER|=0X0010;  //OC2比较输出使能
	}
	

	//*******清除中断挂起标志，防止中断使能直接进入中断*******//
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		TIM_ClearFlag(TIM2,TIM_IT_Update); //清TIM2中断标志
	//使能定时器TIM2
	if((count_3!=0)||(count_2!=0)||(count_1!=0))
	{
		MOTOR_busy=1;
	  TIM_Cmd(TIM2, ENABLE);
	}
}


void GO_3CH(u8 fx_1,u16 jiaodu_1,u8 fx_2,u16 jiaodu_2,u8 fx_3,u16 jiaodu_3)
{
	a1=fx_1;
	a2=fx_2;
	a3=fx_3;
	b1=jiaodu_1;
	b2=jiaodu_2;
	b3=jiaodu_3;
	if(a1==1)
	{
		pwm1_mode+=b1;
	}
	else
	{
		if(pwm1_mode>b1)
		  pwm1_mode-=b1;
		else
		{
			b1=pwm1_mode;
			pwm1_mode=0;
		}
	}
	if(a2==1)
	{
		pwm2_mode+=b2;
	}
	else
	{
		if(pwm2_mode>b2)
		  pwm2_mode-=b2;
		else
		{
		b2=pwm2_mode;
		pwm2_mode=0;
		}
	}
	if(a3==1)
	{
		pwm3_mode+=b3;
	}
	else
	{
		if(pwm3_mode>b3)
		  pwm3_mode-=b3;
		else
		{
			b3=pwm3_mode;
			pwm3_mode=0;
		}
	}
  printf("\n\n CH1:%d CH2:%d CH3:%d\n",pwm1_mode,pwm2_mode,pwm3_mode);
	MOTOR_drv3(a1,b1,a2,b2,a3,b3,3);
}


void GOTO_3CH(u16 jiaodu_1,u16 jiaodu_2,u16 jiaodu_3)
{
	u16 jiao1=0,jiao2=0,jiao3=0;
	u8  dir1=0,dir2=0,dir3=0;
	
  if(jiaodu_1>pwm1_mode)
	{
		dir1=1;
		jiao1=jiaodu_1-pwm1_mode;
	}
	else
	{
		dir1=0;
		jiao1=pwm1_mode-jiaodu_1;
	}
  if(jiaodu_2>pwm2_mode)
	{
		dir2=1;
		jiao2=jiaodu_2-pwm2_mode;
	}
	else
	{
		dir2=0;
		jiao2=pwm2_mode-jiaodu_2;
		
	}
  if(jiaodu_3>pwm3_mode)
	{
		dir3=1;
		jiao3=jiaodu_3-pwm3_mode;
	}
	else
	{
		dir3=0;
		jiao3=pwm3_mode-jiaodu_3;
	}
	pwm1_mode=jiaodu_1;
	pwm2_mode=jiaodu_2;
	pwm3_mode=jiaodu_3;
	
	MOTOR_drv3(dir1,jiao1,dir2,jiao2,dir3,jiao3,3);
//	LCD_ShowNum(50,56,pwm1_mode,5,12);	
//	LCD_ShowNum(50,69,pwm2_mode,5,12);	
//	LCD_ShowNum(50,82,pwm3_mode,5,12);	
}

void GOGO_3CH(u16 jiaodu_1,u16 jiaodu_2,u16 jiaodu_3)
{
	u16 jiao1=0,jiao2=0,jiao3=0;
	u8  dir1=0,dir2=0,dir3=0;
	
  if(jiaodu_1>pwm1_mode)
	{
		dir1=1;
		jiao1=jiaodu_1-pwm1_mode;
	}
	else
	{
		dir1=0;
		jiao1=pwm1_mode-jiaodu_1;
	}
  if(jiaodu_2>pwm2_mode)
	{
		dir2=1;
		jiao2=jiaodu_2-pwm2_mode;
	}
	else
	{
		dir2=0;
		jiao2=pwm2_mode-jiaodu_2;
		
	}
  if(jiaodu_3>pwm3_mode)
	{
		dir3=1;
		jiao3=jiaodu_3-pwm3_mode;
	}
	else
	{
		dir3=0;
		jiao3=pwm3_mode-jiaodu_3;
	}
	pwm1_mode=jiaodu_1;
	pwm2_mode=jiaodu_2;
	pwm3_mode=jiaodu_3;
	
	MOTOR_drv3(dir1,jiao1,dir2,jiao2,dir3,jiao3,4);
//	LCD_ShowNum(50,56,pwm1_mode,5,12);	
//	LCD_ShowNum(50,69,pwm2_mode,5,12);	
//	LCD_ShowNum(50,82,pwm3_mode,5,12);	
}



void STEP_3CH_B(u16 value1,u16 value2,u16 value3)
{
	u8 i;
	u16 temp1=0,temp2=0,temp3=0;
	u16 step1=0,step2=0,step3=0;
	
	if(value1!=pwm1_mode||value2!=pwm2_mode||value3!=pwm3_mode)
	{
		 if(value1!=pwm1_mode)
		 {
			 if(value1>pwm1_mode)
			 {
				 step1=(value1-pwm1_mode)/10;
			 }
			 else  //pwm1_mode>value1
			 {
				 step1=(pwm1_mode-value1)/10;
			 }
		 }
		 if(value2!=pwm2_mode)   //2
		 {
			 if(value2>pwm2_mode)
			 {
				 step2=(value2-pwm2_mode)/10;
			 }
			 else  //pwm2_mode>value2
			 {
				 step2=(pwm2_mode-value2)/10;
			 }
			 
		 }
		 if(value3!=pwm3_mode)    //3
		 {
			 if(value3>pwm3_mode)
			 {
				 step3=(value3-pwm3_mode)/10;
			 }
			 else  //pwm3_mode>value3
			 {
				 step3=(pwm3_mode-value3)/10;
			 }
	  }
	}
	
	for(i=0;i<10;i++)
	{
		temp1+=step1;
		temp2+=step2;
		temp3+=step3;
		GOTO_3CH(temp1,temp2,temp3);
		delay_ms(600);
	}
	
}


//***********电机初始化函数，num为同时操作的电机个数************//
void MOTOR_Init(char num)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器2时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟

    //根据操作电机的个数，决定初始化
		switch(num)
	{
		case 1:
			//输出TIM2 CH2的PWM脉冲波形	GPIOA.1
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM_CH2   电机1CLK
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	    GPIO_ResetBits(GPIOA,GPIO_Pin_1);	//共阴极接法，上电使其保持在低电平
		
	    //电机1方向CW_1的IO设置
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
			GPIO_Init(GPIOB, &GPIO_InitStructure);					 
			GPIO_SetBits(GPIOB,GPIO_Pin_5);						 
	
			//初始化TIM2
			TIM_TimeBaseStructure.TIM_Period = 10; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
			TIM_TimeBaseStructure.TIM_Prescaler =71; //设置用来作为TIMx时钟频率除数的预分频值 
			TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
			
			//初始化TIM2 CH2  PWM模式	 
			TIM_OCInitStructure.TIM_Pulse=5;//占空比设置
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //比较输出失能    
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低	(电机共阴极接法选择)
			TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC2
			TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);  //失能TIM2在CCR2上的预装载寄存器	

		  break;
		case 2:
			
			//输出TIM2 CH2，CH3的PWM脉冲波形	GPIOA.1 A.2
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2; // 电机1,电机2CLK
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	    GPIO_ResetBits(GPIOA,GPIO_Pin_1);	//共阴极接法，上电使其保持在低电平
	    GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
		
		  //电机1方向CW_1,电机2方向CW_2的IO设置
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;	 //电机1,电机2方向CW
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
			GPIO_Init(GPIOB, &GPIO_InitStructure);					 
			GPIO_SetBits(GPIOB,GPIO_Pin_5);	
      GPIO_SetBits(GPIOB,GPIO_Pin_6);				
	
			//初始化TIM2
			TIM_TimeBaseStructure.TIM_Period = 10; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
			TIM_TimeBaseStructure.TIM_Prescaler =71; //设置用来作为TIMx时钟频率除数的预分频值 
			TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
			
			//初始化TIM2 CH2  PWM模式	 
			TIM_OCInitStructure.TIM_Pulse=5;//占空比设置
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //比较输出失能
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低	(电机共阴极接法选择)
			TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC2
			TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);  //失能TIM2在CCR2上的预装载寄存器	
			
			//初始化TIM2 CH3  PWM模式	 
			TIM_OCInitStructure.TIM_Pulse=5;//占空比设置
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //比较输出失能 
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低	(电机共阴极接法选择)
			TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC3
			TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);  //失能TIM2在CCR3上的预装载寄存器	
			
	  	break;
		case 3:
			
			//输出TIM2 CH2,CH3,CH4的PWM脉冲波形	GPIOA.1 A.2 A.3
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; // 电机1,电机2,电机3CLK
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	    GPIO_ResetBits(GPIOA,GPIO_Pin_1);	//共阴极接法，上电使其保持在低电平
	    GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
	    GPIO_ResetBits(GPIOA,GPIO_Pin_3);	
		
		  //电机1方向CW_1,电机2方向CW_2,电机3方向CW_3的IO设置
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //电机1,电机2方向CW
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
			GPIO_Init(GPIOB, &GPIO_InitStructure);					 
			GPIO_SetBits(GPIOB,GPIO_Pin_13);	
      GPIO_SetBits(GPIOB,GPIO_Pin_14);	
      GPIO_SetBits(GPIOB,GPIO_Pin_15);	
		
			//初始化TIM2
			TIM_TimeBaseStructure.TIM_Period = 10; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
			TIM_TimeBaseStructure.TIM_Prescaler =71; //设置用来作为TIMx时钟频率除数的预分频值 
			TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
			
			//初始化TIM2 CH2 PWM模式	 
			TIM_OCInitStructure.TIM_Pulse=5;//占空比设置
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //比较输出失能 
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低	(电机共阴极接法选择)
			TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC2
			TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);  //失能TIM2在CCR2上的预装载寄存器	
			//初始化TIM4 CH3  PWM模式	 
			TIM_OCInitStructure.TIM_Pulse=5;//占空比设置
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //比较输出失能 
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低	(电机共阴极接法选择)
			TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC3
			TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);  //失能TIM2在CCR3上的预装载寄存器	
			//初始化TIM4 CH4  PWM模式	 
			TIM_OCInitStructure.TIM_Pulse=5;//占空比设置
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //比较输出失能 
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低	(电机共阴极接法选择)
			TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC4
			TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);  //失能TIM2在CCR4上的预装载寄存器	
   
		  break;
					
	}
			//初始化中断分组
			NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;  //先占优先级0级
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //从优先级3级
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
			NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	  	TIM_ClearFlag(TIM2,TIM_IT_Update); //清TIM2中断标志
	
    	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //使能指定的TIM2中断,允许更新中断	
	
}

void TIM2_IRQHandler(void)   //TIM2中断
{	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			if(count_1>0)
			{
				count_1--;
				if(count_1==0) 	 //脉冲数等于零则失能OC2
				{				
					TIM2->CCER&=0XFFEF;   //失能OC2输出
					if((count_1==0)&&(count_2==0)&&(count_3==0))//三个电机脉冲均走完后，关闭定时器
					{
						 MOTOR_busy=0;   //清除busy位
					   TIM_Cmd(TIM2, DISABLE);    
					}
				}
	  	}
			
			if(count_2>0)
			{
				count_2--;
				if(count_2==0) 	 //脉冲数等于零则失能OC3
				{				
					TIM2->CCER&=0XFEFF;   //失能OC3输出
					if((count_1==0)&&(count_2==0)&&(count_3==0))//三个电机脉冲均走完后，关闭定时器
					{
						 MOTOR_busy=0;
					   TIM_Cmd(TIM2, DISABLE);
					}
				}
	  	}
			if(count_3>0)
			{
				count_3--;
				if(count_3==0) 	 //脉冲数等于零则失能OC4
				{				
					TIM2->CCER&=0XEFFF;   //失能OC4输出
					if((count_1==0)&&(count_2==0)&&(count_3==0))//三个电机脉冲均走完后，关闭定时器
					{
						 MOTOR_busy=0;
					   TIM_Cmd(TIM2, DISABLE);
					}
				}
	  	}
		 TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
		}
}


