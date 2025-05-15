#ifndef __GIF_H__
#define __GIF_H__

#define MAX_GIF_FRAMES 64 // 最大支持32帧
#define GIF_LCD_WIDTH 320
#define GIF_LCD_HEIGHT 240
#define GIF_DEGBUG 0

typedef struct
{
    u16 width;                      // 宽度
    u16 height;                     // 高度
    u16 delay_time[MAX_GIF_FRAMES]; // 单位：10ms
    u32 frame_offset[MAX_GIF_FRAMES];
} GIF_Header;

typedef struct
{
    GIF_Header header; // GIF头部信息
    u8 mode;           // 播放模式
    u8 *gif_data;      // GIF数据指针
    u16 frame_count;   // 帧数
    u32 res_id;        // 资源ID
    u32 last_update;   // 上次更新时间
    u32 gif_addr;      // GIF地址
    u32 offset;        // 偏移量
    u32 size;          // GIF大小
    u8 *gif_bk;
} GIF_Context;
// 添加YUV420颜色结构体定义
typedef struct yuv_420
{
    uint8_t y; // 亮度分量
    uint8_t u; // 色度U分量
    uint8_t v; // 色度V分量
} yuv_420;
// 初始化GIF（传入NVFS资源ID）
int gif_play(u32 res_id, u8 *gif_bk, int posx, int posy, uint8_t color_check, int gif_music_id);
#endif