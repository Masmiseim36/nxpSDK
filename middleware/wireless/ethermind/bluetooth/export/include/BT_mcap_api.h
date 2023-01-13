
/**
 *  \file BT_mcap_api.h
 *
 *  This file contains the data structures, type definitions, MACROS, constants
 *  definitions and API functions that MCAP exposes for use to the upper layer
 *  profiles/applications
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_MCAP_API_
#define _H_BT_MCAP_API_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "l2cap.h"

/* --------------------------------------------- Global Definitions */
/**
 * \addtogroup bt_protocol Protocols
 * \{
 */
/**
 * \defgroup mcap_module MCAP (Multi-Channel Adaptation Protocol)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Multi-Channel Adaptation Protocol (MCAP) module to the Application and other upper
 *  layers of the stack.
 */
/**
 * \defgroup mcap_defines Defines
 * \{
 * Describes defines for the module.
 */
/**
 * \defgroup mcap_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/** MCAP Event Notification Callback Event Types */
#define MCAP_MD_CREATE_IND              0x01U
#define MCAP_MD_CREATE_CNF              0x02U
#define MCAP_MD_RECONNECT_IND           0x03U
#define MCAP_MD_RECONNECT_CNF           0x04U
#define MCAP_MD_ABORT_IND               0x05U
#define MCAP_MD_ABORT_CNF               0x06U
#define MCAP_MD_DELETE_IND              0x07U
#define MCAP_MD_DELETE_CNF              0x08U
#define MCAP_MD_DELETE_ALL_IND          (MCAP_MD_DELETE_IND | 0x80U)
#define MCAP_MD_DELETE_ALL_CNF          (MCAP_MD_DELETE_CNF | 0x80U)
#define MCAP_MD_DISCONNECT_IND          0x09U
#define MCAP_MD_DISCONNECT_CNF          0x0AU
#define MCAP_MC_CREATE_IND              0x0BU
#define MCAP_MC_CREATE_CNF              0x0CU
#define MCAP_MC_DISCONNECT_IND          0x0DU
#define MCAP_MC_DISCONNECT_CNF          0x0EU
#define MCAP_DATA_IND                   0x0FU
#define MCAP_DATA_WRITE_CNF             0x10U

#define MCAP_SYNC_CAP_REQ               0x11U
#define MCAP_SYNC_CAP_RSP               0x12U
#define MCAP_SYNC_SET_REQ               0x13U
#define MCAP_SYNC_SET_RSP               0x14U
#define MCAP_SYNC_INFO_IND              0x15U

/** TODO: Comment */
#define MCAP_SYNC_CAP_REQ_PARAM_LENGTH      (2U)
#define MCAP_SYNC_CAP_RSP_PARAM_LENGTH      (7U)
#define MCAP_SYNC_SET_REQ_PARAM_LENGTH      (13U)
#define MCAP_SYNC_SET_RSP_PARAM_LENGTH      (14U)
#define MCAP_SYNC_INFO_IND_PARAM_LENGTH     (14U)


/** TODO: Comment */
#define MCAP_INVALID_PSM                    0x0000U
#define MCAP_HDP_RELIABLE                   0x01U
#define MCAP_HDP_STREAMING                  0x02U


/** TODO: Comment */
#define MCAP_INVALID_MCL_ID                 0xFFFFU
#define MCAP_INVALID_MDL_ID                 0x0000U
#define MCAP_INVALID_MDEP_ID                0xFFU
#define MCAP_ALL_MDL_ID                     0xFFFFU
/** \} */
/** \} */


/* --------------------------------------------- Structures/Data Types */

/**
 *  \addtogroup mcap_defines Defines
 *  \{
 */

/**
 *  \addtogroup mcap_structures Structures
 *  \{
 */

/**
 *  This data type represents the abstract handle to refer while
 *  initiating a procedure with MCAP. Except utility APIs, all MCAP
 *  APIs require a MCAP Handle to be provided.
 */
