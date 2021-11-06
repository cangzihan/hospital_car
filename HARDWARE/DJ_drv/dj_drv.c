#include "dj_drv.h"
#include "usart.h"
#include "delay.h"
#include "stm32f10x_tim.h"

/************************************************
����STM32F103C8T6��Сϵͳ�壨�����ĵ���������Գ���
�������оƬΪTB6560,���ù������ӷ�
������ռ����Դ:GPIOA.1 A.2 A.3�ֱ�Ϊ���1,2,3��CLK����
               GPIOB.5 B.6 B.7�ֱ�Ϊ���1,2,3�ķ������ţ����ã�B5,6,7,13,14,15��
							 ��ʱ��TIM2

ע�����
     1:�ڵ����ʼ��ʱ���������һ���жϵĸ��±�־λ����Ҫ�ڶ�ʱ��ʹ��ǰ�����(�������������ڿ�����ʱ����һ˲��ֱ�ӽ��붨ʱ���ж�)
		 2:�ڶ�ʱ��PWMģʽ�£���Ҫ�ڳ�ʼ��ʱ���ú�����Ƚ�ֵ��ԭ32���̲�û�����á�
		 3:�Ƕ�������Ϊfloat����1.8�����������1.79999995��������һ�����塣
		 4:��Ҫ����ʱ�̸ı�Ƚ�����Ĳ���,��Ӧ��ʧ��Ԥװ�ؼĴ�����
************************************************/


int count_1=0,count_2=0,count_3=0;  //���1��2��3�ߵ��������
char MOTOR_busy=0;    //��MOTOR_busy=1,��˵���������ת��

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




void MOTOR_drv1(char fx_1,double jiaodu_1,char sudu)//�������������(���� �Ƕ� �ٶ�)
{
	 //������׼���ߵ��������
  count_1=(int)(jiaodu_1/1.8*XIFEN); 
	 //���õ���ķ���
	switch(fx_1)  
	{
		case 0:    //��ʱ��
			CW_1=0;
	  	break;
		case 1:    //˳ʱ��
			CW_1=1;
  		break;
			
	}
	 //�����ٶȸı䶨ʱ�����ںͱȽ�ֵ  ѡ��1��2��3��4��
	switch(sudu)    
	{
		case 1:
      TIM2->ARR=1000;     //����ת������1ms, ��ÿ����һ������
		  TIM2->CCR2=500;	    //�Ƚ�ֵΪ���ڵ�һ��, ��ռ�ձ�Ϊ�ٷ�֮50
	  	break;
		case 2:
			TIM2->ARR=500;      //����0.5ms
	  	TIM2->CCR2=250;
	  	break;
		case 3:
			TIM2->ARR=240;      //����0.24ms
	    TIM2->CCR2=120;
	  	break;
		case 4:
			TIM2->ARR=100;      //����0.1ms
		  TIM2->CCR2=50;
	  	break;
					
	}
	if(count_1!=0)
	{
		TIM2->CCER|=0X0010;  //OC2�Ƚ����ʹ��
	}

		//*******����жϹ����־����ֹ�ж�ʹ��ֱ�ӽ����ж�*******//
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		TIM_ClearFlag(TIM2,TIM_IT_Update); //��TIM2�жϱ�־
	  //ʹ�ܶ�ʱ��TIM2
	  if(count_1!=0)
		{
			MOTOR_busy=1;
	  	TIM_Cmd(TIM2, ENABLE);
		}
}

void MOTOR_drv2(char fx_1,double jiaodu_1,char fx_2,double jiaodu_2,char sudu)//
{
   //������׼���ߵ��������
	count_1=(int)(jiaodu_1/1.8*XIFEN);  
	count_2=(int)(jiaodu_2/1.8*XIFEN);
	
	//���õ���ķ���
	switch(fx_1)       //���1�ķ���
	{
		case 0:    //��ʱ��
			CW_1=0;
		  break;
		case 1:    //˳ʱ��
			CW_1=1;
	  	break;		
	}
	switch(fx_2)      //���2�ķ���
	{
		case 0:    //��ʱ��
			CW_2=0;
	  	break;
		case 1:    //˳ʱ��
			CW_2=1;
	  	break;		
	}
	
	 //�����ٶȸı䶨ʱ�����ںͱȽ�ֵ  ѡ��1��2��3��4��
	switch(sudu)     
	{
		case 1:
      TIM2->ARR=1000;  //����ת������1ms, ��ÿ����һ������
		  TIM2->CCR2=500;	 //�Ƚ�ֵΪ���ڵ�һ��, ��ռ�ձ�Ϊ�ٷ�֮50
      TIM2->CCR3=500;				
  		break;
		case 2:
			TIM2->ARR=500;    //����0.5ms
	  	TIM2->CCR2=250;
		  TIM2->CCR3=250;		
  		break;
		case 3:
			TIM2->ARR=240;    //����0.24ms
	    TIM2->CCR2=120;
		  TIM2->CCR3=120;		
	  	break;
		case 4:
			TIM2->ARR=100;    //����0.1ms
		  TIM2->CCR2=50;
		  TIM2->CCR3=50;		
		  break;		
	}
	
	if(count_2!=0)
	{
		TIM2->CCER|=0X0100; //OC3�Ƚ����ʹ��
	}
	if(count_1!=0)
	{
		TIM2->CCER|=0X0010; //OC2�Ƚ����ʹ��
	}
	
		//*******����жϹ����־����ֹ�ж�ʹ��ֱ�ӽ����ж�*******//
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		TIM_ClearFlag(TIM2,TIM_IT_Update); //��TIM2�жϱ�־
	  //ʹ�ܶ�ʱ��TIM2
	if((count_2!=0)||(count_1!=0))
	{
		MOTOR_busy=1;
	  TIM_Cmd(TIM2, ENABLE);
	}
	
}

