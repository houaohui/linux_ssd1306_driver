#ifndef __LCD_H
#define __LCD_H			 

#include "common.h"

extern  u8	oledBuffer[FRAME_BUFFER_SIZE];	

void LCD_Init(void);													   	//初始化													//关显示
void LCD_Clear(u16 Color);	 												//清屏

void WriteCmd(char cmd);
void oled_flush(void);






#endif  
	 
	 



