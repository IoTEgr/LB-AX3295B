#ifndef __TASKGAME_H_
#define __TASKGAME_H_


#define TFT_WIDTH		320
#define TFT_HEIGHT		240

#define TRANSPARENT_COLOR 0x8c8080

#define RED_COLOR		0x4c5bff

enum{
GAME_KEY_NONE,
GAME_KEY_OK,
GAME_KEY_U,
GAME_KEY_D,
GAME_KEY_L,
GAME_KEY_R,
GAME_KEY_POWER,
GAME_KEY_P,
};

#define	YUV_BLACK	0x8080
#define	YUV_RED 	0x6C5BFF
#define	YUV_RED1 	0x4C5BFF
#define YUV_WHITE	0xFF8080

typedef struct
{
	INT16U	icon_w;
	INT16U	icon_h;
	INT32U	transparent;
	INT16S	pos_x;
	INT16S	pos_y;
} DISPLAY_ICONSHOW;

typedef void (*msgGameFunc)(void);

extern msgGameFunc gameEntry;
extern bool endGamestatus;
extern u32 setting_frame_buff;
extern u16 sleeptime;

extern void game_snake_entry(void);
extern void game_sokoban_entry(void);
extern void game_tetris_entry(void);
extern void game_block_entry(void);
extern void game_maze_entry(void);
extern void game_plane_entry(void);

#endif
  