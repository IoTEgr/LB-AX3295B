#include"uiWin.h"

enum
{
	ITEM_SELECT_ID=0,
	ITEM_MODE_ID,
	ITEM_HIDE_RECT_ID,
	ITEM_RESOLUTION_ID,
	//ITEM_MD_ID,// motion detect
	//ITEM_MONITOR_ID, // parking monitoring
	//ITEM_IRLED_ID,
	ITEM_SD_ID,
	//ITEM_MIC_ID,
	ITEM_BATERRY_ID,
	//ITEM_POWERON_TIME_ID,
	ITEM_SYSTIME_ID,
};


static widgetCreateInfor menuItemWin[] =
{

#if 0
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_DBLUE,0),
	createRect(ITEM_HIDE_RECT_ID,              Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_BLACK),
	createImageIcon(ITEM_MODE_ID,           Rx(5), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMENU,ALIGNMENT_LEFT),
	//createStringIcon(ITEM_RESOLUTION_ID,Rx(100), Ry(0), Rw(30), Rh(25)," ",ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createImageIcon(ITEM_MD_ID,                Rx(130), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMOTION,ALIGNMENT_CENTER),
	//createImageIcon(ITEM_MONITOR_ID,     Rx(150), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTPARKOFF,ALIGNMENT_CENTER),
	//createImageIcon(ITEM_IRLED_ID,             Rx(170),Ry(0), Rw(20), Rh(25), R_ID_ICON_MTIROFF,ALIGNMENT_CENTER),

	//createImageIcon(ITEM_SD_ID,                  Rx(320-40), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	//createImageIcon(ITEM_MIC_ID,                Rx(230), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMICOFF,ALIGNMENT_CENTER),
	//createImageIcon(ITEM_BATERRY_ID,       Rx(320-20), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	//createStringIcon(ITEM_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHITE,0),
	createItemManage(ITEM_SELECT_ID,         Rx(0),Ry(25), Rw(320),Rh(215),INVALID_COLOR),
	widgetEnd(),
#else
	createFrameWin(Rx(40),Ry(42+8), Rw(240),Rh(180),/*R_COLOR_WHITE*/INVALID_COLOR,0),
	//createFrameWin(Rx(30),Ry(40), Rw(260),Rh(155),R_COLOR_DBLUE,0),
	//createRect(ITEM_HIDE_RECT_ID,              Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_BLACK),
	//createImageIcon(ITEM_MODE_ID,           Rx(5), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMENU,ALIGNMENT_LEFT),
	createItemManage(ITEM_SELECT_ID,         Rx(0),Ry(0), Rw(240),Rh(180),R_COLOR_TRANSFER),//INVALID_COLOR
	widgetEnd(),

#endif
};


static void menuResolutionShow(winHandle handle)
{
/*
	if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_VGA)
		winSetResid(winItem(handle,ITEM_RESOLUTION_ID),strToResId("VGA"));
	else if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_HD)
		winSetResid(winItem(handle,ITEM_RESOLUTION_ID),strToResId("HD"));
	else if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_FHD)
		winSetResid(winItem(handle,ITEM_RESOLUTION_ID),strToResId("FHD"));
	else 
		winSetResid(winItem(handle,ITEM_RESOLUTION_ID),strToResId("???"));
*/
}

static void menuMDShow(winHandle handle)
{
/*
	if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		winSetResid(winItem(handle,ITEM_MD_ID),R_ID_ICON_MTMOTION);
		winSetVisible(winItem(handle,ITEM_MD_ID),true);
	}
	else
		winSetVisible(winItem(handle,ITEM_MD_ID),false);
*/
}
static void menuMonitorShow(winHandle handle)
{
/*
	if(configGet(CONFIG_ID_PARKMODE)==R_ID_STR_COM_ON)
		winSetResid(winItem(handle,ITEM_MONITOR_ID),R_ID_ICON_MTPARKON);
	else
		winSetResid(winItem(handle,ITEM_MONITOR_ID),R_ID_ICON_MTPARKOFF);
*/
}

static void menuIrLedShow(winHandle handle)
{
/*
#if (1 == IR_MENU_EN)
	if(configGet(CONFIG_ID_IR_LED)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,ITEM_IRLED_ID),R_ID_ICON_MTIROFF);
	else
		winSetResid(winItem(handle,ITEM_IRLED_ID),R_ID_ICON_MTIRON);
#else
	winSetVisible(winItem(handle,ITEM_IRLED_ID),false);
#endif
*/
}
static void menuSDShow(winHandle handle)
{
/*
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,ITEM_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,ITEM_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,ITEM_SD_ID),R_ID_ICON_MTSDCNORMAL);
*/
}

static void menuMicShow(winHandle handle)
{
/*
	if(configGet(CONFIG_ID_AUDIOREC)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,ITEM_MIC_ID),R_ID_ICON_MTMICOFF);
	else
		winSetResid(winItem(handle,ITEM_MIC_ID),R_ID_ICON_MTMICON);
*/
}

static void menuBaterryShow(winHandle handle)
{
/*
	if(SysCtrl.usb != USB_STAT_NULL)
		winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	winSetVisible(winItem(handle,ITEM_BATERRY_ID),true);
*/
}


static void itemPoweOnTimeShow(winHandle handle,uint32 sec)
{
/*
	static char powerOnTimeStr[]="00:00";
	powerOnTimeStr[0] = sec/36000+'0';  // h
	powerOnTimeStr[1] = ((sec/3600)%10)+'0';   // h
	powerOnTimeStr[2] = ':';
	sec=sec%3600;
	powerOnTimeStr[3] = sec/600+'0';  // m
	powerOnTimeStr[4] = ((sec/60)%10)+'0';   // m
	powerOnTimeStr[5] = 0;
	winSetResid(winItem(handle,ITEM_POWERON_TIME_ID),strToResId(powerOnTimeStr));
*/
}

