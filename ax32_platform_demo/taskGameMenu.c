#include "application.h"
#include "userInterface.h"


void taskGameMenuOpen(uint32 arg)
{
	deg_Printf("game menu open\n");
	
	if(taskPrevious() == TASK_MAIN_MENU)
	{
		UserInterface gameMenuAnimation;
		ANIMATION(gameMenuAnimation, BOTTOM2UPPER)
		memcpy(&changeUserInterface, &gameMenuAnimation,sizeof(UserInterface));
	}

	hal_csiEnable(0);
	dispLayerSetPIPMode(DISP_PIP_DISABLE);
	yuv_rgb_table_init();

	uiOpenWindow(&gameMenuWindow,0);
}

void taskGameMenuService(uint32 arg)
{

}
void taskGameMenuClose(uint32 arg)
{
	yuv_rgb_table_uninit();

	deg_Printf("game menu exit.\n");
}
sysTask taskGameMenu=
{
	"Game menu",
	(u32)NULL,
	taskGameMenuOpen,
	taskGameMenuClose,
	taskGameMenuService,
};


