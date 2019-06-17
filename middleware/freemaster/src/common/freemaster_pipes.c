/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 *
 * License: NXP LA_OPT_NXP_Software_License
 *
 * NXP Confidential. This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms.  If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.  This code may only be used in a microprocessor,
 * microcontroller, sensor or digital signal processor ("NXP Product")
 * supplied directly or indirectly from NXP.  See the full NXP Software
 * License Agreement in license/LA_OPT_NXP_Software_License.pdf
 *
 * FreeMASTER Communication Driver - Pipe I/O
 */

#include "freemaster.h"
#include "freemaster_private.h"
#include "freemaster_protocol.h"

#if FMSTR_USE_PIPES && (!FMSTR_DISABLE)

#if FMSTR_USE_PIPE_PRINTF_VARG
#include <stdarg.h>
#endif

/***********************************
*  local types
***********************************/

/* Flags for command FMSTR_CMD_GETPIPE */
#define FMSTR_PIPE_GETPIPE_FLAG_PORT    0x01

/* Flags for get pipe info in command FMSTR_CMD_GETPIPE */
#define FMSTR_PIPE_GETINFO_FLAG_ISOPEN  0x01

#define FMSTR_PIPE_CFGCODE_NAME 0x81
#define FMSTR_PIPE_CFGCODE_INFO 0x82

/* runtime flags */
typedef union 
{
    FMSTR_FLAGS all;
    
    struct 
    {
        unsigned bExpectOdd : 1;   /* now expecting even round */
        unsigned bInComm : 1;      /* in protocol handler now */
        unsigned bLocked : 1;      /* data buffer access is locked */
    } flg;
    
} FMSTR_PIPE_FLAGS;

typedef union
{
    FMSTR_FLAGS all;

    struct
    {
        unsigned bIsFull : 1;     /* buffer is full (wp==rp, but not empty) */
    } flg;

} FMSTR_PIPE_BUFF_FLAGS;

typedef struct 
{
    FMSTR_ADDR buff;
    FMSTR_PIPE_SIZE size;
    FMSTR_PIPE_SIZE wp;
    FMSTR_PIPE_SIZE rp;
    FMSTR_PIPE_BUFF_FLAGS flags;
} FMSTR_PIPE_BUFF;

typedef struct 
{
    /* pipe information */
    const FMSTR_CHAR *name;      /* String description of the pipe. */
    FMSTR_U8         type;       /* Type of the usage of pipe */       
    
    /* pipe configuration */
    FMSTR_PIPE_BUFF rx;
    FMSTR_PIPE_BUFF tx;
    FMSTR_PIPE_PORT pipePort;
    FMSTR_PPIPEFUNC pCallback;

    /* runtime information */    
    FMSTR_PIPE_FLAGS flags;
    FMSTR_U8 nLastBytesReceived;
    
    /* residue buffer for 8-bit handling on DSP56F8xx platform */
#if FMSTR_CFG_BUS_WIDTH > 1
    FMSTR_U16 rd8Resid;
    FMSTR_U16 wr8Resid;
#endif

#if FMSTR_USE_PIPE_PRINTF
    char printfBuff[FMSTR_PIPES_PRINTF_BUFF_SIZE];
    FMSTR_SIZE8 printfBPtr;
#endif

} FMSTR_PIPE;

/* Pipe ITOA formatting flags */
typedef union 
{
    FMSTR_FLAGS all;
    
    struct 
    {
        unsigned upperc : 1;     /* uppercase HEX letters */
        unsigned zeroes : 1;     /* prepend with zeroes rather than spaces */
        unsigned showsign : 1;   /* show sign always (even if plus) */
        unsigned negative : 1;   /* sign determined during itoa */
        unsigned signedtype : 1; /* type is signed (used with va_list only) */
        unsigned isstring : 1;   /* formatting %s */
        
    } flg;
    
} FMSTR_PIPE_ITOA_FLAGS;

/* Pipe printf context */
typedef struct
{
    FMSTR_PIPE_ITOA_FLAGS flags;
    FMSTR_U8 radix;
    FMSTR_U8 dtsize;
    FMSTR_SIZE8 alen;
    
} FMSTR_PIPE_PRINTF_CTX;

typedef FMSTR_BOOL (*FMSTR_PIPE_ITOA_FUNC)(FMSTR_HPIPE, const void*, FMSTR_PIPE_PRINTF_CTX*);

/***********************************
*  local variables
***********************************/

static FMSTR_PIPE pcm_pipes[FMSTR_USE_PIPES];

/**********************************************************************
*  local macros
**********************************************************************/

#define FMSTR_PIPE_ITOAFMT_BIN  0U
#define FMSTR_PIPE_ITOAFMT_OCT  1U
#define FMSTR_PIPE_ITOAFMT_DEC  2U
#define FMSTR_PIPE_ITOAFMT_HEX  3U
#define FMSTR_PIPE_ITOAFMT_CHAR 4U

#define FMSTR_IS_DIGIT(x) (((x)>='0') && ((x)<='9'))
#define FMSTR_I2XU(x) (((x)<10) ? ((x)+'0') : ((x)-10+'A'))
#define FMSTR_I2XL(x) (((x)<10) ? ((x)+'0') : ((x)-10+'a'))

/**********************************************************************
*  local functions 
**********************************************************************/

static FMSTR_PIPE* FMSTR_FindPipe(FMSTR_PIPE_PORT pipePort);
static FMSTR_PIPE_SIZE FMSTR_PipeGetBytesFree(FMSTR_PIPE_BUFF* pipeBuff);
static FMSTR_PIPE_SIZE FMSTR_PipeGetBytesReady(FMSTR_PIPE_BUFF* pipeBuff);
static void FMSTR_PipeDiscardBytes(FMSTR_PIPE_BUFF* pipeBuff, FMSTR_SIZE8 countBytes);
static FMSTR_BPTR FMSTR_PipeReceive(FMSTR_BPTR msgBuffIO, FMSTR_PIPE* pp, FMSTR_SIZE8 msgBuffSize);
static FMSTR_BPTR FMSTR_PipeTransmit(FMSTR_BPTR msgBuffIO, FMSTR_PIPE* pp, FMSTR_SIZE8 msgBuffSize);
static FMSTR_BOOL FMSTR_PipeIToAFinalize(FMSTR_HPIPE pipeHandle, FMSTR_PIPE_PRINTF_CTX* pctx);

static FMSTR_BOOL FMSTR_PipePrintfOne(FMSTR_HPIPE pipeHandle, const char* pszFmt, void* parg, FMSTR_PIPE_ITOA_FUNC pItoaFunc);

static FMSTR_BOOL FMSTR_PipePrintfAny(FMSTR_HPIPE pipeHandle, va_list* parg, FMSTR_PIPE_PRINTF_CTX* pctx);
static FMSTR_BOOL FMSTR_PipePrintfV(FMSTR_HPIPE pipeHandle, const char* pszFmt, va_list* parg);

/**********************************************************************
*  local "inline" functions (may also be static on some platforms)
**********************************************************************/

FMSTR_INLINE FMSTR_BOOL FMSTR_PipePrintfFlush(FMSTR_HPIPE pipeHandle);
FMSTR_INLINE FMSTR_BOOL FMSTR_PipePrintfPutc(FMSTR_HPIPE pipeHandle, char c);

/**************************************************************************//*!
*
* @brief  Initialization of pipes layer
*
******************************************************************************/

void FMSTR_InitPipes(void)
{
    FMSTR_INDEX i;
    
    for(i=0; i<FMSTR_USE_PIPES; i++)
        pcm_pipes[i].pipePort = 0;
}

/**************************************************************************//*!
*
* @brief  API: Opening the pipe
*
******************************************************************************/

