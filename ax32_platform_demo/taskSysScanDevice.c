/****************************************************************************
**
 **                              TASK DEMO
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                      TASK DEAMON
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : taskSysScanDevice.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : 
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "application.h"

#define SDC_WAIT_TIME   50 // 500ms       
static int keyTime=0;
extern bool sysPowerOn;
void deamon_sdc_check(void);
void deamon_usb_check(void);
void deamon_gsensor_check(void);
void deamon_key_check(void);
void deamon_astern_check(void);

static void deamon_ir_auto_check(void)
{

#if (1 == IR_MENU_EN)
/*
	uint32 irState=0;
	static uint8 ir_close_cnt=100;//100X10ms=1 s
	if(configGet(CONFIG_ID_IR_LED)!=R_ID_STR_IR_AUTO)
		return;
	if(isp_get_scen_light() < 0xa)		// need ir
	{
		boardIoctrl(SysCtrl.bfd_ir,IOGET_IR_GET,&irState);
		if(irState==0)
			boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,1);

	}
	
	if(isp_get_scen_light() > 0x30)
	{
		ir_close_cnt--;
		if(ir_close_cnt==0)// wait for stable
		{
			boardIoctrl(SysCtrl.bfd_ir,IOGET_IR_GET,&irState);
			if(irState)
			boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,0);
			ir_close_cnt=100;
		}
	}
*/
#endif
}
extern void resBuffConut(void);
extern void taskVideoRecTimeIncreace(void);
void timerMsgSend(void)
{
	static u8 lastSec=255;
	static uint32 lastTime = 0;
	HAL_RTC_T* rtcTime;
	rtcTime=hal_rtcTimeGet();
	if(lastSec!=rtcTime->sec)
	{
		lastSec=rtcTime->sec;
		SysCtrl.powerOnTime++;
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_1S,0));
		taskVideoRecTimeIncreace();
		resBuffConut();
	}
	
	if((XOSTimeGet()-lastTime)<=500)
		return;
	lastTime = XOSTimeGet();
	XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_500MS,0));
}
/*******************************************************************************
* Function Name  : taskSysScanDev
* Description    : system service in deamon
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
void taskSysScanDev(uint32 scanKey)
{
	static uint32 lastTime = 0;
	timerMsgSend();
	//hal_ispService();
	#if CFG_MCU_TFDBG_EN
	//	hal_check_tfdebg();
	#endif
//--------key check -------------------
	if(scanKey)
		deamon_key_check();      // system key read
		
	if((XOSTimeGet()-lastTime)<=10*X_TICK)
		return ;
	lastTime = XOSTimeGet();
    if(taskCurrent() != TASK_POWER_OFF)
    {
	   // boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_CHECK,1);  // SYSTEM CHECK LED STATE
//-------sdc card check-----------------    
	    deamon_sdc_check();  // sdc state check
//--------gsensor check-----------------
	    //deamon_gsensor_check(); // gsensor state check
//--------sereen save check ------------	
	    deamon_screen_save(0);  // system check,no event
//--------auto power off check----------
	    deamon_auto_poweroff(0); //auto power off check 
//--------astern mode check-----------
        //deamon_astern_check(); // astern stat check
//--------ir auto check-------------
		//deamon_ir_auto_check();
    }
//--------usb check---------------------
	deamon_usb_check();  // usb state check
//--------motion dection check----------
/*	if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		if(hal_mdCheck())
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_MD,1));
	}*/
#if CMOS_USART_ONLINE_DBG >0
    sesnorDBGService();
#endif

    //LogShowCheck();
