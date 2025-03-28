#include"uiWin.h"

enum
{
	PLAYBACK_MODE_ID=0,
	PLAYBACK_HIDE_RECT_ID,
	PLAYBACK_RESOLUTION_ID,
	PLAYBACK_STA_ID,
	//PLAYBACK_MD_ID,// motion detect
	PLAYBACK_MONITOR_ID, // parking monitoring
	PLAYBACK_IRLED_ID,
	PLAYBACK_LOCK_ID,
	PLAYBACK_SD_ID,
	//PLAYBACK_MIC_ID,
	PLAYBACK_TIMENUM_ID,
	PLAYBACK_BATERRY_ID,
	PLAYBACK_POWERON_TIME_ID,
	PLAYBACK_FILE_NAME_ID,
	PLAYBACK_FILE_INDEX_ID,
	PLAYBACK_RECT_ID,
	PLAYBACK_PLAY_TIME_ID,
	PLAYBACK_TOTAL_TIME_ID,
	PLAYBACK_MAX_ID
};

static winHandle VideoPlayerProgressBar;


static widgetCreateInfor playBackWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	//createRect(PLAYBACK_HIDE_RECT_ID, Rx(0),Ry(0), Rw(320),Rh(32),R_COLOR_BLACK),
	createImageIcon(PLAYBACK_MODE_ID,Rx(0), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTPLAY,ALIGNMENT_LEFT),
	createImageIcon(PLAYBACK_SD_ID, Rx(2+32), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	createStringIcon(PLAYBACK_FILE_NAME_ID,    Rx(70), Ry(0), Rw(120), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	//createStringIcon(PLAYBACK_PLAY_TIME_ID, Rx(2+64), Ry(0), Rw(75), Rh(25),strToResId("00:00:00"),ALIGNMENT_CENTER, R_COLOR_RED,0),
	createImageIcon(PLAYBACK_STA_ID,Rx(0), Ry(0),Rw(320), Rh(240), R_ID_ICON_MTPAUSE,ALIGNMENT_CENTER),
	createStringIcon(PLAYBACK_RESOLUTION_ID,Rx(320-60), Ry(208-32), Rw(56), Rh(32),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createImageIcon(PLAYBACK_BATERRY_ID,Rx(320-32), Ry(208), Rw(32), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	createStringIcon(PLAYBACK_FILE_INDEX_ID,   Rx(230), Ry(0), Rw(90), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),

	createStringIcon(PLAYBACK_PLAY_TIME_ID, Rx(5), Ry(32), Rw(80), Rh(25),strToResId("00:00"),ALIGNMENT_CENTER, R_COLOR_RED,0),
	createStringIcon(PLAYBACK_TOTAL_TIME_ID, Rx(250), Ry(32), Rw(80), Rh(25),strToResId("00:00"),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	

	widgetEnd(),
};


static void playbackPlayTimeShow(winHandle handle,INT32U playSeconde,INT32U totalSecode)
{
	static char TimeStr[9];
	static char TotalTimeStr[9];
	//static INT32U LastSeconde=0;
	INT32U Minutes,secodes;

#if 0
	if(videoPlaybackGetStatus() == MEDIA_STAT_PLAY)
	{
		secodes = playSeconde;
		winSetStrInfor(winItem(handle,PLAYBACK_PLAY_TIME_ID),0,ALIGNMENT_CENTER,R_COLOR_RED);
	}
	else
	{
		winSetStrInfor(winItem(handle,PLAYBACK_PLAY_TIME_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
		secodes = totalSecode;
	}
#endif

	Minutes=totalSecode/60;
	secodes=totalSecode%60;
	TotalTimeStr[0]=(Minutes/10)+'0';
	TotalTimeStr[1]=(Minutes%10)+'0';
	TotalTimeStr[2]=':';
	TotalTimeStr[3]=(secodes/10)+'0';
	TotalTimeStr[4]=(secodes%10)+'0';
	TotalTimeStr[5]=0;
	winSetResid(winItem(handle,PLAYBACK_TOTAL_TIME_ID),strToResId(TotalTimeStr));


	//if(LastSeconde==secodes)
	//	return;
	//LastSeconde=secodes;
	Minutes=playSeconde/60;
	secodes=playSeconde%60;
	TimeStr[0]=(Minutes/10)+'0';
	TimeStr[1]=(Minutes%10)+'0';
	TimeStr[2]=':';
	TimeStr[3]=(secodes/10)+'0';
	TimeStr[4]=(secodes%10)+'0';
	TimeStr[5]=0;
	winSetResid(winItem(handle,PLAYBACK_PLAY_TIME_ID),strToResId(TimeStr));


	winSetPorgressRate(VideoPlayerProgressBar, playSeconde*100/totalSecode);

#if 0
	if(SysCtrl.file_type == FILE_TYPE_JPG)
	{
		winSetVisible(winItem(handle,PLAYBACK_PLAY_TIME_ID),false);
		winSetVisible(winItem(handle,PLAYBACK_TOTAL_TIME_ID),false);
		winSetVisible(handle,VideoPlayerProgressBar,false);
	}
	else
	{
		winSetVisible(winItem(handle,PLAYBACK_PLAY_TIME_ID),true);
		winSetVisible(winItem(handle,PLAYBACK_TOTAL_TIME_ID),true);
		winSetVisible(VideoPlayerProgressBar,true);
	}
#endif
}

static void playbackResolutionShow(winHandle handle)
{

	INT16U width=0,height=0;
	INT16U  lwidth = 0,lheight = 0;
	if(SysCtrl.file_type==FILE_TYPE_JPG)
	{
		imageDecodeGetResolution(&width,&height);
		imagelDecodeGetResolution(&lwidth,&lheight);
		if(height==240)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId(" "));
		else if(height==480)
		{
			if(lwidth == 7680)
			{
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("48M"));
			}
			else if(lwidth == 7040)
			{
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("40M"));
			}
			else if(lwidth == 5760)
			{
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("24M"));
			}
			else if(lwidth == 5120)
			{
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("20M"));
			}
			else if(lwidth == 4800)
			{
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("18M"));
			}
			else if(lwidth == 4480)
			{
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("16M"));
			}
			else
			{
				winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("VGA"));
			}
		}
		else if(height==720||height==960)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("1M"));
		else if(height==1080)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("2M"));
		else if(height==1440)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("3M"));
		else if(height==1920)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("5M"));
		else if(height==2400)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("8M"));
		else if(height==2460)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("10M"));
		else if(height>=2800)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("12M"));
		else 
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId(" "));
	}
	else if(SysCtrl.file_type==FILE_TYPE_AVI)
	{
		height=videoPlaybackGetResolution()->height;
		if(height==480)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("VGA"));
		else if(height==720)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("720P"));
		else if(height==1080)
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("1080P"));
		else 
			winSetResid(winItem(handle,PLAYBACK_RESOLUTION_ID),strToResId("???"));
	}

}

