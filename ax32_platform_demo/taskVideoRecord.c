#include "application.h"
#include "userInterface.h"


char tempFileName1[MANAGER_NAME_LEN+1+8],tempFileName2[MANAGER_NAME_LEN+1+8];  // there are two memories used by different program temparlly.do not mask
extern int task_image_createfile(int channel,char **filename);
extern msgDealInfor videoRecordeMsgDeal[];
extern sysTask taskVideoRecorde;

/*******************************************************************************
* Function Name  : video_record_callback
* Description    : video record callback function.user should fill the function,video record service will callback
* Input          : 
* Output         : none                                            
* Return         : int fd : file handle
*******************************************************************************/
int video_record_callback(INT32U channel,INT32U cmd,INT32U para)
{
	//static INT8U chBfileFlag = 0; //channel B created file flag,for lock file using
	char *name;
    FHANDLE *fdt;
	int fd[2],ret=0,index,rectime;
	INT32U bsize,size;
	static INT32U oldfilesize;
	if(channel >= VIDEO_CH_MAX)
	{
		deg_Printf("video : unknow channel.%d\n",channel);
		return -1;
	}
    
	if(cmd == CMD_VIDEO_RECORD_START) // video record start
	{
		XMsgQPost(SysCtrl.sysQ,makeEvent(VID_EVENT_REC_RESUME,0));
		rectime = configValue2Int(CONFIG_ID_LOOPTIME);
		//if(channel == VIDEO_CH_A)
		{
			//if(rectime == 0)
			{
				
				//size = videoRecordSizePreSec(channel,30);
				//if(SysCtrl.sd_freesize<size)
				if((SysCtrl.sd_freesize>>10)<REMAIN_MIN_VIDEO)
				{
					deg_Printf("----------1111-------%dM--------un sycl record full.\n",(SysCtrl.sd_freesize>>10));
					 SysCtrl.sdcard = SDC_STAT_FULL;
					XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
					
					//XMsgQPost(SysCtrl.sysQ,makeEvent(VID_EVENT_REC_ERROR,0));
					return -1;
				}
				//rectime = 10*60;
			}
			fd[0] = -1;
		#if RECORD_MODE == RECORD_MODE_AUTO  // create videob folder
             if((SysCtrl.bfolder==0)&&(SysCtrl.usensor!=USENSOR_STAT_NULL))
             {
			 	strcpy(tempFileName1,FILEDIR_VIDEOB);
				tempFileName1[strlen(tempFileName1)-1] = 0;
				f_mkdir((const TCHAR*)tempFileName1);//FILEDIR_VIDEO);
				SysCtrl.bfolder = 1;
				deg_Printf("video : create folder %s\n",tempFileName1);
				tempFileName1[0] = 0;
             }
		#endif	
			oldfilesize = 0;
		#if AVI_TYPE_ODML	
			bsize = videoRecordSizePreSec(channel,rectime);

			memset(tempFileName2,0,MANAGER_NAME_LEN+1+8);
			if(bsize>SysCtrl.sd_freesize)
			{
				index = 1;
				if(channel == VIDEO_CH_A)
					name = manangerFindFileAndDel(SysCtrl.avi_list,&index,MA_FILE_AVIA);
				else if(channel == VIDEO_CH_B)
					name = manangerFindFileAndDel(SysCtrl.avi_list,&index,MA_FILE_AVIB);
				if(name)
				{
					 fd[0] = open(name,FA_WRITE | FA_READ);	
					 if(fd[0]>=0)
					 {
					 	 strcpy(tempFileName2,name);
						 size = fs_size(fd[0])>>10;
						 bsize = videoRecordSizePreSec(channel,rectime);
                         if((size<=(bsize+100*1024))&&(size>=bsize))
                         {
						 	oldfilesize =  bsize = fs_size(fd[0])>>10;
							managerDelFileByIndex(SysCtrl.avi_list,index);
                         }
						 else
						 {
						 	close(fd[0]);
							fd[0] = -1;
							oldfilesize = bsize = 0;
						 }
					 }
				}
				if(fd[0]<0)
				{
					oldfilesize = bsize = 0;
					//tempFileName2[0] = 0;
					memset(tempFileName2,0,MANAGER_NAME_LEN+1+8);
				}
			}
         #endif   
		 if(channel == VIDEO_CH_A)
			name = managerFileNameCreater(SysCtrl.avi_list,SUFFIX_AVI,FILEDIR_VIDEOA); // the static memory is unsafe,maybe used by other programe when video recording
		 else if(channel == VIDEO_CH_B)
			name = managerFileNameCreater(SysCtrl.avi_list,SUFFIX_AVI,FILEDIR_VIDEOB); // the static memory is unsafe,maybe used by other programe when video recording
			if(name == NULL)
			{
				SysCtrl.sdcard = SDC_STAT_FULL;
				XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
				memset(tempFileName1,0,MANAGER_NAME_LEN+1+8);
				//tempFileName1[0] = 0;
				sysLog("managerFileNameCreater fail");
				XMsgQPost(SysCtrl.sysQ,makeEvent(VID_EVENT_REC_ERROR,0));
				return -1;
			}
			strcpy(tempFileName1,name);	
		}
		
		
		if(fd[0]<0)
		{
			fd[0] = open(tempFileName1,FA_CREATE_ALWAYS | FA_WRITE | FA_READ);  // FA_CREATE_NEW
			bsize = 0;
		}
		else
		{
			bsize = fs_size(fd[0]);
			close(fd[0]);
			f_rename(tempFileName2,tempFileName1);  // must close before rename 
			fd[0] = open(tempFileName1,FA_WRITE | FA_READ);
			//f_rename(tempFileName2,tempFileName1);
			deg_Printf("using old file:%s.%d b\n",tempFileName2,bsize);
			//sysLog("using old file.");
		}
	 //   fd[0] = open(tempFileName1,FA_WRITE | FA_READ);  // FA_CREATE_NEW
		deg_Printf("video : record start->%s ",tempFileName1);	
	//	sysLog(tempFileName1);

		
	#if AVI_TYPE_ODML == 0		    
		if(channel == VIDEO_CH_A)
		{
			fd[1] = open("VIDEOA.TMP",FA_CREATE_ALWAYS | FA_WRITE | FA_READ);  // FA_CREATE_NEW
		}
		else if(channel == VIDEO_CH_B)
		{
			fd[1] = open("VIDEOB.TMP",FA_CREATE_ALWAYS | FA_WRITE | FA_READ);  // FA_CREATE_NEW	
		}
		
	#else
	    fd[1] = fd[0];
	#endif
		if((fd[0]>=0) && (fd[1]>=0)) // success
		{	        
			fdt = (FHANDLE *)para;
			*fdt = (FHANDLE)fd[0];
			*(fdt+1) = (FHANDLE)fd[1];
			deg_Printf("success.\n");
			if(bsize == 0) // try to alloc size
			{
			//----count video file size	
				if (oldfilesize)
					size = bsize = oldfilesize;
				else 
					size = bsize = videoRecordSizePreSec(channel,rectime);
				/*
				ret = managerSpaceCheck(SysCtrl.avi_list,FILEDIR_VIDEO,bsize);
				if((ret<0)||((ret<<10)<bsize))
					*/
				ret = managerSpaceCheck(SysCtrl.avi_list,FILEDIR_VIDEO,0);
				if(ret<0)
				{
					deg_Printf("-----------------22222----%d-----no more space\n",(SysCtrl.sd_freesize>>10));
		
					//sysLog("no more space");
				    SysCtrl.sdcard = SDC_STAT_FULL;
					XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
					sysLog("managerSpaceCheck no space");
					//if(task_video_Get_Status() == MEDIA_STAT_START)  // stop current recording
					//{
				     //  	XMsgQPost(SysCtrl.sysQ,makeEvent(KEY_EVENT_OK, KEY_PRESSED));
					//}
			
					ret = -1;
					goto VIDEO_RECORD_ERROR_HANDLE;
				}
				else
				{
					u32 temp=ret;
					if((temp<<10)>=bsize)
					{
					}
					else
					{
						bsize=temp<<10;
					}
					deg_Printf("bsize:%d\n",bsize);
				}
				
				if(size>0x3c0000) // 3.8G
				   size = 0x3c0000;
		#if AVI_TYPE_ODML		
			#if FILE_SIZE_PRE_MALLOC
				ret = XOSTimeGet();	
				if(lseek(fd[0],size<<10,0)<0)
				{			
				    deg_Printf("lseek fail.%d b,%dkb\n",size<<10,size);
					//sysLog("lseek fail");
					ret = -1;
					goto VIDEO_RECORD_ERROR_HANDLE;
				}
				fs_sync(fd[0]);
				ret = XOSTimeGet()-ret;
				if(fs_size(fd[0])<(size<<10))
				{
					debg("fs_size:%d,size:%d\n",fs_size(fd[0]),size<<10);
					deg_Printf("lseek fail.\n");
					ret = -1;
					goto VIDEO_RECORD_ERROR_HANDLE;
				}
				lseek(fd[0],0,0);
				bsize = fs_size(fd[0]);	
				deg_Printf("fs alloc time using : %dms,file size : %dkb,speed = %dkb/ms.\n",ret,bsize>>10,(bsize>>10)/ret);
			#endif
		#endif	
			}
			else
			{
				deamon_fsSizeModify(1,bsize);
			}
		#if AVI_TYPE_ODML		
	 		#if FILE_SIZE_PRE_MALLOC
				ret = XOSTimeGet();
				if(fs_fastseek_init(fd[0])<0)
				{
					deg_Printf("fs_fastseek_init fail.\n");
					//sysLog("fs_fastseek_init fail");
					ret = -1;
					goto VIDEO_RECORD_ERROR_HANDLE;
				}
				ret = XOSTimeGet()-ret;
				deg_Printf("fs_fastseek_init time using : %dms.\n",ret);
				/*if(channel == VIDEO_CH_B)
					chBfileFlag = 2;
				else
					chBfileFlag = 1;
				*/
				return (bsize>>10);
			#else		
				return bsize;
			#endif
		#else	
			//if(channel == VIDEO_CH_A)
			//	    SysCtrl.t_rec = 0;			
				return (SysCtrl.sd_freesize>>10);
		#endif	
		}
		else //fail
		{			
		    deg_Printf("fail.\n");
			ret = -1;
			goto VIDEO_RECORD_ERROR_HANDLE;
		}
	}
	else if(cmd == CMD_VIDEO_RECORD_STOP)// video record start
	{
		//if( channel == VIDEO_CH_A)
		{
		//	SysCtrl.t_rec = 0;
		}
		//SysCtrl.t_rec = 0;
		fdt = (FHANDLE *)para;
		fd[0] = (int)*fdt;
		fd[1] = (int)*(fdt+1);
		if(SysCtrl.sdcard != SDC_STAT_NORMAL)
		{
			SysCtrl.g_lock=0;
			XMsgQPost(SysCtrl.sysQ,makeEvent(VID_EVENT_REC_STOP,0));
			goto VIDEO_RECORD_ERROR_HANDLE;
		}
		bsize = fs_size(fd[0]);
		if(bsize>0)
			deamon_fsSizeModify(-1,bsize);
		bsize>>=10;
		deg_Printf("video : fs size = %dG%dM%dKB\n",bsize>>20,(bsize>>10)&0x3ff,(bsize)&0x3ff);
		close((FHANDLE)fd[0]);
	#if AVI_TYPE_ODML == 0		
		close((FHANDLE)fd[1]);
	#endif
		name = tempFileName1;		    
		if(SysCtrl.g_lock)// video A
		{	
			managerUnlockFile(name);  // maybe file name is locked when the second callback
			strcpy(tempFileName2,tempFileName1);  // backup file name
			managerLockFile(tempFileName1); // rename in resource manager list

			if(channel == VIDEO_CH_A)
			{
				memcpy(tempFileName2,FILEDIR_VIDEOA,strlen(FILEDIR_VIDEOA));
				memcpy(tempFileName1,FILEDIR_VIDEOA,strlen(FILEDIR_VIDEOA));						
			}
			else if(channel == VIDEO_CH_B)
			{
				memcpy(tempFileName2,FILEDIR_VIDEOB,strlen(FILEDIR_VIDEOB));
				memcpy(tempFileName1,FILEDIR_VIDEOB,strlen(FILEDIR_VIDEOB));
			}
			
			f_rename(tempFileName2,tempFileName1);  // rename in file system
			//deg_Printf("video : rename  .%s->%s\n",tempFileName2,tempFileName1);
			deg_Printf("video : lock this file.\n");
		}
		name = &tempFileName1[strlen(FILEDIR_VIDEOA)];
		deg_Printf("video : stop ");
		if(channel == VIDEO_CH_A)
		{
			oldfilesize = 0;
			ret = managerAddFile(SysCtrl.avi_list,FILEDIR_VIDEOA,name);	// add video a
			deg_Printf("%s%s\n",FILEDIR_VIDEOA,name);

		}
		else if(channel == VIDEO_CH_B)
		{
			oldfilesize = 0;
			ret = managerAddFile(SysCtrl.avi_list,FILEDIR_VIDEOB,name);	// add video b
			deg_Printf("%s%s\n",FILEDIR_VIDEOB,name);

		}
		SysCtrl.g_lock = 0; // clear g-sensor lock active flag
		/*if(chBfileFlag)
		{
		     chBfileFlag--;
			 if(chBfileFlag == 0)
			 {
			 	SysCtrl.g_lock = 0; // clear g-sensor lock active flag
			 }
		}*/
		task_video_record_caltime();
		deg_Printf("video : fs free size = %dG%dM%dKB\n",SysCtrl.sd_freesize>>20,(SysCtrl.sd_freesize>>10)&0x3ff,(SysCtrl.sd_freesize)&0x3ff);
		return 0;
	}
	else if(cmd == CMD_COM_ERROR)// video record error
	{
		//chBfileFlag = 0;
		SysCtrl.g_lock = 0; // clear g-sensor lock active flag
		//if(para == CMD_VIDEO_RECORD_START) // start fail
		{
			fdt = (FHANDLE *)para;
			fd[0] = (int)*fdt;
			fd[1] = (int)*(fdt+1);
			ret = 0;
			SysCtrl.g_lock = 0; // clear g-sensor lock active flag
			if(hal_sdGetCardState()==SDC_STATE_ERROR)
			{
				SysCtrl.sdcard = SDC_STAT_ERROR;
				XMsgQPost(SysCtrl.sysQ,makeEvent(VID_EVENT_REC_ERROR,0));
				XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
				deg_Printf("video : error\n");
			}
			else //if(channel == VIDEO_CH_A)
			{
				XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_ON, 0));  // restart
				deg_Printf("[CMD_COM_ERROR]video : send event on\n\n\n");
				//sysLog("video : send event on");
			}
			deg_Printf("video : error fs size %d\n",fs_size(fd[0]));
			deg_Printf("video : fs before size = %dG%dM%dKB\n",SysCtrl.sd_freesize>>20,(SysCtrl.sd_freesize>>10)&0x3ff,(SysCtrl.sd_freesize)&0x3ff);
			bsize = fs_size(fd[0]);
			if(close((FHANDLE)fd[0]) < 0)
				bsize = 0 ;
			if(bsize>0){
		#if AVI_TYPE_ODML
				name = tempFileName1;		    
				name = &tempFileName1[strlen(FILEDIR_VIDEOA)];
				if(channel == VIDEO_CH_A)
				{
					ret = managerAddFile(SysCtrl.avi_list,FILEDIR_VIDEOA,name);	// add video a
					deg_Printf("%s%s\n",FILEDIR_VIDEOA,name);
				}
				else if(channel == VIDEO_CH_B)
				{
					ret = managerAddFile(SysCtrl.avi_list,FILEDIR_VIDEOB,name);	// add video b
					deg_Printf("%s%s\n",FILEDIR_VIDEOB,name);
				}
		#endif
			}
				
			//close((FHANDLE)fd[0]);
		#if AVI_TYPE_ODML == 0		
			close((FHANDLE)fd[1]);
		    goto VIDEO_RECORD_ERROR_HANDLE;
		#else
		    deamon_fsSizeModify(-1,bsize);	
		#endif
		    return -1;
		}
	}
	else if(cmd == CMD_COM_CHECK) // check sdc sapce
	{
		if(para==0)
		{
			ret = videoRecordSizePreSec(channel,configValue2Int(CONFIG_ID_LOOPTIME));
			
			para = ret;
		}
		ret = managerSpaceCheck(SysCtrl.avi_list,FILEDIR_VIDEO,para);//REMAIN_SAPCE_VIDEO);
		if(ret<0)
		{
			deg_Printf("-----------------33333----%d-----no more space\n",(SysCtrl.sd_freesize>>10));
			SysCtrl.sdcard = SDC_STAT_FULL;
			XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
			sysLog("CMD_COM_CHECK no space");
		}
		return ret;
	}

	else if(cmd == CMD_MSG_RECODE_STOP) // check sdc sapce  full 
	{
		deg_Printf("-----------------4444----%d-----no more space\n",(SysCtrl.sd_freesize>>10));
		SysCtrl.sdcard = SDC_STAT_FULL;
		XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
		sysLog("CMD_COM_CHECK no space");
		return 0;
	}
	
	//boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,0);
	return 0;

