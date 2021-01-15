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
 * @file    af_error.h
 * @brief   Contains error and logging definitions for Af.
 */

/*
 * CONSTANTS, DEFINES AND MACROS
 */

/*! Module IDs for error logging. */
/* Comments contain values defined in other modules */
#define LOGMDL_GENERAL        0x00000008  /*  bit  3 */
/*      LOGMDL_DEVICEMGR      0x00000010      bit  4 */
/*      LOGMDL_METADATA       0x00000020      bit  5 */
/*      LOGMDL_PSM            0x00000040      bit  6 */
#define LOGMDL_BROWSE         0x00000080  /*  bit  7 */
#define LOGMDL_MFIHOST        0x00000100  /*  bit  8 */
#define LOGMDL_MEDIADEV       0x00000200  /*  bit  9 */
#define LOGMDL_PLAYER         0x00000400  /*  bit 10 */
#define LOGMDL_PLAYQUEUE      0x00000800  /*  bit 11 */
#define LOGMDL_RECORDER       0x00001000  /*  bit 12 */
#define LOGMDL_SAL            0x00002000  /*  bit 13 */
#define LOGMDL_STREAMER       0x00004000  /*  bit 14 */
#define LOGMDL_TUNER          0x00008000  /*  bit 15 */
/*      LOGMDL_INPUTMGR       0x00010000      bit 16 */
#define LOGMDL_PLAYLIST       0x00020000  /*  bit 17 */
/*      LOGMDL_BTAL           0x00040000      bit 18 */
/*      LOGMDL_BTAL_BACKEND   0x00080000      bit 19 */
/*      LOGMDL_BTD            0x00100000      bit 20 */
#define LOGMDL_PLAYERFS       0x00200000  /*  bit 21 */
#define LOGMDL_PLAYERCDDA     0x00400000  /*  bit 22 */
#define LOGMDL_PLAYERMFI      0x00800000  /*  bit 23 */
#define LOGMDL_PLAYERA2DP     0x01000000  /*  bit 24 */
#define LOGMDL_PLAYERAOAP     0x02000000  /*  bit 25 */
#define LOGMDL_PLAYERAIRPLAY  0x04000000  /*  bit 26 */
#define LOGMDL_PLAYERANALOG   0x08000000  /*  bit 27 */

/*! Error ranges for error codes for these modules. */
/* Based on module bit number */
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
 *  INPUTMGR:  0x1601 to 0x16FF
 *  PLAYLISTS: 0x1701 to 0x17FF
 */

/*! Error codes for the Af system */
#define ERRCODE_AF_OK                          ERRCODE_NO_ERROR
#define ERRCODE_AF_INVALID_PARAM               ERRCODE_INVALID_ARGUMENT
#define ERRCODE_AF_OUT_OF_MEM                  ERRCODE_OUT_OF_MEMORY
#define ERRCODE_AF_FUNCTION_NOT_SUPPORTED      ERRCODE_NOT_SUPPORTED
#define ERRCODE_AF_UNKNOWN                     ERRCODE_UNKNOWN
#define ERRCODE_AF_INTERNAL                    ERRCODE_INTERNAL

/* Indicates start of Browser Error codes. */
#define ERRCODE_AF_BROWSER                         0x0700
#define ERRCODE_AF_BROWSER_CREATE_FAIL             0x0701
#define ERRCODE_AF_BROWSER_SAVE_FAIL               0x0702
#define ERRCODE_AF_BROWSER_LOAD_FAIL               0x0703
#define ERRCODE_AF_BROWSER_NO_MEDIA_DEVICE         0x0704
#define ERRCODE_AF_BROWSER_INVALID_FILE            0x0705
#define ERRCODE_AF_BROWSER_INVALID_SN              0x0706
#define ERRCODE_AF_BROWSER_INVALID_TYPE            0x0707

/* Indicates start of MFIhost Error codes. */
#define ERRCODE_AF_MFIHOST                         0x0800

/* Indicates start of MediaDev Error codes. */
#define ERRCODE_AF_MEDIADEV                        0x0900
#define ERRCODE_AF_MEDIADEV_GENERAL                0x0901
#define ERRCODE_AF_MEDIADEV_ROOTPATH               0x0902
#define ERRCODE_AF_MEDIADEV_OPENDIR                0x0903
#define ERRCODE_AF_MEDIADEV_INVALID_DEVICE         0x0904
#define ERRCODE_AF_MEDIADEV_INTERNAL               0x0905
#define ERRCODE_AF_MEDIADEV_MAX_SIZE               0x0906
#define ERRCODE_AF_MEDIADEV_MAX_DEPTH              0x0907
#define ERRCODE_AF_MEDIADEV_INFO_NOT_READY         0x0908

/* Indicates start of Player Error codes. */
#define ERRCODE_AF_PLAYER                          0x1000
#define ERRCODE_AF_PLAYER_INTERNAL                 0x1001
#define ERRCODE_AF_PLAYER_NOT_SUP                  0x1002
#define ERRCODE_AF_PLAYER_BUSY                     0x1003
#define ERRCODE_AF_PLAYER_INVAL                    0x1004
#define ERRCODE_AF_PLAYER_NO_SPC                   0x1005
#define ERRCODE_AF_PLAYER_EXIST                    0x1006
#define ERRCODE_AF_PLAYER_LIMIT                    0x1007
#define ERRCODE_AF_PLAYER_EPERM                    0x1008
#define ERRCODE_AF_PLAYER_STREAM_ERR               0x1009
#define ERRCODE_AF_PLAYER_SAVE_FAIL                0x100A
#define ERRCODE_AF_PLAYER_LOAD_FAIL                0x100B
#define ERRCODE_AF_PLAYER_CLEAR_FAIL               0x100C

