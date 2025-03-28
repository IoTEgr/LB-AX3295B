#include "uiMenuFormatRes.c"

static uint32 getformatResInfor(uint32 item,uint32* image,uint32* str)
{
	if(item==0)
	{
		if(image)
			*image=INVALID_RES_ID;
		if(str)
			*str=R_ID_STR_COM_OK;
	}
	else if(item==1)
	{
		if(image)
			*image=INVALID_RES_ID;
		if(str)
			*str=R_ID_STR_COM_CANCEL;
	}
	return 0;
}

static int formatKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	INT8U *mem;
	INT8U res;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,FORMAT_SELECT_ID));
		if(item==0)
		{
			mem = (INT8U *)hal_sysMemMalloc(32768,64);
			if(mem == NULL)
			{
				deg_Printf("malloc mem for format fail.\n");
				uiOpenWindow(&tipsWindow,2,"memory malloc failed",2);
				return 0;
			}
			uiOpenWinAndShow(&tipsWindow,2,R_ID_STR_TIPS_PLEASE_WAITING,5);
			if(SDC_STAT_NULL==SysCtrl.sdcard)
			{
				spi_udisk_format(FLASH_CAPACITY);
				uiOpenWindow(&tipsWindow,2,R_ID_STR_TIPS_FMT_SUCCESS,2);
			}
			else
			{
				res =f_mkfs("",NULL,(void *)mem,32768);
				hal_sysMemFree(mem);
				if(res==FR_OK)
				{				
					//SysCtrl.sdcard = SDC_STAT_NULL;  // systemDeamonService will mount 
					managerDestory(SysCtrl.avi_list);
					managerDestory(SysCtrl.jpg_list);
					managerDestory(SysCtrl.wav_list);
					managerInit();
					SysCtrl.avi_list = -1;
					SysCtrl.jpg_list = -1;
					SysCtrl.wav_list = -1;
					deamon_fs_scan();
					spi_udisk_format(FLASH_CAPACITY);
					
					uiOpenWindow(&tipsWindow,2,R_ID_STR_TIPS_FMT_SUCCESS,2);
				}
				else
				{
					deg_Printf ("error : %d\n",res);
					uiOpenWindow(&tipsWindow,2,R_ID_STR_TIPS_FMT_FAIL,2);
				}
			}
		}
		else
			winDestroy(&handle);
	}
	return 0;
}
static int formatKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		itemManagePreItem(winItem(handle,FORMAT_SELECT_ID));
	}
	return 0;
}
static int formatKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		 itemManageNextItem(winItem(handle,FORMAT_SELECT_ID));
	}
	return 0;
}
static int formatKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		winDestroy(&handle);
	}
	return 0;
}
static int formatKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		taskChange();
	}
	return 0;
}
static int formatOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("format Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,FORMAT_SELECT_ID),1,R1h(32));
	#if SMALL_PANEL_SUPPORT==0
	itemManageSetColumnSumWithGap(winItem(handle,FORMAT_SELECT_ID),0,2,R1w(50),R1w(12));
	#else
	itemManageSetColumnSumWithGap(winItem(handle,FORMAT_SELECT_ID),0,2,R1w(100),R1w(6));
	#endif
	itemManageCreateItem(winItem(handle,FORMAT_SELECT_ID),itemCreatemenuOption,getformatResInfor,2);
	itemManageSetCharInfor(winItem(handle,FORMAT_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,FORMAT_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,FORMAT_SELECT_ID),R_COLOR_GRAY);

	itemManageSetCurItem(winItem(handle,FORMAT_SELECT_ID),1);
	
	return 0;
}
static int formatCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("format Close Win!!!\n");
	return 0;
}
static int formatWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("format WinChild Close!!!\n");
	winDestroy(&handle);
	return 0;
}


msgDealInfor formatMsgDeal[]=
{
	{SYS_OPEN_WINDOW,formatOpenWin},
	{SYS_CLOSE_WINDOW,formatCloseWin},
	{SYS_CHILE_COLSE,formatWinChildClose},
	{KEY_EVENT_OK,formatKeyMsgOk},
	{KEY_EVENT_UP,formatKeyMsgUp},
	{KEY_EVENT_DOWN,formatKeyMsgDown},
	{KEY_EVENT_MENU,formatKeyMsgMenu},
	{KEY_EVENT_MODE,formatKeyMsgMode},
	{EVENT_MAX,NULL},
};

WINDOW(formatWindow,formatMsgDeal,formatWin)


