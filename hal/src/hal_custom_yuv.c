
/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         CSI HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_csi.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN CSI HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/hal.h"
#include "../../ax32_platform_demo/application.h"


YUV_ADD_T   yuv_add_s;
enum
{
	FRAME_NULL = 0,
	FRAME_ADD
};
extern int nv_open(int res_num);
extern int nv_size(int res_num);
extern int nv_read(int addr,void *buffer,int size);

#if 0
/*******************************************************************************
* Function Name  : hal_custom_yuv_init
* Description    : frame add yuv buf malloc
* Input          : None
* Output         : None
* Return         : 0:Sucess; -1: GET RESOLUTION FAIL -2: MALLOC BUF FAIL
*******************************************************************************/
INT32S hal_custom_frame_init(void)  //init buf
{
	INT32S ret = 0;
	//--------init yuv_add_s ---------
	memset((void*)&yuv_add_s,0, sizeof(YUV_ADD_T));
	if(hal_csiResolutionGet(&yuv_add_s.sensor_width,&yuv_add_s.sensor_height)<0)
	{
		deg_Printf("----get csi resolution fail-----\n");
		ret = -1;
		goto INIT_FAIL;
	}
	else
	{
		deg_Printf("----sensor width:%d, height:%d-----<<\n",yuv_add_s.sensor_width,yuv_add_s.sensor_height);
		yuv_add_s.sensor_frame_size = yuv_add_s.sensor_width*yuv_add_s.sensor_height;
		yuv_add_s.sensor_yuv_addr = (u32)hal_sysMemMalloc(yuv_add_s.sensor_frame_size*3/2,64);
		if(yuv_add_s.sensor_yuv_addr == 0)
		{
			deg_Printf("----frame add sensor buf malloc fail-----\n");
			ret = -2;
			goto INIT_FAIL;
		}
		else
		{
			deg_Printf("----frame add sensor buf adr:%x, size:%d\n",yuv_add_s.sensor_yuv_addr,yuv_add_s.sensor_frame_size*3/2);
		}
	}
	
	//if(hal_lcdGetOSDShowingResolution(&yuv_add_s.lcd_width,&yuv_add_s.lcd_height)<0)
	if(hal_lcdGetBufferResolution(&yuv_add_s.lcd_width,&yuv_add_s.lcd_height)<0)
	{
		deg_Printf("----get lcd resolution fail-----\n");
		ret = -1;
		goto INIT_FAIL;
	}
	else
	{
		deg_Printf("----lcd width:%d, height:%d-----\n",yuv_add_s.lcd_width,yuv_add_s.lcd_height);
		yuv_add_s.lcd_frame_size = yuv_add_s.lcd_width*yuv_add_s.lcd_height;
		yuv_add_s.lcd_yuv_addr = (u32)hal_sysMemMalloc(yuv_add_s.lcd_frame_size*3/2,64);
		if(yuv_add_s.lcd_yuv_addr == 0)
		{
			deg_Printf("----frame add  lcd buf malloc fail-----\n");
			ret = -2;
			goto INIT_FAIL;
		}
		else
		{
			deg_Printf("----frame add lcd buf adr:%x, size:%d\n",yuv_add_s.lcd_yuv_addr,yuv_add_s.lcd_frame_size*3/2);
		}
	}

	yuv_add_s.lcd_table_size = yuv_add_s.lcd_height*32;
	yuv_add_s.lcd_table_t = (u32)hal_sysMemMalloc(yuv_add_s.lcd_table_size*8,64);
	if(yuv_add_s.lcd_table_t == 0)
	{
		deg_Printf("frame add lcd_table_t malloc fail\n");
		ret = -2;
		goto INIT_FAIL;
	}

	yuv_add_s.sensor_table_size = yuv_add_s.sensor_height*32;
	yuv_add_s.sensor_table_t = (u32)hal_sysMemMalloc(yuv_add_s.sensor_table_size*8,64);
	if(yuv_add_s.sensor_table_t == 0)
	{
		deg_Printf("frame add sensor_table_t malloc fail\n");
		ret = -2;
		goto INIT_FAIL;
	}

	ret = 0;
	yuv_add_s.frame_stat = FRAME_BUF_MALLOC;
	return ret;

INIT_FAIL:
	
	if(yuv_add_s.lcd_yuv_addr)
		hal_sysMemFree((void *)(yuv_add_s.lcd_yuv_addr));
	if(yuv_add_s.lcd_table_t)
		hal_sysMemFree((void *)(yuv_add_s.lcd_table_t));
	if(yuv_add_s.sensor_yuv_addr)
		hal_sysMemFree((void *)(yuv_add_s.sensor_yuv_addr));
	if(yuv_add_s.sensor_table_t)
		hal_sysMemFree((void *)(yuv_add_s.sensor_table_t));

	yuv_add_s.frame_stat = FRAME_PREPARE_NULL;

	return ret;
			
}
/*******************************************************************************
* Function Name  : hal_custom_frame_create
* Description    : hal_custom_frame_create
* Input          : frame id :read from resource
* Output         : None
* Return         : 0:Sucess; -1: Fail
*******************************************************************************/

