#include "application.h"
#include "taskAudioPlayerRes.c"

#define AUDIO_PLAY_FLASH_DEMO		0 // 0: not play flash mp3 demo  1: play flash mp3 demo

static u8* audioplayer_bk_buf;
static u16 bk_w,bk_h;

//static u8 *audio_demo_buf;
static  u32 audio_demo_size;

/*static u32 audioplay_jgp_id[6]={RES_AUDIOPLAY0_BK,RES_AUDIOPLAY1_BK,RES_AUDIOPLAY2_BK,
								 RES_AUDIOPLAY3_BK,RES_AUDIOPLAY4_BK,RES_AUDIOPLAY5_BK,
								};
*/
static u32 audio_show_time;

u16* uni2oem936_data_buf=NULL;
u16* oem2uni936_data_buf=NULL;

static winHandle playbackProgressBar0;


static void audioplay_showbuf();
static	int audio_play_state;

#define PLAYBACK_BAR_NUM 		10

typedef	struct
{
	u16 x0;
	u16 y0; 
	u16 x1;
	u16 y1; 
}Playing_Bar_Pos;

static Playing_Bar_Pos playing_bar_showpos[PLAYBACK_BAR_NUM]=
{
	{38,175,48,180},	
	{38,170,48,180},	
	{38,160,48,180},	
	{38,150,48,180},	
	{38,120,48,180},	
	{38,80,48,180},	
	{38,95,48,180},	
	{38,110,48,180},	
	{38,130,48,180},	
	{38,170,48,180},	
};

static u8 audio_playing_flag;

static void audioPlayerBarShow(winHandle handle)
{
	u8 i,temp;

	if(MP3_DAC_START==mp3_play_sta())
	{
		if(!winIsVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR0))) winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR0),true);
		if(!winIsVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR1))) winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR1),true);
		if(!winIsVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR2))) winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR2),true);
		if(!winIsVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR3))) winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR3),true);
		if(!winIsVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR4))) winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR4),true);
		if(!winIsVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR5))) winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR5),true);
	
		audio_playing_flag++;
		if(audio_playing_flag>=PLAYBACK_BAR_NUM)
		{
			audio_playing_flag=0;
		}

		//for(i=0;i<6;i++)
		{
			temp=audio_playing_flag;
			winResize(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR0),playing_bar_showpos[temp].x0, playing_bar_showpos[temp].y0, playing_bar_showpos[temp].x1, playing_bar_showpos[temp].y1);
			temp++;
			if(temp>=PLAYBACK_BAR_NUM) temp=0;
			winResize(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR1),playing_bar_showpos[temp].x0+20, playing_bar_showpos[temp].y0, playing_bar_showpos[temp].x1+20, playing_bar_showpos[temp].y1);
			temp++;
			if(temp>=PLAYBACK_BAR_NUM) temp=0;
			winResize(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR2),playing_bar_showpos[temp].x0+40, playing_bar_showpos[temp].y0, playing_bar_showpos[temp].x1+40, playing_bar_showpos[temp].y1);
			temp++;
			if(temp>=PLAYBACK_BAR_NUM) temp=0;
			winResize(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR3),playing_bar_showpos[temp].x0+60, playing_bar_showpos[temp].y0, playing_bar_showpos[temp].x1+60, playing_bar_showpos[temp].y1);
			temp++;
			if(temp>=PLAYBACK_BAR_NUM) temp=0;
			winResize(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR4),playing_bar_showpos[temp].x0+80, playing_bar_showpos[temp].y0, playing_bar_showpos[temp].x1+80, playing_bar_showpos[temp].y1);
			temp++;
			if(temp>=PLAYBACK_BAR_NUM) temp=0;
			winResize(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR5),playing_bar_showpos[temp].x0+100, playing_bar_showpos[temp].y0, playing_bar_showpos[temp].x1+100, playing_bar_showpos[temp].y1);
		}
	}

}


