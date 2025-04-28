#include "application.h"
#include "../multimedia/mp3/inc/mp3_decode_api.h"
#include "../multimedia/unifont/ebook.h"
#include "taskAudioPlayerRes.c"

#define AUDIO_PLAY_FLASH_DEMO 0 // 0: not play flash mp3 demo  1: play flash mp3 demo

static u8 *audioplayer_bk_buf, list_flag = 1;
static u16 bk_w, bk_h;

#if AUDIO_PLAY_FLASH_DEMO
static u8 *audio_demo_buf;
static u32 audio_demo_size;
#endif

TCHAR cur_file_name[AUDIOPLAYER_NAME_LEN_MAX] = {0};
void AudioPlayer_MusicList_Refresh(winHandle handle)
{
	int i;
	list_flag = 1;
	winSetVisible(winItem(handle, AUDIOPLAYER_LIST_TIPS1_ID), false);
	winSetVisible(winItem(handle, AUDIOPLAYER_IMAGE_ID), false);
	winSetVisible(winItem(handle, AUDIOPLAYER_LIST_TIPS2_ID), true);
	winSetVisible(winItem(handle, AUDIOPLAYER_PLAY_TIME_ID), false);
	winSetVisible(winItem(handle, AUDIOPLAYER_STA_ID), false);

	for (i = 0; i < AUDIOPLAYER_LIST_NUM; i++)
	{
		winSetfgColor(winItem(handle, AUDIOPLAYER_LIST_ITEM1_ID + i), R_COLOR_TBLACK);
		winSetVisible(winItem(handle, AUDIOPLAYER_LIST_ITEM1_ID + i), true);
		winSetVisible(winItem(handle, AUDIOPLAYER_LIST_STR1_ID + i), true);
		if (audio_player_list.list_idx >= (AUDIOPLAYER_LIST_NUM - 1))
		{
			winSetResid(winItem(handle, AUDIOPLAYER_LIST_STR1_ID + i), strToResId(nename->name[audio_player_list.list_idx - (AUDIOPLAYER_LIST_NUM - 1) + i]));
			if (0 == i)
				first_music = audio_player_list.list_idx - (AUDIOPLAYER_LIST_NUM - 1) + i;
		}
		else
		{
			winSetResid(winItem(handle, AUDIOPLAYER_LIST_STR1_ID + i), strToResId(nename->name[i]));
			if (0 == i)
				first_music = i;
		}
	}
}
void remove_file_extension(unsigned short *str)
{
	if (!str)
		return;

	int last_dot = -1;
	int len = 0;

	// 查找最后一个点号的位置
	while (str[len])
	{
		if (str[len] == L'.' && str[len + 1] != L'/')
		{ // 排除路径中的点
			last_dot = len;
		}
		len++;
	}

	// 找到有效后缀则截断
	if (last_dot != -1 && last_dot > 0)
	{
		str[last_dot] = 0;
	}
}

