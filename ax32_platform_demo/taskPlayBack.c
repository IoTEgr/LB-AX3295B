#include "application.h"
#include "userInterface.h"

extern msgDealInfor playBackMsgDeal[];
extern sysTask taskPlayBack;

#define  TASK_VIDEOPLAY_AUTOPLAY         1      //0,playback end,display last frame of current file
                                                         	  		    // 1,playback end,display first frame of current file
                                             				   // 2,playback end,display first frame of next file
#if TASK_VIDEOPLAY_AUTOPLAY >0
    #define  TASK_VIDEOPLAY_FIRSTFRAME      1
#else
    #define  TASK_VIDEOPLAY_FIRSTFRAME      1
#endif


typedef struct Video_Thumbnail_S
{
	char file_name[MANAGER_NAME_LEN+1];
	INT8U index;
	INT8U stat;
	INT16U file_index;
	INT8U *y_buffer;
	INT8U *uv_buffer;
}Video_Thumbnail_T;

static Video_Thumbnail_T thnumbnailCtrl[THUMBNAIL_CFG_NUM];

bool first_frame_not_display = false;

void video_playback_jpg_scale(void *src_buf,u16 src_w,u16 src_h,u16 ch_src_xs,u16 ch_src_ys,void *dst_buf,u16 dst_w,u16 dst_h)
{
	u8 *src_ybuf = (u8 *)src_buf;
	u8 *src_uvbuf = (u8 *)src_buf+src_w*src_h;
	u8 *dst_ybuf = (u8 *)dst_buf;
	u8 *dst_uvbuf = (u8 *)dst_buf+dst_w*dst_h;
    u16 i,j;
	deg_Printf("src_w=%d,src_h=%d,dst_w=%d,dst_h=%d,ch_src_xs=%d,ch_src_ys=%d\n",src_w,src_h,dst_w,dst_h,ch_src_xs,ch_src_ys);

	//===y===
	src_ybuf += src_w*ch_src_ys;
	src_ybuf += ch_src_xs;
	for(j=0;j<src_h-ch_src_ys;j++)
	{	

		for(i=0;i<src_w;i++)
		{
			if(i < dst_w)
			{
				*dst_ybuf++ = *src_ybuf++;
			}
			else
			{
				*src_ybuf++;
			}
		}

		if(j < dst_h)
		{
			dst_ybuf = (u8 *)dst_buf+(j+1)*dst_w;
		}
		else
		{
			break;
		}
	}
	//===uv===
	src_uvbuf += (src_w/2)*(ch_src_ys/2)*2;
	src_uvbuf += (ch_src_xs/2)*2;
	for(j=0;j<src_h/2-ch_src_ys/2;j++)
	{
		for(i=0;i<src_w/2;i++)
		{
			if(i< dst_w/2)
			{
				*dst_uvbuf++ = *src_uvbuf++;// U
				*dst_uvbuf++ = *src_uvbuf++;// V
			}
			else
			{
				src_uvbuf++;
				src_uvbuf++;
			}
		}

		if(j< dst_h/2)
		{
			dst_uvbuf = (u8 *)dst_buf+dst_w*dst_h+(j+1)*dst_w;
		}
		else
		{
			break;
		}
	}

	ax32xx_sysDcacheFlush((u32)dst_buf,dst_w*dst_h+dst_w*dst_h/2);
}



