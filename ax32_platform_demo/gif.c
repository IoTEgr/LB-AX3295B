#include "application.h"
GIF_Context g_ctx = {0};
GifDecoder *decoder = NULL;
u8 *y_buf, *uv_buf;
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
void gif_drawPixel(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b)
{
    // BT.601标准系数 + 分量钳位
    int32_t y_val = (77 * r + 150 * g + 29 * b + 128) >> 8;
    y_val += 16;
    y_val = (y_val < 16) ? 16 : ((y_val > 235) ? 235 : y_val); // Y分量钳位
    y_buf[y * GIF_WIDTH + x] = (uint8_t)y_val;

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
        const int uv_index = uv_y * (GIF_WIDTH / 2) + uv_x;

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
        hal_dispframeVideoCfg(p_lcd_buffer, 0, 0, GIF_WIDTH, GIF_HEIGHT);
    memcpy(p_lcd_buffer->y_addr, y_buf, GIF_WIDTH * GIF_HEIGHT);
    memcpy(p_lcd_buffer->uv_addr, uv_buf, GIF_WIDTH * GIF_HEIGHT / 2);
    ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr, p_lcd_buffer->w * p_lcd_buffer->h * 3 / 2);
    dispLayerShow(DISP_LAYER_VIDEO, (INT32U)p_lcd_buffer, 0, 0, VIDEO_ROTATE);
}

void gif_screenClear()
{
    // 复用视频播放器的清屏优化算法
    videoPlaybackClear(hal_lcdGetShowingBuffer()->y_addr,
                       hal_lcdGetShowingBuffer()->uv_addr,
                       decoder->maxGifWidth,
                       decoder->maxGifHeight,
                       0x00,  // Y分量清零
                       0x80); // UV置中灰
}
void init_gif_decoder()
{
    decoder = (GifDecoder *)hal_sysMemMalloc(sizeof(GifDecoder), 64);
    decoder->imageData = (uint8_t *)hal_sysMemMalloc(GIF_WIDTH * GIF_HEIGHT, 64);
    decoder->imageDataBU = (uint8_t *)hal_sysMemMalloc(GIF_WIDTH * GIF_HEIGHT, 64);
    GifDecoder_Init(decoder, GIF_WIDTH, GIF_HEIGHT, 12);
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
int gif_play(u32 res_id)
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
    y_buf = (u8 *)hal_sysMemMalloc(YUV_SIZE, 64);
    if (y_buf == NULL)
    {
        deg_Printf("Ybuf ERR: Malloc %d bytes failed\n", YUV_SIZE);
        return -4;
    }
    uv_buf = y_buf + GIF_WIDTH * GIF_HEIGHT;

    init_gif_decoder();
    if (GifDecoder_startDecoding(decoder) != ERROR_NONE)
    {
        deg_Printf("Failed to start decoding\n");
        return -1;
    }
    u32 time;
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
        deg_Printf("framedelay %d time %d\n", decoder->frameDelay * 10, time);
        if (time < decoder->frameDelay * 10)
            hal_sysDelayMS(decoder->frameDelay * 10 - time);
        deg_Printf("Frame %d: %dms\n", g_ctx.frame_count, time);
        hal_wdtClear();
    }
    deg_Printf("GIF Info: %dx%d, Frames:%d\n", decoder->lsdWidth, decoder->lsdHeight, g_ctx.frame_count);
    for (i = 0; i < g_ctx.frame_count; i++)
        deg_Printf("Frame%d:Delay=%dms\n", i, g_ctx.header.delay_time[i]);
    // 释放资源
    hal_sysMemFree(decoder->imageData);
    hal_sysMemFree(decoder->imageDataBU);
    hal_sysMemFree(decoder);
    hal_sysMemFree(g_ctx.gif_data);
    hal_sysMemFree(y_buf);
    deg_Printf("剩余内存：gif play end %dKB\n", hal_sysMemRemain() / 1024);
    return 0;
}