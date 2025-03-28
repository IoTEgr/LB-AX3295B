#include "application.h"
#include "taskPhotoEncodeRes.c"
#include "userInterface.h"


#if (4 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0,RES_FRAME1,RES_FRAME2,RES_FRAME3};
#elif (9 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME8,RES_FRAME7,RES_FRAME6,RES_FRAME5,
								  RES_FRAME4,RES_FRAME3,RES_FRAME2,RES_FRAME1,
								  RES_FRAME0};
#elif (20 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0,RES_FRAME1,RES_FRAME2,RES_FRAME3,RES_FRAME4,
 								  RES_FRAME5,RES_FRAME6,RES_FRAME7,RES_FRAME8,RES_FRAME9,
 								  RES_FRAME10,RES_FRAME11,RES_FRAME12,RES_FRAME13,RES_FRAME14,
 								  RES_FRAME_HOME0,RES_FRAME_HOME1,RES_FRAME_HOME2,RES_FRAME_HOME3,RES_FRAME_HOME4,
 								  };
#else // 20


#endif

INT8U delay_timer=0,ok_key_active=0;




extern int image_take_photo(void);
extern int image_take_photo_to_sdram();

u32 photo_focus_show_count = 0;
static u8 *focus_sound = 0;
static s32 focus_sound_size = 0;
void photoFocusIconCancel(winHandle handle)
{
	if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_RED)
	{
		if(XOSTimeGet() - photo_focus_show_count > 1000)
		{
			SysCtrl.photo_focus=PHOTO_FOCUS_ICON_NONE;
			photoFocusShow(handle);
			photoZoomRatioShow(handle);
		}
	}
}



static int photoKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	int ret;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

		if( configGet(CONFIG_ID_TIMEPHOTO) == R_ID_STR_COM_OFF)
		{
			XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_PHOTO,KEY_PRESSED));
		}
		else
		{
			if(configValue2Int(CONFIG_ID_TIMEPHOTO))
				delay_timer = configValue2Int(CONFIG_ID_TIMEPHOTO)+1;
			ok_key_active = 1;
		}


	
	}
	return 0;
}


static int photoKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	int8 ret;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

		//while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY){XOSTimeDly(10);}
		//audioPlaybackSetVolume(configValue2Int(CONFIG_ID_VOLUME)*10);
		boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_B,0);
		/*if(SysCtrl.f_keysound)
		{
			logo_music_show(RES_MUSIC_TAKE_PHOTO);
		}
		while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);*/

	
		if(SDC_STAT_NULL==SysCtrl.sdcard)
		{
			if(spi_udisk_jpg_free_num()==0)
			{
				uiOpenWindow(&tipsWindow,2,R_ID_STR_TIPS_SPI_FULL,3);
				ret=-1;
			}
			else
			{
				ret=image_take_photo_to_sdram();
			}
		
		}
		else
		{
			ret=image_take_photo();
		}

		boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_B,1);

		if(ret==0)
		{
			UserInterface captureAnimation;
			ANIMATION(captureAnimation, LEFTBOTTOM2RIGHTUPPER)
			photo_animation_effect(captureAnimation,1);
			photoRemainNumShow(handle);
		}
	}
	return 0;
}

static int photoKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

		deamon_keysound_play();
		while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);
		
		taskStart(TASK_MAIN_MENU,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}
	}
	return 0;
}

