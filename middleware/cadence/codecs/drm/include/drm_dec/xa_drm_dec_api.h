/*
 * Copyright (c) 2014-2022 Cadence Design Systems, Inc.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef __XA_DRM_DEC_API_H__
#define __XA_DRM_DEC_API_H__

#include "xa_apicmd_standards.h"
#include "xa_error_standards.h"
#include "xa_type_def.h"

/* codec specific configuration parameters */
enum xa_config_param_drm_dec {

    XA_DRM_DEC_CONFIG_PARAM_DATARATE                = 0 // Not supported in DRM Decoder: Can be derived from Super frame length
        , XA_DRM_DEC_CONFIG_PARAM_SAMPLERATE        = 1
        , XA_DRM_DEC_CONFIG_PARAM_OUT_NUM_CHANNELS      = 2
        , XA_DRM_DEC_CONFIG_PARAM_PCM_WDSZ          = 3
        , XA_DRM_DEC_CONFIG_PARAM_FRAME_SIZE 	    = 4
        , XA_DRM_DEC_CONFIG_PARAM_INPUT_FORMAT 		= 5
        , XA_DRM_DEC_CONFIG_PARAM_NUM_CHANNELS      = 6

#define XA_DRM_DEC_CONFIG_PARAM_BASE 0x10
        , XA_DRM_DEC_CONFIG_PARAM_OUTPUT_MODE       = XA_DRM_DEC_CONFIG_PARAM_BASE + 0
        , XA_DRM_DEC_CONFIG_PARAM_QMF_MODE          = XA_DRM_DEC_CONFIG_PARAM_BASE + 1
        , XA_DRM_DEC_CONFIG_PARAM_DRM_AUDIO_CONFIG  = XA_DRM_DEC_CONFIG_PARAM_BASE + 2
        , XA_DRM_DEC_CONFIG_PARAM_FRAME_CORRUPT     = XA_DRM_DEC_CONFIG_PARAM_BASE + 3 // Not supported
        , XA_DRM_DEC_CONFIG_PARAM_SUPER_FRM_CNT     = XA_DRM_DEC_CONFIG_PARAM_BASE + 4 // Not supported
        , XA_DRM_DEC_CONFIG_PARAM_CONCEAL_STATE     = XA_DRM_DEC_CONFIG_PARAM_BASE + 5
        , XA_DRM_DEC_CONFIG_PARAM_CONCEAL_METHOD    = XA_DRM_DEC_CONFIG_PARAM_BASE + 6 // Not supported
        , XA_DRM_DEC_CONFIG_PARAM_CONCEAL_FADEIN    = XA_DRM_DEC_CONFIG_PARAM_BASE + 7
        , XA_DRM_DEC_CONFIG_PARAM_CONCEAL_FADEOUT   = XA_DRM_DEC_CONFIG_PARAM_BASE + 8
        , XA_DRM_DEC_CONFIG_PARAM_CONCEAL_MUTE_RELEASE = XA_DRM_DEC_CONFIG_PARAM_BASE + 9
        , XA_DRM_DEC_CONFIG_PARAM_CONCEAL_COMFORT_NOISE = XA_DRM_DEC_CONFIG_PARAM_BASE + 10
        , XA_DRM_DEC_CONFIG_PARAM_XHEAAC_HOLDOFF_MODE = XA_DRM_DEC_CONFIG_PARAM_BASE + 11

#define XA_DRM_DEC_AAC_CONFIG_PARAM_BASE 0x20
        , XA_DRM_DEC_AAC_CONFIG_PARAM_ASC           = XA_DRM_DEC_AAC_CONFIG_PARAM_BASE + 0 // Not supported in DRM Decoder ; for internal testing
        , XA_DRM_DEC_AAC_CONFIG_PARAM_ASC_LENGTH    = XA_DRM_DEC_AAC_CONFIG_PARAM_BASE + 1 // Not supported in DRM Decoder ; for internal testing
};

#define XA_CODEC_DRM_DEC 3

/* decoder specific error codes */
/*****************************************************************************/
/* Class 0: API Errors                                                       */
/*****************************************************************************/
/* Non Fatal Errors */
enum xa_error_nonfatal_api_drm_dec {
    XA_DRM_DEC_API_NONFATAL_CMD_TYPE_NOT_SUPPORTED        = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_api, XA_CODEC_DRM_DEC, 0),// Not used in current DRM Decoder
    XA_DRM_DEC_API_NONFATAL_INVALID_API_SEQ               = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_api, XA_CODEC_DRM_DEC, 1)
};

/* Fatal Errors */
enum xa_error_fatal_api_drm_dec {
    XA_DRM_DEC_API_FATAL_INVALID_API_SEQ                  = XA_ERROR_CODE(xa_severity_fatal, xa_class_api, XA_CODEC_DRM_DEC, 4)
};