//	logFlush();
}
static bool keyEn=true;
void setKeyEn(bool en)
{
	keyEn=en;
}
/*******************************************************************************
* Function Name  : deamon_key_check
* Description    : key check value
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
void deamon_key_check(void)
{
//	static uint32 count = 0;
	static INT32U TimeInterval=10000;
	static uint32 lastTime = 0;
	static uint32 LastTime = 0;
//	INT32U CurTime;
	uint32 lcdOn=0;
	if((XOSTimeGet()-lastTime)<=10)
		return ;
	lastTime = XOSTimeGet();
	if(SysCtrl.bfd_key<0)
		return ;
	int adcValue;
	if(boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_READ,(INT32U )&adcValue)>=0)
	{
		if(adcValue ) // astern mode do not check key 
		{
			//debg("adcValue:%x\n",adcValue);
			if(getType(adcValue)==KEY_EVENT_POWEROFF||getType(adcValue)==KEY_EVENT_POWER)
			{
				XMsgQPost(SysCtrl.sysQ,(void*)adcValue);
			}
			else
			{
				boardIoctrl(SysCtrl.bfd_lcd,IOGET_LCD_BKLIGHT,(u32)&lcdOn);
				if(keyEn&&lcdOn)
					XMsgQPost(SysCtrl.sysQ,(void*)adcValue);
				else
				{
					deamon_screen_save(1);
					if(getValue(adcValue)==KEY_RELEASE)
						keyEn=true;
				}
			}
			keyTime =LastTime= XOSTimeGet();
			TimeInterval=10000;
		}
	}
#if 0
	else
	{
	CurTime=XOSTimeGet();
	if(CurTime-LastTime>TimeInterval)
	{
		LastTime=CurTime;
		TimeInterval=500;
		if(CurTime&2)
			XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_MODE,KEY_PRESSED));
		if(CurTime&1)
			XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_MENU,KEY_PRESSED));
		else
			XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_OK,KEY_PRESSED));
	}
}
#endif
}
void deamon_fs_scan(void)
{
	deg_Printf("deamon : fs mount start.%d\n",XOSTimeGet());
	
	char string[16];
#if(AUDIO_REC_EN==1)
	strcpy(string,FILEDIR_AUDIO);
	string[strlen(string)-1] = 0;
	f_mkdir((const TCHAR*)string);//FILEDIR_AUDIO);
#endif
	strcpy(string,FILEDIR_VIDEOA);
	string[strlen(string)-1] = 0;
	f_mkdir((const TCHAR*) string);//FILEDIR_VIDEO);

#if (1 == AVI_LOCK_FOLDER)
	strcpy(string,FILEDIR_LOCKA);
	string[strlen(string)-1] = 0;
	f_mkdir((const TCHAR*) string);
	strcpy(string,FILEDIR_LOCKB);
	string[strlen(string)-1] = 0;
	f_mkdir((const TCHAR*) string);
#endif

#if RECORD_MODE == RECORD_MODE_DEFAULT	
	strcpy(string,FILEDIR_VIDEOB);
	string[strlen(string)-1] = 0;
	f_mkdir((const TCHAR*)string);//FILEDIR_VIDEO);
	SysCtrl.bfolder = 1;
#elif RECORD_MODE == RECORD_MODE_AUTO	
    if(SysCtrl.usensor!=USENSOR_STAT_NULL)
    {
		strcpy(string,FILEDIR_VIDEOB);
		string[strlen(string)-1] = 0;
		f_mkdir((const TCHAR*)string);//FILEDIR_VIDEO);
		SysCtrl.bfolder = 1;
    }
	else
		SysCtrl.bfolder = 0;
#else
    // no B floder
    SysCtrl.bfolder = 0;
#endif

	strcpy(string,FILEDIR_PHOTO);
	string[strlen(string)-1] = 0;
	f_mkdir((const TCHAR*)string);//FILEDIR_PHOTO);

	strcpy(string,FILEDIR_MP3);
	string[strlen(string)-1] = 0;
	f_mkdir((const TCHAR*)string);

	
	hal_sdStop();					
    
	SysCtrl.fs_clustsize = fs_getclustersize();
	deg_Printf("deamon : fs cluster size.%d B\n",SysCtrl.fs_clustsize);
	SysCtrl.sdcard = SDC_STAT_NORMAL; 
    XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
	if(SysCtrl.avi_list<0) // scan file list
	{
		SysCtrl.avi_list = managerCreate(FILEDIR_VIDEO,MA_FILE_AVI,-1);
		SysCtrl.jpg_list = managerCreate(FILEDIR_VIDEO,MA_FILE_JPG,SysCtrl.avi_list); // using the same list with avi_list
		SysCtrl.avia_list = managerCreate(FILEDIR_VIDEO,MA_FILE_AVIA,SysCtrl.avi_list); // using the same list with avi_list
		//SysCtrl.avib_list = managerCreate(FILEDIR_VIDEO,MA_FILE_AVIB,SysCtrl.avi_list); // using the same list with avi_list
		managerScanStart(SysCtrl.avi_list);
		managerFileScan(SysCtrl.avi_list,FILEDIR_VIDEOA);
		//managerFileScan(SysCtrl.avi_list,FILEDIR_VIDEOB);
		#if (1 == AVI_LOCK_FOLDER)
		managerFileScan(SysCtrl.avi_list,FILEDIR_LOCKA);
		//managerFileScan(SysCtrl.avi_list,FILEDIR_LOCKB);
		#endif

		if(strcmp(FILEDIR_PHOTO,FILEDIR_VIDEO)!=0)
			managerFileScan(SysCtrl.jpg_list,FILEDIR_PHOTO);
		managerScanEnd(SysCtrl.avi_list);
	}
#if(AUDIO_REC_EN==1)
	if(SysCtrl.wav_list<0)
    {
		SysCtrl.wav_list = managerCreate(FILEDIR_AUDIO,MA_FILE_WAV,-1);
		deg_Printf("SysCtrl.wav_list %d\n",SysCtrl.wav_list);
		managerScanStart(SysCtrl.wav_list);
		managerFileScan(SysCtrl.wav_list ,FILEDIR_AUDIO);
		managerScanEnd(SysCtrl.wav_list);
    }
#endif
	//if(menuActiveGet())
	{
		deamon_fsFreeSize();
		task_video_record_caltime();
	}/**/
	hal_sdUnlock();
	XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_SDC,SysCtrl.sdcard));
	deg_Printf("deamon : sdc normal\n",XOSTimeGet());
}
/*******************************************************************************
* Function Name  : deamon_sdc_check
* Description    : sd card stat check
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
void deamon_sdc_check(void)
{
	static int sdccnt=SDC_WAIT_TIME/*the initiled value is used when power on*/,scanFlag=0/*,scantime=0*/;
	int temp,res;

	if(scanFlag /*&& (task_get_curstate()!=MAIN_STAT_POWER_ON)*/)
	{
	//	if(scanFlag)
	//		scantime++;
	//	if(scantime>=10)
		{
			deamon_fs_scan();
			scanFlag = 0;
	//		scantime= 0;
		}
	}
