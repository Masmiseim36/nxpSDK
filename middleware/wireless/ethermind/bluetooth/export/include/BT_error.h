
/**
 *  \file BT_error.h
 *
 *  This file lists all the Error Codes returned by EtherMind
 *  Bluetooth APIs.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_ERROR_
#define _H_BT_ERROR_

/**
 * \defgroup BT_ERROR_CODES EtherMind Error Codes
 * \{
 *  This section contains all the error codes defined for EtherMind stack
 *  and profiles.
 *  <p>
 *  \anchor error_code_overview
 *  <b>Theory:</b>
 *  <p>
 *    Every API under EtherMind Protocol Suite returns \ref API_RESULT,
 *    which is a 2 Byte Unsigned Short variable. The higher order byte
 *    signifies the Module from which the Error has been generated, and
 *    the lower order byte encodes the actual reason of Error.
 *    <p>
 *    Each module under EtherMind Stack is given unique
 *    Error ID (the higher order byte). Also, for each module the Error
 *    Code (the lower order byte) signifies an unique error situation.
 *    <p>
 *    For EtherMind Protocol Modules (eg, HCI, RFCOMM etc.), Error IDs are
 *    assigned from the range 0x10 to 0x4F. For Profiles, the range
 *    is from 0x50 to 0x7F.
 *    <p>
 *    The definition of \ref API_SUCCESS is 0x0000 - which is the 'Success'
 *    return value for an API returning \ref API_RESULT. All other values for
 *    should be treated as Errors.
 *    <p>
 *    The definition of \ref API_FAILURE is 0xFFFF - which stands for
 *    "Unknown Error Situation".
 *    <p>
 *  <b>Note:</b>
 *    <p>
 *    The applications using native/profile EtherMind API should NOT do
 *    any check on the basis of \ref API_FAILURE - rather, the correct way to
 *    detect an error situation is by doing a negative check on \ref
 *    API_SUCCESS.
 *    <p>
 *    For example,
 *    \code if ( BT_hci_inquiry(x, y, z) == API_FAILURE ) \endcode
 *    ... Wrong !
 *    <p>
 *    <p>
 *    \code if ( BT_hci_inquiry(x, y, z) != API_SUCCESS ) \endcode
 *    ... Correct !
 */

/**
 * \defgroup ble_error_codes_defines Defines
 * \{
 */

/** Definition of API_RESULT */

#ifndef API_RESULT_DEFINED
typedef UINT16    API_RESULT;
#define API_RESULT_DEFINED
#endif /* API_RESULT_DEFINED */

/* Definitions of API_SUCCESS & API_FAILURE */
#ifdef API_SUCCESS
#undef API_SUCCESS
#endif /* API_SUCCESS */
#define API_SUCCESS             0x0000U

#ifdef API_FAILURE
#undef API_FAILURE
#endif /* API_FAILURE */
#define API_FAILURE             0xFFFFU

/** \} */

/* ====================== EtherMind Module Error IDs ====================== */

/**
 * \defgroup btle_error_codes_groups Error Grouping
 * \{
 */

/**
 * \defgroup btle_error_codes_groups_std Specification Error Codes (0x00 - 0x0F)
 * Error IDs for Bluetooth Specification Defined Error Codes (0x00 - 0x0F).
 * \{
 */

/** Error Codes for Host Controller - \ref btle_error_codes_module_hc */
#define HC_ERR_ID                               0x0000U
/** \cond ignore */
#define AV_ERR_ID                               0x0100U
#define MD_ERR_ID                               0x0200U
#define ATT_SPEC_ERR_ID                         0x0300U
#define SM_LE_ERR_ID                            0x0400U
/** \endcond */

/** \} */

/**
 * \defgroup btle_error_codes_groups_core Core Modules (0x10 - 0x1F)
 * Error IDs for EtherMind Core Modules (0x10 - 0x1F).
 * \{
 */

/** Error Codes for BT Common - \ref btle_error_codes_module_btcommon */
#define BT_COMMON_ERR_ID                        0x1000U
/** Error Codes for Write Task - \ref btle_error_codes_module_wt */
#define WT_ERR_ID                               0x1100U
/** Error Codes for Timer - \ref btle_error_codes_module_timer */
#define TIMER_ERR_ID                            0x1200U
/** Error Codes for State Machine - \ref btle_error_codes_module_utils */
#define FSM_ERR_ID                              0x1300U
/** Error Codes for Device Queue - \ref btle_error_codes_module_utils */
#define DQ_ERR_ID                               0x1400U
/** Error Codes for RACP - \ref btle_error_codes_module_utils */
#define RACP_ERR_ID                             0x1500U
/** Error Codes for HCI Transport - \ref btle_error_codes_module_ht */
#define HT_ERR_ID                               0x1600U

/** \} */

/**
 * \defgroup btle_error_codes_groups_protocols Protocols (0x20 - 0x3F)
 * Error IDs for EtherMind Protocol Modules (0x20 - 0x3F).
 * \{
 */

/** Error Codes for HCI - \ref btle_error_codes_module_hci */
#define HCI_ERR_ID                              0x2000U
/** \cond ignore */
#define SM_ERR_ID                               0x2100U
/** \endcond */
/** Error Codes for Device Manager - \ref btle_error_codes_module_dm */
#define DM_ERR_ID                               0x2200U
/** Error Codes for L2CAP - \ref btle_error_codes_module_l2cap */
#define L2CAP_ERR_ID                            0x2300U
#define SDP_ERR_ID                              0x2400U
#define DB_ERR_ID                               0x2500U
#define RFCOMM_ERR_ID                           0x2600U
/** Error ID 0x2700 used by deprecated TCS module */
#define BNEP_ERR_ID                             0x2800U
#define AVDTP_ERR_ID                            0x2900U
#define AVCTP_ERR_ID                            0x2A00U
#define OBEX_ERR_ID                             0x2B00U
/* Older IrOBEX Error Codes */
#define IROBEX_ERR_ID                           0x2C00U
#define MCAP_ERR_ID                             0x2D00U

/**
 * Error Codes for Security Manager Protocol -
 * \ref btle_error_codes_module_smp
 */
#define SMP_ERR_ID                              0x2E00U
/** Error Codes for Attribute Protocol - \ref btle_error_codes_module_att */
#define ATT_ERR_ID                              0x2F00U

/** \} */

/* Error IDs for EtherMind Driver Modules (0x40 - 0x4F) */
#define DRV_ERR_ID                              0x4000U
#define PAN_DRV_ERR_ID                          0x4100U
#define A2DP_DRV_ERR_ID                         0x4200U
#define AVRCP_DRV_ERR_ID                        0x4300U
#define VCOM_DRV_ERR_ID                         0x4400U
#define HID_DEV_DRV_ERR_ID                      0x4500U

/**
 * \defgroup btle_error_codes_groups_profiles Profiles (0x50 - 0x7F)
 * Error IDs for EtherMind Profile Modules (0x50 - 0x7F).
 * \{
 */

/** Error Codes for GAP - \ref btle_error_codes_module_gap */
#define GAP_ERR_ID                              0x5000U
#define SDAP_ERR_ID                             0x5100U
#define SPP_ERR_ID                              0x5200U
#define HSP_UNIT_ERR_ID                         0x5300U
#define HSP_AG_ERR_ID                           0x5400U
#define HFP_UNIT_ERR_ID                         0x5500U
#define HFP_AG_ERR_ID                           0x5600U
#define DUNP_DT_ERR_ID                          0x5700U
#define DUNP_GW_ERR_ID                          0x5800U
/** Error ID 0x5900 used by deprecated FAXP module */
#define UDIP_ERR_ID                             0x5A00U
#define OPP_ERR_ID                              0x5B00U
#define FTP_ERR_ID                              0x5C00U
#define SYNCP_ERR_ID                            0x5D00U
#define BIP_ERR_ID                              0x5E00U
#define BPP_ERR_ID                              0x5F00U
/** Error ID 0x6000 used by deprecated CTP module */
/** Error ID 0x6100 used by deprecated ICP module */
/** Error ID 0x6200 used by deprecated LAP module */
#define PAN_ERR_ID                              0x6300U
#define HID_ERR_ID                              0x6400U
#define HCRP_ERR_ID                             0x6500U
#define A2DP_ERR_ID                             0x6600U
#define AVRCP_ERR_ID                            0x6700U
#define PBAP_ERR_ID                             0x6800U
#define SAP_ERR_ID                              0x6900U
#define MAP_ERR_ID                              0x6A00U
/** Error Codes for GATT DB - \ref btle_error_codes_module_gatt_db */
#define GATT_DB_ERR_ID                          0x6B00U
/** \cond ignore */

#define GNSS_ERR_ID                             0x6C00U

/* New Module Additions */
#define CTN_ERR_ID                              0x6D00U
#define COVER_ART_ERR_ID                        0x6E00U

/** \endcond */

/** \} */

/**
 * \defgroup btle_error_codes_groups_utility Utility Modules (0xA0 - 0xAF)
 * Error IDs for EtherMind Utility/Tools Modules (0xA0 - 0xAF).
 * \{
 */

/** \cond ignore */
#define BT_FOPS_ERR_ID                          0xA000U
/** \endcond */
#define AT_PARSER_ERR_ID                        0xA100U
#define JPL_ERR_ID                              0xA200U
#define HAM_ERR_ID                              0xA300U

/** Error Codes for Persistant Storage - \ref btle_error_codes_module_utils */
#define STORAGE_ERR_ID                          0xA400U

/** \} */

/** \} */

/* ================== EtherMind Common Reason Error Codes ================= */

/**
 * \defgroup btle_error_codes_module_common Common
 * \{
 */

#define MUTEX_INIT_FAILED                       0x0001U
#define COND_INIT_FAILED                        0x0002U
#define MUTEX_LOCK_FAILED                       0x0003U
#define MUTEX_UNLOCK_FAILED                     0x0004U
#define MEMORY_ALLOCATION_FAILED                0x0005U

/** \} */

/* ===================== EtherMind Module Error Codes ===================== */

/* ======================================= Section 'Host Controller (LMP)' */
/**
 * \defgroup btle_error_codes_module_hc Host Controller (LMP)
 * \{
 */

#define HC_UNKNOWN_HCI_COMMAND                  (0x0001U | HC_ERR_ID)
#define HC_NO_CONNECTION                        (0x0002U | HC_ERR_ID)
#define HC_HARDWARE_FAILURE                     (0x0003U | HC_ERR_ID)
#define HC_PAGE_TIMEOUT                         (0x0004U | HC_ERR_ID)
#define HC_AUTHENTICATION_FAILURE               (0x0005U | HC_ERR_ID)
#define HC_KEY_MISSING                          (0x0006U | HC_ERR_ID)
#define HC_MEMORY_FULL                          (0x0007U | HC_ERR_ID)
#define HC_CONNECTION_TIMEOUT                   (0x0008U | HC_ERR_ID)
#define HC_MAX_NUMBER_OF_CONNECTIONS            (0x0009U | HC_ERR_ID)
#define HC_MAX_NUMBER_OF_SCO_CONNECTIONS        (0x000AU | HC_ERR_ID)
#define HC_ACL_CONNECTION_ALREADY_EXISTS        (0x000BU | HC_ERR_ID)
#define HC_COMMAND_DISALLOWED                   (0x000CU | HC_ERR_ID)
#define HC_HOST_REJECTED_LIMITED_RESOURCES      (0x000DU | HC_ERR_ID)
#define HC_HOST_REJECTED_SECURITY_REASONS       (0x000EU | HC_ERR_ID)
#define HC_HOST_REJECTED_UNACCEPTABLE_BD_ADDR   (0x000FU | HC_ERR_ID)
#define HC_HOST_TIMEOUT                         (0x0010U | HC_ERR_ID)
#define HC_UNSUPPORTED_FEATURE_OR_PARAMETER     (0x0011U | HC_ERR_ID)
#define HC_INVALID_HCI_COMMAND_PARAMETERS       (0x0012U | HC_ERR_ID)
#define HC_OTHER_END_TERMINATED_USER            (0x0013U | HC_ERR_ID)
#define HC_OTHER_END_TERMINATED_LOW_RESOURCES   (0x0014U | HC_ERR_ID)
#define HC_OTHER_END_TERMINATED_POWER_OFF       (0x0015U | HC_ERR_ID)
#define HC_CONNECTION_TERMINATED_BY_LOCAL_HOST  (0x0016U | HC_ERR_ID)
#define HC_REPEATED_ATTEMPTS                    (0x0017U | HC_ERR_ID)
#define HC_PAIRING_NOT_ALLOWED                  (0x0018U | HC_ERR_ID)
#define HC_UNKNOWN_LMP_PDU                      (0x0019U | HC_ERR_ID)
#define HC_UNSUPPORTED_REMOTE_FEATURE           (0x001AU | HC_ERR_ID)
#define HC_SCO_OFFSET_REJECTED                  (0x001BU | HC_ERR_ID)
#define HC_SCO_INTERVAL_REJECTED                (0x001CU | HC_ERR_ID)
#define HC_SCO_AIR_MODE_REJECTED                (0x001DU | HC_ERR_ID)
#define HC_INVALID_LMP_PARAMETERS               (0x001EU | HC_ERR_ID)
#define HC_UNSPECIFIED_ERROR                    (0x001FU | HC_ERR_ID)
#define HC_UNSUPPORTED_LMP_PARAMETER_VALUE      (0x0020U | HC_ERR_ID)
#define HC_ROLE_CHANGE_NOT_ALLOWED              (0x0021U | HC_ERR_ID)
#define HC_LMP_RESPONSE_TIMEOUT                 (0x0022U | HC_ERR_ID)
#define HC_LMP_ERROR_TRANSACTION_COLLISION      (0x0023U | HC_ERR_ID)
#define HC_LMP_PDU_NOT_ALLOWED                  (0x0024U | HC_ERR_ID)
#define HC_ENCRYPTION_MODE_NOT_ACCEPTABLE       (0x0025U | HC_ERR_ID)
#define HC_UNIT_KEY_USED                        (0x0026U | HC_ERR_ID)
#define HC_QOS_IS_NOT_SUPPORTED                 (0x0027U | HC_ERR_ID)
#define HC_INSTANT_PASSED                       (0x0028U | HC_ERR_ID)
#define HC_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED  (0x0029U | HC_ERR_ID)
#define HC_DIFFERENT_TRANSACTION_COLLISION      (0x002AU | HC_ERR_ID)
#define HC_QOS_UNACCEPTABLE_PARAMETER           (0x002CU | HC_ERR_ID)
#define HC_QOS_REJECTED                         (0x002DU | HC_ERR_ID)
#define HC_CHANNEL_CLASSIFICATION_NOT_SUPPORTED (0x002EU | HC_ERR_ID)
#define HC_INSUFFICIENT_SECURITY                (0x002FU | HC_ERR_ID)
#define HC_PARAMETER_OUT_OF_MANDATORY_RANGE     (0x0030U | HC_ERR_ID)
#define HC_ROLE_SWITCH_PENDING                  (0x0032U | HC_ERR_ID)
#define HC_RESERVED_SLOT_VIOLATION              (0x0034U | HC_ERR_ID)
#define HC_ROLE_SWITCH_FAILED                   (0x0035U | HC_ERR_ID)
#define HC_EXTENDED_INQUIRY_RESPONSE_TOO_LARGE  (0x0036U | HC_ERR_ID)
#define HC_SECURE_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST \
                                                (0x0037U | HC_ERR_ID)
#define HC_HOST_BUSY_PAIRING                    (0x0038U | HC_ERR_ID)
#define HC_CONNECTION_REJECTED_DUE_TO_NO_SUITABLE_CHANNEL_FOUND \
                                                (0x0039U | HC_ERR_ID)
#define HC_CONTROLLER_BUSY                      (0x003AU | HC_ERR_ID)
#define HC_UNACCEPTEBALE_CONNECTION_INTERVAL    (0x003BU | HC_ERR_ID)
#define HC_DIRECTED_ADVERTISING_TIMEOUT         (0x003CU | HC_ERR_ID)
#define HC_CONNECTION_TREMINATED_DUE_TO_MIC_FAILURE \
                                                (0x003DU | HC_ERR_ID)
#define HC_CONNECTION_FAILED_TO_BE_ESTABLISHED  (0x003EU | HC_ERR_ID)
#define HC_MAC_CONNECTION_FAILED                (0x003FU | HC_ERR_ID)
#define HC_COARSE_CLOCK_ADJUSTMENT_REJECTED     (0x0040U | HC_ERR_ID)
#define HC_TYPE0_SUBMAP_NOT_DEFINED             (0x0041U | HC_ERR_ID)
#define HC_UNKNOWN_ADVERTISING_IDENTIFIER       (0x0042U | HC_ERR_ID)
#define HC_LIMIT_REACHED                        (0x0043U | HC_ERR_ID)
#define HC_OPERATION_CANCELLED_BY_HOST          (0x0044U | HC_ERR_ID)
#define HC_PACKET_TOO_LONG                      (0x0045U | HC_ERR_ID)
/** \} */

/* ======================================= Section 'Audio-Video' */
#define AVDTP_BAD_HEADER_FORMAT                 (0x0001U | AV_ERR_ID)
#define AVDTP_BAD_LENGTH                        (0x0011U | AV_ERR_ID)
#define AVDTP_BAD_ACP_SEID                      (0x0012U | AV_ERR_ID)
#define AVDTP_SEP_IN_USE                        (0x0013U | AV_ERR_ID)
#define AVDTP_SEP_NOT_IN_USE                    (0x0014U | AV_ERR_ID)
#define AVDTP_BAD_SERV_CATEGORY                 (0x0017U | AV_ERR_ID)
#define AVDTP_BAD_PAYLOAD_FORMAT                (0x0018U | AV_ERR_ID)
#define AVDTP_NOT_SUPPORTED_COMMAND             (0x0019U | AV_ERR_ID)
#define AVDTP_INVALID_CAPABILITIES              (0x001AU | AV_ERR_ID)
#define AVDTP_BAD_RECOVERY_TYPE                 (0x0022U | AV_ERR_ID)
#define AVDTP_BAD_MEDIA_TRANSPORT_FORMAT        (0x0023U | AV_ERR_ID)
#define AVDTP_BAD_RECOVERY_FORMAT               (0x0025U | AV_ERR_ID)
#define AVDTP_BAD_ROHC_FORMAT                   (0x0026U | AV_ERR_ID)
#define AVDTP_BAD_CP_FORMAT                     (0x0027U | AV_ERR_ID)
#define AVDTP_BAD_MULTIPLEXING_FORMAT           (0x0028U | AV_ERR_ID)
#define AVDTP_UNSUPPORTED_CONFIGURATION         (0x0029U | AV_ERR_ID)
#define AVDTP_BAD_STATE                         (0x0031U | AV_ERR_ID)

#define AVDTP_BAD_REPORT_FORMAT                 (0x0065U | AV_ERR_ID)

