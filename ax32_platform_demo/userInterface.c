#include "application.h"
#include "userInterface.h"

UserInterface changeUserInterface = {0};

void actionUpperToBottom(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
  	s32 divide;
	u16 lcd_w,lcd_h;
	u16 *dstUV, *srcUV;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	divide = lcd_h - lcd_h*cnt/ANIMATION_CNT;
	dstUV = (u16 *)(dst + lcd_w*lcd_h);
	srcUV = (u16 *)(src + lcd_w*lcd_h);
	
	for(j = 0; j < lcd_h; j++){
		for(i = 0; i < lcd_w; i++){
			if(j < divide){
				*(dst + j*lcd_w + i) = *(dst + (j + (lcd_h - divide))*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(dstUV + ((j + (lcd_h - divide)) >> 1)*(lcd_w >> 1) + (i >> 1));
			}
			else if(j >= divide + 3){
				*(dst + j*lcd_w + i) = *(src + (j - (divide + 3))*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(srcUV + ((j - (divide + 3)) >> 1)*(lcd_w >> 1) + (i >> 1));
			}
			else{
				*(dst + j*lcd_w + i) = 200;
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = 0xF06E;
			}
		}
	}
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);
}

void actionBottomToUpper(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
  	s32 divide;
	u16 lcd_w,lcd_h;
	u16 *dstUV, *srcUV;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	divide = lcd_h*cnt/ANIMATION_CNT;
	dstUV = (u16 *)(dst + lcd_w*lcd_h);
	srcUV = (u16 *)(src + lcd_w*lcd_h);
	
	for(j = lcd_h - 1; j < lcd_h; j--){
		for(i = 0; i < lcd_w; i++){
			if(j < divide){
				*(dst + j*lcd_w + i) = *(src + (j + (lcd_h - divide))*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(srcUV + ((j + (lcd_h - divide)) >> 1)*(lcd_w >> 1) + (i >> 1));
			}
			else if(j >= divide + 3){
				*(dst + j*lcd_w + i) = *(dst + (j - (divide + 3))*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(dstUV + ((j - (divide + 3)) >> 1)*(lcd_w >> 1) + (i >> 1));
			}
			else{
				*(dst + j*lcd_w + i) = 200;
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = 0xF06E;
			}
		}
	}
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);
}

void actionSquareInsideToOutside(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
  	s32 divide_w, divide_h;
	u16 lcd_w,lcd_h;
	u16 *dstUV, *srcUV;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	divide_h = lcd_h*cnt/ANIMATION_CNT >> 1;
	divide_w = lcd_w*cnt/ANIMATION_CNT >> 1;
	dstUV = (u16 *)(dst + lcd_w*lcd_h);
	srcUV = (u16 *)(src + lcd_w*lcd_h);
	
	for(j = lcd_h - 1; j < lcd_h; j--){
		for(i = 0; i < lcd_w; i++){
			if(((j < divide_h - 3) || (j > (lcd_h - divide_h + 3))) || ((i < divide_w - 3) || (i > (lcd_w - divide_w + 3)))){
				*(dst + j*lcd_w + i) = *(src + j*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(srcUV + (j >> 1)*(lcd_w >> 1) + (i >> 1));
			}else if(((j < divide_h) || (j > (lcd_h - divide_h))) || ((i < divide_w) || (i > (lcd_w - divide_w)))){
				*(dst + j*lcd_w + i) = 200;
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = 0xF06E;
			}
		}
	}
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);
}

void actionSquareOutsideToInside(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
  	s32 divide_w, divide_h;
	u16 lcd_w,lcd_h;
	u16 *dstUV, *srcUV;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	divide_h = (lcd_h - lcd_h*cnt/ANIMATION_CNT) >> 1;
	divide_w = (lcd_w - lcd_w*cnt/ANIMATION_CNT) >> 1;
	dstUV = (u16 *)(dst + lcd_w*lcd_h);
	srcUV = (u16 *)(src + lcd_w*lcd_h);
	
	for(j = lcd_h - 1; j < lcd_h; j--){
		for(i = 0; i < lcd_w; i++){
			if(((j > divide_h) && (j < (lcd_h - divide_h))) && ((i > divide_w) && (i < (lcd_w - divide_w)))){
				*(dst + j*lcd_w + i) = *(src + j*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(srcUV + (j >> 1)*(lcd_w >> 1) + (i >> 1));
			}else if(((j > divide_h - 3) && (j < (lcd_h - divide_h + 3))) && ((i > divide_w - 3) && (i < (lcd_w - divide_w + 3)))){
				*(dst + j*lcd_w + i) = 200;
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = 0xF06E;
			}
		}
	}
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);
}

void actionLeftBottomToRightUpper(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
  	s32 divide_w, divide_h;
	u16 lcd_w,lcd_h;
	u16 *dstUV, *srcUV;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	divide_h = (lcd_h*cnt/ANIMATION_CNT);
	divide_w = (lcd_w - lcd_w*cnt/ANIMATION_CNT);
	dstUV = (u16 *)(dst + lcd_w*lcd_h);
	srcUV = (u16 *)(src + lcd_w*lcd_h);
	
	for(j = 0; j < lcd_h; j++){
		for(i = 0; i < lcd_w; i++){
			if((j < divide_h) && (i > divide_w + 3)){
				*(dst + j*lcd_w + i) = *(src + (j + (lcd_h - divide_h))*lcd_w + (i - divide_w));
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(srcUV + ((j + (lcd_h - divide_h)) >> 1)*(lcd_w >> 1) + ((i - divide_w) >> 1));
			}else if((j < divide_h + 3) && (i > divide_w)){
				*(dst + j*lcd_w + i) = 200;
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = 0xF06E;
			}
		}
	}
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);
}

void actionLeftToRight(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
	s32 divide;
	u16 lcd_w,lcd_h;
	u16 *dstUV, *srcUV;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	divide =lcd_w-lcd_w*cnt/ANIMATION_CNT;//lcd_h - lcd_h*cnt/ANIMATION_CNT;
	dstUV = (u16 *)(dst + lcd_w*lcd_h);
	srcUV = (u16 *)(src + lcd_w*lcd_h);

	//deg_Printf("------------divide=%d---------------------.\n",divide);
	
	for(j = 0; j < lcd_w; j++)
	{
		for(i = 0; i < lcd_h; i++)
		{
			if(j < divide)
			{
				*(dst + j*lcd_w + i) = *(dst + (j + (lcd_h - divide))*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(dstUV + ((j + (lcd_h - divide)) >> 1)*(lcd_w >> 1) + (i >> 1));
			}
			else if(j >= divide + 3)
			{
				*(dst + j*lcd_w + i) = *(src + (j - (divide + 3))*lcd_w + i);
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(srcUV + ((j - (divide + 3)) >> 1)*(lcd_w >> 1) + (i >> 1));
			}
			else
			{
				*(dst + j*lcd_w + i) = 200;
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = 0xF06E;
			}
		}
	}
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);
}

void actionLeftUpperToRightBottom(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
  	s32 divide_w, divide_h;
	u16 lcd_w,lcd_h;
	u16 *dstUV, *srcUV;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	divide_h = (lcd_h - lcd_h*cnt/ANIMATION_CNT);
	divide_w = (lcd_w - lcd_w*cnt/ANIMATION_CNT);
	dstUV = (u16 *)(dst + lcd_w*lcd_h);
	srcUV = (u16 *)(src + lcd_w*lcd_h);
	
	for(j = 0; j < lcd_h; j++){
		for(i = 0; i < lcd_w; i++){
			if((j > divide_h + 3) && (i > divide_w + 3)){
				*(dst + j*lcd_w + i) = *(src + (j - divide_h)*lcd_w + (i - divide_w));
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = *(srcUV + ((j - divide_h) >> 1)*(lcd_w >> 1) + ((i - divide_w) >> 1));
			}else if((j > divide_h) && (i > divide_w)){
				*(dst + j*lcd_w + i) = 200;
				*(dstUV + (j >> 1)*(lcd_w >> 1) + (i >> 1)) = 0xF06E;
			}
		}
	}
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);
}