INT32S hal_custom_frame_create(INT32U idx)
{
	INT8U *jpegBuffer;
	INT32S len,addr;
	u16 src_width , src_height, decode_lcd_width, decode_lcd_height, decode_sensor_width, decode_sensor_height;
	u32 decode_lcd_size, decode_sensor_size;
	INT32S ret = 0;

	jpegBuffer = NULL;
	//--------------frame buf check
	if(yuv_add_s.frame_stat == FRAME_PREPARE_NULL)
	{
		deg_Printf("frame buffer have not been malloced\n");
		return -1;
	}
	//--------init yuv_add_s ---------
	HAL_CRITICAL_INIT();
	HAL_CRITICAL_ENTER();
	
	addr = nv_open((int)idx);
	if(addr<0)
	{
		deg_Printf("open frame from resourse fail\n");
		ret = -2;
		goto CREATE_FAIL;
	}
	jpegBuffer = (INT8U *)hal_sysMemMalloc(nv_size(idx),64);
	if(jpegBuffer == NULL)
	{
		deg_Printf("hal_custom_frame_create malloc fail\n");
		ret = -3;
		goto CREATE_FAIL;
	}
	else
	{
		len = nv_size((int)idx);
		nv_read(addr,jpegBuffer,len);					
	}

	hal_mjpegHeaderParse(jpegBuffer);
	hal_mjpegDecodeGetResolution(&src_width,&src_height);
	//deg_Printf("src_width:%d,src_height:%d\n",src_width,src_height);
	
	yuv_add_s.frame_stat = FRAME_BUF_MALLOC;

	if(hal_mjpegDecodeNoIsr(jpegBuffer,(u8*)(yuv_add_s.sensor_yuv_addr),(u8*)(yuv_add_s.sensor_yuv_addr+yuv_add_s.sensor_width*yuv_add_s.sensor_height),yuv_add_s.sensor_width,yuv_add_s.sensor_height)<0)
	{
		ret = -4;
		goto CREATE_FAIL;
	}
		
	if(!jpeg1_decode_check())
	{
		ret = -5;
		goto CREATE_FAIL;
	}
	
	if(hal_mjpegDecodeNoIsr(jpegBuffer,(u8*)(yuv_add_s.lcd_yuv_addr),(u8*)(yuv_add_s.lcd_yuv_addr+yuv_add_s.lcd_width*yuv_add_s.lcd_height),yuv_add_s.lcd_width,yuv_add_s.lcd_height)<0)
	{
		ret = -6;
		goto CREATE_FAIL;
	}
		
	if(!jpeg1_decode_check())
	{
		ret = -7;
		goto CREATE_FAIL;
	}

	if(hal_custom_frame_scan()<0)
	{
		ret = -8;
		deg_Printf("---------frame scan fail-------------\n");
		goto CREATE_FAIL;
	}

	if(jpegBuffer)
		hal_sysMemFree((void *)jpegBuffer);

	yuv_add_s.frame_stat = FRAME_ADD;
	HAL_CRITICAL_EXIT();
	
	return ret;
	
CREATE_FAIL:
	if(jpegBuffer)
		hal_sysMemFree((void *)jpegBuffer);


	HAL_CRITICAL_EXIT();
	return ret;
}
/*******************************************************************************
* Function Name  : hal_custom_frame_scan
* Description    : hal_custom_frame_scan
* Input          : None
* Output         : None
* Return         : 0:Sucess; -1: Fail
*******************************************************************************/
s32 hal_custom_frame_scan(void)
{
	if(yuv_add_s.frame_stat != FRAME_BUF_MALLOC)
		return -1;
//---------------LCD FRAME SCAN-------------//
	u8* lcd_yuv;
	u32 i = 0,j = 0,k = 0, i_temp = 0;
	YUV_TABLE_T* lcd_yuv_table;
	lcd_yuv_table = (YUV_TABLE_T*)(yuv_add_s.lcd_table_t);
	lcd_yuv = (u8*)(yuv_add_s.lcd_yuv_addr);
	//deg_Printf("lcdyuv:%x ,lcd_yuv_table:%x\n",(u32)lcd_yuv,(u32)lcd_yuv_table);
	for(i=0;i<yuv_add_s.lcd_height;i +=2)
	{
		for(j = 0;j<yuv_add_s.lcd_width; j+=2 )
		{
			i_temp = 0;
			k = i*yuv_add_s.lcd_width;
			if(lcd_yuv[k+j] < Y_THD)
			{
				lcd_yuv[k+j] = 0;
				i_temp++;
			}
			if(lcd_yuv[k+j+1] < Y_THD)
			{
				lcd_yuv[k+j+1] = 0;
				i_temp++;
			}
			if(lcd_yuv[k+j+yuv_add_s.lcd_width] < Y_THD)
			{
				lcd_yuv[k+j+yuv_add_s.lcd_width] = 0;
				i_temp++;
			}
			if(lcd_yuv[k+j+yuv_add_s.lcd_width+1] < Y_THD)
			{
				lcd_yuv[k+j+yuv_add_s.lcd_width+1] = 0;
				i_temp++;
			}
			if(i_temp > 3)
			{
				k = yuv_add_s.lcd_frame_size+yuv_add_s.lcd_width *(i>>1)+j;
				lcd_yuv[k] = 0;
				lcd_yuv[k+1] = 0;
			}
		}
	}
	j = 0;
	for(i=0;i<yuv_add_s.lcd_frame_size;i++)
	{
		if(i==0)
		{
			if(lcd_yuv[i] != 0){
				lcd_yuv_table[j].start = i;
			
			}
		}
		else if(lcd_yuv[i]!=0)
		{
			if(lcd_yuv[i-1] ==0){
				lcd_yuv_table[j].start = i;
			}
			if(i == yuv_add_s.lcd_frame_size-1)
			{
				lcd_yuv_table[j++].end = i;
			}
			
		}
		else{ //no need to add area
				if(lcd_yuv[i-1] != 0){
				lcd_yuv_table[j++].end = i-1;
			}
		}
	}
	for(i=yuv_add_s.lcd_frame_size;i<yuv_add_s.lcd_frame_size*3/2;i++)
	{
		if(i==yuv_add_s.lcd_frame_size)
		{
			if(lcd_yuv[i] != 0){
				lcd_yuv_table[j].start = i;
			
			}
		}
		else if(lcd_yuv[i]!=0)
		{
			if(lcd_yuv[i-1] ==0){
				lcd_yuv_table[j].start = i;
			}
			if(i == yuv_add_s.lcd_frame_size*3/2-1)
			{
				lcd_yuv_table[j++].end = i;
			}
			
		}
		else{//no need to add area
				if(lcd_yuv[i-1] != 0){
				lcd_yuv_table[j++].end = i-1;
			}
		}
	}
	if(j > yuv_add_s.lcd_table_size)
	{
		deg_Printf("j=%d,yuv_add_s.lcd_table_size = %d\n ",j,yuv_add_s.lcd_table_size);
		deg_Printf("malloc lcd_table no enough, the yuv frame may be too complex\n");
		return -1;
	}
	yuv_add_s.lcd_table_index = j;  //save lcd yuv table index

//---------------SENSOR FRAME SCAN-------------//
	u8* sensor_yuv;
	sensor_yuv = (u8*)(yuv_add_s.sensor_yuv_addr);
	YUV_TABLE_T* mjpeg_yuv_table;
	mjpeg_yuv_table = (YUV_TABLE_T*)(yuv_add_s.sensor_table_t);
	//debgbuf(sensor_yuv,yuv_add_s.sensor_frame_size);
	for(i=0;i<yuv_add_s.sensor_height;i +=2)
	{
	
		for(j = 0;j<yuv_add_s.sensor_width; j+=2 )
		{
			i_temp = 0;
			k = i*yuv_add_s.sensor_width;
			if(sensor_yuv[k+j] < Y_THD)
			{
				sensor_yuv[k+j] = 0;
				i_temp++;
				
			}
			if(sensor_yuv[k+j+1] < Y_THD)
			{
				sensor_yuv[k+j+1] = 0;
				i_temp++;
				
			}
			if(sensor_yuv[k+j+yuv_add_s.sensor_width] < Y_THD)
			{
				sensor_yuv[k+j+yuv_add_s.sensor_width] = 0;
				i_temp++;
				
			}
			if(sensor_yuv[k+j+yuv_add_s.sensor_width+1] < Y_THD)
			{
				sensor_yuv[k+j+yuv_add_s.sensor_width+1] = 0;
				i_temp++;
				
			}
			if(i_temp > 3)
			{
				k = yuv_add_s.sensor_frame_size+yuv_add_s.sensor_width *(i>>1)+j;
				sensor_yuv[k] = 0;
				sensor_yuv[k+1] = 0;
			}
			
		}
	}
	j = 0;
	for(i=0;i<yuv_add_s.sensor_frame_size;i++)
	{
		if(i==0)
		{
			if(sensor_yuv[i] != 0){
				mjpeg_yuv_table[j].start = i;
			
			}
		}
		else if(sensor_yuv[i]!=0)
		{
			if(sensor_yuv[i-1] ==0){
				mjpeg_yuv_table[j].start = i;
			}
			if(i == yuv_add_s.sensor_frame_size-1)
			{
				mjpeg_yuv_table[j++].end = i;
			}
			
		}
		else{//no need to add area
				if(sensor_yuv[i-1] != 0){
				mjpeg_yuv_table[j++].end = i-1;
			}
		}
	}
	for(i=yuv_add_s.sensor_frame_size;i<yuv_add_s.sensor_frame_size*3/2;i++)
	{
		if(i==yuv_add_s.sensor_frame_size)
		{
			if(sensor_yuv[i] != 0){
				mjpeg_yuv_table[j].start = i;
			
			}
		}
		else if(sensor_yuv[i]!=0)
		{
			if(sensor_yuv[i-1] ==0){
				mjpeg_yuv_table[j].start = i;
			}
			if(i == yuv_add_s.sensor_frame_size*3/2-1)
			{
				mjpeg_yuv_table[j++].end = i;
			}
			
		}
		else{//no need to add area
				if(sensor_yuv[i-1] != 0){
				mjpeg_yuv_table[j++].end = i-1;
			}
		}
	}
	if(j>yuv_add_s.sensor_table_size)
	{
		deg_Printf("malloc sensor_table no enough, the yuv frame may be too complex\n");
		return -1;
	}
	yuv_add_s.sensor_table_index = j;  //save lcd yuv table index

	//write back all cache line
	SPR_DCAWR = 0xffffffff;
	//yuv_add_s.sensor_yuv_add_ready = 1;
	return 0;
}
/*******************************************************************************
* Function Name  : hal_custom_frame_unit
* Description    : hal_custom_frame_unit, free malloc frame buf
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_unit(void)
{
	HAL_CRITICAL_INIT();
	HAL_CRITICAL_ENTER();
	deg_Printf("---------hal_custom_yuv_unit---------------\n");
	deg_Printf(" %x, %x, %x,%x\n",yuv_add_s.lcd_yuv_addr,yuv_add_s.lcd_table_t,yuv_add_s.sensor_yuv_addr,yuv_add_s.sensor_table_t);
	if(yuv_add_s.lcd_yuv_addr)
		hal_sysMemFree((void *)(yuv_add_s.lcd_yuv_addr));
	if(yuv_add_s.lcd_table_t)
		hal_sysMemFree((void *)(yuv_add_s.lcd_table_t));
	if(yuv_add_s.sensor_yuv_addr)
		hal_sysMemFree((void *)(yuv_add_s.sensor_yuv_addr));
	if(yuv_add_s.sensor_table_t)
		hal_sysMemFree((void *)(yuv_add_s.sensor_table_t));
	memset((void*)&yuv_add_s,0, sizeof(YUV_ADD_T));
	HAL_CRITICAL_EXIT();
}
/*******************************************************************************
* Function Name  : hal_custom_frame_add_enable
* Description    : hal_custom_frame_add_enable
* Input          : en: 1 enable frame add, 0 disable frame add
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_enable(u32 en)
{
	//HAL_CRITICAL_INIT();
	//HAL_CRITICAL_ENTER();
	//deg_Printf("yuv_add_s.frame_stat:%d,en:%d\n",yuv_add_s.frame_stat,en);
	if(en)
	{	
		
		if(yuv_add_s.frame_stat == FRAME_PREPARE_OK)
			yuv_add_s.frame_stat = FRAME_ADD;
	}
	else
	{
		if(yuv_add_s.frame_stat == FRAME_ADD)
			yuv_add_s.frame_stat = FRAME_PREPARE_OK;
	}
	//HAL_CRITICAL_EXIT();
}
/*******************************************************************************
* Function Name  : hal_custom_frame_add_lcd
* Description    : add frame to lcd display
* Input          : dst_y_addr: lcd yuv addr
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_lcd(u32 dst_y_addr)
{
	
	if(yuv_add_s.frame_stat != FRAME_ADD)
	{
		return;
	}
	int i = 0;
	u8  *dst_y, *src_y;
	YUV_TABLE_T* lcd_yuv_table;
	dst_y = (u8*) dst_y_addr;
	src_y = (u8*) (yuv_add_s.lcd_yuv_addr);
	lcd_yuv_table = (YUV_TABLE_T*)(yuv_add_s.lcd_table_t);
	
	for(i = 0; i<yuv_add_s.lcd_table_index;i++)
	{
		//deg_Printf("i:%d,start:%d,end:%d\n",i,lcd_yuv_table[i].start,lcd_yuv_table[i].end);
		ax32xx_mcpy1_sdram2gram_2((void*)&dst_y[lcd_yuv_table[i].start],(void*)&src_y[lcd_yuv_table[i].start],lcd_yuv_table[i].end-lcd_yuv_table[i].start+1);
	}
}
/*******************************************************************************
* Function Name  : hal_custom_frame_add_mjpeg
* Description    : add frame to mjpeg display
* Input          : dst_y_addr: mjpeg yuv addr
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_mjpeg(u32 dst_y_addr)
{
	if(yuv_add_s.frame_stat != FRAME_ADD)
	{
		return ;
	}
	int i = 0;
	u8  *dst_y, *src_y;
	YUV_TABLE_T* mjpeg_yuv_table;
	dst_y = (u8*) dst_y_addr;
	src_y = (u8*) (yuv_add_s.sensor_yuv_addr);
	mjpeg_yuv_table = (YUV_TABLE_T*)(yuv_add_s.sensor_table_t);
	for(i = 0; i<yuv_add_s.sensor_table_index;i++)
	{
		//deg_Printf("i:%d,start:%d,end:%d\n",i,mjpeg_yuv_table[i].start,mjpeg_yuv_table[i].end);
		ax32xx_mcpy1_sdram2gram_2((void*)&dst_y[mjpeg_yuv_table[i].start],(void*)&src_y[mjpeg_yuv_table[i].start],mjpeg_yuv_table[i].end-mjpeg_yuv_table[i].start+1);
	}
}

void hal_custom_frame_reset_sensor_szie()
{
	hal_csiResolutionGet(&yuv_add_s.sensor_width,&yuv_add_s.sensor_height);
	yuv_add_s.sensor_frame_size = yuv_add_s.sensor_width*yuv_add_s.sensor_height;
	yuv_add_s.sensor_table_size = yuv_add_s.sensor_height*32;	
	yuv_add_s.sensor_table_index = 0;
}

#else

INT32S hal_custom_frame_init(void)  //init buf
{
	INT32S ret = 0;
	//--------init yuv_add_s ---------
	memset((void*)&yuv_add_s,0, sizeof(YUV_ADD_T));

/*
	if(hal_csiResolutionGet(&yuv_add_s.sensor_width,&yuv_add_s.sensor_height)<0)
	{
		deg_Printf("----get csi resolution fail-----\n");
		ret = -1;
		goto INIT_FAIL;
	}
	else
	{
		deg_Printf("----sensor width:%d, height:%d-----<<\n",yuv_add_s.sensor_width,yuv_add_s.sensor_height);
		yuv_add_s.sensor_frame_size = yuv_add_s.sensor_width*yuv_add_s.sensor_height;
		yuv_add_s.sensor_yuv_addr = (u32)hal_sysMemMalloc(yuv_add_s.sensor_frame_size*3/2,64);
		if(yuv_add_s.sensor_yuv_addr == 0)
		{
			deg_Printf("----frame add sensor buf malloc fail-----\n");
			ret = -2;
			goto INIT_FAIL;
		}
		else
		{
			deg_Printf("----frame add sensor buf adr:%x, size:%d\n",yuv_add_s.sensor_yuv_addr,yuv_add_s.sensor_frame_size*3/2);
		}
	}
	
	//if(hal_lcdGetOSDShowingResolution(&yuv_add_s.lcd_width,&yuv_add_s.lcd_height)<0)
	if(hal_lcdGetBufferResolution(&yuv_add_s.lcd_width,&yuv_add_s.lcd_height)<0)
	{
		deg_Printf("----get lcd resolution fail-----\n");
		ret = -1;
		goto INIT_FAIL;
	}
	else
	{
		deg_Printf("----lcd width:%d, height:%d-----\n",yuv_add_s.lcd_width,yuv_add_s.lcd_height);
		yuv_add_s.lcd_frame_size = yuv_add_s.lcd_width*yuv_add_s.lcd_height;
		yuv_add_s.lcd_yuv_addr = (u32)hal_sysMemMalloc(yuv_add_s.lcd_frame_size*3/2,64);
		if(yuv_add_s.lcd_yuv_addr == 0)
		{
			deg_Printf("----frame add  lcd buf malloc fail-----\n");
			ret = -2;
			goto INIT_FAIL;
		}
		else
		{
			deg_Printf("----frame add lcd buf adr:%x, size:%d\n",yuv_add_s.lcd_yuv_addr,yuv_add_s.lcd_frame_size*3/2);
		}
	}
*/

	return ret;

}
/*******************************************************************************
* Function Name  : hal_custom_frame_create
* Description    : hal_custom_frame_create
* Input          : frame id :read from resource
* Output         : None
* Return         : 0:Sucess; -1: Fail
*******************************************************************************/