typedef struct
{
    /**
     *  Identifies peer BD_ADDR of the MCL.
     *
     *  This is populated by the application while it is initiating a
     *  connection. In case a connection is received, MCAP will populate
     *  it with remote device's BD_ADDR.
     */
    UCHAR bd_addr [BT_BD_ADDR_SIZE];

    /**
     *  An identifier to uniquely identify an MCL, this is provided by
     *  MCAP shall not be altered by the application.
     */
    UINT16 mcl_id;

    /**
     *  An identifier that uniquely identifies a data channel for a
     *  particular MCL.
     */
    UINT16 mdl_id;

    /**
     *  Identifies the local data exchange endpoint id, to be provided by
     *  the application while initiating a md_create  operation, MCAP will
     *  remember it, provide the id during consequent transactions.
     */
    UINT8 mdep_id;

} MCAP_HANDLE;
/** \} */
/** \} */

/**
 *  \defgroup mcap_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */

/** The MCAP Asynchronous Event Notification Callback */
typedef API_RESULT (* MCAP_EVENT_NTF_CB)
                   (
                       MCAP_HANDLE * handle,
                       UCHAR         mcap_event,
                       UINT16        response,
                       void        * event_param,
                       UINT16        event_paramlen
                   );

/** TODO: Comment */
typedef void (* MCAP_L2CAP_CONFIG_CB)
             (
                 MCAP_HANDLE          * handle,
                 UCHAR                  channel_prf,
                 L2CAP_CONFIG_OPTION ** config_option
             );
/** \} */

/**
 *  \addtogroup mcap_defines Defines
 *  \{
 */

/**
 *  \addtogroup mcap_structures Structures
 *  \{
 */
/** MCAP Exchange Point information to be provided with mcap_register */
typedef struct
{
    /**
     * Event notification call back MCAP will invoke to signal the application
     * or profile, that is MCAP exchange point, of asynchronous events
     */
    MCAP_EVENT_NTF_CB ntf_cb;

    /**
     * Callback to fetch the L2CAP Configuration parameters for control
     * and associated data channels, if any, during channel creation
     */
    MCAP_L2CAP_CONFIG_CB config_cb;

    /** PSM for control channel */
    UINT16 control_channel_psm;

#ifdef MCAP_STANDARD_COMMANDS

    /** PSM for data channel */
    UINT16 data_channel_psm;

#endif /* MCAP_STANDARD_COMMANDS */

} MCAP_MEP;


/** Parameters to be provided at the time of creating a MCAP data channel */
typedef struct
{
    /** Remote Data Channel PSM */
    UINT16 rem_data_ch_psm;

    /**
     *  Channel preference, to indicate what type of L2CAP channel is desired,
     *  whether streaming, enhanced retransmission
     */
    UCHAR channel_preference;

    /** Remote MDEP ID */
    UCHAR rem_mdep_id;

} MCAP_MD_PARAMS;


/** TODO: Comment */
typedef struct
{
    /** Remote Control Channel PSM */
    UINT16 cntrl_ch_psm;

    /** Identifies mep that initiated the connection */
    UCHAR mep_id;

} MCAP_MC_CONNECT_PARAMS;

/** \} */
/** \} */


/* --------------------------------------------- Macros */
/* MCAP Utility macros */

/**
 *  \defgroup mcap_marcos Utility Macros
 *  \{
 *  Initialization and other Utility Macros offered by the module.
 */
/**
 *  Utility macro to initialize MCAP Exchange endpoint.
 *  The application may use it before registering a MEP with MCAP
 */
#ifdef MCAP_STANDARD_COMMANDS
#define MCAP_INIT_MEP(mep_info)                              \
        mep_info.ntf_cb = NULL;                              \
        mep_info.control_channel_psm = MCAP_INVALID_PSM;     \
        mep_info.data_channel_psm = MCAP_INVALID_PSM;        \
        mep_info.config_cb = NULL;

#else /* MCAP_STANDARD_COMMANDS */

#define MCAP_INIT_MEP(mep_info)                              \
        mep_info.ntf_cb = NULL;                              \
        mep_info.control_channel_psm = MCAP_INVALID_PSM;     \
        mep_info.config_cb = NULL;

#endif /* MCAP_STANDARD_COMMANDS */

/** Utility macro to set callback for MCAP Exchange endpoint */
#define MCAP_SET_MEP_CALLBACK(mep_info,cb)                   \
        (mep_info).ntf_cb = (cb);