FMSTR_HPIPE FMSTR_PipeOpen(FMSTR_PIPE_PORT pipePort, FMSTR_PPIPEFUNC pipeCallback,
                           FMSTR_ADDR pipeRxBuff, FMSTR_PIPE_SIZE pipeRxSize, 
                           FMSTR_ADDR pipeTxBuff, FMSTR_PIPE_SIZE pipeTxSize, 
                           FMSTR_U8 type, const FMSTR_CHAR *name)
{
    FMSTR_PIPE* pp = &pcm_pipes[0];
    FMSTR_INDEX ifree = -1;
    FMSTR_INDEX i;
    
    for(i=0; i<FMSTR_USE_PIPES; i++, pp++)
    {
        /* find first free pipe */
        if(pp->pipePort == 0 && ifree < 0)
            ifree = i;
        /* look for existing pipe with the same port */
        if(pp->pipePort == pipePort)
            break;
    }
    
    /* pipe not found */
    if(i >= FMSTR_USE_PIPES)
    {
        /* create new pipe */
        if(ifree >= 0)
            pp = &pcm_pipes[ifree];
        /* no slot for creating pipe */
        else
            return NULL;
    }

    FMSTR_MemSet(pp, 0, sizeof(FMSTR_PIPE));
    
    /* disable pipe (just in case the interrupt would come now) */
    pp->pipePort = 0;
        
    /* initialize pipe */
    pp->rx.buff = pipeRxBuff;
    pp->rx.size = pipeRxSize;
    pp->tx.buff = pipeTxBuff;
    pp->tx.size = pipeTxSize;
    pp->pCallback = pipeCallback;
    pp->name = name;
    pp->type = type;
    
#if FMSTR_USE_PIPE_PRINTF
    pp->printfBPtr = 0;
#endif

    /* activate pipe for the new port */
    pp->pipePort = pipePort;

    return (FMSTR_HPIPE) pp;
}

/**************************************************************************//*!
*
* @brief  PIPE API: Close pipe
*
******************************************************************************/

void FMSTR_PipeClose(FMSTR_HPIPE pipeHandle)
{
    FMSTR_PIPE* pp = (FMSTR_PIPE*) pipeHandle;
    
    /* un-initialize pipe */
    if(pp != NULL)
        pp->pipePort = 0;
}

/**************************************************************************//*!
*
* @brief  PIPE API: Write data to a pipe
*
******************************************************************************/

FMSTR_PIPE_SIZE FMSTR_PipeWrite(FMSTR_HPIPE pipeHandle, FMSTR_ADDR pipeData, FMSTR_PIPE_SIZE pipeDataLen, FMSTR_PIPE_SIZE writeGranularity)
{
    FMSTR_PIPE* pp = (FMSTR_PIPE*) pipeHandle;
    FMSTR_PIPE_BUFF* pbuff = &pp->tx;
    FMSTR_PIPE_SIZE total = FMSTR_PipeGetBytesFree(pbuff);
    FMSTR_PIPE_SIZE s;

    /* when invalid address is given, just return number of bytes free */
    if(pipeData)
    {
        /* only fill the free space */
        if(pipeDataLen > total)
            pipeDataLen = total;
        
        /* obey granularity */
        if(writeGranularity > 1)
        {
            pipeDataLen /= writeGranularity;
            pipeDataLen *= writeGranularity;
        }

        /* return value */
        total = pipeDataLen;

        /* valid length only */
        if(pipeDataLen > 0)
        {
            /* total bytes available in the rest of buffer */
            s = (FMSTR_PIPE_SIZE) ((pbuff->size - pbuff->wp) * FMSTR_CFG_BUS_WIDTH);
            if(s > pipeDataLen)
                s = pipeDataLen;
            
            /* get the bytes */
            FMSTR_MemCpyFrom(pbuff->buff + pbuff->wp, pipeData, (FMSTR_SIZE8) s);
            pipeData += s / FMSTR_CFG_BUS_WIDTH;
            
            /* advance & wrap pointer */
            pbuff->wp += s / FMSTR_CFG_BUS_WIDTH;
            if(pbuff->wp >= pbuff->size)
                pbuff->wp = 0;
        
            /* rest of frame to a (wrapped) beggining of buffer */
            pipeDataLen -= (FMSTR_SIZE8) s;
            if(pipeDataLen > 0)
            {
                FMSTR_MemCpyFrom(pbuff->buff + pbuff->wp, pipeData, (FMSTR_SIZE8) pipeDataLen);
                pbuff->wp += pipeDataLen / FMSTR_CFG_BUS_WIDTH;
            }

            /* buffer got full? */
            if(pbuff->wp == pbuff->rp)
                pbuff->flags.flg.bIsFull = 1;
        }
    }
    
    return total;
}

/**************************************************************************//*!
*
* @brief  PIPE API: Put zero-terminated string into pipe. Succeedes only
* if full string fits into the output buffer and return TRUE if so.
*
******************************************************************************/

FMSTR_BOOL FMSTR_PipePuts(FMSTR_HPIPE pipeHandle, const char* pszStr)
{
    FMSTR_PIPE* pp = (FMSTR_PIPE*) pipeHandle;
    FMSTR_PIPE_BUFF* pbuff = &pp->tx;
    FMSTR_PIPE_SIZE free = FMSTR_PipeGetBytesFree(pbuff);
    FMSTR_PIPE_SIZE slen = (FMSTR_PIPE_SIZE) FMSTR_StrLen((FMSTR_ADDR)pszStr);
    
    if(slen > free)
        return FMSTR_FALSE;
    
    FMSTR_PipeWrite(pipeHandle, (FMSTR_ADDR)pszStr, slen, 0);
    return FMSTR_TRUE;
}

/**************************************************************************//*!
*
* @brief  PRINTF-like formatting functions
*
*****************************************************************************/

#if FMSTR_USE_PIPE_PRINTF

/**************************************************************************//*!
*
* @brief  Flush pipe's printf formatting buffer into pipe output buffer
*
*****************************************************************************/

FMSTR_INLINE FMSTR_BOOL FMSTR_PipePrintfFlush(FMSTR_HPIPE pipeHandle)
{
    FMSTR_PIPE* pp = (FMSTR_PIPE*) pipeHandle;
    FMSTR_BOOL ok = FMSTR_TRUE;
    
    if(pp->printfBPtr)
    {
        FMSTR_SIZE8 sz = (FMSTR_SIZE8) FMSTR_PipeWrite(pipeHandle, (FMSTR_ADDR)pp->printfBuff, (FMSTR_PIPE_SIZE)pp->printfBPtr, 0);
        
        /* all characters could NOT be printed */
        if(sz < pp->printfBPtr)
        {
            /* Move not sent characters */
            for(int i=0; i<(pp->printfBPtr - sz); i++)
                pp->printfBuff[i] = pp->printfBuff[sz+i];
            
            pp->printfBPtr -= sz;
            
            ok = FMSTR_FALSE;   
        }
        else
        {
            /* not a cyclic buffer, must start over anyway (also if error) */
            pp->printfBPtr = 0;
        }
    }
    
    return ok;
}
    
/**************************************************************************//*!
*
* @brief  Put one character into pipe's printf formating buffer
*
*****************************************************************************/

FMSTR_INLINE FMSTR_BOOL FMSTR_PipePrintfPutc(FMSTR_HPIPE pipeHandle, char c)
{
    FMSTR_PIPE* pp = (FMSTR_PIPE*) pipeHandle;

    /* when buffer is full */
    if(pp->printfBPtr >= FMSTR_PIPES_PRINTF_BUFF_SIZE)
    {
        /* try to flush some bytes */
        if(!FMSTR_PipePrintfFlush(pipeHandle))
            return FMSTR_FALSE;
    }

    /* abort if still full */
    if(pp->printfBPtr >= FMSTR_PIPES_PRINTF_BUFF_SIZE)
        return FMSTR_FALSE;

    pp->printfBuff[pp->printfBPtr++] = c;
    return FMSTR_TRUE;    
}

/**************************************************************************//*!
*
* @brief  This function finishes the number formatting, adds spacing, signs
*         and reverses the string (the input is comming in reversed order)
*
*****************************************************************************/