#define A2DP_INVALID_CODEC_TYPE                 (0x00C1U | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_CODEC_TYPE           (0x00C2U | AV_ERR_ID)
#define A2DP_INVALID_SAMPLING_FREQUENCY         (0x00C3U | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_SAMPLING_FREQUENCY   (0x00C4U | AV_ERR_ID)
#define A2DP_INVALID_CHANNEL_MODE               (0x00C5U | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_CHANNEL_MODE         (0x00C6U | AV_ERR_ID)
#define A2DP_INVALID_SUBBANDS                   (0x00C7U | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_SUBBANDS             (0x00C8U | AV_ERR_ID)
#define A2DP_INVALID_ALLOCATION_METHOD          (0x00C9U | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_ALLOCATION_METHOD    (0x00CAU | AV_ERR_ID)
#define A2DP_INVALID_MIN_BITPOOL_VALUE          (0x00CBU | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_MIN_BITPOOL_VALUE    (0x00CCU | AV_ERR_ID)
#define A2DP_INVALID_MAX_BITPOOL_VALUE          (0x00CDU | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_MAX_BITPOOL_VALUE    (0x00CEU | AV_ERR_ID)
#define A2DP_INVALID_LAYER                      (0x00CFU | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_LAYER                (0x00D0U | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_CRC                  (0x00D1U | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_MPF                  (0x00D2U | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_VBR                  (0x00D3U | AV_ERR_ID)
#define A2DP_INVALID_BIT_RATE                   (0x00D4U | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_BIT_RATE             (0x00D5U | AV_ERR_ID)
#define A2DP_INVALID_OBJECT_TYPE                (0x00D6U | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_OBJECT_TYPE          (0x00D7U | AV_ERR_ID)
#define A2DP_INVALID_CHANNELS                   (0x00D8U | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_CHANNELS             (0x00D9U | AV_ERR_ID)
#define A2DP_INVALID_VERSION                    (0x00DAU | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_VERSION              (0x00DBU | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_MAXIMUM_SUL          (0x00DCU | AV_ERR_ID)
#define A2DP_INVALID_BLOCK_LENGTH               (0x00DDU | AV_ERR_ID)
#define A2DP_INVALID_CP_TYPE                    (0x00E0U | AV_ERR_ID)
#define A2DP_INVALID_CP_FORMAT                  (0x00E1U | AV_ERR_ID)
#define A2DP_INVALID_CODEC_PARAMETER            (0x00E2U | AV_ERR_ID)
#define A2DP_NOT_SUPPORTED_CODEC_PARAMETER      (0x00E3U | AV_ERR_ID)

/* ======================================= Section 'Medical' */
#define MCAP_INVALID_COMMAND                    (0x0001U | MD_ERR_ID)
#define MCAP_INVALID_PARAMETER_VALUE            (0x0002U | MD_ERR_ID)
#define MCAP_INVALID_MDEP                       (0x0003U | MD_ERR_ID)
#define MCAP_MDEP_BUSY                          (0x0004U | MD_ERR_ID)
#define MCAP_INVALID_MDL                        (0x0005U | MD_ERR_ID)
#define MCAP_MDL_BUSY                           (0x0006U | MD_ERR_ID)
#define MCAP_INVALID_OPERATION                  (0x0007U | MD_ERR_ID)
#define MCAP_RESOURCE_UNAVIALABLE               (0x0008U | MD_ERR_ID)
#define MCAP_UNSPECIFIED_ERROR                  (0x0009U | MD_ERR_ID)
#define MCAP_REQUEST_NOT_SUPPORTED              (0x000AU | MD_ERR_ID)
#define MCAP_CONFIGURATION_REJECTED             (0x000BU | MD_ERR_ID)


/* ======================================= Section 'BT Common' */
/**
 * \defgroup btle_error_codes_module_btcommon BT Common
 * \{
 */

#define BLUETOOTH_ON_ALREADY_INITIALIZED        (0x0011U | BT_COMMON_ERR_ID)
#define BLUETOOTH_ON_NOT_COMPLETED              (0x0012U | BT_COMMON_ERR_ID)
#define BLUETOOTH_INVALID_PARAMETER_VALUE       (0x0013U | BT_COMMON_ERR_ID)

/** \} */

/* ======================================= Section 'Write Task' */

/**
 * \defgroup btle_error_codes_module_wt Write Task
 * \{
 */

#define WT_MUTEX_INIT_FAILED                    \
        (MUTEX_INIT_FAILED | WT_ERR_ID)
#define WT_COND_INIT_FAILED                     \
        (COND_INIT_FAILED | WT_ERR_ID)
#define WT_MUTEX_LOCK_FAILED                    \
        (MUTEX_LOCK_FAILED | WT_ERR_ID)
#define WT_MUTEX_UNLOCK_FAILED                  \
        (MUTEX_UNLOCK_FAILED | WT_ERR_ID)
#define WT_MEMORY_ALLOCATION_FAILED             \
        (MEMORY_ALLOCATION_FAILED | WT_ERR_ID)

#define WT_NOT_RUNNING                          (0x0011U | WT_ERR_ID)
#define WT_QUEUE_FULL                           (0x0012U | WT_ERR_ID)
#define WT_QUEUE_INVALID                        (0x0013U | WT_ERR_ID)

/** \} */

/* ======================================= Section 'Timer' */
/**
 * \defgroup btle_error_codes_module_timer Timer
 * \{
 */

#define TIMER_MUTEX_INIT_FAILED                 \
        (MUTEX_INIT_FAILED | TIMER_ERR_ID)
#define TIMER_COND_INIT_FAILED                  \
        (COND_INIT_FAILED | TIMER_ERR_ID)
#define TIMER_MUTEX_LOCK_FAILED                 \
        (MUTEX_LOCK_FAILED | TIMER_ERR_ID)
#define TIMER_MUTEX_UNLOCK_FAILED               \
        (MUTEX_UNLOCK_FAILED | TIMER_ERR_ID)
#define TIMER_MEMORY_ALLOCATION_FAILED          \
        (MEMORY_ALLOCATION_FAILED | TIMER_ERR_ID)

#define TIMER_HANDLE_IS_NULL                    (0x0011U | TIMER_ERR_ID)
#define TIMER_CALLBACK_IS_NULL                  (0x0012U | TIMER_ERR_ID)
#define TIMER_QUEUE_EMPTY                       (0x0013U | TIMER_ERR_ID)
#define TIMER_QUEUE_FULL                        (0x0014U | TIMER_ERR_ID)
#define TIMER_ENTITY_SEARCH_FAILED              (0x0015U | TIMER_ERR_ID)
#define TIMER_NULL_PARAMETER_NOT_ALLOWED        (0x0016U | TIMER_ERR_ID)
#define TIMER_TIMEOUT_ZERO_NOT_ALLOWED          (0x0017U | TIMER_ERR_ID)

/** \} */

/* ======================================= Section 'RACP' */
/**
* \defgroup btle_error_codes_module_utils Utilities
* \{
*/

#define RACP_MUTEX_INIT_FAILED                    \
        (MUTEX_INIT_FAILED | RACP_ERR_ID)
#define RACP_COND_INIT_FAILED                     \
        (COND_INIT_FAILED | RACP_ERR_ID)
#define RACP_MUTEX_LOCK_FAILED                    \
        (MUTEX_LOCK_FAILED | RACP_ERR_ID)
#define RACP_MUTEX_UNLOCK_FAILED                  \
        (MUTEX_UNLOCK_FAILED | RACP_ERR_ID)
#define RACP_MEMORY_ALLOCATION_FAILED             \
        (MEMORY_ALLOCATION_FAILED | RACP_ERR_ID)
/** \} */

/**
 * \defgroup btle_error_codes_module_ht HCI Transport
 * \{
 */

#define HT_MUTEX_INIT_FAILED                    \
        (MUTEX_INIT_FAILED | HT_ERR_ID)
#define HT_COND_INIT_FAILED                     \
        (COND_INIT_FAILED | HT_ERR_ID)
#define HT_MUTEX_LOCK_FAILED                    \
        (MUTEX_LOCK_FAILED | HT_ERR_ID)
#define HT_MUTEX_UNLOCK_FAILED                  \
        (MUTEX_UNLOCK_FAILED | HT_ERR_ID)
#define HT_MEMORY_ALLOCATION_FAILED             \
        (MEMORY_ALLOCATION_FAILED | HT_ERR_ID)
 /** \} */

/* ======================================= Section 'HCI' */
/**
 * \defgroup btle_error_codes_module_hci HCI
 * \{
 */

#define HCI_MUTEX_INIT_FAILED                    \
        (MUTEX_INIT_FAILED | HCI_ERR_ID)
#define HCI_COND_INIT_FAILED                     \
        (COND_INIT_FAILED | HCI_ERR_ID)
#define HCI_MUTEX_LOCK_FAILED                    \
        (MUTEX_LOCK_FAILED | HCI_ERR_ID)
#define HCI_MUTEX_UNLOCK_FAILED                  \
        (MUTEX_UNLOCK_FAILED | HCI_ERR_ID)
#define HCI_MEMORY_ALLOCATION_FAILED             \
        (MEMORY_ALLOCATION_FAILED | HCI_ERR_ID)

#define HCI_INVALID_PARAMETER_VALUE             (0x0011U | HCI_ERR_ID)
#define HCI_PARAMETER_OUTSIDE_RANGE             (0x0012U | HCI_ERR_ID)
#define HCI_NULL_PARAMETER_NOT_ALLOWED          (0x0013U | HCI_ERR_ID)
#define HCI_CREATE_CONTEXT_FAILED               (0x0015U | HCI_ERR_ID)
#define HCI_SEND_COMMAND_FAILED                 (0x0016U | HCI_ERR_ID)
#define HCI_DEVICE_ASSERTION_FAILED             (0x0017U | HCI_ERR_ID)
#define HCI_STATE_NOT_CONNECTED                 (0x0018U | HCI_ERR_ID)
#define HCI_STATE_NOT_DISCONNECTED              (0x0019U | HCI_ERR_ID)
#define HCI_STATE_ALREADY_CONNECTED             (0x001AU | HCI_ERR_ID)
#define HCI_STATE_ALREADY_DISCONNECTED          (0x001BU | HCI_ERR_ID)
#define HCI_INVALID_COMMAND_LENGTH              (0x001CU | HCI_ERR_ID)
#define HCI_HOST_CONTROLLER_RESET_FAILED        (0x001DU | HCI_ERR_ID)
#define HCI_DEVICE_ATTR_ADDITION_FAILED         (0x001EU | HCI_ERR_ID)
#define HCI_CREATE_CONNECTION_ENTITY_FAILED     (0x001FU | HCI_ERR_ID)
#define HCI_CONNECTION_ENTITY_ASSERT_FAILED     (0x0020U | HCI_ERR_ID)
#define HCI_TX_BUFFER_FULL                      (0x0021U | HCI_ERR_ID)
#define HCI_ACL_LINK_IN_USE_BY_OTHER_SERVICE    (0x0022U | HCI_ERR_ID)
#define HCI_COMMAND_QUEUE_FULL                  (0x0023U | HCI_ERR_ID)
#define HCI_INVALID_COMMAND_PACKET              (0x0024U | HCI_ERR_ID)
#define HCI_NO_ACTIVE_SCO_CONNECTION            (0x0025U | HCI_ERR_ID)
#define HCI_ESCO_REQ_RETURN_ENH_SYNCHRONOUS_ACCEPT \
                                                (0x0026U | HCI_ERR_ID)
#define HCI_ESCO_REQ_RETURN_NO_AUTO_RESPONSE    (0x0027U | HCI_ERR_ID)
#define HCI_RX_INVALID_PKT_TYPE                 (0x0028U | HCI_ERR_ID)
#define HCI_ISO_DATA_QUEUE_FULL                 (0x0029U | HCI_ERR_ID)
#define HCI_LE_CONNECTION_IN_PROGRESS           (0x002AU | HCI_ERR_ID)
#define HCI_INVALID_CONNECTION_ADDR             (0x002BU | HCI_ERR_ID)

#define HCI_API_NOT_SUPPORTED                   (0x00FFU | HCI_ERR_ID)

 /** \} */

/* ======================================= Section 'SM' */
#define SM_MUTEX_INIT_FAILED                    \
        (MUTEX_INIT_FAILED | SM_ERR_ID)
#define SM_COND_INIT_FAILED                     \
        (COND_INIT_FAILED | SM_ERR_ID)
#define SM_MUTEX_LOCK_FAILED                    \
        (MUTEX_LOCK_FAILED | SM_ERR_ID)
#define SM_MUTEX_UNLOCK_FAILED                  \
        (MUTEX_UNLOCK_FAILED | SM_ERR_ID)
#define SM_MEMORY_ALLOCATION_FAILED             \
        (MEMORY_ALLOCATION_FAILED | SM_ERR_ID)

#define SM_INVALID_PARAMETERS                   (0x0011U | SM_ERR_ID)
#define SM_LINK_KEY_NOT_AVAILABLE               (0x0012U | SM_ERR_ID)
#define SM_NO_ACL_CONNECTION                    (0x0013U | SM_ERR_ID)
#define SM_DEVICE_ENCRYPTED                     (0x0014U | SM_ERR_ID)
#define SM_ACL_LINK_NOT_AUTHENTICATED           (0x0015U | SM_ERR_ID)
#define SM_CONTEXT_QUEUE_FULL                   (0x0016U | SM_ERR_ID)
#define SM_OPERATION_DISALLOWED_ACTIVE_ACL      (0x0017U | SM_ERR_ID)

#define SM_NO_DEVICE_ENTRY                      (0x0020U | SM_ERR_ID)
#define SM_DEVICE_ENTRY_CREATE_FAILED           (0x0021U | SM_ERR_ID)
#define SM_DEVICE_ENTRY_PURGE_FAILED            (0x0022U | SM_ERR_ID)

#define SM_NO_SERVICE_ENTRY                     (0x0030U | SM_ERR_ID)
#define SM_SERVICE_ENTRY_EXISTS                 (0x0031U | SM_ERR_ID)
#define SM_SERVICE_ENTRY_CREATE_FAILED          (0x0032U | SM_ERR_ID)

#define SM_ACCESS_GRANTED                       API_SUCCESS
#define SM_ENCRYPTION_PENDING                   (0x0041U | SM_ERR_ID)
#define SM_AUTHORIZATION_PENDING                (0x0042U | SM_ERR_ID)
#define SM_AUTHENTICATION_PENDING               (0x0043U | SM_ERR_ID)
#define SM_ENCRYPTION_FAILED                    (0x0044U | SM_ERR_ID)
#define SM_AUTHORIZATION_FAILED                 (0x0045U | SM_ERR_ID)
#define SM_AUTHENTICATION_FAILED                (0x0046U | SM_ERR_ID)

#define HCI_ACCESS_GRANTED                      SM_ACCESS_GRANTED
#define HCI_ENCRYPTION_PENDING                  SM_ENCRYPTION_PENDING
#define HCI_AUTHORIZATION_PENDING               SM_AUTHORIZATION_PENDING
#define HCI_AUTHENTICATION_PENDING              SM_AUTHENTICATION_PENDING
#define HCI_ENCRYPTION_FAILED                   SM_ENCRYPTION_FAILED
#define HCI_AUTHORIZATION_FAILED                SM_AUTHORIZATION_FAILED
#define HCI_AUTHENTICATION_FAILED               SM_AUTHENTICATION_FAILED


/* ======================================= Section 'DM' */
/**
 * \defgroup btle_error_codes_module_dm Device Manager
 * \{
 */

#define DM_MUTEX_INIT_FAILED                    \
        (MUTEX_INIT_FAILED | DM_ERR_ID)
#define DM_COND_INIT_FAILED                     \
        (COND_INIT_FAILED | DM_ERR_ID)
#define DM_MUTEX_LOCK_FAILED                    \
        (MUTEX_LOCK_FAILED | DM_ERR_ID)
#define DM_MUTEX_UNLOCK_FAILED                  \
        (MUTEX_UNLOCK_FAILED | DM_ERR_ID)
#define DM_MEMORY_ALLOCATION_FAILED             \
        (MEMORY_ALLOCATION_FAILED | DM_ERR_ID)

#define DM_DEVICE_NOT_FOUND                     (0x0011U | DM_ERR_ID)
#define DM_INVALID_PARAMETER                    (0x0012U | DM_ERR_ID)
#define DM_ACL_DISCONNECT_REJECT                (0x0013U | DM_ERR_ID)
#define DM_INVALID_PROFILE_TYPE                 (0x0014U | DM_ERR_ID)

/** \} */

/* ======================================= Section 'L2CAP' */
/**
 * \defgroup btle_error_codes_module_l2cap L2CAP
 * \{
 */

#define L2CAP_MUTEX_INIT_FAILED                 \
        (MUTEX_INIT_FAILED | L2CAP_ERR_ID)
#define L2CAP_COND_INIT_FAILED                  \
        (COND_INIT_FAILED | L2CAP_ERR_ID)
#define L2CAP_MUTEX_LOCK_FAILED                 \
        (MUTEX_LOCK_FAILED | L2CAP_ERR_ID)
#define L2CAP_MUTEX_UNLOCK_FAILED               \
        (MUTEX_UNLOCK_FAILED | L2CAP_ERR_ID)
#define L2CAP_MEMORY_ALLOCATION_FAILED          \
        (MEMORY_ALLOCATION_FAILED | L2CAP_ERR_ID)

#define L2CAP_INVALID_LMP_STATE                 (0x0011U | L2CAP_ERR_ID)
#define L2CAP_API_INVALID_PARAMS                (0x0012U | L2CAP_ERR_ID)
#define L2CAP_BUFFER_ALLOCATION_FAILED          (0x0013U | L2CAP_ERR_ID)
#define L2CAP_STATE_INCORRECT_FOR_OPERATION     (0x0014U | L2CAP_ERR_ID)
#define L2CAP_INVALID_ECHO_DATA_LENGTH          (0x0015U | L2CAP_ERR_ID)
#define L2CAP_NULL_VALUE_NOT_ACCEPTABLE         (0x0016U | L2CAP_ERR_ID)
#define L2CAP_INSUFFICIENT_BUFFER_SPACE         (0x0017U | L2CAP_ERR_ID)
#define L2CAP_COMMAND_TIMEOUT                   (0x0018U | L2CAP_ERR_ID)

#define L2CAP_PSM_WRONG_ENCODING                (0x0020U | L2CAP_ERR_ID)
#define L2CAP_PSM_HAS_ACTIVE_CID                (0x0021U | L2CAP_ERR_ID)
#define L2CAP_PSM_NOT_REGISTERED                (0x0022U | L2CAP_ERR_ID)
#define L2CAP_PSM_ALREADY_REGISTERED            (0x0024U | L2CAP_ERR_ID)
#define L2CAP_PSM_ENTITY_ALLOC_FAILED           (0x0025U | L2CAP_ERR_ID)
#define L2CAP_RTX_TIMER_START_FAILED            (0x0026U | L2CAP_ERR_ID)
#define L2CAP_MTU_CHECK_FAILED                  (0x0027U | L2CAP_ERR_ID)
#define L2CAP_UNACCEPTABLE_MTU_VALUE            (0x0028U | L2CAP_ERR_ID)
#define L2CAP_MTU_REQUESTED_LT_MIN_MTU          (0x0029U | L2CAP_ERR_ID)
#define L2CAP_CONNECTION_ENTITY_NOT_FOUND       (0x002AU | L2CAP_ERR_ID)
#define L2CAP_CONNECTION_ENTITY_ALLOC_FAILED    (0x002BU | L2CAP_ERR_ID)
#define L2CAP_SIGNALLING_ENTITY_NOT_FOUND       (0x002CU | L2CAP_ERR_ID)
#define L2CAP_SIGNALLING_ENTITY_EXISTS          (0x002DU | L2CAP_ERR_ID)
#define L2CAP_COMMAND_ENTITY_NOT_FOUND          (0x002EU | L2CAP_ERR_ID)
#define L2CAP_COMMAND_ENTITY_ALLOC_FAILED       (0x002FU | L2CAP_ERR_ID)
#define L2CAP_INVALID_CONFIG_TIMEOUT            (0x0030U | L2CAP_ERR_ID)
#define L2CAP_LOCAL_CONFIG_IN_PROGRESS          (0x0031U | L2CAP_ERR_ID)
#define L2CAP_GROUP_CONNECTION_ENTITY_NOT_FOUND (0x0032U | L2CAP_ERR_ID)
#define L2CAP_GROUP_FOR_PSM_EXISTS              (0x0033U | L2CAP_ERR_ID)
#define L2CAP_GROUP_MEMBER_EXISTS               (0x0034U | L2CAP_ERR_ID)
#define L2CAP_GROUP_HAS_NO_DEVICE               (0x0035U | L2CAP_ERR_ID)
#define L2CAP_FLOW_SPEC_BLOCK_ALLOC_FAILED      (0x0036U | L2CAP_ERR_ID)
#define L2CAP_LP_DISCONNECTED                   (0x0037U | L2CAP_ERR_ID)
#define L2CAP_CID_DISCONNECTED                  (0x0038U | L2CAP_ERR_ID)