/** Utility macro to set control channel PSM for MCAP Exchange endpoint */
#define MCAP_SET_MEP_CONTROL_CH_PSM(mep_info,psm)            \
        (mep_info).control_channel_psm = (psm);

/** Utility macro to set data channel PSM for MCAP Exchange endpoint */
#define MCAP_SET_MEP_DATA_CH_PSM(mep_id,psm)                 \
        (mep_id).data_channel_psm = (psm);

/** Utility macro to set the config call back */
#define MCAP_SET_CONFIG_CB(mep_id,cb)                        \
        (mep_id).config_cb = cb;
/**
 * Utility macro to initialize a MCAP Handle, applications may use it prior to
 * setting important elements of MCAP Handle
 */
#define MCAP_INIT_HANDLE(handle, rem_bd_addr)                \
        BT_COPY_BD_ADDR(((handle).bd_addr),(rem_bd_addr));   \
        MCAP_INIT_HANDLE_PARAM((handle));

#define MCAP_INIT_HANDLE_PARAM(handle)\
        (handle).mcl_id     = MCAP_INVALID_MCL_ID;           \
        (handle).mdl_id     = MCAP_INVALID_MDL_ID;           \
        (handle).mdep_id    = MCAP_INVALID_MDEP_ID;

/** Utility macro to set remote bluetooth device address in a MCAP Handle */
#define MCAP_SET_HANDLE_BD_ADDR (handle, rem_bd_addr)        \
          BT_COPY_BD_ADDR((handle.bd_addr),(rem_bd_addr));

/**
 * Utility macro to set the local MCAP Data Exchange Endpoint Identifier in a
 * MCAP handle
 */
#define MCAP_SET_HANDLE_MDEP(x,y)                            \
        (x).mdep_id = (y);

/** Utility macro to get the remote bluetooth device address in a MCAP Handle */
#define MCAP_GET_HANDLE_BD_ADDR (handle,rem_bd_addr)         \
        BT_COPY_BD_ADDR((rem_bd_addr),(handle.bd_addr));

/**
 * Utility macro to get the local MCAP Data Exchange Endpoint Identifier of a
 * MCAP handle
 */
#define MCAP_GET_HANDLE_MDEP(handle,mdep_id)                 \
        (mdep_id) = (handle).mdep_id;


#define MCAP_HANDLE_MDL(x)                                   \
        ((x)->mdl_id)

#define MCAP_GET_HANDLE_MDL(x,y)                             \
        (x) = MCAP_HANDLE_MDL(y);

#define MCAP_HANDLE_MCL(x)                                   \
        ((x)->mcl_id)

#define MCAP_HANDLE_MDEP(x)                                  \
        ((x)->mdep_id)

#define MCAP_GET_HANDLE_MCL(x,y)                             \
        (x) = MCAP_HANDLE_MCL(y);

#define MCAP_INIT_MD_PARAMS(params)                          \
        params.rem_data_ch_psm = MCAP_INVALID_PSM;           \
        params.channel_preference = 0x00U;                   \
        params.rem_mdep_id = MCAP_INVALID_MDEP_ID;

#define MCAP_SET_MD_PARAM_RMT_PSM(params,psm)                \
        params.rem_data_ch_psm = psm;

#define MCAP_SET_MD_PARAMS_CH_TYPE(params,ch_type)           \
        params.channel_preference = ch_type;

#define MCAP_SET_MD_PARAMS_RMT_MDEP(params,mdep_id)          \
        params.rem_mdep_id = mdep_id;

#ifdef MCAP_CLOCK_SYNC

#define MCAP_PACK_1_BYTE(dst,val)                        \
        * (dst) = (UCHAR)val;

#define MCAP_PACK_2_BYTES(dst,val)                       \
        (dst)[1U] = (UCHAR)val;                          \
        (dst)[0U] = (UCHAR)(val >>8U);

#define MCAP_PACK_4_BYTES(dst,val)                       \
        (dst)[3U] = (UCHAR)(val);                        \
        (dst)[2U] = (UCHAR)((val) >>  8U);               \
        (dst)[1U] = (UCHAR)((val) >> 16U);               \
        (dst)[0U] = (UCHAR)((val) >> 24U);

