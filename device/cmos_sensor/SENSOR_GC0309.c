#include "cmos_sensor.h"

#if DEV_SENSOR_GC0309 > 0

SENSOR_INIT_SECTION const unsigned char GC0309InitTable[]=
{
/*
		0x01,// [7:0]:driver type,0x01:sensor whith ISP; 0x02:ISP; 0x03:sensor whithout isp;	
			
		0x01,0x6E,0x36,0x00,// [7:0]:XCLK
		0x00,0x00,0x9C,0x40,// [7:0]:BAUDRATE
		
		0x00,//	[7:0]:chip_ID_addr>>8,when AddrByteNum == 2; 0 when  AddrByteNum == 1
		0x00,// [7:0]:chip_ID_addr
		0x00,//
		0xA0,// [7:0]:chip_ID 
		0x00,0x00,0x00,0x00,//resverd
		
		0x02,0x80,//image_width
		0x01,0xE0,//image_height
		0x43,// [7:0]:read ID
		0x42,// [7:0]:write ID
		0x01,//		[7:0]:AddrByteNum ; 
		0x01,// [7:0]:DataByteNum
		
		0x04,// [2:0]:ldo_config_para
		0x01,// [7:0]:ID nums
*/

		0xfe,0x80,  // soft reset
		0xfe,0x00,  // soft reset      // set page0
		0x1a,0x26,
		0xd2,0x10,  // close AEC
		0x22,0x55,  // close AWB
		0x5a,0x56,
		0x5b,0x40,
		0x5c,0x4a,
		0x22,0x57,
		//0x01,(u32)HBLANK_BASE & 0xff, //fa
		//0x02,(u32)VBLANK_BASE & 0xff, //70
		//0x0f,(((u32)VBLANK_BASE >> 8)<<4)|((u32)HBLANK_BASE >> 8), //01
		0xe2,0x00,
		0xe3,0x64,
		0x03,0x01,
		0x04,0x2c,
		0xe4,0x02,
		0xe5,0x8f,//58
		0xe6,0x02,
		0xe7,0x8f,//58
		0xe8,0x02,
		0xe9,0x8f,//58
		0xea,0x05,
		0xeb,0xdc,
		0x05,0x00,
		0x06,0x00,
		0x07,0x00,
		0x08,0x00,
		0x09,0x01,
		0x0a,0xe8,
		0x0b,0x02,
		0x0c,0x88,
		0x0d,0x02,
		0x0e,0x02,
		0x10,0x26,
		0x11,0x0d,
		0x12,0x2a,
		0x13,0x00,
		0x14,0x13,  //10	//180'
		0x15,0x0a,
		0x16,0x05,
		0x17,0x01,
		0x1b,0x03,
		0x1c,0x49,
		0x1d,0x98,
		0x1e,0x20,
		0x1f,0x16,
		0x20,0xff,
		0x21,0xf8,
		0x24,0xa2,
		0x25,0x0f,
		0x26,0x03, //output sync_mode
		0x2f,0x01,
		0x30,0xf7, //	 grab_t
		0x31,0x60,//60
		0x32,0x00,
		0x39,0x04,
		0x3a,0x20,
		0x3b,0x20,
		0x3c,0x02,
		0x3d,0x02,
		0x3e,0x02,
		0x3f,0x02,
		0x50,0x24,	//gain
		0x53,0x82,
		0x54,0x80,
		0x55,0x80,
		0x56,0x82,
		0x8b,0x22,//0x20// LSC_t 
		0x8c,0x20,
		0x8d,0x20,
		0x8e,0x10,
		0x8f,0x10,
		0x90,0x10,
		0x91,0x3c,
		0x92,0x50,
		0x5d,0x12,
		0x5e,0x1a,
		0x5f,0x24,
		0x60,0x07,// DNDD_t 
		0x61,0x0e,
		0x62,0x0c,
		0x64,0x03,
		0x66,0xe8,
		0x67,0x86,
		0x68,0xa2,
		0x69,0x20,// asde_t
		0x6a,0x0f,
		0x6b,0x00,
		0x6c,0x53,
		0x6d,0x83,
		0x6e,0xac,
		0x6f,0xac,
		0x70,0x15,
		0x71,0x43,//0x33
		0x72,0xdc,// eeintp_t
		0x73,0x80,
		0x74,0x02,	//for high resolution in light scene
		0x75,0x3f,
		0x76,0x02,
		0x77,0x54,
		0x78,0x88,
		0x79,0x81,
		0x7a,0x81,
		0x7b,0x22,
		0x7c,0xff,
		0x93,0x48,//45 //CC_t
		0x94,0x00,
		0x95,0x05,//0x00
		0x96,0xe8,//0x00
		0x97,0x40,//0x45
		0x98,0xf8,//0xf0
		0x9c,0x00,//r
		0x9d,0x00,//g
		0x9e,0x00,//b
		0xb1,0x40,//YCP_t
		0xb2,0x40,
		0xb8,0x20,
		0xbe,0x36,
		0xbf,0x00,
		0xd0,0xcb, //c9//AEC_t
		0xd1,0x10,
		0xd3,0x50,	
		0xd5,0xf2,
		0xd6,0x16,
		0xdb,0x92,
		0xdc,0xa5,
		0xdf,0x23,
		0xd9,0x00,
		0xda,0x00,
		0xe0,0x09,
		0xec,0x20,
		0xed,0x04,
		0xee,0xa0,
		0xef,0x40,
		0x9F, 0x0E,//GAMMA
		0xA0, 0x1C,
		0xA1, 0x34,
		0xA2, 0x48,
		0xA3, 0x5A,
		0xA4, 0x6B,
		0xA5, 0x7B,
		0xA6, 0x95,
		0xA7, 0xAB,
		0xA8, 0xBF,
		0xA9, 0xCE,
		0xAA, 0xD9,
		0xAB, 0xE4,
		0xAC, 0xEC,
		0xAD, 0xF7,
		0xAE, 0xFD,
		0xAF, 0xFF,
		0xc0,0x00,		//Y_gamma
		0xc1,0x0B,
		0xc2,0x15,
		0xc3,0x27,
		0xc4,0x39,
		0xc5,0x49,
		0xc6,0x5A,
		0xc7,0x6A,
		0xc8,0x89,
		0xc9,0xA8,
		0xca,0xC6,
		0xcb,0xE3,
		0xcc,0xFF,
		0xf0,0x02,// ABS_t
		0xf1,0x01,
		0xf2,0x00,
		0xf3,0x30,
		0xf7,0x04,// Measure Window
		0xf8,0x02,
		0xf9,0x9f,
		0xfa,0x78,
		0xfe,0x01,
		0x00,0xf5,//AWB_p
		0x02,0x1a,
		0x0a,0xa0,
		0x0b,0x60,
		0x0c,0x08,
		0x0e,0x4c,
		0x0f,0x39,
		0x11,0x3f,
		0x12,0x72,
		0x13,0x13,
		0x14,0x42,
		0x15,0x43,
		0x16,0xc2,
		0x17,0xa8,
		0x18,0x18,
		0x19,0x40,
		0x1a,0xd0,
		0x1b,0xf5,
		0x70,0x40,
		0x71,0x58,
		0x72,0x30,
		0x73,0x48,
		0x74,0x20,
		0x75,0x60,
		0xfe,0x00,
		0xd2,0x90, // Open AEC at last.
-1,-1};