VIDEO_RECORD_ERROR_HANDLE:	
	XMsgQPost(SysCtrl.sysQ,makeEvent(VID_EVENT_REC_ERROR,0));
	deg_Printf("video : error.delete current file\n");
	close((FHANDLE)fd[0]);
#if AVI_TYPE_ODML == 0	
	close((FHANDLE)fd[1]);
#endif
    if(channel == VIDEO_CH_A)
    {
		memcpy(tempFileName1,FILEDIR_VIDEOA,strlen(FILEDIR_VIDEOA));
    }
	else if(channel == VIDEO_CH_B)
	{
		memcpy(tempFileName1,FILEDIR_VIDEOB,strlen(FILEDIR_VIDEOB));
	}
    f_unlink(tempFileName1);  
#if AVI_TYPE_ODML == 0  
    if(channel == VIDEO_CH_A)
		f_unlink("VIDEOA.TMP");
	if(channel == VIDEO_CH_B)
	    f_unlink("VIDEOB.TMP");

#endif
	task_video_record_caltime();
	return ret;
}
int task_video_Get_Status(void)
{
	return videoRecordGetStatus();
}

int video_record_start(void)
{	
	int ret;
    
	if(SysCtrl.sdcard!=SDC_STAT_NORMAL)
	{
		XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
		return -1;
	}
	if(task_video_Get_Status() == MEDIA_STAT_START)
		return;
    while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);
	SysCtrl.t_rec = 0;
	msgDealByType(VID_EVENT_REC_START,winGetCurrent(),0,NULL);
	//mediaVideoCtrl.stat = MEDIA_STAT_START;
	ret = videoRecordStart();
   // videoRecordFileStartB(); 
	deg_Printf("video record start.%d\n",ret);	
	
	return ret;
}
int task_record_stop(void)
{
	int ret;

//	while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);
	if(task_video_Get_Status() != MEDIA_STAT_START)
		return;

	ret = videoRecordStop();
	XMsgQPost(SysCtrl.sysQ,makeEvent(VID_EVENT_REC_STOP,0));
	SysCtrl.t_rec = 0;

	SysCtrl.g_lock = 0; // clear g-sensor lock active flag
	deg_Printf("video record stop.%d,%d\n",ret,task_video_Get_Status());
 //   task_video_record_caltime();
	boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,1);
	hal_curBuffer_clear();
	return ret;
}
void task_video_record_caltime(void)
{
	INT32U size1,size2;

	if(SysCtrl.pip_mode == DISP_PIP_BACK)
		size1 = videoRecordSizePreSec(VIDEO_CH_B,1);
	else 
		size1 = videoRecordSizePreSec(VIDEO_CH_A,1);
	size2 = SysCtrl.sd_freesize;

    deg_Printf("rec time : %dkb/s,%dkb\n",size1,size2);
	SysCtrl.t_remain = size2/size1;
}
void taskVideoRecTimeIncreace(void)
{
	//if(task_video_Get_Status() == MEDIA_STAT_START)
	//	SysCtrl.t_rec++;
}
INT32S photoFd=-1;
char *photoname=NULL;
void videoCapturePhoto(void)
{
/*
	if((task_video_Get_Status() == MEDIA_STAT_START)&&(photoFd<0))
	{
		photoFd = task_image_createfile(0,&photoname);
		if(photoFd>=0)
		{
			if(videoRecordTakePhoto(photoFd)<0)
			{
				close(photoFd);
				f_unlink(photoname);
				photoFd = -1;
				deg_Printf("recorder : tabke photo fail\n");
			}
			else
				deg_Printf("recorder : tabke photo start\n");
		}
		else if (photoFd == -2)
			videoRecordTakePhoto(photoFd);
	}
*/
}


