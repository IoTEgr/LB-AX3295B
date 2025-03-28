/****************************************************************************
**
 **                              BOARD
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  BOARD BWV1
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : board_bwv1.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is for user board
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "application.h"
//#include "../device/gsensor/gsensor.h"
//#include "../device/tft_lcd/tft_lcd.h"
//#include "../device/cmos_sensor/cmos_sensor.h"


#if SYSTEM_BOARD == BOARD_TYPE_BWV1

	//#define  LED0_CH       GPIO_PB
	//#define  LED0_PIN      GPIO_PIN5


	
	//#define  LEDR_CH	   GPIO_PB
	//#define  LEDR_PIN	   GPIO_PIN5

	#define  LEDB_CH	   GPIO_PB
	#define  LEDB_PIN	   GPIO_PIN5

	#define  LCD_BK_CH       GPIO_PA
	#define  LCD_BK_PIN      GPIO_PIN6

	#define  PWR_KEY_CH      GPIO_PB
	#define  PWR_KEY_PIN     GPIO_PIN0
	
	#define  LCD_RESET_CH 	 GPIO_PB
	#define  LCD_RESET_PIN	 GPIO_PIN6
	
#if IR_MENU_EN	
	#define  IR_LED_CH 			   GPIO_PA
	#define  IR_LED_PIN			   GPIO_PIN9
#endif 

	#define  ADC_BAT_CH      ADC_CH_MVOUT
	#define  ADC_BGP_CH      ADC_CH_BGOP
	#define  ADC_KEY_CH      ADC_CH_PB1
	//#define  ADC_GSR_CH      ADC_CH_PE0

static u8 adCh;

static int board_led_init(void)
{
#if 0
	hal_gpioInit(LED0_CH,LED0_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
	//==300R control==
	hal_gpioLedInit(GPIO_LED5_PB5,GPIO_PULL_UP,0);
	hal_gpioLedInit(GPIO_LED5_PB5,GPIO_PULL_DOWN,0);
	hal_gpioLedPull(LED0_CH,LED0_PIN,GPIO_PULLE_DOWN);
#endif

	//hal_gpioInit(LEDR_CH,LEDR_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	//hal_gpioLedPull(LEDR_CH,LEDR_PIN,GPIO_PULLE_DOWN);

	hal_gpioInit(LEDB_CH,LEDB_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioLedPull(LEDB_CH,LEDB_PIN,GPIO_PULLE_DOWN);


	return 0;
}

static int board_led_ioctrl(INT32U prit,INT32U op,INT32U para)
{

#if 0
	static u8 led0=0;

    if(op == IOCTRL_LED_CHECK)
	{
		*(INT32U*)para=led0;
	}

    if(op == IOCTRL_LED_NO0)
    {
		if(para)
		{
			//hal_gpioWrite(LED0_CH,LED0_PIN,GPIO_HIGH);
			hal_gpioLedPull(LED0_CH,LED0_PIN,GPIO_PULLE_UP);
		}
		else
		{
			//hal_gpioWrite(LED0_CH,LED0_PIN,GPIO_LOW);
			hal_gpioLedPull(LED0_CH,LED0_PIN,GPIO_PULLE_DOWN);
		}
		led0 = para;
    }
#endif

	if(op == IOCTRL_LED_B)
    {
		if(para)
		{
			hal_gpioWrite(LEDB_CH,LEDB_PIN,GPIO_HIGH);
		}
		else
		{
			hal_gpioWrite(LEDB_CH,LEDB_PIN,GPIO_LOW);
		}
    }

	return 0;
}


void board_lcd_reset(void)
{
	
	hal_gpioInit(LCD_RESET_CH,LCD_RESET_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(LCD_RESET_CH,LCD_RESET_PIN,GPIO_HIGH);	
	hal_sysDelayMS(20);	
	hal_gpioWrite(LCD_RESET_CH,LCD_RESET_PIN,GPIO_LOW);	
	hal_sysDelayMS(50);
	hal_gpioWrite(LCD_RESET_CH,LCD_RESET_PIN,GPIO_HIGH);	
	hal_sysDelayMS(50);
}
static int board_lcd_init(void)
{
	hal_gpioInit(LCD_BK_CH,LCD_BK_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(LCD_BK_CH,LCD_BK_PIN,GPIO_LOW);


//--------------LCD RESET------------------------------------
	//hal_dacHPSet(1,HP_VDD_2_8V);
	//hal_sysDelayMS(50);
	
	//hal_dacHPSet(0,0);
	//hal_sysDelayMS(50);
	
	//hal_dacHPSet(1,HP_VDD_2_8V);
	//hal_sysDelayMS(50);

	lcdInit();

	return 0;
}

void board_sensor_reset()
{	
#if 1   //reset
	hal_gpioInit(FRONT_SENSOR_RESET_CH,FRONT_SENSOR_RESET_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(FRONT_SENSOR_RESET_CH,FRONT_SENSOR_RESET_PIN,GPIO_LOW); 
	hal_gpioInit(REAR_SENSOR_RESET_CH,REAR_SENSOR_RESET_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(REAR_SENSOR_RESET_CH,REAR_SENSOR_RESET_PIN,GPIO_HIGH); 	
		
	
	hal_sysDelayMS(10); 
	hal_gpioWrite(REAR_SENSOR_RESET_CH,REAR_SENSOR_RESET_PIN,GPIO_LOW); 
	
	hal_sysDelayMS(30); 
	hal_gpioWrite(REAR_SENSOR_RESET_CH,REAR_SENSOR_RESET_PIN,GPIO_HIGH); 
#else
	//front
	hal_gpioInit(FRONT_SENSOR_RESET_CH,FRONT_SENSOR_RESET_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(FRONT_SENSOR_RESET_CH,FRONT_SENSOR_RESET_PIN,GPIO_HIGH); 
	hal_gpioInit(REAR_SENSOR_RESET_CH,REAR_SENSOR_RESET_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(REAR_SENSOR_RESET_CH,REAR_SENSOR_RESET_PIN,GPIO_LOW); 	
		
	
	hal_sysDelayMS(10); 
	hal_gpioWrite(FRONT_SENSOR_RESET_CH,FRONT_SENSOR_RESET_PIN,GPIO_LOW); 
	
	hal_sysDelayMS(30); 
	hal_gpioWrite(FRONT_SENSOR_RESET_CH,FRONT_SENSOR_RESET_PIN,GPIO_HIGH); 

#endif
}

static int board_cmos_init(void)
{
	board_sensor_reset();
	sensorInit();
	hal_mjpegEncodeInit();
	ax32xx_csi_only_FrameSet();
	hal_csiEnable(1);
	return 0;
}
static int board_cmos_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	if(op == IOCTRL_CMOS_INIT)
	{
		return board_cmos_init();
	}
	return -1;
}
extern void setKeyEn(bool en);
static int board_lcd_ioctrl(INT32U prit,INT32U op,INT32U para)
{
     static INT32U  lcdState=1;
    if(op == IOCTRL_LCD_BKLIGHT)
    {
		if(para)
		{
			//hal_gpioWrite(LCD_BK_CH,LCD_BK_PIN,GPIO_HIGH);
			ax32xx_gpioMapSet(LCD_BK_CH,LCD_BK_PIN,GPIO_FUNC_SFR);
			hal_timerPWMStart(TIMER2,TMR2_POS_PA6,4000,configValue2Int(CONFIG_ID_LCD_BRIGHT)*10+20);
			lcdState=1;
		}
		else
		{
			//hal_gpioWrite(LCD_BK_CH,LCD_BK_PIN,GPIO_LOW);
			hal_timerPWMStop(TIMER2);
			lcdState=0;
			setKeyEn(false);
		}
    }
	else if(op == IOGET_LCD_BKLIGHT)
	{
   		*(INT32U*)para=lcdState;
   	//	*(INT32U*)para=hal_gpioRead(LCD_BK_CH,LCD_BK_PIN);
	}
    else if(op == IOCTRL_LCD_LCMOFF)
    {
//        hal_lcdLCMPowerOff();
    }
	return 0;
}

void board_lcd_ioctrl_pwm(uint8 data )
{
	hal_timerPWMStart(TIMER2,TMR2_POS_PA6,4000,data*10+20);
}


static int board_sdc_init(void)
{
//#if USENSOR_PWRCTRL_EN
//    hal_sysLDOSet(SYS_LDO_SDC,0,0);  // disable SD_VDD
//#else
//    hal_sysLDOSet(SYS_LDO_SDC,0,1);  // enable SD_VDD
//#endif
    return 0;
}

static int board_sdc_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	if(op == IOCTRL_SDC_CHECK)
	{
		if(hal_sdCheck())
			*(INT32U *)para = 1;
		else
			*(INT32U *)para = 0;

		return 0;
	}

	return -1;
}
static int board_usb_init(void)
{
    hal_wki1InputEnable(1);
    return 0;
}

static int board_usb_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	if(op == IOCTRL_USB_CHECK)
	{ 
       *(INT32U *)para = hal_wki1Read();
		return 0;
	}
	return -1;
}
//------------battery --------
#define  BATTERY_MAX     6
#define  BATTERY_AVG     16
#define  BATTERY_INV     30  //6
#define  BATTERY_OFS     (20) //unit:mV

const static u16 batteryValueTable[BATTERY_MAX] = {3500,3600,3750,3950,4000};  //3.45

static int board_battery_init(void)
{
	u16 batValue=0,bgpValue=0;
	int value;

	if(!HAL_CFG_EN_DBG)
	{
 		hal_gpioInit(CHECK_FULL_CH,CHECK_FULL_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
	}

	hal_batDetectEnable(1);

    int i = 3;
    while(i--)
    {
        batValue += hal_adcGetChannel(ADC_BAT_CH);
        bgpValue += hal_adcGetChannel(ADC_BGP_CH);
    }

	if(bgpValue==0)
	{
		value = 0;
	}
	else
	{
		batValue=batValue/BATTERY_AVG;
		bgpValue=bgpValue/BATTERY_AVG;
		value = hal_adcVDDRTCCalculate(batValue,bgpValue) + BATTERY_OFS + 30;
	}

	if(value</*batteryValueTable[0]*/3300)//
	{
		board_usb_ioctrl(0,IOCTRL_USB_CHECK,(INT32U)&value);
		if(value==0)
		{
			deg_Printf("board : battary low power.try to power off.\n");
		    return -1;
		}
	}
	
	return 0;
}
static int curBatteryADC=0;
int getCurBatteryADC(void)
{
	return curBatteryADC;
}
static int board_battery_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	static u8 step = 0,oldBat=0xff;
	static u16 batValue=0,bgpValue=0;
	int i,value;
	if(op == IOCTRL_BAT_CHECK)
	{

		batValue += hal_adcGetChannel(ADC_BAT_CH);
		bgpValue += hal_adcGetChannel(ADC_BGP_CH);
		step++;

		if(step>=BATTERY_AVG)
		{
			if(bgpValue==0)
				value = 0;
            else
            {
            	batValue=batValue/BATTERY_AVG;
				bgpValue=bgpValue/BATTERY_AVG;
                value = hal_adcVDDRTCCalculate(batValue,bgpValue) + BATTERY_OFS;
            }

			for(i=0;i<BATTERY_MAX;i++)
			{
				if(value<=batteryValueTable[i])
					break;
			}
			if(oldBat!=i)
			{
				if(oldBat==0xff)
					oldBat = i;
				else
				{
					if(oldBat>i)
					{
						if(value<batteryValueTable[i]-BATTERY_INV)
							oldBat=i;
					}
					else
					{
						if(value>batteryValueTable[oldBat]+BATTERY_INV)
							oldBat=i;
					}
				}
			}
			curBatteryADC=value;
			//deg_Printf("battery : ad value = %d,i = %d,oldBat=%d\n",value,i,oldBat);
			step = 0;
			bgpValue=0;
			batValue=0;
			*(INT32U *)para = oldBat;
			return 0;
		}
	}

	return -1;
}

