
/**
 *  \file l2cap.h
 *
 *  \brief This is Module Header File for L2CAP Layer.
 */

/*
 *  Copyright (C) 2014. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_L2CAP_
#define _H_L2CAP_

/* ---------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_device_queue.h"


/* ---------------------------------------------- Global Definitions */
/**
 * \addtogroup bt_protocol Protocols
 * \{
 */
/**
 * \defgroup l2cap_module L2CAP (Logical Link Control and Adaptation Protocol)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Logical Link Control and Adaptation Protocol Protocol (L2CAP) module to the
 *  Application and other upper layers of the stack.
 */

/**
 * \defgroup l2cap_defines Defines
 * \{
 * Describes defines for the module.
 */

/**
 * \defgroup l2cap_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/** L2CAP Header Sizes */
/** Basic L2CAP Header Length */
#define L2CAP_HDR_LEN                                   4U
#define L2CAP_GROUP_HDR_LEN                             6U

#if ((defined L2CAP_SUPPORT_CBFC_MODE) || (defined L2CAP_SUPPORT_ECBFC_MODE))
/** L2CAP SDU Header Length for LE Information Frame used in CBFC Mode */
#define L2CAP_CREDIT_MODE_SDU_HDR_LEN                   2U

/** L2CAP Header Length for LE Informaton Frame used in CBFC Mode */
#define L2CAP_CREDIT_MODE_HDR_LEN                       \
        (L2CAP_HDR_LEN + L2CAP_CREDIT_MODE_SDU_HDR_LEN)
#endif /* ((defined L2CAP_SUPPORT_CBFC_MODE) || (defined L2CAP_SUPPORT_ECBFC_MODE)) */

/* TODO: Update */
#ifdef L2CAP_SUPPORT_ECBFC_MODE
/** L2CAP SDU Header Length for LE Information Frame used in ECBFC Mode */
#define L2CAP_ECBFC_SDU_HDR_LEN                         2U

/** L2CAP Header Length for LE Informaton Frame used in ECBFC Mode */
#define L2CAP_ECBFC_HDR_LEN                       \
        (L2CAP_HDR_LEN + L2CAP_ECBFC_SDU_HDR_LEN)

/* L2CAP ECBFC Maximum Number of CIDs - Specification Allowed Value(s) */
#define L2CAP_ECBFC_MAX_NUM_CIDS                        5U
#endif /* L2CAP_SUPPORT_ECBFC_MODE */

/** L2CAP Connect Results */
#define L2CAP_CONNECTION_SUCCESSFUL                               0x0000U
#define L2CAP_CONNECTION_PENDING                                  0x0001U
#define L2CAP_CONNECTION_REFUSED_PSM_UNSUPPORTED                  0x0002U
#define L2CAP_CONNECTION_REFUSED_SECURITY_BLOCK                   0x0003U
#define L2CAP_CONNECTION_REFUSED_NO_RESOURCE                      0x0004U
#define L2CAP_CONNECTION_REFUSED_AUTHENTICATION_INSUFFICIENT      0x0005U
#define L2CAP_CONNECTION_REFUSED_AUTHORIZATION_INSUFFICIENT       0x0006U
#define L2CAP_CONNECTION_REFUSED_ENC_KEY_SIZE_INSUFFICIENT        0x0007U

/* Added with ECBFC Mode */
#define L2CAP_SOME_CONNECTION_REFUSED_INSUF_RESOURCE              0x0004U
#define L2CAP_CONNECTION_REFUSED_INSUFFICIENT_ENCRYPTION          0x0008U

#define L2CAP_CONNECTION_REFUSED_SOME_INVALID_SRC_CID             0x0009U
#define L2CAP_CONNECTION_REFUSED_SOME_SRC_CID_ALREADY_ALLOCATED   0x000AU
#define L2CAP_CONNECTION_REFUSED_UNACCEPTABLE_PARAMETERS          0x000BU
#define L2CAP_CONNECTION_REFUSED_INVALID_PARAMETERS               0x000CU
#define L2CAP_ALL_CONNECTIONS_PENDING_NO_FURTHER_INFO             0x000DU
#define L2CAP_ALL_CONNECTIONS_PENDING_AUTHENTICATION              0x000EU
#define L2CAP_ALL_CONNECTIONS_PENDING_AUTHORIZATION               0x000FU

/** L2CAP Connect Status */
#define L2CAP_CONNECTION_NO_FURTHER_INFORMATION         0x0000U
#define L2CAP_CONNECTION_AUTHENTICATION_PENDING         0x0001U
#define L2CAP_CONNECTION_AUTHORIZATION_PENDING          0x0002U

/** L2CAP Configuration Results */
#define L2CAP_CONFIG_SUCCESSFUL                         0x0000U
#define L2CAP_CONFIG_UNACCEPTABLE_PARAMETERS            0x0001U
#define L2CAP_CONFIG_REJECTED                           0x0002U
#define L2CAP_CONFIG_UNKNOWN_OPTION                     0x0003U
#define L2CAP_CONFIG_PENDING                            0x0004U
#define L2CAP_CONFIG_FLOW_SPEC_REJECTED                 0x0005U

/** L2CAP Information Request Results */
#define L2CAP_INFO_REQ_SUCCESSFUL                       0x0000U
#define L2CAP_INFO_REQ_NOT_SUPPORTED                    0x0001U

/** L2CAP ECBFC Reconfiguration Results */
#define L2CAP_ECBFC_RECONFIG_SUCCESSFUL                 0x0000U
#define L2CAP_ECBFC_RECONFIG_INVALID_MTU_SIZE           0x0001U
#define L2CAP_ECBFC_RECONFIG_INVALID_MPS_SIZE           0x0002U
#define L2CAP_ECBFC_RECONFIG_INVALID_CIDS               0x0003U
#define L2CAP_ECBFC_RECONFIG_UNACCEPTABLE_PARAMETERS    0x0004U

/** L2CAP Parameter Default Values */
#define L2CAP_MIN_MTU                                   48U

/*
 * L2CAP Minimum Signaling MTU for ACL-U.
 * - Not Supporting Extended Flow Specification: 48
 * - Supporting Extended Flow Specification    : 672
 */
#ifndef L2CAP_EXTENDED_FLOW_SPEC
#define L2CAP_SIG_MTU                                   L2CAP_MIN_MTU
#else
#define L2CAP_SIG_MTU                                   672U
#endif /* L2CAP_EXTENDED_FLOW_SPEC */

#define L2CAP_CONNECTIONLESS_MTU                        L2CAP_MIN_MTU

#ifdef BT_LE
/** L2CAP MTU for Fixed channel 0x0006 (used for SMP) */
#ifdef SMP_LESC
#define L2CAP_SMP_MTU                                   65U
#else /* SMP_LESC */
#define L2CAP_SMP_MTU                                   23U
#endif /* SMP_LESC */

/**
 * L2CAP MTU for Fixed channel 0x0005 (Used for LE specific
 * L2CAP commands)
 */
#define L2CAP_LE_MTU                                    23U
#endif /* BT_LE */

#ifdef L2CAP_SUPPORT_ECBFC_MODE
/** L2CAP MTU for Enhanced CBFC Mode. Signalling channel 0x0001 and 0x0005 */
#define L2CAP_MIN_ECBFC_MTU                             64U

/**
 * L2CAP MPS Min and Max values for Enhanced CBFC Mode.
 * Signalling channel 0x0001 and 0x0005.
 */
#define L2CAP_MIN_ECBFC_MPS                             64U
#define L2CAP_MAX_ECBFC_MPS                             65533U

#endif /* L2CAP_SUPPORT_ECBFC_MODE */

/**
 * L2CAP Service Types.
 * Used in Flow Specification Options and Extended Flow Specification Options.
 */
#define L2CAP_SERVICE_TYPE_NO_TRAFFIC                   0x00U
#define L2CAP_SERVICE_TYPE_BEST_EFFORT                  0x01U
#define L2CAP_SERVICE_TYPE_GUARANTEED                   0x02U

#define L2CAP_MTU_DEFAULT                               672U
#define L2CAP_FLUSH_TO_DEFAULT                          0xFFFFU
#define L2CAP_FLOW_SPEC_SERVICE_TYPE_DEFAULT            L2CAP_SERVICE_TYPE_BEST_EFFORT
#define L2CAP_FLOW_SPEC_TOKEN_RATE_DEFAULT              0x00000000U
#define L2CAP_FLOW_SPEC_TOKEN_BKT_SIZE_DEFAULT          0x00000000U
#define L2CAP_FLOW_SPEC_PEAK_BANDWIDTH_DEFAULT          0x00000000U
#define L2CAP_FLOW_SPEC_LATENCY_DEFAULT                 0xFFFFFFFFU
#define L2CAP_FLOW_SPEC_DELAY_VARIATION_DEFAULT         0xFFFFFFFFU

/* Extended Flow Specification (default values) */
#define L2CAP_EFS_IDENTIFIER_DEFAULT                    0x01U
#define L2CAP_EFS_SERVICE_TYPE_DEFAULT                  L2CAP_SERVICE_TYPE_BEST_EFFORT
#define L2CAP_EFS_MAX_SDU_SIZE_DEFAULT                  0xFFFFU
#define L2CAP_EFS_SDU_IA_TIME_DEFAULT                   0xFFFFFFFFU
#define L2CAP_EFS_ACCESS_LATENCY_DEFAULT                0xFFFFFFFFU
#define L2CAP_EFS_FLUSH_TO_DEFAULT                      0xFFFFFFFFU

#ifdef BT_ENH_L2CAP
/* L2CAP FCS Options in non-Basic mode */
#define L2CAP_FCS_OPTION_DEFAULT                        0x01U
#define L2CAP_FCS_OPTION_NO_FCS                         0x00U
#endif /* BT_ENH_L2CAP */

/* L2CAP Information Request Type Definitions */
#define L2CAP_INFO_REQ_CONNECTIONLESS_MTU               0x0001U
#define L2CAP_INFO_REQ_EXTENDED_FEATURES                0x0002U
#define L2CAP_INFO_REQ_FIXED_CHANNELS                   0x0003U

#ifdef BT_ENH_L2CAP
/** L2CAP Operation Modes */
#define L2CAP_MODE_BASIC                                0x00U
#define L2CAP_MODE_RTM                                  0x01U
#define L2CAP_MODE_FC                                   0x02U
#define L2CAP_MODE_ERTM                                 0x03U
#define L2CAP_MODE_SM                                   0x04U

/* TODO: Temporary: this should not be required long term */
#define L2CAP_MODE_MAX                         L2CAP_MODE_SM

/** L2CAP FEC Mode - Transmission Flow Control */
#define L2CAP_FEC_TX_FLOW_OFF                           0x01U
#define L2CAP_FEC_TX_FLOW_ON                            0x00U

/** L2CAP FEC Mode - Reception Flow Control */
#define L2CAP_FEC_RX_FLOW_OFF                           0x01U
#define L2CAP_FEC_RX_FLOW_ON                            0x00U
#endif /* BT_ENH_L2CAP */

#ifdef L2CAP_SUPPORT_CBFC_MODE
#define L2CAP_MODE_LE_CBFC                              0x05U
#endif /* L2CAP_SUPPORT_CBFC_MODE */

#ifdef L2CAP_SUPPORT_ECBFC_MODE
#define L2CAP_MODE_ECBFC                                0x06U
#endif /* L2CAP_SUPPORT_ECBFC_MODE */

/* TODO: Check if there is a better way to define invalid */
#define L2CAP_MODE_INVALID                              0xFFU

/** L2CAP ACL Transmission Queue Flow Control */
#define L2CAP_TX_QUEUE_FLOW_OFF                         0x01U
#define L2CAP_TX_QUEUE_FLOW_ON                          0x00U

#ifdef BT_PBF
/** L2CAP ACL Flushable or Non-Flushable Identifier */
#define L2CAP_USE_NON_FLUSHABLE_ACL_PACKET              0x00U
#define L2CAP_USE_FLUSHABLE_ACL_PACKET                  0x02U
#endif /* BT_PBF */

/** L2CAP Extended Feature Mask values */
#define L2CAP_FEATURE_FC                                0x00000001U
#define L2CAP_FEATURE_RTM                               0x00000002U
#define L2CAP_FEATURE_QOS                               0x00000004U
#define L2CAP_FEATURE_ERTM                              0x00000008U
#define L2CAP_FEATURE_SM                                0x00000010U
#define L2CAP_FEATURE_FCS                               0x00000020U
/* Extended Flow Specification for BR/EDR */
#define L2CAP_FEATURE_EFS_BR_EDR                        0x00000040U
/* Fixed Channels */
#define L2CAP_FEATURE_FIXED_CHANNELS                    0x00000080U
/* Extended Window Size */
#define L2CAP_FEATURE_EXTENDED_WINDOW_SIZE              0x00000100U
/* Unicast Connectionless Data Reception */
#define L2CAP_FEATURE_UCD                               0x00000200U

/* Enhanced Credit Based Flow Control Mode */
#define L2CAP_FEATURE_ECBFCM                            0x00000400U

#define L2CAP_FEATURE_NONE                              0x00000000U
#define L2CAP_FEATURE_ALL                               0x0000003FU

/** Enhanced L2CAP Configuration Mode Preference */
#define L2CAP_CONFIG_PREF_BASIC                         0x00U
#define L2CAP_CONFIG_PREF_ERTM_ONLY                     0x01U
#define L2CAP_CONFIG_PREF_ERTM_OPTIONAL                 0x02U
#define L2CAP_CONFIG_PREF_SM_ONLY                       0x03U
#define L2CAP_CONFIG_PREF_SM_OPTIONAL                   0x04U

/** L2CAP Fixed channel max and min Connection Identifiers (CID) */
#define L2CAP_MIN_FIXED_CHANNEL_CID             L2CAP_ATT_CID
#define L2CAP_MAX_LE_FIXED_CHANNEL_CID          L2CAP_SMP_CID
#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
#define L2CAP_MAX_FIXED_CHANNEL_CID             L2CAP_SMP_BREDR_CID
#else /* SMP_LESC_CROSS_TXP_KEY_GEN */
#define L2CAP_MAX_FIXED_CHANNEL_CID             L2CAP_SMP_CID
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

