#include"uiWin.h"

enum
{
	OPTION_TITLE_ID=0,
	OPTION_RECT_ID,
	OPTION_SELECT_ID,
};
static widgetCreateInfor menuOptionWin[] =
{
	createFrameWin(Rx(70),Ry(52), Rw(160),Rh(/*176*/105),/*R_COLOR_BLACK*/R_COLOR_GRAY,WIN_ABS_POS),
	//createStringIcon(OPTION_TITLE_ID,Rx(0), Ry(0), Rw(180), Rh(32)," ",ALIGNMENT_CENTER, R_COLOR_BLUE,0),
	//createRect(OPTION_RECT_ID,       Rx(0),Ry(32), Rw(180),Rh(2),R_COLOR_DBLUE),
	createItemManage(OPTION_SELECT_ID,Rx(0),Ry(/*34*/0), Rw(160),Rh(/*142*/105),/*INVALID_COLOR*/R_COLOR_GRAY),
	widgetEnd(),
};



