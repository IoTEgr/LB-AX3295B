#if 0

#include "uiThumbnallRes.c"

#define toTypeId(id)  ((id)+2)
extern int drawImageToFrame(disp_frame_t* p_frame,int index,u16 outX,u16 outY,u16 outW,u16 outH);
static int drawImageToFrameByID(disp_frame_t* p_lcd_buffer,int index,winHandle handle,thumbnallID id)
{
	uiRect rect;
	u16 xOffset;
	u16 yOffset;
	winGetPos(winItem(handle,id),&rect);
	return drawImageToFrame(p_lcd_buffer,index,rect.x0,rect.y0,rect.x1-rect.x0+1,rect.y1-rect.y0+1);
	//xOffset=((rect.x1-rect.x0+1)&(0x1f))>>1;
	//yOffset=((rect.y1-rect.y0+1)&(0x1f))>>1;
	//return drawImageToFrame(p_lcd_buffer,index,rect.x0+xOffset,rect.y0+yOffset,(rect.x1-rect.x0+1)&(~0x1f),(rect.y1-rect.y0+1)&(~0x1f));
}
static void showImageThumb(int indexStart,u32 num,winHandle handle)
{
	u16 x,y,i;
	INT16U width;
	INT16U height;
	uiRect rect;
	disp_frame_t *p_lcd_buffer;
	if(num>THUMBNALL_NUM)
		num=THUMBNALL_NUM;
	p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(p_lcd_buffer==NULL)
		return;
	hal_lcdGetOSDShowingPosition(&x,&y);
	hal_lcdGetOSDShowingResolution(&width,&height);
	hal_dispframeVideoCfg(p_lcd_buffer,x,y,width,height);
	hal_lcdSetBufYUV(p_lcd_buffer,0,0x80,0x80);
	for(i=0;i<num;i++)
	{
		if(drawImageToFrameByID(p_lcd_buffer,indexStart+i,handle,thumbnallIDnum[i])<0)
			winSetResid(winItem(handle,toTypeId(thumbnallIDnum[i])),strToResId("error"));
		else if(SysCtrl.file_type==FILE_TYPE_JPG)
			winSetResid(winItem(handle,toTypeId(thumbnallIDnum[i])),strToResId("jpg"));
		else
			winSetResid(winItem(handle,toTypeId(thumbnallIDnum[i])),strToResId("avi"));
	}
	while(num<THUMBNALL_NUM)
		winSetResid(winItem(handle,toTypeId(thumbnallIDnum[num++])),strToResId(" "));
	dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,width,height,VIDEO_ROTATE);
}
static void showImageThumbValid(winHandle handle)
{
	INT32S indexStart;
	int fileNum;
	fileNum=managerFileCount(SysCtrl.avi_list);
	if(fileNum<1)
		return;
	indexStart=((SysCtrl.file_index-1)/THUMBNALL_NUM)*THUMBNALL_NUM+1;
	thumbnallShowPage(handle,(SysCtrl.file_index-1)/THUMBNALL_NUM+1,fileNum/THUMBNALL_NUM+((fileNum%THUMBNALL_NUM)?1:0));
	showImageThumb(indexStart,fileNum-indexStart+1,handle);
}

