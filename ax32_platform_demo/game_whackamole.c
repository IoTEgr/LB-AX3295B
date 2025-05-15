#include "application.h"
#include "taskgame.h"
extern u8 *first_y_buf; // 保存第一帧的Y分量数据 和 UV分量数据
static u8 *curgif_bk_y_buf, *curgif_bk_uv_buf;
static u32 game_whackamole_time, game_whackamole_score, timst, hamstertimer;
uint8_t ratLocation_x[9] = {50, 145, 240, 62, 144, 226, 70, 145, 215};
uint8_t ratLocation_y[9] = {162, 162, 162, 103, 103, 103, 62, 62, 62};
uint8_t hammerPosition, ratPosition, prehammerPosition = 9, preratPosition = 9; // 锤子位置，老鼠位置;

void game_whackamole_bk_add(u16 x, u16 y, u16 w, u16 h, u32 res_id)
{
    u8 *rgb24_buf;
    int i, j, r, g, b, offsetx, offsety;
    rgb24_buf = hal_sysMemMalloc(w * h * 3, 32);
    u32 addr, size;
    u32 line_align;
    line_align = (w * 3 + 3) & ~0x3; // 4bytes align

    addr = nv_open(res_id);
    if (addr < 0)
    {
        deg_Printf("read id err\n");
        return 1;
    }
    size = nv_size(res_id);
    if (size < 54)
    {
        deg_Printf("size err\n");
        return 1;
    }
    size = size - 54; // offset bmp header

    if (size > line_align * h) // not over rgb_buf size
    {
        size = line_align * h;
    }
    else if (size < w * h * 3)
    {
        deg_Printf("bmp format err?\n");
    }

    nv_read(addr + 54, rgb24_buf, size); // offset bmp header

    for (i = 0; i < h; i++)
    {
        // BMP存储是从下到上的，所以需要反转行
        int bmp_row = h - 1 - i;
        for (j = 0; j < w; j++)
        {
            // 计算在BMP数据中的正确位置（考虑行对齐）
            int bmp_offset = bmp_row * line_align + j * 3;

            // 计算在目标YUV缓冲区中的位置（不需要再次反转）
            offsetx = j + x;
            offsety = i + y;

            // 读取RGB值
            b = rgb24_buf[bmp_offset];
            g = rgb24_buf[bmp_offset + 1];
            r = rgb24_buf[bmp_offset + 2];

            // 检查是否为透明色（灰色）
            if (r == 0x80 && g == 0x80 && b == 0x80)
                continue;

            // BT.601标准系数 + 分量钳位
            int32_t y_val = (77 * r + 150 * g + 29 * b + 128) >> 8;
            y_val += 16;
            y_val = (y_val < 16) ? 16 : ((y_val > 235) ? 235 : y_val); // Y分量钳位

            // 确保目标坐标在有效范围内
            if (offsetx >= 0 && offsetx < TFT_WIDTH && offsety >= 0 && offsety < TFT_HEIGHT)
            {
                curgif_bk_y_buf[offsety * TFT_WIDTH + offsetx] = (uint8_t)y_val;
            }

            // 仅处理偶数坐标点的UV分量（NV21格式）
            if ((offsetx % 2 == 0) && (offsety % 2 == 0) &&
                (offsetx / 2 >= 0) && (offsetx / 2 < GIF_LCD_WIDTH / 2) &&
                (offsety / 2 >= 0) && (offsety / 2 < GIF_LCD_HEIGHT / 2))
            {
                // 计算UV分量并钳位
                int32_t u_val = (-43 * r - 84 * g + 127 * b + 128) >> 8;
                int32_t v_val = (127 * r - 106 * g - 21 * b + 128) >> 8;
                u_val += 128;
                v_val += 128;

                // 确保UV在0-255范围内
                uint8_t u = (uint8_t)((u_val < 0) ? 0 : ((u_val > 255) ? 255 : u_val));
                uint8_t v = (uint8_t)((v_val < 0) ? 0 : ((v_val > 255) ? 255 : v_val));

                // 计算UV缓冲区索引（半分辨率）
                const int uv_x = offsetx / 2;
                const int uv_y = offsety / 2;
                const int uv_index = uv_y * (GIF_LCD_WIDTH / 2) + uv_x;

                // 存储为VU顺序（NV21格式）
                curgif_bk_uv_buf[uv_index * 2] = v;
                curgif_bk_uv_buf[uv_index * 2 + 1] = u;
            }
        }
    }
    hal_sysMemFree(rgb24_buf);
}
// 随机数生成器内部状态
static unsigned int seed = 1234;