/*****************************************************************************/
/* Class 1: Configuration Errors                                             */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_config_drm_dec {
    XA_DRM_DEC_CONFIG_NONFATAL_PARAMS_NOT_SET           = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_DRM_DEC, 0),
    XA_DRM_DEC_CONFIG_NONFATAL_INVALID_PARAM            = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_DRM_DEC, 1),// Not used in current DRM Decoder
    XA_DRM_DEC_CONFIG_NONFATAL_INVALID_GEN_STRM_POS     = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_DRM_DEC, 2),
    XA_DRM_DEC_CONFIG_NONFATAL_CONFORMANCE_TEST_ONLY     = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_DRM_DEC, 3) // Not reported by DRM Decoder

};
/* Fatal Errors */
enum xa_error_fatal_config_drm_dec {
    XA_DRM_DEC_CONFIG_FATAL_INVALID_PARAM               = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_DRM_DEC, 0),
    XA_DRM_DEC_CONFIG_FATAL_UNSUPPORTED_FORMAT          = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_DRM_DEC, 1),
    XA_DRM_DEC_CONFIG_FATAL_UNSUPPORTED_BITRATE          = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_DRM_DEC, 2),
    XA_DRM_DEC_CONFIG_FATAL_CONFIG_CHANGES_EXCEEDS_LIMIT = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_DRM_DEC, 3)

};

/*****************************************************************************/
/* Class 2: Execution Class Errors                                           */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_execute_drm_dec {
    XA_DRM_DEC_EXECUTE_NONFATAL_INSUFFICIENT_FRAME_DATA = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_DRM_DEC, 0),
    XA_DRM_DEC_EXECUTE_NONFATAL_PARSE_ERROR             = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_DRM_DEC, 1),
    XA_DRM_DEC_EXECUTE_NONFATAL_DECODE_FRAME_ERROR      = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_DRM_DEC, 2),// Not used in current DRM Decoder
    XA_DRM_DEC_EXECUTE_NONFATAL_EMPTY_IN_BUFFER         = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_DRM_DEC, 3),// Not used in current DRM Decoder
    XA_DRM_DEC_EXECUTE_NONFATAL_CRC_ERROR_CONCEALED     = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_DRM_DEC, 4),
    XA_DRM_DEC_EXECUTE_NONFATAL_STREAM_CHANGE_DETECTED  = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_DRM_DEC, 5),
    XA_DRM_DEC_EXECUTE_NONFATAL_NEED_DATA_SYNC_ERROR    = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_DRM_DEC, 6) // DRM specific error

};

/* Fatal Errors */
enum xa_error_fatal_execute_drm_dec {
    XA_DRM_DEC_EXECUTE_FATAL_PARSE_ERROR                = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_DRM_DEC, 0),
    XA_DRM_DEC_EXECUTE_FATAL_INIT_ERROR             = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_DRM_DEC, 1),
    XA_DRM_DEC_EXECUTE_FATAL_STREAM_CHANGED             = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_DRM_DEC, 2),// Not used in current DRM Decoder
    XA_DRM_DEC_EXECUTE_FATAL_UNSUPPORTED_FEATURE        = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_DRM_DEC, 3)// Not used in current DRM Decoder
};

/*****************************************************************************/
/* Codec Specific enumurations and structures                                */
/*****************************************************************************/

typedef enum
{
    XA_DRM_DEC_INPUT_FORMAT_DRM         = 12,
    XA_DRM_DEC_INPUT_FORMAT_RAW_AAC     = 5

} XA_DRM_DEC_INPUT_FORMAT;

typedef enum
{
    XA_DRM_DEC_OUTPUT_NORMAL   = 0,
    XA_DRM_DEC_OUTPUT_MONO     = 1,
    XA_DRM_DEC_OUTPUT_STEREO   = 2

} XA_DRM_DEC_OUTPUT_MODE;

typedef enum
{
  XA_DRM_DEC_QMF_AUTO = -1,
  XA_DRM_DEC_QMF_HIGH_QUALITY = 0,
  XA_DRM_DEC_QMF_LOW_POWER = 1

} XA_DRM_DEC_QMF_MODE;

typedef enum
{
  XA_DRM_CONCEAL_STATE_NORMAL = 0,    /* No concealment or single frame (interpolation) concealment without signal level attenuation*/
  XA_DRM_CONCEAL_STATE_FADE_IN = 2,   /* The decoder is in fade-in mode (decoded but attenuated signal)*/
  XA_DRM_CONCEAL_STATE_MUTE = 3,      /* The decoder is in mute mode (silence or comfort noise)*/
  XA_DRM_CONCEAL_STATE_FADE_OUT = 4   /* The decoder is in fade-out mode (concealed and attenuated signal)*/

} XA_DRM_CONCEAL_STATE;

#define INTERFACE_MAX_CONFIG_SIZE  64   /* max size of the interface header extension in bytes */

typedef struct
{
    UWORD32   audioSamplingRate;
    UWORD32   lengthOfAudioSuperFrame;
    UWORD32   lengthHigherProtected;
    UWORD16 configLength;
    UWORD8  audioCoding;
    UWORD8  audioMode;
    UWORD8  sbrFlag;
    UWORD8  coderField;
    UWORD8  drmPlus;
    UWORD8  config[INTERFACE_MAX_CONFIG_SIZE];  /* raw SDC */

} XA_DRM_DEC_DRM_AUDIO_CONFIG;

#if defined(USE_DLL) && defined(_WIN32)
#define DLL_SHARED __declspec(dllimport)
#elif defined (_WINDLL)
#define DLL_SHARED __declspec(dllexport)
#else
#define DLL_SHARED
#endif

#if defined(__cplusplus)
extern "C" {
#endif
    DLL_SHARED xa_codec_func_t xa_drm_dec;
#if defined(__cplusplus)
}
#endif

#endif /* __XA_DRM_DEC_API_H__ */