static void GC0309_rotate(u32 r)
{
	unsigned char buf[2];
	buf[0] = 0x14;		//0',180'
	buf[1] = 0x10|(r<<0);
	//	sensor_iic_enable();
	//	sensor_iic_info();
	sensor_iic_write(buf);
	// sensor_iic_disable();

}

static void GC0309_hvblank(u32 h,u32 v)
{

	u32 i;
	const u8 t[][2] = {
		{0x01,h & 0xff},//0x6a//0xde //HB[7:0]
		{0x02,v & 0xff},  //0x0c //VB[7:0]
		{0x0f,((v >> 8)<<4)|(h >> 8)} //{0x0f,0x00}, //VB/HB [11:8]
	};
	//	sensor_iic_enable();
	//	sensor_iic_info();	
	for(i=0;i<3;i++)
		sensor_iic_write((u8 *)&t[i][0]);
	// sensor_iic_disable();

}

SENSOR_OP_SECTION const Sensor_Op_t gc0309_cmd= 
{ 
	.typ = CSI_TYPE_YUV422,
	.pixelw = 640,
	.pixelh= 480,
	.hsyn = 1,
	.vsyn = 1,
	.AVDD = SYS_VOL_V2_7,
	.DVDD = SYS_VOL_V1_5,
	.VDDIO = SYS_VOL_V2_7,
	.colrarray = 2,//0:_RGRG_ 1:_GRGR_,2:_BGBG_,3:_GBGB_
	.rotate_adapt = {0},
	.hvb_adapt = {42000000,1120,0,0,0xff,25,0},
	.mclk = 24000000,
	.p_fun_adapt = {NULL,NULL,NULL},
};

SENSOR_HEADER_ITEM_SECTION const Sensor_Init_t gc0309_init=
{
	.sensor_struct_addr =	(u32 *)&gc0309_cmd, 
	.sensor_struct_size = sizeof(Sensor_Op_t),
	.sensor_init_tab_adr =	(u32 *)GC0309InitTable,     
	.sensor_init_tab_size = sizeof(GC0309InitTable),
	.id               =	0xa0,               
	.w_cmd            =	0x42,               
	.r_cmd            =	0x43,               
	.addr_num         =	0x01,               
	.data_num         =	0x01,               
	.id_reg           =	0x00                
};

#endif