//-------------mark modify,2017/11/30.when sdc pulled in,the power of sdc may not stable,wait about 500ms	
    if(SysCtrl.bfd_sdcard<0)
		return ;
	boardIoctrl(SysCtrl.bfd_sdcard,IOCTRL_SDC_CHECK,(INT32U)&temp);
	if(SysCtrl.sdcard <= SDC_STAT_UNSTABLE)
	{
	#if SDC_WIDTH == 1
		if(hal_sdInit(SD_BUS_WIDTH1)>=0)
	#else
		if(hal_sdInit(SD_BUS_WIDTH4)>=0)
	#endif
		{
			if(SysCtrl.sdcard == SDC_STAT_NULL)
			{
			    SysCtrl.sdcard = SDC_STAT_UNSTABLE;
				deg_Printf("deamon : sdc pulled in.\n");
			}
			else if(SysCtrl.sdcard == SDC_STAT_UNSTABLE)
			{
			    sdccnt++;
				if(sdccnt<SDC_WAIT_TIME)  // 500 ms,wait sdc stable
				    return ;
				sdccnt = 0;
				deg_Printf("deamon : sdc stable.\n");
				res = fs_mount(0);
				if(res!=FR_OK)
				{
					hal_sdSetCardState(SDC_STATE_NULL);
					if(hal_sdInit(SD_BUS_WIDTH1)>=0) // sdc intial 1line
					{
						res = fs_mount(0);
					}
				}

				if(res<0)
				{
					SysCtrl.sdcard = SDC_STAT_ERROR; 
					XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_SDC,0));
					deg_Printf("deamon : sdc error.%d\n",res);
				}
				else
				{
                    scanFlag = 1;
					SysCtrl.sdcard = SDC_STAT_UNSTABLE; 
					deg_Printf("deamon : sdc mounted ok\n");
				}
		    }
			temp = 1;
		}
	}
	if(temp==0) // no sdc dectcted
	{
		if(SysCtrl.sdcard == SDC_STAT_NULL)
		{
			if(sdccnt>=SDC_WAIT_TIME)
				XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
		}
		else if(SysCtrl.sdcard == SDC_STAT_UNSTABLE)
		{
			deg_Printf("deamon : sdc pulled out\n");
			SysCtrl.sdcard = SDC_STAT_NULL;
		}
		else// if(SysCtrl.sdcard!=SDC_STAT_NULL)
		{
			fs_nodeinit();  // initial fs node
			managerDestory(SysCtrl.avi_list);
			managerDestory(SysCtrl.jpg_list);
			managerDestory(SysCtrl.avia_list);
			managerDestory(SysCtrl.avib_list);
			#if(AUDIO_REC_EN==1)
			managerDestory(SysCtrl.wav_list);
			#endif
			SysCtrl.avi_list = -1;
			SysCtrl.jpg_list = -1;
			SysCtrl.avia_list = -1;
			SysCtrl.avib_list = -1;
			SysCtrl.wav_list = -1;
		    SysCtrl.sdcard = SDC_STAT_NULL;  // sdc out
		    XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
			deamon_fsFreeSize();
		    task_video_record_caltime();
		    deg_Printf("deamon : sdc unmounted.\n");
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,SysCtrl.sdcard));
		}
		sdccnt = 0;
	}
	
}
/*******************************************************************************
* Function Name  : deamon_astern_check
* Description    : astern mode & backrec status 
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
void deamon_astern_check(void)
{
#if 0
	bool ret;

	api_backrec_linking();	

	ret = api_astern_status();
	if((SysCtrl.astern==ASTERN_OFF) && (ret == true))
	{
		deg_Printf("deamon : astern on\n");
		SysCtrl.astern = ASTERN_ON;
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_ASTERN,ASTERN_ON));
	}
	else if((SysCtrl.astern==ASTERN_ON) && (ret == false))
	{
		deg_Printf("deamon : astern off\n");
		SysCtrl.astern = ASTERN_OFF;
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_ASTERN,ASTERN_OFF));
	}

	ret = api_backrec_status();
	if((SysCtrl.usensorshow==USENSOR_SHOW_OFF) && (ret == true))
	{
		deg_Printf("deamon : uensor show on\n");
		SysCtrl.usensorshow = USENSOR_SHOW_ON;
		SysCtrl.t_rec = 0;
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_USENSOR,USENSOR_SHOW_ON));
	}
	else if((SysCtrl.usensorshow==USENSOR_SHOW_ON) && (ret == false))
	{
		deg_Printf("deamon : uensor show off\n");
		SysCtrl.usensorshow = USENSOR_SHOW_OFF;
		SysCtrl.t_rec = 0;
		api_astern_set_status(false);
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_USENSOR,USENSOR_SHOW_OFF));
	}
#endif
}
void reflush_usensor_check(void)
{
#if 0
	boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,0);
	deg_Printf("{deamon : usensor out}\n");
	SysCtrl.usensor=USENSOR_STAT_NULL;	
	SysCtrl.usensorshow = USENSOR_SHOW_OFF;
	SysCtrl.t_rec = 0;
	XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_USENSOR,USENSOR_STAT_NULL));
#endif
}
/*******************************************************************************
* Function Name  : deamon_usb_check
* Description    : usb & battery stat check
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
extern int uninit(void);
static BOOL softwareDetachUsensor=FALSE;
void setSoftwareDetachUsensor(BOOL en)
{
	softwareDetachUsensor=en;
}
extern s32 usb11HostCheckNoWait(void);
void deamon_usb_check(void)
{
	static INT32U usbdelay = 0;
	static INT32U usensordect_time = 0, set_power = 0;
	static INT8S checkTime = 1,checkStable=0,usbdev=0;
	static INT32U nopower=0;
	static INT8U lastBatLevel=BATTERY_STAT_5;
	int temp,ret;
	if(usensordect_time == 0)
		usensordect_time =  XOSTimeGet();
#if RECORD_MODE != RECORD_MODE_SIGNAL
//-------------------usensor detect----------------------
	ret = boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_CHECK,(INT32U)&temp);
	if(ret>=0)
	{
		if(((temp ==0)&&(SysCtrl.usensor!=USENSOR_STAT_NULL))||softwareDetachUsensor)
		{
		//	sysLog("out");
            boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,0);
			set_power = 0;
			usensordect_time = XOSTimeGet();
			if(softwareDetachUsensor==FALSE)
				deg_Printf("deamon : usensor out\n");
			SysCtrl.usensor=USENSOR_STAT_NULL;
			backrec_sensor(0);
			usbdelay = 0;
		}
		else if(temp)
		{
			if(SysCtrl.usensor==USENSOR_STAT_NULL)
			{
			#if (AX32_PLATFORM == AX3292)
				if(set_power == 0){
					if(XOSTimeGet()>usensordect_time+500)
					{
						boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,1);
						set_power =1;
						usensordect_time = XOSTimeGet();
					}
				}
				if((XOSTimeGet()>usensordect_time+100)&&(set_power==1)){
				#if IO1DO1_USE_SOFTSTART
					hal_io1d1_pd_enable(IO1_CH_PA7,1);
				#endif
				}
				if((XOSTimeGet()>usensordect_time+500)&&(set_power==1))
				{
					
					//usensordect_time = XOSTimeGet();
					s32 check_flag = usb11HostCheckNoWait();
					if(-1 != check_flag)
					{
						if(check_flag)
			#endif
						{
							usbdelay = XOSTimeGet();
							SysCtrl.usensor=USENSOR_STAT_UNSTABLE;
							deg_Printf("deamon : usensor in\n");
						}
			#if (AX32_PLATFORM == AX3292)
						else
						{
							boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,0);
							set_power = 0;
							usensordect_time = XOSTimeGet();
						}
					}
				}
			#endif	
			}
			else if((SysCtrl.usensor==USENSOR_STAT_UNSTABLE) &&((usbdelay+100)<XOSTimeGet()))
			{
				usbdelay = 0;
			#if (AX32_PLATFORM != AX3292)
                boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,1);
			    deg_Printf("deamon : usensor power on\n");
			#endif
			    SysCtrl.usensor=USENSOR_STAT_IN;
			    backrec_sensor(1);
			#if (AX32_PLATFORM == FPGA)
				u8 usb_type = usb_host_detect();
			#elif (AX32_PLATFORM == AX3282S)
				u8 usb_type = 2; //USB2.0
			#elif ((AX32_PLATFORM == AX3292)||(AX32_PLATFORM == AX3291A))
				u8 usb_type = 1; //USB1.1
			#else
				#error "please check usensor usb type!"
			#endif
				usb_host_type_set(usb_type);
				if(usb_type ==1)
					hal_husb11_init_typeA();
				else if(usb_type ==2)
					hal_husb_init_typeA();
				else
					SysCtrl.usensor=USENSOR_STAT_NULL;
			}
		}
		/*else if((temp)&&(SysCtrl.usensor!=USENSOR_STAT_IN))
		{
			boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,0);
			XOSTimeDly(200);
            boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,1);
			sysLog("in");
			deg_Printf("deamon : usensor in\n");
			SysCtrl.usensor=USENSOR_STAT_IN;
			backrec_sensor(1);
			hal_husb_init_typeA();
		}*/
	}