INT32S hal_custom_frame_create(INT32U idx)
{
	INT8U *jpegBuffer;
	INT32S len,addr;
	u16 src_width , src_height;
	u32 decode_lcd_size, decode_sensor_size;
	INT32S ret = 0;

	jpegBuffer = NULL;

	//--------init yuv_add_s ---------
	HAL_CRITICAL_INIT();
	HAL_CRITICAL_ENTER();
	deg_Printf("idx=%d\n",idx);

	//==malloc lcd buf==
	hal_lcdGetBufferResolution(&yuv_add_s.lcd_width,&yuv_add_s.lcd_height);
	deg_Printf("lcd w=%d,h=%d\n",yuv_add_s.lcd_width,yuv_add_s.lcd_height);
	//yuv_add_s.lcd_frame_size = yuv_add_s.lcd_width*yuv_add_s.lcd_height;

	if(( yuv_add_s.lcd_width == 220 ) && (yuv_add_s.lcd_height == 176 ) )
	{
		yuv_add_s.lcd_width =320;
		yuv_add_s.lcd_height=240;
		yuv_add_s.lcd_frame_size = yuv_add_s.lcd_width*yuv_add_s.lcd_height;

		yuv_add_s.lcd_small_width =220;
		yuv_add_s.lcd_small_height=176;
		yuv_add_s.lcd_small_width=(yuv_add_s.lcd_small_width + 0x1f) & (~0x1f);  // add 32bit alignment
		yuv_add_s.lcd_small_frame_size = yuv_add_s.lcd_small_width*yuv_add_s.lcd_small_height;
	}
	else
	{
		yuv_add_s.lcd_small_width =0;
		yuv_add_s.lcd_small_height=0;

		yuv_add_s.lcd_frame_size = yuv_add_s.lcd_width*yuv_add_s.lcd_height;
		yuv_add_s.lcd_small_frame_size = yuv_add_s.lcd_small_width*yuv_add_s.lcd_small_height;
	}

	
	if(NULL==yuv_add_s.lcd_yuv_buf)
	{
		yuv_add_s.lcd_yuv_buf = hal_sysMemMalloc(yuv_add_s.lcd_frame_size*3/2,64);
		if(NULL==yuv_add_s.lcd_yuv_buf)
		{
			deg_Printf("lcd buf malloc fail\n");
			ret = -1;
			goto CREATE_FAIL;
		}
	}

	if( (NULL==yuv_add_s.lcd_small_yuv_buf) && ( yuv_add_s.lcd_small_frame_size != 0 )) 
	{
		yuv_add_s.lcd_small_yuv_buf = hal_sysMemMalloc(yuv_add_s.lcd_small_frame_size*3/2,64);
		if(NULL==yuv_add_s.lcd_small_yuv_buf)
		{
			deg_Printf("lcd buf malloc fail\n");
			ret = -1;
			goto CREATE_FAIL;
		}
	}
	
	//==end malloc lcd buf==
	addr = nv_open((int)idx);
	if(addr<0)
	{
		deg_Printf("open frame from resourse fail\n");
		ret = -2;
		goto CREATE_FAIL;
	}
	jpegBuffer = (INT8U *)hal_sysMemMalloc(nv_size(idx),64);
	if(jpegBuffer == NULL)
	{
		deg_Printf("hal_custom_frame_create malloc fail\n");
		ret = -3;
		goto CREATE_FAIL;
	}
	else
	{
		len = nv_size((int)idx);
		nv_read(addr,jpegBuffer,len);
	}

	hal_lcdSetPIPEnable(0);

	ret=hal_mjpegHeaderParse(jpegBuffer);
	if(ret<0)
	{
		deg_Printf("jpeg header parse fail.%d\n",ret);
		goto CREATE_FAIL;
	}
	hal_mjpegDecodeGetResolution(&src_width,&src_height);
	deg_Printf("src_width:%d,src_height:%d,len=%d\n",src_width,src_height,len);


	if(hal_mjpegDecodeNoIsr(jpegBuffer,(u8*)(yuv_add_s.lcd_yuv_buf),(u8*)(yuv_add_s.lcd_yuv_buf+yuv_add_s.lcd_width*yuv_add_s.lcd_height),yuv_add_s.lcd_width,yuv_add_s.lcd_height)<0)
	{
		ret = -5;
		goto CREATE_FAIL;
	}

	if(!jpeg1_decode_check())
	{
		ret = -6;
		goto CREATE_FAIL;
	}

	if( yuv_add_s.lcd_small_yuv_buf != NULL )
	{
		if(hal_mjpegDecodeNoIsr(jpegBuffer,(u8*)(yuv_add_s.lcd_small_yuv_buf),(u8*)(yuv_add_s.lcd_small_yuv_buf+yuv_add_s.lcd_small_width*yuv_add_s.lcd_small_height),yuv_add_s.lcd_small_width,yuv_add_s.lcd_small_height)<0)
		{
			ret = -5;
			goto CREATE_FAIL;
		}

		if(!jpeg1_decode_check())
		{
			ret = -6;
			goto CREATE_FAIL;
		}
	}

	if(jpegBuffer)
		hal_sysMemFree((void *)jpegBuffer);

	yuv_add_s.frame_stat = FRAME_ADD;
	HAL_CRITICAL_EXIT();
	
	return ret;
	
CREATE_FAIL:

	if(jpegBuffer)
	{
		hal_sysMemFree(jpegBuffer);
	}

	if(yuv_add_s.lcd_yuv_buf)
	{
		hal_sysMemFree(yuv_add_s.lcd_yuv_buf);
		yuv_add_s.lcd_yuv_buf=NULL;
	}

	if(yuv_add_s.lcd_small_yuv_buf)
	{
		hal_sysMemFree(yuv_add_s.lcd_small_yuv_buf);
		yuv_add_s.lcd_small_yuv_buf=NULL;
	}

	HAL_CRITICAL_EXIT();
	return ret;
}

