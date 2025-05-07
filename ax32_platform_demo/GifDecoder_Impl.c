#include "application.h"
extern GIF_Context g_ctx;
void GifDecoder_Init(GifDecoder *decoder, int maxGifWidth, int maxGifHeight, int lzwMaxBits)
{
    decoder->maxGifWidth = maxGifWidth;
    decoder->maxGifHeight = maxGifHeight;
    decoder->lzwMaxBits = lzwMaxBits;

    // Masks for 0 .. 16 bits
    decoder->mask[0] = 0x0000;
    decoder->mask[1] = 0x0001;
    decoder->mask[2] = 0x0003;
    decoder->mask[3] = 0x0007;
    decoder->mask[4] = 0x000F;
    decoder->mask[5] = 0x001F;
    decoder->mask[6] = 0x003F;
    decoder->mask[7] = 0x007F;
    decoder->mask[8] = 0x00FF;
    decoder->mask[9] = 0x01FF;
    decoder->mask[10] = 0x03FF;
    decoder->mask[11] = 0x07FF;
    decoder->mask[12] = 0x0FFF;
    decoder->mask[13] = 0x1FFF;
    decoder->mask[14] = 0x3FFF;
    decoder->mask[15] = 0x7FFF;
    decoder->mask[16] = 0xFFFF;
}

void GifDecoder_setStartDrawingCallback(GifDecoder *decoder, callback f)
{
    decoder->startDrawingCallback = f;
}

void GifDecoder_setUpdateScreenCallback(GifDecoder *decoder, callback f)
{
    decoder->updateScreenCallback = f;
}

void GifDecoder_setDrawPixelCallback(GifDecoder *decoder, pixel_callback f)
{
    decoder->drawPixelCallback = f;
}

void GifDecoder_setScreenClearCallback(GifDecoder *decoder, callback f)
{
    decoder->screenClearCallback = f;
}

void GifDecoder_setFileSeekCallback(GifDecoder *decoder, file_seek_callback f)
{
    decoder->fileSeekCallback = f;
}

void GifDecoder_setFilePositionCallback(GifDecoder *decoder, file_position_callback f)
{
    decoder->filePositionCallback = f;
}

void GifDecoder_setFileReadCallback(GifDecoder *decoder, file_read_callback f)
{
    decoder->fileReadCallback = f;
}

void GifDecoder_setFileReadBlockCallback(GifDecoder *decoder, file_read_block_callback f)
{
    decoder->fileReadBlockCallback = f;
}

int GifDecoder_GetGifWidth(GifDecoder *decoder)
{
    return decoder->lsdWidth;
}

int GifDecoder_GetGifHeight(GifDecoder *decoder)
{
    return decoder->lsdHeight;
}

// Backup the read stream by n bytes
void BackupStream(GifDecoder *decoder, int n)
{
    decoder->fileSeekCallback(decoder->filePositionCallback() - n);
}

// Read a file byte
int ReadByte(GifDecoder *decoder)
{
    int b = decoder->fileReadCallback();
    if (b == -1)
    {
#if GIFDEBUG == 1
        deg_Printf("Read error or EOF occurred\n");
#endif
    }
    return b;
}

// Read a file word
int ReadWord(GifDecoder *decoder)
{
    int b0 = ReadByte(decoder);
    int b1 = ReadByte(decoder);
    return (b1 << 8) | b0;
}

// Read the specified number of bytes into the specified buffer
int ReadIntoBuffer(GifDecoder *decoder, void *buffer, int numberOfBytes)
{
    int result = decoder->fileReadBlockCallback(buffer, numberOfBytes);
    if (result == -1)
    {
#if GIFDEBUG == 1
        deg_Printf("Read error or EOF occurred\n");
#endif
    }
    return result;
}

// Fill a portion of imageData buffer with a color index
void FillImageDataRect(GifDecoder *decoder, uint8_t colorIndex, int x, int y, int width, int height)
{
    int yOffset;
    int yy, xx;

    for (yy = y; yy < height + y; yy++)
    {
        yOffset = yy * decoder->maxGifWidth;
        for (xx = x; xx < width + x; xx++)
        {
            decoder->imageData[yOffset + xx] = colorIndex;
        }
    }
}