//----------------ad key-----------------------
#define  _POWR_OFF_LGC_		1
#define  KEY_NUM			   6

/*
if you want to implement the long press function of the key,
please refer to the getKey() function, when the key long pressed,
change the value of key
*/
typedef struct _keyInfor
{
	u8   keyType;      // key event
	u16  keyADCvalue;  // adc value of key
	u16  keyADCmin;    // no need initialzation,in board_adkey_init() function,this value calculated according to (keyADCvalue-ADC_KEY_DEVIATION)
	u16  keyADCmax;    // no need initialzation,in board_adkey_init() function,this value calculated according to (keyADCvalue+ADC_KEY_DEVIATION)
}keyInfor;
#define ADC_KEY_DEVIATION     29  // if ADC value of key has a large error,ADC_KEY_DEVIATION shud be large 
#define ADD_KEY(key,keyADCvalue)  {key,keyADCvalue,0,0}
#if  ((AX32_PLATFORM == AX3295)||(AX32_PLATFORM == AX3295B))
keyInfor adckeyInfor[]=
{
	ADD_KEY(KEY_EVENT_OK,  0),
	ADD_KEY(KEY_EVENT_DOWN,306),
	ADD_KEY(KEY_EVENT_UP,  484),
	ADD_KEY(KEY_EVENT_MENU,658),
	ADD_KEY(KEY_EVENT_MODE,817),
};
#endif

