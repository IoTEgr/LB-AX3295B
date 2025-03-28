#include"uiWin.h"

enum
{
	FORMAT_TIPS_ID=0,
	FORMAT_SELECT_ID
};
static widgetCreateInfor formatWin[] =
{
	#if SMALL_PANEL_SUPPORT==0
	createFrameWin(Rx(70),Ry(42), Rw(180),Rh(142),R_COLOR_GRAY2,WIN_ABS_POS),
	createStringIcon(FORMAT_TIPS_ID,Rx(0),Ry(0), Rw(180),Rh(100),R_ID_STR_FMT_FORMAT,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createItemManage(FORMAT_SELECT_ID,Rx(0),Ry(100), Rw(180),Rh(40),INVALID_COLOR),
	#else
	createFrameWin(Rx(30),Ry(50), Rw(260),Rh(140),R_COLOR_GRAY2,WIN_ABS_POS),
	createStringIcon(FORMAT_TIPS_ID,Rx(0),Ry(0), Rw(260),Rh(100),R_ID_STR_FMT_FORMAT,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createItemManage(FORMAT_SELECT_ID,Rx(0),Ry(100), Rw(260),Rh(40),INVALID_COLOR),
	#endif
	widgetEnd(),
};