extern INT8U photoPipMode;
static int photoKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{
		deamon_keysound_play();
		while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);
		
		if(delay_timer)
		{
			return 0;
		}

		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW)
		{
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}
	
		if(0==SysCtrl.photo_mode_switch)	// photo mode   相框
		{
			if(SysCtrl.photo_mode_idx>0)	// pre photo mode 
			{
				SysCtrl.photo_mode_idx--;
			}
			else							//  swith frame mode
			{
				SysCtrl.photo_mode_switch=3;
				hal_custom_frame_add_enable(1);
				videoRecordCmdSet(CMD_COM_FRAME,1);
				SysCtrl.photo_frame_idx = (ICON_FRAME_NUM -1);
				deg_Printf("photo_frame_idx:%d\n",SysCtrl.photo_frame_idx);
				s32 ret=hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
				if(ret<0)
				{
					deg_Printf(">>frame err!!%d<<\n",ret);
				}

				SysCtrl.crop_level=0;
				dispLayerSetFrontCrop(SysCtrl.crop_level);
				photoZoomRatioShow(handle);

			}
		}
		else if(1==SysCtrl.photo_mode_switch)	// spec col mode   颜色
		{
			if(SysCtrl.spec_color_index>0)
			{
				SysCtrl.spec_color_index--;
				cmos_spec_color(SysCtrl.spec_color_index);
				if(0==SysCtrl.spec_color_index)// switch photo mode
				{
					//==color normal
					cmos_spec_color(SysCtrl.spec_color_index);
					SysCtrl.photo_mode_switch=0;
				}
			}
		}
		else if(2==SysCtrl.photo_mode_switch)	// effect mode  特效
		{
			if(SysCtrl.photo_effect_idx>0)
			{
				SysCtrl.photo_effect_idx--;
			}
			else
			{
				//==switch spec color mode==
				SysCtrl.photo_mode_switch=1;
				SysCtrl.spec_color_index=5;
				cmos_spec_color(SysCtrl.spec_color_index);
			}
		}
		else if(3==SysCtrl.photo_mode_switch)	// frame mode  相框
		{
			if(SysCtrl.photo_frame_idx>0)
			{
				SysCtrl.photo_frame_idx--;

				s32 ret=hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
				if(ret<0)
				{
					deg_Printf(">>frame err!!%d<<\n",ret);
				}
			}
			else	
			{
				//==close frame==
				videoRecordCmdSet(CMD_COM_FRAME,0);
				hal_custom_frame_add_enable(0);
				hal_custom_frame_unit();
				//SysCtrl.photo_frame_idx = ICON_FRAME_NUM -1;

				// switch effect mode 
				SysCtrl.photo_mode_switch=2;
				SysCtrl.photo_effect_idx=CAM_EFFECT_MAX-1;
				//==open effect==
			}
		}

		//photo_mode_show(handle);
	}
	else if(keyState==KEY_CONTINUE)
	{
		if(0==SysCtrl.photo_mode_switch)
		{
			SysCtrl.crop_level+=1;
			if(SysCtrl.crop_level > 30)
				SysCtrl.crop_level=30;
			dispLayerSetFrontCrop(SysCtrl.crop_level);
			photoZoomRatioShow(handle);
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
			photoFocusShow(handle);
		}
		photo_focus_show_count=XOSTimeGet();
	}
	return 0;
}
static int photoKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{

		deamon_keysound_play();
		while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);

		if(delay_timer)
		{
			return 0;
		}
		
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW)
		{
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}
		
		if(0==SysCtrl.photo_mode_switch)	// photo mode    
		{
			/*if(SysCtrl.photo_mode_idx<6)	// next photo mode 
			{
				SysCtrl.photo_mode_idx++;
			}
			else	//	swith spec color mode*/
			{
				//==close photo mode show==
				SysCtrl.photo_mode_idx=0;
				
				SysCtrl.photo_mode_switch=1;
				SysCtrl.spec_color_index=1;
				cmos_spec_color(SysCtrl.spec_color_index);

				SysCtrl.crop_level=0;
				dispLayerSetFrontCrop(SysCtrl.crop_level);
				photoZoomRatioShow(handle);
			}
		}
		else if(1==SysCtrl.photo_mode_switch)	// spec col mode
		{
			if(SysCtrl.spec_color_index<5)
			{
				SysCtrl.spec_color_index++;
				cmos_spec_color(SysCtrl.spec_color_index);
			}
			else	// switch effect mode
			{
				//==normal color==
				SysCtrl.spec_color_index=0;
				cmos_spec_color(SysCtrl.spec_color_index);
				//==
				SysCtrl.photo_mode_switch=2;
				SysCtrl.photo_effect_idx=0;
				deg_Printf("photo_mode_switch=%d,idx=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_effect_idx);
			}
		}
		else if(2==SysCtrl.photo_mode_switch)	// effect mode
		{
			if(SysCtrl.photo_effect_idx<CAM_EFFECT_MAX-1)
			{
				SysCtrl.photo_effect_idx++;
			}
			else
			{
				//==switch frame mode==
				SysCtrl.photo_mode_switch=3;
				hal_custom_frame_add_enable(1);
				videoRecordCmdSet(CMD_COM_FRAME,1);
				SysCtrl.photo_frame_idx = 0;
				deg_Printf("frame_index:%d\n",SysCtrl.photo_frame_idx);
				s32 ret=hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
				if(ret<0)
				{
					deg_Printf(">>frame err!!%d<<\n",ret);
				}
			}
		}
		else if(3==SysCtrl.photo_mode_switch)	// frame mode
		{
			if(SysCtrl.photo_frame_idx<ICON_FRAME_NUM -1)
			{
				SysCtrl.photo_frame_idx++;
				s32 ret=hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
				if(ret<0)
				{
					deg_Printf(">>frame err!!%d<<\n",ret);
				}
			}
			else	
			{
				//==close frame==
				videoRecordCmdSet(CMD_COM_FRAME,0);
				hal_custom_frame_add_enable(0);
				hal_custom_frame_unit();

				// switch photo mode 
				SysCtrl.photo_mode_switch=0;
				SysCtrl.photo_mode_idx=0;
			}
		}
		//photo_mode_show(handle);
	}
	else if(keyState==KEY_CONTINUE)
	{
		if(0==SysCtrl.photo_mode_switch)
		{
			SysCtrl.crop_level-=1;
			if(SysCtrl.crop_level > 200)
				SysCtrl.crop_level=0;
			dispLayerSetFrontCrop(SysCtrl.crop_level);
			photoZoomRatioShow(handle);
 			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
			photoFocusShow(handle);
		}
		photo_focus_show_count=XOSTimeGet();
	}

	return 0;
}


