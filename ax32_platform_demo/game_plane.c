#include "application.h"
#include "taskgame.h"


extern INT32U setting_frame_buff;
extern INT32U Home_Background_Frame;
extern INT16U sleeptime;

#define RECT_W 20
#define RECT_H 20
INT16U fps_delay = 0;
INT16U fps_panle = 2;
INT16U fps_bullet = 6;
INT16U fps_lvel = 20;//4//
INT32U dfj_score = 0;
void key_read();

/***********************************/


//#define TFT_GAME_WIDTH TFT_HEIGHT
//#define TFT_GAME_HEIGHT TFT_WIDTH
#define MY_PLANE_W 25
#define MY_PLANE_H 30

#define ENEMY_PLANE_W 28
#define ENEMY_PLANE_H 20

#define BULLET_W 6
#define BULLET_H 14

#define P_BOOM_W 38
#define P_BOOM_H 32

#define MAP_EMPTY			0
#define MAP_ENEMY_PALNE		1
#define MAP_MY_PLANE		2
#define MAP_BULLET			3
#define MAP_BOOM            4


#define icon_boom		0
#define icon_bullet		1824
#define icon_enemy_blue		1950
#define icon_enemy_green		2790
#define icon_enemy_red		3630
#define icon_plane		4470



enum {
	KEY_RIGHT,
	KEY_LEFT,
    KEY_UP,
    KEY_DOWN,
 
};
typedef struct {
    INT16S x;
    INT16S y;
    INT16S w;
    INT16S h;
    INT16U move_speed;
    INT16S HP;
    void(*move_func)(struct Plane_c *p,INT8U fx);
}Plane_c;





void pGame_show_icon_rect(Plane_c *c,INT16U type);
void new_p_boom(INT16U x,INT16U y);

static u8 *dfj_icon = 0;
static u8 *game_plane_sound = 0;
static s32 game_plane_sound_size = 0;

Plane_c my_Plane_c;

#define BULLET_MAX 10
Plane_c my_bullet_c[BULLET_MAX];

Plane_c enemy_Plane_c[BULLET_MAX];
Plane_c enemy_bullet_c[BULLET_MAX];

Plane_c P_boom[BULLET_MAX];

#define LIMIT(x,a,b) {if(x<a) x = a; if(x>b) x = b;}

/*
 * return 1  ¡À¨ª¨º?¨ºy?Y???¨²?????¨²
 *
*/
INT8U is_in_range(INT16S data,INT16S min,INT16S max)
{
    if(data>min&&data<max)
        return 1;
    else
        return 0;
}
/*
 *return 1  ??¡Á2
 */
INT8U collision_chect( Plane_c *a, Plane_c *b)
{
	INT8U co = 0;
	if(a->HP<=0||b->HP<=0) return 0;
	if(is_in_range(a->x, b->x, b->x+b->h)||is_in_range(a->x+a->h, b->x, b->x+b->h)||
			is_in_range(b->x, a->x, a->x+a->h)||is_in_range(b->x+b->h, a->x, a->x+a->h))
		co++;
	if(is_in_range(a->y, b->y, b->y+b->w)||is_in_range(a->y+a->w, b->y, b->y+b->w)||
			is_in_range(b->y, a->y, a->y+a->w)||is_in_range(b->y+b->w, a->y, a->y+a->w))
		co++;
	if(co==2)
		return 1;
	else
		return 0;
}

/*
 *¨°??¡¥¡Á??o¦Ì?¡Á¨®¦Ì¡¥
 */
void move_my_bullet_c()
{
    INT8U i,j;
    for(i = 0; i< BULLET_MAX; i++){
		 my_bullet_c[i].x-=my_bullet_c[i].move_speed;
        for(j = 0; j< BULLET_MAX; j++){
           
            if(collision_chect(&enemy_Plane_c[j],&my_bullet_c[i]))
            {
                if(enemy_Plane_c[j].HP) {
                    enemy_Plane_c[j].HP--;
                    if(enemy_Plane_c[j].HP == 0)
                        new_p_boom(enemy_Plane_c[j].x,enemy_Plane_c[j].y);
                }
                my_bullet_c[i].HP = 0;
            }
            if(my_bullet_c[i].x<=0)
                my_bullet_c[i].HP = 0;
        }
    }
}

void show_plane_boom()
{
    INT8U i = 0;
 
    for(i = 0; i< BULLET_MAX; i++){
        if(P_boom[i].HP){
            pGame_show_icon_rect(&P_boom[i],MAP_BOOM);
            P_boom[i].HP--;
        }
    }
}
/*
 *¨°??¡¥¦ÌD¡¤??o¦Ì?¡Á¨®¦Ì¡¥
 */
