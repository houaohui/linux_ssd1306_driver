#ifndef _OLED_H_
#define _OLED_H_

extern struct oled_dev oleddev;
extern struct oled_dev *dev;

#define OLED_SCLK_Clr()  gpio_set_value(oleddev.CLK, 0)  //CLK
#define OLED_SCLK_Set()  gpio_set_value(oleddev.CLK, 1) 

#define OLED_SDIN_Clr()  gpio_set_value(oleddev.DI,  0)  //DI
#define OLED_SDIN_Set()  gpio_set_value(oleddev.DI,  1)

#define OLED_RST_Clr()   gpio_set_value(oleddev.RST, 0)  //RES
#define OLED_RST_Set()   gpio_set_value(oleddev.RST, 1)

#define OLED_DC_Clr()    gpio_set_value(oleddev.DC,  0)  //DC
#define OLED_DC_Set()    gpio_set_value(oleddev.DC,  1)
 		     
#define OLED_CS_Clr()    gpio_set_value(oleddev.CS,  0)  //CS
#define OLED_CS_Set()    gpio_set_value(oleddev.CS,  1)


/*oled设备树结构体*/
struct oled_dev{
    dev_t devid;
    struct cdev cdev;
    struct class *class;
    struct device *device;
    int major;
    int minor;
    struct device_node *nd;
    int CS;
    int DC;
    int RST;
    int DI;
    int CLK;
};




void OLED_Init(void);
void WriteCmd(unsigned char dat);

void oled_fulsh(unsigned char BMP[]);


#endif