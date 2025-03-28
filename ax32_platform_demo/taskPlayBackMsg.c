#include "application.h"
#include "taskPlayBackRes.c"
#include "userInterface.h"
#define  VIDEO_PLAY_WAIT_KEYSOUND_END           while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY){XOSTimeDly(10);}




extern int video_playback_start(int index);
static int playBackKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	//int ret;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		if(managerFileCount(SysCtrl.avi_list)>=1)
		{
			if(FILE_TYPE_JPG == SysCtrl.file_type)	// scale up
			{
				deamon_keysound_play();
				VIDEO_PLAY_WAIT_KEYSOUND_END;	
			}
			else
			{
				if(videoPlaybackGetStatus()== MEDIA_STAT_START)
				{
					videoPlaybackPuase();
					while(videoPlaybackGetStatus()==MEDIA_STAT_START);
					playbackStateShow(handle,8);
				}
				else if(videoPlaybackGetStatus() == MEDIA_STAT_PAUSE)
				{
					videoPlaybackResume();
					playbackStateShow(handle,2);
					winSetVisible(winItem(handle,PLAYBACK_PLAY_TIME_ID),true);
					winSetVisible(winItem(handle,PLAYBACK_TOTAL_TIME_ID),true);
					winSetVisible(VideoPlayerProgressBar,true);
				}
			}
		}
	}
	return 0;
}

static int playBackKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoPlaybackGetStatus()== MEDIA_STAT_START)
		{
			videoPlaybackPuase();
			XOSTimeDly(50);
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

static int playBackKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

		winSetVisible(winItem(handle,PLAYBACK_PLAY_TIME_ID),false);
		winSetVisible(winItem(handle,PLAYBACK_TOTAL_TIME_ID),false);
		winSetVisible(VideoPlayerProgressBar,false);
		
		if(NULL==SysCtrl.sdcard)
		{
			if(spi_udisk_jpg_num()>0)
			{
				deamon_keysound_play();
				SysCtrl.file_index--;
				if(SysCtrl.file_index<0)
					SysCtrl.file_index = spi_udisk_jpg_num()-1;

				video_playback_start(SysCtrl.file_index);
				playbackResolutionShow(handle);
				playbackFileNameShow(handle);
				playbackLockShow(handle);
				playbackStateShow(handle,2);
			}
		}
		else
		{
			if(managerFileCount(SysCtrl.avi_list)>=1)
			{
				if(MEDIA_STAT_PLAY==videoPlaybackGetStatus())
				{
					//videoPlaybackFastForward();
					//playbackStateShow(handle,videoPlaybackGetplayspeed());
				}
				else
				{
					/*
					if(videoPlaybackGetStatus()!= MEDIA_STAT_STOP)
					{
		                  videoPlaybackStop();
					}
					*/
					deamon_keysound_play();
					VIDEO_PLAY_WAIT_KEYSOUND_END;
					SysCtrl.file_index--;
					if(SysCtrl.file_index<1)
						SysCtrl.file_index = managerFileCount(SysCtrl.avi_list);

					video_playback_start(SysCtrl.file_index);
					playbackResolutionShow(handle);
					playbackFileNameShow(handle);
					playbackLockShow(handle);
					if(FILE_TYPE_JPG==SysCtrl.file_type)
					{
						playbackStateShow(handle,2);
					}
					else
					{
						if(videoPlaybackGetStatus() == MEDIA_STAT_PAUSE)
						{
							playbackStateShow(handle,8);
						}
					}
				}
			}
		}
	}
	return 0;
}
static int playBackKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		winSetVisible(winItem(handle,PLAYBACK_PLAY_TIME_ID),false);
		winSetVisible(winItem(handle,PLAYBACK_TOTAL_TIME_ID),false);
		winSetVisible(VideoPlayerProgressBar,false);

		if(NULL==SysCtrl.sdcard)
		{
			if(spi_udisk_jpg_num()>0)
			{
				deamon_keysound_play();
				SysCtrl.file_index++;
				if(SysCtrl.file_index>=spi_udisk_jpg_num())
					SysCtrl.file_index = 0;

				video_playback_start(SysCtrl.file_index);
				playbackResolutionShow(handle);
				playbackFileNameShow(handle);
				playbackLockShow(handle);
				playbackStateShow(handle,2);
			}
		}
		else
		{
			if(managerFileCount(SysCtrl.avi_list)>=1)
			{
				if(MEDIA_STAT_PLAY==videoPlaybackGetStatus())
				{
					//videoPlaybackFastBackward();
					//playbackStateShow(handle,videoPlaybackGetplayspeed());
				}
				else
				{
					/*
					if(videoPlaybackGetStatus() != MEDIA_STAT_STOP)
					{
		                  videoPlaybackStop();
					}
					*/
					deamon_keysound_play();
					VIDEO_PLAY_WAIT_KEYSOUND_END;
					SysCtrl.file_index++;
					if(SysCtrl.file_index>managerFileCount(SysCtrl.avi_list))
						SysCtrl.file_index = 1;

					video_playback_start(SysCtrl.file_index);
					playbackResolutionShow(handle);
					playbackFileNameShow(handle);
					playbackLockShow(handle);
					if(FILE_TYPE_JPG==SysCtrl.file_type)
					{
						playbackStateShow(handle,2);
					}
					else
					{
						if(videoPlaybackGetStatus() == MEDIA_STAT_PAUSE)
						{
							playbackStateShow(handle,8);
						}
					}
				}
			}
		}
	}
	return 0;
}
extern menu MENU(playBack);
static int playBackKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	int ret;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(NULL==SysCtrl.sdcard)
		{
			if(spi_udisk_jpg_num()>0)
			{
				videoPlaybackStop();
				deamon_keysound_play();
				VIDEO_PLAY_WAIT_KEYSOUND_END;
				uiOpenWindow(&delCurWindow,0);
			}
		}
		else
		{
			if((videoPlaybackGetStatus() != MEDIA_STAT_START)&&(managerFileCount(SysCtrl.avi_list)>0))
			{
				videoPlaybackStop();
				deamon_keysound_play();
				VIDEO_PLAY_WAIT_KEYSOUND_END;
				uiOpenWindow(&delCurWindow,0);
			}
		}
	}
	return 0;
}
static int playBackKeyMsgMenu2(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	int ret;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(NULL==SysCtrl.sdcard)
		{
			if(spi_udisk_jpg_num()>0)
			{
				videoPlaybackStop();
				deamon_keysound_play();
				VIDEO_PLAY_WAIT_KEYSOUND_END;
				uiOpenWindow(&delAllWindow,0);
			}
		}
		else
		{
			if((videoPlaybackGetStatus() != MEDIA_STAT_START)&&(managerFileCount(SysCtrl.avi_list)>0))
			{
				videoPlaybackStop();
				deamon_keysound_play();
				VIDEO_PLAY_WAIT_KEYSOUND_END;
				uiOpenWindow(&delAllWindow,0);
			}
		}
	}
	return 0;
}


