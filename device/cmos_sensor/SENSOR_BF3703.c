#include "cmos_sensor.h"

#if DEV_SENSOR_BF3703 >0


SENSOR_INIT_SECTION const unsigned char BF3703InitTable[]=
{
/*
	0x01,// [7:0]:driver type,0x01:sensor whith ISP; 0x02:ISP; 0x03:sensor whithout isp;	
	
	0x01,0x6E,0x36,0x00,// [7:0]:XCLK(BE)
	0x00,0x01,0x86,0xA0,// [7:0]:BAUDRATE(BE)
	
	0x00,//	[7:0]:chip_ID_addr>>8,when AddrByteNum == 2; 0 when  AddrByteNum == 1
	0xfc,// [7:0]:chip_ID_addr
	0x00,// [7:0]:chip_ID  
	0x37,//
	0x00,0x00,0x00,0x00,//resverd
	
	0x02,0x80,//image_width
	0x01,0xE0,//image_height
	0xdd,// [7:0]:read ID
	0xdc,// [7:0]:write ID
	0x01,//	[7:0]:AddrByteNum ; 
	0x01,// [7:0]:DataByteNum
	
	0x04,// [2:0]:ldo_config_para
	0x01,// [7:0]:ID nums
*/
0x15,0x22,//02
0x09,0x01,
0x13,0x00,
0x01,0x15,
0x02,0x21,
0x8c,0x01,
0x8d,0x32,
0x87,0x18,
0x13,0x07,
0x1f,0x20,
0x22,0x1d,
0x26,0x20,
0x1e,0x30,		//0x30 is 0' , 0x00 is 180' 
0xbb,0x20,
0x11,0x80,
//0x2a,0x00,//HBH[11:8]
//0x2b,0xff,//ff//HBL
//0x92,0x10,//VB
//0x93,0x00,//VB
//0xe3.0x00,//VB
//0xe4.0x00,//VB
0x35,0x38,
0x65,0x30,
0x66,0x30,
0x36,0x05,
0x37,0xf6,
0x38,0x46,
0x9b,0xf6,
0x9c,0x46,
0xbc,0x01,
0xbd,0xf6,
0xbe,0x46,
0x70,0xff,//70[1] should be set to 1'b1. 70[1]=1'b1:Bad pixel enable.
0x72,0x2f,//8f
0x73,0x2f,
0x74,0x27,
0x75,0x0e,
0x77,0x90,
0x7A,0x06,
0x2f,0xf6,
0x16,0x01,
0x80,0x41,
0x9d,0x73,//99
0x9e,0x7f,
0x24,0x8c,//0x90,//0x8c,//88
0x25,0x7c,//0x80,//0x7c,//78
0x94,0x0a,
0x56,0x46, //5b
0x3f,0xb8,
0x3b,0x60,
0x3c,0x18,
0x40,0x58,
0x41,0x54,
0x42,0x4e,
0x43,0x44,
0x44,0x3e,
0x45,0x39,
0x46,0x35,
0x47,0x31,
0x48,0x2e,
0x49,0x2b,
0x4b,0x29,
0x4c,0x27,
0x4e,0x23,
0x4f,0x20,
0x50,0x1e,
0x51,0x19,//0f
0x52,0x21,//24
0x53,0x0c,//07
0x54,0x0f,//17
0x57,0x76,//73
0x58,0x85,//82
0x59,0x7b,//88
0x5a,0xa6,//a3
0x5b,0x2b,//33
0x5D,0x9d,//9d
0x5C,0x0e,//0e
0x60,0x31,//20
0xb0,0xf0,//e0
0xb1,0xc8,//c0
0xb2,0xd8,//b0
0xb3,0x9a,//8a
0x82,0x14,
0x83,0x23,
0x84,0x1a,
0x85,0x20,
0x86,0x2c,//2b
0x7b,0x2b,
0x9a,0x23,
0x98,0x18,
0x96,0xa6,
0x97,0x0c,
0x89,0x01,
0x8a,0xcb,
0x8b,0x03,
0x6a,0x81,
0x23,0x66,//66
0xa0,0x03,
0x06,0xe0,
0xa1,0X31,
0xa2,0X0e,
0xa3,0X25,
0xa4,0X0d,
0xa5,0x26,//24
0xa6,0x08,
0xa7,0x7e,
0xa8,0x80,//7b
0xa9,0x1d,
0xaa,0x20,
0xab,0x1d,
0xac,0x30,
0xad,0xf0,
0xc8,0x16,
0xc9,0x22,
0xca,0x15,
0xcb,0x21,
0xaf,0x00,//16
0xc5,0x16,//16
0xc6,0x00,
0xc7,0x22,
0xae,0x83,
0xcc,0x60,
0xcd,0x60,
0xee,0x3c,
0x8e,0x02,
0x8f,0x40,
-1,-1};

static void BF3703_rotate(u32 r)
{
	unsigned char buf[2];
	buf[0] = 0x1e;
	//buf[1] = Sensor_ReadRegister(buf,u8AddrLength,u8DataLength);
	//buf[1] &=~(3<<4);
	buf[1] = (r<<4)|0x00;
	sensor_iic_write(buf);
}

static void BF3703_hvblank(u32 h,u32 v)
{
	u32 i;
	const u8 t[][2] = {
		{0x2a,(h >> 8) << 4 },
		{0x2b,h & 0xff},
		{0x92,v & 0xff},
		{0x93,v >> 8},
		{0xe3,0x00},
		{0xe4,0},
	};
	
	for(i=0;i<6;i++)
		sensor_iic_write((u8 *)&t[i][0]);
}

SENSOR_OP_SECTION const Sensor_Op_t bf3703_cmd=
{
	.typ = CSI_TYPE_YUV422,
	.pixelw = 640,
	.pixelh= 480,
//	.pixelw = 320,
//	.pixelh= 240,
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


SENSOR_HEADER_ITEM_SECTION const Sensor_Init_t bf3703_init =
{
	.sensor_struct_addr = (u32 *)&bf3703_cmd,   
	.sensor_struct_size = sizeof(Sensor_Op_t),
	.sensor_init_tab_adr =	(u32 *)BF3703InitTable,     
	.sensor_init_tab_size = sizeof(BF3703InitTable),
	.id               = 0x03,                 
	.w_cmd            = 0xdc,                 
	.r_cmd            = 0xdd,                 
	.addr_num         = 0x01,                 
	.data_num         = 0x01,                 
	.id_reg           = 0xfd,//bf3703         
};



#endif