//==sta: 0 backward 4X, 1: backward 2x, 2:normal , 3:forward 2X, 4: forward 4X ==
static void playbackStateShow(winHandle handle,s8 sta)
{

	if(0==sta)
	{
		//winSetResid(winItem(handle,PLAYBACK_STA_ID),R_ID_ICON_MTBACKWARD4);
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),true);
	}
	else if(1==sta)
	{
		//winSetResid(winItem(handle,PLAYBACK_STA_ID),R_ID_ICON_MTBACKWARD2);
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),true);
	}
	else if(2==sta)
	{
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),false);
	}
	else if(3==sta)
	{
		//winSetResid(winItem(handle,PLAYBACK_STA_ID),R_ID_ICON_MTFORWARD2);
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),true);
	}
	else if(4==sta)
	{
		//winSetResid(winItem(handle,PLAYBACK_STA_ID),R_ID_ICON_MTFORWARD4);
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),true);
	}
	else
	{
		winSetResid(winItem(handle,PLAYBACK_STA_ID),R_ID_ICON_MTPAUSE);
		winSetVisible(winItem(handle,PLAYBACK_STA_ID),true);
	}

}


static void playbackMDShow(winHandle handle)
{
	/*if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		winSetResid(winItem(handle,PLAYBACK_MD_ID),R_ID_ICON_MTMOTION);
		winSetVisible(winItem(handle,PLAYBACK_MD_ID),true);
	}
	else
		winSetVisible(winItem(handle,PLAYBACK_MD_ID),false);*/
}
static void playbackMonitorShow(winHandle handle)
{
/*
	if(configGet(CONFIG_ID_PARKMODE)==R_ID_STR_COM_ON)
		winSetResid(winItem(handle,PLAYBACK_MONITOR_ID),R_ID_ICON_MTPARKON);
	else
		winSetResid(winItem(handle,PLAYBACK_MONITOR_ID),R_ID_ICON_MTPARKOFF);
*/
}
static void playbackIrLedShow(winHandle handle)
{
/*
#if (1 == IR_MENU_EN)
	if(configGet(CONFIG_ID_IR_LED)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,PLAYBACK_IRLED_ID),R_ID_ICON_MTIROFF);
	else
		winSetResid(winItem(handle,PLAYBACK_IRLED_ID),R_ID_ICON_MTIRON);
#else
	winSetVisible(winItem(handle,PLAYBACK_IRLED_ID),false);
#endif
*/
}
static void playbackLockShow(winHandle handle)
{
/*
	if(managerCheckLockExt(SysCtrl.avi_list,SysCtrl.file_index)>0)
	{
		winSetResid(winItem(handle,PLAYBACK_LOCK_ID),R_ID_ICON_MTLOCK);
		winSetVisible(winItem(handle,PLAYBACK_LOCK_ID),true);
	}
	else
		winSetVisible(winItem(handle,PLAYBACK_LOCK_ID),false);
*/
}
static void playbackSDShow(winHandle handle)
{
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,PLAYBACK_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,PLAYBACK_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,PLAYBACK_SD_ID),R_ID_ICON_MTSDCNORMAL);
}
static void playbackMicShow(winHandle handle)
{
	/*if(configGet(CONFIG_ID_AUDIOREC)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,PLAYBACK_MIC_ID),R_ID_ICON_MTMICOFF);
	else
		winSetResid(winItem(handle,PLAYBACK_MIC_ID),R_ID_ICON_MTMICON);*/
}

