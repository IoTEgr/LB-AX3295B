/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                  THE APPOTECH HAL
   **** **                 MJPEG HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_mjpeg.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN MJPEG HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef HAL_MJPEG_H
     #define HAL_MJPEG_H

#define MJPEG_SRC_CSI 		        0
#define MJPEG_SRC_BAR		        1
#define MJPEG_SRC_RAM            	2

#define MJPEG_SRC 	                MJPEG_SRC_CSI//MJPEG_SRC_RAM//
	
#define MJPEG_SINGLE_LINEBUF        1
#define MJPEG_DOUBLE_LINEBUF		2	
#define MJPEG_USE_BUF				MJPEG_SINGLE_LINEBUF //1: single linebuf, 2: double linebuf

#include "../../xos/xos.h"
#include "hal_stream.h"
/*
typedef union
{
	u8 mjp[HAL_CFG_MJPEG_BUFFER_NUM][HAL_CFG_MJPEG_BUFFER_SIZE];
	u8 uvc[HAL_UVC_BUFFER_NUM][HAL_UVC_BUFFER_SIZE];		
}_MJBUF_;

typedef struct _MJPEG_S
{
	_MJBUF_ mjpeg_buffer;
	u8 y_buffer[HAL_CFG_MJPEG_WIDTH * HAL_CFG_MJPEG_HEIGHT];
	u8 uv_buffer[HAL_CFG_MJPEG_WIDTH * HAL_CFG_MJPEG_HEIGHT/2];
}_MJPEG_T;
*/
typedef struct _MJQDT
{
	u8 sta;
	s8 endf;
	s32 tgl;
	u32 fsize;
	u8  *pbuf;
	u32 buflen;
	u8  *dribuf;
	struct _MJQDT *phead; //JPG?a篓潞?QDT
	struct _MJQDT *plink; //JPG??篓掳???QDT
	struct _MJQDT *next;  //??篓掳???QDT
}QDT_MJP;
#define  MJPEG_ITEM_NUM         HAL_CFG_MJPEG_BUFFER_NUM

typedef struct MJPEG_ENC_OP_S
{
	u8 type;
	u8 drop;
	u8 frame_enable;
#if HAL_CFG_MJPEG_QULITY_AUTO>0
	s8 qulity;
    u8 q_cnt;
	u8 q_auto;
	s8 q_dir;
	u32 q_csize;
#endif
	u16 csi_width;
	u16 csi_height;
    u16 mjpeg_width;
	u16 mjpeg_height;

	u32 jfcnt;
    u8 *ybuffer;
	u8 *uvbuffer;

	u32 ybuffer_limage;
	u32 uvbuffer_limage;
	u32 line_buf_using;  //0: no use, 1: ybuffer, 2: ybuffer_tmp
	u32 line_buf_deal;   //0: no use, 1: ybuffer, 2: ybuffer_tmp
	u32 csi_to_mjpeg_dma; //1:csi_to_wifi_dma, 2: csi_to_mjpeg_dma
	u32 mjpeg_encode_done;
	u8 *mjpbuf;
	u32 mjpsize;
	u8 photo_flag;
	XMsg *photoCahQ;
	u32 curBuffer;
	u32 curEncodeBuffer;
	u32 curLen;
	Stream_Head_T vids;
	Stream_Node_T mjpegNode[MJPEG_ITEM_NUM];
	u8 yuvFlag;
	u32 lastFrameLen;
}MJPEG_ENC_OP_T;
u16 dec_lheight;	
u16 dec_lwidth; 
MJPEG_ENC_OP_T  mjpegEncCtrl;

typedef struct _JMSG{
	void *pmsg;
	struct _JMSG *next;	
}QUE_J;

typedef struct JPEG_PHOTO_CACHE_S{
	uint32 mem;
	uint32 len;
	uint32 flag;
}JPEG_PHOTO_CACHE_T;

JPEG_PHOTO_CACHE_T jpeg_photo_cache[20];

u32 HAL_CFG_MJPEG_HEIGHT,HAL_CFG_MJPEG_WIDTH;


/*******************************************************************************
* Function Name  : hal_mjpegInit
* Description    : hal layer .mjpeg initial
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpegEncodeInit(void);
/*******************************************************************************
* Function Name  : hal_mjpegEncodeUninit
* Description    : hal layer .mjpeg uninitial 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpegEncodeUninit(void);
/*******************************************************************************
* Function Name  : hal_mjpegInit
* Description    : hal layer .mjpeg memory initial
* Input          : 0: video , 1: photo
* Output         : None
* Return         : None
*******************************************************************************/
int hal_mjpegMemInit(u8 flag);

