/****************************************************************************
**
 **                              CONFIGURE
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  CONFIGURE
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : config.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image encode file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "application.h"


typedef struct
{
	INT32U flag[127];	// must  CFG_ID_MAX < 127 
	INT32U CheckSum;
}SYSTEM_FLAY;		// size must 256 aligned 
static SYSTEM_FLAY SysFlay __attribute__ ((aligned(32)));
static INT32U configAddress=0;

/*******************************************************************************
* Function Name  : userConfigReset
* Description    : reset or default user configure value
* Input          : none
* Output         : None
* Return         : s32_t 
*                    0 : always
*******************************************************************************/
s32_t userConfigReset(void)
{
    deg_Printf("config : reset\n");
	configSet(CONFIG_ID_YEAR,             2022);
	configSet(CONFIG_ID_MONTH,            1);
	configSet(CONFIG_ID_MDAY,             1);
	configSet(CONFIG_ID_WDAY,             4);
	configSet(CONFIG_ID_HOUR,             0);
	configSet(CONFIG_ID_MIN,              0);
	configSet(CONFIG_ID_SEC,              0);
	configSet(CONFIG_ID_LANGUAGE,         R_ID_STR_LAN_ENGLISH);
	configSet(CONFIG_ID_AUTOOFF,          R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_SCREENSAVE,       R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_FREQUNCY,         R_ID_STR_COM_50HZ);
	configSet(CONFIG_ID_ROTATE,           R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_FILLIGHT,         R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_RESOLUTION,       R_ID_STR_RES_1080P_SHORT);
	configSet(CONFIG_ID_TIMESTAMP,        R_ID_STR_COM_ON);
	configSet(CONFIG_ID_MOTIONDECTION,    R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_TIMEPHOTO,        R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_PARKMODE,         R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_GSENSOR,          R_ID_STR_COM_MIDDLE);
	configSet(CONFIG_ID_KEYSOUND,         R_ID_STR_COM_ON);
	configSet(CONFIG_ID_IR_LED,           R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_LOOPTIME,         R_ID_STR_TIM_3MIN);
	configSet(CONFIG_ID_AUDIOREC,         R_ID_STR_COM_ON);
	configSet(CONFIG_ID_EV,               R_ID_STR_COM_P0_0);
	configSet(CONFIG_ID_WBLANCE,          R_ID_STR_ISP_AUTO);
	configSet(CONFIG_ID_PRESLUTION,       R_ID_STR_RES_18M);
	configSet(CONFIG_ID_PFASTVIEW,        R_ID_STR_COM_OFF/*R_ID_STR_TIM_2SEC*/);
	configSet(CONFIG_ID_PTIMESTRAMP,      R_ID_STR_COM_ON);
	configSet(CONFIG_ID_PEV,              R_ID_STR_COM_P0_0);
	configSet(CONFIG_ID_VOLUME,           R_ID_STR_COM_LEVEL_8);
	configSet(CONFIG_ID_LCD_BRIGHT,       R_ID_STR_COM_LEVEL_8);
	configSet(CONFIG_ID_THUMBNAIL,        R_ID_STR_COM_ON);
	configSet(CONFIG_ID_GSENSORMODE,      R_ID_STR_COM_ON);

	userConfigSave();



	return 0;
}
static configReadFromSD(INT32U buff,INT32U len)
{
	int fd;
    UINT size;
    fd = open((const char *)"user.cfg",FA_READ);
	if(fd<0)
	    fd = open((const char *)"user.cfg",FA_READ|FA_CREATE_NEW);
	read((FHANDLE)fd,(void *)buff,len);
    close((FHANDLE)fd);
}
static void configWriteToSD(INT32U buff,INT32U len)
{
	int fd;
    UINT size;
    fd = open((const char *)"user.cfg",FA_READ|FA_WRITE);
	lseek((FHANDLE)fd,0,0);
	write((FHANDLE)fd,(void *)buff,len);	
	close((FHANDLE)fd);
}
static configReadFromSPI(INT32U buff,INT32U len)
{
    INT32U i=0,readPages=len>>8;
	for(i=0;i<readPages;i++)
	{
		hal_spiFlashRead(configAddress+256*i,(INT32U)(buff+256*i),256);
	}
}
static void configWriteToSPI(INT32U buff,INT32U len)
{
	 INT32U i=0,writePages=len>>8;
	 hal_spiFlashEraseSector(configAddress);
     for(i=0;i<writePages;i++)
     {
	     hal_spiFlashWrite(configAddress+256*i,(INT32U)(buff+256*i),256);
     }
}
static void config_read(INT32U buff,INT32U len)
{
#if NVFS_STROAGE == NV_STROAGE_SDC 
	configReadFromSD(buff,len);
#elif NVFS_STROAGE == NV_STROAGE_SPI
	configReadFromSPI(buff,len);
#elif NVFS_STROAGE == NV_STROAGE_SDRAM
	configReadFromSPI(buff,len);
#endif
}
static void config_write(INT32U buff,INT32U len)
{
#if NVFS_STROAGE == NV_STROAGE_SDC 
	configWriteToSD(buff,len);
#elif NVFS_STROAGE == NV_STROAGE_SPI
	configWriteToSPI(buff,len);
#elif NVFS_STROAGE == NV_STROAGE_SDRAM
	configWriteToSPI(buff,len);
#endif
}

