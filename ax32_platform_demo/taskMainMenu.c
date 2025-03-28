#include "application.h"
#include "userInterface.h"

void taskMainMenuOpen(uint32 arg)
{
	deg_Printf("main menu open\n");

#if 1
	UserInterface mainMenuAnimation;
	u8 action_type = SQUARE_OUTSIDE2INSIDE;
	switch(taskPrevious()){
		case TASK_POWER_ON:
		case TASK_PLAY_BACK:
		case TASK_SETTING_MENU:
		case TASK_GAME_MENU:
		case TASK_AUDIO_PLAYER:
			action_type = UPPER2BOTTOM;
			break;
		case TASK_VIDEO_RECORD:
		case TASK_PHOTO_ENCODE:
			action_type = SQUARE_OUTSIDE2INSIDE;
			break;
	}
	ANIMATION(mainMenuAnimation, action_type)
	memcpy(&changeUserInterface, &mainMenuAnimation,sizeof(UserInterface));
#endif
	hal_csiEnable(0);
	dispLayerInit(DISP_LAYER_OSD0);//game back to main Menu
	dispLayerSetPIPMode(DISP_PIP_DISABLE);
	yuv_rgb_table_init();
	
	uiOpenWindow(&mainMenuWindow,0);
}

void taskMainMenuService(uint32 arg)
{

}
void taskMainMenuClose(uint32 arg)
{
	yuv_rgb_table_uninit();

	deg_Printf("main menu exit.\n");
}
sysTask taskMainMenu=
{
	"Main menu",
	(u32)NULL,
	taskMainMenuOpen,
	taskMainMenuClose,
	taskMainMenuService,
};


