/****************************************************************************
**
**                              MULTIMEDIA
** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
**** **                  MULTIMEDIA IMAGE ENCODE
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : image_encode.c
* Author      : Mark.Douglas
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image encode file
*
* History     :
* 2016-09-22  :
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "../media.h"
#include "../../ax32_platform_demo/application.h"

#if MEDIA_CFG_IMAGE_ENCODE_EN > 0

//===EXIF format ===
#if (1 == JPEG_EXIF)

#define IFD0_DE_CNT 0xa
#define CHILD_IFD_DE_CNT 0x22

typedef struct
{
	u16 tag;
	u16 type; // 1:u8bit ,2:8bit,3:u16bit,4:u32bit,5:u64bits,6:s8bit,7:8bit,8:s16bit,9:s32bit,10:s64bit,11:float,12:double
	u16 count[2];
	u16 value_offset[2];
} EXIF_IFD0_DE;

typedef struct
{
	u16 soi;
	u16 app1;
	u16 app1_size;
	u16 flag[2];
	u16 flag_v;
	u16 byte_order;
	u16 bo_flag;
	u16 ifd0_offset[2];
	u16 ifd0_cnt;
	EXIF_IFD0_DE ifd0_de[IFD0_DE_CNT];
	u16 de_end[2];
	u8 de0_buf[0xC];
	u8 de1_buf[0xA];
	u16 de3_buf[4];
	u16 de4_buf[4];
	u8 de6_buf[0xc];
	u8 de7_buf[0x14];
	u16 child_ifd_cnt;
	EXIF_IFD0_DE child_de[CHILD_IFD_DE_CNT];
	u16 child_de_end[2];
	u16 child_de0_buf[4];
	u16 child_de1_buf[4];
	u8 child_de5_buf[0x14];
	u8 child_de6_buf[0x14];
	u16 child_de8_buf[4];
	u16 child_de9_buf[4];
	u16 child_de10_buf[4];
	u16 child_de11_buf[4];
	u16 child_de15_buf[4];
	u16 child_de16_buf[4];
	u16 child_de27_buf[4];
} EXIF_JPEG_HEAD;

extern INT32U configValue2Int(INT8U config_id);
int jpeg_exif_head_init(u8 *head_buf, u16 width, u16 height, u8 *datatime)
{
	u32 offset;
	u32 *temp;
	int res = 0;
	EXIF_JPEG_HEAD *jpeg_exif = (EXIF_JPEG_HEAD *)head_buf;
	//==init head==
	jpeg_exif->soi = 0xD8FF;
	jpeg_exif->app1 = 0xE1FF;
	jpeg_exif->app1_size = 0;
	jpeg_exif->flag[0] = 0x7845; // "Exif"
	jpeg_exif->flag[1] = 0x6669;
	jpeg_exif->flag_v = 0;
	jpeg_exif->byte_order = 0x4949; //"II"
	jpeg_exif->bo_flag = 0x2A;
	jpeg_exif->ifd0_offset[0] = 0x08;
	jpeg_exif->ifd0_cnt = IFD0_DE_CNT;
	jpeg_exif->de_end[0] = 0x0304;
	jpeg_exif->child_ifd_cnt = CHILD_IFD_DE_CNT;
	jpeg_exif->child_de_end[0] = 0x0;

	//==init de0==
	jpeg_exif->ifd0_de[0].tag = 0x010F; // make
	jpeg_exif->ifd0_de[0].type = 0x02;
	jpeg_exif->ifd0_de[0].count[0] = 0x0c;
	offset = sizeof(EXIF_IFD0_DE) * IFD0_DE_CNT + jpeg_exif->ifd0_offset[0] + 0x6;
	jpeg_exif->ifd0_de[0].value_offset[0] = offset & 0xffff;
	strcpy(jpeg_exif->de0_buf, "BuildWin");
	//==init de1==
	jpeg_exif->ifd0_de[1].tag = 0x0110; // model
	jpeg_exif->ifd0_de[1].type = 0x02;
	jpeg_exif->ifd0_de[1].count[0] = 0x0a;
	offset = jpeg_exif->ifd0_de[0].value_offset[0] + jpeg_exif->ifd0_de[0].count[0];
	jpeg_exif->ifd0_de[1].value_offset[0] = offset & 0xffff;
	strcpy(jpeg_exif->de1_buf, "HDV");
	//==init de2==
	jpeg_exif->ifd0_de[2].tag = 0x0112; // orientation
	jpeg_exif->ifd0_de[2].type = 0x03;
	jpeg_exif->ifd0_de[2].count[0] = 0x01;
	jpeg_exif->ifd0_de[2].value_offset[0] = 0x1;
	//==init de3==
	jpeg_exif->ifd0_de[3].tag = 0x011a; // x resolution
	jpeg_exif->ifd0_de[3].type = 0x05;
	jpeg_exif->ifd0_de[3].count[0] = 0x01;
	offset = jpeg_exif->ifd0_de[1].value_offset[0] + jpeg_exif->ifd0_de[1].count[0];
	jpeg_exif->ifd0_de[3].value_offset[0] = offset & 0xffff;
	jpeg_exif->de3_buf[0] = 0x48; // 72 dpi
	jpeg_exif->de3_buf[2] = 0x01;
	//==init de4==
	jpeg_exif->ifd0_de[4].tag = 0x011b; // y resolution
	jpeg_exif->ifd0_de[4].type = 0x05;
	jpeg_exif->ifd0_de[4].count[0] = 0x01;
	offset = jpeg_exif->ifd0_de[3].value_offset[0] + 0x8;
	jpeg_exif->ifd0_de[4].value_offset[0] = offset & 0xffff;
	jpeg_exif->de4_buf[0] = 0x48; // 72 dpi
	jpeg_exif->de4_buf[2] = 0x01;
	//==init de5==
	jpeg_exif->ifd0_de[5].tag = 0x0128; // resolution unit
	jpeg_exif->ifd0_de[5].type = 0x03;
	jpeg_exif->ifd0_de[5].count[0] = 0x01;
	jpeg_exif->ifd0_de[5].value_offset[0] = 0x02;
	//==init de6==
	jpeg_exif->ifd0_de[6].tag = 0x0131; // software
	jpeg_exif->ifd0_de[6].type = 0x02;
	jpeg_exif->ifd0_de[6].count[0] = 0x0c;
	offset = jpeg_exif->ifd0_de[4].value_offset[0] + 0x8;
	jpeg_exif->ifd0_de[6].value_offset[0] = offset & 0xffff;
	strcpy(jpeg_exif->de6_buf, "V233-00-01");
	//==init de7==
	jpeg_exif->ifd0_de[7].tag = 0x0132; // date time
	jpeg_exif->ifd0_de[7].type = 0x02;
	jpeg_exif->ifd0_de[7].count[0] = 0x14;
	offset = jpeg_exif->ifd0_de[6].value_offset[0] + jpeg_exif->ifd0_de[6].count[0];
	jpeg_exif->ifd0_de[7].value_offset[0] = offset & 0xffff;
	strcpy(jpeg_exif->de7_buf, datatime);
	//==init de8==
	jpeg_exif->ifd0_de[8].tag = 0x0213; // ycbcr positioning
	jpeg_exif->ifd0_de[8].type = 0x03;
	jpeg_exif->ifd0_de[8].count[0] = 0x01;
	jpeg_exif->ifd0_de[8].value_offset[0] = 0x02;
	//==init de9==
	jpeg_exif->ifd0_de[9].tag = 0x8769; // Exif child ifd offset
	jpeg_exif->ifd0_de[9].type = 0x04;
	jpeg_exif->ifd0_de[9].count[0] = 0x01;
	offset = jpeg_exif->ifd0_de[7].value_offset[0] + jpeg_exif->ifd0_de[7].count[0];
	jpeg_exif->ifd0_de[9].value_offset[0] = offset & 0xffff;
	//==init child de0==
	jpeg_exif->child_de[0].tag = 0x829a; // exposure time
	jpeg_exif->child_de[0].type = 0x05;
	jpeg_exif->child_de[0].count[0] = 0x01;
	offset = jpeg_exif->ifd0_de[9].value_offset[0] + sizeof(EXIF_IFD0_DE) * CHILD_IFD_DE_CNT + 0x6;
	jpeg_exif->child_de[0].value_offset[0] = offset & 0xffff;
	jpeg_exif->child_de0_buf[0] = 0x0a; // 1/99 s		(child_de0_buf[0]/child_de0_buf[2])
	jpeg_exif->child_de0_buf[2] = 0x3e1;
	//==init child de1==
	jpeg_exif->child_de[1].tag = 0x829D; // Fnumber
	jpeg_exif->child_de[1].type = 0x05;
	jpeg_exif->child_de[1].count[0] = 0x01;
	offset = jpeg_exif->child_de[0].value_offset[0] + 0x8;
	jpeg_exif->child_de[1].value_offset[0] = offset & 0xffff;
	jpeg_exif->child_de1_buf[0] = 0x20; // f/3.2		(child_de0_buf[0]/child_de0_buf[2])
	jpeg_exif->child_de1_buf[2] = 0x0a;
	//==init child de2==
	jpeg_exif->child_de[2].tag = 0x8822; // exposure program
	jpeg_exif->child_de[2].type = 0x03;
	jpeg_exif->child_de[2].count[0] = 0x01;
	jpeg_exif->child_de[2].value_offset[0] = 0x2;
	//==init child de3==
	jpeg_exif->child_de[3].tag = 0x8827; // ISO speedRatings
	jpeg_exif->child_de[3].type = 0x03;
	jpeg_exif->child_de[3].count[0] = 0x01;
	jpeg_exif->child_de[3].value_offset[0] = 0x64; // ISO-100
	//==init child de4==
	jpeg_exif->child_de[4].tag = 0x9000; // ExifVersion
	jpeg_exif->child_de[4].type = 0x07;
	jpeg_exif->child_de[4].count[0] = 0x04;
	strcpy(&jpeg_exif->child_de[4].value_offset[0], "02");
	strcpy(&jpeg_exif->child_de[4].value_offset[1], "20");
	//==init child de5==
	jpeg_exif->child_de[5].tag = 0x9003; // DateTimeOriginal
	jpeg_exif->child_de[5].type = 0x02;
	jpeg_exif->child_de[5].count[0] = 0x14;
	offset = jpeg_exif->child_de[1].value_offset[0] + 0x8;
	jpeg_exif->child_de[5].value_offset[0] = offset & 0xffff;
	strcpy(jpeg_exif->child_de5_buf, datatime);
	//==init child de6==
	jpeg_exif->child_de[6].tag = 0x9004; // DateTimeDigitized
	jpeg_exif->child_de[6].type = 0x02;
	jpeg_exif->child_de[6].count[0] = 0x14;
	offset = jpeg_exif->child_de[5].value_offset[0] + jpeg_exif->child_de[5].count[0];
	jpeg_exif->child_de[6].value_offset[0] = offset & 0xffff;
	strcpy(jpeg_exif->child_de6_buf, datatime);
	//==init child de7==
	jpeg_exif->child_de[7].tag = 0x9101; // Compornents Configuration
	jpeg_exif->child_de[7].type = 0x07;
	jpeg_exif->child_de[7].count[0] = 0x4;
	offset = 0x00030201;
	jpeg_exif->child_de[7].value_offset[0] = offset & 0xffff;
	jpeg_exif->child_de[7].value_offset[1] = (offset >> 16) & 0xffff;
	//==init child de8==
	jpeg_exif->child_de[8].tag = 0x9102; // Compressed bitsperpixel
	jpeg_exif->child_de[8].type = 0x05;
	jpeg_exif->child_de[8].count[0] = 0x01;
	offset = jpeg_exif->child_de[6].value_offset[0] + jpeg_exif->child_de[6].count[0];
	jpeg_exif->child_de[8].value_offset[0] = offset & 0xffff;
	jpeg_exif->child_de8_buf[0] = 0x02;
	jpeg_exif->child_de8_buf[2] = 0x01;
	//==init child de9==
	jpeg_exif->child_de[9].tag = 0x9203; // brightness value
	jpeg_exif->child_de[9].type = 0x0a;
	jpeg_exif->child_de[9].count[0] = 0x01;
	offset = jpeg_exif->child_de[8].value_offset[0] + 0x8;
	jpeg_exif->child_de[9].value_offset[0] = offset & 0xffff;
	offset = 0xFFFFEC78; // -5      ((int)0xFFFFEC78/(int)0x3E8)
	jpeg_exif->child_de9_buf[0] = offset & 0xffff;
	jpeg_exif->child_de9_buf[1] = (offset >> 16) & 0xffff;
	offset = 0x3E8;
	jpeg_exif->child_de9_buf[2] = offset & 0xffff;
	jpeg_exif->child_de9_buf[3] = (offset >> 16) & 0xffff;
	//==init child de10==
	jpeg_exif->child_de[10].tag = 0x9204; // exposure bias value
	jpeg_exif->child_de[10].type = 0x0a;
	jpeg_exif->child_de[10].count[0] = 0x01;
	offset = jpeg_exif->child_de[9].value_offset[0] + 0x8;
	jpeg_exif->child_de[10].value_offset[0] = offset & 0xffff;
	jpeg_exif->child_de10_buf[0] = 0x0a; // +1 bias      (*(int*)child_de10_buf[0]/*(int*)child_de10_buf[2])
	jpeg_exif->child_de10_buf[2] = 0x0a;
	//==init child de11==
	jpeg_exif->child_de[11].tag = 0x9205; // max aperture value
	jpeg_exif->child_de[11].type = 0x05;
	jpeg_exif->child_de[11].count[0] = 0x01;
	offset = jpeg_exif->child_de[10].value_offset[0] + 0x8;
	jpeg_exif->child_de[11].value_offset[0] = offset & 0xffff;
	jpeg_exif->child_de11_buf[0] = 0x3; // 1.5      (*(int*)child_de11_buf[0]/*(int*)child_de11_buf[2])
	jpeg_exif->child_de11_buf[2] = 0x2;
	//==init child de12==
	jpeg_exif->child_de[12].tag = 0x9207; // Metering mode
	jpeg_exif->child_de[12].type = 0x03;
	jpeg_exif->child_de[12].count[0] = 0x01;
	jpeg_exif->child_de[12].value_offset[0] = 0x04; // 0: unknow ,1: avd ,2: cent ,3:one point,4: more point ,5:more place , 6: apart, 255: other
	//==init child de13==
	jpeg_exif->child_de[13].tag = 0x9208; // light source
	jpeg_exif->child_de[13].type = 0x03;
	jpeg_exif->child_de[13].count[0] = 0x01;
	jpeg_exif->child_de[13].value_offset[0] = 0x0; // 0: unknow ,1:sun ,2:led ,3:tungsten,10:flash,17:stand A, 18: stand B, 18: stand C,255: other
	//==init child de14==
	jpeg_exif->child_de[14].tag = 0x9209; // flash
	jpeg_exif->child_de[14].type = 0x03;
	jpeg_exif->child_de[14].count[0] = 0x01;
	jpeg_exif->child_de[14].value_offset[0] = 0x0; // 0: unflash ,1:flash ,5:flash & unreflector 7:flash &reflector
	//==init child de15==
	jpeg_exif->child_de[15].tag = 0x920a; // focal length
	jpeg_exif->child_de[15].type = 0x05;
	jpeg_exif->child_de[15].count[0] = 0x01;
	offset = jpeg_exif->child_de[11].value_offset[0] + 0x8;
	jpeg_exif->child_de[15].value_offset[0] = offset & 0xffff;
	jpeg_exif->child_de15_buf[0] = 0x52; // 7mm      (*(int*)child_de15_buf[0]/*(int*)child_de15_buf[2])
	jpeg_exif->child_de15_buf[2] = 0x0B;
	//==init child de16==
	jpeg_exif->child_de[16].tag = 0x9214;
	jpeg_exif->child_de[16].type = 0x03;
	jpeg_exif->child_de[16].count[0] = 0x04;
	offset = jpeg_exif->child_de[15].value_offset[0] + 0x8;
	jpeg_exif->child_de[16].value_offset[0] = offset & 0xffff;
	jpeg_exif->child_de16_buf[0] = 0x510;
	jpeg_exif->child_de16_buf[1] = 0x3cc;
	jpeg_exif->child_de16_buf[2] = 0xa20;
	jpeg_exif->child_de16_buf[3] = 0x798;
	//==init child de17==
	jpeg_exif->child_de[17].tag = 0xa000; // flash pix version
	jpeg_exif->child_de[17].type = 0x07;
	jpeg_exif->child_de[17].count[0] = 0x04;
	strcpy(&jpeg_exif->child_de[17].value_offset[0], "01");
	strcpy(&jpeg_exif->child_de[17].value_offset[1], "00");
	//==init child de18==
	jpeg_exif->child_de[18].tag = 0xa001; // color space
	jpeg_exif->child_de[18].type = 0x03;
	jpeg_exif->child_de[18].count[0] = 0x01;
	jpeg_exif->child_de[18].value_offset[0] = 0x01; // 1:sRGB
	//==init child de19==
	jpeg_exif->child_de[19].tag = 0xa002; // exif image width
	jpeg_exif->child_de[19].type = 0x04;
	jpeg_exif->child_de[19].count[0] = 0x01;
	jpeg_exif->child_de[19].value_offset[0] = width;
	//==init child de20==
	jpeg_exif->child_de[20].tag = 0xa003; // exif image height
	jpeg_exif->child_de[20].type = 0x04;
	jpeg_exif->child_de[20].count[0] = 0x01;
	jpeg_exif->child_de[20].value_offset[0] = height;
	//==init child de21==
	jpeg_exif->child_de[21].tag = 0xa005; // exif interoperability offset
	jpeg_exif->child_de[21].type = 0x04;
	jpeg_exif->child_de[21].count[0] = 0x01;
	jpeg_exif->child_de[21].value_offset[0] = 0x2e6;
	//==init child de22==
	jpeg_exif->child_de[22].tag = 0xa300; // file source
	jpeg_exif->child_de[22].type = 0x07;
	jpeg_exif->child_de[22].count[0] = 0x01;
	jpeg_exif->child_de[22].value_offset[0] = 0x03;
	//==init child de23==
	jpeg_exif->child_de[23].tag = 0xa301; // scene type
	jpeg_exif->child_de[23].type = 0x07;
	jpeg_exif->child_de[23].count[0] = 0x01;
	jpeg_exif->child_de[23].value_offset[0] = 0x01;
	//==init child de24==
	jpeg_exif->child_de[24].tag = 0xa401; // custom rendered
	jpeg_exif->child_de[24].type = 0x03;
	jpeg_exif->child_de[24].count[0] = 0x01;
	jpeg_exif->child_de[24].value_offset[0] = 0x0; // 0:normal process ,1:custom process
	//==init child de25==
	jpeg_exif->child_de[25].tag = 0xa402; // exposure mode
	jpeg_exif->child_de[25].type = 0x03;
	jpeg_exif->child_de[25].count[0] = 0x01;
	jpeg_exif->child_de[25].value_offset[0] = 0x0; // 0:auto exposure ,1:manual exposure ,2:auto bracket
	//==init child de26==
	jpeg_exif->child_de[26].tag = 0xa403; // while balance mode
	jpeg_exif->child_de[26].type = 0x03;
	jpeg_exif->child_de[26].count[0] = 0x01;
	jpeg_exif->child_de[26].value_offset[0] = 0x0; // 0:auto white balance ,1:manual white balance
	//==init child de27==
	jpeg_exif->child_de[27].tag = 0xa404; // digital zoom ratio
	jpeg_exif->child_de[27].type = 0x05;
	jpeg_exif->child_de[27].count[0] = 0x01;
	offset = jpeg_exif->child_de[16].value_offset[0] + 0x8;
	jpeg_exif->child_de[27].value_offset[0] = offset & 0xffff;
	jpeg_exif->child_de27_buf[0] = 0x0; // 0 	(*(int*)child_de27_buf[0]/*(int*)child_de27_buf[2])
	jpeg_exif->child_de27_buf[2] = 0x1;
	//==init child de28==
	jpeg_exif->child_de[28].tag = 0xa405; // 35mm film equiv focal length
	jpeg_exif->child_de[28].type = 0x03;
	jpeg_exif->child_de[28].count[0] = 0x01;
	jpeg_exif->child_de[28].value_offset[0] = 0x0;
	//==init child de29==
	jpeg_exif->child_de[29].tag = 0xa406; // scene capture type
	jpeg_exif->child_de[29].type = 0x03;
	jpeg_exif->child_de[29].count[0] = 0x01;
	jpeg_exif->child_de[29].value_offset[0] = 0x0; // 0:standard 1: landscape 2: portrait 3:night scene
	//==init child de30==
	jpeg_exif->child_de[30].tag = 0xa407; // gain control
	jpeg_exif->child_de[30].type = 0x03;
	jpeg_exif->child_de[30].count[0] = 0x01;
	jpeg_exif->child_de[30].value_offset[0] = 0x0; // 0:none ,1:low gain up 2:high gain up 3:low gain down ,4: low gain down
	//==init child de31==
	jpeg_exif->child_de[31].tag = 0xa408; // contrast
	jpeg_exif->child_de[31].type = 0x03;
	jpeg_exif->child_de[31].count[0] = 0x01;
	jpeg_exif->child_de[31].value_offset[0] = 0x0; // 0:normal ,1:soft ,2:hard
	//==init child de32==
	jpeg_exif->child_de[32].tag = 0xa409; // saturation
	jpeg_exif->child_de[32].type = 0x03;
	jpeg_exif->child_de[32].count[0] = 0x01;
	jpeg_exif->child_de[32].value_offset[0] = 0x0; // 0:normal ,1:low saturation ,2:high saturation
	//==init child de33==
	jpeg_exif->child_de[33].tag = 0xa40a; // sharpness
	jpeg_exif->child_de[33].type = 0x03;
	jpeg_exif->child_de[33].count[0] = 0x01;
	jpeg_exif->child_de[33].value_offset[0] = 0x0; // 0:normal ,1:soft ,2:hard

	u16 app_size = (jpeg_exif->child_de[27].value_offset[0] + 0x8 + 0xA);
	jpeg_exif->app1_size = (app_size >> 8 | (app_size & 0xff) << 8);
	//	deg_Printf("jpeg_exif.app1_size=0x%x\n",jpeg_exif->app1_size);
	//	deg_Printf("sizeof(jpeg_exif)=0x%x,sizeof(ifd0_de)=0x%x,sizeof(jpeg_exif.child_de)=0x%x\n",sizeof(jpeg_exif),sizeof(jpeg_exif->ifd0_de),sizeof(jpeg_exif->child_de));

	return res;
}
#endif
//===end EXIF format===

extern void videoRecordImageWatermark(INT16U width, INT16U height, INT8U en);

/*******************************************************************************
 * Function Name  : imageEncodeInit
 * Description	 : image encode initial
 * Input 		 : none
 * Output		 : none
 * Return		 : int 0
 *******************************************************************************/
