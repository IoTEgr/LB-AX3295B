#include "application.h"

void LzwSetTempBuffer(GifDecoder *decoder, uint8_t *tempBuffer)
{
    decoder->temp_buffer = tempBuffer;
}

// Initialize LZW decoder
//   csize initial code size in bits
//   buf input data
void LzwDecodeInit(GifDecoder *decoder, int csize)
{
    // Initialize read buffer variables
    decoder->bbuf = 0;
    decoder->bbits = 0;
    decoder->bs = 0;
    decoder->bcnt = 0;

    // Initialize decoder variables
    decoder->codesize = csize;
    decoder->cursize = csize + 1;
    decoder->curmask = decoder->mask[decoder->cursize];
    decoder->top_slot = 1 << decoder->cursize;
    decoder->clear_code = 1 << csize;
    decoder->end_code = decoder->clear_code + 1;
    decoder->slot = decoder->newcodes = decoder->clear_code + 2;
    decoder->oc = decoder->fc = -1;
    decoder->sp = decoder->stack;
}

//  Get one code of given number of bits from stream
int LzwGetCode(GifDecoder *decoder)
{
    while (decoder->bbits < decoder->cursize)
    {
        if (decoder->bcnt == decoder->bs)
        {
            // get number of bytes in next block
            ReadIntoBuffer(decoder, decoder->temp_buffer, 1);
            decoder->bs = decoder->temp_buffer[0];
            ReadIntoBuffer(decoder, decoder->temp_buffer, decoder->bs);
            decoder->bcnt = 0;
        }
        decoder->bbuf |= decoder->temp_buffer[decoder->bcnt] << decoder->bbits;
        decoder->bbits += 8;
        decoder->bcnt++;
    }
    int c = decoder->bbuf;
    decoder->bbuf >>= decoder->cursize;
    decoder->bbits -= decoder->cursize;
    return c & decoder->curmask;
}

// Decode given number of bytes
//   buf 8 bit output buffer
//   len number of pixels to decode
//   returns the number of bytes decoded
int LzwDecode(GifDecoder *decoder, uint8_t *buf, int len, uint8_t *bufend)
{
    int l, c, code;

    if (decoder->end_code < 0)
    {
        return 0;
    }
    l = len;

    for (;;)
    {
        while (decoder->sp > decoder->stack)
        {
            // load buf with data if we're still within bounds
            if (buf < bufend)
            {
                *buf++ = *(--decoder->sp);
            }
            else
            {
                // out of bounds, keep incrementing the pointers, but don't use the data
            }
            if ((--l) == 0)
            {
                return len;
            }
        }
        c = LzwGetCode(decoder);
        if (c == decoder->end_code)
        {
            break;
        }
        else if (c == decoder->clear_code)
        {
            decoder->cursize = decoder->codesize + 1;
            decoder->curmask = decoder->mask[decoder->cursize];
            decoder->slot = decoder->newcodes;
            decoder->top_slot = 1 << decoder->cursize;
            decoder->fc = decoder->oc = -1;
        }
        else
        {

            code = c;
            if ((code == decoder->slot) && (decoder->fc >= 0))
            {
                *decoder->sp++ = decoder->fc;
                code = decoder->oc;
            }
            else if (code >= decoder->slot)
            {
                break;
            }
            while (code >= decoder->newcodes)
            {
                *decoder->sp++ = decoder->suffix[code];
                code = decoder->prefix[code];
            }
            *decoder->sp++ = code;
            if ((decoder->slot < decoder->top_slot) && (decoder->oc >= 0))
            {
                decoder->suffix[decoder->slot] = code;
                decoder->prefix[decoder->slot++] = decoder->oc;
            }
            decoder->fc = code;
            decoder->oc = c;
            if (decoder->slot >= decoder->top_slot)
            {
                if (decoder->cursize < LZW_MAXBITS)
                {
                    decoder->top_slot <<= 1;
                    decoder->curmask = decoder->mask[++decoder->cursize];
                }
                else
                {
                }
            }
        }
        hal_wdtClear();
    }
#if GIFDEBUG == 1
    deg_Printf("LZWDecode success\n");
#endif
    decoder->end_code = -1;
    return len - l;
}