#define MCAP_PACK_8_BYTES(dst,val)                       \
        (dst)[7U] = (UCHAR)(val);                        \
        (dst)[6U] = (UCHAR)((val) >>  8U);               \
        (dst)[5U] = (UCHAR)((val) >> 16U);               \
        (dst)[4U] = (UCHAR)((val) >> 24U);               \
        (dst)[3U] = (UCHAR)((val) >> 32U);               \
        (dst)[2U] = (UCHAR)((val) >> 40U);               \
        (dst)[1U] = (UCHAR)((val) >> 48U);               \
        (dst)[0U] = (UCHAR)((val) >> 56U);

#define MCAP_UNPACK_1_BYTE(dst,src)                          \
        *((UCHAR *)(dst)) = (UCHAR)(*(src));

#define MCAP_UNPACK_2_BYTES(dst,src)                         \
        *((UINT16 *)(dst)) = (UINT16)((src)[0U]);            \
        *((UINT16 *)(dst)) = ((*((UINT16 *)(dst))) << 8U);   \
        *((UINT16 *)(dst)) |= (UINT16)((src)[1U]);

#define MCAP_UNPACK_4_BYTES(dst,src)                         \
        *((UINT32 *)(dst)) = (UINT32)((src)[0U]);            \
        *((UINT32 *)(dst)) = ((*((UINT32 *)(dst))) << 8U);   \
        *((UINT32 *)(dst)) |= (UINT32)((src)[1U]);           \
        *((UINT32 *)(dst)) = ((*((UINT32 *)(dst))) << 8U);   \
        *((UINT32 *)(dst)) |= (UINT32)((src)[2U]);           \
        *((UINT32 *)(dst)) = ((*((UINT32 *)(dst))) << 8U);   \
        *((UINT32 *)(dst)) |= (UINT32)((src)[3U]);

#define MCAP_UNPACK_8_BYTES(dst,src)                         \
        *((UINT64 *)(dst)) = (UINT64)*((src) + 0U);          \
        *((UINT64 *)(dst)) = ((*((UINT64 *)(dst))) << 8U);   \
        *((UINT64 *)(dst)) |= (UINT64)*((src) + 1U);         \
        *((UINT64 *)(dst)) = ((*((UINT64 *)(dst))) << 8U);   \
        *((UINT64 *)(dst)) |= (UINT64)*((src) + 2U);         \
        *((UINT64 *)(dst)) = ((*((UINT64 *)(dst))) << 8U);   \
        *((UINT64 *)(dst)) |= (UINT64)*((src) + 3U);         \
        *((UINT64 *)(dst)) = ((*((UINT64 *)(dst))) << 8U);   \
        *((UINT64 *)(dst)) |= (UINT64)*((src) + 4U);         \
        *((UINT64 *)(dst)) = ((*((UINT64 *)(dst))) << 8U);   \
        *((UINT64 *)(dst)) |= (UINT64)*((src) + 5U);         \
        *((UINT64 *)(dst)) = ((*((UINT64 *)(dst))) << 8U);   \
        *((UINT64 *)(dst)) |= (UINT64)*((src) + 6U);         \
        *((UINT64 *)(dst)) = ((*((UINT64 *)(dst))) << 8U);   \
        *((UINT64 *)(dst)) |= (UINT64)*((src) + 7U);

#define MCAP_WRITE_TIMESTAMP_CLOCK_INSTANT(dst,val)          \
        MCAP_PACK_8_BYTES((dst),(val));

#define MCAP_WRITE_TIMESTAMP_REQUIRED_ACCURACY(dst,val)      \
        MCAP_PACK_2_BYTES((dst),(val));

#define MCAP_WRITE_BLUETOOTH_CLOCK_ACCESS_RESOLUTION(dst,val)\
        MCAP_PACK_1_BYTE((dst),(val));

#define MCAP_WRITE_SYNC_LEAD_TIME(dst,val)                   \
        MCAP_PACK_2_BYTES((dst),(val));

#define MCAP_WRITE_TIMESTAMP_NATIVE_RESOLUTION(dst,val)      \
        MCAP_PACK_2_BYTES((dst),(val));

#define MCAP_WRITE_TIMESTAMP_NATIVE_ACCURACY(dst,val)        \
        MCAP_PACK_2_BYTES(dst,val);

