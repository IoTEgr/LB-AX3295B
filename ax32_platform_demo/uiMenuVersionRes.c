#include"uiWin.h"
#if 1//SMALL_PANEL_SUPPORT>0
enum
{
	VERSION_TIPS_ID=0,
	VERSION_LOG1_ID,
	VERSION_LOG2_ID,
};
static widgetCreateInfor versionWin[] =
{
	createFrameWin(Rx(70),Ry(50), Rw(180),Rh(120),R_COLOR_BLACK,WIN_ABS_POS),
	//createStringIcon(VERSION_TIPS_ID,Rx(0),Ry(10), Rw(180),Rh(40),R_ID_STR_SET_VERSION,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(VERSION_LOG1_ID,Rx(0),Ry(40), Rw(180),Rh(40),"JPY-V3.0-0329-030A",ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createStringIcon(VERSION_LOG2_ID,Rx(0),Ry(90), Rw(180),Rh(40),"20181010",ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	widgetEnd(),
};
#endif

