#include "application.h"

extern int nesStart(/*char* gameName*/int res_num);
extern void nesStop(void);
extern int InfoNES_Main(void);
char gameFileName[] = {"game00.nes"};
XWork_T *nes_pAPU_sync = NULL;
extern void InfoNES_pAPUVsync(void);

void NesGameOpen(int item)
{
	u16 width, height;
	dispLayerGetResolution(DISP_LAYER_OSD0,&width,&height);
	deg_Printf("the width == %d,the height == %d\n",width,height);
	InfoNES_OSD_Resolution_Set(width,height);	
	if(!InfoNES_OSD_Resolution_Check())
		return;
	
	if(nesStart(RES_GAME001+item)<0)
	{
		deg_Printf("nes start error\n");
		//taskChange();
		taskStart(TASK_MAIN_MENU,0);
		return ;
	}

	
	ax32xx_intEnable(IRQ_USB20,0);	
	InfoNES_Init();
	ax32xx_intCriticalInit();
	ax32xx_intCriticalEnter();
	if(nes_pAPU_sync == NULL)
		nes_pAPU_sync = XWorkCreate(16*X_TICK,InfoNES_pAPUVsync);  // isp process
	ax32xx_intCriticalExit();
	NesStat_Set(true);

}

void taskNesGameOpen(uint32 arg1)
{
	deg_Printf("NES game task enter\n");
	background_logo_image_show(RES_ICONSETTINGMENU);
	//if(taskPrevious() == TASK_PLAY_GAME || taskPrevious() == TASK_PLAY_GAME_BACK)
	{
		dispLayerInit(DISP_LAYER_OSD0);
	}
	uiOpenWindow(&nesGameWindow,0);
	deg_Printf("game start\n");
}
void taskNesGameClose(uint32 arg)
{
	dispLayerUninit(DISP_LAYER_OSD0);
	NesStat_Set(false);
	InfoNES_Fin();
	nesStop();
	ax32xx_intCriticalInit();
	ax32xx_intCriticalEnter();
	XWorkDestory(nes_pAPU_sync);
	nes_pAPU_sync = NULL;
	ax32xx_intCriticalExit() ;
	//dispLayerUninit(DISP_LAYER_OSD0);
	deg_Printf("NES game task exit\n");
}
void taskNesGameService(uint32 arg)
{
	if(NesStat_Get())
	{
		if(InfoNES_Main()<0)
		{
			ax32xx_intEnable(IRQ_USB20,1);
			//taskChange();
			//deg_Printf("the game quit\n");
			if(taskCurrent() == TASK_PLAY_GAME)
				taskStart(TASK_PLAY_GAME_BACK,0);
			else
				taskStart(TASK_PLAY_GAME,0);
			XMsgQFlush(SysCtrl.sysQ);
			////XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVNET_GAMEBACK,0));	
		}
	}
}

sysTask taskNesGame=
{
	"NES game",
	NULL,
	taskNesGameOpen,
	taskNesGameClose,
	taskNesGameService,
};

sysTask taskNesGameBack=
{
	"NES game back",
	NULL,
	taskNesGameOpen,
	taskNesGameClose,
	taskNesGameService,
};


