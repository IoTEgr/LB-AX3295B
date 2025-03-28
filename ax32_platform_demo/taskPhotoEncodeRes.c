#include"uiWin.h"

enum
{
	PHOTO_ICON_ID=0,
	PHOTO_HIDE_RECT_ID,
	PHOTO_BOTTOM_RECT_ID,
	PHOTO_PIC_REMAIN_ID,
	PHOTO_RESOLUTION_ID,
	PHOTO_MD_ID,// motion detect
	PHOTO_MONITOR_ID, // parking monitoring
	PHOTO_IRLED_ID,
	//PHOTO_LOCK_ID,
	PHOTO_SD_ID,
	PHOTO_MIC_ID,
	PHOTO_BATERRY_ID,
	PHOTO_POWERON_TIME_ID,
	PHOTO_SYSTIME_ID,
	PHOTO_TIMEPHOTO_ID,
	PHOTO_MOREPHOTO_ID,
	PHOTO_MODE_ID,
	PHOTO_TIMENUM_ID,
	PHOTO_TAKETIME_ID,
	PHOTO_ZOOM_RATIO_ID,
	PHOTO_FOCUS_ID,
	PHOTO_PROGRESS_BAR,
	PHOTO_MAX_ID
};

enum
{
	PHOTO_FOCUS_ICON_NONE = 0,
	PHOTO_FOCUS_ICON_YELLOW,
	PHOTO_FOCUS_ICON_RED
};



