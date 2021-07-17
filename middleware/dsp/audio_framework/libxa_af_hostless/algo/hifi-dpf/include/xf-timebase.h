/*
* Copyright (c) 2015-2021 Cadence Design Systems Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*******************************************************************************
 * xf-timebase.h
 *
 * Common timebase for deadline-driven scheduler
 *******************************************************************************/

#ifndef __XF_H
#error "xf-timebase.h mustn't be included directly"
#endif

/*******************************************************************************
 * Timebase for deadline-driven scheduler
 ******************************************************************************/

/* ...set internal scheduler frequency as a LCM of all supported sample rates;
 * it is in general not a problem to have large number here, however it should
 * be noted that maximal-size audio-buffer that we handle, when expressed in
 * ticks of this virtual frequency, must not exceed 2**31 (for otherwise
 * scheduler timestamp comparison function will misbehave).
 */
#define XF_TIMEBASE_FREQ           ((UWORD64)16 * 4 * 3 * 56448000ULL)

/* ...add paranoic check considering maximal audio-buffer duration as 0.1 sec */
C_BUG((UWORD32)(XF_TIMEBASE_FREQ / 10) >= ((UWORD32)1 << 31));

/* ...supported sampling rates */
C_BUG(XF_TIMEBASE_FREQ % 4000);
C_BUG(XF_TIMEBASE_FREQ % 8000);
C_BUG(XF_TIMEBASE_FREQ % 11025);
C_BUG(XF_TIMEBASE_FREQ % 12000);
C_BUG(XF_TIMEBASE_FREQ % 16000);
C_BUG(XF_TIMEBASE_FREQ % 22050);
C_BUG(XF_TIMEBASE_FREQ % 24000);
C_BUG(XF_TIMEBASE_FREQ % 32000);
C_BUG(XF_TIMEBASE_FREQ % 44100);
C_BUG(XF_TIMEBASE_FREQ % 48000);
C_BUG(XF_TIMEBASE_FREQ % 64000);
C_BUG(XF_TIMEBASE_FREQ % 88200);
C_BUG(XF_TIMEBASE_FREQ % 96000);
C_BUG(XF_TIMEBASE_FREQ % 128000);
C_BUG(XF_TIMEBASE_FREQ % 176400);
C_BUG(XF_TIMEBASE_FREQ % 192000);

/* ...calculate upsampling factor for given sample rate */
static inline UWORD32 xf_timebase_factor(UWORD32 sample_rate)
{
    /* ...probably we can tolerate single division */
    switch(sample_rate)
    {
    case 4000:
        return (UWORD32)(XF_TIMEBASE_FREQ / 4000);
    case 8000:
        return (UWORD32)(XF_TIMEBASE_FREQ / 8000);
    case 11025:
        return (UWORD32)(XF_TIMEBASE_FREQ / 11025);
    case 12000:
        return (UWORD32)(XF_TIMEBASE_FREQ / 12000);
    case 16000:
        return (UWORD32)(XF_TIMEBASE_FREQ / 16000);
    case 22050:
        return (UWORD32)(XF_TIMEBASE_FREQ / 22050);
    case 24000:
        return (UWORD32)(XF_TIMEBASE_FREQ / 24000);
    case 32000:
        return (UWORD32)(XF_TIMEBASE_FREQ / 32000);
    case 44100:
        return (UWORD32)(XF_TIMEBASE_FREQ / 44100);
    case 48000:
        return (UWORD32)(XF_TIMEBASE_FREQ / 48000);
    case 64000:
        return (UWORD32)(XF_TIMEBASE_FREQ / 64000);
    case 88200:
        return (UWORD32)(XF_TIMEBASE_FREQ / 88200);
    case 96000:
        return (UWORD32)(XF_TIMEBASE_FREQ / 96000);
    case 128000:
        return (UWORD32)(XF_TIMEBASE_FREQ / 128000);
    case 176400:
        return (UWORD32)(XF_TIMEBASE_FREQ / 176400);
    case 192000:
        return (UWORD32)(XF_TIMEBASE_FREQ / 192000);
    default:
        return 0;
    }
}

/* ...compare timestamps */
static inline int xf_time_after(UWORD32 a, UWORD32 b)
{
    return ((WORD32)(a - b) > 0);
}
    
/* ...compare timstamps */
static inline int xf_time_before(UWORD32 a, UWORD32 b)
{
    return ((WORD32)(a - b) < 0);
}
    
