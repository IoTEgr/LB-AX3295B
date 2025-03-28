#include"uiWin.h"

enum
{
	VIDEO_MODE_ID=0,
	VIDEO_HIDE_RECT_ID,
	VIDEO_REC_TIME_ID,
	VIDEO_RESOLUTION_ID,
	VIDEO_MD_ID,// motion detect
	VIDEO_MONITOR_ID, // parking monitoring
	VIDEO_IRLED_ID,	// 6
	VIDEO_LOCK_ID,
	VIDEO_SD_ID,
	VIDEO_MIC_ID,	// 9
	VIDEO_BATERRY_ID,
	VIDEO_POWERON_TIME_ID,
	VIDEO_CAPTURE_PHOTO_ID,	// 12
	VIDEO_SYSTIME_ID,
	VIDEO_BOTTOM_RECT_ID,
	VIDEO_STATE_ID,
	VIDEO_REC_MODE_ID,

	VIDEO_ZOOM_RATIO_ID,
	VIDEO_FOCUS_ID,
	VIDEO_PROGRESS_BAR,
	VIDEO_MAX_ID
};


enum
{
	VIDEO_FOCUS_ICON_NONE = 0,
	VIDEO_FOCUS_ICON_YELLOW,
	VIDEO_FOCUS_ICON_RED
};


static widgetCreateInfor videoRecordeWin[] =
{
	createFrameWin( Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	//createRect(VIDEO_HIDE_RECT_ID,       Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_TBLACK),
	//createRect(VIDEO_HIDE_RECT_ID, Rx(0),Ry(0), Rw(320),Rh(32),R_COLOR_BLACK),
	createImageIcon(VIDEO_MODE_ID, Rx(2), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTRECORD,ALIGNMENT_CENTER),
	createImageIcon(VIDEO_SD_ID, Rx(34), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	createImageIcon(VIDEO_RESOLUTION_ID,Rx(70), Ry(4), Rw(48), Rh(28),R_ID_ICON_MTRECORD1080P,ALIGNMENT_CENTER),
	createImageIcon(VIDEO_STATE_ID, Rx(320-75-32), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTRECORDING,ALIGNMENT_CENTER),
	createStringIcon(VIDEO_REC_TIME_ID,  Rx(320-75), Ry(0), Rw(75), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_BLACK,0),
	createStringIcon(VIDEO_REC_MODE_ID,  Rx(5), Ry(32), Rw(200), Rh(25),strToResId("normal"),ALIGNMENT_LEFT, R_COLOR_RED,0),
	//createStringIcon(VIDEO_RESOLUTION_ID,Rx(220), Ry(0), Rw(32), Rh(32),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createImageIcon(VIDEO_BATERRY_ID,Rx(280), Ry(208), Rw(32), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
//	createStringIcon(VIDEO_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHITE,0),
//	createImageIcon(VIDEO_CAPTURE_PHOTO_ID,  Rx(147), Ry(26), Rw(20), Rh(25), R_ID_ICON_MTPHOTO,ALIGNMENT_CENTER),
	//createRect(VIDEO_BOTTOM_RECT_ID,Rx(0),Ry(215),  Rw(320),Rh(25),R_COLOR_BLACK),
	createStringIcon(VIDEO_SYSTIME_ID,Rx(5), Ry(215), Rw(220), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_YELLOW,0),

	createImageIcon(VIDEO_FOCUS_ID,Rx(60), Ry(45), Rw(200), Rh(144),R_ID_ICON_MTNULL,ALIGNMENT_CENTER),
	createStringIcon(VIDEO_ZOOM_RATIO_ID, Rx(320-58), Ry(240-25-32), Rw(58), Rh(25),strToResId("1.0X"),ALIGNMENT_CENTER, R_COLOR_YELLOW,R_FONT_NUM2),
	createProgressBar(VIDEO_PROGRESS_BAR,R1x(320-20),R1y(70),R1w(8),R1h(100),R_COLOR_GRAY3,R_COLOR_BLUE ),

	widgetEnd(),
};

static void videoResolutionShow(winHandle handle)
{
	if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_720P_SHORT)
		winSetResid(winItem(handle,VIDEO_RESOLUTION_ID),R_ID_ICON_MTRECORD720P);
	else if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_1080P_SHORT)
		winSetResid(winItem(handle,VIDEO_RESOLUTION_ID),R_ID_ICON_MTRECORD1080P);
	else if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_VGA)
		winSetResid(winItem(handle,VIDEO_RESOLUTION_ID),R_ID_ICON_MTRECORDVGA);
	else 
		winSetResid(winItem(handle,VIDEO_RESOLUTION_ID),strToResId("???"));
}
static void videoMDShow(winHandle handle)
{
/*
	if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		winSetResid(winItem(handle,VIDEO_MD_ID),R_ID_ICON_MTMOTION);
		winSetVisible(winItem(handle,VIDEO_MD_ID),true);
	}
	else
		winSetVisible(winItem(handle,VIDEO_MD_ID),false);
*/
}
static void videoMonitorShow(winHandle handle)
{
/*
	if(configGet(CONFIG_ID_PARKMODE)==R_ID_STR_COM_ON)
		winSetResid(winItem(handle,VIDEO_MONITOR_ID),R_ID_ICON_MTPARKON);
	else
		winSetResid(winItem(handle,VIDEO_MONITOR_ID),R_ID_ICON_MTPARKOFF);
*/
}
static void videoIrLedShow(winHandle handle)
{
/*
#if (1 == IR_MENU_EN)
	if(configGet(CONFIG_ID_IR_LED)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,VIDEO_IRLED_ID),R_ID_ICON_MTIROFF);
	else
		winSetResid(winItem(handle,VIDEO_IRLED_ID),R_ID_ICON_MTIRON);
#endif
*/
}
static void videoLockShow(winHandle handle)
{
/*
	if(SysCtrl.g_lock)
	{
		winSetResid(winItem(handle,VIDEO_LOCK_ID),R_ID_ICON_MTLOCK);
		winSetVisible(winItem(handle,VIDEO_LOCK_ID),true);
	}
	else
		winSetVisible(winItem(handle,VIDEO_LOCK_ID),false);
*/
}
static void videoSDShow(winHandle handle)
{
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,VIDEO_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,VIDEO_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,VIDEO_SD_ID),R_ID_ICON_MTSDCNORMAL);
}
static void videoMicShow(winHandle handle)
{
/*
	if(configGet(CONFIG_ID_AUDIOREC)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,VIDEO_MIC_ID),R_ID_ICON_MTMICOFF);
	else
		winSetResid(winItem(handle,VIDEO_MIC_ID),R_ID_ICON_MTMICON);
*/
}

