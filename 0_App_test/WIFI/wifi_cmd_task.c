#include "wifi_cmd_task.h"
#include "common.h"
#include "wifi_task.h"



#define STR_WIFICMDMENU  "WiFi CMD"
#define CMD1_NAME "deep sleep"
#define CMD2_NAME "light sleep"
#define CMD3_NAME "updata time&date"
#define CMD4_NAME "updata weather"
#define CMD5_NAME "reset wifi"

#define OPTION_COUNT 5

typedef void (*_cmd)(void);


static _cmd ondoing;
unsigned char cmd_finish=1;


static void mSelect(void);

static void itemLoader(byte num);

char CMD_S[10];

char skip_aim_flag=0;

static void cmd_start(_cmd wifi_cmd)
{
	CMD_S[0]=2;
	CMD_S[1]=0;
	CMD_S[2]=0;
	CMD_S[3]=0;
	CMD_S[4]=0;
	CMD_S[5]=0;
	CMD_S[6]=0;
	CMD_S[7]=0;
	cmd_finish=0;
	skip_aim_flag=1;
	ondoing = wifi_cmd;
}


void WiFi_CMD_updata(void)
{
	static millis8_t lastUpdate;
	static unsigned short time=200;
	
	  // Update every 20ms
	millis8_t now = millis();
	
	if(skip_aim_flag)
	{
		skip_aim_flag=0;
		lastUpdate = now;  //���¿�ʼ��ʱ��
		time =800;  //�ȴ�800�ȶ������
	}
	
	if ((millis8_t)(now - lastUpdate) >= time)
	{
		lastUpdate = now;
		time=200;   //200ms��ʱ����
		
		if(!cmd_finish)
		{
			ondoing();
		}
		  
	
	}
	
}

void wifi_comd_open(void)
{
	menuData.isOpen = true;  //�򿪲˵�
	
	display_setDrawFunc(menu_draw);  //�󶨻��ƺ���Ϊmenu_draw

	buttons_setFuncs(menu_up, menu_select, menu_down);  //�󶨰������ܺ���

	setMenuInfo(OPTION_COUNT, MENU_TYPE_STR, PSTR(STR_WIFICMDMENU));   //��ȡ��ǰ�˵���Ϣ��ѡ��������˵����������ֻ���ͼ�꣩
	setMenuFuncs(MENUFUNC_NEXT, mSelect, MENUFUNC_PREV, itemLoader);  //�󶨲˵��ĺ���,��ǰ������ѡ��ȷ��
	beginAnimation2(NULL);      
}



static void mSelect()
{
	doAction(false);  //ִ��ָ��
	menuData.isOpen = false;  //�رղ˵�
	beginAnimation(wifi_test_open);  //����״̬�������,�ۿ�ָ��ִ�����
}




void cmd1(void)
{
	//cmd_start(deep_sleep);
}

void cmd2(void)
{
	//cmd_start(light_sleep);
}



void cmd3(void)
{
	
	//cmd_start(connect_my_phone);
}


void cmd4(void)
{
	//cmd_start(get_weather_cmd); 
}

void cmd5(void)
{
	//cmd_start(reset_wifi_cmd); 
}

static void itemLoader(byte num)
{
	num = 0;

	setMenuOption_P(num++, PSTR(CMD1_NAME), NULL, cmd1);

	setMenuOption_P(num++, PSTR(CMD2_NAME), NULL, cmd2);

	setMenuOption_P(num++, PSTR(CMD3_NAME), NULL, cmd3);
	
	setMenuOption_P(num++, PSTR(CMD4_NAME), NULL, cmd4);
	
	setMenuOption_P(num++, PSTR(CMD5_NAME), NULL, cmd5);

	addBackOption();
}