static u32 audioplayer_file_list_scan(u8 *dir)
{
	FRESULT res;
	FILINFO finfo;
	int count = 0, type;
	TCHAR dirpath[128];
	TCHAR pattern[] = {'*', '.', '*', 0};
	char extname[8];
	u8 *path;
	DIR drent;
	count = 0;
	path = dir;
	while (*path)
	{
		if ((*path == '/') && (0 == *(path + 1)))
		{
			break;
		}
		dirpath[count++] = *path++;
	}
	dirpath[count++] = 0;

	count = 0;
	res = f_findfirst(&drent, &finfo, dirpath, pattern); //--find first file
	while (1)
	{
		if (res != FR_OK || finfo.fname[0] == 0)
			break;
		if (finfo.fname[0] != '.') //--skip hide file
		{
			getExtName(finfo.fname, extname, sizeof(extname));
			type = manangerCheckFileType(/*finfo.fname*/ extname);
			if (type < 0)
			{
				// deg_Printf("unknow type:%s extname:%s\n",finfo.fname,extname);
			}
			else
			{
				// deg_Printf("mp3:%s  extname:%s\n",finfo.fname,extname);
				if (FILE_TYPE_MP3 == type)
				{
					tchNCopy(audio_player_list.file_list->name[count], AUDIOPLAYER_NAME_LEN_MAX, finfo.fname);
					memset(cur_file_name, 0, AUDIOPLAYER_NAME_LEN_MAX);
					tchNCopy(cur_file_name, AUDIOPLAYER_NAME_LEN_MAX, finfo.fname);
					remove_file_extension(cur_file_name); // 新增调用
					tchNCopy(nename->name[count], AUDIOPLAYER_NAME_LEN_MAX, cur_file_name);
					count++;
					if (count >= AUDIOPLAYER_LIST_FILE_MAX)
					{
						break;
					}
				}
			}
		}
		res = f_findnext(&drent, &finfo);
	}
	f_closedir(&drent);

	return count;
}
static void audioplayer_play_mp3()
{
	//==play mp3==
	if ((SDC_STAT_NORMAL == SysCtrl.sdcard) || (SDC_STAT_FULL == SysCtrl.sdcard)) // play sd file
	{
		if (audio_player_list.list_num > 0)
		{
			//==open mp3 file and play==
			if (MP3_DAC_STOP != mp3_play_sta())
			{
				mp3_dacPlay_Stop();
			}

			if (audio_player_list.cur_file >= 0)
			{
				close(audio_player_list.cur_file);
				audio_player_list.cur_file = -1;
			}

			TCHAR filepath[128];
			int dirLen = strlen(FILEDIR_MP3);
			int bufLen = sizeof(filepath) / sizeof(filepath[0]);
			Ascii2Tchar(FILEDIR_MP3, filepath, bufLen);
			tchNCopy(filepath + dirLen, bufLen - dirLen, audio_player_list.file_list->name[audio_player_list.list_idx]);

			audio_player_list.cur_file = (s32)open(filepath, FA_READ);
			if (audio_player_list.cur_file < 0)
			{
				deg_Printf("open [%s] error\n", filepath);
			}
			deg_Printf("--VOL:%d\n", VOLUME_LEVEL * SysCtrl.volume_level);
			mp3_decode_play(audio_player_list.cur_file, VOLUME_LEVEL * SysCtrl.volume_level);
			audio_player_list.list_play_idx = audio_player_list.list_idx;

			audio_player_list.total_time = mp3_file_time_cal();
			audio_player_list.play_time = 0;
			deg_Printf("mp3_play_time=%d,%d\n", audio_player_list.play_time, audio_player_list.total_time);
		}
	}
	else
	{
#if AUDIO_PLAY_FLASH_DEMO
		//==play mp3 demo==
		u32 addr, size;
		addr = nv_open(RES_AUDIO_PLAYER_DEMO);
		if (addr < 0)
		{
			deg_Printf("read id err\n");
		}
		size = nv_size(RES_AUDIO_PLAYER_DEMO);
		if ((size < 64) || (size > 2 * 1024 * 1024))
		{
			deg_Printf("size err:%d\n", size);
		}

		if (NULL == audio_demo_buf)
		{
			audio_demo_size = size;
			audio_demo_buf = hal_sysMemMalloc(audio_demo_size, 32);
		}

		if (audio_demo_buf)
		{
			deg_Printf("demo size:%d\n", audio_demo_size);
			nv_read(addr, audio_demo_buf, audio_demo_size);

			mp3_decode_play_buf(audio_demo_buf, audio_demo_size, AUDIO_MP3_VOLUME);

			audio_player_list.total_time = mp3_file_time_cal();
			audio_player_list.play_time = 0;
			deg_Printf("mp3_play_time=%d,%d\n", audio_player_list.play_time, audio_player_list.total_time);
		}
		else
		{
			deg_Printf("mem err:%d\n", audio_demo_size);
		}
#endif
	}
	hal_dacSetVolume(SysCtrl.volume_level * VOLUME_LEVEL);
}

