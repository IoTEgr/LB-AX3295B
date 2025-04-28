#include "application.h"
#include "userInterface.h"
#include "../multimedia/imageL/inc/imageL_encode.h"
#include "../multimedia/watermark/image_watermark.h"

extern msgDealInfor photoEncodeMsgDeal[];
extern sysTask taskPhotoEncode;
#define JPG_ENC_Q_TABLE_NUM 8
u8 jpg_encode_q_talbe[JPG_ENC_Q_TABLE_NUM] =
	{
		JPEG_Q_27, JPEG_Q_36, JPEG_Q_40, JPEG_Q_42, JPEG_Q_50, JPEG_Q_62, JPEG_Q_75, JPEG_Q_81};

INT32U sdFreesize = 0;
INT32U picSum = 0;

void photoImagePreSizeGet(void)
{
	if (SysCtrl.sdcard == SDC_STAT_NULL)
	{
		sdFreesize = 0; // spi_udisk_jpg_free_num();
		picSum = spi_udisk_jpg_free_num();
		return;
	}

	if (SysCtrl.sd_freesize >= 1024 * 5) // 5M
	{
		sdFreesize = SysCtrl.sd_freesize;
	}
	else
	{
		sdFreesize = 0;
	}

	if (configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_48M)
		picSum = sdFreesize / 1680;
	else if (configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_40M)
		picSum = sdFreesize / 1440;
	else if (configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_24M)
		picSum = sdFreesize / 800;
	else if (configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_20M)
		picSum = sdFreesize / 750;
	else if (configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_18M)
		picSum = sdFreesize / 700;
	else if (configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_16M)
		picSum = sdFreesize / 660;
	else if (configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_12M)
		picSum = sdFreesize / 630;
	else if (configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_10M)
		picSum = sdFreesize / 600;
	else if (configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_8M)
		picSum = sdFreesize / 480;
	else if (configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_5M)
		picSum = sdFreesize / 290;
	else if (configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_3M)
		picSum = sdFreesize / 130;
	else if (configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_2M)
		picSum = sdFreesize / 85;
	else if (configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_1M)
	{
		if (sdFreesize / 70 > 99999)
		{
			picSum = 99999;
		}
		else
		{
			picSum = sdFreesize / 70;
		}
	}
	else if (configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_VGA)
	{
		if (sdFreesize / 25 > 99999)
			picSum = 99999;
		else
			picSum = sdFreesize / 25;
	}
}

static int8 task_image_arg(INT16U *width, INT16U *height, INT8U *timestramp)
{
	// time stramp
	//*timestramp = configValue2Int(CONFIG_ID_TIMESTAMP);
	// resolution
	INT32U value = configValue2Int(CONFIG_ID_PRESLUTION);
	*width = value >> 16;
	*height = value & 0xffff;
	//*width = 4000;
	//*height = 3000;
	deg_Printf("take photo : [%d:%d]\n", *width, *height);
}

int task_image_createfile(int channel, char **filename)
{
	int fHandle, ret;
	char *name;
	TCHAR fname[64];

	if (SysCtrl.sdcard != SDC_STAT_NORMAL)
	{
		XMsgQPost(SysCtrl.sysQ, (void *)makeEvent(SYS_EVENT_SDC, 0));
		return -1;
	}

	ret = managerSpaceCheck(SysCtrl.jpg_list, FILEDIR_PHOTO, 1024 * 5);
	deg_Printf("in photo:show ret[%d]\n", ret);
	if (ret < 0)
	{
		deg_Printf("sd card full\n");
		SysCtrl.sdcard = SDC_STAT_FULL;
		XMsgQPost(SysCtrl.sysQ, (void *)makeEvent(SYS_EVENT_SDC, 0));
		return -2;
	}
	//---------creater file name
	name = managerFileNameCreater(SysCtrl.jpg_list, SUFFIX_JPG, FILEDIR_PHOTO);
	deg_Printf("photo : create file name:%s\n", name);
	if (name == NULL)
	{
		deg_Printf("photo : create file name fail.\n");
		SysCtrl.sdcard = SDC_STAT_FULL;
		XMsgQPost(SysCtrl.sysQ, (void *)makeEvent(SYS_EVENT_SDC, 0));
		return -2;
	}
	//---------open file from file system
	hal_sdLock();
	Ascii2Tchar(name, fname, sizeof(fname) / sizeof(fname[0]));
	fHandle = open(/*name*/ fname, FA_CREATE_NEW | FA_WRITE | FA_READ);
	if (fHandle < 0)
	{
		deg_Printf("photo : open file fail.%s\n", fname);

		ret = -3;
	}
	if (filename)
		*filename = name;
	return fHandle;
}

