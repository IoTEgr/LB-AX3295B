#include "application.h"


void taskAudioPlayerOpen(uint32 arg)
{
	deg_Printf("AudioPlayer open\n");

	hal_csiEnable(0);
	dispLayerSetPIPMode(DISP_PIP_DISABLE);


	uni2oem936_init();
	oem2uni936_init();

	s32 ret=mp3_dac_memInit(MP3_PCM_FIFO_NUM,MP3_PCM_FIFO_SIZE,MP3_BITSTREAM_BUFSIZE);
	if(0!=ret)
	{
		deg_Printf("mp3 malloc err\n");
	}

	dispLayerInit(DISP_LAYER_OSD0);  
	
	uiOpenWindow(&audioPlayerWindow,0);
}


void taskAudioPlayerService(uint32 arg)
{
	if(MP3_DAC_START==mp3_play_sta())
	{
		int ret=mp3_decode();
		//deg_Printf("ret=%d,",ret);
		if(2==ret)
		{
			XMsgQPost(SysCtrl.sysQ,makeEvent(MP3_EVENT_PLAY_FINISH,ret));
		}
	}
}

void taskAudioPlayerClose(uint32 arg)
{
	deg_Printf("AudioPlayer exit.\n");
	hal_dacPlayStop();
	mp3_dac_memUinit();
}

sysTask taskAudioPlayer=
{
	"Audio Player",
	(u32)NULL,
	taskAudioPlayerOpen,
	taskAudioPlayerClose,
	taskAudioPlayerService,
};


