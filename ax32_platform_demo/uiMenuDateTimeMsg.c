#include "uiMenuDateTimeRes.c"
#define DATE_TIME_ITEM_MAX      6//8//20220823
#define DATE_TIME_STR_LEN       10
static char dateTimeStr[6][DATE_TIME_STR_LEN];
static uint32 dateTimeData[6];
static unsigned char daysPerMon[12]={31,28,31,30,31,30,31,31,30,31,30,31};

void dateTimeStrUpdate(uint32 num)
{
	if(num<6)
		intToString(&dateTimeStr[num],DATE_TIME_STR_LEN,dateTimeData[num]);
}
void dateTimeIncreace(uint32 num)
{
	if(num>=6)
		return;
	dateTimeData[num]+=1;
	if(num==0&&dateTimeData[num]>2050)
		dateTimeData[num]=1990;
	if(num==1&&dateTimeData[num]>12)
		dateTimeData[num]=1;
	if(num==2&&dateTimeData[num]>daysPerMon[dateTimeData[num-1]-1])
		dateTimeData[num]=1;
	if(num==3&&dateTimeData[num]>23)
		dateTimeData[num]=0;
	if(num==4&&dateTimeData[num]>59)
		dateTimeData[num]=0;
	if(num==5&&dateTimeData[num]>59)
		dateTimeData[num]=0;
	dateTimeStrUpdate(num);
}
void dateTimeDecreace(uint32 num)
{
	if(num>=6)
		return;
	dateTimeData[num]-=1;
	if(num==0&&dateTimeData[num]<1990)
		dateTimeData[num]=2050;
	if(num==1&&dateTimeData[num]==0)
		dateTimeData[num]=12;
	if(num==2&&dateTimeData[num]==0)
		dateTimeData[num]=daysPerMon[dateTimeData[num-1]-1];
	if(num==3&&dateTimeData[num]>23)
		dateTimeData[num]=23;
	if(num==4&&dateTimeData[num]>59)
		dateTimeData[num]=59;
	if(num==5&&dateTimeData[num]>59)
		dateTimeData[num]=59;
	dateTimeStrUpdate(num);
}

static uint32 getDateTimeResInfor(uint32 item,uint32* image,uint32* str)
{
	if(item<6)
	{
		if(image)
			*image=INVALID_RES_ID;
		if(str)
			*str=(uint32)(&dateTimeStr[item]);
	}
	else if(item==6)
	{
		if(image)
			*image=INVALID_RES_ID;
		if(str)
			*str=R_ID_STR_COM_OK;
	}
	else if(item==7)
	{
		if(image)
			*image=INVALID_RES_ID;
		if(str)
			*str=R_ID_STR_COM_CANCEL;
	}
	else
	{
		if(image)
			*image=INVALID_RES_ID;
		if(str)
			*str=INVALID_RES_ID;
	}
	return 0;
}

