#include "application.h"
#include "taskVideoRecordRes.c"
#include "userInterface.h"


static uint32 mdRecTime=0;
static uiRect recordTimeRect;
static bool curWinIsVideo=false;
static bool firstPowerOnRec=true;

u32 video_focus_show_count = 0;
static u8 *focus_sound = 0;
static s32 focus_sound_size = 0;

void videoFocusIconCancel(winHandle handle)
{
	if(SysCtrl.video_focus == VIDEO_FOCUS_ICON_RED)
	{
		if(XOSTimeGet() - video_focus_show_count > 1000)
		{
			SysCtrl.video_focus=VIDEO_FOCUS_ICON_NONE;
			videoFocusShow(handle);
			videoZoomRatioShow(handle);
		}
	}
}



static int videoKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deg_Printf("ok key\n");
	}
	return 0;
}
/*
static int videoKeyMsgIr(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		u32 val=0;
		boardIoctrl(SysCtrl.bfd_ir,IOGET_IR_GET,&val);
		deg_Printf("ir:%d\n",val);
		if(val)
		{
			boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,0);
		}
		else
		{
			boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,1);
		}
	}
	return 0;
}
*/
static int videoKeyMsgRecord(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

		deg_Printf("record key\n");
		if(task_video_Get_Status() == MEDIA_STAT_START)
		{
			
			if(winIsVisible(winItem(handle,VIDEO_STATE_ID)))
			{
				winSetVisible(winItem(handle,VIDEO_STATE_ID),false);
			}
			
			task_record_stop();
#if 1
			SysCtrl.photo_task=1;
			UserInterface captureAnimation;
			
			ANIMATION(captureAnimation, LEFTBOTTOM2RIGHTUPPER)
			photo_animation_effect(captureAnimation,1);

			SysCtrl.photo_task=0;
#endif
		}
		else
		{
			if(SysCtrl.videorec_mode == VIDEOREC_SLOW)
			{
				videoRecordCmdSet(CMD_COM_FPS,10);
			}
			else
			{
				videoRecordCmdSet(CMD_COM_FPS,30);
			}

			if(SysCtrl.videorec_mode == VIDEOREC_FAST)
			{
				videoRecordCmdSet(CMD_COM_VIDEO_FAST_EN,1);
			}
			else
			{
				videoRecordCmdSet(CMD_COM_VIDEO_FAST_EN,0);
			}
			SysCtrl.t_rec_fast=0;
			video_record_start();
		
		}
	}
	return 0;
}