/*******************************************************************************
 * Function Name  : image_take_photo
 * Description    : take a photo by user config
 * Input          : none
 * Output         : none
 * Return         : int <0 fail
 *******************************************************************************/
int image_take_photo(void)
{
	char *name;
	int fHandle;
	int ret = -1, flag;
	INT16U width, height;
	INT8U timestramp;
	INT8U frame_enable;
	hal_watermarkClose(mediaVideoCtrl.tinfo);
	mediaVideoCtrl.tinfo = hal_watermarkOpen();
	if (mediaVideoCtrl.tinfo < 0)
		videor_print("photo watermark open fail:%d\n", mediaVideoCtrl.tinfo);

	ret = task_image_arg(&width, &height, &timestramp);

	timestramp = configValue2Int(CONFIG_ID_TIMESTAMP);
	if (videoRecordFrameEnGet())
		frame_enable = 1;
	else
		frame_enable = 0;

	flag = 0;

	//-------------channel A
	if (1)
	{
		u8 q;

		if (SysCtrl.photo_mode_switch == 3)
		{
			q = JPEG_Q_27;
		}
		else
		{

			if (width <= 1280)
			{
				q = JPEG_Q_81;
			}
			else if (width <= 2560)
			{
				q = JPEG_Q_75;
			}
			else if (width <= 3200)
			{
				q = JPEG_Q_62;
			}
			else if (width <= 3800)
			{
				q = JPEG_Q_36;
			}
			else
			{
				q = JPEG_Q_27;
			}
		}
		fHandle = task_image_createfile(VIDEO_CH_A, &name);
		if (fHandle < 0)
			goto TAKE_PHOTO_END;
		yuv_rgb_table_init();
		watermark_bmp2yuv_init(ST_PIXEL_W, ST_PIXEL_H, SM_PIC_ST_NUM);
		deg_Printf("-------------------have mem init after %d-------------------\n", hal_sysMemRemain());
		if (configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_12M || configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_16M || configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_18M || configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_20M || configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_24M || configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_40M || configGet(CONFIG_ID_PRESLUTION) == R_ID_STR_RES_48M)
		{
			ret = imageEncodeQuadStart(name, (FHANDLE)fHandle, width, height, JPEG_Q_27, timestramp, frame_enable);
		}
		else
		{
			ret = imageEncodeStart((FHANDLE)fHandle, width, height, q, timestramp, frame_enable);
		}
		watermark_buf_bmp2yuv_free();
		yuv_rgb_table_uninit();
		deg_Printf("-------------------have mem after %d-------------------\n", hal_sysMemRemain());
		if (ret < 0)
		{
			deg_Printf("photo : take photo fail.<%d>\n", ret);
			TCHAR fname[64];
			Ascii2Tchar(name, fname, sizeof(fname) / sizeof(fname[0]));
			close((FHANDLE)fHandle);
			f_unlink(/*name*/ fname);
			ret = -3;
			goto TAKE_PHOTO_END;
		}

		//===handle exif===
#if (1 == JPEG_EXIF)
		int res;
		u8 *temp_ptr = (u32)hal_sysMemMalloc(fs_size(fHandle), 32);
		if (NULL == temp_ptr)
		{
			deg_Printf("image encode : exif handle mem err.\n");
		}
		else
		{
			// memset(temp_ptr,0,fs_size(fHandle)+512);
			res = lseek(fHandle, 0x14, 0);
			res = read(fHandle, (void *)temp_ptr, fs_size(fHandle) - 0x14);
			u8 *head_ptr = (u32)hal_sysMemMalloc(0x2e8, 32);
			if (NULL != head_ptr)
			{
				u8 datatime[20];
				DATE_TIME_T *rtcTime = hal_rtcTimeGet();
				datatime[0] = (rtcTime->year / 1000) % 10 + '0';
				datatime[1] = (rtcTime->year / 100) % 10 + '0';
				datatime[2] = (rtcTime->year / 10) % 10 + '0';
				datatime[3] = (rtcTime->year) % 10 + '0';
				datatime[4] = ':';
				datatime[5] = (rtcTime->month / 10) % 10 + '0';
				datatime[6] = (rtcTime->month) % 10 + '0';
				datatime[7] = ':';
				datatime[8] = (rtcTime->day / 10) % 10 + '0';
				datatime[9] = (rtcTime->day) % 10 + '0';
				datatime[10] = ' ';
				datatime[11] = (rtcTime->hour / 10) % 10 + '0';
				datatime[12] = (rtcTime->hour) % 10 + '0';
				datatime[13] = ':';
				datatime[14] = (rtcTime->min / 10) % 10 + '0';
				datatime[15] = (rtcTime->min) % 10 + '0';
				datatime[16] = ':';
				datatime[17] = (rtcTime->sec / 10) % 10 + '0';
				datatime[18] = (rtcTime->sec) % 10 + '0';
				datatime[19] = 0;
				memset(head_ptr, 0, 0x2e8);
				res = jpeg_exif_head_init(head_ptr, width, height, datatime);
				head_ptr[0x2e6] = 0xff;
				head_ptr[0x2e7] = 0xD9;
				res = lseek(fHandle, 0, 0);
				res = write(fHandle, (void *)head_ptr, 0x2e8);
				res = write(fHandle, (void *)temp_ptr, fs_size(fHandle) - 0x14);
				hal_sysMemFree(head_ptr);
			}
			hal_sysMemFree(temp_ptr);
			deg_Printf("image encode : exif handle filesize 0x%x.\n", fs_size(fHandle));
		}

#endif
		//===end handle exif===
		deamon_fsSizeModify(-1, fs_size(fHandle));
		close(fHandle);
		managerAddFile(SysCtrl.jpg_list, FILEDIR_PHOTO, &name[strlen(FILEDIR_PHOTO)]);

		if (picSum > 0)
			picSum--;
		flag |= 1;
	}

	hal_sdStop();
	ret = 0;
TAKE_PHOTO_END:
	hal_sdUnlock();

	if (flag)
		return 0;
	else
		return -1;
}
int image_take_photo_to_sdram()
{
	s32 res = 0;
	u32 addr, size, timeout, delaytime, allsize;
	void *buff; //,*tbuff;
	s32 sync, sync_next;
	u16 image_width, image_height;
	u8 timestramp, q_idx;
	u8 frame_enable;
	INT16U width, height;

	hal_watermarkClose(mediaVideoCtrl.tinfo);
	mediaVideoCtrl.tinfo = hal_watermarkOpen();
	if (mediaVideoCtrl.tinfo < 0)
		videor_print("photo watermark open fail:%d\n", mediaVideoCtrl.tinfo);

	task_image_arg(&width, &height, &timestramp);

	timestramp = configValue2Int(CONFIG_ID_TIMESTAMP);

	//==flash for save more jpg ,set jpg is senser size==
	image_width = devSensorOp->pixelw;
	image_height = devSensorOp->pixelh;

	if (videoRecordFrameEnGet())
		frame_enable = 1;
	else
		frame_enable = 0;

	yuv_rgb_table_init();
	watermark_bmp2yuv_init(ST_PIXEL_W, ST_PIXEL_H, SM_PIC_ST_NUM);
	hal_mjpegEncodeResolutionImage(image_width, image_height);

	if (hal_mjpegMemInit(1) < 0)
	{
		watermark_buf_bmp2yuv_free();
		deg_Printf("image encode : memory malloc fail\n");
		res = -1;
		goto IMAGE_ENCODE_ERR;
	}

	hal_mjpegPhotoStart2(devSensorOp->pixelw, devSensorOp->pixelh, JPEG_Q_81, 0 /*timestramp*/, frame_enable, 1, 1); // didn't crop ,use orig
	//==wait csi yuv buf ok==
	timeout = XOSTimeGet();
	while (1)
	{
		if (hal_mjpeg_manual_get_csi_yuv_sta())
		{
			break;
		}

		if ((timeout + 1500) < XOSTimeGet())
		{
			res = -2;
			goto IMAGE_ENCODE_ERR;
		}
	}
	//==set lcd image stop==
	delaytime = XOSTimeGet();
	hal_wdtClear();
	hal_csiEnable(0);
	dispLayerSetPIPMode(DISP_PIP_DISABLE);

	//==end set lcd image stop==

	if (SysCtrl.f_keysound)
	{
		logo_music_show(RES_MUSIC_TAKE_PHOTO);
	}
	while (audioPlaybackGetStatus() == MEDIA_STAT_PLAY)
		;

	//==wait jpg encode==
	ax32xx_mjpeg_manual_on();
	ax32xx_intEnable(IRQ_JPGA, 1); // enable jpegirq

	if (&mjpegEncCtrl.vids == NULL)
	{
		deg_Printf("no vids frame\r\n");
	}
	if (mjpegEncCtrl.curBuffer == NULL)
		deg_Printf("no curBuffer frame\r\n");

	timeout = XOSTimeGet();
	while (1)
	{
		buff = hal_mjpegRawBufferGet(buff, &addr, &size, &sync, &sync_next); // hal_mjpegRawDataGet(&tbuff,&addr,&size);   // get jpeg frame addr & jpeg size.a total frame contains some of small buffers.here is the total size & start buffer addr
		if (buff)
		{
			// deg_Printf("buff:%x size:%d\r\n",buff,size);
			break;
		}
		if (buff == NULL)
		{
			// deg_Printf("bobobob\r\n");
		}
		// deg_Printf("buff:%x size:%d\r\n",buff,size);
		if ((timeout + 3000) < XOSTimeGet())
		{
			// deg_Printf("image encode : timeout 2-second.\n");
			break;
		}
	}
	ax32xx_csiEnable(1);
	dispLayerSetPIPMode(DISP_PIP_DISABLE);
	XOSTimeDly(20);
	u32 ticks = XOSTimeGet();
	Image_ARG_T arg;
#if CHANGE_4_3
	arg.target.width = devSensorOp->pixelw;
	arg.target.height = devSensorOp->pixelh;
#else
	arg.target.width = mjpegEncCtrl.csi_width;
	arg.target.height = mjpegEncCtrl.csi_height;

#endif
	arg.yout = (u8 *)mjpegEncCtrl.ybuffer;
	arg.uvout = (u8 *)mjpegEncCtrl.uvbuffer;
	arg.media.type = MEDIA_SRC_RAM;
	arg.media.src.buff = buff;
	arg.wait = 1; // wait decode end
	if (imageDecodeStart(&arg) < 0)
	{
		deg("jpg decode fail\n");
	}
	else
	{
		ticks = XOSTimeGet() - ticks;
		u16 dec_width, dec_height;
		imageDecodeGetResolution(&dec_width, &dec_height);
	}
	hal_csiEnable(1);
	dispLayerSetPIPMode(SysCtrl.pip_mode);
	hal_mjpeg_software_handle_csi_yuvbuf();
	if (timestramp)
	{
		watermark_bmp2yuv_draw((u8 *)mjpegEncCtrl.ybuffer, WATERMAKE_SET_X_POS, WATERMAKE_SET_Y_POS, WATER_CHAR_GAP);
		deg_Printf("add watermark finish \n");
	}
	hal_mjpegRawDataSet(buff);
	hal_mjpegPhotoStart2(image_width, image_height, JPEG_Q_81, 0 /*timestramp*/, frame_enable, 0, 0); // didn't crop ,use orig

	u32 jpg_addr;

ENCODE_AGAIN:
	//==scale down image handle==
	res = hal_mjpg_scale_down((u8 *)addr, 320, 240, JPEG_Q_81, &jpg_addr, &size);
	deg_Printf("res=%d,%d\n", res, size);
	if (0 == res)
	{
		if (size > FLASH_UDISK_JPG_SIZE)
		{
			if (q_idx > 0)
			{
				q_idx--;
			}
			goto ENCODE_AGAIN;
		}
		spi_udisk_write_jpg((u8 *)jpg_addr, size);
	}

	//==lcd image active==
	if (XOSTimeGet() > 1000 + delaytime)
	{
	}
	else
	{
		if (delaytime + 5 < XOSTimeGet())
		{
			XOSTimeDly(1000 - (XOSTimeGet() - delaytime));
		}
		else
		{
			XOSTimeDly(1000);
		}
	}
	photoImagePreSizeGet();
	hal_wdtClear();
	hal_csiEnable(1);
	dispLayerSetPIPMode(SysCtrl.pip_mode);
	hal_curBuffer_clear();

	boardIoctrl(SysCtrl.bfd_led, IOCTRL_LED_NO0, 1);

IMAGE_ENCODE_ERR:

	hal_mjpegEncodeStop();

	if (timestramp)
		videoRecordImageWatermark(image_width, image_height, 0); // disable

	watermark_buf_bmp2yuv_free();
	yuv_rgb_table_uninit();
	return res;
}
void photo_animation_effect(UserInterface name, uint8 flag)
{
	bool change_finir = false;
	disp_frame_t *handle_frame;

	uint16 lcd_w, lcd_h;

	while (1)
	{
		if (0 == SysCtrl.photo_software_handle)
		{
			handle_frame = yuv420_software_get_handle_frame();
			if (handle_frame)
				SysCtrl.photo_software_handle = 1;
			// deg_Printf("...\n");
		}
		else if (1 == SysCtrl.photo_software_handle)
		{
			SysCtrl.photo_software_handle = 2;

			if (flag)
			{
				flag = 0;
				hal_lcdGetResolution(&lcd_w, &lcd_h);
				lcd_w = (lcd_w + 0x1f) & (~0x1f); // add 32bit align
				memcpy(name.preFrameBuf, handle_frame->y_addr, lcd_w * lcd_h * 3 / 2);
				ax32xx_sysDcacheFlush((u32)name.preFrameBuf, lcd_w * lcd_h * 3 / 2);
			}

			change_finir = name.run(&name, handle_frame->y_addr);
			hal_lcdDealBuffer(handle_frame);
			hal_lcd_get_next_frame();
			if (change_finir == true)
				break;
		}
	}
}

