
/**
 *  \file BT_hdp_api.h
 *
 *  This file contains the data structures, type definitions, MACROS, consts
 *  definitions and API functions that HDP exposes for use to the upper layer
 *  applications
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_HDP_API_
#define _H_BT_HDP_API_

/* --------------------------------------------- Header File Inclusion */

#include "BT_mcap_api.h"

/* --------------------------------------------- Global Definitions */
/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
/**
 * \defgroup  hdp_module  HDP (Health Device Profile)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Health Device Profile module to the Application and other upper
 *  layers of the stack.
 */
/**
 * \defgroup  hdp_defines Defines
 * \{
 * Describes defines for HDP module.
 */
/**
 * \defgroup hdp_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/*
 * HDP event notification call back event types
 * Constant Definitions for HDP Notification Call back Event types
 */
/**
 * @name HDP event notification call back event types
 *
 * Constant Definitions for HDP Notification Call back Event types
 */
/*@{*/
#define HDP_MC_CREATE_IND              MCAP_MC_CREATE_IND
#define HDP_MC_CREATE_CNF              MCAP_MC_CREATE_CNF

#define HDP_MC_DISCONNECT_IND          MCAP_MC_DISCONNECT_IND
#define HDP_MC_DISCONNECT_CNF          MCAP_MC_DISCONNECT_CNF

#define HDP_MD_CREATE_IND              MCAP_MD_CREATE_IND
#define HDP_MD_CREATE_CNF              MCAP_MD_CREATE_CNF

#define HDP_MD_RECONNECT_IND           MCAP_MD_RECONNECT_IND
#define HDP_MD_RECONNECT_CNF           MCAP_MD_RECONNECT_CNF

#define HDP_MD_ABORT_IND               MCAP_MD_ABORT_IND
#define HDP_MD_ABORT_CNF               MCAP_MD_ABORT_CNF

#define HDP_MD_DELETE_IND              MCAP_MD_DELETE_IND
#define HDP_MD_DELETE_CNF              MCAP_MD_DELETE_CNF

#define HDP_MD_DELETE_ALL_IND          MCAP_MD_DELETE_ALL_IND
#define HDP_MD_DELETE_ALL_CNF          MCAP_MD_DELETE_ALL_CNF

#define HDP_MD_DISCONNECT_IND          MCAP_MD_DISCONNECT_IND
#define HDP_MD_DISCONNECT_CNF          MCAP_MD_DISCONNECT_CNF

#define HDP_DATA_IND                   MCAP_DATA_IND
#define HDP_DATA_WRITE_CNF             MCAP_DATA_WRITE_CNF

#define HDP_SYNC_CAP_REQ               MCAP_SYNC_CAP_REQ
#define HDP_SYNC_CAP_RSP               MCAP_SYNC_CAP_RSP
#define HDP_SYNC_SET_REQ               MCAP_SYNC_SET_REQ
#define HDP_SYNC_SET_RSP               MCAP_SYNC_SET_RSP
#define HDP_SYNC_INFO_IND              MCAP_SYNC_INFO_IND
/*@}*/

/* HDP error codes to be moved to BT_error.h */
#define HDP_ERR_ID                                0x2E00U
#define HDP_INVALID_MDEP                          MCAP_INVALID_MDEP
#define HDP_CONFIG_REJECTED                       MCAP_CONFIGURATION_REJECTED

/* HDP Data Channel Configuration Preferences */
/**
 * @name HDP Data Channel Configuration Preferences
 *
 * Constant Definitions for HDP Data Channel Configuration Preferences
 */
/*@{*/
#define HDP_NO_PREFERENCE                         0x00U
#define HDP_RELIABLE_CHNL                         MCAP_HDP_RELIABLE
#define HDP_STREAMING_CHNL                        MCAP_HDP_STREAMING
/*@}*/
/* HDP mcl_role */
/**
 * @name HDP mcl_role
 *
 * Constant Definitions for HDP mcl_role
 */
