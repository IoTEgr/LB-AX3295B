#include "application.h"
GIF_Context g_ctx = {0};
GifDecoder *decoder = NULL;
u8 *y_buf, *uv_buf;

#define GIF_PCM_BUFFER_SIZE_MAX (300 * 1024)
#define GIF_PCM_BUFFER_SIZE 512
#define GAME_AUDIO_NUM 2
static volatile u8 *gif_audio_buf;
static volatile u32 gif_audio_buf_pos;
static volatile u32 gif_audio_buf_size;
static volatile u32 gif_audio_id;
u8 gif_audio_init(int id)
{
    int fd, i;
    fd = nv_open(id);
    if (fd < 0)
    {
        return 1;
    }
    gif_audio_buf_size = nv_size(id);
    // deg_Printf("%d \n",system_audio_buf_size);
    if (gif_audio_buf_size <= WAV_HEADER_SIZE)
    {
        return 2;
    }
    gif_audio_buf_size = gif_audio_buf_size - WAV_HEADER_SIZE;
    gif_audio_buf_size = gif_audio_buf_size & (~0x1f); // 32bytes align
    if (gif_audio_buf_size >= GIF_PCM_BUFFER_SIZE_MAX)
    {
        deg_Printf("audio size over\n");
        gif_audio_buf_size = GIF_PCM_BUFFER_SIZE_MAX;
    }
    if (0 == gif_audio_buf)
    {
        gif_audio_buf = hal_sysMemMalloc(gif_audio_buf_size + HAL_CFG_PCM_BUFFER_SIZE, 64);
        if (0 == gif_audio_buf)
        {
            deg_Printf("audio mem over\n");
            return 0;
        }
        memset(gif_audio_buf, 0, gif_audio_buf_size + HAL_CFG_PCM_BUFFER_SIZE);
        ax32xx_sysDcacheFlush(gif_audio_buf, gif_audio_buf_size + HAL_CFG_PCM_BUFFER_SIZE);
        nv_read(fd + WAV_HEADER_SIZE, gif_audio_buf, gif_audio_buf_size);
    }
}

static void gif_audio_isr(int flag)
{
    INT8U err;
    if (flag & DAC_INT_PEND)
    {
        // uart_Printf("1");
    }
    if (flag & DAC_INT_HALF) // half finish
    {
        gif_audio_buf_pos += GIF_PCM_BUFFER_SIZE;
        if (gif_audio_buf_pos >= gif_audio_buf_size + GIF_PCM_BUFFER_SIZE) // play finish
        {
            ax32xx_dacVolumeSet(0);
        }
        else
        {
            hal_dacSetBuffer((u32)(gif_audio_buf + gif_audio_buf_pos), GIF_PCM_BUFFER_SIZE);
        }
    }
    else if (flag & DAC_INT_EMPTY) // all finish
    {
        hal_sysMemFree(gif_audio_buf);
        // uart_Printf("3");
        ax32xx_dacStop();

        audioPlaybackSetStatus(MEDIA_STAT_STOP);
    }
}