/** L2CAP Maximum number of fixed channels */
#define L2CAP_MAX_FIXED_CHANNELS                \
                L2CAP_MAX_FIXED_CHANNEL_CID -   \
                L2CAP_MIN_FIXED_CHANNEL_CID + 1U

#define L2CAP_MAX_LE_FIXED_CHANNELS                \
                L2CAP_MAX_LE_FIXED_CHANNEL_CID -   \
                L2CAP_MIN_FIXED_CHANNEL_CID + 1U

#if ((defined L2CAP_SUPPORT_CBFC_MODE) || (defined L2CAP_SUPPORT_ECBFC_MODE))
/** Maximum number of credits allowed for each L2CAP CBFC channel */
#define L2CAP_MAX_CREDITS                               0xFFFFU
#endif /* L2CAP_SUPPORT_CBFC_MODE || L2CAP_SUPPORT_ECBFC_MODE */

/* LMP Link types */
/** BR/EDR LMP Link type */
#define L2CAP_LINK_TYPE_BR_EDR                  0x01U
/** LE LMP Link type */
#define L2CAP_LINK_TYPE_LE                      0x03U

/* Response codes for Connection parameter update request */
/** Connection Parameters accepted */
#define L2CAP_CONNECTION_PARAMETERS_ACCEPTED   0x0000U

/** Connection Parameters rejected */
#define L2CAP_CONNECTION_PARAMETERS_REJECTED   0x0001U

/* Response Codes for Invalid Command Packets */
/** Command Not Understood */
#define L2CAP_COMMAND_NOT_UNDERSTOOD           0x0000U

/** Signaling MTU exceeded */
#define L2CAP_SIGNALLING_MTU_EXCEEDED          0x0001U

/** Invalid Connection Identifier in request */
#define L2CAP_INVALID_CID_IN_REQUEST           0x0002U

#ifdef BT_L2CAP_FIXED_CHANNEL

/* L2CAP Fixed Channel Events */
/**
 * This event indicates L2CAP Fixed Channel connection from remote device.
 * Fixed Channel Event handler registered with L2CAP for specific fixed channel
 * will receive this event, when the underlying ACL link is established,
 * as there is no separate connection procedure for L2CAP Fixed Channel.
 */
#define L2CAP_FIXED_CH_CONNECTED_EVENT         0x01U

/**
 * This event indicates L2CAP Fixed Channel disconnection from remote device.
 * Fixed Channel Event handler registered with L2CAP for specific fixed channel
 * will receive this event, when the underlying ACL link is disconnected,
 * as there is no separate disconnection procedure for L2CAP Fixed Channel.
 */
#define L2CAP_FIXED_CH_DISCONNECTED_EVENT      0x02U

/**
 * This event indicates data received on L2CAP Fixed Channel from remote device.
 * Fixed Channel Event handler registered with L2CAP for specific fixed channel
 * will receive this event.
 */
#define L2CAP_FIXED_CH_DATA_RX_EVENT           0x03U

/**
 * This event indicates data transmitted to L2CAP Fixed Channel from remote device.
 * Fixed Channel Event handler registered with L2CAP for specific fixed channel
 * will receive this event.
 */
#define L2CAP_FIXED_CH_DATA_TX_EVENT           0x04U

#ifdef BT_L2CAP_FIXED_CHANNEL_TX_COMPLETE
#define L2CAP_FIXED_CH_MAX_EVENT           L2CAP_FIXED_CH_DATA_TX_EVENT
#else
#define L2CAP_FIXED_CH_MAX_EVENT           L2CAP_FIXED_CH_DATA_RX_EVENT
#endif /* BT_L2CAP_FIXED_CHANNEL_TX_COMPLETE */
#endif /* BT_L2CAP_FIXED_CHANNEL */

#ifdef BT_LE

/* L2CAP LE events */
/**
 * This event indicates the connection parameter update received from remote
 * device. The application expected to reply to L2CAP using
 * \ref BT_l2cap_le_connection_param_update_response API, to resond to remote
 * device whether parameters are accepted.
 *
 * If the connection parameter update request is accepted then application is
 * expected to perform physical link connection parameter update using
 * \ref BT_hci_le_connection_update.
 */
#define L2CAP_CONNECTION_UPDATE_REQUEST_EVENT  0x01U

/**
 * This event indicates the connection parameter update request response
 * received. The result field indicates the reason for rejection. The possible
 * responses are \ref L2CAP_CONNECTION_PARAMETERS_ACCEPTED and \ref
 * L2CAP_CONNECTION_PARAMETERS_REJECTED.
 */
#define L2CAP_CONNECTION_UPDATE_RESPONSE_EVENT 0x02U

/**
 * This event indicates the connection parameter update request is rejected.
 * The reason field indicates the reason for rejection.
 * Possible reasons for rejection are
 * - \ref L2CAP_COMMAND_NOT_UNDERSTOOD,
 * - \ref L2CAP_SIGNALLING_MTU_EXCEEDED
 * - \ref L2CAP_INVALID_CID_IN_REQUEST
 */
#define L2CAP_COMMAND_REJECTED_EVENT           0x03U

/* L2CAP connection update error code */
#define L2CAP_CONNECTION_PARAM_REJECTED        0x0001U
#endif /* BT_LE */
/** \} */
/** \} */


/* ---------------------------------------------- Structures/Data Types */
/**
 *  \addtogroup l2cap_defines Defines
 *  \{
 */
/**
 * \defgroup l2cap_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
/**
 *   The Data Type representing the L2CAP Flow Specification
 */
typedef struct
{
    /** Service Type - Guaranteed or Best Effort */
    UCHAR service_type;

    /** Token Rate */
    UINT32 token_rate;

    /** Token Bucket Size */
    UINT32 token_bucket_size;

    /** Peak Bandwidth */
    UINT32 peak_bandwidth;

    /** Latency */
    UINT32 latency;

    /** Delay Variation */
    UINT32 delay_variation;

} FLOW_SPEC, L2CAP_FLOW_SPEC;


#ifdef L2CAP_EXTENDED_FLOW_SPEC
/**
 *   The Data Type representing the L2CAP Extended Flow Specification
 */
typedef struct
{
    /** Identifier */
    UCHAR id;

    /** Service Type - Guaranteed or Best Effort */
    UCHAR service_type;

    /** Maximum SDU Size */
    UINT16 max_sdu_size;

    /** SDU Inter-arrival Time */
    UINT32 sdu_ia_time;

    /** Access Latency */
    UINT32 access_latency;

    /** Flush Timeout */
    UINT32 flush_to;

} L2CAP_EXT_FLOW_SPEC;
#endif /* L2CAP_EXTENDED_FLOW_SPEC */


#ifdef BT_ENH_L2CAP

/**
 *  The Data Type representing L2CAP FEC Options
 */
typedef struct
{
    /** Value in milliseconds of the retransmission time-out */
    UINT16   retx_timeout;

    /** Value in milliseconds of the retransmission time-out */
    UINT16   monitor_timeout;

    /**
     *  Maximum size of paylod data L2CAP entity is
     *  capable of accepting
     */
    UINT16   mps;

    /**
     *  Maximum number of retransmissions of a single
     *  I-frame in retransmission mode
     */
    UCHAR    max_transmit;

    /** Size of Transmission WIndow for FEC mode */
    UCHAR    tx_window;

    /** Mode of the Link */
    UCHAR    mode;

} L2CAP_FEC_OPTION;

#endif /* BT_ENH_L2CAP */


/**
 *  The Data Type representing L2CAP Configuration Options
 */
typedef struct
{
    /** Flow Specification */
    FLOW_SPEC *           flow_spec;

#ifdef L2CAP_EXTENDED_FLOW_SPEC
    /** Extended Flow Specification */
    L2CAP_EXT_FLOW_SPEC * ext_flow_spec;
#endif /* L2CAP_EXTENDED_FLOW_SPEC */

#ifdef BT_ENH_L2CAP
    /** FEC Options */
    L2CAP_FEC_OPTION *    fec;
#endif /* BT_ENH_L2CAP */

    /** L2CAP MTU - Maximum Transmission Unit */
    UINT16                mtu;

    /** Flush Timeout */
    UINT16                flush_to;

#ifdef BT_ENH_L2CAP
    /** FCS Options */
    UCHAR                 fcs;
#endif /* BT_ENH_L2CAP */

} L2CAP_CONFIG_OPTION;

/**
 *  The structure representing the PSMs. It stores all the callback
 *  functions. The upper layer uses this structure to register itself
 *  with L2CAP.
 */
typedef struct
{
    /** Callback for l2ca_connect_ind (Device Queue Handle, CID, PSM) */
    API_RESULT (* l2ca_connect_ind_cb)(DEVICE_HANDLE *handle, UINT16 lcid, UINT16 psm);

    /** Callback for l2ca_connect_cnf (Device Queue Handle, CID, Result, Status) */
    API_RESULT (* l2ca_connect_cnf_cb)(DEVICE_HANDLE *handle, UINT16 lcid, UINT16 result, UINT16 status);

    /** Callback for l2ca_config_ind (CID, L2CAP_CONFIG_OPTION) */
    API_RESULT (* l2ca_config_ind_cb)(UINT16 lcid, L2CAP_CONFIG_OPTION *config_option);

    /** Callback for l2ca_config_cnf (CID, Result, L2CAP_CONFIG_OPTION) */
    API_RESULT (* l2ca_config_cnf_cb)(UINT16 lcid, UINT16 result, L2CAP_CONFIG_OPTION *config_option);

    /** Callback for l2ca_disconnect_ind (CID) */
    API_RESULT (* l2ca_disconnect_ind_cb)(UINT16 lcid);

    /** Callback for l2ca_disconnect_cnf (CID, Result) */
    API_RESULT (* l2ca_disconnect_cnf_cb)(UINT16 lcid, UINT16 result);

    /** Callback function for l2ca_data_read (CID, Buffer, Length) */
    API_RESULT (* l2ca_data_read_cb)(UINT16 lcid, UCHAR *data, UINT16 datalen);

#ifdef L2CAP_CHANNEL_DATAWRITE_CALLBACK
    /** Callback function for l2ca_data_write (CID, Buffer, Length) */
    API_RESULT(*l2ca_data_write_cb)(UINT16 lcid, UCHAR *data, UINT16 datalen);
#endif /* L2CAP_CHANNEL_DATAWRITE_CALLBACK */

    /** Callback for l2ca_qos_violation_ind (Device Queue Handle) */
    API_RESULT (* l2ca_qos_violation_ind_cb)(DEVICE_HANDLE * handle);

#ifdef BT_ENH_L2CAP
    /** Callback for l2ca_tx_flow_ind (CID, Flow On/Off) */
    API_RESULT (* l2ca_tx_flow_ind_cb)(UINT16 lcid, UCHAR flow_ctrl);

    /** Callback for l2ca_get_fec_params (CID, FEC) */
    API_RESULT (* l2ca_get_fec_params_cb) (UINT16 lcid, L2CAP_FEC_OPTION *fec_option);

    /* Bitmap to indicate support for L2CAP extended features */
    UINT32 feature_mask;
#endif /* BT_ENH_L2CAP */

    /** The PSM value for the Protocol */
    UINT16 psm;

#ifdef L2CAP_HAVE_GROUP_SUPPORT
    /**
     *  Boolean Flag to indicate whether Connectionless Traffic
     *  is enabled for this PSM.
     */
    UCHAR clt;
#endif /* L2CAP_HAVE_GROUP_SUPPORT */

} PSM_STRUCT, L2CAP_PSM;

/** Common Callback structure for Ping and Info */
typedef struct
{
#ifdef L2CAP_HAVE_PING_INFO_SUPPORT

    /** Callback for Ping, called on reception of L2CAP ECHO RSP */
    API_RESULT (* l2ca_ping_cnf)(DEVICE_HANDLE *handle, UINT16 result, UCHAR *data, UINT16 datalen);
    API_RESULT (* l2ca_getinfo_cnf)(DEVICE_HANDLE *handle, UINT16 result, UCHAR *info_data, UINT16 info_datalen);
#endif /* L2CAP_HAVE_PING_INFO_SUPPORT */

#ifdef L2CAP_TX_COMPLETE_CALLBACK
    /** Callback for TX, called on transmission of L2CAP PDU */
    API_RESULT (* l2ca_tx_cnf)(DEVICE_HANDLE *handle);
#endif /* L2CAP_TX_COMPLETE_CALLBACK */

#if (!(defined L2CAP_HAVE_PING_INFO_SUPPORT) && !(defined L2CAP_TX_COMPLETE_CALLBACK))

   /**
    *  If both L2CAP_HAVE_PING_INFO_SUPPORT and L2CAP_TX_COMPLETE_CALLBACK
    *  are not defined compiler will generate warnings due to the empty srtucure.
    * To avoid this a dummy variable is declared, which serves no purpose.
    */
   UCHAR dummy;

#endif /* !L2CAP_HAVE_PING_INFO_SUPPORT && !L2CAP_TX_COMPLETE_CALLBACK */

} L2CAP_COMMON_CB;

#ifdef BT_UCD
/**
 *  The structure representing L2CAP UCD. It stores the callback
 *  functions. The upper layer uses this structure to register itself
 *  with L2CAP for UCD.
 */
typedef struct
{
    /**
     * Callback to inform UL/application of Unicast Connectionless data reception.
     *
     * l2ca_ucd_data_read_cb provides following parameters to the application:
     *     1. Pointer to Device Queue Handle: the peer device sending the request.
     *     2. data: Data Buffer Pointer containing receiving UCD payload
     *     3. datalen: Received UCD payload length
     */
    API_RESULT (* l2ca_ucd_data_cb)(DEVICE_HANDLE *handle, UCHAR *data, UINT16 datalen);

} L2CAP_UCD_STRUCT;
#endif /* BT_UCD */