void move_enemy_bullet_c()
{
    INT8U i,j;
    for(i = 0; i< BULLET_MAX; i++){
        enemy_bullet_c[i].x+=enemy_bullet_c[i].move_speed;
        if(collision_chect(&my_Plane_c,&enemy_bullet_c[i]))
        {
            my_Plane_c.HP = 0;
            enemy_bullet_c[i].HP = 0;
        }
        if(enemy_bullet_c[i].y+enemy_bullet_c[i].h>=TFT_WIDTH)
            enemy_bullet_c[i].HP = 0;

    }
}
/*¨°??¡¥¡Á¨®¦Ì¡¥*/
void move_bullet()
{
    move_my_bullet_c();
    move_enemy_bullet_c();
}
/**¦Ì?¡¤?¡¤¨¦?¨²ai**/
void AI_mage(Plane_c *p)
{
    if(p->y+p->w<my_Plane_c.y)
    {
        p->y += 4;//p->move_speed;
    }
    else if(p->y>my_Plane_c.y + my_Plane_c.w)
    {
        p->y -= 4;//p->move_speed;
    }

}


static void game_plane_over()
{
	XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_POWER,0));
}

void move_one_enemy_planle( Plane_c *p,INT8U fx)
{
    if(p->HP == 0) return;
    if(fx == KEY_UP)
        p->y-=p->move_speed;
    else if(fx == KEY_DOWN)
        p->y+=p->move_speed;
    if(fx == KEY_LEFT)
        p->x-=p->move_speed;
    else if(fx == KEY_RIGHT)
        p->x+=p->move_speed;
    AI_mage(p);
    LIMIT(p->x,0, TFT_WIDTH - p->h - 1);
    LIMIT(p->y,0, TFT_HEIGHT - p->w -1);
    //¡Á¨®¦Ì¡¥ ??¡Á2?¨¬2a
    {
        INT8U i;
        for(i = 0; i< BULLET_MAX; i++){
            if(collision_chect(p,&my_bullet_c[i]))
            {
                my_bullet_c[i].HP = 0;
                if(p->HP){
                    p->HP--;
                    if(p->HP == 0)
                        new_p_boom(p->x,p->y);
                }
            }
        }
    }
    //¦ÌD¡¤¨¦?¨² ??¡Á2?¨¬2a
    if(collision_chect(p,&my_Plane_c))
    {
       my_Plane_c.HP = 0;
       game_plane_over();
       return;
    }
    if(p->x+p->h >= TFT_WIDTH - 2)
        p->HP = 0;
}


/*
 * ¨°??¡¥¦ÌD¨¨?????¦Ì?¡¤¨¦?¨²
*/
void move_enemy_planle()
{
    INT8U i;
    for(i = 0; i<BULLET_MAX;i++)
    {
        move_one_enemy_planle(&enemy_Plane_c[i],KEY_RIGHT);
    }
}


/*
 * ¨°??¡¥¨®??¡¤????¦Ì?¡¤¨¦?¨²
*/
void move_my_planle( Plane_c *p,INT8U fx)
{
    if(p->HP == 0) return;
    if(fx == KEY_RIGHT)
        p->y+=p->move_speed;
    else if(fx == KEY_LEFT)
        p->y-=p->move_speed;
    if(fx == KEY_DOWN)
        p->x-=p->move_speed;
    else if(fx == KEY_UP)
        p->x+=p->move_speed;
    LIMIT(p->x,0, TFT_WIDTH - p->h - 1);
    LIMIT(p->y,0, TFT_HEIGHT - p->w - 1);
    //¡Á¨®¦Ì¡¥ ??¡Á2?¨¬2a
    {
        INT8U i;
        for(i = 0; i< BULLET_MAX; i++){
            if(collision_chect(p,&enemy_bullet_c[i]))
            {
                p->HP = 0;
                game_plane_over();

            }
        }
    }
}
/*
 *¡ä¡ä?¡§¡¤¨¦?¨²¦Ì?¡Á¨®¦Ì¡¥
 */
void new_my_bullet()
{
    INT8U i;
    for(i = 0; i< BULLET_MAX; i++)
    {
        if(my_bullet_c[i].HP == 0)
         {
            my_bullet_c[i].w = BULLET_W;
            my_bullet_c[i].h = BULLET_H;
            my_bullet_c[i].move_speed = BULLET_H;
            my_bullet_c[i].HP = 1;
            my_bullet_c[i].x = my_Plane_c.x - 1 - my_bullet_c[i].h;
            my_bullet_c[i].y = my_Plane_c.y+my_Plane_c.w/2-my_bullet_c[i].w/2;
            break;
        }
    }
}

