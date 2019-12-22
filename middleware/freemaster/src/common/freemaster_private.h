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
 * FreeMASTER Communication Driver - private API
 */

#ifndef __FREEMASTER_PRIVATE_H
#define __FREEMASTER_PRIVATE_H

#ifndef __FREEMASTER_H
#error Please include the freemaster.h master header file before the freemaster_private.h
#endif

/******************************************************************************
* Inline functions
******************************************************************************/

/* we do not assume the inline is always supported by compiler
  rather each platform header defines its FMSTR_INLINE */
#ifndef FMSTR_INLINE
#define FMSTR_INLINE static inline
#endif

/* building macro-based inline code */
#define FMSTR_MACROCODE_BEGIN()     do{
#define FMSTR_MACROCODE_END()       }while(0)


#ifdef __cplusplus
  extern "C" {
#endif
/******************************************************************************
* Internal data types used
******************************************************************************/

/* Transport protocol interface between the Protocol layer and communication subsystem */

typedef struct FMSTR_TRANSPORT_INTF_S
{
    FMSTR_BOOL (*Init)(void);
    void (*Poll)(void);
    void (*SendResponse)(FMSTR_BPTR pResponse, FMSTR_SIZE nLength, FMSTR_U8 statusCode);
} FMSTR_TRANSPORT_INTF;

/* There are multiple global instances of different transports. User selects one in
   the application configuration file (or defines it indirectly by macros like
   FMSTR_USE_UART). */

extern const FMSTR_TRANSPORT_INTF FMSTR_TRANSPORT;

/******************************************************************************
* Global non-API functions (used internally in FreeMASTER driver)
******************************************************************************/

FMSTR_BOOL FMSTR_ProtocolDecoder(FMSTR_BPTR msgBuffIO, FMSTR_SIZE msgSize, FMSTR_U8 cmdCode);
FMSTR_BOOL FMSTR_SendTestFrame(FMSTR_BPTR msgBuffIO);

void FMSTR_InitAppCmds(void);
FMSTR_BPTR FMSTR_StoreAppCmd(FMSTR_BPTR msgBuffIO, FMSTR_SIZE msgSize, FMSTR_U8 *retStatus);
FMSTR_BPTR FMSTR_GetAppCmdStatus(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus);
FMSTR_BPTR FMSTR_GetAppCmdRespData(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus);

void FMSTR_InitScope(void);
FMSTR_BPTR FMSTR_SetScope(FMSTR_BPTR msgBuffIO, FMSTR_SIZE inputLen, FMSTR_U8 *retStatus);
FMSTR_BPTR FMSTR_ReadScope(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus, FMSTR_SIZE maxOutSize);

void FMSTR_InitRec(void);
FMSTR_BPTR FMSTR_SetRecCmd(FMSTR_BPTR msgBuffIO, FMSTR_SIZE inputLen, FMSTR_U8 *retStatus);
FMSTR_BPTR FMSTR_GetRecCmd(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus);
FMSTR_BOOL FMSTR_IsInRecBuffer(FMSTR_ADDR addr, FMSTR_SIZE8 size);

void FMSTR_InitTsa(void);
FMSTR_BPTR FMSTR_GetTsaInfo(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus);
FMSTR_BPTR FMSTR_GetStringLen(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus);
FMSTR_BOOL FMSTR_CheckTsaSpace(FMSTR_ADDR varAddr, FMSTR_SIZE varSize, FMSTR_BOOL writeAccess);

FMSTR_BPTR FMSTR_UresControl(FMSTR_BPTR msgBuffIO, FMSTR_SIZE msgSize, FMSTR_U8 *retStatus);

void FMSTR_InitPipes(void);
FMSTR_BPTR FMSTR_PipeFrame(FMSTR_BPTR msgBuffIO, FMSTR_SIZE msgSize, FMSTR_U8 *retStatus);
FMSTR_BPTR FMSTR_GetPipe(FMSTR_BPTR msgBuffIO, FMSTR_SIZE msgSize, FMSTR_U8 *retStatus);

void FMSTR_InitPDBdm(void);

/******************************************************************************
* communication buffer access functions
******************************************************************************/

void FMSTR_MemCpyTo(volatile void* destAddr, volatile void* srcBuff, FMSTR_SIZE size);
void FMSTR_MemCpyFrom(volatile void* destBuff, volatile void* srcAddr, FMSTR_SIZE size);
void FMSTR_MemCpyToMasked(volatile void* destAddr, volatile void* srcBuff, volatile void* maskBuff, FMSTR_SIZE size);

FMSTR_BPTR FMSTR_CopyToBuffer(FMSTR_BPTR destBuff, FMSTR_ADDR srcAddr, FMSTR_SIZE size);
FMSTR_BPTR FMSTR_CopyFromBuffer(FMSTR_ADDR destAddr, FMSTR_BPTR srcBuff, FMSTR_SIZE size);
void FMSTR_CopyFromBufferWithMask(FMSTR_ADDR destAddr, FMSTR_BPTR srcBuff, FMSTR_SIZE size);

FMSTR_BPTR FMSTR_ValueFromBuffer16(FMSTR_U16* dest, FMSTR_BPTR src);
FMSTR_BPTR FMSTR_ValueFromBuffer32(FMSTR_U32* dest, FMSTR_BPTR src);
FMSTR_BPTR FMSTR_ValueToBuffer16(FMSTR_BPTR dest, FMSTR_U16 src);
FMSTR_BPTR FMSTR_ValueToBuffer32(FMSTR_BPTR dest, FMSTR_U32 src);

#define FMSTR_ValueFromBuffer8(dest, src) \
    ( (*((FMSTR_U8*)(dest)) = *(FMSTR_U8*)(src)), (((FMSTR_BPTR)(src))+1) )

#define FMSTR_ValueToBuffer8(dest, src) \
    ( (*((FMSTR_U8*)(dest)) = (FMSTR_U8)(src)), (((FMSTR_BPTR)(dest))+1) )

#define FMSTR_SkipInBuffer(dest, size) \
    ( ((FMSTR_BPTR)(dest)) + (size) )

FMSTR_BPTR FMSTR_AddressFromBuffer(FMSTR_ADDR* paddr, FMSTR_BPTR src);
FMSTR_BPTR FMSTR_AddressToBuffer(FMSTR_BPTR dest, FMSTR_ADDR addr);
FMSTR_BPTR FMSTR_SizeFromBuffer(FMSTR_SIZE* psize, FMSTR_BPTR src);
FMSTR_BPTR FMSTR_SizeToBuffer(FMSTR_BPTR dest, FMSTR_SIZE size);
FMSTR_BPTR FMSTR_IndexFromBuffer(FMSTR_INDEX* pindex, FMSTR_BPTR src);
FMSTR_BPTR FMSTR_IndexToBuffer(FMSTR_BPTR dest, FMSTR_INDEX index);
FMSTR_BPTR FMSTR_ULebToBuffer(FMSTR_BPTR dest, FMSTR_U32 num);
FMSTR_BPTR FMSTR_ULebFromBuffer(FMSTR_U32* pnum, FMSTR_BPTR src);
FMSTR_SIZE FMSTR_GetAlignmentCorrection(FMSTR_ADDR addr, FMSTR_SIZE size);

#define FMSTR_GetS8(addr)  ( *(FMSTR_S8*)(addr) )
#define FMSTR_GetU8(addr)  ( *(FMSTR_U8*)(addr) )
#define FMSTR_GetS16(addr) ( *(FMSTR_S16*)(addr) )
#define FMSTR_GetU16(addr) ( *(FMSTR_U16*)(addr) )
#define FMSTR_GetS32(addr) ( *(FMSTR_S32*)(addr) )
#define FMSTR_GetU32(addr) ( *(FMSTR_U32*)(addr) )
#define FMSTR_GetS64(addr) ( *(FMSTR_S64*)(addr) )
#define FMSTR_GetU64(addr) ( *(FMSTR_U64*)(addr) )

#if FMSTR_REC_FLOAT_TRIG
#define FMSTR_GetFloat(addr) ( *(FMSTR_FLOAT*)(addr) )
#define FMSTR_GetDouble(addr) ( *(FMSTR_DOUBLE*)(addr) )
#endif

/******************************************************************************
* SHA calculation
******************************************************************************/

typedef struct
{
    FMSTR_U8 data[64];
    FMSTR_U32 datalen;
    FMSTR_U64 bitlen;
    FMSTR_U32 state[5];
    FMSTR_U32 k[4];
} FMSTR_SHA1_CTX;

void FMSTR_Sha1Init(FMSTR_SHA1_CTX *ctx);
void FMSTR_Sha1Update(FMSTR_SHA1_CTX *ctx, const FMSTR_U8* data, size_t len);
void FMSTR_Sha1Final(FMSTR_SHA1_CTX *ctx, FMSTR_U8* hash);

#ifdef __cplusplus
  }