static int dateTimeKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	DATE_TIME_T rtcTime;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,DATETIME_SELECT_ID));

		if(item<5)
			itemManageNextItem(winItem(handle,DATETIME_SELECT_ID));
		else if(item==5)
		{
			winDestroy(&handle);
			rtcTime.year = dateTimeData[0];
			rtcTime.month =dateTimeData[1];
			rtcTime.day = dateTimeData[2];
			rtcTime.hour = dateTimeData[3];
			rtcTime.min = dateTimeData[4];
			rtcTime.sec = dateTimeData[5];		
			hal_rtcTimeSet(&rtcTime);
			winDestroy(&handle);
		}
		else if(item==7)
			winDestroy(&handle);
	}
	return 0;
}
static int dateTimeKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		item=itemManageGetCurrentItem(winItem(handle,DATETIME_SELECT_ID));
		if(item<6)
		{
			dateTimeIncreace(item);
			//itemManageSetCurItem(winItem(handle,DATETIME_SELECT_ID),item);
			itemManageUpdateCurItem(winItem(handle,DATETIME_SELECT_ID));
		}
		else
			itemManagePreItem(winItem(handle,DATETIME_SELECT_ID));
	}
	return 0;
}
static int dateTimeKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		item=itemManageGetCurrentItem(winItem(handle,DATETIME_SELECT_ID));
		if(item<6)
		{
			dateTimeDecreace(item);
			//itemManageSetCurItem(winItem(handle,DATETIME_SELECT_ID),item);
			itemManageUpdateCurItem(winItem(handle,DATETIME_SELECT_ID));
		}
		else
		  itemManageNextItem(winItem(handle,DATETIME_SELECT_ID));
	}
	return 0;
}
static int dateTimeKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int dateTimeKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//taskChange();
		winDestroy(&handle);
	}
	return 0;
}
static int dateTimeOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	DATE_TIME_T *rtcTime;
	deg_Printf("dateTime Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,DATETIME_SELECT_ID),/*3*/2,R1h(32));
	#if SMALL_PANEL_SUPPORT==0
	itemManageSetColumnSumWithGap(winItem(handle,DATETIME_SELECT_ID),0,3,R1w(50),R1w(3));
	itemManageSetColumnSumWithGap(winItem(handle,DATETIME_SELECT_ID),1,3,R1w(40),R1w(3));
	itemManageSetColumnSumWithGap(winItem(handle,DATETIME_SELECT_ID),2,2,R1w(50),R1w(12));
	#else
	itemManageSetColumnSumWithGap(winItem(handle,DATETIME_SELECT_ID),0,3,R1w(60),R1w(3));
	itemManageSetColumnSumWithGap(winItem(handle,DATETIME_SELECT_ID),1,3,R1w(30),R1w(3));
	itemManageSetColumnSumWithGap(winItem(handle,DATETIME_SELECT_ID),2,2,R1w(60),R1w(6));
	#endif
	itemManageCreateItem(winItem(handle,DATETIME_SELECT_ID),itemCreatemenuOption,getDateTimeResInfor,DATE_TIME_ITEM_MAX);
	itemManageSetCharInfor(winItem(handle,DATETIME_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,DATETIME_SELECT_ID),R_COLOR_YELLOW);
	itemManageSetUnselectColor(winItem(handle,DATETIME_SELECT_ID),R_COLOR_BLUE1);
	rtcTime = hal_rtcTimeGet();
	dateTimeData[0]=rtcTime->year;
	dateTimeData[1]=rtcTime->month;
	dateTimeData[2]=rtcTime->day;
	dateTimeData[3]=rtcTime->hour;
	dateTimeData[4]=rtcTime->min;
	dateTimeData[5]=rtcTime->sec;
	dateTimeStrUpdate(0);
	dateTimeStrUpdate(1);
	dateTimeStrUpdate(2);
	dateTimeStrUpdate(3);
	dateTimeStrUpdate(4);
	dateTimeStrUpdate(5);
	itemManageSetCurItem(winItem(handle,DATETIME_SELECT_ID),0);
	
	return 0;
}
static int dateTimeCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("dateTime Close Win!!!\n");
	return 0;
}
static int dateTimeWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("dateTime WinChild Close!!!\n");
	return 0;
}
static int dateTimeSysMsg500Ms(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

msgDealInfor dateTimeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,dateTimeOpenWin},
	{SYS_CLOSE_WINDOW,dateTimeCloseWin},
	{SYS_CHILE_COLSE,dateTimeWinChildClose},
	{KEY_EVENT_OK,dateTimeKeyMsgOk},
	{KEY_EVENT_UP,dateTimeKeyMsgUp},
	{KEY_EVENT_DOWN,dateTimeKeyMsgDown},
	{KEY_EVENT_MENU,dateTimeKeyMsgMenu},
	{KEY_EVENT_MODE,dateTimeKeyMsgMode},
	{EVENT_MAX,NULL},
};

WINDOW(dateTimeWindow,dateTimeMsgDeal,dateTimeWin)