static u8 gif_audio_start(u16 sampleRate, u16 volume)
{
    u16 i;
    static u32 GameSampleRateTable[] =
        {
            48000, 44100, 3200, 0, 2400, 22050, 16000, 0, 12000, 11025, 8000};

    if (sampleRate == 0)
        return 1;
    for (i = 0; i < 11; i++)
    {
        if (sampleRate == GameSampleRateTable[i])
            break;
    }
    if (i >= 11)
    {
        return 2;
    }

    ax32xx_dacSampleRateSet(i);
    if (volume > 100)
    {
        volume = 100;
    }
    ax32xx_dacVolumeSet((volume * 0x7fff) / 100);
    hal_dacCallBackRegister(gif_audio_isr);
    ax32xx_dacStart(gif_audio_buf, GIF_PCM_BUFFER_SIZE);
}
u8 gif_audio_play(u8 wait, u8 finish)
{
    gif_audio_buf_pos = 0;
    if (gif_audio_buf)
    {
        if (wait)
        {
            u32 timeout = XOSTimeGet();
            while (1) // wait key sound play finish
            {
                hal_wdtClear();
                if (audioPlaybackGetStatus() == MEDIA_STAT_PLAY)
                {
                    if (XOSTimeGet() - timeout > 1000) // time out 1s
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }

        audioPlaybackSetStatus(MEDIA_STAT_PLAY);
        // deg_Printf("configValue2Int(CONFIG_ID_VOLUME)=%d\n",configValue2Int(CONFIG_ID_VOLUME));
        gif_audio_start(ADC_SAMPLEREATE_16000, /*VOLUME_LEVEL*SysCtrl.volume_level*/ configValue2Int(CONFIG_ID_VOLUME) * 20);

        if (finish)
        {
            while (1)
            {
                hal_wdtClear();
                if (MEDIA_STAT_PLAY != audioPlaybackGetStatus())
                {
                    break;
                }
            }
        }
    }

    return 0;
}
// 文件操作回调（保持原有实现）
bool gif_fileSeek(unsigned long pos)
{
    if (pos >= g_ctx.size)
    {
        return false; // 越界检查
    }
    else
    {
        g_ctx.offset = pos;
        return true;
    }
}

unsigned long gif_filePosition()
{
    return g_ctx.offset;
}

int gif_fileRead()
{
    return g_ctx.gif_data[g_ctx.offset++];
}

int gif_fileReadBlock(void *buffer, int numberOfBytes)
{
    if (g_ctx.offset + numberOfBytes > g_ctx.size)
    {
        return -1; // 越界检查
    }
    else
    {
        memcpy(buffer, &g_ctx.gif_data[g_ctx.offset], numberOfBytes);
        g_ctx.offset += numberOfBytes;
        return numberOfBytes;
    }
}

// 显示相关回调需要修改为使用全局decoder
void gif_startDrawing()
{
}
uint8_t gif_color_check(uint8_t r, uint8_t g, uint8_t b)
{
    if (r >= 123 && r <= 137 && g >= 123 && g <= 137 && b >= 123 && b <= 137)
    {
        return 1; // 黑色像素不绘制
    }
    else
    {
        return 0;
    }
}
void gif_drawPixel(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b)
{
    if (decoder->color_check)
    {
        if (gif_color_check(r, g, b))
        {
            return; // 黑色像素不绘制
        }
    }
    // BT.601标准系数 + 分量钳位
    int32_t y_val = (77 * r + 150 * g + 29 * b + 128) >> 8;
    y_val += 16;
    y_val = (y_val < 16) ? 16 : ((y_val > 235) ? 235 : y_val); // Y分量钳位
    y_buf[y * GIF_LCD_WIDTH + x] = (uint8_t)y_val;

    if ((x % 2 == 0) && (y % 2 == 0))
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
        const int uv_x = x / 2;
        const int uv_y = y / 2;
        const int uv_index = uv_y * (GIF_LCD_WIDTH / 2) + uv_x;

        // 存储为VU顺序（NV21格式）
        uv_buf[uv_index * 2] = v;
        uv_buf[uv_index * 2 + 1] = u;
    }
}
void gif_updateScreen()
{
    disp_frame_t *p_lcd_buffer;
    p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
    if (p_lcd_buffer)
        hal_dispframeVideoCfg(p_lcd_buffer, 0, 0, p_lcd_buffer->w, p_lcd_buffer->h);
    memcpy(p_lcd_buffer->y_addr, y_buf, p_lcd_buffer->w * p_lcd_buffer->h);
    memcpy(p_lcd_buffer->uv_addr, uv_buf, p_lcd_buffer->w * p_lcd_buffer->h / 2);
    ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr, p_lcd_buffer->w * p_lcd_buffer->h * 3 / 2);
    dispLayerShow(DISP_LAYER_VIDEO, (INT32U)p_lcd_buffer, 0, 0, VIDEO_ROTATE);
    if (g_ctx.gif_bk != NULL)
        memcpy(y_buf, g_ctx.gif_bk, GIF_LCD_WIDTH * GIF_LCD_HEIGHT * 3 / 2);
}