static int videoKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		taskStart(TASK_MAIN_MENU,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}
	}
	return 0;
}
/*
static int videoKeyMsgSensorChange(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(task_video_Get_Status() == MEDIA_STAT_STOP)
		{
			sensor_change();
		}
	}
	return 0;
}
*/
extern void board_lcd_ioctrl_pwm(uint8 data );
uint8 rotate_flag=0;
static int videoKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{

		if(SysCtrl.video_focus == VIDEO_FOCUS_ICON_YELLOW)
		{
			video_focus_show_count = XOSTimeGet();
			SysCtrl.video_focus = VIDEO_FOCUS_ICON_RED;
			videoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}

		if(XOSTimeGet()-video_focus_show_count<500)
		{
			return 0;
		}

	
		if(task_video_Get_Status() != MEDIA_STAT_START)
		{
			if(SysCtrl.spec_color_index>0)
			{
				SysCtrl.spec_color_index--;
			}
			else
			{
				SysCtrl.spec_color_index=5;
			}
			cmos_spec_color(SysCtrl.spec_color_index);
		}
	}
	else if(keyState==KEY_CONTINUE)
	{
		//if(0==SysCtrl.photo_mode_switch)
		if(SysCtrl.videorec_mode == VIDEOREC_NORMAL&&task_video_Get_Status() == MEDIA_STAT_STOP)
		{
			SysCtrl.crop_level+=1;
			if(SysCtrl.crop_level > 30)
				SysCtrl.crop_level=30;
			dispLayerSetFrontCrop(SysCtrl.crop_level);
			videoZoomRatioShow(handle);
			SysCtrl.video_focus = VIDEO_FOCUS_ICON_YELLOW;
			videoFocusShow(handle);
		}
		video_focus_show_count=XOSTimeGet();
	}

	return 0;
}
uint8 sensor_flag=0;
static int videoKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_RELEASE)
	{
		if(SysCtrl.video_focus == VIDEO_FOCUS_ICON_YELLOW)
		{
			video_focus_show_count = XOSTimeGet();
			SysCtrl.video_focus = VIDEO_FOCUS_ICON_RED;
			videoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}

		if(XOSTimeGet()-video_focus_show_count<500)
		{
			return 0;
		}
	
		if(task_video_Get_Status() != MEDIA_STAT_START)
		{
			SysCtrl.spec_color_index++;
			if(SysCtrl.spec_color_index>5)
			{
				SysCtrl.spec_color_index=0;
			}
		cmos_spec_color(SysCtrl.spec_color_index);
		}
	}
	else if(keyState==KEY_CONTINUE)
	{
		//if(0==SysCtrl.photo_mode_switch)
		if(SysCtrl.videorec_mode == VIDEOREC_NORMAL&&task_video_Get_Status() == MEDIA_STAT_STOP)
		{
			SysCtrl.crop_level-=1;
			if(SysCtrl.crop_level > 200)
				SysCtrl.crop_level=0;
			dispLayerSetFrontCrop(SysCtrl.crop_level);
			videoZoomRatioShow(handle);
 			SysCtrl.video_focus = VIDEO_FOCUS_ICON_YELLOW;
			videoFocusShow(handle);
		}
		video_focus_show_count=XOSTimeGet();
	}


	
	return 0;
}
static int videoKeyMsgLeft(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_RELEASE)
	{
		if(task_video_Get_Status() != MEDIA_STAT_START)
		{
			SysCtrl.videorec_mode++;
			if( SysCtrl.videorec_mode > VIDEOREC_FAST)
			{
				SysCtrl.videorec_mode=VIDEOREC_NORMAL;
			}

			if( SysCtrl.videorec_mode==VIDEOREC_SLOW )
			{
				winSetResid(winItem(handle,VIDEO_REC_MODE_ID),R_ID_STR_COM_VIDEOREC_SLOW);
				SysCtrl.crop_level=0;
				dispLayerSetFrontCrop(SysCtrl.crop_level);
				videoZoomRatioShow(handle);
				if(!winIsVisible(winItem(handle,VIDEO_REC_MODE_ID)))
				{
					winSetVisible(winItem(handle,VIDEO_REC_MODE_ID),true);
				}
			}
			else if( SysCtrl.videorec_mode==VIDEOREC_FAST )
			{
				winSetResid(winItem(handle,VIDEO_REC_MODE_ID),R_ID_STR_COM_VIDEOREC_FAST);
				SysCtrl.crop_level=0;
				dispLayerSetFrontCrop(SysCtrl.crop_level);
				videoZoomRatioShow(handle);
				if(!winIsVisible(winItem(handle,VIDEO_REC_MODE_ID)))
				{
					winSetVisible(winItem(handle,VIDEO_REC_MODE_ID),true);
				}
			}
			else
			{
				if(winIsVisible(winItem(handle,VIDEO_REC_MODE_ID)))
				{
					winSetVisible(winItem(handle,VIDEO_REC_MODE_ID),false);
				}
			}
		}
	}
	return 0;
}
static int videoKeyMsgRight(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint8 flag=0;
	uint16 lcd_w,lcd_h;

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_RELEASE)
	{
		if(task_video_Get_Status() != MEDIA_STAT_START)
		{
			sensorChange();
#if 1
			{
				disp_frame_t * handle_frame;
				UserInterface recordeAnimation;
				ANIMATION(recordeAnimation, SQUARE_INSIDE2OUTSIDE)

				SysCtrl.photo_task=1;
				bool change_finir = false;
				while(1)
				{
					if(0==SysCtrl.photo_software_handle)
					{
						handle_frame=yuv420_software_get_handle_frame();
						if(handle_frame)
							SysCtrl.photo_software_handle=1;
					}
					else if(1==SysCtrl.photo_software_handle)
					{
						SysCtrl.photo_software_handle=2;
						if(!flag)
						{
							flag=1;
							hal_lcdGetResolution(&lcd_w,&lcd_h);
							lcd_w=(lcd_w + 0x1f) & (~0x1f); // add 32bit align
							memcpy(recordeAnimation.preFrameBuf,handle_frame->y_addr,lcd_w*lcd_h*3/2);
							ax32xx_sysDcacheFlush((u32)recordeAnimation.preFrameBuf,lcd_w*lcd_h*3/2);
						}
						change_finir = recordeAnimation.run(&recordeAnimation, handle_frame->y_addr);
						hal_lcdDealBuffer(handle_frame);
						hal_lcd_get_next_frame();
						if(change_finir == true)
						break;
					}
				}
				SysCtrl.photo_task=0;
			}
#endif
			
		}
	}
	return 0;
}