static int photoKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint8 flag=0;
	uint16 lcd_w,lcd_h;

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_RELEASE)
	{

		deamon_keysound_play();
		while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);
		
		if(delay_timer)
		{
			return 0;
		}

		if( configGet(CONFIG_ID_TIMEPHOTO) == R_ID_STR_COM_OFF)
		{
			configSet(CONFIG_ID_TIMEPHOTO,R_ID_STR_TIM_5SEC);
			configSys(CONFIG_ID_TIMEPHOTO);
			userConfigSave();
			delay_timer=0;
			ok_key_active=0;
		}
		else
		{
			configSet(CONFIG_ID_TIMEPHOTO,R_ID_STR_COM_OFF);
			configSys(CONFIG_ID_TIMEPHOTO);
			userConfigSave();
			winSetVisible(winItem(handle,PHOTO_TAKETIME_ID),FALSE);
		}
		photo_delay_show(handle);
	}
	return 0;
}



static int photoKeyMsgRight(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint8 flag=0;
	uint16 lcd_w,lcd_h;

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_RELEASE)
	{
		deamon_keysound_play();
		while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);

		if(delay_timer)
		{
			return 0;
		}
		
		sensorChange();
#if 1
		{
			disp_frame_t * handle_frame;
			UserInterface recordeAnimation;
			ANIMATION(recordeAnimation, SQUARE_INSIDE2OUTSIDE)

			//SysCtrl.photo_task=1;
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
		//SysCtrl.photo_task=0;
		}
#endif
	}
	return 0;
}
static int photoSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	photoSDShow(handle);
	photoImagePreSizeGet();
	photoRemainNumShow(handle);
	if(SysCtrl.sdcard == SDC_STAT_NULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_NOT_INSERT,SDC_NULL_TIMEOUT);
	else if(SysCtrl.sdcard == SDC_STAT_FULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_FULL,SDC_NULL_TIMEOUT);
	else if(SysCtrl.sdcard == SDC_STAT_ERROR)
		uiOpenWindow(&tips1Window,2,TIPS_SD_ERROR,SDC_NULL_TIMEOUT);
	deg_Printf("SD state:%d\n",SysCtrl.sdcard);
	return 0;
}

static int photoSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	photoBaterryShow(handle);
	return 0;
}
static int photoSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		photoBaterryShow(handle);
	return 0;
}
static int photoSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int photoOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("photo Open Win!!!\n");
	layout_version_get();
	photoResolutionShow(handle);
	photoMDShow(handle);
	photoMonitorShow(handle);
	photoIrLedShow(handle);
	photoLockShow(handle,false);
	photoSDShow(handle);
	photoMicShow(handle);
	photoBaterryShow(handle);
	photoImagePreSizeGet();
	photoRemainNumShow(handle);
	
	//photoZoomRatioShow(handle);
	SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
	photoFocusShow(handle);
	if(focus_sound==0)
	focus_sound = layout_sound_load(RES_MUSIC_PHOTO_FOCUS, &focus_sound_size);
	winSetPorgressDirction(winItem(handle,PHOTO_PROGRESS_BAR),1);
	winSetVisible(winItem(handle,PHOTO_PROGRESS_BAR),false);
	winSetVisible(winItem(handle,PHOTO_TAKETIME_ID),false);
//	photo_timephoto(handle,true);
//	photo_morephoto(handle,true);
	//photo_mode_show(handle);
	photo_delay_show(handle);
