/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                     MULTIMEDIA LAYER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : USER_MEDIA.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is user MEDIA header file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  USER_MEDIA_H
    #define  USER_MEDIA_H

	
#include "../hal/inc/hal.h"
#include "../bwlib/bwlib.h"
#include "../xos/xos.h"

//typedef  FIL*        FHANDLE;
//typedef  FRESULT    __RES;

extern INT8U *jpegCache,*aviIdx1Cache,*aviIdx1CacheB, *audioCache,*thumbnailYBuffer,*thumbnailUVBuffer;

extern MSG_T audsTStack[],audsIStack[],audsBStack[],vidsIStack[],vidsBStack[],jpegIStack[],jpegBStack[];
/*******************************************************************************
* Function Name  : shareMemMalloc
* Description    : memory share for other application.for that this memory is define for multimedia using,user can only temparary using.
* Input          : INT32U size : need memory size 
* Output         : none
* Return         : 
*******************************************************************************/
void *shareMemMalloc(INT32U size);
/*******************************************************************************
* Function Name  : shareMemFree
* Description    : memory share for other application,memory share for other application.for that this memory is define for multimedia using,user can only temparary using.
* Input          : void *mm : memory addr
* Output         : none
* Return         : 
*******************************************************************************/
void shareMemFree(void *mm);
/*
extern int f_fastseek_init(FIL *fp);

extern int media_open(const char *name,INT32U op);


extern __RES media_close(FHANDLE fd);


extern __RES media_read(FHANDLE  fp,void *buff,UINT len,UINT * reslen);


extern __RES media_write(FHANDLE  fp,const void *buff,UINT len,UINT * reslen);


extern __RES media_lseek(FHANDLE fp,INT32S offset,INT32U point);


extern __RES media_merge(FHANDLE fp1,FHANDLE fp2);

extern INT32U media_fsize(FHANDLE fp);

extern INT32U media_free_size(void);
*/


//==this is yuv_buf alpha pixel yuv, comefrom bmp image==
#define YUV_ALPHA_Y	0x8A
#define YUV_ALPHA_UV	0x81


void yuv_rgb_table_init();


u8 bmp24_to_yuv420_buf(u32 id,u8 *rgb_buf,u8 *ybuf,u8 *uvbuf,u16 w,u16 h);

//==tips: draw_x,drawy,src_w,src_h is 2X size,X is pixel num==
void yuv420_draw_buf(u8 *dst_ybuf,u16 dst_w,u16 dst_h,s16 draw_x,s16 draw_y,u16 draw_w,u16 draw_h,u8 *src_ybuf,u16 src_w,u16 src_h,u8 alpha_y,u8 alpha_uv);

u8 jpg_decode_buf(u32 jpg_id,u8 *ybuf,u8 *uvbuf,u16 w,u16 h);
u8 jpg_buf_decode_buf(u8* jpg_buf,u8 *ybuf,u8 *uvbuf,u16 w,u16 h);


enum
{
	CAM_EFFECT_0=0,			//  buf flip
	CAM_EFFECT_1,			//  Prismatic Mirror
//	CAM_EFFECT_2,			//  ConcavelensMirror
//	CAM_EFFECT_3,			//  ConvexlensMirror
	CAM_EFFECT_2,			//  trilateral_16_window
	CAM_EFFECT_3,			//  stream_10_window
	CAM_EFFECT_4,			//  rismatic_multiwindow
	CAM_EFFECT_5,			//  hexagon_multiwindow

	CAM_EFFECT_MAX
};
void yuv420_software_set_handle_frame(disp_frame_t * frame);
disp_frame_t * yuv420_software_get_handle_frame();

void yuv420_effect_handle(u8* ybuf,u8*uvbuf,u16 buf_w,u16 buf_h);

#endif