static void audioplayer_showbuf()
{
	disp_frame_t *p_lcd_buffer;

	p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if (p_lcd_buffer)
	{
		hal_dispframeVideoCfg(p_lcd_buffer, 0, 0, p_lcd_buffer->w, p_lcd_buffer->h);
		//==draw bk==
		memcpy(p_lcd_buffer->y_addr, audioplayer_bk_buf, bk_w * bk_h * 3 / 2);
		ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr, p_lcd_buffer->w * p_lcd_buffer->h * 3 / 2);
		dispLayerShow(DISP_LAYER_VIDEO, (INT32U)p_lcd_buffer, 0, 0, VIDEO_ROTATE);
	}
	else
	{
		deg_Printf("video buf null!\n");
	}
}

static int audioPlayerKeyMsgList(winHandle handle, uint32 parameNum, uint32 *parame)
{
	uint32 keyState = KEY_STATE_INVALID;
	//	int ret;
	if (parameNum == 1)
		keyState = parame[0];
	if (keyState == KEY_PRESSED)
	{
		return 0;
		deg("press key photo ");
		if (audio_player_list.list_show_en)
		{
			audio_player_list.list_show_en = 0;
		}
		else
		{
			audio_player_list.list_show_en = 1;
			audio_player_list.list_show_time = AUDIOPLAYER_LIST_SHOW_TIME;
		}
		deg("show list_show_en[%d]\n", audio_player_list.list_show_en);
		audioPlayerFileListShow(handle);
	}
	return 0;
}

static int audioPlayerKeyMsgOk(winHandle handle, uint32 parameNum, uint32 *parame)
{
	uint32 keyState = KEY_STATE_INVALID;
	// static u8 ListEnFlag = 0;
	if (parameNum == 1)
		keyState = parame[0];
	if (keyState == KEY_PRESSED)
	{
		if (list_flag)
		{
			if (audio_player_list.list_num > 0)
			{
				list_flag = 0;
				audio_player_list.list_show_en = 0;
				audioPlayerTimeShow(handle);
				audioPlayerFileBkShow(handle);
				winSetResid(winItem(handle, AUDIOPLAYER_LIST_TIPS1_ID), strToResId(nename->name[audio_player_list.list_idx]));
				winSetVisible(winItem(handle, AUDIOPLAYER_LIST_TIPS2_ID), false);
				winSetVisible(winItem(handle, AUDIOPLAYER_LIST_TIPS1_ID), true);
				winSetVisible(winItem(handle, AUDIOPLAYER_PLAY_TIME_ID), true);
				winSetVisible(winItem(handle, AUDIOPLAYER_STA_ID), true);
				winSetVisible(winItem(handle, AUDIOPLAYER_IMAGE_ID), true);
				audioPlayerStaShow(handle, 1);
				audioPlayerFileListShow(handle);
				audioplayer_play_mp3();
			}
		}
		else
		{
			if (audio_player_list.list_show_en)
			{
				audio_player_list.list_show_time = AUDIOPLAYER_LIST_SHOW_TIME;
				// if(ListEnFlag == 0)
				if (MP3_DAC_PAUSE == mp3_play_sta())
				{
#if 1
					while (audioPlaybackGetStatus() == MEDIA_STAT_PLAY)
					{
						XOSTimeDly(10);
					}
					mp3_dacPlay_Start(VOLUME_LEVEL * SysCtrl.volume_level);
					hal_dacSetVolume(SysCtrl.volume_level * VOLUME_LEVEL);
					audioPlayerStaShow(handle, 1);
#else
					audioplayer_play_mp3();
					audioPlayerStaShow(handle, 1);
#endif
				}
				else if (MP3_DAC_START == mp3_play_sta())
				{
					mp3_dacPlay_Pause();
					audioPlayerStaShow(handle, 0);
				}
				else
				{
					audioplayer_play_mp3();
					if ((audio_player_list.list_num == 0) || (SysCtrl.sdcard == SDC_STAT_NULL))
					{
						audioPlayerStaShow(handle, 0);
						audioPlayerMsgfile(handle, 0, 0);
					}
					else
						audioPlayerStaShow(handle, 1);
				}
				// ListEnFlag ^= 1;
			}
			else
			{
				if (MP3_DAC_START == mp3_play_sta())
				{
					mp3_dacPlay_Pause();
					audioPlayerStaShow(handle, 0);
				}
				else if (MP3_DAC_PAUSE == mp3_play_sta())
				{
					while (audioPlaybackGetStatus() == MEDIA_STAT_PLAY)
					{
						XOSTimeDly(10);
					}
					mp3_dacPlay_Start(VOLUME_LEVEL * SysCtrl.volume_level);
					hal_dacSetVolume(SysCtrl.volume_level * VOLUME_LEVEL);
					audioPlayerStaShow(handle, 1);
				}
				else
				{
					audioplayer_play_mp3();
					if ((audio_player_list.list_num == 0) || (SysCtrl.sdcard == SDC_STAT_NULL))
					{
						audioPlayerStaShow(handle, 0);
						audioPlayerMsgfile(handle, 0, 0);
					}
					else
						audioPlayerStaShow(handle, 1);
				}
			}
		}
		audioPlayerTimeShow(handle);
	}
	return 0;
}
static int audioPlayerKeyMsgUp(winHandle handle, uint32 parameNum, uint32 *parame)
{
	uint32 keyState = KEY_STATE_INVALID;
	if (parameNum == 1)
		keyState = parame[0];
	if (keyState == KEY_RELEASE)
	{
		if (list_flag)
		{
			if (audio_player_list.list_num > 0)
			{
				if (audio_player_list.list_idx > 0)
				{
					audio_player_list.list_idx--;
				}
				else
				{
					audio_player_list.list_idx = audio_player_list.list_num - 1;
				}

				audio_player_list.list_show_time = AUDIOPLAYER_LIST_SHOW_TIME;
				audioPlayerFileListShow(handle);
				audioPlayerStaShow(handle, 1);
			}
		}
		else
		{
			SysCtrl.volume_level++;
			if (SysCtrl.volume_level > 8)
				SysCtrl.volume_level = 8;
			configSet(CONFIG_ID_VOLUME, SysCtrl.volume_level + R_ID_STR_COM_LEVEL_0);
			hal_dacSetVolume(SysCtrl.volume_level * VOLUME_LEVEL);
			audioPlayerVolumeShow(handle, 1);
			userConfigSave();
		}
	}
	return 0;
}

