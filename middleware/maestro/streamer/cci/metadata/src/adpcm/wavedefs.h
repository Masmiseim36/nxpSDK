/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004
//
// Filename: 	wavedefs.h
// Description: Includes header data for wav meta-data functions.
///////////////////////////////////////////////////////////////////////////////

#define ADPCM_READ_BUFFER_SIZE 156

/* WAVE form wFormatTag IDs */
#define WAVE_FORMAT_UNKNOWN               0x0000                  /*  Microsoft Corporation  */
#define WAVE_FORMAT_PCM                   0x0001                  /* Microsoft PCM */
#define WAVE_FORMAT_ADPCM                 0x0002                  /*  Microsoft Corporation  */
#define WAVE_FORMAT_IBM_CVSD              0x0005                  /*  IBM Corporation  */
#define WAVE_FORMAT_ALAW                  0x0006                  /*  Microsoft Corporation  */
#define WAVE_FORMAT_MULAW                 0x0007                  /*  Microsoft Corporation  */
#define WAVE_FORMAT_OKI_ADPCM             0x0010                  /*  OKI  */
#define WAVE_FORMAT_DVI_ADPCM             0x0011                  /*  Intel Corporation  */
#define WAVE_FORMAT_IMA_ADPCM             (WAVE_FORMAT_DVI_ADPCM) /*  Intel Corporation  */
#define WAVE_FORMAT_MEDIASPACE_ADPCM      0x0012                  /*  Videologic  */
#define WAVE_FORMAT_SIERRA_ADPCM          0x0013                  /*  Sierra Semiconductor Corp  */
#define WAVE_FORMAT_G723_ADPCM            0x0014                  /*  Antex Electronics Corporation  */
#define WAVE_FORMAT_DIGISTD               0x0015                  /*  DSP Solutions, Inc.  */
#define WAVE_FORMAT_DIGIFIX               0x0016                  /*  DSP Solutions, Inc.  */
#define WAVE_FORMAT_DIALOGIC_OKI_ADPCM    0x0017                  /*  Dialogic Corporation  */
#define WAVE_FORMAT_YAMAHA_ADPCM          0x0020                  /*  Yamaha Corporation of America  */
#define WAVE_FORMAT_SONARC                0x0021                  /*  Speech Compression  */
#define WAVE_FORMAT_DSPGROUP_trueSPEECH   0x0022                  /*  DSP Group, Inc  */
#define WAVE_FORMAT_ECHOSC1               0x0023                  /*  Echo Speech Corporation  */
#define WAVE_FORMAT_AUDIOFILE_AF36        0x0024                  /*    */
#define WAVE_FORMAT_APTX                  0x0025                  /*  Audio Processing Technology  */
#define WAVE_FORMAT_AUDIOFILE_AF10        0x0026                  /*    */
#define WAVE_FORMAT_DOLBY_AC2             0x0030                  /*  Dolby Laboratories  */
#define WAVE_FORMAT_GSM610                0x0031                  /*  Microsoft Corporation  */
#define WAVE_FORMAT_ANTEX_ADPCME          0x0033                  /*  Antex Electronics Corporation  */
#define WAVE_FORMAT_CONTROL_RES_VQLPC     0x0034                  /*  Control Resources Limited  */
#define WAVE_FORMAT_DIGIREAL              0x0035                  /*  DSP Solutions, Inc.  */
#define WAVE_FORMAT_DIGIADPCM             0x0036                  /*  DSP Solutions, Inc.  */
#define WAVE_FORMAT_CONTROL_RES_CR10      0x0037                  /*  Control Resources Limited  */
#define WAVE_FORMAT_NMS_VBXADPCM          0x0038                  /*  Natural MicroSystems  */
#define WAVE_FORMAT_G721_ADPCM            0x0040                  /*  Antex Electronics Corporation  */
#define WAVE_FORMAT_MPEG                  0x0050                  /*  Microsoft Corporation  */
#define WAVE_FORMAT_CREATIVE_ADPCM        0x0200                  /*  Creative Labs, Inc  */
#define WAVE_FORMAT_CREATIVE_FASTSPEECH8  0x0202                  /*  Creative Labs, Inc  */
#define WAVE_FORMAT_CREATIVE_FASTSPEECH10 0x0203                  /*  Creative Labs, Inc  */
#define WAVE_FORMAT_FM_TOWNS_SND          0x0300                  /*  Fujitsu Corp.  */
#define WAVE_FORMAT_OLIGSM                0x1000                  /*  Ing C. Olivetti & C., S.p.A.  */
#define WAVE_FORMAT_OLIADPCM              0x1001                  /*  Ing C. Olivetti & C., S.p.A.  */
#define WAVE_FORMAT_OLICELP               0x1002                  /*  Ing C. Olivetti & C., S.p.A.  */
#define WAVE_FORMAT_OLISBC                0x1003                  /*  Ing C. Olivetti & C., S.p.A.  */
#define WAVE_FORMAT_OLIOPR                0x1004                  /*  Ing C. Olivetti & C., S.p.A.  */
#define WAVE_FORMAT_EXTENSIBLE            0xfffe                  /* wave format extensible */

// Supported chunk types
#define RIFF_CKID 0
#define WAVE_CKID 1
#define FMT_CKID  2
#define FACT_CKID 3
#define DATA_CKID 4
#define LIST_CKID 5
#define INFO_CKID 6
#define ISFT_CKID 7
#define IART_CKID 8
#define DISP_CKID 9
#define IGNR_CKID 10
#define ICRD_CKID 11
// Add more Ckids here
#define INAM_CKID 12
#define IPRD_CKID 13

#define UNKNOWN_CKID 14
#define TOTAL_CKID   14

#define FORM_CKID 0
#define AIFF_CKID 1
#define COMM_CKID 2
#define SSND_CKID 3
#define MARK_CKID 4

#define ADPCM_SOURCE_BUF_SIZE  2048
#define ADPCM_READ_BUFFER_SIZE 156
