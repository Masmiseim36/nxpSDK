
/**
 *  \file LC3_ndsp_api.h
 *
 *  \brief This file contains interface of encoder,decoder.
 */

/*
 *  Copyright (C) 2021. Mindtree Ltd.
 *  All rights reserved.
 */

/* LC3_DSP Macro defines LC3 Feature as below,
 * LC3_DSP = 0 : LC3 NODSP Feature Enabled
 * LC3_DSP = 1 : LC3 DSP Default Feature Enabled
 * LC3_DSP = 2 : LC3 DSP OPT Feature Enabled
 */

#if defined(LC3_DSP) && (LC3_DSP == 0)

#ifndef _H_LC3_NDSP_API_
#define _H_LC3_NDSP_API_

/* --------------------------------------------- Header File Inclusion */
//#include "LC3_os.h"
#include "lc3_ndsp_internal.h"

/* --------------------------------------------- Global Definitions */

/** LC3 maximum number of supported samples */
#define  LC3_INPUT_FRAME_SIZE_MAX                     	480
/** LC3 maximum supported framesize */
#define  LC3_FRAME_SIZE_MAX                       400

/** Buffer Size information */
#define  LC3_ENCODER_CORE_BUFFER_SIZE_MAX             3412   /* 3512 */
#define  LC3_ENCODER_WORK_BUFFER_SIZE_MAX             6848   /* 6784 */
#define  LC3_DECODER_CORE_BUFFER_SIZE_MAX             5100   /* 5216 */
#define  LC3_DECODER_WORK_BUFFER_SIZE_MAX             6528   /* 6512 */

/**
 * \name LC3 Encoder Error Codes
 * \{
 */
/** LC3 Encoder Success */
#define LC3_ENCODER_SUCCESS                           0x00
/** LC3 Encoder Size Allocation Error */
#define LC3_ENCODER_SIZE_ALLOCATE_ERROR               0x01
/** LC3 Encoder Core Buffer Null Pointer Error */
#define LC3_ENCODER_CORE_BUFFER_NULL_POINTER          0x02
/** LC3 Encoder Setup Parameter Error */
#define LC3_ENCODER_SETUP_PARAM_ERROR                 0x03
/** LC3 Encoder Bitrate Update Error */
#define LC3_ENCODER_BITRATE_UPDATE_ERROR              0x04
/** LC3 Encoder Work Buffer NULL Pointer Error */
#define LC3_ENCODER_WORK_BUFFER_NULL_POINTER          0x05
/** LC3 Encoder Invalid Sample Rate Error */
#define LC3_ENCODER_INVALID_SAMPLE_RATE               0x06
/** LC3 Encoder Invalid Frame MS Error */
#define LC3_ENCODER_INVALID_FRAME_MS                  0x07
/** LC3 Encoder Invalid Channel Count Error */
#define LC3_ENCODER_INVALID_CHANNEL_COUNT             0x08
/** LC3 Encoder Invalid Target Bytes Error */
#define LC3_ENCODER_INVALID_TARGET_BYTES              0x09
/** LC3 Encoder Invalid Context Error */
#define LC3_ENCODER_INVALID_CONTEXT                   0x0A
/** \} */

/**
 * \name LC3 Decoder Error Codes
 * \{
 */
/** LC3 Decoder Success */
#define LC3_DECODER_SUCCESS                           0x00
/** LC3 Decoder Size Allocation Error */
#define LC3_DECODER_SIZE_ALLOCATE_ERROR               0x01
/** LC3 Decoder Core Buffer Null Pointer Error */
#define LC3_DECODER_CORE_BUFFER_NULL_POINTER          0x02
/** LC3 Decoder Setup Parameter Error */
#define LC3_DECODER_SETUP_PARAM_ERROR                 0x03
/** LC3 Decoder Work Buffer NULL Pointer Error */
#define LC3_DECODER_WORK_BUFFER_NULL_POINTER          0x04
/** LC3 Decoder Invalid Sample Rate Error */
#define LC3_DECODER_INVALID_SAMPLE_RATE               0x05
/** LC3 Decoder Invalid Frame MS Error */
#define LC3_DECODER_INVALID_FRAME_MS                  0x06
/** LC3 Decoder Invalid Channel Count Error */
#define LC3_DECODER_INVALID_CHANNEL_COUNT             0x07
/** LC3 Decoder Invalid PLC Type Error */
#define LC3_DECODER_INVALID_PLC_TYPE                  0x08
/** LC3 Decoder Invalid Context Error */
#define LC3_DECODER_INVALID_CONTEXT                   0x09
/** \} */

#define LC3_DEC_SET_CONFIG_PARAMS lc3_ndsp_dec_set_config_params
#define LC3_ENC_SET_CONFIG_PARAMS lc3_ndsp_enc_set_config_params
/* --------------------------------------------- Data Types/ Structures */


/* --------------------------------------------- Function */

#ifdef __cplusplus
extern "C" {
#endif


/* 
 * initialization of the config, which will be shared for all the channels in a CIG or BIG
 */
INT32 lc3_ndsp_enc_set_config_params(
	LC3_ENCODER_CONFIG *config, 				//config
	INT32 sample_rate, 							//the sampling rate
	INT32 channels, 							//number of channels.  If CIS/BIS, = 1
	INT32 n_dms, 								//frame period
	INT32 bps									//the compressed data rate
);

/* 
 * initialize the encoder channel context
 */
INT32 lc3_enc_init_ch_cntx(
	LC3_ENCODER_CHANNEL_CNTX *channel_cntx,    	//channel context
	LC3_ENCODER_CONFIG *config, 				//config
	INT32 bytes_count							//byte per frame
);

/* 
 * encode a frame.  It is channel based
 */
INT32 LC3_encode_a_frame (
	LC3_ENCODER_CHANNEL_CNTX *channel_cntx, 	//channel context
	INT32 *pcm_in, 								//input sample buffer
	UINT8 *bs_out, 								//output encoded data
	UINT8 *work_buffer							//the scratch buffer
);

/* 
 * initialization of the config, which will be shared for all the channels in a CIG or BIG
 */
INT32 lc3_ndsp_dec_set_config_params(
	LC3_DECODER_CONFIG *config, 				//the config
	INT32 sample_rate, 							//the sampling rate
	INT32 channels, 							//the number of channels
	INT32 plc_type, 							//the type of PLC
	INT32 n_dms, 								//the frame period
	INT32 bitPerSmpl							//the bit per sample
);

/* 
 * initialize the decoder channel context
 */
INT32 lc3_dec_init_ch_cntx (
	LC3_DECODER_CHANNEL_CNTX *channel_cntx, 	//channel context
	LC3_DECODER_CONFIG *config, 				//config
	INT32 channel_bytes							//byte per frame
);

/* 
 * decode a frame.  It is channel based
 */
INT32 LC3_decode_a_frame(
	LC3_DECODER_CHANNEL_CNTX *channel_cntx,		//channel context
	UINT8 *bs_in,								//the input compressed data
	INT32 *pcm_out,								//the decoded sample buffer
	INT16  flg_BFI,								//the bad frame flag
	UINT8 *work_buffer							//the scratch buffer
);


#ifdef __cplusplus
};
#endif

#endif /* _H_LC3_NDSP_API_ */

#endif /* defined(LC3_DSP) && (LC3_DSP == 0) */
