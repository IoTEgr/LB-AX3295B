#include "application.h"

extern void winUpdateAllResId(void);
int configLanguage(void)
{
	static uint32 lastSetting;
	R_loadResource((void *)User_String_Table,R_STR_MAX);// load string table
	fontSetLanguage(configValue2Int(CONFIG_ID_LANGUAGE));
	if(configGet(CONFIG_ID_LANGUAGE)!=lastSetting)
	{
		lastSetting=configGet(CONFIG_ID_LANGUAGE);
		winUpdateAllResId();
	}
	return 0;
}
void configSys(uint32 configId)
{
	INT32U value = configValue2Int(configId);
    switch(configId)
	{
		case CONFIG_ID_KEYSOUND:
			  SysCtrl.f_keysound = value;break;	   
		case CONFIG_ID_RESOLUTION:
			   videoRecordCmdSet(CMD_COM_RESOLUTIONN,value);
			   task_video_record_caltime();
			   break;
		case CONFIG_ID_PRESLUTION:	
			   break;
		/*case CONFIG_ID_MOTIONDECTION:
			   if(value)
			   	   hal_mdEnable(1);
			   else
			   	   hal_mdEnable(0);
			   break;*/
/*
		case CONFIG_ID_PARKMODE:
			   if(SysCtrl.bfd_gsensor<0)
			   {
			   }
			   break;
*/
		case CONFIG_ID_AUDIOREC:
			   videoRecordCmdSet(CMD_COM_AUDIOEN,value);
			   break;
	    case CONFIG_ID_TIMESTAMP	: 
			   videoRecordCmdSet(CMD_COM_TIMESTRAMP,value);break;
	    case CONFIG_ID_LOOPTIME : 
			   SysCtrl.t_mrec = value;
			  if(value == 0)
		      {
			      videoRecordCmdSet(CMD_COM_LOOPREC,0);
				  videoRecordCmdSet(CMD_COM_LOOPTIME,10*60);
				  SysCtrl.t_mrec = 10*60;
		      }
			  else
			  {
				  videoRecordCmdSet(CMD_COM_LOOPREC,1);
				  videoRecordCmdSet(CMD_COM_LOOPTIME,value);
			  }
			  break;
/*
		 case CONFIG_ID_PFASTVIEW: 
				SysCtrl.t_fastview = value;
				deg_Printf("SysCtrl.t_fastview=%d\n",SysCtrl.t_fastview);
				break;
*/
	    case CONFIG_ID_LANGUAGE:	
			  configLanguage();  
			  break;
/*
		case CONFIG_ID_GSENSOR:
			  if(SysCtrl.bfd_gsensor>=0)
			      boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_SETLOCK,(INT32U)value);
			  break;
	    case CONFIG_ID_EV:
			  sensor_compensate_exp_set(value);
			  break;
		case CONFIG_ID_WBLANCE:
			  awb_scene_mod_set(value);
			  break;
*/
		case CONFIG_ID_FREQUNCY:
			  //sensor_frame_rate_adapt(value,25);
			  break;
		//case CONFIG_ID_IR_LED:
			  //boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,value);
			  //break;
		default:
			 break;

	}	/**/
}



void configSystem(void)
{
	uint32 i;
	for(i=0;i<CONFIG_ID_MAX;i++)
		configSys(i);
}