#define L2CAP_FEC_OPTIONS_NULL                  (0x0040U | L2CAP_ERR_ID)
#define L2CAP_UNSUPPORTED_PEER_FEC_MODE         (0x0041U | L2CAP_ERR_ID)
#define L2CAP_FEC_ENTITY_ALLOCATION_FAILED      (0x0042U | L2CAP_ERR_ID)
#define L2CAP_FEC_TX_WINDOW_FULL                (0x0043U | L2CAP_ERR_ID)
#define L2CAP_FEC_DATA_WRITE_FAILURE            (0x0044U | L2CAP_ERR_ID)
#define L2CAP_PSM_WRONG_FEC_SUPPORT             (0x0045U | L2CAP_ERR_ID)
#define L2CAP_INCONSISTENT_FEC_MODE             (0x0046U | L2CAP_ERR_ID)
#define L2CAP_INCORRECT_SDU_LENGTH              (0x0047U | L2CAP_ERR_ID)
#define L2CAP_FEC_I_FRAME_MAX_TRANSMIT          (0x0048U | L2CAP_ERR_ID)
#define L2CAP_FEC_INVALID_REQ_SEQ               (0x0049U | L2CAP_ERR_ID)
#define L2CAP_FEC_UNACCEPTABLE_MPS_VALUE        (0x0050U | L2CAP_ERR_ID)
#define L2CAP_FEC_UNACCEPTABLE_TX_WINDOW_VALUE  (0x0051U | L2CAP_ERR_ID)
#define L2CAP_UNACCEPTABLE_FCS_VALUE            (0x0052U | L2CAP_ERR_ID)
#define L2CAP_FEC_OPTIONS_NOT_RENEGOTIATABLE    (0x0053U | L2CAP_ERR_ID)

#define L2CAP_NO_LMP_CONNECTION                 (0x0054U | L2CAP_ERR_ID)
#define L2CAP_API_NOT_ALLOWED                   (0x0055U | L2CAP_ERR_ID)
#define L2CAP_TX_FLOW_OFF                       (0x0056U | L2CAP_ERR_ID)

#define L2CAP_CHANNEL_NOT_FOUND                 (0x0070U | L2CAP_ERR_ID)
#define L2CAP_NOT_ENOUGH_CREDITS                (0x0071U | L2CAP_ERR_ID)
#define L2CAP_CHANNEL_INVALID_STATE             (0x0072U | L2CAP_ERR_ID)
#define L2CAP_CREDIT_OVERFLOW                   (0x0073U | L2CAP_ERR_ID)
#define L2CAP_UNACCEPTABLE_CREDIT_VALUE         (0x0074U | L2CAP_ERR_ID)
#define L2CAP_INVALID_CHANNEL_PARAM             (0x0081U | L2CAP_ERR_ID)
#define L2CAP_CBFC_PSM_NOT_IN_RANGE             (0x0082U | L2CAP_ERR_ID)
#define L2CAP_TX_QUEUE_NOT_EMPTY                (0x0083U | L2CAP_ERR_ID)
#define L2CAP_CBFC_TX_ALREADY_IN_PROGRESS       (0x0084U | L2CAP_ERR_ID)
#define L2CAP_ECBFC_PSM_NOT_IN_RANGE            (0x0085U | L2CAP_ERR_ID)
#define L2CAP_CBFC_COMMAND_REJECTED             (0x0086U | L2CAP_ERR_ID)
#define L2CAP_CHANNEL_DUPLICATE_REMOTE_CID      (0x0087U | L2CAP_ERR_ID)

#define L2CAP_API_NOT_SUPPORTED                 (0x00F0U | L2CAP_ERR_ID)

/** \} */

/* ======================================= Section 'SMP' */

/**
 * \defgroup btle_error_codes_module_smp SMP
 * \{
 */

/* SMP Specification defiend Error Codes */
#define SMP_LE_PASSKEY_ENTRY_FAILED              (0x01U | SM_LE_ERR_ID)
#define SMP_LE_OOB_DATA_NOT_AVAILABLE            (0x02U | SM_LE_ERR_ID)
#define SMP_LE_AUTHENTICATION_REQ_NOT_MET        (0x03U | SM_LE_ERR_ID)
#define SMP_LE_CONFIRM_VALUE_NOT_MATCH           (0x04U | SM_LE_ERR_ID)
#define SMP_LE_PAIRING_NOT_SUPPORTED             (0x05U | SM_LE_ERR_ID)
#define SMP_LE_INSUFFICIENT_ENCRYPTION_KEY_SIZE  (0x06U | SM_LE_ERR_ID)
#define SMP_LE_COMMAND_NOT_SUPPORTED             (0x07U | SM_LE_ERR_ID)
#define SMP_LE_UNSPECIFIED_REASON                (0x08U | SM_LE_ERR_ID)
#define SMP_LE_REPEATED_ATTEMTS                  (0x09U | SM_LE_ERR_ID)
#define SMP_LE_INVALID_PARAMETERS                (0x0AU | SM_LE_ERR_ID)
#define SMP_LE_DHKEY_CHECK_FAILED                (0x0BU | SM_LE_ERR_ID)
#define SMP_LE_NUMERIC_COMPARISON_FAILED         (0x0CU | SM_LE_ERR_ID)
#define SMP_LE_BREDR_PAIRING_IN_PROGRESS         (0x0DU | SM_LE_ERR_ID)
#define SMP_LE_CT_KEY_GEN_NOT_ALLOWED            (0x0EU | SM_LE_ERR_ID)

/* SMP Internal error code */
#define SMP_MUTEX_INIT_FAILED                   \
        (MUTEX_INIT_FAILED | SMP_ERR_ID)
#define SMP_COND_INIT_FAILED                    \
        (COND_INIT_FAILED | SMP_ERR_ID)
#define SMP_MUTEX_LOCK_FAILED                   \
        (MUTEX_LOCK_FAILED | SMP_ERR_ID)
#define SMP_MUTEX_UNLOCK_FAILED                 \
        (MUTEX_UNLOCK_FAILED | SMP_ERR_ID)
#define SMP_MEMORY_ALLOCATION_FAILED            \
        (MEMORY_ALLOCATION_FAILED | SMP_ERR_ID)

#define SMP_INVALID_PARAMETERS                         (0x0011U | SMP_ERR_ID)
#define SMP_NO_DEVICE_ENTITY                           (0x0012U | SMP_ERR_ID)
#define SMP_CONTEXT_TABLE_FULL                         (0x0013U | SMP_ERR_ID)
#define SMP_CONTEXT_NOT_FOUND                          (0x0014U | SMP_ERR_ID)
#define SMP_AUTHENTICATION_TIMEOUT                     (0x0015U | SMP_ERR_ID)
#define SMP_INCORRECT_STATE                            (0x0016U | SMP_ERR_ID)
#define SMP_OPERATION_IN_PROGRESS                      (0x0017U | SMP_ERR_ID)
#define SMP_LINK_DISCONNECTED                          (0x0018U | SMP_ERR_ID)
#define SMP_PROCEDURE_FAILED                           (0x0019U | SMP_ERR_ID)
#define SMP_DEVICE_ENTRY_PURGE_FAILED                  (0x001AU | SMP_ERR_ID)

#define SMP_DISPLAY_PASSKEY_UPDATED                    (0x00A0U | SMP_ERR_ID)
#define SMP_REMOTE_SIDE_PIN_KEY_MISSING                (0x00A1U | SMP_ERR_ID)
/** \} */

/* ======================================= Section 'ATT' */
/**
 * \defgroup btle_error_codes_module_att ATT
 * \{
 */

#define ATT_MUTEX_INIT_FAILED                   \
        (MUTEX_INIT_FAILED | ATT_ERR_ID)
#define ATT_COND_INIT_FAILED                    \
        (COND_INIT_FAILED | ATT_ERR_ID)
#define ATT_MUTEX_LOCK_FAILED                   \
        (MUTEX_LOCK_FAILED | ATT_ERR_ID)
#define ATT_MUTEX_UNLOCK_FAILED                 \
        (MUTEX_UNLOCK_FAILED | ATT_ERR_ID)
#define ATT_ERR_MEM_ALLOCATION_FAILED            \
        (MEMORY_ALLOCATION_FAILED | ATT_ERR_ID)

/* ATT Common Error Codes */
#define ATT_NULL_PARAMETER_NOT_ALLOWED                 (0x0011U | ATT_ERR_ID)
#define ATT_UNKNOWN_PDU_TYPE                           (0x0012U | ATT_ERR_ID)
#define ATT_INVALID_INSTANCE                           (0x0013U | ATT_ERR_ID)
#define ATT_INVALID_OPERATION                          (0x0014U | ATT_ERR_ID)
#define ATT_INVLAID_DB_HANDLE                          (0x0015U | ATT_ERR_ID)
#define ATT_INVALID_PARAMETER                          (0x0016U | ATT_ERR_ID)
#define ATT_RESPONSE_TIMED_OUT                         (0x0017U | ATT_ERR_ID)
#define ATT_UNEXPECTED_PDU                             (0x0018U | ATT_ERR_ID)
#define ATT_NO_FREE_INSTANCE                           (0x0019U | ATT_ERR_ID)
#define ATT_INVALID_PARAM_LEN                          (0x0020U | ATT_ERR_ID)
#define ATT_DEVICE_NOT_FOUND                           (0x0040U | ATT_ERR_ID)
#define ATT_LCID_NOT_FOUND                             (0x0041U | ATT_ERR_ID)
#define ATT_L2CAP_PROCEDURE_FAILED                     (0x0042U | ATT_ERR_ID)
#define ATT_LCID_DISCONNECTED                          (0x0043U | ATT_ERR_ID)
#define ATT_CONNECTION_ALREADY_EXISTS                  (0x0044U | ATT_ERR_ID)

/** \} */

/* ======================================= Section 'GATT DB' */
/**
 * \defgroup btle_error_codes_module_gatt_db GATT DB
 * \{
 */

#define GATT_DB_MUTEX_INIT_FAILED                               \
        (MUTEX_INIT_FAILED | GATT_DB_ERR_ID)
#define GATT_DB_COND_INIT_FAILED                                \
        (COND_INIT_FAILED | GATT_DB_ERR_ID)
#define GATT_DB_MUTEX_LOCK_FAILED                               \
        (MUTEX_LOCK_FAILED | GATT_DB_ERR_ID)
#define GATT_DB_MUTEX_UNLOCK_FAILED                             \
        (MUTEX_UNLOCK_FAILED | GATT_DB_ERR_ID)
#define GATT_DB_MEMORY_ALLOCATION_FAILED                        \
        (MEMORY_ALLOCATION_FAILED | GATT_DB_ERR_ID)

#define GATT_DB_INVALID_OFFSET                   (0x0007U | GATT_DB_ERR_ID)
#define GATT_DB_NULL_PARAMETER_NOT_ALLOWED       (0x0011U | GATT_DB_ERR_ID)
#define GATT_DB_UNSUPPORTED_GROUP_TYPE           (0x0012U | GATT_DB_ERR_ID)
#define GATT_DB_INSUFFICIENT_BUFFER_LEN          (0x0013U | GATT_DB_ERR_ID)
#define GATT_DB_MORE_MATCHING_RESULT_FOUND       (0x0014U | GATT_DB_ERR_ID)
#define GATT_DB_NO_MATCHING_RESULT               (0x0015U | GATT_DB_ERR_ID)
#define GATT_DB_HANDLE_NOT_FOUND                 (0x0016U | GATT_DB_ERR_ID)
#define GATT_DB_HANDLE_NOT_IN_RANGE              (0x0017U | GATT_DB_ERR_ID)
#define GATT_DB_HANDLE_IN_GROUP_RANGE            (0x0018U | GATT_DB_ERR_ID)
#define GATT_DB_INVALID_OPERATION                (0x0019U | GATT_DB_ERR_ID)
#define GATT_DB_INVALID_VALUE_LENGTH             (0x001AU | GATT_DB_ERR_ID)
#define GATT_DB_UUID_NOT_IN_BT_SPACE             (0x0020U | GATT_DB_ERR_ID)
#define GATT_DB_INVALID_ATTR_HANDLE              (0x0021U | GATT_DB_ERR_ID)
#define GATT_DB_INSUFFICIENT_SECURITY            (0x0022U | GATT_DB_ERR_ID)
#define GATT_DB_INSUFFICIENT_ENC_KEY_SIZE        (0x0023U | GATT_DB_ERR_ID)
#define GATT_DB_INVALID_INSTANCE                 (0x0024U | GATT_DB_ERR_ID)
#define GATT_DB_INCORRECT_UUID_FRMT              (0x0025U | GATT_DB_ERR_ID)
#define GATT_DB_UUID_FRMT_UNSUPPORTED            (0x0026U | GATT_DB_ERR_ID)
#define GATT_DB_TYPE_MISMATCH                    (0x0027U | GATT_DB_ERR_ID)
#define GATT_DB_INSUFFICIENT_ENCRYPTION          (0x0028U | GATT_DB_ERR_ID)
#define GATT_DB_NON_EDITABLE_ATTR_VALUE          (0x0029U | GATT_DB_ERR_ID)
#define GATT_DB_INSUFFICIENT_AUTHORIZATION       (0x002AU | GATT_DB_ERR_ID)
#define GATT_DB_INVALID_TRANSPORT_ACCESS         (0x002BU | GATT_DB_ERR_ID)
#define GATT_DB_INVALID_PEER_SPECIFIC_ACCESS     (0x002CU | GATT_DB_ERR_ID)
#define GATT_DB_INVALID_UL_ATTR_VALUE_ACCESS     (0x002DU | GATT_DB_ERR_ID)
#define GATT_DB_HASH_CALC_ALREADY_IN_PROGRESS    (0x002EU | GATT_DB_ERR_ID)
#define GATT_DB_API_NOT_IMPLEMENTED              (0x0050U | GATT_DB_ERR_ID)
#define GATT_DB_DYN_FULL                         (0x0058U | GATT_DB_ERR_ID)
#define GATT_DB_DYN_MAX_NUM_SERVICES             (0x0059U | GATT_DB_ERR_ID)
#define GATT_DB_DYN_MAX_NUM_CHARACTERISTICS      (0x005AU | GATT_DB_ERR_ID)
#define GATT_DB_DYN_MAX_NUM_ATTRIBUTES           (0x005BU | GATT_DB_ERR_ID)
/** GATT DB Attribute Handle Specified is currently Disabled */
#define GATT_DB_ATTR_HANDLE_DISABLED             (0x005CU | GATT_DB_ERR_ID)
/** GATT DB Service Identifier Specified is Invalid */
#define GATT_DB_INVALID_SERV_IDENTIFIER          (0x005DU | GATT_DB_ERR_ID)
/** GATT DB Characteristic Identifier Specified is Invalid */
#define GATT_DB_INVALID_CHAR_IDENTIFIER          (0x005EU | GATT_DB_ERR_ID)

/* Not an error code. More of a status code */
/** GATT DB status code to inform Delayed Response */
#define GATT_DB_DELAYED_RESPONSE                 (0x0060U | GATT_DB_ERR_ID)
/** GATT DB status code to inform not to Respond */
#define GATT_DB_DONOT_RESPOND                    (0x0061U | GATT_DB_ERR_ID)
/** GATT DB status code to inform Response is already triggered */
#define GATT_DB_ALREADY_RESPONDED                (0x0062U | GATT_DB_ERR_ID)

/** \} */

/**
 * \addtogroup btle_error_codes_module_utils Utilities
 * \{
 */

/* ======================================= Section 'FSM' */
#define FSM_UNKNOWN_EVENT_TYPE                   (0x0011U | FSM_ERR_ID)

/* ======================================= Section 'Device Queue' */
#define DQ_MUTEX_INIT_FAILED                    \
        (MUTEX_INIT_FAILED | DQ_ERR_ID)
#define DQ_COND_INIT_FAILED                     \
        (COND_INIT_FAILED | DQ_ERR_ID)
#define DQ_MUTEX_LOCK_FAILED                    \
        (MUTEX_LOCK_FAILED | DQ_ERR_ID)
#define DQ_MUTEX_UNLOCK_FAILED                  \
        (MUTEX_UNLOCK_FAILED | DQ_ERR_ID)
#define DQ_MEMORY_ALLOCATION_FAILED             \
        (MEMORY_ALLOCATION_FAILED | DQ_ERR_ID)

#define DQ_NULL_PARAMETER_NOT_ALLOWED           (0x0011U | DQ_ERR_ID)
#define DQ_DEVICE_ALREADY_ENQUEUED              (0x0012U | DQ_ERR_ID)
#define DQ_QUEUE_FULL                           (0x0013U | DQ_ERR_ID)
#define DQ_DEVICE_NOT_FOUND                     (0x0014U | DQ_ERR_ID)
#define DQ_INVALID_HANDLE                       (0x0015U | DQ_ERR_ID)
#define DQ_DEVICE_IN_RESOLUTION                 (0x0016U | DQ_ERR_ID)

/* ======================================= Section 'Storage' */
/* Storage Internal error code */
#define STORAGE_MUTEX_INIT_FAILED                   \
        (MUTEX_INIT_FAILED | STORAGE_ERR_ID)
#define STORAGE_COND_INIT_FAILED                    \
        (COND_INIT_FAILED | STORAGE_ERR_ID)
#define STORAGE_MUTEX_LOCK_FAILED                   \
        (MUTEX_LOCK_FAILED | STORAGE_ERR_ID)
#define STORAGE_MUTEX_UNLOCK_FAILED                 \
        (MUTEX_UNLOCK_FAILED | STORAGE_ERR_ID)
#define STORAGE_MEMORY_ALLOCATION_FAILED            \
        (MEMORY_ALLOCATION_FAILED | STORAGE_ERR_ID)

#define STORAGE_INVALID_PARAMETERS              (0x0011U | STORAGE_ERR_ID)
#define STORAGE_OPERATION_FAILED                (0x0012U | STORAGE_ERR_ID)
#define STORAGE_OPERATION_MASKED                (0x0013U | STORAGE_ERR_ID)

/** \} */

/* ======================================= Section 'SDP' */
#define SDP_MUTEX_INIT_FAILED                   \
        (MUTEX_INIT_FAILED | SDP_ERR_ID)
#define SDP_COND_INIT_FAILED                    \
        (COND_INIT_FAILED | SDP_ERR_ID)
#define SDP_MUTEX_LOCK_FAILED                   \
        (MUTEX_LOCK_FAILED | SDP_ERR_ID)
#define SDP_MUTEX_UNLOCK_FAILED                 \
        (MUTEX_UNLOCK_FAILED | SDP_ERR_ID)
#define SDP_MEMORY_ALLOCATION_FAILED            \
        (MEMORY_ALLOCATION_FAILED | SDP_ERR_ID)

/* SDP Error Response Messages */
#define SDP_BDADDR_ALREADY_ACTIVE               (0x0011U | SDP_ERR_ID)
#define SDP_INVALID_INPUT_PARAMETERS            (0x0012U | SDP_ERR_ID)
#define SDP_UNSUPPORTED_ELEMENT_TYPE            (0x0013U | SDP_ERR_ID)
#define SDP_UNSUPPORTED_ELEMENT_SIZE            (0x0014U | SDP_ERR_ID)
#define SDP_INCORRECT_TRANSACTION_ID            (0x0015U | SDP_ERR_ID)
#define SDP_ATTRIBUTE_VALUE_NOT_FOUND           (0x0016U | SDP_ERR_ID)

