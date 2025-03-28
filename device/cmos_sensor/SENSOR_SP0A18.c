#include "cmos_sensor.h"

#if DEV_SENSOR_SP0A18 > 0

SENSOR_INIT_SECTION const unsigned char SP0A18InitTable[]=
{

	0xfd , 0x00,  
	0x1C , 0x00, 
	0x1B , 0x2f, 
	0x32 , 0x00, 
	0xfd , 0x01,  
	0x28 , 0xc4,                                      
	0x29 , 0x9e,  
	0xfd , 0x00, 
	0xe7 , 0x03, 
	0xe7 , 0x00, 
	0x0f , 0x3f,                                      
	0x10 , 0x0f,                                      
	0x13 , 0x3f,  //5f                                    
	0x15 , 0x0a, // 4f 
	0x16 , 0x00, 
	0x1a , 0x09,                                   
	0x44 , 0x0f,                                      
	0x22 , 0x13,                                      
	0x26 , 0x02,                                      
	0x27 , 0x68, //e8                                    
	0x28 , 0x12, //b                                    
	0x30 , 0x00,                                      
	0x31 , 0x70, //mirror  10 70                                                           
	0x34 , 0x00,         
	//special
	0xfd , 0x01,                                      
	0x00 , 0x01,  
	//pregain
	0xfd , 0x01,                                      
	0xc6 , 0x0a,//a~f  
	0xc7 , 0x0a,//a~f   
	0xc8 , 0x0a,//a~f   
	0xc9 , 0x0a,//a~f                                            
	//Del sunspot                                       
	0xfd , 0x00,                                      
	0xcd , 0x24,                                      
	0xd1 , 0x03,                                      
	0xd5 , 0xff,                                      
	0xfc , 0x03,                                      
	0xfb , 0x0f, //27                                                                              
	//Blacklevel                                        
	0xfd , 0x00,                                      
	0x65 , 0x00,//blue_suboffset                       
	0x66 , 0x00,//red_suboffset                        
	0x67 , 0x00,//gr_suboffset                         
	0x68 , 0x00,//gb_suboffset                                                                            
	//AWB                                               
	0xfd , 0x01,  
	0x10 , 0x0d,                                                                                                            
	0x11 , 0x0d,                                      
	0x12 , 0x0d,  
	0x17 , 0x27,                                    
	0x18 , 0x45,                                      
	0x1a , 0xac,                                      
	0x1b , 0x9e,                                      
	0x2a , 0xE0,                                      
	0x2b , 0x10,                                      
	0x2c , 0x9f,//90    
	0x2d , 0x99,
	0x2e , 0x99,
	0x6f , 0x05,                                      
	0x8b , 0x2f, 
	0x20 , 0xd0, 
	0x21 , 0x78, 
	0x22 , 0xe0, 
	0x23 , 0x80, 
	0x16 , 0x0d,
	//smooth   
#if 1//预览轮廓清晰
	0xfd , 0x00,                                      
	0x4f , 0x54,//raw_gf_fac_outdoor raw_gf_fac_nr 55  
	0x50 , 0x22,//draw_gf_fac_dummy raw_gf_fac_low     
	0x57 , 0x06,//raw_dif_thr_outdoor                  
	0x58 , 0x0f,//raw_dif_thr_normal                   
	0x56 , 0x10,//dummy                                
	0x59 , 0x10,//low                                                                                    
	0x5a , 0x70,//raw_rb_fac_outdoor,raw_rb_fac_nr     
	0x5b , 0x00,//raw_rb_fac_dummy,raw_rb_fac_low  
#else//预览噪点相对较少
	0xfd , 0x00,                                      
	0x4f , 0x43,//raw_gf_fac_outdoor raw_gf_fac_nr 55  
	0x50 , 0x11,//draw_gf_fac_dummy raw_gf_fac_low     
	0x57 , 0x06,//raw_dif_thr_outdoor                  
	0x58 , 0x16,//raw_dif_thr_normal                   
	0x56 , 0x18,//dummy                                
	0x59 , 0x18,//low                                                                                    
	0x5a , 0x70,//raw_rb_fac_outdoor,raw_rb_fac_nr     
	0x5b , 0x00,//raw_rb_fac_dummy,raw_rb_fac_low  
#endif
	//sharpen                                           
	0xfd , 0x01, 
	0xe5 , 0x20,
	0xe6 , 0x10,
	0xe7 , 0x20,  
	0xe8 , 0x38,//sharp_fac_pos_outdoor                
	0xec , 0x38,//sharp_fac_neg_outdoor                
	0xe9 , 0x30,//sharp_fac_pos_nr                     
	0xed , 0x30,//sharp_fac_neg_nr                     
	0xea , 0x30,//sharp_fac_pos_dummy                  
	0xef , 0x08,//sharp_fac_neg_dummy                  
	0xeb , 0x10,//sharp_fac_pos_low                    
	0xf0 , 0x20,//sharp_fac_neg_low                                                                  
	//rpc                                               
	0xfd , 0x00,                 
	0xe0 , 0x32,//rpc_1base_max
	0xe1 , 0x26,//rpc_2base_max
	0xe2 , 0x22,//rpc_3base_max
	0xe3 , 0x1e,//rpc_4base_max 
	0xe4 , 0x1e,//rpc_5base_max 
	0xe5 , 0x1d,//rpc_6base_max 
	0xe6 , 0x1d,//rpc_7base_max 
	0xe8 , 0x1b,//rpc_8base_max 
	0xe9 , 0x1b,//rpc_9base_max 
	0xea , 0x1b,//rpc_10base_max
	0xeb , 0x1a,//rpc_11base_max
	0xf5 , 0x1a,//rpc_12base_max
	0xf6 , 0x1a,//rpc_13base_max                                                       
	//ae min gain                                       
	0xfd , 0x01,                                                        
	0x95 , 0x1a,//rpc_min_indr                                        
	0x9d , 0x1a,//rpc_min_outdr   
	//ae setting       
	0xfd , 0x01,       
        0x94 , 0x70,//rpc_max_indr
        0x9c , 0x70,//rpc_max_outdr
	0xfd , 0x00,                                      
	0x05 , 0x00,                                      
	0x06 , 0x00,                                      
	0x07 , 0x00,                                      
	0x08 , 0x00,                                      
	0x09 , 0x01,                                      
	0x0A , 0x76,                                      
	0xf0 , 0x62,//ABF_exp_base_8lsb                    
	0xf1 , 0x00,//exp_min_base_msb                     
	0xfd , 0x01,                                      
	0x90 , 0x98,//exp_max_indr[7:0]                    
	0x91 , 0x04,//exp_max_indr[12:8]                   
	0x92 , 0x62,//exp_min_indr[7:0]                    
	0x93 , 0x00,//exp_min_indr[12:8]                   
	0x98 , 0x62,//exp_max_outdr[7:0]                   
	0x99 , 0x00,//exp_max_outdr[12:8]                  
	0x9a , 0x01,//exp_min_outdr[7:0]                   
	0x9b , 0x00,//exp_min_outdr[12:8]                                                                                                 
	0xfd , 0x01,                                      
	0xc4 , 0x70,//mean_nr_dummy                        
	0xc5 , 0x74,//mean_dummy_nr                        
	0xca , 0x30,//mean_heq_low                         
	0xcb , 0x45,//mean_heq_dummy                       
	0xcc , 0x70,//rpc_heq_low                          
	0xcd , 0x70,//rpc_heq_dummy                        
	0xce , 0x98,//exp_heq_dummy_8lsm                   
	0xcf , 0x04,//exp_heq_dummy_5hsm                   
	0xd0 , 0x98,//exp_heq_low_8lsm                     
	0xd1 , 0x04,//exp_heq_low_5hsm                                                                      
	//ae target                                         
	0xfd , 0x00,                                      
	0xed , 0x8c,//80,//90                                   
	0xf7 , 0x88,//7c,//8c                                   
	0xf8 , 0x80,//70,//84                                   
	0xec , 0x7c,//6c,//80                                                                                    
	0xef , 0x74,//99                                   
	0xf9 , 0x70,//90                                   
	0xfa , 0x68,//80                                   
	0xee , 0x64,//78                                                                                    
	/*// gamma 1 低端较暗                         
	0xfd , 0x00,  
	0x8b , 0x0 ,  
	0x8c , 0x6 ,  
	0x8d , 0xC ,  
	0x8e , 0x1A,  
	0x8f , 0x3A,  
	0x90 , 0x56,  
	0x91 , 0x70,  
	0x92 , 0x86,  
	0x93 , 0x99,  
	0x94 , 0xA9,  
	0x95 , 0xB5,  
	0x96 , 0xC0,  
	0x97 , 0xCA,  
	0x98 , 0xD4,  
	0x99 , 0xDD,  
	0x9a , 0xE6,  
	0x9b , 0xEF,  
	0xfd , 0x01,  
	0x8d , 0xF7,  
	0x8e , 0xFF,*/
	/*// gamma 2 低端稍暗                  
	0xfd , 0x00,  
	0x8b , 0x0 ,  
	0x8c , 0xA ,  
	0x8d , 0x13,  
	0x8e , 0x25,  
	0x8f , 0x43,  
	0x90 , 0x5D,  
	0x91 , 0x74,  
	0x92 , 0x88,  
	0x93 , 0x9A,  
	0x94 , 0xA9,  
	0x95 , 0xB5,  
	0x96 , 0xC0,  
	0x97 , 0xCA,  
	0x98 , 0xD4,  
	0x99 , 0xDD,  
	0x9a , 0xE6,  
	0x9b , 0xEF,  
	0xfd , 0x01,  
	0x8d , 0xF7,  
	0x8e , 0xFF,*/
	//gamma 3 推荐使用                       
	0xfd , 0x00,  
	0x8b , 0x0 ,  
	0x8c , 0xC ,  
	0x8d , 0x19,  
	0x8e , 0x2C,  
	0x8f , 0x49,  
	0x90 , 0x61,  
	0x91 , 0x77,  
	0x92 , 0x8A,  
	0x93 , 0x9B,  
	0x94 , 0xA9,  
	0x95 , 0xB5,  
	0x96 , 0xC0,  
	0x97 , 0xCA,  
	0x98 , 0xD4,  
	0x99 , 0xDD,  
	0x9a , 0xE6,  
	0x9b , 0xEF,  
	0xfd , 0x01,  
	0x8d , 0xF7,  
	0x8e , 0xFF,  
	/*//gamma 4 低端稍亮                       
	0xfd , 0x00,  
	0x8b , 0x0 ,  
	0x8c , 0x14,  
	0x8d , 0x22,  
	0x8e , 0x36,  
	0x8f , 0x51,  
	0x90 , 0x67,  
	0x91 , 0x7A,  
	0x92 , 0x8C,  
	0x93 , 0x9B,  
	0x94 , 0xA9,  
	0x95 , 0xB5,  
	0x96 , 0xC0,  
	0x97 , 0xCA,  
	0x98 , 0xD4,  
	0x99 , 0xDD,  
	0x9a , 0xE6,  
	0x9b , 0xEF,  
	0xfd , 0x01,  
	0x8d , 0xF7,  
	0x8e , 0xFF,*/
	/*//gamma 5 低光较亮                
	0xfd , 0x00,  
	0x8b , 0x0 ,  
	0x8c , 0x20,  
	0x8d , 0x2d,  
	0x8e , 0x40,  
	0x8f , 0x5a,  
	0x90 , 0x6e,  
	0x91 , 0x80,  
	0x92 , 0x8f,  
	0x93 , 0x9c,  
	0x94 , 0xa9,  
	0x95 , 0xb5,  
	0x96 , 0xc0,  
	0x97 , 0xca,  
	0x98 , 0xd4,  
	0x99 , 0xdd,  
	0x9a , 0xe6,  
	0x9b , 0xef,  
	0xfd , 0x01,  
	0x8d , 0xf7,  
	0x8e , 0xff,*/


	//Lens shading                                      
	0xfd , 0x00,                                      
	0xa0 , 0x10,//lsc_gr_gain                          
	0xa1 , 0x10,//lsc_gb_gain                          
	0xa2 , 0x14,//lsc_r_gain                           
	0xa3 , 0x10,//lsc_b_gain                           
	0xa4 , 0x14,//lsc_gain_max                         
	0xa5 , 0x80,//col_center                           
	0xa6 , 0x80,//row_center                                                                        
	//uv en                                             
	0xfd , 0x00,                                      
	0xb0 , 0x00,//uv_dif_gain_outdoor,uv_dif_gain_nr   
	0xb1 , 0x33,//uv_dif_gain_dummy,uv_dif_gain_low    
	//neg
	0xfd , 0x01,                                      
	0xd2 , 0x00,                                                   
	//lowlight lum                                      
	0xfd , 0x00,                                      
	0xb2 , 0x08,//lum_limit                            
	0xb3 , 0x1f,//lum_set                              
	0xb4 , 0x20,//black_vt                             
	0xb5 , 0x45,//white_vt                                                                              
	//auto sat                                          
	0xfd , 0x00,                                      
	0xbf , 0x01,//autosa_en                            
	0xc0 , 0xff,//y_p_th                               
	0xc1 , 0xff,//y_mean_th                                                                              
	//saturation                                        
	0xfd , 0x00,                                      
	0xd8 , 0x80,//74 //sat_uv_s1                            
	0xd9 , 0x80,//74 //sat_uv_s2                            
	0xda , 0x80,//74 //sat_uv_s3                            
	0xdb , 0x80,//74 //sat_uv_s4                                                                             
	//heq                                               
	0xfd , 0x00,                                      
	0xdc , 0x00,//heq_offset                           
	0xdd , 0x80,//ku                                   
	0xde , 0xa8,//90 //kl                                 
	0xdf , 0x70,//heq_mean                                                                               
	//CCM                                               
	0xfd , 0x01,                                      
	0xa0 , 0x80,//80(红色接近，肤色不理想)                                        
	0xa1 , 0x0 ,//0                                                               
	0xa2 , 0x0 ,//0                                                              
	0xa3 , 0xf3,//f0                                                              
	0xa4 , 0x8e,//a6                                                              
	0xa5 , 0x0 ,//ea                                                              
	0xa6 , 0x0 ,//0                                                               
	0xa7 , 0xe6,//e6                         
	0xa8 , 0x9a,//9a                        
	0xa9 , 0x0 ,//0                                                              
	0xaa , 0x3,//33                                                              
	0xab , 0xc ,//c                                                                                                      
	//LSC                                               
	0xfd , 0x00,                                      
	0x33 , 0xcf,                                                                                      
	//skin                                              
	0xfd , 0x01,                                                                                       
	0x8b , 0x2f,//bit7,0 en                            
	0x01 , 0xf0,                                      
	0x02 , 0x04,                                      
	0x03 , 0x70,                                      
	0x04 , 0x99,                                      
	0x05 , 0x24,                                                                                        
	//resize_en                                         
	0xfd , 0x01,                                      
	0x0e , 0x00,                                                                                                                                       
	//outformat                                         
	0xfd , 0x00,                                      
	0x35 , 0x40,                                      
	0x5f , 0x01,                                                                                        
	//En AE                                             
	0xfd , 0x00,                                      
	0x32 , 0x0d,                                      
	0x34 , 0x06,        
	0xf4 , 0x09, 
	
-1,-1};

