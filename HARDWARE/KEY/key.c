#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"

								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��ʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_15;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO

	//��ʼ�� WK_UP-->GPIOA.1	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.1
}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
	{
		delay_ms(5);//ȥ���� 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
		else if(WK_UP==0)return WKUP_PRES;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==1)key_up=1; 	    
 	return 0;// �ް�������
}
