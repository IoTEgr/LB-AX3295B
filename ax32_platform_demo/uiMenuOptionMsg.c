#include "uiMenuOptionRes.c"

static menuItem* currentItem=NULL;
static uint8 SDC_NULL_flag=false;
static uint32 getOptionResInfor(uint32 item,uint32* image,uint32* str)
{
	if(image)
		*image=currentItem->pOption[item].image;
	if(str)
		*str=currentItem->pOption[item].str;
}

static int  menuformat(winHandle handle,uint32 parameNum,uint32* parame)
{
	INT8U *mem;
	INT8U res;

	if(SDC_STAT_NULL==SysCtrl.sdcard)
	{
		uiOpenWinAndShow(&tipsWindow,2,R_ID_STR_TIPS_PLEASE_WAITING,5);
		spi_udisk_format(FLASH_CAPACITY);
		uiOpenWindow(&tipsWindow,2,R_ID_STR_TIPS_FMT_SUCCESS,2);
	}
	else
	{

		if(SysCtrl.sdcard == SDC_STAT_NULL)
		{
			uiOpenWindow(&tipsWindow,2,TIPS_SD_NOT_INSERT,2);
			SDC_NULL_flag = true;
			return 0;
		}

		mem = (INT8U *)hal_sysMemMalloc(32768,64);
		if(mem == NULL)
		{
			deg_Printf("malloc mem for format fail.\n");
			uiOpenWindow(&tipsWindow,2,"memory malloc failed",2);
			//winDestroy(&handle);
			SDC_NULL_flag = true;
			return 0;
		}
		uiOpenWinAndShow(&tipsWindow,2,R_ID_STR_TIPS_PLEASE_WAITING,5);
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
		SDC_NULL_flag = true;
		//winDestroy(&handle);
	}
	return 0;
}
extern void board_lcd_ioctrl_pwm(uint8 data );

static int menuOptionKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(currentItem->configId == CONFIG_ID_FORMAT)
		{
			if(currentItem->pOption[itemManageGetCurrentItem(winItem(handle,OPTION_SELECT_ID))].str == R_ID_STR_COM_OK)
			{
				menuformat(handle,parameNum,parame);
			}
			else
			{
				winDestroy(&handle);
			}
		}
		else if(currentItem->configId == CONFIG_ID_DEFUALT)
		{
			if(currentItem->pOption[itemManageGetCurrentItem(winItem(handle,OPTION_SELECT_ID))].str == R_ID_STR_COM_OK)
			{
				userConfigReset();
				configSystem();		

				board_lcd_ioctrl_pwm(configValue2Int(CONFIG_ID_LCD_BRIGHT));
			}
			else
			{
			}
			winDestroy(&handle);
		}
		else
		{
			if(currentItem)
			{
				configSet(currentItem->configId,currentItem->pOption[itemManageGetCurrentItem(winItem(handle,OPTION_SELECT_ID))].str);
				configSys(currentItem->configId);
				userConfigSave();
			}
			
			if(currentItem->configId == CONFIG_ID_LCD_BRIGHT)
			{
				board_lcd_ioctrl_pwm(configValue2Int(CONFIG_ID_LCD_BRIGHT));
			}
			
			winDestroy(&handle);
		}
	}
	return 0;
}
static int menuOptionKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		itemManagePreItem(winItem(handle,OPTION_SELECT_ID));

		if(currentItem->configId == CONFIG_ID_LCD_BRIGHT)
		{
			board_lcd_ioctrl_pwm(itemManageGetCurrentItem(winItem(handle,OPTION_SELECT_ID))+1);
		}
	}
	return 0;
}
static int menuOptionKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		itemManageNextItem(winItem(handle,OPTION_SELECT_ID));
		
		if(currentItem->configId == CONFIG_ID_LCD_BRIGHT)
		{
			board_lcd_ioctrl_pwm(itemManageGetCurrentItem(winItem(handle,OPTION_SELECT_ID))+1);
		}
	}
	return 0;
}
static int menuOptionKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int menuOptionKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
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
static int menuOptionSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int menuOptionOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 itemNum=0,config,i;
	if(parameNum<1)
	{
		winDestroy(&handle);
		return 0;
	}
	currentItem=(menu*)parame[0];
	deg_Printf("menuOption Open Win!!!\n");
	itemNum=itemManageSetHeightAvgGap(winItem(handle,OPTION_SELECT_ID),R1h(/*32*/25));
	//for(i=0;i<itemNum;i++)
	//	itemManageSetColumnMargin(winItem(handle,OPTION_SELECT_ID),i,1,5);
	itemManageCreateItem(winItem(handle,OPTION_SELECT_ID),itemCreatemenuOption,getOptionResInfor,currentItem->optionSum);
	itemManageSetCharInfor(winItem(handle,OPTION_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,OPTION_SELECT_ID),/*R_COLOR_BLUE1*/R_COLOR_YELLOW);
	itemManageSetUnselectColor(winItem(handle,OPTION_SELECT_ID),/*R_COLOR_BLACK*/R_COLOR_DBLUE);
	config=configGet(currentItem->configId);
	itemNum=0;
	while(itemNum<currentItem->optionSum)
	{
		if(currentItem->pOption[itemNum].str==config)
			break;
		itemNum++;
	}
	if(itemNum>=currentItem->optionSum)
		itemNum=0;
	itemManageSetCurItem(winItem(handle,OPTION_SELECT_ID),itemNum);
	//winSetResid(winItem(handle,OPTION_TITLE_ID),currentItem->str);
	return 0;
}
static int menuOptionCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("menuOption Close Win!!!\n");
	currentItem=NULL;
	return 0;
}
static int menuOptionWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("menuOption WinChild Close!!!\n");
	return 0;
}
static int menuOptionSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if( SDC_NULL_flag == true)
	{
		SDC_NULL_flag = false;
		winDestroy(&handle);
	}
	return 0;
}
static int menuOptionKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
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

msgDealInfor menuOptionMsgDeal[]=
{
	{SYS_OPEN_WINDOW,menuOptionOpenWin},
	{SYS_CLOSE_WINDOW,menuOptionCloseWin},
	{SYS_CHILE_COLSE,menuOptionWinChildClose},
	{KEY_EVENT_POWER,menuOptionKeyMsgPower},
	{KEY_EVENT_OK,menuOptionKeyMsgOk},
	{KEY_EVENT_UP,menuOptionKeyMsgUp},
	{KEY_EVENT_DOWN,menuOptionKeyMsgDown},
	{KEY_EVENT_MENU,menuOptionKeyMsgMenu},
	{KEY_EVENT_MODE,menuOptionKeyMsgMode},
	{SYS_EVENT_SDC,menuOptionSysMsgSD},
	{SYS_EVENT_500MS,menuOptionSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(menuOptionWindow,menuOptionMsgDeal,menuOptionWin)


