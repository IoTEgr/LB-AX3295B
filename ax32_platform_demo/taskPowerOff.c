#include "application.h"
#include "userInterface.h"

extern int usb_bat_flag;
extern int usb_bat_draw_first_pic;

int uninit(void)
{
	int ret,temp;

//    hal_batDetectEnable(0);
    boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,0); // back light off
    //boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,0); // led off
    boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_B,0);
#if (1 == IR_MENU_EN)
    boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,0);
#endif
	DAC_PA_MUTE_ON();

    deg_Printf("system power off\n");

    if(SysCtrl.battery != 0)
    {
	    while(1) // wait power release
	    {
	        ret = boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_POWER,(INT32U )&temp);
			if((ret<0) ||(temp==0))
				break;
	    }
    }

//	boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_POWEROFF,0);
//	temp = configValue2Int(CONFIG_ID_PARKMODE);
//    boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_STABLE,temp); // wait gsensor stable	

//    boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_SETPARK,temp); // gsensor park mode set.do not care gsensor.

//    boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,0); // usensor off
    hal_timerStop(TIMER0); 
	hal_sysUninit();


//  XOSTimeDly(100);
//	hal_timerStop(TIMER0); 
/*
	boardIoctrl(SysCtrl.bfd_usb,IOCTRL_USB_CHECK,(INT32U)&temp);
	if(temp && (SysCtrl.usb==USB_STAT_NULL))
	{
		deg_Printf("during system off, usb dc in, restart system \n");
		//ax32xx_WKI1WakeupTriger(1); //wki wakeup rising trigger
		//ax32xx_wkiCleanPending();
		//hal_vddWKOEnable(0);
		SysCtrl.usb = USB_STAT_DCIN;
		hal_sysReset(); // reset
		while(1);
		//	hal_vddWKOEnable(1);
		//	goto POWER_OFF_DCIN;
	}
	else
	{
		ax32xx_WKI1WakeupTriger(1); //wki wakeup rising trigger
		ax32xx_wkiCleanPending();
		hal_vddWKOEnable(0);
		ax32xx_sysCpuMsDelay(50);
	}
*/
	ax32xx_WKI1WakeupTriger(1); 
	ax32xx_wkiCleanPending();
	hal_vddWKOEnable(0);
	ax32xx_sysCpuMsDelay(50);

	while(1)
	{
/*
		boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_LOCK,(INT32U)&temp);
		if(hal_rtcAlarmStatusGet(1))
			hal_sysReset();
		temp = 0;
		boardIoctrl(SysCtrl.bfd_usb,IOCTRL_USB_CHECK,(INT32U)&temp);
		if(temp && (SysCtrl.usb==USB_STAT_NULL))
		{
			SysCtrl.usb = USB_STAT_DCIN;
			hal_sysReset(); // reset
		//	hal_vddWKOEnable(1);
		//	goto POWER_OFF_DCIN;
		}
		//XOSTimeDly(1000);
		//deg_Printf(".");
		//hal_vddWKOEnable(0);
       // ax32xx_wkiCleanPending();
*/
		ax32xx_sysCpuMsDelay(50);
	}
	return 0; // makesure the program won't be here
}

void taskPowerOffOpen(uint32 arg)
{
	int8 ret ,temp;
	
    layout_logo_show(1,0,0);                                       // Solve the problem that the LOGO is not displayed during the shutdown
    //layout_logo_show(1,1,0);                                       // power off.music en,wait music end
	XOSTimeDly(800);
		
	UserInterface powerOffAnimation;
	ANIMATION(powerOffAnimation, SQUARE_OUTSIDE2INSIDE)
	bool change_finir = false;
	while(1){
		disp_frame_t *p_lcd_buffer;

		p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
		if(p_lcd_buffer)
		{
			u16 lcd_w,lcd_h;
			hal_lcdGetResolution(&lcd_w,&lcd_h);
			hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);
			lcd_w=(lcd_w + 0x1f) & (~0x1f);  // add 32bit alignment

			memset(p_lcd_buffer->y_addr,0,lcd_w*lcd_h);
			memset(p_lcd_buffer->uv_addr, 0x80,lcd_w*lcd_h/2);
			change_finir = powerOffAnimation.run(&powerOffAnimation, p_lcd_buffer->y_addr);
			ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,lcd_w*lcd_h*3/2);
			dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
			XOSTimeDly(40);
			if(change_finir == true)
				break;
		}
		else
		{
			deg_Printf("video buf null!\n");
		}
	}

	//if(SysCtrl.usb != USB_STAT_DCIN) 
	ret = boardIoctrl(SysCtrl.bfd_usb,IOCTRL_USB_CHECK,(INT32U)&temp);
	deg_Printf("-------------ret=%d,temp=%d-------------------------.\n",ret,temp);
	if((ret>=0) &&temp)
	{
		boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_B,0);
		XOSTimeDly(800);
		taskStart(TASK_USB_DEVICE,USB_DEV_PCCAMERA);
		usb_bat_flag=1;
		usb_bat_draw_first_pic = 1;
		SysCtrl.usb=USB_STAT_DCIN;
		//uiOpenWindow(&menuUsbWindow,0);
	}
	else
	{
		 winUninit();
  	//  userConfigSave();
		uninit();
	}


   
}

sysTask taskPowerOff=
{
	"Power Off",
	NULL,
	taskPowerOffOpen,
	NULL,
	NULL,
};