int imageEncodeInit(void)
{
	/*
		csi_isp_cfg();

	//	csi_frameend_isr_register(csi_lframe_done);
		csi_on();
	*/
	return 0;
}
/*******************************************************************************
 * Function Name  : imageEncodeUninit
 * Description	 : image encode uninitial
 * Input 		 : none
 * Output		 : none
 * Return		 : int 0 success
 *******************************************************************************/
int imageEncodeUninit(void)
{
	//	csi_off();
	return 0;
}
int hal_mjpegPhotoStart2(u16 win_w, u16 win_h, u8 quality, u8 timestramp, u8 frame_enable, u8 type1, u8 type2)
{
	hal_mjpegEncodeInit();
	hal_mjpeg_queue_init();
	hal_mjpeg_rev_buf_init();
	mjpegEncCtrl.frame_enable = frame_enable;
	mjpegEncCtrl.line_buf_using = 0;
#if CHANGE_4_3
	if (0)
#else
	if ((mjpegEncCtrl.csi_width > win_w) || (mjpegEncCtrl.csi_height > win_h))
#endif
	{
		mjpegEncCtrl.csi_to_mjpeg_dma = 1;
		mjpegEncCtrl.type = 1;
		mjpegEncCtrl.mjpeg_width = win_w;
		mjpegEncCtrl.mjpeg_height = win_h;

#if HAL_CFG_MJPEG_QULITY_AUTO > 0
		mjpegEncCtrl.qulity = hal_mjpegQualityCheck(quality);
#endif
		if (!ax32xx_hvScaler(mjpegEncCtrl.csi_width, mjpegEncCtrl.csi_height, win_w, win_h))
		{
			return -1;
		}
		ax32xx_csihv_tomjp_dma_cfg((u8 *)mjpegEncCtrl.ybuffer, (u8 *)mjpegEncCtrl.uvbuffer, win_h, win_w);

		hal_streamInit(&mjpegEncCtrl.vids, mjpegEncCtrl.mjpegNode, MJPEG_ITEM_NUM, (u32)mjpegEncCtrl.mjpbuf, mjpegEncCtrl.mjpsize);
		if (mjpegEncCtrl.curBuffer == NULL)
			mjpegEncCtrl.curBuffer = hal_streamMalloc(&mjpegEncCtrl.vids, mjpegEncCtrl.mjpsize);
		if (mjpegEncCtrl.curBuffer == 0)
			return -1;
		mjpegEncCtrl.curLen = mjpegEncCtrl.mjpsize;
		ax32xx_mjpegEncodeBufferSet(mjpegEncCtrl.curBuffer, mjpegEncCtrl.curBuffer + mjpegEncCtrl.curLen);

		ax32xx_mjpegEncodeInit(1, 0);
		ax32xx_mjpeg_inlinebuf_init((u8 *)mjpegEncCtrl.ybuffer, (u8 *)mjpegEncCtrl.uvbuffer);
		// ax32xx_mjpeg_inlinebuf_init((u8*)mjpeg_yuv_add,(u8*)&mjpeg_yuv_add[CSI_TO_MJPEG_SIZE]);
		ax32xx_mjpegEncodeSizeSet(mjpegEncCtrl.csi_width, mjpegEncCtrl.csi_height, mjpegEncCtrl.mjpeg_width, mjpegEncCtrl.mjpeg_height);
#if HAL_CFG_MJPEG_QULITY_AUTO > 0
		ax32xx_mjpegEncodeQuilitySet(mjpegEncCtrl.qulity);
#endif
		ax32xx_mjpegEncodeInfoSet(0);
		videoRecordImageWatermark(win_w, win_h, timestramp);

		ax32xx_csiISRRegiser(CSI_IRQ_JPG_FRAME_END, NULL);
		ax32xx_csiISRRegiser(CSI_IRQ_SEN_STATE_INT, NULL);
		ax32xx_csiISRRegiser(CSI_IRQ_VXY_FRAME_EN, (void *)hal_mjpeg_manual_on);
		ax32xx_mjpegEA_ncodeISRRegister(hal_mjpeg_manual_fdown);
		ax32xx_csiOutputSet(CSI_OUTPUT_WIFIEN, 1);
		ax32xx_csiEnable(1);
		mjpegEncCtrl.drop = 0;
	}
	else
	{
		mjpegEncCtrl.csi_to_mjpeg_dma = 2;
		mjpegEncCtrl.type = 1;
		mjpegEncCtrl.mjpeg_width = win_w;
		mjpegEncCtrl.mjpeg_height = win_h;
		quality = hal_mjpegQualityCheck(quality);

		// ax32xx_csiScaler(mjpegEncCtrl.csi_width,mjpegEncCtrl.csi_height, win_w, win_h,0,0,mjpegEncCtrl.csi_width, mjpegEncCtrl.csi_height);
		// ax32xx_csiMJPEGFrameSet(mjpegEncCtrl.ybuffer,mjpegEncCtrl.uvbuffer,HAL_CFG_MJPEG_HEIGHT,HAL_CFG_MJPEG_WIDTH);//mjpegEncCtrl.csi_height,mjpegEncCtrl.csi_width);
		ax32xx_mjpegEncodeInit(1, 0);
		// ax32xx_mjpegEncodeSizeSet(mjpegEncCtrl.csi_width,mjpegEncCtrl.csi_height, win_w, win_h);
		ax32xx_mjpeg_inlinebuf_init((u8 *)mjpegEncCtrl.ybuffer, (u8 *)mjpegEncCtrl.uvbuffer);
		// ax32xx_mjpeg_inlinebuf_init((u8*)mjpeg_yuv_add,(u8*)&mjpeg_yuv_add[CSI_TO_MJPEG_SIZE]);

		if (type2)
		{
			u16 csi_w, csi_h, crop_w, crop_h;
			u16 sx, sy, ex, ey, cropcsi_w, cropcsi_h;
			cropcsi_w = devSensorOp->pixelw;
			cropcsi_h = devSensorOp->pixelh;
			hal_csiResolutionGet(&csi_w, &csi_h);
#if CHANGE_4_3
#if 0
				    crop_w =  ((cropcsi_w * (100 - SysCtrl.crop_level*5/3) / 100) + 0x1f) & (~0x1f);
				    crop_h = (cropcsi_h * (100 - SysCtrl.crop_level*5/3) / 100) & ~1;
#else
			crop_w = cropcsi_w - SysCtrl.crop_level * 16;
			crop_h = cropcsi_h - SysCtrl.crop_level * 12;
#endif
#else
			crop_w = ((csi_w * (100 - SysCtrl.crop_level * 5 / 3) / 100) + 0x1f) & (~0x1f);
			crop_h = (csi_h * (100 - SysCtrl.crop_level * 5 / 3) / 100) & ~1;
#endif
			sx = (csi_w - crop_w) / 2;
			ex = sx + crop_w;
			sy = (csi_h - crop_h) / 2;
			ey = sy + crop_h;

			ax32xx_csiScaler(csi_w, csi_h, crop_w, crop_h, sx, sy, ex, ey);
			ax32xx_mjpegEncodeSizeSet(crop_w, crop_h, win_w, win_h);
			ax32xx_csiMJPEGFrameSet(mjpegEncCtrl.ybuffer, mjpegEncCtrl.uvbuffer, HAL_CFG_MJPEG_HEIGHT, crop_w);
			deg_Printf("fangda\r\n");
		}
		else
		{
#if CHANGE_4_3
			ax32xx_mjpegEncodeSizeSet(devSensorOp->pixelw, devSensorOp->pixelh, win_w, win_h);
#else
			ax32xx_mjpegEncodeSizeSet(mjpegEncCtrl.csi_width, mjpegEncCtrl.csi_height, win_w, win_h);
#endif
			ax32xx_csiMJPEGFrameSet(mjpegEncCtrl.ybuffer, mjpegEncCtrl.uvbuffer, win_h, win_w);
		}

		ax32xx_mjpegEncodeQuilitySet(quality);
		ax32xx_mjpegEncodeInfoSet(0);
		if ((type2 && !type1) || (!type1 && !type2))
			videoRecordImageWatermark(win_w, win_h, timestramp);

		if (type2)
		{
			hal_streamInit(&mjpegEncCtrl.vids, mjpegEncCtrl.mjpegNode, MJPEG_ITEM_NUM, (u32)mjpegEncCtrl.mjpbuf, mjpegEncCtrl.mjpsize);
			deg_Printf("the vids == 0x%x,the jpegNode == 0x%x mjpegEncCtrl.mjpsize:%d\r\n",
					   mjpegEncCtrl.vids, mjpegEncCtrl.mjpegNode, mjpegEncCtrl.mjpsize);
		}
		if (mjpegEncCtrl.curBuffer == NULL)
			mjpegEncCtrl.curBuffer = hal_streamMalloc(&mjpegEncCtrl.vids, mjpegEncCtrl.mjpsize);
		if (mjpegEncCtrl.curBuffer == 0)
			return -1;
		if (type1 == 0)
			mjpegEncCtrl.curLen = (50 * 1024); // mjpegEncCtrl.mjpsize;
		else
			mjpegEncCtrl.curLen = mjpegEncCtrl.mjpsize;
		deg_Printf("the curBuffer:%x sizeof:%x\r\n", mjpegEncCtrl.curBuffer, mjpegEncCtrl.curBuffer + mjpegEncCtrl.curLen);
		mjpegEncCtrl.photo_flag = 1;
		mjpegEncCtrl.yuvFlag = 0;
		mjpegEncCtrl.lastFrameLen = 0;
		SysCtrl.photo_finish_flag = 1;
		mjpegEncCtrl.photo_flag = 1;
		ax32xx_mjpegEncodeBufferSet(mjpegEncCtrl.curBuffer, mjpegEncCtrl.curBuffer + mjpegEncCtrl.curLen);
		if (type2)
			ax32xx_csiISRRegiser(CSI_IRQ_JPG_FRAME_END, hal_mjpeg_manual_quad_on);
		// ax32xx_csiISRRegiser(CSI_IRQ_JPG_FRAME_END,(void*)hal_mjpeg_manual_on);
		if (!type1 && !type2)
		{
			ax32xx_csiISRRegiser(CSI_IRQ_JPG_FRAME_END, (void *)NULL);
		}
		ax32xx_csiISRRegiser(CSI_IRQ_SEN_STATE_INT, (void *)NULL);
		ax32xx_csiISRRegiser(CSI_IRQ_VXY_FRAME_EN, NULL);
		if (type1 == 0)
		{
			// ax32xx_mjpegEA_ncodeISRRegister(hal_mjpeg_manual_fdown);
			ax32xx_mjpegEA_ncodeISRRegister(hal_mjpeg_manual_quad_fdown);
		}
		else
		{
			deg_Printf("gongbanbianmaliuchen\r\n");
			ax32xx_mjpegEA_ncodeISRRegister(hal_mjpeg_manual_fdown_mini); // gongbande s
		}

		if (!type1 && !type2)
		{
			ax32xx_mjpeg_manual_on();
			ax32xx_intEnable(IRQ_JPGA, 1); // enable jpegirq
		}
		else
		{
			ax32xx_csiOutputSet(CSI_OUTPUT_MJPGEN, 1);
			ax32xx_csiEnable(1);
		}
		mjpegEncCtrl.drop = 0;
	}

	// debg("PRE_JPCON1:%x,%x\n",quality,(PRE_JPCON1 >> 23)&0x0f);
	return 0;
}
/*******************************************************************************
* Function Name  : imageEncodeStart
* Description	 : take a photo
* Input 		 : FHANDLE fileHanle : file handle for write photo ram data to the file
				 : INT16U image_width : image width
				 : INT16U image_height:image height
				 : INT8U image_q qaulity,refence to __42__,__50__,__62__,__75__,__81__
				 : INT8U timestramp : time stramp.0->disable,
* Output		 : none
* Return		 : int : 0 success
								-1 fail
*******************************************************************************/

