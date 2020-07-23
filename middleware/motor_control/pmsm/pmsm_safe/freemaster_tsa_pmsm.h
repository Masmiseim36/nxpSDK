/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FREEMASTER_TSA_PMSM_H_
#define _FREEMASTER_TSA_PMSM_H_

MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */

#include <stdint.h>
#include "freemaster.h"

MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* FreeMASTER console messages - indexes. */
#define FMSTR_MSG_INIT_DONE     (1U+32U)   /* Message  1 */
#define FMSTR_MSG_INRUSH_ON     (2U+32U)   /* Message  2 */
#define FMSTR_MSG_SM_TRF_FREE   (3U+32U)   /* Message  3 */
#define FMSTR_MSG_SM_MS_HS      (4U+32U)   /* Message  4 */
#define FMSTR_MSG_SM_LS_MS      (5U+32U)   /* Message  5 */
#define FMSTR_MSG_SM_A_LS       (6U+32U)   /* Message  6 */
#define FMSTR_MSG_SM_C_R        (7U+32U)   /* Message  7 */
#define FMSTR_MSG_SM_S_C        (8U+32U)   /* Message  8 */
#define FMSTR_MSG_SM_F_S        (9U+32U)   /* Message  9 */
#define FMSTR_MSG_SM_FLT       (10U+32U)   /* Message 10 */
#define FMSTR_MSG_SM_S         (11U+32U)   /* Message 11 */
#define FMSTR_MSG_SM_R_A       (12U+32U)   /* Message 12 */
#define FMSTR_MSG_TRUE         (13U+32U)   /* Message 13 */
#define FMSTR_MSG_FALSE        (14U+32U)   /* Message 14 */
#define FMSTR_MSG13            (15U+32U)   /* Message 15 */
#define FMSTR_MSG14            (16U+32U)   /* Message 16 */

/* FreeMASTER console messages - content.
   Following color tags can be used:
   class='r' - red
   class='g' - green
   class='b' - blue
   class='y' - yellow */
#define FMSTR_DBG_MESSAGES \
    "\"INIT COMPLETE(PMSM_RD_V0.2.0)\","\
    "\"IN-RUSH RELAY ON\","\
    "\"<b class='b'>FREE\","\
    "\"<b class='g'>HI_SPD\","\
    "\"<b class='g'>MI_SPD\","\
    "\"<b class='g'>LO_SPD\","\
    "\"<b class='y'>READY\","\
    "\"<b class='y'>CALIB\","\
    "\"<b>STOP\","\
    "\"<b class='r'>FAULT\","\
    "\"<b>STOP\","\
    "\"<b class='g'>ALIGN\","\
    "\"<i>TRUE\","\
    "\"<i>FALSE\","\
    "\"FMSTR_MSG15\","\
    "\"FMSTR_MSG16\""




/*
 * Specify the TSA table function and data location as follows:
 * - Section .tsacode: TSA static functions.
 * - Section .tsadata: Full variable names.
 * - Section .tsaentry: The TSA table entry (pointer to variable, its name, etc.).
 */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
#if defined(__IAR_SYSTEMS_ICC__) /* IAR */
    #define FMSTR_TSA_PLACE_IN_TSACODE   @ ".tsacode"
    #define FMSTR_TSA_PLACE_IN_TSADATA   @ ".tsadata"
    #define FMSTR_TSA_PLACE_IN_TSAENTRY  @ ".tsaentry"
    #define FMSTR_TSA_DATA_PREFIX        __root
    #define FMSTR_TSA_DATA_POSTFIX
#elif (defined(__GNUC__) && (__ARMCC_VERSION >= 6010050)) /* KEIL */
    #define FMSTR_TSA_PLACE_IN_TSACODE   __attribute__((section(".tsacode")))
    #define FMSTR_TSA_PLACE_IN_TSADATA   __attribute__((section(".tsadata")))
    #define FMSTR_TSA_PLACE_IN_TSAENTRY  __attribute__((section(".tsaentry")))
    #define FMSTR_TSA_DATA_PREFIX
    #define FMSTR_TSA_DATA_POSTFIX       __attribute__((used))
#elif defined(__GNUC__) /* MCUXpresso */
    #define FMSTR_TSA_PLACE_IN_TSACODE   __attribute__((section(".tsacode")))
    #define FMSTR_TSA_PLACE_IN_TSADATA   __attribute__((section(".tsadata")))
    #define FMSTR_TSA_PLACE_IN_TSAENTRY  __attribute__((section(".tsaentry")))
    #define FMSTR_TSA_DATA_PREFIX
    #define FMSTR_TSA_DATA_POSTFIX       __attribute__((used))