void taskPhotoEncodeOpen(uint32 arg1)
{
	deg_Printf("-------------------have mem begin %d-----------------\n", hal_sysMemRemain());
	UserInterface photoAnimation;
	ANIMATION(photoAnimation, SQUARE_INSIDE2OUTSIDE)

	hal_mjpegEncodeInit();
	hal_csiEnable(1);

	//==close frame===
	hal_custom_frame_init();
	videoRecordCmdSet(CMD_COM_FRAME, 0);
	hal_custom_frame_add_enable(0);
	SysCtrl.photo_frame_idx = 0;

	//==normal color===
	SysCtrl.spec_color_index = 0;
	cmos_spec_color(SysCtrl.spec_color_index);
	//==close effect===

	SysCtrl.crop_level = 0;
	dispLayerSetFrontCrop(SysCtrl.crop_level);

	SysCtrl.pip_mode = DISP_PIP_FRONT;

	dispLayerSetPIPMode(SysCtrl.pip_mode);

	dispLayerInit(DISP_LAYER_VIDEO); // preview

	SysCtrl.photo_task = 1;
	SysCtrl.photo_mode_switch = 0;
	SysCtrl.photo_mode_idx = 0;

	photo_animation_effect(photoAnimation, 0);

	// msgFuncRegister(photoEncodeMsgDeal);

	uiOpenWindow(&photoEncodeWindow, 0);
}