/*******************************************************************************
* Function Name  : hal_custom_frame_unit
* Description    : hal_custom_frame_unit, free malloc frame buf
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_unit(void)
{
	if(yuv_add_s.lcd_yuv_buf)
	{
		hal_sysMemFree(yuv_add_s.lcd_yuv_buf);
		yuv_add_s.lcd_yuv_buf=NULL;
	}

	if(yuv_add_s.lcd_small_yuv_buf)
	{
		hal_sysMemFree(yuv_add_s.lcd_small_yuv_buf);
		yuv_add_s.lcd_small_yuv_buf=NULL;
	}

	if(yuv_add_s.sensor_yuv_buf)
	{
		hal_sysMemFree(yuv_add_s.sensor_yuv_buf);
		yuv_add_s.sensor_yuv_buf=NULL;
	}
}
/*******************************************************************************
* Function Name  : hal_custom_frame_add_enable
* Description    : hal_custom_frame_add_enable
* Input          : en: 1 enable frame add, 0 disable frame add
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_enable(u32 en)
{
	//HAL_CRITICAL_INIT();
	//HAL_CRITICAL_ENTER();
	//deg_Printf("yuv_add_s.frame_stat:%d,en:%d\n",yuv_add_s.frame_stat,en);
	if(en)
	{	
		if(yuv_add_s.frame_stat == FRAME_NULL)
		{
			yuv_add_s.frame_stat = FRAME_ADD;
		}
	}
	else
	{
		if(yuv_add_s.frame_stat == FRAME_ADD)
		{
			yuv_add_s.frame_stat = FRAME_NULL;
		}
	}
	//HAL_CRITICAL_EXIT();
}

 
static void frame_draw_buf(u8 *dst_ybuf,u8 *src_ybuf,u16 src_w,u16 src_h,u8 alpha_y)
{
	u16 i,j;
	u8 *dy,*duv,*sy,*suv;

	src_w=(src_w + 0x1f) & (~0x1f);  // add 32bit alignment

	dy=dst_ybuf;
	duv=dst_ybuf+src_w*src_h;
	sy=src_ybuf;
	suv=src_ybuf+src_w*src_h;

	//==draw ==
	for(j=0;j<src_h;j+=2)
	{
		for(i=0;i<src_w;i++)
		{
			if((*(sy+i)>alpha_y))  // y
			{
				*(dy+i)=*(sy+i);
				*(duv+i)=*(suv+i);
			}
		}

		dy+=src_w;
		sy+=src_w;

		for(i=0;i<src_w;i++)
		{
			if((*(sy+i)>alpha_y))  //uv
			{
				*(dy+i)=*(sy+i);
				*(duv+i)=*(suv+i);
			}
		}
		
		dy+=src_w;
		sy+=src_w;
		duv+=src_w;
		suv+=src_w;
	}
	
}
static void  frame_draw_sensor_buf(u8 *dst_ybuf,u16 *dst_uvbuf,u16 dst_w,u16 dst_h,u8 *src_ybuf,u16 src_w,u16 src_h)
{
	u32 i, j, x, y, offset_pixel, offset_pixel_uv, offset_data, offset_data_uv;
	u16 *src_uvbuf = (u16 *)(src_ybuf + src_w*src_h);
	u32 interval_w = 1000000*src_w/dst_w + 1, interval_h = 1000000*src_h/dst_h + 1, interval_i, interval_j;
	
	for(j = 0, y = 0, interval_j = 0; j < dst_h; j++, interval_j += interval_h){
		if(interval_j >= 1000000){
			interval_j -= 1000000;
			y++;
		}
		offset_data = y * src_w;
		offset_pixel = j * dst_w;
		offset_data_uv = (y >> 1) * (src_w >> 1);
		offset_pixel_uv = (j >> 1) * (dst_w >> 1);
		
		for(i = 0, x = 0, interval_i = 0; i < dst_w; i++, interval_i += interval_w){
			if(interval_i >= 1000000){
				interval_i -= 1000000;
				x++;
			}
			if(*(src_ybuf + offset_data + x) < Y_THD)
				continue;
			*(dst_ybuf + offset_pixel + i) = *(src_ybuf + offset_data + x);
			*(dst_uvbuf + offset_pixel_uv + (i >> 1)) = *(src_uvbuf + offset_data_uv + (x >> 1));
		}
	}
	ax32xx_sysDcacheWback(dst_ybuf,dst_w*dst_h);  
	ax32xx_sysDcacheWback(dst_uvbuf, dst_w*dst_h/2);
}


/*******************************************************************************
* Function Name  : hal_custom_frame_add_lcd
* Description    : add frame to lcd display
* Input          : dst_y_addr: lcd yuv addr
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_lcd(u8 *dst_ybuf)
{
	if(yuv_add_s.frame_stat != FRAME_ADD)
	{
		return;
	}

	if( yuv_add_s.lcd_small_yuv_buf )
	{
		frame_draw_buf(dst_ybuf,yuv_add_s.lcd_small_yuv_buf,yuv_add_s.lcd_small_width,yuv_add_s.lcd_small_height,Y_THD);
		ax32xx_sysDcacheFlush((u32)dst_ybuf,yuv_add_s.lcd_small_frame_size*3/2);
	}
	else
	{
		frame_draw_buf(dst_ybuf,yuv_add_s.lcd_yuv_buf,yuv_add_s.lcd_width,yuv_add_s.lcd_height,Y_THD);
		ax32xx_sysDcacheFlush((u32)dst_ybuf,yuv_add_s.lcd_frame_size*3/2);
	}
	//ax32xx_sysDcacheFlush((u32)dst_ybuf,yuv_add_s.lcd_frame_size*3/2);
}
/*******************************************************************************
* Function Name  : hal_custom_frame_add_mjpeg
* Description    : add frame to mjpeg display
* Input          : dst_y_addr: mjpeg yuv addr
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_mjpeg(u8 *dst_ybuf)
{
	if(yuv_add_s.frame_stat != FRAME_ADD)
	{
		return ;
	}
	u16 csi_w,csi_h,crop_w,crop_h;
	
	hal_csiResolutionGet(&csi_w,&csi_h);
	crop_w =  ((csi_w * (100 - SysCtrl.crop_level*5/3) / 100) + 0x1f) & (~0x1f);
	crop_h = (csi_h * (100 - SysCtrl.crop_level*5/3) / 100) & ~1;
	//deg_Printf("-----------0x%x,0x%x--------------.\n",dst_ybuf[0],yuv_add_s.lcd_yuv_buf[0]);
	frame_draw_sensor_buf(dst_ybuf,(u16 *)(dst_ybuf+csi_w*csi_h),crop_w,crop_h,yuv_add_s.lcd_yuv_buf,yuv_add_s.lcd_width,yuv_add_s.lcd_height);
	
}

#endif