/*
extern MENU(movie);
static int videoKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(task_video_Get_Status() != MEDIA_STAT_START)
		{
			uiOpenWindow(&menuItemWindow,1,&MENU(movie));
		}
		else
		{
			//winSetVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID),true);
			//videoCapturePhoto();
		}
	}
	return 0;
}


static int videoKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(task_video_Get_Status() != MEDIA_STAT_START)
				taskStart(TASK_PHOTO_ENCODE,0);
	}
	return 0;
}
*/

static int videoSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.sdcard != SDC_STAT_NORMAL && task_video_Get_Status() == MEDIA_STAT_START) // sdc out when recording
	{
		task_record_stop();
		if(SysCtrl.pip_mode != DISP_PIP_BACK)
		{
			if(SysCtrl.battery > BATTERY_STAT_2)
				setSoftwareDetachUsensor(false);
		}
	}
	videoRemainTimeShow(handle, SysCtrl.t_remain);
	videoSDShow(handle);
	if(SysCtrl.sdcard == SDC_STAT_NULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_NOT_INSERT,SDC_NULL_TIMEOUT);
	else if(SysCtrl.sdcard == SDC_STAT_FULL)
	{
		winSetVisible(winItem(handle,VIDEO_STATE_ID),false);
		uiOpenWindow(&tips1Window,2,TIPS_SD_FULL,SDC_NULL_TIMEOUT);
	}
		
	else if(SysCtrl.sdcard == SDC_STAT_ERROR)
		uiOpenWindow(&tips1Window,2,TIPS_SD_ERROR,SDC_NULL_TIMEOUT);
	deg_Printf("SD state:%d\n",SysCtrl.sdcard);
	return 0;
}

static int videoSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	videoBaterryShow(handle);
	return 0;
}