static u32 audioplayer_file_list_scan(u8 *dir)
{
	FRESULT res;
	FILINFO finfo;
	int count=0,type;
	u8 dirpath[128],*path;
	DIR drent;
    //deg_Printf("file scan : dir:%s\n",dir);
	count = 0;
	path = dir;
	while(*dir)
	{
		if((*path == '/')&&(0 == *(path+1)))
		{
			break;
		}
		dirpath[count++] = *path++;
	}
	dirpath[count++] = 0;

	count = 0;
	res = f_findfirst(&drent,&finfo,dirpath,"*.*");
	while(1)
	{         
		 if(res!=FR_OK || finfo.fname[0] == 0)
		 	break;
		 if(finfo.fname[0] != '.')
		 {
		 	 //deg_Printf("scan:%s\n",finfo.fname);
		 	 type = manangerCheckFileType(finfo.fname);
		 	 if(type<0)
		 	 {
				 //deg_Printf("unknow type:%s\n",finfo.fname);
		 	 }
			 else
			 {
			 	 //deg_Printf("mp3:%s\n",finfo.fname);
				 if(FILE_TYPE_MP3==type)
				 {
					 //deg_Printf("mp3 file\n");
					 //deg_Printf("scan:%s\n",finfo.fname);
					 if(strlen(finfo.fname)<AUDIOPLAYER_NAME_LEN_MAX)
					 {
						 strcpy(audio_player_list.file_list->name[count],finfo.fname);
					 }

					 count++;
					 if(count>=AUDIOPLAYER_LIST_FILE_MAX)
					 {
						break;
					 }
				 }
			 }
		 }
		 res = f_findnext(&drent,&finfo);

	}
	f_closedir(&drent);

	return count;	

}

static void audioplayer_play_mp3()
{
	//==play mp3==
	if((SDC_STAT_NORMAL==SysCtrl.sdcard)||(SDC_STAT_FULL==SysCtrl.sdcard))	// play sd file  
	{
		if(audio_player_list.list_num>0)
		{
			//==open mp3 file and play==
			if(MP3_DAC_STOP!=mp3_play_sta())
			{
				mp3_dacPlay_Stop();
			}

			if(audio_player_list.cur_file>=0)
			{
				close(audio_player_list.cur_file);
				audio_player_list.cur_file=-1;
			}

			u8 filepath[256];
			memset(filepath,0,sizeof(filepath));
			memcpy(filepath,FILEDIR_MP3,sizeof(FILEDIR_MP3));
			strcpy(filepath+strlen(FILEDIR_MP3),audio_player_list.file_list->name[audio_player_list.list_idx]);

			audio_player_list.cur_file=(s32)open(filepath,FA_READ);
			if(audio_player_list.cur_file < 0)
			{
				deg_Printf("open [%s] error\n",filepath);
			}

			mp3_decode_play(audio_player_list.cur_file,/*AUDIO_MP3_VOLUME*/ configValue2Int(CONFIG_ID_VOLUME)*20 );
			audio_player_list.list_play_idx=audio_player_list.list_idx;

			audio_player_list.total_time = mp3_file_time_cal();
			audio_player_list.play_time = 0;
			deg_Printf("mp3_play_time=%d,%d\n",audio_player_list.play_time,audio_player_list.total_time);

		}
	}
	else
	{
		#if AUDIO_PLAY_FLASH_DEMO
		//==play mp3 demo==
		u32 addr,size;
		addr= nv_open(RES_AUDIO_PLAYER_DEMO);
		if(addr<0)
		{
			deg_Printf("read id err\n");
		}
		size = nv_size(RES_AUDIO_PLAYER_DEMO);
		if((size<64)||(size>2*1024*1024))
		{
			deg_Printf("size err:%d\n",size);
		}

		if(NULL==audio_demo_buf)
		{
			audio_demo_size=size;
			audio_demo_buf=hal_sysMemMalloc(audio_demo_size,32);
		}

		if(audio_demo_buf)
		{
			deg_Printf("demo size:%d\n",audio_demo_size);
			nv_read(addr,audio_demo_buf,audio_demo_size);

			mp3_decode_play_buf(audio_demo_buf,audio_demo_size,AUDIO_MP3_VOLUME);

			audio_player_list.total_time = mp3_file_time_cal();
			audio_player_list.play_time = 0;
			deg_Printf("mp3_play_time=%d,%d\n",audio_player_list.play_time,audio_player_list.total_time);

		}
		else
		{
			deg_Printf("mem err:%d\n",audio_demo_size);
		}
		#endif
		
	}

}

static int audioPlayerKeyMsgList(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	int ret;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(audio_player_list.list_show_en)
		{
			audio_player_list.list_show_en=0;
		}
		else
		{
			audio_player_list.list_show_en=1;
			audio_player_list.list_show_time=AUDIOPLAYER_LIST_SHOW_TIME;
		}

		audioPlayerFileListShow(handle);
	}
	return 0;
}