// Fill entire imageData buffer with a color index
void FillImageData(GifDecoder *decoder, uint8_t colorIndex)
{
    memset(decoder->imageData, colorIndex, decoder->maxGifWidth * decoder->maxGifHeight);
}

// Copy image data in rect from a src to a dst
void CopyImageDataRect(GifDecoder *decoder, uint8_t *dst, uint8_t *src, int x, int y, int width, int height)
{
    int yOffset, offset;
    int yy, xx;

    for (yy = y; yy < height + y; yy++)
    {
        yOffset = yy * decoder->maxGifWidth;
        for (xx = x; xx < width + x; xx++)
        {
            offset = yOffset + xx;
            dst[offset] = src[offset];
        }
    }
}

// Make sure the file is a Gif file
bool ParseGifHeader(GifDecoder *decoder)
{
    char buffer[GIFHDRSIZE];
    ReadIntoBuffer(decoder, buffer, GIFHDRSIZE);
    if ((strncmp(buffer, GIFHDRTAGNORM, GIFHDRSIZE) != 0) &&
        (strncmp(buffer, GIFHDRTAGNORM1, GIFHDRSIZE) != 0))
    {
        return false;
    }
    else
    {
        return true;
    }
}

// Parse the logical screen descriptor
void ParseLogicalScreenDescriptor(GifDecoder *decoder)
{
    decoder->lsdWidth = ReadWord(decoder);
    decoder->lsdHeight = ReadWord(decoder);
    decoder->lsdPackedField = ReadByte(decoder);
    decoder->lsdBackgroundIndex = ReadByte(decoder);
    decoder->lsdAspectRatio = ReadByte(decoder);
    g_ctx.header.width = decoder->lsdWidth;
    g_ctx.header.height = decoder->lsdHeight;

#if GIFDEBUG == 1 && DEBUG_SCREEN_DESCRIPTOR == 1
    deg_Printf("lsdWidth: %d\n", decoder->lsdWidth);
    deg_Printf("lsdHeight: %d\n", decoder->lsdHeight);
    deg_Printf("lsdPackedField: 0x%x\n", decoder->lsdPackedField);
    deg_Printf("lsdBackgroundIndex: %d\n", decoder->lsdBackgroundIndex);
    deg_Printf("lsdAspectRatio: %d\n", decoder->lsdAspectRatio);
#endif
}

// Parse the global color table
void ParseGlobalColorTable(GifDecoder *decoder)
{
    // Does a global color table exist?
    if (decoder->lsdPackedField & COLORTBLFLAG)
    {
        // A GCT was present determine how many colors it contains
        decoder->colorCount = 1 << ((decoder->lsdPackedField & 7) + 1);

#if GIFDEBUG == 1 && DEBUG_GLOBAL_COLOR_TABLE == 1
        deg_Printf("Global color table with %d colors present\n", decoder->colorCount);
#endif
        // Read color values into the palette array
        int colorTableBytes = sizeof(rgb_24) * decoder->colorCount;
        ReadIntoBuffer(decoder, decoder->palette, colorTableBytes);
    }
}

// Parse plain text extension and dispose of it
void ParsePlainTextExtension(GifDecoder *decoder)
{
#if GIFDEBUG == 1 && DEBUG_PROCESSING_PLAIN_TEXT_EXT == 1
    deg_Printf("\nProcessing Plain Text Extension\n");
#endif
    // Read plain text header length
    uint8_t len = ReadByte(decoder);

    // Consume plain text header data
    ReadIntoBuffer(decoder, decoder->tempBuffer, len);

    // Consume the plain text data in blocks
    len = ReadByte(decoder);
    while (len != 0)
    {
        ReadIntoBuffer(decoder, decoder->tempBuffer, len);
        len = ReadByte(decoder);
    }
}