static widgetCreateInfor photoEncodeWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	//createRect(PHOTO_HIDE_RECT_ID,              Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_TBLACK),
	//createRect(PHOTO_HIDE_RECT_ID, Rx(0),Ry(0),  Rw(320),Rh(32),R_COLOR_BLACK),//YTS ºÚ¿ò
	createImageIcon(PHOTO_ICON_ID,           Rx(2), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTPHOTO,ALIGNMENT_LEFT),
	createStringIcon(PHOTO_PIC_REMAIN_ID,      Rx(320-75), Ry(0), Rw(75), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
//	createImageIcon(PHOTO_TIMEPHOTO_ID,Rx(132), Ry(0), Rw(20), Rh(25),R_ID_ICON_MTTIMEPHOTO3,ALIGNMENT_CENTER),
//	createImageIcon(PHOTO_MOREPHOTO_ID,Rx(164), Ry(0), Rw(20), Rh(25),R_ID_ICON_MTPHOTO_3,ALIGNMENT_CENTER),
//	createImageIcon(PHOTO_TIMENUM_ID,Rx((320-64)/2),Ry((240-64)/2), Rw(64), Rh(64),R_ID_ICON_NUM1,ALIGNMENT_CENTER),
//	createStringIcon(PHOTO_PRINTER_NOW_ID,     1      Rx(132), Ry(120), Rw(100), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
//	createImageIcon(PHOTO_MD_ID,                Rx(130), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMOTION,ALIGNMENT_CENTER),
//	createImageIcon(PHOTO_MONITOR_ID,     Rx(150), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTPARKOFF,ALIGNMENT_CENTER),
//	createImageIcon(PHOTO_IRLED_ID,             Rx(170),Ry(0), Rw(20), Rh(25), R_ID_ICON_MTIROFF,ALIGNMENT_CENTER),
	//createImageIcon(PHOTO_LOCK_ID,              Rx(190),Ry(0), Rw(20), Rh(25), R_ID_ICON_MTLOCK,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_SD_ID, Rx(40), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_MODE_ID, Rx(75), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTPHOTOTIME5,ALIGNMENT_CENTER),
	createStringIcon(PHOTO_RESOLUTION_ID,Rx(285), Ry(25), Rw(32), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(PHOTO_TAKETIME_ID, Rx(120), Ry(80), Rw(100), Rh(80),strToResId("  "),ALIGNMENT_CENTER, R_COLOR_RED,R_FONT_NUM3),
//	createImageIcon(PHOTO_MIC_ID,                Rx(230), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMICOFF,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_BATERRY_ID,Rx(280), Ry(240-32), Rw(32), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
//	createStringIcon(PHOTO_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHITE,0),
	//createRect(PHOTO_BOTTOM_RECT_ID,	   Rx(0),Ry(215),  Rw(320),Rh(25),R_COLOR_BLACK),//YTS ºÚ¿ò
	createStringIcon(PHOTO_SYSTIME_ID, Rx(5), Ry(215), Rw(220), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_YELLOW,0),
	createImageIcon(PHOTO_FOCUS_ID,Rx(60), Ry(45), Rw(200), Rh(144), /*R_ID_ICON_MTPHOTOFOCUSYELLOW*/R_ID_ICON_MTNULL,ALIGNMENT_CENTER),
	createStringIcon(PHOTO_ZOOM_RATIO_ID, Rx(320-58), Ry(240-25-32), Rw(58), Rh(25),strToResId("1.0X"),ALIGNMENT_CENTER, R_COLOR_YELLOW,R_FONT_NUM2),
	createProgressBar(PHOTO_PROGRESS_BAR,R1x(320-20),R1y(70),R1w(8),R1h(100),R_COLOR_GRAY3,R_COLOR_BLUE ),
	widgetEnd(),
};


static void photoResolutionShow(winHandle handle)
{
	if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_1M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("1M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_2M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("2M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_3M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("3M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_5M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("5M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_8M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("8M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_10M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("10M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_12M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("12M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_16M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("16M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_18M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("18M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_20M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("20M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_24M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("24M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_40M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("40M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_48M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("48M"));
	else 
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("VGA"));
}
static void photoMDShow(winHandle handle)
{
/*
	if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		winSetResid(winItem(handle,PHOTO_MD_ID),R_ID_ICON_MTMOTION);
		winSetVisible(winItem(handle,PHOTO_MD_ID),true);
	}
	else
		winSetVisible(winItem(handle,PHOTO_MD_ID),false);
*/
}
static void photoMonitorShow(winHandle handle)
{
/*
	if(configGet(CONFIG_ID_PARKMODE)==R_ID_STR_COM_ON)
		winSetResid(winItem(handle,PHOTO_MONITOR_ID),R_ID_ICON_MTPARKON);
	else
		winSetResid(winItem(handle,PHOTO_MONITOR_ID),R_ID_ICON_MTPARKOFF);
*/
}
static void photoIrLedShow(winHandle handle)
{
/*
#if (1 == IR_MENU_EN)
	if(configGet(CONFIG_ID_IR_LED)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,PHOTO_IRLED_ID),R_ID_ICON_MTIROFF);
	else
		winSetResid(winItem(handle,PHOTO_IRLED_ID),R_ID_ICON_MTIRON);
#else
	winSetVisible(winItem(handle,PHOTO_IRLED_ID),false);
#endif
*/
}
static void photoLockShow(winHandle handle,bool show)
{
	/*if(show)
	{
		winSetResid(winItem(handle,PHOTO_LOCK_ID),R_ID_ICON_MTLOCK);
		winSetVisible(winItem(handle,PHOTO_LOCK_ID),true);
	}
	else
		winSetVisible(winItem(handle,PHOTO_LOCK_ID),false);*/
}
static void photoSDShow(winHandle handle)
{
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,PHOTO_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,PHOTO_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,PHOTO_SD_ID),R_ID_ICON_MTSDCNORMAL);
}
static void photoMicShow(winHandle handle)
{
/*
	if(configGet(CONFIG_ID_AUDIOREC)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,PHOTO_MIC_ID),R_ID_ICON_MTMICOFF);
	else
		winSetResid(winItem(handle,PHOTO_MIC_ID),R_ID_ICON_MTMICON);
*/
}

static void photoPrintShow(winHandle handle)
{
//	winSetResid(winItem(handle,PHOTO_PRINTER_NOW_ID),strToResId(R_ID_STR_FIL_NULL));
}

static void photoPrintOff(winHandle handle)
{
/*
	deg_Printf("photoPrintOff  ***\n");
	winSetResid(winItem(handle,PHOTO_PRINTER_NOW_ID),strToResId(" "));
*/
}


static void photoBaterryShow(winHandle handle)
{
	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
#if !HAL_CFG_EN_DBG
		if( hal_gpioRead(CHECK_FULL_CH,CHECK_FULL_PIN))
		{
			winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY4);
			//boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_B,1);
		}
		else
		{
			winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY5);
		}
#endif
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,PHOTO_BATERRY_ID),true);
}