static void playbackBaterryShow(winHandle handle)
{
	if(SysCtrl.usb != USB_STAT_NULL)
	{
#if !HAL_CFG_EN_DBG
		if( hal_gpioRead(CHECK_FULL_CH,CHECK_FULL_PIN))
		{
			winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY4);
			//boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_B,1);
		}
		else
		{
			winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY5);
		}
#endif
		
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,PLAYBACK_BATERRY_ID),R_ID_ICON_MTBATTERY4);//llh
	winSetVisible(winItem(handle,PLAYBACK_BATERRY_ID),true);
}


static void playbackFileNameShow(winHandle handle)
{
	int idx;
	static char indexStr[10];
	static char filename[16]="PHO0000.JPG";
	if(NULL==SysCtrl.sdcard)
	{
		if(spi_udisk_jpg_num()>0)
		{
			u16 list_idx=spi_udisk_get_list_idx(SysCtrl.file_index);
			//file name
			filename[3]=((list_idx/1000)%10)+'0';
			filename[4]=((list_idx/100)%10)+'0';
			filename[5]=((list_idx/10)%10)+'0';
			filename[6]=((list_idx)%10)+'0';

			winSetResid(winItem(handle,PLAYBACK_FILE_NAME_ID),strToResId(filename));
			idx = SysCtrl.file_index+1;
			indexStr[0] = ((idx/1000)%10)+'0'; 
			indexStr[1] = ((idx/100)%10)+'0';
			indexStr[2] = ((idx/10)%10)+'0';
			indexStr[3] = ((idx)%10)+'0';
			indexStr[4] = '/';
			idx = spi_udisk_jpg_num();
			indexStr[5] = ((idx/1000)%10)+'0'; 
			indexStr[6] = ((idx/100)%10)+'0';
			indexStr[7] = ((idx/10)%10)+'0';
			indexStr[8] = ((idx)%10)+'0';
			indexStr[9]= 0;
			if(idx>0)
				winSetResid(winItem(handle,PLAYBACK_FILE_INDEX_ID),strToResId(indexStr));
		}
		else
		{
			winSetResid(winItem(handle,PLAYBACK_FILE_NAME_ID),strToResId(" "));
		}
	}
	else
	{
		char* name=manangerGetFileName(SysCtrl.avi_list,SysCtrl.file_index);
		if(name)
			winSetResid(winItem(handle,PLAYBACK_FILE_NAME_ID),strToResId(name));
		else
			winSetResid(winItem(handle,PLAYBACK_FILE_NAME_ID),strToResId(" "));
		idx = SysCtrl.file_index;
		indexStr[0] = ((idx/1000)%10)+'0'; 
		indexStr[1] = ((idx/100)%10)+'0';
		indexStr[2] = ((idx/10)%10)+'0';
		indexStr[3] = ((idx)%10)+'0';
		indexStr[4] = '/';
		idx = managerFileCount(SysCtrl.avi_list);
		indexStr[5] = ((idx/1000)%10)+'0'; 
		indexStr[6] = ((idx/100)%10)+'0';
		indexStr[7] = ((idx/10)%10)+'0';
		indexStr[8] = ((idx)%10)+'0';
		indexStr[9]= 0;

		if(idx>0)
			winSetResid(winItem(handle,PLAYBACK_FILE_INDEX_ID),strToResId(indexStr));

	}
}
#if SMALL_PANEL_SUPPORT==0
static void playbackPoweOnTimeShow(winHandle handle,uint32 sec)
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
	winSetResid(winItem(handle,PLAYBACK_POWERON_TIME_ID),strToResId(powerOnTimeStr));
*/
}
#endif