#define MCAP_WRITE_TIMESTAMP_UPDATE_INFORMATION(dst,val)     \
        MCAP_PACK_1_BYTE((dst),(val));

#define MCAP_WRITE_BLUETOOTH_CLOCK_SYNC_TIME(dst,val)        \
        MCAP_PACK_4_BYTES((dst),(val));

#define MCAP_WRITE_TIMESTAMP_SYNC_TIME(dst,val)              \
        MCAP_PACK_8_BYTES((dst),(val));

#define MCAP_WRITE_TIMESTAMP_SAMPLE_ACCURACY(dst,val)        \
        MCAP_PACK_2_BYTES((dst),(val));

#define MCAP_SET_TIMESTAMP(handle,timestamp)                 \
        BT_mcap_set_timestamp (handle,timestamp);

#define MCAP_GET_TIMESTAMP(handle,timestamp)                 \
        BT_mcap_get_timestamp (handle,timestamp);

#endif /* MCAP_CLOCK_SYNC */
/** \} */


/* --------------------------------------------- API Declarations */
/**
 * \defgroup mcap_api_defs API Definitions
 * \{
 * This section describes the EtherMind Multi-Channel Adaptation Protocol APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \brief To register application Exchange Point with MCAP
 *
 *  \Description
 *  This routine enables MCAP applications/profiles an exchange point with MCAP
 *  in order to receive asynchronous events from MCAP. The profile or
 *  application is also expected to provide the PSM values to be used for
 *  control channel and data channel PSM, to facilitate creation of control
 *  channel and subsequently data channels for it.
 *
 *  \param [in] mep
 *         MCAP Exchange point info to be provided by the MCAP application or
 *         profile in order to start receiving MCAP events.
 *
 *  \param [out] mep_id
 *         Unique MCAP Exchange Point identifier provided by MCAP. This is
 *         identifier is to be provided by the application during subsequent
 *         creation of control channel connection
 *
 *  \return API_RESULT: API_SUCCESS or an error code indicating reason for
 *                      failure
 */
API_RESULT BT_mcap_register_mep
           (
               /* IN */  MCAP_MEP    * mep,
               /* OUT */ UCHAR       * mep_id
           );


/* TODO: Add Header */
API_RESULT BT_mcap_common_api_handler
           (
                /* IN */ MCAP_HANDLE    * handle,
                /* IN */ UCHAR            mcap_cmd,
                /* IN */ UINT16           rsp_code,
                /* IN */ void           * params,
                /* IN */ UINT16           size
           );


/**
 *  \brief To initiate control channel connection with a remote device
 *
 *  \Description
 *  This routine enables an Exchange point [application / profile] to create a
 *  control channel with a remote device
 *
 *  NOTE: MCAP_MC_CONNECT_CNF marks the end of this procedure
 *
 *  \param handle
 *         MCAP Handle containing the remote bd address with the connection is
 *         desired
 *
 *  \param mc_params
 *         Indicating remote side's control channel PSM and indentifying the
 *         MCAP Exchange Point Identifier, that is initiating the connection
 *
 *  \return API_RESULT
 *          API_SUCCESS or an error code indicating reason for failure
 */
#define BT_mcap_mc_connect_req(handle,mc_params)             \
        BT_mcap_common_api_handler                           \
        (                                                    \
            handle,                                          \
            MCAP_MC_CREATE_IND,                              \
            API_SUCCESS,                                     \
            mc_params,                                       \
            sizeof(MCAP_MC_CONNECT_PARAMS)                   \
        );


/**
 *  \brief To initiate disconnect control channel connection
 *
 *  \Description
 *  This routine initiates disconnection procedure for control channel
 *  identified by handle. Apart from disconnecting the control channel, this
 *  routine would also initiate disconnection of all associated data channels
 *
 *  NOTE: MCAP_MC_DISCONNECT_CNF marks the end of this procedure
 *
 *  \param handle
 *         MCAP Handle to identify the control channel
 *
 *  \return API_RESULT: API_SUCCESS or an error code indicating reason for
 *                      failure
 */