static void photo_mode_show(winHandle handle)
{

	deg_Printf("SysCtrl.photo_mode_idx=%d\n",SysCtrl.photo_mode_idx);

	if(0==SysCtrl.photo_mode_idx)
	{
		winSetVisible(winItem(handle,PHOTO_MODE_ID),false);
	}
	else
	{
		if(1==SysCtrl.photo_mode_idx)
		{
			winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTO3);
			winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
		}
		else if(2==SysCtrl.photo_mode_idx)
		{
			winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTO5);
			winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
		}
		else if(3==SysCtrl.photo_mode_idx)
		{
			winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTOTIME3);
			winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
		}
		else if(4==SysCtrl.photo_mode_idx)
		{
			winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTOTIME5);
			winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
		}
		else if(5==SysCtrl.photo_mode_idx)
		{
			winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTOTIME10);
			winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
		}
		else if(6==SysCtrl.photo_mode_idx)
		{
			winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTOTIME5P3);
			winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
		}

	}


}

static void photo_delay_show(winHandle handle)
{

#if 1

	if( configGet(CONFIG_ID_TIMEPHOTO) == R_ID_STR_COM_OFF)
	{
		winSetVisible(winItem(handle,PHOTO_MODE_ID),false);
	}
	else if( configGet(CONFIG_ID_TIMEPHOTO) == R_ID_STR_TIM_3SEC)
	{
		winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTOTIME3);
		winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
	}
	else if( configGet(CONFIG_ID_TIMEPHOTO) == R_ID_STR_TIM_5SEC)
	{
		winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTOTIME5);
		winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
	}
	else if( configGet(CONFIG_ID_TIMEPHOTO) == R_ID_STR_TIM_10SEC)
	{
		winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTOTIME10);
		winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
	}
	else 
	{
		winSetVisible(winItem(handle,PHOTO_MODE_ID),false);
	}
#else

	 if(0==SysCtrl.photo_delay_flag)
	{
		winSetVisible(winItem(handle,PHOTO_MODE_ID),false);
	}
	 else
 	{
		winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
 	}
#endif
}


/*
static void photo_timephoto(winHandle handle,bool show)
{
	if(true == show)
	{
		if(3 == configValue2Int(CONFIG_ID_TIMEPHOTO))
		{
			winSetResid(winItem(handle,PHOTO_TIMEPHOTO_ID),R_ID_ICON_MTTIMEPHOTO3);
			winSetVisible(winItem(handle,PHOTO_TIMEPHOTO_ID),true);
		}
		else if(5 == configValue2Int(CONFIG_ID_TIMEPHOTO))
		{
			winSetResid(winItem(handle,PHOTO_TIMEPHOTO_ID),R_ID_ICON_MTTIMEPHOTO5);
			winSetVisible(winItem(handle,PHOTO_TIMEPHOTO_ID),true);
		}
		else if(10 == configValue2Int(CONFIG_ID_TIMEPHOTO))
		{
			winSetResid(winItem(handle,PHOTO_TIMEPHOTO_ID),R_ID_ICON_MTTIMEPHOTO10);
			winSetVisible(winItem(handle,PHOTO_TIMEPHOTO_ID),true);
		}
		else
		{
			winSetVisible(winItem(handle,PHOTO_TIMEPHOTO_ID),false);
		}
	}
	else
	{
		winSetVisible(winItem(handle,PHOTO_TIMEPHOTO_ID),false);
	}
}


static void photo_morephoto(winHandle handle,bool show)
{
	if(true==show)
	{
		if(3==configValue2Int(CONFIG_ID_MOREPHOTO))
		{
			winSetResid(winItem(handle,PHOTO_MOREPHOTO_ID),R_ID_ICON_MTPHOTO_3);
			winSetVisible(winItem(handle,PHOTO_MOREPHOTO_ID),true);
		}
		else if(5==configValue2Int(CONFIG_ID_MOREPHOTO))
		{
			winSetResid(winItem(handle,PHOTO_MOREPHOTO_ID),R_ID_ICON_MTPHOTO_5);
			winSetVisible(winItem(handle,PHOTO_MOREPHOTO_ID),true);
		}
		else
		{
			winSetVisible(winItem(handle,PHOTO_MOREPHOTO_ID),false);
		}
	}
	else
	{
		winSetVisible(winItem(handle,PHOTO_MOREPHOTO_ID),false);
	}
}
*/