// Parse a graphic control extension
void ParseGraphicControlExtension(GifDecoder *decoder)
{
#if GIFDEBUG == 1 && DEBUG_PROCESSING_GRAPHIC_CONTROL_EXT == 1
    deg_Printf("\nProcessing Graphic Control Extension\n");
#endif
    int len = ReadByte(decoder); // Check length
    if (len != 4)
    {
#if GIFDEBUG == 1
        deg_Printf("Bad graphic control extension\n");
#endif
    }

    int packedBits = ReadByte(decoder);
    decoder->frameDelay = ReadWord(decoder);
    decoder->transparentColorIndex = ReadByte(decoder);

    if ((packedBits & TRANSPARENTFLAG) == 0)
    {
        // Indicate no transparent index
        decoder->transparentColorIndex = NO_TRANSPARENT_INDEX;
    }
    decoder->disposalMethod = (packedBits >> 2) & 7;
    if (decoder->disposalMethod > 3)
    {
        decoder->disposalMethod = 0;
#if GIFDEBUG == 1
        deg_Printf("Invalid disposal value\n");
#endif
    }

    ReadByte(decoder); // Toss block end

#if GIFDEBUG == 1 && DEBUG_PROCESSING_GRAPHIC_CONTROL_EXT == 1
    deg_Printf("PacketBits: 0x%x\n", packedBits);
    deg_Printf("Frame delay: %d\n", decoder->frameDelay);
    deg_Printf("transparentColorIndex: %d\n", decoder->transparentColorIndex);
    deg_Printf("disposalMethod: %d\n", decoder->disposalMethod);
#endif
}

// Parse application extension
void ParseApplicationExtension(GifDecoder *decoder)
{
    memset(decoder->tempBuffer, 0, sizeof(decoder->tempBuffer));

#if GIFDEBUG == 1 && DEBUG_PROCESSING_APP_EXT == 1
    deg_Printf("\nProcessing Application Extension\n");
#endif

    // Read block length
    uint8_t len = ReadByte(decoder);

    // Read app data
    ReadIntoBuffer(decoder, decoder->tempBuffer, len);

#if GIFDEBUG == 1 && DEBUG_PROCESSING_APP_EXT == 1
    // Conditionally display the application extension string
    if (strlen(decoder->tempBuffer) != 0)
    {
        deg_Printf("Application Extension: %s\n", decoder->tempBuffer);
    }
#endif

    // Consume any additional app data
    len = ReadByte(decoder);
    while (len != 0)
    {
        ReadIntoBuffer(decoder, decoder->tempBuffer, len);
        len = ReadByte(decoder);
    }
}

// Parse comment extension
void ParseCommentExtension(GifDecoder *decoder)
{
#if GIFDEBUG == 1 && DEBUG_PROCESSING_COMMENT_EXT == 1
    deg_Printf("\nProcessing Comment Extension\n");
#endif

    // Read block length
    uint8_t len = ReadByte(decoder);
    while (len != 0)
    {
        // Clear buffer
        memset(decoder->tempBuffer, 0, sizeof(decoder->tempBuffer));

        // Read len bytes into buffer
        ReadIntoBuffer(decoder, decoder->tempBuffer, len);

#if GIFDEBUG == 1 && DEBUG_PROCESSING_COMMENT_EXT == 1
        // Display the comment extension string
        if (strlen(decoder->tempBuffer) != 0)
        {
            deg_Printf("Comment Extension: %s\n", decoder->tempBuffer);
        }
#endif
        // Read the new block length
        len = ReadByte(decoder);
    }
}

// Parse file terminator
int ParseGIFFileTerminator(GifDecoder *decoder)
{
#if GIFDEBUG == 1 && DEBUG_PROCESSING_FILE_TERM == 1
    deg_Printf("\nProcessing file terminator\n");
#endif

    uint8_t b = ReadByte(decoder);
    if (b != 0x3B)
    {

#if GIFDEBUG == 1 && DEBUG_PROCESSING_FILE_TERM == 1
        deg_Printf("Terminator byte: 0x%x\n", b);
        deg_Printf("Bad GIF file format - Bad terminator\n");
#endif

        return ERROR_BADGIFFORMAT;
    }
    else
    {
        return ERROR_NONE;
    }
}

