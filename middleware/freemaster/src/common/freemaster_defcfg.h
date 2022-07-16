/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2021 NXP
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
 * FreeMASTER Communication Driver - default configuration options
 */

#ifndef __FREEMASTER_DEFCFG_H
#define __FREEMASTER_DEFCFG_H

/* Read user configuration file. This file should be created by user in the
 * project directory and it should be accessible on "include path".
 *
 * Refer to freemaster_cfg.h file in existing example applications
 * for SERIAL, CAN, PD_BDM and other transports available for your target
 * microcontroller platform.
 */
#include "freemaster_cfg.h"

/* Define global version macro */
#define FMSTR_VERSION     0x00030004
#define FMSTR_VERSION_STR "3.0.4"

/******************************************************************************
 * Configuration check
 ******************************************************************************/

/* polling mode as default when nothing selected */
#if !defined(FMSTR_POLL_DRIVEN) && !defined(FMSTR_LONG_INTR) && !defined(FMSTR_SHORT_INTR)
#define FMSTR_LONG_INTR   0
#define FMSTR_SHORT_INTR  0
#define FMSTR_POLL_DRIVEN 1
#endif

/* otherwise, "undefined" means false for all three options */
#ifndef FMSTR_POLL_DRIVEN
#define FMSTR_POLL_DRIVEN 0
#endif
#ifndef FMSTR_LONG_INTR
#define FMSTR_LONG_INTR 0
#endif
#ifndef FMSTR_SHORT_INTR
#define FMSTR_SHORT_INTR 0
#endif

/* FreeMASTER driver is enabled by default */
#ifndef FMSTR_DISABLE
#define FMSTR_DISABLE 0
#endif

/* Protocol session */
#ifndef FMSTR_SESSION_COUNT
#define FMSTR_SESSION_COUNT 1
#endif

#if FMSTR_SESSION_COUNT == 0
#error Count of sessions (FMSTR_SESSION_COUNT) cannot be zero!
#endif

/* transport MUST be defined in configuration */
#if FMSTR_DISABLE == 0 && !defined(FMSTR_TRANSPORT)
#error No FreeMASTER communication transport interface is enabled. Please choose the interface (FMSTR_TRANSPORT) or set FMSTR_DISABLE option to 1.
#endif

#ifndef FMSTR_APPLICATION_STR
#define FMSTR_APPLICATION_STR "Not defined"
#endif

#ifndef FMSTR_DESCRIPTION_STR
#define FMSTR_DESCRIPTION_STR "Not defined"
#endif

#ifndef FMSTR_BUILDTIME_STR
#define FMSTR_BUILDTIME_STR "Build date not specified" /* e.g: __DATE__ " " __TIME__ */
#endif

/* Remote access to device enabled by default */
#ifndef FMSTR_ENABLE_REMOTE_ACCESS
#define FMSTR_ENABLE_REMOTE_ACCESS 1
#endif

/* Passwords may be specified as SHA1 hash instead of plaintext. But plaintext is default. */
#ifndef FMSTR_USE_HASHED_PASSWORDS
#define FMSTR_USE_HASHED_PASSWORDS 0
#endif

/* Build CFG_F1 configuration flags: */

/* CFG_F1 0x01: Big endian */
#define FMSTR_CFG_F1_BIG_ENDIAN ((FMSTR_PLATFORM_BIG_ENDIAN != 0U ? 1U : 0U) << 0)

/* CFG_F1 0x02: Remote access enabled */
#define FMSTR_CFG_F1_ENABLE_REMOTE_ACCESS ((FMSTR_ENABLE_REMOTE_ACCESS != 0U ? 1U : 0U) << 1)

/* CFG_F1 0x30: access level which needs passwords */
#if (defined(FMSTR_RESTRICTED_ACCESS_R_PASSWORD))
#define FMSTR_CFG_F1_RESTRICTED_ACCESS ((FMSTR_RESTRICTED_ACCESS_R) << 4)
#elif defined(FMSTR_RESTRICTED_ACCESS_RW_PASSWORD)
#define FMSTR_CFG_F1_RESTRICTED_ACCESS ((FMSTR_RESTRICTED_ACCESS_RW) << 4)
#elif defined(FMSTR_RESTRICTED_ACCESS_RWF_PASSWORD)
#define FMSTR_CFG_F1_RESTRICTED_ACCESS ((FMSTR_RESTRICTED_ACCESS_RWF) << 4)
#else
#define FMSTR_CFG_F1_RESTRICTED_ACCESS 0U
#endif

/* CFG_F1 value */
#define FMSTR_CFG_F1 (FMSTR_CFG_F1_BIG_ENDIAN | FMSTR_CFG_F1_ENABLE_REMOTE_ACCESS | FMSTR_CFG_F1_RESTRICTED_ACCESS)

/* read memory commands are ENABLED by default */
#ifndef FMSTR_USE_READMEM
#define FMSTR_USE_READMEM 1
#endif
#ifndef FMSTR_USE_WRITEMEM
#define FMSTR_USE_WRITEMEM 1
#endif
#ifndef FMSTR_USE_WRITEMEMMASK
#define FMSTR_USE_WRITEMEMMASK 1
#endif

/* default scope settings */
#ifndef FMSTR_USE_SCOPE
#define FMSTR_USE_SCOPE 0
#endif

#ifndef FMSTR_MAX_SCOPE_VARS
#define FMSTR_MAX_SCOPE_VARS 8
#endif
/* default recorder settings */
#ifndef FMSTR_USE_RECORDER
#define FMSTR_USE_RECORDER 0
#endif