#ifdef L2CAP_SUPPORT_CBFC_MODE
/**
 *  The structure representing the Connection Parameter for
 *  L2CAP Credit Based Flow Control Mode.
 */
typedef struct l2cap_cbfc_connect_param
{
    /**
     * MTU - Maximum SDU Size
     *
     * The MTU field specifies the maximum SDU size (in octets) that the L2CAP
     * layer entity sending the LE Credit Based Connection Request can receive
     * on this channel. L2CAP implementations shall support a minimum MTU size
     * of 23 octets.
     */
    UINT16   mtu;

    /**
     * MPS - Maximum PDU Size
     *
     * The MPS field specifies the maximum payload size (in octets) that the
     * L2CAP layer entity sending the LE Credit Based Connection Request is
     * capable of receiving on this channel. L2CAP implementations shall support
     * a minimum MPS of 23 octets and may support an MPS up to 65533 octets.
     */
    UINT16   mps;

    /**
     * Initial number of Credits
     *
     * The initial credit value indicates the number of LE-frames that the peer
     * device can send to the L2CAP layer entity sending the LE Credit Based
     * Connection Request. The initial credit value shall be in the range of 0 to
     * 65535.
     */
    UINT16   credit;

#ifdef L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT
    /**
     * Local/Remote Channel Identifier
     *
     * - Local CID when l2ca_cbfc_connect_req() returns
     * - Remote CID when l2ca_connect_ind_cb()/l2ca_connect_cnf_cb()
     *   for CBFC PSM is invoked
     */
    UINT16   cid;
#endif /* L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT */

}L2CAP_CBFC_CONNECT_PARAM;

/**
 *  The structure representing the PSMs. It stores all the callback
 *  functions. The upper layer uses this structure to register itself
 *  with L2CAP.
 */
typedef struct psm_struct_cbfc
{
    /**
     * Callback to inform application of the incoming L2CAP CBFC Connection Request.
     *
     * l2ca_connect_ind_cb provides following parameters to the application:
     *     1. Pointer to Device Queue Handle: the peer device sending the request.
     *     2. CID: Channel Identifier.
     *     3. PSM: LE Protocol/Service Multiplexer.
     *     4. CBFC connect Request Parameters (MTU, MPS and Initial Credits)
     */
    API_RESULT (* l2ca_connect_ind_cb)(DEVICE_HANDLE *handle, UINT16 lcid, UINT16 psm, L2CAP_CBFC_CONNECT_PARAM *param);

    /**
     * Callback to inform application of the L2CAP CBFC Connection Response/Confirmation.
     *
     * l2ca_connect_cnf_cb provides following parameters to the application:
     *     1. Pointer to Device Queue Handle: the peer device sending the request.
     *     2. CID: Channel Identifier.
     *     3. Result: A result value of 0x0000 indicates success while a non-zero value
     *                indicates the connection request was refused.
     *     4. CBFC connect Response Parameters (MTU, MPS and Initial Credits)
     */
    API_RESULT (* l2ca_connect_cnf_cb)(DEVICE_HANDLE *handle, UINT16 lcid, UINT16 result, L2CAP_CBFC_CONNECT_PARAM *param);

    /**
     * Callback to inform application of the L2CAP CBFC Disconnection Request
     * received from the Peer device. This callback is for the responder of
     * the L2CAP Disconnection request.
     *
     * l2ca_disconnect_ind_cb provides following parameters to the application:
     *     1. CID: Channel Identifier of a previously connected channel.
     *
     * Note: On receiving this callback, previously connected channel for which
     * disconnection is initiated by the peer device will be considered closed.
     */
    API_RESULT (* l2ca_disconnect_ind_cb)(UINT16 lcid);

    /**
     * Callback to inform application of the L2CAP CBFC Disconnection
     * confirmation/Response received from the Peer device. This callback is
     * for the initiator of the L2CAP Disconnection request.
     *
     * l2ca_disconnect_cnf_cb provides following parameters to the application:
     *     1. CID: Channel Identifier of a previously connected channel
     *             for which disconnection was initiated by local device.
     *     2. Result: A result value of 0x0000 indicates success while a
     *                non-zero value indicates the disconnection request
     *                has failed (timed out).
     *
     * Note: Irrespective of the 'Result' is success/failure, once the local
     * device has initiated L2CAP Disconnection, the channel will be
     * considered closed.
     */
    API_RESULT (* l2ca_disconnect_cnf_cb)(UINT16 lcid, UINT16 result);

    /**
     * Callback to inform application of data received over L2CAP CBFC channel.
     *
     * l2ca_data_read_cb provides following parameters to the application:
     *     1. CID: Channel Identifier of a previously connected channel.
     *     2. Result: A result value of 0x0000 indicates success, while a non-zero value
     *                indicates an error condition
     *                (e.g. peer device violating credit flow, or MTU size limit)
     *     3. Buffer Pointer: Data/payload received from the peer device.
     *     4. Buffer Length: Receieved data/payload length.
     */
    API_RESULT (* l2ca_data_read_cb)(UINT16 lcid, UINT16 result, UCHAR *data, UINT16 datalen);

    /**
     * Callback to inform application of receive credits reached low water mark or below.
     * After receiving L2CAP data/payload from peer device for a specification Channel,
     * the available credits are calculated. If the credit count reaches or goes below
     * the low water mark, this callback is called to inform application of the condition,
     * so that if application wants can send more credits to the peer device.
     *
     * l2ca_rx_credit_ind provides following parameters :
     *     1. CID: Channel Identifier of a previously connected channel.
     *     2. Receive Credits: The current received credits count.
     */
    API_RESULT (* l2ca_rx_credit_ind) (UINT16 lcid, UINT16 credit);

    /**
     * Callback to inform application of having received transmit credits.
     * This callback is called on receiving LE Flow Control Credit from peer device.
     *
     * l2ca_tx_credit_ind provides following parameters to the application:
     *     1. CID: Channel Identifier of a previously connected channel.
     *     2. Result: A result value of 0x0000 indicates success, while a non-zero value
     *                indicates an error condition (e.g. credit overflow, if total number of credits
     *                crosses specification defined maximum limit of 0xFFFF)
     *     3. Transmit Credits: The cumulative transmit credit count. This is the number of LE-frames
     *                          that can be sent to the peer device sending the LE Flow Control
     *                          Credit packet.
     */
    API_RESULT (* l2ca_tx_credit_ind) (UINT16 lcid, UINT16 result, UINT16 credit);

    /** The LE Protocol/Service Multiplexer */
    UINT16 psm;

#ifdef L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM
    /** Upper Layer defined Receive Credit Low Water Mark */
    UINT16 credit_lwm;
#endif /* L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM */

    /**
     * Callback to inform application of data transmission completion over L2CAP CBFC channel.
     *
     * l2ca_data_write_cb provides following parameters to the application:
     *     1. CID: Channel Identifier of a previously connected channel.
     *     2. Result: A result value of 0x0000 indicates success, while a non-zero value
     *                indicates an error condition
     *                (e.g. peer device violating credit flow, or MTU size limit)
     *     3. Buffer Pointer: Currently NULL. For future usage.
     *     4. Buffer Length: Currently 0. For future usage.
     */
    API_RESULT (* l2ca_data_write_cb)(UINT16 lcid, UINT16 result, UCHAR *data, UINT16 datalen);

} PSM_STRUCT_CBFC, L2CAP_PSM_CBFC;

#endif /* L2CAP_SUPPORT_CBFC_MODE */

#ifdef L2CAP_SUPPORT_ECBFC_MODE
/**
 *  The structure representing the Connection Parameter for
 *  L2CAP Ehnaced Credit Based Flow Control Mode.
 */
typedef struct l2cap_ecbfc_connect_param
{
    /**
     * MTU - Maximum SDU Size
     *
     * The MTU field specifies the maximum SDU size (in octets) that the L2CAP
     * layer entity sending the Enhanced Credit Based Connection Request can receive
     * on this channel. L2CAP implementations shall support a minimum MTU size
     * of 23 octets.
     */
    UINT16   mtu;

    /**
     * MPS - Maximum PDU Size
     *
     * The MPS field specifies the maximum payload size (in octets) that the
     * L2CAP layer entity sending the Enhanced Credit Based Connection Request is
     * capable of receiving on this channel. L2CAP implementations shall support
     * a minimum MPS of 23 octets and may support an MPS up to 65533 octets.
     */
    UINT16   mps;

    /**
     * Initial number of Credits
     *
     * The initial credit value indicates the number of LE-frames that the peer
     * device can send to the L2CAP layer entity sending the Enhanced Credit Based
     * Connection Request. The initial credit value shall be in the range of 0 to
     * 65535.
     */
    UINT16   credit;

    /**
     * Source or Destination CID
     * - based on if used with connection request or response
     *
     * The Source CID is an array of 5 two octet values and represents the channel
     * endpoints on the device sending the request.
     *
     * The Destination CID is an array of 5 two octet values and represents
     * the channel endpoints on the device sending the response.
     */
    UINT16   cid[L2CAP_ECBFC_MAX_NUM_CIDS];

    /** Number of L2CAP Channels to be connected or requested for connection */
    UINT8    num_cids;

}L2CAP_ECBFC_CONNECT_PARAM;

/**
 *  The structure representing the Reconfiguration Parameter for
 *  L2CAP Ehnaced Credit Based Flow Control Mode.
 */
typedef struct l2cap_ecbfc_reconfig_param
{
    /**
     * MTU - Maximum SDU Size
     *
     * The MTU field specifies the maximum SDU size (in octets) that the L2CAP
     * layer entity sending the Enhanced Credit Based Connection Request can receive
     * on this channel. L2CAP implementations shall support a minimum MTU size
     * of 23 octets.
     */
    UINT16   mtu;

    /**
     * MPS - Maximum PDU Size
     *
     * The MPS field specifies the maximum payload size (in octets) that the
     * L2CAP layer entity sending the Enhanced Credit Based Connection Request is
     * capable of receiving on this channel. L2CAP implementations shall support
     * a minimum MPS of 23 octets and may support an MPS up to 65533 octets.
     */
    UINT16   mps;

    /* TODO: Below comment to be updated */
    /**
     * Source or Destination CID
     * - based on if used with connection request or response
     *
     * The Source CID is an array of 5 two octet values and represents the channel
     * endpoints on the device sending the request.
     *
     * The Destination CID is an array of 5 two octet values and represents
     * the channel endpoints on the device sending the response.
     */
    UINT16   cid[L2CAP_ECBFC_MAX_NUM_CIDS];

    /** Number of L2CAP Channels to be reconfigured */
    UINT8    num_cids;

}L2CAP_ECBFC_RECONFIG_PARAM;

/**
 *  The structure representing the PSMs. It stores all the callback
 *  functions. The upper layer uses this structure to register itself
 *  with L2CAP.
 */