#endif	
//--------------------usb detect------------------------
	ret = boardIoctrl(SysCtrl.bfd_usb,IOCTRL_USB_CHECK,(INT32U)&temp);
	if(ret>=0)
    {	
		if((temp == 0) && (SysCtrl.usb !=USB_STAT_NULL)) // dc out
		{			
		    SysCtrl.usb = USB_STAT_NULL;  // dc out
			checkStable = 5; // wait stable
			usbdev = 0;
			usbdev_reset();
			deg_Printf("usb out\n");
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_USB,SysCtrl.usb));
		}
		else if(temp&& (SysCtrl.usb ==USB_STAT_NULL)) // dc in
		{
			SysCtrl.t_usb = USB_STAT_DCIN;
			ret = hal_usbdCheck();

			if(ret>=0)
			{
				hal_usbdInit(3);
			}

		 	SysCtrl.usb = USB_STAT_DCIN;
			SysCtrl.battery = lastBatLevel=BATTERY_STAT_5;
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_USB,SysCtrl.usb));
			nopower = 0;
		}
		else
		{
			if((SysCtrl.usb == USB_STAT_DCIN)&&usbdev_rqu()&&(usbdev==0))
			{
				if((taskCurrent()!=TASK_POWER_ON)&&(taskCurrent()!=TASK_USB_DEVICE))
				{
					SysCtrl.usb = USB_STAT_PC;
					usbdev = 1;
					lastBatLevel=BATTERY_STAT_5;
					XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_USB,SysCtrl.usb));
					//hal_usbdUninit();
				}
			}
		}
    }
