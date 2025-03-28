#include"uiWin.h"

enum
{
	AUDIOPLAYER_ICON_ID=0,
	AUDIOPLAYER_SD_ID,
	AUDIOPLAYER_BATERRY_ID,

	//==file list==
	AUDIOPLAYER_LIST_BK_ID,
	AUDIOPLAYER_LIST_TIPS_ID,
	AUDIOPLAYER_LIST_ITEM1_ID,
	AUDIOPLAYER_LIST_ITEM2_ID,
	AUDIOPLAYER_LIST_ITEM3_ID,
	AUDIOPLAYER_LIST_ITEM4_ID,
	AUDIOPLAYER_LIST_ITEM5_ID,
	AUDIOPLAYER_LIST_ITEM6_ID,
	AUDIOPLAYER_LIST_STR1_ID,
	AUDIOPLAYER_LIST_STR2_ID,
	AUDIOPLAYER_LIST_STR3_ID,
	AUDIOPLAYER_LIST_STR4_ID,
	AUDIOPLAYER_LIST_STR5_ID,
	AUDIOPLAYER_LIST_STR6_ID,
	AUDIOPLAYER_LIST_FILE_NUM_ID,

	AUDIOPLAYER_VOL_ID,
	
	AUDIOPLAYER_STA_ID,
	AUDIOPLAYER_PLAY_TIME_ID,

	AUDIOPLAYBACK_PROGRESSBAR0,
	AUDIOPLAYBACK_PROGRESSBAR1,
	AUDIOPLAYBACK_PROGRESSBAR2,
	AUDIOPLAYBACK_PROGRESSBAR3,
	AUDIOPLAYBACK_PROGRESSBAR4,
	AUDIOPLAYBACK_PROGRESSBAR5,
	AUDIOPLAYER_MAX_ID
};

#define AUDIO_MP3_VOLUME		50	

#define AUDIOPLAYER_LIST_NUM		6
#define AUDIOPLAYER_LIST_SHOW_TIME 	6

#define AUDIOPLAYER_LIST_FILE_MAX		512
#define AUDIOPLAYER_NAME_LEN_MAX		128

#define AUDIOPLAYER_DEC_CNT		32

typedef struct AudioPlayerMp3NameList
{
	u8 name[AUDIOPLAYER_LIST_FILE_MAX][AUDIOPLAYER_NAME_LEN_MAX+1];
};

typedef struct AudioPlayerList
{
	struct AudioPlayerMp3NameList *file_list;
	s32 mp3_list;
	u32 list_num;
	u32 list_idx;
	u32 list_play_idx;
	u32 list_show_en;
	u32 list_show_time;
	s32 cur_file;
	u32 total_time;
	u32 play_time;
};

static struct AudioPlayerList audio_player_list;

static u8 audio_player_mp3_num[]="000/000";
static u8 audio_player_mp3_time[]="00:00";

static winHandle audioPlayerProgressBar;

static widgetCreateInfor audioPlayerWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),

	//createImageIcon(AUDIOPLAYER_ICON_ID,Rx(2), Ry(0), Rw(40), Rh(36), R_ID_ICON_MENUMUSIC,ALIGNMENT_LEFT),
	createImageIcon(AUDIOPLAYER_SD_ID, Rx(40), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	createStringIcon(AUDIOPLAYER_VOL_ID,Rx(130),Ry(0),Rw(48),Rh(32),strToResId("7"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createImageIcon(AUDIOPLAYER_BATERRY_ID,Rx(280), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	
	createStringRim(AUDIOPLAYER_PLAY_TIME_ID,    Rx(10),       Ry(190), Rw(64), Rh(28),strToResId(audio_player_mp3_time),ALIGNMENT_CENTER, R_COLOR_WHITE,R_FONT_DEFAULT,R_COLOR_BLACK),
	createStringRim(AUDIOPLAYER_LIST_FILE_NUM_ID,Rx(320-80-10),Ry(190), Rw(80), Rh(28),strToResId(audio_player_mp3_num),ALIGNMENT_CENTER, R_COLOR_WHITE,0,R_COLOR_BLACK),

	//==end file list==
	createImageIcon(AUDIOPLAYER_STA_ID,Rx(140), Ry(100), Rw(48), Rh(48), R_ID_ICON_MTPAUSE,ALIGNMENT_CENTER),


	//createFrameWin(Rx(38),Ry(180), Rw(10),Rh(100),R_COLOR_RED,WIN_ABS_POS),

	createRect(AUDIOPLAYBACK_PROGRESSBAR0, Rx(38), Ry(80), Rw(10),Rh(100),R_COLOR_RED),
	createRect(AUDIOPLAYBACK_PROGRESSBAR1, Rx(58), Ry(80), Rw(10),Rh(100),R_COLOR_RED),
	createRect(AUDIOPLAYBACK_PROGRESSBAR2, Rx(78), Ry(80), Rw(10),Rh(100),R_COLOR_RED),
	createRect(AUDIOPLAYBACK_PROGRESSBAR3, Rx(98), Ry(80), Rw(10),Rh(100),R_COLOR_RED),
	createRect(AUDIOPLAYBACK_PROGRESSBAR4, Rx(118),Ry(80), Rw(10),Rh(100),R_COLOR_RED),
	createRect(AUDIOPLAYBACK_PROGRESSBAR5, Rx(138), Ry(80), Rw(10),Rh(100),R_COLOR_RED),
	
	widgetEnd(),
};


static void audioPlayerSDShow(winHandle handle)
{
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,AUDIOPLAYER_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,AUDIOPLAYER_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,AUDIOPLAYER_SD_ID),R_ID_ICON_MTSDCNORMAL);
}


static void audioPlayerBaterryShow(winHandle handle)
{
	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		if( hal_gpioRead(CHECK_FULL_CH,CHECK_FULL_PIN))
		{
			winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY4);
			//boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_B,1);
		}
		else
		{
			winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY5);
		}
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,AUDIOPLAYER_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,AUDIOPLAYER_BATERRY_ID),true);
}

