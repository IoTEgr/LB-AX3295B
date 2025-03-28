#include"uiWin.h"

#if 1
enum
{
	DELALL_TIPS_ID=0,
	DELALL_SELECT_ID
};
static widgetCreateInfor delAllWin[] =
{

	createFrameWin(Rx(50),Ry(50), Rw(220),Rh(140),R_COLOR_BLACK,WIN_ABS_POS),
	createStringIcon(DELALL_TIPS_ID,Rx(0),Ry(0), Rw(220),Rh(60),R_ID_STR_SET_DELETEALL,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createItemManage(DELALL_SELECT_ID,Rx(0),Ry(60), Rw(220),Rh(80),INVALID_COLOR),

	widgetEnd(),
};
#endif


