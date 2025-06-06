/****************************************************************************
       ***             ***                      MAXLIB-GRAPHC                  
      ** **           ** **                                                    
     **   **         **   **            THE MAXLIB FOR IMAGE SHOW PROCESS      
    **     **       **     **                                                  
   **       **     **       **              MAX ROURCE ICON MANAGEMENT         
  **         **   **         **                                                
 **           ** **           **              (C) COPYRIGHT 2016 MAX           
**             ***             **                                              
                                                                               
* File Name   : user_str.c                                                     
* Author      : Mark.Douglas                                                   
* Version     : V0100                                                          
* Date        : 05/25/2016                                                     
* Description : This file for maxlib resource str managemant                   
*                                                                              
* History     :                                                                
* 2016-05-25  :                                                                
*      <1>.This is created by mark,set version as v0103.                       
*      <2>.Add basic functions.                                                
******************************************************************************/

#ifndef USER_STR_H
   #define USER_STR_H

extern R_STRING_T User_String_Table[];

enum
{
   R_ID_STR_LAN_ENGLISH = R_ID_TYPE_STR,
   R_ID_STR_LAN_SCHINESE,
   R_ID_STR_LAN_TCHINESE,
   R_ID_STR_LAN_JAPANESE,
   R_ID_STR_LAN_GERMAN,
   R_ID_STR_LAN_FRECH,
   R_ID_STR_LAN_RUSSIAN,
   R_ID_STR_LAN_ITALIAN,
   R_ID_STR_LAN_KOERA,
   R_ID_STR_LAN_TAI,
   R_ID_STR_LAN_HEBREW,
   R_ID_STR_LAN_DUTCH,
   R_ID_STR_LAN_UKRAINIAN,
   R_ID_STR_LAN_SPANISH,
   R_ID_STR_LAN_PORTUGUESE,
   R_ID_STR_LAN_POLISH,
   R_ID_STR_LAN_CZECH,
   R_ID_STR_LAN_TURKEY,
   R_ID_STR_UPDATE_START,
   R_ID_STR_UPDATE_END,
   R_ID_STR_COM_OFF,
   R_ID_STR_COM_ON,
   R_ID_STR_COM_OK,
   R_ID_STR_COM_CANCEL,
   R_ID_STR_COM_YES,
   R_ID_STR_COM_NO,
   R_ID_STR_COM_LOW,
   R_ID_STR_COM_MIDDLE,
   R_ID_STR_COM_HIGH,
   R_ID_STR_COM_50HZ,
   R_ID_STR_COM_60HZ,
   R_ID_STR_COM_LEVEL_0,
   R_ID_STR_COM_LEVEL_1,
   R_ID_STR_COM_LEVEL_2,
   R_ID_STR_COM_LEVEL_3,
   R_ID_STR_COM_LEVEL_4,
   R_ID_STR_COM_LEVEL_5,
   R_ID_STR_COM_LEVEL_6,
   R_ID_STR_COM_LEVEL_7,
   R_ID_STR_COM_LEVEL_8,
   R_ID_STR_COM_LEVEL_9,
   R_ID_STR_COM_P4_0,
   R_ID_STR_COM_P3_0,
   R_ID_STR_COM_P2_0,
   R_ID_STR_COM_P1_0,
   R_ID_STR_COM_P0_0,
   R_ID_STR_COM_N1_0,
   R_ID_STR_COM_N2_0,
   R_ID_STR_COM_ALWAYSON,
   R_ID_STR_COM_ECONOMIC,
   R_ID_STR_COM_NORMAL,
   R_ID_STR_COM_FINE,
   R_ID_STR_TIM_1MIN,
   R_ID_STR_TIM_2MIN,
   R_ID_STR_TIM_3MIN,
   R_ID_STR_TIM_5MIN,
   R_ID_STR_TIM_10MIN,
   R_ID_STR_TIM_2SEC,
   R_ID_STR_TIM_3SEC,
   R_ID_STR_TIM_5SEC,
   R_ID_STR_TIM_10SEC,
   R_ID_STR_TIM_30SEC,
   R_ID_STR_PHOTO_NUM_3,
   R_ID_STR_PHOTO_NUM_5,
   R_ID_STR_SET_SETTING,
   R_ID_STR_SET_VIDEO,
   R_ID_STR_SET_PHOTO,
   R_ID_STR_SET_PLAY,
   R_ID_STR_SET_MUSIC,
   R_ID_STR_SET_GAME,
   R_ID_STR_SET_LANGUAGE,
   R_ID_STR_SET_PRINT_DENSITY,
   R_ID_STR_SET_PRINT_MODE,
   R_ID_STR_SET_DATETIME,
   R_ID_STR_SET_VOLUME,
   R_ID_STR_SET_VIDEO_RESOLUTION,
   R_ID_STR_SET_CAMERA_RESOLUTION,
   R_ID_STR_SET_FORMAT,
   R_ID_STR_SET_AUTOOFF,
   R_ID_STR_SET_SCREEN_SAVERS,
   R_ID_STR_SET_LOOPRECORD,
   R_ID_STR_SET_FREQUENCY,
   R_ID_STR_SET_DEFAULT_SETTING,
   R_ID_STR_SET_VERSION,
   R_ID_STR_SET_PRINT_GRAY,
   R_ID_STR_SET_PRINT_DOT,
   R_ID_STR_TIPS_PRINTING,
   R_ID_STR_TIPS_NO_PAPER,
   R_ID_STR_TIPS_OVER_HEAT,
   R_ID_STR_TIPS_SDC_NULL,
   R_ID_STR_TIPS_SDC_FULL,
   R_ID_STR_TIPS_NO_FILES,
   R_ID_STR_TIPS_NO_POWER,
   R_ID_STR_TIPS_PLEASE_WAITING,
   R_ID_STR_TIPS_FMT_SUCCESS,
   R_ID_STR_TIPS_FMT_FAIL,
   R_ID_STR_TIPS_DELETECUR,
   R_ID_STR_SET_AUDIOMODE,
   R_ID_STR_SET_USBMASS,
   R_ID_STR_SET_USBCAM,
   R_ID_STR_SET_TIMEPHOTO,
   R_ID_STR_SET_MOREPHOTO,
   R_ID_STR_SET_VIDEOROTATE,
   R_ID_STR_SET_MOTIONDET,
   R_ID_STR_SET_AUDIOREC,
   R_ID_STR_SET_TIMESTRAMP,
   R_ID_STR_SET_FILLIGHT,
   R_ID_STR_IR_AUTO,
   R_ID_STR_SET_GSENSOR,
   R_ID_STR_SET_PARKMODE,
   R_ID_STR_SET_FASTVIEW,
   R_ID_STR_SET_DELETEALL,
   R_ID_STR_SET_LOCKCUR,
   R_ID_STR_SET_LOCKALL,
   R_ID_STR_SET_UNLOCKCUR,
   R_ID_STR_SET_UNLOCKALL,
   R_ID_STR_SET_LOCKED,
   R_ID_STR_SET_LOCK,
   R_ID_STR_SET_DELETE,
   R_ID_STR_SET_PLAYMODE,
   R_ID_STR_SET_REPEATALL,
   R_ID_STR_SET_REPEATRAD,
   R_ID_STR_SET_REPEATSIG,
   R_ID_STR_SET_AUDIOPLAY,
   R_ID_STR_SET_THUMBNAIL,
   R_ID_STR_SET_BEEPSOUND,
   R_ID_STR_SET_QUALITY,
   R_ID_STR_SET_PROMT,
   R_ID_STR_RES_240P,
   R_ID_STR_RES_480P,
   R_ID_STR_RES_480FHD,
   R_ID_STR_RES_720P,
   R_ID_STR_RES_1024P,
   R_ID_STR_RES_1080P,
   R_ID_STR_RES_1080FHD,
   R_ID_STR_RES_1440P,
   R_ID_STR_RES_3024P,
   R_ID_STR_RES_720P_SHORT,
   R_ID_STR_RES_1080P_SHORT,
   R_ID_STR_RES_QVGA,
   R_ID_STR_RES_VGA,
   R_ID_STR_RES_HD,
   R_ID_STR_RES_FHD,
   R_ID_STR_RES_48M,
   R_ID_STR_RES_40M,
   R_ID_STR_RES_24M,
   R_ID_STR_RES_20M,
   R_ID_STR_RES_18M,
   R_ID_STR_RES_16M,
   R_ID_STR_RES_12M,
   R_ID_STR_RES_10M,
   R_ID_STR_RES_8M,
   R_ID_STR_RES_5M,
   R_ID_STR_RES_3M,
   R_ID_STR_RES_2M,
   R_ID_STR_RES_1M,
   R_ID_STR_SDC_NULL1,
   R_ID_STR_SDC_ERROR,
   R_ID_STR_FIL_LOCKED,
   R_ID_STR_FMT_ING,
   R_ID_STR_FMT_RESET,
   R_ID_STR_FMT_FORMAT,
   R_ID_STR_FMT_DELETE,
   R_ID_STR_PWR_LOW,
   R_ID_STR_PWR_BACKLOW,
   R_ID_STR_PWR_CHARGELOW,
   R_ID_STR_ISP_WHITEBL,
   R_ID_STR_ISP_ISO,
   R_ID_STR_ISP_ANTISHANK,
   R_ID_STR_ISP_AUTO,
   R_ID_STR_ISP_SOFT,
   R_ID_STR_ISP_STRONG,
   R_ID_STR_ISP_SUNLIGHT,
   R_ID_STR_ISP_CLOUDY,
   R_ID_STR_ISP_TUNGSTEN,
   R_ID_STR_ISP_FLUORESCENT,
   R_ID_STR_ISP_BLACKWHITE,
   R_ID_STR_ISP_SEPIA,
   R_ID_STR_ISP_ISO100,
   R_ID_STR_ISP_ISO200,
   R_ID_STR_ISP_ISO400,
   R_ID_STR_ISP_WDR,
   R_ID_STR_ISP_EXPOSURE,
   R_ID_STR_COM_SUCCESS,
   R_ID_STR_COM_FAILED,
   R_ID_STR_COM_LIGHTING_FORM,
   R_ID_STR_GAME_TIPS_GAME_OVER,
   R_ID_STR_GAME_TIPS_START,
   R_ID_STR_GAME_TIPS_PLAY_AGAIN,
   R_ID_STR_GAME_TIPS_CONTINUE,
   R_ID_STR_GAME_TIPS_EXIT,
   R_ID_STR_COM_VIDEOREC_NORMAL,
   R_ID_STR_COM_VIDEOREC_SLOW,
   R_ID_STR_COM_VIDEOREC_FAST,
   R_ID_STR_COM_LCD_BRIGHTNESS,
   R_ID_STR_GAME_NES,
   R_ID_STR_GAME_001,
   R_ID_STR_GAME_002,
   R_ID_STR_GAME_003,
   R_ID_STR_GAME_004,
   R_ID_STR_GAME_005,
   R_ID_STR_GAME_006,
   R_ID_STR_GAME_007,
   R_ID_STR_TIPS_SPI_FULL,
   R_ID_STR_MUSICLIST,
   R_STR_MAX
};