static int audioPlayerKeyMsgDown(winHandle handle, uint32 parameNum, uint32 *parame)
{
	uint32 keyState = KEY_STATE_INVALID;
	if (parameNum == 1)
		keyState = parame[0];

	if (keyState == KEY_RELEASE)
	{
		if (list_flag)
		{
			if (audio_player_list.list_num > 0)
			{
				if (audio_player_list.list_idx < audio_player_list.list_num - 1)
				{
					audio_player_list.list_idx++;
				}
				else
				{
					audio_player_list.list_idx = 0;
				}

				audio_player_list.list_show_time = AUDIOPLAYER_LIST_SHOW_TIME;
				audioPlayerFileListShow(handle);
				audioPlayerStaShow(handle, 1);
			}
		}
		else
		{
			if (SysCtrl.volume_level > 1)
			{
				SysCtrl.volume_level--;
				if (SysCtrl.volume_level < 0)
					SysCtrl.volume_level = 0;
				audioPlayerVolumeShow(handle, 1);
				configSet(CONFIG_ID_VOLUME, SysCtrl.volume_level + R_ID_STR_COM_LEVEL_0);
				userConfigSave();
				hal_dacSetVolume(SysCtrl.volume_level * VOLUME_LEVEL);
			}
		}
	}
	return 0;
}

static int audioPlayerKeyMsgLeft(winHandle handle, uint32 parameNum, uint32 *parame)
{
	uint32 keyState = KEY_STATE_INVALID;
	if (parameNum == 1)
		keyState = parame[0];
	if (keyState == KEY_RELEASE)
	{
	}
}

static int audioPlayerKeyMsgRight(winHandle handle, uint32 parameNum, uint32 *parame)
{
	uint32 keyState = KEY_STATE_INVALID;
	if (parameNum == 1)
		keyState = parame[0];
	if (keyState == KEY_RELEASE)
	{
	}
}

extern menu MENU(playBack);
#define VIDEO_PLAY_WAIT_KEYSOUND_END2                   \
	while (audioPlaybackGetStatus() == MEDIA_STAT_PLAY) \
	{                                                   \
		XOSTimeDly(10);                                 \
	}