#define BT_mcap_mc_disconnect_req(handle)                    \
        BT_mcap_common_api_handler                           \
        (                                                    \
            handle,                                          \
            MCAP_MC_DISCONNECT_IND,                          \
            API_SUCCESS,                                     \
            NULL,                                            \
            0                                                \
        );

/**
 *  \brief To create data channel with remote a device.
 *
 *  \Description
 *  This routine initiates a data channel connection procedure on control
 *  channel which is identified by handle
 *
 *  NOTE: MCAP_MD_CREATE_CNF marks the end of this procedure
 *
 *  \param handle
 *         MCAP handle to identify the control channel
 *
 *  \param md_params (MCAP_MD_PARAMS *)
 *         Parameters to be provided to initiate data channel connection.
 *
 *  \return API_RESULT: API_SUCCESS or an error code indicating reason for
 *          failure
 */
#define BT_mcap_md_create_req(handle,md_params)              \
        BT_mcap_common_api_handler                           \
        (                                                    \
            handle,                                          \
            MCAP_MD_CREATE_IND,                              \
            API_SUCCESS,                                     \
            md_params,                                       \
            sizeof(MCAP_MD_PARAMS)                           \
        );

/**
 *  \brief To reconnect a data channel with remote device
 *
 *  \Description
 *  This routine initiate data channel reconnect procedure on data channel
 *  identified by handle.
 *
 *  NOTE: MCAP_MD_RECONNECT_CNF marks the end of this procedure
 *
 *  \param handle
 *         MCAP handle to identify the data channel
 *
 *  \param rem_data_ch_psm (UINT16 *)
 *         Remore side's data channel PSM
 *
 *  \return API_RESULT: API_SUCCESS or an error code indicating reason for
 *          failure
 */
#define BT_mcap_md_reconnect_req(handle,rem_data_ch_psm)     \
        BT_mcap_common_api_handler                           \
        (                                                    \
            handle,                                          \
            MCAP_MD_RECONNECT_IND,                           \
            API_SUCCESS,                                     \
            rem_data_ch_psm,                                 \
            sizeof(UINT16)                                   \
        );

/**
 *  \brief To abort a data channel connection with remote device
 *
 *  \Description
 *  This routine aborts data channel creation or reconnection procedure on data
 *  channel identified by handle
 *
 *  NOTE: MCAP_MD_ABORT_CNF event marks the end of this procedure
 *
 *  \param handle
 *         MCAP handle to identify the data channel
 *
 *  \return API_RESULT: API_SUCCESS or an error code indicating reason for
 *          failure
*/
#define BT_mcap_md_abort_req(handle)                         \
        BT_mcap_common_api_handler                           \
        (                                                    \
            handle,                                          \
            MCAP_MD_ABORT_IND,                               \
            API_SUCCESS,                                     \
            NULL,                                            \
            0U                                               \
        );

/**
 *  \brief To initiate delete mdl
 *
 *  \Description
 *  This routine initiates delete procedure for data channel identified by
 *  handle.
 *
 *  NOTE: MCAP_MD_DELETE_CNF event marks the end of this procedure
 *
 *  \param handle
 *         MCAP handle to identify the data channel
 *
 *  \return API_RESULT: API_SUCCESS or an error code indicating reason for
 *          failure
 */
#define BT_mcap_md_delete_req(handle)                        \
        BT_mcap_common_api_handler                           \
        (                                                    \
            handle,                                          \
            MCAP_MD_DELETE_IND,                              \
            API_SUCCESS,                                     \
            NULL,                                            \
            0U                                               \
        );

/**
 *  \brief To initiate delete mdl
 *
 *  \Description
 *  This routine initiates delete procedure for data channel identified by
 *  handle.
 *
 *  NOTE: MCAP_MD_DELETE_CNF event marks the end of this procedure
 *
 *  \param handle
 *         MCAP handle to identify the data channel
 *
 *  \return API_RESULT: API_SUCCESS or an error code indicating reason for
 *          failure
 */
#define BT_mcap_md_delete_all_req(mcap_handle)               \
        ((MCAP_HANDLE_MDL((MCAP_HANDLE *)mcap_handle) =      \
        MCAP_ALL_MDL_ID) &                                   \
        BT_mcap_common_api_handler                           \
        (                                                    \
            mcap_handle,                                     \
            MCAP_MD_DELETE_ALL_IND,                          \
            API_SUCCESS,                                     \
            NULL,                                            \
            0U                                               \
        ));