void MOTOR_drv3(char fx_1,u16 jiaodu_1,char fx_2,u16 jiaodu_2,char fx_3,u16 jiaodu_3,char sudu)//
{
    //������׼���ߵ��������
	  count_1=(int)(jiaodu_1/1.8*XIFEN);  
	  count_2=(int)(jiaodu_2/1.8*XIFEN);
	  count_3=(int)(jiaodu_3/1.8*XIFEN);
	 //���õ���ķ���
	  switch(fx_1)    //���1�ķ���
	{
		case 0:    
			CW_1=0;     //��ʱ��
	  	break;
		case 1:  
			CW_1=1;     //˳ʱ��
  		break;
			
	}
		switch(fx_2)   //���2�ķ���
	{
		case 0:    
			CW_2=0;     //��ʱ��
	  	break;
	 	case 1:    
			CW_2=1;     //˳ʱ��
	  	break; 
			
	}

		switch(fx_3)   //���3�ķ���
	{
		case 0:  
			CW_3=0;    //��ʱ��
	  	break;
		case 1:    
			CW_3=1;    //˳ʱ��
		  break; 
			
	}	
	  //�����ٶȸı䶨ʱ�����ںͱȽ�ֵ  ѡ��1��2��3��4��
		switch(sudu)   
	{
		case 1:
      TIM2->ARR=1000;   //����ת������1ms, ��ÿ����һ������
		  TIM2->CCR2=500;	  //�Ƚ�ֵΪ���ڵ�һ��, ��ռ�ձ�Ϊ�ٷ�֮50
      TIM2->CCR3=500;
      TIM2->CCR4=500;		
		  break;
		case 2:
			TIM2->ARR=500;    //����0.5ms
	  	TIM2->CCR2=250;
		  TIM2->CCR3=250;	
		  TIM2->CCR4=250;			
	  	break;
		case 3:
			TIM2->ARR=240;    //����0.24ms
	    TIM2->CCR2=120;
		  TIM2->CCR3=120;		
		  TIM2->CCR4=120;
	  	break;
		case 4:
			TIM2->ARR=100;    //����0.1ms
		  TIM2->CCR2=50;
		  TIM2->CCR3=50;	
		  TIM2->CCR4=50;		
		  break;
					
	}
	
	if(count_3!=0)
	{
		TIM2->CCER|=0X1000;  //OC4�Ƚ����ʹ��
	}
	if(count_2!=0)
	{
		TIM2->CCER|=0X0100;  //OC3�Ƚ����ʹ��
	}
	if(count_1!=0)
	{
		TIM2->CCER|=0X0010;  //OC2�Ƚ����ʹ��
	}
	

	//*******����жϹ����־����ֹ�ж�ʹ��ֱ�ӽ����ж�*******//
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		TIM_ClearFlag(TIM2,TIM_IT_Update); //��TIM2�жϱ�־
	//ʹ�ܶ�ʱ��TIM2
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


//***********�����ʼ��������numΪͬʱ�����ĵ������************//
void MOTOR_Init(char num)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��2ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��

    //���ݲ�������ĸ�����������ʼ��
		switch(num)
	{
		case 1:
			//���TIM2 CH2��PWM���岨��	GPIOA.1
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM_CH2   ���1CLK
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	    GPIO_ResetBits(GPIOA,GPIO_Pin_1);	//�������ӷ����ϵ�ʹ�䱣���ڵ͵�ƽ
		
	    //���1����CW_1��IO����
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
			GPIO_Init(GPIOB, &GPIO_InitStructure);					 
			GPIO_SetBits(GPIOB,GPIO_Pin_5);						 
	
			//��ʼ��TIM2
			TIM_TimeBaseStructure.TIM_Period = 10; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
			TIM_TimeBaseStructure.TIM_Prescaler =71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
			TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
			
			//��ʼ��TIM2 CH2  PWMģʽ	 
			TIM_OCInitStructure.TIM_Pulse=5;//ռ�ձ�����
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //�Ƚ����ʧ��    
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�	(����������ӷ�ѡ��)
			TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC2
			TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);  //ʧ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���	

		  break;
		case 2:
			
			//���TIM2 CH2��CH3��PWM���岨��	GPIOA.1 A.2
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2; // ���1,���2CLK
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	    GPIO_ResetBits(GPIOA,GPIO_Pin_1);	//�������ӷ����ϵ�ʹ�䱣���ڵ͵�ƽ
	    GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
		
		  //���1����CW_1,���2����CW_2��IO����
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;	 //���1,���2����CW
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
			GPIO_Init(GPIOB, &GPIO_InitStructure);					 
			GPIO_SetBits(GPIOB,GPIO_Pin_5);	
      GPIO_SetBits(GPIOB,GPIO_Pin_6);				
	
			//��ʼ��TIM2
			TIM_TimeBaseStructure.TIM_Period = 10; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
			TIM_TimeBaseStructure.TIM_Prescaler =71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
			TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
			
			//��ʼ��TIM2 CH2  PWMģʽ	 
			TIM_OCInitStructure.TIM_Pulse=5;//ռ�ձ�����
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //�Ƚ����ʧ��
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�	(����������ӷ�ѡ��)
			TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC2
			TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);  //ʧ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���	
			
			//��ʼ��TIM2 CH3  PWMģʽ	 
			TIM_OCInitStructure.TIM_Pulse=5;//ռ�ձ�����
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //�Ƚ����ʧ�� 
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�	(����������ӷ�ѡ��)
			TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC3
			TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);  //ʧ��TIM2��CCR3�ϵ�Ԥװ�ؼĴ���	
			
	  	break;
		case 3:
			
			//���TIM2 CH2,CH3,CH4��PWM���岨��	GPIOA.1 A.2 A.3
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; // ���1,���2,���3CLK
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	    GPIO_ResetBits(GPIOA,GPIO_Pin_1);	//�������ӷ����ϵ�ʹ�䱣���ڵ͵�ƽ
	    GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
	    GPIO_ResetBits(GPIOA,GPIO_Pin_3);	
		
		  //���1����CW_1,���2����CW_2,���3����CW_3��IO����
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //���1,���2����CW
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
			GPIO_Init(GPIOB, &GPIO_InitStructure);					 
			GPIO_SetBits(GPIOB,GPIO_Pin_13);	
      GPIO_SetBits(GPIOB,GPIO_Pin_14);	
      GPIO_SetBits(GPIOB,GPIO_Pin_15);	
		
			//��ʼ��TIM2
			TIM_TimeBaseStructure.TIM_Period = 10; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
			TIM_TimeBaseStructure.TIM_Prescaler =71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
			TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
			
			//��ʼ��TIM2 CH2 PWMģʽ	 
			TIM_OCInitStructure.TIM_Pulse=5;//ռ�ձ�����
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //�Ƚ����ʧ�� 
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�	(����������ӷ�ѡ��)
			TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC2
			TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);  //ʧ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���	
			//��ʼ��TIM4 CH3  PWMģʽ	 
			TIM_OCInitStructure.TIM_Pulse=5;//ռ�ձ�����
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //�Ƚ����ʧ�� 
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�	(����������ӷ�ѡ��)
			TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC3
			TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);  //ʧ��TIM2��CCR3�ϵ�Ԥװ�ؼĴ���	
			//��ʼ��TIM4 CH4  PWMģʽ	 
			TIM_OCInitStructure.TIM_Pulse=5;//ռ�ձ�����
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //�Ƚ����ʧ�� 
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�	(����������ӷ�ѡ��)
			TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC4
			TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);  //ʧ��TIM2��CCR4�ϵ�Ԥװ�ؼĴ���	
   
		  break;
					
	}
			//��ʼ���жϷ���
			NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;  //��ռ���ȼ�0��
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //�����ȼ�3��
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
			NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	  	TIM_ClearFlag(TIM2,TIM_IT_Update); //��TIM2�жϱ�־
	
    	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //ʹ��ָ����TIM2�ж�,��������ж�	
	
}

