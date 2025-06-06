#include "cmos_sensor.h"

#if DEV_SENSOR_BF20A6 > 0

SENSOR_INIT_SECTION const u8 BF20a6InitTable[] =
	{
#if 0
	0xf2,0x01,
	0x12,0x20,
	0x3a,0x00,
	0xe1,0x92,
	0xe3,0x02,
	0xe0,0x00,
	0x2a,0x98,
	0x0e,0x47,
	0x0f,0x60,
	0x10,0x57,
	0x11,0x60,
	0x30,0x61,
	0x62,0xcd,
	0x63,0x1a,
	0x64,0x38,
	0x65,0x52,
	0x66,0x68,
	0x67,0xc2,
	0x68,0xa7,
	0x69,0xab,
	0x6a,0xad,
	0x6b,0xa9,
	0x6c,0xc4,
	0x6d,0xc5,
	0x6f,0x04,
	0x6e,0x18,
	0xc0,0x20,
	0xc1,0x24,
	0xc2,0x29,
	0xc3,0x25,
	0xc4,0x28,
	0xc5,0x2a,
	0xc6,0x41,
	0xca,0x23,
	0xcd,0x34,
	0xce,0x32,
	0xcf,0x35,
	0xd0,0x6c,
	0xd1,0x6e,
	0xd2,0xcb,
	0xe2,0x09,
	0xe4,0x73,
	0xe5,0x22,
	0xe6,0x24,
	0xe7,0x64,
	0xe8,0xa2,//DVP:a2;  SPI:f2        VDDIO=1.8V,E8[2]=1;VDDIO=2.8V,E8[2]=0;
	0x4a,0x00,
	0x00,0x03,
	0x1f,0x02,
	0x22,0x02,
	0x0c,0x31,
	0x00,0x00,
	0x60,0x81,
	0x61,0x81,
	0xa0,0x08,
	0x01,0x1a,
	0x01,0x1a,
	0x01,0x1a,
	0x02,0x15,
	0x02,0x15,
	0x02,0x1a,
	0x13,0x08,
	0x8a,0x96,
	0x8b,0x06,
	0x87,0x18,


	0x34,0x48,//lens
	0x35,0x40,
	0x36,0x40,

	0x71,0x44,
	0x72,0x3a,//48 ddc
	0x74,0xa2,
	0x75,0xa9,
	0x78,0x12,//12  eed   54
	0x79,0xa0,
	0x7a,0x94,
	0x7c,0x97,
	0x40,0x30,
	0x41,0x30,
	0x42,0x28,
	0x43,0x1f,
	0x44,0x1c,
	0x45,0x16,
	0x46,0x13,
	0x47,0x10,
	0x48,0x0D,
	0x49,0x0C,
	0x4B,0x0A,
	0x4C,0x0B,
	0x4E,0x09,
	0x4F,0x08,
	0x50,0x08,


	0x5f,0x29,
	0x23,0x33,
	0xa1,0x10,//AWB
	0xa2,0x0d,
	0xa3,0x30,
	0xa4,0x06,
	0xa5,0x22,
	0xa6,0x56,
	0xa7,0x18,
	0xa8,0x1a,
	0xa9,0x12,
	0xaa,0x12,
	0xab,0x16,
	0xac,0xb1,
	0xba,0x12,
	0xbb,0x12,
	0xad,0x12,
	0xae,0x56,
	0xaf,0x0a,
	0x3b,0x30,
	0x3c,0x12,
	0x3d,0x22,
	0x3e,0x3f,
	0x3f,0x28,
	0xb8,0xc3,
	0xb9,0xA3,
	0x39,0x47,//pure color threshold
	0x26,0x13,
	0x27,0x16,
	0x28,0x14,
	0x29,0x18,
	0xee,0x0d,

	
	0x13,0x05,//05
	0x24,0x48,
	0x81,0x20,
	0x82,0x40,
	0x83,0x30,
	0x84,0x58,
	0x85,0x30,
	0x92,0x08,
	0x86,0xA0,
	0x8a,0x96,
	0x91,0xff,
	0x94,0x62,
	0x9a,0x18,//outdoor threshold
	0xf0,0x4e,
	0x51,0x17,//color normal
	0x52,0x03,
	0x53,0x5F,
	0x54,0x47,
	0x55,0x66,
	0x56,0x0F,
	0x7e,0x14,
	0x57,0x36,//A光color
	0x58,0x2A,
	0x59,0xAA,
	0x5a,0xA8,
	0x5b,0x43,
	0x5c,0x10,
	0x5d,0x00,
	0x7d,0x36,
	0x5e,0x10,

	0xd6,0x88,//contrast  88
	0xd5,0x20,//低光加亮度
	0xb0,0x84,//灰色区域降饱和度
	0xb5,0x08,//低光降饱和度阈值
	0xb1,0xc8,//saturation
	0xb2,0xc0,
	0xb3,0xd0,
	0xb4,0xB0,



	0x32,0x00,



	0xa0,0x09,

	0x00,0x03,

	0x0b,0x02,
#else
		0xf2, 0x01,
		0x12, 0x20,
		0x3a, 0x00,
		0xe1, 0x92,
		0xe3, 0x02,
		0xe0, 0x00,
		0x2a, 0x98,

		0x0e, 0x47,
		0x0f, 0x60,
		0x10, 0x57,
		0x11, 0x60,
		0x30, 0x61,
		0x62, 0xcd,
		0x63, 0x1a,
		0x64, 0x38,
		0x65, 0x52,
		0x66, 0x68,
		0x67, 0xc2,
		0x68, 0xa7,
		0x69, 0xab,
		0x6a, 0xad,
		0x6b, 0xa9,
		0x6c, 0xc4,
		0x6d, 0xc5,
		0x6e, 0x18,
		0xc0, 0x20,
		0xc1, 0x24,
		0xc2, 0x29,
		0xc3, 0x25,
		0xc4, 0x28,
		0xc5, 0x2a,
		0xc6, 0x41,
		0xca, 0x23,
		0xcd, 0x34,
		0xce, 0x32,
		0xcf, 0x35,
		0xd0, 0x6c,
		0xd1, 0x6e,
		0xd2, 0xcb,

		0xe2, 0x09,
		0xe4, 0x73, // 72->73 20220722
		0xe5, 0x22,
		0xe6, 0x24,
		0xe7, 0x64,
		0xe8, 0xa2, // VDDIO=1.8v 0xe8[2]=1,VDDIO=2.8v 0xe8[2]=0
		0x4a, 0x0C, // 0x0C,//flip&mirror 0x0C
		0x00, 0x03,
		0x1f, 0x02,
		0x22, 0x02,
		0x0c, 0x31,
		0x00, 0x00,
		0x60, 0x81,
		0x61, 0x81,
		0xa0, 0x08,
		0x01, 0x1a,
		0x01, 0x1a,
		0x01, 0x1a,
		0x02, 0x15,
		0x02, 0x15,
		0x02, 0x15,
		0x13, 0x08,
		0x8a, 0x85,
		0x8b, 0x06,
		0x87, 0x10, // 18
		0x34, 0x48, // lens
		0x35, 0x40,
		0x36, 0x40,
		0x70, 0x1f,
		0x71, 0x44,
		0x72, 0x52, // ddn [7:4]high en  [3:0]low en 56
		0x73, 0x6b,
		0x74, 0x6a,
		0x75, 0xa9,
		0x78, 0x64, // 0x23,//ee
		0x79, 0xa0,
		0x7a, 0x94,
		0x7c, 0x97,
		0x40, 0x20, // 清晰
		0x41, 0x30,
		0x42, 0x30,
		0x43, 0x28,
		0x44, 0x22,
		0x45, 0x1a,
		0x46, 0x13,
		0x47, 0x10,
		0x48, 0x0d,
		0x49, 0x0a,
		0x4B, 0x09,
		0x4C, 0x08,
		0x4E, 0x07,
		0x4F, 0x06,
		0x50, 0x04,

		/*0x40,0x30,//低噪
		0x41,0x28,
		0x42,0x20,
		0x43,0x1a,
		0x44,0x18,
		0x45,0x14,
		0x46,0x12,
		0x47,0x10,
		0x48,0x0f,
		0x49,0x0e,
		0x4B,0x0d,
		0x4C,0x0c,
		0x4E,0x0b,
		0x4F,0x0a,
		0x50,0x09,*/

		0x5f, 0x29,
		0x23, 0x00, // green 55 33
		0xa1, 0x10, // AWB
		// 0xa2,0x0b,//awb_b 0d
		// 0xa3,0x18,//23
		// 0xa4,0x07,//r
		// 0xa5,0x1e,//23

		0xa2, 0x0b, // awb_b 0d
		0xa3, 0x23, // 23
		0xa4, 0x07, // r
		0xa5, 0x1d, // 23
		0xa6, 0x5f,
		0xa7, 0x18,
		0xa8, 0x1a,
		0xa9, 0x12,
		0xaa, 0x12,
		0xab, 0x16,
		0xac, 0xb1,
		0xba, 0x12,
		0xbb, 0x12,
		0xad, 0x12,
		0xae, 0x56,
		0xaf, 0x0a,
		0x3b, 0x30,
		0x3c, 0x12,
		0x3d, 0x22,
		0x3e, 0x3f,
		0x3f, 0x28,
		0xb8, 0xc3,
		0xb9, 0xA3,
		0x39, 0x47, // pure color threshold
		0x26, 0x0b,
		0x27, 0x12,
		0x28, 0x09,
		0x29, 0x16,
		0xee, 0x08,
		0x13, 0x05,
		0x24, 0x40, // 42,//48
		0x81, 0x20,
		0x82, 0x40,
		0x83, 0x30,
		0x84, 0x40,
		0x85, 0x30,
		0x92, 0x08,
		0x86, 0x40, // 70  max3d
		0x91, 0xff,
		0x94, 0x62, // 42
		0x95, 0x88, // fd
		0x9a, 0x28,
		0x9e, 0xbc,
		0xf0, 0x4a,
		0x51, 0x1c, // 0c
		0x52, 0x07, // 05
		0x53, 0x73, // 3d
		0x54, 0x5e, // 47
		0x55, 0x81, // 86
		0x56, 0x17, // 26
		0x7e, 0x16, // 17
		0x57, 0x36, // A光color
		0x58, 0x2A,
		0x59, 0xAA,
		0x5a, 0xA8,
		0x5b, 0x43,
		0x5c, 0x10,
		0x5d, 0x00,
		0x7d, 0x36,
		0x5e, 0x10,
		0xd6, 0x80, // a0,//80
		0xd5, 0x20,
		0xb0, 0x24,
		0xb5, 0x0c,
		0xb1, 0xa0,
		0xb2, 0xa0,
		0xb3, 0xa0, // b0
		0xb4, 0xb0, // b0
		0x32, 0x10,
		0xa0, 0x09, // 09
		0x00, 0x03,
		0x0b, 0x02,
#endif
		0xff, 0xff};