static int audioPlayerKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	/*	if(audio_player_list.list_show_en)
		{
			audio_player_list.list_show_time=AUDIOPLAYER_LIST_SHOW_TIME;
			audioplayer_play_mp3();
			audioPlayerStaShow(handle,1);
		}
		else*/
		{
			if(MP3_DAC_START==mp3_play_sta())
			{
				mp3_dacPlay_Pause();
				audioPlayerStaShow(handle,0);
				winSetVisible(winItem(handle,AUDIOPLAYER_STA_ID),true);

				jpg_decode_buf(RES_AUDIOPLAY0_BK,audioplayer_bk_buf,audioplayer_bk_buf+bk_w*bk_h,bk_w,bk_h);
				XOSTimeDly(30);
				audioplay_showbuf();
			}
			else if(MP3_DAC_PAUSE==mp3_play_sta())
			{
				while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY){XOSTimeDly(10);}
				mp3_dacPlay_Start();
				audioPlayerStaShow(handle,1);
				winSetVisible(winItem(handle,AUDIOPLAYER_STA_ID),false);
				hal_dacSetVolume(configValue2Int(CONFIG_ID_VOLUME)*10);
			}
			else
			{
				audioplayer_play_mp3();
				hal_dacSetVolume(configValue2Int(CONFIG_ID_VOLUME)*10);
				audioPlayerStaShow(handle,1);
			}
		}
		audioPlayerTimeShow(handle);
	}
	return 0;
}
static int audioPlayerKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(audio_player_list.list_num>0)
		{
			if(audio_player_list.list_idx>0)
			{
				audio_player_list.list_idx--;
			}
			else
			{
				audio_player_list.list_idx=audio_player_list.list_num-1;
			}
			
		/*	if(audio_player_list.list_show_en)
			{
				audio_player_list.list_show_time=AUDIOPLAYER_LIST_SHOW_TIME;
				audioPlayerFileListShow(handle);
			}
			else*/
			{
				if((MP3_DAC_START==mp3_play_sta())||(MP3_DAC_PAUSE==mp3_play_sta()))
				{
					audioplayer_play_mp3();
					audioPlayerStaShow(handle,1);
					audioPlayerTimeShow(handle);
					audioPlayerFileListShow(handle);
					
					winSetVisible(winItem(handle,AUDIOPLAYER_STA_ID),FALSE);
				}
			}
		}
	}
	return 0;
}
static int audioPlayerKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		SysCtrl.volume_level++;
		if(SysCtrl.volume_level>8)
			SysCtrl.volume_level=8;

		audioPlayerVolShow(handle,SysCtrl.volume_level);

		configSet(CONFIG_ID_VOLUME,SysCtrl.volume_level+R_ID_STR_COM_LEVEL_0);

		hal_dacSetVolume(SysCtrl.volume_level*10);
		userConfigSave();

	}
	return 0;
}

static int audioPlayerKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		
		if(SysCtrl.volume_level>1)
		{
			SysCtrl.volume_level--;
			audioPlayerVolShow(handle,SysCtrl.volume_level);

			configSet(CONFIG_ID_VOLUME,SysCtrl.volume_level+R_ID_STR_COM_LEVEL_0);
			userConfigSave();
			hal_dacSetVolume(SysCtrl.volume_level*10);
		}
	}
	return 0;
}
static int audioPlayerKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		if(audio_player_list.list_num>0)
		{
			if(audio_player_list.list_idx<audio_player_list.list_num-1)
			{
				audio_player_list.list_idx++;
			}
			else
			{
				audio_player_list.list_idx=0;
			}

		/*	if(audio_player_list.list_show_en)
			{
				audio_player_list.list_show_time=AUDIOPLAYER_LIST_SHOW_TIME;
				audioPlayerFileListShow(handle);
			}
			else*/
			{
				
				if((MP3_DAC_START==mp3_play_sta())||(MP3_DAC_PAUSE==mp3_play_sta()))
				{
					audioplayer_play_mp3();
					audioPlayerStaShow(handle,1);
					audioPlayerTimeShow(handle);
					audioPlayerFileListShow(handle);
					winSetVisible(winItem(handle,AUDIOPLAYER_STA_ID),FALSE);
				}
			}
		}
	}
	return 0;
}

static int audioPlayerKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(MP3_DAC_STOP!=mp3_play_sta())
		{
			mp3_dacPlay_Stop();
			XOSTimeDly(20);
		}
		
		taskStart(TASK_MAIN_MENU,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}
	}
	return 0;
}