static int board_adkey_init(void)
{
	u32 i;
	for(i=0;i<sizeof(adckeyInfor)/sizeof(adckeyInfor[0]);i++)
	{
		if(adckeyInfor[i].keyADCvalue<=ADC_KEY_DEVIATION)
			adckeyInfor[i].keyADCmin=0;
		else
			adckeyInfor[i].keyADCmin=adckeyInfor[i].keyADCvalue-ADC_KEY_DEVIATION;
		adckeyInfor[i].keyADCmax=adckeyInfor[i].keyADCvalue+ADC_KEY_DEVIATION;
		if(adckeyInfor[i].keyADCmax>1024)
			adckeyInfor[i].keyADCmax=1024;
	}
	adCh = ADC_KEY_CH;
    hal_gpioInit(PWR_KEY_CH,PWR_KEY_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
	hal_gpioInit(GPIO_PB,GPIO_PIN1,GPIO_INPUT,GPIO_PULL_FLOATING);

	return 0;
}
uint32 keyADCvalue=0;
uint32 getKeyADCvalue(void)
{
	return keyADCvalue;
}

#define KEY_SCAN_UNFINISHED             0xffffffff
uint32 getKeyVal(void)
{
	static uint32 lastKey=KEY_EVENT_END;
	static uint32 keyScanTimes=0;
	uint32 value,i,key=0;

	value = hal_gpioRead(PWR_KEY_CH,PWR_KEY_PIN);

	   if(value)
	   	   key = KEY_EVENT_POWER;   // power key ,key value
	   else
	   {
		  value = hal_adcGetChannel(ADC_KEY_CH);
		  if(value < 1000 )
		  {
		  	keyADCvalue=value;
		    //deg_Printf("adcKeyValue:%d\n",value);
		  }
		  
		   for(i=0;i<sizeof(adckeyInfor)/sizeof(adckeyInfor[0]);i++)
		   {
		   	   if(value>=adckeyInfor[i].keyADCmin&&value<=adckeyInfor[i].keyADCmax)
			   	  key = adckeyInfor[i].keyType;
		   }
	   }
	   
	   if(lastKey!=key)
   	   {
   	  	lastKey=key;
		keyScanTimes=1;
   	   }
	   else
	   	keyScanTimes++;
	   
	   if(keyScanTimes>=3)
   	  {
   	  	keyScanTimes=0;
		return key;
   	  }
	   return KEY_SCAN_UNFINISHED;
}
uint32 getKey(void)
{
	static uint32 keyPowerPressCnt=0;
	uint32 keyVal;
	keyVal=getKeyVal();
	if(keyVal==KEY_SCAN_UNFINISHED)
		return KEY_SCAN_UNFINISHED;
	if(keyVal==KEY_EVENT_POWER)
	{
		keyPowerPressCnt++;
		if(keyPowerPressCnt>=30)
			keyVal=KEY_EVENT_POWEROFF;
		else
			keyVal=0;	
	}
	else
	{
		if(keyPowerPressCnt>0&&keyPowerPressCnt<30)
			keyVal = KEY_EVENT_POWER;
		keyPowerPressCnt=0;
	}


	static uint32 keyOKPowerPressCnt=0;
	if(keyVal==KEY_EVENT_OK)
	{
		keyOKPowerPressCnt++;
		if(keyOKPowerPressCnt>=30)
		{
			keyVal=KEY_EVENT_OKLONG;
		}
		else
			keyVal=0;	
	}
	else
	{
		if(keyOKPowerPressCnt>0&&keyOKPowerPressCnt<30)
		{
			keyVal = KEY_EVENT_OK;
		}
		keyOKPowerPressCnt=0;
	}
	
	return keyVal;
}
#define SCAN_KEY_CONTINUE_INTERVAL  (2)
uint32 keyScanKey(void)
{
	static uint32 LastKeyVal=0;
	static int32 KeyContinueCnt=0;
	uint32 CurKeyVal;
	uint32 KeySta=0;
	CurKeyVal=getKey();
	if(CurKeyVal==KEY_SCAN_UNFINISHED)
		return 0;
	if(LastKeyVal!=CurKeyVal)
	{
		if(CurKeyVal)
			KeySta=makeEvent(CurKeyVal,KEY_PRESSED);
		else
			KeySta=makeEvent(LastKeyVal,KEY_RELEASE);
		LastKeyVal=CurKeyVal;
		KeyContinueCnt=-10;
	}
	else
	{
		if(CurKeyVal)
		{
			KeyContinueCnt++;
			if(KeyContinueCnt>=SCAN_KEY_CONTINUE_INTERVAL)
			{
				KeyContinueCnt=0;
				KeySta=makeEvent(CurKeyVal,KEY_CONTINUE);
			}
			else
				KeySta=0;
		}
		else
			KeySta=0;
	}
	return KeySta;
}
static int board_adkey_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	int value;
	if(op == IOCTRL_KEY_READ)
	{   
		   if((*(INT32U *)para = keyScanKey())==0)
		   	   return -1;			   	   
	}
    else if(op == IOCTRL_KEY_POWER)
    {
		value = hal_gpioRead(PWR_KEY_CH,PWR_KEY_PIN);
		*(int *)para = value; 
    }
	else if(op == IOCTRL_KEY_POWEROFF)
	{
//		hal_gpioInit(PWR_KEY_CH,PWR_KEY_PIN,GPIO_OUTPUT,GPIO_PULL_DOWN);
//		hal_gpioWrite(PWR_KEY_CH,PWR_KEY_PIN,GPIO_LOW);
	}
	return 0;
}

