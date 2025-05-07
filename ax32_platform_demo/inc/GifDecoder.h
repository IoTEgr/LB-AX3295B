#ifndef _GIFDECODER_H_
#define _GIFDECODER_H_

#define GIFDEBUG 0

#if defined(ARDUINO)
#include <Arduino.h>
#elif defined(SPARK)
#include "application.h"
#endif

// This file contains C code, and ESP32 Arduino has changed to use the C++ template version of min()/max() which we can't use with C, so we can't depend on a #define min() from Arduino anymore
#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#if GIFDEBUG == 1
#define DEBUG_SCREEN_DESCRIPTOR 1
#define DEBUG_GLOBAL_COLOR_TABLE 1
#define DEBUG_PROCESSING_PLAIN_TEXT_EXT 1
#define DEBUG_PROCESSING_GRAPHIC_CONTROL_EXT 1
#define DEBUG_PROCESSING_APP_EXT 1
#define DEBUG_PROCESSING_COMMENT_EXT 1
#define DEBUG_PROCESSING_FILE_TERM 1
#define DEBUG_PROCESSING_TABLE_IMAGE_DESC 1
#define DEBUG_PROCESSING_TBI_DESC_START 1
#define DEBUG_PROCESSING_TBI_DESC_INTERLACED 1
#define DEBUG_PROCESSING_TBI_DESC_LOCAL_COLOR_TABLE 1
#define DEBUG_PROCESSING_TBI_DESC_LZWCODESIZE 1
#define DEBUG_PROCESSING_TBI_DESC_DATABLOCKSIZE 0
#define DEBUG_PROCESSING_TBI_DESC_LZWIMAGEDATA_OVERFLOW 1
#define DEBUG_PROCESSING_TBI_DESC_LZWIMAGEDATA_SIZE 1
#define DEBUG_PARSING_DATA 1
#define DEBUG_DECOMPRESS_AND_DISPLAY 1

#define DEBUG_WAIT_FOR_KEY_PRESS 0

#endif

// Error codes
#define ERROR_NONE 0
#define ERROR_DONE_PARSING 1
#define ERROR_WAITING 2
#define ERROR_FILEOPEN -1
#define ERROR_FILENOTGIF -2
#define ERROR_BADGIFFORMAT -3
#define ERROR_UNKNOWNCONTROLEXT -4

#define GIFHDRTAGNORM "GIF87a"  // tag in valid GIF file
#define GIFHDRTAGNORM1 "GIF89a" // tag in valid GIF file
#define GIFHDRSIZE 6

// Global GIF specific definitions
#define COLORTBLFLAG 0x80
#define INTERLACEFLAG 0x40
#define TRANSPARENTFLAG 0x01

#define NO_TRANSPARENT_INDEX -1

// Disposal methods
#define DISPOSAL_NONE 0
#define DISPOSAL_LEAVE 1
#define DISPOSAL_BACKGROUND 2
#define DISPOSAL_RESTORE 3
typedef void (*callback)(void);
typedef void (*pixel_callback)(int16_t x, int16_t y, uint8_t red, uint8_t green, uint8_t blue);
typedef void *(*get_buffer_callback)(void);

typedef bool (*file_seek_callback)(unsigned long position);
typedef unsigned long (*file_position_callback)(void);
typedef int (*file_read_callback)(void);
typedef int (*file_read_block_callback)(void *buffer, int numberOfBytes);

typedef struct rgb_24
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} rgb_24;

// LZW constants
#define LZW_MAXBITS 12
#define LZW_SIZTABLE (1 << LZW_MAXBITS)

typedef struct
{
    int maxGifWidth;
    int maxGifHeight;
    int lzwMaxBits;
    uint8_t *imageData;
    uint8_t *imageDataBU;
    callback screenClearCallback;
    callback updateScreenCallback;
    pixel_callback drawPixelCallback;
    callback startDrawingCallback;
    file_seek_callback fileSeekCallback;
    file_position_callback filePositionCallback;
    file_read_callback fileReadCallback;
    file_read_block_callback fileReadBlockCallback;

    // Logical screen descriptor attributes
    int lsdWidth;
    int lsdHeight;
    int lsdPackedField;
    int lsdAspectRatio;
    int lsdBackgroundIndex;

    // Table based image attributes
    int tbiImageX;
    int tbiImageY;
    int tbiWidth;
    int tbiHeight;
    int tbiPackedBits;
    bool tbiInterlaced;

    int frameDelay;
    int transparentColorIndex;
    int prevBackgroundIndex;
    int prevDisposalMethod;
    int disposalMethod;
    int lzwCodeSize;
    bool keyFrame;
    int rectX;
    int rectY;
    int rectWidth;
    int rectHeight;

    unsigned long nextFrameTime_ms;

    int colorCount;
    rgb_24 palette[256];

    char tempBuffer[260];
    uint8_t *temp_buffer; // 添加 temp_buffer 成员变量

    // LZW variables
    int bbits;
    int bbuf;
    int cursize; // The current code size
    int curmask;
    int codesize;
    int clear_code;
    int end_code;
    int newcodes; // First available code
    int top_slot; // Highest code for current size
    int extra_slot;
    int slot; // Last read code
    int fc, oc;
    int bs; // Current buffer size for GIF
    int bcnt;
    uint8_t *sp;
    uint8_t stack[LZW_SIZTABLE];
    uint8_t suffix[LZW_SIZTABLE];
    uint16_t prefix[LZW_SIZTABLE];
    // Masks for 0 .. 16 bits
    unsigned int mask[17];
} GifDecoder;

void GifDecoder_Init(GifDecoder *decoder, int maxGifWidth, int maxGifHeight, int lzwMaxBits);
int GifDecoder_startDecoding(GifDecoder *decoder);
int GifDecoder_decodeFrame(GifDecoder *decoder, bool loop);
void GifDecoder_setScreenClearCallback(GifDecoder *decoder, callback f);
void GifDecoder_setUpdateScreenCallback(GifDecoder *decoder, callback f);
void GifDecoder_setDrawPixelCallback(GifDecoder *decoder, pixel_callback f);
void GifDecoder_setStartDrawingCallback(GifDecoder *decoder, callback f);
void GifDecoder_setFileSeekCallback(GifDecoder *decoder, file_seek_callback f);
void GifDecoder_setFilePositionCallback(GifDecoder *decoder, file_position_callback f);
void GifDecoder_setFileReadCallback(GifDecoder *decoder, file_read_callback f);
void GifDecoder_setFileReadBlockCallback(GifDecoder *decoder, file_read_block_callback f);
int GifDecoder_GetGifWidth(GifDecoder *decoder);
int GifDecoder_GetGifHeight(GifDecoder *decoder);

void LzwSetTempBuffer(GifDecoder *decoder, uint8_t *tempBuffer);
void LzwDecodeInit(GifDecoder *decoder, int csize);
int LzwGetCode(GifDecoder *decoder);
int LzwDecode(GifDecoder *decoder, uint8_t *buf, int len, uint8_t *bufend);

#endif