static int playBackSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("player : sdc stat ->%d\n",SysCtrl.sdcard);

	 videoPlaybackStop();
	 playbackSDShow(handle);
	 
	if((SysCtrl.sdcard != SDC_STAT_NORMAL)/* && (state == MEDIA_STAT_PLAY)*/) // sdc out when recording
	{
		//if(videoPlaybackGetStatus() == MEDIA_STAT_PLAY)
		{
			// videoPlaybackStop();
			winSetVisible(winItem(handle,PLAYBACK_PLAY_TIME_ID),false);
			winSetVisible(winItem(handle,PLAYBACK_TOTAL_TIME_ID),false);
			winSetVisible(VideoPlayerProgressBar,false);
		}
		  
		
		if(spi_udisk_jpg_num()>0)
		{
			SysCtrl.file_index = 0;
			video_playback_start(SysCtrl.file_index); 
			playbackResolutionShow(handle);
			playbackFileNameShow(handle);
			playbackStateShow(handle,2);
		}
		else
		{
			uiOpenWindow(&noFileWindow,0);
			
		}
	}
	else
	{
		SysCtrl.file_index = managerFileCount(SysCtrl.avi_list);
		if(SysCtrl.file_index>0)
		{
			video_playback_start(SysCtrl.file_index);
			playbackResolutionShow(handle);
			playbackFileNameShow(handle);

			if(FILE_TYPE_JPG==SysCtrl.file_type)
			{
				playbackStateShow(handle,2);
			}
			else
			{
				if(videoPlaybackGetStatus() == MEDIA_STAT_PAUSE)
				{
					playbackStateShow(handle,8);
				}
			}
		}
		else
		{
			uiOpenWindow(&noFileWindow,0);
		}
	}

	return 0;
}

