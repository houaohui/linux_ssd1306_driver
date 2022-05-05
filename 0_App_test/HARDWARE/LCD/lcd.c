#include "lcd.h"
#include "util.h"
#include "common.h"
#include "main.h"

u8	oledBuffer[FRAME_BUFFER_SIZE];			    

//初始化lcd
//该初始化函数可以初始化各种ALIENTEK出品的LCD液晶屏
//本函数占用较大flash,用户可以根据自己的实际情况,删掉未用到的LCD初始化代码.以节省空间.

void OLED_Init(void)
{
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
	//OLED_CLS();
}

void WriteCmd(char cmd)
{
	int retvalue;
	char databuf;
	databuf = cmd;
	retvalue = write(fd, &databuf, sizeof(databuf));
	if(retvalue < 0)
		printf("oled writecmd err!\n");
}

void LCD_Init(void)
{ 
	OLED_Init();
    if(!appConfig.display180)
	{
		WriteCmd(0xA1); 
		WriteCmd(0XC8); 
	}
	else
	{
		WriteCmd(0xA0); 
		WriteCmd(0xC0); 
	}
}
  


void oled_flush(void)
{
	int retvalue;
	retvalue = write(fd, oledBuffer, sizeof(oledBuffer));
	printf("fulsh ok\r\n");
	if(retvalue < 0)
		printf("oled_fluse err!\r\n");
}




