#define TFT_WIDTH		320
#define TFT_HEIGHT		240
#define GET_16BIT_DATA(addr)		(*((INT16U *)addr))

void actionCircleInside2OutSide(u8 *dst, u8 *src, u8 cnt)
{
	u32 i, j;
  	s32 divide_w, divide_h;
	u16 lcd_w,lcd_h;
	u16 *dstUV, *srcUV;

	INT16S x0=160, y0=120;
	INT16S x = 0, y = 0;
	INT32U offset;
	INT16U color = 0xc618;
	INT16U r=0;
	INT8U flag=0;
	INT32S d = 3 - 2 * r;

	INT16U jump_process_color=0x9fff;

	
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	divide_h = (lcd_h - lcd_h*cnt/ANIMATION_CNT);
	divide_w = (lcd_w - lcd_w*cnt/ANIMATION_CNT);
	dstUV = (u16 *)(dst + lcd_w*lcd_h);
	srcUV = (u16 *)(src + lcd_w*lcd_h);

for(j=0;j<8;j++)	
{
	r=j*25;
	
	if(y + y0 >= 0 && y + y0 < TFT_HEIGHT){
			offset = (y + y0) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - x || i > x0 + x){
					dst[offset + i] = src[offset + i];
				}else if(flag){
					dst[offset + i] = color;
				}
			}
		}
		if(y0 - y >= 0 && y0 - y < TFT_HEIGHT){
			offset = (y0 - y) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - x || i > x0 + x){
					dst[offset + i] = src[offset + i];
				}else if(flag){
					dst[offset + i] = color;
				}
			}
		}
		if(x + y0 >= 0 && x + y0 < TFT_HEIGHT){
			offset = (x + y0) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - y || i > x0 + y){
					dst[offset + i] = src[offset + i];
				}else if(flag){
					dst[offset + i] = color;
				}
			}
		}
		if(y0 - x >= 0 && y0 - x < TFT_HEIGHT){
			offset = (y0 - x) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - y || i > x0 + y){
					dst[offset + i] = src[offset + i];
				}else if(flag){
					dst[offset + i] = color;
				}
			}
		}

		if(y + y0 >= 0 && y + y0 < TFT_HEIGHT)
		{
			if(x0 - x >= 0 && x0 - x < TFT_WIDTH){
				GET_16BIT_DATA(dst + (y + y0) * TFT_WIDTH + x0 - x) = jump_process_color;
			}
			if(x0 + x >= 0 && x0 + x < TFT_WIDTH)	{
				GET_16BIT_DATA(dst + (y + y0) * TFT_WIDTH + x0 + x) = jump_process_color;
			}
		}
		if(y0 - y >= 0 && y0 - y < TFT_HEIGHT)
		{
			if(x0 - x >= 0 && x0 - x < TFT_WIDTH){
				GET_16BIT_DATA(dst + (y0 - y) * TFT_WIDTH + x0 - x) = jump_process_color;
			}
			if(x0 + x >= 0 && x0 + x < TFT_WIDTH)	{
				GET_16BIT_DATA(dst + (y0 - y) * TFT_WIDTH + x0 + x) = jump_process_color;
			}
			
		}
		if(x + y0 >= 0 && x + y0 < TFT_HEIGHT)
		{
			if(x0 - y >= 0 && x0 - y < TFT_WIDTH){
				GET_16BIT_DATA(dst + (x + y0) * TFT_WIDTH + x0 - y) = jump_process_color;
			}
			if(x0 + y >= 0 && x0 + y < TFT_WIDTH)	{
				GET_16BIT_DATA(dst + (x + y0) * TFT_WIDTH + x0 + y) = jump_process_color;
			}	
		}
		if(y0 - x >= 0 && y0 - x < TFT_HEIGHT)
		{
			if(x0 - y >= 0 && x0 - y < TFT_WIDTH){
				GET_16BIT_DATA(dst + (y0 - x) * TFT_WIDTH + x0 - y) = jump_process_color;
			}
			if(x0 + y >= 0 && x0 + y < TFT_WIDTH)	{
				GET_16BIT_DATA(dst + (y0 - x) * TFT_WIDTH + x0 + y) = jump_process_color;
			}
		}
		//#endif
		if(d < 0)
		{
			d += 4* x + 6;
			x++;
		}
		else
		{
			d += 10 + 4 * (x - y);
			x++;
			y--;
		}
		
		if(y0 - r > 0)
		{
			INT32U i = 0;
			for(i = 0; i < y0 - r; i++)
			{
				memcpy((INT32U *)(dst + i * TFT_WIDTH * 2),(INT32U *)(src + i * TFT_WIDTH * 2), TFT_WIDTH << 1);
			}
		}
	if(y0 + r + 1 < TFT_HEIGHT)
	{
		INT32U i = 0;
		for(i = y0 + r + 1; i < TFT_HEIGHT; i++)
		{
			memcpy((INT32U *)(dst + i * TFT_WIDTH * 2),
						(INT32U *)(src + i * TFT_WIDTH * 2), 
						TFT_WIDTH << 1);
		}
	}

	
	ax32xx_sysDcacheFlush((u32)dst,lcd_w*lcd_h*3/2);

  }
}