static void audioPlayerStaShow(winHandle handle,u8 playing)
{
/*	if(playing)
		winSetResid(winItem(handle,AUDIOPLAYER_STA_ID),R_ID_ICON_MENUMUSICPAUSE);
	else
		winSetResid(winItem(handle,AUDIOPLAYER_STA_ID),R_ID_ICON_MENUMUSICPLAY);*/
}

static void audioPlayerTimeShow(winHandle handle)
{

	u16 min,sec;
	audio_player_list.play_time = (mp3_flie_playtime()+600)/1000;
	if(audio_player_list.play_time>audio_player_list.total_time)
	{
		audio_player_list.play_time=audio_player_list.total_time;
	}

	min=audio_player_list.play_time/60;
	sec=audio_player_list.play_time%60;
	
	audio_player_mp3_time[0]=((min/10)%10)+'0';
	audio_player_mp3_time[1]=(min%10)+'0';
	audio_player_mp3_time[3]=((sec/10)%10)+'0';
	audio_player_mp3_time[4]=(sec%10)+'0';

	audio_player_mp3_time[5]=0;

#if 0
	min=audio_player_list.total_time/60;
	sec=audio_player_list.total_time%60;
	audio_player_mp3_time[6]=((min/10)%10)+'0';
	audio_player_mp3_time[7]=(min%10)+'0';
	audio_player_mp3_time[9]=((sec/10)%10)+'0';
	audio_player_mp3_time[10]=(sec%10)+'0';
#endif
	winSetResid(winItem(handle,AUDIOPLAYER_PLAY_TIME_ID),strToResId(audio_player_mp3_time));

	winSetPorgressRate(audioPlayerProgressBar, audio_player_list.play_time*100/audio_player_list.total_time);

}

static void audioPlayerFileListShow(winHandle handle)
{
	u8 i;
	
	if(audio_player_list.list_show_en)
	{
		for(i=0;i<AUDIOPLAYER_LIST_NUM;i++)
		{
			winSetfgColor(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+i),R_COLOR_BLACK);
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+i),true);
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_STR1_ID+i),true);
			if(audio_player_list.list_idx>=(AUDIOPLAYER_LIST_NUM-1))
			{
				winSetResid(winItem(handle,AUDIOPLAYER_LIST_STR1_ID+i),strToResId(audio_player_list.file_list->name[audio_player_list.list_idx-(AUDIOPLAYER_LIST_NUM-1)+i]));
			}
			else
			{
				winSetResid(winItem(handle,AUDIOPLAYER_LIST_STR1_ID+i),strToResId(audio_player_list.file_list->name[i]));
			}
		}

		if(audio_player_list.list_idx>=(AUDIOPLAYER_LIST_NUM-1))
		{
			winSetfgColor(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+AUDIOPLAYER_LIST_NUM-1),R_COLOR_BLUE2);
		}
		else
		{
			winSetfgColor(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+audio_player_list.list_idx),R_COLOR_BLUE2);
		}
		
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_BK_ID),true);
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_TIPS_ID),true);

		//==set file num str==
		if(audio_player_list.list_num>0)
		{
			u16 sel=audio_player_list.list_idx+1;
			audio_player_mp3_num[0]=((sel/100)%10)+'0';
			audio_player_mp3_num[1]=((sel/10)%10)+'0';
			audio_player_mp3_num[2]=sel%10+'0';
			audio_player_mp3_num[4]=((audio_player_list.list_num/100)%10)+'0';
			audio_player_mp3_num[5]=((audio_player_list.list_num/10)%10)+'0';
			audio_player_mp3_num[6]=audio_player_list.list_num%10+'0';
			winSetResid(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),strToResId(audio_player_mp3_num));
		}
		else
		{
			strcpy(audio_player_mp3_num,"000/000");
		}
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),true);

		winSetVisible(winItem(handle,AUDIOPLAYER_PLAY_TIME_ID),true);
		winSetVisible(audioPlayerProgressBar, true);
		
	}
	else
	{
		for(i=0;i<AUDIOPLAYER_LIST_NUM;i++)
		{
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_ITEM1_ID+i),false);
			winSetVisible(winItem(handle,AUDIOPLAYER_LIST_STR1_ID+i),false);
		}
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_BK_ID),false);
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_TIPS_ID),false);
		winSetVisible(winItem(handle,AUDIOPLAYER_LIST_FILE_NUM_ID),false);

		
		winSetVisible(winItem(handle,AUDIOPLAYER_PLAY_TIME_ID),false);
		winSetVisible(audioPlayerProgressBar, false);
	}

}

static char audio_player_mp3_vol[2];
static void audioPlayerVolShow(winHandle handle,uint8 vol)
{
	audio_player_mp3_vol[0]= vol+ '0';
	audio_player_mp3_vol[1]= 0;
	winSetResid(winItem(handle,AUDIOPLAYER_VOL_ID),strToResId(audio_player_mp3_vol));
}