#endif

/****************************************************************************************
* Potentially unused variable declaration
*****************************************************************************************/

#if defined(_lint) || defined(__IAR_SYSTEMS_ICC__) || defined(__ARMCC_VERSION)
#define FMSTR_UNUSED(sym) /*lint -esym(715,sym) -esym(818,sym) -esym(529,sym) -e{960} */
#elif defined(__GNUC__)
#define FMSTR_UNUSED(x) (void)(x)
#else
#define FMSTR_UNUSED(sym) ((sym),0)
#endif

/****************************************************************************************
* Potentially unused variable declaration
*****************************************************************************************/

/** Get count of elements in a static array. */
#define FMSTR_COUNTOF(arr) (sizeof(arr)/sizeof(arr[0]))

/****************************************************************************************
* Configuration checking
*****************************************************************************************/

/* obsolete transport and driver macros */
#if defined(FMSTR_USE_PDBDM)
    #warning Obsolete configuration option detected. Define FMSTR_TRANSPORT as FMSTR_PDBDM instead.
#endif

#if (FMSTR_LONG_INTR && (FMSTR_SHORT_INTR || FMSTR_POLL_DRIVEN)) || \
    (FMSTR_SHORT_INTR && (FMSTR_LONG_INTR || FMSTR_POLL_DRIVEN)) || \
    (FMSTR_POLL_DRIVEN && (FMSTR_LONG_INTR || FMSTR_SHORT_INTR)) || \
    !(FMSTR_POLL_DRIVEN || FMSTR_LONG_INTR || FMSTR_SHORT_INTR)
    /* mismatch in interrupt modes, only one can be selected */
    #error You have to enable exctly one of FMSTR_LONG_INTR or FMSTR_SHORT_INTR or FMSTR_POLL_DRIVEN