/*
 *¡ä¡ä?¡§¡¤¨¦?¨²¡À??¡§
 */
void new_p_boom(INT16U x,INT16U y)
{
    INT8U i;
    for(i = 0; i< BULLET_MAX; i++)
    {
        if(P_boom[i].HP == 0)
         {
            P_boom[i].w = P_BOOM_W;
            P_boom[i].h = P_BOOM_H;
            P_boom[i].move_speed = 0;
            P_boom[i].HP = 10;
            P_boom[i].x = x;
            P_boom[i].y = y;
            dfj_score+=1;
			
			deamon_sound_play((u32)game_plane_sound, game_plane_sound_size);
			//audio_effect_play(EFFECT_GAME_BOOM);//.................................................................
            break;
        }
    }
}
/*
 *¡ä¡ä?¡§¦ÌD¡¤?¡¤¨¦?¨²¦Ì?
 */
void new_enemy_Plane()
{
    INT8U i;
    for(i = 0; i< BULLET_MAX; i++)
    {
        if(enemy_Plane_c[i].HP == 0)
         {
            enemy_Plane_c[i].w = ENEMY_PLANE_W;
            enemy_Plane_c[i].h = ENEMY_PLANE_H;
            enemy_Plane_c[i].move_speed = 8;
            enemy_Plane_c[i].HP = rand_int()%3+1;
            enemy_Plane_c[i].x = 0;
            enemy_Plane_c[i].y = rand_int()%(TFT_HEIGHT - enemy_Plane_c[i].w);
            break;
        }
    }
}


static void game_plane_init()
{
	INT8U i;
    my_Plane_c.w = MY_PLANE_W;
    my_Plane_c.h = MY_PLANE_H;
    my_Plane_c.x = TFT_WIDTH - my_Plane_c.h;
    my_Plane_c.y = (TFT_HEIGHT - my_Plane_c.w)/2;
    my_Plane_c.HP = 1;
    my_Plane_c.move_speed = 25;
    endGamestatus = 0;
    sleeptime = 200;
	for(i = 0; i< BULLET_MAX; i++)
    {
        enemy_Plane_c[i].HP = 0;
		enemy_bullet_c[i].HP = 0;
		P_boom[i].HP = 0;
		my_bullet_c[i].HP = 0;
	}
	dfj_score = 0;
    dfj_icon = get_bin_data(RES_GAME_PLANE_ICON);
	game_bk_draw((u8 *)get_game_base_buf(), NULL, 0xff8080);
	game_plane_sound = layout_sound_load(RES_GAME_PLANE_AUDIO, &game_plane_sound_size);
}