int video_playback_start(int index)
{
	char *name;
	int type;
	if(SDC_STAT_NULL==SysCtrl.sdcard) 
	{
	display_udisk:
		if(spi_udisk_jpg_num()>0)
		{
			Image_ARG_T arg;
			disp_frame_t *p_lcd_buffer;
			u16 x,y;
			u16 lcd_w,lcd_h;
			u8 *temp_ybuf,*temp_uv_buf;
			SysCtrl.file_type=FILE_TYPE_JPG;
			
			p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
			hal_lcdGetVideoPos(&x,&y);
			dispLayerGetResolution(DISP_LAYER_VIDEO,&lcd_w,&lcd_h);

			arg.target.width = lcd_w;
			arg.target.height = lcd_h;

			if(p_lcd_buffer)
			{
				deg_Printf("0:x:%d,y:%d,w:%d,h:%d\n",x,y,arg.target.width,arg.target.height);
				hal_dispframeVideoCfg(p_lcd_buffer,x,y,arg.target.width,arg.target.height);
			}
			else
			{
				deg_Printf("0get null buf!!\n");
			}
			arg.yout  = p_lcd_buffer->y_addr;
			arg.uvout = p_lcd_buffer->uv_addr;

			deg_Printf("index jpg=%d\n",index);

			u8 *jpg;
			u32 jpg_size =spi_udisk_read_jpg_size(index);
			jpg=hal_sysMemMalloc(jpg_size,32);
			if(NULL==jpg)
			{
				deg_Printf("jpg malloc err!! %d\n",jpg_size);
			}
			else
			{
				u8 ret=spi_udisk_read_jpg(index,jpg);
				if(0!=ret)
				{
					deg_Printf("jpg read %d\n",ret);
				}
			}
			arg.media.type = MEDIA_SRC_RAM;
			arg.media.src.buff = (u32)jpg;
			arg.wait = 1;  // wait decode end
			if(imageDecodeStart(&arg)<0)
			{
				deg_Printf("jpg decode fail\n");
				dispLayerFreeFrame(DISP_LAYER_VIDEO,p_lcd_buffer);
			}
			if(NULL!=jpg)
				hal_sysMemFree(jpg);

			INT16U width,height;
			imageDecodeGetResolution(&width,&height);
			deg_Printf("pass jpg decode\n");
			deg_Printf("show jpeg:w=%d,h=%d\n",lcd_w,lcd_h);

			ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,lcd_w*lcd_h*3/2);
			if(first_frame_not_display)
				dispLayerFreeFrame(DISP_LAYER_VIDEO,p_lcd_buffer);
			else
				dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,lcd_w,lcd_h,VIDEO_ROTATE);
			deg_Printf("video player : JPG[%d:%d]\n",width,height);
		}
	}
	else
	{
		if( (spi_udisk_jpg_num()>0)&&(managerFileCount(SysCtrl.avi_list) <= 0) )
		{
			goto  display_udisk;
		}
	
		if(videoPlaybackGetStatus() != MEDIA_STAT_STOP)
	          	videoPlaybackStop();
		if(managerFileCount(SysCtrl.avi_list)<1)
			return -1;
		deg_Printf("video play : index = %d ",index);
		name = manangerGetFileFullPathName(SysCtrl.avi_list,index,&type);
		if(name == NULL)
		{
			return -1;
		}
	    SysCtrl.file_index = index;
	    SysCtrl.file_type=type;
	    deg_Printf(": %s\n",name);
		if(type == FILE_TYPE_JPG)
		{
			Image_ARG_T arg;
			disp_frame_t *p_lcd_buffer;
			u16 x,y;
			u16 lcd_w,lcd_h;
			u8 *temp_ybuf,*temp_uv_buf;
			p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
			hal_lcdGetVideoPos(&x,&y);
			dispLayerGetResolution(DISP_LAYER_VIDEO,&lcd_w,&lcd_h);

	#if (1 == LCD_PLAY_SCALE)
				if((320 == lcd_w)&&(240 == lcd_h))
				{
					x = 0;
					y = 30;
					arg.target.width = 320;
					arg.target.height = 180;
					lcd_w = arg.target.width;
					lcd_h = arg.target.height;
				}
				else
				{
					arg.target.width = lcd_w;
					arg.target.height = lcd_h;
				}
			#else
				arg.target.width = lcd_w;
				arg.target.height = lcd_h;

	#endif
		
			if(p_lcd_buffer)
			{
				deg_Printf("0:x:%d,y:%d,w:%d,h:%d\n",x,y,arg.target.width,arg.target.height);
				hal_dispframeVideoCfg(p_lcd_buffer,x,y,arg.target.width,arg.target.height);
			}
			else
			{
				deg_Printf("0get null buf!!\n");
			}
			arg.yout  = p_lcd_buffer->y_addr;
			arg.uvout = p_lcd_buffer->uv_addr;

			arg.media.type = MEDIA_SRC_FS;
			arg.media.src.fd = (FHANDLE)open(name,FA_READ);
			if((int)arg.media.src.fd<0)
				return -1;
			//layout_video_playback_name(manangerGetFileFullPathName(SysCtrl.avi_list,index,NULL));
			arg.wait = 1;  // wait decode end
			if(imageDecodeStart(&arg)<0)
			{
				deg_Printf("jpg decode fail\n");
				close(arg.media.src.fd);
				dispLayerFreeFrame(DISP_LAYER_VIDEO,p_lcd_buffer);
				return -1;
			}
			INT16U width,height;
			imageDecodeGetResolution(&width,&height);
			deg_Printf("pass jpg decode\n");
			deg_Printf("show jpeg:w=%d,h=%d\n",lcd_w,lcd_h);

			ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,lcd_w*lcd_h*3/2);
			if(first_frame_not_display)
				dispLayerFreeFrame(DISP_LAYER_VIDEO,p_lcd_buffer);
			else
				dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,lcd_w,lcd_h,VIDEO_ROTATE);

			close(arg.media.src.fd);
			
			deg_Printf("video player : JPG[%d:%d]\n",width,height);

		}
		else
		{
			VIDEO_PARG_T arg;
			INT16U width,height;
			
			arg.media.type = MEDIA_SRC_FS;
		    arg.media.src.fd = (FHANDLE)open(name,FA_READ);
			if((int)arg.media.src.fd<0)
				return -1;
	        //layout_video_playback_name(manangerGetFileFullPathName(SysCtrl.avi_list,index,NULL));
			dispLayerGetResolution(DISP_LAYER_VIDEO,&width,&height);
			arg.tar.width = width;
			arg.tar.height = height;
			hal_lcdGetVideoPos(&arg.pos.x,&arg.pos.y);
	#if (1 == LCD_PLAY_SCALE)
			if(320 == arg.tar.width)
			{
				arg.pos.x = 0;
				arg.pos.y = 30;
				arg.tar.width = 320;
				arg.tar.height = 180;
			}
	#endif
			//dispRatioResolutionCalculate(VIDEO_RATIO,&arg.tar.width,&arg.tar.height); 
			//arg.pos.x = (width-arg.tar.width)>>1;
			//arg.pos.y = (height-arg.tar.height)>>1;
			arg.rotate = VIDEO_ROTATE;
			arg.firstframe = TASK_VIDEOPLAY_FIRSTFRAME; // pause at first frame waiting user key
			
	        fs_fastseek_init(arg.media.src.fd);  // enable fast seek for this file
	       
			while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);  // wait key sound end
			if(videoPlaybackStart(&arg)<0)
			{
				winSetVisible(winItem(playBackWindow.handle,2),false);
				backBrushBlack(40);
				deg_Printf("avi decode fail<0x%x>\n",(int)arg.media.src.fd);
				close(arg.media.src.fd);
				return -1;
			}
			
			Media_Res_T *res= videoPlaybackGetResolution();
			INT32U totatime;
			videoPlaybackGetTime(&totatime,NULL);
			deg_Printf("video player : AVI[%d:%d],total time = %dm %ds %dms\n",res->width,res->height,(totatime/1000)/60,(totatime/1000)%60,totatime%1000);
		}
	    //layout_video_playback_name(manangerGetFileName(SysCtrl.avi_list,index));
	}
	XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_TIME_UPDATE,0));
	//deg_Printf("video play start.<%s>\n",name);
	return type;

}