// Parse table based image data
void ParseTableBasedImage(GifDecoder *decoder)
{
#if GIFDEBUG == 1 && DEBUG_PROCESSING_TBI_DESC_START == 1
    deg_Printf("\nProcessing Table Based Image Descriptor\n");
#endif

#if GIFDEBUG == 1 && DEBUG_PARSING_DATA == 1
    deg_Printf("File Position: %u\n", decoder->filePositionCallback());
    // deg_Printf("File Size: %u\n", file.size());
#endif

    // Parse image descriptor
    decoder->tbiImageX = ReadWord(decoder);
    decoder->tbiImageY = ReadWord(decoder);
    decoder->tbiWidth = ReadWord(decoder);
    decoder->tbiHeight = ReadWord(decoder);
    decoder->tbiPackedBits = ReadByte(decoder);

#if GIFDEBUG == 1
    deg_Printf("tbiImageX: %d\n", decoder->tbiImageX);
    deg_Printf("tbiImageY: %d\n", decoder->tbiImageY);
    deg_Printf("tbiWidth: %d\n", decoder->tbiWidth);
    deg_Printf("tbiHeight: %d\n", decoder->tbiHeight);
    deg_Printf("PackedBits: 0x%x\n", decoder->tbiPackedBits);
#endif

    // Is this image interlaced ?
    decoder->tbiInterlaced = ((decoder->tbiPackedBits & INTERLACEFLAG) != 0);

#if GIFDEBUG == 1 && DEBUG_PROCESSING_TBI_DESC_INTERLACED == 1
    deg_Printf("Image interlaced: %s\n", decoder->tbiInterlaced ? "Yes" : "No");
#endif

    // Does this image have a local color table ?
    bool localColorTable = ((decoder->tbiPackedBits & COLORTBLFLAG) != 0);

    if (localColorTable)
    {
        int colorBits = ((decoder->tbiPackedBits & 7) + 1);
        decoder->colorCount = 1 << colorBits;

#if GIFDEBUG == 1 && DEBUG_PROCESSING_TBI_DESC_LOCAL_COLOR_TABLE == 1
        deg_Printf("Local color table with %d colors present\n", decoder->colorCount);
#endif
        // Read colors into palette
        int colorTableBytes = sizeof(rgb_24) * decoder->colorCount;
        ReadIntoBuffer(decoder, decoder->palette, colorTableBytes);
    }

    // One time initialization of imageData before first frame
    if (decoder->keyFrame)
    {
        if (decoder->transparentColorIndex == NO_TRANSPARENT_INDEX)
        {
            FillImageData(decoder, decoder->lsdBackgroundIndex);
        }
        else
        {
            FillImageData(decoder, decoder->transparentColorIndex);
        }
        decoder->keyFrame = false;

        decoder->rectX = 0;
        decoder->rectY = 0;
        decoder->rectWidth = decoder->maxGifWidth;
        decoder->rectHeight = decoder->maxGifHeight;
    }
    // Don't clear matrix screen for these disposal methods
    if ((decoder->prevDisposalMethod != DISPOSAL_NONE) && (decoder->prevDisposalMethod != DISPOSAL_LEAVE))
    {
        if (decoder->screenClearCallback)
            decoder->screenClearCallback();
    }

    // Process previous disposal method
    if (decoder->prevDisposalMethod == DISPOSAL_BACKGROUND)
    {
        // Fill portion of imageData with previous background color
        FillImageDataRect(decoder, decoder->prevBackgroundIndex, decoder->rectX, decoder->rectY, decoder->rectWidth, decoder->rectHeight);
    }
    else if (decoder->prevDisposalMethod == DISPOSAL_RESTORE)
    {
        CopyImageDataRect(decoder, decoder->imageData, decoder->imageDataBU, decoder->rectX, decoder->rectY, decoder->rectWidth, decoder->rectHeight);
    }

    // Save disposal method for this frame for next time
    decoder->prevDisposalMethod = decoder->disposalMethod;

    if (decoder->disposalMethod != DISPOSAL_NONE)
    {
        // Save dimensions of this frame
        decoder->rectX = decoder->tbiImageX;
        decoder->rectY = decoder->tbiImageY;
        decoder->rectWidth = decoder->tbiWidth;
        decoder->rectHeight = decoder->tbiHeight;

        // limit rectangle to the bounds of maxGifWidth*maxGifHeight
        if (decoder->rectX + decoder->rectWidth > decoder->maxGifWidth)
            decoder->rectWidth = decoder->maxGifWidth - decoder->rectX;
        if (decoder->rectY + decoder->rectHeight > decoder->maxGifHeight)
            decoder->rectHeight = decoder->maxGifHeight - decoder->rectY;
        if (decoder->rectX >= decoder->maxGifWidth || decoder->rectY >= decoder->maxGifHeight)
        {
            decoder->rectX = decoder->rectY = decoder->rectWidth = decoder->rectHeight = 0;
        }

        if (decoder->disposalMethod == DISPOSAL_BACKGROUND)
        {
            if (decoder->transparentColorIndex != NO_TRANSPARENT_INDEX)
            {
                decoder->prevBackgroundIndex = decoder->transparentColorIndex;
            }
            else
            {
                decoder->prevBackgroundIndex = decoder->lsdBackgroundIndex;
            }
        }
        else if (decoder->disposalMethod == DISPOSAL_RESTORE)
        {
            CopyImageDataRect(decoder, decoder->imageDataBU, decoder->imageData, decoder->rectX, decoder->rectY, decoder->rectWidth, decoder->rectHeight);
        }
    }

    // Read the min LZW code size
    decoder->lzwCodeSize = ReadByte(decoder);

#if GIFDEBUG == 1 && DEBUG_PROCESSING_TBI_DESC_LZWCODESIZE == 1
    deg_Printf("LzwCodeSize: %d\n", decoder->lzwCodeSize);
    deg_Printf("File Position Before: %u\n", decoder->filePositionCallback());
#endif

    unsigned long filePositionBefore = decoder->filePositionCallback();

    // Gather the lzw image data
    // NOTE: the dataBlockSize byte is left in the data as the lzw decoder needs it
    int offset = 0;
    int dataBlockSize = ReadByte(decoder);
    while (dataBlockSize != 0)
    {
#if GIFDEBUG == 1 && DEBUG_PROCESSING_TBI_DESC_DATABLOCKSIZE == 1
        deg_Printf("dataBlockSize: %d\n", dataBlockSize);
#endif
        BackupStream(decoder, 1);
        dataBlockSize++;
        decoder->fileSeekCallback(decoder->filePositionCallback() + dataBlockSize);

        offset += dataBlockSize;
        dataBlockSize = ReadByte(decoder);
    }

#if GIFDEBUG == 1 && DEBUG_PROCESSING_TBI_DESC_LZWIMAGEDATA_SIZE == 1
    deg_Printf("total lzwImageData Size: %d\n", offset);
    deg_Printf("File Position Test: %u\n", decoder->filePositionCallback());
#endif

    // this is the position where GIF decoding needs to pick up after decompressing frame
    unsigned long filePositionAfter = decoder->filePositionCallback();

    decoder->fileSeekCallback(filePositionBefore);

    // Process the animation frame for display

    // Initialize the LZW decoder for this frame
    LzwDecodeInit(decoder, decoder->lzwCodeSize);
#if GIFDEBUG == 1
    deg_Printf("LZWDecodeInit success\n");
#endif
    LzwSetTempBuffer(decoder, (uint8_t *)decoder->tempBuffer);

    // Make sure there is at least some delay between frames
    if (decoder->frameDelay < 1)
    {
        decoder->frameDelay = 1;
    }

    // Decompress LZW data and display the frame
    DecompressAndDisplayFrame(decoder, filePositionAfter);
#if GIFDEBUG == 1
    deg_Printf("DecompressAndDisplayFrame success\n");
#endif
    // Graphic control extension is for a single frame
    decoder->transparentColorIndex = NO_TRANSPARENT_INDEX;
    decoder->disposalMethod = DISPOSAL_NONE;
}