/*@{*/
#define HDP_SOURCE                                0x00U
#define HDP_SINK                                  0x01U
#define HDP_INVALID_ROLE                          0xFFU
/*@}*/
/** \} */
/* --------------------------------------------- Structures/Data Types */
/**
 * \defgroup hdp_strcutures Structures
 * \{
 * Describes Structures defined by the module.
 */
typedef MCAP_HANDLE  HDP_HANDLE ;

typedef MCAP_EVENT_NTF_CB  HDP_EVENT_NTF_CB;

typedef MCAP_L2CAP_CONFIG_CB  HDP_L2CAP_CONFIG_CB;

typedef MCAP_MEP  HDP_MEP;

typedef MCAP_MD_PARAMS  HDP_MD_PARAMS ;

typedef MCAP_MC_CONNECT_PARAMS  HDP_MC_CONNECT_PARAMS ;
/** \} */
/** \} */
/* --------------------------------------------- Macros */

/* HDP Utility macros */
/**
 * \defgroup hdp_utility_macros Utility Macros
 * \{
 * Describes Utility Macros defined by the module.
 */
/**
 * Utility macro to initialize MCAP Exchange endpoint, application may use it
 * before registering a MEP with HDP
 */

#define HDP_INIT_MEP(mep_info)                                \
        MCAP_INIT_MEP(mep_info)

/* Utility macro to set callback for MCAP Exchange endpoint */
#define HDP_SET_MEP_CALLBACK(mep_info,cb)                     \
        MCAP_SET_MEP_CALLBACK(mep_info,cb)

/* Utility macro to set control channel PSM for MCAP Exchange endpoint */
#define HDP_SET_MEP_CONTROL_CH_PSM(mep_info,psm)              \
        MCAP_SET_MEP_CONTROL_CH_PSM(mep_info,psm)

/* Utility macro to set data channel PSM for MCAP Exchange endpoint */
#define HDP_SET_MEP_DATA_CH_PSM(mep_info,psm)                 \
        MCAP_SET_MEP_DATA_CH_PSM(mep_info,psm)

/* Utility macro to set the config call back */
#define HDP_SET_CONFIG_CB(mep_id,cb)                          \
        MCAP_SET_CONFIG_CB(mep_id,cb)

/**
 * Utility macro to initialize a HDP Handle, applications may use it prior to
 * setting important elements of HDP Handle
 */
#define HDP_INIT_HANDLE(handle, rem_bd_addr)                  \
        MCAP_INIT_HANDLE(handle, rem_bd_addr)

/* Utility macro to set remote bluetooth device address in a HDP Handle */
#define HDP_SET_HANDLE_BD_ADDR(handle, rem_bd_addr)           \
        MCAP_SET_HANDLE_BD_ADDR(handle, rem_bd_addr)

/**
 * Utility macro to set the local MCAP Data Exchange Endpoint Identifier in a
 * HDP handle
 */
#define HDP_SET_HANDLE_MDEP(handle,mdep_id)                   \
        MCAP_SET_HANDLE_MDEP(handle,mdep_id)

/* Utility macro to get the remote bluetooth device address in a HDP Handle */
#define HDP_GET_HANDLE_BD_ADDR(handle,rem_bd_addr)            \
        MCAP_GET_HANDLE_BD_ADDR(handle,rem_bd_addr)

/**
 * Utility macro to get the local MCAP Data Exchange Endpoint Identifier of a
 * HDP handle
 */
#define HDP_GET_HANDLE_MDEP(handle,mdep_id)                   \
        MCAP_GET_HANDLE_MDEP(handle,mdep_id)

#define HDP_INIT_MD_PARAMS(params)                            \
        MCAP_INIT_MD_PARAMS(params)

#define HDP_SET_MD_PARAM_RMT_PSM(params,psm)                  \
        MCAP_SET_MD_PARAM_RMT_PSM(params,psm)

