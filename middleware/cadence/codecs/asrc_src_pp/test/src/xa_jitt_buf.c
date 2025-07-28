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


#include "xa_jitt_buf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int JittBufInit(xa_src_jitt_struct *pJittBuf, void * pBuf, int JittBufLen, int PcmWidth,  int NumCh, int JittBufInitLvl)
{
    if ( PcmWidth == 2 )
    {
        JittBufLen = JittBufLen / sizeof(short);
    }
    else
    {
        JittBufLen = JittBufLen / sizeof(int);
    }

    if((pJittBuf == NULL) || (pBuf == NULL))
        return JITT_BUF_PTR_ERR;

    if ((JittBufLen <= 0) || (JittBufLen >= JITT_BUF_MAX_SIZE))
        return  JITT_BUF_INIT_ERR;

    pJittBuf->pJittBuf = pBuf;
    pJittBuf->JittBufSize = JittBufLen;


    pJittBuf->JittBufLvl = JittBufInitLvl * NumCh;

    // Zero out jitter buffer.
    if ( PcmWidth == 2 )
    {
        memset(pJittBuf->pJittBuf, 0, pJittBuf->JittBufSize * sizeof(short)); 
    }
    else
    {
        memset(pJittBuf->pJittBuf, 0, pJittBuf->JittBufSize * sizeof(int));     
    }

    return JITT_BUF_NO_ERR;
}

int JittBufLevelUpdate(xa_src_jitt_struct *pJittBuf, int filled_data)
{
    if(pJittBuf == NULL) 
        return JITT_BUF_PTR_ERR;

    if ((filled_data < 0) || (filled_data >= JITT_BUF_MAX_SIZE))
        return  JITT_BUF_INIT_ERR;

    pJittBuf->JittBufLvl =  pJittBuf->JittBufLvl + filled_data;  // init level to half of size.

    if ( (pJittBuf->JittBufLvl < 0) ||(pJittBuf->JittBufLvl > pJittBuf->JittBufSize) )
    {
        return  JITT_BUF_ERR_BUF_OVERFLOW;
    }

    return JITT_BUF_NO_ERR;
}

int JittBufMove(xa_src_jitt_struct *pJittBuf, int consumed_samples, int pcm_width)
{ 
    if(pJittBuf == NULL) 
        return JITT_BUF_PTR_ERR;

    if ((consumed_samples <= 0) || (consumed_samples >= JITT_BUF_MAX_SIZE))
        return  JITT_BUF_INIT_ERR;

    if ( consumed_samples >  pJittBuf->JittBufLvl )
    {
        return  JITT_BUF_ERR_BUF_UNDERFLOW;
    }

    if ( pcm_width == 2 )
    {
        short *pDest = ( short *)pJittBuf->pJittBuf;
        memmove( pJittBuf->pJittBuf, &pDest[consumed_samples], ( pJittBuf->JittBufLvl - consumed_samples)* sizeof(short) );
    }
    else
    {
        int *pDest = ( int *)pJittBuf->pJittBuf;
        memmove( pJittBuf->pJittBuf, &pDest[ consumed_samples ], ( pJittBuf->JittBufLvl - consumed_samples)*sizeof(int) );    
    }

    pJittBuf->JittBufLvl = pJittBuf->JittBufLvl - consumed_samples;

    if ( (pJittBuf->JittBufLvl < 0) ||(pJittBuf->JittBufLvl >= pJittBuf->JittBufSize) )
    {
        return  JITT_BUF_ERR_BUF_OVERFLOW;
    }

    return JITT_BUF_NO_ERR;
}



int JittBufWrite(xa_src_jitt_struct *pJittBuf, void * pInBuf, int InpBufLen, int PcmWidth, int NumCh )
{    
    int tot_bytes_to_write;
    if((pJittBuf == NULL) || (pInBuf == NULL))
        return JITT_BUF_PTR_ERR;

    // Check if there is space in jitter buffer
    if ((pJittBuf->JittBufLvl + InpBufLen) > pJittBuf->JittBufSize)
        return JITT_BUF_ERR_BUF_OVERFLOW;

    //  memcpy the latest input
    tot_bytes_to_write = InpBufLen * NumCh;

    if ( PcmWidth == 2 )
    {
        short *pOut = ( short *)pJittBuf->pJittBuf;
        tot_bytes_to_write *= 2;      
        memcpy(&pOut[pJittBuf->JittBufLvl], pInBuf, tot_bytes_to_write);
    }
    else
    {
        int *pOut = ( int *)pJittBuf->pJittBuf;
        tot_bytes_to_write *= 4;            
        memcpy(&pOut[pJittBuf->JittBufLvl], pInBuf, tot_bytes_to_write);
    }

    pJittBuf->JittBufLvl += (InpBufLen*NumCh);

    return JITT_BUF_NO_ERR;
}

int JittBufRead(xa_src_jitt_struct *pJittBuf, void * pOutBuf, int OutBufLen, int PcmWidth, int NumCh)
{
    int tot_bytes_read;
    if((pJittBuf == NULL) || (pOutBuf == NULL))
        return JITT_BUF_PTR_ERR;
    
    // Check if there is data in jitter buffer
    if (pJittBuf->JittBufLvl < OutBufLen)
        return JITT_BUF_ERR_BUF_UNDERFLOW;

    tot_bytes_read = OutBufLen * NumCh;

    if ( PcmWidth == 2 )
    {
        short *pIn = ( short *)pJittBuf->pJittBuf;
        tot_bytes_read *= 2;      
        memcpy(pOutBuf, pIn, tot_bytes_read);
    }
    else
    {
        int *pIn = ( int *)pJittBuf->pJittBuf;
        tot_bytes_read *= 4;            
        memcpy(pOutBuf, pIn, tot_bytes_read);
    }    

    return JITT_BUF_NO_ERR;
}

int JittBufGetLvl(xa_src_jitt_struct *pJittBuf)
{
      if(pJittBuf == NULL)
        return JITT_BUF_PTR_ERR;

    return pJittBuf->JittBufLvl;
}
