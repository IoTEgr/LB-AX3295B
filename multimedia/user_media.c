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
* File Name   : USER_MEDIA.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is user MEDIA header file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
* 2017-04-24  : mark
*      <1>.Modify memory define,to hal_sys.c,share memory width sytem other application.
******************************************************************************/
#include "media.h"
#include "multimedia.h"

#include "../ax32_platform_demo/application.h"

//---------------memory define for multimedia------------------------------------------------

//SECTION(".big_buffer") ALIGNED(32) INT8U aviIdx1Cache[AVI_CFG_IDX1_BUFF_SIZE];  // for avi record & play,cache idx1 table

//SECTION(".big_buffer") ALIGNED(32) INT8U jpegCache[JPEG_CFG_CACHE_SIZE*JPEG_CFG_CACHE_NUM]; // for jpeg decode cache

//SECTION(".big_buffer") ALIGNED(32) INT8U audioCache[AUDIO_CFG_BUFFER_SIZE*AUDIO_CFG_BUFFER_NUM]; // for audio decode cache

//SECTION(".big_buffer") ALIGNED(64) INT8U thumbnailYBuffer[THUMBNAIL_CFG_NUM*THUMBNAIL_CFG_WIDTH*THUMBNAIL_CFG_HEIGHT],thumbnailUVBuffer[THUMBNAIL_CFG_NUM*THUMBNAIL_CFG_WIDTH*THUMBNAIL_CFG_HEIGHT/2];

MSG_T audsIStack[AUDIO_CFG_BUFFER_NUM],audsTStack[AUDIO_CFG_BUFFER_NUM],audsBStack[AUDIO_CFG_BUFFER_NUM],vidsIStack[VIDEO_CFG_BUFFER_NUM],vidsBStack[VIDEO_CFG_BUFFER_NUM],jpegIStack[JPEG_CFG_CACHE_NUM],jpegBStack[JPEG_CFG_CACHE_NUM];
//------------memory share width other application.[mark modify .2017/04/24]
INT8U *jpegCache;// = (INT8U *)&halSysMemMJPEGBuffer;
INT8U *audioCache;// = (INT8U *)&halSysMemPCMBuffer;
INT8U *aviIdx1Cache;// = halSysMemIdxBuffer;  
INT8U *aviIdx1CacheB;
// thumbnail using the frame of jpeg cache buffer
INT8U *thumbnailYBuffer;// = ((INT8U *)&halSysMemMJPEGBuffer)+JPEG_CFG_CACHE_SIZE;
INT8U *thumbnailUVBuffer;// = (((INT8U *)&halSysMemMJPEGBuffer)+JPEG_CFG_CACHE_SIZE+THUMBNAIL_CFG_NUM*THUMBNAIL_CFG_WIDTH*THUMBNAIL_CFG_HEIGHT);
#if 0

typedef struct SHARE_MEM_S
{
	void *addr;
	INT32U size;
}SHARE_MEM_T;

#define  SHARE_MEM_NUM     3
static SHARE_MEM_T SHARE_MEM_NODE[SHARE_MEM_NUM]=
{
	{// avi idx cache buffer
		.addr = (void *)&halSysMemIdxBuffer, // aviIdx1Cache
		.size = AVI_CFG_IDX1_BUFF_SIZE,
    },
    {// jpeg file cache buffer
		.addr = (void *)&halSysMemMJPEGBuffer, // jpegCache
		.size = JPEG_CFG_CACHE_SIZE*JPEG_CFG_CACHE_NUM,
    },
    {// audio cache buffer
		.addr = (void *)&halSysMemPCMBuffer,// audioCache,
		.size = AUDIO_CFG_BUFFER_SIZE*AUDIO_CFG_BUFFER_NUM,
    }

};

int multimediaInit(void)
{
    return 0;
}
/*******************************************************************************
* Function Name  : shareMemMalloc
* Description    : memory share for other application.for that this memory is define for multimedia using,user can only temparary using.
* Input          : INT32U size : need memory size 
* Output         : none
* Return         : 
*******************************************************************************/
void *shareMemMalloc(INT32U size)
{
	int idx,i;

	idx = SHARE_MEM_NUM;
	for(i=0;i<SHARE_MEM_NUM;i++)
	{
		if((SHARE_MEM_NODE[i].size&0x80000000)==0)
		{
			if(size == SHARE_MEM_NODE[i].size)
				break;
			else if(size<SHARE_MEM_NODE[i].size)
			{
				if(idx == SHARE_MEM_NUM)
					idx = i;
			}
		}
	}

	if(i>=SHARE_MEM_NUM && idx<SHARE_MEM_NUM)
		i = idx;
	if(i<SHARE_MEM_NUM)
	{
		SHARE_MEM_NODE[i].size|=0x80000000;
		return SHARE_MEM_NODE[i].addr;
	}

	return NULL;
}
/*******************************************************************************
* Function Name  : shareMemFree
* Description    : memory share for other application,memory share for other application.for that this memory is define for multimedia using,user can only temparary using.
* Input          : void *mm : memory addr
* Output         : none
* Return         : 
*******************************************************************************/
void shareMemFree(void *mm)
{
	int i;

	for(i=0;i<SHARE_MEM_NUM;i++)
	{
		if(SHARE_MEM_NODE[i].addr == mm)
			SHARE_MEM_NODE[i].size&=~0x80000000;
	}	
}