// 生成0-32767之间的随机数
int my_rand()
{
    // 使用ANSI C推荐的参数
    const unsigned int a = 1103515245;
    const unsigned int c = 12345;
    const unsigned int m = 2147483648; // 2^31

    seed = (a * seed + c) % m;
    return (int)(seed / 65536); // 返回0-32767之间的值
}

// 生成0-8之间的随机数
int generate_random_number_0_to_8()
{
    return my_rand() % 9;
}
void game_whackamole_init()
{
    deg_Printf("game_whackamole_init mem %dKB\n", hal_sysMemRemain() / 1024);
    sleeptime = 20;
    prehammerPosition = 9;
    preratPosition = 9;
    hammerPosition = 0;
    ratPosition = 0;
    game_whackamole_time = 90;
    game_whackamole_score = 0;
    game_bk_draw((u8 *)get_game_base_buf(), RES_GAME_WHACKAMOLE_BK, NULL);
    setting_frame_buff = get_game_show_buf(TRUE);
    frame_buff_display();
    memcpy(first_y_buf, (u8 *)setting_frame_buff, TFT_WIDTH * TFT_HEIGHT * 3 / 2);
    curgif_bk_y_buf = hal_sysMemMalloc(TFT_WIDTH * TFT_HEIGHT * 3 / 2, 32);
    curgif_bk_uv_buf = curgif_bk_y_buf + TFT_WIDTH * TFT_HEIGHT;
    memcpy(curgif_bk_y_buf, first_y_buf, TFT_WIDTH * TFT_HEIGHT * 3 / 2);
}
void game_whackamole_add_time()
{
    if (game_whackamole_time >= 10)
    {
        game_whackamole_bk_add(150, 0, 16, 32, RES_NUM_0 + game_whackamole_time / 10);
        game_whackamole_bk_add(170, 0, 16, 32, RES_NUM_0 + game_whackamole_time % 10);
    }
    else
    {
        game_whackamole_bk_add(160, 0, 16, 32, RES_NUM_0 + game_whackamole_time % 10);
    }
}
void game_whackamole_add_score()
{
    if (game_whackamole_score >= 10)
    {
        game_whackamole_bk_add(153, 22, 16, 32, RES_NUM_0 + game_whackamole_score / 10);
        game_whackamole_bk_add(173, 22, 16, 32, RES_NUM_0 + game_whackamole_score % 10);
    }
    else
    {
        game_whackamole_bk_add(163, 22, 16, 32, RES_NUM_0 + game_whackamole_score % 10);
    }
}
void game_whackamole_lcd_refresh()
{
    disp_frame_t *p_lcd_buffer;
    p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
    if (p_lcd_buffer)
        hal_dispframeVideoCfg(p_lcd_buffer, 0, 0, p_lcd_buffer->w, p_lcd_buffer->h);
    memcpy(p_lcd_buffer->y_addr, curgif_bk_y_buf, p_lcd_buffer->w * p_lcd_buffer->h);
    memcpy(p_lcd_buffer->uv_addr, curgif_bk_uv_buf, p_lcd_buffer->w * p_lcd_buffer->h / 2);
    ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr, p_lcd_buffer->w * p_lcd_buffer->h * 3 / 2);
    dispLayerShow(DISP_LAYER_VIDEO, (INT32U)p_lcd_buffer, 0, 0, VIDEO_ROTATE);
}
void game_whackamole_refresh()
{
    memcpy(curgif_bk_y_buf, first_y_buf, TFT_WIDTH * TFT_HEIGHT * 3 / 2);
    game_whackamole_bk_add(ratLocation_x[ratPosition], ratLocation_y[ratPosition], 40, 35, RES_GAME_WHACKAMOLE_APPEAR_STATIC);
    game_whackamole_bk_add(ratLocation_x[hammerPosition] + 15, ratLocation_y[hammerPosition] - 40, 53, 43, RES_GAME_WHACKAMOLE_STRIKE_STATIC);
    game_whackamole_add_time();
    game_whackamole_add_score();
    game_whackamole_lcd_refresh();
}
void game_whackamole_exit()
{
    memcpy(curgif_bk_y_buf, first_y_buf, TFT_WIDTH * TFT_HEIGHT * 3 / 2);
    game_whackamole_bk_add(81, 68, 158, 104, RES_GAME_WHACKAMOLE_END);
    if (game_whackamole_score >= 10)
    {
        game_whackamole_bk_add(175, 118, 16, 32, RES_NUM_0 + game_whackamole_score / 10);
        game_whackamole_bk_add(195, 118, 16, 32, RES_NUM_0 + game_whackamole_score % 10);
    }
    else
    {
        game_whackamole_bk_add(185, 118, 16, 32, RES_NUM_0 + game_whackamole_score % 10);
    }
    game_whackamole_lcd_refresh();
    hal_wdtClear();
    XOSTimeDly(3000);
    endGamestatus = 0;
    sleeptime = 60;
    hal_sysMemFree(curgif_bk_y_buf);
    prehammerPosition = 9;
    preratPosition = 9;
    deg_Printf("game_whackamole_exit mem %dKB\n", hal_sysMemRemain() / 1024);
}
void game_whackamole_entry(void)
{
    u8 msg, err;
    game_whackamole_init();
    if (!game_tips(R_ID_STR_GAME_TIPS_START))
    {
        game_whackamole_exit();
        return;
    }
    timst = XOSTimeGet();
    hamstertimer = XOSTimeGet();
    while (1)
    {
        if ((XOSTimeGet() - timst) / 1000)
        {
            game_whackamole_time -= (XOSTimeGet() - timst) / 1000;
            timst = XOSTimeGet();
        }
        if ((XOSTimeGet() - hamstertimer) / 700 >= 2)
        {
            while (preratPosition == ratPosition)
                ratPosition = generate_random_number_0_to_8();
            hamstertimer = XOSTimeGet();
        }
        if (game_whackamole_time <= 0)
        {
            game_whackamole_exit();
            return;
        }
        msg = (u8)XMsgQPend(SysCtrl.sysQ, &err);
        switch (msg)
        {
        case KEY_EVENT_OK:
            if (hammerPosition == ratPosition)
            {
                game_whackamole_score++;
                memcpy(curgif_bk_y_buf, first_y_buf, TFT_WIDTH * TFT_HEIGHT * 3 / 2);
                game_whackamole_add_time();
                game_whackamole_add_score();
                gif_play(RES_GAME_WHACKAMOLE_STRIKE, curgif_bk_y_buf, ratLocation_x[ratPosition] + 5, ratLocation_y[ratPosition] - 35, 1, -1);
                while (preratPosition == ratPosition)
                    ratPosition = generate_random_number_0_to_8();
                hamstertimer = XOSTimeGet();
                game_audio_play(RES_GAME_BLOCK_KNOCK, 0, 0);
            }
            else
            {
                memcpy(curgif_bk_y_buf, first_y_buf, TFT_WIDTH * TFT_HEIGHT * 3 / 2);
                game_whackamole_add_time();
                game_whackamole_add_score();
                game_whackamole_bk_add(ratLocation_x[ratPosition], ratLocation_y[ratPosition], 40, 35, RES_GAME_WHACKAMOLE_APPEAR_STATIC);
                gif_play(RES_GAME_WHACKAMOLE_STRIKE_FALSE, curgif_bk_y_buf, ratLocation_x[hammerPosition] + 5, ratLocation_y[hammerPosition] - 35, 1, -1);
            }
            game_whackamole_refresh();
            break;
        case KEY_EVENT_POWER:
            if (game_tips(R_ID_STR_GAME_TIPS_EXIT))
            {
                game_whackamole_exit();
                return;
            }
            break;
        case KEY_EVENT_UP:
            hammerPosition = (hammerPosition + 3) % 9;
            break;
        case KEY_EVENT_DOWN:
            hammerPosition = (hammerPosition - 3 + 9) % 9; // 修复负数问题
            break;
        case KEY_EVENT_LEFT:
            // 行内左循环：0→2, 3→5, 6→8
            hammerPosition = hammerPosition - (hammerPosition % 3 == 0 ? -2 : 1);
            break;
        case KEY_EVENT_RIGHT:
            // 行内右循环：2→0, 5→3, 8→6
            hammerPosition = hammerPosition + (hammerPosition % 3 == 2 ? -2 : 1);
            break;
        case KEY_EVENT_SETTING:
            break;
        case SYS_EVENT_GAME_TICK:
            break;
        }
        if (hammerPosition != prehammerPosition || ratPosition != preratPosition)
        {
            if (ratPosition != preratPosition)
            {
                memcpy(curgif_bk_y_buf, first_y_buf, TFT_WIDTH * TFT_HEIGHT * 3 / 2);
                game_whackamole_add_time();
                game_whackamole_add_score();
                game_whackamole_bk_add(ratLocation_x[hammerPosition] + 15, ratLocation_y[hammerPosition] - 40, 53, 43, RES_GAME_WHACKAMOLE_STRIKE_STATIC);
                gif_play(RES_GAME_WHACKAMOLE_APPEAR, curgif_bk_y_buf, ratLocation_x[ratPosition], ratLocation_y[ratPosition], 1, -1);
            }
            else
                game_whackamole_refresh();
            prehammerPosition = hammerPosition;
            preratPosition = ratPosition;
        }
    }
}