void gif_screenClear()
{
    // 复用视频播放器的清屏优化算法
    videoPlaybackClear(hal_lcdGetShowingBuffer()->y_addr,
                       hal_lcdGetShowingBuffer()->uv_addr,
                       GIF_LCD_WIDTH,
                       GIF_LCD_HEIGHT,
                       0x00,  // Y分量清零
                       0x80); // UV置中灰
}
void init_gif_decoder()
{
    decoder = (GifDecoder *)hal_sysMemMalloc(sizeof(GifDecoder), 64);
    GifDecoder_Init(decoder, GIF_LCD_WIDTH, GIF_LCD_HEIGHT, 12);
    // 文件操作回调
    GifDecoder_setFileSeekCallback(decoder, gif_fileSeek);
    GifDecoder_setFilePositionCallback(decoder, gif_filePosition);
    GifDecoder_setFileReadCallback(decoder, gif_fileRead);
    GifDecoder_setFileReadBlockCallback(decoder, gif_fileReadBlock);

    // 显示相关回调
    GifDecoder_setDrawPixelCallback(decoder, gif_drawPixel);
    GifDecoder_setUpdateScreenCallback(decoder, gif_updateScreen);
    GifDecoder_setScreenClearCallback(decoder, gif_screenClear);
    GifDecoder_setStartDrawingCallback(decoder, gif_startDrawing);
}
int gif_play(u32 res_id, u8 *gif_bk, int posx, int posy, uint8_t color_check, int gif_music_id)
{
    deg_Printf("剩余内存：gif play begin %dKB\n", hal_sysMemRemain() / 1024);
    int i;
    memset(&g_ctx, 0, sizeof(GIF_Context));
    g_ctx.gif_addr = nv_open(res_id);
    g_ctx.res_id = res_id;
    g_ctx.offset = 0;
    g_ctx.size = nv_size(res_id);
    // 分配对齐内存（32字节对齐）
    if (!(g_ctx.gif_data = hal_sysMemMalloc(g_ctx.size, 32)))
    {
        deg_Printf("ERR: Malloc %d bytes failed\n", g_ctx.size);
        return -3;
    }
    // 读取完整资源数据
    nv_read(g_ctx.gif_addr, g_ctx.gif_data, g_ctx.size);
    deg_Printf("Load GIF res 0x%X (%d bytes) 剩余内存：%dKB\n", res_id, g_ctx.size, hal_sysMemRemain() / 1024);
    init_gif_decoder();
    if (GifDecoder_startDecoding(decoder) != ERROR_NONE)
    {
        deg_Printf("Failed to start decoding\n");
        return -1;
    }
    deg_Printf("GIF Info: %dx%d\n", g_ctx.header.width, g_ctx.header.height);
    decoder->imageData = (uint8_t *)hal_sysMemMalloc(GIF_LCD_WIDTH * GIF_LCD_HEIGHT, 64);
    decoder->imageDataBU = (uint8_t *)hal_sysMemMalloc(GIF_LCD_WIDTH * GIF_LCD_HEIGHT, 64);
    decoder->offsetx = posx;
    decoder->offsety = posy;
    decoder->color_check = color_check;
    y_buf = (u8 *)hal_sysMemMalloc(GIF_LCD_WIDTH * GIF_LCD_HEIGHT * 3 / 2, 64);
    if (y_buf == NULL)
    {
        deg_Printf("Ybuf ERR: Malloc %d bytes failed\n", GIF_LCD_WIDTH * GIF_LCD_HEIGHT * 3 / 2);
        return -4;
    }
    uv_buf = y_buf + GIF_LCD_WIDTH * GIF_LCD_HEIGHT;
    if (gif_bk != NULL)
    {
        memcpy(y_buf, gif_bk, GIF_LCD_WIDTH * GIF_LCD_HEIGHT * 3 / 2);
        g_ctx.gif_bk = gif_bk;
    }

    u32 time;
    if (gif_music_id >= 0)
    {
        gif_audio_init(gif_music_id);
        gif_audio_play(1, 0);
    }
    while (1)
    {
        time = XOSTimeGet();
        int result = GifDecoder_decodeFrame(decoder, false);
        if (result != ERROR_NONE)
        {
            deg_Printf("Decoding error: %d\n", result);
            break;
        }
        g_ctx.frame_count++;
        g_ctx.header.delay_time[g_ctx.frame_count - 1] = decoder->frameDelay * 10;
        time = XOSTimeGet() - time;
#if GIF_DEGBUG == 1
        deg_Printf("framedelay %d time %d\n", decoder->frameDelay * 10, time);
#endif
        if (time < decoder->frameDelay * 10)
            XOSTimeDly(decoder->frameDelay * 10 - time);
#if GIF_DEGBUG == 1
        deg_Printf("Frame %d: %dms\n", g_ctx.frame_count, time);
#endif
        hal_wdtClear();
    }

    deg_Printf("GIF Frames:%d\n", g_ctx.frame_count);
#if GIF_DEGBUG == 1
    for (i = 0; i < g_ctx.frame_count; i++)
        deg_Printf("Frame%d:Delay=%dms\n", i, g_ctx.header.delay_time[i]);
#endif
    // 释放资源
    hal_sysMemFree(decoder->imageData);
    hal_sysMemFree(decoder->imageDataBU);
    hal_sysMemFree(decoder);
    hal_sysMemFree(g_ctx.gif_data);
    hal_sysMemFree(y_buf);
    deg_Printf("剩余内存：gif play end %dKB\n", hal_sysMemRemain() / 1024);
    return 0;
}