actionType *getActionFunc(u8 action)
{
	switch(action){
		case UPPER2BOTTOM:
			return actionUpperToBottom;
		case BOTTOM2UPPER:
			return actionBottomToUpper;
		case SQUARE_INSIDE2OUTSIDE:
			return actionSquareInsideToOutside;
		case SQUARE_OUTSIDE2INSIDE:
			return actionSquareOutsideToInside;
		case LEFTBOTTOM2RIGHTUPPER:
			return actionLeftBottomToRightUpper;
		case LEFTUPPER2RIGHTBOTTOM:
			return actionLeftUpperToRightBottom;
		case LEFT2RIGHT:
			return actionLeftToRight;
		case RIGHT2LEFT:
		case CIRCLE_INSIDE2OUTSIDE:
			return actionCircleInside2OutSide;
		case CIRCLE_OUTSIDE2INSIDE:
			return NULL;
	}
}


void animationInit(UserInterface *objName)
{
	disp_frame_t *buf = NULL;
	u16 lcd_w,lcd_h;
	hal_lcdGetResolution(&lcd_w,&lcd_h);
	lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment
	buf = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(buf)
	{
		hal_dispframeVideoCfg(buf,0,0,lcd_w,lcd_h);
	}
	objName->preFrameBuf=hal_sysMemMalloc(lcd_w*lcd_h*3/2,32);
	if(!objName->preFrameBuf)
	{
		deg_Printf("animationInit mem err!\n");
		return ;
	};
	memcpy(objName->preFrameBuf,buf->y_addr,lcd_w*lcd_h*3/2);
	dispLayerFreeFrame(DISP_LAYER_VIDEO,buf);
	objName->actionFunc= getActionFunc(objName->action);
	dispLayerUninit(DISP_LAYER_OSD0);
	deg_Printf("...animationInit!\n");

}