void taskPlayBackOpen(uint32 arg)
{
	deg_Printf("video playback task enter.\n");
	deg_Printf("video a count = %d\n",managerFileCountEx(SysCtrl.avia_list));
	deg_Printf("video b count = %d\n",managerFileCountEx(SysCtrl.avib_list));
	while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);  // wait key sound end

	SysCtrl.volume_level = configGet(CONFIG_ID_VOLUME);
	videoPlaybackInit();
	dispLayerSetPIPMode(DISP_PIP_DISABLE);

#if 1
	UserInterface playbackMenuAnimation;
	ANIMATION(playbackMenuAnimation, BOTTOM2UPPER)
	u16 lcd_w,lcd_h;
	u8 *play_bk_buf;
	bool exist_file = false;
	//exist_file = true;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	play_bk_buf = hal_sysMemMalloc(lcd_w*lcd_h*3/2,32);
	if(!play_bk_buf)
		deg_Printf("play_bk_buf malloc err.\n");
	memset(play_bk_buf,80,lcd_w*lcd_h);
	memset(play_bk_buf + lcd_w*lcd_h,0x80,lcd_w*lcd_h/2);
	first_frame_not_display = true;

	bool change_finir = false;
	first_frame_not_display = false;
	disp_frame_t *p_lcd_buffer;
	p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	
	if(exist_file)
	{
		if(p_lcd_buffer)
		{
			hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);
			memcpy(play_bk_buf,p_lcd_buffer->y_addr,lcd_w*lcd_h*3/2);
			dispLayerFreeFrame(DISP_LAYER_VIDEO,p_lcd_buffer);
		}
		else
		{
			deg_Printf("video buf null!\n");
		}
	}
	
	while(1)
	{
		if(p_lcd_buffer)
		{
			hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);

			memcpy(p_lcd_buffer->y_addr,play_bk_buf,lcd_w*lcd_h*3/2);
			change_finir = playbackMenuAnimation.run(&playbackMenuAnimation, p_lcd_buffer->y_addr);
			ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,lcd_w*lcd_h*3/2);
			dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
			XOSTimeDly(40);
			if(change_finir == true)
				break;
		}
		else
		{
			deg_Printf("video buf null!\n");
		}
	}
	if(play_bk_buf)
		hal_sysMemFree(play_bk_buf);