int imageEncodeStart(FHANDLE fileHanle, INT16U image_width, INT16U image_height, INT8U image_q, INT8U timestramp, INT8U frame_enable)
{
	INT32S res = 0;
	INT32U addr, size, timeout, delaytime, allsize;
	void *buff; //,*tbuff;
	s32 sync, sync_next;
	u8 flag = 0, flag2;
	u16 csi_w, csi_h;
	u32 len;
	u8 uCount = 0;
	INT8U err;
	JPEG_PHOTO_CACHE_T photo_cache;
	// boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,0);

	hal_mjpegEncodeResolutionImage(image_width, image_height);

	if (hal_mjpegMemInit(1) < 0)
	{
		watermark_buf_bmp2yuv_free();
		deg_Printf("image encode : memory malloc fail\n");
		res = -1;
		goto IMAGE_ENCODE_ERR;
	}
	// deg_Printf("SysCtrl.crop_level:%d mjpegEncCtrl.frame_enable:%d\r\n",SysCtrl.crop_level,mjpegEncCtrl.frame_enable);

#if CHANGE_4_3
	// deg_Printf("--------4  :   3-----------------------\r\n");
	hal_mjpegPhotoStart2(devSensorOp->pixelw, devSensorOp->pixelh, image_q, 0 /*timestramp*/, frame_enable, 1, 1); // didn't crop ,use orig
#else
	// if((SysCtrl.crop_level>0) && (frame_enable!=0))
	if (SysCtrl.crop_level > 0)
	{
		// deg_Printf("gongbanbianma\r\n");
		hal_mjpegPhotoStart(1280, devSensorOp->pixelh, image_q, 0 /*timestramp*/, frame_enable, 1, 1); // didn't crop ,use orig
	}
	else
	{
		// deg_Printf("yuanbenbianma\r\n");
		hal_mjpegPhotoStart(image_width, image_height, image_q, 0 /*timestramp*/, frame_enable, 0, 1); // didn't crop ,use orig
	}
#endif
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
	//==end wait csi yuv buf ok==
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

	// boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,1);

	//==software handle yuv buf ==
#if CHANGE_4_3
	// deg_Printf("datoutie  4:3\r\n");
#else
	if (0 == SysCtrl.crop_level)
	{
		hal_mjpeg_software_handle_csi_yuvbuf();
		if (timestramp)
		{
			watermark_bmp2yuv_draw((u8 *)mjpegEncCtrl.ybuffer, WATERMAKE_SET_X_POS, WATERMAKE_SET_Y_POS, WATER_CHAR_GAP, CHANGE_4_3);
			deg_Printf("add watermark finish \n");
		}
	}
#if 0 // yuv
	int fd1=open("txt.yuv", FA_CREATE_NEW | FA_WRITE | FA_READ);
	write(fd1,(void *)mjpegEncCtrl.ybuffer,1280*devSensorOp->pixelh*3/2);	
	close(fd1);
	deg_Printf("=====txt.yuv====\n");
	return 0;
#endif
#endif
	//==end software handle yuv buf ==

	//==wait jpg encode==
	ax32xx_mjpeg_manual_on();
	ax32xx_intEnable(IRQ_JPGA, 1); // enable jpegirq

	if (&mjpegEncCtrl.vids == NULL)
	{
		deg_Printf("no vids frame\r\n");
	}
	if (mjpegEncCtrl.curBuffer == NULL)
		deg_Printf("no curBuffer frame\r\n");

#if CHANGE_4_3
	if (1)
#else
	// if(0!=SysCtrl.crop_level && mjpegEncCtrl.frame_enable==1)
	if (0 != SysCtrl.crop_level)
#endif
	{
		// deg_Printf("get buff\r\n");
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

	} //==end wait jpg encode==
	else
	{
		timeout = XOSTimeGet();
		while (1)
		{
			photo_cache = *((JPEG_PHOTO_CACHE_T *)XMsgQPend(mjpegEncCtrl.photoCahQ, &err));

			if ((timeout + 5000) < XOSTimeGet())
			{
				res = -3;
				deg_Printf("5s timeout : error\n");
				break;
			}
			if (SysCtrl.photo_finish_flag == 2) // 异常处理
			{
				res = -2;
				break;
			}

			if (err != X_ERR_NONE) // no buffe
				continue;

			if (fileHanle >= 0)
			{
				res = write(fileHanle, (void *)photo_cache.mem, (UINT)photo_cache.len);

				if (res < 0)
				{
					deg_Printf("image encode : error\n");
					res = -1;
					break;
				}
			}
			if (photo_cache.flag)
			{
				res = 0;
				deg_Printf("image ok \n");
				break;
			}
			deg_Printf("the mem == 0x%x,the len == %d\n", photo_cache.mem, photo_cache.len);

			if (photo_cache.mem >= mjpegEncCtrl.mjpbuf && photo_cache.mem <= mjpegEncCtrl.mjpbuf + mjpegEncCtrl.mjpsize)
			{
				hal_mjpegRawDataSet(photo_cache.mem);
			}
			else
			{
				hal_mjpeg_rev_buf_free(photo_cache.mem);
			}
		}
	}

//-----
#if CHANGE_4_3
	if (1)
#else
	// if(0!=SysCtrl.crop_level && mjpegEncCtrl.frame_enable==1)
	if (0 != SysCtrl.crop_level)
#endif
	{
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
			watermark_bmp2yuv_draw((u8 *)mjpegEncCtrl.ybuffer, WATERMAKE_SET_X_POS, WATERMAKE_SET_Y_POS, WATER_CHAR_GAP, CHANGE_4_3);
			deg_Printf("add watermark finish \n");
		}
		hal_mjpegRawDataSet(buff);
		// deg_Printf("aisghoiesegosa\r\n");
	}