bool animationRun(void *objName, u8 *dstBuf)
{
	UserInterface *name = (UserInterface *)objName;

	if(!name->preFrameBuf || !dstBuf)
	{
		deg_Printf("preFrameBuf or dstBuf err\n");
		name->cnt = 0;
	}
	deg_Printf("%d\n", name->cnt);
	if(!name->cnt){
		if(name->preFrameBuf){
			hal_sysMemFree(name->preFrameBuf);
			name->preFrameBuf = NULL;
		}
		dispLayerInit(DISP_LAYER_OSD0);
		return true;
	}
	name->actionFunc(dstBuf, name->preFrameBuf, name->cnt);
	name->cnt--;
	return false;
}


void animationUnInit(UserInterface *objName)
{
	if(NULL!=objName->preFrameBuf)
	{
		hal_sysMemFree(objName->preFrameBuf);
		objName->preFrameBuf = NULL;
		deg_Printf("animationInit free.\n");
	}

}
INT32U jump_process_cnt=1;
INT16U jump_process_color=0x9fff;
uint8 jump_process_black=1;
uint8 jump_process_type;




//#define GET_16BIT_DATA(addr)		(*((INT16U *)addr))
void circle_fill(INT16S x0, INT16S y0, INT16U r, INT32U buff, INT32U src, INT8U flag)
{
	INT16S x = 0, y = r, i, len;
	INT32U offset;
	INT16U *_dest = (INT16U *)buff, *_src = (INT16U *)src;
	INT32S d = 3 - 2 * r;
	INT16U color = 0xc618;
	if(r == 0){
		memcpy((INT32U *)buff,(INT32U *)src, TFT_WIDTH*TFT_HEIGHT*2);
		return;
	}
	while(x <= y){
		#if 0//0, because of bug
		if(y + y0 >= 0 && y + y0 < TFT_HEIGHT){
			len = x0 - x;
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + ((INT32U)(y + y0)) * TFT_WIDTH * 2),
					(INT32U *)(src + ((INT32U)(y + y0)) * TFT_WIDTH * 2), 
					(INT32U)(len << 1));
			}
			len =  TFT_WIDTH - (x0 + x);
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
				offset = 0;
			}else{
				offset = (x0 + x) * 2;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + offset + ((INT32U)(y + y0)) * TFT_WIDTH * 2),
						(INT32U *)(src + offset + ((INT32U)(y + y0)) * TFT_WIDTH * 2), 
						(INT32U)(len << 1));
			}
			
		}
		if(y0 - y >= 0 && y0 - y < TFT_HEIGHT){
			len = x0 - x;
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + ((INT32U)(y0 - y)) * TFT_WIDTH * 2),
					(INT32U *)(src + ((INT32U)(y0 - y)) * TFT_WIDTH * 2), 
					(INT32U)(len << 1));
			}
			len =  TFT_WIDTH - (x0 + x);
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
				offset = 0;
			}else{
				offset = (x0 + x) * 2;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + offset + ((INT32U)(y0 - y)) * TFT_WIDTH * 2),
						(INT32U *)(src + offset + ((INT32U)(y0 - y)) * TFT_WIDTH * 2), 
						(INT32U)(len << 1));
			}
			
		}
		if(x + y0 >= 0 && x + y0 < TFT_HEIGHT){
			len = x0 - y;
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + ((INT32U)(x + y0)) * TFT_WIDTH * 2),
					(INT32U *)(src + ((INT32U)(x + y0)) * TFT_WIDTH * 2), 
					(INT32U)(len << 1));
			}	
			len =  TFT_WIDTH - (x0 + y);
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
				offset = 0;
			}else{
				offset = (x0 + y) * 2;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + offset + ((INT32U)(x + y0)) * TFT_WIDTH * 2),
						(INT32U *)(src + offset + ((INT32U)(x + y0)) * TFT_WIDTH * 2), 
						(INT32U)(len << 1));
			}
			
		}
		if(y0 - x >= 0 && y0 - x < TFT_HEIGHT){
			len = x0 - y;
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + ((INT32U)(y0 - x)) * TFT_WIDTH * 2),
					(INT32U *)(src + ((INT32U)(y0 - x)) * TFT_WIDTH * 2), 
					(INT32U)(len << 1));
			}
			len =  TFT_WIDTH - (x0 + y);
			if(len > TFT_WIDTH){
				len = TFT_WIDTH;
				offset = 0;
			}else{
				offset = (x0 + y) * 2;
			}
			if(len > 0){
				gp_memcpy_align((INT32U *)(buff + offset + ((INT32U)(y0 - x)) * TFT_WIDTH * 2),
						(INT32U *)(src + offset + ((INT32U)(y0 - x)) * TFT_WIDTH * 2), 
						(INT32U)(len << 1));
			}
			
		}
		#else
		if(y + y0 >= 0 && y + y0 < TFT_HEIGHT){
			offset = (y + y0) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - x || i > x0 + x){
					_dest[offset + i] = _src[offset + i];
				}else if(flag){
					_dest[offset + i] = color;
				}
			}
		}
		if(y0 - y >= 0 && y0 - y < TFT_HEIGHT){
			offset = (y0 - y) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - x || i > x0 + x){
					_dest[offset + i] = _src[offset + i];
				}else if(flag){
					_dest[offset + i] = color;
				}
			}
		}
		if(x + y0 >= 0 && x + y0 < TFT_HEIGHT){
			offset = (x + y0) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - y || i > x0 + y){
					_dest[offset + i] = _src[offset + i];
				}else if(flag){
					_dest[offset + i] = color;
				}
			}
		}
		if(y0 - x >= 0 && y0 - x < TFT_HEIGHT){
			offset = (y0 - x) * TFT_WIDTH;
			for(i = 0; i < TFT_WIDTH; i++){
				if(i < x0 - y || i > x0 + y){
					_dest[offset + i] = _src[offset + i];
				}else if(flag){
					_dest[offset + i] = color;
				}
			}
		}
		#endif
		if(y + y0 >= 0 && y + y0 < TFT_HEIGHT){
			if(x0 - x >= 0 && x0 - x < TFT_WIDTH){
				GET_16BIT_DATA(buff + (y + y0) * TFT_WIDTH + x0 - x) = jump_process_color;
			}
			if(x0 + x >= 0 && x0 + x < TFT_WIDTH)	{
				GET_16BIT_DATA(buff + (y + y0) * TFT_WIDTH + x0 + x) = jump_process_color;
			}
		}
		if(y0 - y >= 0 && y0 - y < TFT_HEIGHT){
			if(x0 - x >= 0 && x0 - x < TFT_WIDTH){
				GET_16BIT_DATA(buff + (y0 - y) * TFT_WIDTH + x0 - x) = jump_process_color;
			}
			if(x0 + x >= 0 && x0 + x < TFT_WIDTH)	{
				GET_16BIT_DATA(buff + (y0 - y) * TFT_WIDTH + x0 + x) = jump_process_color;
			}
			
		}
		if(x + y0 >= 0 && x + y0 < TFT_HEIGHT){
			if(x0 - y >= 0 && x0 - y < TFT_WIDTH){
				GET_16BIT_DATA(buff + (x + y0) * TFT_WIDTH + x0 - y) = jump_process_color;
			}
			if(x0 + y >= 0 && x0 + y < TFT_WIDTH)	{
				GET_16BIT_DATA(buff + (x + y0) * TFT_WIDTH + x0 + y) = jump_process_color;
			}	
		}
		if(y0 - x >= 0 && y0 - x < TFT_HEIGHT){
			if(x0 - y >= 0 && x0 - y < TFT_WIDTH){
				GET_16BIT_DATA(buff + (y0 - x) * TFT_WIDTH + x0 - y) = jump_process_color;
			}
			if(x0 + y >= 0 && x0 + y < TFT_WIDTH)	{
				GET_16BIT_DATA(buff + (y0 - x) * TFT_WIDTH + x0 + y) = jump_process_color;
			}
		}
		//#endif
		if(d < 0){
			d += 4* x + 6;
			x++;
		}else{
			d += 10 + 4 * (x - y);
			x++;
			y--;
		}
		
	}
#if 1
	if(y0 - r > 0){
		INT32U i = 0;
		for(i = 0; i < y0 - r; i++){
			memcpy((INT32U *)(buff + i * TFT_WIDTH * 2),
						(INT32U *)(src + i * TFT_WIDTH * 2), 
						TFT_WIDTH << 1);
		}
	}
	if(y0 + r + 1 < TFT_HEIGHT){
		INT32U i = 0;
		for(i = y0 + r + 1; i < TFT_HEIGHT; i++){
			memcpy((INT32U *)(buff + i * TFT_WIDTH * 2),
						(INT32U *)(src + i * TFT_WIDTH * 2), 
						TFT_WIDTH << 1);
		}
	}
#endif
}


void type_function_circle(INT32U buff)
{
	if(jump_process_cnt < 8)
	{
		if(jump_process_type == CIRCLE_OPEN)
		{
			INT32U r = jump_process_cnt * 25;
			circle_fill(160, 120, r, buff, /*jump_process_buff*/0, jump_process_black);
		}
		else if(jump_process_type == CIRCLE_CLOSE)
		{
			
		}
		jump_process_cnt++;
	}
	else
	{
		jump_process_type = JUMP_PROCESS_TYPE_NONE;
	}
}