#define SDP_CID_IN_USE                          (0x0020U | SDP_ERR_ID)
#define SDP_CONNECTION_ENTITY_NOT_FOUND         (0x0021U | SDP_ERR_ID)
#define SDP_L2CAP_CHANNEL_NOT_FOUND             (0x0022U | SDP_ERR_ID)
#define SDP_UNKNOWN_STATE_ERROR                 (0x0023U | SDP_ERR_ID)
#define SDP_PACKET_HEADER_CORRUPTED             (0x0024U | SDP_ERR_ID)
#define SDP_DE_NOT_UNIQUE_IN_LIST               (0x0025U | SDP_ERR_ID)
#define SDP_INSUFFICIENT_LENGTH                 (0x0026U | SDP_ERR_ID)
#define SDP_USER_CONTEXT_NOT_FOUND              (0x0027U | SDP_ERR_ID)
#define SDP_CLIENT_FLAG_DISABLED                (0x0028U | SDP_ERR_ID)
#define SDP_L2CAP_CONNECT_FAILED                (0x0029U | SDP_ERR_ID)
#define SDP_L2CAP_CONFIG_FAILED                 (0x002AU | SDP_ERR_ID)
#define SDP_INCORRECT_PDU_ID                    (0x002BU | SDP_ERR_ID)
#define SDP_INCORRECT_SERVICE_RECORD_COUNT      (0x002CU | SDP_ERR_ID)
#define SDP_INVALID_CONT_STATE_INFO             (0x002DU | SDP_ERR_ID)
#define SDP_ATTRIBUTE_ID_NOT_ASCENDING          (0x002EU | SDP_ERR_ID)
#define SDP_ATTRIBUTE_RANGE_NOT_ASCENDING       (0x002FU | SDP_ERR_ID)
#define SDP_EMPTY_SERVICE_RECORD_COUNT          (0x0030U | SDP_ERR_ID)
#define SDP_MUTEX_OPERATION_FAILED              (0x0031U | SDP_ERR_ID)
#define SDP_VALID_CS_EXISTS                     (0x0032U | SDP_ERR_ID)
#define SDP_SERVER_FLAG_DISABLED                (0x0033U | SDP_ERR_ID)
#define SDP_L2CAP_CHANNEL_DISCONNECTED          (0x0034U | SDP_ERR_ID)
#define SDP_RECEIVED_PARTIAL_RESPONSE           (0x0035U | SDP_ERR_ID)


/* ======================================= Section 'DB' */
#define DB_MUTEX_INIT_FAILED                    \
        (MUTEX_INIT_FAILED | DB_ERR_ID)
#define DB_COND_INIT_FAILED                     \
        (COND_INIT_FAILED | DB_ERR_ID)
#define DB_MUTEX_LOCK_FAILED                    \
        (MUTEX_LOCK_FAILED | DB_ERR_ID)
#define DB_MUTEX_UNLOCK_FAILED                  \
        (MUTEX_UNLOCK_FAILED | DB_ERR_ID)
#define DB_MEMORY_ALLOCATION_FAILED             \
        (MEMORY_ALLOCATION_FAILED | DB_ERR_ID)

#define DB_RECORD_HANDLE_NOT_FOUND              (0x0011U | DB_ERR_ID)
#define DB_ATTRIBUTE_NOT_SUPPORTED              (0x0012U | DB_ERR_ID)
#define DB_MAX_RECORDS_OVERFLOW                 (0x0013U | DB_ERR_ID)
#define DB_INSUFFICIENT_BUFFER_LEN              (0x0014U | DB_ERR_ID)
#define DB_ATTR_LEN_INVALID                     (0x0015U | DB_ERR_ID)
#define DB_INSUFFICIENT_ATTR_LEN                (0x0016U | DB_ERR_ID)
#define DB_RECORD_HANDLE_NOT_ENABLED            (0x0017U | DB_ERR_ID)
#define DB_SERVER_CHANNEL_NOT_FOUND             (0x0018U | DB_ERR_ID)
#define DB_NO_MATCH_FOR_UUID_SEARCH_PATTERN     (0x0019U | DB_ERR_ID)
#define DB_NULL_PARAM_NOT_ALLOWED               (0x001AU | DB_ERR_ID)
#define DB_MORE_SERVICE_CLASS_UUIDS_AVAILABLE   (0x001BU | DB_ERR_ID)
#define DB_INVALID_ATTR_ID                      (0x001CU | DB_ERR_ID)
#define DB_PSM_NOT_FOUND                        (0x001DU | DB_ERR_ID)


/* ======================================= Section 'RFCOMM' */
#define RFCOMM_MUTEX_INIT_FAILED                \
        (MUTEX_INIT_FAILED | RFCOMM_ERR_ID)
#define RFCOMM_COND_INIT_FAILED                 \
        (COND_INIT_FAILED | RFCOMM_ERR_ID)
#define RFCOMM_MUTEX_LOCK_FAILED                \
        (MUTEX_LOCK_FAILED | RFCOMM_ERR_ID)
#define RFCOMM_MUTEX_UNLOCK_FAILED              \
        (MUTEX_UNLOCK_FAILED | RFCOMM_ERR_ID)
#define RFCOMM_MEMORY_ALLOCATION_FAILED         \
        (MEMORY_ALLOCATION_FAILED | RFCOMM_ERR_ID)

#define RFCOMM_INVALID_PARAMETER_VALUE          (0x0011U | RFCOMM_ERR_ID)
#define RFCOMM_NULL_PARAMETER_NOT_ALLOWED       (0x0012U | RFCOMM_ERR_ID)
#define RFCOMM_NOTIFY_CB_NOT_PRESENT            (0x0013U | RFCOMM_ERR_ID)
#define RFCOMM_INVALID_SERVER_CHANNEL           (0x0014U | RFCOMM_ERR_ID)
#define RFCOMM_INVALID_PN_FRAME_SIZE            (0x0015U | RFCOMM_ERR_ID)
#define RFCOMM_INVALID_RPN_REQUEST_PARAM        (0x0016U | RFCOMM_ERR_ID)
#define RFCOMM_INVALID_DLCI_FOR_QUERY           (0x0017U | RFCOMM_ERR_ID)
#define RFCOMM_INVALID_DLCI_FOR_CREDIT_TX       (0x0018U | RFCOMM_ERR_ID)

#define RFCOMM_CONTEXT_ADDITION_FAILED          (0x0021U | RFCOMM_ERR_ID)
#define RFCOMM_SESSION_ADDITION_FAILED          (0x0022U | RFCOMM_ERR_ID)
#define RFCOMM_DLCI_ADDITION_FAILED             (0x0023U | RFCOMM_ERR_ID)
#define RFCOMM_TX_BUFFER_ADDITION_FAILED        (0x0024U | RFCOMM_ERR_ID)
#define RFCOMM_CONTEXT_SEARCH_FAILED            (0x0025U | RFCOMM_ERR_ID)
#define RFCOMM_SESSION_SEARCH_FAILED            (0x0026U | RFCOMM_ERR_ID)
#define RFCOMM_DLCI_SEARCH_FAILED               (0x0027U | RFCOMM_ERR_ID)

#define RFCOMM_SESSION_NOT_CONNECTED            (0x0031U | RFCOMM_ERR_ID)
#define RFCOMM_SESSION_NOT_DISCONNECTED         (0x0032U | RFCOMM_ERR_ID)
#define RFCOMM_DLCI_NOT_CONNECTED               (0x0033U | RFCOMM_ERR_ID)
#define RFCOMM_STATE_INCORRECT_FOR_OPEN         (0x0034U | RFCOMM_ERR_ID)
#define RFCOMM_STATE_INCORRECT_FOR_CLOSE        (0x0035U | RFCOMM_ERR_ID)
#define RFCOMM_STATE_INCORRECT_FOR_READ         (0x0036U | RFCOMM_ERR_ID)
#define RFCOMM_STATE_INCORRECT_FOR_WRITE        (0x0037U | RFCOMM_ERR_ID)
#define RFCOMM_STATE_INCORRECT_FOR_PN           (0x0038U | RFCOMM_ERR_ID)
#define RFCOMM_STATE_INCORRECT_FOR_RPN          (0x0039U | RFCOMM_ERR_ID)
#define RFCOMM_STATE_INCORRECT_FOR_MSC          (0x003AU | RFCOMM_ERR_ID)
#define RFCOMM_STATE_INCORRECT_FOR_RLS          (0x003BU | RFCOMM_ERR_ID)

#define RFCOMM_DLCI_CLOSED_BY_USER_ACTION       (0x0041U | RFCOMM_ERR_ID)
#define RFCOMM_DLCI_CLOSED_BY_DISC_COMMAND      (0x0042U | RFCOMM_ERR_ID)
#define RFCOMM_DLCI_CLOSED_BY_DM_RESPONSE       (0x0043U | RFCOMM_ERR_ID)
#define RFCOMM_DLCI_CLOSED_BY_L2CAP_DISCONNECT  (0x0044U | RFCOMM_ERR_ID)
#define RFCOMM_DLCI_CLOSED_BY_COMMAND_TIMEOUT   (0x0045U | RFCOMM_ERR_ID)
#define RFCOMM_DLCI_CLOSED_BY_SHUTDOWN          (0x0046U | RFCOMM_ERR_ID)

#define RFCOMM_ACCEPT_CANCELLED_BY_USER         (0x0051U | RFCOMM_ERR_ID)
#define RFCOMM_L2CAP_CONNECT_FAILED             (0x0052U | RFCOMM_ERR_ID)
#define RFCOMM_L2CAP_CONFIG_FAILED              (0x0053U | RFCOMM_ERR_ID)
#define RFCOMM_PN_CBFC_MISMATCH                 (0x0054U | RFCOMM_ERR_ID)
#define RFCOMM_PN_FRAME_SIZE_MISMATCH           (0x0055U | RFCOMM_ERR_ID)
#define RFCOMM_SESSION_FC_FAILED_CBFC_ENABLED   (0x0056U | RFCOMM_ERR_ID)
#define RFCOMM_TEST_RESPONSE_MISMATCH           (0x0057U | RFCOMM_ERR_ID)
#define RFCOMM_INVALID_TEST_DATA_LENGTH         (0x0058U | RFCOMM_ERR_ID)
#define RFCOMM_UNSUPPORTED_FEATURE_RESPONSE     (0x0059U | RFCOMM_ERR_ID)
#define RFCOMM_UKNOWN_MUX_FRAME_REQUESTED       (0x005AU | RFCOMM_ERR_ID)
#define RFCOMM_TX_QUEUE_NOT_EMPTY               (0x005BU | RFCOMM_ERR_ID)

#define RFCOMM_API_NOT_SUPPORTED                (0x00FFU | RFCOMM_ERR_ID)


/* ======================================= Section 'BNEP' */
#define BNEP_MUTEX_INIT_FAILED                  \
        (MUTEX_INIT_FAILED | BNEP_ERR_ID)
#define BNEP_COND_INIT_FAILED                   \
        (COND_INIT_FAILED | BNEP_ERR_ID)
#define BNEP_MUTEX_LOCK_FAILED                  \
        (MUTEX_LOCK_FAILED | BNEP_ERR_ID)
#define BNEP_MUTEX_UNLOCK_FAILED                \
        (MUTEX_UNLOCK_FAILED | BNEP_ERR_ID)
#define BNEP_MEMORY_ALLOCATION_FAILED           \
        (MEMORY_ALLOCATION_FAILED | BNEP_ERR_ID)

#define BNEP_NULL_VALUE_NOT_ALLOWED             (0x0011U | BNEP_ERR_ID)
#define BNEP_INVALID_PARAMETER_VALUE            (0x0012U | BNEP_ERR_ID)

#define BNEP_INVALID_BD_ADDR_NULL               (0x0013U | BNEP_ERR_ID)
#define BNEP_INVALID_REMOTE_SERVICE             (0x0014U | BNEP_ERR_ID)
#define BNEP_INVALID_BD_ADDR_IN_USE             (0x0015U | BNEP_ERR_ID)
#define BNEP_INVALID_LOCAL_SERVICE              (0x0016U | BNEP_ERR_ID)
#define BNEP_INVALID_SERVICES_FOR_SETUP         (0x0017U | BNEP_ERR_ID)

#define BNEP_ENTITY_ALLOCATION_FAILED           (0x0018U | BNEP_ERR_ID)
#define BNEP_ENTITY_ASSERTION_FAILED            (0x0019U | BNEP_ERR_ID)

#define BNEP_ALREADY_DISCONNECTED               (0x001AU | BNEP_ERR_ID)
#define BNEP_NOT_CONNECTED                      (0x001BU | BNEP_ERR_ID)
#define BNEP_STATE_INCORRECT_FOR_SETUP          (0x001CU | BNEP_ERR_ID)

#define BNEP_L2CAP_CONNECT_FAILED               (0x001DU | BNEP_ERR_ID)
#define BNEP_L2CAP_CONFIG_FAILED                (0x001EU | BNEP_ERR_ID)
#define BNEP_L2CAP_CHANNEL_DISCONNECTED         (0x001FU | BNEP_ERR_ID)

#define BNEP_PROTOCOL_DISALLOWED                (0x0020U | BNEP_ERR_ID)
#define BNEP_DEST_ADDRESS_DISALLOWED            (0x0021U | BNEP_ERR_ID)

#define BNEP_ETH_PAYLOAD_SIZE_INCONSISTENT      (0x0022U | BNEP_ERR_ID)
#define BNEP_ETH_PAYLOAD_SIZE_MORE_THAN_MAX     (0x0023U | BNEP_ERR_ID)

#define BNEP_NW_TYPE_FILTER_UNSUPPORTED         (0x0024U | BNEP_ERR_ID)
#define BNEP_NW_TYPE_FILTER_INVALID_RANGE       (0x0025U | BNEP_ERR_ID)
#define BNEP_NW_TYPE_FILTER_LIMIT_REACHED       (0x0026U | BNEP_ERR_ID)
#define BNEP_NW_TYPE_FILTER_SECURITY_BLOCK      (0x0027U | BNEP_ERR_ID)
#define BNEP_NW_TYPE_FILTER_PENDING_RESPONSE    (0x0028U | BNEP_ERR_ID)
#define BNEP_NW_TYPE_FILTER_MAX_RETX            (0x0029U | BNEP_ERR_ID)
#define BNEP_NW_TYPE_FILTER_UNKNOWN_ERROR       (0x002AU | BNEP_ERR_ID)

#define BNEP_MC_ADDR_FILTER_UNSUPPORTED         (0x0031U | BNEP_ERR_ID)
#define BNEP_MC_ADDR_FILTER_INVALID_RANGE       (0x0032U | BNEP_ERR_ID)
#define BNEP_MC_ADDR_FILTER_LIMIT_REACHED       (0x0033U | BNEP_ERR_ID)
#define BNEP_MC_ADDR_FILTER_SECURITY_BLOCK      (0x0034U | BNEP_ERR_ID)
#define BNEP_MC_ADDR_FILTER_PENDING_RESPONSE    (0x0035U | BNEP_ERR_ID)
#define BNEP_MC_ADDR_FILTER_MAX_RETX            (0x0036U | BNEP_ERR_ID)
#define BNEP_MC_ADDR_FILTER_UNKNOWN_ERROR       (0x0037U | BNEP_ERR_ID)

#define BNEP_SETUP_FAILED_INVALID_DEST_UUID     (0x0041U | BNEP_ERR_ID)
#define BNEP_SETUP_FAILED_INVALID_SRC_UUID      (0x0042U | BNEP_ERR_ID)
#define BNEP_SETUP_FAILED_INVALID_UUID_SIZE     (0x0043U | BNEP_ERR_ID)
#define BNEP_SETUP_FAILED_CONNECTION_DISALLOWED (0x0044U | BNEP_ERR_ID)
#define BNEP_SETUP_FAILED_MAX_RETX              (0x0045U | BNEP_ERR_ID)
#define BNEP_SETUP_FAILED_UNKNOWN_ERROR         (0x0046U | BNEP_ERR_ID)


/* ======================================= Section 'AVDTP' */
#define AVDTP_MUTEX_INIT_FAILED                 \
        (MUTEX_INIT_FAILED | AVDTP_ERR_ID)
#define AVDTP_COND_INIT_FAILED                  \
        (COND_INIT_FAILED | AVDTP_ERR_ID)
#define AVDTP_MUTEX_LOCK_FAILED                 \
        (MUTEX_LOCK_FAILED | AVDTP_ERR_ID)
#define AVDTP_MUTEX_UNLOCK_FAILED               \
        (MUTEX_UNLOCK_FAILED | AVDTP_ERR_ID)
#define AVDTP_MEMORY_ALLOCATION_FAILED          \
        (MEMORY_ALLOCATION_FAILED | AVDTP_ERR_ID)

#define AVDTP_SIG_ENTITY_ALLOC_FAILED           (0x0011U | AVDTP_ERR_ID)
#define AVDTP_SIG_ENTITY_SEARCH_FAILED          (0x0012U | AVDTP_ERR_ID)

#define AVDTP_LCID_ENTITY_ALLOC_FAILED          (0x0013U | AVDTP_ERR_ID)
#define AVDTP_LCID_ENTITY_SEARCH_FAILED         (0x0014U | AVDTP_ERR_ID)

#define AVDTP_USER_CONTEXT_ALLOC_FAILED         (0x0015U | AVDTP_ERR_ID)
#define AVDTP_USER_CONTEXT_SEARCH_FAILED        (0x0016U | AVDTP_ERR_ID)

#define AVDTP_STREAM_CONTEXT_ALLOC_FAILED       (0x0017U | AVDTP_ERR_ID)
#define AVDTP_STREAM_CONTEXT_SEARCH_FAILED      (0x0018U | AVDTP_ERR_ID)


#define AVDTP_CB_NOT_FOUND                      (0x0019U | AVDTP_ERR_ID)
#define AVDTP_USER_AREA_NOT_PRESENT             (0x001AU | AVDTP_ERR_ID)
#define AVDTP_USER_CONTEXT_INVALID              (0x001BU | AVDTP_ERR_ID)


#define AVDTP_SIGNALING_ENTITY_ACTIVE           (0x001CU | AVDTP_ERR_ID)
#define AVDTP_SIGNALING_ENTITY_INACTIVE         (0x001DU | AVDTP_ERR_ID)
#define AVDTP_STREAM_CONTEXT_ACTIVE             (0x001EU | AVDTP_ERR_ID)

#define AVDTP_UNEXPECTED_L2CA_CONFIG_IND        (0x001FU | AVDTP_ERR_ID)

#define AVDTP_INVALID_STREAM_CONTEXT_STATE      (0x0020U | AVDTP_ERR_ID)
#define AVDTP_MEDIA_TRANSPORT_ABSENT            (0x0021U | AVDTP_ERR_ID)
#define AVDTP_TRANSPORT_CONNECTION_FAILED       (0x0022U | AVDTP_ERR_ID)
#define AVDTP_SEP_NOT_FOUND                     (0x0023U | AVDTP_ERR_ID)

#define AVDTP_CREATE_TSID_ENTITY_FAILED         (0x0024U | AVDTP_ERR_ID)
#define AVDTP_TSID_ENTITY_SEARCH_FAILED         (0x0025U | AVDTP_ERR_ID)
#define AVDTP_BAD_INT_SEID                      (0x0026U | AVDTP_ERR_ID)
#define AVDTP_COMMAND_TIMEOUT                   (0x0027U | AVDTP_ERR_ID)
#define AVDTP_INSUFFICIENT_USER_AREA            (0x0028U | AVDTP_ERR_ID)
#define AVDTP_TS_SEARCH_FAILED                  (0x0029U | AVDTP_ERR_ID)
#define AVDTP_SIGNALING_ENTITY_EXISTS           (0x002AU | AVDTP_ERR_ID)

#define AVDTP_INVALID_PARAMETERS                (0x002BU | AVDTP_ERR_ID)
#define AVDTP_TCID_ALLOCATION_FAILED            (0x002CU | AVDTP_ERR_ID)
#define AVDTP_TSID_ALLOCATION_FAILED            (0x002DU | AVDTP_ERR_ID)
#define AVDTP_TCID_ENTITY_SEARCH_FAILED         (0x002EU | AVDTP_ERR_ID)

