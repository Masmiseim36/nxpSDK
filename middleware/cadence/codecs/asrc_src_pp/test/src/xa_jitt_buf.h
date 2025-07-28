/*
 * Copyright (c) 2011-2025 Cadence Design Systems, Inc.
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

#include<math.h>

// Defines
#define JITT_BUF_MAX_SIZE                212992 //ceil(((512*24*8+512*8)*2)*1.04) 

#define MAX_ASRC_DRIFT_FLOAT                0.04
#define MAX_REMAINING_SAMPLES               7

// Structs
typedef struct
{
    void *pJittBuf;
    int JittBufSize;
    int JittBufLvl;

}xa_src_jitt_struct;


// Function definitions
int JittBufInit(xa_src_jitt_struct *pJittBuf, void * pBuf, int JittBufLen, int PcmWidth, int NumCh, int JittBufInitLvl);

int JittBufWrite(xa_src_jitt_struct *pJittBuf, void * pInBuf, int InpBufLen, int PcmWidth, int NumCh);

int JittBufRead(xa_src_jitt_struct *pJittBuf, void * pOutBuf, int OutBufLen, int PcmWidth, int NumCh);

int JittBufGetLvl(xa_src_jitt_struct *pJittBuf);

int JittBufLevelUpdate(xa_src_jitt_struct *pJittBuf, int filled_data);
int JittBufMove(xa_src_jitt_struct *pJittBuf, int consumed_samples, int pcm_width);

// Error codes
#define    JITT_BUF_ERR_BUF_OVERFLOW    -1
#define    JITT_BUF_ERR_INP_LARGE       -2
#define    JITT_BUF_ERR_BUF_UNDERFLOW   -3
#define    JITT_BUF_PTR_ERR             -4
#define    JITT_BUF_INIT_ERR            -5

#define    JITT_BUF_NO_ERR              0

#define CHKERR( e ) \
    if(e != JITT_BUF_NO_ERR )\
    {\
        printf("JITT_BUF_ERR : %d \n", e);\
    } 