#define HDP_SET_MD_PARAMS_CH_TYPE(params,ch_type)             \
        MCAP_SET_MD_PARAMS_CH_TYPE(params,ch_type)

#define HDP_SET_MD_PARAMS_RMT_MDEP(params,mdep_id)            \
        MCAP_SET_MD_PARAMS_RMT_MDEP(params,mdep_id)

#ifdef MCAP_CLOCK_SYNC
#define HDP_CLOCK_SYNC
#endif /* MCAP_CLOCK_SYNC */

#ifdef HDP_CLOCK_SYNC

#define HDP_PACK_1_BYTE(dst,val)                              \
        MCAP_PACK_1_BYTE(dst,val)

#define HDP_PACK_2_BYTES(dst,val)                             \
        MCAP_PACK_2_BYTES(dst,val)

#define HDP_PACK_4_BYTES(dst,val)                             \
        MCAP_PACK_4_BYTES(dst,val)

#define HDP_PACK_8_BYTES(dst,val)                             \
        MCAP_PACK_8_BYTES(dst,val)

#define HDP_UNPACK_1_BYTE(dst,src)                            \
        MCAP_UNPACK_1_BYTE(dst,src)

#define HDP_UNPACK_2_BYTES(dst,src)                           \
        MCAP_UNPACK_2_BYTES(dst,src)

#define HDP_UNPACK_4_BYTES(dst,src)                           \
        MCAP_UNPACK_4_BYTES(dst,src)

#define HDP_UNPACK_8_BYTES(dst,src)                           \
        MCAP_UNPACK_8_BYTES(dst,src)

#define HDP_WRITE_TIMESTAMP_CLOCK_INSTANT(dst,val)            \
        MCAP_WRITE_TIMESTAMP_CLOCK_INSTANT(dst,val);

#define HDP_WRITE_TIMESTAMP_REQUIRED_ACCURACY(dst,val)        \
        MCAP_WRITE_TIMESTAMP_REQUIRED_ACCURACY(dst,val);

#define HDP_WRITE_BLUETOOTH_CLOCK_ACCESS_RESOLUTION(dst,val)  \
        MCAP_WRITE_BLUETOOTH_CLOCK_ACCESS_RESOLUTION(dst,val);

#define HDP_WRITE_SYNC_LEAD_TIME(dst,val)                     \
        MCAP_WRITE_SYNC_LEAD_TIME(dst,val);

#define HDP_WRITE_TIMESTAMP_NATIVE_RESOLUTION(dst,val)        \
        MCAP_WRITE_TIMESTAMP_NATIVE_RESOLUTION(dst,val);

#define HDP_WRITE_TIMESTAMP_NATIVE_ACCURACY(dst,val)          \
        MCAP_WRITE_TIMESTAMP_NATIVE_ACCURACY(dst,val);

#define HDP_WRITE_TIMESTAMP_UPDATE_INFORMATION(dst,val)       \
        MCAP_WRITE_TIMESTAMP_UPDATE_INFORMATION(dst,val);

#define HDP_WRITE_BLUETOOTH_CLOCK_SYNC_TIME(dst,val)          \
        MCAP_WRITE_BLUETOOTH_CLOCK_SYNC_TIME(dst,val);

#define HDP_WRITE_TIMESTAMP_SYNC_TIME(dst,val)                \
        MCAP_WRITE_TIMESTAMP_SYNC_TIME(dst,val);

#define HDP_WRITE_TIMESTAMP_SAMPLE_ACCURACY(dst,val)          \
        MCAP_WRITE_TIMESTAMP_SAMPLE_ACCURACY(dst,val);

#define BT_hdp_set_timestamp (handle,timestamp)               \
        BT_mcap_set_timestamp (handle,timestamp);

#define BT_hdp_get_timestamp (handle,timestamp)               \
        BT_mcap_get_timestamp (handle,timestamp);