int board_powerCheck(void)
{
	hal_gpioInit(PWR_KEY_CH,PWR_KEY_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);

	return hal_gpioRead(PWR_KEY_CH,PWR_KEY_PIN);
}


static int board_ir_init(void)
{
#if (1 == IR_MENU_EN)
//	ax32xx_gpioDigitalSet(IR_LED_CH,IR_LED_PIN,GPIO_DIGITAL_EN);		// set ir io is digital .
//	hal_gpioInit(IR_LED_CH,IR_LED_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
//	hal_gpioWrite(IR_LED_CH,IR_LED_PIN,GPIO_LOW);
	hal_gpioInit(IR_LED_CH,IR_LED_PIN,GPIO_INPUT,GPIO_PULL_DOWN);

#endif
	return 0;
}

static int board_ir_ioctrl(INT32U prit,INT32U op,INT32U para)
{
#if (1 == IR_MENU_EN)

	static uint32 irState=0;
	if(op == IOCTRL_IR_SET)
	{
	   if(para)	// on
	   {
		   //hal_gpioInit(IR_LED_CH,IR_LED_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
		   //hal_gpioWrite(IR_LED_CH,IR_LED_PIN,GPIO_HIGH);
		   hal_gpioInit(IR_LED_CH,IR_LED_PIN,GPIO_INPUT,GPIO_PULL_UP);
		   irState=1;
	   }
	   else		//off
	   {
		   //hal_gpioInit(IR_LED_CH,IR_LED_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
		   //hal_gpioWrite(IR_LED_CH,IR_LED_PIN,GPIO_LOW);
		   hal_gpioInit(IR_LED_CH,IR_LED_PIN,GPIO_INPUT,GPIO_PULL_DOWN);
		   irState=0;
	   }
	}
	else if(op == IOGET_IR_GET)
		*(INT32U*)para=irState;
#endif
	return 0;
}

