#include"uiWin.h"

enum
{
	DATETIME_SELECT_ID=0,
};
static widgetCreateInfor dateTimeWin[] =
{
	createFrameWin(Rx(60),Ry(68), Rw(180),Rh(112),R_COLOR_BLUE1,WIN_ABS_POS),
	createItemManage(DATETIME_SELECT_ID,Rx(0),Ry(0), Rw(180),Rh(142),INVALID_COLOR),
	widgetEnd(),
};