#endif



u8 jpg_decode_buf(u32 jpg_id,u8 *ybuf,u8 *uvbuf,u16 w,u16 h)
{
	Image_ARG_T arg;
	u16 dec_w,dec_h;
	arg.target.width=w;
	arg.target.height=h;
	arg.yout  = ybuf;
	arg.uvout = uvbuf;
	arg.media.type = MEDIA_SRC_NVFS;
	arg.media.src.fd = (FHANDLE)jpg_id;
    arg.wait = 1;  // wait decode end
	if(imageDecodeStart(&arg)<0)
	{
		deg_Printf("image decode fail.\n");
		return 1;
	}

	imageDecodeGetResolution(&dec_w,&dec_h);
	//deg_Printf("dec_w=%d,dec_h=%d\n",dec_w,dec_h);

	ax32xx_sysDcacheFlush((u32)ybuf,w*h*3/2);
	return 0;
}

u8 jpg_buf_decode_buf(u8* jpg_buf,u8 *ybuf,u8 *uvbuf,u16 w,u16 h)
{
	Image_ARG_T arg;
	u16 dec_w,dec_h;
	arg.target.width=w;
	arg.target.height=h;
	arg.yout  = ybuf;
	arg.uvout = uvbuf;
	arg.media.type = MEDIA_SRC_RAM;
	arg.media.src.buff = (u32)jpg_buf;
    arg.wait = 1;  // wait decode end
	if(imageDecodeStart(&arg)<0)
	{
		deg_Printf("image decode fail.\n");
		return 1;
	}

	imageDecodeGetResolution(&dec_w,&dec_h);
	//deg_Printf("dec_w=%d,dec_h=%d\n",dec_w,dec_h);

	ax32xx_sysDcacheFlush((u32)ybuf,w*h*3/2);
	return 0;
}


//==========
#define BYTE_TUNE(r) (r<0?0:(r>255?255:r)) 

//==rgb2yuv table==
/*
static  u8 Y_R[256]; 
static  u8 Y_G[256]; 
static  u8 Y_B[256]; 

static  u8 U_R[256]; 
static  u8 U_G[256]; 
static  u8 U_B[256]; 

static  u8 V_R[256]; 
static  u8 V_G[256]; 
static  u8 V_B[256]; 
*/
static  u8 *yuv_rgb_table_buf=NULL;
static	u8 *Y_R; 
static	u8 *Y_G; 
static	u8 *Y_B; 

static	u8 *U_R; 
static	u8 *U_G; 
static	u8 *U_B; 

static	u8 *V_R; 
static	u8 *V_G; 
static	u8 *V_B; 


void yuv_rgb_table_init()
{ 
	u32 i; 
	if(NULL==yuv_rgb_table_buf)
	{
		yuv_rgb_table_buf=hal_sysMemMalloc(256*9,32);
		if(NULL==yuv_rgb_table_buf)
		{
			deg_Printf("mem err\n");
			return;
		}

		Y_R=yuv_rgb_table_buf;
		Y_G=yuv_rgb_table_buf+256;
		Y_B=yuv_rgb_table_buf+256*2;

		U_R=yuv_rgb_table_buf+256*3;
		U_G=yuv_rgb_table_buf+256*4;
		U_B=yuv_rgb_table_buf+256*5;

		V_R=yuv_rgb_table_buf+256*6;
		V_G=yuv_rgb_table_buf+256*7;
		V_B=yuv_rgb_table_buf+256*8;

	}

	for(i = 0;i<256;i++) 
	{ 
	   //==rgb2yuv table==
	   Y_R[i]=(i*1224)>>12;
	   Y_G[i]=(i*2404)>>12;
	   Y_B[i]=(i*467)>>12;

	   U_R[i]=(i*602)>>12;
	   U_G[i]=(i*1183)>>12;
	   U_B[i]=(i*1785)>>12;

	   V_R[i]=(i*2519)>>12;
	   V_G[i]=(i*2109)>>12;
	   V_B[i]=(i*409)>>12;
	} 
} 

void yuv_rgb_table_uninit()
{ 
	if(yuv_rgb_table_buf)
	{
		hal_sysMemFree(yuv_rgb_table_buf);
		yuv_rgb_table_buf=NULL;
	}
}