#define STRID2CFGVALUE(strid)    (strid&0xff)
enum
{ // configure id table
   CFG_LAN_ENGLISH=0,
   CFG_LAN_SCHINESE,
   CFG_LAN_TCHINESE,
   CFG_LAN_JAPANESE,
   CFG_LAN_GERMAN,
   CFG_LAN_FRECH,
   CFG_LAN_RUSSIAN,
   CFG_LAN_ITALIAN,
   CFG_LAN_KOERA,
   CFG_LAN_TAI,
   CFG_LAN_HEBREW,
   CFG_LAN_DUTCH,
   CFG_LAN_UKRAINIAN,
   CFG_LAN_SPANISH,
   CFG_LAN_PORTUGUESE,
   CFG_LAN_POLISH,
   CFG_LAN_CZECH,
   CFG_LAN_TURKEY,
   CFG_UPDATE_START,
   CFG_UPDATE_END,
   CFG_COM_OFF,
   CFG_COM_ON,
   CFG_COM_OK,
   CFG_COM_CANCEL,
   CFG_COM_YES,
   CFG_COM_NO,
   CFG_COM_LOW,
   CFG_COM_MIDDLE,
   CFG_COM_HIGH,
   CFG_COM_50HZ,
   CFG_COM_60HZ,
   CFG_COM_LEVEL_0,
   CFG_COM_LEVEL_1,
   CFG_COM_LEVEL_2,
   CFG_COM_LEVEL_3,
   CFG_COM_LEVEL_4,
   CFG_COM_LEVEL_5,
   CFG_COM_LEVEL_6,
   CFG_COM_LEVEL_7,
   CFG_COM_LEVEL_8,
   CFG_COM_LEVEL_9,
   CFG_COM_P4_0,
   CFG_COM_P3_0,
   CFG_COM_P2_0,
   CFG_COM_P1_0,
   CFG_COM_P0_0,
   CFG_COM_N1_0,
   CFG_COM_N2_0,
   CFG_COM_ALWAYSON,
   CFG_COM_ECONOMIC,
   CFG_COM_NORMAL,
   CFG_COM_FINE,
   CFG_TIM_1MIN,
   CFG_TIM_2MIN,
   CFG_TIM_3MIN,
   CFG_TIM_5MIN,
   CFG_TIM_10MIN,
   CFG_TIM_2SEC,
   CFG_TIM_3SEC,
   CFG_TIM_5SEC,
   CFG_TIM_10SEC,
   CFG_TIM_30SEC,
   CFG_PHOTO_NUM_3,
   CFG_PHOTO_NUM_5,
   CFG_SET_SETTING,
   CFG_SET_VIDEO,
   CFG_SET_PHOTO,
   CFG_SET_PLAY,
   CFG_SET_MUSIC,
   CFG_SET_GAME,
   CFG_SET_LANGUAGE,
   CFG_SET_PRINT_DENSITY,
   CFG_SET_PRINT_MODE,
   CFG_SET_DATETIME,
   CFG_SET_VOLUME,
   CFG_SET_VIDEO_RESOLUTION,
   CFG_SET_CAMERA_RESOLUTION,
   CFG_SET_FORMAT,
   CFG_SET_AUTOOFF,
   CFG_SET_SCREEN_SAVERS,
   CFG_SET_LOOPRECORD,
   CFG_SET_FREQUENCY,
   CFG_SET_DEFAULT_SETTING,
   CFG_SET_VERSION,
   CFG_SET_PRINT_GRAY,
   CFG_SET_PRINT_DOT,
   CFG_TIPS_PRINTING,
   CFG_TIPS_NO_PAPER,
   CFG_TIPS_OVER_HEAT,
   CFG_TIPS_SDC_NULL,
   CFG_TIPS_SDC_FULL,
   CFG_TIPS_NO_FILES,
   CFG_TIPS_NO_POWER,
   CFG_TIPS_PLEASE_WAITING,
   CFG_TIPS_FMT_SUCCESS,
   CFG_TIPS_FMT_FAIL,
   CFG_TIPS_DELETECUR,
   CFG_SET_AUDIOMODE,
   CFG_SET_USBMASS,
   CFG_SET_USBCAM,
   CFG_SET_TIMEPHOTO,
   CFG_SET_MOREPHOTO,
   CFG_SET_VIDEOROTATE,
   CFG_SET_MOTIONDET,
   CFG_SET_AUDIOREC,
   CFG_SET_TIMESTRAMP,
   CFG_SET_FILLIGHT,
   CFG_IR_AUTO,
   CFG_SET_GSENSOR,
   CFG_SET_PARKMODE,
   CFG_SET_FASTVIEW,
   CFG_SET_DELETEALL,
   CFG_SET_LOCKCUR,
   CFG_SET_LOCKALL,
   CFG_SET_UNLOCKCUR,
   CFG_SET_UNLOCKALL,
   CFG_SET_LOCKED,
   CFG_SET_LOCK,
   CFG_SET_DELETE,
   CFG_SET_PLAYMODE,
   CFG_SET_REPEATALL,
   CFG_SET_REPEATRAD,
   CFG_SET_REPEATSIG,
   CFG_SET_AUDIOPLAY,
   CFG_SET_THUMBNAIL,
   CFG_SET_BEEPSOUND,
   CFG_SET_QUALITY,
   CFG_SET_PROMT,
   CFG_RES_240P,
   CFG_RES_480P,
   CFG_RES_480FHD,
   CFG_RES_720P,
   CFG_RES_1024P,
   CFG_RES_1080P,
   CFG_RES_1080FHD,
   CFG_RES_1440P,
   CFG_RES_3024P,
   CFG_RES_720P_SHORT,
   CFG_RES_1080P_SHORT,
   CFG_RES_QVGA,
   CFG_RES_VGA,
   CFG_RES_HD,
   CFG_RES_FHD,
   CFG_RES_48M,
   CFG_RES_40M,
   CFG_RES_24M,
   CFG_RES_20M,
   CFG_RES_18M,
   CFG_RES_16M,
   CFG_RES_12M,
   CFG_RES_10M,
   CFG_RES_8M,
   CFG_RES_5M,
   CFG_RES_3M,
   CFG_RES_2M,
   CFG_RES_1M,
   CFG_SDC_NULL1,
   CFG_SDC_ERROR,
   CFG_FIL_LOCKED,
   CFG_FMT_ING,
   CFG_FMT_RESET,
   CFG_FMT_FORMAT,
   CFG_FMT_DELETE,
   CFG_PWR_LOW,
   CFG_PWR_BACKLOW,
   CFG_PWR_CHARGELOW,
   CFG_ISP_WHITEBL,
   CFG_ISP_ISO,
   CFG_ISP_ANTISHANK,
   CFG_ISP_AUTO,
   CFG_ISP_SOFT,
   CFG_ISP_STRONG,
   CFG_ISP_SUNLIGHT,
   CFG_ISP_CLOUDY,
   CFG_ISP_TUNGSTEN,
   CFG_ISP_FLUORESCENT,
   CFG_ISP_BLACKWHITE,
   CFG_ISP_SEPIA,
   CFG_ISP_ISO100,
   CFG_ISP_ISO200,
   CFG_ISP_ISO400,
   CFG_ISP_WDR,
   CFG_ISP_EXPOSURE,
   CFG_COM_SUCCESS,
   CFG_COM_FAILED,
   CFG_COM_LIGHTING_FORM,
   CFG_GAME_TIPS_GAME_OVER,
   CFG_GAME_TIPS_START,
   CFG_GAME_TIPS_PLAY_AGAIN,
   CFG_GAME_TIPS_CONTINUE,
   CFG_GAME_TIPS_EXIT,
   CFG_COM_VIDEOREC_NORMAL,
   CFG_COM_VIDEOREC_SLOW,
   CFG_COM_VIDEOREC_FAST,
   CFG_COM_LCD_BRIGHTNESS,
   CFG_GAME_NES,
   CFG_GAME_001,
   CFG_GAME_002,
   CFG_GAME_003,
   CFG_GAME_004,
   CFG_GAME_005,
   CFG_GAME_006,
   CFG_GAME_007,
   CFG_TIPS_SPI_FULL,
   CFG_MUSICLIST,
   CFG_ID_MAX
};

enum // langauage table
{
   LANUAGE_ENGLISH = 0,//english.txt
   LANUAGE_SCHINESE, //
   LANUAGE_TCHINESE, //
   LANUAGE_JAPANESE, //
   LANUAGE_RUSSIAN, //
   LANUAGE_KOERA, //
   LANUAGE_ITALIAN, //
   LANUAGE_DUTCH, //
   LANUAGE_GERMAN, //
   LANUAGE_POLISH, //
   LANUAGE_SPANISH, //
   LANUAGE_TAI, //
   LANUAGE_FRENCH, //
   LANUAGE_PORTUGUESE, //
   LANUAGE_MAX
};

#endif