void TIM2_IRQHandler(void)   //TIM2�ж�
{	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			if(count_1>0)
			{
				count_1--;
				if(count_1==0) 	 //��������������ʧ��OC2
				{				
					TIM2->CCER&=0XFFEF;   //ʧ��OC2���
					if((count_1==0)&&(count_2==0)&&(count_3==0))//����������������󣬹رն�ʱ��
					{
						 MOTOR_busy=0;   //���busyλ
					   TIM_Cmd(TIM2, DISABLE);    
					}
				}
	  	}
			
			if(count_2>0)
			{
				count_2--;
				if(count_2==0) 	 //��������������ʧ��OC3
				{				
					TIM2->CCER&=0XFEFF;   //ʧ��OC3���
					if((count_1==0)&&(count_2==0)&&(count_3==0))//����������������󣬹رն�ʱ��
					{
						 MOTOR_busy=0;
					   TIM_Cmd(TIM2, DISABLE);
					}
				}
	  	}
			if(count_3>0)
			{
				count_3--;
				if(count_3==0) 	 //��������������ʧ��OC4
				{				
					TIM2->CCER&=0XEFFF;   //ʧ��OC4���
					if((count_1==0)&&(count_2==0)&&(count_3==0))//����������������󣬹رն�ʱ��
					{
						 MOTOR_busy=0;
					   TIM_Cmd(TIM2, DISABLE);
					}
				}
	  	}
		 TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		}
}


