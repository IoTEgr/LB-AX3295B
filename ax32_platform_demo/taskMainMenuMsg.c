#include "application.h"
#include "taskMainMenuRes.c"
#include "userInterface.h"


#define MAINMENU_TASK_NUM 		6

static u8 show_bk;
static u32 mainmenu_show_time;

static u8* mainmenu_bk_buf=NULL;
static u16 bk_w,bk_h;

static u32 mainmenu_jgp_id[MAINMENU_TASK_NUM]={RES_MAIN_PHOTO_BK,RES_MAIN_VIDEO_BK,RES_MAIN_PLAYBACK_BK,
								               RES_MAIN_GAME_BK,RES_MAIN_MP3_BK,RES_MAIN_SETTING_BK
								};

static void mainMenu_showbuf(void)	 
{
	disp_frame_t *p_lcd_buffer;

	p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(p_lcd_buffer)
	{
		u16 lcd_w,lcd_h;
		hal_lcdGetResolution(&lcd_w,&lcd_h);
		//deg_Printf("video:w=%d,h=%d,bufw=%d,bufh=%d,\n",lcd_w,lcd_h,p_lcd_buffer->w,p_lcd_buffer->h);
		hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);

		memcpy(p_lcd_buffer->y_addr,mainmenu_bk_buf,bk_w*bk_h*3/2);
		ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,bk_w*bk_h*3/2);
		dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
	}
	else
	{
		deg_Printf("video buf null!\n");
	}
}
static int mainMenuKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	int ret;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int mainMenuKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		mainMenuStrShow(handle,false);
		drawUIService(true);		// clean str
		
		if(0==SysCtrl.mainmenu_taskidx)
		{
			taskStart(TASK_PHOTO_ENCODE,0);
		}
		else if(1==SysCtrl.mainmenu_taskidx)
		{
			taskStart(TASK_VIDEO_RECORD,0);
		}
		else if(2==SysCtrl.mainmenu_taskidx)
		{
			taskStart(TASK_PLAY_BACK,0);
		}
		else if(3==SysCtrl.mainmenu_taskidx)
		{
			taskStart(TASK_GAME_MENU,0);
			
		}
		else if(4==SysCtrl.mainmenu_taskidx)
		{
			taskStart(TASK_AUDIO_PLAYER,0);
		}
		else if(5==SysCtrl.mainmenu_taskidx)
		{
			taskStart(TASK_SETTING_MENU,0);
		}
		deg_Printf("id_idx:%d\n",SysCtrl.mainmenu_taskidx);
	}
	return 0;
}
static int mainMenuKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(SysCtrl.mainmenu_taskidx>2)
		{
			SysCtrl.mainmenu_taskidx-=3;
		}
		else
		{
			SysCtrl.mainmenu_taskidx+=3;
		}
		
		if(NULL!=mainmenu_bk_buf)
		{
			jpg_decode_buf(mainmenu_jgp_id[SysCtrl.mainmenu_taskidx],mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
		}
		XOSTimeDly(30);
		mainMenu_showbuf();
		mainmenu_show_time=XOSTimeGet();
		show_bk=1;

	}
	return 0;
}
static int mainMenuKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(SysCtrl.mainmenu_taskidx>2)
		{
			SysCtrl.mainmenu_taskidx-=3;
		}
		else
		{
			SysCtrl.mainmenu_taskidx+=3;
		}
		
		if(NULL!=mainmenu_bk_buf)
		{
			jpg_decode_buf(mainmenu_jgp_id[SysCtrl.mainmenu_taskidx],mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
		}
		XOSTimeDly(30);
		mainMenu_showbuf();
		mainmenu_show_time=XOSTimeGet();
		show_bk=1;

	}
	return 0;
}