// Parse gif data
int ParseData(GifDecoder *decoder)
{
#if GIFDEBUG == 1 && DEBUG_PARSING_DATA == 1
    deg_Printf("\nParsing Data Block\n");
#endif

    bool parsedFrame = false;
    while (!parsedFrame)
    {

#if GIFDEBUG == 1 && DEBUG_WAIT_FOR_KEY_PRESS == 1
        deg_Printf("\nPress Key For Next\n");
        while (getchar() <= 0)
            ;
#endif

        // Determine what kind of data to process
        uint8_t b = ReadByte(decoder);

        if (b == 0x2c)
        {
            // Parse table based image
#if GIFDEBUG == 1 && DEBUG_PARSING_DATA == 1
            deg_Printf("\nParsing Table Based\n");
#endif
            ParseTableBasedImage(decoder);
            parsedFrame = true;
        }
        else if (b == 0x21)
        {
            // Parse extension
            b = ReadByte(decoder);

#if GIFDEBUG == 1 && DEBUG_PARSING_DATA == 1
            deg_Printf("\nParsing Extension\n");
#endif

            // Determine which kind of extension to parse
            switch (b)
            {
            case 0x01:
                // Plain test extension
                ParsePlainTextExtension(decoder);
                break;
            case 0xf9:
                // Graphic control extension
                ParseGraphicControlExtension(decoder);
                break;
            case 0xfe:
                // Comment extension
                ParseCommentExtension(decoder);
                break;
            case 0xff:
                // Application extension
                ParseApplicationExtension(decoder);
                break;
            default:
#if GIFDEBUG == 1
                deg_Printf("Unknown control extension: 0x%x\n", b);
#endif
                return ERROR_UNKNOWNCONTROLEXT;
            }
        }
        else
        {
#if GIFDEBUG == 1 && DEBUG_PARSING_DATA == 1
            deg_Printf("\nParsing Done\n");
#endif

            // Push unprocessed byte back into the stream for later processing
            BackupStream(decoder, 1);

            return ERROR_DONE_PARSING;
        }
    }
    return ERROR_NONE;
}