#endif /* HDP_CLOCK_SYNC */
/** \} */
/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
/**
 * \defgroup hdp_api API Definitions
 * \{
 * Describes API definitions of this module.
 */
/**
 *  \brief To register Application End Point with HDP
 */
#define BT_hdp_register(mep_info,mep_id)                      \
        BT_mcap_register_mep(mep_info,mep_id);


/**
 *  \brief To initiate control channel connection with a remote device
 */
#define BT_hdp_mc_connect_req(mcap_handle,mc_params)          \
        BT_mcap_mc_connect_req(mcap_handle,mc_params);


/**
 *  \brief To disconnect control channel connection with a remote device
 */
#define BT_hdp_mc_disconnect_req(mcap_handle)                 \
        BT_mcap_mc_disconnect_req(mcap_handle);


/**
 *  \brief To create data channel associated with a remote device.
 */
#define BT_hdp_md_create_req(mcap_handle,md_params)           \
        BT_mcap_md_create_req(mcap_handle,md_params);


/**
 *  \brief To reconnect data channel associated with a remote device.
 */
#define BT_hdp_md_reconnect_req(mcap_handle,rem_data_ch_psm)  \
        BT_mcap_md_reconnect_req(mcap_handle,rem_data_ch_psm);


/**
 *  \brief To abort data channel associated with a remote device.
 */
#define BT_hdp_md_abort_req(mcap_handle)                      \
        BT_mcap_md_abort_req(mcap_handle);


/**
 *  \brief To disconnect data channel associated with a remote device.
 */
#define BT_hdp_md_disconnect_req(mcap_handle)                 \
        BT_mcap_md_disconnect_req(mcap_handle);


/**
 *  \brief To delete data channel associated with a remote device.
 */
#define BT_hdp_md_delete_req(mcap_handle)                     \
        BT_mcap_md_delete_req(mcap_handle);


/**
 *  \brief To delete all data channel chnls associated with a Control Chnl
 */
#define BT_hdp_md_delete_all_req(mcap_handle)                 \
        BT_mcap_md_delete_all_req(mcap_handle);


/**
 *  \brief To send data on the data channel associated with a remote device.
 */
#define BT_hdp_md_write(mcap_handle,data,data_len)            \
        BT_mcap_md_write(mcap_handle,data,data_len);


/**
 *  \brief To send the response to remote device data chnl create request
 */
#define BT_hdp_md_create_rsp(mcap_handle,rsp_code,rsp_params,rsp_len)  \
        BT_mcap_md_create_rsp(mcap_handle,rsp_code,rsp_params,rsp_len);


/**
 *  \brief To send the response to remote device for data chnl reconnect request
 */
#define BT_hdp_md_reconnect_rsp(mcap_handle,rsp_code)          \
        BT_mcap_md_reconnect_rsp(mcap_handle,rsp_code);


/**
 *  \brief Routine to send the response for control channel connect req
 */
#define BT_hdp_mc_connect_rsp(mcap_handle,rsp_code)            \
        BT_mcap_mc_connect_rsp(mcap_handle,rsp_code);


#ifdef HDP_CLOCK_SYNC

#define BT_hdp_sync_cap_req(handle,req_accuracy)               \
        BT_mcap_sync_cap_req(handle,req_accuracy);


#define BT_hdp_sync_cap_rsp(handle,rsp_code,rsp_params)        \
        BT_mcap_sync_cap_rsp(handle,rsp_code,rsp_params);


#define BT_hdp_sync_set_req(handle,req_params)                 \
        BT_mcap_sync_set_req(handle,req_params);


#define BT_hdp_sync_set_rsp(handle,rsp_code,rsp_params)        \
        BT_mcap_sync_set_rsp(handle,rsp_code,rsp_params);


#define BT_hdp_sync_info_ind(handle,params)                    \
        BT_mcap_sync_info_ind(handle,params);

#endif /* HDP_CLOCK_SYNC */
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_HDP_API_ */