/**
 *  \brief To initiate disconnect data channel
 *
 *  \Description
 *  This routine initiate data channel disconnect procedurefor data
 *  channel identified by handle
 *
 *  NOTE: MCAP_MD_DISCONNECT_CNF event marks the end of this procedure
 *
 *  \param handle
 *         MCAP handle to identify the data channel
 *
 *  \return API_RESULT: API_SUCCESS or an error code indicating reason for
 *          failure
 */
#define BT_mcap_md_disconnect_req(handle)                    \
        BT_mcap_common_api_handler                           \
        (                                                    \
            handle,                                          \
            MCAP_MD_DISCONNECT_IND,                          \
            API_SUCCESS,                                     \
            NULL,                                            \
            0U                                               \
        );

/**
 *  \brief To write the data on to mdl
 *
 *  \Description
 *  This routine initiate data write procedure on mdl which is identified by
 *  handle
 *
 *  \param handle
 *         MCAP handle to identify the mdl and cotrol channel
 *
 *  \param data
 *         pointer to data to be written
 *
 *  \param data_len
 *         length of data to be written
 *
 *  \return API_SUCCESS on successfully initiating the data write procedure or
 *          an error code indicating reason for failure
 *
 *  NOTE: MCAP_DATA_WRITE_CNF will indicate whether the write succeeded or not,
 *  also on reception of this event, memory, in case allocated for holding the
 *  data can be freed
 */
#ifdef MCAP_STANDARD_COMMANDS
API_RESULT BT_mcap_md_write
           (
                /* IN */ MCAP_HANDLE    * handle,
                /* IN */ UCHAR          * data,
                /* IN */ UINT16           data_len
           );
#else
#define BT_mcap_md_write(h, d, dl)   API_SUCCESS
#endif /* MCAP_STANDARD_COMMANDS */

/**
 *  \brief To send the response for MD_CREATE request to remote device
 *
 *  \Description
 *  This routine initiate send response procedure on control channel which is
 *  identified by handle.
 *
 *  \param handle
 *         MCAP handle to identify the control channel
 *
 *  \param rsp_code
 *         MCAP response code generated for the event
 *
 *  \param rsp_params (UCHAR *)
 *         MCAP response parameters to be sent in the response
 *
 *  \return API_SUCCESS or an error code indicating reason for
 *          failure
 */
#define BT_mcap_md_create_rsp(handle,rsp_code,rsp_params,size)    \
        BT_mcap_common_api_handler                                \
        (                                                         \
            handle,                                               \
            MCAP_MD_CREATE_CNF,                                   \
            rsp_code,                                             \
            rsp_params,                                           \
            size                                                  \
        );

/**
 *  \brief To send the response for MD_CREATE request to remote device
 *
 *  \Description
 *  This routine initiate send response procedure on control channel which is
 *  identified by handle.
 *
 *  \param handle
 *         MCAP handle to identify the control channel
 *
 *  \param rsp_code
 *         MCAP response code generated for the event
 *
 *  \return API_SUCCESS or an error code indicating reason for
 *          failure
 */
#define BT_mcap_md_reconnect_rsp(handle,rsp_code)            \
        BT_mcap_common_api_handler                           \
        (                                                    \
            handle,                                          \
            MCAP_MD_RECONNECT_CNF,                           \
            rsp_code,                                        \
            NULL,                                            \
            0U                                               \
        );

/**
 *  \brief Routine to send the connect response for control channel connect req
 *
 *  \Description
 *  This routine is used to indicate to the MCAP whether the control channel
 *  connection is accepted or not. If accepted, the response code is
 *  API_SUCCESSS else it is an appropriate response code to indicate reason for
 *  rejection
 *
 *  \param handle
 *         Handle to uniquely identify the device from which the connection
 *         request is received
 *
 *  \param response_code (UINT16 *)
 *         Response code to indicate to MCAP whether the connection is to be
 *         accepted or not
 *
 *  \return API_SUCCESS on success, or a response code indicating the reason for
 *          rejection
 */
