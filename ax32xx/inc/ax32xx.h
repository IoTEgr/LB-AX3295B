/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                           AX32XX 
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AX32XX HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef  AX32XX_H
    #define  AX32XX_H


#include <stdio.h>  // c-std lib
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "typedef.h"
#include "ax32xx_spr_defs.h"
#include "ax329x.h"

#include "ax32xx_cfg.h"
#include "../boot_config.h"

#include "ax32xx_sys.h"
#include "ax32xx_int.h"
#include "ax32xx_dma.h"
#include "ax32xx_wdt.h"
#include "ax32xx_gpio.h"
#include "ax32xx_uart.h"
#include "ax32xx_timer.h"
#include "ax32xx_tminf.h"
#include "ax32xx_spi.h"
#include "ax32xx_rtc.h"
#include "ax32xx_md.h"
#include "ax32xx_iic.h"
#include "ax32xx_dac.h"
#include "ax32xx_adc.h"
#include "ax32xx_sd.h"
#include "ax32xx_auadc.h"
#include "ax32xx_osd.h"
#include "ax32xx_lcd.h"
#include "ax32xx_csi.h"
#include "ax32xx_mjpeg.h"
#include "ax32xx_isp.h"
#include "ax32xx_usb.h"
#include "ax32xx_misc.h"
#include "ax32xx_crc.h"
#include "ax32xx_btuart.h"
#include "ax32xx_emi.h"
#include "ax32xx_pip.h"
#include "ax32xx_rotate.h"
#include "ax32xx_osdcmp.h"
//---------------------------------------ax32xx configure table------------------------------
#if  (AX32_PLATFORM == AX3295)||(AX32_PLATFORM == AX3295B)
	#define  UART0_TX_POS    UART0_POS_PA7
    #define  UART0_RX_POS     UART0_POS_NONE
    #define  UART0_BAUDRATE   115200

    #define  IIC0_POS         IIC0_POS2 //IIC0_POS_SCL_PE13_SDA_PE15
    #define  IIC1_POS         IIC1_POS_NONE//IIC1_POS6 //IIC1_POS_SCL_PE0_SDA_PE1

    #define  SD0_POS          SD0_POS0 //SD0_POS_CLK_PA3_CMD_PA2_D0_PA4_D1_PA5_D2_PA0_D3_PA1

    #define  LCD_POS          LCD_POS12 //MCU POS, LCD_POS_CS_PA4_WR_PF7_RS_PF8_TE_PF9_DATA_PD13_6
	 
	//#define  LCD_POS          LCD_POS9 //HDLCD GROUP
	
    #define  CSI_POS          CSI_POS1

    #define  JPEG_QI          4
	
	
//	#define  USENSOR_PWRCTRL_EN    0  
#else
	#warning "define me\n"
#endif

//--------------------------------------basic function define--------------------------------
#define __FP__(f)               if(f){(*f)();}
#define  AX32XX_WAIT(v,t)      while((v)&&(t--))


extern void uart_Printf(const char *pszStr, ...);
#if CFG_MCU_DBG_EN
#define debg(...)   uart_Printf(__VA_ARGS__)
#else
#define debg(...) 
#endif

#endif