//----------------------battery detect---------------------------------------	
	if(SysCtrl.usb == USB_STAT_NULL) // only dc out check battery
	{
		checkTime--;
		if(checkTime)
			return ;
		checkTime = 1;
	    ret = boardIoctrl(SysCtrl.bfd_battery,IOCTRL_BAT_CHECK,(INT32U)&temp);  
		if(ret>=0)
		{
			if(SysCtrl.battery != temp&&checkStable==0) // need battery stable
				checkStable = 5;//  3;
			else if(SysCtrl.battery == temp)
			{
				checkStable=0;
				return ; // no need update
			}
			if(checkStable>0)
				checkStable--;
            
			if(checkStable == 0)
			{
				if(temp>lastBatLevel)
					return;
				lastBatLevel=temp;
				SysCtrl.battery = temp;
				//if(temp == 0)
				//	taskStart(TASK_POWER_OFF,0);					
				XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_BAT,SysCtrl.battery));
				deg_Printf("battery = %x\n",SysCtrl.battery);
			}
		}
	}
}
/*******************************************************************************
* Function Name  : deamon_gsensor_check
* Description    : gsensor stat check
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
void deamon_gsensor_check(void) // 10 ms
{
#if 0
	static INT8U count=0;
	int temp,ret;
	count++;
	if(count<50)
		return ;
	count = 0;

	if((keyTime+3000)>XOSTimeGet())
		return ;
	if((taskCurrent() != TASK_VIDEO_RECORD)/* || menuActiveGet()*/)
		return ;
	if((SysCtrl.bfd_gsensor>0) && configValue2Int(CONFIG_ID_GSENSOR))
    {
    	if(videoRecordGetStatus()==MEDIA_STAT_START)	// recording will check Gsensor
    	{
			ret = boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_LOCK,(INT32U)&temp);
			//deg_Printf("deamon gsensor: ret=%d,temp=0x%x\n",ret,temp);
			if((ret>=0) && temp)
			{
				XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(VID_EVENT_LOCK_FILE,0));
				deg_Printf("deamon : gsensor active.\n");
				//sysLog("deamon : gsensor active.");
			}
    	}
    }
