/****************************************************************************
**
 **                              MENU 
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MENU SETTING
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : menu_user_setting.c
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

INT32U configValue2Int(INT8U config_id)
{
	INT32U value=0;
	switch(configGet(config_id))
	{
// lanuage		
		//case R_ID_STR_LAN_CZECH : value = LANUAGE_CZECH;break;
		case R_ID_STR_LAN_DUTCH: value = LANUAGE_DUTCH;break;
		case R_ID_STR_LAN_FRECH: value = LANUAGE_FRENCH;break;
		case R_ID_STR_LAN_GERMAN: value = LANUAGE_GERMAN;break;
		//case R_ID_STR_LAN_HEBREW: value = LANUAGE_HEBREW;break;
		case R_ID_STR_LAN_ITALIAN: value = LANUAGE_ITALIAN;break;
		case R_ID_STR_LAN_JAPANESE: value = LANUAGE_JAPANESE;break;
		case R_ID_STR_LAN_KOERA: value = LANUAGE_KOERA;break;
		case R_ID_STR_LAN_POLISH: value = LANUAGE_POLISH;break;
		case R_ID_STR_LAN_PORTUGUESE: value = LANUAGE_PORTUGUESE;break;
		case R_ID_STR_LAN_RUSSIAN: value = LANUAGE_RUSSIAN;break;
		case R_ID_STR_LAN_SCHINESE: value = LANUAGE_SCHINESE;break;
		case R_ID_STR_LAN_SPANISH: value = LANUAGE_SPANISH;break;
		case R_ID_STR_LAN_TAI: value = LANUAGE_TAI;break;
		case R_ID_STR_LAN_TCHINESE: value = LANUAGE_TCHINESE;break;
		//case R_ID_STR_LAN_TURKEY: value = LANUAGE_TURKEY;break;
		//case R_ID_STR_LAN_UKRAINIAN: value = LANUAGE_UKRAINIAN;break;
        case R_ID_STR_LAN_ENGLISH: value = LANUAGE_ENGLISH;break;
// common
        case R_ID_STR_COM_ON:
		case R_ID_STR_COM_OK:
		case R_ID_STR_COM_YES:value = 1;break;
		case R_ID_STR_COM_OFF:
		case R_ID_STR_COM_CANCEL:
		case R_ID_STR_COM_NO:value = 0;break;	   
// time		
//		case R_ID_STR_TIM_1SEC : value = 1;break;
		case R_ID_STR_TIM_2SEC : value = 2;break;
		case R_ID_STR_TIM_3SEC : value = 3;break;
		case R_ID_STR_TIM_5SEC : value = 5;break;
		case R_ID_STR_TIM_10SEC : value = 10;break;
		case R_ID_STR_TIM_30SEC : value = 30;break;
		case R_ID_STR_TIM_1MIN : value = 1*60;break;
		case R_ID_STR_TIM_2MIN : value = 2*60;break;
		case R_ID_STR_TIM_3MIN : value = 3*60;break;
		case R_ID_STR_TIM_5MIN : value = 5*60;break;
		case R_ID_STR_TIM_10MIN: value = 10*60;break;

// resolution
		case R_ID_STR_RES_480FHD: value = (720<<16)|480;break;
        case R_ID_STR_RES_1024P: value = (1280<<16)|1024;break;
		case R_ID_STR_RES_1080P: value = (1440<<16)|1080;break;
		case R_ID_STR_RES_FHD:
		case R_ID_STR_RES_1080FHD:
		case R_ID_STR_RES_1080P_SHORT:
			{
				if(videoRecordFrameEnGet())	//frame
				{
					value = (640<<16)|480;
				}
				else  //no frame
				{
					value = (1920<<16)|1080;
				}
				break;
			}
		case R_ID_STR_RES_HD:
		case R_ID_STR_RES_720P :
		case R_ID_STR_RES_720P_SHORT:
			{
				if(videoRecordFrameEnGet())	//frame
				{
					value = (640<<16)|480;
				}
				else  //no frame
				{
					value = (1280<<16)|720;
				}
				break;
			}
		case R_ID_STR_RES_240P :
		case R_ID_STR_RES_QVGA : value = (320<<16)|240;break;
		case R_ID_STR_RES_480P :
		case R_ID_STR_RES_VGA   : value = (640<<16)|480;break;
		case R_ID_STR_RES_1M    : value = (1280<<16)|960;break;
		case R_ID_STR_RES_2M    : value = (1440<<16)|1080;break;
		case R_ID_STR_RES_3M    : value = (1920<<16)|1440;break;
		case R_ID_STR_RES_5M    : value = (2560<<16)|1920;break;
		case R_ID_STR_RES_8M    : value = (3200<<16)|2400;break;
		case R_ID_STR_RES_10M   : value = (3520<<16)|2460;break;
		case R_ID_STR_RES_12M   : value = (3840<<16)|2880;break;
		case R_ID_STR_RES_16M   : value = (4480<<16)|3360;break;
		case R_ID_STR_RES_18M   : value = (4800<<16)|3600;break;
		case R_ID_STR_RES_20M   : value = (5120<<16)|3840;break;
		case R_ID_STR_RES_24M   : value = (5760<<16)|4320;break;
		case R_ID_STR_RES_40M   : value = (7040<<16)|5280;break;
		case R_ID_STR_RES_48M   : value = (7680<<16)|5760;break;
		//case R_ID_STR_RES_40M    : value = (7040<<16)|5280;break;
		/*case R_ID_STR_RES_12M    : 
			{
				if(videoRecordFrameEnGet()) //���
				{
					value = (4032<<16)|2520;  //10M
				}
				else
				{
					value = (4032<<16)|2880;
				}
				break;	
			}*/
// level
        case R_ID_STR_COM_LOW : value = 1;break;
		case R_ID_STR_COM_MIDDLE: value = 2;break;
		case R_ID_STR_COM_HIGH: value = 3;break;
// frq 
        case R_ID_STR_COM_50HZ: value = 0;break;
		case R_ID_STR_COM_60HZ: value = 1;break;
// IR
        case R_ID_STR_IR_AUTO: value = 2;break;

//isp 
    // white blance
        case R_ID_STR_ISP_AUTO: value = 0;break;
		case R_ID_STR_ISP_SUNLIGHT: value = 1;break;
		case R_ID_STR_ISP_CLOUDY: value = 2;break;
		case R_ID_STR_ISP_TUNGSTEN: value = 3;break;
		case R_ID_STR_ISP_FLUORESCENT: value = 4;break;
    //  ev
        case R_ID_STR_COM_N2_0: value = 1;break;
		case R_ID_STR_COM_N1_0: value = 2;break;
		case R_ID_STR_COM_P0_0: value = 3;break;
		case R_ID_STR_COM_P1_0: value = 4;break;
		case R_ID_STR_COM_P2_0: value = 5;break;

	//  
		case R_ID_STR_COM_LEVEL_0: value = 0;break;
        case R_ID_STR_COM_LEVEL_1: value = 1;break;
		case R_ID_STR_COM_LEVEL_2: value = 2;break;
		case R_ID_STR_COM_LEVEL_3: value = 3;break;
		case R_ID_STR_COM_LEVEL_4: value = 4;break;
		case R_ID_STR_COM_LEVEL_5: value = 5;break;
		case R_ID_STR_COM_LEVEL_6: value = 6;break;
		case R_ID_STR_COM_LEVEL_7: value = 7;break;
		case R_ID_STR_COM_LEVEL_8: value = 8;break;
	}

	return value;
}