static FMSTR_BOOL FMSTR_PipeIToAFinalize(FMSTR_HPIPE pipeHandle, FMSTR_PIPE_PRINTF_CTX* pctx)
{
    FMSTR_PIPE* pp = (FMSTR_PIPE*) pipeHandle;
    FMSTR_SIZE8 bptr, minlen, i, bhalf;
    char z, sgn;

    /* buffer pointer into local variable */
    bptr = pp->printfBPtr;
    
    /* if anything goes wrong, throw our prepared string away */
    pp->printfBPtr = 0;

    /* zero may come as an empty string from itoa procedures */
    if(bptr == 0)
    {
        pp->printfBuff[0] = '0';
        bptr = 1;
    }

    /* first strip the zeroes put by itoa */
    while(bptr > 1 && pp->printfBuff[bptr-1] == '0')
        bptr--;

    /* determine sign to print */       
    if(pctx->flags.flg.negative)
    {
        sgn = '-';
        
        /* minus need to be shown always */
        pctx->flags.flg.showsign = 1U;
    }
    else
    {
        /* plus will be shown if flg.showsign was set by caller */
        sgn = '+';
    }

    /* unsigned types can not print sign */
    if(!pctx->flags.flg.signedtype)
        pctx->flags.flg.showsign = 0U;
    
    /* calculate minimum buffer length needed */
    minlen = bptr;
    if(pctx->flags.flg.showsign)
        minlen++;
    
    /* will it fit? */
    if(FMSTR_PIPES_PRINTF_BUFF_SIZE < minlen)
        return FMSTR_FALSE;

    /* required length should never exceed the buffer length */
    if(pctx->alen > FMSTR_PIPES_PRINTF_BUFF_SIZE)
        pctx->alen = FMSTR_PIPES_PRINTF_BUFF_SIZE;

    /* choose prefix character (zero, space or sign-extension OCT/HEX/BIN) */
    if(pctx->flags.flg.zeroes)
    {
        z = '0';
        
        /* sign extend? */
        if(pctx->flags.flg.negative)
        {
            switch(pctx->radix)
            {
            case FMSTR_PIPE_ITOAFMT_BIN: 
                z = '1'; 
                break;
            case FMSTR_PIPE_ITOAFMT_OCT: 
                z = '7'; 
                break;
            case FMSTR_PIPE_ITOAFMT_HEX: 
                z = pctx->flags.flg.upperc ? 'F' : 'f'; 
                break;
            }
        }
        
        /* the sign will be in front of added zeroes */
        if(pctx->flags.flg.showsign)
            pctx->alen--;
    }
    else
    {
        z = ' ';
        
        /* sign should be in front of the number */
        if(pctx->flags.flg.showsign)
        {
            pp->printfBuff[bptr++] = sgn;
            pctx->flags.flg.showsign = 0; /* prevent it to be added again below  */
        }
    }
    
    /* now fill to required len */
    while(bptr < pctx->alen)
        pp->printfBuff[bptr++] = z;

    /* add the sign if needed */
    if(pctx->flags.flg.showsign)
        pp->printfBuff[bptr++] = sgn;
    
    /* buffer contains this number of characters */
    pp->printfBPtr = bptr;
    
    /* now reverse the string and feed it to pipe */
    bhalf = bptr/2;
    bptr--;
    for(i=0; i<bhalf; i++)
    {
        z = pp->printfBuff[i];
        pp->printfBuff[i] = pp->printfBuff[bptr-i];
        pp->printfBuff[bptr-i] = z;
    }

    return FMSTR_TRUE;  
}

/**************************************************************************//*!
*
* @brief  This function formats the argument into the temporary printf buffer
*         It is granted by the caller that the buffer is empty before calling.
*
*****************************************************************************/

static FMSTR_BOOL FMSTR_PipeU8ToA(FMSTR_HPIPE pipeHandle, const FMSTR_U8* parg, FMSTR_PIPE_PRINTF_CTX* pctx)
{
    FMSTR_PIPE* pp = (FMSTR_PIPE*) pipeHandle;
    FMSTR_U8 arg = *parg;
    FMSTR_U8 tmp;
    FMSTR_INDEX i;
    
    switch(pctx->radix)
    {
        case FMSTR_PIPE_ITOAFMT_CHAR:
            pp->printfBuff[pp->printfBPtr++] = (char)arg;
            break;
        
        case FMSTR_PIPE_ITOAFMT_BIN:
            if(FMSTR_PIPES_PRINTF_BUFF_SIZE < 8)
                return FMSTR_FALSE;
            
            for(i=0; arg && i<8; i++)
            {
                pp->printfBuff[pp->printfBPtr++] = (char)((arg & 1) + '0');
                arg >>= 1;
            }
            break;
             
        case FMSTR_PIPE_ITOAFMT_OCT:
            if(FMSTR_PIPES_PRINTF_BUFF_SIZE < 3)
                return FMSTR_FALSE;
            
            for(i=0; arg && i<3; i++)
            {
                pp->printfBuff[pp->printfBPtr++] = (char)((arg & 7) + '0');
                arg >>= 3;
            }
            break;
             
        case FMSTR_PIPE_ITOAFMT_DEC:
            if(FMSTR_PIPES_PRINTF_BUFF_SIZE < 3)
                return FMSTR_FALSE;
        
            for(i=0; arg && i<3; i++)
            {
                pp->printfBuff[pp->printfBPtr++] = (char)((arg % 10) + '0');
                arg /= 10;
            }
            break;
            
        case FMSTR_PIPE_ITOAFMT_HEX:
        default:
            if(FMSTR_PIPES_PRINTF_BUFF_SIZE < 2)
                return FMSTR_FALSE;
            
            for(i=0; arg && i<2; i++)
            {
                tmp = arg & 15;
                pp->printfBuff[pp->printfBPtr++] = (char)(pctx->flags.flg.upperc ? FMSTR_I2XU(tmp) : FMSTR_I2XL(tmp));
                arg >>= 4;
            }
            break;
    }
    
    return FMSTR_PipeIToAFinalize(pipeHandle, pctx);
}

/**************************************************************************//*!
*
* @brief  This function formats the argument into the temporary printf buffer
*         It is granted by the caller that the buffer is empty before calling.
*
*****************************************************************************/

static FMSTR_BOOL FMSTR_PipeS8ToA(FMSTR_HPIPE pipeHandle, const FMSTR_S8* parg, FMSTR_PIPE_PRINTF_CTX* pctx)
{
    FMSTR_S8 arg = *parg;
    
    if(arg < 0)
    {
        pctx->flags.flg.negative = 1U;

        /* if sign will be shown, then negate the number */     
        if(pctx->flags.flg.signedtype)
            arg *= -1;
    }
        
    return FMSTR_PipeU8ToA(pipeHandle, (const FMSTR_U8*)&arg, pctx);
}

/**************************************************************************//*!
*
* @brief  This function formats the argument into the temporary printf buffer
*         It is granted by the caller that the buffer is empty before calling.
*
*****************************************************************************/