static void rgb24_to_yuv420(u8* rgb_buf,u8 *ybuf,u8 *uvbuf,u16 w,u16 h)
{
	u8 r,g,b,r1,g1,b1; 
	s16 y,u,v,y1,u1,v1;
	u8 *rgb_line_buf,*temp_buf;
	u32 i,j;
	u32 line_align;
	line_align=(w*3+3)&~0x3;	// 4bytes align
	if(NULL==yuv_rgb_table_buf)
	{
		return ;
	}

	rgb_line_buf=rgb_buf+(h-1)*line_align; // last line
	for(j=0;j<h;j+=2)
	{
		temp_buf=rgb_line_buf;
		for(i=0;i<w;i+=2)
		{ 
			 r = *temp_buf++; 
			 g = *temp_buf++; 
			 b = *temp_buf++; 
			 r1 = *temp_buf++; 
			 g1 = *temp_buf++; 
			 b1 = *temp_buf++; 

			 y = Y_R[r]+Y_G[g]+Y_B[b];		
			 //u = (U_B[b]-U_R[r]-U_G[g])+128; 
			 //v = (V_R[r]-V_G[g]-V_B[b])+128; 
			 u = (U_B[b]-U_R[r]-U_G[g])+128; 

			 y1 = Y_R[r1]+Y_G[g1]+Y_B[b1]; 
			 //u1 = (U_B[b1]-U_R[r1]-U_G[g1])+128;
			 //v1 = (V_R[r1]-V_G[g1]-V_B[b1])+128;
			 v1 = (V_R[r1]-V_G[g1]-V_B[b1])+128;

			 *ybuf++ = BYTE_TUNE(y);
			 *uvbuf++ = BYTE_TUNE(u);
			 *ybuf++ = BYTE_TUNE(y1);
			 *uvbuf++ = BYTE_TUNE(v1);
		}

		rgb_line_buf=rgb_line_buf-line_align;
		temp_buf=rgb_line_buf;
		for(i=0;i<w;i+=2)
		{ 
			 r = *temp_buf++; 
			 g = *temp_buf++; 
			 b = *temp_buf++; 
			 r1 = *temp_buf++; 
			 g1 = *temp_buf++; 
			 b1 = *temp_buf++; 
		
			 y = Y_R[r]+Y_G[g]+Y_B[b];		
			 //v = (V_R[r]-V_G[g]-V_B[b])+128; 
			 y1 = Y_R[r1]+Y_G[g1]+Y_B[b1]; 
			 //u1 = (U_B[b1]-U_R[r1]-U_G[g1])+128; 
			 *ybuf++ = BYTE_TUNE(y);
			 *ybuf++ = BYTE_TUNE(y1);
		}
		rgb_line_buf=rgb_line_buf-line_align;

	}
}

u8 bmp24_to_yuv420_buf(u32 id,u8 *rgb_buf,u8 *ybuf,u8 *uvbuf,u16 w,u16 h)
{
	u32	addr,size;
	u32 line_align;
	line_align=(w*3+3)&~0x3;	// 4bytes align

	addr= nv_open(id);
	if(addr<0)
	{
		deg_Printf("read id err\n");
		return 1;
	}
	size = nv_size(id);
	if(size<54)
	{
		deg_Printf("size err\n");
		return 1;
	}
	size=size-54;	// offset bmp header
	
	if(size>line_align*h)	// not over rgb_buf size
	{
		size=line_align*h;
	}
	else if(size<w*h*3)
	{
		deg_Printf("bmp format err?\n");
	}
	
	nv_read(addr+54,rgb_buf,size);	// offset bmp header
	//deg_Printf("data size=%d,w*h*3=%d\n",size-54,w*h*3);
	rgb24_to_yuv420(rgb_buf,ybuf,uvbuf,w,h);
	ax32xx_sysDcacheFlush((u32)ybuf,w*h*3/2);
	return 0;
}