#ifndef FMSTR_REC_FARBUFF
#define FMSTR_REC_FARBUFF 0
#endif

/* Enable Floating point support in Recorder triggering */
#ifndef FMSTR_REC_FLOAT_TRIG
#define FMSTR_REC_FLOAT_TRIG 0
#endif

/* Debug-TX mode is a special mode used to test or debug the data transmitter. Our driver
   will be sending test frames periodically until a first valid command is received from the
   PC Host. You can hook a logic analyzer to transmission pins to determine port and baudrate.
   Or you can connect the FreeMASTER tool and run the connection wizard to listen for the
   dummy frames. */
#ifndef FMSTR_DEBUG_TX
#define FMSTR_DEBUG_TX 0
#endif

#if FMSTR_USE_RECORDER > 255
#error Number of enabled recorders MUST be smaller than 255.
#endif

#if FMSTR_USE_SCOPE > 255
#error Number of enabled oscilloscopes MUST be smaller than 255.
#endif

/* check recorder settings */
#if FMSTR_USE_RECORDER > 0

/* 0 means recorder time base is "unknown" */
#ifndef FMSTR_REC_TIMEBASE
#define FMSTR_REC_TIMEBASE 0
#endif

/* 0 means that default recorder will not be created, user needs to call RecoderCreate */
#ifndef FMSTR_REC_BUFF_SIZE
#define FMSTR_REC_BUFF_SIZE 0
#endif

#endif
/* default app.cmds settings */
#ifndef FMSTR_USE_APPCMD
#define FMSTR_USE_APPCMD 0
#endif

#ifndef FMSTR_APPCMD_BUFF_SIZE
#define FMSTR_APPCMD_BUFF_SIZE 16
#endif

#ifndef FMSTR_MAX_APPCMD_CALLS
#define FMSTR_MAX_APPCMD_CALLS 0
#endif

/* TSA configuration check */
#ifndef FMSTR_USE_TSA
#define FMSTR_USE_TSA 0
#endif

#if FMSTR_USE_TSA > 0 && FMSTR_DISABLE > 0
#undef FMSTR_USE_TSA
#define FMSTR_USE_TSA 0
#endif

#ifndef FMSTR_USE_TSA_SAFETY
#define FMSTR_USE_TSA_SAFETY 0
#endif

/* TSA table allocation modifier */
#ifndef FMSTR_USE_TSA_INROM
#define FMSTR_USE_TSA_INROM 0
#endif

#ifndef FMSTR_TSA_CDECL
#if FMSTR_USE_TSA_INROM > 0
#define FMSTR_TSA_CDECL const
#else
#define FMSTR_TSA_CDECL
#endif
#endif

#ifndef FMSTR_USE_TSA_DYNAMIC
#define FMSTR_USE_TSA_DYNAMIC 0
#endif

/* use transport "pipe" functionality */
#ifndef FMSTR_USE_PIPES
#define FMSTR_USE_PIPES 0
#endif

/* "pipe" putstring formatting (enabled by default) */
#ifndef FMSTR_USE_PIPE_PRINTF
#define FMSTR_USE_PIPE_PRINTF 1
#endif

/* "pipe" variable-argument printf (enabled by default) */
#ifndef FMSTR_USE_PIPE_PRINTF_VARG
#define FMSTR_USE_PIPE_PRINTF_VARG FMSTR_USE_PIPE_PRINTF
#endif

#if FMSTR_USE_PIPES > 127
#error Number of enabled pipes MUST be smaller than 127.
#endif

#if FMSTR_USE_PIPES > 0
/* pipe printf buffer */
#ifndef FMSTR_PIPES_PRINTF_BUFF_SIZE
#define FMSTR_PIPES_PRINTF_BUFF_SIZE 48U
#endif
#endif

/* use default buffer size */
#if !defined(FMSTR_COMM_BUFFER_SIZE) || (FMSTR_COMM_BUFFER_SIZE < 32)
#if defined(FMSTR_COMM_BUFFER_SIZE)
#if FMSTR_COMM_BUFFER_SIZE > 0
#warning FMSTR_COMM_BUFFER_SIZE set too small
#endif

#undef FMSTR_COMM_BUFFER_SIZE
#elif !(FMSTR_DISABLE)
#warning FMSTR_COMM_BUFFER_SIZE is not set in configuration, the default value is used.
#endif

#define FMSTR_COMM_BUFFER_SIZE 240U
#endif

/* PDBDM buffer is defined in the driver by default */
#ifndef FMSTR_PDBDM_USER_BUFFER
#define FMSTR_PDBDM_USER_BUFFER 0
#endif

/* Help macro to create ID string for preprocessor to recognize the drivers to enable or not */
#define FMSTR_MK_IDSTR1(x) x##_ID
#define FMSTR_MK_IDSTR(x)  FMSTR_MK_IDSTR1(x)

/* Default driver debugging print level (0=none, 1=errors, 2=normal, 3=verbose) */
#ifndef FMSTR_DEBUG_LEVEL
#ifdef FMSTR_DEBUG_PRINTF
#define FMSTR_DEBUG_LEVEL 3
#else
#define FMSTR_DEBUG_LEVEL 0
#endif
#endif

#if FMSTR_DEBUG_LEVEL > 0 && !defined(FMSTR_DEBUG_PRINTF)
#undef FMSTR_DEBUG_LEVEL
#define FMSTR_DEBUG_LEVEL 0
#endif

#endif /* __FREEMASTER_DEF_CFG_H */
