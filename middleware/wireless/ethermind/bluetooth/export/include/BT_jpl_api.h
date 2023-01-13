
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

/**
 * \addtogroup bt_utils Utilities
 * \{
 */
/**
 * \defgroup jpl_module JPL (Jitter Protection Layer)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  module to the Application
 *  and other upper layers of the stack.
 */
/* -------------------------------------------- Global Definitions */
/**
 * \defgroup jpl_events Events
 * \{
 * Describes Events for the module.
 */
/** JPL Event Codes */
/**
 * @name JPL Event Codes
 *
 * Constant Definitions for JPL Event Codes
 */
/*@{*/
#define JPL_DATA_IND                            0x01U
#define JPL_OVERFLOW_IND                        0x02U
#define JPL_UNDERFLOW_IND                       0x03U
#define JPL_STALE_PACKET_IND                    0x04U
#define JPL_SILENCE_DATA_IND                    0x05U
/*@}*/
/** \} */
/**
 * \defgroup jpl_defines Defines
 * \{
 * Describes defines for the module.
 */
/**
 * \defgroup jpl_constants Constants
 * \{
 * Describes constants defined by the module.
 */
/** Tolerance limit for switching from Silence to actual PCM data */
#define JPL_TOLERANCE                           375
/** \} */
/** \} */
/* -------------------------------------------- Structures/Data Types */
/**
 * \defgroup jpl_appl_callback Application Callback
 * \{
 * Describes Application Callback defined by the module.
 */
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
/** \} */
/**
 * \addtogroup jpl_defines Defines
 * \{
 */
/**
 * \defgroup jpl_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
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
/** \} */
/** \} */
/* -------------------------------------------- Macros */


/* -------------------------------------------- API Declaration */
#ifdef __cplusplus
extern "C" {
#endif
/**
 * \defgroup jpl_api API Definitions
 *
 * Describes API definitions of this module.
 * \{
 *
 */
/** To initialize the JPL module */
/**
 *  \brief To initialize the JPL module
 *
 *  \par Description
 *       This API initializes the Jitter Protection Library.
 *
 *  \param [in] callback
 *         Application provided Event Indication Callback Function
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
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
/**
 *  \fn To start JPL for accepting SBC frames
 *
 *  \par Description
 *       This function starts the JPL module for accepting SBC frames for
 *       Jitter Protection.
 *
 *  \param [in] params
 *         Pointer to allocated JPL_PARAM structure containing filled up
 *         runtime configuration settings
 *
 *  \return API_RESULT
 *          API_SUCCESS, or, an Error Code from BT_error.h
 */
API_RESULT BT_jpl_start
           (
               /* IN */  JPL_PARAM *    params
           );

/** To suspend JPL from accepting SBC frames */
/*
 *  \brief To suspend JPL from accepting SBC frames
 *
 *  \par Description
 *  To indicate that audio streaming is suspended, and can be reconfigured
 *
 *  \param None
 *
 *  \return API_RESULT
 *          API_SUCCESS, or, an Error Code from BT_error.h
 */
API_RESULT BT_jpl_stop ( void );

/** To add incoming SBC frames for Jitter Protection */
/**
 *  \brief To add incoming SBC frames for Jitter Protection
 *
 *  \par Description
 *       This function adds incoming SBC frames, and decodes the same to PCM if
 *       required, to Jitter Protection Buffer.
 *
 *  \param [in] seq
 *         The sequence number of the packet containing SBC
 *
 *  \param [in] timestamp
 *         The timestamp of the media packet containing SBC
 *
 *  \param [in] data
 *         Pointer to SBC frames that are to be added to JPL Buffer
 *
 *  \param [in] datalen
 *         The total length of all the frames, inclusive of 1 octet SBC header
 *
 *  \return API_RESULT
 *          API_SUCCESS, or, an Error Code from BT_error.h
 */
API_RESULT BT_jpl_add_frames
           (
               /* IN */  UINT16     seq,
               /* IN */  UINT32     timestamp,
               /* IN */  UCHAR *    data,
               /* IN */  UINT16     datalen
           );

/** To request delivery of decoded SBC frames from Jitter Protection */
/*
 *  \brief To request delivery of decoded SBC frames from Jitter Protection
 *
 *  \par Description
 *       To remove PCM data from JPL.
 *
 *  \param [in] data 
 *         Pointer to the PCM buffer block that has been played and
 *         can be freed. All buffer blocks till this one will be
 *         freed.
 *
 *  \param [in] datalen
 *         Datalen of PCM buffer to be freed
 *
 *  \return API_RESULT
 *          API_SUCCESS, or, an Error Code from BT_error.h
 */
API_RESULT BT_jpl_remove_frames
           (
               /* IN */  UCHAR *    data,
               /* IN */  UINT16     datalen
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
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_JPL_API_ */

