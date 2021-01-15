/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __METADATA_ERROR_H_
#define __METADATA_ERROR_H_

/*!
 * @file    metadata_error.h
 * @brief   Contains error and logging definitions for Metadata.
 */

/*
 * CONSTANTS, DEFINES AND MACROS
 */

/*! Module IDs for error logging. */
#define LOGMDL_METADATA   0x00000020  /*    32 */

/*! Error ranges for error codes for these modules. */
/*
 *  DEVICEMGR: 0x0401 to 0x04FF
 *  METADATA:  0x0501 to 0x05FF
 *  PSM:       0x0601 to 0x06FF

 *  BROWSE:    0x0701 to 0x07FF
 *  MFIHOST:   0x0801 to 0x08FF
 *  MEDIADEV:  0x0901 to 0x09FF
 *  PLAYER:    0x1001 to 0x10FF
 *  PLAYQUEUE: 0x1101 to 0x11FF
 *  RECORDER:  0x1201 to 0x12FF
 *  SAL:       0x1301 to 0x13FF
 *  STREAMER:  0x1401 to 0x14FF
 *  TUNER:     0x1501 to 0x15FF
*/

/*! Error codes for the Metadata Feature */
#define ERR_META_GENERAL                        0x0501
#define ERR_META_NOT_INDEXABLE                  0x0502
#define ERR_META_INDEX_RUNNING                  0x0503
#define ERR_META_DATABASE                       0x0504
#define ERR_META_MAX_DEVICES                    0x0505
#define ERRCODE_METADATA_FILE_IO_ERROR          0x0506
#define ERRCODE_METADATA_NO_META_INFO           0x0507
#define ERRCODE_METADATA_PARSER_ERROR           0x0508
#define ERRCODE_METADATA_OUT_OF_MEM             0x0509
#define ERRCODE_METADATA_FILETYPE_FAILED        0x050A

#endif