/* Indicates start of Playqueue Error codes. */
#define ERRCODE_AF_PLAYQUEUE                       0x1100
#define ERRCODE_AF_PLAYQUEUE_CREATE_FAIL           0x1101
#define ERRCODE_AF_PLAYQUEUE_EMPTY                 0x1102
#define ERRCODE_AF_PLAYQUEUE_FULL                  0x1103
#define ERRCODE_AF_PLAYQUEUE_OUT_OF_RANGE          0x1104
#define ERRCODE_AF_PLAYQUEUE_ADD_FAIL              0x1105
#define ERRCODE_AF_PLAYQUEUE_CLEAR_FAIL            0x1106
#define ERRCODE_AF_PLAYQUEUE_MOVE_FAIL             0x1107
#define ERRCODE_AF_PLAYQUEUE_SWAP_FAIL             0x1108
#define ERRCODE_AF_PLAYQUEUE_SAVE_FAIL             0x1109
#define ERRCODE_AF_PLAYQUEUE_LOAD_FAIL             0x110A
#define ERRCODE_AF_PLAYQUEUE_INVALID_FILE          0x110B
#define ERRCODE_AF_PLAYQUEUE_IN_USE                0x110C
#define ERRCODE_AF_PLAYQUEUE_NO_MORE_TRACK         0x110D
#define ERRCODE_AF_PLAYQUEUE_UNSUPPORT_SHUFFLE     0x110E
#define ERRCODE_AF_PLAYQUEUE_UNSUPPORT_REPEAT      0x110F
#define ERRCODE_AF_PLAYQUEUE_GROUP_NOT_FOUND       0x1110
#define ERRCODE_AF_PLAYQUEUE_KEY_NOT_FOUND         0x1111

/* Indicates start of Recorder Error codes. */
#define ERRCODE_AF_RECORDER                        0x1200

/* Indicates start of SAL Error codes. */
#define ERRCODE_AF_SAL                             0x1300

/* Indicates start of Streamer Error codes. */
#define ERRCODE_AF_STREAMER                        0x1400
#define ERRCODE_AF_STREAMER_MP3D_INIT              0x1401
#define ERRCODE_AF_STREAMER_MP3D_DEC_FRAME         0x1402
#define ERRCODE_AF_STREAMER_WAVD_INIT              0x1403
#define ERRCODE_AF_STREAMER_WAVD_DEC_FRAME         0x1404
#define ERRCODE_AF_STREAMER_ASF_GET_FILEPROP       0x1405
#define ERRCODE_AF_STREAMER_ASF_GET_STREAMPROP     0x1406
#define ERRCODE_AF_STREAMER_ASF_ERR                0x1407
#define ERRCODE_AF_STREAMER_ASF_UNSUP_STREAM_TYPE  0x1408
#define ERRCODE_AF_STREAMER_WMAD_DEC_FRAME         0x1409
#define ERRCODE_AF_STREAMER_MP4_CREATE_PARSER      0x140A
#define ERRCODE_AF_STREAMER_MP4_NEXT_SAMPLE        0x140B
#define ERRCODE_AF_STREAMER_MP4_INVALID_BUFF       0x140C
#define ERRCODE_AF_STREAMER_AACD_DEC_INIT          0x140D
#define ERRCODE_AF_STREAMER_AACD_DEC_FRAME         0x140E
#define ERRCODE_AF_STREAMER_MP3D_INVALID_SAMPLERATE 0x140F
#define ERRCODE_AF_STREAMER_MP3D_INVALID_BITRATE   0x1410
#define ERRCODE_AF_STREAMER_MP3D_FMT_NOT_SUPPORT   0x1411
#define ERRCODE_AF_STREAMER_WMAD_FMT_NOT_SUPPORT   0x1412
#define ERRCODE_AF_STREAMER_WMAD_PROFILE_NOT_SUPPORT 0x1413
#define ERRCODE_AF_STREAMER_NOT_SEEKABLE           0x1414
#define ERRCODE_AF_STREAMER_NO_AUDIO_STREAM        0x1415
#define ERRCODE_AF_STREAMER_SINK_FMT_NOT_SUPPORT   0x1416
#define ERRCODE_AF_STREAMER_MP4_FMT_NOT_SUPPORT    0x1417
#define ERRCODE_AF_STREAMER_FLACD_INIT             0x1418
#define ERRCODE_AF_STREAMER_FLACD_FMT_NOT_SUPPORT  0x1419
#define ERRCODE_AF_STREAMER_FLACD_DEC_FRAME        0x1420
#define ERRCODE_AF_STREAMER_CCID_INIT              0x1421
#define ERRCODE_AF_STREAMER_CCID_FMT_NOT_SUPPORT   0x1422
#define ERRCODE_AF_STREAMER_CCID_DEC_FRAME         0x1423

/* Indicates start of Tuner Error codes. */
#define ERRCODE_AF_TUNER                           0x1500
#define ERRCODE_AF_TUNER_INVALID_VALUE             0x1501
#define ERRCODE_AF_TUNER_COMMUNICATION             0x1502
#define ERRCODE_AF_TUNER_SEEK_TIMEOUT              0x1503
#define ERRCODE_AF_TUNER_OPEN                      0x1504
#define ERRCODE_AF_TUNER_CLOSE                     0x1505
#define ERRCODE_AF_TUNER_STREAMER_ERR              0x1506
#define ERRCODE_AF_TUNER_SAVE_FAIL                 0x1507
#define ERRCODE_AF_TUNER_LOAD_FAIL                 0x1508
#define ERRCODE_AF_TUNER_CLEAR_FAIL                0x1509

/* Indicates start of Input Manager Error codes. */
#define ERRCODE_INPUTMGR                                0x1600

#endif

