/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                     HARD WARE MANNAGER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_cfg.h
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
#ifndef  HAL_CFG_H
#define  HAL_CFG_H


//debug uart output
#define  HAL_CFG_EN_DBG             1   // debug printf

//mcu moudle function
#define  HAL_CFG_EN_IIC0            1
#define  HAL_CFG_EN_IIC1            0
#define  HAL_CFG_EN_LCD             1
#define  HAL_CFG_EN_OSD             1

//cache 
#define  HAL_CFG_EN_LINE            1    // line cache 1k-byte for icon,string ,mjpeg decode
#define  HAL_CFG_EN_OSDTEMP         1   // osd temp buffer for osd drawing

//lcd
#define  HAL_CFG_LCD_BUFFER_NUM     5  //hal-lcd will define the buffer using by auto check 

//osd 
#define  HAL_CFG_OSD0_ENABLE        1
#define  HAL_CFG_OSD1_ENABLE        0

//uart
#define  HAL_CFG_UART0_BAUDRATE     115200

//adc
#define  HAL_CFG_ADC_BAUDRATE       1000000UL

// iic
#define  HAL_CFG_IIC0_BAUDRATE      200000   // 400000
#define  HAL_CFG_IIC1_BAUDRATE      40000   // 40k

//spi
#define  HAL_CFG_SPI_BAUD_RATE      6000000
#define  HAL_CFG_SPI_DMA_USE        1
#define  HAL_CFG_SPI_DMA_THROD      4096


//auadc buffer size
#define  HAL_CFG_PCM_BUFFER_SIZE    8192
#define  HAL_CFG_PCM_BUFFER_NUM     6// 

//mjpeg
#define  HAL_CFG_MJPEG_BUFFER_NUM		128// 256//HAL_RSV_MJPEG_BUFFER_NUM)// 8  64  (256+32)//AVI_CBUF_SIZE/AVI_SHEET_SIZE  
#define  HAL_CFG_MJPEG_QULITY_AUTO  	1  // auto ajust qulity when mjpeg size is to small or big
#define  HAL_CFG_MJPEG_720_SIZE_MIN     (40*1024)
#define  HAL_CFG_MJPEG_720_SIZE_MAX     (60*1024)
#define  HAL_CFG_MJPEG_1080_SIZE_MIN    (70*1024)
#define  HAL_CFG_MJPEG_1080_SIZE_MAX    (90*1024)

#define  HAL_CFG_MJPEGB_BUFFER_SIZE   (1024*1024)
// cache
#define  HAL_CFG_LINE_CACHE         1024 

#define  WAV_HEADER_SIZE	48
#define  VOLUME_LEVEL 	14

#define  HAL_CFG_OS_TIMER            TIMER0

#define  HAL_CFG_SYNC_TIMER         TIMER1   // sync to video play & video record


#define  LCD_CROP_SCALE       1 //lcd crop scale show image ,  0 : is disable , 1 : is enable 
#define  MJPG_CROP_SCALE      1 //lcd crop scale show image ,  0 : is disable , 1 : is enable 

#define  MSC_LABLE_EN 		0 	// 0: disable , 1: enable
#define  MSC_LABEL	"JRX"

#endif