static int playBackSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{	
	playbackBaterryShow(handle);
	return 0;
}
static int playBackSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int playBackSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

void backBrushBlack(u8 brightness)
{
	disp_frame_t *show_buf = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(show_buf)
	{
		u16 lcd_w,lcd_h;
		hal_lcdGetResolution(&lcd_w,&lcd_h);
		lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
		hal_dispframeVideoCfg(show_buf,0,0,lcd_w,lcd_h);

		memset(show_buf->y_addr,brightness,show_buf->w*show_buf->h);
		memset(show_buf->uv_addr,0x80,show_buf->w*show_buf->h/2);

		ax32xx_sysDcacheFlush((u32)show_buf->y_addr,show_buf->w*show_buf->h*3/2);
		dispLayerShow(DISP_LAYER_VIDEO,(INT32U)show_buf,0,0,VIDEO_ROTATE);
		XOSTimeDly(5);
	}
}

static int playBackOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("playBack Open Win!!!\n");
	winSetName(handle,"play back");
	winSetResid(winItem(handle,PLAYBACK_MODE_ID),R_ID_ICON_MTPLAY);

	VideoPlayerProgressBar = progressBarCreate(Rx(85), Ry(40), Rw(150), Rh(8), INVALID_HANDLE, WIN_ABS_POS, 0);
	winSetfgColor(VideoPlayerProgressBar, R_COLOR_BLUE);
	winSetbgColor(VideoPlayerProgressBar, R_COLOR_GRAY3);
	progressBarSetDirction(0);

	winSetVisible(VideoPlayerProgressBar,false);
	winSetVisible(winItem(handle,PLAYBACK_PLAY_TIME_ID),false);
	winSetVisible(winItem(handle,PLAYBACK_TOTAL_TIME_ID),false);

//	#if SMALL_PANEL_SUPPORT==0
//	playbackPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif
//	playbackPlayPrint_num(handle,false);

	playbackResolutionShow(handle);
	playbackFileNameShow(handle);
	playbackLockShow(handle);
	playbackMDShow(handle);
	playbackMonitorShow(handle);
	playbackIrLedShow(handle);
	playbackSDShow(handle);
	playbackMicShow(handle);
	playbackBaterryShow(handle);
	
	if(SysCtrl.file_type==FILE_TYPE_JPG)
	{
		playbackStateShow(handle,2);
	}
	

	if(NULL==SysCtrl.sdcard)
	{
		if(0==spi_udisk_jpg_num())
		{
			uiOpenWindow(&noFileWindow,0);
		}
	}
	else
	{
		if(managerFileCount(SysCtrl.avi_list)<1)
		{
			uiOpenWindow(&noFileWindow,0);
		}
	}

	if(0==SysCtrl.cartoon_mode)
	{
		SysCtrl.cartoon_mode=1;
		SysCtrl.cartoon_show_cnt=0;
	}
	
	return 0;
}
static int playBackCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("playBack Close Win!!!\n");
	return 0;
}
static int playBackWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("playBack WinChild Close!!!\n");

#if 0
	if(NULL==SysCtrl.sdcard)
	{
		if(0==spi_udisk_jpg_num())
		{
			uiOpenWindow(&noFileWindow,0);
		}
		else
		{
			video_playback_start(SysCtrl.file_index); 
		}
	}
	else
	{
		if(managerFileCount(SysCtrl.avi_list)<1)
		{
			uiOpenWindow(&noFileWindow,0);
			return 0;
		}
		else
		{
			video_playback_start(SysCtrl.file_index); 
		}
	}
#endif

