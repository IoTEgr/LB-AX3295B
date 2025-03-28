#include "uiMenuDelCurRes.c"

static uint32 getdelCurResInfor(uint32 item,uint32* image,uint32* str)
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

static int delCurKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	char *name;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,DELCUR_SELECT_ID));
		if(item==0)
		{
			if(NULL==SysCtrl.sdcard)
			{
				if(spi_udisk_jpg_num()>0)
				{
					if(0==spi_udisk_del_jpg(SysCtrl.file_index))
					{
						if(SysCtrl.file_index>=spi_udisk_jpg_num())
						{
							SysCtrl.file_index=spi_udisk_jpg_num()-1;
						}
						deg_Printf("SysCtrl.file_index=%d\n",SysCtrl.file_index);
						uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
					}
					else
					{
						uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_FAILED,2);
					}
				}
				else
				{
					uiOpenWindow(&tipsWindow,2,R_ID_STR_TIPS_NO_FILES,2);
					return;
				}
			}
			else
			{
				if(managerFileCount(SysCtrl.avi_list)<=0)
				{
					uiOpenWindow(&tipsWindow,2,R_ID_STR_TIPS_NO_FILES,2);
					return;
				}
				if(managerCheckLockExt(SysCtrl.avi_list,SysCtrl.file_index)<=0)
				{
					name = manangerGetFileFullPathName(SysCtrl.avi_list,SysCtrl.file_index,NULL);
					deg_Printf("delete : %s.",name);
					if(f_unlink(name)==FR_OK)
					{
						deg_Printf("->ok\n");
						managerDelFileByIndex(SysCtrl.avi_list,SysCtrl.file_index);
						if(SysCtrl.file_index>managerFileCount(SysCtrl.avi_list))
							SysCtrl.file_index=managerFileCount(SysCtrl.avi_list);
						deamon_fsFreeSize();
						uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
					}
					else
					{
						deg_Printf("->fail\n");
						uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_FAILED,2);
					}
				}
				else
					uiOpenWindow(&tipsWindow,2,R_ID_STR_SET_LOCKED,2);

			}
		}
		else
			winDestroy(&handle);
	}
	return 0;
}
static int delCurKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		itemManagePreItem(winItem(handle,DELCUR_SELECT_ID));
	}
	return 0;
}
static int delCurKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		 itemManageNextItem(winItem(handle,DELCUR_SELECT_ID));
	}
	return 0;
}
static int delCurKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int delCurKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int delCurOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("delCur Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,DELCUR_SELECT_ID),2,R1h(40));
	itemManageSetColumnSumWithGap(winItem(handle,DELCUR_SELECT_ID),2,2,R1w(220),0);
	itemManageCreateItem(winItem(handle,DELCUR_SELECT_ID),itemCreatemenuOption,getdelCurResInfor,2);
	itemManageSetCharInfor(winItem(handle,DELCUR_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,DELCUR_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,DELCUR_SELECT_ID),R_COLOR_BLACK);

	itemManageSetCurItem(winItem(handle,DELCUR_SELECT_ID),0);
	
	return 0;
}
static int delCurCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("delCur Close Win!!!\n");
	return 0;
}
static int delCurWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("delCur WinChild Close!!!\n");
	winDestroy(&handle);
	return 0;
}


msgDealInfor delCurMsgDeal[]=
{
	{SYS_OPEN_WINDOW,delCurOpenWin},
	{SYS_CLOSE_WINDOW,delCurCloseWin},
	{SYS_CHILE_COLSE,delCurWinChildClose},
	{KEY_EVENT_OK,delCurKeyMsgOk},
	{KEY_EVENT_UP,delCurKeyMsgUp},
	{KEY_EVENT_DOWN,delCurKeyMsgDown},
	{KEY_EVENT_POWER,delCurKeyMsgMenu},
//	{KEY_EVENT_MODE,delCurKeyMsgMode},
	{EVENT_MAX,NULL},
};

WINDOW(delCurWindow,delCurMsgDeal,delCurWin)