static int videoSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		videoBaterryShow(handle);

	return 0;
}
/*
static int videoSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(task_video_Get_Status() != MEDIA_STAT_START)
	{
		
		if((video_record_start()>=0)&&(SysCtrl.pip_mode != DISP_PIP_BACK))
			setSoftwareDetachUsensor(true);
	}
	mdRecTime=XOSTimeGet();
	return 0;
}
*/
static int videoSysMsgRestart(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("!!!\n");
	if(task_video_Get_Status() != MEDIA_STAT_START)
	{
		if((video_record_start()>=0)&&(SysCtrl.pip_mode != DISP_PIP_BACK))
			setSoftwareDetachUsensor(true);
	}
	return 0;
}
extern uint32 videoRecordGetTimeSec(void);
extern void drawUIService1(void);
extern XWork_T *  recordIncrease1S;
void recordTimeCount1S(void)
{
	static char recTimeStr[]="00:00";
	uint32 sec,frameCntTime;
	if(task_video_Get_Status() == MEDIA_STAT_START)
	{
//		if(SysCtrl.pip_mode == DISP_PIP_BACK )
//			frameCntTime=videoRecordGetTimeSecB();
//		else
			frameCntTime=videoRecordGetTimeSec();
		if(SysCtrl.t_rec<frameCntTime)
		{
			if(recordIncrease1S)
				recordIncrease1S->reload=900*X_TICK;
		}
		else if(SysCtrl.t_rec>frameCntTime)
		{
			if(recordIncrease1S)
				recordIncrease1S->reload=1100*X_TICK;
		}
		else
		{
			if(recordIncrease1S)
				recordIncrease1S->reload=1000*X_TICK;
		}
		sec=SysCtrl.t_rec;
		if( SysCtrl.videorec_mode==VIDEOREC_FAST )
		{
			SysCtrl.t_rec_fast++;
			if(SysCtrl.t_rec_fast>=3)
			{
				SysCtrl.t_rec_fast=0;
				SysCtrl.t_rec++;
			}
		}
		else
		{
			SysCtrl.t_rec++;
		}
		
		if(SysCtrl.t_rec>SysCtrl.t_mrec)
			SysCtrl.t_rec=0;
		if(curWinIsVideo)
		{
			sec=sec%3600;
			recTimeStr[0] = sec/600+'0';  // m
			recTimeStr[1] = ((sec/60)%10)+'0';	 // m
			recTimeStr[2] = ':';
			sec=sec%60;
			recTimeStr[3] = sec/10+'0';  // s
			recTimeStr[4] = sec%10+'0';   // s
			recTimeStr[5] = 0;
			//deg_Printf("rec:%s\n",recTimeStr);
			drawString(&recordTimeRect,&recordTimeRect,strToResId(recTimeStr),R_FONT_DEFAULT,ALIGNMENT_LEFT,R_COLOR_RED,R_COLOR_TRANSFER,R_COLOR_TRANSFER);
			drawUIService1();
		}
	}
}

static int videoOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("video Open Win!!!%d %d\n",SysCtrl.sdcard,SysCtrl.usb);
	layout_version_get();
	winSetResid(winItem(handle,VIDEO_MODE_ID),R_ID_ICON_MTRECORD);
	videoRemainTimeShow(handle, SysCtrl.t_remain);
//	#if SMALL_PANEL_SUPPORT==0
//	videoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif
	videoResolutionShow(handle);
	videoMDShow(handle);
	videoMonitorShow(handle);
	videoIrLedShow(handle);
	videoLockShow(handle);
	videoSDShow(handle);
	videoMicShow(handle);
	videoBaterryShow(handle);
//	winSetVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID),false);
#if 0
	if(firstPowerOnRec)
	{
		if(SysCtrl.sdcard == SDC_STAT_NORMAL&&SysCtrl.battery > BATTERY_STAT_2)
		{
			deg_Printf("power on,start record\n");
			if(SysCtrl.usensor==USENSOR_STAT_NULL||(SysCtrl.usensor!=USENSOR_STAT_NULL&&api_backrec_status()))
			{
				firstPowerOnRec=false;
				XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_ON,0));
			}
		}
		else
			firstPowerOnRec=false;
	}
