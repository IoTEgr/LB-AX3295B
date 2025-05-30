/****************************************************************************
**
**                              DEVICE
** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
**** **                  DEVICE CMOS-SENSOR HEADER
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : cmos_sensor.h
* Author      : Mark.Douglas
* Version     : V100
* Date        : 09/22/2016
* Description : This file is CMOS-SENSOR device file
*
* History     :
* 2016-09-22  :
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef CMOS_SENSOR_H
#define CMOS_SENSOR_H

#include "../../hal/inc/hal.h"

#include "../tft_lcd/tft_lcd.h"

#define Sensor_Op_t _Sensor_Adpt_
#define Sensor_Init_t _Sensor_Ident_

#define CMOS_VGA_SUPPORT 1

#define SENSOR_HEADER_SECTION SECTION(".sensor_res.header")
#define SENSOR_HEADER_ITEM_SECTION SECTION(".sensor_res.header.items")

#define SENSOR_OP_SECTION SECTION(".sensor_res.struct") static
#define SENSOR_INIT_SECTION SECTION(".sensor_res.init_tab") static
#define CMOS_INIT_LEN_MAX 4096
/*
#define _SEN_YGAMA_CLASSES_    8
#define _SEN_RGBGAMA_CLASSES_    8
#define  _SEN_YGAMA_TABLE_       __attribute__ ((section(".sensor_res.isp_tab")))   static
#define  _SEN_RGBGAMA_TABLE_       __attribute__ ((section(".sensor_res.isp_tab")))   static
#define  _SEN_LSC_TABLE_       __attribute__ ((section(".sensor_res.isp_tab")))   static
#define  _SEN_AWB_TABLE_       __attribute__ ((section(".sensor_res.isp_tab")))   static
*/
#define DEV_SENSOR_BF2013 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_BF20A6 1 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_BF3703 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_BF3A03 1 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_GC0307 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_GC0308 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_GC0309 1 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_GC0328 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_GC0329 1 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_OV7670 1 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_OV7725 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_SP0718 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_SP0838 0 & CMOS_VGA_SUPPORT
#define DEV_SENSOR_SIV100B 1 & CMOS_VGA_SUPPORT

extern Sensor_Init_t *devSensorInit;
extern Sensor_Op_t *devSensorOp;

/*******************************************************************************
 * Function Name  : sensor_iic_enable
 * Description    : enable senor iic for write/read
 * Input          : none
 * Output         : none
 * Return         : none
 *******************************************************************************/
void sensor_iic_enable(void);
/*******************************************************************************
 * Function Name  : sensor_iic_disable
 * Description    : disable senor iic for write/read
 * Input          : none
 * Output         : none
 * Return         : none
 *******************************************************************************/
void sensor_iic_disable(void);
/*******************************************************************************
 * Function Name  : sensor_iic_write
 * Description    : senor iic  write
 * Input          : INT8U *data : data & addr
 * Output         : none
 * Return         : none
 *******************************************************************************/
void sensor_iic_write(INT8U *data);
/*******************************************************************************
 * Function Name  : sensor_iic_read
 * Description    : senor iic  read
 * Input          : INT8U *data : data & addr
 * Output         : none
 * Return         : none
 *******************************************************************************/
INT32U sensor_iic_read(INT8U *data);
/*******************************************************************************
* Function Name  : sensor_iic_info
* Description    : senor iic  slave set
* Input          : INT8U wid : write id
                      INT8U rid  : read id
                      INT8U awidth : addr width
                      INT8U dwidth : data width
* Output         : none
* Return         : none
*******************************************************************************/
void sensor_iic_info(INT8U wid, INT8U rid, INT8U awidth, INT8U dwidth);
/*******************************************************************************
 * Function Name  : sensorInit
 * Description    : initial cmos sensor
 * Input          : none
 * Output         : none
 * Return         : none
 *******************************************************************************/
void sensorInit(void);
/*******************************************************************************
 * Function Name  : uart_to_iic
 * Description    :
 * Output         : None
 * Return         :
 *******************************************************************************/
INT8U *uart_to_iic(u8 *p);
/*******************************************************************************
 * Function Name  : SensorGetName
 * Description    : get Sensor name
 * Input          :
 * Output         : none
 * Return         : char *
 *******************************************************************************/
char *SensorGetName(void);

/*******************************************************************************
 * Function Name  : sensorDBGService
 * Description	  : sensor on line debug initial
 * Input		  : none
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void sensorDBGInit(void);
/*******************************************************************************
 * Function Name  : sesnorDBGService
 * Description	  : sensor on line debug service.user should not call this function in isr
 * Input		  : none
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void sesnorDBGService(void);
/*******************************************************************************
 * Function Name  : sensorDBGService
 * Description	  : sensor on line debug timeout check
 * Input		  : none
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void sensorDBGTimeout(void);
/*******************************************************************************
 * Function Name  : sensorDBGRecv
 * Description	  : sensor on line debug recivce
 * Input		  : u8 data : data
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void sensorDBGRecv(u8 data);

/*******************************************************************************
 * Function Name  : CopyToMember
 * Description	  : copy data to member
 * Input		  :void* ispModuleData, int offsetOfMember, int sizeOfMember, void* pbdata, int startPos, int trans_len
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void CopyToMember(void *ispModuleData, int offsetOfMember, int sizeOfMember, void *pbdata, int startPos, int trans_len);

/*******************************************************************************
 * Function Name  : cmos_spec_color
 * Description    : set spec color
 * Input          : index 0: normal color 1:red color,2 green color ,3 blue color , 4 B&W color , 5 old time color
 * Output         : none
 * Return         : none
 *******************************************************************************/
void cmos_spec_color(u8 index);
void sensor_rotate(u32 r);

#endif