SENSOR_OP_SECTION const Sensor_Op_t bf20a6_cmd =
	{
		.typ = CSI_TYPE_YUV422,
		.pixelw = 640,
		.pixelh = 480,
		.hsyn = 1,
		.vsyn = 1,
		.AVDD = SYS_VOL_V3_0,  // 模拟地电源 3.0
		.DVDD = SYS_VOL_V1_8,  // 数字地电源1.8
		.VDDIO = SYS_VOL_V3_0, // IO电源3.0
		.colrarray = 2,		   // 0:_RGRG_ 1:_GRGR_,2:_BGBG_,3:_GBGB_
		.hvb_adapt = {12000000, 500, 0, 0, 0xff, 25, 0},
		.mclk = 24000000,
		.p_fun_adapt = {NULL, NULL, NULL},
};
SENSOR_HEADER_ITEM_SECTION const Sensor_Init_t bf20a6_init =
	{
		.sensor_struct_addr = (u32 *)&bf20a6_cmd,
		.sensor_struct_size = sizeof(Sensor_Op_t),
		.sensor_init_tab_adr = (u32 *)BF20a6InitTable,
		.sensor_init_tab_size = sizeof(BF20a6InitTable),

		.id = 0xa6, // id值
		.w_cmd = 0xdc,
		.r_cmd = 0xdd,
		.addr_num = 0x01,
		.data_num = 0x01,
		.sensor_name = "BF20A6",
		.id_reg = 0xfd, // bf3016   // 寄存器的值 id值
};

#endif