int GifDecoder_startDecoding(GifDecoder *decoder)
{
    // Initialize variables
    decoder->keyFrame = true;
    decoder->prevDisposalMethod = DISPOSAL_NONE;
    decoder->transparentColorIndex = NO_TRANSPARENT_INDEX;
    decoder->nextFrameTime_ms = 0;
    decoder->fileSeekCallback(0);

    // Validate the header
    if (!ParseGifHeader(decoder))
    {
#if GIFDEBUG == 1
        deg_Printf("Not a GIF file\n");
#endif
        return ERROR_FILENOTGIF;
    }
    // If we get here we have a gif file to process

    // Parse the logical screen descriptor
    ParseLogicalScreenDescriptor(decoder);

    // Parse the global color table
    ParseGlobalColorTable(decoder);

    return ERROR_NONE;
}

int GifDecoder_decodeFrame(GifDecoder *decoder, bool loop)
{
    // Parse gif data
    int result = ParseData(decoder);
    if (result < ERROR_NONE)
    {
#if GIFDEBUG == 1
        deg_Printf("Error: %d occurred during parsing of data\n", result);
#endif
        return result;
    }

    if (result == ERROR_DONE_PARSING && loop == true)
    {
        // startDecoding();
        //  Initialize variables like with a new file
        decoder->keyFrame = true;
        decoder->prevDisposalMethod = DISPOSAL_NONE;
        decoder->transparentColorIndex = NO_TRANSPARENT_INDEX;
        decoder->nextFrameTime_ms = 0;
        decoder->fileSeekCallback(0);

        // parse Gif Header like with a new file
        ParseGifHeader(decoder);

        // Parse the logical screen descriptor
        ParseLogicalScreenDescriptor(decoder);

        // Parse the global color table
        ParseGlobalColorTable(decoder);
    }

    return result;
}

