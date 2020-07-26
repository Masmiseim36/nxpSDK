/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _MEMTYPES_H_
#define _MEMTYPES_H_

/*!
 * @file    memtypes.h
 * @brief   Contains mem types definitions for all modules in impresario
 */


/*
 * CONSTANTS, DEFINES AND MACROS
 */
/*
 * Dynamic memory type in streamer
 * OSA_AF_MEMTYPE_BASE defined in osa_memory.h. = 57344(MQX), 24576(Linux)
 */
#define STREAMER_MEM_TYPE_CORE          (OSA_AF_MEMTYPE_BASE + 1)
//#define STREAMER_MEM_TYPE_AACDEC        (OSA_AF_MEMTYPE_BASE + 2)
//#define STREAMER_MEM_TYPE_ID3           (OSA_AF_MEMTYPE_BASE + 3)
#define STREAMER_MEM_TYPE_MP3DEC        (OSA_AF_MEMTYPE_BASE + 4)
#define STREAMER_MEM_TYPE_WAVDEC        (OSA_AF_MEMTYPE_BASE + 5)
//#define STREAMER_MEM_TYPE_WMADEC        (OSA_AF_MEMTYPE_BASE + 6)
//#define STREAMER_MEM_TYPE_SINK_ALSA     (OSA_AF_MEMTYPE_BASE + 7)
//#define STREAMER_MEM_TYPE_SINK_ESAI     (OSA_AF_MEMTYPE_BASE + 8)
//#define STREAMER_MEM_TYPE_SINK_PCMMGR   (OSA_AF_MEMTYPE_BASE + 9)
//#define STREAMER_MEM_TYPE_SINK_SSI      (OSA_AF_MEMTYPE_BASE + 10)
//#define STREAMER_MEM_TYPE_SRC_ALSA      (OSA_AF_MEMTYPE_BASE + 11)
#define STREAMER_MEM_TYPE_AUDIO_SRC     (OSA_AF_MEMTYPE_BASE + 12)
#define STREAMER_MEM_TYPE_FILE_SRC      (OSA_AF_MEMTYPE_BASE + 13)
#define STREAMER_MEM_TYPE_MEM_SRC       (OSA_AF_MEMTYPE_BASE + 14)
//#define STREAMER_MEM_TYPE_ASFPARSER     (OSA_AF_MEMTYPE_BASE + 15)
//#define STREAMER_MEM_TYPE_MP4PARSER     (OSA_AF_MEMTYPE_BASE + 16)
//#define STREAMER_MEM_TYPE_FLACDEC       (OSA_AF_MEMTYPE_BASE + 17)
#define STREAMER_MEM_TYPE_CCIDEC        (OSA_AF_MEMTYPE_BASE + 18)
//#define STREAMER_MEM_TYPE_AC3DEC        (OSA_AF_MEMTYPE_BASE + 19)
#define STREAMER_MEM_TYPE_TEST          (OSA_AF_MEMTYPE_BASE + 20)


#define STREAMER_MEM_TYPE_END           (STREAMER_MEM_TYPE_TEST)

/*
 * Dynamic memory type in Af
 * OSA_AF_MEMTYPE_BASE defined in osa_memory.h. = 57344(MQX), 24576(Linux)
 * OSA_AF_MEMTYPE_BASE ~ (OSA_AF_MEMTYPE_BASE + 99) was reserved
 * for streamer.
 */
/*
#define AF_MEM_TYPE_BROWSER        (OSA_AF_MEMTYPE_BASE + 100)
#define AF_MEM_TYPE_PLAYER         (OSA_AF_MEMTYPE_BASE + 101)
#define AF_MEM_TYPE_FS_PLAYER      (OSA_AF_MEMTYPE_BASE + 102)
#define AF_MEM_TYPE_MFI_PLAYER     (OSA_AF_MEMTYPE_BASE + 103)
#define AF_MEM_TYPE_CDDA_PLAYER    (OSA_AF_MEMTYPE_BASE + 104)
#define AF_MEM_TYPE_A2DP_PLAYER    (OSA_AF_MEMTYPE_BASE + 105)

#define AF_MEM_TYPE_PLAYQUEUE      (OSA_AF_MEMTYPE_BASE + 107)
#define AF_MEM_TYPE_MEDIADEV       (OSA_AF_MEMTYPE_BASE + 108)
#define AF_MEM_TYPE_TUNER          (OSA_AF_MEMTYPE_BASE + 109)
*/

#endif