void pGame_show_icon_rect(Plane_c *c,INT16U type)
{
    INT16U pox,poy;
    u8 * p, *p_icon;
    u16 *p_uv, *p_icon_uv;
    INT16S x = c->x, y = c->y;
    INT16U w = c->w, h = c->h;
	u32 offset = 0;
    if(type == MAP_ENEMY_PALNE)
    {
        if(c->HP==3)
            p_icon = dfj_icon+icon_enemy_red;
        else if(c->HP==2)
            p_icon = dfj_icon+icon_enemy_green;
        else
            p_icon = dfj_icon+icon_enemy_blue;
    }
    else if(type == MAP_MY_PLANE)
        p_icon = dfj_icon+icon_plane;
    else if(type == MAP_BULLET){
        p_icon = dfj_icon+icon_bullet;
		x = (x + 0x1)&(~0x1);
		y = (y + 0x1)&(~0x1);
	}
    else if(type == MAP_BOOM)
        p_icon = dfj_icon+icon_boom;
    else
        p_icon = dfj_icon+icon_enemy_blue;
	p_icon_uv = (u16 *)(p_icon + w*h);
    p = (u8 *)setting_frame_buff;
	p_uv = (u16 *)(p + TFT_WIDTH*TFT_HEIGHT);
	{
		u32 i, j, offset_pixel, offset_pixel_uv, offset_data, offset_data_uv;
		u16 tft_w = TFT_WIDTH, tft_h = TFT_HEIGHT, tft_w_uv = ((tft_w + 0x1)&(~0x1)) >> 1, tft_h_uv = ((tft_h + 0x1)&(~0x1)) >> 1, icon_w_uv = ((w + 0x1)&(~0x1)) >> 1, icon_h_uv = ((h  + 0x1)&(~0x1)) >> 1;

		for(i = 0; i < h; i++){
			for(j = 0; j < w; j++){
				offset_data = i * w;
				offset_pixel = (y + j) * tft_w + x;
				offset_data_uv = (i >> 1) * icon_w_uv;
				offset_pixel_uv = (((y + j) >> 1) * tft_w_uv);
				
				if(0xff8080 == (*(p_icon + offset_data + j) << 16) + *(p_icon_uv + offset_data_uv + (j >> 1)))
					continue;
				*(p + offset_pixel + i) = *(p_icon + offset_data + j);
				*(p_uv + offset_pixel_uv + ((x + i) >> 1)) = *(p_icon_uv + offset_data_uv + (j >> 1));
			}
		}
	}


    /*for(pox = x;pox < x+h;pox++)
    {
        for(poy = y;poy < y+w;poy++)
        {
       		*(p+pox+poy*TFT_WIDTH) = *(p_icon++);
        }
        
    }*/
}
void show_enemy_Plane()
{
    INT8U i;
    for(i = 0; i< BULLET_MAX; i++)
    {
        if(enemy_Plane_c[i].HP)
            pGame_show_icon_rect(&enemy_Plane_c[i],MAP_ENEMY_PALNE);
    }
}
void show_my_bullet()
{
    INT8U i;
    for(i = 0; i< BULLET_MAX; i++)
    {
        if(my_bullet_c[i].HP)
            pGame_show_icon_rect(&my_bullet_c[i],MAP_BULLET);
    }
}
void show_score()
{
	int8_t i;
	INT16U pos_x_shift = 0;
	INT32U buf_show = setting_frame_buff;
	
	pos_x_shift += (R_drawAsciiChar((unsigned short *)buf_show, 48 + dfj_score/1000%10,pos_x_shift,0, 320, 0, 0, R_FONT_DEFAULT) & 0xff);
	pos_x_shift += (R_drawAsciiChar((unsigned short *)buf_show, 48 + dfj_score/100%10,pos_x_shift,0, 320, 0, 0, R_FONT_DEFAULT) & 0xff);
	pos_x_shift += (R_drawAsciiChar((unsigned short *)buf_show, 48 + dfj_score/10%10,pos_x_shift,0, 320, 0, 0, R_FONT_DEFAULT) & 0xff);
	pos_x_shift += (R_drawAsciiChar((unsigned short *)buf_show, 48 + dfj_score%10,pos_x_shift,0, 320, 0, 0, R_FONT_DEFAULT) & 0xff);
}



static void show_all()
{
    setting_frame_buff = get_game_show_buf(TRUE);
	show_plane_boom();
    show_enemy_Plane();
    show_my_bullet();
    pGame_show_icon_rect(&my_Plane_c,MAP_MY_PLANE);
    
	show_score();//ÏÔÊ¾µÃ·Ö
	frame_buff_display();
}
static void game_plane_key_scant(INT32U key){
    move_my_planle(&my_Plane_c,key);
    show_all();
}

static void game_plane_tick()
{
    static INT8U fps = 0;
	INT8U lv = 5;

    fps++;
	if(dfj_score>200)
		lv = 1;
	else if(dfj_score>150)
		lv = 2;
	else if(dfj_score>100)
		lv = 3;
	else if(dfj_score>50)
		lv = 4;
	else 
		lv = 5;

    if(fps%lv == 0)
        new_enemy_Plane();
    if(fps%2==0)
        new_my_bullet();
    move_enemy_planle();
    move_bullet();
    show_all();

}


/***********************************/


#define MAP_EMPTY			0
#define MAP_ENEMY_PALNE		1
#define MAP_MY_PLANE		2
#define MAP_BULLET			3


void game_plane_entry(void)
{
	u8 msg, err, temp = SysCtrl.f_keysound;
	
	SysCtrl.f_keysound=0;
	game_plane_init();
	game_plane_tick();
	
	//game_entry_animation();
	while(1){
		msg = (u8)XMsgQPend(SysCtrl.sysQ,&err); 
		switch (msg){
			case KEY_EVENT_UP:
				game_plane_key_scant(1);
				break;
			case KEY_EVENT_DOWN:
				game_plane_key_scant(0);
				break;
			case KEY_EVENT_MODE:
				game_plane_key_scant(2);
				break;
			case KEY_EVENT_MENU:
	    		game_plane_key_scant(3);
				break;
			case KEY_EVENT_POWEROFF:
				taskStart(TASK_POWER_OFF,0);
			case KEY_EVENT_POWER:
				endGamestatus = 1;
				free_buf(dfj_icon);
				free_buf(game_plane_sound);
				SysCtrl.f_keysound=temp;
				return;
			case SYS_EVENT_GAME_TICK:
				game_plane_tick();
				break;
		}
	}
}