#if 0
static int audioPlayerKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//if((videoPlaybackGetStatus() != MEDIA_STAT_START)&&(managerFileCount(SysCtrl.avi_list)>0))
		{
			//hal_dacPlayStop();
				mp3_dacPlaySetStaPause();
				audioPlayerStaShow(handle,0);deg_Printf("========222============\n");
				XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_ROTATE,0));
				XOSTimeDly(50);  // delay 80ms,wait lcd frame end	
			//videoPlaybackStop();
			//deamon_keysound_play();
			//VIDEO_PLAY_WAIT_KEYSOUND_END2;
			uiOpenWindow(&delCurWindow,0);
		}
		
	}
	return 0;
}
#endif

static int audioPlayerKeyMsgExit(winHandle handle, uint32 parameNum, uint32 *parame)
{
	uint32 keyState = KEY_STATE_INVALID;
	int i;
	if (parameNum == 1)
		keyState = parame[0];
	if (keyState == KEY_PRESSED)
	{
#if BT_MODE
		taskStart(TASK_GAME_MENU, 0);
#else
		if (list_flag)
		{
			taskStart(TASK_MAIN_MENU, 0);
			if (0 == SysCtrl.cartoon_mode)
			{
				SysCtrl.cartoon_mode = 2;
				SysCtrl.cartoon_show_cnt = 0;
			}
		}
		else
		{
			list_flag = 1;
			audio_player_list.list_show_en = 1;
			AudioPlayer_MusicList_Refresh(handle);
			mp3_dacPlay_Pause();
			audioPlayerStaShow(handle, 0);
			audioPlayerFileListShow(handle);
		}
#endif
	}
	return 0;
}

static int audioPlayerSysMsgSD(winHandle handle, uint32 parameNum, uint32 *parame)
{
	audioPlayerSDShow(handle);
	memset(audio_player_list.file_list, 0, sizeof(struct AudioPlayerMp3NameList));
	memset(nename, 0, sizeof(struct AudioPlayerMp3NameList));
	if ((SDC_STAT_NULL == SysCtrl.sdcard) || (SDC_STAT_ERROR == SysCtrl.sdcard))
	{
		if (MP3_DAC_STOP != mp3_play_sta())
		{
			mp3_dacPlay_Stop();
		}
		audio_player_list.list_num = 0;
		audio_player_list.list_idx = 0;
		audio_player_list.list_show_en = 1;

		audio_player_list.play_time = 0;
		audio_player_list.total_time = 0;
		AudioPlayer_MusicList_Refresh(handle);
		audioPlayerFileListShow(handle);
		audioPlayerStaShow(handle, 0);
		audioPlayerTimeShow(handle);
	}
	else if (SDC_STAT_NORMAL == SysCtrl.sdcard)
	{
		if (NULL != audio_player_list.file_list)
		{
			audio_player_list.list_num = audioplayer_file_list_scan((u8 *)FILEDIR_MP3);
			audio_player_list.list_idx = 0;
			deg_Printf("audio_player_list.list_num=%d\n", audio_player_list.list_num);
			if (audio_player_list.list_num > 0)
			{
				audio_player_list.list_show_en = 1;
				audio_player_list.list_show_time = AUDIOPLAYER_LIST_SHOW_TIME;
			}

			//==osd show==
			audioPlayerSDShow(handle);
			audioPlayerBaterryShow(handle);
			if (MP3_DAC_START == mp3_play_sta())
			{
				audioPlayerStaShow(handle, 1);
			}
			else
			{
				audioPlayerStaShow(handle, 0);
			}
			AudioPlayer_MusicList_Refresh(handle);
			audioPlayerTimeShow(handle);
			audioPlayerFileListShow(handle);
		}
	}

	return 0;
}

extern u8 mp3_flag;
extern u8 mp3_flag2;

