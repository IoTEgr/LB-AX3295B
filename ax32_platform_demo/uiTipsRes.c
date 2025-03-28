#include"uiWin.h"

enum
{
	TIP_STRING_ID=0,
};
static widgetCreateInfor tipsWin[] =
{
	createFrameWin(Rx(40),Ry(60), Rw(240),Rh(120),R_COLOR_BLUE,WIN_ABS_POS),
	createStringIcon(TIP_STRING_ID,Rx(0),Ry(0), Rw(240),Rh(120)," ",ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	widgetEnd(),
};



