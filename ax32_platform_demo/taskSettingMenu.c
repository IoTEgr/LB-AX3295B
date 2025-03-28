#include "application.h"
#include "userInterface.h"

extern MENU(movie);

void taskSettingMenuOpen(uint32 arg)
{
	deg_Printf("setting menu open\n");	
	
	UserInterface settingAnimation;
	ANIMATION(settingAnimation, BOTTOM2UPPER)
	memcpy(&changeUserInterface, &settingAnimation,sizeof(UserInterface));


	hal_csiEnable(0);
	dispLayerSetPIPMode(DISP_PIP_DISABLE);
	uiOpenWindow(&menuItemWindow,1,&MENU(movie)); 
}

void taskSettingMenuService(uint32 arg)
{

}
void taskSettingMenuClose(uint32 arg)
{
	//yuv_rgb_table_uninit();
	deg_Printf("setting menu exit.\n");
}
sysTask taskSettingMenu=
{
	"setting menu",
	(u32)NULL,
	taskSettingMenuOpen,
	taskSettingMenuClose,
	taskSettingMenuService,
};