/*******************************************************************************
* Function Name  : hal_mjpegMemUninit
* Description    : hal layer .mjpeg memory uninitial
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpegMemUninit(void);
void hal_mjpegBMemUninit(void);
/*******************************************************************************
* Function Name  : hal_mjpegEncodeResolution
* Description    : hal layer .mjpeg line scaler cal
* Input          : u16 width : target width
                      u16 height: target height
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpegEncodeResolution(u16 width,u16 height);
/*******************************************************************************
* Function Name  : hal_mjpegEncodeResolutionImage
* Description    : hal layer .mjpeg line scaler cal for pic 
* Input          : u16 width : target width
                      u16 height: target height
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpegEncodeResolutionImage(u16 width,u16 height);

u8 hal_mjpeg_manual_get_csi_yuv_sta();
void hal_mjpeg_software_handle_csi_yuvbuf();

//==ret : 0 is ok , 1 is err==
u8 hal_mjpg_scale_down(u8 * jpeg_src,u16 dst_width,u16 dst_height,u8 q,u32 *jpg_addr,u32 *jpg_size);



/*******************************************************************************
* Function Name  : hal_mjpegSizecalculate
* Description    : hal layer .mjpeg output jpeg size
* Input          : u8 quailty : quality
                      u16 width : output width
                      u16 height: output height
* Output         : None
* Return         : int : size
*******************************************************************************/
int hal_mjpegSizecalculate(u8 quailty,u16 width,u16 height);
/*******************************************************************************
* Function Name  : hal_mjpegUVCInit
* Description    : hal layer .mjpeg initial for uvc
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int hal_mjpegUVCStart(u16 win_w, u16 win_h, u8 quality);
/*******************************************************************************
* Function Name  : hal_mjpegPhotoInit
* Description    : hal layer .mjpeg initial for photo
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//int hal_mjpegPhotoStart(u16 win_w, u16 win_h, u8 quality, u8 timestramp);
int hal_mjpegPhotoStart(u16 win_w, u16 win_h, u8 quality, u8 timestramp,u8 frame_enable);

/*******************************************************************************
* Function Name  : hal_mjpegAVIInit
* Description    : hal layer .mjpeg initial for avi
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int hal_mjpegAVIStart(u16 win_w, u16 win_h, u8 quality, u8 timestramp);

/*******************************************************************************
* Function Name  : hal_mjpegEncodestop
*Bref                 : void hal_mjpegEncodeStop(void)
* Description    : hal layer .mjpeg stop 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#define hal_mjpegEncodeStop         hal_mjpegEncodeUninit
/*******************************************************************************
* Function Name  : hal_mjpegRawDataGet
* Description    : hal layer.get mjpeg fram raw data
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void *hal_mjpegRawDataGet(void **hbuff,u32 *addr,u32 *len);
/*******************************************************************************
* Function Name  : hal_mjpegRawBufferGet
* Description    : hal layer.check raw buffer addr &length
* Input          : u32 *addr : addr
                      u32 *len : length
* Output         : None
* Return         : int <0  : end
*******************************************************************************/
void *hal_mjpegRawBufferGet(void *buffer,u32 *addr,u32 *len,s32 *sync, s32 *sync_next);
/*******************************************************************************
* Function Name  : hal_mjpegRawBufferfree
* Description    : hal layer.set mjpeg fram raw data
* Input          : void *buffer : frame buffer
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpegRawBufferfree(void);
/*******************************************************************************
* Function Name  : hal_mjpegRawDataSet
* Description    : hal layer.set mjpeg fram raw data
* Input          : void *buffer : frame buffer
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpegRawDataSet(void *buffer);
/*******************************************************************************
* Function Name  : hal_mjpegDecodeGetResolution
* Description    : hal layer.mjpeg decode get jpeg size
* Input          :  u16 width: out size width
                      u16 height:out size height
* Output         : None
* Return         : None
*******************************************************************************/
int hal_mjpegDecodeGetResolution(u16 *width,u16 *height);
/*******************************************************************************
* Function Name  : hal_mjpegDecodeSetResolution
* Description    : hal layer.mjpeg decode Set jpeg size
* Input          :  u16 width: out size width
                      u16 height:out size height
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpegDecodeSetResolution(u16 width,u16 height);
/*******************************************************************************
* Function Name  : hal_mjpegDecodePicture
* Description    : hal layer.mjpeg decode one picture
* Input          : u8 * p_jpeg_start : buffer
                      u8 * p_out_y : out ybuffer
                      u8 * p_out_uv: out uvbuffer
                      u16 dst_width: out size width
                      u16 dst_height:out size height
* Output         : None
* Return         : int 0:success
                          <0: fail
*******************************************************************************/
int hal_mjpegDecodePicture(u8 * p_jpeg_start,u8 * p_out_y,u8 * p_out_uv,u16 dst_width,u16 dst_height);

