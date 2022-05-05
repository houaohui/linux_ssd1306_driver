/*
 * Project: Lightweight millisecond tracking library
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/millisecond-tracking-library-for-avr/
 */

#ifndef ARDUINO


#include "common.h"
// #include "sys.h"
#include "led.h"
//#include "misc.h"

 millis_t milliseconds;


//static inline bool millis_interrupt_off(void)
//{
//	TIM_Cmd(TIM2,DISABLE);//开启时钟
//	return true;
//}

//static inline bool millis_interrupt_on(void)
//{
//	TIM_Cmd(TIM2,ENABLE);//开启时钟
//	return false;
//}

////// NEED TO DO A RESTORE THING! if paused then millis() is called it will unpause
//#define MILLIS_ATOMIC() for(bool cs = millis_interrupt_off(); cs; cs = millis_interrupt_on())


// Initialise library
void millis_init()
{
     
	
}

// Get current milliseconds
millis_t millis_get()
{
	millis_t ms;
		ms = milliseconds;
	return ms;
}



extern char imu_run;
 //定时器中断函数处理。 //TIM2通用定时器
void TIM3_IRQHandler(void)
{

				milliseconds++;		
				update = true;
			
			#ifdef RTC_SRC
			  update = true;
			#else
			
			
			
			if(milliseconds%1000==0)
			{
				update = true;
				++timeDate.time.secs;   //每秒钟标志位+1;
				
			}

			#endif

}










#endif
