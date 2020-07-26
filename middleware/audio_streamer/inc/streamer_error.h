/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __AF_ERROR_H_
#define __AF_ERROR_H_

/*!
 * @file    streamer_error.h
 * @brief   Contains error and logging definitions for streamer.
 */

/*
 * CONSTANTS, DEFINES AND MACROS
 */

/*! Module IDs for error logging. */
/* Comments contain values defined in other modules */
#define LOGMDL_GENERAL        0x00000008  /*  bit  3 */
#define LOGMDL_STREAMER       0x00004000  /*  bit 14 */

/*! Error codes for the Af system */
#define ERRCODE_AF_OK                          ERRCODE_NO_ERROR
#define ERRCODE_AF_INVALID_PARAM               ERRCODE_INVALID_ARGUMENT
#define ERRCODE_AF_OUT_OF_MEM                  ERRCODE_OUT_OF_MEMORY
#define ERRCODE_AF_FUNCTION_NOT_SUPPORTED      ERRCODE_NOT_SUPPORTED
#define ERRCODE_AF_UNKNOWN                     ERRCODE_UNKNOWN
#define ERRCODE_AF_INTERNAL                    ERRCODE_INTERNAL

/* Indicates start of Streamer Error codes. */
#define ERRCODE_AF_STREAMER                        0x1400
#define ERRCODE_AF_STREAMER_NOT_SEEKABLE           0x1414
#define ERRCODE_AF_STREAMER_NO_AUDIO_STREAM        0x1415
#define ERRCODE_AF_STREAMER_SINK_FMT_NOT_SUPPORT   0x1416
#define ERRCODE_AF_STREAMER_CCID_INIT              0x1421
#define ERRCODE_AF_STREAMER_CCID_FMT_NOT_SUPPORT   0x1422
#define ERRCODE_AF_STREAMER_CCID_DEC_FRAME         0x1423

#endif