extern INT32U picSum;

static void photoRemainNumShow(winHandle handle)
{
#if 1
	static char picSumStr[7];

	//photoImagePreSizeGet();

	if(picSum>=0)
	{
		picSumStr[0] = ((picSum/1000000)%10)+'0'; 
		picSumStr[1] = ((picSum/100000)%10)+'0';
		picSumStr[2] = ((picSum/10000)%10)+'0'; 
		picSumStr[3] = ((picSum/1000)%10)+'0';
		picSumStr[4] = ((picSum/100)%10)+'0';
		picSumStr[5] = ((picSum/10)%10)+'0';
		picSumStr[6] = ((picSum)%10)+'0';
		//intToString(&picSumStr,5,/*MANAGER_LIST_MAX-*/picSum);
		winSetResid(winItem(handle,PHOTO_PIC_REMAIN_ID),strToResId(picSumStr));
	}
	else
		winSetResid(winItem(handle,PHOTO_PIC_REMAIN_ID),strToResId("0"));
#else
//winSetResid(winItem(handle,PHOTO_PIC_REMAIN_ID),strToResId(" "));
#endif
}


static void photoPoweOnTimeShow(winHandle handle,uint32 sec)
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
	winSetResid(winItem(handle,PHOTO_POWERON_TIME_ID),strToResId(powerOnTimeStr));
*/
}

static void photoFocusShow(winHandle handle)
{
	if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_NONE)
	{
		winSetVisible(winItem(handle,PHOTO_FOCUS_ID),false);
		winSetVisible(winItem(handle,PHOTO_PROGRESS_BAR),false);
	}
	else
	{
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW)
			winSetResid(winItem(handle,PHOTO_FOCUS_ID),R_ID_ICON_MTPHOTOFOCUSYELLOW);
		else if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_RED)
			winSetResid(winItem(handle,PHOTO_FOCUS_ID),R_ID_ICON_MTPHOTOFOCUSRED);
		winSetVisible(winItem(handle,PHOTO_FOCUS_ID),true);
		winSetVisible(winItem(handle,PHOTO_PROGRESS_BAR),true);
	}
}

static void photoZoomRatioShow(winHandle handle)
{
	char TimeStr[10];
	TimeStr[0]=SysCtrl.crop_level/10%10 + '1';
	TimeStr[1]='.';
	TimeStr[2]=SysCtrl.crop_level%10 + '0';
	TimeStr[3]='X';
	TimeStr[4]=0;
	
	hal_wdtClear();
	winSetPorgressRate(winItem(handle,PHOTO_PROGRESS_BAR),SysCtrl.crop_level*100/30);
	
	winSetResid(winItem(handle,PHOTO_ZOOM_RATIO_ID),strToResId(TimeStr));
	winSetVisible(winItem(handle,PHOTO_ZOOM_RATIO_ID),true);
	drawUIService(true);
}