static int thumbnallKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
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
static int thumbnallKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fileNum,lastIndexStart,curIndexStart;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		fileNum=managerFileCount(SysCtrl.avi_list);
		if(fileNum<2)
			return 0;
		lastIndexStart=((SysCtrl.file_index-1)/THUMBNALL_NUM)*THUMBNALL_NUM+1;
		thumbnallSelect(handle,SysCtrl.file_index-lastIndexStart,false);
		SysCtrl.file_index--;
			if(SysCtrl.file_index<1)
				SysCtrl.file_index = fileNum;
		curIndexStart=((SysCtrl.file_index-1)/THUMBNALL_NUM)*THUMBNALL_NUM+1;
		if(curIndexStart!=lastIndexStart)
		{
			showImageThumbValid(handle);
			SysCtrl.file_index=curIndexStart+THUMBNALL_NUM-1;
			if(SysCtrl.file_index>fileNum)
				SysCtrl.file_index=fileNum;
			thumbnallSelect(handle,SysCtrl.file_index-curIndexStart,true);
		}
		else
			thumbnallSelect(handle,SysCtrl.file_index-curIndexStart,true);
	}
	return 0;
}
static int thumbnallKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	int fileNum,lastIndexStart,curIndexStart;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		fileNum=managerFileCount(SysCtrl.avi_list);
		if(fileNum<2)
			return 0;
		lastIndexStart=((SysCtrl.file_index-1)/THUMBNALL_NUM)*THUMBNALL_NUM+1;
		thumbnallSelect(handle,SysCtrl.file_index-lastIndexStart,false);
		SysCtrl.file_index++;
		if(SysCtrl.file_index>fileNum)
			SysCtrl.file_index = 1;
		curIndexStart=((SysCtrl.file_index-1)/THUMBNALL_NUM)*THUMBNALL_NUM+1;
		if(curIndexStart!=lastIndexStart)
		{
			showImageThumbValid(handle);
			SysCtrl.file_index=curIndexStart;
			thumbnallSelect(handle,0,true);
		}
		else
			thumbnallSelect(handle,SysCtrl.file_index-curIndexStart,true);
	}
	return 0;
}
static int thumbnallKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int thumbnallKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
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
static int thumbnallSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("thumbnall : sdc stat ->%d\n",SysCtrl.sdcard);
	if((SysCtrl.sdcard != SDC_STAT_NORMAL)/* && (state == MEDIA_STAT_PLAY)*/) // sdc out when recording
	{
		uiOpenWindow(&noFileWindow,0);
	}
	else
	{
		SysCtrl.file_index = managerFileCount(SysCtrl.avi_list);
		if(SysCtrl.file_index<1)
			uiOpenWindow(&noFileWindow,0);
	}
	return 0;
}

static int thumbnallSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb != USB_STAT_NULL)
	{
	}
	return 0;
}
static int thumbnallSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}
static int thumbnallOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	u32 i;
	INT32S indexStart,tem;
	playEnSet(false);
	tem=SysCtrl.file_index;
	indexStart=((SysCtrl.file_index-1)/THUMBNALL_NUM)*THUMBNALL_NUM+1;
	if(videoPlaybackGetStatus() != MEDIA_STAT_STOP)
		videoPlaybackStop();
	for(i=0;i<THUMBNALL_NUM;i++)
		thumbnallSelect(handle,i,false);
	showImageThumbValid(handle);
	SysCtrl.file_index=tem;
	thumbnallSelect(handle,SysCtrl.file_index-indexStart,true);
	deg_Printf("thumbnall Open Win!!!\n");
	return 0;
}
static int thumbnallCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("thumbnall Close Win!!!\n");
	return 0;
}
static int thumbnallWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	u32 i;
	INT32S indexStart,tem;
	if(managerFileCount(SysCtrl.avi_list)<1)
	{
		uiOpenWindow(&noFileWindow,0);
		return 0;
	}
	playEnSet(false);
	tem=SysCtrl.file_index;
	indexStart=((SysCtrl.file_index-1)/THUMBNALL_NUM)*THUMBNALL_NUM+1;
	if(videoPlaybackGetStatus() != MEDIA_STAT_STOP)
		videoPlaybackStop();
	for(i=0;i<THUMBNALL_NUM;i++)
		thumbnallSelect(handle,i,false);
	showImageThumbValid(handle);
	SysCtrl.file_index=tem;
	thumbnallSelect(handle,SysCtrl.file_index-indexStart,true);
	deg_Printf("thumbnall WinChild Close!!!\n");
	return 0;
}
static int thumbnallSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

msgDealInfor thumbnallMsgDeal[]=
{
	{SYS_OPEN_WINDOW,thumbnallOpenWin},
	{SYS_CLOSE_WINDOW,thumbnallCloseWin},
	{SYS_CHILE_COLSE,thumbnallWinChildClose},
	{KEY_EVENT_OK,thumbnallKeyMsgOk},
	{KEY_EVENT_UP,thumbnallKeyMsgUp},
	{KEY_EVENT_DOWN,thumbnallKeyMsgDown},
	{KEY_EVENT_MENU,thumbnallKeyMsgMenu},
	{KEY_EVENT_MODE,thumbnallKeyMsgMode},
	{SYS_EVENT_SDC,thumbnallSysMsgSD},
	{SYS_EVENT_USB,thumbnallSysMsgUSB},
	{SYS_EVENT_BAT,thumbnallSysMsgBattery},
	{SYS_EVENT_1S,thumbnallSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(thumbnallWindow,thumbnallMsgDeal,thumbnallWin)

#endif