static FMSTR_BOOL FMSTR_PipeU16ToA(FMSTR_HPIPE pipeHandle, const FMSTR_U16* parg, FMSTR_PIPE_PRINTF_CTX* pctx)
{
    FMSTR_PIPE* pp = (FMSTR_PIPE*) pipeHandle;
    FMSTR_U16 arg = *parg;
    FMSTR_U16 tmp;
    FMSTR_INDEX i;
    
    switch(pctx->radix)
    {
        case FMSTR_PIPE_ITOAFMT_CHAR:
            pp->printfBuff[pp->printfBPtr++] = (char)arg;
            break;
        
        case FMSTR_PIPE_ITOAFMT_BIN:
            if(FMSTR_PIPES_PRINTF_BUFF_SIZE < 16)
                return FMSTR_FALSE;
            
            for(i=0; arg && i<16; i++)
            {
                pp->printfBuff[pp->printfBPtr++] = (char)((arg & 1) + '0');
                arg >>= 1;
            }
            break;
             
        case FMSTR_PIPE_ITOAFMT_OCT:
            if(FMSTR_PIPES_PRINTF_BUFF_SIZE < 6)
                return FMSTR_FALSE;
            
            for(i=0; arg && i<6; i++)
            {
                pp->printfBuff[pp->printfBPtr++] = (char)((arg & 7) + '0');
                arg >>= 3;
            }
            break;
             
        case FMSTR_PIPE_ITOAFMT_DEC:
            if(FMSTR_PIPES_PRINTF_BUFF_SIZE < 5)
                return FMSTR_FALSE;
        
            for(i=0; arg && i<5; i++)
            {
                pp->printfBuff[pp->printfBPtr++] = (char)((arg % 10) + '0');
                arg /= 10;
            }
            break;
            
        case FMSTR_PIPE_ITOAFMT_HEX:
        default:
            if(FMSTR_PIPES_PRINTF_BUFF_SIZE < 4)
                return FMSTR_FALSE;
            
            for(i=0; arg && i<4; i++)
            {
                tmp = arg & 15;
                pp->printfBuff[pp->printfBPtr++] = (char)(pctx->flags.flg.upperc ? FMSTR_I2XU(tmp) : FMSTR_I2XL(tmp));
                arg >>= 4;
            }
            break;
    }
    
    return FMSTR_PipeIToAFinalize(pipeHandle, pctx);
}

/**************************************************************************//*!
*
* @brief  This function formats the argument into the temporary printf buffer
*         It is granted by the caller that the buffer is empty before calling.
*
*****************************************************************************/

static FMSTR_BOOL FMSTR_PipeS16ToA(FMSTR_HPIPE pipeHandle, const FMSTR_S16* parg, FMSTR_PIPE_PRINTF_CTX* pctx)
{
    FMSTR_S16 arg = *parg;
    
    if(arg < 0)
    {
        pctx->flags.flg.negative = 1U;
        
        /* if sign will be shown, then negate the number */     
        if(pctx->flags.flg.signedtype)
            arg *= -1;
    }
        
    return FMSTR_PipeU16ToA(pipeHandle, (const FMSTR_U16*)&arg, pctx);
}

/**************************************************************************//*!
*
* @brief  This function formats the argument into the temporary printf buffer
*         It is granted by the caller that the buffer is empty before calling.
*
*****************************************************************************/

static FMSTR_BOOL FMSTR_PipeU32ToA(FMSTR_HPIPE pipeHandle, const FMSTR_U32* parg, FMSTR_PIPE_PRINTF_CTX* pctx)
{
    FMSTR_PIPE* pp = (FMSTR_PIPE*) pipeHandle;
    FMSTR_U32 arg = *parg;
    FMSTR_U32 tmp;
    FMSTR_INDEX i;
    
    switch(pctx->radix)
    {
        case FMSTR_PIPE_ITOAFMT_CHAR:
            pp->printfBuff[pp->printfBPtr++] = (char)arg;
            break;
        
        case FMSTR_PIPE_ITOAFMT_BIN:
            if(FMSTR_PIPES_PRINTF_BUFF_SIZE < 32)
                return FMSTR_FALSE;
            
            for(i=0; arg && i<32; i++)
            {
                pp->printfBuff[pp->printfBPtr++] = (char)((arg & 1) + '0');
                arg >>= 1;
            }
            break;
             
        case FMSTR_PIPE_ITOAFMT_OCT:
            if(FMSTR_PIPES_PRINTF_BUFF_SIZE < 11)
                return FMSTR_FALSE;
            
            for(i=0; arg && i<11; i++)
            {
                pp->printfBuff[pp->printfBPtr++] = (char)((arg & 7) + '0');
                arg >>= 3;
            }
            break;
             
        case FMSTR_PIPE_ITOAFMT_DEC:
            if(FMSTR_PIPES_PRINTF_BUFF_SIZE < 10)
                return FMSTR_FALSE;
        
            for(i=0; arg && i<10; i++)
            {
                pp->printfBuff[pp->printfBPtr++] = (char)((arg % 10) + '0');
                arg /= 10;
            }
            break;
            
        case FMSTR_PIPE_ITOAFMT_HEX:
        default:
            if(FMSTR_PIPES_PRINTF_BUFF_SIZE < 8)
                return FMSTR_FALSE;
            
            for(i=0; arg && i<8; i++)
            {
                tmp = arg & 15;
                pp->printfBuff[pp->printfBPtr++] = (char)(pctx->flags.flg.upperc ? FMSTR_I2XU(tmp) : FMSTR_I2XL(tmp));
                arg >>= 4;
            }
            break;
    }
    
    return FMSTR_PipeIToAFinalize(pipeHandle, pctx);
}

/**************************************************************************//*!
*
* @brief  This function formats the argument into the temporary printf buffer
*         It is granted by the caller that the buffer is empty before calling.
*
*****************************************************************************/

static FMSTR_BOOL FMSTR_PipeS32ToA(FMSTR_HPIPE pipeHandle, const FMSTR_S32* parg, FMSTR_PIPE_PRINTF_CTX* pctx)
{
    FMSTR_S32 arg = *parg;
    
    if(arg < 0)
    {
        pctx->flags.flg.negative = 1U;
        
        /* if sign will be shown, then negate the number */     
        if(pctx->flags.flg.signedtype)
            arg *= -1;
    }
        
    return FMSTR_PipeU32ToA(pipeHandle, (const FMSTR_U32*)&arg, pctx);
}

/**************************************************************************//*!
*
* @brief  This function parses the printf format and sets the context 
*         structure properly.
*
* @return The function returns the pointer to end of format string handled
*
*****************************************************************************/

static const char* FMSTR_PipeParseFormat(const char* pszFmt, FMSTR_PIPE_PRINTF_CTX* pctx)
{
    pctx->flags.all = 0;

    /* skip percent sign */
    if(*pszFmt == '%')
        pszFmt++;
    
    /* show sign always? */
    if(*pszFmt == '+')
    {
        pctx->flags.flg.showsign = 1U;
        pszFmt++;
    }

    /* prefix with zeroes? */
    if(*pszFmt == '0')
    {
        pctx->flags.flg.zeroes = 1U;
        pszFmt++;
    }

    /* parse length */              
    pctx->alen = 0;
    while(FMSTR_IS_DIGIT(*pszFmt))
    {
        pctx->alen *= 10;
        pctx->alen += *pszFmt - '0';
        pszFmt++;
    }
    
    /* default data type is 'int' */
    pctx->dtsize = sizeof(int);
    
    /* parse dtsize modifier */
    switch(*pszFmt)
    {
        /* short modifier (char for hh)*/
        case 'h':
            pctx->dtsize = sizeof(short);
            pszFmt++;
            
            /* one more 'h' means 'char' */
            if(*pszFmt == 'h')
            {
                pctx->dtsize = sizeof(char);
                pszFmt++;
            }
            break;
            
        case 'l':
            pctx->dtsize = sizeof(long);
            pszFmt++;
            break;
    }
    
    /* now finaly concluding to format letter */
    switch(*pszFmt++)
    {
        /* HEXADECIMAL */
        case 'X':
            pctx->flags.flg.upperc = 1U;
            /* falling thru case */
            
        /* hexadecimal */
        case 'x':
            pctx->radix = FMSTR_PIPE_ITOAFMT_HEX;
            break;
    
        /* octal */
        case 'o':
            pctx->radix = FMSTR_PIPE_ITOAFMT_OCT;
            break;
    
        /* binary */
        case 'b':
            pctx->radix = FMSTR_PIPE_ITOAFMT_BIN;
            break;

        /* decimal signed */
        case 'd':
        case 'i':
            pctx->flags.flg.signedtype = 1U;
            /* falling thru case */

        /* decimal unsigned */
        case 'u':
            pctx->radix = FMSTR_PIPE_ITOAFMT_DEC;
            break;

        /* character */ 
        case 'c':
            pctx->radix = FMSTR_PIPE_ITOAFMT_CHAR;
            pctx->dtsize = sizeof(char);
            break;

        /* string */    
        case 's':
            pctx->flags.flg.isstring = 1U;
            pctx->dtsize = sizeof(void*);
            break;
    
    }
    
    return pszFmt;
}