#endif
	winGetPos(winItem(handle,VIDEO_REC_TIME_ID),&recordTimeRect);
	curWinIsVideo=true;
	
	if(0==SysCtrl.cartoon_mode)
	{
		SysCtrl.cartoon_mode=1;
		SysCtrl.cartoon_show_cnt=0;
	}

	SysCtrl.videorec_mode=VIDEOREC_NORMAL;
	if(winIsVisible(winItem(handle,VIDEO_REC_MODE_ID)))
	{
		winSetVisible(winItem(handle,VIDEO_REC_MODE_ID),false);
	}

	
	if(winIsVisible(winItem(handle,VIDEO_STATE_ID)))
	{
		winSetVisible(winItem(handle,VIDEO_STATE_ID),false);
	}


	SysCtrl.video_focus = VIDEO_FOCUS_ICON_NONE;
	videoFocusShow(handle);

	if(focus_sound==0)
	focus_sound = layout_sound_load(RES_MUSIC_PHOTO_FOCUS, &focus_sound_size);
	winSetPorgressDirction(winItem(handle,VIDEO_PROGRESS_BAR),1);
	winSetVisible(winItem(handle,VIDEO_PROGRESS_BAR),false);

	return 0;
}
static int videoCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("video Close Win!!!\n");
	curWinIsVideo=false;
	
	if(focus_sound)
	{
		hal_sysMemFree(focus_sound);
		focus_sound = 0;
	}
	return 0;
}
static int videoWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("video WinChild Close!!!\n");
	curWinIsVideo=true;

	if(task_video_Get_Status() != MEDIA_STAT_START)
		videoRemainTimeShow(handle, SysCtrl.t_remain);
//	#if SMALL_PANEL_SUPPORT==0
//	videoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif
	videoResolutionShow(handle);
	videoMDShow(handle);
	videoMonitorShow(handle);
	videoIrLedShow(handle);
	videoLockShow(handle);
	videoSDShow(handle);
	videoMicShow(handle);
	videoBaterryShow(handle);
//	winSetVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID),false);
	drawUIService(true);
	//useDoubleBuff(false);

	SysCtrl.videorec_mode=VIDEOREC_NORMAL;
	if(winIsVisible(winItem(handle,VIDEO_REC_MODE_ID)))
	{
		winSetVisible(winItem(handle,VIDEO_REC_MODE_ID),false);
	}
	return 0;
}
static int videoWinChildOpen(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("video WinChild Open!!!\n");
	curWinIsVideo=false;
	return 0;
}
static int videoRecordStopped(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("stop msg\n");
//	videoLockShow(handle);
	boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_B,1);
	videoRemainTimeShow(handle, SysCtrl.t_remain);
	return 0;
}
static int videoRecordStarted(winHandle handle,uint32 parameNum,uint32* parame)
{
	//SysCtrl.t_rec = 0;
	deg_Printf("start msg\n");
	videoLockShow(handle);
	videoRecTimeShow(handle,0);
	drawUIService(true);
	return 0;
}
static int videoRecordResume1(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("resume msg\n");
//	videoLockShow(handle);
	return 0;
}

static int videoRecordErr(winHandle handle,uint32 parameNum,uint32* parame)
{
//	videoLockShow(handle);
	boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_B,1);
	videoRemainTimeShow(handle, SysCtrl.t_remain);
	hal_curBuffer_clear();
	return 0;
}
static int videoRecordLockFile(winHandle handle,uint32 parameNum,uint32* parame)
{
//	SysCtrl.g_lock = 1;
//	videoLockShow(handle);
	return 0;
}
static int videoSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	static uint32 flag=0;

	if( configGet(CONFIG_ID_TIMESTAMP) == R_ID_STR_COM_ON)
	{
		if(!winIsVisible(winItem(handle,VIDEO_SYSTIME_ID)))
			winSetVisible(winItem(handle,VIDEO_SYSTIME_ID),true);
		winSetResid(winItem(handle,VIDEO_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
	}
	else
	{
		if(winIsVisible(winItem(handle,VIDEO_SYSTIME_ID)))
			winSetVisible(winItem(handle,VIDEO_SYSTIME_ID),false);
	}
//	#if SMALL_PANEL_SUPPORT==0
//	videoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif
	videoIrLedShow(handle);
	
	//if(SysCtrl.usb == USB_STAT_DCIN)
	{
		videoBaterryShow(handle);	
	}
/*
	if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		if(mdRecTime&&XOSTimeGet()-mdRecTime>=MOTION_DEC_TIME*1000)
		{
			task_record_stop();
			if(SysCtrl.pip_mode != DISP_PIP_BACK)
			{
				if(SysCtrl.battery > BATTERY_STAT_2)
					setSoftwareDetachUsensor(false);
			}
			mdRecTime=0;
		}
	}
*/
	if(task_video_Get_Status() == MEDIA_STAT_START)
	{
		if(flag&1)
			boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_B,1);
		else
			boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_B,0);
		flag++;

		if(winIsVisible(winItem(handle,VIDEO_STATE_ID)))
		{
			winSetVisible(winItem(handle,VIDEO_STATE_ID),false);
		}
		else
		{
			winSetVisible(winItem(handle,VIDEO_STATE_ID),true);
		}
	}
	/*
	if(winIsVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID)))
		winSetVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID),false);
		*/
	return 0;
}