#endif
}
/*******************************************************************************
* Function Name  : deamon_screen_save
* Description    : screen save check
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
int deamon_screen_save(int on)
{
	static INT32U LastTime=0;
	uint32 cfg_save;
	uint32 lcdOn=0;
	if(taskCurrent() == TASK_POWER_OFF)
		return 0;
	cfg_save = configValue2Int(CONFIG_ID_SCREENSAVE);
   	if(SysCtrl.astern == ASTERN_ON||on)
	{
		boardIoctrl(SysCtrl.bfd_lcd,IOGET_LCD_BKLIGHT,(u32)&lcdOn);
		if(lcdOn==0)
	        boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,1);   // screen on			
		LastTime=XOSTimeGet();
		return 1;
	}
	if(cfg_save==0)
		return 0;
	
	if((XOSTimeGet()-LastTime)>cfg_save*1000)
	{
		LastTime=XOSTimeGet();
		boardIoctrl(SysCtrl.bfd_lcd,IOGET_LCD_BKLIGHT,(u32)&lcdOn);
		if(lcdOn)
		{
			boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,0);   // screen off
		}
	}
	return 0;
}
/*******************************************************************************
* Function Name  : deamon_auto_poweroff
* Description    : system auto power off check
* Input          : int event : system event
* Output         : None
* Return         : None
*******************************************************************************/
int deamon_auto_poweroff(int ClearPowerOffTime)
{
#define TIME_INTERNAL        500
	INT8U task;
	INT32U AutoPowerOffTime;
	static INT32U LastTime=0,PowerOffTime=0;
	if(ClearPowerOffTime)
		goto RESET_POWER_OFF_TIME;
	if((INT32U)(XOSTimeGet()-LastTime)<TIME_INTERNAL)
		return 0;
	LastTime=XOSTimeGet();

	task = taskCurrent();
	if(task == TASK_POWER_OFF)
		return 0;

	AutoPowerOffTime=configValue2Int(CONFIG_ID_AUTOOFF)*1000;
	if(AutoPowerOffTime==0)
		goto RESET_POWER_OFF_TIME;

	if((task == TASK_VIDEO_RECORD && videoRecordGetStatus() == MEDIA_STAT_START) || \
		(task == TASK_PLAY_BACK && videoPlaybackGetStatus() == MEDIA_STAT_PLAY)||\
		(task == TASK_USB_DEVICE)||(task == TASK_POWER_ON))
	{
		goto RESET_POWER_OFF_TIME;
	}
		
	PowerOffTime+=TIME_INTERNAL;
	if(PowerOffTime>=AutoPowerOffTime)
	{
		deg_Printf("Auto power off\n");
		taskStart(TASK_POWER_OFF,0);
	}
	return 0;
RESET_POWER_OFF_TIME:
	PowerOffTime=0;
	return 0;
}