void taskPhotoEncodeService(uint32 arg)
{

	photoFocusIconCancel(photoEncodeWindow.handle);

	if (0 == SysCtrl.photo_software_handle) // not effect handle
	{
		SysCtrl.photo_software_handle = 1;
		disp_frame_t *handle_frame = yuv420_software_get_handle_frame();
		if (NULL != handle_frame)
		{
			if (2 == SysCtrl.photo_mode_switch) // effect mode
			{
				yuv420_effect_handle(handle_frame->y_addr, handle_frame->uv_addr, handle_frame->w, handle_frame->h);
			}
			else if (3 == SysCtrl.photo_mode_switch) // frame mode
			{
				// photo frame add to lcd
				hal_custom_frame_add_lcd(handle_frame->y_addr);
			}
			else // face det handle
			{
			}

			SysCtrl.photo_software_handle = 2; // software handle finish
			hal_lcdDealBuffer(handle_frame);
			hal_lcd_get_next_frame();
		}
		else
		{
			deg_Printf("null\n");
		}
	}
}
void taskPhotoEncodeClose(uint32 arg)
{
	SysCtrl.photo_task = 0;
	SysCtrl.photo_mode_switch = 0;
	SysCtrl.photo_mode_idx = 0;

	//==cancal frame===
	videoRecordCmdSet(CMD_COM_FRAME, 0);
	hal_custom_frame_add_enable(0);
	//==normal color===
	SysCtrl.spec_color_index = 0;
	cmos_spec_color(SysCtrl.spec_color_index);
	//==close effect===

	if (0 == SysCtrl.photo_software_handle)
	{
		SysCtrl.photo_software_handle = 1;
		disp_frame_t *handle_frame = yuv420_software_get_handle_frame();
		if (NULL != handle_frame)
		{
			SysCtrl.photo_software_handle = 2; // effect handle finish
			hal_lcdDealBuffer(handle_frame);
			hal_lcd_get_next_frame();
		}
		else
		{
			deg_Printf("null\n");
		}
	}

	dispLayerSetPIPMode(SysCtrl.pip_mode);
#if FILE_SYNC_DELETE > 0
	while (f_unlink_ext_service())
		;
#endif

	dispLayerUninit(DISP_LAYER_VIDEO);
	imageEncodeUninit();
	videoRecordUninit();
	if (SysCtrl.sdcard == SDC_STAT_FULL)
	{
		SysCtrl.sdcard = SDC_STAT_NORMAL;
	}

	hal_custom_frame_unit();
	deg_Printf("-------------------have mem end %d-----------------\n", hal_sysMemRemain());
	deg_Printf("image encode task exit.\n");
}
sysTask taskPhotoEncode =
	{
		"Photo Encode",
		(u32)NULL,
		taskPhotoEncodeOpen,
		taskPhotoEncodeClose,
		taskPhotoEncodeService,
};