int hal_mjpegDecodeNoIsr(u8 * p_jpeg_start,u8 * p_out_y,u8 * p_out_uv,u16 dst_width,u16 dst_height);
/*******************************************************************************
* Function Name  : hal_mjpegDecodeParse
* Description    : hal layer.mjpeg decode stop
* Input          :  u8 * p_jpeg_start : buffer
                       u16 dst_width : out width
                       u16 dst_height: out height
* Output         : None
* Return         : 0 : success
                      <0 : fail
*******************************************************************************/
int hal_mjpegDecodeParse(u8 * p_jpeg_start,u16 dst_width,u16 dst_height);
/*******************************************************************************
* Function Name  : hal_mjpegDecodeOneFrame
* Description    : hal layer.mjpeg decode one frame,video file using
* Input          :  u8 * p_jpeg_start : buffer
                       u8 * p_out_y : out ybuffer
                       u8 * p_out_uv:out uvbuffer
* Output         : None
* Return         : 0 : success
                      <0 : fail
*******************************************************************************/
void hal_mjpegDecodeOneFrame(u8 * p_jpeg_start,u8 * p_out_y,u8 * p_out_uv);
/*******************************************************************************
* Function Name  : hal_mjpegDecodeOneFrame_ext
* Description    : hal layer.mjpeg decode one frame,
* Input          :  u8 * p_jpeg_start : buffer
                       u8 * p_out_y : out ybuffer
                       u8 * p_out_uv:out uvbuffer
* Output         : None
* Return         : 0 : success
                      <0 : fail
*******************************************************************************/
void hal_mjpegDecodeOneFrame_Ext(u8 * p_jpeg_start,u8 * p_out_y,u8 * p_out_uv);
/*******************************************************************************
* Function Name  : hal_mjpegDecodeStatusCheck
* Description    : hal layer.mjpeg decode get status
* Input          :  
* Output         : None
* Return         : 1 : busy
                       0: idle
*******************************************************************************/
int hal_mjpegDecodeStatusCheck(void);
/*******************************************************************************
* Function Name  : hal_mjpegDecodeErrorCheck
* Description    : hal layer.mjpeg decode get error
* Input          :  
* Output         : None
* Return         : 0 : no error
*******************************************************************************/
u32 hal_mjpegDecodeErrorCheck(void);
/*******************************************************************************
* Function Name  : hal_mjpegDecodeStop
* Description    : hal layer.mjpeg decode stop
* Input          :  
* Output         : None
* Return         : 
*******************************************************************************/
void hal_mjpegDecodeStop(void);
/*******************************************************************************
* Function Name  : hal_mjpegDecodeOneFrame_Fast
* Description    : quickly mjpeg decoder initial and kick decoder run
*                  NEED call hal_mjpegDecodeInit before call this function
* Input          : u8 * p_jpeg_start : buffer
                   u8 * p_out_y : out ybuffer
                   u8 * p_out_uv: out uvbuffer
                   u16 dst_width: out size width
                   u16 dst_height:out size height
                   u16 stride :
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpegDecodeOneFrame_Fast(u8 * p_jpeg_start,u8 * p_out_y,u8 * p_out_uv,u16 dst_width,u16 dst_height,u16 stride);
/*******************************************************************************
* Function Name  : hal_mjpegDecodeReset
* Description    : hal layer.mjpeg decoder reset
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int hal_mjpegDecodeReset(void);
/*******************************************************************************
* Function Name  : hal_mjpegDecodeRestart_Ext
* Description    : hal layer.mjpeg decode one frame,use last configration.
* Input          :  u8 * p_jpeg_start : buffer
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpegDecodeRestart_Ext(u8 * p_jpeg_start);

int hal_BackRecDecodeStatusCheck(void);

/*******************************************************************************
* Function Name  : hal_mjpegRawBuffer_prefull
* Description    : hal layer.mjpeg buffer pre full(2/3) just
* Input          : None
* Output         : None
* Return         : true:mjpeg buffer is pre_full
*******************************************************************************/
bool hal_mjpegRawBuffer_prefull(void);
bool hal_mjpB_Buffer_prefull(void);
bool hal_mjpegRawBuffer_halffull(void);
/***********hal_mjpegB********/
void hal_mjpB_ecdown(void);
void api_mjp_avi_stop(void);
void hal_mjpB_ecbusy(void);
void hal_curBuffer_clear(void);

int hal_mjpegHeaderParse (u8 * p_jpeg_header);


void hal_mjpeg_manual_quad_on(void);

void hal_mjpeg_manual_quad_fdown(int flag);

void hal_mjpeg_queue_init();

void hal_mjpeg_rev_buf_init();

s8 hal_mjpegQualityCheck(u8 quality);

#endif