#endif

/* check scope settings */
#if FMSTR_USE_SCOPE
    #if FMSTR_MAX_SCOPE_VARS > 32 || FMSTR_MAX_SCOPE_VARS < 2
        #error Error in FMSTR_MAX_SCOPE_VARS value. Use a value in range 2..32
    #endif
#endif

/* check recorder settings */
#if (FMSTR_USE_RECORDER)
    #if !FMSTR_USE_READMEM
        #error Recorder needs the FMSTR_USE_READMEM feature
    #endif

    #ifdef FMSTR_REC_OWNBUFF
    #warning The FMSTR_REC_OWNBUFF is obsolete. Use FMSTR_REC_BUFF_SIZE for Recorder 0 and/or FMSTR_RecorderCreate for other Recorders.
    #endif
#endif

#if FMSTR_USE_TSA
    #if !FMSTR_USE_READMEM
        #error TSA needs the FMSTR_USE_READMEM feature
    #endif
#endif

#if FMSTR_USE_PIPES

    #ifdef FMSTR_PIPES_EXPERIMENTAL
    #warning The "pipes" feature is now in experimental code phase. Not yet tested on this platform.
    #endif

    /* must enable printf for vararg printf */
    #if !(FMSTR_USE_PIPE_PRINTF) && (FMSTR_USE_PIPE_PRINTF_VARG)
    #error You must enable pipe printf for vararg printf (see FMSTR_USE_PIPE_PRINTF)
    #endif

    /* printf buffer should accept one integer printed */
    #if FMSTR_PIPES_PRINTF_BUFF_SIZE < 8
    #error Pipe printf buffer should be at least 8 (see FMSTR_PIPES_PRINTF_BUFF_SIZE)
    #endif
    #if FMSTR_PIPES_PRINTF_BUFF_SIZE > 255
    #error Pipe printf buffer should not exceed 255 (see FMSTR_PIPES_PRINTF_BUFF_SIZE)
    #endif

    /* obsolete configuration options */
	#ifdef FMSTR_MAX_PIPES_COUNT
	#warning The FMSTR_MAX_PIPES_COUNT option is obsolete. Specify number of used pipes as FMSTR_USE_PIPES
    #endif

#endif

#if FMSTR_DEBUG_TX
/* When communication debugging mode is requested, this global variable is used to
   turn the debugging off once a valid connection is detected */
extern FMSTR_BOOL fmstr_doDebugTx;
#endif

/* maximum data item width in bytes when performing optimized/aligned memory copy */
#ifndef FMSTR_MEMCPY_MAX_SIZE
	/* 32-bit transfers maximum by default */
	#define FMSTR_MEMCPY_MAX_SIZE 4
#endif

#endif /* __FREEMASTER_PRIVATE_H */

