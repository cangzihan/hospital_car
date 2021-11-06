#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PAout(8)
#define LED1 PAout(11)
#define LED2 PAout(12)
#define LED3 PBout(5)
#define LED4 PBout(6)
#define LED5 PBout(7)
#define LED6 PBout(8)
#define LED7 PBout(9)

void LED_Init(void);//≥ı ºªØ
void led_output(u8 cmd);

		 				    
#endif