#if 1
	u16 lcd_w,lcd_h;
	disp_frame_t *play_bk_buf = NULL;
	bool exist_file = false;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	
	play_bk_buf = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(play_bk_buf)
	{
		hal_dispframeVideoCfg(play_bk_buf,0,0,lcd_w,lcd_h);
	}
	else
	{
		deg_Printf("video buf null!\n");
	}
	
	if(NULL==SysCtrl.sdcard)
	{
		if(0==spi_udisk_jpg_num())
		{
			exist_file = false;
		}
		else
		{
			exist_file = true;
		}
	}
	else
	{
		if(managerFileCount(SysCtrl.avi_list)<1)
		{
			exist_file = false;
		}
		else
		{
			exist_file = true;
		}
	}

	if(exist_file == false)
	{
		memset(play_bk_buf->y_addr,80,lcd_w*lcd_h);
		memset(play_bk_buf->uv_addr, 0x80,lcd_w*lcd_h/2);
	}

	disp_frame_t *p_lcd_buffer;
	p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(p_lcd_buffer)
	{
		hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);
		memcpy(p_lcd_buffer->y_addr,play_bk_buf->y_addr,lcd_w*lcd_h*3/2);
		ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,lcd_w*lcd_h*3/2);
		dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
	}

	if(play_bk_buf)
		dispLayerFreeFrame(DISP_LAYER_VIDEO,play_bk_buf);

	if(exist_file == false)
	{
		uiOpenWindow(&noFileWindow,0);
		return 0;
	}
	else
	{
		video_playback_start(SysCtrl.file_index); 
	}
#endif	

	playbackResolutionShow(handle);
	playbackFileNameShow(handle);
	playbackLockShow(handle);
	playbackMDShow(handle);
	playbackMonitorShow(handle);
	playbackIrLedShow(handle);
	playbackSDShow(handle);
	playbackMicShow(handle);
	playbackBaterryShow(handle);	
	playbackStateShow(handle,2);
	return 0;
}
static int playBackSysMsgTimeUpdate(winHandle handle,uint32 parameNum,uint32* parame)
{
	INT32 playtime=0,totaltime = 0;
	videoPlaybackGetTime((u32*)&totaltime,(u32*)&playtime);
	if(playtime%1000 >500)
		playtime=playtime/1000;
	else
		playtime=playtime/1000;
	if(totaltime%1000 >500)
		totaltime = totaltime/1000 + 1;
	else 
		totaltime = totaltime/1000;
	playbackPlayTimeShow(handle,playtime,totaltime);
	return 0;
}

static int playBackSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
//	#if SMALL_PANEL_SUPPORT==0
//	playbackPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif

	if(FILE_TYPE_AVI==SysCtrl.file_type)
	{
		if(videoPlaybackGetStatus()==MEDIA_STAT_PAUSE)
		{
			if( winIsVisible(winItem(handle,PLAYBACK_PLAY_TIME_ID)))
			{
				playbackStateShow(handle,8);
				winSetVisible(winItem(handle,PLAYBACK_PLAY_TIME_ID),false);
				winSetVisible(winItem(handle,PLAYBACK_TOTAL_TIME_ID),false);
				winSetVisible(VideoPlayerProgressBar,false);
				playbackFileNameShow(handle);
			}
			
		}
	}

	//if(SysCtrl.usb == USB_STAT_DCIN)
	{
		playbackBaterryShow(handle);	
	}

	return 0;
}


msgDealInfor playBackMsgDeal[]=
{
	{SYS_OPEN_WINDOW,playBackOpenWin},
	{SYS_CLOSE_WINDOW,playBackCloseWin},
	{SYS_CHILE_COLSE,playBackWinChildClose},
	{KEY_EVENT_OK,playBackKeyMsgOk},
	{KEY_EVENT_POWER,playBackKeyMsgExit},
	{KEY_EVENT_MODE,playBackKeyMsgUp},
	{KEY_EVENT_MENU,playBackKeyMsgDown},
	{KEY_EVENT_UP,playBackKeyMsgMenu},
	{KEY_EVENT_DOWN,playBackKeyMsgMenu2},

	//{KEY_EVENT_POWER,playBackKeyMsgMenu},
//	{KEY_EVENT_PLAYBACK,playBackKeyMsgPlayBack},
//	{KEY_EVENT_PHOTO,playBackKeyMsgPhoto},
//	{KEY_EVENT_RECORD,playBackKeyMsgVideo},
	{SYS_EVENT_SDC,playBackSysMsgSD},
	{SYS_EVENT_USB,playBackSysMsgUSB},
	{SYS_EVENT_BAT,playBackSysMsgBattery},
	{SYS_EVENT_MD,playBackSysMsgMD},
	{SYS_EVENT_1S,playBackSysMsg1S},
	{SYS_EVENT_TIME_UPDATE,playBackSysMsgTimeUpdate},
	{EVENT_MAX,NULL},
};

WINDOW(playBackWindow,playBackMsgDeal,playBackWin)