#endif /* defined(__GNUC__) */
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/* The TSA table and function names. */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
#define FMSTR_TSA_VAR_NAME(id)             TSA_Name##id
#define FMSTR_TSA_VAR_ENTRY(id)            TSA_Entry##id
#define FMSTR_TSA_NAME_CONTENT(name, type) #name"\0."type
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*
 * The TSA table and function definitions.
 * - id: TSA table identificator.
 */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
#define FMSTR_TSA_FUNC_PROTO_FS(id) const FMSTR_TSA_ENTRY* FMSTR_TSA_FUNC(id) (FMSTR_SIZE* pui16TableSize)
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*
 * TSA read-only variable entry.
 * - id: Unique variable identificator (only alphanumeric symbols allowed).
 * - name: The variable name.
 * - type: The variable data type macro.
 */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
#define FMSTR_TSA_RO_VAR_FS(id,name,type) \
        FMSTR_TSA_DATA_PREFIX static const char FMSTR_TSA_VAR_NAME(id)[] FMSTR_TSA_PLACE_IN_TSADATA FMSTR_TSA_DATA_POSTFIX = FMSTR_TSA_NAME_CONTENT(name, type);\
        FMSTR_TSA_DATA_PREFIX static const FMSTR_TSA_ENTRY FMSTR_TSA_VAR_ENTRY(id) FMSTR_TSA_PLACE_IN_TSAENTRY FMSTR_TSA_DATA_POSTFIX =\
	{\
            {FMSTR_TSA_VAR_NAME(id)}, \
	    {&FMSTR_TSA_VAR_NAME(id)[sizeof(#name) + 1UL]}, \
	    FMSTR_TSATBL_VOIDPTR_ENTRY(&(name)), \
	    FMSTR_TSATBL_VOIDPTR_ENTRY(FMSTR_TSA_INFO1(name, FMSTR_TSA_INFO_RO_VAR))\
	};\
        ui16TabSize += sizeof(FMSTR_TSA_ENTRY);
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */
	
/*
 * TSA read-write variable entry macro.
 *  - id: Unique variable identificator (only alphanumeric symbols allowed).
 *  - name: The variable name.
 *  - type: The variable data type macro.
 */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
#define FMSTR_TSA_RW_VAR_FS(id,name,type) \
        FMSTR_TSA_DATA_PREFIX static const char FMSTR_TSA_VAR_NAME(id)[] FMSTR_TSA_PLACE_IN_TSADATA FMSTR_TSA_DATA_POSTFIX = FMSTR_TSA_NAME_CONTENT(name, type);\
        FMSTR_TSA_DATA_PREFIX static const FMSTR_TSA_ENTRY FMSTR_TSA_VAR_ENTRY(id) FMSTR_TSA_PLACE_IN_TSAENTRY FMSTR_TSA_DATA_POSTFIX =\
	{\
            {FMSTR_TSA_VAR_NAME(id)}, \
	    {&FMSTR_TSA_VAR_NAME(id)[sizeof(#name) + 1UL]}, \
	    FMSTR_TSATBL_VOIDPTR_ENTRY(&(name)), \
	    FMSTR_TSATBL_VOIDPTR_ENTRY(FMSTR_TSA_INFO1(name, FMSTR_TSA_INFO_RW_VAR))\
	};\
        ui16TabSize += sizeof(FMSTR_TSA_ENTRY);
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*
 * Start the TSA table (define function and static variable) macro.
 * - id: TSA table identificator.
 */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
#define FMSTR_TSA_TABLE_BEGIN_FS(id)\
    FMSTR_TSA_FUNC_PROTO_FS(id) FMSTR_TSA_PLACE_IN_TSACODE;\
    FMSTR_TSA_FUNC_PROTO_FS(id)\
    { \
        static const FMSTR_TSA_ENTRY TSA_Start##id FMSTR_TSA_PLACE_IN_TSAENTRY = {{0},{0},{0},{0}};\
        uint32_t ui32StartAddr = (uint32_t)(&TSA_Start##id) + sizeof(FMSTR_TSA_ENTRY); \
        uint16_t ui16TabSize = 0U;
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*
 * End the TSA table macro.
 * - id - TSA table identificator.
 */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
#define FMSTR_TSA_TABLE_END_FS(id) \
        if(pui16TableSize > 0U)\
        {\
            *pui16TableSize = ui16TabSize;\
        }\
        return (FMSTR_TSA_ENTRY*)(ui32StartAddr);}
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief   The FreeMASTER pipe-based debug console initialization.
 *
 * @param   void
 *
 * @return  none
 */
void FMSTR_PipeMessage_Init(void);

/*!
 * @brief   The FreeMASTER pipe-based debug console.
 *
 * @param   ui8MsgIdx - Message index.
 *
 * @return  none
 */
void FMSTR_PipeMessage(uint8_t ui8MsgIdx);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FREEMASTER_TSA_PMSM_H_ */
