#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "oled.h"

#define OLEDDEV_CNT			1		  	/* 设备号个数 */
#define OLEDDEV_NAME		"oled"	/* 名字 */



struct oled_dev oleddev;  /*OLED设备*/

/*
 * @description		: 打开设备
 * @param - inode 	: 传递给驱动的inode
 * @param - filp 	: 设备文件，file结构体有个叫做private_data的成员变量
 * 					  一般在open的时候将private_data指向设备结构体。
 * @return 			: 0 成功;其他 失败
 */
static int oled_open(struct inode *inode, struct file *filp)
{
	filp->private_data = &oleddev; /* 设置私有数据 */
	return 0;
}

/*
 * @description		: 从设备读取数据 
 * @param - filp 	: 要打开的设备文件(文件描述符)
 * @param - buf 	: 返回给用户空间的数据缓冲区
 * @param - cnt 	: 要读取的数据长度
 * @param - offt 	: 相对于文件首地址的偏移
 * @return 			: 读取的字节数，如果为负值，表示读取失败
 */
static ssize_t oled_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
	return 0;
}
/*
 * @description		: 向设备写数据 
 * @param - filp 	: 设备文件，表示打开的文件描述符
 * @param - buf 	: 要写给设备写入的数据
 * @param - cnt 	: 要写入的数据长度
 * @param - offt 	: 相对于文件首地址的偏移
 * @return 			: 写入的字节数，如果为负值，表示写入失败
 */
struct oled_dev *dev;

static ssize_t oled_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
	int retvalue;
	unsigned char databuf[1024];
	dev = filp->private_data;

	retvalue = copy_from_user(databuf, buf, cnt);
	if(retvalue < 0) {
		printk("kernel write failed!\r\n");
		return -EFAULT;
	}

	printk("cnt: %d\r\n",cnt);

	if(cnt ==1 ) {
		WriteCmd(databuf[0]);
		printk("cmd: %x\r\n",databuf[0]);
	}else {
		oled_fulsh(databuf);
		printk("oled_fulsh: 1024\r\n");
	}


	return 0;
}

/*
 * @description		: 关闭/释放设备
 * @param - filp 	: 要关闭的设备文件(文件描述符)
 * @return 			: 0 成功;其他 失败
 */
static int oled_release(struct inode *inode, struct file *filp)
{
	return 0;
}
/*设备操作函数*/
static struct file_operations oleddev_fops = {
    .owner = THIS_MODULE,
    .open = oled_open,
    .read = oled_read,
    .write = oled_write,
    .release = oled_release,
};
/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */

unsigned char oledbuf[1024];
static int __init oled_init(void)
{
    int ret = 0;
    /*设置LED所使用的GPIO*/
    /*获取设备结点*/
    oleddev.nd = of_find_node_by_path("/oleddev");
    if(oleddev.nd == NULL) {
        printk("oleddev node not found!\r\n");
        return -EINVAL;

    }else {
        printk("oleddev node found!\r\n");
    }

    /*获取设备树中的gpio属性，得到所使用的OLED_GPIO编号*/
    oleddev.CS  = of_get_named_gpio(oleddev.nd, "oled-gpio-cs",  0);
    oleddev.DC  = of_get_named_gpio(oleddev.nd, "oled-gpio-dc",  0);
    oleddev.RST = of_get_named_gpio(oleddev.nd, "oled-gpio-rst", 0);
    oleddev.DI  = of_get_named_gpio(oleddev.nd, "oled-gpio-di",  0);
    oleddev.CLK = of_get_named_gpio(oleddev.nd, "oled-gpio-clk", 0);


	if((oleddev.CS + oleddev.DC + oleddev.RST + oleddev.DI + oleddev.CLK)<0) {
		printk("gpio-num err!\r\n");
		return -EINVAL;
	}
	printk("gpio num = %d,%d,%d,%d,%d\r\n", oleddev.CS,oleddev.DC,oleddev.RST,oleddev.DI,oleddev.CLK);

	
    /* 设置gpio1_io为输出，并且输出低电平 */
    ret =  gpio_direction_output(oleddev.CS, 0);
	if(ret != 0) {
        printk("cant set gpio cs!\r\n");
    }
    ret = gpio_direction_output(oleddev.DC, 0);
	if(ret != 0) {
        printk("cant set gpio dc!\r\n");
    }
    ret = gpio_direction_output(oleddev.RST,0);
	if(ret != 0) {
        printk("cant set gpio rst!\r\n");
    }
    ret = gpio_direction_output(oleddev.DI, 0);
	if(ret != 0) {
        printk("cant set gpio di!\r\n");
    }
    ret = gpio_direction_output(oleddev.CLK,0);
	if(ret != 0) {
        printk("cant set gpio clk!\r\n");
    }
	// gpio_set_value(oleddev.CS, 1);
	// gpio_set_value(oleddev.DC, 1);
	// gpio_set_value(oleddev.RST, 1);
	// gpio_set_value(oleddev.DI, 1);
	// gpio_set_value(oleddev.CLK, 1);

	OLED_RST_Set();
	mdelay(100);
	OLED_RST_Clr();
	mdelay(100);
	OLED_RST_Set();
	printk("oled init: \r\n");
	OLED_Init();
	oled_fulsh(oledbuf);
	printk("fulsh test\r\n");

	printk("ooookk\r\n");

    if(ret != 0) {
        printk("can't set gpio!\r\n");
    }

    /*申请设备号*/
	if (oleddev.major) {		/*  定义了设备号 */
		oleddev.devid = MKDEV(oleddev.major, 0);
		register_chrdev_region(oleddev.devid, OLEDDEV_CNT, OLEDDEV_NAME);
	} else {						/* 没有定义设备号 */
		alloc_chrdev_region(&oleddev.devid, 0, OLEDDEV_CNT, OLEDDEV_NAME);	/* 申请设备号 */
		oleddev.major = MAJOR(oleddev.devid);	/* 获取分配号的主设备号 */
		oleddev.minor = MINOR(oleddev.devid);	/* 获取分配号的次设备号 */
	}
	printk("oleddev major=%d,minor=%d\r\n",oleddev.major, oleddev.minor);	

    /* 2、初始化cdev */
	oleddev.cdev.owner = THIS_MODULE;
	cdev_init(&oleddev.cdev, &oleddev_fops);
	
	/* 3、添加一个cdev */
	cdev_add(&oleddev.cdev, oleddev.devid, OLEDDEV_CNT);

	/* 4、创建类 */
	oleddev.class = class_create(THIS_MODULE, OLEDDEV_NAME);
	if (IS_ERR(oleddev.class)) {
		return PTR_ERR(oleddev.class);
	}

	/* 5、创建设备 */
	oleddev.device = device_create(oleddev.class, NULL, oleddev.devid, NULL, OLEDDEV_NAME);
	if (IS_ERR(oleddev.device)) {
		return PTR_ERR(oleddev.device);
	}
	return 0;


}

/*
 * @description	: 驱动出口函数
 * @param 		: 无
 * @return 		: 无
 */
static void __exit oled_exit(void)
{
	/* 注销字符设备驱动 */
	cdev_del(&oleddev.cdev);/*  删除cdev */
	unregister_chrdev_region(oleddev.devid, OLEDDEV_CNT); /* 注销设备号 */

	device_destroy(oleddev.class, oleddev.devid);
	class_destroy(oleddev.class);
}




/////////////////////////////////////////////////////////////////////////
//extern oled_dev *dev;

// #define OLED_SCLK_Clr()  gpio_set_value(dev->CLK, 0)  //CLK
// #define OLED_SCLK_Set()  gpio_set_value(dev->CLK, 1) 

// #define OLED_SDIN_Clr()  gpio_set_value(dev->DI,  0)  //DI
// #define OLED_SDIN_Set()  gpio_set_value(dev->DI,  1)

// #define OLED_RST_Clr()   gpio_set_value(dev->RST, 0)  //RES
// #define OLED_RST_Set()   gpio_set_value(dev->RST, 1)

// #define OLED_DC_Clr()    gpio_set_value(dev->DC,  0)  //DC
// #define OLED_DC_Set()    gpio_set_value(dev->DC,  1)
 		     
// #define OLED_CS_Clr()    gpio_set_value(dev->CS,  0)  //CS
// #define OLED_CS_Set()    gpio_set_value(dev->CS,  1)



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


void WriteCmd(unsigned char dat)
{
	char i;			  
	OLED_DC_Clr();
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1; 
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set(); 
}

void WriteDat(unsigned char dat)
{	
	char i;			  
	OLED_DC_Set();
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
}

void oled_fulsh(unsigned char BMP[])
{
	char i,j;
	unsigned char *p;
	p=BMP;

	for(i=0;i<8;i++)
	{
		WriteCmd(0xb0+i);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);	

		for(j=0;j<128;j++)
		{
			WriteDat(*p++);
		}
	}
}





module_init(oled_init);
module_exit(oled_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("hah");