static int audioPlayerSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	audioPlayerSDShow(handle);

	winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR0),false);
	winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR1),false);
	winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR2),false);
	winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR3),false);
	winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR4),false);
	winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR5),false);

	if(audio_playing_flag!=0) audio_playing_flag=0;

	
	if((SDC_STAT_NULL==SysCtrl.sdcard)||(SDC_STAT_ERROR==SysCtrl.sdcard))
	{
		if(MP3_DAC_STOP!=mp3_play_sta())
		{
			mp3_dacPlay_Stop();
		}

		audio_player_list.list_num=0;
		audio_player_list.list_idx=0;
		audio_player_list.list_show_en=0;

		audio_player_list.play_time=0;
		audio_player_list.total_time=0;
		audioPlayerFileListShow(handle);
		audioPlayerStaShow(handle,0);
		audioPlayerTimeShow(handle);
	}
	else if(SDC_STAT_NORMAL==SysCtrl.sdcard)
	{
		if(NULL!=audio_player_list.file_list)
		{
			memset(audio_player_list.file_list,0,sizeof(struct AudioPlayerMp3NameList));
			audio_player_list.list_num=audioplayer_file_list_scan(FILEDIR_MP3);
			audio_player_list.list_idx=0;
			deg_Printf("audio_player_list.list_num=%d\n",audio_player_list.list_num);
			if(audio_player_list.list_num>0)
			{
				audio_player_list.list_show_en=1;
				audio_player_list.list_show_time=AUDIOPLAYER_LIST_SHOW_TIME;			
			}

			//==play mp3==
			audioplayer_play_mp3();
		
			//==osd show==
			audioPlayerSDShow(handle);
			audioPlayerBaterryShow(handle);
			if(MP3_DAC_START==mp3_play_sta())
			{
				audioPlayerStaShow(handle,1);
				
				audio_player_list.list_show_en=1;
				audioPlayerFileListShow(handle);
			}
			else
			{
				audioPlayerStaShow(handle,0);
				audio_player_list.list_show_en=0;
				audioPlayerFileListShow(handle);
			}
			
			audioPlayerTimeShow(handle);
			audioPlayerFileListShow(handle);

		}
	}

	if(MP3_DAC_START==mp3_play_sta())
	{
		jpg_decode_buf(RES_AUDIOPLAY0_BK,audioplayer_bk_buf,audioplayer_bk_buf+bk_w*bk_h,bk_w,bk_h);
		XOSTimeDly(30);
		audioplay_showbuf();
	}
	else
	{
		jpg_decode_buf(RES_AUDIOPLAY_BK,audioplayer_bk_buf,audioplayer_bk_buf+bk_w*bk_h,bk_w,bk_h);
		XOSTimeDly(30);
		audioplay_showbuf();
	}

	return 0;
}

void uni2oem936_init( void )
{
	int32 i,size;
	
	i = nv_open(RES_UNI2OEM936);
	if(i<0)
	{
		deg_Printf("open str error.\n");
	}
	size = nv_size(RES_UNI2OEM936);
	
	if(uni2oem936_data_buf == NULL)
	{
		uni2oem936_data_buf=hal_sysMemMalloc(size,32);
	}
	//deg_Printf("uni2oem936 size=%d.\n",size);
	
	nv_read(i,uni2oem936_data_buf,size);
}
void uni2oem936_uninit( void )
{
	if(NULL!=uni2oem936_data_buf)
	{
		hal_sysMemFree(uni2oem936_data_buf);
		uni2oem936_data_buf=NULL;
	}
}

void oem2uni936_init( void )
{
	int32 i,size;
	
	i = nv_open(RES_OEM2UNI936);
	if(i<0)
	{
		deg_Printf("open str error.\n");
	}
	size = nv_size(RES_OEM2UNI936);
	//deg_Printf("oem2uni936 size=%d.\n",size);

	if(oem2uni936_data_buf == NULL)
	{
		oem2uni936_data_buf=hal_sysMemMalloc(size,32);
	}
	nv_read(i,oem2uni936_data_buf,size);
}
void oem2uni936_uninit( void )
{
	if(NULL!=oem2uni936_data_buf)
	{
		hal_sysMemFree(oem2uni936_data_buf);
		oem2uni936_data_buf=NULL;
	}
}