static int audioPlayerOpenWin(winHandle handle, uint32 parameNum, uint32 *parame)
{
	mp3_flag = 1;
	deg_Printf("audioPlayer Open Win:%d\n", XOSTimeGet());
	u16 i /*,ret*/;
	memset(&audio_player_list, 0, sizeof(struct AudioPlayerList));
	audio_player_list.cur_file = -1;

	Ebook_Init();
	//	ret=MP3_unicode_font_init();
	//	if(0==ret)
	//	{
	//		SysCtrl.mp3_font_use=1;
	//	}
	//	deg_Printf("mp3 font:%d\n",ret);

	//==video buf init and draw bkground====
	hal_lcdGetResolution(&bk_w, &bk_h);
	audioplayer_bk_buf = hal_sysMemMalloc(bk_w * bk_h * 3 / 2, 32);
	if (NULL != audioplayer_bk_buf)
	{
#if MAIN_MENU_UI
		jpg_decode_buf(RES_GAMEMENU_BK /*RES_MP3_BK*/, audioplayer_bk_buf, audioplayer_bk_buf + bk_w * bk_h, bk_w, bk_h);
#else
		jpg_decode_buf(RES_AUDIOPLAY_BK, audioplayer_bk_buf, audioplayer_bk_buf + bk_w * bk_h, bk_w, bk_h);
#endif
	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}
	audioplayer_showbuf();
	deg_Printf("show time:%d\n", XOSTimeGet());

	//==mp3 list name init==
	audio_player_list.file_list = hal_sysMemMalloc(sizeof(struct AudioPlayerMp3NameList), 64);
	nename = hal_sysMemMalloc(sizeof(struct AudioPlayerMp3NameList), 64);
	if (NULL == audio_player_list.file_list)
	{
		deg_Printf("mp3 list name  malloc fail\n");
	}
	else
	{
		memset(audio_player_list.file_list, 0, sizeof(struct AudioPlayerMp3NameList));
		memset(nename, 0, sizeof(struct AudioPlayerMp3NameList));
	}

	if ((SDC_STAT_NORMAL == SysCtrl.sdcard) || (SDC_STAT_FULL == SysCtrl.sdcard)) // play sd file
	{
		audio_player_list.list_num = audioplayer_file_list_scan((u8 *)FILEDIR_MP3);
		deg_Printf("audio_player_list.list_num=%d\n", audio_player_list.list_num);

		//==debug name==
		for (i = 0; i < audio_player_list.list_num; i++)
			deg_Printf("list_name[%d]=%s\n", i, audio_player_list.file_list->name[i]);
	}
	audio_player_list.list_show_en = 1;
	audio_player_list.list_show_time = AUDIOPLAYER_LIST_SHOW_TIME;
	//==play mp3==
	// audioplayer_play_mp3();
	AudioPlayer_MusicList_Refresh(handle);
	//==osd show==
	audioPlayerSDShow(handle);
	audioPlayerBaterryShow(handle);
	if (MP3_DAC_START == mp3_play_sta())
	{
		audioPlayerStaShow(handle, 1);
	}
	else
	{
		audioPlayerStaShow(handle, 0);
	}
	audioPlayerVolumeShow(handle, 1);
	audioPlayerTimeShow(handle);
	audioPlayerFileListShow(handle);

	return 0;
}

static int audioPlayerCloseWin(winHandle handle, uint32 parameNum, uint32 *parame)
{
	deg_Printf("audioPlayer Close Win!!!\n");
	mp3_flag = 0;
	if (NULL != audio_player_list.file_list)
	{
		hal_sysMemFree(audio_player_list.file_list);
		audio_player_list.file_list = NULL;
	}
	if (NULL != nename)
	{
		hal_sysMemFree(nename);
		nename = NULL;
	}
	if (NULL != audioplayer_bk_buf)
	{
		hal_sysMemFree(audioplayer_bk_buf);
		audioplayer_bk_buf = NULL;
	}

#if AUDIO_PLAY_FLASH_DEMO
	if (NULL != audio_demo_buf)
	{
		hal_sysMemFree(audio_demo_buf);
		audio_demo_buf = NULL;
	}
#endif

	if (audio_player_list.cur_file >= 0)
	{
		close(audio_player_list.cur_file);
		audio_player_list.cur_file = -1;
	}

	//	if(SysCtrl.mp3_font_use)
	//	{
	//		SysCtrl.mp3_font_use=0;
	//		MP3_unicode_font_uninit();
	//	}

	Ebook_Uninit();

	return 0;
}

