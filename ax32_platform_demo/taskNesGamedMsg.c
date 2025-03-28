#include"taskNesGameRes.c"

#define R_COLOR_OUT			0xfe
#define R_COLOR_IN			0xfd
#define R_COLOR_FILE		0xfc

#define NES_GAME_NUM 		6


static u32 NesGame_str_id[NES_GAME_NUM]={R_ID_STR_GAME_001,R_ID_STR_GAME_002,R_ID_STR_GAME_003,R_ID_STR_GAME_004,
											R_ID_STR_GAME_005,R_ID_STR_GAME_007};


static uint32 getNesGameResInfor(uint32 item,uint32* image,uint32* str)
{

	if(image)
			*image=INVALID_RES_ID;
	if(str)
			*str= NesGame_str_id[item];////R_ID_STR_GAME_001+item;

	return 0;
}
static int nesGameBack(winHandle handle,uint32 parameNum,uint32* parame)
{
	//taskNesGameClose();
	//itemManageSetCurItem(winItem(handle,NES_SELECT_ID),0);
}
static int nesGameOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{

	deg_Printf("nesGame Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,NES_SELECT_ID),4,R1h(50));
	
	itemManageSetColumnSumWithGap(winItem(handle,NES_SELECT_ID),0,1,R1w(200),R1w(0));
	itemManageSetColumnSumWithGap(winItem(handle,NES_SELECT_ID),1,1,R1w(000),R1w(0));
	itemManageSetColumnSumWithGap(winItem(handle,NES_SELECT_ID),2,1,R1w(200),R1w(0));
	itemManageSetColumnSumWithGap(winItem(handle,NES_SELECT_ID),3,1,R1w(200),R1w(0));


	itemManageCreateItem(winItem(handle,NES_SELECT_ID),itemCreatemenuOption,getNesGameResInfor,/*7*/NES_GAME_NUM);
	itemManageSetCharInfor(winItem(handle,NES_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	//temManageSetSelectColor(winItem(handle,NES_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,NES_SELECT_ID),R_COLOR_IN);

	resInfor infor;
	resInforInit(&infor);
	infor.font=0;
	infor.strAlign=ALIGNMENT_CENTER;
	infor.fontColor=/*R_COLOR_GROUND2*/R_COLOR_FILE;
	infor.bgColor=INVALID_COLOR;
	winSetSelectInfor(winItem(handle,NES_SELECT_ID),&infor);
	////itemManageSetSelectImage(winItem(handle,NES_SELECT_ID),R_ID_ICON_BARSEL);
	itemManageSetCurItem(winItem(handle,NES_SELECT_ID),0);
	return 0;
}
static int nesGameCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	
	return 0;
}
static int nesGameWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int nesGameKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,NES_SELECT_ID));
		NesGameOpen(item);
	}
	return 0;
}
static int nesGameKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		itemManagePreItem(winItem(handle,NES_SELECT_ID));
	}
	return 0;
}
static int nesGameKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		itemManageNextItem(winItem(handle,NES_SELECT_ID));
	}
	return 0;
}
static int nesGameKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int nesGameKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		taskStart(TASK_MAIN_MENU,0);
	}
	return 0;
}
static int nesGameSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	int sum;
	deg_Printf("player : sdc stat ->%d\n",SysCtrl.sdcard);
	return 0;
}

static int nesGameSysMsgTimeUpdate(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

static int nesGameMsgRight(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_RELEASE)
	{
	}
	return 0;
}

msgDealInfor nesGameMsgDeal[]=
{
	{SYS_OPEN_WINDOW,nesGameOpenWin},
	{SYS_CLOSE_WINDOW,nesGameCloseWin},
	{SYS_CHILE_COLSE,nesGameWinChildClose},
	{KEY_EVENT_OK,nesGameKeyMsgOk},
	{KEY_EVENT_UP,nesGameKeyMsgUp},
	{KEY_EVENT_DOWN,nesGameKeyMsgDown},
	{KEY_EVENT_MENU,nesGameKeyMsgMenu},
	{KEY_EVENT_POWER,nesGameKeyMsgMode},
	{KEY_EVENT_MODE,nesGameMsgRight},
	{SYS_EVENT_SDC,nesGameSysMsgSD},
	{SYS_EVENT_TIME_UPDATE,nesGameSysMsgTimeUpdate},
//	{SYS_EVNET_GAMEBACK,nesGameBack},
	{EVENT_MAX,NULL},
};

WINDOW(nesGameWindow,nesGameMsgDeal,nesGameWin)