#endif

	if(NULL==SysCtrl.sdcard)
	{
		if(spi_udisk_jpg_num()>0)
		{
			SysCtrl.file_index = 0;
			video_playback_start(SysCtrl.file_index); 
//			exist_file = true;
		}
	}
	else
	{
		if(managerFileCount(SysCtrl.avi_list)>0)
		{
			SysCtrl.file_index = managerFileCount(SysCtrl.avi_list);
			video_playback_start(SysCtrl.file_index); 
//			exist_file = true;		  
		}
	}
	uiOpenWindow(&playBackWindow,0);
	
}

void taskPlayBackService(uint32 arg)
{
	static uint32 lastPlayTime=0;
	u32 totoltime=0;
	uint32 playTime=0;

	if(SDC_STAT_NULL==SysCtrl.sdcard)
	{
		return ;
	}
	
	#if TASK_VIDEOPLAY_AUTOPLAY>0
	if((videoPlaybackGetStatus() == MEDIA_STAT_STOP) && (SysCtrl.file_type==FILE_TYPE_AVI))
	{
        #if (TASK_VIDEOPLAY_AUTOPLAY > 1)
		SysCtrl.file_index++;
		if(SysCtrl.file_index>managerFileCount(SysCtrl.avi_list))
			SysCtrl.file_index = 1;
        #endif
		video_playback_start(SysCtrl.file_index);	
	}
    #endif
	videoPlaybackGetTime(&totoltime,&playTime);
	if(playTime>totoltime)
	{
		playTime=totoltime;
	}
	playTime = playTime/1000;
	if(lastPlayTime!=playTime)
	{
		lastPlayTime=playTime;
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_TIME_UPDATE,0));
	}

	videoPlaybackService();
}
void taskPlayBackClose(uint32 arg)
{
	if(videoPlaybackGetStatus() != MEDIA_STAT_STOP)
          videoPlaybackStop();
	while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);  // wait key sound end	
	videoPlaybackUninit();
	//hal_mjpB_ecdown();
	deg_Printf("video playback task exit.\n");
}
sysTask taskPlayBack=
{
	"Play Back",
	NULL,
	taskPlayBackOpen,
	taskPlayBackClose,
	taskPlayBackService,	
};