typedef struct psm_struct_ecbfc
{
    /**
     * Callback to inform application of the incoming L2CAP CBFC Connection Request.
     *
     * l2ca_connect_ind_cb provides following parameters to the application:
     *     1. Pointer to Device Queue Handle: the peer device sending the request.
     *     2. CID: List of Channel Identifiers.
     *     3. PSM: LE Protocol/Service Multiplexer.
     *     4. CBFC connect Request Parameters (MTU, MPS and Initial Credits)
     */
    API_RESULT (* l2ca_connect_ind_cb)(DEVICE_HANDLE * handle, UINT16 * lcid, UINT16 psm, L2CAP_ECBFC_CONNECT_PARAM * param);

    /**
     * Callback to inform application of the L2CAP CBFC Connection Response/Confirmation.
     *
     * l2ca_connect_cnf_cb provides following parameters to the application:
     *     1. Pointer to Device Queue Handle: the peer device sending the request.
     *     2. CID: Channel Identifier.
     *     3. Result: A result value of 0x0000 indicates success while a non-zero value
     *                indicates the connection request was refused.
     *     4. CBFC connect Response Parameters (MTU, MPS and Initial Credits)
     */
    API_RESULT (* l2ca_connect_cnf_cb)(DEVICE_HANDLE * handle, UINT16 * lcid, UINT16 result, L2CAP_ECBFC_CONNECT_PARAM * param);

    /**
     * Callback to inform application of the incoming L2CAP ECBFC Reconfiguration Request.
     *
     * l2ca_reconfig_ind_cb provides following parameters to the application:
     *     1. Pointer to Device Queue Handle: the peer device sending the request.
     *     2. CID: List of Channel Identifiers.
     *     3. ECBFC Reconfiguration Request Parameters (MTU, MPS and Num of CIDs in Reconfig Request)
     */
    API_RESULT(* l2ca_reconfig_ind_cb)(DEVICE_HANDLE * handle, UINT16 * lcid, L2CAP_ECBFC_RECONFIG_PARAM * param);

    /**
     * Callback to inform application of the L2CAP ECBFC Reconfig Response/Confirmation.
     *
     * l2ca_reconfig_cnf_cb provides following parameters to the application:
     *     1. Pointer to Device Queue Handle: the peer device sending the response.
     */
    API_RESULT(* l2ca_reconfig_cnf_cb)(DEVICE_HANDLE * handle);

    /**
     * Callback to inform application of the L2CAP CBFC Disconnection Request
     * received from the Peer device. This callback is for the responder of
     * the L2CAP Disconnection request.
     *
     * l2ca_disconnect_ind_cb provides following parameters to the application:
     *     1. CID: Channel Identifier of a previously connected channel.
     *
     * Note: On receiving this callback, previously connected channel for which
     * disconnection is initiated by the peer device will be considered closed.
     */
    API_RESULT (* l2ca_disconnect_ind_cb)(UINT16 lcid);

    /**
     * Callback to inform application of the L2CAP CBFC Disconnection
     * confirmation/Response received from the Peer device. This callback is
     * for the initiator of the L2CAP Disconnection request.
     *
     * l2ca_disconnect_cnf_cb provides following parameters to the application:
     *     1. CID: Channel Identifier of a previously connected channel
     *             for which disconnection was initiated by local device.
     *     2. Result: A result value of 0x0000 indicates success while a
     *                non-zero value indicates the disconnection request
     *                has failed (timed out).
     *
     * Note: Irrespective of the 'Result' is success/failure, once the local
     * device has initiated L2CAP Disconnection, the channel will be
     * considered closed.
     */
    API_RESULT (* l2ca_disconnect_cnf_cb)(UINT16 lcid, UINT16 result);

    /**
     * Callback to inform application of data received over L2CAP CBFC channel.
     *
     * l2ca_data_read_cb provides following parameters to the application:
     *     1. CID: Channel Identifier of a previously connected channel.
     *     2. Result: A result value of 0x0000 indicates success, while a non-zero value
     *                indicates an error condition
     *                (e.g. peer device violating credit flow, or MTU size limit)
     *     3. Buffer Pointer: Data/payload received from the peer device.
     *     4. Buffer Length: Receieved data/payload length.
     */
    API_RESULT (* l2ca_data_read_cb)(UINT16 lcid, UINT16 result, UCHAR * data, UINT16 datalen);

    /**
     * Callback to inform application of receive credits reached low water mark or below.
     * After receiving L2CAP data/payload from peer device for a specification Channel,
     * the available credits are calculated. If the credit count reaches or goes below
     * the low water mark, this callback is called to inform application of the condition,
     * so that if application wants can send more credits to the peer device.
     *
     * l2ca_rx_credit_ind provides following parameters :
     *     1. CID: Channel Identifier of a previously connected channel.
     *     2. Receive Credits: The current received credits count.
     */
    API_RESULT (* l2ca_rx_credit_ind) (UINT16 lcid, UINT16 credits);

    /**
     * Callback to inform application of having received transmit credits.
     * This callback is called on receiving LE Flow Control Credit from peer device.
     *
     * l2ca_tx_credit_ind provides following parameters to the application:
     *     1. CID: Channel Identifier of a previously connected channel.
     *     2. Result: A result value of 0x0000 indicates success, while a non-zero value
     *                indicates an error condition (e.g. credit overflow, if total number of credits
     *                crosses specification defined maximum limit of 0xFFFF)
     *     3. Transmit Credits: The cumulative transmit credit count. This is the number of LE-frames
     *                          that can be sent to the peer device sending the LE Flow Control
     *                          Credit packet.
     */
    API_RESULT (* l2ca_tx_credit_ind) (UINT16 lcid, UINT16 result, UINT16 credits);

    /** The LE Protocol/Service Multiplexer */
    UINT16 psm;

#ifdef L2CAP_ECBFC_SUPPORT_UL_CREDIT_LWM
    /** Upper Layer defined Receive Credit Low Water Mark */
    UINT16 credit_lwm;
#endif /* L2CAP_ECBFC_SUPPORT_UL_CREDIT_LWM */

    /**
     * Callback to inform application of data transmission completion over L2CAP CBFC channel.
     *
     * l2ca_data_write_cb provides following parameters to the application:
     *     1. CID: Channel Identifier of a previously connected channel.
     *     2. Result: A result value of 0x0000 indicates success, while a non-zero value
     *                indicates an error condition
     *                (e.g. peer device violating credit flow, or MTU size limit)
     *     3. Buffer Pointer: Currently NULL. For future usage.
     *     4. Buffer Length: Currently 0. For future usage.
     */
    API_RESULT (* l2ca_data_write_cb)(UINT16 lcid, UINT16 result, UCHAR * data, UINT16 datalen);

} PSM_STRUCT_ECBFC, L2CAP_PSM_ECBFC;

#endif /* L2CAP_SUPPORT_ECBFC_MODE */

/** \} */
/** \} */

/**
 *  \defgroup le_l2cap_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */
#ifdef BT_L2CAP_FIXED_CHANNEL
/**
 *  L2CAP Fixed channel Packet handler.
 *  Note: To receive Data Tx Event, BT_L2CAP_FIXED_CHANNEL_TX_COMPLETE feature
 *  flag shall be defined.
 */
typedef void (* L2CAP_FIXED_CHANNEL_EVENT_HANDLER)
             (
                 /* Device Handle */
                 /* IN */ DEVICE_HANDLE *handle,

                 /* Event Type - Connect/Disconnect/Data Rx/Tx */
                 /* IN */ UCHAR event_type,

                 /**
                  *  Event -> Parameter
                  *  ------------------
                  *  Connected -> Peer Address, Connection Identifier
                  *  Disconnected -> Connection Identifier
                  *  Data RX -> PDU received
                  *  Data TX -> PDU transmitted
                  */
                 /* IN */ void *param,

                 /**
                  *  Event -> Parameter Length
                  *  -------------------------
                  *  Connected -> Size of structure storing
                  *               Peer Address, Connection Identifier
                  *  Disconnected -> Size of Connection Identifier
                  *  Data RX -> Length of received PDU
                  *  Data TX -> Length of transmitted PDU
                  */
                 /* IN */ UINT16 param_length
             ) DECL_REENTRANT;
#endif /* BT_L2CAP_FIXED_CHANNEL */

#ifdef BT_LE
/**
 * LE L2CAP Application Asynchronous Notification Callback.
 *
 * L2CAP calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param handle Identifies peer device on which event occurred.
 * \param event Any of the Event occurred from the list at \ref le_l2cap_events.
 * \param data Data associated with the event if any or NULL.
 * \param data_len Size of the event data. 0 if event data is NULL.
 */
typedef void (* L2CAP_LE_EVENT_HANDLER)
             (
                 /* IN */ DEVICE_HANDLE * handle,        /* BD_ADDR and Type */
                 /* IN */ UCHAR           event_type,    /* Event */
                 /* IN */ UCHAR         * param,         /* PDU received */
                 /* IN */ UINT16          param_length   /* length of PDU received */
             ) DECL_REENTRANT;
#endif /* BT_LE */
/** \} */

/* ---------------------------------------------- Macros */
/**
 *  Check if L2CA Data Write API returned success. This macro
 *  takes in the actual retval from the API and evaluates based
 *  on whether the retval is any of the flow control error codes
 *  that eventually mean a success.
 *
 *  It's not really a failure, but a warning that next
 *  write on this L2CAP CID will fail, and the flow is
 *  off till L2CA_TxFlowIND indicates flow on again.
 *
 *  \param ret
 */
#define L2CA_IS_DATA_WRITE_SUCCESS(ret) \
    ((API_SUCCESS != (ret)) && \
     (L2CAP_TX_FLOW_OFF != (ret)) && \
     (L2CAP_FEC_TX_WINDOW_FULL != (ret)))? (ret): (API_SUCCESS)

/* ---------------------------------------------- API Declarations */
/**
 * \defgroup l2cap_api API Definitions
 * \{
 * Describes APIs defined by the module.
 */

#ifdef __cplusplus
extern "C"{
#endif

#ifdef L2CAP

/** L2CAP Initialisation & Shutdown */
/**
 *  \brief To do power on initialization of L2CAP module
 *
 *  \par Description:
 *       This function is the EtherMind-Init handler for the L2CAP module
 *       and performs power-on initialization.
 *
 *  \note This function must be called only once.
 */
void em_l2cap_init (void);

#ifdef BT_HAVE_SHUTDOWN
/**
 *  \brief To do de-initialization of L2CAP module
 *
 *  \par Description:
 *       This function is the EtherMind-De-Init handler for the L2CAP module.
 */
void em_l2cap_shutdown (void);
#endif /* BT_HAVE_SHUTDOWN */

/**
 *  \brief To perform Bluetooth specific initializations for L2CAP module
 *
 *  \par Description:
 *       This function is the Bluetooth-ON handler for L2CAP module, and it
 *       performs bluetooth specific initializations for the L2CAP module.
 */
void l2cap_bt_init (void);
/**
 *  \brief To perform Bluetooth specific shutdown for L2CAP module
 *
 *  \par Description:
 *       This function is the Bluetooth-OFF handler for L2CAP module, and it
 *       performs bluetooth specific shutdown for the L2CAP module.
 */
void l2cap_bt_shutdown ( void );
#else
#define em_l2cap_init()
#define em_l2cap_shutdown()
#define l2cap_bt_init()
#define l2cap_bt_shutdown()
#endif /* L2CAP */


/** Initialization of L2CAP_PSM Structure */
/**
 *  \brief To initialize L2CAP PSM data structure
 *
 *  \par Description:
 *  This L2CAP API can be used by any Upper Layer PSM
 *  to initialize L2CAP_PSM Structure before registering
 *  the same to L2CAP using l2cap_register_psm(). This API
 *  initializes PSM value, and sets NULL for all optional
 *  L2CA Callbacks. Also, it disables Connectionless data
 *  reception by default.
 *
 *  \param [in] psm
 *         PSM value of the upper layer.
 *  \param [out] l2cap_psm
 *         Pointer to the L2CAP_PSM Structure, allocated
 *         by the Upper Layer prior to calling this API
 *
 *  \return None
 *
 *  \note
 *  It is adviced that the Upper Layers call this API
 *  prior to registration of the PSM to initialize
 *  unused (optional) L2CA Callbacks to NULL.
 */
void l2cap_init_psm
     (
         /* IN */  UINT16       psm,
         /* OUT */ L2CAP_PSM *  l2cap_psm
     );

/** Registration of Upper Layer PSM to L2CAP */
/**
 *  \brief To register Upper Layer PSM to L2CAP
 *
 *  \par Description:
 *       This API enables upper layers of L2CAP to register their PSM.
 *       During registration, upper layers are required to provide a set of
 *       callback functions (L2CA Callbacks) that L2CAP will use to report
 *       various events (L2CA Indications & Confirmation).
 *
 *  \param [in] l2cap_psm
 *         Pointer to the L2CAP_PSM structure, allocated by the upper
 *         layer prior to calling this API. It is adviced that the upper
 *         layer use l2cap_init_psm() API to initialize the L2CAP_PSM
 *         structure, to set NULL for the optional L2CA callbacks. All
 *         mandatory L2CA callbacks must be populated
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *           describing the cause of failure.
 *
 *  \note
 *         A PSM must be assigned according to the rule stated in the L2CAP
 *         specification - all PSM values must be Odd, that is, the least
 *         significant bit of the least significant octet must be 1. Also, all
 *         PSM values must be assigned such that the least significant bit of
 *         the most significant octet equals 0.
 */

API_RESULT l2cap_register_psm
           (
               /* IN */ L2CAP_PSM *l2cap_psm
           );

/** De-registration of Upper Layer PSM to L2CAP */
/**
 *  \brief To de-register Upper Layer PSM to L2CAP
 *
 *  \par Description:
 *       This API de-registers an already registered PSM from the L2CAP.
 *
 *  \param [in] psm
 *         The PSM value of the upper layer to be de-registered.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *           describing the cause of failure.
 *
 *  \note
 *  It must be noted that this routine can succeed for a registered
 *  PSM only if there exists no active L2CAP channels for it.
 *
 */
API_RESULT l2cap_unregister_psm ( /* IN */ UINT16 psm );

/** Registration of Common Callbacks (L2CA_PingCNF & L2CA_InfoCNF) */
/**
 *  \brief To register Echo and Get Information Confirmation callbacks.
 *
 *  \par Description:
 *       This API registers common set of L2CAP callbacks for handling
 *       Echo/Info Confirmation events.
 *
 *
 *  \param [in] cb_struct
 *         Pointer to the L2CAP_COMMON_CB structure allocated by
 *         the caller
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *           describing the cause of failure.
 */
API_RESULT l2cap_register_common_cb ( /* IN */ L2CAP_COMMON_CB *cb_struct );


#ifdef L2CAP_TX_QUEUE_FLOW
/**
 *  \brief To register a L2CAP Transmission Queue Flow Control callback.
 *
 *  \par Description:
 *  This API registers the L2CAP ACL transmission queue flow control callback.
 *  If registered, L2CAP calls this callback function whenever number of
 *  available buffers, in the ACL transmission queue, goes above or below
 *  pre-configured limits.
 *
 *  \param [in] callback_fn
 *         The callback function to be called when number of available buffers,
 *         in the ACL transmission queue, goes above or below a pre-set limits.
 *         This parameter cannot be NULL.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT l2cap_register_tx_queue_flow_cb
           (
               /* IN */ API_RESULT (* callback_fn) (UCHAR tx_q_state, UINT16 num_buf_available)
           );

/**
 *  \brief To get number of available buffers in ACL Transmission Queue.
 *
 *  \par Description:
 *  This API provides the number of available buffers, in the L2CAP's ACL
 *  transmission queue, to application. The application may use the number
 *  of buffers to implement its flow control logic.
 *
 *  \param [out] num_buffer
 *         The number of available buffer, in the ACL transmission queue,
 *         at the time of calling this callback. This parameter cannot be NULL.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT l2cap_get_tx_queue_num_buffer
           (
               /* OUT */ UINT32 *    num_buffer
           );

/**
 *  \brief To set the low & high water marks for the ACL transmission queue
 *  buffers.
 *
 *  \par Description:
 *  This API provides a method for application to dynamically change/control
 *  the low & high water marks for ACL transmission queue, based upon which
 *  L2CAP decides to call the L2CAP Transmission Queue Flow Control Callback.
 *
 *  The above callback mechanism is provided in L2CAP to inform application
 *  when the number of available buffers, in the ACL transmission queue, is
 *  equal or less than the configured low water mark. The application can
 *  decide what to do to stop data transmission temporarily, so as to allow
 *  L2CAP for some time to transmit ACL packets from transmission queue to
 *  the lower layer/Controller (LMP/Baseband).
 *
 *  The L2CAP, using the same callback, informs the application when the number
 *  of available buffer, in the ACL transmission queue, becomes equal of more
 *  than the configured high water mark. At this point, application can
 *  re-start data write/transmission.
 *
 *  \param [in] tx_queue_lwm
 *         The Low Water Mark for number of available buffers in the ACL
 *         transmission queue.
 *
 *  \param [in] tx_queue_hwm
 *         The High Water Mark for number of available buffers in the ACL
 *         transmission queue.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT l2cap_set_tx_queue_flow_limits
           (
               /* IN */ UINT32    tx_queue_lwm,
               /* IN */ UINT32    tx_queue_hwm
           );