static int audioPlayerOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("audioPlayer Open Win:%d\n",XOSTimeGet());
	u16 i,ret;

	audioPlayerProgressBar = progressBarCreate(Rx(20), Ry(220-4), Rw(280), Rh(6+4), INVALID_HANDLE, WIN_ABS_POS, 0);
	winSetfgColor(audioPlayerProgressBar, R_COLOR_WHITE);
	winSetbgColor(audioPlayerProgressBar, R_COLOR_GRAY3);
	progressBarSetDirction(0);

	
	memset(&audio_player_list,0,sizeof(struct AudioPlayerList));
	audio_player_list.cur_file=-1;

	//==mp3 font init==
	ret=MP3_unicode_font_init();
	if(0==ret)
	{
		SysCtrl.mp3_font_use=1;
	}
	deg_Printf("mp3 font:%d\n",ret);


	//==mp3 list name init==
	audio_player_list.file_list=hal_sysMemMalloc(sizeof(struct AudioPlayerMp3NameList),64);
	if(NULL==audio_player_list.file_list)
	{
		deg_Printf("mp3 list name  malloc fail\n");
	}
	else
	{
		memset(audio_player_list.file_list,0,sizeof(struct AudioPlayerMp3NameList));
	}

	if((SDC_STAT_NORMAL==SysCtrl.sdcard)||(SDC_STAT_FULL==SysCtrl.sdcard))	// play sd file  
	{
		audio_player_list.list_num=audioplayer_file_list_scan(FILEDIR_MP3);
		deg_Printf("audio_player_list.list_num=%d\n",audio_player_list.list_num);

		//==debug name==
		for(i=0;i<audio_player_list.list_num;i++)
			deg_Printf("list_name[%d]=%s\n",i,audio_player_list.file_list->name[i]);

		if(audio_player_list.list_num>0)
		{
			audio_player_list.list_show_en=1;
			audio_player_list.list_show_time=AUDIOPLAYER_LIST_SHOW_TIME;			
		}
	}

	//==osd show==
	audioPlayerSDShow(handle);
	audioPlayerBaterryShow(handle);
	if(MP3_DAC_START==mp3_play_sta())
	{
		audioPlayerStaShow(handle,1);
	}
	else
	{
		audioPlayerStaShow(handle,0);
	}

	audioPlayerTimeShow(handle);
	audioPlayerFileListShow(handle);


	//==show bk==
	hal_lcdGetResolution(&bk_w,&bk_h);
	bk_w=(bk_w + 0x1f) & (~0x1f);  // add 32bit alignment

	if(audioplayer_bk_buf == NULL)
	{
		audioplayer_bk_buf=hal_sysMemMalloc(bk_w*bk_h*3/2,32);
	}

	if(NULL!=audioplayer_bk_buf)
	{
		jpg_decode_buf(RES_AUDIOPLAY_BK,audioplayer_bk_buf,audioplayer_bk_buf+bk_w*bk_h,bk_w,bk_h);
	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}
	XOSTimeDly(30);
	audioplay_showbuf();

	SysCtrl.volume_level = configValue2Int(CONFIG_ID_VOLUME);
	audioPlayerVolShow(handle,SysCtrl.volume_level);
	


	if(0==SysCtrl.cartoon_mode)
	{
		SysCtrl.cartoon_mode=1;
		SysCtrl.cartoon_show_cnt=0;
	}
	//==play mp3==
	audioplayer_play_mp3();


	winSetVisible(winItem(handle,AUDIOPLAYER_STA_ID),false);

	if(MP3_DAC_START!=mp3_play_sta())
	{
		audio_player_list.list_show_en=0;
		audioPlayerFileListShow(handle);
	}

	audio_play_state=0;

	winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR0),false);
	winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR1),false);
	winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR2),false);
	winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR3),false);
	winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR4),false);
	winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR5),false);

	if(MP3_DAC_START==mp3_play_sta())
	{
		jpg_decode_buf(RES_AUDIOPLAY0_BK,audioplayer_bk_buf,audioplayer_bk_buf+bk_w*bk_h,bk_w,bk_h);
		XOSTimeDly(30);
		audioplay_showbuf();
	}

	return 0;
}