#define AVDTP_INVALID_USER_AREA_LEN             (0x002FU | AVDTP_ERR_ID)
#define AVDTP_SC_OPEN_IN_PROGRESS_NOT_FOUND     (0x0030U | AVDTP_ERR_ID)
#define AVDTP_STREAM_TRANSPORT_CHANNEL_CLOSED   (0x0031U | AVDTP_ERR_ID)
#define AVDTP_TSID_TCID_ALLOCATION_FAILED       (0x0032U | AVDTP_ERR_ID)
#define AVDTP_INSUFFICIENT_BUFFER_SIZE          (0x0033U | AVDTP_ERR_ID)

#define AVDTP_BAD_RSP_LENGTH                    (0x0034U | AVDTP_ERR_ID)
#define AVDTP_PROCEDURE_ABORTED                 (0x0035U | AVDTP_ERR_ID)

#define AVDTP_API_NOT_SUPPORTED                 (0x0036U | AVDTP_ERR_ID)
#define AVDTP_INVALID_SIG_ID                    (0x0037U | AVDTP_ERR_ID)


/* ======================================= Section 'AVCTP' */
#define AVCTP_MUTEX_INIT_FAILED                 \
        (MUTEX_INIT_FAILED | AVCTP_ERR_ID)
#define AVCTP_COND_INIT_FAILED                  \
        (COND_INIT_FAILED | AVCTP_ERR_ID)
#define AVCTP_MUTEX_LOCK_FAILED                 \
        (MUTEX_LOCK_FAILED | AVCTP_ERR_ID)
#define AVCTP_MUTEX_UNLOCK_FAILED               \
        (MUTEX_UNLOCK_FAILED | AVCTP_ERR_ID)
#define AVCTP_MEMORY_ALLOCATION_FAILED          \
        (MEMORY_ALLOCATION_FAILED | AVCTP_ERR_ID)

#define AVCTP_SIG_ENTITY_EXISTS                 (0x0011U | AVCTP_ERR_ID)
#define AVCTP_SIG_ENTITY_ALLOC_FAILED           (0x0012U | AVCTP_ERR_ID)
#define AVCTP_SIG_ENTITY_SEARCH_FAILED          (0x0013U | AVCTP_ERR_ID)
#define AVCTP_INVALID_PARAMETERS                (0x0014U | AVCTP_ERR_ID)
#define AVCTP_PROFILE_ID_NOT_FOUND              (0x0015U | AVCTP_ERR_ID)
#define AVCTP_PROFILE_ALLOC_FAILED              (0x0016U | AVCTP_ERR_ID)
#define AVCTP_PROFILE_ALRAEDY_REGISTERED        (0x0017U | AVCTP_ERR_ID)
#define AVCTP_PROFILE_ID_NOT_REGISTRED          (0x0018U | AVCTP_ERR_ID)
#define AVCTP_CONNECTION_FAILED                 (0x0019U | AVCTP_ERR_ID)
#define AVCTP_CONNECTION_DONT_EXISTS            (0x001AU | AVCTP_ERR_ID)
#define AVCTP_PID_BIT_SET_IN_RSP                (0x001BU | AVCTP_ERR_ID)
#define AVCTP_CONNECTION_ALREADY_EXISTS         (0x001CU | AVCTP_ERR_ID)


/* ======================================= Section 'IrOBEX' */
#define IROBEX_MUTEX_INIT_FAILED                \
        (MUTEX_INIT_FAILED | IROBEX_ERR_ID)
#define IROBEX_COND_INIT_FAILED                 \
        (COND_INIT_FAILED | IROBEX_ERR_ID)
#define IROBEX_MUTEX_LOCK_FAILED                \
        (MUTEX_LOCK_FAILED | IROBEX_ERR_ID)
#define IROBEX_MUTEX_UNLOCK_FAILED              \
        (MUTEX_UNLOCK_FAILED | IROBEX_ERR_ID)
#define IROBEX_MEMORY_ALLOCATION_FAILED         \
        (MEMORY_ALLOCATION_FAILED | IROBEX_ERR_ID)

#define IROBEX_INVALID_INPUT_PARAMETERS         (0x0011U | IROBEX_ERR_ID)
#define IROBEX_TRANSPORT_CONNECTION_FAILED      (0x0012U | IROBEX_ERR_ID)
#define IROBEX_SESSION_EXIST_ERR                (0x0013U | IROBEX_ERR_ID)
#define IROBEX_NO_TRANSPORT_CONNECTION          (0x0014U | IROBEX_ERR_ID)
#define IROBEX_CREATE_SESSION_FAILED            (0x0015U | IROBEX_ERR_ID)
#define IROBEX_INVALID_SESSION                  (0x0016U | IROBEX_ERR_ID)
#define IROBEX_READ_ERR                         (0x0017U | IROBEX_ERR_ID)
#define IROBEX_INVALID_RESPONSE_RECEIVED        (0x0018U | IROBEX_ERR_ID)
#define IROBEX_PACKET_ALLOC_FAILED              (0x0019U | IROBEX_ERR_ID)
#define IROBEX_INVALID_REQUEST                  (0x001AU | IROBEX_ERR_ID)
#define IROBEX_INVALID_STATE                    (0x001BU | IROBEX_ERR_ID)
#define IROBEX_RESPONSE_MISMATCH                (0x001CU | IROBEX_ERR_ID)
#define IROBEX_PACKET_FRAME_ERR                 (0x001DU | IROBEX_ERR_ID)
#define IROBEX_PACKET_PARSE_ERR                 (0x001EU | IROBEX_ERR_ID)
#define IROBEX_TRANSPORT_ERR                    (0x001FU | IROBEX_ERR_ID)
#define IROBEX_AUTH_FAILED                      (0x0020U | IROBEX_ERR_ID)
#define IROBEX_UNKNOWN_EVENT_ERR                (0x0021U | IROBEX_ERR_ID)


/* ======================================= Section 'OBEX' */
#define OBEX_MUTEX_INIT_FAILED                  \
        (MUTEX_INIT_FAILED | OBEX_ERR_ID)
#define OBEX_COND_INIT_FAILED                   \
        (COND_INIT_FAILED | OBEX_ERR_ID)
#define OBEX_MUTEX_LOCK_FAILED                  \
        (MUTEX_LOCK_FAILED | OBEX_ERR_ID)
#define OBEX_MUTEX_UNLOCK_FAILED                \
        (MUTEX_UNLOCK_FAILED | OBEX_ERR_ID)
#define OBEX_MEMORY_ALLOCATION_FAILED           \
        (MEMORY_ALLOCATION_FAILED | OBEX_ERR_ID)

#define OBEX_INVALID_INPUT_PARAMETERS           (0x0011U | OBEX_ERR_ID)
#define OBEX_TRANSPORT_CONNECTION_FAILED        (0x0012U | OBEX_ERR_ID)
#define OBEX_SESSION_EXIST_ERR                  (0x0013U | OBEX_ERR_ID)
#define OBEX_NO_TRANSPORT_CONNECTION            (0x0014U | OBEX_ERR_ID)
#define OBEX_CREATE_SESSION_FAILED              (0x0015U | OBEX_ERR_ID)
#define OBEX_INVALID_SESSION                    (0x0016U | OBEX_ERR_ID)
#define OBEX_READ_ERR                           (0x0017U | OBEX_ERR_ID)
#define OBEX_INVALID_RESPONSE_RECEIVED          (0x0018U | OBEX_ERR_ID)
#define OBEX_PACKET_ALLOC_FAILED                (0x0019U | OBEX_ERR_ID)
#define OBEX_INVALID_REQUEST                    (0x001AU | OBEX_ERR_ID)
#define OBEX_INVALID_STATE                      (0x001BU | OBEX_ERR_ID)
#define OBEX_RESPONSE_MISMATCH                  (0x001CU | OBEX_ERR_ID)
#define OBEX_PACKET_FRAME_ERR                   (0x001DU | OBEX_ERR_ID)
#define OBEX_PACKET_PARSE_ERR                   (0x001EU | OBEX_ERR_ID)
#define OBEX_TRANSPORT_ERR                      (0x001FU | OBEX_ERR_ID)
#define OBEX_AUTH_FAILED                        (0x0020U | OBEX_ERR_ID)
#define OBEX_UNKNOWN_EVENT_ERR                  (0x0021U | OBEX_ERR_ID)
#define OBEX_BUFFER_INSUFFICIENT_ERR            (0x0022U | OBEX_ERR_ID)
#define OBEX_INVALID_OP_MODE                    (0x0023U | OBEX_ERR_ID)
#define OBEX_ERROR_RESPONSE                     (0x0024U | OBEX_ERR_ID)
#define OBEX_INVALID_OBJ_HDR                    (0x0025U | OBEX_ERR_ID)
#define OBEX_L2CAP_CONNECT_FAILED               (0x0026U | OBEX_ERR_ID)
#define OBEX_L2CAP_PROCEDURE_FAILED             (0x0027U | OBEX_ERR_ID)

/* ======================================= Section 'MCAP' */
#define MCAP_MUTEX_INIT_FAILED                  \
        (MUTEX_INIT_FAILED | MCAP_ERR_ID)
#define MCAP_COND_INIT_FAILED                   \
        (COND_INIT_FAILED | MCAP_ERR_ID)
#define MCAP_MUTEX_LOCK_FAILED                  \
        (MUTEX_LOCK_FAILED | MCAP_ERR_ID)
#define MCAP_MUTEX_UNLOCK_FAILED                \
        (MUTEX_UNLOCK_FAILED | MCAP_ERR_ID)
#define MCAP_MEMORY_ALLOCATION_FAILED           \
        (MEMORY_ALLOCATION_FAILED | MCAP_ERR_ID)

#define MCAP_OPERATION_NOT_ALLOWED              (0x0011U | MCAP_ERR_ID)
#define MCAP_NO_MEP_INSTANCE_AVAILABLE          (0x0012U | MCAP_ERR_ID)
#define MCAP_CONTROL_CHANNEL_NOT_FOUND          (0x0013U | MCAP_ERR_ID)
#define MCAP_DATA_CHANNEL_NOT_FOUND             (0x0014U | MCAP_ERR_ID)
#define MCAP_CONTROL_CHANNEL_EXISTS             (0x0015U | MCAP_ERR_ID)
#define MCAP_NO_CHANNEL_INSTANCE_AVAILABLE      (0x0016U | MCAP_ERR_ID)
#define MCAP_FEATURE_NOT_SUPPORTED              (0x0017U | MCAP_ERR_ID)
#define MCAP_INVALID_COMMAND_LEN                (0x0018U | MCAP_ERR_ID)
#define MCAP_INVALID_COMMAND_PARAMETER          (0x0019U | MCAP_ERR_ID)
#define MCAP_MCL_BUSY                           (0x001AU | MCAP_ERR_ID)
#define MCAP_RSP_TIMED_OUT                      (0x001BU | MCAP_ERR_ID)
#define MCAP_CONFIG_MISMATCH                    (0x001CU | MCAP_ERR_ID)
#define MCAP_CONTROL_CHANNEL_DISCONNECTED       (0x001DU | MCAP_ERR_ID)
#define MCAP_L2CAP_PROCEDURE_FAILED             (0x001EU | MCAP_ERR_ID)
#define MCAP_DATA_CHANNEL_ABORTED               (0x001FU | MCAP_ERR_ID)
#define MCAP_L2CAP_CONNECTION_REJECTED          (0x0020U | MCAP_ERR_ID)
#define MCAP_REALLOCATING_INSTANCE              (0x0021U | MCAP_ERR_ID)
#define MCAP_CHANNEL_DISCONNECTION              (0x0022U | MCAP_ERR_ID)
#define MCAP_L2CAP_CHANNEL_DISCONNECTED         (0x0023U | MCAP_ERR_ID)
#define MCAP_NULL_PARAMETER_NOT_ALLOWED         (0x0024U | MCAP_ERR_ID)
#define MCAP_ERROR_RSP_RECEIVED_ON_MCL          (0x0025U | MCAP_ERR_ID)
#define MCAP_DATA_CHANNEL_DELETED               (0x0026U | MCAP_ERR_ID)
#define MCAP_TEARING_DOWN_MCL_CONFIG_RCVD       (0x0027U | MCAP_ERR_ID)
#define MCAP_CHANNEL_L2CAP_FLOW_OFF             (0x0028U | MCAP_ERR_ID)
#define MCAP_INVALID_PSM_VALUE                  (0x0029U | MCAP_ERR_ID)
#define MCAP_DATA_CHANNEL_ID_INVALID            (0x002AU | MCAP_ERR_ID)
#define MCAP_CONTROL_CHANNEL_ID_INVALID         (0x002BU | MCAP_ERR_ID)
#define MCAP_INVALID_MEP_ID                     (0x002CU | MCAP_ERR_ID)
#define MCAP_OUT_MTU_SMALLER_THAN_DATA_SIZE     (0x002DU | MCAP_ERR_ID)
#define MCAP_MDL_ALREADY_ACTIVE                 (0x002EU | MCAP_ERR_ID)


/* ======================================= Section 'Driver' */
#define DRV_MUTEX_INIT_FAILED                   \
        (MUTEX_INIT_FAILED | DRV_ERR_ID)
#define DRV_COND_INIT_FAILED                    \
        (COND_INIT_FAILED | DRV_ERR_ID)
#define DRV_MUTEX_LOCK_FAILED                   \
        (MUTEX_LOCK_FAILED | DRV_ERR_ID)
#define DRV_MUTEX_UNLOCK_FAILED                 \
        (MUTEX_UNLOCK_FAILED | DRV_ERR_ID)
#define DRV_MEMORY_ALLOCATION_FAILED            \
        (MEMORY_ALLOCATION_FAILED | DRV_ERR_ID)

#define DRV_IOCTL_FAILED                        (0x0011U | DRV_ERR_ID)
#define DRV_READ_TASK_CREATE_FAILED             (0x0012U | DRV_ERR_ID)
#define DRV_WRITE_TASK_CREATE_FAILED            (0x0013U | DRV_ERR_ID)
#define DRV_CALLBACK_TASK_CREATE_FAILED         (0x0014U | DRV_ERR_ID)

#define DRV_INVALID_SOCKET_FD                   (0x0021U | DRV_ERR_ID)
#define DRV_CANT_FIND_INODE                     (0x0022U | DRV_ERR_ID)
#define DRV_FILE_POINTERS_MISMATCH              (0x0023U | DRV_ERR_ID)
#define DRV_UBUF_VERIFY_FAILED                  (0x0024U | DRV_ERR_ID)
#define DRV_MAX_SOCKET_LEN                      (0x0025U | DRV_ERR_ID)


/* ======================================= Section 'PAN Driver' */
#define PAN_DRV_MUTEX_INIT_FAILED               \
        (MUTEX_INIT_FAILED | PAN_DRV_ERR_ID)
#define PAN_DRV_COND_INIT_FAILED                \
        (COND_INIT_FAILED | PAN_DRV_ERR_ID)
#define PAN_DRV_MUTEX_LOCK_FAILED               \
        (MUTEX_LOCK_FAILED | PAN_DRV_ERR_ID)
#define PAN_DRV_MUTEX_UNLOCK_FAILED             \
        (MUTEX_UNLOCK_FAILED | PAN_DRV_ERR_ID)
#define PAN_DRV_MEMORY_ALLOCATION_FAILED        \
        (MEMORY_ALLOCATION_FAILED | PAN_DRV_ERR_ID)

#define PAN_DRV_IOCTL_UNKNOWN_API_TYPE          (0x0011U | PAN_DRV_ERR_ID)
#define PAN_DRV_LOCAL_SERVICE_PANU_EXISTS       (0x0012U | PAN_DRV_ERR_ID)


/* ======================================= Section 'A2DP Driver' */
#define A2DP_DRV_MUTEX_INIT_FAILED              \
        (MUTEX_INIT_FAILED | A2DP_DRV_ERR_ID)
#define A2DP_DRV_COND_INIT_FAILED               \
        (COND_INIT_FAILED | A2DP_DRV_ERR_ID)
#define A2DP_DRV_MUTEX_LOCK_FAILED              \
        (MUTEX_LOCK_FAILED | A2DP_DRV_ERR_ID)
#define A2DP_DRV_MUTEX_UNLOCK_FAILED            \
        (MUTEX_UNLOCK_FAILED | A2DP_DRV_ERR_ID)
#define A2DP_DRV_MEMORY_ALLOCATION_FAILED       \
        (MEMORY_ALLOCATION_FAILED | A2DP_DRV_ERR_ID)

#define A2DP_DRV_INVALID_PARAMETERS             (0x0011U | A2DP_DRV_ERR_ID)
#define A2DP_DRV_IOCTL_UNKNOWN_API_TYPE         (0x0012U | A2DP_DRV_ERR_ID)


/* ======================================= Section 'AVRCP Driver' */
#define AVRCP_DRV_MUTEX_INIT_FAILED             \
        (MUTEX_INIT_FAILED | AVRCP_DRV_ERR_ID)
#define AVRCP_DRV_COND_INIT_FAILED              \
        (COND_INIT_FAILED | AVRCP_DRV_ERR_ID)
#define AVRCP_DRV_MUTEX_LOCK_FAILED             \
        (MUTEX_LOCK_FAILED | AVRCP_DRV_ERR_ID)
#define AVRCP_DRV_MUTEX_UNLOCK_FAILED           \
        (MUTEX_UNLOCK_FAILED | AVRCP_DRV_ERR_ID)
#define AVRCP_DRV_MEMORY_ALLOCATION_FAILED      \
        (MEMORY_ALLOCATION_FAILED | AVRCP_DRV_ERR_ID)

#define AVRCP_DRV_IOCTL_UNKNOWN_API_TYPE        (0x0011U | AVRCP_DRV_ERR_ID)


/* ======================================= Section 'VCOM Driver' */
#define VCOM_DRV_MUTEX_INIT_FAILED              \
        (MUTEX_INIT_FAILED | VCOM_DRV_ERR_ID)
#define VCOM_DRV_COND_INIT_FAILED               \
        (COND_INIT_FAILED | VCOM_DRV_ERR_ID)
#define VCOM_DRV_MUTEX_LOCK_FAILED              \
        (MUTEX_LOCK_FAILED | VCOM_DRV_ERR_ID)
#define VCOM_DRV_MUTEX_UNLOCK_FAILED            \
        (MUTEX_UNLOCK_FAILED | VCOM_DRV_ERR_ID)
#define VCOM_DRV_MEMORY_ALLOCATION_FAILED       \
        (MEMORY_ALLOCATION_FAILED | VCOM_DRV_ERR_ID)

#define VCOM_DRV_TX_QUEUE_FULL                  (0x0011U | VCOM_DRV_ERR_ID)


/* ======================================= Section 'HID DEVICE Driver' */
#define HID_DEV_DRV_MUTEX_INIT_FAILED           \
        (MUTEX_INIT_FAILED | HID_DEV_DRV_ERR_ID)
#define HID_DEV_DRV_COND_INIT_FAILED            \
        (COND_INIT_FAILED | HID_DEV_DRV_ERR_ID)
#define HID_DEV_DRV_MUTEX_LOCK_FAILED           \
        (MUTEX_LOCK_FAILED | HID_DEV_DRV_ERR_ID)
#define HID_DEV_DRV_MUTEX_UNLOCK_FAILED         \
        (MUTEX_UNLOCK_FAILED | HID_DEV_DRV_ERR_ID)
#define HID_DEV_DRV_MEMORY_ALLOCATION_FAILED    \
        (MEMORY_ALLOCATION_FAILED | HID_DEV_DRV_ERR_ID)

#define HID_DEV_DRV_IOCTL_UNKNOWN_API_TYPE      (0x0011U | HID_DEV_DRV_ERR_ID)


/* ======================================= Section 'GAP' */


/* ======================================= Section 'SDAP' */


/* ======================================= Section 'SPP' */
#define SPP_MUTEX_INIT_FAILED                   \
        (MUTEX_INIT_FAILED | SPP_ERR_ID)
#define SPP_COND_INIT_FAILED                    \
        (COND_INIT_FAILED | SPP_ERR_ID)