/**************************************************************************//*!
*
* @brief  Printf with one argument passed by pointer.
*
******************************************************************************/

static FMSTR_BOOL FMSTR_PipePrintfOne(FMSTR_HPIPE pipeHandle, const char* pszFmt, void* parg, FMSTR_PIPE_ITOA_FUNC pItoaFunc)
{
    FMSTR_BOOL ok = FMSTR_TRUE;
    FMSTR_PIPE_PRINTF_CTX ctx;

    while(*pszFmt && ok)
    {
        if(*pszFmt == '%')
        {
            pszFmt++;
            
            if(*pszFmt == '%')
            {
                ok = FMSTR_PipePrintfPutc(pipeHandle, '%');
                pszFmt++;
                continue;
            }

            /* empty the pipe's temporary buffer */         
            ok = FMSTR_PipePrintfFlush(pipeHandle);
            
            if(ok)
            {
                pszFmt = FMSTR_PipeParseFormat(pszFmt, &ctx);
                
                if(ctx.flags.flg.isstring)
                {
                    const char* psz = (const char*) parg;
                    ok = FMSTR_PipePuts(pipeHandle, psz ? psz : "NULL");
                }
                else
                {
                    ok = pItoaFunc(pipeHandle, parg, &ctx);
                }
            }
        }
        else
        {
            ok = FMSTR_PipePrintfPutc(pipeHandle, *pszFmt++);
        }
    }
    
    return ok && FMSTR_PipePrintfFlush(pipeHandle);
}

/**************************************************************************//*!
*
* @brief  PIPE API: Format argument into the pipe output stream. The format
*         follows the standard printf format. The leading '%' is optional.
*
******************************************************************************/

FMSTR_BOOL FMSTR_PipePrintfU8(FMSTR_HPIPE pipeHandle, const char* pszFmt, FMSTR_U8 arg)
{
    return FMSTR_PipePrintfOne(pipeHandle, pszFmt, &arg, (FMSTR_PIPE_ITOA_FUNC)FMSTR_PipeU8ToA);
}

/**************************************************************************//*!
*
* @brief  PIPE API: Format argument into the pipe output stream. The format
*         follows the standard printf format. The leading '%' is optional.
*
******************************************************************************/

FMSTR_BOOL FMSTR_PipePrintfS8(FMSTR_HPIPE pipeHandle, const char* pszFmt, FMSTR_S8 arg)
{
    return FMSTR_PipePrintfOne(pipeHandle, pszFmt, &arg, (FMSTR_PIPE_ITOA_FUNC)FMSTR_PipeS8ToA);
}

/**************************************************************************//*!
*
* @brief  PIPE API: Format argument into the pipe output stream. The format
*         follows the standard printf format. The leading '%' is optional.
*
******************************************************************************/

FMSTR_BOOL FMSTR_PipePrintfU16(FMSTR_HPIPE pipeHandle, const char* pszFmt, FMSTR_U16 arg)
{
    return FMSTR_PipePrintfOne(pipeHandle, pszFmt, &arg, (FMSTR_PIPE_ITOA_FUNC)FMSTR_PipeU16ToA);
}

/**************************************************************************//*!
*
* @brief  PIPE API: Format argument into the pipe output stream. The format
*         follows the standard printf format. The leading '%' is optional.
*
******************************************************************************/

FMSTR_BOOL FMSTR_PipePrintfS16(FMSTR_HPIPE pipeHandle, const char* pszFmt, FMSTR_S16 arg)
{
    return FMSTR_PipePrintfOne(pipeHandle, pszFmt, &arg, (FMSTR_PIPE_ITOA_FUNC)FMSTR_PipeS16ToA);
}

/**************************************************************************//*!
*
* @brief  PIPE API: Format argument into the pipe output stream. The format
*         follows the standard printf format. The leading '%' is optional.
*
******************************************************************************/

FMSTR_BOOL FMSTR_PipePrintfU32(FMSTR_HPIPE pipeHandle, const char* pszFmt, FMSTR_U32 arg)
{
    return FMSTR_PipePrintfOne(pipeHandle, pszFmt, &arg, (FMSTR_PIPE_ITOA_FUNC)FMSTR_PipeU32ToA);
}

/**************************************************************************//*!
*
* @brief  PIPE API: Format argument into the pipe output stream. The format
*         follows the standard printf format. The leading '%' is optional.
*
******************************************************************************/

FMSTR_BOOL FMSTR_PipePrintfS32(FMSTR_HPIPE pipeHandle, const char* pszFmt, FMSTR_S32 arg)
{
    return FMSTR_PipePrintfOne(pipeHandle, pszFmt, &arg, (FMSTR_PIPE_ITOA_FUNC)FMSTR_PipeS32ToA);
}


/**************************************************************************//*!
*
* @brief  Format va_list argument into the pipe output stream. This function
*         is called as a part of our printf routine.
*
******************************************************************************/

static FMSTR_BOOL FMSTR_PipePrintfAny(FMSTR_HPIPE pipeHandle, va_list* parg, FMSTR_PIPE_PRINTF_CTX* pctx)
{
    FMSTR_BOOL ok = FMSTR_FALSE;

    switch(pctx->dtsize)
    {
        case 1:
            if(pctx->flags.flg.signedtype)
            {
                FMSTR_S8 arg = (FMSTR_S8)va_arg(*parg, int);
                ok = FMSTR_PipeS8ToA(pipeHandle, &arg, pctx);
            }
            else
            {
                FMSTR_U8 arg = (FMSTR_U8)va_arg(*parg, unsigned);
                ok = FMSTR_PipeU8ToA(pipeHandle, &arg, pctx);
            }
            break;
        
        case 2:
            if(pctx->flags.flg.signedtype)
            {
                FMSTR_S16 arg = (FMSTR_S16)va_arg(*parg, int);
                ok = FMSTR_PipeS16ToA(pipeHandle, &arg, pctx);
            }
            else
            {
                FMSTR_U16 arg = (FMSTR_U16)va_arg(*parg, unsigned);
                ok = FMSTR_PipeU16ToA(pipeHandle, &arg, pctx);
            }
            break;
        
        case 4:
            if(pctx->flags.flg.signedtype)
            {
                FMSTR_S32 arg = (FMSTR_S32)va_arg(*parg, long);
                ok = FMSTR_PipeS32ToA(pipeHandle, &arg, pctx);
            }
            else
            {
                FMSTR_U32 arg = (FMSTR_U32)va_arg(*parg, unsigned long);
                ok = FMSTR_PipeU32ToA(pipeHandle, &arg, pctx);
            }
            break;
    }
    
    return ok;
}

/**************************************************************************//*!
*
* @brief  Printf with va_list arguments prepared.
*
*         This function is not declared static (may be reused as global), 
*         but public prototype is not available (not to force user to
*         have va_list defined.
*
******************************************************************************/

static FMSTR_BOOL FMSTR_PipePrintfV(FMSTR_HPIPE pipeHandle, const char* pszFmt, va_list* parg)
{
    FMSTR_BOOL ok = FMSTR_TRUE;
    FMSTR_PIPE_PRINTF_CTX ctx;

    while(*pszFmt && ok)
    {
        if(*pszFmt == '%')
        {
            pszFmt++;
            
            if(*pszFmt == '%')
            {
                ok = FMSTR_PipePrintfPutc(pipeHandle, '%');
                pszFmt++;
            }
            else
            {
                ok = FMSTR_PipePrintfFlush(pipeHandle);
            
                if(ok)
                {
                    pszFmt = FMSTR_PipeParseFormat(pszFmt, &ctx);

                    if(ctx.flags.flg.isstring)
                    {
                        const char* psz = va_arg(*parg, char*);
                        ok = FMSTR_PipePuts(pipeHandle, psz ? psz : "NULL");
                    }
                    else
                    {
                        ok = FMSTR_PipePrintfAny(pipeHandle, parg, &ctx);
                    }
                }
            }
        }
        else
        {
            ok = FMSTR_PipePrintfPutc(pipeHandle, *pszFmt++);
        }
    }
    
    return ok && FMSTR_PipePrintfFlush(pipeHandle);
}