#endif /* L2CAP_TX_QUEUE_FLOW */


/** LP Callbacks */
#ifdef BT_ARCH_1_2
/**
 *  \brief To inform L2CAP about new ACL Connection.
 *
 *  \par Description:
 *  This API implements the LP_ConnectIND primitive for the lower layer
 *  HCI or LMP. LP_ConnectIND should be called to inform L2CAP about
 *  establishment of new ACL Connection with a remote Bluetooth device.
 *  Successful ACL Connection also marks creation of L2CAP Signaling
 *  Channel, and a Signaling Entity is created for the purpose.
 *
 *  \param [in] device_handle
 *         The DEVICE_HANDLE of the remote device
 *  \param [in] handle
 *         The LMP Connection Handle for the remote device
 *  \param [in] link_type
 *         BR/EDR or LE Link
 *  \param [in] role
 *         The LMP Role of the local device
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2cap_lp_connect_ind
           (
               /* IN */ DEVICE_HANDLE  * device_handle,
               /* IN */ UINT16           handle,
               /* IN */ UCHAR            link_type,
               /* IN */ UCHAR            role
           );

/**
 *  \brief To inform L2CAP about ACL Disconnection.
 *
 *  \par Description:
 *  This API implements the LP_DisconnectIND primitive for the lower layer
 *  HCI or LMP. LP_DisconnectIND should be called to inform L2CAP about
 *  disconnection of ACL Connection with a remote Bluetooth device.
 *  Disconnection of ACL Connection also marks deletion of L2CAP Signalling
 *  Channel.
 *
 *  \param [in] device_handle
 *         The DEVICE_HANDLE of the remote device
 *  \param [in] reason
 *         The Reason for Disconnection of LMP connection
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2cap_lp_disconnect_ind
           (
               /* IN */ DEVICE_HANDLE * device_handle,
               /* IN */ UCHAR           reason
           );

/**
 *  \brief To inform L2CAP about incoming ACL Data.
 *
 *  \par Description:
 *  This function handles the incoming ACL fragments from the LMP
 *
 *  \param [in] acl_header
 *         Buffer containing the ACL Header from the LMP
 *  \param [in] packet
 *         Buffer containing the L2CAP Payload from the LMP
 *  \param [in] packet_len
 *         Size of the Buffer containing the ACL packet
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2cap_lp_data_read
           (
               /* IN */ UCHAR *  acl_header,
               /* IN */ UCHAR *  packet,
               /* IN */ UINT16   packet_len
           );
/**
 *  \brief To create a handler for LP_ModeChangeIND from HCI or LMP
 *
 *  \par Description
 *       This function is the handler routine for the LP_ModeChangeIND for
 *       L2CAP. This event is generated by the lower protocol (HCI or LMP)
 *       whenever the connection mode of the Signalling Entity changes
 *       between Active or one of the Low Power modes (Park, Sniff etc.).
 *       This information is used by L2CAP to decide whether to use
 *       Active or Piconet Broadcast when sending Connectionless Group
 *       Data.
 *
 *  \param handle (IN)
 *         LMP Handle of the remote device.
 *  \param mode (IN)
 *         Current Mode of the ACL Connection to Device corresponding
 *         to BD_ADDR
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *           describing the cause of failure.
 */
API_RESULT l2cap_lp_mode_change_ind
           (
               /* IN */ UINT16  handle,
               /* IN */ UCHAR   mode
           );

/**
 *  \brief To inform L2CAP about ACL Buffer Sizes of Bluetooth Controller.
 *
 *  \par Description:
 *  This function handles LP_ACLBufferSize service primitive for the
 *  HCI/LMP. The HCI/LMP must inform L2CAP about the ACL Buffer sizes
 *  of LMP, so that L2CAP can perform fragmentation of L2CAP PDUs before
 *  they are transmitted to LMP.
 *
 *  \param lmp_packet_size (IN)
 *         Maximum LMP Fragment size
 *  \param lmp_max_packets (IN)
 *         Number of LMP Fragments that can be outstanding
 *
 *  \return None
 */
void l2cap_lp_acl_buffer_size
     (
         /* IN */ UINT16  lmp_packet_size,
         /* IN */ UINT16  lmp_max_packets
     );

/**
 *  \brief To inform L2CAP about ACL Number of Completed Packets event.
 *
 *  \par Description:
 *  This function handles LP_NumCompletedPackets service primitive for the
 *  HCI/LMP. The HCI/LMP must inform L2CAP submitted LMP Fragments containing
 *  L2CAP PDUs are transmitted over the air, and the corresponding LMP
 *  buffer is now free to use.
 *
 *  \param [in] handle
 *         LMP (ACL) Connection Handle
 *  \param [in] num_completed
 *         Number of LMP Fragments sent (completed) for the LMP Handle
 *
 *  \return API_RESULT
 *          API_SUCCESS if ACL handle is known
 *          API_FAILURE if ACL handle is not present with L2CAP
 */
API_RESULT l2cap_lp_num_completed_packets
           (
               /* IN */ UINT16  handle,
               /* IN */ UINT16  num_completed
           );

#else  /* BT_ARCH_1_2 */
/**
 *  \brief To create a handler for LP_ConnectIND from HCI or LMP
 *
 *  \par Description
 *       This API implements the LP_ConnectIND primitive for the lower layer
 *       HCI or LMP. LP_ConnectIND should be called to inform L2CAP about
 *       establishment of new ACL Connection with a remote Bluetooth device.
 *       Successful ACL Connection also marks creation of L2CAP Signalling
 *       Channel, and a Signalling Entity is created for the purpose.
 *
 *  \param [in] device_handle
 *         The DEVICE_HANDLE of the remote device
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *           describing the cause of failure.
 */
API_RESULT l2cap_lp_connect_ind
           (
               /* IN */ DEVICE_HANDLE    * device_handle
           );
/**
 *  \brief To create a handler for LP_DisconnectIND from HCI or LMP
 *
 *  \par Description
 *       This API implements the LP_DisconnectIND primitive for the lower layer
 *       HCI or LMP. LP_DisconnectIND should be called to inform L2CAP about
 *       disconnection of ACL Connection with a remote Bluetooth device.
 *       Disconnection of ACL Connection also marks deletion of L2CAP Signalling
 *       Channel.
 *
 *  \param [in] handle
 *         LMP Handle of the remote device.
 *
 *  \param [in] reason
 *         The Reason for Disconnection of LMP connection
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *           describing the cause of failure.
 */
API_RESULT l2cap_lp_disconnect_ind
           (
               /* IN */ DEVICE_HANDLE    * device_handle,
               /* IN */ UCHAR            reason
           );

API_RESULT l2cap_lp_data_read
           (
               /* IN */ DEVICE_HANDLE    * device_handle,
               /* IN */ UCHAR            * data_buf,
               /* IN */ UINT16           length,
               /* IN */ UCHAR            pb,
               /* IN */ UCHAR            bc
           );

API_RESULT lp_device_mode_change_ind
           (
               /* IN */ DEVICE_HANDLE    * device_handle,
               /* IN */ UCHAR            mode
           );

#endif /* BT_ARCH_1_2 */

#ifdef BT_ENH_L2CAP
/**
 *  \brief To handle indication of setting automatic flush timeout.
 *
 *  \par Description
 *       To handle indication of setting automatic flush timeout.
 *
 *  \param [in]handle
 *         LMP Handle of the remote device.
 *
 *  \param [in]flush_to
 *         New Automatic Flush Timeout for the ACL link.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *           describing the cause of failure.
 */
API_RESULT l2cap_lp_set_flush_to_ind
           (
               /* IN */ UINT16  handle,
               /* IN */ UINT16  flush_to
           );
#endif /* BT_ENH_L2CAP */

#ifdef BT_LE
/**
 *  \brief To inform L2CAP about ACL Buffer Sizes of Bluetooth Controller.
 *
 *  \par Description:
 *  This function handles LP_ACLBufferSize service primitive for the
 *  HCI/LMP. The HCI/LMP must inform L2CAP about the ACL Buffer sizes
 *  of LMP, so that L2CAP can perform fragmentation of L2CAP PDUs before
 *  they are transmitted to LMP.
 *
 *  \param [in] lmp_packet_size
 *         Maximum LMP Fragment size
 *  \param [in] lmp_max_packets
 *         Number of LMP Fragments that can be outstanding
 *
 *  \return None
 */
void l2cap_lp_le_buffer_size
     (
         /* IN */ UINT16  lmp_packet_size,
         /* IN */ UINT16  lmp_max_packets
     );
#endif /* BT_LE */

/**
 *  'l2ca_' APIs are those called from the upper layer to send a relevant
 *  command from L2CAP to the peer. These are the interface functions
 *  L2CAP and the upper layer.
 */

#ifdef BT_ENH_L2CAP

/** L2CA Connect Request */
API_RESULT l2ca_connect_req_ex
           (
               /* IN */ UINT16           local_psm,
               /* IN */ UINT16           remote_psm,
               /* IN */ DEVICE_HANDLE    * handle,
               /* IN */ UCHAR            config_pref
           );

#define l2ca_connect_req(p1, p2, p3) \
        l2ca_connect_req_ex(p1, p2, p3, L2CAP_CONFIG_PREF_BASIC)

/** L2CA Connect Response */
API_RESULT l2ca_connect_rsp_ex
           (
               /* IN */ UINT16     local_cid,
               /* IN */ UINT16     response,
               /* IN */ UINT16     status,
               /* IN */ UCHAR      config_pref
           );

#define l2ca_connect_rsp(p1, p2, p3) \
        l2ca_connect_rsp_ex(p1, p2, p3, L2CAP_CONFIG_PREF_BASIC)

#else  /* BT_ENH_L2CAP */

/** L2CA Connect Request */
API_RESULT l2ca_connect_req
           (
               /* IN */ UINT16           local_psm,
               /* IN */ UINT16           remote_psm,
               /* IN */ DEVICE_HANDLE    * handle
           );

/** L2CA Connect Response */
API_RESULT l2ca_connect_rsp
           (
               /* IN */ UINT16     local_cid,
               /* IN */ UINT16     response,
               /* IN */ UINT16     status
           );

#endif /* BT_ENH_L2CAP */

API_RESULT l2cap_sm_access_response
           (
               DEVICE_HANDLE    * handle,
               UINT16           psm,
               UINT32           l2cap_data,
               UINT16           result
           );

/** L2CA Config Request - CID, Config Options */
API_RESULT l2ca_config_req
           (
               /* IN */  UINT16                  local_cid,
               /* IN */  L2CAP_CONFIG_OPTION    *config_option
           );

/** L2CA Config Response - CID, Config Options */
API_RESULT l2ca_config_rsp
           (
               /* IN */  UINT16                 local_cid,
               /* IN */  L2CAP_CONFIG_OPTION    *config_option
           );

/** L2CA Disconnect Request - CID */
/**
 *  \brief To disconnect an established L2CAP channel.
 *
 *  \par Description:
 *  This service primitive initiates sending of an L2CAP Disconnect Request
 *  command to the remote L2CAP entity to initiate disconnection of
 *  the referred L2CAP channel.
 *  Disconnection of the L2CAP channel always succeeds - either by reception
 *  of the L2CAP Disconnect Response message from the peer, or by timeout.
 *  In any case, L2CAP will confirm disconnection of the channel,
 *  by calling the L2CA_DisconnectCNF callback.
 *
 *  \param [in] local_cid
 *         This parameter specifies the local channel end-point for the L2CAP
 *         channel. For initiator of L2CAP channel establishment, this must be
 *         set to the value indicated by the L2CA_ConnectCNF callback.
 *         For responder, upper layer protocol obtains this value when L2CAP
 *         invokes L2CA_ConnectIND callback.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT l2ca_disconnect_req
           (
               /* IN */  UINT16  local_cid
           );

#ifdef L2CAP_SUPPORT_CBFC_MODE
/**
 *  \par Description:
 *  This API implements the L2CAP_DisconnectREQ service primitive for CBFC mode,
 *  which is used by the L2CAP upper layers to disconnect a L2CAP connection-oriented
 *  data channel.
 *
 *  \param [in] local_cid
 *         Local Channel Identifier for the L2CAP CBFC channel
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */

API_RESULT l2ca_cbfc_disconnect_req
           (
               /* IN */  UINT16  local_cid
           );
#endif /* L2CAP_SUPPORT_CBFC_MODE */

#ifdef L2CAP_SUPPORT_ECBFC_MODE
/**
 *  \par Description:
 *  This API implements the L2CAP_DisconnectREQ service primitive for CBFC mode,
 *  which is used by the L2CAP upper layers to disconnect a L2CAP connection-oriented
 *  data channel.
 *
 *  \param [in] local_cid
 *         Local Channel Identifier for the L2CAP CBFC channel
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */

API_RESULT l2ca_ecbfc_disconnect_req
           (
               /* IN */  UINT16  local_cid
           );
#endif /* L2CAP_SUPPORT_ECBFC_MODE */

/**
 *  \par Description:
 *  This API return the Remote CID associated with the provided Local CID.
 *
 *  \param [in] local_cid
 *         Local Channel Identifier for the L2CAP channel
 *
 *  \param [out] remote_cid
 *         Remote Channel Identifier for the L2CAP channel
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2cap_get_remote_cid
           (
               /* IN */   UINT16  local_cid,
               /* OUT */  UINT16 *remote_cid
           );

