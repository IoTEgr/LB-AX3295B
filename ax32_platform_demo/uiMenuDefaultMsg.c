#include "uiMenuDefaultRes.c"

static uint32 getdefaultResInfor(uint32 item,uint32* image,uint32* str)
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

static int defaultKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,DEFAULT_SELECT_ID));
		if(item==0)
		{
			userConfigReset();
			configSystem();			
		}
		winDestroy(&handle);
	}
	return 0;
}
static int defaultKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		itemManagePreItem(winItem(handle,DEFAULT_SELECT_ID));
	}
	return 0;
}
static int defaultKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		 itemManageNextItem(winItem(handle,DEFAULT_SELECT_ID));
	}
	return 0;
}
static int defaultKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int defaultKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
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
static int defaultOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("default Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,DEFAULT_SELECT_ID),1,R1h(32));
	#if SMALL_PANEL_SUPPORT==0
	itemManageSetColumnSumWithGap(winItem(handle,DEFAULT_SELECT_ID),0,2,R1w(50),R1w(12));
	#else
	itemManageSetColumnSumWithGap(winItem(handle,DEFAULT_SELECT_ID),0,2,R1w(100),R1w(6));
	#endif 
	itemManageCreateItem(winItem(handle,DEFAULT_SELECT_ID),itemCreatemenuOption,getdefaultResInfor,2);
	itemManageSetCharInfor(winItem(handle,DEFAULT_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,DEFAULT_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,DEFAULT_SELECT_ID),R_COLOR_GRAY);

	itemManageSetCurItem(winItem(handle,DEFAULT_SELECT_ID),1);
	
	return 0;
}
static int defaultCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("default Close Win!!!\n");
	return 0;
}
static int defaultWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("default WinChild Close!!!\n");
	return 0;
}


msgDealInfor defaultMsgDeal[]=
{
	{SYS_OPEN_WINDOW,defaultOpenWin},
	{SYS_CLOSE_WINDOW,defaultCloseWin},
	{SYS_CHILE_COLSE,defaultWinChildClose},
	{KEY_EVENT_OK,defaultKeyMsgOk},
	{KEY_EVENT_UP,defaultKeyMsgUp},
	{KEY_EVENT_DOWN,defaultKeyMsgDown},
	{KEY_EVENT_MENU,defaultKeyMsgMenu},
	{KEY_EVENT_MODE,defaultKeyMsgMode},
	{EVENT_MAX,NULL},
};

WINDOW(defaultWindow,defaultMsgDeal,defaultWin)