static void SP0A18_rotate(u32 r)
{
	unsigned char buf[2];
	buf[0] = 0x14;		//0',180'
	buf[1] = 0x10|(r<<0);
	//	sensor_iic_enable();
	//	sensor_iic_info();
	sensor_iic_write(buf);
	// sensor_iic_disable();

}

static void SP0A18_hvblank(u32 h,u32 v)
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

SENSOR_OP_SECTION const Sensor_Op_t SP0A18_cmd= 
{ 
	.typ = CSI_TYPE_YUV422,
	.pixelw = 640,
	.pixelh= 480, 
	.hsyn = 1,
	.vsyn = 1,
	.AVDD = SYS_VOL_V3_0,
	.DVDD = SYS_VOL_V1_5,
	.VDDIO = SYS_VOL_V2_8,
	.colrarray = 2,//0:_RGRG_ 1:_GRGR_,2:_BGBG_,3:_GBGB_
	.rotate_adapt = {0},
	.hvb_adapt = {42000000,1120,0,0,0xff,20,0},
	
//	.hvb_adapt = {21000000,1120,0,0,0xff,20,0},
	.mclk = 24000000,
	.p_fun_adapt = {NULL,NULL,NULL},
};

SENSOR_HEADER_ITEM_SECTION const Sensor_Init_t SP0A18_init=
{
	.sensor_struct_addr =	(u32 *)&SP0A18_cmd, 
	.sensor_struct_size = sizeof(Sensor_Op_t),
	.sensor_init_tab_adr =	(u32 *)SP0A18InitTable,     
	.sensor_init_tab_size = sizeof(SP0A18InitTable),
	.id               =	0x0A,               
	.w_cmd            =	0x42,               
	.r_cmd            =	0x43,               
	.addr_num         =	0x01,               
	.data_num         =	0x01,               
	.id_reg           =	0x02                
};

#endif