//	photo_time_num_show(handle,false,0);
	if(0==SysCtrl.cartoon_mode)
	{
		SysCtrl.cartoon_mode=1;
		SysCtrl.cartoon_show_cnt=0;
	}
	
	delay_timer=0;
	ok_key_active=0;

	audioPlaybackSetVolume(configValue2Int(CONFIG_ID_VOLUME)*10);
	return 0;
}
static int photoCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("photo Close Win!!!\n");

	if(focus_sound)
	{
		hal_sysMemFree(focus_sound);
		focus_sound = 0;
	}

	return 0;
}
static int photoWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("photo WinChild Close!!!\n");
	photoImagePreSizeGet();
	photoRemainNumShow(handle);
	photoResolutionShow(handle);
	photoMDShow(handle);
	photoMonitorShow(handle);
	photoIrLedShow(handle);
	photoLockShow(handle,false);
	photoSDShow(handle);
	photoMicShow(handle);
	photoBaterryShow(handle);
	photo_delay_show(handle);
	winSetVisible(winItem(handle,PHOTO_TAKETIME_ID),false);

//	photo_timephoto(handle,true);
//	photo_morephoto(handle,true);
//	photo_time_num_show(handle,false,0);

//	#if SMALL_PANEL_SUPPORT==0
//	photoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif
	return 0;
}
static int photoSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	static uint32 flag=0;
	static char str[3] = "000";

	if( configGet(CONFIG_ID_TIMESTAMP) == R_ID_STR_COM_ON)
	{
		if(!winIsVisible(winItem(handle,PHOTO_SYSTIME_ID)))
			winSetVisible(winItem(handle,PHOTO_SYSTIME_ID),true);
		winSetResid(winItem(handle,PHOTO_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
	}
	else
	{
		if(winIsVisible(winItem(handle,PHOTO_SYSTIME_ID)))
			winSetVisible(winItem(handle,PHOTO_SYSTIME_ID),false);
	}
	
	winSetResid(winItem(handle,PHOTO_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
	photoIrLedShow(handle);
//	photo_timephoto(handle,true);
//	photo_morephoto(handle,true);
	//if(SysCtrl.usb == USB_STAT_DCIN)
	{
		photoBaterryShow(handle);	
	}

	if(configValue2Int(CONFIG_ID_TIMEPHOTO ))
	{
		if(delay_timer)
		{	
			if((delay_timer-1)/10%10 > 0 )
			{
				str[0] = (delay_timer-1)/10%10+'0';	
				str[1] = (delay_timer-1)%10+'0';	
				str[2] =0;
			}
			else
			{
				
				str[0] = (delay_timer-1)%10+'0';		
				str[1] =0;
			}
			winSetVisible(winItem(handle,PHOTO_TAKETIME_ID),TRUE);
			if( !(delay_timer-1) )		
				winSetVisible(winItem(handle,PHOTO_TAKETIME_ID),FALSE);
			winSetResid(winItem(handle,PHOTO_TAKETIME_ID),strToResId(str));	
			delay_timer--;
		}
		
		if(ok_key_active && !delay_timer)
		{
			ok_key_active=0;
			//winSetVisible(winItem(handle,PHOTO_TAKETIME_ID),FALSE);
			XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_PHOTO,KEY_PRESSED));
		}
		
	}
	
	return 0;
}



msgDealInfor photoEncodeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,photoOpenWin},
	{SYS_CLOSE_WINDOW,photoCloseWin},
	{SYS_CHILE_COLSE,photoWinChildClose},
	{KEY_EVENT_OK,photoKeyMsgPhoto},
	{KEY_EVENT_PHOTO,photoKeyMsgOk},
//	{KEY_EVENT_SENSOR_CHANGE,photoKeyMsgSensorChange},
	{KEY_EVENT_UP,photoKeyMsgUp},
	{KEY_EVENT_DOWN,photoKeyMsgDown},
	{KEY_EVENT_MENU,photoKeyMsgMenu},
	{KEY_EVENT_MODE,photoKeyMsgRight},
	
	{KEY_EVENT_POWER,photoKeyMsgExit},

//	{KEY_EVENT_RECORD,photoKeyMsgVideo},
//	{KEY_EVENT_PLAYBACK,photoKeyMsgPlayBack},//return
//	{KEY_EVENT_PRINTER_EN,photoKeyMsgPrinterEn},
	{SYS_EVENT_SDC,photoSysMsgSD},
	{SYS_EVENT_USB,photoSysMsgUSB},
	{SYS_EVENT_BAT,photoSysMsgBattery},
	{SYS_EVENT_MD,photoSysMsgMD},
	{SYS_EVENT_1S,photoSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(photoEncodeWindow,photoEncodeMsgDeal,photoEncodeWin)