// Decompress LZW data and display animation frame
void DecompressAndDisplayFrame(GifDecoder *decoder, unsigned long filePositionAfter)
{
    // Each pixel of image is 8 bits and is an index into the palette

    // How the image is decoded depends upon whether it is interlaced or not
    // Decode the interlaced LZW data into the image buffer
    int line, time;
    time = XOSTimeGet();
    if (decoder->tbiInterlaced)
    {
        // 四阶段隔行解码（8行->4行->2行->1行）采用标准GIF隔行扫描解码策略，分四次完成全幅图像的解码，逐步提高图像清晰度。
        // Decode every 8th line starting at line 0
        for (line = decoder->tbiImageY + 0; line < decoder->tbiHeight + decoder->tbiImageY; line += 8)
        {
            LzwDecode(decoder, decoder->imageData + (line * decoder->maxGifWidth) + decoder->tbiImageX, decoder->tbiWidth, decoder->imageData + decoder->maxGifWidth * decoder->maxGifHeight);
        }
        // Decode every 8th line starting at line 4
        for (line = decoder->tbiImageY + 4; line < decoder->tbiHeight + decoder->tbiImageY; line += 8)
        {
            LzwDecode(decoder, decoder->imageData + (line * decoder->maxGifWidth) + decoder->tbiImageX, decoder->tbiWidth, decoder->imageData + decoder->maxGifWidth * decoder->maxGifHeight);
        }
        // Decode every 4th line starting at line 2
        for (line = decoder->tbiImageY + 2; line < decoder->tbiHeight + decoder->tbiImageY; line += 4)
        {
            LzwDecode(decoder, decoder->imageData + (line * decoder->maxGifWidth) + decoder->tbiImageX, decoder->tbiWidth, decoder->imageData + decoder->maxGifWidth * decoder->maxGifHeight);
        }
        // Decode every 2nd line starting at line 1
        for (line = decoder->tbiImageY + 1; line < decoder->tbiHeight + decoder->tbiImageY; line += 2)
        {
            LzwDecode(decoder, decoder->imageData + (line * decoder->maxGifWidth) + decoder->tbiImageX, decoder->tbiWidth, decoder->imageData + decoder->maxGifWidth * decoder->maxGifHeight);
        }
    }
    else
    {
        // Decode the non interlaced LZW data into the image data buffer
        for (line = decoder->tbiImageY; line < decoder->tbiHeight + decoder->tbiImageY; line++)
        {
            LzwDecode(decoder, decoder->imageData + (line * decoder->maxGifWidth) + decoder->tbiImageX, decoder->tbiWidth, decoder->imageData + decoder->maxGifWidth * decoder->maxGifHeight);
        }
    }
    time = XOSTimeGet() - time;
    deg_Printf("LZW Decode Time: %d ms\n", time);
#if GIFDEBUG == 1 && DEBUG_DECOMPRESS_AND_DISPLAY == 1
    deg_Printf("File Position After: %u\n", decoder->filePositionCallback());
#endif

#if GIFDEBUG == 1 && DEBUG_WAIT_FOR_KEY_PRESS == 1
    deg_Printf("\nPress Key For Next\n");
    while (getchar() <= 0)
        ;
#endif

    // LZW doesn't parse through all the data, manually set position
    decoder->fileSeekCallback(filePositionAfter); // 同步文件指针

    // Optional callback can be used to get drawing routines ready
    if (decoder->startDrawingCallback) // 触发绘图前准备回调
        decoder->startDrawingCallback();
    int yOffset, pixel;
    int y, x;
    for (y = decoder->tbiImageY; y < decoder->tbiHeight + decoder->tbiImageY; y++)
    {
        yOffset = y * decoder->maxGifWidth;
        for (x = decoder->tbiImageX; x < decoder->tbiWidth + decoder->tbiImageX; x++)
        {
            // Get the next pixel
            pixel = decoder->imageData[yOffset + x];

            // Check pixel transparency
            if (pixel == decoder->transparentColorIndex)
            {
                continue;
            }
            // 像素级操作（约执行 width*height 次）
            // Pixel not transparent so get color from palette and draw the pixel
            if (decoder->drawPixelCallback)
                decoder->drawPixelCallback(x, y, decoder->palette[pixel].red, decoder->palette[pixel].green, decoder->palette[pixel].blue);
        }
    }
    time = XOSTimeGet() - time;
    decoder->updateScreenCallback();
    // Image data is decompressed, now display portion of image affected by frame
    // deg_Printf("tbiX:%d tbiY:%d tbiW:%d tbiH:%d\n", decoder->tbiImageX, decoder->tbiImageY, decoder->tbiWidth, decoder->tbiHeight);
    // Make animation frame visible
    // swapBuffers() call can take up to 1/framerate seconds to return (it waits until a buffer copy is complete)
    // note the time before calling
    // ax32xx_sysCpuMsDelay(10 * decoder->frameDelay);
}