static int audioPlayerCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("audioPlayer Close Win!!!\n");
	if(NULL!=audio_player_list.file_list)
	{
		hal_sysMemFree(audio_player_list.file_list);
		audio_player_list.file_list=NULL;
	}

	/*if(NULL!=audio_demo_buf) 
	{
		hal_sysMemFree(audio_demo_buf);
		audio_demo_buf=NULL;
	}*/

	if(NULL!=audioplayer_bk_buf) 
	{
		hal_sysMemFree(audioplayer_bk_buf);
		audioplayer_bk_buf=NULL;
	}

	if(audio_player_list.cur_file>=0)
	{
		close(audio_player_list.cur_file);
		audio_player_list.cur_file=-1;
	}

	if(SysCtrl.mp3_font_use)
	{
		SysCtrl.mp3_font_use=0;
		MP3_unicode_font_uninit();
	}

	uni2oem936_uninit();
	oem2uni936_uninit();

	return 0;
}

static void audioplay_showbuf()	 
{
	disp_frame_t *p_lcd_buffer;

	p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(p_lcd_buffer)
	{
		u16 lcd_w,lcd_h;
		hal_lcdGetResolution(&lcd_w,&lcd_h);
		//deg_Printf("video:w=%d,h=%d,bufw=%d,bufh=%d,\n",lcd_w,lcd_h,p_lcd_buffer->w,p_lcd_buffer->h);
		hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);

		memcpy(p_lcd_buffer->y_addr,audioplayer_bk_buf,bk_w*bk_h*3/2);
		ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,bk_w*bk_h*3/2);
		dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
	}
	else
	{
		deg_Printf("video buf null!\n");
	}
}


static int audioPlayerSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{


	if(MP3_DAC_START==mp3_play_sta())
	{

		audioPlayerTimeShow(handle);
	}


	audioPlayerBarShow(handle);

	return 0;
}

static int audioPlayerSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	//if(SysCtrl.usb == USB_STAT_DCIN)
	{
		audioPlayerBaterryShow(handle);	
	}

#if 0
	if(audio_player_list.list_show_en)
	{
		if(audio_player_list.list_show_time>0)
		{
			audio_player_list.list_show_time--;
			if(0==audio_player_list.list_show_time)
			{
				audio_player_list.list_show_en=0;
				audio_player_list.list_idx=audio_player_list.list_play_idx;
				audioPlayerFileListShow(handle);
			}
		}
	}
#endif

	return 0;
}

static int audioPlayerSysMsgPlayFinish(winHandle handle,uint32 parameNum,uint32* parame)
{
	u8 ret=0;
	if(parameNum==1)
		ret=parame[0];
	
	deg_Printf("audioPlayerSysMsgPlayFinish:%d,%d\n",parameNum,ret);
	if(ret)
	{
		if(audio_player_list.list_num>0)
		{
			if(audio_player_list.list_idx<audio_player_list.list_num-1)
			{
				audio_player_list.list_idx++;
			}
			else
			{
				audio_player_list.list_idx=0;
			}

			audioplayer_play_mp3();
			audioPlayerStaShow(handle,1);
		}
		else
		{
			audioPlayerTimeShow(handle);
			audioPlayerStaShow(handle,0);
		}
	}

	if(winIsVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR0))) winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR0),false);
	if(winIsVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR1))) winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR1),false);
	if(winIsVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR2))) winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR2),false);
	if(winIsVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR3))) winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR3),false);
	if(winIsVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR4))) winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR4),false);
	if(winIsVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR5))) winSetVisible(winItem(handle,AUDIOPLAYBACK_PROGRESSBAR5),false);

	if(audio_playing_flag!=0) audio_playing_flag=0;

	
	return 0;
}


msgDealInfor audioPlayerMsgDeal[]=
{
	{SYS_OPEN_WINDOW,audioPlayerOpenWin},
	{SYS_CLOSE_WINDOW,audioPlayerCloseWin},
	{KEY_EVENT_UP,audioPlayerKeyMsgUp},
	{KEY_EVENT_DOWN,audioPlayerKeyMsgDown},
	{KEY_EVENT_MENU,audioPlayerKeyMsgMenu},
	{KEY_EVENT_MODE,audioPlayerKeyMsgMode},
	{KEY_EVENT_OK,audioPlayerKeyMsgOk},
	{KEY_EVENT_POWER,audioPlayerKeyMsgExit},
	{SYS_EVENT_500MS,audioPlayerSysMsg500MS},
	{SYS_EVENT_1S,audioPlayerSysMsg1S},
	{SYS_EVENT_SDC,audioPlayerSysMsgSD},
	{MP3_EVENT_PLAY_FINISH,audioPlayerSysMsgPlayFinish},
	
	{EVENT_MAX,NULL},
};

WINDOW(audioPlayerWindow,audioPlayerMsgDeal,audioPlayerWin)



