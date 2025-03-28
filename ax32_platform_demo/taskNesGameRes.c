#include"uiWin.h"



enum
{
	NES_STRING = 0,
	NES_SELECT_ID,
};

static widgetCreateInfor nesGameWin[] =
{
	//createFrameWin( Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createStringIcon(NES_STRING,Rx(120), Ry(30), Rw(80), Rh(25),R_ID_STR_GAME_NES,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createItemManage(NES_SELECT_ID, Rx((320-220)/2),Ry((240-140)/2+10), Rw(220),Rh(140),INVALID_COLOR),
	widgetEnd()
};