msgDealInfor videoRecordeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,videoOpenWin},
	{SYS_CLOSE_WINDOW,videoCloseWin},
	{SYS_CHILE_COLSE,videoWinChildClose},
	{SYS_CHILE_OPEN,videoWinChildOpen},
//	{KEY_EVENT_RECORD,videoKeyMsgRecord},
	{KEY_EVENT_OK,videoKeyMsgRecord},
	{KEY_EVENT_POWER,videoKeyMsgExit},
//	{KEY_EVENT_SENSOR_CHANGE,videoKeyMsgSensorChange},
	{KEY_EVENT_UP,videoKeyMsgUp},
	{KEY_EVENT_DOWN,videoKeyMsgDown},
	{KEY_EVENT_MENU,videoKeyMsgLeft},
	{KEY_EVENT_MODE,videoKeyMsgRight},
//	{KEY_EVENT_IR,videoKeyMsgIr},
//	{KEY_EVENT_MENU,videoKeyMsgMenu},
//	{KEY_EVENT_PHOTO,videoKeyMsgPhoto},
//	{KEY_EVENT_PLAYBACK,videoKeyMsgPlayBack},
	{SYS_EVENT_SDC,videoSysMsgSD},
	{SYS_EVENT_USB,videoSysMsgUSB},
	{SYS_EVENT_BAT,videoSysMsgBattery},
//	{SYS_EVENT_MD,videoSysMsgMD},
//	{SYS_EVENT_ON,videoSysMsgRestart},
	{VID_EVENT_REC_STOP,videoRecordStopped},
	{VID_EVENT_REC_START,videoRecordStarted},
	{VID_EVENT_REC_RESUME,videoRecordResume1},
	{VID_EVENT_REC_ERROR,videoRecordErr},