void taskVideoRecordeOpen(uint32 arg1)
{
	
	{
		disp_frame_t * handle_frame;
		UserInterface recordeAnimation;
		ANIMATION(recordeAnimation, SQUARE_INSIDE2OUTSIDE)
		
		hal_csiEnable(1);
		
	    SysCtrl.crop_level = 0;
	    dispLayerSetFrontCrop(SysCtrl.crop_level);

		SysCtrl.pip_mode = DISP_PIP_FRONT;

	    dispLayerSetPIPMode(SysCtrl.pip_mode);

		dispLayerInit(DISP_LAYER_VIDEO); // preview

		SysCtrl.photo_task=1;
		photo_animation_effect(recordeAnimation,0);
		SysCtrl.photo_task=0;
	}
	
		
	VIDEO_ARG_T arg;
	INT32S ret;
	
	deg_Printf("video record task enter.\n");
    //sysLog("video record task enter.");
	//task_get_state(NULL,(INT8U *)&curstat);
	arg.quality =  JPEG_Q_40;
	arg.fps = 30;
	arg.timestramp = configValue2Int(CONFIG_ID_TIMESTAMP);
	arg.audio = configValue2Int(CONFIG_ID_AUDIOREC);
	deg_Printf("arg.audio=%d\n",arg.audio);
	arg.rectime = configValue2Int(CONFIG_ID_LOOPTIME);
	arg.frame_enable = 0;
	
	if(arg.rectime==0)
	{
		arg.rectime = 10*60;  // 10 min
		arg.looprecord = 0;
	}
	else
		arg.looprecord = 1;
	ret = configValue2Int(CONFIG_ID_RESOLUTION);
	arg.resolution.width = ret>>16;
	arg.resolution.height = ret&0xffff;	
    arg.callback = video_record_callback;
//	arg.fastwr = FILE_FAST_WRITE;
	arg.ftype = AVI_TYPE_ODML;
	arg.syncwr = FILE_SYNC_WRITE;
	
    videoRecordInit(&arg);	

#if USENSOR_AUDIO_ON == 0
		if (arg.audio)
			arg.audio = 0;
#endif
#if USENSOR_RESOLUTION == 1
	arg.resolution.width = 640;
	arg.resolution.height = 480;
#elif USENSOR_RESOLUTION == 2
	arg.resolution.width = 1280;
	arg.resolution.height = 720;
#elif USENSOR_RESOLUTION == 3
	arg.resolution.width = 1280;
	arg.resolution.height = 960;
#else
	arg.resolution.width = 640;
	arg.resolution.height = 480;
#endif
//	videoRecordInitB(&arg);	

	//hal_csiEnable(1);

	if(NULL==audiobuff)
	{
		audiobuff=hal_sysMemMalloc(AUDIO_FRAMES_SIZE,64);
	}
	
/*	
	//layout_video_record_init();
	dispLayerInit(DISP_LAYER_VIDEO); // preview sensor,必须先初始化再设置pip

    SysCtrl.crop_level = 0;
    dispLayerSetFrontCrop(SysCtrl.crop_level);

	SysCtrl.pip_mode = DISP_PIP_FRONT;

    dispLayerSetPIPMode(SysCtrl.pip_mode); //should disable pip mode 
*/	
	
	task_video_record_caltime(); // re-cal
	//dispLayerSetPIPMode(DISP_PIP_DISABLE);  
	//msgFuncRegister(videoRecordeMsgDeal);

	uiOpenWindow(&videoRecordWindow,0);
	photoFd=-1;
	photoname=NULL;

}