static int mainMenuKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		if(SysCtrl.mainmenu_taskidx<(MAINMENU_TASK_NUM-1))
		{
			SysCtrl.mainmenu_taskidx++;
		}
		else
		{
			SysCtrl.mainmenu_taskidx=0;
		}
		
		if(NULL!=mainmenu_bk_buf)
		{
			jpg_decode_buf(mainmenu_jgp_id[SysCtrl.mainmenu_taskidx],mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
		}
		XOSTimeDly(30);
		mainMenu_showbuf();
		mainmenu_show_time=XOSTimeGet();
		show_bk=1;
	}
	return 0;
}
static int mainMenuKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(SysCtrl.mainmenu_taskidx>0)
		{
			SysCtrl.mainmenu_taskidx--;
		}
		else
		{
			SysCtrl.mainmenu_taskidx=(MAINMENU_TASK_NUM-1);
		}
		
		if(NULL!=mainmenu_bk_buf)
		{
			jpg_decode_buf(mainmenu_jgp_id[SysCtrl.mainmenu_taskidx],mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
		}
		XOSTimeDly(30);
		mainMenu_showbuf();
		mainmenu_show_time=XOSTimeGet();
		show_bk=1;
	}
	return 0;
}

static int mainMenuOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("main menu Open Win:%d\n",XOSTimeGet());
	show_bk=0;
	if(SysCtrl.mainmenu_taskidx>=MAINMENU_TASK_NUM)
	{
		SysCtrl.mainmenu_taskidx=0;
	}
	deg_Printf("SysCtrl.mainmenu_taskidx=%d\n",SysCtrl.mainmenu_taskidx);
	//==init==
	hal_lcdGetResolution(&bk_w,&bk_h);
	bk_w=(bk_w + 0x1f) & (~0x1f);  // add 32bit alignment

	if(mainmenu_bk_buf == NULL)
	{
		mainmenu_bk_buf=hal_sysMemMalloc(bk_w*bk_h*3/2,32);
	}

	if(NULL!=mainmenu_bk_buf)
	{
		jpg_decode_buf(RES_MAIN_BK,mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}

	bool change_finir = false;
	while(1)
	{
		disp_frame_t *p_lcd_buffer;

		p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
		if(p_lcd_buffer)
		{
			u16 lcd_w,lcd_h;
			hal_lcdGetResolution(&lcd_w,&lcd_h);
			hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);

			memcpy(p_lcd_buffer->y_addr,mainmenu_bk_buf,bk_w*bk_h*3/2);
			change_finir = changeUserInterface.run(&changeUserInterface, p_lcd_buffer->y_addr);
			ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,bk_w*bk_h*3/2);
			dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
			XOSTimeDly(40);
			if(change_finir == true)
				break;
		}
		else
		{
			deg_Printf("video buf null!\n");
		}
	}

	SysCtrl.volume_level = configValue2Int(CONFIG_ID_VOLUME);
	hal_dacSetVolume(SysCtrl.volume_level*10);

	return 0;
}

static int mainMenuCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("main menu Close Win!!!\n");
	if(NULL!=mainmenu_bk_buf)
	{
		hal_sysMemFree(mainmenu_bk_buf);
		mainmenu_bk_buf=NULL;
	}
	return 0;
}

static int mainMenuSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(XOSTimeGet()<mainmenu_show_time+500)	// too short not show it
	{
		return 0;
	}

	if(show_bk)
	{
		if(NULL!=mainmenu_bk_buf)
		{
			jpg_decode_buf(RES_MAIN_BK,mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
		}
		show_bk=0;
	}
	else
	{
		if(NULL!=mainmenu_bk_buf)
		{
			jpg_decode_buf(mainmenu_jgp_id[SysCtrl.mainmenu_taskidx],mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
		}
		show_bk=1;
		
	}
	mainMenu_showbuf();
	mainmenu_show_time=XOSTimeGet();
	return 0;
}
static int mainMenuSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
msgDealInfor mainMenuMsgDeal[]=
{
	{SYS_OPEN_WINDOW,mainMenuOpenWin},
	{SYS_CLOSE_WINDOW,mainMenuCloseWin},
	{KEY_EVENT_UP,mainMenuKeyMsgUp},
	{KEY_EVENT_DOWN,mainMenuKeyMsgDown},
	{KEY_EVENT_MODE,mainMenuKeyMsgMode},
	{KEY_EVENT_MENU,mainMenuKeyMsgMenu},
	{KEY_EVENT_OK,mainMenuKeyMsgOk},
	{SYS_EVENT_500MS,mainMenuSysMsg500MS},
	{SYS_EVENT_1S,mainMenuSysMsg1S},
	{EVENT_MAX,NULL},
};
WINDOW(mainMenuWindow,mainMenuMsgDeal,mainMenuWin)