//==tips: draw_x,drawy,src_w,src_h is 2X size,X is pixel num==
void yuv420_draw_buf(u8 *dst_ybuf,u16 dst_w,u16 dst_h,s16 draw_x,s16 draw_y,u16 draw_w,u16 draw_h,u8 *src_ybuf,u16 src_w,u16 src_h,u8 alpha_y,u8 alpha_uv)
{
	u16 i,j;
	u16 draw_offset_x,draw_offset_y;
	u8 *dy,*duv,*sy,*suv;

	draw_x&=~0x1;		// align
	draw_y&=~0x1;		// align
	draw_w&=~0x1;		// align
	draw_h&=~0x1;		// align
	dst_w=(dst_w + 0x1f) & (~0x1f);  // add 32bit alignment

	if((draw_x+draw_w<=0)||(draw_y+draw_h<=0)||(draw_x>=dst_w)||(draw_y>=dst_h))	// draw nothing
	{
		return;
	}

	if(draw_x<0)
	{
		draw_offset_x=-draw_x;
		draw_x=0;
		//deg_Printf("draw_offset_x=%d\n",draw_offset_x);
	}
	else
	{
		draw_offset_x=0;
	}
	
	if(draw_y<0)
	{
		draw_offset_y=-draw_y;
		draw_y=0;
		//deg_Printf("draw_offset_y=%d\n",draw_offset_y);
		// not finish!!
	}
	else
	{
		draw_offset_y=0;
	}

	if(draw_x+src_w>dst_w)
	{
		draw_w=dst_w-draw_x;
	}
	else
	{
		draw_w=src_w-draw_offset_x;
	}

	if(draw_y+src_h>dst_h)
	{
		draw_h=dst_h-draw_y;
	}
	else
	{
		draw_h=src_h-draw_offset_y;
	}
	
	//deg_Printf("draw_x=%d,draw_y=%d,draw_w=%d,draw_h=%d\n",draw_x,draw_y,draw_w,draw_h);
	dy=dst_ybuf+dst_w*draw_y+draw_x;
	duv=dst_ybuf+dst_w*dst_h+dst_w*draw_y/2+draw_x;
	sy=src_ybuf;
	suv=src_ybuf+src_w*src_h;

	//==draw ==
	for(j=0;j<draw_h;j+=2)
	{
		for(i=0;i<draw_w;i++)
		{
			if((*(sy+i+draw_offset_x)!=alpha_y)||(*(suv+i+draw_offset_x)!=alpha_uv))
			{
				*(dy+i)=*(sy+i+draw_offset_x);
				*(duv+i)=*(suv+i+draw_offset_x);
			}
		}

		dy+=dst_w;
		sy+=src_w;

		for(i=0;i<draw_w;i++)
		{
			if((*(sy+i+draw_offset_x)!=alpha_y)||(*(suv+i+draw_offset_x)!=alpha_uv))
			{
				*(dy+i)=*(sy+i+draw_offset_x);
				*(duv+i)=*(suv+i+draw_offset_x);
			}
		}
		
		dy+=dst_w;
		sy+=src_w;
		duv+=dst_w;
		suv+=src_w;

	}
}


//==cam effect ==
static disp_frame_t * software_handle_frame;

void yuv420_software_set_handle_frame(disp_frame_t * frame)
{
	software_handle_frame=frame;
}

disp_frame_t * yuv420_software_get_handle_frame()
{
	return  software_handle_frame;
}

void yuv420_effect_handle(u8* ybuf,u8*uvbuf,u16 buf_w,u16 buf_h)
{
	u16 i,j;

	buf_w=(buf_w + 0x1f) & (~0x1f);  // add 32bit alignment


	if(CAM_EFFECT_0==SysCtrl.photo_effect_idx)
	{
		u8 *y,*uv;
		u16 half_w=buf_w>>1;
		y=ybuf;
		uv=uvbuf;
		for(j=0;j<buf_h;j+=2)
		{
			for(i=0;i<half_w;i++)
			{
				*(y+half_w-i-1)=*(y+half_w+i);
			}
			y+=buf_w;
			for(i=0;i<half_w;i++)
			{
				*(y+half_w-i-1)=*(y+half_w+i);
			}

			for(i=0;i<half_w;i+=2)
			{
				*(uv+half_w-i-2)=*(uv+half_w+i);
				*(uv+half_w-i-1)=*(uv+half_w+i+1);
			}

			y+=buf_w;
			uv+=buf_w;
		}
	}
	else if(CAM_EFFECT_1==SysCtrl.photo_effect_idx)
	{
		warpHandle(ybuf, uvbuf, buf_w, buf_h, NULL);
	}
	else if(CAM_EFFECT_2==SysCtrl.photo_effect_idx)
	{
		trilateral_16_window(ybuf, (u16 *)uvbuf, buf_w, buf_h);
	}
	else if(CAM_EFFECT_3==SysCtrl.photo_effect_idx)
	{
		stream_10_window(ybuf, (u16 *)uvbuf, buf_w, buf_h);
	}
	else if(CAM_EFFECT_4==SysCtrl.photo_effect_idx)
	{
		rismatic_multiwindow(ybuf, (u16 *)uvbuf, buf_w, buf_h);
	}
	else if(CAM_EFFECT_5==SysCtrl.photo_effect_idx)
	{
		hexagon_multiwindow(ybuf, (u16 *)uvbuf, buf_w, buf_h);
	}
	ax32xx_sysDcacheFlush((u32)ybuf,buf_w*buf_h*3/2);
}



