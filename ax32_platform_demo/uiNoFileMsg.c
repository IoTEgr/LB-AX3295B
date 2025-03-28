#include "uiNoFileRes.c"

static int noFileKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_keysound_play();
		taskStart(TASK_MAIN_MENU,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}

	}
	return 0;
}

static int noFileSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
	{
		if(managerFileCount(SysCtrl.avi_list)>0)
			SysCtrl.file_index = managerFileCount(SysCtrl.avi_list);
		else
			SysCtrl.file_index=0;
			winDestroy(&handle);
		
	}
	return 0;
}
static int noFileOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("noFile open Win!!!\n");
	if(parameNum==1)
		winSetResid(winItem(handle,NO_FILE_TIP),parame[0]);
	return 0;
}
static int noFileCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("noFile Close Win!!!\n");
	return 0;
}
static int noFileWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("noFile WinChild Close!!!\n");
	return 0;
}
static int nofileMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
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


msgDealInfor noFileMsgDeal[]=
{
	{KEY_EVENT_MODE,noFileKeyMsgExit},
	{SYS_OPEN_WINDOW,noFileOpenWin},
	{SYS_CLOSE_WINDOW,noFileCloseWin},
	{SYS_CHILE_COLSE,noFileWinChildClose},
	{SYS_EVENT_SDC,noFileSysMsgSD},
	{KEY_EVENT_POWER,nofileMsgExit},
	{EVENT_MAX,NULL},
};

WINDOW(noFileWindow,noFileMsgDeal,noFileWin)