static void videoBaterryShow(winHandle handle)
{

	if(SysCtrl.usb != USB_STAT_NULL)
	{
#if !HAL_CFG_EN_DBG
	if( hal_gpioRead(CHECK_FULL_CH,CHECK_FULL_PIN))
	{
		winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY4);
		//boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_B,1);
	}
	else
	{
		winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	}
#endif
		
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	else
	    winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY4); //llh
	winSetVisible(winItem(handle,VIDEO_BATERRY_ID),true);
}

#if SMALL_PANEL_SUPPORT==0
static void videoPoweOnTimeShow(winHandle handle,uint32 sec)
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
	winSetResid(winItem(handle,VIDEO_POWERON_TIME_ID),strToResId(powerOnTimeStr));
*/
}
#endif

static void videoRemainTimeShow(winHandle handle,uint32 sec)
{
	if((SysCtrl.sd_freesize>>10)>REMAIN_MIN_VIDEO)
	{
		sec=sec;
	}
	else
	{
		sec=0;
	}
	
	static char recTimeStr[]="00:00:00";
	recTimeStr[0] = sec/36000+'0';  // h
	recTimeStr[1] = ((sec/3600)%10)+'0';   // h
	recTimeStr[2] = ':';
	sec=sec%3600;
	recTimeStr[3] = sec/600+'0';  // m
	recTimeStr[4] = ((sec/60)%10)+'0';   // m
	recTimeStr[5] = ':';
	sec=sec%60;
	recTimeStr[6] = sec/10+'0';  // s
	recTimeStr[7] = sec%10+'0';   // s
	recTimeStr[8] = 0;
	winSetStrInfor(winItem(handle,VIDEO_REC_TIME_ID),0,ALIGNMENT_LEFT,R_COLOR_WHITE);
	winSetResid(winItem(handle,VIDEO_REC_TIME_ID),strToResId(recTimeStr));
}

static void videoRecTimeShow(winHandle handle,uint32 sec)
{
	static char recTimeStr[]="00:00";
	sec=sec%3600;
	recTimeStr[0] = sec/600+'0';  // m
	recTimeStr[1] = ((sec/60)%10)+'0';   // m
	recTimeStr[2] = ':';
	sec=sec%60;
	recTimeStr[3] = sec/10+'0';  // s
	recTimeStr[4] = sec%10+'0';   // s
	recTimeStr[5] = 0;
	winSetStrInfor(winItem(handle,VIDEO_REC_TIME_ID),0,ALIGNMENT_LEFT,R_COLOR_RED);
	winSetResid(winItem(handle,VIDEO_REC_TIME_ID),strToResId(recTimeStr));
}

static void videoFocusShow(winHandle handle)
{
	if(SysCtrl.video_focus == VIDEO_FOCUS_ICON_NONE)
	{
		winSetVisible(winItem(handle,VIDEO_FOCUS_ID),false);
		winSetVisible(winItem(handle,VIDEO_PROGRESS_BAR),false);
	}
	else
	{
		if(SysCtrl.video_focus == VIDEO_FOCUS_ICON_YELLOW)
			winSetResid(winItem(handle,VIDEO_FOCUS_ID),R_ID_ICON_MTPHOTOFOCUSYELLOW);
		else if(SysCtrl.video_focus == VIDEO_FOCUS_ICON_RED)
			winSetResid(winItem(handle,VIDEO_FOCUS_ID),R_ID_ICON_MTPHOTOFOCUSRED);
		winSetVisible(winItem(handle,VIDEO_FOCUS_ID),true);
		winSetVisible(winItem(handle,VIDEO_PROGRESS_BAR),true);
	}
}

static void videoZoomRatioShow(winHandle handle)
{
	char TimeStr[10];
	TimeStr[0]=SysCtrl.crop_level/10%10 + '1';
	TimeStr[1]='.';
	TimeStr[2]=SysCtrl.crop_level%10 + '0';
	TimeStr[3]='X';
	TimeStr[4]=0;
	
	hal_wdtClear();
	winSetPorgressRate(winItem(handle,VIDEO_PROGRESS_BAR),SysCtrl.crop_level*100/30);
	
	winSetResid(winItem(handle,VIDEO_ZOOM_RATIO_ID),strToResId(TimeStr));
	winSetVisible(winItem(handle,VIDEO_ZOOM_RATIO_ID),true);
	drawUIService(true);
}