//-----decode hal_mjpegPhotoStart

//-----
#if CHANGE_4_3
	if (1)
#else
	// if(0!=SysCtrl.crop_level && mjpegEncCtrl.frame_enable==1)
	if (0 != SysCtrl.crop_level)
#endif
	{
		hal_mjpegPhotoStart2(image_width, image_height, image_q, 0 /*timestramp*/, frame_enable, 0, 0); // didn't crop ,use orig
		timeout = XOSTimeGet();
		while (1)
		{
			photo_cache = *((JPEG_PHOTO_CACHE_T *)XMsgQPend(mjpegEncCtrl.photoCahQ, &err));

			if ((timeout + 5000) < XOSTimeGet())
			{
				res = -3;
				deg_Printf("5s timeout : error\n");
				break;
			}
			if (SysCtrl.photo_finish_flag == 2) // 异常处理
			{
				res = -2;
				break;
			}

			if (err != X_ERR_NONE) // no buffe
				continue;

			if (fileHanle >= 0)
			{
				res = write(fileHanle, (void *)photo_cache.mem, (UINT)photo_cache.len);

				if (res < 0)
				{
					deg_Printf("image encode : error\n");
					res = -1;
					break;
				}
			}
			if (photo_cache.flag)
			{
				res = 0;
				deg_Printf("image ok \n");
				break;
			}
			deg_Printf("the mem == 0x%x,the len == %d\n", photo_cache.mem, photo_cache.len);

			if (photo_cache.mem >= mjpegEncCtrl.mjpbuf && photo_cache.mem <= mjpegEncCtrl.mjpbuf + mjpegEncCtrl.mjpsize)
			{
				hal_mjpegRawDataSet(photo_cache.mem);
			}
			else
			{
				hal_mjpeg_rev_buf_free(photo_cache.mem);
			}
		}
		//==end wait jpg encode==
	}

	//----- encode newphoto

	//==lcd image active==
	if (XOSTimeGet() > 200 + delaytime) // 200ms
	{
	}
	else
	{
		if (delaytime + 5 < XOSTimeGet())
		{
			XOSTimeDly(200 - (XOSTimeGet() - delaytime));
		}
		else
		{
			XOSTimeDly(200);
		}
	}
	hal_wdtClear();
	hal_csiEnable(1);
	dispLayerSetPIPMode(SysCtrl.pip_mode);
	hal_curBuffer_clear();

IMAGE_ENCODE_ERR:

	// boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,1);

	hal_mjpegEncodeStop();
	watermark_buf_bmp2yuv_free();
	if (timestramp)
		videoRecordImageWatermark(image_width, image_height, 0); // disable
	return res;
}
#endif