/** L2CA Data Write - CID, Length, Buffer, Length Written */
/**
 *  \brief To request transmission of data through a connected and configured
 *         L2CAP channel.
 *
 *  \par Description:
 *        This service primitive enables upper layer protocol to request L2CAP
 *        for transmission of service data units (SDUs) through a specified
 *        L2CAP channel.
 *        The state of the L2CAP channel must be in OPEN state (connected &
 *        configured) to be able to transmit the given SDU.
 *        If the length of the SDU is more than the OutMTU of the L2CAP channel,
 *        then only the first OutMTU octets will be sent.
 *        The same API can be used by upper layer protocols to transmit Group
 *        data as well. To broadcast a Group data from a PSM, a group is to be
 *        created for the PSM and members must be added to the group. If there
 *        are no members in a group then the data will not be broadcasted.
 *
 *  \param [in] local_cid
 *         This parameter specifies the local channel end-point for the L2CAP
 *         channel.
 *
 *  \param [in] buffer_len
 *         This parameter specifies the size of the requested SDU to be
 *         transmitted over the L2CAP channel, inclusive of 4 octets of
 *         L2CAP Header size.
 *
 *  \param [in] buffer
 *         This parameter specifies the pointer holding the SDU to be
 *         transmitted of size data_length. The SDU must have 4 octets of empty
 *         space in front to hold the 4 octets of L2CAP Header.
 *
 *  \param [out] actual
 *         This parameter returns the actual number of octets, of
 *         'buffer_length', scheduled for transmission over the L2CAP channel.
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2ca_data_write
           (
               /* IN */  UINT16    local_cid,
               /* IN */  UINT16    buffer_len,
               /* IN */  UCHAR  *  buffer,
               /* OUT */ UINT16 *  actual
           );

#ifdef BT_UCD
/**
 *  \brief To request transmission of Unicast Connectionless Data.
 *
 *  \par Description:
 *  This API implements the L2CA_DataWriteREQ service for the Unicast
 *  Connectionless Data to the L2CAP's upper layers.
 *  This service primitive is offered to enable upper
 *  layers to request transmission of upper layer payload over an
 *  Unicast L2CAP channel.
 *
 *  \param [in] handle
 *         Pointer to Device Queue Handle: the peer device to send data to.
 *  \param [in] buffer_len
 *         Size of the upper layer payload to be transmitted
 *  \param [in] buffer
 *         Buffer containing the upper layer payload
 *  \param [out] actual
 *         Actual number of octets transmitted
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 *
 *  \note
 *  The upper layer must allocate for L2CAP Header while requesting
 *  for data transmission. For, connection-less data channels,
 *  L2CAP Header consists of 6 octets.
 */
API_RESULT l2ca_ucd_write
           (
               /* IN */  DEVICE_HANDLE * handle,
               /* IN */  UINT16          buffer_len,
               /* IN */  UCHAR         * buffer,
               /* OUT */ UINT16        * actual
           );
#endif /* BT_UCD */

/** L2CA Ping/Echo Request - BD_ADDR, Echo Data, Data Length */
/**
 *  \brief To ping for a Bluetooth device address.
 *
 *  \par Description:
 *       This service interface allows application over L2CAP to ping for a
 *       remote Bluetooth device using its Bluetooth device address (BD_ADDR).
 *       It is a prerequisite that an ACL connection with the device to be
 *       pinged already exists. If there is no ACL connection then an error
 *       is returned.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address, BD_ADDR, of the remote Bluetooth device.
 *
 *  \param [in] echo_data
 *         This parameter specifies a pointer holding the data to be sent to
 *         the remote device with the echo request.
 *
 *  \param [in] echo_datalen
 *         This parameter specifies the size of the echo data.
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2ca_ping_req
           (
               /* IN */  UCHAR *  bd_addr,
               /* IN */  UCHAR *  echo_data,
               /* IN */  UINT16   echo_datalen
           );

/** L2CA Get Information Request - BD_ADDR, Info Type */
/**
 *  \brief To send Get Information Request.
 *
 *  \par Description:
 *  This API implements the L2CA_GetInfoREQ service to the L2CAP's
 *  upper layers. This service primitive is offered to enable upper
 *  layers to request transmission of L2CAP_InfoREQ to peer Bluetooth
 *  device, and get response for it.
 *
 *  \param [in] bd_addr
 *         The BD_ADDR of the Remote device to which a channel needs to
 *         be established
 *
 *  \param [in] info_type
 *         The Information Type for which L2CAP_InfoREQ is to be sent.
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2ca_getinfo_req
           (
               /* IN */ UCHAR   * bd_addr,
               /* IN */ UINT16    info_type
           );

#ifdef BT_ENH_L2CAP
/** L2CA Rx Flow REQ */
/**
 *  \brief To request for SDU reception flow control
 *
 *  \par Description:
 *       This service interface enables upper layer protocol to request for data
 *       reception flow control for a specified L2CAP channel, configured for
 *       either Retransmission Mode or Flow Control Mode.
 *       This API is only available in L2CAP if the BT_1_2 compilation switch
 *       is enabled.
 *
 *  \param [in] local_cid
 *         This parameter specifies the local channel end-point for the L2CAP
 *         channel.
 *
 *  \param [in] flow
 *         This parameter specifies whether data reception flow control to be
 *         switched on or off for the specified L2CAP channel. The valid values
 *         are specified in the Reception Flow Control Constant section.
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2ca_rx_flow_req
           (
               /* IN */ UINT16   lcid,
               /* IN */ UCHAR    flow
           );
#endif /* BT_ENH_L2CAP */

#ifdef BT_PBF
/** To set flushability of L2CAP packets transmitted for a CID */
API_RESULT l2cap_set_flushability
           (
               /* IN */ UINT16    lcid,
               /* IN */ UCHAR     flushable
           );
#endif /* BT_PBF */

#ifdef L2CAP_HAVE_GROUP_SUPPORT

/** L2CA Group Create - PSM, CID */
/**
 *  \brief To create a group to which data can be broadcasted for a PSM.
 *
 *  \par Description:
 *       This service primitive creates a new group and allocates a new channel
 *       end-point identified (CID) to the group. The PSM value for which the
 *       group is created should be a registered PSM.
 *
 *  \param [in] psm
 *         Indicates the PSM for which the group is being created.
 *
 *  \param [out] cid
 *         Pointer to an UINT16 upper layer allocated UINT16 variable onto which
 *         L2CAP copies the new channel identifier for the created group.
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2ca_group_create
           (
               /* IN */  UINT16   psm,
               /* OUT */ UINT16 * cid
           );

/** L2CA Group Add Member - CID, BD_ADDR */
/**
 *  \brief To add a member to an existing group.
 *
 *  \par Description:
 *       This service primitive adds a remote device to the group. The group CID
 *       must be a valid one and an ACL connection must exist with the given
 *       Bluetooth device address for the device to be added to the group.
 *
 *  \param [in] cid
 *         Local channel end-point for the L2CAP group channel.
 *
 *  \param [in] handle
 *         Pointer to a DEVICE_HANDLE holding the remote address of the device to be
 *         added to the group.
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2ca_group_add_member
           (
                /* IN */ UINT16           cid,
                /* IN */ DEVICE_HANDLE  * handle
           );

/** L2CA Group Remove Member  - CID, BD_ADDR */
/**
 *  \brief To remove a member from an existing group.
 *
 *  \par Description:
 *       This service primitive removes a remote device from a group. Both the
 *       group cid and the device address must be valid.
 *
 *  \param [in] cid
 *         Local channel end-point for the L2CAP group channel.
 *
 *  \param [in] handle
 *         Pointer to a DEVICE_HANDLE holding the remote address of the device to be
 *         removed to the group.
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2ca_group_remove_member
           (
               /* IN */ UINT16          cid,
               /* IN */ DEVICE_HANDLE * handle
           );

/** L2CA Group Destroy - CID */
/**
 *  \brief To close an existing group
 *
 *  \par Description:
 *       This service primitive closes a group.
 *
 *  \param [in] cid
 *         Local channel end-point for the L2CAP group channel.
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2ca_group_destroy ( /* IN */ UINT16 cid);

/** L2CA Group Get Membership - CID, Buffer_len, Buffer */
/**
 *  \brief To report the existing members of the group
 *
 *  \par Description:
 *       This service primitive gives the address information of all the
 *       existing members of the group. The primitive has to passed a pointer to
 *       buffer sufficient to accommodate all the member device addresses. If
 *       the buffer size is insufficient then a failure is returned and the
 *       number of devices in the group is reported so that the upper layer
 *       allocates a buffer appropriate size.
 *
 *  \param [in] cid
 *         Local channel end-point for the L2CAP group channel.
 *
 *  \param [in,out] num_bd_addr
 *         This is an Input/output parameter which indicates the number of
 *         device addresses that can be stored in the buffer or in case of
 *         insufficient space this variable will report the number of devices
 *         in the group.
 *
 *  \param [out] buffer
 *         Pointer to the buffer in which the device addresses are to be stored.
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2ca_group_get_membership
           (
                /* IN */    UINT16   cid,
                /* INOUT */ UINT16 * num_bd_addr,
                /* IN */    UCHAR  * buffer
           );

/** L2CA Group Enable CLT - PSM */
/**
 *  \brief To enables the connectionless channel for receiving broadcast data.
 *
 *  \par Description:
 *       This service primitive enables the connectionless traffic reception for
 *       the given PSM. The service primitive can be called with PSM value
 *       0x0000 to enable the connectionless traffic for all the active PSM's.
 *
 *  \param [in] psm
 *         PSM for which the connectionless traffic is to be enabled.
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2ca_group_enable_clt ( /* IN */ UINT16 psm);

/** L2CA Group Disable CLT - PSM */
/**
 *  \brief To disable reception of Connectionless data.
 *
 *  \par Description:
 *       This service primitive disables the connectionless traffic reception
 *       for the given PSM. The service primitive can be called with PSM value
 *       0x0000 to disable the connectionless traffic for all the active PSM's.
 *
 *  \param [in] psm
 *         PSM for which the connectionless traffic is to be disabled.
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2ca_group_disable_clt ( /* IN */ UINT16 psm);

#endif /* L2CAP_HAVE_GROUP_SUPPORT */


/** L2CA Utility APIs */
/**
 *  \brief To configure timeout duration for L2CAP.
 *
 *  \par Description:
 *  This API allows upper layers or applications to configure the Timeout
 *  for L2CAP Configuration Timer.
 *
 *  \param seconds (IN)
 *         Timeout value in seconds for L2CAP Configuration Timer
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT l2cap_set_config_timeout (/* IN */ UINT16 seconds);
/**
 *  \brief To initialize a L2CAP_CONFIG_OPTION variable.
 *
 *  \par Description:
 *       This utility routine can be used by upper layer protocols
 *       to initialize a L2CAP_CONFIG_OPTION variable to default parameter
 *       values that is used with l2ca_config_req() or l2ca_config_rsp() APIs.
 *       The L2CAP_CONFIG_OPTION contains pointer for FLOW_SPEC and
 *       L2CAP_FEC_OPTION data types - these are initialized to NULL. The
 *       MTU and Flush Timeout parameters are initialized to L2CAP_MTU_DEFAULT
 *       and L2CAP_FLUSH_TO_DEFAULT respectively.
 *
 *  \param [out] ptr
 *         Pointer to an upper layer protocol allocated L2CAP_CONFIG_OPTION
 *         variable, which needs to be initialized.
 */
void l2cap_init_config_option (/* OUT */ L2CAP_CONFIG_OPTION * ptr);

/**
 *  \brief To initialize a FLOW_SPEC variable.
 *
 *  \par Description:
 *       This utility routine can be used by upper layer protocols to initialize
 *       a FLOW_SPEC variable to default parameter values that can be used in a
 *       L2CAP_CONFIG_OPTION variable in l2ca_config_req() or l2ca_config_rsp()
 *       APIs.
 *
 *  \param [out] flow_spec
 *         Pointer to an upper layer protocol allocated FLOW_SPEC variable,
 *         which needs to be initialized to default parameter values.
 */
void l2cap_init_flow_spec_default (/* OUT */ FLOW_SPEC * flow_spec);

#ifdef L2CAP_EXTENDED_FLOW_SPEC
/**
 *  \brief To initialize Extended Flow Specification.
 *
 *  \par Description:
 *  This utility function initializes the Extended Flow Specification
 *  to the default values, as described below:
 *    - Identifier             : 0x01
 *    - Service Type           : Best Effort
 *    - Maximum SDU Size       : 0xFFFF
 *    - SDU Inter-arrival Time : 0xFFFFFFFF
 *    - Access Latency         : 0xFFFFFFFF
 *    - Flush Timeout          : 0xFFFFFFFF
 *
 *  \param [out] ext_flow_spec
 *         Pointer to the Flow Specification to be initialized.
 */
void l2cap_init_ext_flow_spec_default
     (
         /* OUT */  L2CAP_EXT_FLOW_SPEC *  ext_flow_spec
     );
#else
#define l2cap_init_ext_flow_spec_default(ext_flow_spec)
#endif /* L2CAP_EXTENDED_FLOW_SPEC */

#ifdef BT_ENH_L2CAP
/** Utility API to Initialize FEC Option */
/**
 *  \brief To initialize FEC Option.
 *
 *  \par Description:
 *  This utility function initializes the FEC Option to
 *  the default values, as described below:
 *    - Mode          : Basic
 *    - MaxTransmit   : 0
 *    - MonitorTimeout: 0
 *    - RetxTimeout   : 0
 *    - TxWindow      : 0
 *    - MPS           : 0
 *
 *  \param [out] fec
 *         Pointer to the FEC Option to be initialized.
 *
 *  \return None
 */
void l2cap_init_fec_option_default (/* OUT */ L2CAP_FEC_OPTION *fec);
#endif /* BT_ENH_L2CAP */

#ifdef BT_UCD
/**
 *  \brief To register UCD payload reception callback with L2CAP.
 *
 *  \par Description:
 *  This API registers a callback for Unicast Connectionless Data payload
 *  reception with L2CAP.
 *
 *  \param [in] ucd
 *         This parameter is pointer to UCD data data structure
 *         containing pointer to function implemented by higher-level
 *         protocol/application to handle received UCD payload.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT l2cap_register_ucd_callback
           (
               /* IN */ L2CAP_UCD_STRUCT   * ucd
           );