INT32U configCheckSumCal(void)
{
	INT32U CheckSum=0,i;
	for(i=0;i<sizeof(SysFlay.flag)/sizeof(SysFlay.flag[0]);i++)
	{
		CheckSum+=SysFlay.flag[i];
	}
	if(CheckSum==0)
		CheckSum=0xaa55aa55;
	return CheckSum;
}
/*******************************************************************************
* Function Name  : userConfigSave
* Description    : save user configure value to spi flash
* Input          : none
* Output         : None
* Return         : s32_t 
*                    0 : always
*******************************************************************************/
s32_t userConfigSave(void)
{
  	SysFlay.CheckSum=configCheckSumCal();
	deg_Printf("write config check sum:0x%x\n",SysFlay.CheckSum);
	config_write((INT32U)(&SysFlay),sizeof(SYSTEM_FLAY));
    return 0;
}

/*******************************************************************************
* Function Name  : userConfigInitial
* Description    : initial user configure value
* Input          : none
* Output         : None
* Return         : s32_t 
*                      0 : always
*******************************************************************************/
s32_t userConfigInitial(void)
{
    INT32U CheckSum=0;
	configAddress=nv_configAddr();
	config_read((INT32U)(&SysFlay),sizeof(SYSTEM_FLAY));
	CheckSum=configCheckSumCal();
	deg_Printf("read config check sum:0x%x,0x%x\n",CheckSum,SysFlay.CheckSum);
	if((CheckSum!=SysFlay.CheckSum)||(0 == CheckSum))
	{
		deg_Printf("config error,reset flag.0x%x\n",SysFlay.CheckSum);
		memset(&SysFlay,0,sizeof(SYSTEM_FLAY));
		userConfigReset();
	}
	/*
	int i;
	for(i=0;i < 32;i++)
	{
		deg_Printf("SysFlay.flag[%d]=0x%x\n",i,SysFlay.flag[i]);
	}
	*/
    return 0;
}
/*******************************************************************************
* Function Name  : configSet
* Description    : set configure value
* Input          : u8_t configId : configure id
*                  u32_t value   : configure value
* Output         : None
* Return         : none
*******************************************************************************/
void configSet(u8_t configId,u32_t value)
{
	if(configId<sizeof(SysFlay.flag)/sizeof(SysFlay.flag[0]))
    	SysFlay.flag[configId]=value;
}

/*******************************************************************************
* Function Name  : userCofigGetValue
* Description    : get configure value in configure table
* Input          : u8_t configId : configure id
* Output         : None
* Return         : u32_t : configure value
*******************************************************************************/
u32_t configGet(u8_t configId)
{
    if(configId<sizeof(SysFlay.flag)/sizeof(SysFlay.flag[0]))
		return SysFlay.flag[configId];
    return 0;
}