#define BT_mcap_mc_connect_rsp(handle,rsp_code)              \
        BT_mcap_common_api_handler                           \
        (                                                    \
            handle,                                          \
            MCAP_MC_CREATE_CNF,                              \
            rsp_code,                                        \
            NULL,                                            \
            0U                                               \
        );

#ifdef MCAP_CLOCK_SYNC

/**
 *  \brief To handle functionality common to MCAP Clock Sync commands
 *
 *  \Description This routine enqueues MCAP Clock Sync command request
 *               responses.
 *
 *  \param [in] handle
 *         Indentifies the MCAP control channel
 *
 *  \param [in] mcap_csp_cmd
 *         MCAP Clock Sync command to be processed
 *
 *  \param [in] rsp_code
 *         Response code in case it is a response
 *
 *  \param [in] cmd_params
 *         Command Parameters
 *
 *  \param [in] param_len
 *         Length of command parameters.
 *
 *  \return API_SUCCESS in case of success else an error code indicating reason
 *  for failure.
 */
API_RESULT BT_mcap_sync_common_api_handler
           (
               /* IN */ MCAP_HANDLE    * handle,
               /* IN */ UCHAR          mcap_csp_cmd,
               /* IN */ UINT16         rsp_code,
               /* IN */ UCHAR          * cmd_params,
               /* IN */ UINT8          param_len
           );

#define BT_mcap_sync_cap_req(handle,req_accuracy)            \
        BT_mcap_sync_common_api_handler                      \
        (                                                    \
            handle,                                          \
            MCAP_SYNC_CAP_REQ,                               \
            API_SUCCESS,                                     \
            req_accuracy,                                    \
            MCAP_SYNC_CAP_REQ_PARAM_LENGTH                   \
        );

#define BT_mcap_sync_cap_rsp(handle,rsp_code,rsp_params)     \
        BT_mcap_sync_common_api_handler                      \
        (                                                    \
            handle,                                          \
            MCAP_SYNC_CAP_RSP,                               \
            rsp_code,                                        \
            rsp_params,                                      \
            MCAP_SYNC_CAP_RSP_PARAM_LENGTH                   \
        );

#define BT_mcap_sync_set_req(handle,req_params)              \
        BT_mcap_sync_common_api_handler                      \
        (                                                    \
            handle,                                          \
            MCAP_SYNC_SET_REQ,                               \
            API_SUCCESS,                                     \
            req_params,                                      \
            MCAP_SYNC_SET_REQ_PARAM_LENGTH                   \
        );

#define BT_mcap_sync_set_rsp(handle,rsp_code,rsp_params)     \
        BT_mcap_sync_common_api_handler                      \
        (                                                    \
            handle,                                          \
            MCAP_SYNC_SET_RSP,                               \
            rsp_code,                                        \
            rsp_params,                                      \
            MCAP_SYNC_SET_RSP_PARAM_LENGTH                   \
        );


#define BT_mcap_sync_info_ind(handle,params)                 \
        BT_mcap_sync_common_api_handler                      \
        (                                                    \
            handle,                                          \
            MCAP_SYNC_INFO_IND,                              \
            API_SUCCESS,                                     \
            params,                                          \
            MCAP_SYNC_INFO_IND_PARAM_LENGTH                  \
        );

#endif /* MCAP_CLOCK_SYNC */

/**
 *  \brief To delete peer device information
 *
 *  \Description This routine deletes information about peer device.
 *  This interface can be used in the scenarios like deletion of bonding
 *  information of a device to also remove associated MCAP information (if any).
 *
 *  \param [in] device_handle
 *         Identifies the peer device for which the inforation to be deleted
 *
 *  \return API_SUCCESS in case of success else an error code indicating reason
 *  for failure.
 */
API_RESULT BT_mcap_delete_device
           (
               /* IN */ DEVICE_HANDLE  device_handle
           );

#ifdef MCAP_TEST
/* TODO: Add Comment */
API_RESULT mcap_send_invalid_command
           (
                MCAP_HANDLE    * handle,
                UCHAR          op_code,
                UCHAR          * params,
                UINT16         param_len
           );

void BT_mcap_earse_data_channels (void);
#endif /* MCAP_TEST */

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */

#endif /* _H_BT_MCAP_API_ */