#if FMSTR_USE_PIPE_PRINTF_VARG

/**************************************************************************//*!
*
* @brief  PIPE API: The printf into the pipe
*
******************************************************************************/

FMSTR_BOOL FMSTR_PipePrintf(FMSTR_HPIPE pipeHandle, const char* pszFmt, ...)
{
    FMSTR_BOOL ok;
    
    va_list args;                            
    va_start(args, pszFmt);
    ok = FMSTR_PipePrintfV(pipeHandle, pszFmt, &args);
    va_end(args);
    
    return ok;
}

#endif /* FMSTR_USE_PIPE_PRINTF_VARG */
#endif /* FMSTR_USE_PIPE_PRINTF */

/**************************************************************************//*!
*
* @brief  PIPE API: Read data from a pipe
*
******************************************************************************/

FMSTR_PIPE_SIZE FMSTR_PipeRead(FMSTR_HPIPE pipeHandle, FMSTR_ADDR pipeData, FMSTR_PIPE_SIZE pipeDataLen, FMSTR_PIPE_SIZE readGranularity)
{
    FMSTR_PIPE* pp = (FMSTR_PIPE*) pipeHandle;
    FMSTR_PIPE_BUFF* pbuff = &pp->rx;
    FMSTR_PIPE_SIZE total = FMSTR_PipeGetBytesReady(pbuff);
    FMSTR_PIPE_SIZE s;

    /* when invalid address is given, return number of bytes available */
    if(pipeData)
    {
        /* round length to bus width */
        pipeDataLen /= FMSTR_CFG_BUS_WIDTH;
        pipeDataLen *= FMSTR_CFG_BUS_WIDTH;
        
        /* only fetch what we have cached */
        if(pipeDataLen > total)
            pipeDataLen = total;

        /* obey granularity */
        if(readGranularity > 1)
        {
            pipeDataLen /= readGranularity;
            pipeDataLen *= readGranularity;
        }
        
        /* return value */
        total = pipeDataLen;

        /* rest of cyclic buffer */
        if(pipeDataLen > 0)
        {
            /* total bytes available in the rest of buffer */
            s = (FMSTR_PIPE_SIZE) ((pbuff->size - pbuff->rp) * FMSTR_CFG_BUS_WIDTH);
            if(s > pipeDataLen)
                s = pipeDataLen;
            
            /* put bytes */
            FMSTR_MemCpyTo(pipeData, pbuff->buff + pbuff->rp, (FMSTR_SIZE8) s);
            pipeData += s / FMSTR_CFG_BUS_WIDTH;
            
            /* advance & wrap pointer */
            pbuff->rp += s / FMSTR_CFG_BUS_WIDTH;
            if(pbuff->rp >= pbuff->size)
                pbuff->rp = 0;
        
            /* rest of frame to a (wrapped) beggining of buffer */
            pipeDataLen -= (FMSTR_SIZE8) s;
            if(pipeDataLen > 0)
            {
                FMSTR_MemCpyTo(pipeData, pbuff->buff + pbuff->rp, (FMSTR_SIZE8) pipeDataLen);
                pbuff->rp += pipeDataLen / FMSTR_CFG_BUS_WIDTH;
            }

            /* buffer for sure not full now */
            pbuff->flags.flg.bIsFull = 0;
        }
    }
    
    return total;
}

/**************************************************************************//*!
*
* @brief  Find pipe by port number
*
******************************************************************************/

static FMSTR_PIPE* FMSTR_FindPipe(FMSTR_PIPE_PORT pipePort)
{
    FMSTR_PIPE* pp = &pcm_pipes[0];
    FMSTR_INDEX i;
    
    for(i=0; i<FMSTR_USE_PIPES; i++, pp++)
    {
        /* look for existing pipe with the same port */
        if(pp->pipePort == pipePort)
            return pp;
    }
    
    return NULL;
}

/**************************************************************************//*!
*
* @brief  Get number of bytes free in the buffer
*
******************************************************************************/

static FMSTR_PIPE_SIZE FMSTR_PipeGetBytesFree(FMSTR_PIPE_BUFF* pipeBuff)
{
    FMSTR_PIPE_SIZE free;

    if(pipeBuff->flags.flg.bIsFull)
    {
        free = 0;
    }
    else if(pipeBuff->wp < pipeBuff->rp)
    {
        free = (FMSTR_PIPE_SIZE)(pipeBuff->rp - pipeBuff->wp);
    }
    else
    {
        free = (FMSTR_PIPE_SIZE)(pipeBuff->size - pipeBuff->wp + pipeBuff->rp);
    }
    
    return (FMSTR_PIPE_SIZE)(free * FMSTR_CFG_BUS_WIDTH);
}

static FMSTR_PIPE_SIZE FMSTR_PipeGetBytesReady(FMSTR_PIPE_BUFF* pipeBuff)
{
    FMSTR_PIPE_SIZE full;

    if(pipeBuff->flags.flg.bIsFull)
    {
        full = (FMSTR_PIPE_SIZE)pipeBuff->size;
    }
    else if(pipeBuff->wp >= pipeBuff->rp)
    {
        full = (FMSTR_PIPE_SIZE)(pipeBuff->wp - pipeBuff->rp);
    }
    else
    {
        full = (FMSTR_PIPE_SIZE)(pipeBuff->size - pipeBuff->rp + pipeBuff->wp);
    }
    
    return (FMSTR_PIPE_SIZE)(full * FMSTR_CFG_BUS_WIDTH);
}

static void FMSTR_PipeDiscardBytes(FMSTR_PIPE_BUFF* pipeBuff, FMSTR_SIZE8 countBytes)
{
    FMSTR_PIPE_SIZE total;
    FMSTR_PIPE_SIZE discard;
    
    total = FMSTR_PipeGetBytesReady(pipeBuff);
    discard = (FMSTR_PIPE_SIZE) (countBytes > total ? total : countBytes);
    discard /= FMSTR_CFG_BUS_WIDTH;

    if(discard > 0)
    {
        FMSTR_PIPE_SIZE rest = (FMSTR_PIPE_SIZE)(pipeBuff->size - pipeBuff->rp);
        FMSTR_PIPE_SIZE rp;

        /* will RP wrap? */
        if(rest <= discard)
        {
            rp = (FMSTR_PIPE_SIZE) (discard - rest);
        }
        else
        {
            rp = (FMSTR_PIPE_SIZE) (pipeBuff->rp + discard);
        }

        /* buffer is for sure not full */
        pipeBuff->flags.flg.bIsFull = 0;
        pipeBuff->rp = rp;
    }
}

/* get data from frame into our Rx buffer, we are already sure it fits */

