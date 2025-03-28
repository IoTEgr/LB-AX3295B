#include "application.h"
#include "taskgame.h"

void maze_DrawMap();
void show_maze_win();
void maze_show_all();
#define MAP_W 22
#define MAP_H 15

#define RECT_W 14//(TFT_HEIGHT/(MAP_W+2))
#define RECT_H 14//(TFT_HEIGHT/(MAP_H+2))


#define MAZE_MAP_EMPTY		4
#define MAZE_MAP_DESTINATION 2
#define MAZE_MAP_WALL		1
#define MAZE_MAP_BOX			6
#define MAZE_MAP_MAN			8
#define MAZE_MAP_BOX_SUCCESS	16
INT8U *maze_map[MAP_W];


extern void game_back_ground_ini_maze();

INT8U maze_row = 0, maze_column = 0;    // 人物在第几行，第几列
INT8U maze_oldType = 0;    // 人物站着的地方原来是什么地面
INT8U maze_oldBoxType = 0; // 箱子站着的地方原来是什么地面 
INT8U maze_leve = 0;


void Move_maze(INT8S r, INT8S c)
{
	if (maze_map[maze_row + r][maze_column + c] != MAZE_MAP_WALL)  // 如果前面不是墙壁的话，就可以移动
	{
		if (maze_map[maze_row + r][maze_column + c] == MAZE_MAP_BOX ||
			maze_map[maze_row + r][maze_column + c] == MAZE_MAP_BOX_SUCCESS)	// 如果遇到箱子的处理办法
		{
			if (maze_map[maze_row + 2 * r][maze_column + 2 * c] != MAZE_MAP_WALL &&
				maze_map[maze_row + 2 * r][maze_column + 2 * c] != MAZE_MAP_BOX &&
				maze_map[maze_row + 2 * r][maze_column + 2 * c] != MAZE_MAP_BOX_SUCCESS)   // 箱子后面不是墙壁和箱子
			{
				// 移动箱子
				if (maze_map[maze_row + r][maze_column + c] == MAZE_MAP_BOX_SUCCESS)
				{
					maze_oldBoxType = MAZE_MAP_DESTINATION;
				}
				else if (maze_map[maze_row + r][maze_column + c] == MAZE_MAP_BOX)
				{
					maze_oldBoxType = MAZE_MAP_EMPTY;
				}
				maze_map[maze_row + r][maze_column + c] = maze_oldBoxType;
				if (maze_map[maze_row + 2 * r][maze_column + 2 * c] == MAZE_MAP_DESTINATION)
				{
					maze_map[maze_row + 2 * r][maze_column + 2 * c] = MAZE_MAP_BOX_SUCCESS;
				}
				else
				{
					maze_map[maze_row + 2 * r][maze_column + 2 * c] = MAZE_MAP_BOX;
				}
			}
			else {
				return; // 如果箱子后面是墙壁，那么停止移动
			}
		}
		maze_map[maze_row][maze_column] = maze_oldType; // 将人物走过的地方设为原来的地面类型
		maze_oldType = maze_map[maze_row + r][maze_column + c]; // 存储人物接下来要走的地方的地面类型
		maze_map[maze_row + r][maze_column + c] = MAZE_MAP_MAN;   // 人移动的位置发生变化
	}
}

INT8U resultCheck_maze()
{
	INT8U result = TRUE, i, j;
	if(MAZE_MAP_MAN!= maze_map[MAP_W-3][MAP_H-1])
		result = FALSE;

	if (result)
	{
		//game_result_tips(R_ID_STR_GAME_TIPS_NEXT_LEVEL, 500, true, true); //show pass a barrier
		//if(maze_leve++>=10)
		//	maze_leve=0;
			if(maze_leve++>=9)//玩到第十关跳回第一关
			maze_leve=0;
}
	return result;
}

void maze_map_set(INT8U *data)
{

	INT8U i, j = 0;
	for (i = 0; i < MAP_W; i++)
	{
		maze_map[i] = (data + i * MAP_H );
	}
}
INT16U get_map_maze_size()
{
	return MAP_W * MAP_H;
}

void man_maze_init()
{
	maze_map[2][0] = MAZE_MAP_MAN;
}


void maze_map_init()
{
	INT8U* data;
	INT8U* data2;
	data = DataRead_CMode(RES_GAME_MAZE_MAP,get_map_maze_size() *maze_leve, get_map_maze_size());
	maze_map_set(data);
	man_maze_init();
	maze_show_all();


}


void maze_show_one_rect(INT16U x, INT16U y,INT8U type)
{
	INT16U pox,poy;

	DISPLAY_ICONSHOW icon;
	u32 color;

	if (type == MAZE_MAP_WALL)
		color = YUV_RED;
	else if (type == MAZE_MAP_EMPTY)
		color = YUV_BLACK;
	else if (type == MAZE_MAP_MAN)				
	{
		maze_row = x;
		maze_column = y;
		color = YUV_WHITE;
	}
	else
		color = YUV_BLACK;

	icon.icon_w = RECT_W;
	icon.icon_h = RECT_H;
	icon.transparent = TRANSPARENT_COLOR;
	icon.pos_x = x*RECT_W;
	icon.pos_y = y*RECT_H + 12;
	
	frame_icon_draw((u8 *)setting_frame_buff, NULL, &icon, color);
	
}

void maze_DrawMap() {
	INT8U x, y;
	for (x = 0; x < MAP_W; x++)
	{
		for (y = 0; y < MAP_H; y++)
		{
			maze_show_one_rect(x, y, maze_map[x][y]);
			
		}
	}

}

void maze_show_all()
{
	setting_frame_buff = get_game_show_buf(TRUE);
	maze_DrawMap();
	frame_buff_display();

}

void maze_keypress(INT8S r, INT8S c)
{

	Move_maze(r, c);
	maze_show_all();
	if(resultCheck_maze())
	{
		free_buf(maze_map[0]);
		maze_map_init();
	}
}

void maze_init()
{
	maze_leve = 0;
	game_bk_draw((u8 *)get_game_base_buf(), NULL, 0x8080);
	maze_map_init();
}


void game_maze_entry(void)
{
	u8 msg, err;
	maze_init();
	//game_entry_animation();
	while(1){
		msg = (u8)XMsgQPend(SysCtrl.sysQ,&err); 
		switch (msg){
			case KEY_EVENT_UP:
				maze_keypress(0, -1);
				break;
			case KEY_EVENT_DOWN:
				maze_keypress(0, 1);
				break;
			case KEY_EVENT_RIGHT:
				maze_keypress(1, 0);
				break;
			case KEY_EVENT_LEFT:
	    		maze_keypress(-1, 0);
				break;
			case KEY_EVENT_POWEROFF:
				taskStart(TASK_POWER_OFF,0);
			case KEY_EVENT_POWER:
				free_buf(maze_map[0]);
				return;
		}
	}
}