#define SPP_MUTEX_LOCK_FAILED                   \
        (MUTEX_LOCK_FAILED | SPP_ERR_ID)
#define SPP_MUTEX_UNLOCK_FAILED                 \
        (MUTEX_UNLOCK_FAILED | SPP_ERR_ID)
#define SPP_MEMORY_ALLOCATION_FAILED            \
        (MEMORY_ALLOCATION_FAILED | SPP_ERR_ID)

#define SPP_ERR_ALREADY_INITIALIZED        (0x0011U | SPP_ERR_ID)
#define SPP_ERR_NULL_PARAMETER             (0x0012U | SPP_ERR_ID)
#define SPP_ERR_INVALID_PARAM              (0x0013U | SPP_ERR_ID)
#define SPP_ERR_INVALID_STATE              (0x0014U | SPP_ERR_ID)
#define SPP_ERR_STOP_PENDING               (0x0015U | SPP_ERR_ID)
#define SPP_ERR_MAX_CONNECT_LIMIT_REACHED  (0x0016U | SPP_ERR_ID)
#define SPP_ERR_INVALID_HANDLE             (0x0017U | SPP_ERR_ID)
#define SPP_ERR_INVALID_GAIN               (0x0018U | SPP_ERR_ID)
#define SPP_ERR_NO_FREE_ENTITY             (0x0019U | SPP_ERR_ID)


/* ======================================= Section 'HSP Unit' */
#define HSP_UNIT_MUTEX_INIT_FAILED              \
        (MUTEX_INIT_FAILED | HSP_UNIT_ERR_ID)
#define HSP_UNIT_COND_INIT_FAILED               \
        (COND_INIT_FAILED | HSP_UNIT_ERR_ID)
#define HSP_UNIT_MUTEX_LOCK_FAILED              \
        (MUTEX_LOCK_FAILED | HSP_UNIT_ERR_ID)
#define HSP_UNIT_MUTEX_UNLOCK_FAILED            \
        (MUTEX_UNLOCK_FAILED | HSP_UNIT_ERR_ID)
#define HSP_UNIT_MEMORY_ALLOCATION_FAILED       \
        (MEMORY_ALLOCATION_FAILED | HSP_UNIT_ERR_ID)

#define HSP_UNIT_ERR_ALREADY_INITIALIZED        (0x0011U | HSP_UNIT_ERR_ID)
#define HSP_UNIT_ERR_NULL_PARAMETER             (0x0012U | HSP_UNIT_ERR_ID)
#define HSP_UNIT_ERR_INVALID_PARAM              (0x0013U | HSP_UNIT_ERR_ID)
#define HSP_UNIT_ERR_INVALID_STATE              (0x0014U | HSP_UNIT_ERR_ID)
#define HSP_UNIT_ERR_STOP_PENDING               (0x0015U | HSP_UNIT_ERR_ID)
#define HSP_UNIT_ERR_MAX_CONNECT_LIMIT_REACHED  (0x0016U | HSP_UNIT_ERR_ID)
#define HSP_UNIT_ERR_INVALID_HANDLE             (0x0017U | HSP_UNIT_ERR_ID)
#define HSP_UNIT_ERR_INVALID_GAIN               (0x0018U | HSP_UNIT_ERR_ID)
#define HSP_UNIT_ERR_NO_FREE_ENTITY             (0x0019U | HSP_UNIT_ERR_ID)


/* ======================================= Section 'HSP AG' */
#define HSP_AG_MUTEX_INIT_FAILED                \
        (MUTEX_INIT_FAILED | HSP_AG_ERR_ID)
#define HSP_AG_COND_INIT_FAILED                 \
        (COND_INIT_FAILED | HSP_AG_ERR_ID)
#define HSP_AG_MUTEX_LOCK_FAILED                \
        (MUTEX_LOCK_FAILED | HSP_AG_ERR_ID)
#define HSP_AG_MUTEX_UNLOCK_FAILED              \
        (MUTEX_UNLOCK_FAILED | HSP_AG_ERR_ID)
#define HSP_AG_MEMORY_ALLOCATION_FAILED         \
        (MEMORY_ALLOCATION_FAILED | HSP_AG_ERR_ID)

#define HSP_AG_ERR_NULL_PARAMETER               (0x0011U | HSP_AG_ERR_ID)
#define HSP_AG_ERR_INVALID_PARAMETER            (0x0012U | HSP_AG_ERR_ID)
#define HSP_AG_ERR_STOP_PENDING                 (0x0013U | HSP_AG_ERR_ID)
#define HSP_AG_ERR_MUTEX_INIT_FAILED            (0x0014U | HSP_AG_ERR_ID)
#define HSP_AG_ERR_INVALID_STATE                (0x0015U | HSP_AG_ERR_ID)


/* ======================================= Section 'HFP Unit' */
#define HFP_UNIT_MUTEX_INIT_FAILED              \
        (MUTEX_INIT_FAILED | HFP_UNIT_ERR_ID)
#define HFP_UNIT_COND_INIT_FAILED               \
        (COND_INIT_FAILED | HFP_UNIT_ERR_ID)
#define HFP_UNIT_MUTEX_LOCK_FAILED              \
        (MUTEX_LOCK_FAILED | HFP_UNIT_ERR_ID)
#define HFP_UNIT_MUTEX_UNLOCK_FAILED            \
        (MUTEX_UNLOCK_FAILED | HFP_UNIT_ERR_ID)
#define HFP_UNIT_MEMORY_ALLOCATION_FAILED       \
        (MEMORY_ALLOCATION_FAILED | HFP_UNIT_ERR_ID)

#define HFP_UNIT_ERR_INVALID_PARAMETERS         (0x0011U | HFP_UNIT_ERR_ID)
#define HFP_UNIT_ERR_INVALID_STATE              (0x0012U | HFP_UNIT_ERR_ID)
#define HFP_UNIT_ERR_START_FAILED               (0x0013U | HFP_UNIT_ERR_ID)
#define HFP_UNIT_ERR_NO_FREE_INSTANCE           (0x0014U | HFP_UNIT_ERR_ID)
#define HFP_UNIT_ERR_UNIT_BUSY                  (0x0015U | HFP_UNIT_ERR_ID)
#define HFP_UNIT_ERR_STOP_PENDING               (0x0017U | HFP_UNIT_ERR_ID)
#define HFP_UNIT_ERR_RSP_ERR_FROM_AG            (0x0018U | HFP_UNIT_ERR_ID)
#define HFP_UNIT_ERR_AG_BLACKLISTED             (0x0019U | HFP_UNIT_ERR_ID)
#define HFP_UNIT_ERR_AG_BUSY                    (0x001AU | HFP_UNIT_ERR_ID)
#define HFP_UNIT_ERR_AG_DELAYED                 (0x001BU | HFP_UNIT_ERR_ID)
#define HFP_UNIT_ERR_AG_NOANSWER                (0x001CU | HFP_UNIT_ERR_ID)
#define HFP_UNIT_ERR_NO_CARRIER                 (0x001DU | HFP_UNIT_ERR_ID)
#define HFP_UNIT_ERR_TIMER_EXPIRED              (0x001EU | HFP_UNIT_ERR_ID)
#define HFP_UNIT_ALREADY_INITIALIZED            (0x001FU | HFP_UNIT_ERR_ID)
#define HFP_UNIT_ERR_NOPROCESSING_DONE          AT_PARSER_ERR_NOPROCESSING_DONE


/* ======================================= Section 'HFP AG' */
#define HFP_AG_MUTEX_INIT_FAILED                \
        (MUTEX_INIT_FAILED | HFP_AG_ERR_ID)
#define HFP_AG_COND_INIT_FAILED                 \
        (COND_INIT_FAILED | HFP_AG_ERR_ID)
#define HFP_AG_MUTEX_LOCK_FAILED                \
        (MUTEX_LOCK_FAILED | HFP_AG_ERR_ID)
#define HFP_AG_MUTEX_UNLOCK_FAILED              \
        (MUTEX_UNLOCK_FAILED | HFP_AG_ERR_ID)
#define HFP_AG_MEMORY_ALLOCATION_FAILED         \
        (MEMORY_ALLOCATION_FAILED | HFP_AG_ERR_ID)

#define HFP_AG_ERR_NULL_PARAMETER               (0x0011U | HFP_AG_ERR_ID)
#define HFP_AG_ERR_INVALID_PARAMETER            (0x0012U | HFP_AG_ERR_ID)
#define HFP_AG_ERR_STOP_PENDING                 (0x0013U | HFP_AG_ERR_ID)
#define HFP_AG_ERR_INVALID_STATE                (0x0014U | HFP_AG_ERR_ID)


/* ======================================= Section 'DUNP DT' */
#define DUNP_DT_MUTEX_INIT_FAILED               \
        (MUTEX_INIT_FAILED | DUNP_DT_ERR_ID)
#define DUNP_DT_COND_INIT_FAILED                \
        (COND_INIT_FAILED | DUNP_DT_ERR_ID)
#define DUNP_DT_MUTEX_LOCK_FAILED               \
        (MUTEX_LOCK_FAILED | DUNP_DT_ERR_ID)
#define DUNP_DT_MUTEX_UNLOCK_FAILED             \
        (MUTEX_UNLOCK_FAILED | DUNP_DT_ERR_ID)
#define DUNP_DT_MEMORY_ALLOCATION_FAILED        \
        (MEMORY_ALLOCATION_FAILED | DUNP_DT_ERR_ID)

#define DUNP_DT_ERR_INPUT_PTR_NULL              (0x0011U | DUNP_DT_ERR_ID)
#define DUNP_DT_ERR_ALREADY_STARTED             (0x0012U | DUNP_DT_ERR_ID)
#define DUNP_DT_ERR_ZERO_LENGTH                 (0x0013U | DUNP_DT_ERR_ID)
#define DUNP_DT_ERR_INVALID_DT_ID               (0x0014U | DUNP_DT_ERR_ID)
#define DUNP_DT_ERR_NOT_CONNECTED               (0x0015U | DUNP_DT_ERR_ID)
#define DUNP_DT_ERR_INVALID_PARAM               (0x0016U | DUNP_DT_ERR_ID)
#define DUNP_DT_ERR_STOP_PENDING                (0x0017U | DUNP_DT_ERR_ID)
#define DUNP_DT_ERR_NOT_STARTED                 (0x0018U | DUNP_DT_ERR_ID)
#define DUNP_DT_ERR_INVALID_STATE               (0x0019U | DUNP_DT_ERR_ID)
#define DUNP_DT_ERR_NOT_INITIALIZED             (0x001AU | DUNP_DT_ERR_ID)
#define DUNP_DT_ERR_MAX_CONNECTION_REACHED      (0x001BU | DUNP_DT_ERR_ID)


/* ======================================= Section 'DUNP GW' */
#define DUNP_GW_MUTEX_INIT_FAILED               \
        (MUTEX_INIT_FAILED | DUNP_GW_ERR_ID)
#define DUNP_GW_COND_INIT_FAILED                \
        (COND_INIT_FAILED | DUNP_GW_ERR_ID)
#define DUNP_GW_MUTEX_LOCK_FAILED               \
        (MUTEX_LOCK_FAILED | DUNP_GW_ERR_ID)
#define DUNP_GW_MUTEX_UNLOCK_FAILED             \
        (MUTEX_UNLOCK_FAILED | DUNP_GW_ERR_ID)
#define DUNP_GW_MEMORY_ALLOCATION_FAILED        \
        (MEMORY_ALLOCATION_FAILED | DUNP_GW_ERR_ID)

#define DUNP_GW_ERR_INPUT_PTR_NULL              (0x0011U | DUNP_GW_ERR_ID)
#define DUNP_GW_ERR_ALREADY_STARTED             (0x0012U | DUNP_GW_ERR_ID)
#define DUNP_GW_ERR_ZERO_LENGTH                 (0x0013U | DUNP_GW_ERR_ID)
#define DUNP_GW_ERR_INVALID_DT_ID               (0x0014U | DUNP_GW_ERR_ID)
#define DUNP_GW_ERR_NOT_CONNECTED               (0x0015U | DUNP_GW_ERR_ID)
#define DUNP_GW_ERR_INVALID_PARAM               (0x0016U | DUNP_GW_ERR_ID)
#define DUNP_GW_ERR_STOP_PENDING                (0x0017U | DUNP_GW_ERR_ID)
#define DUNP_GW_ERR_NOT_STARTED                 (0x0018U | DUNP_GW_ERR_ID)
#define DUNP_GW_ERR_INVALID_STATE               (0x0019U | DUNP_GW_ERR_ID)

#define DUNP_GW_NULL_APPL_CB_NOT_ALLOWED        (0x0021U | DUNP_GW_ERR_ID)
#define DUNP_GW_NOT_READY                       (0x0022U | DUNP_GW_ERR_ID)
#define DUNP_GW_DISCONNECT_IN_PROGRESS          (0x0023U | DUNP_GW_ERR_ID)
#define DUNP_GW_ALREADY_DISCONNECTED            (0x0024U | DUNP_GW_ERR_ID)
#define DUNP_GW_NOT_BUSY                        (0x0025U | DUNP_GW_ERR_ID)
#define DUNP_GW_CALL_DISC_IN_PROGRESS           (0x0026U | DUNP_GW_ERR_ID)
#define DUNP_GW_INVALID_DT_ID                   (0x0027U | DUNP_GW_ERR_ID)
#define DUNP_GW_INVALID_PARAM                   (0x0028U | DUNP_GW_ERR_ID)
#define DUNP_GW_DISCONNECTED                    (0x0029U | DUNP_GW_ERR_ID)
#define DUNP_GW_CMD_BEING_PROCESSED             (0x002AU | DUNP_GW_ERR_ID)
#define DUNP_GW_INVALID_STATE                   (0x002BU | DUNP_GW_ERR_ID)


/* ======================================= Section 'FAXP' */


/* ======================================= Section 'UDIP' */
#define UDIP_MUTEX_INIT_FAILED                  \
        (MUTEX_INIT_FAILED | UDIP_ERR_ID)
#define UDIP_COND_INIT_FAILED                   \
        (COND_INIT_FAILED | UDIP_ERR_ID)
#define UDIP_MUTEX_LOCK_FAILED                  \
        (MUTEX_LOCK_FAILED | UDIP_ERR_ID)
#define UDIP_MUTEX_UNLOCK_FAILED                \
        (MUTEX_UNLOCK_FAILED | UDIP_ERR_ID)
#define UDIP_MEMORY_ALLOCATION_FAILED           \
        (MEMORY_ALLOCATION_FAILED | UDIP_ERR_ID)

#define UDIP_INVALID_STATE                      (0x0011U | UDIP_ERR_ID)
#define UDIP_INVALID_PARAMETERS                 (0x0012U | UDIP_ERR_ID)
#define UDIP_INVALID_ROLE                       (0x0013U | UDIP_ERR_ID)
#define UDIP_ENTITY_UNAVAILABLE                 (0x0014U | UDIP_ERR_ID)


/* ======================================= Section 'GOEP' */


/* ======================================= Section 'OPP' */
#define OPP_MUTEX_INIT_FAILED                   \
        (MUTEX_INIT_FAILED | OPP_ERR_ID)
#define OPP_COND_INIT_FAILED                    \
        (COND_INIT_FAILED | OPP_ERR_ID)
#define OPP_MUTEX_LOCK_FAILED                   \
        (MUTEX_LOCK_FAILED | OPP_ERR_ID)
#define OPP_MUTEX_UNLOCK_FAILED                 \
        (MUTEX_UNLOCK_FAILED | OPP_ERR_ID)
#define OPP_MEMORY_ALLOCATION_FAILED            \
        (MEMORY_ALLOCATION_FAILED | OPP_ERR_ID)

#define OPP_INVALID_STATE                       (0x0011U | OPP_ERR_ID)
#define OPP_INVALID_PARAMETERS                  (0x0012U | OPP_ERR_ID)
#define OPP_ENTITY_START_FAILED                 (0x0013U | OPP_ERR_ID)
#define OPP_ENTITY_STOP_FAILED                  (0x0014U | OPP_ERR_ID)
#define OPP_INVALID_CONNECT_REQ_LENGTH          (0x0015U | OPP_ERR_ID)
#define OPP_GET_OPERATION_ABORTED               (0x0016U | OPP_ERR_ID)
#define OPP_PUT_OPERATION_ABORTED               (0x0017U | OPP_ERR_ID)
#define OPP_INVALID_CONNECTION_ID               (0x0018U | OPP_ERR_ID)
#define OPP_ENTITY_ALLOC_FAILED                 (0x0019U | OPP_ERR_ID)
#define OPP_ENTITY_SEARCH_FAILED                (0x001AU | OPP_ERR_ID)
#define OPP_INVALID_SETPATH_FLAGS               (0x001BU | OPP_ERR_ID)

/* ======================================= Section 'FTP' */
#define FTP_MUTEX_INIT_FAILED                   \
        (MUTEX_INIT_FAILED | FTP_ERR_ID)
#define FTP_COND_INIT_FAILED                    \
        (COND_INIT_FAILED | FTP_ERR_ID)
#define FTP_MUTEX_LOCK_FAILED                   \
        (MUTEX_LOCK_FAILED | FTP_ERR_ID)
#define FTP_MUTEX_UNLOCK_FAILED                 \
        (MUTEX_UNLOCK_FAILED | FTP_ERR_ID)
#define FTP_MEMORY_ALLOCATION_FAILED            \
        (MEMORY_ALLOCATION_FAILED | FTP_ERR_ID)

#define FTP_INVALID_STATE                       (0x0011U | FTP_ERR_ID)
#define FTP_INVALID_PARAMETERS                  (0x0012U | FTP_ERR_ID)
#define FTP_ENTITY_START_FAILED                 (0x0013U | FTP_ERR_ID)
#define FTP_ENTITY_STOP_FAILED                  (0x0014U | FTP_ERR_ID)
#define FTP_INVALID_CONNECT_REQ_LENGTH          (0x0015U | FTP_ERR_ID)
#define FTP_GET_OPERATION_ABORTED               (0x0016U | FTP_ERR_ID)
#define FTP_PUT_OPERATION_ABORTED               (0x0017U | FTP_ERR_ID)
#define FTP_INVALID_CONNECTION_ID               (0x0018U | FTP_ERR_ID)
#define FTP_ENTITY_ALLOC_FAILED                 (0x0019U | FTP_ERR_ID)
#define FTP_ENTITY_SEARCH_FAILED                (0x001AU | FTP_ERR_ID)
#define FTP_INVALID_SETPATH_FLAGS               (0x001BU | FTP_ERR_ID)



/* ======================================= Section 'SYNCP' */
#define SYNCP_MUTEX_INIT_FAILED                 \
        (MUTEX_INIT_FAILED | SYNCP_ERR_ID)
#define SYNCP_COND_INIT_FAILED                  \
        (COND_INIT_FAILED | SYNCP_ERR_ID)
#define SYNCP_MUTEX_LOCK_FAILED                 \
        (MUTEX_LOCK_FAILED | SYNCP_ERR_ID)
#define SYNCP_MUTEX_UNLOCK_FAILED               \
        (MUTEX_UNLOCK_FAILED | SYNCP_ERR_ID)
#define SYNCP_MEMORY_ALLOCATION_FAILED          \
        (MEMORY_ALLOCATION_FAILED | SYNCP_ERR_ID)