void taskVideoRecordeService(uint32 arg)
{

	videoFocusIconCancel(videoRecordWindow.handle);

	if (videoRecordTakePhotoStatus() == MEDIA_STAT_ERROR)
	{
		if(SysCtrl.sdcard == SDC_STAT_FULL )
		{
			SysCtrl.sdcard = SDC_STAT_NORMAL;
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
			videoRecordSetPhotoStatus(MEDIA_STAT_STOP);
		}
	}
	if(photoFd>=0)
	{
		if(videoRecordTakePhotoStatus() == MEDIA_STAT_STOP)
		{
			deg_Printf("recorder: take photo [%s] sucess\n",photoname);
			deamon_fsSizeModify(-1,fs_size(photoFd));
	        close(photoFd);
	        managerAddFile(SysCtrl.jpg_list,FILEDIR_PHOTO,&photoname[strlen(FILEDIR_PHOTO)]);
			photoFd = -1;
			deg_Printf("recorder : take photo stop.\n");
		}
	}
#if FILE_SYNC_DELETE > 0	
	f_unlink_ext_service(); // fs service
#endif
	//if(SysCtrl.pip_mode == DISP_PIP_BACK)	
	//	videoRecordServiceB(); // record service B
	//else
		videoRecordService();  // record  service
}
void taskVideoRecordeClose(uint32 arg)
{
	task_record_stop();
	if(photoFd>=0)
	{
		if(videoRecordTakePhotoStatus() == MEDIA_STAT_STOP)
		{
			deamon_fsSizeModify(-1,fs_size(photoFd));
	        close(photoFd);
	        managerAddFile(SysCtrl.jpg_list,FILEDIR_PHOTO,&photoname[strlen(FILEDIR_PHOTO)]);
			deg_Printf("recorder : take photo stop.\n");
		}
		else
		{
			deg_Printf("recorder : take photo unfinish.delete file:%s\n",photoname);
			close(photoFd);
			f_unlink(photoname);
		}
	}
#if FILE_SYNC_DELETE > 0	
	while(f_unlink_ext_service());
#endif	
	if(SysCtrl.sdcard == SDC_STAT_FULL)
	{
		SysCtrl.sdcard = SDC_STAT_NORMAL;
	}
	photoFd=-1;
	photoname=NULL;
	//if(hal_lcdGetShowingRotate()==LCD_ROTATE_0)
	//	hal_rtcCallBackRelease(rtcfd);
	dispLayerUninit(DISP_LAYER_VIDEO);  
	if(audiobuff)
	{
		hal_sysMemFree(audiobuff);
		audiobuff=NULL;
	}

	//winDestroy(&taskVideoRecorde.handle);
	deg_Printf("video record task exit.\n");
}
sysTask taskVideoRecorde=
{
	"Video Recorde",
	NULL,
	taskVideoRecordeOpen,
	taskVideoRecordeClose,
	taskVideoRecordeService,
};


