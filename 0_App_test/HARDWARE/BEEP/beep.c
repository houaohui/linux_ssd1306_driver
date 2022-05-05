#include "beep.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//蜂鸣器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

// Buzzer
// Timer1 is used for buzzing

#include "common.h"



static uint buzzLen;
static millis8_t startTime;
static buzzFinish_f onFinish;
static tonePrio_t prio;

static void stop(void);



//TIM13 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM_PWM_Init_Init(u32 arr,u32 psc)
{                                                

                                                       
}  



//蜂鸣器初始化
//初始化PF8为输出口		    
//BEEP IO初始化
void buzzer_init()
{
	


}
 
// Non-blocking buzz
void buzzer_buzz(uint16_t len, uint16_t tone, vol_t volType, tonePrio_t _prio, buzzFinish_f _onFinish)
{
	if(_prio < prio)
		return;
	else if(tone == TONE_STOP)
	{
		stop();
		return;
	}


	
	// Tell power manager that we are busy buzzing
//	pwrmgr_setState(PWR_ACTIVE_BUZZER, PWR_STATE_IDLE);

	prio = _prio;
	onFinish = _onFinish;
	buzzLen = len;
	startTime = millis();

	// Silent pause tone   //无声暂停音
	if(tone == TONE_PAUSE)
	{
//		CLEAR_BITS(TCCR1A, COM1A1, COM1A0);
//		power_timer1_disable();
		//TIM_SetCompare1(TIM2,5000);
		
		//TIM_Cmd(TIM2, DISABLE);  //停止TIM3 

		//BEEP=0; 
		return;
	}

	// Workout volume
	uint ocr;
	byte vol;
	
	

	switch(volType)
	{
		case VOL_UI:
			vol = appConfig.volUI;
			break;
		case VOL_ALARM:
			vol = appConfig.volAlarm;
			break;
		case VOL_HOUR:
			vol = appConfig.volHour;
			break;
		default:
			vol = 2;
			break;
	}

	// Pulse width goes down as freq goes up
	// This keeps power consumption the same for all frequencies, but volume goes down as freq goes up

	//vol--;
//	if(vol > 2)
//		return;

//  uint icr = tone * (8 << 1);

//	ocr = icr - (icr / (32>>vol));
	
	ocr = 50000/tone;


	//BEEP=1; //PB.5 输出低 
	
	switch(vol)
	{
		case 0:
				//TIM_SetCompare1(TIM2,5000);//给一个超级大值关闭音量
			break;
		case 1:
			//TIM_SetCompare1(TIM2,ocr/8);//1/4音量
			break;
		case 2:
			//TIM_SetCompare1(TIM2,ocr/4);//1/2音量
			break;
		case 3:
			//TIM_SetCompare1(TIM2,ocr/2);//1/1音量
			break;
	}
	//TIM_SetCompare1(TIM1,(ocr>>(2-vol))/2);
	
	// TIM_SetAutoreload(TIM2,ocr);  //改变频率
	// TIM_SetCounter(TIM2,0);
	// TIM_Cmd(TIM2, ENABLE);

}
/*
#include "led.h"

void buzzer_buzzb(byte len, tone_t tone, vol_t volType)
{
	(void)(volType);

	led_flash(LED_GREEN, 50, 255);
	led_flash(LED_RED, 50, 255);

	power_timer1_enable();
	TCCR1A |= _BV(COM1A1)|_BV(COM1A0);

//	static uint vol = 0;
//	vol++;
//	if(vol > 790)
//		vol = 1;
//	OCR1A = vol;

//	if(vol > 3)
//		vol = 0;
//	if(vol == 0)
//		OCR1A = tone; // normal
//	else if(vol == 1)
//		OCR1A = (tone * 2) - 50; // quiet
//	else if(vol == 2)
//		OCR1A = (tone / 2); // loud
//	else if(vol == 3)
//		OCR1A = (tone / 4); // loader (acually quiter)

	OCR1A = (tone * 2) - 100;
	ICR1 = tone * 2;
	while(len--)
	{
		delay(1);
		led_update();
	}		
//	delay(20);
	TCCR1A &= ~(_BV(COM1A1)|_BV(COM1A0));
	power_timer1_disable();
}
*/
// Are we buzzing?
bool buzzer_buzzing()
{
	return buzzLen;
}
#include "led.h"
// See if its time to stop buzzing
void buzzer_update()
{

	if(buzzLen && (millis() - startTime) >= buzzLen)
	{
		stop();
		led_flash(LED_GREEN, 50, 255);

		if(onFinish != NULL)
			onFinish();  //播放下一个音节
	}
}

static void stop()
{
//	CLEAR_BITS(TCCR1A, COM1A1, COM1A0);
//	power_timer1_disable();
    //TIM_SetCompare1(TIM2,5000);
    //TIM_Cmd(TIM2, DISABLE);  //停止TIM3
	//BEEP=0; //PB.5 输出低 
	buzzLen = 0;
	prio = PRIO_MIN;

//	pwrmgr_setState(PWR_ACTIVE_BUZZER, PWR_STATE_NONE);
}

// Sometimes the buzzer kind of 'pops' from a bad waveform output (the first HIGH pulse is too long)
// Here we wait until a timer overflow and then turn on the timer output
// It still sometimes pops, but much less so than turning on the timer output in buzzer_buzz()
//ISR(TIMER1_OVF_vect)
//{
//	SET_BITS(TCCR1A, COM1A1, COM1A0);
//	TIMSK1 = 0;
//}