#define SYNCP_INVALID_HEADER_ID                 (0x0011U | SYNCP_ERR_ID)
#define SYNCP_HEADER_NOT_FOUND                  (0x0012U | SYNCP_ERR_ID)
#define SYNCP_NO_FREE_ENTITY                    (0x0013U | SYNCP_ERR_ID)
#define SYNCP_NULL_APPL_CB                      (0x0014U | SYNCP_ERR_ID)
#define SYNCP_STATE_NOT_IDLE                    (0x0015U | SYNCP_ERR_ID)
#define SYNCP_SESSION_ALREADY_CONNECTED         (0x0016U | SYNCP_ERR_ID)
#define SYNCP_INVALID_IROBEX_SESSION            (0x0017U | SYNCP_ERR_ID)
#define SYNCP_NO_ACTIVE_OPERATION               (0x0018U | SYNCP_ERR_ID)
#define SYNCP_STATE_NOT_CONNECTED               (0x0019U | SYNCP_ERR_ID)
#define SYNCP_GET_SERVICE_HANDLE_FAIL           (0x001AU | SYNCP_ERR_ID)
#define SYNCP_INVALID_SERVICE                   (0x001BU | SYNCP_ERR_ID)
#define SYNCP_NULL_HEADER_LIST                  (0x001CU | SYNCP_ERR_ID)
#define SYNCP_INVALID_PUT_TYPE                  (0x001DU | SYNCP_ERR_ID)
#define SYNCP_INVALID_ENTITY                    (0x001EU | SYNCP_ERR_ID)
#define SYNCP_NULL_HEADER_VALUE                 (0x001FU | SYNCP_ERR_ID)
#define SYNCP_OPERATION_NOT_IDLE                (0x0020U | SYNCP_ERR_ID)
#define SYNCP_NULL_HANDLE                       (0x0021U | SYNCP_ERR_ID)
#define SYNCP_NULL_HEADER                       (0x0022U | SYNCP_ERR_ID)
#define SYNCP_INVALID_STATE                     (0x0023U | SYNCP_ERR_ID)


/* ======================================= Section 'BIP' */
#define BIP_MUTEX_INIT_FAILED                   \
        (MUTEX_INIT_FAILED | BIP_ERR_ID)
#define BIP_COND_INIT_FAILED                    \
        (COND_INIT_FAILED | BIP_ERR_ID)
#define BIP_MUTEX_LOCK_FAILED                   \
        (MUTEX_LOCK_FAILED | BIP_ERR_ID)
#define BIP_MUTEX_UNLOCK_FAILED                 \
        (MUTEX_UNLOCK_FAILED | BIP_ERR_ID)
#define BIP_MEMORY_ALLOCATION_FAILED            \
        (MEMORY_ALLOCATION_FAILED | BIP_ERR_ID)

#define BIP_INIT_ALREADY_DONE                   (0x0011U | BIP_ERR_ID)
#define BIP_NOT_INITIALIZED                     (0x0012U | BIP_ERR_ID)
#define BIP_RESOURCE_UNAVAILABLE                (0x0013U | BIP_ERR_ID)
#define BIP_ENTITY_SEARCH_FAILED                (0x0014U | BIP_ERR_ID)
#define BIP_INVALID_PARAMETERS                  (0x0015U | BIP_ERR_ID)
#define BIP_INVALID_STATE                       (0x0016U | BIP_ERR_ID)
#define BIP_ENTITY_START_FAILED                 (0x0017U | BIP_ERR_ID)
#if 0
#define BIP_FILE_WRITE_FAILED                   (0x0017 | BIP_ERR_ID)
#define BIP_FILE_READ_FAILED                    (0x0018 | BIP_ERR_ID)
#define BIP_FILE_OPEN_FAILED                    (0x0019 | BIP_ERR_ID)
#define BIP_FILE_DELETE_FAILED                  (0x001A | BIP_ERR_ID)
#define BIP_FILE_CLOSE_FAILED                   (0x001B | BIP_ERR_ID)
#endif /* 0 */
#define BIP_UNRECOGNIZED_PROTO_DISC             (0x001CU | BIP_ERR_ID)
#define BIP_HEADER_RECEIVED_OUT_OF_SEQ          (0x001DU | BIP_ERR_ID)
#define BIP_WRONG_EVENT_RSP                     (0x001EU | BIP_ERR_ID)
#define BIP_HEADER_NOT_FOUND                    (0x001FU | BIP_ERR_ID)
#define BIP_APPL_PARAM_NOT_FOUND                (0x0020U | BIP_ERR_ID)
#define BIP_REQUEST_INVALID                     (0x0021U | BIP_ERR_ID)
#define BIP_IMAGE_HANDLE_INVALID                (0x0022U | BIP_ERR_ID)
#define BIP_FILE_HANDLE_INVALID                 (0x0023U | BIP_ERR_ID)
#define BIP_BIP_HANDLE_INVALID                  (0x0024U | BIP_ERR_ID)
#define BIP_MANDATORY_HDR_NOT_PRESENT           (0x0025U | BIP_ERR_ID)
#define BIP_PUT_IMAGE_NOT_DONE                  (0x0026U | BIP_ERR_ID)
#define BIP_BAD_FUNCTION_REQUEST                (0x0027U | BIP_ERR_ID)
#define BIP_THUMBNAIL_IMAGE_REQUESTED           (0x0028U | BIP_ERR_ID)
#define BIP_SDP_REGISTRATION_FAILED             (0x0029U | BIP_ERR_ID)
#define BIP_AUTH_RESP_NOT_RECEIVED              (0x002AU | BIP_ERR_ID)
#define BIP_ENTITY_STOP_FAILED                  (0x002BU | BIP_ERR_ID)
#define BIP_GET_OPERATION_ABORTED               (0X002CU | BIP_ERR_ID)
#define BIP_INVALID_CONNECTION_ID               (0x002DU | BIP_ERR_ID)
#define BIP_INVALID_CONNECT_REQ_LENGTH          (0x002EU | BIP_ERR_ID)
#define BIP_PUT_OPERATION_ABORTED               (0x002FU | BIP_ERR_ID)

/* ======================================= Section 'BPP' */
#define BPP_MUTEX_INIT_FAILED                   \
        (MUTEX_INIT_FAILED | BPP_ERR_ID)
#define BPP_COND_INIT_FAILED                    \
        (COND_INIT_FAILED | BPP_ERR_ID)
#define BPP_MUTEX_LOCK_FAILED                   \
        (MUTEX_LOCK_FAILED | BPP_ERR_ID)
#define BPP_MUTEX_UNLOCK_FAILED                 \
        (MUTEX_UNLOCK_FAILED | BPP_ERR_ID)
#define BPP_MEMORY_ALLOCATION_FAILED            \
        (MEMORY_ALLOCATION_FAILED | BPP_ERR_ID)

#define BPP_INVALID_STATE                       (0x0011U | BPP_ERR_ID)
#define BPP_INVALID_PARAMETERS                  (0x0012U | BPP_ERR_ID)
#define BPP_INVALID_ROLE                        (0x0013U | BPP_ERR_ID)
#define BPP_SENDER_ENTITY_UNAVAILABLE           (0x0014U | BPP_ERR_ID)
#define BPP_PRINTER_ENTITY_UNAVAILABLE          (0x0015U | BPP_ERR_ID)
#define BPP_INVALID_OPERATION                   (0x0016U | BPP_ERR_ID)
#define BPP_ENTITY_START_FAILED                 (0x0017U | BPP_ERR_ID)
#define BPP_ENTITY_STOP_FAILED                  (0x0018U | BPP_ERR_ID)
#define BPP_INVALID_CONNECTION_ID               (0x002DU | BPP_ERR_ID)
#define BPP_INVALID_CONNECT_REQ_LENGTH          (0x002EU | BPP_ERR_ID)
#define BPP_PUT_OPERATION_ABORTED               (0x002FU | BPP_ERR_ID)
#define BPP_NOT_AUTHENTICATED                   (0x0030U | BPP_ERR_ID)
#define BPP_AUTHENTICATION_FAILED               (0x0031U | BPP_ERR_ID)

/* ======================================= Section 'PAN' */
#define PAN_MUTEX_INIT_FAILED                   \
        (MUTEX_INIT_FAILED | PAN_ERR_ID)
#define PAN_COND_INIT_FAILED                    \
        (COND_INIT_FAILED | PAN_ERR_ID)
#define PAN_MUTEX_LOCK_FAILED                   \
        (MUTEX_LOCK_FAILED | PAN_ERR_ID)
#define PAN_MUTEX_UNLOCK_FAILED                 \
        (MUTEX_UNLOCK_FAILED | PAN_ERR_ID)
#define PAN_MEMORY_ALLOCATION_FAILED            \
        (MEMORY_ALLOCATION_FAILED | PAN_ERR_ID)

#define PAN_PROFILE_ALREADY_INITIALIZED         (0x0011U | PAN_ERR_ID)
#define PAN_NULL_PARAMETER_NOT_ALLOWED          (0x0012U | PAN_ERR_ID)
#define PAN_API_NOT_SUPPORTED                   (0x0013U | PAN_ERR_ID)
#define PAN_INVALID_PARAMETER_VALUE             (0x0014U | PAN_ERR_ID)
#define PAN_ENTITY_ALLOCATION_FAILED            (0x0015U | PAN_ERR_ID)
#define PAN_ENTITY_SEARCH_FAILED                (0x0016U | PAN_ERR_ID)

#define PAN_ARP_LEARN_CANNOT_CONTINUE           (0x00A1U | PAN_ERR_ID)
#define PAN_ARP_LEARN_ETH_ADDR_MISMATCH         (0x00A2U | PAN_ERR_ID)
#define PAN_ARP_FILTER_DROP                     (0x00A3U | PAN_ERR_ID)
#define PAN_ARP_FILTER_CONTINUE                 (0x00A4U | PAN_ERR_ID)
#define PAN_ARP_FILTER_CANNOT_CONTINUE          (0x00A5U | PAN_ERR_ID)

#define PAN_BRIDGE_UNICAST                      (0x00B1U | PAN_ERR_ID)
#define PAN_BRIDGE_BROADCAST                    (0x00B2U | PAN_ERR_ID)
#define PAN_BRIDGE_NOT_UNICAST_BROADCAST        (0x00B3U | PAN_ERR_ID)


/* ======================================= Section 'HID' */
#define HID_MUTEX_INIT_FAILED                   \
        (MUTEX_INIT_FAILED | HID_ERR_ID)
#define HID_COND_INIT_FAILED                    \
        (COND_INIT_FAILED | HID_ERR_ID)
#define HID_MUTEX_LOCK_FAILED                   \
        (MUTEX_LOCK_FAILED | HID_ERR_ID)
#define HID_MUTEX_UNLOCK_FAILED                 \
        (MUTEX_UNLOCK_FAILED | HID_ERR_ID)
#define HID_MEMORY_ALLOCATION_FAILED            \
        (MEMORY_ALLOCATION_FAILED | HID_ERR_ID)

#define HID_HOST_MUTEX_INIT_FAILED              \
        (MUTEX_INIT_FAILED | HID_ERR_ID)

/* HID-Device Error IDs */
#define HID_DEVICE_NOT_INIT                     (0x0011U | HID_ERR_ID)
#define HID_DEVICE_INVALID_PARAMS               (0x0012U | HID_ERR_ID)
#define HID_DEVICE_NOT_CONNECTED                (0x0013U | HID_ERR_ID)
#define HID_DEVICE_QUEUE_FULL                   (0x0014U | HID_ERR_ID)
#define HID_DEVICE_NO_MEMORY                    (0x0015U | HID_ERR_ID)
#define HID_DEVICE_LINK_LOST                    (0x0016U | HID_ERR_ID)
#define HID_DEVICE_SUSPENDED                    (0x0017U | HID_ERR_ID)
#define HID_DEVICE_INVALID_STATE                (0x0018U | HID_ERR_ID)
#define HID_DEVICE_MTU_CHECK_FAILED             (0x0019U | HID_ERR_ID)
#define HID_DEVICE_INVALID_INTERNAL_STATE       (0x001AU | HID_ERR_ID)

/* HID-Host Error IDs */
#define HID_HOST_INVALID_PARAM                  (0x0021U | HID_ERR_ID)
#define HID_HOST_ADD_DEV_FAILED                 (0x0022U | HID_ERR_ID)
#define HID_HOST_MEMORY_ALLOCATION_FAILED       (0x0023U | HID_ERR_ID)
#define HID_HOST_ALREADY_CONNECTED_TO_SPECIFIED_DEVICE \
                                                (0x0024U | HID_ERR_ID)
#define HID_HOST_NOT_CONNECTED_TO_SPECIFIED_DEVICE \
                                                (0x0025U | HID_ERR_ID)
#define HID_HOST_INVALID_INTERNAL_STATE         (0x0026U | HID_ERR_ID)
#define HID_HOST_HANDSHAKE_ERR_IND              (0x0027U | HID_ERR_ID)
#define HID_HOST_STACK_FULL                     (0x0028U | HID_ERR_ID)
#define HID_HOST_STACK_EMPTY                    (0x0029U | HID_ERR_ID)
#define HID_HOST_MTU_CHECK_FAILED               (0x002AU | HID_ERR_ID)

/* ======================================= Section 'HCRP' */
#define HCRP_MUTEX_INIT_FAILED                  \
        (MUTEX_INIT_FAILED | HCRP_ERR_ID)
#define HCRP_COND_INIT_FAILED                   \
        (COND_INIT_FAILED | HCRP_ERR_ID)
#define HCRP_MUTEX_LOCK_FAILED                  \
        (MUTEX_LOCK_FAILED | HCRP_ERR_ID)
#define HCRP_MUTEX_UNLOCK_FAILED                \
        (MUTEX_UNLOCK_FAILED | HCRP_ERR_ID)
#define HCRP_MEMORY_ALLOCATION_FAILED           \
        (MEMORY_ALLOCATION_FAILED | HCRP_ERR_ID)

#define HCRP_SUCCESS                            (0x0000U | HCRP_ERR_ID)
#define HCRP_WRONG_ARGUMENT_ERR                 (0x0011U | HCRP_ERR_ID)
#define HCRP_ENTITY_SEARCH_FAILED               (0x0012U | HCRP_ERR_ID)
#define HCRP_ENTITY_NOT_FOUND_ERR               (0x0013U | HCRP_ERR_ID)
#define HCRP_ERR_WRONG_ARGUMENT                 (0x0014U | HCRP_ERR_ID)
#define HCRP_DISCONNECT_FAILED                  (0x0015U | HCRP_ERR_ID)
#define HCRP_CONNECTION_SEARCH_FAILED           (0x0016U | HCRP_ERR_ID)
#define HCRP_CONNECTION_OPEN_ERR                (0x0017U | HCRP_ERR_ID)
#define HCRP_CONTROL_CHANNEL_ERR                (0x0018U | HCRP_ERR_ID)
#define HCRP_ENTITY_ALLOCATION_FAILED           (0x0019U | HCRP_ERR_ID)
#define HCRP_DATA_SEND_ERR                      (0x001AU | HCRP_ERR_ID)
#define HCRP_MALLOC_FAILED                      (0x001BU | HCRP_ERR_ID)
#define HCRP_PRINTER_STATUS_NOT_AVAILABLE       (0x001CU | HCRP_ERR_ID)
#define HCRP_PRINTER_PAPER_EMPTY                (0x001DU | HCRP_ERR_ID)
#define HCRP_PRINTER_ERROR                      (0x001EU | HCRP_ERR_ID)
#define HCRP_PRINTER_NOT_SELECTED               (0x001FU | HCRP_ERR_ID)

#define HCRP_FAILURE                            (0x00FFU | HCRP_ERR_ID)


/* ======================================= Section 'A2DP' */
#define A2DP_MUTEX_INIT_FAILED                  \
        (MUTEX_INIT_FAILED | A2DP_ERR_ID)
#define A2DP_COND_INIT_FAILED                   \
        (COND_INIT_FAILED | A2DP_ERR_ID)
#define A2DP_MUTEX_LOCK_FAILED                  \
        (MUTEX_LOCK_FAILED | A2DP_ERR_ID)
#define A2DP_MUTEX_UNLOCK_FAILED                \
        (MUTEX_UNLOCK_FAILED | A2DP_ERR_ID)
#define A2DP_MEMORY_ALLOCATION_FAILED           \
        (MEMORY_ALLOCATION_FAILED | A2DP_ERR_ID)

#define A2DP_UNKNOWN_CODEC_TYPE                 (0x0011U | A2DP_ERR_ID)
#define A2DP_CODEC_ENTITY_ALLOC_FAILED          (0x0012U | A2DP_ERR_ID)
#define A2DP_INVALID_PARAMETERS                 (0x0013U | A2DP_ERR_ID)
#define A2DP_INVALID_STATE                      (0x0014U | A2DP_ERR_ID)
#define A2DP_NO_MATCHING_CODEC_FOUND            (0x0015U | A2DP_ERR_ID)
#define A2DP_CONNECTION_ABORT_RECEIVED          (0x0016U | A2DP_ERR_ID)
#define A2DP_CODEC_ENTITY_DISCONNECTED          (0x0017U | A2DP_ERR_ID)
#define A2DP_CODEC_ENTITY_NOT_SOURCE            (0x0018U | A2DP_ERR_ID)
#define A2DP_API_NOT_SUPPORTED                  (0x0019U | A2DP_ERR_ID)
#define A2DP_ALREADY_INITIALIZED                (0x001AU | A2DP_ERR_ID)
#define A2DP_CODEC_ENTITY_NOT_DISCONNECTED      (0x001BU | A2DP_ERR_ID)
#define A2DP_CODEC_ENTITY_NOT_REGISTERED        (0x001CU | A2DP_ERR_ID)
#define A2DP_INSUFFICIENT_BUFFER_SIZE           (0x001DU | A2DP_ERR_ID)
#define A2DP_DELAY_REPORT_SEND_FAILED           (0x001EU | A2DP_ERR_ID)


/* ======================================= Section 'AVRCP' */
#define AVRCP_MUTEX_INIT_FAILED                 \
        (MUTEX_INIT_FAILED | AVRCP_ERR_ID)
#define AVRCP_COND_INIT_FAILED                  \
        (COND_INIT_FAILED | AVRCP_ERR_ID)
#define AVRCP_MUTEX_LOCK_FAILED                 \
        (MUTEX_LOCK_FAILED | AVRCP_ERR_ID)
#define AVRCP_MUTEX_UNLOCK_FAILED               \
        (MUTEX_UNLOCK_FAILED | AVRCP_ERR_ID)
#define AVRCP_MEMORY_ALLOCATION_FAILED          \
        (MEMORY_ALLOCATION_FAILED | AVRCP_ERR_ID)

#define AVRCP_INVALID_PARAMETERS                (0x0011U | AVRCP_ERR_ID)
#define AVRCP_PROFILE_ENTITY_ALLOC_FAILED       (0x0012U | AVRCP_ERR_ID)
#define AVRCP_COMMAND_CONTEXT_ALLOC_FAILED      (0x0013U | AVRCP_ERR_ID)
#define AVRCP_INVALID_STATE                     (0x0014U | AVRCP_ERR_ID)
#define AVRCP_INVALID_PROFILE                   (0x0015U | AVRCP_ERR_ID)
#define AVRCP_PROFILE_DISCONNECTED              (0x0016U | AVRCP_ERR_ID)
#define AVRCP_PROFILE_NOT_REGISTERED            (0x0017U | AVRCP_ERR_ID)
#define AVRCP_PROFILE_NOT_STARTED               (0x0018U | AVRCP_ERR_ID)
#define AVRCP_PROFILE_NOT_CONNECTED             (0x0019U | AVRCP_ERR_ID)
#define AVRCP_CONNECT_IND_NOT_EXPECTED          (0x001AU | AVRCP_ERR_ID)
#define AVRCP_RESPONSE_NOT_EXPECTED             (0x001BU | AVRCP_ERR_ID)
#define AVRCP_EVENT_NTF_NOT_EXPECTED            (0x001CU | AVRCP_ERR_ID)
#define AVRCP_COMMAND_TIMEOUT                   (0x001DU | AVRCP_ERR_ID)
#define AVRCP_PROFILE_NOT_DISCONNECTED          (0x001EU | AVRCP_ERR_ID)
#define AVRCP_PROFILE_NOT_STOPPED               (0x001FU | AVRCP_ERR_ID)
#define AVRCP_ALREADY_INITIALIZED               (0x0020U | AVRCP_ERR_ID)

/* ======================================= Cover Art Module Related  */
#define COVER_ART_MUTEX_INIT_FAILED                 \
        (MUTEX_INIT_FAILED | COVER_ART_ERR_ID)