static FMSTR_BPTR FMSTR_PipeReceive(FMSTR_BPTR msgBuffIO, FMSTR_PIPE* pp, FMSTR_SIZE8 msgBuffSize)
{
    FMSTR_PIPE_BUFF* pbuff = &pp->rx;
    FMSTR_PIPE_SIZE s;
    
    if(msgBuffSize > 0)
    {
        /* total bytes available in the rest of buffer */
        s = (FMSTR_PIPE_SIZE) ((pbuff->size - pbuff->wp) * FMSTR_CFG_BUS_WIDTH);
        if(s > (FMSTR_PIPE_SIZE) msgBuffSize)
            s = (FMSTR_PIPE_SIZE) msgBuffSize;
        
        /* get the bytes */
        msgBuffIO = FMSTR_CopyFromBuffer(pbuff->buff + pbuff->wp, msgBuffIO, (FMSTR_SIZE8) s);
        
        /* advance & wrap pointer */
        pbuff->wp += s / FMSTR_CFG_BUS_WIDTH;
        if(pbuff->wp >= pbuff->size)
            pbuff->wp = 0;
    
        /* rest of frame to a (wrapped) beginning of buffer */
        msgBuffSize -= (FMSTR_SIZE8) s;
        if(msgBuffSize > 0)
        {
            msgBuffIO = FMSTR_CopyFromBuffer(pbuff->buff + pbuff->wp, msgBuffIO, msgBuffSize);
            pbuff->wp += msgBuffSize / FMSTR_CFG_BUS_WIDTH;
        }

        /* buffer got full? */
        if(pbuff->wp == pbuff->rp)
            pbuff->flags.flg.bIsFull = 1;
    }
    
    return msgBuffIO;
}

/* put data into the comm buffer, we are already sure it fits, buffer's RP is not modified */

static FMSTR_BPTR FMSTR_PipeTransmit(FMSTR_BPTR msgBuffIO, FMSTR_PIPE* pp, FMSTR_SIZE8 msgBuffSize)
{
    FMSTR_PIPE_BUFF* pbuff = &pp->tx;
    FMSTR_PIPE_SIZE s, rp = pbuff->rp;
    
    if(msgBuffSize > 0)
    {
        /* total bytes available in the rest of buffer */
        s = (FMSTR_PIPE_SIZE) ((pbuff->size - rp) * FMSTR_CFG_BUS_WIDTH);
        if(s > (FMSTR_PIPE_SIZE) msgBuffSize)
            s = (FMSTR_PIPE_SIZE) msgBuffSize;
        
        /* put bytes */
        msgBuffIO = FMSTR_CopyToBuffer(msgBuffIO, pbuff->buff + rp, (FMSTR_SIZE8) s);
        
        /* advance & wrap pointer */
        rp += s / FMSTR_CFG_BUS_WIDTH;
        if(rp >= pbuff->size)
            rp = 0;
    
        /* rest of frame to a (wrapped) beggining of buffer */
        msgBuffSize -= (FMSTR_SIZE8) s;
        if(msgBuffSize > 0)
        {
            msgBuffIO = FMSTR_CopyToBuffer(msgBuffIO, pbuff->buff + rp, msgBuffSize);
        }
    }
    
    return msgBuffIO;
}

/**************************************************************************//*!
*
* @brief  Get PIPE info
*
* @param    msgBuffIO - original command (in) and response buffer (out)
* @param    msgSize   - size of data in buffer
* @param    retStatus - response status
*
* @param  msgBuffIO - original command (in) and response buffer (out) 
*
******************************************************************************/

FMSTR_BPTR FMSTR_GetPipe(FMSTR_BPTR msgBuffIO, FMSTR_SIZE msgSize, FMSTR_U8 *retStatus)
{
    FMSTR_BPTR response = msgBuffIO;
    FMSTR_U8 cfgCode, pipeIndex, pipeFlags;
    FMSTR_PIPE* pp;
    
    FMSTR_ASSERT(msgBuffIO != NULL);
    FMSTR_ASSERT(retStatus != NULL);
    
    /* need at least pipe index and cfgCode */
    if(msgSize < 2)
    {
        /* return status  */
        *retStatus = FMSTR_STC_PIPEERR;
        return response;
    }
    
    /* get Pipe flags */
    msgBuffIO = FMSTR_ValueFromBuffer8(&pipeFlags, msgBuffIO);
    
    /* get Pipe index */
    msgBuffIO = FMSTR_ValueFromBuffer8(&pipeIndex, msgBuffIO);
    
    /* get Pipe cfgCode */
    msgBuffIO = FMSTR_ValueFromBuffer8(&cfgCode, msgBuffIO);
    
    /* Pipe index is a port number of a pipe */
    if(pipeFlags & FMSTR_PIPE_GETPIPE_FLAG_PORT)
    {
        /* Find pipe by port number */
        pp = FMSTR_FindPipe(pipeIndex);
        if(!pp)
        {
            /* return status  */
            *retStatus = FMSTR_STC_PIPEERR;
            return response;
        }
    }
    else
    {
        /* Check maximum pipe index */
        if(pipeIndex >= FMSTR_USE_PIPES)
        {
            /* return status  */
            *retStatus = FMSTR_STC_INSTERR;
            return response;
        }
        
        /* Get pipe from the list */
        pp = &pcm_pipes[pipeIndex];
    }
    
    switch(cfgCode)
    {
    case FMSTR_PIPE_CFGCODE_NAME:
        /* Put pipe name */
        if(pp->name)
            response = FMSTR_CopyToBuffer(response, (FMSTR_ADDR) pp->name, FMSTR_StrLen(pp->name));
        break;
    case FMSTR_PIPE_CFGCODE_INFO:     
        pipeFlags = 0;
        
        /* Add flag isOpen, when pipe is opened (has port number) */
        if(pp->pipePort != 0)
            pipeFlags |= FMSTR_PIPE_GETINFO_FLAG_ISOPEN;
        
        /* Put pipe port */
        response = FMSTR_ValueToBuffer8(response, pp->pipePort);
        /* Put pipe type */
        response = FMSTR_ValueToBuffer8(response, pp->type);
        /* Put pipe flags */
        response = FMSTR_ValueToBuffer8(response, pipeFlags);
        break;
    }
    
    /* success  */
    *retStatus = FMSTR_STS_OK | FMSTR_STSF_VARLEN;
    return response;
}

/**************************************************************************//*!
*
* @brief  Handling PIPE commands
*
* @param    msgBuffIO - original command (in) and response buffer (out)
* @param    msgSize   - size of data in buffer
* @param    retStatus - response status
*
* @param  msgBuffIO - original command (in) and response buffer (out) 
*
******************************************************************************/