static int audioPlayerSysMsgUSB(winHandle handle, uint32 parameNum, uint32 *parame)
{
	audioPlayerBaterryShow(handle);
	return 0;
}

static int audioPlayerSysMsgBattery(winHandle handle, uint32 parameNum, uint32 *parame)
{
	if (SysCtrl.usb == USB_STAT_NULL)
		audioPlayerBaterryShow(handle);
	return 0;
}

static int audioPlayerSysMsg500MS(winHandle handle, uint32 parameNum, uint32 *parame)
{
	if (MP3_DAC_START == mp3_play_sta())
	{
		audioPlayerTimeShow(handle);
		audioPlayerFileBkShow(handle);
	}

	if (mp3_flag2)
	{
		mp3_flag2 = 0;
		// audioPlayerSysMsgdele(handle);
	}
	return 0;
}

static int audioPlayerSysMsg1S(winHandle handle, uint32 parameNum, uint32 *parame)
{
	if (SysCtrl.usb == USB_STAT_DCIN)
	{
		audioPlayerBaterryShow(handle);
	}
	return 0;
}

static int audioPlayerSysMsgPlayFinish(winHandle handle, uint32 parameNum, uint32 *parame)
{
	u8 ret = 0;
	if (parameNum == 1)
		ret = parame[0];

	deg_Printf("audioPlayerSysMsgPlayFinish:%d,%d\n", parameNum, ret);
	if (ret)
	{
		if (audio_player_list.list_num > 0)
		{
			if (audio_player_list.list_idx < audio_player_list.list_num - 1)
			{
				audio_player_list.list_idx++;
			}
			else
			{
				audio_player_list.list_idx = 0;
			}

			audioplayer_play_mp3();
			audioPlayerStaShow(handle, 1);

			// show  auto list follow
			audio_player_list.list_show_time = AUDIOPLAYER_LIST_SHOW_TIME;
			audioPlayerFileListShow(handle);
		}
		else
		{
			audioPlayerTimeShow(handle);
			audioPlayerStaShow(handle, 0);
		}
	}
	return 0;
}

static int playCount = 0;
static int audioPlayerSysMsgPlay(winHandle handle, uint32 parameNum, uint32 *parame)
{
	//	u8 ret = 0;
	//	if(parameNum == 1)
	//		ret = parame[0];

	playCount++;
	if (playCount > 5)
	{
		playCount = 0;
		if (MP3_DAC_START == mp3_play_sta())
		{
			audioPlayerFileBkShow(handle);
		}
	}
	return 0;
}
msgDealInfor audioPlayerMsgDeal[] =
	{
		{SYS_OPEN_WINDOW, audioPlayerOpenWin},
		{SYS_CLOSE_WINDOW, audioPlayerCloseWin},
		{KEY_EVENT_UP, audioPlayerKeyMsgUp},
		{KEY_EVENT_DOWN, audioPlayerKeyMsgDown},
		{KEY_EVENT_MENU, audioPlayerKeyMsgLeft},
		{KEY_EVENT_MODE, audioPlayerKeyMsgRight},
		{KEY_EVENT_OK, audioPlayerKeyMsgOk},
		{KEY_EVENT_POWER, audioPlayerKeyMsgExit},
		{SYS_EVENT_500MS, audioPlayerSysMsg500MS},
		{SYS_EVENT_1S, audioPlayerSysMsg1S},
		{SYS_EVENT_SDC, audioPlayerSysMsgSD},
		{SYS_EVENT_USB, audioPlayerSysMsgUSB},
		{SYS_EVENT_BAT, audioPlayerSysMsgBattery},
		{MP3_EVENT_PLAY_FINISH, audioPlayerSysMsgPlayFinish},
		// {MP3_EVENT_PLAY, audioPlayerSysMsgPlay},
		{EVENT_MAX, NULL},
};

WINDOW(audioPlayerWindow, audioPlayerMsgDeal, audioPlayerWin)