#define COVER_ART_COND_INIT_FAILED                  \
        (COND_INIT_FAILED | COVER_ART_ERR_ID)
#define COVER_ART_MUTEX_LOCK_FAILED                 \
        (MUTEX_LOCK_FAILED | COVER_ART_ERR_ID)
#define COVER_ART_MUTEX_UNLOCK_FAILED               \
        (MUTEX_UNLOCK_FAILED | COVER_ART_ERR_ID)
#define COVER_ART_MEMORY_ALLOCATION_FAILED          \
        (MEMORY_ALLOCATION_FAILED | COVER_ART_ERR_ID)


#define AVRCP_CA_INVALID_STATE                  (0x0011U | COVER_ART_ERR_ID)
#define AVRCP_CA_INVALID_PARAMETERS             (0x0012U | COVER_ART_ERR_ID)
#define AVRCP_CA_ENTITY_START_FAILED            (0x0013U | COVER_ART_ERR_ID)
#define AVRCP_CA_ENTITY_STOP_FAILED             (0x0014U | COVER_ART_ERR_ID)
#define AVRCP_CA_INVALID_CONNECTION_ID          (0x0015U | COVER_ART_ERR_ID)
#define AVRCP_CA_GET_OPERATION_ABORTED          (0x0016U | COVER_ART_ERR_ID)
#define AVRCP_CA_INVALID_OPERATION              (0x0017U | COVER_ART_ERR_ID)
#define AVRCP_CA_INVALID_CONNECT_REQ_LENGTH     (0x0018U | COVER_ART_ERR_ID)

/* ======================================= Section 'PBAP' */
#define PBAP_MUTEX_INIT_FAILED                  \
        (MUTEX_INIT_FAILED | PBAP_ERR_ID)
#define PBAP_COND_INIT_FAILED                   \
        (COND_INIT_FAILED | PBAP_ERR_ID)
#define PBAP_MUTEX_LOCK_FAILED                  \
        (MUTEX_LOCK_FAILED | PBAP_ERR_ID)
#define PBAP_MUTEX_UNLOCK_FAILED                \
        (MUTEX_UNLOCK_FAILED | PBAP_ERR_ID)
#define PBAP_MEMORY_ALLOCATION_FAILED           \
        (MEMORY_ALLOCATION_FAILED | PBAP_ERR_ID)

#define PBAP_INVALID_STATE                      (0x0011U | PBAP_ERR_ID)
#define PBAP_INVALID_PARAMETERS                 (0x0012U | PBAP_ERR_ID)
#define PBAP_ENTITY_UNAVAILABLE                 (0x0013U | PBAP_ERR_ID)
#define PBAP_INVALID_REQUEST                    (0x0014U | PBAP_ERR_ID)
#define PBAP_NOT_AUTHENTICATED                  (0x0015U | PBAP_ERR_ID)
#define PBAP_INVALID_OPERATION                  (0x0016U | PBAP_ERR_ID)
#define PBAP_AUTHENTICATION_FAILED              (0x0017U | PBAP_ERR_ID)
#define PBAP_GET_OPERATION_ABORTED              (0x0018U | PBAP_ERR_ID)
#define PBAP_INVALID_CONNECT_REQ_MAX_TX_LENGTH  (0x0019U | PBAP_ERR_ID)


/* ======================================= Section 'SAP' */
#define SAP_MUTEX_INIT_FAILED                  \
        (MUTEX_INIT_FAILED | SAP_ERR_ID)
#define SAP_COND_INIT_FAILED                   \
        (COND_INIT_FAILED | SAP_ERR_ID)
#define SAP_MUTEX_LOCK_FAILED                  \
        (MUTEX_LOCK_FAILED | SAP_ERR_ID)
#define SAP_MUTEX_UNLOCK_FAILED                \
        (MUTEX_UNLOCK_FAILED | SAP_ERR_ID)
#define SAP_MEMORY_ALLOCATION_FAILED           \
        (MEMORY_ALLOCATION_FAILED | SAP_ERR_ID)

#define SAP_ERR_CLIENT_NOT_INITILIZED           (0x0011U | SAP_ERR_ID)
#define SAP_ERR_CLIENT_ALREADY_INITILIZED       (0x0012U | SAP_ERR_ID)
#define SAP_ERR_SERVER_ALREADY_INITILIZED       (0x0013U | SAP_ERR_ID)
#define SAP_ERR_NULL_APPL_CALLBACK              (0x0014U | SAP_ERR_ID)
#define SAP_ERR_RFCOMM_ALREADY_CONNECTED        (0x0015U | SAP_ERR_ID)
#define SAP_ERR_ALREADY_CONNECTED               (0x0016U | SAP_ERR_ID)
#define SAP_ERR_NOT_CONNECTED                   (0x0017U | SAP_ERR_ID)
#define SAP_ERR_SELF_INITIATED_DISCONNECTION_ALREADY_STARTED \
                                                (0x0018U | SAP_ERR_ID)
#define SAP_ERR_PEER_INITIATED_DISCONNECTION_ALREADY_STARTED \
                                                (0x0019U | SAP_ERR_ID)
#define SAP_ERR_PROCEDURE_DISALLOWED            (0x001AU | SAP_ERR_ID)
#define SAP_ERR_INVALID_PARAMETER               (0x001BU | SAP_ERR_ID)


/* ======================================= Section 'MAP' */
#define MAP_MUTEX_INIT_FAILED                  \
        (MUTEX_INIT_FAILED | MAP_ERR_ID)
#define MAP_COND_INIT_FAILED                   \
        (COND_INIT_FAILED | MAP_ERR_ID)
#define MAP_MUTEX_LOCK_FAILED                  \
        (MUTEX_LOCK_FAILED | MAP_ERR_ID)
#define MAP_MUTEX_UNLOCK_FAILED                \
        (MUTEX_UNLOCK_FAILED | MAP_ERR_ID)
#define MAP_MEMORY_ALLOCATION_FAILED           \
        (MEMORY_ALLOCATION_FAILED | MAP_ERR_ID)

#define MAP_INVALID_STATE                       (0x0011U | MAP_ERR_ID)
#define MAP_INVALID_PARAMETERS                  (0x0012U | MAP_ERR_ID)
#define MAP_ENTITY_START_FAILED                 (0x0013U | MAP_ERR_ID)
#define MAP_ENTITY_STOP_FAILED                  (0x0014U | MAP_ERR_ID)
#define MAP_INVALID_CONNECT_REQ_LENGTH          (0x0015U | MAP_ERR_ID)
#define MAP_OPERATION_ABORTED                   (0x0016U | MAP_ERR_ID)
#define MAP_INVALID_CONNECTION_ID               (0x0017U | MAP_ERR_ID)
#define MAP_ENTITY_ALLOC_FAILED                 (0x0018U | MAP_ERR_ID)
#define MAP_ENTITY_SEARCH_FAILED                (0x0019U | MAP_ERR_ID)

/* ======================================= Section 'CTN' */
#define CTN_MUTEX_INIT_FAILED                  \
        (MUTEX_INIT_FAILED | CTN_ERR_ID)
#define CTN_COND_INIT_FAILED                   \
        (COND_INIT_FAILED | CTN_ERR_ID)
#define CTN_MUTEX_LOCK_FAILED                  \
        (MUTEX_LOCK_FAILED | CTN_ERR_ID)
#define CTN_MUTEX_UNLOCK_FAILED                \
        (MUTEX_UNLOCK_FAILED | CTN_ERR_ID)
#define CTN_MEMORY_ALLOCATION_FAILED           \
        (MEMORY_ALLOCATION_FAILED | CTN_ERR_ID)

#define CTN_INVALID_STATE                       (0x0011U | CTN_ERR_ID)
#define CTN_INVALID_PARAMETERS                  (0x0012U | CTN_ERR_ID)
#define CTN_ENTITY_START_FAILED                 (0x0013U | CTN_ERR_ID)
#define CTN_ENTITY_STOP_FAILED                  (0x0014U | CTN_ERR_ID)
#define CTN_INVALID_CONNECT_REQ_LENGTH          (0x0015U | CTN_ERR_ID)
#define CTN_GET_OPERATION_ABORTED               (0x0016U | CTN_ERR_ID)
#define CTN_PUT_OPERATION_ABORTED               (0x0017U | CTN_ERR_ID)
#define CTN_INVALID_CONNECTION_ID               (0x0018U | CTN_ERR_ID)
#define CTN_ENTITY_ALLOC_FAILED                 (0x0019U | CTN_ERR_ID)
#define CTN_ENTITY_SEARCH_FAILED                (0x001AU | CTN_ERR_ID)



/* ======================================= Section 'GNSS' */
#define GNSS_MUTEX_INIT_FAILED                  \
        (MUTEX_INIT_FAILED | GNSS_ERR_ID)
#define GNSS_COND_INIT_FAILED                   \
        (COND_INIT_FAILED | GNSS_ERR_ID)
#define GNSS_MUTEX_LOCK_FAILED                  \
        (MUTEX_LOCK_FAILED | GNSS_ERR_ID)
#define GNSS_MUTEX_UNLOCK_FAILED                \
        (MUTEX_UNLOCK_FAILED | GNSS_ERR_ID)
#define GNSS_MEMORY_ALLOCATION_FAILED           \
        (MEMORY_ALLOCATION_FAILED | GNSS_ERR_ID)

#define GNSS_ERR_BAD_STATE                      (0x0011U | GNSS_ERR_ID)
#define GNSS_ERR_DATALEN_ZERO                   (0x0012U | GNSS_ERR_ID)
#define GNSS_ERR_DATA_NULL                      (0x0013U | GNSS_ERR_ID)
#define GNSS_ERR_CB_NULL                        (0x0014U | GNSS_ERR_ID)
#define GNSS_ERR_INVALID_CONN_ENTITY            (0x0015U | GNSS_ERR_ID)
#define GNSS_ERR_CONNECTIONS_EXIST              (0x0016U | GNSS_ERR_ID)
#define GNSS_ERR_CONNECTION_LIMIT_REACHED       (0x0017U | GNSS_ERR_ID)


/* ======================================= Section 'BT-FOPS' */
#define BT_FOPS_MUTEX_INIT_FAILED               \
        (MUTEX_INIT_FAILED | BT_FOPS_ERR_ID)
#define BT_FOPS_COND_INIT_FAILED                \
        (COND_INIT_FAILED | BT_FOPS_ERR_ID)
#define BT_FOPS_MUTEX_LOCK_FAILED               \
        (MUTEX_LOCK_FAILED | BT_FOPS_ERR_ID)
#define BT_FOPS_MUTEX_UNLOCK_FAILED             \
        (MUTEX_UNLOCK_FAILED | BT_FOPS_ERR_ID)
#define BT_FOPS_MEMORY_ALLOCATION_FAILED        \
        (MEMORY_ALLOCATION_FAILED | BT_FOPS_ERR_ID)

#define BT_FOPS_ERR_GET_CURRECT_DIRECTORY       (0x0011U | BT_FOPS_ERR_ID)
#define BT_FOPS_ERR_GET_FILE_ATTRIBUTES         (0x0012U | BT_FOPS_ERR_ID)
#define BT_FOPS_ERR_SET_PATH_FORWARD            (0x0013U | BT_FOPS_ERR_ID)
#define BT_FOPS_ERR_SET_PATH_BACKWARD           (0x0014U | BT_FOPS_ERR_ID)
#define BT_FOPS_ERR_CREATE_FOLDER               (0x0015U | BT_FOPS_ERR_ID)
#define BT_FOPS_ERR_FILE_OPEN                   (0x0016U | BT_FOPS_ERR_ID)
#define BT_FOPS_ERR_FILE_WRITE                  (0x0017U | BT_FOPS_ERR_ID)
#define BT_FOPS_ERR_FILE_READ                   (0x0018U | BT_FOPS_ERR_ID)
#define BT_FOPS_ERR_FOLDER_DELETE               (0x0019U | BT_FOPS_ERR_ID)
#define BT_FOPS_ERR_FILE_DELETE                 (0x001AU | BT_FOPS_ERR_ID)
#define BT_FOPS_ERR_FILE_NOT_FOUND              (0x001BU | BT_FOPS_ERR_ID)
#define BT_FOPS_INVALID_PARAMETER_VALUE         (0x001CU | BT_FOPS_ERR_ID)
#define BT_FOPS_ERR_FILE_SEEK_FAILED            (0x001DU | BT_FOPS_ERR_ID)
#define BT_FOPS_ERR_FILE_COPY                   (0x001EU | BT_FOPS_ERR_ID)
#define BT_FOPS_ERR_FILE_MOVE                   (0x001FU | BT_FOPS_ERR_ID)
#define BT_FOPS_ERR_SET_FILE_ATTRIBUTES         (0x0020U | BT_FOPS_ERR_ID)

/* ======================================= Section 'AT Parser' */
#define AT_PARSER_MUTEX_INIT_FAILED             \
        (MUTEX_INIT_FAILED | AT_PARSER_ERR_ID)
#define AT_PARSER_COND_INIT_FAILED              \
        (COND_INIT_FAILED | AT_PARSER_ERR_ID)
#define AT_PARSER_MUTEX_LOCK_FAILED             \
        (MUTEX_LOCK_FAILED | AT_PARSER_ERR_ID)
#define AT_PARSER_MUTEX_UNLOCK_FAILED           \
        (MUTEX_UNLOCK_FAILED | AT_PARSER_ERR_ID)
#define AT_PARSER_MEMORY_ALLOCATION_FAILED      \
        (MEMORY_ALLOCATION_FAILED | AT_PARSER_ERR_ID)

#define AT_PARSER_ERROR_NULL_INPUT              (0x0011U | AT_PARSER_ERR_ID)
#define AT_PARSER_ERROR_INVALID_STRING          (0x0012U | AT_PARSER_ERR_ID)
#define AT_PARSER_ERROR_INSUFFICIENT_STRING_LEN (0x0013U | AT_PARSER_ERR_ID)
#define AT_PARSER_ERROR_STR_INCOMPLETE          (0x0014U | AT_PARSER_ERR_ID)
#define AT_PARSER_ERROR_CMD_IMPROPER_END        (0x0015U | AT_PARSER_ERR_ID)
#define AT_PARSER_ERROR_PARAMETER_LENGTH_MISMATCH \
                                                (0x0016U | AT_PARSER_ERR_ID)
#define AT_PARSER_ERROR_INVALID_KEYWORD         (0x0017U | AT_PARSER_ERR_ID)
#define AT_PARSER_ERROR_INVALID_KEYWORD_QUALIFIER \
                                                (0x0018U | AT_PARSER_ERR_ID)
#define AT_PARSER_ERROR_SPECIAL_KEYWORD         (0x0019U | AT_PARSER_ERR_ID)
#define AT_PARSER_ERROR_INVALID_PARAMETER       (0x001AU | AT_PARSER_ERR_ID)
#define AT_PARSER_ERROR_STR_INVALID             (0x001BU | AT_PARSER_ERR_ID)

#define AT_PARSER_ERR_NOPROCESSING_DONE         (0x0021U | AT_PARSER_ERR_ID)
#define AT_PARSER_PARTIAL_RESPONSE              (0x0022U | AT_PARSER_ERR_ID)
#define AT_PARSER_INVALID_KW                    (0x0023U | AT_PARSER_ERR_ID)
#define AT_PARSER_PARSE_COMPLETE_PENDING_RESPONSE\
                                                (0x0024U | AT_PARSER_ERR_ID)
#define AT_PARSER_PARSE_COMPLETE                (0x0025U | AT_PARSER_ERR_ID)
#define AT_PARSER_PARTIAL_SUCCESS               (0x0026U | AT_PARSER_ERR_ID)
#define AT_PARSER_STR_INCOMPLETE                (0x0027U | AT_PARSER_ERR_ID)
#define AT_PARSER_INVALID_KW_PENDING_RESPONSE \
                                                (0x0028U | AT_PARSER_ERR_ID)


/* ======================================= Section 'JPL' */
#define JPL_MUTEX_INIT_FAILED                   \
        (MUTEX_INIT_FAILED | JPL_ERR_ID)
#define JPL_COND_INIT_FAILED                    \
        (COND_INIT_FAILED | JPL_ERR_ID)
#define JPL_MUTEX_LOCK_FAILED                   \
        (MUTEX_LOCK_FAILED | JPL_ERR_ID)
#define JPL_MUTEX_UNLOCK_FAILED                 \
        (MUTEX_UNLOCK_FAILED | JPL_ERR_ID)
#define JPL_MEMORY_ALLOCATION_FAILED            \
        (MEMORY_ALLOCATION_FAILED | JPL_ERR_ID)

#define JPL_ERR_INVALID_PARAMETER               (0x0011U | JPL_ERR_ID)
#define JPL_ERR_INVALID_STATE                   (0x0012U | JPL_ERR_ID)
#define JPL_ERR_BUFFER_OVERFLOW                 (0x0013U | JPL_ERR_ID)
#define JPL_ERR_SBC_FRAG_NOT_SUPPORTED          (0x0014U | JPL_ERR_ID)
#define JPL_ERR_MORE_PCM_FRAMES_AVAILABLE       (0x0015U | JPL_ERR_ID)


/* ======================================= Section 'HAM' */
#define HAM_MUTEX_INIT_FAILED                   \
        (MUTEX_INIT_FAILED | HAM_ERR_ID)
#define HAM_COND_INIT_FAILED                    \
        (COND_INIT_FAILED | HAM_ERR_ID)
#define HAM_MUTEX_LOCK_FAILED                   \
        (MUTEX_LOCK_FAILED | HAM_ERR_ID)
#define HAM_MUTEX_UNLOCK_FAILED                 \
        (MUTEX_UNLOCK_FAILED | HAM_ERR_ID)
#define HAM_MEMORY_ALLOCATION_FAILED            \
        (MEMORY_ALLOCATION_FAILED | HAM_ERR_ID)

#define HAM_SUCCESS                             API_SUCCESS

#define HAM_ERR_STOP_PENDING                    (0x0011U | HAM_ERR_ID)
#define HAM_ERR_HCI_CON_COMP_ERR                (0x0012U | HAM_ERR_ID)
#define HAM_ERR_HCI_ACL_PARAM_ERR               (0x0013U | HAM_ERR_ID)
#define HAM_ERR_HCI_DISCON_COMP_ERR             (0x0014U | HAM_ERR_ID)
#define HAM_ERR_LINK_FROM_BD_ADDR               (0x0015U | HAM_ERR_ID)
#define HAM_ERR_NO_FREE_LINK_HDL                (0x0016U | HAM_ERR_ID)
#define HAM_ERR_INVALID_PROFILE_IDX             (0x0017U | HAM_ERR_ID)
#define HAM_ERR_NO_LINK_WITH_GIVEN_ACL          (0x0018U | HAM_ERR_ID)
#define HAM_ERR_NO_LINK_WITH_GIVEN_SCO          (0x0019U | HAM_ERR_ID)
#define HAM_ERR_HCI_ACL_RECONNECT               (0x001AU | HAM_ERR_ID)
#define HAM_ERR_PARAM_MISMATCH                  (0x001BU | HAM_ERR_ID)
#define HAM_ERR_INVALID_PROFILE                 (0x001CU | HAM_ERR_ID)

/* SDP Errors */
#define HAM_ERR_SDP_SEARCH_ATTRIB_RESP          (0x0020U | HAM_ERR_ID)
#define HAM_ERR_SDP_ERROR_RESP                  (0x0021U | HAM_ERR_ID)
#define HAM_ERR_SDP_OPEN                        (0x0022U | HAM_ERR_ID)
#define HAM_ERR_SDP_MEM_ALLOC                   (0x0023U | HAM_ERR_ID)
#define HAM_ERR_SDP_SEARCH_ATTRIB               (0x0024U | HAM_ERR_ID)
#define HAM_ERR_SDP_EVENT_ERR                   (0x0025U | HAM_ERR_ID)

/** \} */

#endif /* _H_BT_ERROR_ */