#endif /* BT_UCD */

/** L2CA Fixed Channel APIs */
#ifdef BT_LE
/**
 *  \brief Register callback with LE L2CAP
 *
 *  \par Description:
 *       This routine registers callback with LE L2CAP
 *
 *  \param [in] callback
 *         LE L2CAP Application Notification Callback
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_l2cap_register_le_event_cb
           (
               /* IN */ L2CAP_LE_EVENT_HANDLER callback
           );

/**
 *  \brief Send Connection Update Request
 *
 *  \par Description:
 *       This routine sends LE L2CAP Connection Update Request.
 *       LE L2CAP Connection Update Request shall be sent
 *       only in Peripheral (slave) role.
 *
 *  \param  [in] device_handle
 *          Device handle of the peer device.
 *
 *  \param  [in] min_interval
 *          Minimum value for the connection event interval.
 *          This shall be less than or equal to Conn_Interval_Max.
 *
 *  \param  [in] max_interval
 *          Maximum value for the connection event interval.
 *          This shall be greater than or equal to Conn_Interval_Min.
 *
 *  \param  [in] slave_latency
 *          Slave latency for the connection in number of connection events.
 *
 *  \param  [in] supervision_timeout
 *          Supervision Timeout for the LE Link.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_l2cap_le_connection_param_update_request
           (
               /* IN */ DEVICE_HANDLE * device_handle,
               /* IN */ UINT16          min_interval,
               /* IN */ UINT16          max_interval,
               /* IN */ UINT16          slave_latency,
               /* IN */ UINT16          supervision_timeout
           );

/**
 *  \brief Send Connection Update Response
 *
 *  \par Description:
 *       This routine sends LE L2CAP Connection Update Response.
 *       On receiving LE L2CAP Connection Update Request,
 *       only the Central (master) device, shall send Connection Update
 *       Response.
 *
 *  \param  [in] handle
 *          Device handle of the peer device.
 *
 *  \param  [in] result
 *          The result field indicates the response to the Connection Parameter
 *          Update Request.
 *          The result value of 0x0000 (\ref L2CAP_CONNECTION_PARAMETERS_ACCEPTED)
 *          indicates that LE master Host has accepted the connection parameters.
 *          Value 0x0001 (\ref L2CAP_CONNECTION_PARAMETERS_REJECTED) indicates that
 *          LE master Host has rejected the connection parameters.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT BT_l2cap_le_connection_param_update_response
           (
               /* IN */ DEVICE_HANDLE * handle,
               /* IN */ UINT16          result
           );

/**
 *  \brief To write on a L2CAP fixed channel.
 *
 *  \par Description:
 *      This API is used to send a packet on L2CAP fixed channel.
 *
 *  \param [in] device_handle
 *         Device handle of the peer device.
 *
 *  \param [in] cid
 *         Fixed channel identifier
 *
 *  \param [in] buffer
 *         Buffer containing packet to be sent. See note below.
 *
 *  \param [in] buffer_len
 *         Packet length
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT l2cap_fixed_channel_data_write
           (
               /* IN */ DEVICE_HANDLE  * device_handle,
               /* IN */ UINT16           cid,
               /* IN */ UCHAR          * buffer,
               /* IN */ UINT16           buffer_len
           );

/**
 *  \brief To register a callback to handle fixed channel events and data.
 *
 *  \par Description:
 *  This API is used to register a callback which can handle incoming
 *  fixed channel data of maximum packet size specified and connect/disconnect
 *  event.
 *
 *  \param [in] cid
 *         Fixed Channel Identifier
 *
 *  \param [in] mtu
 *         Maximum size of the packet callback function can handle
 *
 *  \param [in] callback
 *         Pointer to callback function
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 *
 *  \note
 *  This function does not validate the specified MTU size.
 */
API_RESULT l2cap_register_fixed_channel_cb
           (
               /* IN */ UINT16                            cid,
               /* IN */ UINT16                            mtu,
               /* IN */ L2CAP_FIXED_CHANNEL_EVENT_HANDLER callback
           );

/**
 *  \brief To update the MTU for an already registered fixed channel.
 *
 *  \par Description:
 *  This API can be used to update MTU size of an already registered fixed
 *  channel.
 *
 *  \param [in] device_handle
 *         Device handle of the peer device.
 *
 *  \param [in] cid
 *         Fixed Channel Identifier
 *
 *  \param [in] mtu
 *         Maximum size of the packet can be received on the associated fixed
 *         channel
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT l2cap_update_fixed_channel_mtu
           (
               /* IN */ DEVICE_HANDLE * device_handle,
               /* IN */ UINT16          cid,
               /* IN */ UINT16          mtu
           );

#ifdef L2CAP_SUPPORT_CBFC_MODE
/**
 *  \brief To register a higher level protocol (PSM) which supports CBFC mode
 *  over L2CAP.
 *
 *  \par Description:
 *  This API registers a new upper layer protocol, or, PSM to L2CAP,
 *  along with the set of callbacks, for the L2CAP Credit Based Flow Control
 *  mode.
 *
 *  \param [in] l2cap_psm
 *         This parameter must be a pointer to the L2CAP_PSM_CBFC variable
 *         containing the PSM value of the higher-level protocol and
 *         the set of callbacks for this protocol (PSM).
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT l2cap_cbfc_register_psm
           (
               /* IN */ L2CAP_PSM_CBFC   * l2cap_psm
           );

/**
 *  \brief To de-register an Upper Layer LE_PSM from L2CAP.
 *
 *  \par Description:
 *  This API de-registers a upper layer protocol, or, LE_PSM from L2CAP,
 *  for the L2CAP Credit Based Flow Control mode.
 *
 *  \param [in] psm
 *         Already registered LE Protocol/Service Multiplexer for L2CAP
 *         Credit Based Flow Control mode.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 *  \note
 *  It must be noted that this routine can succeed for a registered
 *  LE_PSM only if there exists no active L2CAP channels for it.
 */
API_RESULT l2cap_cbfc_unregister_psm
           (
               /* IN */ UINT16  psm
           );

/**
 *  'l2ca_' APIs are those called from the upper layer to send a relevant
 *  command from L2CAP to the peer. These are the interface functions
 *  L2CAP and the upper layer.
 */

/** L2CA Utility APIs */

/**
 *  \brief To establish L2CAP channel for LE Credit Based Flow Control mode.
 *
 *  \par Description:
 *  This L2CAP service interface initiates L2CAP channel establishment
 *  procedure for CBFC mode, to the specified remote Bluetooth device,
 *  for the specified PSM representing an upper layer protocol above L2CAP.
 *
 *  \param [in] handle
 *         Device handle of the peer device.
 *
 *  \param [in] remote_psm
 *         Remote PSM, representing the upper layer protocol above L2CAP.
 *
 *  \param [in] local_psm
 *         Local PSM, representing the upper layer protocol above L2CAP.
 *
 *  \param [in] param
 *         This parameter must be a pointer to the L2CAP_CBFC_CONNECT_PARAM
 *         variable containing the connection parameters for the L2CAP channel.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT l2ca_cbfc_connect_req
           (
               /* IN */ DEVICE_HANDLE            * handle,
               /* IN */ UINT16                     remote_psm,
               /* IN */ UINT16                     local_psm,
               /* IN */ L2CAP_CBFC_CONNECT_PARAM * param
           );

/**
 *  \brief To respond to an incoming L2CAP connection request for
 *  LE Credit Based Flow Control mode.
 *
 *  \par Description:
 *  This L2CAP service interface enables an upper layer protocol to respond
 *  to L2CAP connection request for LE Credit Based Flow Control mode
 *  for the specified PSM from the specified remote Bluetooth device.
 *  It is mandatory that the upper layer PSM always responds back by calling
 *  this service interface upon receiving CBFC Connection Request.
 *  The channel is established (opened) only when the PSM concerned responds
 *  back with successful response. Otherwise, the channel establishment request
 *  from the peer will be rejected by L2CAP with appropriate result
 *  and status as received from the upper layer PSM.
 *
 *  \param [in] local_cid
 *         This parameter specifies the local L2CAP channel end-point for
 *         this new L2CAP channel. On receipt of L2CAP Connect Request command
 *         from the peer, local L2CAP will temporarily create a channel -
 *         this parameter identifies the new channel.
 *         If the upper layer PSM chooses to reject this connection,
 *         this temporary channel will be closed.
 *  \param [in] response
 *         This parameter specifies the response of the upper layer for
 *         the new L2CAP channel establishment request from the peer.
 *         It must be set to a value as specified in L2CAP Connect Result Codes.
 *  \param [in] param
 *         This parameter must be a pointer to the L2CAP_CBFC_CONNECT_PARAM
 *         variable containing the connection parameters for the L2CAP channel.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT l2ca_cbfc_connect_rsp
           (
               /* IN */ UINT16 local_cid,
               /* IN */ UINT16 response,
               /* IN */ L2CAP_CBFC_CONNECT_PARAM * param
           );

/**
 *  \brief To send LE Flow Control Credit packet.
 *
 *  \par Description:
 *  This L2CAP service interface enables an upper layer protocol to send
 *  LE Flow Control Credit packet to remote Bluetooth device, when it is
 *  capable of receiving additional LE-frames.
 *
 *  \param [in] local_cid
 *         This parameter specifies the local channel end-point for the L2CAP
 *         channel. For initiator of L2CAP channel establishment, this must be
 *         set to the value indicated by the L2CA_ConnectCNF callback.
 *         For responder, upper layer protocol obtains this value when L2CAP
 *         invokes L2CA_ConnectIND callback.
 *  \param [in] credit
 *         The credit value field represents number of credits the receiving
 *         device can increment, corresponding to the number of LE-frames
 *         that can be sent to the peer device sending the LE Flow Control
 *         Credit packet.
 *         The credit value field shall be a number between 1 and 65535.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT l2ca_cbfc_send_flow_control_credit
           (
               /* IN */ UINT16 local_cid,
               /* IN */ UINT16 credit
           );
#endif /* L2CAP_SUPPORT_CBFC_MODE */

#if ((defined L2CAP_SUPPORT_CBFC_MODE) || (defined L2CAP_SUPPORT_ECBFC_MODE))
/**
 *  \brief To write on an L2CAP CBFC Channel.
 *
 *  \par Description:
 *  This API is used to send a packet on L2CAP CBFC channel.
 *
 *  \param [in] device_handle
 *         Device handle of the peer device.
 *  \param [in] cid
 *         This parameter specifies the local channel end-point for the L2CAP
 *         channel. For initiator of L2CAP channel establishment, this must be
 *         set to the value indicated by the L2CA_ConnectCNF callback.
 *         For responder, upper layer protocol obtains this value when L2CAP
 *         invokes L2CA_ConnectIND callback.
 *  \param [in] buffer
 *         Buffer containing packet to be sent
 *  \param [in] buffer_len
 *         Packet length
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 *
 *  \note
 *
 *  LE Information frame (LE-Frame) packet format:
 *
 *    16    16    16       n
 *  +-----+-----+-----+---...---+
 *  |  L  |  C  |  S  |    I    |
 *  +-----+-----+-----+---...---+
 *  LSB                       MSB
 *  <-   BLH  ->
 *
 *  BLH: Basic L2CAP Header (4 Octets).
 *  L: Length (2 Octets) and C: Channel ID (2 Octets) of Basic L2CAP Header.
 *  S: L2CAP SDU Length (2 Octets) present in first LE Frame of the SDU.
 *  I: Information Payload ('n' Octets)
 *     For first LE-frame of the SDU, 'n' = L - 2
 *     For subsequent LE-frames, 'n' = L
 *
 *  There are two ways 'buffer' and 'buffer_len' can be passed to this interface,
 *  and that dependts on if 'L2CAP_ALLOC_FOR_CBFC_SDU' feature flag is defined
 *  or not.
 *
 *  If 'L2CAP_ALLOC_FOR_CBFC_SDU' feature flag is defined:
 *  - 'buffer' shall only contain Information Payload.
 *  - 'buffer_len' shall contain the length of the Information Payload.
 *  - L2CAP will allocate memory for complete LE-frame, including associated
 *    header and copy the application provided Information Payload in 'buffer'.
 *    Application can free (if allocated) or reuse the 'buffer' once this API
 *    returns.
 *  - If the 'buffer' is allocated, application can free the memory
 *    on return of this API.
 *
 *  If 'L2CAP_ALLOC_FOR_CBFC_SDU' feature flag is not defined:
 *  - 'buffer' shall have additional 6 Octets of memory for L2CAP header,
 *    and have L2CAP SDU Length field and Information Payload filled.
 *    L2CAP will costruct the remaining Basic L2CAP Header fields,
 *    in the 'buffer' provided by the caller.
 *  - 'buffer_len' shall contain the length 'L2CAP SDU Length' field
 *    in addition to the length of Information Payload (i.e., all the octets
 *    filled by the caller).
 *  - L2CAP will assume the caller has provided an allocate memory for
 *    the complete LE-frame. L2CAP will frame the Basic L2CAP Header in the
 *    provied buffer. L2CAP will free the application 'buffer' once the
 *    transmission of SDU is complete.
 *  - If the 'buffer' is allocated, application shall not free the memory
 *    on return of this API. Application shall free the buffer in the handling
 *    of corresponding Tx complete callback l2ca_data_write_cb().
 *
 *  In general, for memory constaint devices, 'L2CAP_ALLOC_FOR_CBFC_SDU'
 *  should not be defined.
 *  This is to avoid allocation of buffer for L2CAP SDU in the L2CAP implementation
 *  and also to avoid copying the data from application buffer to L2CAP buffer
 *  (as much as possible).
 *  In some cases where the SDU has to be segmented to smaller PDUs, based on the MTU
 *  and MPS value, L2CAP needs to frame the PDUs from the SDU provided by the application.
 */