/*******************************************************************************
* Function Name  : deamon_keysound_check
* Description    : deamon key sound condition check
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
int deamon_keysound_play(void)
{
	if(SysCtrl.f_keysound==0) // key sound off
	{
		return 0;
	}
	if(audioPlaybackGetStatus()==MEDIA_STAT_PLAY) // video/audio playing
	{
		return 0;
	}
    if(taskCurrent() == TASK_POWER_OFF)
		return 0;
	
	Audio_ARG_T music;

//	music.size = 2160;
	music.media.src.buff = (INT32U)layout_keysound_load(&music.size);//buildwin_res_key;
	if(music.media.src.buff==0)
	{
		deg_Printf("load music fail\n");
		return -1;
	}
	music.channel =1 ;
	music.type = FILE_TYPE_RAW;
	music.samplerate = 16000;
	music.media.type = MEDIA_SRC_RAM;

	music.media.src.buff += 44;	// cut wav head data
	music.size -= 44;

	SysCtrl.volume_level = configValue2Int(CONFIG_ID_VOLUME);
	audioPlaybackSetVolume(configValue2Int(CONFIG_ID_VOLUME)*10);
	audioPlaybackStart(&music);

	return 0;		
}

/*******************************************************************************
* Function Name  : deamon_keysound_check
* Description    : deamon key sound condition check
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
int deamon_sound_play(INT32U sound_buf, s32 sound_size)
{
	if(audioPlaybackGetStatus()==MEDIA_STAT_PLAY) // video/audio playing
	{
		return 0;
	}
    if(taskCurrent() == TASK_POWER_OFF)
		return 0;
	
	Audio_ARG_T music;

	music.size = sound_size;
	music.media.src.buff = sound_buf;//(INT32U)layout_sound_load(&music.size);//buildwin_res_key;
	if(music.media.src.buff==0)
	{
		deg_Printf("load music fail\n");
		return -1;
	}
	music.channel =1 ;
	music.type = FILE_TYPE_RAW;
	music.samplerate = 16000;
	music.media.type = MEDIA_SRC_RAM;

	music.media.src.buff += 44;	// cut wav head data
	music.size -= 44;
	
	SysCtrl.volume_level = configValue2Int(CONFIG_ID_VOLUME);
    audioPlaybackSetVolume(configValue2Int(CONFIG_ID_VOLUME)*10);
	audioPlaybackStart(&music);

	return 0;		
}
/*******************************************************************************
* Function Name  : deamon_fsFreeSize
* Description    : deamon get fs free size
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
int deamon_fsFreeSize(void)
{
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
	{
		SysCtrl.sd_freesize = fs_free_size()>>1;  // kb
		/*if(fs_check()<0)
		{
			SysCtrl.sd_freesize = 0;
			deg_Printf("deamon : fs system too more debris\n");
		}*/
		if(SysCtrl.sd_freesize == 0)
		{
			SysCtrl.sdcard = SDC_STAT_FULL; 
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
			deg_Printf("deamon : sdc normal but free space is 0.set sdc error\n");  // user need format sdc
		}
	}
	else
		SysCtrl.sd_freesize = 0;
	deg_Printf("deamon : fs free size = %dG%dM%dKB\n",SysCtrl.sd_freesize>>20,(SysCtrl.sd_freesize>>10)&0x3ff,(SysCtrl.sd_freesize)&0x3ff);
	return SysCtrl.sd_freesize;
}
/*******************************************************************************
* Function Name  : deamon_fsSizeAdd
* Description    : deamon dec size from free size
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
int deamon_fsSizeModify(INT8S dec,INT32U size)
{
	if(size&(SysCtrl.fs_clustsize-1))
	{
		size = (size&(~(SysCtrl.fs_clustsize-1)))+SysCtrl.fs_clustsize;
		//size+=1024;
	}
	size>>=10;
//    deg_Printf("deamon 0: fs free size = %dG%dM%dKB\n",SysCtrl.sd_freesize>>20,(SysCtrl.sd_freesize>>10)&0x3ff,(SysCtrl.sd_freesize)&0x3ff);
	if(dec<0)
	{
		if(SysCtrl.sd_freesize>size)
			SysCtrl.sd_freesize-=size;
		else
			SysCtrl.sd_freesize = 0;
	}
	else
	{
		SysCtrl.sd_freesize+=size;
	}
//	deg_Printf("deamon 1: fs free size = %dG%dM%dKB\n",SysCtrl.sd_freesize>>20,(SysCtrl.sd_freesize>>10)&0x3ff,(SysCtrl.sd_freesize)&0x3ff);
	return SysCtrl.sd_freesize;
}

