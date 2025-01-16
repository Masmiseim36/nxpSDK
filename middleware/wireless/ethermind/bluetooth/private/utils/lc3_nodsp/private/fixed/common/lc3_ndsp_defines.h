
/**
 *  \file lc3_ndsp_defines.h
 *
 *  \brief This file contains define configs of encoder, decoder.
 */

/*
 *  Copyright (C) 2021. Mindtree Ltd.
 *  All rights reserved.
 */

#if defined(LC3_DSP) && (LC3_DSP == 0)
#ifndef _H_LC3_NDSP_DEFINES_
#define _H_LC3_NDSP_DEFINES_

/* --------------------------------------------- Header File Inclusion */


/* --------------------------------------------- Global Definitions */

/* Bitrate limits for 10ms, 7.5ms */
#define  LC3_MIN_BITRATE_10_MS          16000
#define  LC3_MIN_BITRATE_10_MS_44K      14700
#define  LC3_MIN_BITRATE_7_5_MS         21334
#define  LC3_MIN_BITRATE_7_5_MS_44K     19600

#define  LC3_MAX_BITRATE_10_MS          320000
#define  LC3_MAX_BITRATE_10_MS_44K      294000
#define  LC3_MAX_BITRATE_7_5_MS         426667
#define  LC3_MAX_BITRATE_7_5_MS_44K     392000

#define  LC3_INPUT_FRAME_SIZE_MAX			480
#define  LC3_MAX_IDCT_MEM_SIZE			(LC3_INPUT_FRAME_SIZE_MAX * 5 / 8)		//300, short
#define  LC3_MAX_PITCH_6K4				114
#define  LC3_MAX_PITCH_12K8				(LC3_MAX_PITCH_6K4 * 2)
#define  LC3_LTPF_X_LEN					(LC3_INPUT_FRAME_SIZE_MAX + LC3_INPUT_FRAME_SIZE_MAX / 40 - 2)	//490, short
#define  LC3_LTPF_Y_LEN					(LC3_INPUT_FRAME_SIZE_MAX + LC3_MAX_PITCH_12K8 * LC3_INPUT_FRAME_SIZE_MAX / 128 + LC3_INPUT_FRAME_SIZE_MAX / 80) 	//=480 855+6=1341, short
#define  LC3_X_Q_LEN					(400)	//short

#endif /* _H_LC3_NDSP_DEFINES_ */
#endif /*defined(LC3_DSP) && (LC3_DSP == 0)*/
