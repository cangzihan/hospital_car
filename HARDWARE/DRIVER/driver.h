#ifndef __DRIVER_H
#define __DRIVER_H
#include "sys.h"

#define LEFT_UP     PBout(6) 
#define LEFT_DOWN   PBout(7)	 
#define RIGHT_UP    PBout(9)	 
#define RIGHT_DOWN  PBout(8)

void driver_Init(void);
void car_run(u8 run_mode);
void driver_test(void);

		 				    
#endif