API_RESULT l2ca_channel_data_write
            (
                /* IN */ DEVICE_HANDLE * device_handle,
                /* IN */ UINT16          cid,
                /* IN */ UCHAR         * buffer,
                /* IN */ UINT16          buffer_len
            );
#endif /* L2CAP_SUPPORT_CBFC_MODE || L2CAP_SUPPORT_ECBFC_MODE */

#ifdef L2CAP_SUPPORT_ECBFC_MODE
/**
 *  \brief To register a higher level protocol (PSM) which supports ECBFC mode
 *  over L2CAP.
 *
 *  \par Description:
 *  This API registers a new upper layer protocol, or, PSM to L2CAP,
 *  along with the set of callbacks, for the L2CAP Enhanced Credit Based Flow
 *  Control mode.
 *
 *  \param [in] l2cap_psm
 *         This parameter must be a pointer to the L2CAP_PSM_ECBFC variable
 *         containing the PSM value of the higher-level protocol and
 *         the set of callbacks for this protocol (PSM).
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 */
API_RESULT l2cap_ecbfc_register_psm
           (
               /* IN */ L2CAP_PSM_ECBFC   * l2cap_psm
           );

/**
 *  \brief To de-register an Upper Layer LE_PSM from L2CAP.
 *
 *  \par Description:
 *  This API de-registers a upper layer protocol, or, LE_PSM from L2CAP,
 *  for the L2CAP Enhanced Credit Based Flow Control mode.
 *
 *  \param [in] psm
 *         Already registered LE Protocol/Service Multiplexer for L2CAP
 *         Enhanced Credit Based Flow Control mode.
 *
 *  \return API_RESULT
 *          - API_SUCCESS: If successful.
 *          - Error Codes: Error code describing cause of failure.
 *  \note
 *  It must be noted that this routine can succeed for a registered
 *  LE_PSM only if there exists no active L2CAP channels for it.
 */
API_RESULT l2cap_ecbfc_unregister_psm
           (
               /* IN */ UINT16  psm
           );

/**
 *  'l2ca_' APIs are those called from the upper layer to send a relevant
 *  command from L2CAP to the peer. These are the interface functions
 *  L2CAP and the upper layer.
 */

/** L2CA Utility APIs */

/**
 *  \brief To establish L2CAP channel for Enhanced Credit Based Flow Control mode.
 *
 *  \par Description:
 *  This L2CAP service interface initiates L2CAP channel establishment
 *  procedure for ECBFC mode, to the specified remote Bluetooth device,
 *  for the specified PSM representing an upper layer protocol above L2CAP.
 *
 *  \param [in] handle
 *         Device handle of the peer device.
 *  \param [in] remote_psm
 *         Remote PSM, representing the upper layer protocol above L2CAP.
 *  \param [in] local_psm
 *         Local PSM, representing the upper layer protocol above L2CAP.
 *  \param [in] param
 *         This parameter must be a pointer to the L2CAP_ECBFC_CONNECT_PARAM
 *         variable containing the connection parameters for the L2CAP channel.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT l2ca_ecbfc_connect_req
           (
               /* IN */ DEVICE_HANDLE            * handle,
               /* IN */ UINT16                     remote_psm,
               /* IN */ UINT16                     local_psm,
               /* IN */ L2CAP_ECBFC_CONNECT_PARAM * param
           );

/**
 *  \brief To respond to an incoming L2CAP connection request for
 *  Enhanced Credit Based Flow Control mode.
 *
 *  \par Description:
 *  This L2CAP service interface enables an upper layer protocol to respond
 *  to L2CAP connection request for LE Credit Based Flow Control mode
 *  for the specified PSM from the specified remote Bluetooth device.
 *  It is mandatory that the upper layer PSM always responds back by calling
 *  this service interface upon receiving ECBFC Connection Request.
 *  The channel is established (opened) only when the PSM concerned responds
 *  back with successful response. Otherwise, the channel establishment request
 *  from the peer will be rejected by L2CAP with appropriate result
 *  and status as received from the upper layer PSM.
 *
 *  \param [in] local_cid
 *         TODO: Edit the text below to take care of list of CIDs
 *
 *         This parameter specifies the local L2CAP channel end-points for
 *         this new set of L2CAP channels. On receipt of L2CAP Connect Request command
 *         from the peer, local L2CAP will temporarily create a channel -
 *         this parameter identifies the new channel.
 *         If the upper layer PSM chooses to reject this connection,
 *         this temporary channel will be closed.
 *  \param [in] local_cid_len
 *         Number of Local CIDs
 *  \param [in] response
 *         This parameter specifies the response of the upper layer for
 *         the new L2CAP channel establishment request from the peer.
 *         It must be set to a value as specified in L2CAP Connect Result Codes.
 *  \param [in] param
 *         This parameter must be a pointer to the L2CAP_ECBFC_CONNECT_PARAM
 *         variable containing the connection parameters for the L2CAP channel.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT l2ca_ecbfc_connect_rsp
           (
               /* IN */ UINT16 * local_cid,
               /* IN */ UINT16   local_cid_len,
               /* IN */ UINT16 response,
               /* IN */ L2CAP_ECBFC_CONNECT_PARAM * param
           );

/**
 *  \brief To reconfigure L2CAP channel for Enhanced Credit Based Flow Control mode.
 *
 *  \par Description:
 *  This L2CAP service interface initiates L2CAP channel reconfiguration
 *  procedure for ECBFC mode, to the specified remote Bluetooth device.
 *
 *  \param [in] handle
 *         Device handle of the peer device.
 *  \param [in] param
 *         This parameter must be a pointer to the L2CAP_ECBFC_RECONFIG_PARAM
 *         variable containing the reconfiguration parameters for the L2CAP channel.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT l2ca_ecbfc_reconfig_req
           (
               /* IN */ DEVICE_HANDLE            * handle,
               /* IN */ L2CAP_ECBFC_RECONFIG_PARAM * param
           );

/**
 *  \brief To respond to an incoming L2CAP reconfiguration request for
 *  Enhanced Credit Based Flow Control mode.
 *
 *  \par Description:
 *  This L2CAP service interface enables an upper layer protocol to respond
 *  to L2CAP reconfiguration request for Enhanced Credit Based Flow Control mode
 *  for the already established L2CAP Channels in ECBFC mode.
 *  It is mandatory that the upper layer PSM always responds back by calling
 *  this service interface upon receiving ECBFC Reconfiguration Request.
 *
 *  \param [in] local_cid
 *         List of CIDs for which the Reconfiguration response is being sent.
 *         This list of CIDs shall be the exact match of the list of CIDs that
 *         is informed about through \ref l2ca_reconfig_ind_cb. It is the
 *         responsibility of the application to match this CID list. L2CAP
 *         does not maintain a separate context to track the CIDs that are being
 *         reconfigured.

 *  \param [in] local_cid_len
 *         Number of Local CIDs which the Reconfiguration response is being sent.
 *         This Number of CID shall be the exact match of the Number of CIDs that
 *         is informed about through \ref l2ca_reconfig_ind_cb. It is the
 *         responsibility of the application to match this CID Number. L2CAP
 *         does not maintain a separate context to track the CIDs that are being
 *         reconfigured.
 *
 *  \param [in] result
 *         Result of the Reconfiguration request
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT l2ca_ecbfc_reconfig_rsp
           (
               UINT16 *local_cid,
               UINT16  local_cid_len,
               UINT16  result
           );

/**
 *  \brief To send Enhanced Flow Control Credit packet.
 *
 *  \par Description:
 *  This L2CAP service interface enables an upper layer protocol to send
 *  Enhanced Flow Control Credit packet to remote Bluetooth device, when it is
 *  capable of receiving additional LE-frames.
 *
 *  \param [in] local_cid
 *         This parameter specifies the local channel end-point for the L2CAP
 *         channel. For initiator of L2CAP channel establishment, this must be
 *         set to the value indicated by the L2CA_ConnectCNF callback.
 *         For responder, upper layer protocol obtains this value when L2CAP
 *         invokes L2CA_ConnectIND callback.
 *  \param [in] credit
 *         The credit value field represents number of credits the receiving
 *         device can increment, corresponding to the number of LE-frames
 *         that can be sent to the peer device sending the Enhanced Flow Control
 *         Credit packet.
 *         The credit value field shall be a number between 1 and 65535.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define l2ca_ecbfc_send_flow_control_credit(local_cid, credit) \
        l2ca_cbfc_send_flow_control_credit((local_cid), (credit))

/**
 *  \brief To write on an L2CAP ECBFC Channel.
 *
 *  \par Description:
 *  This API is used to send a packet on L2CAP ECBFC channel.
 *
 *  \param [in] device_handle
 *         Device handle of the peer device.
 *  \param [in] cid
 *         This parameter specifies the local channel end-point for the L2CAP
 *         channel. For initiator of L2CAP channel establishment, this must be
 *         set to the value indicated by the L2CA_ConnectCNF callback.
 *         For responder, upper layer protocol obtains this value when L2CAP
 *         invokes L2CA_ConnectIND callback.
 *  \param [in] buffer
 *         Buffer containing packet to be sent
 *  \param [in] buffer_len
 *         Packet length
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 *
 *  \note
 *
 *  LE Information frame (LE-Frame) packet format:
 *
 *    16    16    16       n
 *  +-----+-----+-----+---...---+
 *  |  L  |  C  |  S  |    I    |
 *  +-----+-----+-----+---...---+
 *  LSB                       MSB
 *  <-   BLH  ->
 *
 *  BLH: Basic L2CAP Header (4 Octets).
 *  L: Length (2 Octets) and C: Channel ID (2 Octets) of Basic L2CAP Header.
 *  S: L2CAP SDU Length (2 Octets) present in first LE Frame of the SDU.
 *  I: Information Payload ('n' Octets)
 *     For first LE-frame of the SDU, 'n' = L - 2
 *     For subsequent LE-frames, 'n' = L
 *
 *  There are two ways 'buffer' and 'buffer_len' can be passed to this interface,
 *  and that dependts on if 'L2CAP_ALLOC_FOR_CBFC_SDU' feature flag is defined
 *  or not.
 *
 *  If 'L2CAP_ALLOC_FOR_CBFC_SDU' feature flag is defined:
 *  - 'buffer' shall only contain Information Payload.
 *  - 'buffer_len' shall contain the length of the Information Payload.
 *  - L2CAP will allocate memory for complete LE-frame, including associated
 *    header and copy the application provided Information Payload in 'buffer'.
 *    Application can free (if allocated) or reuse the 'buffer' once this API
 *    returns.
 *  - If the 'buffer' is allocated, application can free the memory
 *    on return of this API.
 *
 *  If 'L2CAP_ALLOC_FOR_CBFC_SDU' feature flag is not defined:
 *  - 'buffer' shall have additional 6 Octets of memory for L2CAP header,
 *    and have L2CAP SDU Length field and Information Payload filled.
 *    L2CAP will costruct the remaining Basic L2CAP Header fields,
 *    in the 'buffer' provided by the caller.
 *  - 'buffer_len' shall contain the length 'L2CAP SDU Length' field
 *    in addition to the length of Information Payload (i.e., all the octets
 *    filled by the caller).
 *  - L2CAP will assume the caller has provided an allocate memory for
 *    the complete LE-frame. L2CAP will frame the Basic L2CAP Header in the
 *    provied buffer. L2CAP will free the application 'buffer' once the
 *    transmission of SDU is complete.
 *  - If the 'buffer' is allocated, application shall not free the memory
 *    on return of this API. Application shall free the buffer in the handling
 *    of corresponding Tx complete callback l2ca_data_write_cb().
 *
 *  In general, for memory constaint devices, 'L2CAP_ALLOC_FOR_CBFC_SDU'
 *  should not be defined.
 *  This is to avoid allocation of buffer for L2CAP SDU in the L2CAP implementation
 *  and also to avoid copying the data from application buffer to L2CAP buffer
 *  (as much as possible).
 *  In some cases where the SDU has to be segmented to smaller PDUs, based on the MTU
 *  and MPS value, L2CAP needs to frame the PDUs from the SDU provided by the application.
 */
#define l2ca_ecbfc_data_write(device_handle, cid, buffer, buffer_len) \
        l2ca_channel_data_write((device_handle), (cid), (buffer), (buffer_len))
#endif /* L2CAP_SUPPORT_ECBFC_MODE */

#endif /* BT_LE */


/* ---------------------------------------------- Platform Definitions */


/* ---------------------------------------------- Feature Definitions */


/* ---------------------------------------------- Test Hooks */
#ifdef L2CAP_TEST_PACKET_DROP

/*
 *  Application hooks for dropping packets during transmission
 *  and/or reception for I/S-frames. L2CAP does not implement
 *  these functions, rather the application willing to test
 *  L2CAP with simulated packet drops must implement these.
 *
 *  Return value: 1 - Drop the packet, do not process
 *                0 - process the packet as usual
 *
 *  Currently, no packet drop is implemented for received I/S-frames,
 *  though l2cap_test_packet_drop_rx() is called - so, the appl can
 *  print what is being received, but returning 1 from this function
 *  will be ignored.
 *
 *  Also, l2cap_test_packet_drop_rx() is called only if FCS check passes
 *  for the received frame.
 */
UCHAR l2cap_test_packet_drop_tx (UCHAR *header, UCHAR header_len);
UCHAR l2cap_test_packet_drop_rx (UCHAR *header, UCHAR header_len);

/* Application hook to report FCS error */
void l2cap_test_packet_fcs_error
     (UINT16 cid, UINT16 fcs_recv, UINT16 fcs_calc);

#endif /* L2CAP_TEST_PACKET_DROP */

/* void l2cap_test_drop_config (UCHAR flag); */
/* void l2cap_test_check_up (void); */

#ifdef L2CAP_TEST_UPF
API_RESULT l2cap_upf_i_frame_rx (UCHAR tx_seq);
#endif /* L2CAP_TEST_UPF */


/* ---------------------------------------------- Feature Definitions */


/* ---------------------------------------------- Test Hooks */

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_L2CAP_ */