Board_Node_T  board_bwv1[]=
{
	{//------bat-----------
		.name = DEV_NAME_BATTERY,
		.init = board_battery_init,
		.ioctrl = board_battery_ioctrl, 
		.prit = 0
	},
	{//------sdc-----------
		.name = DEV_NAME_SDCARD,
		.init = board_sdc_init,
		.ioctrl = board_sdc_ioctrl, 
		.prit = 0
	},
    {//------lcd-----------
		.name = DEV_NAME_LCD,
		.init = board_lcd_init,
		.ioctrl = board_lcd_ioctrl,	
		.prit = 0
    }, 	
    {//------usb-----------
		.name = DEV_NAME_USB,
		.init = board_usb_init,
		.ioctrl = board_usb_ioctrl, 
		.prit = 0
	},
	{//------led-----------
		.name = DEV_NAME_LED,
		.init = board_led_init,
		.ioctrl = board_led_ioctrl,	
		.prit = 0
    },
     
	{//------cmos -sensor
		.name = "cmos-sensor",
		.init = NULL,//booard_cmos_init,
		.ioctrl = board_cmos_ioctrl,
		.prit=0
	},
    {//------ad key-----------
		.name = DEV_NAME_ADKEY,
		.init = board_adkey_init,
		.ioctrl = board_adkey_ioctrl, 
		.prit = 0
	},
#if (1 == IR_MENU_EN)
	{//------ir led-----------
		.name = DEV_NAME_IR,
		.init = board_ir_init,
		.ioctrl = board_ir_ioctrl, 
		.prit = 0
	},
#endif
    {
		.name[0] = 0,
		.init = NULL,
		.ioctrl = NULL,
    }
};



#endif