FMSTR_BPTR FMSTR_PipeFrame(FMSTR_BPTR msgBuffIO, FMSTR_SIZE msgSize, FMSTR_U8 *retStatus)
{
    FMSTR_BPTR response = msgBuffIO;
    FMSTR_U8 skipLen, pipePort;
    FMSTR_PIPE* pp;
    
    FMSTR_ASSERT(msgBuffIO != NULL);
    FMSTR_ASSERT(retStatus != NULL);
    
    /* need at least port number and tx-discard bytes */
    if(msgSize < 1)
    {
        /* return status  */
        *retStatus = FMSTR_STC_PIPEERR;
        return response;
    }
    
    /* get port number and even/odd flag */
    msgBuffIO = FMSTR_ValueFromBuffer8(&pipePort, msgBuffIO);

    /* get pipe by port */
    pp = FMSTR_FindPipe((FMSTR_PIPE_PORT)(pipePort & 0x7f));

    /* pipe port must exist (i.e. be open) */    
    if(!pp)
    {
        /* return status  */
        *retStatus = FMSTR_STC_PIPEERR;
        return response;
    }
    
    /* data-in are valid only in "matching" request (even to even, odd to odd) */
    if(pipePort & 0x80)
    {
        if(!pp->flags.flg.bExpectOdd)
            msgSize = 0;
        else
            pp->flags.flg.bExpectOdd = !pp->flags.flg.bExpectOdd;
    }
    else
    {
        if(pp->flags.flg.bExpectOdd)
            msgSize = 0;
        else
            pp->flags.flg.bExpectOdd = !pp->flags.flg.bExpectOdd;
    }    
    
    /* process received data */
    if(msgSize > 0)
    {
        /* first byte tells me how many output bytes can be discarded from my 
           pipe-transmit buffer (this is how PC acknowledges how many bytes it 
           received and saved from the last response) */
        msgBuffIO = FMSTR_ValueFromBuffer8(&skipLen, msgBuffIO);
    
        /* discard bytes from pipe's transmit buffer */
        if(skipLen)
            FMSTR_PipeDiscardBytes(&pp->tx, skipLen);
        
        /* next come (msgSize-2) bytes to be received */
        if(msgSize > 2)
        {
            /* how many bytes may I accept? */
            FMSTR_PIPE_SIZE rxFree = FMSTR_PipeGetBytesFree(&pp->rx);
            /* how many bytes PC want to push? */
            FMSTR_U8 rxToRead = (FMSTR_U8)(msgSize - 2);
            
            /* round to bus width */
            rxToRead /= FMSTR_CFG_BUS_WIDTH;
            rxToRead *= FMSTR_CFG_BUS_WIDTH;
            
            /* get the lower of the two numbers */
            if(rxFree < (FMSTR_PIPE_SIZE)rxToRead)
                rxToRead = (FMSTR_U8)rxFree;
            
            /* get frame data */
            /* msgBuffIO = */FMSTR_PipeReceive(msgBuffIO, pp, rxToRead);
            
            /* this is the number to be returned to PC to inform it how
               many bytes it may discard in his transmit buffer */
            pp->nLastBytesReceived = rxToRead;
        }
        else
        {
            /* no bytes received */
            pp->nLastBytesReceived = 0;
        }
    }
    
    /* now call the pipe's handler, it may read or write data */
    if(pp->pCallback)
    {
        pp->flags.flg.bInComm = 1;
        pp->pCallback((FMSTR_HPIPE) pp);
        pp->flags.flg.bInComm = 0;
    }
    
    /* now put our output data */
    {
        /* how many bytes are waiting to be sent? */
        FMSTR_PIPE_SIZE txAvail = FMSTR_PipeGetBytesReady(&pp->tx);
        /* how many bytes I can safely put? */
        FMSTR_U8 txToSend = FMSTR_COMM_BUFFER_SIZE - 3;
        
        /* round to bus width */
        txToSend /= FMSTR_CFG_BUS_WIDTH;
        txToSend *= FMSTR_CFG_BUS_WIDTH;
        
        /* get the lower of two values */
        if(txAvail < (FMSTR_PIPE_SIZE)txToSend)
            txToSend = (FMSTR_U8)txAvail;
        
        /* send pipe's transmit data back */
        response = FMSTR_ValueToBuffer8(response, pipePort);
    
        /* inform PC how many bytes it may discard from its pipe's transmit buffer */
        skipLen = pp->nLastBytesReceived;
        response = FMSTR_ValueToBuffer8(response, skipLen);

        /* put data */    
        if(txToSend)
            response = FMSTR_PipeTransmit(response, pp, txToSend);        
    }
    
    /* success  */
    *retStatus = FMSTR_STS_OK | FMSTR_STSF_VARLEN;
    return response;
}

#else /* FMSTR_USE_PIPES && (!FMSTR_DISABLE) */

/* implement void pipe-API functions */

FMSTR_HPIPE FMSTR_PipeOpen(FMSTR_PIPE_PORT pipePort, FMSTR_PPIPEFUNC pipeCallback,
                           FMSTR_ADDR pipeRxBuff, FMSTR_PIPE_SIZE pipeRxSize, 
                           FMSTR_ADDR pipeTxBuff, FMSTR_PIPE_SIZE pipeTxSize,
                           FMSTR_U8 type, const FMSTR_CHAR *name)
{
    FMSTR_UNUSED(pipePort);
    FMSTR_UNUSED(pipeCallback);
    FMSTR_UNUSED(pipeRxBuff);
    FMSTR_UNUSED(pipeRxSize);
    FMSTR_UNUSED(pipeTxBuff);
    FMSTR_UNUSED(pipeTxSize);
    FMSTR_UNUSED(type);
    FMSTR_UNUSED(name);
    
    return NULL;
}

void FMSTR_PipeClose(FMSTR_HPIPE pipeHandle)
{
    FMSTR_UNUSED(pipeHandle);
}

FMSTR_PIPE_SIZE FMSTR_PipeWrite(FMSTR_HPIPE pipeHandle, FMSTR_ADDR pipeData, FMSTR_PIPE_SIZE pipeDataLen, FMSTR_PIPE_SIZE writeGranularity)
{
    FMSTR_UNUSED(pipeHandle);
    FMSTR_UNUSED(pipeData);
    FMSTR_UNUSED(pipeDataLen);
    FMSTR_UNUSED(writeGranularity);
    
    return 0U;
}

FMSTR_PIPE_SIZE FMSTR_PipeRead(FMSTR_HPIPE pipeHandle, FMSTR_ADDR pipeData, FMSTR_PIPE_SIZE pipeDataLen, FMSTR_PIPE_SIZE readGranularity)
{
    FMSTR_UNUSED(pipeHandle);
    FMSTR_UNUSED(pipeData);
    FMSTR_UNUSED(pipeDataLen);
    FMSTR_UNUSED(readGranularity);
    
    return 0U;
}

/*lint -efile(766, freemaster_protocol.h) include file is not used in this case */

#endif /* FMSTR_USE_PIPES  && (!FMSTR_DISABLE) */

#if (FMSTR_DISABLE) || (!(FMSTR_USE_PIPES)) || (!(FMSTR_USE_PIPE_PRINTF))

FMSTR_BOOL FMSTR_PipePrintfU8(FMSTR_HPIPE pipeHandle, const char* pszFmt, FMSTR_U8 arg)
{
    FMSTR_UNUSED(pipeHandle);
    FMSTR_UNUSED(pszFmt);
    FMSTR_UNUSED(arg);
    
    return FMSTR_FALSE;
}

FMSTR_BOOL FMSTR_PipePrintfS8(FMSTR_HPIPE pipeHandle, const char* pszFmt, FMSTR_S8 arg)
{
    FMSTR_UNUSED(pipeHandle);
    FMSTR_UNUSED(pszFmt);
    FMSTR_UNUSED(arg);
    
    return FMSTR_FALSE;
}

FMSTR_BOOL FMSTR_PipePrintfU16(FMSTR_HPIPE pipeHandle, const char* pszFmt, FMSTR_U16 arg)
{
    FMSTR_UNUSED(pipeHandle);
    FMSTR_UNUSED(pszFmt);
    FMSTR_UNUSED(arg);
    
    return FMSTR_FALSE;
}

FMSTR_BOOL FMSTR_PipePrintfS16(FMSTR_HPIPE pipeHandle, const char* pszFmt, FMSTR_S16 arg)
{
    FMSTR_UNUSED(pipeHandle);
    FMSTR_UNUSED(pszFmt);
    FMSTR_UNUSED(arg);
    
    return FMSTR_FALSE;
}

FMSTR_BOOL FMSTR_PipePrintfU32(FMSTR_HPIPE pipeHandle, const char* pszFmt, FMSTR_U32 arg)
{
    FMSTR_UNUSED(pipeHandle);
    FMSTR_UNUSED(pszFmt);
    FMSTR_UNUSED(arg);
    
    return FMSTR_FALSE;
}

FMSTR_BOOL FMSTR_PipePrintfS32(FMSTR_HPIPE pipeHandle, const char* pszFmt, FMSTR_S32 arg)
{
    FMSTR_UNUSED(pipeHandle);
    FMSTR_UNUSED(pszFmt);
    FMSTR_UNUSED(arg);
    
    return FMSTR_FALSE;
}

#endif /* (!(FMSTR_USE_PIPES)) || (!(FMSTR_USE_PIPE_PRINTF)) */

#if (FMSTR_DISABLE) || (!(FMSTR_USE_PIPES)) || (!(FMSTR_USE_PIPE_PRINTF_VARG))

FMSTR_BOOL FMSTR_PipePrintf(FMSTR_HPIPE pipeHandle, const char* pszFmt, ...)
{
    FMSTR_UNUSED(pipeHandle);
    FMSTR_UNUSED(pszFmt);
    
    return FMSTR_FALSE;
}

#endif /* (!(FMSTR_USE_PIPES)) || (!(FMSTR_USE_PIPE_PRINTF_VARG)) */

