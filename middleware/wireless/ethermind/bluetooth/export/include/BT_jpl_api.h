
/**
 *  \file BT_jpl_api.h
 *
 *  \brief EtherMind Jitter Protection Library (JPL) API Header File.
 *
 *  This Header File contains declaration of JPL APIs, Macros and
 *  Constant Definitions for use in applications.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_JPL_API_
#define _H_BT_JPL_API_

/* -------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "sbc_api.h"


/* -------------------------------------------- Global Definitions */
/** JPL Event Codes */
#define JPL_DATA_IND                            0x01
#define JPL_OVERFLOW_IND                        0x02
#define JPL_UNDERFLOW_IND                       0x03
#define JPL_STALE_PACKET_IND                    0x04
#define JPL_SILENCE_DATA_IND                    0x05

/** Tolerance limit for switching from Silence to actual PCM data */
#define JPL_TOLERANCE                           375


/* -------------------------------------------- Structures/Data Types */
/**
 *  JPL_CB
 *  Typedef for the Application notification callback pointer
 *
 *  \param [in] event_type
 *         The JPL Event Type for which the callback is called
 *
 *  \param [in] event_data
 *         Pointer to the PCM data returned for playing out.
 *         Valid only in case of event JPL_DATA_IND
 *
 *  \param [in] event_datalen
 *         Size of PCM data, in number of octets, passed to application
 *         Valid only in case of event JPL_DATA_IND
 *
 *  \return API_RESULT
 *          API_SUCCESS, otherwise an error code describing the reason
 *          for failure
 */
typedef API_RESULT (* JPL_CB)
        (
            /* IN */  UCHAR      event_type,
            /* IN */  UCHAR *    event_data,
            /* IN */  UINT16     event_datalen
        );


/**
 *  JPL_PARAM
 *
 *  This Data Type is used to configure various run-time parameters.
 */
typedef struct
{
    /**
     *  Number of PCM frames (decoded SBC frames) to be delivered
     *  to the application each time data is requested for removal
     */
    UINT16 pcm_num_frames;

    /**
     *  The Play-out Delay in Number of PCM frames.
     */
    UINT16 pod_frames;

    /** Parameters for SBC Decoder */
    SBC_CODEC_PARAM sbc_param;

} JPL_PARAM;

#ifdef JPL_USE_APP_MEMORY
/** JPL buffer memory from application */
typedef struct _JPL_BUFFERS
{
    /**
     * Memory for SBC encoded frames
     *
     * Currently, the JPL core expects the application
     * to allocate a buffer of size
     * (JPL_SBC_FRAME_SIZE * JPL_SBC_NUM_FRAMES)
     */
    UCHAR * sbc;

    /**
     * Memory for PCM frames
     *
     * Currently, the JPL core expects the application
     * to allocate a buffer of size
     * (JPL_PCM_BLOCK_SIZE * JPL_PCM_NUM_BLOCKS)
     */
    UCHAR * pcm;

    /**
     * Memory for Silence frames
     *
     * Currently, the JPL core expects the application
     * to allocate a buffer of size
     * JPL_PCM_BLOCK_SIZE
     */
    UCHAR * silence;

} JPL_BUFFERS;
#endif /* JPL_USE_APP_MEMORY */

#ifdef JPL_TEST_MODE
/*
 *  JPL_STATS
 *
 *  This data structure is used to return statistical (state) information
 *  to application (in test mode).
 */
typedef struct
{
    /* State */
    UCHAR state;

    /* JPL Buffer Pointers */
    UINT16 pcm_rd;
    UINT16 pcm_wr;
    UINT16 dac_rd;

    UINT16 pcm_count;
    UINT16 pcm_frames;

    UINT16 sbc_rd;
    UINT16 sbc_wr;

    UINT16 sbc_count;
    UINT16 sbc_frames;

    /* Timestamps & Sequence Number */
    UINT32 current_ts;
    UINT32 expected_ts;
    UINT16 expected_seq;

} JPL_STATS;
#endif /* JPL_TEST_MODE */


/* -------------------------------------------- Macros */


/* -------------------------------------------- API Declaration */
#ifdef __cplusplus
extern "C" {
#endif

/** To initialize the JPL module */
API_RESULT BT_jpl_init
           (
                /* IN */  JPL_CB    callback_fn
           );

#ifdef JPL_USE_APP_MEMORY
/** To register memory for SBC, PCM and Silence frames */
API_RESULT BT_jpl_register_buffers
           (
               /* IN */  JPL_BUFFERS * buffer
           );
#endif /* JPL_USE_APP_MEMORY */

/** To start JPL for accepting SBC frames */
API_RESULT BT_jpl_start
           (
               /* IN */  JPL_PARAM *    params
           );

/** To suspend JPL from accepting SBC frames */
API_RESULT BT_jpl_stop ( void );

/** To add incoming SBC frames for Jitter Protection */
API_RESULT BT_jpl_add_frames
           (
               /* IN */  UINT16     seq_num,
               /* IN */  UINT32     timestamp,
               /* IN */  UCHAR *    data,
               /* IN */  UINT16     datalen
           );

/** To request delivery of decoded SBC frames from Jitter Protection */
API_RESULT BT_jpl_remove_frames
           (
               /* IN */  UCHAR *    old_frame,
               /* IN */  UINT16     old_frame_datalen
           );


#ifdef JPL_TEST_MODE
/** To get statistical (state) information from JPL (in test mode) */
API_RESULT BT_jpl_get_stats
           (
               /* IN */  JPL_STATS *    stats
           );
#endif /* JPL_TEST_MODE */

#ifdef __cplusplus
};
#endif

#endif /* _H_BT_JPL_API_ */