//	{VID_EVENT_LOCK_FILE,videoRecordLockFile},
	{SYS_EVENT_1S,videoSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(videoRecordWindow,videoRecordeMsgDeal,videoRecordeWin)

bool cut_sdr_data_en = 0;
u32 cut_sdr_data_addr = 0;

#if _DEBG_CUT_SDR_DATA_
typedef struct 
{
	void * sdr_data_buff;
	u32 len;
}SDR_DATA_MSG;
SDR_DATA_MSG sdr_data;
/*******************************************************************************
* Function Name  : sdram_data_capture_cfg
* Description    : config for capture sdram_data
* Input          : void *sdr_data_buff
* Output         : None
* Return         : None
*******************************************************************************/
void sdram_data_capture_cfg(void *sdr_data_buff)
{
	//dvp
	void hal_Cut_SDR_SrcCSI(void *sdr_buf,bool csi_save);
#if (_DEBG_CUT_SDR_DATA_&0xf0) == _DEBG_CUT_BEFOR_CSI_
	hal_Cut_SDR_SrcCSI(sdr_data_buff,1);
#elif _DEBG_CUT_SDR_DATA_ == _DEBG_CUT_POST_YUV_
	hal_Cut_SDR_SrcCSI(sdr_data_buff,0);
#endif
}
/*******************************************************************************
* Function Name  : sdram_data_capture_task
* Description    : capture sdram data 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void sdram_data_capture_task(u8 *fname)
{
	ax32xx_csiEnable(0);
	hal_mjpegEncodeUninit();
	//capture rawdata
	_Sensor_Adpt_ *senradpt = hal_csiAdptGet();
#if _DEBG_CUT_SDR_DATA_ == _DEBG_CUT_CSI_RAW_
	debg("capture rawdata start\n");
	char filename[18] = "DEBG/RAWD0000.raw";
	sdr_data.len = senradpt->pixelw * senradpt->pixelh * 2;
	sdr_data.sdr_data_buff = hal_sysMemMalloc(senradpt->pixelh*senradpt->pixelw*2,64);
#elif _DEBG_CUT_SDR_DATA_ == _DEBG_CUT_CSI_YUV_
	debg("capture csi yuv422 data start\n");
	char filename[18] = "DEBG/YUV80000.raw";
	sdr_data.len = senradpt->pixelw * senradpt->pixelh * 2;
#else
	debg("capture post yuv420 data start\n");
	char filename[18] = "DEBG/YUV60000.yuv";
	sdr_data.len = senradpt->pixelw * senradpt->pixelh * 3 / 2;
	#if _DEBG_CUT_SDR_DATA_  ==  _DEBG_CUT_POST_YUV_INAVI_    
		sdr_data.sdr_data_buff = (void * )cut_sdr_data_addr;
	#elif _DEBG_CUT_SDR_DATA_  ==  _DEBG_CUT_POST_YUV_LCD_
		sdr_data.sdr_data_buff = (void * )cut_sdr_data_addr;
		sdr_data.len = 480*272*3/2;
	#endif
#endif
	if(sdr_data.sdr_data_buff ==NULL){
		debg("sdr_data_buff  apply fail \n");
		return;
	}
	sdram_data_capture_cfg(sdr_data.sdr_data_buff);
	//msg deal
	FIL Fil;
	FRESULT ret = f_mkdir("DEBG\0");
	if(ret != FR_OK){	
		if(ret != FR_EXIST){
			debg("DEBG mkdir err\n");
			return;
		}
	}
	u32 filenum ,loop = 1000000;
	debg("CSIMODE:%x CSIPMODE2:%x CSI_DMA_MODE:%x ",CSIMODE,ISPMODE,CSI_DMA_MODE);
	while(loop--){
		hal_wdtClear();
		if(cut_sdr_data_en == 1){
			debg("capture sdrdata pending\n");
			cut_sdr_data_en = 0;
			uint write;
			for(filenum = 0;filenum < 1000;filenum++){
				filename[9]='0'+ ((filenum / 1000)%10);
				filename[10]='0'+ ((filenum / 100)%10);
				filename[11]='0'+ ((filenum / 10)%10);
				filename[12]='0'+ (filenum  % 10);
				if(FR_OK != f_open(&Fil,(char const *)filename, FA_READ)){
					if(FR_OK == f_open(&Fil,(char const *)filename, FA_CREATE_NEW | FA_WRITE)){
						f_write(&Fil, (const void*)sdr_data.sdr_data_buff, sdr_data.len, &write);
						debg("Raw len = %x\n",write);
						loop = 0;
						hal_sysMemFree(sdr_data.sdr_data_buff);
						memcpy(fname,filename,sizeof(filename));
						hal_mjpegMemUninit();//mjpegEncCtrl.ybuffer = 0;
					}
					f_close(&Fil);
					break;
				}
				f_close(&Fil);
			}		
		}
	}
	
}
#endif


