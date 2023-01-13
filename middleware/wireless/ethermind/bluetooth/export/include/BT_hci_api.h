
/**
 *  \file BT_hci_api.h
 *
 *  \brief This Header File contains the API Function declarations of the APIs
 *  exported by the Mindtree Bluetooth Stack for the HCI Layer.
 */

/*
 *  Copyright (C) 2014. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_HCI_API_
#define _H_BT_HCI_API_

/* =================================================== Header File Inclusion */
#include "BT_common.h"

/**
 * \addtogroup bt_protocol Protocols
 * \{
 */
/**
 * \defgroup  hci_module  HCI (Host Controller Interface)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Host Controller Interface module to the Application and other upper
 *  layers of the stack.
 */

/**
 * \defgroup hci_defines Defines
 * \{
 */

/**
 * \defgroup hci_constants Constants
 * \{
 */

/* =================================================== Constants/Definitions */
/** HCI Device Role - Master/Slave */
/** Device Role Master */
#define BT_DEVICE_ROLE_MASTER                            0x00U
/** Device Role Slave */
#define BT_DEVICE_ROLE_SLAVE                             0x01U

/* HCI Link Types ACL/SCO/eSCO */
#define HCI_SCO_LINK                            0x00U
#define HCI_ACL_LINK                            0x01U
#ifdef BT_HCI_1_2
#define HCI_ESCO_LINK                           0x02U
#endif /* BT_HCI_1_2 */
#ifdef BT_4_0
#define HCI_LE_LINK                             0x03U
#endif /* BT_4_0 */

/* LMP Packet Types - ACL Connection */
#define LMP_ACL_DM1                             0x0008U
#define LMP_ACL_DH1                             0x0010U
#define LMP_ACL_DM3                             0x0400U
#define LMP_ACL_DH3                             0x0800U
#define LMP_ACL_DM5                             0x4000U
#define LMP_ACL_DH5                             0x8000U

#ifdef BT_2_0_EDR
#define LMP_ACL_2_DH1                           0x0002U
#define LMP_ACL_3_DH1                           0x0004U
#define LMP_ACL_2_DH3                           0x0100U
#define LMP_ACL_3_DH3                           0x0200U
#define LMP_ACL_2_DH5                           0x1000U
#define LMP_ACL_3_DH5                           0x2000U
#endif /* BT_2_0_EDR */

/* LMP Packet Types - SCO Connection */
#define LMP_SCO_HV1                             0x0020U
#define LMP_SCO_HV2                             0x0040U
#define LMP_SCO_HV3                             0x0080U

/* LMP Packet Types - eSCO Connection */
#define LMP_ESCO_HV1                            0x0001U
#define LMP_ESCO_HV2                            0x0002U
#define LMP_ESCO_HV3                            0x0004U
#define LMP_ESCO_EV3                            0x0008U
#define LMP_ESCO_EV4                            0x0010U
#define LMP_ESCO_EV5                            0x0020U

#ifdef BT_2_0_EDR
#define LMP_ESCO_2_EV3                          0x0040U
#define LMP_ESCO_3_EV3                          0x0080U
#define LMP_ESCO_2_EV5                          0x0100U
#define LMP_ESCO_3_EV5                          0x0200U
#endif /* BT_2_0_EDR */

/* Synchronous Connections Parameters - Max Latency */
#define LMP_MAX_LATENCY_DONT_CARE               0xFFFFU

/* Synchronous Connections Parameters - Retransmission Effort */
#define LMP_RETX_EFFORT_NONE                    0x00U
#define LMP_RETX_EFFORT_POWER                   0x01U
#define LMP_RETX_EFFORT_QUALITY                 0x02U
#define LMP_RETX_EFFORT_DONT_CARE               0xFFU

/* Synchronous Connections Parameters - Voice Settings */
/* Input Coding */
#define LMP_VOICE_INPUT_CODING_LINEAR           0x0000U
#define LMP_VOICE_INPUT_CODING_U_LAW            0x0100U
#define LMP_VOICE_INPUT_CODING_A_LAW            0x0200U

/* Input Data Format */
#define LMP_VOICE_INPUT_FORMAT_1S_COMPLEMENT    0x0000U
#define LMP_VOICE_INPUT_FORMAT_2S_COMPLEMENT    0x0040U
#define LMP_VOICE_INPUT_FORMAT_SIGN_MAGNITUDE   0x0080U
#define LMP_VOICE_INPUT_FORMAT_UNSIGNED         0x00C0U

/* Input Sample Size */
#define LMP_VOICE_INPUT_SAMPLE_8_BIT            0x0000U
#define LMP_VOICE_INPUT_SAMPLE_16_BIT           0x0020U

/* Linear PCM Bit Position */
#define LMP_VOICE_LINEAR_BIT_POS_0              0x0000U
#define LMP_VOICE_LINEAR_BIT_POS_1              0x0004U
#define LMP_VOICE_LINEAR_BIT_POS_2              0x0008U
#define LMP_VOICE_LINEAR_BIT_POS_3              0x000CU
#define LMP_VOICE_LINEAR_BIT_POS_4              0x0010U
#define LMP_VOICE_LINEAR_BIT_POS_5              0x0014U
#define LMP_VOICE_LINEAR_BIT_POS_6              0x0018U
#define LMP_VOICE_LINEAR_BIT_POS_7              0x001CU

/* Air Coding Format */
#define LMP_VOICE_AIR_CODING_CVSD               0x0000U
#define LMP_VOICE_AIR_CODING_U_LAW              0x0001U
#define LMP_VOICE_AIR_CODING_A_LAW              0x0002U
#define LMP_VOICE_AIR_CODING_TRANSPARENT        0x0003U

/*
 *  Default Voice Settings to be used for Synchronous Connections:
 *  Input Coding: A-Law
 *  Input Format: 2's Compliment
 *  Input Sample Size: 8-bit
 *  Linear PCM Bit Position: 3
 *  Air Coding Format: CVSD
 */
#define LMP_VOICE_SETTING_DEFAULT \
        (LMP_VOICE_INPUT_CODING_LINEAR|\
         LMP_VOICE_INPUT_FORMAT_2S_COMPLEMENT|\
         LMP_VOICE_INPUT_SAMPLE_16_BIT|\
         LMP_VOICE_LINEAR_BIT_POS_0|\
         LMP_VOICE_AIR_CODING_CVSD)

/* Default eSCO Channel Parameters */
/* Transmit & Receive Bandwidth - 8000 octets/sec */
#define LMP_ESCO_TX_BANDWIDTH_DEFAULT           8000U
#define LMP_ESCO_RX_BANDWIDTH_DEFAULT           8000U

#ifdef HCI_ENH_SCO
/* Parameters related to Enhaced SCO Configuration */

/* Coding Format */
#define LMP_CODING_FRMT_U_LAW                       0x00U
#define LMP_CODING_FRMT_A_LAW                       0x01U
#define LMP_CODING_FRMT_CVSD                        0x02U
#define LMP_CODING_FRMT_TRANSPARENT                 0x03U
#define LMP_CODING_FRMT_LINEAR_PCM                  0x04U
#define LMP_CODING_FRMT_MSBC                        0x05U
#define LMP_CODING_FRMT_VS                          0xFFU

/* Transmit and Receive Bandwidth */
#define LMP_ENH_SCO_TX_BANDWIDTH_DEFAULT            16000U
#define LMP_ENH_SCO_RX_BANDWIDTH_DEFAULT            16000U

/* Input and Output Data Path */
#define ENH_SCO_DATA_PATH_HCI                       0x00U
#define ENH_SCO_DATA_PATH_AUDIO_TEST_MODE           0xFFU

/* MSBC Default Frame length */
#define MSBC_FRAME_SIZE                             60U
#endif /* HCI_ENH_SCO */

/* Default eSCO Max Latency */
#define LMP_ESCO_MAX_LATENCY_DEFAULT            LMP_MAX_LATENCY_DONT_CARE

/* Default eSCO Retransmission Effort */
#define LMP_ESCO_RETX_EFFORT_DEFAULT            LMP_RETX_EFFORT_DONT_CARE

/* Default eSCO Packet Type */
#define LMP_ESCO_PACKET_TYPE_DEFAULT \
        (LMP_ESCO_HV1|LMP_ESCO_HV2|LMP_ESCO_HV3|\
         LMP_ESCO_EV3|LMP_ESCO_EV4|LMP_ESCO_EV5)

/* Type of PIN Codes */
#define HCI_VARIABLE_PIN_CODE                   0x00U
#define HCI_FIXED_PIN_CODE                      0x01U

/** Invalid HCI Connection Handle */
#define HCI_INVALID_CONNECTION_HANDLE           0xFFFFU

/* Type of Link Keys */
#define HCI_LINK_KEY_COMBINATION                0x00U
#define HCI_LINK_KEY_LOCAL_UNIT                 0x01U
#define HCI_LINK_KEY_REMOTE_UNIT                0x02U
#define HCI_LINK_KEY_DEBUG_COMBINATION          0x03U
#define HCI_LINK_KEY_UNAUTHENTICATED            0x04U
#define HCI_LINK_KEY_AUTHENTICATED              0x05U
#define HCI_LINK_KEY_CHANGED_COMBINATION        0x06U

#ifdef BT_BRSC
#define HCI_LINK_KEY_UNAUTHENTICATED_P_256      0x07U
#define HCI_LINK_KEY_AUTHENTICATED_P_256        0x08U
#endif /* BT_BRSC */

#ifdef BT_EIR
#define HCI_EIR_MAX_DATA_LEN                    240U
#endif /* BT_EIR */

/** Maximum Advertising Data Length */
#define HCI_MAX_ADVERTISING_DATA_LENGTH                  0x1FU

/** Advertising Enable/Disable */
/** Advertising disabled */
#define HCI_ADVERTISING_DISABLE                          0x00U
/** Advertising enabled */
#define HCI_ADVERTISING_ENABLE                           0x01U

/** Advertising Type */
/** Connectable undirected advertising */
#define HCI_ADV_IND                                      0x00U
/** Connectable directed advertising - High Duty Cycle */
#define HCI_ADV_DIRECT_IND                               0x01U
/** Scannable undirected advertising */
#define HCI_ADV_SCAN_IND                                 0x02U
/** Non connectable undirected advertising */
#define HCI_ADV_NONCONN_IND                              0x03U
/** Connectable directed advertising - Low Duty Cycle */
#define HCI_ADV_DIRECT_LDC_IND                           0x04U

/** Scan Respond Event Type */
#define HCI_SCAN_RSP                                     0x04U

/** Address Type */
/** Public Device Address */
#define BD_ADDR_TYPE_PUBLIC                              0x00U
/** Random Device Address */
#define BD_ADDR_TYPE_RANDOM                              0x01U

/** Advertising Filter Policy */
/** Allow Scan Request from Any, Allow Connect Request from Any */
#define HCI_ADV_FLTR_SCAN_REQ_ANY_CON_REQ_ANY            0x00U
/** Allow Scan Request from White List Only, Allow Connect Request from Any */
#define HCI_ADV_FLTR_SCAN_REQ_WH_LIST_CON_REQ_ANY        0x01U
/** Allow Scan Request from Any, Allow Connect Request from White List Only */
#define HCI_ADV_FLTR_SCAN_REQ_ANY_CON_REQ_WH_LIST        0x02U
/**
 * Allow Scan Request from White List Only, Allow Connect Request
 * from White List Only.
 */
#define HCI_ADV_FLTR_SCAN_REQ_WH_LIST_CON_REQ_WH_LIST    0x03U

/** LE Scan Enable/Disable */
/** Scanning disabled */
#define HCI_LE_SCAN_DISABLE                              0x00U
/** Scanning enabled */
#define HCI_LE_SCAN_ENABLE                               0x01U

/** LE Scan Filter Duplicates Enable/Disable */
/** Duplicate filtering disabled */
#define HCI_LE_SCAN_DUP_FLTR_DISABLE                     0x00U
/** Duplicate filtering enabled */
#define HCI_LE_SCAN_DUP_FLTR_ENABLE                      0x01U

/** LE Scan Type */
/** Passive Scanning. No SCAN_REQ packets shall be sent */
#define HCI_LE_SCAN_TYPE_PASSIVE                         0x00U
/** Active scanning. SCAN_REQ packets may be sent */
#define HCI_LE_SCAN_TYPE_ACTIVE                          0x01U

/** LE Scanning Filter Policy */
/**
 * Accept all advertisement packets.
 * Directed advertising packets which are not addressed for this device
 * shall be ignored.
 */
#define HCI_LE_SCAN_FLTR_ACCEPT_ALL                      0x00U

/**
 * Ignore advertisement packets from devices not in the White List Only.
 * Directed advertising packets which are not addressed for this device
 * shall be ignored.
 */
#define HCI_LE_SCAN_FLTR_USE_WH_LIST                     0x01U

/** LE Initiator Filter Policy */
/**
 * White list is not used to determine which advertiser to connect to.
 * Peer_Address_Type and Peer_Address shall be used.
 */
#define HCI_INIT_FLTR_DONOT_USE_WH_LIST                  0x00U
/**
 * White list is used to determine which advertiser to connect to.
 * Peer_Address_Type and Peer_Address shall be ignored.
 */
#define HCI_INIT_FLTR_USE_WH_LIST                        0x01U

/** LE Transmitter Test Packet Payload */
/** Pseudo-Random bit sequence 9 */
#define HCI_LE_TX_TEST_PKT_TYPE_00                       0x00U
/** Pattern of alternating bits '11110000' */
#define HCI_LE_TX_TEST_PKT_TYPE_01                       0x01U
/** Pattern of alternating bits '10101010' */
#define HCI_LE_TX_TEST_PKT_TYPE_02                       0x02U
/** Pseudo-Random bit sequence 15 */
#define HCI_LE_TX_TEST_PKT_TYPE_03                       0x03U
/** Pattern of All '1' bits */
#define HCI_LE_TX_TEST_PKT_TYPE_04                       0x04U
/** Pattern of All '0' bits */
#define HCI_LE_TX_TEST_PKT_TYPE_05                       0x05U
/** Pattern of alternating bits '00001111' */
#define HCI_LE_TX_TEST_PKT_TYPE_06                       0x06U
/** Pattern of alternating bits '0101' */
#define HCI_LE_TX_TEST_PKT_TYPE_07                       0x07U

/** Transmit Power Level Type */
/** Read Current Transmit Power Level */
#define HCI_READ_CUR_TX_POWER_LEVEL                      0x00U
/** Read Maximum Transmit Power Level */
#define HCI_READ_MAX_TX_POWER_LEVEL                      0x01U

/** Connection Parameter Range */
/** Connection Interval Minimum - 0x0006 to 0x0C80 */
#define HCI_LE_MIN_CONN_INTRVL_MIN_RANGE                 0x0006U
#define HCI_LE_MIN_CONN_INTRVL_MAX_RANGE                 0x0C80U

/** Connection Interval Maximum - 0x0006 to 0x0C80 */
#define HCI_LE_MAX_CONN_INTRVL_MIN_RANGE                 0x0006U
#define HCI_LE_MAX_CONN_INTRVL_MAX_RANGE                 0x0C80U

/** Connection Latency - 0x0000 to 0x01F4 */
#define HCI_LE_CONN_LATENCY_MIN_RANGE                    0x0000U
#define HCI_LE_CONN_LATENCY_MAX_RANGE                    0x01F4U

/** Supervision Timeout - 0x000A to 0x0C80 */
#define HCI_LE_SUPERVISION_TO_MIN_RANGE                  0x000AU
#define HCI_LE_SUPERVISION_TO_MAX_RANGE                  0x0C80U

/**
 *  HCI Packet Types - exchanged between
 *  the HCI & HCI-Transport Layers.
 */
#define HCI_COMMAND_PACKET                               0x01U
#define HCI_ACL_DATA_PACKET                              0x02U
#define HCI_SCO_DATA_PACKET                              0x03U
#define HCI_EVENT_PACKET                                 0x04U
#define HCI_ISO_DATA_PACKET                              0x05U

#ifdef HCI_LE_SETUP_ISO_DATA_PATH_SUPPORT
/**
 * ISO Data Path Direction.
 */

/* Input: Host to Controller */
#define HCI_ISO_INPUT                                    0x00U
/* Output: Controller to Host */
#define HCI_ISO_OUTPUT                                   0x01U

/**
 * HCI Data Path ID.
 */

/* HCI Data Path ID: HCI */
#define HCI_DATA_PATH_ID_HCI                             0x01U
#endif /* HCI_LE_SETUP_ISO_DATA_PATH_SUPPORT */

#ifdef HCI_LE_REMOVE_ISO_DATA_PATH_SUPPORT
/**
 * ISO Data Path Direction.
 */

 /* Input: Host to Controller */
#define HCI_ISO_REMOVE_INPUT                             (0x01U << 0U)
/* Output: Controller to Host */
#define HCI_ISO_REMOVE_OUTPUT                            (0x01U << 1U)
#endif /* HCI_LE_REMOVE_ISO_DATA_PATH_SUPPORT */

/* Packing: Sequential */
#define HCI_PACKING_SEQUENTIAL                           0x00U
/* Packing: Interleaved */
#define HCI_PACKING_INTERLEAVED                          0x01U

/* Framing: Unframed */
#define HCI_FRAMING_UNFRAMED                             0x00U
/* Framing: Framed */
#define HCI_FRAMING_FRAMED                               0x01U

/**
 * \defgroup hci_command_opcodes Command Opcodes
 * \{
 */

/* +++ HCI Command OpCodes ++++++++++++++++++++++++++++++++++++++++ */
/*
 * HCI Commands defined for BLE Single Mode, with corresponding feature flags
 *
 * Note:
 * ## Commands primarily required for Master(M) and Slave(S) are marked
 * ## Remaining commands are applicable for both roles
 *
 * -- Reset Command
 *    <HCI_RESET_SUPPORT>
 *
 * -- Read Local Version Information Command
 *    <HCI_READ_LOCAL_VERSION_INFORMATION_SUPPORT>
 *
 * -- Read Local Supported Features Command
 *    <HCI_READ_LOCAL_SUPPORTED_FEATURES_SUPPORT>
 *
 * -- Read BD_ADDR Command
 *    <HCI_READ_BD_ADDR_SUPPORT>
 *
 * -- Set Event Mask Command
 *    <HCI_SET_EVENT_MASK_SUPPORT>
 *
 * -- Disconnect Command
 *    <HCI_DISCONNECT_SUPPORT>
 *
 * -- Read Remote Version Information Command
 *    <HCI_READ_REMOTE_VERSION_INFORMATION_SUPPORT>
 *
 * -- Read Transmit Power Level Command
 *    <HCI_READ_TRANSMIT_POWER_LEVEL_SUPPORT>
 *
 * -- Read RSSI Command
 *    <HCI_READ_RSSI_SUPPORT>
 *
 * -- Read Buffer Size Command
 *    <HCI_READ_BUFFER_SIZE_SUPPORT>
 *
 * -- Host Buffer Size Command
 *    <HCI_HOST_BUFFER_SIZE_SUPPORT>
 *
 * -- Set Controller To Host Flow Control Command
 *    <HCI_SET_HOST_CONTROLLER_TO_HOST_FLOW_CONTROL_SUPPORT>
 *
 * -- Host Number Of Completed Packets Command
 * -- Read Local Supported Commands Command
 *    <HCI_READ_LOCAL_SUPPORTED_COMMANDS_SUPPORT>
 *
 * -- LE Read Buffer Size Command
 *    <HCI_LE_READ_BUFFER_SIZE_SUPPORT>
 *
 * -- LE Read Local Supported Features Command
 *    <HCI_LE_READ_LOCAL_SUPPORTED_FEATURES_SUPPORT>
 *
 * -- LE Read Supported States Command
 *    <HCI_LE_READ_SUPPORTED_STATES_SUPPORT>
 *
 * -- LE Add Device To White List Command
 *    <HCI_LE_ADD_DEVICE_TO_WHITE_LIST_SUPPORT>
 *
 * -- LE Clear White List Command
 *    <HCI_LE_CLEAR_WHITE_LIST_SUPPORT>
 *
 * -- LE Read White List Size Command
 *    <HCI_LE_READ_WHITE_LIST_SIZE_SUPPORT>
 *
 * -- LE Remove Device From White List Command
 *    <HCI_LE_REMOVE_DEVICE_FROM_WHITE_LIST_SUPPORT>
 *
 * -- LE Set Event Mask Command
 *    <HCI_LE_SET_EVENT_MASK_SUPPORT>
 *
 * -- LE Test End Command
 *    <HCI_LE_TEST_END_SUPPORT>
 *
 * -- LE Set Advertise Enable Command (S)
 *    <HCI_LE_SET_ADVERTISING_ENABLE_SUPPORT>
 *
 * -- LE Set Advertising Data Command (S)
 *    <HCI_LE_SET_ADVERTISING_DATA_SUPPORT>
 *
 * -- LE Set Advertising Parameters Command (S)
 *    <HCI_LE_SET_ADVERTISING_PARAMETERS_SUPPORT>
 *
 * -- LE Set Random Address Command
 *    <HCI_LE_SET_RANDOM_ADDRESS_SUPPORT>
 *
 * -- LE Read Advertising Channel TX Power Command
 *    <HCI_LE_READ_ADVERTISING_CHANNEL_TX_POWER_SUPPORT>
 *
 * -- LE Transmitter Test Command
 *    <HCI_LE_TRANSMITTER_TEST_COMMAND_SUPPORT>
 *
 * -- LE Set Scan Enable Command (M)
 *    <HCI_LE_SET_SCAN_ENABLE_SUPPORT>
 *
 * -- LE Set Scan Parameters Command (M)
 *    <HCI_LE_SET_SCAN_PARAMETERS_SUPPORT>
 *
 * -- LE Receiver Test Command
 *    <HCI_LE_RECEIVER_TEST_COMMAND_SUPPORT>
 *
 * -- LE Set Scan Response Data Command (S)
 *    <HCI_LE_SET_SCAN_RESPONSE_DATA_SUPPORT>
 *
 * -- LE Create Connection Cancel Command (M)
 *    <HCI_LE_CREATE_CONNECTION_CANCEL_SUPPORT>
 *
 * -- LE Create Connection Command (M)
 *    <HCI_LE_CREATE_CONNECTION_SUPPORT>
 *
 * -- LE Read Remote Used Features Command
 *    <HCI_LE_READ_REMOTE_USED_FEATURES_SUPPORT>
 *
 * -- LE Connection Update Command (M)
 *    <HCI_LE_CONNECTION_UPDATE_SUPPORT>
 *
 * -- LE Set Host Channel Classification Command (M)
 *    <HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION_SUPPORT>
 *
 * -- LE Read Channel Map Command
 *    <HCI_LE_READ_CHANNEL_MAP_SUPPORT>
 *
 * -- LE Encrypt Command
 *    <HCI_LE_ENCRYPT_SUPPORT>
 *
 * -- LE Long Term Key Requested Reply Command
 *    <HCI_LE_LONG_TERM_KEY_REQUESTED_REPLY_SUPPORT>
 *
 * -- LE Long Term Key Requested Negative Reply Command
 *    <HCI_LE_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY_SUPPORT>
 *
 * -- LE Rand Command
 *    <HCI_LE_RAND_SUPPORT>
 *
 * -- LE Start Encryption Command (M)
 *    <HCI_LE_START_ENCRYPTION_SUPPORT>
 */

/* No Operation Opcode */
#define HCI_NO_OPERATION_OPCODE                                 0x0000U

/* Link Control Commands - OGF : 0x01 */
#define HCI_INQUIRY_OPCODE                                      0x0401U
#define HCI_INQUIRY_CANCEL_OPCODE                               0x0402U
#define HCI_PERIODIC_INQUIRY_MODE_OPCODE                        0x0403U
#define HCI_EXIT_PERIODIC_INQUIRY_MODE_OPCODE                   0x0404U
#define HCI_CREATE_CONNECTION_OPCODE                            0x0405U
#define HCI_DISCONNECT_OPCODE                                   0x0406U
#define HCI_ADD_SCO_CONNECTION_OPCODE                           0x0407U
#define HCI_ACCEPT_CONNECTION_REQUEST_OPCODE                    0x0409U
#define HCI_REJECT_CONNECTION_REQUEST_OPCODE                    0x040AU
#define HCI_LINK_KEY_REQUEST_REPLY_OPCODE                       0x040BU
#define HCI_LINK_KEY_REQUEST_NEGATIVE_REPLY_OPCODE              0x040CU
#define HCI_PIN_CODE_REQUEST_REPLY_OPCODE                       0x040DU
#define HCI_PIN_CODE_REQUEST_NEGATIVE_REPLY_OPCODE              0x040EU
#define HCI_CHANGE_CONNECTION_PACKET_TYPE_OPCODE                0x040FU
#define HCI_AUTHENTICATION_REQUESTED_OPCODE                     0x0411U
#define HCI_SET_CONNECTION_ENCRYPTION_OPCODE                    0x0413U
#define HCI_CHANGE_CONNECTION_LINK_KEY_OPCODE                   0x0415U
#define HCI_MASTER_LINK_KEY_OPCODE                              0x0417U
#define HCI_REMOTE_NAME_REQUEST_OPCODE                          0x0419U
#define HCI_READ_REMOTE_SUPPORTED_FEATURES_OPCODE               0x041BU
#define HCI_READ_REMOTE_VERSION_INFORMATION_OPCODE              0x041DU
#define HCI_READ_CLOCK_OFFSET_OPCODE                            0x041FU

#ifdef BT_HCI_1_2
#define HCI_CREATE_CONNECTION_CANCEL_OPCODE                     0x0408U
#define HCI_REMOTE_NAME_REQUEST_CANCEL_OPCODE                   0x041AU
#define HCI_READ_REMOTE_EXTENDED_FEATURES_OPCODE                0x041CU
#define HCI_READ_LMP_HANDLE_OPCODE                              0x0420U
#define HCI_SETUP_SYNCHRONOUS_CONNECTION_OPCODE                 0x0428U
#define HCI_ACCEPT_SYNCHRONOUS_CONNECTION_REQUEST_OPCODE        0x0429U
#define HCI_REJECT_SYNCHRONOUS_CONNECTION_REQUEST_OPCODE        0x042AU
#endif /* BT_HCI_1_2 */

#ifdef BT_SSP
#define HCI_IO_CAP_REQUEST_REPLY_OPCODE                         0x042BU
#define HCI_IO_CAPABILITY_REQUEST_REPLY_OPCODE                  0x042BU
#define HCI_IO_CAP_REQUEST_NEGATIVE_REPLY_OPCODE                0x0434U
#define HCI_IO_CAPABILITY_REQUEST_NEGATIVE_REPLY_OPCODE         0x0434U
#endif /* BT_SSP */

#ifdef BT_SSP_UC
#define HCI_USER_CONF_REQUEST_REPLY_OPCODE                      0x042CU
#define HCI_USER_CONFIRMATION_REQUEST_REPLY_OPCODE              0x042CU
#define HCI_USER_CONF_REQUEST_NEGATIVE_REPLY_OPCODE             0x042DU
#define HCI_USER_CONFIRMATION_REQUEST_NEGATIVE_REPLY_OPCODE     0x042DU
#endif /* BT_SSP_UC */

#ifdef BT_SSP_PE
#define HCI_USER_PASSKEY_REQUEST_REPLY_OPCODE                   0x042EU
#define HCI_USER_PASSKEY_REQUEST_NEGATIVE_REPLY_OPCODE          0x042FU
#endif /* BT_SSP_PE */

#ifdef BT_SSP_OOB
#define HCI_REMOTE_OOB_DATA_REQUEST_REPLY_OPCODE                0x0430U
#define HCI_REMOTE_OOB_DATA_REQUEST_NEGATIVE_REPLY_OPCODE       0x0433U
#endif /* BT_SSP_OOB */

#ifdef HCI_ENH_SCO
#define HCI_SETUP_ENHANCED_SYNCHRONOUS_CONNECTION_OPCODE        0x043DU
#define HCI_ACCEPT_ENHANCED_SYNCHRONOUS_CONNECTION_OPCODE       0x043EU
#endif /* HCI_ENH_SCO */

/* Link Policy Commands - OGF : 0x02 */
#define HCI_HOLD_MODE_OPCODE                                    0x0801U
#define HCI_SNIFF_MODE_OPCODE                                   0x0803U
#define HCI_EXIT_SNIFF_MODE_OPCODE                              0x0804U
#define HCI_PARK_MODE_OPCODE                                    0x0805U
#define HCI_EXIT_PARK_MODE_OPCODE                               0x0806U
#define HCI_QOS_SETUP_OPCODE                                    0x0807U
#define HCI_ROLE_DISCOVERY_OPCODE                               0x0809U
#define HCI_SWITCH_ROLE_OPCODE                                  0x080BU
#define HCI_READ_LINK_POLICY_SETTINGS_OPCODE                    0x080CU
#define HCI_WRITE_LINK_POLICY_SETTINGS_OPCODE                   0x080DU

#ifdef BT_HCI_1_2
#define HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_OPCODE            0x080EU
#define HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_OPCODE           0x080FU
#define HCI_FLOW_SPECIFICATION_OPCODE                           0x0810U
#endif /* BT_HCI_1_2 */

#ifdef BT_SSR
#define HCI_SNIFF_SUBRATING_OPCODE                              0x0811U
#endif /* BT_SSR */

/* Host Controller & BaseBand Commands - OGF : 0x03 */
#define HCI_SET_EVENT_MASK_OPCODE                               0x0C01U
#define HCI_RESET_OPCODE                                        0x0C03U
#define HCI_SET_EVENT_FILTER_OPCODE                             0x0C05U
#define HCI_FLUSH_OPCODE                                        0x0C08U
#define HCI_READ_PIN_TYPE_OPCODE                                0x0C09U
#define HCI_WRITE_PIN_TYPE_OPCODE                               0x0C0AU
#define HCI_CREATE_NEW_UNIT_KEY_OPCODE                          0x0C0BU
#define HCI_READ_STORED_LINK_KEY_OPCODE                         0x0C0DU
#define HCI_WRITE_STORED_LINK_KEY_OPCODE                        0x0C11U
#define HCI_DELETE_STORED_LINK_KEY_OPCODE                       0x0C12U
#define HCI_CHANGE_LOCAL_NAME_OPCODE                            0x0C13U
#define HCI_READ_LOCAL_NAME_OPCODE                              0x0C14U
#define HCI_READ_CONNECTION_ACCEPT_TIMEOUT_OPCODE               0x0C15U
#define HCI_WRITE_CONNECTION_ACCEPT_TIMEOUT_OPCODE              0x0C16U
#define HCI_READ_PAGE_TIMEOUT_OPCODE                            0x0C17U
#define HCI_WRITE_PAGE_TIMEOUT_OPCODE                           0x0C18U
#define HCI_READ_SCAN_ENABLE_OPCODE                             0x0C19U
#define HCI_WRITE_SCAN_ENABLE_OPCODE                            0x0C1AU
#define HCI_READ_PAGE_SCAN_ACTIVITY_OPCODE                      0x0C1BU
#define HCI_WRITE_PAGE_SCAN_ACTIVITY_OPCODE                     0x0C1CU
#define HCI_READ_INQUIRY_SCAN_ACTIVITY_OPCODE                   0x0C1DU
#define HCI_WRITE_INQUIRY_SCAN_ACTIVITY_OPCODE                  0x0C1EU
#define HCI_READ_AUTHENTICATION_ENABLE_OPCODE                   0x0C1FU
#define HCI_WRITE_AUTHENTICATION_ENABLE_OPCODE                  0x0C20U
#define HCI_READ_ENCRYPTION_MODE_OPCODE                         0x0C21U
#define HCI_WRITE_ENCRYPTION_MODE_OPCODE                        0x0C22U
#define HCI_READ_CLASS_OF_DEVICE_OPCODE                         0x0C23U
#define HCI_WRITE_CLASS_OF_DEVICE_OPCODE                        0x0C24U
#define HCI_READ_VOICE_SETTING_OPCODE                           0x0C25U
#define HCI_WRITE_VOICE_SETTING_OPCODE                          0x0C26U
#define HCI_READ_AUTOMATIC_FLUSH_TIMEOUT_OPCODE                 0x0C27U
#define HCI_WRITE_AUTOMATIC_FLUSH_TIMEOUT_OPCODE                0x0C28U
#define HCI_READ_NUM_BROADCAST_RETRANSMISSIONS_OPCODE           0x0C29U
#define HCI_WRITE_NUM_BROADCAST_RETRANSMISSIONS_OPCODE          0x0C2AU
#define HCI_READ_HOLD_MODE_ACTIVITY_OPCODE                      0x0C2BU
#define HCI_WRITE_HOLD_MODE_ACTIVITY_OPCODE                     0x0C2CU
#define HCI_READ_TRANSMIT_POWER_LEVEL_OPCODE                    0x0C2DU
#define HCI_READ_SCO_FLOW_CONTROL_ENABLE_OPCODE                 0x0C2EU
#define HCI_READ_SYNCHRONOUS_FLOW_CONTROL_ENABLE_OPCODE         0x0C2EU
#define HCI_WRITE_SCO_FLOW_CONTROL_ENABLE_OPCODE                0x0C2FU
#define HCI_SET_HOST_CONTROLLER_TO_HOST_FLOW_CONTROL_OPCODE     0x0C31U
#define HCI_HOST_BUFFER_SIZE_OPCODE                             0x0C33U
#define HCI_HOST_NUMBER_OF_COMPLETED_PACKETS_OPCODE             0x0C35U
#define HCI_READ_LINK_SUPERVISION_TIMEOUT_OPCODE                0x0C36U
#define HCI_WRITE_LINK_SUPERVISION_TIMEOUT_OPCODE               0x0C37U
#define HCI_READ_NUMBER_OF_SUPPORTED_IAC_OPCODE                 0x0C38U
#define HCI_READ_CURRENT_IAC_LAP_OPCODE                         0x0C39U
#define HCI_WRITE_CURRENT_IAC_LAP_OPCODE                        0x0C3AU
#define HCI_READ_PAGE_SCAN_PERIOD_MODE_OPCODE                   0x0C3BU
#define HCI_WRITE_PAGE_SCAN_PERIOD_MODE_OPCODE                  0x0C3CU
#define HCI_READ_PAGE_SCAN_MODE_OPCODE                          0x0C3DU
#define HCI_WRITE_PAGE_SCAN_MODE_OPCODE                         0x0C3EU

#ifdef BT_HCI_1_2
#define HCI_SET_AFH_HOST_CHANNEL_CLASSIFICATION_OPCODE          0x0C3FU
#define HCI_READ_INQUIRY_SCAN_TYPE_OPCODE                       0x0C42U
#define HCI_WRITE_INQUIRY_SCAN_TYPE_OPCODE                      0x0C43U
#define HCI_READ_INQUIRY_MODE_OPCODE                            0x0C44U
#define HCI_WRITE_INQUIRY_MODE_OPCODE                           0x0C45U
#define HCI_READ_PAGE_SCAN_TYPE_OPCODE                          0x0C46U
#define HCI_WRITE_PAGE_SCAN_TYPE_OPCODE                         0x0C47U
#define HCI_READ_AFH_CHANNEL_ASSESSMENT_MODE_OPCODE             0x0C48U
#define HCI_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_OPCODE            0x0C49U
#endif /* BT_HCI_1_2 */

#ifdef BT_EIR
#define HCI_READ_EXTENDED_INQUIRY_RESPONSE_OPCODE               0x0C51U
#define HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_OPCODE              0x0C52U
#define HCI_READ_INQUIRY_RESP_TX_POWER_LEVEL_OPCODE             0x0C58U
#define HCI_READ_INQUIRY_RESPONSE_TRANSMIT_POWER_LEVEL_OPCODE   0x0C58U
#define HCI_WRITE_INQUIRY_RESP_TX_POWER_LEVEL_OPCODE            0x0C59U
#endif /* BT_EIR */

#ifdef BT_EPR
#define HCI_REFRESH_ENCRYPTION_KEY_OPCODE                       0x0C53U
#endif /* BT_EPR */

#ifdef BT_EDR
#define HCI_READ_DEFAULT_ERR_DATA_REPORTING_OPCODE              0x0C5AU
#define HCI_WRITE_DEFAULT_ERR_DATA_REPORTING_OPCODE             0x0C5BU
#endif /* BT_EDR */

#ifdef BT_SSP
#define HCI_READ_SIMPLE_PAIRING_MODE_OPCODE                     0x0C55U
#define HCI_WRITE_SIMPLE_PAIRING_MODE_OPCODE                    0x0C56U
#endif /* BT_SSP */

#ifdef BT_SSP_PE
#define HCI_SEND_KEY_PRESS_NOTIFICATION_OPCODE                  0x0C60U
#define HCI_SEND_KEYPRESS_NOTIFICATION_OPCODE                   0x0C60U
#endif /* BT_SSP_PE */

#ifdef BT_SSP_OOB
#define HCI_READ_LOCAL_OOB_DATA_OPCODE                          0x0C57U
#endif /* BT_SSP_OOB */

#ifdef BT_ENHANCED_FLUSH
#define HCI_ENHANCED_FLUSH_OPCODE                               0x0C5FU
#endif /* BT_ENHANCED_FLUSH */

#ifdef BT_3_0

#ifdef HCI_FLOW_CONTROL_COMMANDS
#define HCI_READ_FLOW_CONTROL_MODE_OPCODE                       0x0C66U
#define HCI_WRITE_FLOW_CONTROL_MODE_OPCODE                      0x0C67U
#endif /* HCI_FLOW_CONTROL_COMMANDS */

#define HCI_READ_ENHANCED_TRANSMIT_POWER_LEVEL_OPCODE           0x0C68U
#endif /* BT_3_0 */

#ifdef HCI_READ_LE_HOST_SUPPORT
#define HCI_READ_LE_HOST_SUPPORT_OPCODE                         0x0C6CU
#endif /* HCI_READ_LE_HOST_SUPPORT */

#ifdef HCI_WRITE_LE_HOST_SUPPORT
#define HCI_WRITE_LE_HOST_SUPPORT_OPCODE                        0x0C6DU
#endif /* HCI_WRITE_LE_HOST_SUPPORT */

#ifdef BT_BRSC
#define HCI_READ_SECURE_CONNECTIONS_HOST_SUPPORT_OPCODE         0x0C79U
#define HCI_WRITE_SECURE_CONNECTIONS_HOST_SUPPORT_OPCODE        0x0C7AU
#endif /* BT_BRSC */

#ifdef HCI_SET_EVENT_MASK_PAGE_2_SUPPORT
#define HCI_SET_EVENT_MASK_PAGE_2_OPCODE                        0x0C63U
#endif /* HCI_SET_EVENT_MASK_PAGE_2_SUPPORT */

#ifdef HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT
#define HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_OPCODE           0x0C7BU
#endif /* HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT */

#ifdef HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT
#define HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT_OPCODE          0x0C7CU
#endif /* HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT */

/* Informational Parameters - OGF : 0x04 */
#define HCI_READ_LOCAL_VERSION_INFORMATION_OPCODE               0x1001U
#define HCI_READ_LOCAL_SUPPORTED_COMMANDS_OPCODE                0x1002U
#define HCI_READ_LOCAL_SUPPORTED_FEATURES_OPCODE                0x1003U
#define HCI_READ_BUFFER_SIZE_OPCODE                             0x1005U
#define HCI_READ_COUNTRY_CODE_OPCODE                            0x1007U
#define HCI_READ_BD_ADDR_OPCODE                                 0x1009U

#ifdef BT_HCI_1_2
#define HCI_READ_LOCAL_SUPPORTED_COMMANDS_OPCODE                0x1002U
#define HCI_READ_LOCAL_EXTENDED_FEATURES_OPCODE                 0x1004U
#endif /* BT_HCI_1_2 */

#ifdef BT_3_0
#define HCI_READ_DATA_BLOCK_SIZE_OPCODE                         0x100AU
#endif /* BT_3_0 */

#ifdef HCI_ENH_SCO
#define HCI_READ_LOCAL_SUPPORTED_CODECS_OPCODE                  0x100BU
#endif /* HCI_ENH_SCO */

/* Status Parameters - OGF : 0x05 */
#define HCI_READ_FAILED_CONTACT_COUNTER_OPCODE                  0x1401U
#define HCI_RESET_FAILED_CONTACT_COUNTER_OPCODE                 0x1402U
#define HCI_GET_LINK_QUALITY_OPCODE                             0x1403U
#define HCI_READ_RSSI_OPCODE                                    0x1405U

#ifdef BT_HCI_1_2
#define HCI_READ_AFH_CHANNEL_MAP_OPCODE                         0x1406U
#define HCI_READ_CLOCK_OPCODE                                   0x1407U
#endif /* BT_HCI_1_2 */

#ifdef BT_3_0
#define HCI_READ_ENCRYPTION_KEY_SIZE_OPCODE                     0x1408U
#endif /* BT_3_0 */

/* Testing Commands - OGF : 0x06 */
#ifdef HCI_TESTING_COMMANDS
#define HCI_READ_LOOPBACK_MODE_OPCODE                           0x1801U
#define HCI_WRITE_LOOPBACK_MODE_OPCODE                          0x1802U
#define HCI_ENABLE_DEVICE_UNDER_TEST_MODE_OPCODE                0x1803U
#endif /* HCI_TESTING_COMMANDS */

#ifdef BT_SSP_DEBUG
#define HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE_OPCODE              0x1804U
#endif /* BT_SSP_DEBUG */

#ifdef BT_BRSC_TEST
#define HCI_WRITE_SECURE_CONNECTIONS_TEST_MODE_OPCODE           0x180AU
#endif /* BT_BRSC_TEST */

/* Bluetooth Low Energy Commands */
#ifdef BT_4_0
#define HCI_LE_SET_EVENT_MASK_OPCODE                            0x2001U
#define HCI_LE_READ_BUFFER_SIZE_OPCODE                          0x2002U
#define HCI_LE_READ_LOCAL_SUPPORTED_FEATURES_OPCODE             0x2003U
#define HCI_LE_SET_RANDOM_ADDRESS_OPCODE                        0x2005U
#define HCI_LE_SET_ADVERTISING_PARAMETERS_OPCODE                0x2006U
#define HCI_LE_READ_ADVERTISING_CHANNEL_TX_POWER_OPCODE         0x2007U
#define HCI_LE_READ_ADVERTISING_PHYSICAL_CHANNEL_TX_POWER_OPCODE \
                                                                0x2007U
#define HCI_LE_SET_ADVERTISING_DATA_OPCODE                      0x2008U
#define HCI_LE_SET_SCAN_RESPONSE_DATA_OPCODE                    0x2009U
#define HCI_LE_SET_ADVERTISING_ENABLE_OPCODE                    0x200AU
#define HCI_LE_SET_SCAN_PARAMETERS_OPCODE                       0x200BU
#define HCI_LE_SET_SCAN_ENABLE_OPCODE                           0x200CU
#define HCI_LE_CREATE_CONNECTION_OPCODE                         0x200DU
#define HCI_LE_CREATE_CONNECTION_CANCEL_OPCODE                  0x200EU
#define HCI_LE_READ_WHITE_LIST_SIZE_OPCODE                      0x200FU
#define HCI_LE_CLEAR_WHITE_LIST_OPCODE                          0x2010U
#define HCI_LE_ADD_DEVICE_TO_WHITE_LIST_OPCODE                  0x2011U
#define HCI_LE_REMOVE_DEVICE_FROM_WHITE_LIST_OPCODE             0x2012U
#define HCI_LE_CONNECTION_UPDATE_OPCODE                         0x2013U
#define HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION_OPCODE           0x2014U
#define HCI_LE_READ_CHANNEL_MAP_OPCODE                          0x2015U
#define HCI_LE_READ_REMOTE_USED_FEATURES_OPCODE                 0x2016U
#define HCI_LE_ENCRYPT_OPCODE                                   0x2017U
#define HCI_LE_RAND_OPCODE                                      0x2018U
#define HCI_LE_START_ENCRYPTION_OPCODE                          0x2019U
#define HCI_LE_LONG_TERM_KEY_REQUESTED_REPLY_OPCODE             0x201AU
#define HCI_LE_LONG_TERM_KEY_REQUEST_REPLY_OPCODE               0x201AU
#define HCI_LE_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY_OPCODE    0x201BU
#define HCI_LE_LONG_TERM_KEY_REQUEST_NEGATIVE_REPLY_OPCODE      0x201BU
#define HCI_LE_READ_SUPPORTED_STATES_OPCODE                     0x201CU
#define HCI_LE_RECEIVER_TEST_COMMAND_OPCODE                     0x201DU
#define HCI_LE_TRANSMITTER_TEST_COMMAND_OPCODE                  0x201EU
#define HCI_LE_TEST_END_OPCODE                                  0x201FU
#endif /* BT_4_0 */

#ifdef HCI_LL_TOPOLOGY_CONN_UPDATE_SUPPORT
#define HCI_LE_REMOTE_CONN_PARAM_REQ_RPLY_OPCODE                0x2020U
#define HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST_REPLY_OPCODE 0x2020U
#define HCI_LE_REMOTE_CONN_PARAM_REQ_NEG_RPLY_OPCODE            0x2021U
#define HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST_NEGATIVE_REPLY_OPCODE \
                                                                0x2021U
#endif /* HCI_LL_TOPOLOGY_CONN_UPDATE_SUPPORT */

/* BT 4.1 Specification */
#ifdef BT_4_1
#define HCI_TRUNCATED_PAGE_OPCODE                               0x043FU
#define HCI_TRUNCATED_PAGE_CANCEL_OPCODE                        0x0440U
#define HCI_SET_CONNECTIONLESS_SLAVE_BROADCAST_OPCODE           0x0441U
#define HCI_SET_CONNECTIONLESS_SLAVE_BROADCAST_RECEIVE_OPCODE   0x0442U
#define HCI_START_SYNCHRONIZATION_TRAIN_OPCODE                  0x0443U
#define HCI_RECEIVE_SYNCHRONIZATION_TRAIN_OPCODE                0x0444U
#define HCI_REMOTE_OOB_EXTENDED_DATA_REQUEST_REPLY_OPCODE       0x0445U
#define HCI_SET_MWS_CHANNEL_PARAMETERS_OPCODE                   0x0C6EU
#define HCI_SET_EXTERNAL_FRAME_CONFIGURATION_OPCODE             0x0C6FU
#define HCI_SET_MWS_SIGNALING_OPCODE                            0x0C70U
#define HCI_SET_MWS_TRANSPORT_LAYER_OPCODE                      0x0C71U
#define HCI_SET_MWS_SCAN_FREQUENCY_TABLE_OPCODE                 0x0C72U
#define HCI_SET_MWS_PATTERN_CONFIGURATION_OPCODE                0x0C73U
#define HCI_SET_RESERVED_LT_ADDR_OPCODE                         0x0C74U
#define HCI_DELETE_RESERVED_LT_ADDR_OPCODE                      0x0C75U
#define HCI_SET_CONNECTIONLESS_SLAVE_BROADCAST_DATA_OPCODE      0x0C76U
#define HCI_READ_SYNCHRONIZATION_TRAIN_PARAMETERS_OPCODE        0x0C77U
#define HCI_WRITE_SYNCHRONIZATION_TRAIN_PARAMETERS_OPCODE       0x0C78U
#define HCI_READ_LOCAL_OOB_EXTENDED_DATA_OPCODE                 0x0C7DU
#define HCI_READ_EXTENDED_PAGE_TIMEOUT_OPCODE                   0x0C7EU
#define HCI_WRITE_EXTENDED_PAGE_TIMEOUT_OPCODE                  0x0C7FU
#define HCI_READ_EXTENDED_INQUIRY_LENGTH_OPCODE                 0x0C80U
#define HCI_WRITE_EXTENDED_INQUIRY_LENGTH_OPCODE                0x0C81U
#define HCI_GET_MWS_TRANSPORT_LAYER_CONFIGURATION_OPCODE        0x140CU
#define HCI_SET_TRIGGERED_CLOCK_CAPTURE_OPCODE                  0x140DU
#endif /* BT_4_1 */

/* BT 4.2 Specification */
#ifdef BT_4_2
#define HCI_LE_SET_DATA_LENGTH_OPCODE                           0x2022U
#define HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LEN_OPCODE           0x2023U
#define HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LENGTH_OPCODE        0x2023U
#define HCI_LE_WRITE_SUGGESTED_DEFAULT_DATA_LEN_OPCODE          0x2024U
#define HCI_LE_READ_LOCAL_P_256_PUBLIC_KEY_OPCODE               0x2025U
#define HCI_LE_GENERATE_DHKEY_OPCODE                            0x2026U
#define HCI_LE_ADD_DEV_TO_RESOLVING_LIST_OPCODE                 0x2027U
#define HCI_LE_REMOVE_DEV_FROM_RESOLVING_LIST_OPCODE            0x2028U
#define HCI_LE_CLR_RESOLVING_LIST_OPCODE                        0x2029U
#define HCI_LE_READ_RESOLVING_LIST_SIZE_OPCODE                  0x202AU
#define HCI_LE_READ_PEER_RESOLVABLE_ADDR_OPCODE                 0x202BU
#define HCI_LE_READ_PEER_RESOLVABLE_ADDRESS_OPCODE              0x202BU
#define HCI_LE_READ_LOCAL_RESOLVABLE_ADDR_OPCODE                0x202CU
#define HCI_LE_READ_LOCAL_RESOLVABLE_ADDRESS_OPCODE             0x202CU
#define HCI_LE_SET_ADDR_RESOLUTION_ENABLE_OPCODE                0x202DU
#define HCI_LE_SET_RESOLVABLE_PRIVATE_ADDR_TIMEOUT_OPCODE       0x202EU
#define HCI_LE_READ_MAXIMUM_DATA_LENGTH_OPCODE                  0x202FU
#endif /* BT_4_2 */

#ifdef BT_5_0
/* BT 5.0 Specification */
#define HCI_LE_READ_PHY_OPCODE                                  0x2030U
#define HCI_LE_SET_DEFAULT_PHY_OPCODE                           0x2031U
#define HCI_LE_SET_PHY_OPCODE                                   0x2032U
#define HCI_LE_ENHANCED_RECEIVER_TEST_OPCODE                    0x2033U
#define HCI_LE_ENHANCED_TRANSMITTER_TEST_OPCODE                 0x2034U

/* HCI LE Advertising Extension support related defines */
#define HCI_LE_SET_ADVERTISING_SET_RANDOM_ADDRESS_OPCODE        0x2035U
#define HCI_LE_SET_EXTENDED_ADV_PARAMS_OPCODE                   0x2036U
#define HCI_LE_SET_EXTENDED_ADVERTISING_PARAMETERS_OPCODE       0x2036U
#define HCI_LE_SET_EXTENDED_ADVERTISING_DATA_OPCODE             0x2037U
#define HCI_LE_SET_EXTENDED_SCAN_RESPONSE_DATA_OPCODE           0x2038U
#define HCI_LE_SET_EXTENDED_ADVERTISE_ENABLE_OPCODE             0x2039U
#define HCI_LE_READ_MAX_ADV_DATA_LENGTH_OPCODE                  0x203AU
#define HCI_LE_READ_MAXIMUM_ADVERTISING_DATA_LENGTH_OPCODE      0x203AU
#define HCI_LE_READ_NUM_SUPPORTED_ADV_SETS_OPCODE               0x203BU
#define HCI_LE_READ_NUMBER_OF_SUPPORTED_ADVERTISING_SETS_OPCODE 0x203BU
#define HCI_LE_REMOVE_ADVERTISING_SET_OPCODE                    0x203CU
#define HCI_LE_CLEAR_ADVERTISING_SETS_OPCODE                    0x203DU
#define HCI_LE_SET_PERIODIC_ADV_PARAMS_OPCODE                   0x203EU
#define HCI_LE_SET_PERIODIC_ADVERTISING_DATA_OPCODE             0x203FU
#define HCI_LE_SET_PERIODIC_ADVERTISING_ENABLE_OPCODE           0x2040U
#define HCI_LE_SET_EXTENDED_SCAN_PARAMETERS_OPCODE              0x2041U
#define HCI_LE_SET_EXTENDED_SCAN_ENABLE_OPCODE                  0x2042U
#define HCI_LE_EXTENDED_CREATE_CONNECTION_OPCODE                0x2043U
#define HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_OPCODE          0x2044U
#define HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_CANCEL_OPCODE   0x2045U
#define HCI_LE_PERIODIC_ADVERTISING_TERMINATE_SYNC_OPCODE       0x2046U
#define HCI_LE_ADD_DEV_TO_PERIODIC_ADVSER_LIST_OPCODE           0x2047U
#define HCI_LE_RM_DEV_FRM_PERIODIC_ADVSER_LIST_OPCODE           0x2048U
#define HCI_LE_CLEAR_PERIODIC_ADVERTISER_LIST_OPCODE            0x2049U
#define HCI_LE_READ_PERIODIC_ADVERTISER_LIST_SIZE_OPCODE        0x204AU

#define HCI_LE_READ_TRANSMIT_POWER_OPCODE                       0x204BU
#define HCI_LE_READ_RF_PATH_COMPENSATION_OPCODE                 0x204CU
#define HCI_LE_WRITE_RF_PATH_COMPENSATION_OPCODE                0x204DU
#define HCI_LE_SET_PRIVACY_MODE_OPCODE                          0x204EU
#endif /* BT_5_0 */

#ifdef BT_5_1
#define HCI_READ_LOCAL_SIMPLE_PAIRING_OPTIONS_OPCODE                    0x100CU
/* HCI LE Connection Oriented AOA and Connectionless AOD related defines */
#define HCI_LE_RECEIVER_TEST_V3_OPCODE                                  0x204FU
#define HCI_LE_TRANSMITTER_TEST_V3_OPCODE                               0x2050U
#define HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_PARAMETERS_OPCODE        0x2051U
#define HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_ENABLE_OPCODE            0x2052U
#define HCI_LE_SET_CONNECTIONLESS_IQ_SAMPLING_ENABLE_OPCODE             0x2053U
#define HCI_LE_SET_CONNECTION_CTE_RECEIVE_PARAMETERS_OPCODE             0x2054U
#define HCI_LE_SET_CONNECTION_CTE_TRANSMIT_PARAMETERS_OPCODE            0x2055U
#define HCI_LE_SET_CONNECTION_CTE_REQUEST_ENABLE_OPCODE                 0x2056U
#define HCI_LE_CONNECTION_CTE_REQUEST_ENABLE_OPCODE                     0x2056U
#define HCI_LE_SET_CONNECTION_CTE_RESPONSE_ENABLE_OPCODE                0x2057U
#define HCI_LE_CONNECTION_CTE_RESPONSE_ENABLE_OPCODE                    0x2057U
#define HCI_LE_READ_ANTENNA_INFORMATION_OPCODE                          0x2058U

/* HCI LE Periodic Advertising Sync Transfer related defines */
#define HCI_LE_SET_PERIODIC_ADVERTISING_RECEIVE_ENABLE_OPCODE           0x2059U
#define HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_OPCODE                0x205AU
#define HCI_LE_PERIODIC_ADVERTISING_SET_INFO_TRANSFER_OPCODE            0x205BU
#define HCI_LE_SET_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS_OPCODE 0x205CU
#define HCI_LE_SET_DEFAULT_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS_OPCODE 0x205DU
#define HCI_LE_GENERATE_DHKEY_V2_OPCODE                                 0x205EU
#define HCI_LE_MODIFY_SLEEP_CLOCK_ACCURACY_OPCODE                       0x205FU
#endif /* BT_5_1 */

#ifdef BT_5_2
#define HCI_SET_ECOSYSTEM_BASE_INTERVAL_OPCODE                          0x0C82U
#define HCI_CONFIGURE_DATA_PATH_OPCODE                                  0x0C83U
#define HCI_READ_LOCAL_SUPPORTED_CODECS_V2_OPCODE                       0x100DU
#define HCI_READ_LOCAL_SUPPORTED_CODEC_CAPABILITIES_OPCODE              0x100EU
#define HCI_READ_LOCAL_SUPPORTED_CONTROLLER_DELAY_OPCODE                0x100FU
#define HCI_LE_READ_BUFFER_SIZE_V2_OPCODE                               0x2060U
#define HCI_LE_READ_ISO_TX_SYNC_OPCODE                                  0x2061U
#define HCI_LE_SET_CIG_PARAMETERS_OPCODE                                0x2062U
#define HCI_LE_SET_CIG_PARAMETERS_TEST_OPCODE                           0x2063U
#define HCI_LE_CREATE_CIS_OPCODE                                        0x2064U
#define HCI_LE_REMOVE_CIG_OPCODE                                        0x2065U
#define HCI_LE_ACCEPT_CIS_REQUEST_OPCODE                                0x2066U
#define HCI_LE_REJECT_CIS_REQUEST_OPCODE                                0x2067U
#define HCI_LE_CREATE_BIG_OPCODE                                        0x2068U
#define HCI_LE_CREATE_BIG_TEST_OPCODE                                   0x2069U
#define HCI_LE_TERMINATE_BIG_OPCODE                                     0x206AU
#define HCI_LE_BIG_CREATE_SYNC_OPCODE                                   0x206BU
#define HCI_LE_BIG_TERMINATE_SYNC_OPCODE                                0x206CU
#define HCI_LE_REQUEST_PEER_SCA_OPCODE                                  0x206DU
#define HCI_LE_SETUP_ISO_DATA_PATH_OPCODE                               0x206EU
#define HCI_LE_REMOVE_ISO_DATA_PATH_OPCODE                              0x206FU
#define HCI_LE_ISO_TRANSMIT_TEST_OPCODE                                 0x2070U
#define HCI_LE_ISO_RECEIVE_TEST_OPCODE                                  0x2071U
#define HCI_LE_ISO_READ_TEST_COUNTERS_OPCODE                            0x2072U
#define HCI_LE_ISO_TEST_END_OPCODE                                      0x2073U
#define HCI_LE_SET_HOST_FEATURE_OPCODE                                  0x2074U
#define HCI_LE_READ_ISO_LINK_QUALITY_OPCODE                             0x2075U
#define HCI_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL_OPCODE                0x2076U
#define HCI_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL_OPCODE                  0x2077U
#define HCI_LE_SET_PATH_LOSS_REPORTING_PARAMETERS_OPCODE                0x2078U
#define HCI_LE_SET_PATH_LOSS_REPORTING_ENABLE_OPCODE                    0x2079U
#define HCI_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE_OPCODE               0x207AU
#endif /* BT_5_2 */

/* Vendor Specific Parameters - OGF : 0x3F */
/* TODO: Move to Vendor specific File */
#define HCI_VENDOR_READ_VERSION_INFO_OPCODE                             0xFC0F
/** \} */

/** \} */

/** \} */

/**
 * \defgroup hci_events Events
 * \{
 */

/* +++ HCI Event Codes ++++++++++++++++++++++++++++++++++++++++++++ */
#define HCI_INQUIRY_COMPLETE_EVENT                              0x01U
#define HCI_INQUIRY_RESULT_EVENT                                0x02U
#define HCI_CONNECTION_COMPLETE_EVENT                           0x03U
#define HCI_CONNECTION_REQUEST_EVENT                            0x04U
#define HCI_DISCONNECTION_COMPLETE_EVENT                        0x05U
#define HCI_AUTHENTICATION_COMPLETE_EVENT                       0x06U
#define HCI_REMOTE_NAME_REQUEST_COMPLETE_EVENT                  0x07U
#define HCI_ENCRYPTION_CHANGE_EVENT                             0x08U
#define HCI_CHANGE_CONNECTION_LINK_KEY_COMPLETE_EVENT           0x09U
#define HCI_MASTER_LINK_KEY_COMPLETE_EVENT                      0x0AU
#define HCI_READ_REMOTE_SUPPORTED_FEATURES_COMPLETE_EVENT       0x0BU
#define HCI_READ_REMOTE_VERSION_INFORMATION_COMPLETE_EVENT      0x0CU
#define HCI_QOS_SETUP_COMPLETE_EVENT                            0x0DU
#define HCI_COMMAND_COMPLETE_EVENT                              0x0EU
#define HCI_COMMAND_STATUS_EVENT                                0x0FU
#define HCI_HARDWARE_ERROR_EVENT                                0x10U
#define HCI_FLUSH_OCCURRED_EVENT                                0x11U
#define HCI_ROLE_CHANGE_EVENT                                   0x12U
#define HCI_NUMBER_OF_COMPLETED_PACKETS_EVENT                   0x13U
#define HCI_MODE_CHANGE_EVENT                                   0x14U
#define HCI_RETURN_LINK_KEYS_EVENT                              0x15U
#define HCI_PIN_CODE_REQUEST_EVENT                              0x16U
#define HCI_LINK_KEY_REQUEST_EVENT                              0x17U
#define HCI_LINK_KEY_NOTIFICATION_EVENT                         0x18U
#define HCI_LOOPBACK_COMMAND_EVENT                              0x19U
#define HCI_DATA_BUFFER_OVERFLOW_EVENT                          0x1AU
#define HCI_MAX_SLOTS_CHANGE_EVENT                              0x1BU
#define HCI_READ_CLOCK_OFFSET_COMPLETE_EVENT                    0x1CU
#define HCI_CONNECTION_PACKET_TYPE_CHANGED_EVENT                0x1DU
#define HCI_QOS_VIOLATION_EVENT                                 0x1EU
/* Event 0x1F removed from specification */
/* #define HCI_PAGE_SCAN_MODE_CHANGE_EVENT                      0x1F */
#define HCI_PAGE_SCAN_REPETITION_MODE_CHANGE_EVENT              0x20U

#ifdef BT_HCI_1_2
#define HCI_FLOW_SPECIFICATION_COMPLETE_EVENT                   0x21U
#define HCI_INQUIRY_RESULT_WITH_RSSI_EVENT                      0x22U
#define HCI_READ_REMOTE_EXTENDED_FEATURES_COMPLETE_EVENT        0x23U
#define HCI_SYNCHRONOUS_CONNECTION_COMPLETE_EVENT               0x2CU
#define HCI_SYNCHRONOUS_CONNECTION_CHANGED_EVENT                0x2DU
#endif /* BT_HCI_1_2 */

#ifdef BT_2_1_EDR
#define HCI_SNIFF_SUBRATING_EVENT                               0x2EU
#define HCI_EXTENDED_INQUIRY_RESULT_EVENT                       0x2FU
#define HCI_IO_CAPABILITY_REQUEST_EVENT                         0x31U
#define HCI_IO_CAPABILITY_RESPONSE_EVENT                        0x32U
#define HCI_USER_CONFIRMATION_REQUEST_EVENT                     0x33U
#define HCI_USER_PASSKEY_REQUEST_EVENT                          0x34U
#define HCI_REMOTE_OOB_DATA_REQUEST_EVENT                       0x35U
#define HCI_SIMPLE_PAIRING_COMPLETE_EVENT                       0x36U
#define HCI_LINK_SUPERVISION_TIMEOUT_CHANGED_EVENT              0x38U
#define HCI_ENHANCED_FLUSH_COMPLETE_EVENT                       0x39U
#define HCI_USER_PASSKEY_NOTIFICATION_EVENT                     0x3BU
#define HCI_KEY_PRESS_NOTIFICATION_EVENT                        0x3CU
#define HCI_REMOTE_HOST_SUPPORTED_FEATURES_NOTIFICATION_EVENT   0x3DU
#endif /* BT_2_1_EDR */

#if ((defined BT_2_1_EDR) || (defined BT_4_0))
#define HCI_ENCRYPTION_KEY_REFRESH_COMPLETE_EVENT               0x30U
#endif /* ((defined BT_2_1_EDR) || (defined BT_4_0)) */

#define HCI_FLOW_SPEC_MODIFY_COMPLETE_EVENT                     0x47U
#define HCI_NUMBER_OF_COMPLETED_DATA_BLOCKS_EVENT               0x48U

#ifdef BT_4_1
#define HCI_TRIGGERED_CLOCK_CAPTURE_EVENT                       0x4EU
#define HCI_SYNCHRONIZATION_TRAIN_COMPLETE_EVENT                0x4FU
#define HCI_SYNCHRONIZATION_TRAIN_RECEIVED_EVENT                0x50U
#define HCI_CONNECTIONLESS_SLAVE_BROADCAST_RECEIVE_EVENT        0x51U
#define HCI_CONNECTIONLESS_SLAVE_BROADCAST_TIMEOUT_EVENT        0x52U
#define HCI_TRUNCATED_PAGE_COMPLETE_EVENT                       0x53U
#define HCI_SLAVE_PAGE_RESPONSE_TIMEOUT_EVENT                   0x54U
#define HCI_CONNECTIONLESS_SLAVE_BROADCAST_CHANNEL_MAP_CHANGE_EVENT \
                                                                0x55U
#define HCI_INQUIRY_RESPONSE_NOTIFICATION_EVENT                 0x56U
#define HCI_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED_EVENT         0x57U
#endif /* BT_4_1 */

#ifdef BT_5_0
#define HCI_SAM_STATUS_CHANGE_EVENT                             0x58U
#endif /* BT_5_0 */

/* HCI Vendor Specific Debug Event */
#define HCI_VENDOR_SPECIFIC_DEBUG_EVENT                         0xFFU

#ifdef BT_4_0
/**
 *  LE Meta Event is used to encapsulate all LE Controller specific events.
 *  Subevnt code the first octect of the event parameters.
 *  The possible subevnt codes are:
 *   - \ref HCI_LE_CONNECTION_COMPLETE_SUBEVENT
 *   - \ref HCI_LE_ADVERTISING_REPORT_SUBEVENT
 *   - \ref HCI_LE_CONNECTION_UPDATE_COMPLETE_SUBEVENT
 *   - \ref HCI_LE_READ_REMOTE_USED_FEATURES_COMPLETE_SUBEVENT
 *   - \ref HCI_LE_LONG_TERM_KEY_REQUESTED_SUBEVENT
 *   - \ref HCI_LE_REMOTE_CONN_PARAM_REQ_SUBEVENT
 *   - \ref HCI_LE_DATA_LENGTH_CHANGE_SUBEVENT
 *   - \ref HCI_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_SUBEVENT
 *   - \ref HCI_LE_GENERATE_DHKEY_COMPLETE_SUBEVENT
 *   - \ref HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVENT
 *   - \ref HCI_LE_DIRECT_ADVERTISING_REPORT_SUBEVENT
 *   - \ref HCI_LE_PHY_UPDATE_COMPLETE_SUBEVENT
 *   - \ref HCI_LE_EXTENDED_ADVERTISING_REPORT_SUBEVENT
 *   - \ref HCI_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED_SUBEVENT
 *   - \ref HCI_LE_PERIODIC_ADVERTISING_REPORT_SUBEVENT
 *   - \ref HCI_LE_PERIODIC_ADVERTISING_SYNC_LOST_SUBEVENT
 *   - \ref HCI_LE_SCAN_TIMEOUT_SUBEVENT
 *   - \ref HCI_LE_ADVERTISING_SET_TERMINATED_SUBEVENT
 *   - \ref HCI_LE_SCAN_REQUEST_RECEIVED_SUBEVENT
 *   - \ref HCI_LE_CHANNEL_SELECTION_ALGORITHM_SUBEVENT
 */
#define HCI_LE_META_EVENT                                       0x3EU

/* Bluetooth Low Energy Subevents */
/**
 *  The LE Connection Complete subevent indicates to both of the devices
 *  forming the connection that a new connection has been created.
 */
#define HCI_LE_CONNECTION_COMPLETE_SUBEVENT                     0x01U

/**
 *  The LE Advertising Report subevent indicates that a Bluetooth device or
 *  multiple Bluetooth devices have responded to an active scan or received
 *  some information during a passive scan.
 */
#define HCI_LE_ADVERTISING_REPORT_SUBEVENT                      0x02U

/**
 *  The LE Connection Update Complete subevent is used to indicate that the
 *  Controller process to update the connection has completed.
 */
#define HCI_LE_CONNECTION_UPDATE_COMPLETE_SUBEVENT              0x03U

/**
 *  The LE Read Remote Used Features Complete subevent is used to indicate the
 *  completion of the process of the Controller obtaining the used features
 *  of the remote Bluetooth device.
 */
#define HCI_LE_READ_REMOTE_USED_FEATURES_COMPLETE_SUBEVENT      0x04U
#define HCI_LE_READ_REMOTE_FEATURES_COMPLETE_SUBEVENT           0x04U

/**
 *  The LE Long Term Key Requested subevent indicates that the master device is
 *  attempting to encrypt or re-encrypt the link and is requesting the Long Term
 *  key.
 */
#define HCI_LE_LONG_TERM_KEY_REQUESTED_SUBEVENT                 0x05U
#define HCI_LE_LONG_TERM_KEY_REQUEST_SUBEVENT                   0x05U
#endif /* BT_4_0 */

#ifdef HCI_LL_TOPOLOGY_CONN_UPDATE_SUPPORT
/**
 *  The LE Remote connection Paramter Request subevent indicates that the master
 *  device or the slave device has received a change in connection paramters
 *  request from its peer.
 */
#define HCI_LE_REMOTE_CONN_PARAM_REQ_SUBEVENT                   0x06U
#define HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST_SUBEVENT     0x06U
#endif /* HCI_LL_TOPOLOGY_CONN_UPDATE_SUPPORT */

#ifdef BT_4_2
/**
 *  The LE Data Length Change subevent notifies the Host of a change to either the
 *  maximum Payload length or the maximum transmission time of Data Channel
 *  PDUs in either direction. The values reported are the maximum that will
 *  actually be used on the connection following the change.
 */
#define HCI_LE_DATA_LENGTH_CHANGE_SUBEVENT                      0x07U

/**
 *  The LE Read Local P-256 Public Key Complete subevent is generated when
 *  local P-256 key generation is complete.
 */
#define HCI_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_SUBEVENT     0x08U

/**
 *  The LE Generate DHKey Complete subevent indicates that LE Diffie Hellman
 *  key generation has been completed by the Controller.
 */
#define HCI_LE_GENERATE_DHKEY_COMPLETE_SUBEVENT                 0x09U

/**
 *  The LE Enhanced Connection Complete subevent indicates to both of the Hosts
 *  forming the connection that a new connection has been created.
 */
#define HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVENT            0x0AU

/**
 *  The LE Direct Advertising Report subevent indicates that directed advertisements
 *  have been received where the advertiser is using a resolvable private address
 *  and the Scan_Filter_Policy is equal to 0x02 or 0x03.
 */
#define HCI_LE_DIRECT_ADVERTISING_REPORT_SUBEVENT               0x0BU
#define HCI_LE_DIRECTED_ADVERTISING_REPORT_SUBEVENT             0x0BU
#endif /* BT_4_2 */

#ifdef BT_5_0
/**
 *  The LE PHY Update Complete subevent indicates that the Controller has changed the
 *  transmitter PHY or receiver PHY in use.
 */
#define HCI_LE_PHY_UPDATE_COMPLETE_SUBEVENT                     0x0CU

/**
 *  The LE Extended Scan Ended subevent indicates that scanning has finished.
 */
#define HCI_LE_EXTENDED_ADVERTISING_REPORT_SUBEVENT             0x0DU

/**
 *  The LE Periodic Advertising Sync Established Event indicates that the
 *  Controller has received the first periodic advertising packet from an
 *  advertiser after the LE_Periodic_Advertising_Create_Sync Command has
 *  been sent to the Controller.
 */
#define HCI_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED_SUBEVENT   0x0EU

/**
 *  The LE Periodic Advertising Data Receive subevent provides the Host with
 *  the data received from a Periodic Advertising packet.
 */
#define HCI_LE_PERIODIC_ADVERTISING_REPORT_SUBEVENT             0x0FU

/**
 *  The LE Periodic Advertising Receive Ended subevent indicates when the
 *  Controller has lost synchronization with the Periodic Advertiser
 */
#define HCI_LE_PERIODIC_ADVERTISING_SYNC_LOST_SUBEVENT          0x10U

/**
 *  The LE Extended Advertising Ended subevent indicates that advertising in
 *  a given advertising set has finished
 */
#define HCI_LE_SCAN_TIMEOUT_SUBEVENT                            0x11U

/**
 *  The LE Extended Advertising Ended subevent indicates that advertising in
 *  a given advertising set has finished
 */
#define HCI_LE_ADVERTISING_SET_TERMINATED_SUBEVENT              0x12U

/**
 *  The LE Scan Request Received subevent indicates that a SCAN_REQ PDU or an
 *  AUX_SCAN_REQ PDU has been received by the advertiser.
 */
#define HCI_LE_SCAN_REQUEST_RECEIVED_SUBEVENT                   0x13U

/**
 *  The LE Channel Selection Algorithm subevent indicates which channel selection
 *  algorithm is used on a data channel connection.
 */
#define HCI_LE_CHANNEL_SELECTION_ALGORITHM_SUBEVENT             0x14U

#ifdef HCI_LE_SET_EXTENDED_SCAN_PARAMETERS_SUPPORT
/* HCI Extended Scan Parameters related Mask Values */
#define HCI_LE_SET_EXT_SCAN_PARAM_1M_PHY_MASK                   0x01U
#define HCI_LE_SET_EXT_SCAN_PARAM_2M_PHY_MASK                   0x02U
#define HCI_LE_SET_EXT_SCAN_PARAM_CODED_PHY_MASK                0x04U
#endif /* HCI_LE_SET_EXTENDED_SCAN_PARAMETERS_SUPPORT */
#endif /* BT_5_0 */

#ifdef BT_5_1
/**
 *  The HCI_LE_Connectionless_IQ_Report event is used by the Controller to
 *  report IQ information from the Constant Tone Extension of a received
 *  advertising packet forming part of the periodic advertising identified by
 *  Sync_Handle.
 */
#define HCI_LE_CONNECTIONLESS_IQ_REPORT_SUBEVENT                0x15U

/**
 *  The HCI_LE_Connection_IQ_Report event is used by the Controller to report
 *  the IQ samples from the Constant Tone Extension of a received packet
 */
#define HCI_LE_CONNECTION_IQ_REPORT_SUBEVENT                    0x16U

/**
 *  This event is used by the Controller to report an issue following a request
 *  to a peer device to reply with a packet containing an LL_CTE_RSP PDU and a
 *  Constant Tone Extension.
 */
#define HCI_LE_CTE_REQUEST_FAILED_SUBEVENT                      0x17U

/**
 * The HCI_LE_Periodic_Advertising_Sync_Transfer_Received event is used by
 * the Controller to report that it has received periodic advertising synchronization
 * information from the device referred to by the Connection_Handle parameter
 * and either successfully synchronized to the periodic advertising train or timed
 * out while attempting to synchronize.
 */
#define HCI_LE_PERIODIC_ADV_SYNC_TX_RECEIVED_SUBEVENT           0x18U
#define HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECEIVED_SUBEVENT \
                                                                0x18U
#endif /* BT_5_1 */

#ifdef BT_5_2
/**
 * The HCI_LE_CIS_Established event indicates that the CIS with the Connection_Handle
 * is established. The slave Controller shall set the Connection_Handle to the same
 * value as that provided in the HCI_LE_CIS_Request event. This event is generated by
 * the Controller in the master and slave.
 */
#define HCI_LE_CIS_ESTABLISHED_SUBEVENT                         0x19U

/**
 * The HCI_LE_CIS_Request event indicates that a Controller has received a request
 * to establish a CIS. The Controller shall assign a connection handle for
 * the requested CIS and send the handle in the CIS_Connection_Handle parameter
 * of the event.
 */
#define HCI_LE_CIS_REQUEST_SUBEVENT                             0x1AU

/**
 * The HCI_LE_Create_BIG_Complete event indicates that the HCI_LE_Create_BIG command
 * has completed.
 */
#define HCI_LE_CREATE_BIG_COMPLETE_SUBEVENT                     0x1BU

/**
 * The HCI_LE_Terminate_BIG_Complete event indicates that the transmission
 * of all the BISes in the BIG are terminated.
 */
#define HCI_LE_TERMINATE_BIG_COMPLETE_SUBEVENT                  0x1CU

/**
 * The HCI_LE_BIG_Sync_Established event indicates that the HCI_LE_BIG_Create_Sync
 * command has completed.
 */
#define HCI_LE_BIG_SYNC_ESTABLISHED_SUBEVENT                    0x1DU

/**
 * The HCI_LE_BIG_Sync_Lost event indicates that the Controller has either not
 * received any PDUs on a BIG within the timeout period BIG_Sync_Timeout,
 * or the BIG has been terminated by the remote device, or the local Host has
 * terminated synchronization using the HCI_LE_BIG_Terminate_Sync command.
 */
#define HCI_LE_BIG_SYNC_LOST_SUBEVENT                           0x1EU

/**
 * The HCI_LE_Request_Peer_SCA_Complete event indicates that
 * the HCI_LE_Request_Peer_SCA command has been completed.
 */
#define HCI_LE_REQUEST_PEER_SCA_COMPLETE_SUBEVENT               0x1FU

/**
 * The HCI_LE_Path_Loss_Threshold event is used to report a path loss threshold
 * crossing on the ACL connection identified by the Connection_Handle parameter.
 */
#define HCI_LE_PATH_LOSS_THRESHOLD_SUBEVENT                     0x20U

/**
 * The HCI_LE_Transmit_Power_Reporting event is used to report the transmit power
 * level on the ACL connection identified by the Connection_Handle parameter.
 */
#define HCI_LE_TX_POWER_REPORTING_SUBEVENT                      0x21U
#define HCI_LE_TRANSMIT_POWER_REPORTING_SUBEVENT                0x21U

/**
 * The HCI_LE_BIGInfo_Advertising_Report event indicates that the Controller
 * has received an Advertising PDU that contained a BIGInfo field.
 */
#define HCI_LE_BIGINFO_ADVERTISING_REPORT_SUBEVENT              0x22U
#endif /* BT_5_2 */

/*
 * Defines for Link status (current mode) returned by
 * BT_hci_get_connection_details()
 */
#define BT_HCI_ACTIVE_MODE           0x00U
#define BT_HCI_HOLD_MODE             0x01U
#define BT_HCI_SNIFF_MODE            0x02U
#define BT_HCI_PARK_MODE             0x03U

/* \} */

/**
 * \defgroup hci_utility_macros Utility Macros
 * \{
 */

/* =================================================== Macros */
#define hci_pack_1_byte_param(dest, src) \
    (dest)[0U] = (UCHAR)(*((UCHAR *)(src)));

#define hci_pack_2_byte_param(dest, src) \
    (dest)[0U] = (UCHAR)(*((UINT16 *)(src))); \
    (dest)[1U] = (UCHAR)(*((UINT16 *)(src)) >> 8U);

#define hci_pack_3_byte_param(dest, src) \
    (dest)[0U] = (UCHAR)(*((UINT32 *)(src)));\
    (dest)[1U] = (UCHAR)(*((UINT32 *)(src)) >> 8U);\
    (dest)[2U] = (UCHAR)(*((UINT32 *)(src)) >> 16U);

#define hci_pack_4_byte_param(dest, src) \
    (dest)[0U] = (UCHAR)(*((UINT32 *)(src)));\
    (dest)[1U] = (UCHAR)(*((UINT32 *)(src)) >> 8U);\
    (dest)[2U] = (UCHAR)(*((UINT32 *)(src)) >> 16U);\
    (dest)[3U] = (UCHAR)(*((UINT32 *)(src)) >> 24U);

#define hci_unpack_1_byte_param BT_UNPACK_LE_1_BYTE

#define hci_unpack_2_byte_param BT_UNPACK_LE_2_BYTE

#define hci_unpack_3_byte_param BT_UNPACK_LE_3_BYTE

#define hci_unpack_4_byte_param BT_UNPACK_LE_4_BYTE

#ifdef HCI_ISO_DATA
/** Extract ISO Handle from header */
#define hci_extract_iso_connection_handle(header, conn_handle) \
    { \
        hci_unpack_2_byte_param((conn_handle), (header)); \
        (*conn_handle) &= 0x0FFFU; \
    }

/** Extract Packet Broadcast flag from header */
#define hci_extract_iso_pb_flag(header, pb) \
        { \
            UINT16 value_2B_pb; \
            hci_unpack_2_byte_param(&value_2B_pb, header); \
            *(pb) = (value_2B_pb >> 12U); \
            *(pb) &= 0x03U; \
        }

/** Extract Timestamp flag from header */
#define hci_extract_iso_ts_flag(header, ts) \
        { \
            UINT16 value_2B_ts; \
            hci_unpack_2_byte_param(&value_2B_ts, header); \
            *(ts) = (value_2B_ts >> 14U); \
            *(ts) &= 0x01U; \
        }

/** Extract ISO Data Load length from header */
#define hci_extract_iso_data_load_len(header, iso_dataload_len) \
        { \
            hci_unpack_2_byte_param((iso_dataload_len), &((header)[2U])); \
            *(iso_dataload_len) &= 0x3FFFU; \
        }

/** Extract Timestamp flag from header */
#define hci_extract_timestamp(header, timestamp) \
        { \
            hci_unpack_4_byte_param((timestamp), &((header)[4U])); \
        }

/** Extract Sequence Number from header */
#define hci_extract_packet_sequence_number(header, seqnum) \
        { \
            UINT16 ts; \
            hci_extract_iso_ts_flag((header), &ts); \
            if (0U != ts) \
            { \
                hci_unpack_2_byte_param((seqnum), &((header)[8U])); \
            } \
            else \
            { \
                hci_unpack_2_byte_param((seqnum), &((header)[4U])); \
            } \
        }

/** Extract ISO SDU length from header */
#define hci_extract_iso_sdu_length(header, iso_sdulen) \
        { \
            UINT16 ts; \
            hci_extract_iso_ts_flag((header), &ts); \
            if (0U != ts) \
            { \
                hci_unpack_2_byte_param((iso_sdulen), &((header)[10U])); \
            } \
            else \
            { \
                hci_unpack_2_byte_param((iso_sdulen), &((header)[6U])); \
            } \
            *(iso_sdulen) &= 0x0FFFU; \
        }

/** Extract Packet Status from header */
#define hci_extract_iso_packet_status_flag(header, ps) \
        { \
            UINT16 value_2B_ps; \
            hci_extract_iso_sdu_length((header), &value_2B_ps); \
            *(ps) = (value_2B_ps >> 14U); \
            *(ps) &= 0x03U; \
        }

/** Extract the ISO header onto the HCI_ISO_HEADER */
#define HCI_EXTRACT_ISO_HEADER(header, iso_header) \
        { \
            hci_extract_iso_connection_handle((header), &((iso_header)->conn_handle)); \
            hci_extract_iso_pb_flag((header), &((iso_header)->pb_flag)); \
            hci_extract_iso_ts_flag((header), &((iso_header)->ts_flag)); \
            hci_extract_iso_data_load_len((header), &((iso_header)->dataload_len)); \
            if ((iso_header)->ts_flag) \
            { \
                hci_extract_timestamp((header), &((iso_header)->ts)); \
            } \
            hci_extract_packet_sequence_number((header), &((iso_header)->seqnum)); \
            hci_extract_iso_sdu_length((header), &((iso_header)->sdulen)); \
            hci_extract_iso_packet_status_flag((header), &((iso_header)->ps_flag)); \
        }
#endif /* HCI_ISO_DATA */

/** \} */

/**
 * \addtogroup hci_defines Defines
 * \{
 */

/**
 * \defgroup hci_structures Structures
 * \{
 */

/* =================================================== Structures/Data Types */
/* HCI Link Key */
typedef struct
{
    UCHAR  bd_addr[BT_BD_ADDR_SIZE];
    UCHAR  link_key[16U];

} HCI_H_LINK_KEY;


/* HCI Command/API Opcode */
typedef struct
{
    UCHAR  ogf;
    UINT16 ocf;

} HCI_OPCODE;


/* HCI Inquiry Result Structure */
typedef struct
{
    UCHAR   bd_addr[BT_BD_ADDR_SIZE];
    UCHAR   page_scan_repetition_mode;
    UCHAR   page_scan_period_mode;
    UCHAR   page_scan_mode;
    UINT32  class_of_device;
    UINT16  clock_offset;

#ifdef BT_HCI_1_2
    UCHAR   rssi;
#endif /* BT_HCI_1_2 */

} HCI_INQUIRY_RESULT;


#ifdef BT_HCI_1_2
/* Synchronous Connection Input Parameters */
typedef struct
{
    UINT32  tx_bandwidth;
    UINT32  rx_bandwidth;
    UINT16  max_latency;
    UINT16  voice_setting;
    UINT16  packet_type;
    UCHAR   rtx_effort;

} HCI_SCO_IN_PARAMS;


/* Synchronous Connection Output Parameters */
typedef struct
{
    UINT16  sco_handle;
    UCHAR   link_type;
    UCHAR   tx_interval;
    UCHAR   rtx_window;
    UINT16  rx_length;
    UINT16  tx_length;
    UCHAR   air_mode;

} HCI_SCO_OUT_PARAMS;
#endif /* BT_HCI_1_2 */

#ifdef HCI_ENH_SCO
typedef struct
{
    /**
     *  Coding format defined in Assigned Numbers document.
     *  Example: LMP_CODING_FRMT_PCM, LMP_CODING_FRMT_VS etc.
     */
    UCHAR  coding_format;

    /**
     *  If Coding Format is not 0xFF (LMP_CODING_FRMT_VS),
     *  Company ID and Vendor Specific Codec ID shall be ignored.
     */
    UINT16 company_id;

    UINT16 vendor_codec_id;

} HCI_CODING_FORMAT;

typedef struct
{
    UINT32             tx_bandwidth;
    UINT32             rx_bandwidth;
    HCI_CODING_FORMAT  tx_coding_frmt;
    HCI_CODING_FORMAT  rx_coding_frmt;
    UINT16             tx_codec_frame_size;
    UINT16             rx_codec_frame_size;
    UINT32             input_bandwidth;
    UINT32             output_bandwidth;
    HCI_CODING_FORMAT  input_coding_frmt;
    HCI_CODING_FORMAT  output_coding_frmt;
    UINT16             input_coded_data_size;
    UINT16             output_coded_data_size;
    UCHAR              input_pcm_data_frmt;
    UCHAR              output_pcm_data_frmt;
    UCHAR              input_pcm_smpl_payload_msb_pos;
    UCHAR              output_pcm_smpl_payload_msb_pos;
    UCHAR              input_data_path;
    UCHAR              output_data_path;
    UCHAR              input_transport_unit_size;
    UCHAR              output_transport_unit_size;
    UINT16             max_latency;
    UINT16             packet_type;
    UCHAR              retransmission_effort;
} HCI_ENH_SCO_PARAMS;
#endif /* HCI_ENH_SCO */

/* HCI Connection List */
typedef struct
{
    /* ACL Connection Handle */
    UINT16 acl_handle;

    /*
     *  SCO Handles for all SCO connections that exists with the
     *  remote Device identified by the ACL Connection Handle.
     */
    UINT16 sco_handle [ HCI_MAX_SCO_CHANNELS ];

    /* BD_ADDR of the Remote Device */
    UCHAR  bd_addr[BT_BD_ADDR_SIZE];

    /* Current Mode of the ACL Link */
    UCHAR  current_mode;

} HCI_CONNECTION_LIST;

#ifdef HCI_TX_RUN_TIME_SELECTION
typedef API_RESULT (* HCI_TRANSPORT_CB)
                   (
                       UCHAR    packet_type,
                       UCHAR *  packet_data,
                       UINT16   packet_len,
                       UCHAR    flag
                   ) DECL_REENTRANT;

API_RESULT BT_hci_register_tx_cb (/* IN */ HCI_TRANSPORT_CB callback_ptr);
#endif /* HCI_TX_RUN_TIME_SELECTION */


/* HCI QoS */
typedef struct
{
    /* Token Rate */
    UINT32 token_rate;

    /* Token Bucket Size */
    UINT32 token_bucket_size;

    /* Peak Bandwidth */
    UINT32 peak_bandwidth;

    /* Latency */
    UINT32 latency;

    /* Delay Variation */
    UINT32 delay_variation;

    /* Service Type */
    UCHAR service_type;

    /* Flow Direction */
    UCHAR flow_direction;

} HCI_QOS;


#ifdef BT_EIR
typedef struct
{
    /* EIR Data */
    UCHAR  * eir_data;

    /* EIR Data Length */
    UCHAR    eir_data_len;

    /* EIR Data Type */
    UCHAR    eir_data_type;

} HCI_EIR_DATA;
#endif /* BT_EIR */

#ifdef HCI_ISO_DATA
/** ISO Header information */
typedef struct _HCI_ISO_HEADER
{
    /* ISO connection handle */
    UINT16 conn_handle;

    /* Packet Broadcast Flag */
    UINT8  pb_flag;

    /* Timestamp presence flag */
    UINT8  ts_flag;

    /* ISO Dataload length */
    UINT16 dataload_len;

    /* Timestamp value */
    UINT32 ts;

    /* Sequence Number value */
    UINT16 seqnum;

    /* ISO SDU Length */
    UINT16 sdulen;

    /* ISO packet status */
    UINT8  ps_flag;
} HCI_ISO_HEADER;
#endif /* HCI_ISO_DATA */

/** \} */

/** \} */



/**
 * \defgroup hci_api API Definitions
 * \{
 */

/* ==========================================================  API Functions */

#ifdef __cplusplus
extern "C"{
#endif

/* Internal Functions (Not APIs) ------------------------------------------- */
/* \cond ignore_this Not to be documented - not used by application */
API_RESULT hci_write_command_UCHAR
           (
               UINT16    opcode,
               UCHAR     param
           );

API_RESULT hci_write_command_UCHAR_UCHAR
           (
               UINT16    opcode,
               UCHAR     param_1,
               UCHAR     param_2
           );

API_RESULT hci_write_command_UINT16
           (
               UINT16    opcode,
               UINT16    param
           );

API_RESULT hci_write_command_UINT16_UINT16
           (
               UINT16    opcode,
               UINT16    param_1,
               UINT16    param_2
           );

#ifdef HCI_SUPPORT_STORED_LINK_KEY_COMMANDS
API_RESULT hci_write_command_BD_ADDR_UCHAR
           (
               UINT16    opcode,
               UCHAR   * bd_addr,
               UCHAR     param_2
           );
#endif /* HCI_SUPPORT_STORED_LINK_KEY_COMMANDS */

API_RESULT hci_write_command_UCHAR_array
           (
               UINT16    opcode,
               UCHAR   * param,
               UINT16    length
           );

/* Common API Handler */
API_RESULT hci_common_api_handler_no_params
           (
               UINT16    opcode
           );

API_RESULT hci_write_command_connection_handle
           (
               UINT16    opcode,
               UINT16    connection_handle
           );

API_RESULT hci_write_command_adv_scan_response_data
           (
               UINT16   opcode,
               UCHAR    data_len,
               UCHAR *  data_param
           );

#ifdef BT_4_2
API_RESULT hci_write_command_BD_ADDR_with_TYPE
           (
               UINT16    opcode,
               UCHAR     addr_type,
               UCHAR   * bd_addr
           );
#endif /* BT_4_2 */

/** \endcond */

/* ------------------------------------------------------------------------- */


/* HCI External APIs ------------------------------------------------------- */
/**
 *  \brief To register the HCI Event Indication Callback of Application with HCI
 *
 *  \par Description:
 *       This API is for registering the HCI Event Indication Callback.
 *       The callback Function Pointer cannot be NULL. Reregistration of the
 *       event handler is not allowed.
 *
 *  \param [in] callback_ptr
 *         Function Pointer to application callback
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_register_event_indication_callback
           (
               API_RESULT (* callback_ptr)
                          (
                              UCHAR    event_type,
                              UCHAR *  event_data,
                              UCHAR    event_datalen
                          )
           );
/**
 *  \brief To register the HCI Error Indication Callback of Application with HCI
 *
 *  \par Description:
 *       This API is for registering the HCI Error Indication Callback.
 *       The callback Function Pointer cannot be NULL. Reregistration of the
 *       event handler is not allowed.
 *
 *  \param [in] callback_ptr
 *         Function Pointer to application callback
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_register_error_indication_callback
           (
               API_RESULT (* callback_ptr)
                          (
                              UINT16  opcode,
                              UINT16  error_code
                          )
           );

#ifdef HCI_SCO
/**
 *  \brief To register the HCI SCO data handler of Application with HCI
 *
 *  \par Description:
 *       This API is for registering the HCI SCO data handler.
 *       The callback Function Pointer cannot be NULL. Reregistration of the
 *       event handler is not allowed.
 *
 *  \param [in] callback_ptr
 *         Function Pointer to application HCI SCO data handler
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_register_sco_data_handler
           (
               API_RESULT (* callback_ptr) (UCHAR *, UINT16)
           );
#endif /* HCI_SCO */

#ifdef HCI_ISO_DATA
/**
 *  \brief To register the HCI ISO data handler of Application with HCI
 *
 *  \par Description:
 *       This API is for registering the HCI ISO data handler.
 *       The callback Function Pointer cannot be NULL. Reregistration of the
 *       event handler is not allowed.
 *
 *  \param [in] callback_ptr
 *         Function Pointer to application HCI ISO data handler
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_register_iso_data_handler
           (
               API_RESULT (* callback_ptr) (UCHAR *header, UCHAR *data, UINT16 datalen)
           );
#endif /* HCI_ISO_DATA */

/**
 *  \brief To set the role of the local Device role for all new ACL connections.
 *
 *  \par Description:
 *       This API sets the Role that the local Bluetooth device will attempt
 *       to assume, either Master or Slave, for all new ACL connection that
 *       is remotely initiated from a particular specified, or, all remote
 *       Bluetooth device(s).
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of a remote Bluetooth device if the
 *         Role parameter is to  be specified for a particular remote
 *         Bluetooth device. If this parameter is set to NULL, then the
 *         Role parameter is applied to incoming ACL connections from all
 *         remote Bluetooth devices.
 *
 *  \param [in] Role
 *         The value of the Device Role parameter to be set in HCI layer
 *         Valid values are:
 *         Value 0x00 - be Master.
 *         Value 0x01 - be Slave (Default).
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_set_device_role
           (
               UCHAR *  bd_addr,
               UCHAR    role
           );
/**
 *  \brief To retrieve the BD_ADDR of the local Bluetooth device, as stored in
 *         the HCI module.
 *
 *  \par Description:
 *       This API is used to get the BD address of the local device.
 *
 *  \param [out] bd_addr
 *         The variable passed by the user into which the BD address is copied.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_get_local_bd_addr
           (
               UCHAR *  bd_addr
           );
/**
 *  \brief To retrieve the ACL Connection Handle for a specified
 *         Bluetooth Device Address.
 *
 *  \par Description:
 *       This API retrieves the ACL Connection Handle, if one exists,
 *       for a specified Remote Bluetooth device as given by the
 *       BD_ADDR parameter.
 *
 *  \param [in] bd_addr
 *         The Remote Bluetooth Device Address for which the
 *         ACL Connection Handle to be retrieved.
 *
 *  \param [out] handle
 *         Pointer to a caller allocated UINT16 type variable onto which the
 *         ACL Connection Handle will be stored.
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_get_acl_connection_handle
           (
               UCHAR *   bd_addr,
               UINT16 *  handle
           );
/**
 *  \brief To retrieve the list of SCO Connection Handles for a specified remote
 *         Bluetooth Device Address.
 *
 *  \par Description:
 *       This API retrieves the SCO Connection Handles for a remote
 *       Bluetooth device, if one exists.
 *
 *  \param [in] bd_addr
 *         The remote Bluetooth Device Address for which the SCO Connection
 *         Handles have to be retrieved.
 *
 *  \param [out] handle
 *         Pointer to a caller allocated UINT16 type variable onto which the
 *         ACL Connection Handle will be stored.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_get_sco_connection_handle
           (
               UCHAR *   bd_addr,
               UINT16 *  handle,
               UCHAR *   num_handle
           );
/**
 *  \brief To get details for all the active connections from the HCI.
 *
 *  \par Description:
 *       This API can be used to retrieve the details of the active ACL, SCO and
 *       eSCO connections to all remote Bluetooth devices from the HCI module.
 *       The details, that can be retrieved using this API, include
 *       ACL connection status, ACL handle and the SCO/eSCO Connection Handles
 *       for every remote Bluetooth device.The Connection Handle values are set
 *       to 0xFFFF, if they are invalid, and these should be interpreted as
 *       non-existent ACL/SCO/eSCO links.
 *
 *  \param [in] num_allocated
 *         Array size of the hci_conn_list allocated by the caller.
 *         This parameter must be less than HCI_MAX_REMOTE_DEVICES.
 *
 *  \param [out] hci_conn_list
 *         Pointer to a caller allocated HCI_CONNECTION_LIST Array, that will be
 *         used to fill up the details of ACL/SCO/eSCO link information from HCI.
 *         The allocated array size is given by the num_allocated parameter.
 *
 *  \param [out] Num_present
 *         Pointer to a caller allocated UCHAR variable that is populated by HCI
 *         to denote actual number of location filled in hci_conn_list. This
 *         value will less than or equal to num_allocated.
 *         When num_allocated is greater than HCI_MAX_REMOTE_DEVICES, the API
 *         will provide connection information for a maximum of
 *         HCI_MAX_REMOTE_DEVICES elements only.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_get_connection_details
           (
               HCI_CONNECTION_LIST *   hci_conn_list,
               UCHAR                   num_allocated,
               UCHAR *                 num_present
           );

#ifdef BT_4_0
/**
 *  \brief To get own Bluetooth device address used in the connection.
 *
 *  \par Description:
 *       This API is used to get the own BD address used in the connection.
 *
 *  \param [in] remote_bd_addr
 *         The variable passed by the user which conatins remote device
 *         BD address.
 *
 *  \param [in] remote_bd_addr_type
 *         The variable passed by the user which conatins remote device BD
 *         address type.
 *
 *  \param [out] bd_addr
 *         The variable passed by the user into which the BD address is copied.
 *
 *  \param [out] bd_addr_type
 *         The variable passed by the user into which the BD address type is
 *         copied.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_own_conn_bd_address
           (
               /* IN */  BT_DEVICE_ADDR *  remote_bd_addr,
               /* OUT */ BT_DEVICE_ADDR *  bd_addr
           );
#endif /* BT_4_0 */
/**
 *  \brief To decode a given HCI Command Opcode.
 *
 *  \par Description:
 *       This utility API decodes a given HCI Command Opcode into OCF & OGF
 *       values. For more on OCF, OGF and HCI Command Opcode, please refer to
 *       the Specification of the Bluetooth System, v1.2, Vol. 2, Part E - Host
 *       Controller Interface Functional Specification, Section 5.4.1.
 *
 *  \param [in] opcode
 *         The HCI Command Opcode to be decoded.
 *
 *  \param [out] Parameters
 *         Pointer to a HCI_OPCODE type variable containing OCF & OGF values,
 *         onto which decoded values will be copied.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_decode_opcode
           (
               UINT16        opcode,
               HCI_OPCODE *  hci_opcode
           );
/**
 *  \brief To decode a HCI Inquiry response Bytes.
 *
 *  \par Description:
 *       This utility API decodes a given Inquiry Result Bytes into HCI Inquiry
 *       Result parameters. Each HCI Inquiry Result consists of 14 bytes
 *       containing the BD_ADDR, various Page Scan Modes, Class of Device
 *       and Clock Offset.This API should not be used to decode Inquiry Result
 *       bytes, if the Inquiry Mode (HCI-1.2 only) is set to return Inquiry
 *       Result with RSSI Event (see BT_hci_write_inquiry_mode() API). Instead,
 *       the BT_hci_decode_inquiry_result_rssi() API should be used.
 *
 *  \param [in] buffer
 *         Pointer to a 14-bytes UCHAR array containing the Inquiry Result bytes.
 *
 *  \param [out] result
 *         Pointer to a HCI_INQUIRY_RESULT type variable onto which decoded
 *         information will be stored.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_decode_inquiry_result
           (
               UCHAR *               buffer,
               HCI_INQUIRY_RESULT *  result
           );

#ifdef HCI_HAVE_INIT_COMMAND_MASK
/**
 *  \brief To initiate initialization of Command mask
 *
 *  \par Description:
 *       This API is used to initiate the initialization of a mask for HCI
 *       commands which disables processing of those selected commands during
 *       Bluetooth Initialization
 *
 *  \param [in] command_mask
 *         Mask (Bitmap) for HCI Commands that are allowed to be switched off
 *         (not to be sent) during HCI Bluetooth-ON processing (HCI BT-Init).
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_set_init_command_mask
           (
               UINT32    command_mask
           );
#endif /* HCI_HAVE_INIT_COMMAND_MASK */

#ifdef BT_SUPPORT_CONTROLLER_INIT_SKIP
/**
 *  \brief To skip Bluetooth Controller Initialization
 *
 *  \par Description:
 *       This API is used to set a global state identifier to skip
 *       initialization of Bluetooth Controller
 *
 *  \param [in] state
 *         The value of state identifier to be set
 *         Value BT_TRUE - Initialize controller on BT Init
 *         Value BT_FALSE - Skip initialization of Bluetooth Controller
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_set_controller_init (/* IN */ UCHAR state);
#endif /* BT_SUPPORT_CONTROLLER_INIT_SKIP */
/**
 *  \brief To skip Bluetooth Controller Initialization
 *
 *  \par Description:
 *       This API is used to set a global state identifier to skip
 *       initialization of Bluetooth Controller
 *
 *  \param [in] state
 *         The value of state identifier to be set
 *         Value BT_TRUE - Initialize controller on BT Init
 *         Value BT_FALSE - Skip initialization of Bluetooth Controller
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_get_connection_handle
           (
               BT_DEVICE_ADDR * bd_addr,
               UCHAR link_type,
               UINT16 *  handle
           );

#if 0
/**
 *  \brief To get connection type by using Bluetooth Device Address.
 *
 *  \par Description:
 *       This API is used to get the connection type with respect to a
 *       particular device or connection handle.
 *
 *  \param [in] bd_addr
 *         The variable passed by the user which has the BD address of the
 *         remote device.
 *
 *  \param [in] bd_addr_type
 *         The variable passed by the user which has the BD address type of the
 *         remote device.
 *
 *  \param [out] conn_type
 *         The variable into which the connection type is copied.
 *
 *  \return
 *      API_RESULT:
 *      API_SUCCESS : After the handle is copied.
 *      HCI_STATE_NOT_CONNECTED : If the device is not connected.
 *      HCI_DEVICE_ASSERTION_FAILED : If the device is not found
 *                                    in the list of remote devices.
 */
API_RESULT BT_hci_get_connection_type
           (
               UCHAR *   bd_addr,
               UCHAR     bd_addr_type,
               UCHAR *   conn_type
           );
#endif /* 0 */

#ifdef BT_4_0

#ifdef HCI_SUPPORT_GET_DEVICE_ROLE
/**
 *  \brief To get role of the local device.
 *
 *  \par Description:
 *       This API can be used to get the local device role
 *       (Master or Slave) in a HCI LE connection.
 *
 *       If the local device has a HCI BR/EDR or LE connection,
 *       role will be one of the following values
 *       - BT_DEVICE_ROLE_MASTER
 *       - BT_DEVICE_ROLE_SLAVE
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_get_device_role
           (
               BT_DEVICE_ADDR * bd_addr,
               UCHAR            link_type,
               UCHAR          * role
           );

#define BT_hci_br_edr_get_device_role(b,r)\
        BT_hci_get_device_role ((b),DQ_BR_LINK,(r))

#define BT_hci_le_get_device_role(b,r)\
        BT_hci_get_device_role ((b),DQ_LE_LINK,(r))
#endif /* HCI_SUPPORT_GET_DEVICE_ROLE */

/**
 *  \brief To get LE connection handle for specified Bluetooth Address and
 *         Address type.
 *
 *  \par Description:
 *      This API retrives the LE connection handle, if one exist, for a
 *      specified remote Bluetooth Address and Address type as given by bd_addr
 *      and bd_addr_type paramters respectively.
 *
 *  \param [in] bd_addr
 *         The Remote Bluetooth Device Address for which the Connection
 *         Handle to retrieved.
 *  \param [out] handle
 *         Pointer to a caller allocated UINT16 type variable onto which the
 *         Connection Handle will be stored.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_get_le_connection_handle(bd_addr, handle)\
        BT_hci_get_connection_handle   \
        ((bd_addr), HCI_LE_LINK, (handle));
#endif /* BT_4_0 */


#ifdef BT_HCI_1_2
/**
 *  \brief To decode a given HCI Inquiry Result Bytes with RSSI values.
 *
 *  \par Description:
 *       This utility API decodes a given Inquiry Result Bytes into HCI Inquiry
 *       Result parameters. Each HCI Inquiry Result consists of 14 bytes
 *       containing the BD_ADDR, various Page Scan Modes, Class of Device,
 *       Clock Offset, and RSSI. This API should not be used to decode
 *       Inquiry Result bytes, if the Inquiry Mode (HCI-1.2 only) is set to
 *       return standard Inquiry Result Event format
 *       (see BT_hci_write_inquiry_mode() API).
 *       Instead, the BT_hci_decode_inquiry_result() API should be used.
 *
 *  \param [in] buffer
 *         Pointer to a 14-bytes UCHAR array containing the Inquiry Result bytes.
 *
 *  \param [out] result
 *         Pointer to a HCI_INQUIRY_RESULT type variable onto which decoded
 *         information will be stored.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_decode_inquiry_result_rssi
           (
               UCHAR *               buffer,
               HCI_INQUIRY_RESULT *  result
           );
/**
 *  \brief To create or update a Synchronous Connection (eSCO).
 *
 *  \par Description:
 *       This API enables Profiles, and Applications, to specify eSCO channel
 *       parameters, to be used while accepting an incoming eSCO Connection
 *       Request using the HCI Accept Synchronous Connection Command.
 *       The EtherMind HCI module automatically accepts incoming SCO connection
 *       request from the Bluetooth hardware. But for eSCO connection request,
 *       HCI cannot perform automatic accept since it needs to know choice of
 *       profile for eSCO channel parameters.The Connection Request Event for
 *       eSCO is delivered to Profiles on its registered HCI Event Indication
 *       Callback. The Profile is required to supply HCI with the required eSCO
 *       channel parameter using this API from the context of the
 *       HCI Event Indication Callback.
 *
 *  \param [in] sco_params
 *         Pointer to caller allocated HCI_SCO_IN_PARAMS variable containing the
 *         input parameters for negotiating the new or existing synchronous link.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_set_esco_channel_parameters
           (
               HCI_SCO_IN_PARAMS *  sco_params
           );

#ifdef HCI_ENH_SCO
/**
 *  \brief To specify Synchronous Connection (eSCO) channel parameters to be
 *         used while accepting eSCO connection request.
 *
 *  \par Description:
 *       This API enables application, to specify eSCO channel parameters, to be
 *       used while accepting an incoming enhanced eSCO Connection Request using
 *       the HCI Accept Enhanced Synchronous Connection Command.
 *       The Connection Request Event for enhanced eSCO is delivered to the
 *       application on its registered HCI Event Indication Callback.
 *       The application is required to supply HCI with the required
 *       eSCO channel parameter using this API from the context of the HCI Event
 *       Indication Callback.
 *
 *  \param [in] params
 *         Pointer to caller allocated HCI_ENH_SCO_PARAMS variable containing
 *         the input parameters for negotiating the new or existing synchronous
 *         link.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_set_enh_sco_channel_parameters
           (
               HCI_ENH_SCO_PARAMS *  params
           );
#endif /* HCI_ENH_SCO */
#endif /* BT_HCI_1_2 */

#ifdef HCI_SCO
API_RESULT BT_hci_sco_write
           (
               UINT16     connection_handle,
               UCHAR *    packet,
               UINT16     packet_len
           );
#endif /* HCI_SCO */

#ifdef HCI_ISO_DATA
API_RESULT BT_hci_iso_write
           (
               UINT16     connection_handle,
               UCHAR      ts_flag,
               UINT32     ts,
               UINT16     seq_num,
               UCHAR *    sdu,
               UINT16     sdu_len
           );

#ifdef HCI_SUPPORT_ISO_WRITE_PDU
/**
 *  API_RESULT BT_hci_iso_write_pdu:
 *  API to write ISO data PDU, where the application manages the queue.
 *
 *  @param: (IN) connection_handle: ISO Connection Handle
 *  @param: (IN) packet: ISO fragment packet pointer
 *  @param: (IN) packet_len: ISO fragment length
 *
 *  @return: API_RESULT
 *           API_SUCCESS : On successful registration of the callback pointer.
 */
API_RESULT BT_hci_iso_write_pdu
           (
               UINT16     connection_handle,
               UCHAR *    packet,
               UINT16     packet_len
           );
#endif /* HCI_SUPPORT_ISO_WRITE_PDU */
#endif /* HCI_ISO_DATA */

/* ------------------------------------------------------------------------- */

/* HCI Link Control APIs --------------------------------------------------- */

#ifndef HCI_LITE
/**
 *  \brief To initiate/perform Bluetooth Device Inquiry/Discovery.
 *
 *  \par Description:
 *       This API initiates/performs the Bluetooth Device Inquiry/Discovery for
 *       the specified Inquiry Length and Number of Responses, and with the
 *       specified LAP (Lower Address Part) from which the local Bluetooth
 *       device derives the Inquiry Access Code (IAC).
 *       The local Bluetooth device starts the Bluetooth Inquiry on reception
 *       of the HCI Inquiry Command, and sends a Command Status Event to the
 *       HCI. At the end of the Inquiry process, the Inquiry Complete Event is
 *       received. Between these two events, none, one or more Inquiry Results
 *       Event may be received, when one or more remote Bluetooth devices
 *       respond to the Inquiry process initiated by the local Bluetooth
 *       device. This API must not be called if the local Bluetooth device is
 *       in the process of establishing a connection with another Bluetooth
 *       device. A remote Bluetooth device will respond to Inquiry procedure
 *       only if it has its Inquiry Scan enabled.
 *
 *  \param [in] lap
 *         This is the Lower Address Part from which the Bluetooth Hardware
 *         should derive Inquiry Access Code when the Inquiry procedure is made.
 *         Few of the commonly used values are defined in BT_assigned_numbers.h:
 *         * BT_GIAC - The General Inquiry Access Code (0x9E8B33)
 *         * BT_LIAC - The Limited Inquiry Access Code (0x9E8B30)
 *
 *  \param [in] inquiry_length
 *         Maximum amount of time specified before the Inquiry is halted.
 *         Value Range: 0x01 to 0x30. Inquiry Time = inquiry_length * 1.28 Sec.
 *
 *  \param [in] Num_responses
 *         Maximum number of responses from the Inquiry before the Inquiry is
 *         halted. Range: 0x00 - 0xFF. A value 0f 0x00 means unlimited number
 *         of responses.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_inquiry
           (
               UINT32   lap,
               UCHAR    inquiry_length,
               UCHAR    num_responses
           );

#define BT_hci_inquiry_cancel() \
        hci_common_api_handler_no_params (HCI_INQUIRY_CANCEL_OPCODE)
/**
 *  \brief To initiate/perform Bluetooth Device Inquiry/Discovery.
 *
 *  \par Description:
 *       This API can be used to configure the local Bluetooth device to enter
 *       the Periodic Inquiry Mode that performs automatic Inquiries at periodic
 *       intervals. Maximum and Minimum Period Lengths define the time range
 *       between two consecutive inquiries. The Bluetooth Controller uses this
 *       range to determine a new random time between two consecutive inquiries
 *       for each Inquiry.
 *       The local Bluetooth device starts the periodic Bluetooth Inquiry on
 *       reception of the HCI Periodic Inquiry Mode Command, and sends a Command
 *       Complete Event to the HCI to confirm the same. At the end of each
 *       Inquiry process, an Inquiry Complete Event will be received. Also
 *       during each Inquiry process, local Bluetooth device may send none,
 *       one or more Inquiry Results Events, when one or more remote Bluetooth
 *       devices respond to the Inquiry process initiated by the local Bluetooth
 *       device. The results of the periodic inquiry are returned to
 *       Application using HCI Event Indication Callback.
 *       This API must not be called if the local Bluetooth device is in the
 *       process of establishing a connection with another Bluetooth device.
 *       A remote Bluetooth device will respond to Inquiry procedure only
 *       if it has its Inquiry Scan enabled.
 *       This API must not be called if BT_hci_inquiry() was called earlier
 *       and has not completed yet.
 *
 *  \param [in] max_period_length
 *         Maximum time period between two consecutive Inquiry processes.
 *         The time is calculated as follows:
 *         Time = max_period_length * 1.28 Sec
 *         Value Range = 0x0003 - 0xFFFF
 *         Time Range = 3.84 - 83884.3 Sec
 *
 *  \param [in] min_period_length
 *         Minimum time period between two consecutive Inquiry processes.
 *         The time is calculated as follows:
 *         Time = min_period_length * 1.28 Sec
 *         Value Range = 0x0002 - 0xFFFE
 *         Time Range = 2.56 - 83883.52 Sec
 *
 *  \param [in] lap
 *         This is the Lower Address Part from which the Bluetooth Hardware
 *         should derive Inquiry Access Code when the Inquiry procedure is made.
 *         Few of the commonly used values are (defined in BT_assigned_numbers.h):
 *         BT_GIAC - The General Inquiry Access Code (0x9E8B33)
 *         BT_LIAC - The Limited Inquiry Access Code (0x9E8B30)
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_periodic_inquiry_mode
           (
               UINT16  max_period_length,
               UINT16  min_period_length,
               UINT32  lap,
               UCHAR   inquiry_length,
               UCHAR   num_responses
           );

#define BT_hci_exit_periodic_inquiry_mode() \
        hci_common_api_handler_no_params (HCI_EXIT_PERIODIC_INQUIRY_MODE_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To initiate ACL link creation process.
 *
 *  \par Description:
 *       This API is used to create an ACL connection to the specified remote
 *       Bluetooth device.
 *       The local Bluetooth device starts the Bluetooth Paging process to
 *       create an ACL link on reception of the HCI Create Connection Command,
 *       and sends a Command Status Event to the HCI. At the end of the
 *       Paging process, the Connection Complete Event is received.
 *       Before receiving the Connection Complete Event, Bluetooth Security
 *       related events (Link Key Request Event and/or PIN Code Request Event)
 *       might be received, depending on the security settings on the local
 *       and/or remote Bluetooth device.
 *       For details on the Bluetooth ACL link establishment, with or without
 *       link-level authentication procedure, refer to the Specification of the
 *       Bluetooth System, v1.2, Vol. 2, Part F - Message Sequence Charts. There
 *       can only be one ACL connection between a pair of Bluetooth devices.
 *       Establishment of ACL link depends to the Page Scan mode of the remote
 *       Bluetooth device.
 *
 *  \param [in] bd_addr
 *         This is the Bluetooth Device Address for the remote device to which
 *         an attempt will be made to create an ACL connection.
 *
 *  \param [in] Packet_type
 *         This is the ACL Packet Types to be used by the Baseband Controller
 *         for this new ACL connection. Refer to the EtherMind HCI Constants
 *         section for the valid values for this parameter. One or more DHx/DMx
 *         packet types can be combined together.
 *
 *  \param [in] Page_scan_repetition_mode and page_scan_mode
 *         These two parameters specify the Page Scan modes supported by the
 *         remote Bluetooth device - this information must have been acquired
 *         during the Bluetooth Inquiry procedure.
 *
 *  \param [in] Clock_offset
 *         The clock offset between the local Baseband Controller  and the
 *         remote one - this information must have been acquired during
 *         Bluetooth Inquiry procedure.
 *
 *  \param [in] Allow_role_switch
 *         This parameter specified whether the local Baseband Controller
 *         would accept or reject the request of a Master-Slave role switch
 *         during the ACL connection establishment phase.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_create_connection
           (
               UCHAR *   bd_addr,
               UINT16    packet_type,
               UCHAR     page_scan_repetition_mode,
               UCHAR     page_scan_mode,
               UINT16    clock_offset,
               UCHAR     allow_role_switch
           );
/**
 *  \brief To add a SCO connection with the remote device.
 *
 *  \par Description:
 *       This API attempts to add a SCO connection with the specified remote
 *       Bluetooth device. An ACL connection must exist before an attempt is
 *       made to add a SCO connection.
 *       The local Bluetooth device starts the Link Manager procedure to add an
 *       SCO link on reception of the HCI Add SCO Connection Command, and sends
 *       a Command Status Event to the HCI. At the end of the link creation
 *       process, the Connection Complete Event is received.
 *       For details on the Bluetooth Synchronous link establishment,
 *       refer to the Specification of the Bluetooth System,
 *       v1.2, Vol. 2, Part F - Message Sequence Charts.
 *
 *  \param [in] bd_addr
 *         This is the Bluetooth Device Address for the remote device.
 *
 *  \param [in] Packet_type
 *         This is the SCO Packet Type to be used for this new SCO connection.
 *         Refer to the EtherMind HCI Constants section for the valid values for
 *         this parameter. One or more HVx packet types can be combined together.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_add_sco_connection
           (
               UCHAR *   bd_addr,
               UINT16    packet_type
           );

#define BT_hci_change_connection_packet_type(p1, p2) \
        hci_write_command_UINT16_UINT16 \
        (HCI_CHANGE_CONNECTION_PACKET_TYPE_OPCODE, (p1), (p2))
/**
 *  \brief To retrieve the Name of the remote Bluetooth device.
 *
 *  \par Description:
 *       This API is used to get the Name of a remote Bluetooth device, as
 *       identified by the supplied Bluetooth Device Address.
 *       The local Bluetooth device starts the name retrieval process on
 *       reception of the HCI Remote Name Request Command, and sends a
 *       Command Status Event to the HCI. At the end this process,
 *       the Remote Name Request Complete Event is received. To retrieve the
 *       name, an ACL connection is required between the devices. A temporary
 *       ACL connection is made if it does not exist. Hence this API can fail,
 *       if the remote device does not have Page Scan enabled.
 *
 *  \param [in] bd_addr
 *         Bluetooth Device Address of the remote Bluetooth device.
 *
 *  \param [in] Page_scan_repetition_mode and page_scan_mode
 *         These two parameters specify the Page Scan modes supported by the
 *         remote Bluetooth device - this information must have been acquired
 *         during the Bluetooth Inquiry procedure.
 *
 *  \param [in] Clock_offset
 *         The clock offset between the local Baseband Controller and the
 *         remote one - this information must have been acquired during
 *         Bluetooth Inquiry procedure.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_remote_name_request
           (
               UCHAR *   bd_addr,
               UCHAR     page_scan_repetition_mode,
               UCHAR     page_scan_mode,
               UINT16    clock_offset
           );
/**
 *  \brief To retrieve supported features of a remote Bluetooth device.
 *
 *  \par Description:
 *       This API is used to retrieve supported features of a remote Bluetooth
 *       device, as identified by the supplied Bluetooth Device Address.
 *       The local Bluetooth device starts the feature request process on
 *       reception of the HCI Read Remote Supported Features Command, and
 *       sends a Command Status Event to the HCI. At the end this process,
 *       the Read Remote Supported Features Complete Event is received.
 *
 *  \param [in] connection_handle
 *         Connection Handle of the ACL Connection with the remote Bluetooth device.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_read_remote_supported_features
           (
               UINT16   connection_handle
           );

#ifdef HCI_READ_REMOTE_VERSION_INFORMATION_SUPPORT
/**
 *  \brief To retrieve version information of a remote Bluetooth device.
 *
 *  \par Description:
 *       This API is used to obtain the values for the version information
 *       of a remote Bluetooth device, identified by the connection_handle.
 *       The connection_handle must be a connection handle for an ACL or LE
 *       connection.
 *
 *       When the Controller receives the Read_Remote_Version_Information
 *       command, the Controller shall send the Command Status event to
 *       the Host. When the Link Manager or Link Layer has completed the
 *       sequence to determine the remote version information, the local
 *       Controller shall send a Read Remote Ver-sion Information Complete
 *       event to the Host. The Read Remote Version Information Complete event
 *       contains the status of this command, and parameters describing
 *       the version and subversion of the LMP or Link Layer used by
 *       the remote device.
 *
 *  \param [in] connection_handle
 *         Connection Handle of the ACL Connection with
 *         the remote Bluetooth device.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_read_remote_version_information(connection_handle) \
        hci_write_command_connection_handle \
        ( \
               HCI_READ_REMOTE_VERSION_INFORMATION_OPCODE, \
               (connection_handle) \
        )
#endif /* HCI_READ_REMOTE_VERSION_INFORMATION_SUPPORT */
/**
 *  \brief To retrieve clock offset of the local Bluetooth device with respect
 *         to the remote one.
 *
 *  \par Description:
 *       This API attempts to retrieve the clock offset of the local Bluetooth
 *       device with respect to the remote one, as identified by the supplied
 *       ACL Connection Handle.
 *       Both the System Clock and the Clock Offset to a remote Bluetooth device
 *       is used to determine the hopping frequency used by a remote device for
 *       page scan. This command allows the Host to read clock offset to remote
 *       devices. The clock offset can be used to speed up the paging procedure
 *       when the local device tries to establish a connection with a
 *       remote device, for example, when the local Host has issued
 *       Create Connection or Remote Name Request.
 *
 *  \param [in] connection_handle
 *         Connection Handle of the ACL Connection with the remote Bluetooth
 *         device with respect to which the clock offset needs to be determined.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_clock_offset(p1) \
        hci_write_command_UINT16 (HCI_READ_CLOCK_OFFSET_OPCODE, (p1))

#ifdef HCI_READ_LMP_HANDLE_SUPPORT
/**
 *  \brief To read the current LMP Handle associated with the
 *         Connection_Handle.
 *
 *  \par Description:
 *       This command reads the current LMP Handle associated with
 *       the Connection_Handle.
 *
 *  \param [in] connection_handle
 *              Connection_Handle
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_read_lmp_handle
           (
               UINT16 connection_handle
           );
#endif /* HCI_READ_LMP_HANDLE_SUPPORT */

#ifdef HCI_NO_ESCO_AUTO_ACCEPT
/**
 *  \brief
 *  \par Description:
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
void BT_hci_esco_connection_response
     (
         UCHAR *  bd_addr,
         UCHAR    reject
     );
/**
 *  \brief To accept a Synchronous Connection (eSCO).
 *
 *  \par Description:
 *       This API enables application, to accept an incoming eSCO Connection
 *       Request.
 *       By default, the EtherMind HCI module automatically accepts incoming
 *       eSCO connection request from the Bluetooth hardware.
 *       If feature flag 'HCI_NO_ESCO_AUTO_ACCEPT' is defined, application need
 *       to use BT_hci_accept_esco_connection_request() or
 *       BT_hci_reject_esco_connection_request() API to accept or reject the
 *       eSCO connection request.
 *
 *  \param [in] bd_addr
 *         This is the Bluetooth Device Address for the remote device.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_accept_esco_connection_request(addr) \
        BT_hci_esco_connection_response ((addr), 0x00U)
/**
 *  \brief To reject a Synchronous Connection (eSCO).
 *
 *  \par Description:
 *       This API enables application, to reject an incoming eSCO Connection
 *       Request.
 *       By default, the EtherMind HCI module automatically accepts incoming
 *       eSCO connection request from the Bluetooth hardware.
 *       If feature flag 'HCI_NO_ESCO_AUTO_ACCEPT' is defined, application need
 *       to use BT_hci_accept_esco_connection_request() or
 *       BT_hci_reject_esco_connection_request() API to accept or reject the
 *       eSCO connection request.
 *
 *  \param [in] bd_addr
 *         This is the Bluetooth Device Address for the remote device.
 *
 *  \param [in] reject
 *         This parameter specifies the reason for rejecting eSCO
 *         connection request.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_reject_esco_connection_request(addr, reason) \
        BT_hci_esco_connection_response ((addr), (reason))

#ifdef HCI_ENH_SCO
void BT_hci_enh_sco_connection_response
     (
         UCHAR *  bd_addr,
         UCHAR    reject
     );
/**
 *  \brief To accept an Enhanced Synchronous Connection (eSCO).
 *
 *  \par Description:
 *       This API enables application, to accept an incoming enhanced eSCO
 *       Connection Request.
 *
 *  \param [in] bd_addr
 *         This is the Bluetooth Device Address for the remote device.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_accept_enh_sco_connection_request(addr) \
        BT_hci_enh_sco_connection_response ((addr), 0x00U)
/**
 *  \brief To reject an Enhanced Synchronous Connection (eSCO).
 *
 *  \par Description:
 *       This API enables application, to reject an incoming eSCO
 *       Connection Request.
 *
 *  \param [in] bd_addr
 *         This is the Bluetooth Device Address for the remote device.
 *
 *  \param [in] reason
 *         This parameter specifies the reason for rejecting enhanced eSCO
 *         connection request.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_reject_enh_sco_connection_request(addr, reason) \
        BT_hci_enh_sco_connection_response ((addr), (reason))
#endif /* HCI_ENH_SCO */

#endif /* HCI_NO_ESCO_AUTO_ACCEPT */

#ifdef HCI_DISCONNECT_SUPPORT
/**
 *  \brief To disconnect an ACL/SCO/eSCO/BLE link.
 *
 *  \par Description:
 *       This API initiates termination of an existing ACL, SCO, eSCO or BLE
 *       connection with a remote Bluetooth device, as identified by
 *       the supplied Connection Handle.
 *       The Reason command parameter indicates the reason for ending
 *       the connection. The remote Controller will receive the Reason command
 *       parameter in the Disconnection Complete event. All synchronous
 *       connections on a physical link should be disconnected before the ACL
 *       connection on the same physical connection is disconnected.
 *
 *       When the Controller receives the Disconnect command, it shall send
 *       the Command Status event to the Host. The Disconnection Complete event
 *       will occur at each Host when the termination of the connection has
 *       completed, and indicates that this command has been completed.
 *
 *  \param [in] connection_handle
 *         Connection Handle for the existing ACL, SCO, eSCO or BLE connection
 *         to be disconnected.
 *
 *  \param [in] reason
 *         This specifies the reason for disconnection.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
API_RESULT BT_hci_disconnect
           (
               UINT16  connection_handle,
               UCHAR   reason
           );
#endif /* HCI_DISCONNECT_SUPPORT */

/* ------------------------------------------------------------------------- */

/* HCI Link Policy APIs ---------------------------------------------------- */
/**
 *  \brief To place an ACL Connection with a remote Bluetooth device to Hold mode.
 *
 *  \par Description:
 *       This API can be used to place an ACL connection between the local and
 *       remote Bluetooth device (identified by the ACL Connection Handle)
 *       into Hold mode.
 *
 *  \param [in] connection_handle
 *         The Connection Handle of the ACL connection with a remote Bluetooth
 *         device that needs to be placed into Hold mode.
 *
 *  \param [in] Hold_mode_max_interval
 *         This parameter specifies the maximum acceptable number of Baseband
 *         time slots to wait in Hold mode.
 *         Value Range = 0x0002 to 0xFFFE. Only even values are valid.
 *         Hold time = hold_mode_max_interval * 0.625 msec
 *         Time Range = 1.25 msec to 40.9 sec
 *         hold_mode_min_interval    This parameter specifies the minimum
 *         acceptable number of BaseBand time slots to wait in Hold mode.
 *         Value Range = 0x0002 to 0xFF00. Only even values are valid.
 *         Hold time = hold_mode_min_interval * 0.625 msec
 *         Time Range = 0.625 msec to 40.9 sec
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_hold_mode
           (
               UINT16  connection_handle,
               UINT16  hold_max_interval,
               UINT16  hold_min_interval
           );
/**
 *  \brief To place an ACL Connection with a remote Bluetooth device to
 *         Sniff mode.
 *
 *  \par Description:
 *       This API can be used to place an ACL connection between the local and
 *       remote Bluetooth device (identified by the ACL Connection Handle) into
 *       Sniff mode.
 *       The local Bluetooth device starts the Sniff mode process on reception
 *       of the HCI Sniff Mode Command, and sends a Command Status Event to the
 *       HCI. At the end of this process, the Mode Change Event is received.
 *
 *  \param [in] connection_handle
 *         The Connection Handle of the ACL connection with a remote Bluetooth
 *         device that needs to be placed into Sniff mode.
 *
 *  \param [in] Sniff_mode_max_interval
 *         This parameter specifies the maximum acceptable number of Baseband
 *         time slots to wait in Sniff mode.
 *         Value Range = 0x0002 to 0xFFFE. Only even values are valid
 *         Sniff time = sniff_mode_max_interval * 0.625 msec
 *         Time Range = 1.25 msec to 40.9 sec
 *
 *  \param [in] sniff_mode_min_interval
 *         This parameter specifies the maximum acceptable number of Baseband
 *         time slots to wait in Sniff mode.
 *         Value Range = 0x0002 to 0xFFFE. Only even values are valid
 *         Sniff time = sniff_mode_min_interval * 0.625 msec
 *         Time Range = 1.25 msec to 40.9 sec
 *         Note: sniff_mode_max_interval > sniff_mode_min_interval
 *
 *  \param [in] sniff_attempt
 *         This parameter specifies the number of Baseband receive slots for
 *         Sniff attempt.
 *         Value Range = 0x0001 to 0x7FFF.
 *         Time = (2 * sniff_attempt  - 1) * 0.625 msec
 *         Time Range = 0.625 msec to 40.9 sec
 *
 *  \param [in] sniff_timeout
 *         This parameter specifies the number of Baseband receive slots for
 *         Sniff timeout.
 *         Value Range = 0x0000 to 0x7FFF.
 *         If sniff_timeout > 0, Time = (2 * sniff_timeout  - 1) * 0.625 msec. Otherwise, Time = 0.
 *         Time Range = 0 msec to 40.9 sec
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_sniff_mode
           (
               UINT16  connection_handle,
               UINT16  sniff_max_interval,
               UINT16  sniff_min_interval,
               UINT16  sniff_attempt,
               UINT16  sniff_timeout
           );
/**
 *  \brief To end/cancel the Sniff mode.
 *
 *  \par Description:
 *       This API can be used to end the Sniff mode for an ACL Connection, which
 *       is currently in the Sniff mode.
 *       The local Bluetooth device starts the exit from the Sniff mode process
 *       on reception of the HCI Exit Sniff Mode Command, and sends a Command
 *       Status Event to the HCI. At the end of this process, the Mode Change
 *       Event is received.
 *
 *  \param [in] connection_handle
 *         The Connection Handle of the ACL connection with a remote Bluetooth
 *         device, for which the Sniff mode needs to be cancelled.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_exit_sniff_mode(p1) \
        hci_write_command_UINT16 (HCI_EXIT_SNIFF_MODE_OPCODE, (p1))

API_RESULT BT_hci_park_mode
           (
               UINT16  connection_handle,
               UINT16  beacon_max_interval,
               UINT16  beacon_min_interval
           );
/**
 *  \brief To end/cancel the Park mode & switch to Active mode.
 *
 *  \par Description:
 *       This API can be used to end the Park mode for an ACL Connection, which
 *       is currently in the Sniff mode.
 *       The local Bluetooth device starts the exit from the Park mode process
 *       on reception of the HCI Exit Park Mode Command, and sends a Command
 *       Status Event to the HCI. At the end of this process, the Mode Change
 *       Event is received.
 *
 *  \param [in] connection_handle
 *         The Connection Handle of the ACL connection with a remote Bluetooth
 *         device, for which Park mode to Active mode switch needs to happen.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_exit_park_mode
           (
               UINT16  connection_handle
           );
/**
 *  \brief To specify Quality of Service parameters for a connection.
 *
 *  \par Description:
 *       This API enables applications to specify the Quality of Service
 *       parameters for an ACL connection to a remote Bluetooth device.
 *       The local Bluetooth device starts the QoS Setup process to negotiate
 *       QoS parameters with the remote device on reception of the HCI QoS Setup
 *       Command, and sends a Command Status Event to the HCI. At the end of the
 *       QoS Setup process, the QoS Setup Complete Event is received.
 *
 *  \param [in] connection_handle
 *         Connection Handle of the ACL link for which QoS parameters to be specified.
 *
 *  \param [in] Qos_requested
 *         This parameter must be a pointer to the HCI_QOS data type containing
 *         the requested QoS parameters to be negotiated and specified for the
 *         ACL link with the remote Bluetooth device.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_qos_setup
           (
               UINT16       connection_handle,
               HCI_QOS *    qos_requested
           );
/**
 *  \brief To determine the role of a Bluetooth ACL connection.
 *
 *  \par Description:
 *       This API determines the role that the local Bluetooth device is
 *       performing for an ACL link with a remote Bluetooth device,
 *       as identified by the ACL Connection Handle.
 *
 *  \param [in] connection_handle
 *         The ACL Connection Handle for which role needs to be determined
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_role_discovery
           (
               UINT16   connection_handle
           );
/**
 *  \brief To switch Bluetooth role for the specified connection.
 *
 *  \par Description:
 *       This API can be used to switch the current role of the ACL Connection
 *       that exists in the local Bluetooth device with a remote Bluetooth device,
 *       as identified by the supplied Connection Handle.
 *       on reception of the HCI Switch Role Command, and sends a Command Status
 *       Event to the HCI. At the end of the Master-Slave Role Switch process,
 *       the Role Change Event is received.
 *
 *  \param [in] bd_addr
 *         The Bluetooth Device Address of the remote Bluetooth device.
 *
 *  \param [in] Role
 *         This parameter specifies new Bluetooth Role that the local Bluetooth
 *         device should assume for the ACL connection to the remote Bluetooth
 *         device. Valid values are:
 *         Value 0x00: Change own role to Master.
 *         Value 0x01: Change own role to Slave.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_switch_role
           (
               UCHAR *  bd_addr,
               UCHAR    role
           );
/**
 *  \brief To read the current Link Policy Settings of the local Bluetooth device.
 *
 *  \par Description:
 *       This API reads the Link Policy Setting configuration parameter from the
 *       local Bluetooth device for a specified ACL Connection Handle.
 *       The Link Policy Settings parameter determines the behavior of the local
 *       Bluetooth device when it receives a request from a remote Bluetooth
 *       device to change the Master-Slave role or to enter the Hold, Sniff,
 *       or Park mode. The local Bluetooth device will automatically accept or
 *       reject such a request from the remote device based on the value of the
 *       Link Policy Settings parameter for the corresponding Connection Handle.
 *
 *
 *  \param [in] connection_handle
 *         The Connection Handle of the ACL connection with a remote Bluetooth
 *         device, for which the Link Policy Settings to be read.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_read_link_policy_settings
           (
               UINT16    connection_handle
           );
/**
 *  \brief To write the Link Policy Settings configuration parameter to the local Bluetooth device.
 *
 *  \par Description:
 *       This API writes the Link Policy Setting configuration parameter to the
 *       local Bluetooth device for a specified ACL Connection Handle.
 *       The Link Policy Settings parameter determines the behavior of the
 *       local Bluetooth device when it receives a request from a remote
 *       Bluetooth device to change the Master-Slave role or to enter the
 *       Hold, Sniff, or Park mode. The local Bluetooth device will
 *       automatically accept or reject such a request from the remote device
 *       based on the value of the Link Policy Settings parameter for the
 *       corresponding Connection Handle
 *
 *  \param [in] connection_handle
 *         The Connection Handle of the ACL connection with a remote Bluetooth
 *         device, for which the Link Policy Settings to be written.
 *
 *  \param [in] Link_policy_settings
 *         The Link Policy Settings to be set.
 *         Value 0x0000: Disable all LM modes. (Default)
 *         Value 0x0001: Enable Master-Slave switch.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_write_link_policy_settings(p1, p2) \
        hci_write_command_UINT16_UINT16 \
        (HCI_WRITE_LINK_POLICY_SETTINGS_OPCODE, (p1), (p2))

/* ------------------------------------------------------------------------- */

/* HCI Host Controller & Baseband APIs ------------------------------------- */

#ifdef HCI_SET_EVENT_MASK_SUPPORT
/**
 *  \brief To set the HCI Event Filter at the local Bluetooth device.
 *
 *  \par Description:
 *       This API is used to control which events are generated by the HCI for
 *       the Host. If the bit in the Event_Mask is set to a one, then the
 *       event associated with that bit will be enabled. For an LE Controller,
 *       the "LE Meta Event" bit in the Event_Mask shall enable or disable all
 *       LE events in the LE Meta Event.
 *       The Host has to deal with each event that occurs. The event mask
 *       allows the Host to control how much it is interrupted.
 *
 *  \param [in] event_mask
 *         It is an 8 octet value. Refer to the Bluetooth HCI specification
 *         for the definition of the masks for each event.
 *         For LE Meta-Event, mask 0x2000000000000000 shall be set.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_set_event_mask(event_mask) \
        hci_write_command_UCHAR_array \
        (HCI_SET_EVENT_MASK_OPCODE, (event_mask), 8)
#endif /* HCI_SET_EVENT_MASK_SUPPORT */

#ifdef HCI_SET_EVENT_MASK_PAGE_2_SUPPORT
/**
 *  \brief To set the HCI Event Filter at the local Bluetooth device.
 *
 *  \par Description:
 *       This API is used to control which events are generated by the HCI for
 *       the Host. If the bit in the Event_Mask is set to a one, then the
 *       event associated with that bit will be enabled. For an LE Controller,
 *       the "LE Meta Event" bit in the Event_Mask shall enable or disable all
 *       LE events in the LE Meta Event.
 *       The Host has to deal with each event that occurs. The event mask
 *       allows the Host to control how much it is interrupted.
 *
 *  \param [in] event_mask
 *         It is an 8 octet value. Refer to the Bluetooth HCI specification
 *         for the definition of the masks for each event.
 *         For LE Meta-Event, mask 0x2000000000000000 shall be set.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_set_event_mask_page_2(event_mask) \
        hci_write_command_UCHAR_array \
        (HCI_SET_EVENT_MASK_PAGE_2_OPCODE, (event_mask), 8U)
#endif /* HCI_SET_EVENT_MASK_PAGE_2_SUPPORT */

#ifdef HCI_RESET_SUPPORT
/**
 *  \brief To reset the local Bluetooth controller.
 *
 *  \par Description:
 *       The Reset command will reset the Controller and the Link Manager
 *       on the BR/EDR Controller, the PAL on an AMP Controller,
 *       or the Link Layer on an LE Controller.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_reset() \
        hci_common_api_handler_no_params (HCI_RESET_OPCODE)
#endif /* HCI_RESET_SUPPORT */
/**
 *  \brief To set the HCI Event Filter at the Bluetooth device
 *
 *  \par Description:
 *       This API sets the HCI Event filters at the local Bluetooth device.
 *       For details on the Event Filters, refer to the Specification of the
 *       Bluetooth System, v1.2, Vol. 2, Part E - Host Controller Interface
 *       Functional Specification, Section 7.3.3.
 *
 *  \param [in] filter_type
 *         Information about the kind of filter to be set at the Bluetooth device.
 *
 *  \param [in] filter_condition_type
 *         Type of condition to be set for the filter.
 *
 *  \param [in] condition
 *         Condition of filtering to be set at the Bluetooth device.
 *
 *  \param [in] condition_length
 *         Length of condition to be set at Bluetooth device.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 *
 *  \note For details on the these parameter values, refer to the Specification of
 *        the Bluetooth System, v1.2, Vol. 2, Part E - Host Controller Interface
 *        Functional Specification, Section 7.3.3.
 */
API_RESULT BT_hci_set_event_filter
           (
               UCHAR    filter_type,
               UCHAR    filter_condition_type,
               UCHAR *  condition,
               UINT16   condition_length
           );
/**
 *  \brief To flush all pending ACL data from the Bluetooth device for the
 *         specified ACL Connection Handle.
 *
 *  \par Description:
 *       This API can be used to discard all data that is currently pending for
 *       transmission in the local Bluetooth device for the specified Connection
 *       Handle, even if there currently are fragments of data that belong to
 *       more than one L2CAP packet in the Host Controller.
 *       After this API has successfully executed, the local Bluetooth device
 *       will discard all data that is sent to the Controller for the same
 *       connection handle until an HCI Data Packet with the "Start" Packet
 *       Boundary Flag (0x02) is received. When this happens, a new transmission
 *       attempt can be made.
 *       This API allows Applications to control how long the Baseband should
 *       try to retransmit a baseband packet for a connection handle before all
 *       data that is currently pending for transmission in the local Bluetooth
 *       device should be flushed.
 *
 *  \param [in] connection_handle
 *         Connection Handle of the ACL connection with a remote Bluetooth device,
 *         data for which needs to be flushed.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 *
 *  \note This API is available for ACL packets only, and a valid ACL Connection
 *        Handle must be specified.
 *        In addition to HCI Flush command, the applications can make use of
 *        Automatic Flush Timeout in the Bluetooth device to automatically
 *        flush data after a timeout.
 *
 */
API_RESULT BT_hci_flush
           (
               UINT16  connection_handle
           );

#ifndef HCI_LITE
/**
 *  \brief To create a new unit key by the Bluetooth device.
 *
 *  \par Description:
 *       This API can be used to create a new unit key.
 *       The Bluetooth hardware will generate a random seed that will be used to
 *       generate the new unit key. All new connection will use the new unit key,
 *       but the old unit key will still be used for all current connections.
 *       This API will not have any effect for a device that does not use unit
 *       keys (i.e. a device which uses only combination keys).
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_create_new_unit_key() \
        hci_common_api_handler_no_params (HCI_CREATE_NEW_UNIT_KEY_OPCODE)
#endif /* HCI_LITE */

#ifdef HCI_SUPPORT_STORED_LINK_KEY_COMMANDS
/**
 *  \brief
 *
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_stored_link_key(bd_addr, read_all_flag) \
        hci_write_command_BD_ADDR_UCHAR( \
            HCI_READ_STORED_LINK_KEY_OPCODE, \
            (bd_addr), \
            (read_all_flag))
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_stored_link_key
           (
               UCHAR            num_keys_to_write,
               HCI_H_LINK_KEY * link_keys
           );
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_delete_stored_link_key(bd_addr, delete_all_flag) \
        hci_write_command_BD_ADDR_UCHAR( \
            HCI_DELETE_STORED_LINK_KEY_OPCODE, \
            (bd_addr), \
            (delete_all_flag))

#endif /* HCI_SUPPORT_STORED_LINK_KEY_COMMANDS */
/**
 *  \brief To set the local name of the device.
 *  \par Description: This API configures the name of the local Bluetooth device.
 *
 *  \param [in] name
 *         Caller allocated CHAR array of size 248 bytes, which will be used to
 *         copy the name of the remote Bluetooth device.
 *
 *  \param [in] Name_length
 *         Length of the Name to be set. This must be less than or equal to
 *         248 bytes.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 *
 *  \note On embedded version of the EtherMind stack, the size of the name is
 *        limited to 12 due to memory constraints.
 *        The BT_hci_write_local_name() behaves exactly the same way as
 *        BT_hci_change_local_name(), but it is available only in Bluetooth-1.2.
 */
API_RESULT BT_hci_change_local_name
           (
               UCHAR *  name,
               UINT16   name_length
           );

#ifndef HCI_LITE
/**
 *  \brief To read the name of the local Bluetooth device
 *
 *  \par Description:
 *       This API reads the name set in the local Bluetooth device.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_local_name() \
        hci_common_api_handler_no_params (HCI_READ_LOCAL_NAME_OPCODE)
#endif /* HCI_LITE */

#ifndef HCI_LITE
/**
 *  \brief To read the value of Connection Accept Timeout configuration parameter.
 *
 *  \par Description:
 *       This API reads the value of Connection Accept Timeout configuration
 *       parameter from the local Bluetooth device.
 *       The Connection Accept Timeout parameter allows the local Bluetooth
 *       device to automatically deny a connection request after a specified
 *       time period has occurred and the Host (EtherMind Stack and Application)
 *       has not accepted the new connection request. The parameter defines the
 *       time duration from when the Bluetooth device sends a Connection Request
 *       Event to the Host Stack until the local Bluetooth device automatically
 *       rejects an incoming connection.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_connection_accept_timeout() \
        hci_common_api_handler_no_params \
        (HCI_READ_CONNECTION_ACCEPT_TIMEOUT_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To write the value of Connection Accept Timeout configuration parameter.
 *
 *  \par Description:
 *       This API writes the value of Connection Accept Timeout configuration
 *       parameter to the local Bluetooth device.
 *       The Connection Accept Timeout parameter allows the local Bluetooth
 *       device to automatically deny a connection request after a specified
 *       time period has occurred and the Host (EtherMind Stack and Application)
 *       has not accepted the new connection request. The parameter defines the
 *       time duration from when the Bluetooth device sends a Connection Request
 *       Event to the Host Stack until the local Bluetooth device automatically
 *       rejects an incoming connection.
 *
 *
 *  \param [in] timeout
 *         The Connection Accept Timeout measured in number of Baseband slots.
 *         Value Range = 0x0001 - 0xB540
 *         Time Range (conn_accept_timeout * 0.625 msec)  = 0.625 msec to 29 sec
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_connection_accept_timeout
           (
               UINT16  timeout
           );

#ifndef HCI_LITE
/**
 *  \brief To read the value of Page Timeout configuration parameter.
 *
 *  \par Description:
 *       This API reads the value of the Page Timeout configuration
 *       parameter set in the local Bluetooth device.
 *       The Page Timeout configuration parameter defines the maximum amount of
 *       time the local Bluetooth device will wait for a response from a remote
 *       Bluetooth device for a locally initiated connection.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_page_timeout() \
        hci_common_api_handler_no_params (HCI_READ_PAGE_TIMEOUT_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To set the Page Timeout configuration parameter value in the local
 *         Bluetooth device.
 *
 *  \par Description:
 *       This API writes the value of the Page Timeout configuration parameter
 *       to the local Bluetooth device.
 *       The Page Timeout configuration parameter defines the maximum amount of
 *       time the local Bluetooth device will wait for a response from a remote
 *       Bluetooth device for a locally initiated connection.
 *
 *
 *  \param [in] page_timeout
 *         The value of Page Timeout to be set. The actual timeout is calculated
 *         as follows: Time = page_timeout * 0.625 msec; Value Range: 0x0001 - 0xFFFF.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_write_page_timeout(p1) \
        hci_write_command_UINT16 (HCI_WRITE_PAGE_TIMEOUT_OPCODE, (p1))

#ifndef HCI_LITE
/**
 *  \brief To read the Scan Mode settings of the local Bluetooth device.
 *  \par Description:
 *       This API reads the value of Scan Enable configuration parameter in the
 *       Baseband Controller.
 *       The Scan Enable defines whether the local Bluetooth device will
 *       periodically scan for responding to Paging attempts and/or Inquiry
 *       attempts from other remote Bluetooth devices or not.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_scan_enable() \
        hci_common_api_handler_no_params (HCI_READ_SCAN_ENABLE_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To write the Scan Mode settings in the local Bluetooth device.
 *
 *  \par Description:
 *       This API writes the value of Scan Enable configuration parameter in the
 *       local Bluetooth device.
 *       The Scan Enable defines whether the local Bluetooth device will
 *       periodically scan for responding to Paging attempts and/or Inquiry
 *       attempts from other remote Bluetooth devices or not.
 *
 *  \param [in] scan_enable
 *         Value of the Scan Enable parameter to be set in the local
 *         Bluetooth device.
 *         The valid values and their meanings are shown below:
 *         0x00: No Scans enabled (Default).
 *         0x01: Inquiry Scan enabled, Page Scan disabled.
 *         0x02: Inquiry Scan disabled, Page Scan enabled.
 *         0x03: Inquiry Scan enabled, Page Scan enabled.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_write_scan_enable(p1) \
        hci_write_command_UCHAR (HCI_WRITE_SCAN_ENABLE_OPCODE, (p1))

#ifndef HCI_LITE
/**
 *  \brief To read the Page Scan Activity configuration variable from the local
 *         Bluetooth device.
 *
 *  \par Description:
 *       This API reads the value for Page Scan Activity configuration
 *       parameters from the Baseband Controller.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_page_scan_activity() \
        hci_common_api_handler_no_params (HCI_READ_PAGE_SCAN_ACTIVITY_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To write the Page Scan Activity configuration parameters to the
 *         local Bluetooth device.
 *
 *  \par Description:
 *       This API writes the value for Page Scan Activity to the local Bluetooth
 *       device. The Page Scan Interval configuration parameter defines the amount
 *       of time between consecutive page scans. This is defined as the time
 *       intervals from when the local Bluetooth device started its last
 *       page scan until it begins the next page scan.
 *       The Page Scan Window configuration parameter defines the amount of
 *       time for the duration of the page scan.
 *       The Page Scan Window must be less than or equal to the Page Scan Interval.
 *
 *  \param [in] page_scan_interval
 *         The value of Page Scan Interval to be set.
 *         Value Range: 0x0012 - 0x1000, only even values are valid.
 *         Time Range: page_scan_interval * 0.625 msec = 11.25 msec to 2560 msec
 *
 *  \param [in] page_scan_window
 *         The value of Page Scan Window to be set.
 *         Value Range: 0x0011 - 0x1000
 *         Time Range: page_scan_window * 0.625 msec = 10.625 msec to 2560 msec
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_write_page_scan_activity(p1, p2) \
        hci_write_command_UINT16_UINT16 \
        (HCI_WRITE_PAGE_SCAN_ACTIVITY_OPCODE, (p1), (p2))

#ifndef HCI_LITE
/**
 *  \brief To read the Inquiry Scan Activity configuration variable from the local Bluetooth device.
 *  \par Description:
 *       This API reads the value for Inquiry Scan Activity configuration
 *       parameters from the Baseband Controller.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_inquiry_scan_activity() \
        hci_common_api_handler_no_params \
        (HCI_READ_INQUIRY_SCAN_ACTIVITY_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To write the Inquiry Scan Activity configuration parameters to the
 *         local Bluetooth device.
 *
 *  \par Description:
 *       This API writes the value for Inquiry Scan Activity to the local
 *       Bluetooth device. The Inquiry Scan Interval configuration parameter
 *       defines the amount of time between consecutive inquiry scans. This is
 *       defined as the time intervals from when the local Bluetooth device
 *       started its last inquiry scan until it begins the next inquiry scan.
 *       The Inquiry Scan Window configuration parameter defines the amount of
 *       time for the duration of the inquiry scan.
 *       The Inquiry Scan Window must be less than or equal to the
 *       Inquiry Scan Interval.
 *
 *  \param [in] inquiry_scan_interval
 *         The value of Inquiry Scan Interval to be set.
 *         Value Range: 0x0012 - 0x1000, only even values are valid.
 *         Time Range: inquiry_scan_interval * 0.625 msec = 11.25 msec to 2560 msec
 *
 *  \param [in] inquiry_scan_window
 *         The value of Inquiry Scan Window to be set.
 *         Value Range: 0x0011 - 0x1000
 *         Time Range: inquiry_scan_window * 0.625 msec = 10.625 msec to 2560 msec
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_write_inquiry_scan_activity(p1, p2) \
        hci_write_command_UINT16_UINT16 \
        (HCI_WRITE_INQUIRY_SCAN_ACTIVITY_OPCODE, (p1), (p2))

#ifndef HCI_LITE
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_authentication_enable() \
        hci_common_api_handler_no_params (HCI_READ_AUTHENTICATION_ENABLE_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_write_authentication_enable(p1) \
        hci_write_command_UCHAR (HCI_WRITE_AUTHENTICATION_ENABLE_OPCODE, (p1))

#ifndef HCI_LITE
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_encryption_mode() \
        hci_common_api_handler_no_params (HCI_READ_ENCRYPTION_MODE_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_write_encryption_mode(p1) \
        hci_write_command_UCHAR (HCI_WRITE_ENCRYPTION_MODE_OPCODE, (p1))

#ifndef HCI_LITE
/**
 *  \brief To read the Class of Device configuration parameter from the local
 *         Bluetooth device.
 *
 *  \par Description:
 *       This API reads the Class of Device configuration parameter from the
 *       local Bluetooth device.
 *       The Class of Device is used to indicate the capabilities of the local
 *       Bluetooth device to the other remote Bluetooth devices when at the time
 *       of their inquiry processes.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_class_of_device() \
        hci_common_api_handler_no_params (HCI_READ_CLASS_OF_DEVICE_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To set the Class of Device in the local Bluetooth device
 *
 *  \par Description:
 *       This API writes the Class of Device configuration parameter to the
 *       local Bluetooth device.
 *       The Class of Device is used to indicate the capabilities of the local
 *       Bluetooth device to the other remote Bluetooth devices when at the
 *       time of their inquiry processes.
 *
 *  \param [in] class_of_device
 *         Value of the Class of Device for the local device. Though represented
 *         in 4-byte UINT32 type, Class of Device is a 3 Byte value. Population
 *         of Class of Device should be according to Bluetooth Assigned Numbers
 *         document - http://www.bluetooth.org/assigned-numbers/.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_class_of_device
           (
               UINT32  class_of_device
           );

#ifndef HCI_LITE
/**
 *  \brief To read the value of the Voice Setting configuration parameter.
 *
 *  \par Description:
 *       This API reads the Voice Setting configuration parameter from the local
 *       Bluetooth device.
 *       For details on the Voice Setting configuration parameter, refer to the
 *       Specification of the Bluetooth System, v1.2, Vol. 2,
 *       Part E - Host Controller Interface Functional Specification, Section 6.12.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_voice_setting() \
        hci_common_api_handler_no_params (HCI_READ_VOICE_SETTING_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To set the Voice Setting configuration parameter in the local
 *         Bluetooth device.
 *
 *  \par Description:
 *       This API writes the Voice Setting configuration parameter in the local
 *       Bluetooth device.
 *       For details on the Voice Setting configuration parameter,
 *       refer to the Specification of the Bluetooth System, v1.2, Vol. 2,
 *       Part E - Host Controller Interface Functional Specification, Section 6.12.
 *
 *  \param [in] voice_setting
 *         The value of Voice Setting parameter to be set.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_voice_setting
           (
               UINT16  voice_setting
           );
/**
 *  \brief To read the value of the Flush Timeout configuration parameter from
 *         the local Bluetooth device.
 *
 *  \par Description:
 *       This API reads the Flush Timeout configuration parameter from the local
 *       Bluetooth device.
 *       The Flush Timeout parameter defines the amount of time before all
 *       fragments of the L2CAP packet, of which a Baseband packet is currently
 *       being transmitted, are automatically flushed by the local Bluetooth
 *       device. The timeout period starts when a transmission attempt is made
 *       for the first Baseband packet of an L2CAP packet. This allows ACL
 *       packets to be automatically flushed without the Host device issuing a
 *       \Flush command, using the BT_hci_flush() API. This provides support
 *       for isochronous data, such as audio. When the L2CAP packet that is
 *       currently being transmitted is automatically 'flushed', the Failed
 *       Contact Counter is incremented by one.
 *
 *  \param [in] connection_handle
 *         The ACL Connection Handle for the connection with the remote
 *         Bluetooth device for which the automatic flush timeout to be read.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_read_automatic_flush_timeout
           (
               UINT16    handle
           );
/**
 *  \brief To read the value of the Flush Timeout configuration parameter from the local Bluetooth device.
 *
 *  \par Description:
 *       This API reads the Flush Timeout configuration parameter from the local
 *       Bluetooth device.
 *       The Flush Timeout parameter defines the amount of time before all
 *       fragments of the L2CAP packet, of which a Baseband packet is currently
 *       being transmitted, are automatically flushed by the local Bluetooth
 *       device. The timeout period starts when a transmission attempt is made
 *       for the first Baseband packet of an L2CAP packet. This allows
 *       ACL packets to be automatically flushed without the Host device issuing
 *       a Flush command, using the BT_hci_flush() API. This provides support
 *       for isochronous data, such as audio. When the L2CAP packet that is
 *       currently being transmitted is automatically 'flushed', the Failed
 *       Contact Counter is incremented by one.
 *
 *  \param [in] connection_handle
 *         The ACL Connection Handle for the connection with the remote
 *         Bluetooth device for which the automatic flush timeout to be read.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_automatic_flush_timeout
           (
               UINT16  handle,
               UINT16  timeout
           );

#ifdef HCI_READ_LE_HOST_SUPPORT
/**
 *  \brief To read LE host suppport.
 *
 *  \par Description:
 *       This API is used to read the LE Supported (Host) and
 *       Simultaneous LE and BR/EDR to Same Device Capable (Host) Link
 *       Manager Protocol feature bits.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_read_le_host_support() \
        hci_common_api_handler_no_params \
        (HCI_READ_LE_HOST_SUPPORT_OPCODE)
#endif /* HCI_WRITE_LE_HOST_SUPPORT */

#ifdef HCI_WRITE_LE_HOST_SUPPORT
/**
 *  \brief To write LE host suppport.
 *
 *  \par Description:
 *       This API is used to set the LE Supported (Host) and Simultaneous LE
 *       and BR/EDR to Same Device Capable (Host) Link Manager Protocol feature bits.
 *       These Link Manager Protocol feature bits are used by a remote Host.
 *
 *       The default value for these feature bits shall be disabled. When
 *       LE_Supported_Host is set to enabled the bit in LMP features mask indicating
 *       support for LE Support (Host) shall be set. The Simultaneous_LE_Host
 *       parameter shall be set to disabled.
 *
 *  \param [in] le_supported_host
 *         LE_Supported_Host parameter.
 *
 *  \param [in] simultaneous_le_host
 *         Simultaneous_LE_Host parameter.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_write_le_host_support(le_supported_host, simultaneous_le_host) \
        hci_write_command_UCHAR_UCHAR(HCI_WRITE_LE_HOST_SUPPORT_OPCODE, (le_supported_host), (simultaneous_le_host))
#endif /* HCI_WRITE_LE_HOST_SUPPORT */

#ifndef HCI_LITE
/**
 *  \brief To read the Number of Broadcast Retransmissions parameter from the
 *         local Bluetooth device
 *
 *  \par Description:
 *       This API reads the value of the Number of Broadcast Retransmissions from the local
 *       Bluetooth device.
 *       The Number of Broadcast Retransmissions parameter is used to increase
 *       the reliability of a broadcast message by retransmitting the broadcast
 *       message multiple times. This parameter defines the number of times the
 *       device will retransmit a broadcast data packet. This parameter should
 *       be adjusted as the link quality measurement changes.
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_num_broadcast_retransmissions() \
        hci_common_api_handler_no_params \
        (HCI_READ_NUM_BROADCAST_RETRANSMISSIONS_OPCODE)
#endif /* HCI_LITE */

/**
 *  \brief To set the Number of Broadcast Retransmissions parameter to the local
 *         Bluetooth device.
 *
 *  \par Description:
 *       This API writes the value of the Number of Broadcast Retransmissions to
 *       the local Bluetooth device.
 *       The Number of Broadcast Retransmissions parameter is used to increase
 *       the reliability of a broadcast message by retransmitting the broadcast
 *       message multiple times. This parameter defines the number of times the
 *       device will retransmit a broadcast data packet. This parameter should
 *       be adjusted as the link quality measurement changes.
 *
 *  \param [in] num_broadcast_rx
 *         The value of the Number of Broadcast Retransmissions to be set.
 *         Range = 0x00 - 0xFE.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_num_broadcast_retransmissions
           (
               UCHAR  num_broadcast_rx
           );

#ifndef HCI_LITE
/**
 *  \brief To read the Hold Mode Activity configuration parameter of the local
 *         Bluetooth device.
 *
 *  \par Description:
 *       This API reads the Hold Mode Activity configuration parameter from the
 *       local Bluetooth device.
 *       The Hold Mode Activity value is used to determine what activities
 *       should be suspended when the device is in hold mode.
 *       After the Hold period has expired, the device will return to the
 *       previous mode of operation.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_hold_mode_activity() \
        hci_common_api_handler_no_params (HCI_READ_HOLD_MODE_ACTIVITY_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To write the Hold Mode Activity configuration parameter in the local
 *         Bluetooth device.
 *
 *  \par Description:
 *       This API writes the Hold Mode Activity configuration parameter in the
 *       local Bluetooth device.
 *       The Hold Mode Activity value is used to determine what activities
 *       should be suspended when the device is in hold mode. After the hold
 *       period has expired, the device will return to the previous mode of
 *       operation.
 *       Multiple hold mode activities may be specified for the Hold Mode
 *       Activity parameter by performing a bitwise OR operation of the
 *       different activity types. If no activities are suspended, then all
 *       of the current Periodic Inquiry, Inquiry Scan, and Page Scan settings
 *       remain valid during the Hold Mode. If the Hold Mode Activity parameter
 *       is set to Suspend Page Scan, Suspend Inquiry Scan, and Suspend Periodic
 *       Inquiries, then the device can enter a low-power state during the Hold
 *       Mode period, and all activities are suspended. Suspending multiple
 *       activities can be specified for the Hold Mode Activity parameter by
 *       performing a bitwise OR operation of the different activity types.
 *       The Hold Mode Activity is only valid if all connections are in Hold Mode.
 *
 *  \param [in] hold_mode_activity
 *         Value of the Hold Mode Activity parameter to be set in the
 *         Baseband Controller .
 *         The valid values and their meanings are shown below:
 *         0x00: Maintain current power state.
 *         0x01: Suspend Page Scan.
 *         0x02: Suspend Inquiry Scan.
 *         0x04: Suspend Periodic Inquiries.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_hold_mode_activity
           (
               UCHAR  hold_mode_activity
           );

#ifndef HCI_LITE
/**
 *  \brief To read the SCO Flow Control settings from the local Bluetooth device.
 *
 *  \par Description:
 *      This API reads the value of SCO/eSCO Flow Control Enable configuration parameter from the local Bluetooth device.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_sco_flow_control_enable() \
        hci_common_api_handler_no_params \
        (HCI_READ_SCO_FLOW_CONTROL_ENABLE_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To write the SCO Flow Control settings in the local Bluetooth device.
 *
 *  \par Description:
 *       This API writes the value of SCO/eSCO Flow Control setting in the
 *       local Bluetooth device.
 *
 *  \param [in] flow_enable
 *         Value of the SCO Flow Control setting to be set in the local
 *         Bluetooth device.
 *         Value 0x00: SCO Flow Control Disabled.
 *         Value 0x01: SCO Flow Control Enabled.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_sco_flow_control_enable
           (
               UCHAR  flow_enable
           );

#ifdef HCI_SET_HOST_CONTROLLER_TO_HOST_FLOW_CONTROL_SUPPORT
/**
 *  \brief To set the Flow Control setting in the direction from the Bluetooth
 *         device to the Host.
 *
 *  \par Description:
 *       This API can be used to turn flow control on or off for ACL or
 *       SCO/eSCO data sent in the direction from the Bluetooth device to
 *       the Host Stack.
 *       If flow control is turned off, the Host will not send the Host Number
 *       of Completed Packets HCI command. The Bluetooth device will ignore
 *       that command if the Host sends it and flow control is off.
 *       If flow control is turned on for HCI ACL Data Packets and off for HCI
 *       SCO Data Packets, Host Number of Completed Packets commands sent by the
 *       Host should only contain Connection Handles for ACL connections.
 *       If flow control is turned off for HCI ACL Data Packets and on for HCI
 *       SCO Data Packets, Host Number of Completed Packets commands sent by the
 *       Host should only contain Connection Handles for SCO connections.
 *       If flow control is turned on for HCI ACL Data Packets and HCI SCO Data
 *       Packets, the Host will send Host Number Of Completed Packets commands
 *       both for ACL connections and SCO connections.
 *       This Flow Control Enable setting must only be attempted if no
 *       connections to other remote Bluetooth devices exist.
 *
 *  \param [in] flow_enable
 *         Value of the Flow Control setting to be set in the Bluetooth device.
 *         Value 0x00: Flow Control OFF for both ACL and SCO packets.
 *         Value 0x01: Flow Control ON for ACL, OFF for SCO packets.
 *         Value 0x02: Flow Control OFF for ACL, ON for SCO packets.
 *         Value 0x03: Flow Control ON for both ACL and SCO packets.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
API_RESULT BT_hci_set_host_controller_to_host_flow_control
           (
               UCHAR  flow_enable
           );
#endif /* HCI_SET_HOST_CONTROLLER_TO_HOST_FLOW_CONTROL_SUPPORT */


#ifdef HCI_HOST_CONTROLLER_FLOW_ON
#ifdef HCI_HOST_BUFFER_SIZE_SUPPORT
/**
 *  \brief To notify the Controller about maximum size and number of ACL
 *         and SCO data packets.
 *
 *  \par Description:
 *       This API is to notify the Controller about the maximum size of
 *       the data portion of HCI ACL and Synchronous Data packets sent
 *       from the Controller to the Host. The HCI_Host_Buffer_Size command
 *       also notifies the Controller about the total number of HCI ACL and
 *       Synchronous Data packets that can be stored in the data buffers of
 *       the Host.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_host_buffer_size(void);
#endif /* HCI_HOST_BUFFER_SIZE_SUPPORT */
#endif /* HCI_HOST_CONTROLLER_FLOW_ON */
/**
 *  \brief To read the value of the Link Supervision Timeout configuration
 *         parameter from the local Bluetooth device.
 *
 *  \par Description:
 *       This API reads the Link Supervision Timeout configuration parameter
 *       from the local Bluetooth device.
 *       The Master or Slave Bluetooth device uses the Link Supervision Timeout
 *       configuration parameter to monitor link loss. If, for any reason,
 *       no Baseband packets are received from a Connection Handle for duration
 *       longer than the Link Supervision Timeout, the connection is disconnected.
 *       The same timeout value is used for both Synchronous (SCO/eSCO) and
 *       ACL connections for the device specified by the Connection Handle.
 *
 *  \param [in] handle
 *         The ACL Connection Handle for the connection with the remote
 *         Bluetooth device for which the Link Supervision Timeout to be read.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_read_link_supervision_timeout
           (
               UINT16    handle
           );
/**
 *  \brief To write the value of the Link Supervision Timeout configuration
 *         parameter in the local Bluetooth device.
 *
 *  \par Description:
 *       This API writes the value of the Link Supervision Timeout configuration
 *       parameter in the local Bluetooth device.
 *       The Master or Slave Bluetooth device uses the Link Supervision Timeout
 *       configuration parameter to monitor link loss. If, for any reason,
 *       no Baseband packets are received from a Connection Handle for duration
 *       longer than the Link Supervision Timeout, the connection is disconnected.
 *       The same timeout value is used for both Synchronous (SCO/eSCO) and ACL
 *       connections for the device specified by the Connection Handle.
 *
 *  \param [in] connection_handle
 *         The ACL Connection Handle for the connection with the remote
 *         Bluetooth device for which the link supervision timeout to be set.
 *
 *  \param [in] Link_supervision_timeout
 *         The value of Link Supervision Timeout parameter to be set.
 *         Value Range = 0x0001 - 0x07FF. A value of 0x0000 means
 *         No Link Supervision Timeout at all.
 *         Actual Time = link_supervision_timeout * 0.625 msec.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 *
 *  \note Setting the Link Supervision Timeout to 0x0000 will disable the Link
 *        Supervision Timeout check for the specified Connection Handle.
 *        This makes it unnecessary for the Bluetooth Master of the Piconet to
 *        unpark and then park each Bluetooth Device every ~40 seconds. By using
 *        a value of 0x0000 for the Link Supervision Timeout setting,
 *        the scalability of the Park state is not limited.
 */
#define BT_hci_write_link_supervision_timeout(p1, p2) \
        hci_write_command_UINT16_UINT16 \
        (HCI_WRITE_LINK_SUPERVISION_TIMEOUT_OPCODE, (p1), (p2))

#ifndef HCI_LITE
/**
 *  \brief To read the number of Inquiry Access Codes the local Bluetooth device
 *         can listen on.
 *
 *  \par Description:
 *       This API reads the value for the Number of Inquiry Access Codes (IAC)
 *       that the local Bluetooth device can simultaneous listen for during an
 *       Inquiry Scan performed by other Bluetooth devices.
 *       All Bluetooth devices should support at least one IAC,
 *       the General Inquiry Access Code (the GIAC).
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_number_of_supported_iac() \
        hci_common_api_handler_no_params \
        (HCI_READ_NUMBER_OF_SUPPORTED_IAC_OPCODE)
/**
 *  \brief To read the current Inquiry Access Code LAP(s) stored in the local
 *         Bluetooth device
 *
 *  \par Description:
 *       This API reads the currently configured values for the Inquiry
 *       Access Code LAP(s) the local Bluetooth device can simultaneous
 *       listen for during an Inquiry Scan performed by other Bluetooth devices.
 *       All Bluetooth devices should support at least one IAC,
 *       the General Inquiry Access Code (GIAC).
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_current_iac_lap() \
        hci_common_api_handler_no_params (HCI_READ_CURRENT_IAC_LAP_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To write the current Inquiry Access Code LAP(s) in the local
 *         Bluetooth device
 *
 *  \par Description:
 *       This API writes the current values for the Inquiry Access Code LAP(s),
 *       to be used by the local Bluetooth device to simultaneous listen for
 *       during an Inquiry Scan performed by other Bluetooth devices.
 *       All Bluetooth devices should support at least one IAC,
 *       the General Inquiry Access Code (GIAC).
 *
 *  \param [in] num_iac
 *         This parameter specifies the number of IACs, which are currently to
 *         be used by the local Baseband Controller to simultaneously listen
 *         for during an Inquiry Scan. Range: 0x01-0x40.
 *         The value specified must not be greater than the number of IACs that
 *         the local Bluetooth device is capable of supporting, as returned by
 *         the BT_hci_read_number_of_supported_iac() API.
 *
 *  \param [in] iac_lap
 *         Pointer to a caller allocated UINT32 array of size num_current_iac,
 *         which holds the values of each LAP to be set. Each IAC LAP is
 *         actually a 3-byte value represented in a 4-byte UINT32 variable.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_current_iac_lap
           (
               UCHAR     num_iac,
               UINT32 *  iac_lap
           );

#ifndef HCI_LITE
/**
 *  \brief To read the Page Scan Period Mode settings of the local
 *         Bluetooth device.
 *
 *  \par Description:
 *       This API can be used to read the mandatory Page Scan Period Mode of the
 *       local Bluetooth device.
 *       Every time an inquiry response message is sent, the Bluetooth device
 *       will start a timer, the value of which is dependent on the Page Scan
 *       Period Mode. As long as this timer has not expired, the Bluetooth device
 *       will use the Page Scan Period Mode for all following page scans.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_page_scan_period_mode() \
        hci_common_api_handler_no_params (HCI_READ_PAGE_SCAN_PERIOD_MODE_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To set the Page Scan Period Mode in the local Bluetooth device.
 *
 *  \par Description:
 *       This API can be used to write the mandatory Page Scan Period Mode
 *       configuration parameter to the local Bluetooth device.
 *       Every time an inquiry response message is sent, the Bluetooth device
 *       will start a timer, the value of which is dependent on the Page Scan
 *       Period Mode. As long as this timer has not expired,
 *       the Bluetooth device will use the Page Scan Period Mode for all
 *       following page scans.
 *
 *  \param [in] mode
 *         Value of the Page Scan Period Mode parameter to be set in the local
 *         Bluetooth device.
 *         The valid values and their meanings are shown below:
 *         0x00: P0 (Default)
 *         0x01: P1.
 *         0x02: P2.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_page_scan_period_mode
           (
               UCHAR  mode
           );

#ifndef HCI_LITE
/**
 *  \brief To read the Page Scan Mode of the local Bluetooth device.
 *
 *  \par Description:
 *       This API can be used to read the Page Scan Mode configuration parameter
 *       from the local Bluetooth device.
 *       The Page Scan Mode configuration parameter indicates the mode for the
 *       default page scan. Currently, one mandatory page scan mode and three
 *       optional page scan modes are defined.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_page_scan_mode() \
        hci_common_api_handler_no_params (HCI_READ_PAGE_SCAN_MODE_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To set the Page Scan Mode in the local Bluetooth device.
 *
 *  \par Description:
 *       This API can be used to write the default Page Scan Mode for the local
 *       Bluetooth device.
 *       The Page Scan Mode configuration parameter indicates the mode for the
 *       default page scan. Currently, one mandatory page scan mode and three
 *       optional page scan modes are defined.
 *
 *  \param [in] mode
 *         Value of the Page Scan Mode parameter to be set in the local
 *         Bluetooth device .
 *         The valid values and their meanings are shown below:
 *         0x00: Mandatory Page Scan Mode (Default)
 *         0x01: Optional Page Scan Mode I
 *         0x02: Optional Page Scan Mode II
 *         0x03: Optional Page Scan Mode III
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_page_scan_mode
           (
               UCHAR  mode
           );

#ifdef HCI_READ_TRANSMIT_POWER_LEVEL_SUPPORT
/**
 *  \brief To read the value of Transmit Power Level.
 *
 *  \par Description:
 *       This command reads the values for the Transmit_Power_Level parameter
 *       for the specified Connection_Handle. The Connection_Handle shall be
 *       a Connection_Handle for an ACL connection.
 *
 *  \param [in] handle
 *         Specifies which Connection_Handle's Transmit Power Level setting
 *         to read.
 *         Range: 0x0000-0x0EFF (0x0F00 - 0x0FFF Reserved for future use)
 *
 *  \param [in] power_type
 *         This parameter specifies whether the current or maximum power level
 *         to read.
 *         The valid values are:
 *         Value 0x00: Read current Transmit Power Level.
 *         Value 0x01: Read maximum Transmit Power Level.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
API_RESULT BT_hci_read_transmit_power_level
           (
               UINT16   handle,
               UCHAR    power_type
           );
#endif /* HCI_READ_TRANSMIT_POWER_LEVEL_SUPPORT */

/* ------------------------------------------------------------------------- */

/* HCI Host Controller Informational APIs ---------------------------------- */
#ifdef HCI_READ_LOCAL_VERSION_INFORMATION_SUPPORT
/**
 *  \brief To read Version information of the local Bluetooth device.
 *
 *  \par Description:
 *       This API reads the values for the version information for the local
 *       Controller.
 *       The HCI Version information defines the version information of
 *       the HCI layer. The LMP/PAL Version information defines the version
 *       of the LMP or PAL. The Manufacturer_Name information indicates
 *       the manufacturer of the local device.
 *       The HCI Revision and LMP/PAL Subversion are implementation dependent.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_read_local_version_information() \
        hci_common_api_handler_no_params \
        (HCI_READ_LOCAL_VERSION_INFORMATION_OPCODE)
#endif /* HCI_READ_LOCAL_VERSION_INFORMATION_SUPPORT */

#ifdef HCI_READ_LOCAL_SUPPORTED_FEATURES_SUPPORT
/**
 *  \brief To read list of LMP features of the local Bluetooth device.
 *
 *  \par Description:
 *       This API reads the list of features supported by the Link Manager
 *       Protocol of the local Bluetooth device.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_read_local_supported_features() \
        hci_common_api_handler_no_params \
        (HCI_READ_LOCAL_SUPPORTED_FEATURES_OPCODE)
#endif /* HCI_READ_LOCAL_SUPPORTED_FEATURES_SUPPORT */

#ifdef HCI_READ_BD_ADDR_SUPPORT
/**
 *  \brief To read the Bluetooth Device Address from local Bluetooth device.
 *
 *  \par Description:
 *       This API reads the Bluetooth Device Address (BD_ADDR) of the local
 *       Bluetooth device.
 *       On a BR/EDR Controller, this command reads the Bluetooth Controller
 *       address (BD_ADDR).
 *       On an LE Controller, this command shall read the Public Device
 *       Address. If this Controller does not have a Public Device Address,
 *       the value 0x000000000000 shall be returned.
 *       On a BR/EDR/LE Controller, the public address shall be the same as
 *       the BD_ADDR.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_read_bd_addr() \
        hci_common_api_handler_no_params (HCI_READ_BD_ADDR_OPCODE)
#endif /* HCI_READ_BD_ADDR_SUPPORT */

#ifdef HCI_READ_BUFFER_SIZE_SUPPORT
/**
 *  \brief To read the size and number of buffers the local Bluetooth device
 *         maintains for outgoing ACL, Synchronous (SCO/eSCO) and BLE data.
 *
 *  \par Description:
 *       This API reads reads the size and number of buffers the local
 *       Bluetooth device maintains for outgoing ACL, Synchronous (SCO/eSCO)
 *       and BLE data.
 *       The Read_Buffer_Size command is used toread the maximum size of
 *       the data portion of HCI ACL and synchronous Data Packets sent from
 *       the Host to the Controller. The Host will segment the data to be
 *       transmitted from the Host to the Controller according to these sizes,
 *       so that the HCI Data Packets will con-tain data with up to these
 *       sizes. The Read_Buffer_Size command also returns the total number of
 *       HCI ACL and synchronous Data Packets that can be stored in the data
 *       buffers of the Controller. The Read_Buffer_Size command must be
 *       issued by the Host before it sends any data to the Controller.
 *       For a device supporting BR/EDR and LE, if the LE_Read_Buffer_Size
 *       command returned zero for the number of buffers, then buffers
 *       returned by Read_Buffer_Size are shared between BR/EDR and LE.
 *       On an Primary Controller that supportsLE only, the Read_Buffer_Size
 *       command shall not be supported (the LE_Read_Buffer_Size command is
 *       to be used in this case).
 *       The HC_ACL_Data_Packet_Length return parameter will be used to
 *       determine the size of the L2CAP segments contained in ACL Data
 *       Packets, which are transferred from the Host to the Controller to be
 *       broken up into baseband pack-ets by the Link Manager.
 *       The HC_Synchronous_Data_Packet_Length return parameter is used to
 *       determine the maximum size of HCI synchronous Data Packets.
 *       Both the Host and the Controller must support command and event
 *       packets, where the data portion (excluding header) contained in the
 *       packets is 255 octets in size. The HC_Total_Num_ACL_Data_Packets
 *       return parameter contains the total number of HCI ACL Data Packets
 *       that can be stored in the data buffers of the Controller. The Host
 *       will determine how the buffers are to be divided between different
 *       Connection Handles. The HC_Total_Num_Synchronous_Data_Packets return
 *       parameter gives the sameinformation but for HCI synchronous Data
 *       Packets.
 *       Note: The HC_ACL_Data_Packet_Length and
 *       HC_Synchronous_Data_Packet_Length return parameters do not include
 *       the length of the HCI Data Packet header.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_read_buffer_size() \
        hci_common_api_handler_no_params (HCI_READ_BUFFER_SIZE_OPCODE)
#endif /* HCI_READ_BUFFER_SIZE_SUPPORT */


#ifdef HCI_ENH_SCO
/**
 *  \brief To read list of codecs supported by the Controller.
 *
 *  \par Description:
 *  This command reads a list of the Bluetooth SIG approved codecs supported
 *  by the Controller, as well as vendor specific codecs, which are defined
 *  by an individual manufacturer.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_read_local_supported_codecs() \
        hci_common_api_handler_no_params \
        (HCI_READ_LOCAL_SUPPORTED_CODECS_OPCODE)
#endif /* HCI_ENH_SCO */

/* ------------------------------------------------------------------------- */

/* HCI Status Parameters APIs ---------------------------------------------- */

#ifdef HCI_READ_RSSI_SUPPORT
/**
 *  \brief To retrieve the RSSI value for an ACL or BLE connection.
 *
 *  \par Description:
 *       This API reads the value of Received Signal Strength Indication (RSSI)
 *       value from the controller.
 *       For a BR/EDR Controller, a Connection_Handle is used as the Handle
 *       command parameter and return parameter. The RSSI parameter returns
 *       the difference between the measured Received Signal Strength
 *       Indication (RSSI) and the limits of the Golden Receive Power Range
 *       for a Connection Handle to another BR/EDR Controller.
 *       The Connection_Handle must be a Connection_Handle for an ACL
 *       connection. Any positive RSSI value returned by the Controller
 *       indicates how many dB the RSSI is above the upper limit, any
 *       negative value indicates how many dB the RSSI is below the lower
 *       limit. The value zero indicates that the RSSI is inside the Golden
 *       Receive Power Range.
 *       Note: How accurate the dB values will be depends on the Bluetooth
 *       hardware. The only requirements for the hardware are that the BR/EDR
 *       Controller is able to tell whether the RSSI is inside, above or below
 *       the Golden Device Power Range.
 *       The RSSI measurement compares the received signal power with two
 *       threshold levels, which define the Golden Receive Power Range.
 *       The lower threshold level corresponds to a received power between
 *       -56 dBm and 6 dB above the actual sensitivity of the receiver.
 *       The upper threshold level is 20 dB above the lower threshold level
 *       to an accuracy of +/- 6 dB.
 *       For an AMP Controller, a Physical_Link_Handle is used for the Handle
 *       command parameter and return parameter. The meaning of the RSSI metric
 *       is AMP type specific and defined in the AMP PALs.
 *       For an LE transport, a Connection_Handle is used as the Handle command
 *       parameter and return parameter. The meaning of the RSSI metric is an
 *       absolute receiver signal strength value in dBm to +/- 6 dBm accuracy.
 *       If the RSSI cannot be read, the RSSI metric shall be set to 127.
 *
 *  \param [in] connection_handle
 *         The Connection Handle to be used to identify the connection
 *         for reading the RSSI.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_read_rssi(connection_handle) \
        hci_write_command_connection_handle \
        ( \
               HCI_READ_RSSI_OPCODE, \
               (connection_handle) \
        )
#endif /* HCI_READ_RSSI_SUPPORT */

/* ------------------------------------------------------------------------- */
/* HCI Bluetooth Low Energy API's ----------------------------------------------- */

/* New APIs from Bluetooth Spec 4.0 ---------------------------------------- */
#ifdef BT_4_0
#ifdef HCI_LE_SET_EVENT_MASK_SUPPORT
/**
 *  \brief To set the LE event mask.
 *
 *  \par Description:
 *    The BT_hci_le_set_event_mask API is used to control which LE events are
 *    generated by the HCI for the Host. If the bit in the LE_Event_Mask is set
 *    to a one, then the event associated with that bit will be enabled.
 *    The Host has to deal with each event that is generated by an LE
 *    Controller. The event mask allows the Host to control which events will
 *    interrupt it.
 *    For LE events to be generated, the LE Meta Event bit in the Event_Mask
 *    shall also be set. If that bit is not set, then LE events not shall be
 *    generated, regardless of how the LE_Event_Mask is set.
 *
 *  \param [in] p1 LE Event Mask (8 Octets)
 *          Bit   LE Event Types
 *         ----   --------------
 *         - 0  : LE Connection Complete Event
 *         - 1  : LE Advertising Report Event
 *         - 2  : LE Connection Update Complete Event
 *         - 3  : LE Read Remote Features Complete Event
 *         - 4  : LE Long Term Key Request Event
 *         - 5  : LE Remote Connection Parameter Request Event
 *         - 6  : LE Data Length Change Event
 *         - 7  : LE Read Local P-256 Public Key Complete Event
 *         - 8  : LE Generate DHKey Complete Event
 *         - 9  : LE Enhanced Connection Complete Event
 *         - 10 : LE Directed Advertising Report Event
 *         - 11 : LE PHY Update Complete Event
 *         - 12 : LE Extended Advertising Report Event
 *         - 13 : LE Periodic Advertising Sync Established Event
 *         - 14 : LE Periodic Advertising Report Event
 *         - 15 : LE Periodic Advertising Sync Lost Event
 *         - 16 : LE Extended Scan Timeout Event
 *         - 17 : LE Extended Advertising Set Terminated Event
 *         - 18 : LE Scan Request Received Event
 *         - 19 : LE Channel Selection Algorithm Event
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_set_event_mask(p1)  \
        hci_write_command_UCHAR_array \
        (HCI_LE_SET_EVENT_MASK_OPCODE, (p1), 8U)
#endif /* HCI_LE_SET_EVENT_MASK_SUPPORT */

#ifdef HCI_LE_READ_BUFFER_SIZE_SUPPORT
/**
 *  \brief To read the size and number of buffers the local Bluetooth
 *         controller maintains for outgoing LE data.
 *
 *  \par Description:
 *       This API is usedto read the maximum size of the data portion of HCI
 *       LE ACL Data Packets sent from the Host to the Controller.
 *       The Host will segment the data transmitted to the Controller
 *       according to these values, so that the HCIData Packets will contain
 *       datawith up to this size. The LE_Read_Buffer_Size command also returns
 *       the total number of HCI LE ACL Data Packets that can be stored in the
 *       data buffers of the Controller. The LE_Read_Buffer_Size command must
 *       be issued by the Host before it sends any data to an LE Controller.
 *       If the Controller returns a length value of zero, the Host shall use
 *       the Read_Buffer_Size command to determine the size of the data
 *       buffers (shared between BR/EDR and LE transports).
 *       Note: Both the Read_Buffer_Size and LE_Read_Buffer_Size commands may
 *       return buffer length parameter valuesthat are non-zero. This allows
 *       a Controller to offer differentsized buffers for BR/EDRdata packets
 *       and LE data packets.
 *       The HC_LE_ACL_Data_Packet_Length return parameter shall be used to
 *       determine the size of the L2CAP PDU segments contained in ACL Data
 *       Packets, which are transferred from the Host to the Controller to be
 *       broken up into packets by the Link Layer. Both the Host and the
 *       Controller shall support command and event packets, where the data
 *       portion (excluding header) contained in the packets is 255 octets in
 *       size. The HC_Total_Num_LE_ACL_Data_Packets return parameter contains
 *       the total number of HCI ACL Data Packets that can be stored in the
 *       data buffers of the Controller. The Host determines how the buffers
 *       are to be divided between different Connection Handles.
 *       Note: The HC_LE_ACL_Data_Packet_Length return parameter does not
 *       include the length of the HCI Data Packet header.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_read_buffer_size()  \
        hci_common_api_handler_no_params \
        (HCI_LE_READ_BUFFER_SIZE_OPCODE)
#endif /* HCI_LE_READ_BUFFER_SIZE_SUPPORT */

#ifdef HCI_LE_READ_BUFFER_SIZE_V2_SUPPORT
/**
 *  \brief To read the maximum size of the data portion of ACL data packets
 *  and isochronous data packets sent from the Host to the Controller.
 *
 *  \par Description:
 *       This API is used to read the maximum size of the data portion of ACL
 *       data packets and isochronous data packets sent from the Host to the
 *       Controller.
 *       The Host shall segment the data transmitted to the Controller
 *       according to these values so that the HCI Data packets and isochronous
 *       data packets will contain data up to this size.
 *       The HCI_LE_Read_Buffer_Size command also returns the total number of
 *       HCI LE ACL Data packets and isochronous data packets that can be
 *       stored in the data buffers of the Controller.
 *       The HCI_LE_Read_Buffer_Size command shall be issued by the Host before
 *       it sends any data to an LE Controller. If the Controller supports HCI
 *       ISO Data packets, it shall return non-zero values for the
 *       ISO_Data_Packet_Length and Total_Num_ISO_Data_Packets parameters.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_read_buffer_size_v2()  \
        hci_common_api_handler_no_params \
        (HCI_LE_READ_BUFFER_SIZE_V2_OPCODE)
#endif /* HCI_LE_READ_BUFFER_SIZE_V2_SUPPORT */

#ifdef HCI_LE_READ_LOCAL_SUPPORTED_FEATURES_SUPPORT
/**
 *  \brief To read supported LE features by local Bluetooth Controller.
 *
 *  \par Description:
 *       This API can be used to request the list of the supported LE features.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_read_local_supported_features()  \
        hci_common_api_handler_no_params \
        (HCI_LE_READ_LOCAL_SUPPORTED_FEATURES_OPCODE)
#endif /* HCI_LE_READ_LOCAL_SUPPORTED_FEATURES_SUPPORT */

#ifdef HCI_LE_SET_RANDOM_ADDRESS_SUPPORT
/**
 *  \brief To set the LE Random Device Address of local Bluetooth Device.
 *
 *  \par Description:
 *       This API can be used to set the LE Random Device Address in the
 *       Controller.
 *       If this command is used to change the address, the new random address
 *       shall take effect for advertising no later than the next successful
 *       LE Set Advertising Enable Command, for scanning no later than the next
 *       successful LE Set Scan Enable Command or LE Set Extended Scan Enable
 *       Command, and for initiating no later than the next successful
 *       LE Create Connection Command or LE Extended Create Connection Command.
 *
 *  \param [in] rand_addr
 *         LE Random Device Address
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note If Extended Advertising is in use, this command only affects
 *  the address used for scanning and initiating. The addresses used for
 *  advertising are set by the LE_Set_Advertising_Set_Random_Address command.
 *
 *  If the Host issues this command when scanning or legacy advertising is
 *  enabled, the Controller shall return the error code Command Disallowed
 *  (0x0C).
 */
#define BT_hci_le_set_random_address(rand_addr)  \
        hci_write_command_UCHAR_array \
        (HCI_LE_SET_RANDOM_ADDRESS_OPCODE, (rand_addr), BT_BD_ADDR_SIZE)
#endif /* HCI_LE_SET_RANDOM_ADDRESS_SUPPORT */

#ifdef HCI_LE_SET_ADVERTISING_PARAMETERS_SUPPORT
/**
 *  \brief To set advertising parameters.
 *
 *  \par Description:
 *       This API can be used to set the advertising parameters.
 *
 *       The Advertising_Interval_Min shall be less than or equal to the
 *       Advertising_Interval_Max. The Advertising_Interval_Min and
 *       Advertising_Interval_Max should not be the same value to enable the
 *       Controller to determine the best advertising interval given other
 *       activities.
 *
 *       For high duty cycle directed advertising, i.e. when Advertising_Type
 *       is 0x01 (ADV_DIRECT_IND, high duty cycle), the Advertising_Interval_Min
 *       and Advertising_Interval_Max parameters are not used and shall be ignored.
 *
 *       The Advertising_Type is used to determine the packet type that is used
 *       for advertising when advertising is enabled.
 *
 *       Own_Address_Type parameter indicates the type of address being used
 *       in the advertising packets.
 *
 *       If Own_Address_Type equals 0x02 or 0x03, the Peer_Address parameter
 *       contains the peer's Identity Address and the Peer_Address_Type
 *       parameter contains the Peer's Identity Type (i.e. 0x00 or 0x01).
 *       These parameters are used to locate the corresponding local IRK
 *       in the resolving list; this IRK is used to generate the own address
 *       used in the advertisement.
 *
 *       If directed advertising is performed, i.e. when Advertising_Type is
 *       set to 0x01 (ADV_DIRECT_IND, high duty cycle) or 0x04 (ADV_DIRECT_IND,
 *       low duty cycle mode), then the Peer_Address_Type and Peer_Address
 *       shall be valid.
 *
 *       If Own_Address_Type equals 0x02 or 0x03, the Controller generates
 *       the peer's Resolvable Private Address using the peer's IRK
 *       corresponding to the peer's Identity Address contained in
 *       the Peer_Address parameter and peer's Identity Address Type
 *       (i.e. 0x00 or 0x01) contained in the Peer_Address_Type parameter.
 *
 *       The Advertising_Channel_Map is a bit field that indicates the advertising
 *       channels that shall be used when transmitting advertising packets.
 *       At least one channel bit shall be set in the Advertising_Channel_Map parameter.
 *
 *       The Advertising_Filter_Policy parameter shall be ignored when
 *       directed advertising is enabled.
 *
 *       The Host shall not issue this command when advertising is enabled
 *       in the Controller; if it is the Command Disallowed error code shall be used.
 *
 *       If the advertising interval range provided by the Host
 *       (Advertising_Interval_Min, Advertising_Interval_Max) is outside
 *       the advertising interval range supported by the Controller,
 *       then the Controller shall return the Unsupported Feature or
 *       Parameter Value (0x11) error code.
 *
 *  \param [in] advertising_interval_min
 *         Minimum advertising interval for undirected and low duty cycle
 *         direct advertising.
 *
 *  \param [in] advertising_interval_max
 *         Maximum advertising interval for undirected and low duty cycle
 *         directed advertising.
 *
 *  \param [in] advertising_type
 *         One of the following types
 *         0x00: Connectable undirected advertising (ADV_IND) (default)
 *         0x01: Connectable high duty cycle directed advertising
 *               (ADV_DIRECT_IND, high duty cycle)
 *         0x02: Scannable undirected advertising (ADV_SCAN_IND)
 *         0x03: Non connectable undirected advertising (ADV_NONCONN_IND)
 *         0x04: Connectable low duty cycle directed advertising
 *               (ADV_DIRECT_IND, low duty cycle)
 *
 *  \param [in] own_addr_type
 *         One of the following types
 *         0x00: Public Device Address (default)
 *         0x01: Random Device Address
 *         0x02: Controller generates Resolvable Private Address based on
 *               the local IRK from the resolving list. If the resolving list
 *               contains no matching entry, use the public address.
 *         0x03: Controller generates Resolvable Private Address based on
 *               the local IRK from the resolving list. If the resolving list
 *               contains no matching entry, use the random address
 *               from LE_Set_Random_Address.
 *
 *  \param [in] peer_addr_type
 *         Address Type of Remote Device
 *         0x00 - Public Device Address (default) or Public Identity Address.
 *         0x01 - Random Device Address or Random (static) Identity Address.
 *
 *  \param [in] peer_addr
 *         Bluetooth Device Address of Remote Device.
 *         Public Device Address, Random Device Address,
 *         Public Identity Address, or Random (static) Identity Address
 *         of the device to be connected.
 *
 *  \param [in] advertising_channel_map
 *         Channle map selection, using bitmask.
 *         xxxxxxx1b: Enable channel 37 use
 *         xxxxxx1xb: Enable channel 38 use
 *         xxxxx1xxb: Enable channel 39 use
 *         00000111b: Default (all channels enabled)
 *
 *  \param [in] advertising_filter_policy
 *         One of the following values
 *         0x00: Process scan and connection requests from all devices
 *               (i.e., the White List is not in use) (default).
 *         0x01: Process connection requests from all devices and only scan
 *               requests from devices that are in the White List.
 *         0x02: Process scan requests from all devices and only connection
 *               requests from devices that are in the White List.
 *         0x03: Process scan and connection requests only from devices
 *               in the White List.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_advertising_parameters
           (
               UINT16   advertising_interval_min,
               UINT16   advertising_interval_max,
               UCHAR    advertising_type,
               UCHAR    own_addr_type,
               UCHAR    peer_addr_type,
               UCHAR  * peer_addr,
               UCHAR    advertising_channel_map,
               UCHAR    advertising_filter_policy
           );
#endif /* HCI_LE_SET_ADVERTISING_PARAMETERS_SUPPORT */

#ifdef HCI_LE_READ_ADVERTISING_CHANNEL_TX_POWER_SUPPORT
/**
 *  \brief To read the transmit power level used for LE advertising channel
 *         packets.
 *
 *  \par Description:
 *       This API can be used to read the transmit power level used for LE
 *       advertising channel packets.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_read_advertising_channel_tx_power()  \
        hci_common_api_handler_no_params \
        (HCI_LE_READ_ADVERTISING_CHANNEL_TX_POWER_OPCODE)
#endif /* HCI_LE_READ_ADVERTISING_CHANNEL_TX_POWER_SUPPORT */

#ifdef HCI_LE_SET_ADVERTISING_DATA_SUPPORT
/**
 *  \brief To set advertising data used in advertsing packets that have a data
 *  field.
 *
 *  \par Description:
 *       This API is used to set the data used in advertsing packets
 *       that have a data field.
 *
 *       If advertising is currently enabled, the Controller shall use the new
 *       data in subsequent advertising events. If an advertising event is in
 *       progress when this command is issued, the Controller may use the old
 *       or new data for that event.
 *       If advertising is currently disabled, the data shall be kept by
 *       the Controller and used once advertising is enabled.
 *
 *  \param [in] advertising_data_len
 *         The number of significant octets in the Advertising_Data.
 *
 *  \param [in] advertising_data
 *         Advertising data.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_set_advertising_data(advertising_data_len, advertising_data)\
        hci_write_command_adv_scan_response_data \
        ( \
            HCI_LE_SET_ADVERTISING_DATA_OPCODE, \
            (advertising_data_len), \
            (advertising_data) \
        )
#endif /* HCI_LE_SET_ADVERTISING_DATA_SUPPORT */

#ifdef HCI_LE_SET_SCAN_RESPONSE_DATA_SUPPORT
/**
 *  \brief To provide data used in Scanning Packets that have a data field.
 *
 *  \par Description:
 *       This API can be used to set the data used in Scanning Packets that
 *       have a data field.
 *
 *       If advertising is currently enabled, the Controller shall use the new
 *       data in subsequent advertising events. If an advertising event is in
 *       progress when this command is issued, the Controller may use the old
 *       or new data for that event.  If advertising is currently disabled,
 *       the data shall be kept by the Controller and used once advertising
 *       is enabled.
 *
 *  \param [in] scan_data_len
 *         The number of significant octets in the Scan_Response_Data
 *
 *  \param [in] scan_data
 *         Scan data used in scan packets
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_set_scan_response_data(scan_data_len, scan_data) \
        hci_write_command_adv_scan_response_data \
        ( \
            HCI_LE_SET_SCAN_RESPONSE_DATA_OPCODE, \
            (scan_data_len), \
            (scan_data) \
        )
#endif /* HCI_LE_SET_SCAN_RESPONSE_DATA_SUPPORT */

#ifdef HCI_LE_SET_ADVERTISING_ENABLE_SUPPORT
/**
 *  \brief To request the local Bluetooth Device to start or stop advertising.
 *
 *  \par Description:
 *       This API can be used to request the Controller to start or stop
 *       advertising. The Controller manages the timing of advertisements
 *       as per the advertising parameters given in the
 *       \ref BT_hci_le_set_advertising_parameters command.
 *
 *       The Controller shall continue advertising until the Host issues an
 *       LE_Set_Advertising_Enable command with Advertising_Enable set to 0x00
 *       (Advertising is disabled) or until a connection is created or until
 *       the Advertising is timed out due to high duty cycle Directed Advertising.
 *       In these cases, advertising is then disabled.
 *       If the advertising parameters' Own_Address_Type parameter is set to 0x01
 *       and the random address for the device has not been initialized,
 *       the Controller shall return the error code Invalid HCI Command Parameters (0x12).
 *       If the advertising parameters' Own_Address_Type parameter is set to 0x03,
 *       the controller's resolving list did not contain a matching entry, and the random
 *       address for the device has not been initialized, the Controller shall return the
 *       error code Invalid HCI Command Parameters (0x12).
 *
 *  \param [in] advertising_enable
 *       Enable/Disable Advertising
 *       0x00: Disable Advertising (Default)
 *       0x01: Enable Advertising
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note Enabling advertising when it is already enabled can cause the random address
 *  to change. Disabling advertising when it is already disabled has no effect.
 */
#define BT_hci_le_set_advertising_enable(advertising_enable)  \
        hci_write_command_UCHAR \
        (HCI_LE_SET_ADVERTISING_ENABLE_OPCODE, (advertising_enable))
#endif /* HCI_LE_SET_ADVERTISING_ENABLE_SUPPORT */

#ifdef HCI_LE_SET_SCAN_PARAMETERS_SUPPORT
/**
 *  \brief To set scan parameters.
 *
 *  \par Description:
 *       This API can be used to set the scan parameters.
 *
 *       The LE_Scan_Type parameter controls the type of scan to perform.
 *
 *       The LE_Scan_Interval and LE_Scan_Window parameters are recommendations
 *       from the Host on how long (LE_Scan_Window) and how frequently
 *       (LE_Scan_Interval) the Controller should scan. The LE_Scan_Window
 *       parameter shall always be set to a value smaller or equal to the value
 *       set for the LE_Scan_Interval parameter. If they are set to the same
 *       value scanning should be run continuously.
 *
 *       The Own_Address_Type parameter determines the address being used
 *       in the scan request packets.
 *
 *       The Host shall not issue this command when scanning is enabled in the
 *       Controller; if it is the Command Disallowed error code shall be used.
 *
 *  \param [in] le_scan_type
 *         One of the following scan types.
 *         0x00: Passive Scanning. No scanning PDUs shall be sent (default)
 *         0x01: Active scanning. Scanning PDUs may be sent.
 *
 *  \param [in] le_scan_interval
 *         The time interval from when the Controller started its last
 *         LE scan until it begins the subsequent LE scan.
 *
 *  \param [in] le_scan_window
 *         The duration of the LE scan. LE_Scan_Window shall be less than
 *         or equal to LE_Scan_Interval
 *
 *  \param [in] own_addr_type
 *         One of the following types
 *         0x00: Public Device Address (default)
 *         0x01: Random Device Address
 *         0x02: Controller generates Resolvable Private Address based on
 *               the local IRK from the resolving list. If the resolving list
 *               contains no matching entry, use the public address.
 *         0x03: Controller generates Resolvable Private Address based on
 *               the local IRK from the resolving list. If the resolving list
 *               contains no matching entry, use the random address
 *               from LE_Set_Random_Address.
 *
 *  \param [in] scan_filter_policy
 *         One of the following values
 *         0x00: Accept all advertising packets except directed advertising
 *         packets not addressed to this device (default).
 *
 *         0x01: Accept only advertising packets from devices where
 *         the advertiser's address is in the White List. Directed advertising
 *         packets which are not addressed to this device shall be ignored.
 *
 *         0x02: Accept all advertising packets except directed advertising
 *         packets where the initiator's identity address does not address
 *         this device.
 *         Note: Directed advertising packets where the initiator's address is
 *         a resolvable private address that cannot be resolved are also accepted.
 *
 *         0x03: Accept all advertising packets except:
 *               - advertising packets where the advertiser's identity address
 *                 is not in the White List; and
 *               - directed advertising packets where the initiator's identity
 *                 address does not address this device
 *         Note: Directed advertising packets where the initiator's address is a
 *         resolvable private address that cannot be resolved are also accepted.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_scan_parameters
           (
               UCHAR    le_scan_type,
               UINT16   le_scan_interval,
               UINT16   le_scan_window,
               UCHAR    own_addr_type,
               UCHAR    scan_filter_policy
           );
#endif /* HCI_LE_SET_SCAN_PARAMETERS_SUPPORT */

#ifdef HCI_LE_SET_SCAN_ENABLE_SUPPORT
/**
 *  \brief To request the local Bluetooth Device to start or stop scanning.
 *
 *  \par Description:
 *       This API can be used to request the Controller to start or stop
 *       scanning. Scanning is used to discover advertising devices nearby.
 *
 *       The Filter_Duplicates parameter controls whether the Link Layer should
 *       filter duplicate advertising reports (Filtering_Enabled) to the Host,
 *       or if the Link Layer should generate advertising reports for each packet
 *       received (Filtering_Disabled).
 *
 *       If the scanning parameters' Own_Address_Type parameter is set to
 *       0x01 or 0x03 and the random address for the device has not been
 *       initialized, the Controller shall return the error code
 *       Invalid HCI Command Parameters (0x12).
 *
 *       If the LE_Scan_Enable parameter is set to 0x01 and scanning is already
 *       enabled, any change to the Filter_Duplicates setting shall take effect.
 *       Note: Disabling scanning when it is disabled has no effect.
 *
 *  \param [in] le_scan_enable
 *       Enable/Disable Scanning
 *       0x00: Disable Scanning
 *       0x01: Enable Scanning
 *
 *  \param [in] filter_dups
 *       Enable/Disable Duplicate Filtering
 *       0x00: Disable Duplicate Filtering
 *       0x01: Enable Duplicate Filtering
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_scan_enable
           (
               UCHAR    le_scan_enable,
               UCHAR    filter_dups
           );
#endif /* HCI_LE_SET_SCAN_ENABLE_SUPPORT */

#ifdef HCI_LE_CREATE_CONNECTION_SUPPORT
/**
 *  \brief To create LE Connection with remote device.
 *
 *  \par Description:
 *       This API is used to create a Link Layer connection to a connectable
 *       advertiser.
 *
 *       The LE_Scan_Interval and LE_Scan_Window parameters are recommendations
 *       from the Host on how long (LE_Scan_Window) and how frequently
 *       (LE_Scan_Interval) the Controller should scan. The LE_Scan_Window
 *       parameter shall be set to a value smaller or equal to the value set
 *       for the LE_Scan_Interval parameter.  If both are set to the same value,
 *       scanning should run continuously.
 *
 *       The Initiator_Filter_Policy is used to determine whether the White List
 *       is used. If the White List is not used, the Peer_Address_Type and
 *       the Peer_Address parameters specify the address type and address of
 *       the advertising device to connect to.
 *
 *       Peer_Address_Type parameter indicates the type of address used in
 *       the connectable advertisement sent by the peer. The Host shall not set
 *       Peer_Address_Type to either 0x02 or 0x03 if both the Host and
 *       the Controller support the LE Set Privacy Mode command. If a Controller
 *       that supports the LE Set Privacy Mode command receives the LE Create
 *       Connection command with Peer_Address_Type set to either 0x02 or 0x03,
 *       it may use either device privacy mode or network privacy mode for that
 *       peer device.
 *
 *       Peer_Address parameter indicates the Peer's Public Device Address,
 *       Random (static) Device Address, Non-Resolvable Private Address or
 *       Resolvable Private Address depending on the Peer_Address_Type parameter.
 *
 *       Own_Address_Type parameter indicates the type of address being used
 *       in the connection request packets.
 *
 *       The Conn_Interval_Min and Conn_Interval_Max parameters define the minimum
 *       and maximum allowed connection interval. The Conn_Interval_Min parameter
 *       shall not be greater than the Conn_Interval_Max parameter.
 *
 *       The Conn_Latency parameter defines the maximum allowed connection latency.
 *
 *       The Supervision_Timeout parameter defines the link supervision timeout
 *       for the connection. The Supervision_Timeout in milliseconds shall be
 *       larger than (1 + Conn_Latency) * Conn_Interval_Max * 2,
 *       where Conn_Interval_Max is given in milliseconds.
 *
 *       The Minimum_CE_Length and Maximum_CE_Length parameters are informative
 *       parameters providing the Controller with the expected minimum and
 *       maximum length of the connection events. The Minimum_CE_Length parameter
 *       shall be less than or equal to the Maximum_CE_Length parameter.
 *
 *       If the Host issues this command when another LE_Create_Connection
 *       command is pending in the Controller, the Controller shall return
 *       the error code Command Disallowed (0x0C).
 *
 *       If the Own_Address_Type parameter is set to 0x01 and the random address
 *       for the device has not been initialized, the Controller shall return
 *       the error code Invalid HCI Command Parameters (0x12).
 *
 *       If the Own_Address_Type parameter is set to 0x03, the
 *       Initiator_Filter_Policy parameter is set to 0x00, the controller's
 *       resolving list did not contain a matching entry, and the random address
 *       for the device has not been initialized, the Controller shall return
 *       the error code Invalid HCI Command Parameters (0x12).
 *
 *       If the Own_Address_Type parameter is set to 0x03, the
 *       Initiator_Filter_Policy parameter is set to 0x01, and the random address
 *       for the device has not been initialized, the Controller shall return
 *       the error code Invalid HCI Command Parameters (0x12).
 *
 *  \param [in] le_scan_interval
 *         The time interval from when the Controller started its last
 *         LE scan until it begins the subsequent LE scan.
 *
 *  \param [in] le_scan_window
 *         The duration of the LE scan. LE_Scan_Window shall be less than
 *         or equal to LE_Scan_Interval.
 *
 *  \param [in] initiator_filter_policy
 *         0x00: White list is not used to determine which advertiser
 *               to connect to.
 *               Peer_Address_Type and Peer_Address shall be used.
 *         0x01: White list is used to determine which advertiser
 *               to connect to.
 *               Peer_Address_Type and Peer_Address shall be ignored.
 *
 *  \param [in] peer_address_type
 *         One of the following types
 *         0x00: Public Device Address
 *
 *         0x01: Random Device Address
 *
 *         0x02: Public Identity Address (Corresponds to peer's Resolvable
 *         Private Address). This value shall only be used by the Host if
 *         either the Host or the Controller does not support
 *         the LE Set Privacy Mode command.
 *
 *         0x03: Random (static) Identity Address (Corresponds to peer's
 *         Resolvable Private Address). This value shall only be used by
 *         a Host if either the Host or the Controller does not support
 *         the LE Set Privacy Mode command.
 *
 *  \param [in] peer_address
 *         Public Device Address, Random Device Address, Public Identity
 *         Address, or Random (static) Identity Address of the device to be
 *         connected.
 *
 *  \param [in] own_address_type
 *         One of the following types
 *         0x00: Public Device Address
 *         0x01: Random Device Address
 *         0x02: Controller generates Resolvable Private Address based on
 *               the local IRK from the resolving list. If the resolving list
 *               contains no matching entry, use the public address.
 *         0x03: Controller generates Resolvable Private Address based on
 *               the local IRK from the resolving list. If the resolving list
 *               contains no matching entry, use the random address
 *               from LE_Set_Random_Address.
 *
 *  \param [in] conn_interval_min
 *         Minimum value for the connection event interval.
 *         This shall be less than or equal to Conn_Interval_Max.
 *
 *  \param [in] conn_interval_max
 *         Maximum value for the connection event interval.
 *         This shall be greater than or equal to Conn_Interval_Min.
 *
 *  \param [in] conn_latency
 *         Slave latency for the connection in number of connection events.
 *
 *  \param [in] supervision_timeout
 *         Supervision Timeout for the LE Link.
 *
 *  \param [in] minimum_ce_length
 *         Information parameter about the minimum length of connection
 *         needed for this LE connection.
 *
 *  \param [in] maximum_ce_length
 *         Information parameter about the maximum length of connection
 *         needed for this LE connection.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_create_connection
           (
               UINT16   le_scan_interval,
               UINT16   le_scan_window,
               UCHAR    initiator_filter_policy,
               UCHAR    peer_address_type,
               UCHAR *  peer_address,
               UCHAR    own_address_type,
               UINT16   conn_interval_min,
               UINT16   conn_interval_max,
               UINT16   conn_latency,
               UINT16   supervision_timeout,
               UINT16   maximum_ce_length,
               UINT16   minimum_ce_length
           );
#endif /* HCI_LE_CREATE_CONNECTION_SUPPORT */

#ifdef HCI_LE_CREATE_CONNECTION_CANCEL_SUPPORT
/**
 *  \brief To cancel the Create Connection procedure.
 *
 *  \par Description:
 *       This API is used to cancel the \ref BT_hci_le_create_connection or
 *       \ref BT_hci_le_extended_create_connection commands.
 *       This command shall only be issued after the \ref BT_hci_le_create_connection
 *       or \ref BT_hci_le_extended_create_connection commands have been issued,
 *       a Command Status event has been received for the LE Create Connection
 *       or LE_Extended_Create_Connection commands, and before the LE Connection
 *       Complete or LE Enhanced Connection Complete events.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_create_connection_cancel()  \
        hci_common_api_handler_no_params \
        (HCI_LE_CREATE_CONNECTION_CANCEL_OPCODE)
#endif /* HCI_LE_CREATE_CONNECTION_CANCEL_SUPPORT */

#ifdef HCI_LE_READ_WHITE_LIST_SIZE_SUPPORT
/**
 *  \brief To read the total number of white list entries that can be stored
 *         in the Controller.
 *
 *  \par Description:
 *       This API can be used to read the total number of white list entries
 *       that can be stored in the Controller.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note: The number of entries that can be stored is not fixed and
 *  the Controller can change it at any time (e.g. because the memory used to
 *  store the White List can also be used for other purposes).
 */
#define BT_hci_le_read_white_list_size()  \
        hci_common_api_handler_no_params \
        (HCI_LE_READ_WHITE_LIST_SIZE_OPCODE)
#endif /* HCI_LE_READ_WHITE_LIST_SIZE_SUPPORT */

#ifdef HCI_LE_CLEAR_WHITE_LIST_SUPPORT
/**
 *  \brief To clear the white list stored in the Controller.
 *
 *  \par Description:
 *       This API can be used to clear the white list stored in the Controller.
 *       This command can be used at any time except when:
 *       - The advertising filter policy uses the white list and advertising is
 *          enabled.
 *       - The scanning filter policy uses white list and scanning is enabled.
 *       - The initiator filter policy uses the white list and a response to
 *         \ref BT_hci_le_create_connection API is outstanding.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_clear_white_list()  \
        hci_common_api_handler_no_params \
        (HCI_LE_CLEAR_WHITE_LIST_OPCODE)
#endif /* HCI_LE_CLEAR_WHITE_LIST_SUPPORT */

#if ((defined HCI_LE_ADD_DEVICE_TO_WHITE_LIST_SUPPORT) || \
     (defined HCI_LE_REMOVE_DEVICE_FROM_WHITE_LIST_SUPPORT))
/**
 * \cond ignore_this Ignore this fundtion while generating doxygen document
 */
API_RESULT BT_hci_le_add_remove_device_to_white_list
           (
               UINT16   opcode,
               UCHAR    addr_type,
               UCHAR  * addr
           );
/**
 * \endcond
 */
#endif /* HCI_LE_ADD_DEVICE_TO_WHITE_LIST_SUPPORT || HCI_LE_REMOVE_DEVICE_FROM_WHITE_LIST_SUPPORT */

#ifdef HCI_LE_ADD_DEVICE_TO_WHITE_LIST_SUPPORT
/**
 *  \brief To add a single device to the white list stored in the Controller.
 *
 *  \par Description:
 *       This API can be used to add a single device to the white list
 *       stored in the Controller.
 *       This command can be used at any time except when:
 *       - The advertising filter policy uses the white list and advertising is
 *          enabled.
 *       - The scanning filter policy uses the white list and
 *         scanning is enabled.
 *       - The initiator filter policy uses the white list and a response to
 *         \ref BT_hci_le_create_connection API is outstanding.
 *
 *       When a Controller cannot add a device to the White List because there
 *       is no space available, it shall return the error code Memory Capacity
 *       Exceeded (0x07).
 *
 *       Address is ignored when Address_Type is set to 0xFF.
 *
 *  \param [in] addr_type
 *         Bluetooth Device Address Type of the device address to be added.
 *         0x00: Public Device Address
 *         0x01: Random Device Address
 *         0xFF: Devices sending anonymous advertisements
 *
 *  \param [in] addr
 *         Public Device Address or Random Device Address of the device
 *         to be added to the White List.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_add_device_to_white_list(addr_type, addr)\
        BT_hci_le_add_remove_device_to_white_list\
        (HCI_LE_ADD_DEVICE_TO_WHITE_LIST_OPCODE, (addr_type), (addr))
#endif /* HCI_LE_ADD_DEVICE_TO_WHITE_LIST_SUPPORT */

#ifdef HCI_LE_REMOVE_DEVICE_FROM_WHITE_LIST_SUPPORT
/**
 *  \brief To remove a single device from the white list stored in the Controller.
 *
 *  \par Description:
 *       This API can be used to remove a single device from the white list
 *       stored in the Controller.
 *       This command can be used at any time except when:
 *       - The advertising filter policy uses the white list and advertising is
 *         enabled.
 *       - The scanning filter policy uses the white list and scanning is
 *         enabled.
 *       - The initiator filter policy uses the white list and a response to
 *         \ref BT_hci_le_create_connection API is outstanding.
 *
 *       Address is ignored when Address_Type is set to 0xFF.
 *
 *  \param [in] addr_type
 *         Bluetooth Device Address Type of the device address to be removed.
 *         0x00: Public Device Address
 *         0x01: Random Device Address
 *         0xFF: Devices sending anonymous advertisements
 *
 *  \param [in] addr
 *         Public Device Address or Random Device Address of the device
 *         to be removed from the White List.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_remove_device_from_white_list(addr_type, addr)\
        BT_hci_le_add_remove_device_to_white_list\
        (HCI_LE_REMOVE_DEVICE_FROM_WHITE_LIST_OPCODE, (addr_type), (addr))
#endif /* HCI_LE_REMOVE_DEVICE_FROM_WHITE_LIST_SUPPORT */

#ifdef HCI_LE_CONNECTION_UPDATE_SUPPORT
/**
 *  \brief To change LE Connection Parameters with remote device.
 *
 *  \par Description:
 *       This API is used to change the Link Layer connection parameters of
 *       a connection. This command may be issued on both the master and slave.
 *
 *       The Conn_Interval_Min and Conn_Interval_Max parameters are used to
 *       define the minimum and maximum allowed connection interval.
 *       The Conn_Interval_Min parameter shall not be greater than
 *       the Conn_Interval_Max parameter.
 *
 *       The Conn_Latency parameter shall define the maximum allowed connection
 *       latency.
 *
 *       The Supervision_Timeout parameter shall define the link supervision
 *       timeout for the LE link. The Supervision_Timeout in milliseconds
 *       shall be larger than (1 + Conn_Latency) * Conn_Interval_Max * 2,
 *       where Conn_Interval_Max is given in milliseconds.
 *
 *       The Minimum_CE_Length and Maximum_CE_Length are information parameters
 *       providing the Controller with a hint about the expected minimum and
 *       maximum length of the connection events. The Minimum_CE_Length shall
 *       be less than or equal to the Maximum_CE_Length.
 *
 *       The actual parameter values selected by the Link Layer may be
 *       different from the parameter values provided by the Host through
 *       this command.
 *
 *  \param [in] connection_handle
 *         Connection handle to identify the connection.
 *
 *  \param [in] conn_interval_min
 *         Minimum value for the connection interval.
 *         This shall be less than or equal to Conn_Interval_Max.
 *
 *  \param [in] conn_interval_max
 *         Maximum value for the connection interval.
 *         This shall be greater than or equal to Conn_Interval_Min.
 *
 *  \param [in] conn_latency
 *         Slave latency for the connection in number of connection events.
 *
 *  \param [in] supervision_timeout
 *         Supervision Timeout for the LE Link.
 *
 *  \param [in] minimum_ce_length
 *         Information parameter about the minimum length of connection event
 *         needed for this LE connection. How this value is used is outside the
 *         scope of this specification.
 *
 *  \param [in] maximum_ce_length
 *         Information parameter about the maximum length of connection event
 *         needed for this LE connection. How this value is used is outside the
 *         scope of this specification.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_connection_update
           (
               UINT16   connection_handle,
               UINT16   conn_interval_min,
               UINT16   conn_interval_max,
               UINT16   conn_latency,
               UINT16   supervision_timeout,
               UINT16   minimum_ce_length,
               UINT16   maximum_ce_length
           );
#endif /* HCI_LE_CONNECTION_UPDATE_SUPPORT */

#ifdef HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION_SUPPORT
/**
 *  \brief To specify a channel classification for data channels based on its
 *         \"local information\".
 *
 *  \par Description:
 *       This API is used to specify a channel classification for data
 *       channels based on its \"local information\". This classification
 *       persists until overwritten with a subsequent
 *       \ref BT_hci_le_set_host_channel_classification API is initiated or
 *       until the Controller is reset.
 *
 *       If this command is used, the Host should send it within 10 seconds
 *       of knowing that the channel classification has changed. The interval
 *       between two successive commands sent shall be at least one second.
 *
 *       This command shall only be used when the local device supports
 *       the Master role.
 *
 *  \param [in] channel_map
 *         This parameter contains 5 octet byte stream conatining, 37 1-bit
 *         fields. The nth such field (in the range 0 to 36) contains the
 *         value for the link laye channel index n.
 *         - Channel n is bad: 0
 *         - Channel n is unknown: 1
 *
 *         The most significant bits are reserved and shall be set to 0
 *         for future use.
 *         At least one channel shall be marked as unknown.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_set_host_channel_classification(channel_map)  \
        hci_write_command_UCHAR_array \
        (HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION_OPCODE, (channel_map), 5)
#endif /* HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION_SUPPORT */

#ifdef HCI_LE_READ_CHANNEL_MAP_SUPPORT
/**
 *  \brief To read current channel map for specified connection handle.
 *
 *  \par Description:
 *       This API can be used to read current channel map for the specified
 *       connection handle.
 *
 *       The returned value indicates the state of the Channel_Map specified
 *       by the last transmitted or received Channel_Map (in a CONNECT_IND or
 *       LL_CHANNEL_MAP_IND message) for the specified Connection_Handle,
 *       regardless of whether the Master has received an acknowledgement.
 *
 *  \param [in] conn_handle
 *         Connection Handle to be used to identify remote device.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_read_channel_map(conn_handle)  \
        hci_write_command_connection_handle \
        (HCI_LE_READ_CHANNEL_MAP_OPCODE, (conn_handle))
#endif /* HCI_LE_READ_CHANNEL_MAP_SUPPORT */

#ifdef HCI_LE_READ_REMOTE_USED_FEATURES_SUPPORT
/**
 *  \brief To read a list of the LE features from the remote device.
 *
 *  \par Description:
 *       This API can be used to request for the list of LE features from
 *       remote device which is associated with specfied connection handle.
 *
 *       This command may be issued on both the master and slave.
 *
 *       Note: If a connection already exists between the two devices and
 *       the features have already been fetched on that connection,
 *       the Controller may use a cached copy of the features.
 *
 *  \param [in] conn_handle
 *         Connection Handle to be used to identify remote device.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_read_remote_used_features(conn_handle)  \
        hci_write_command_connection_handle \
        (HCI_LE_READ_REMOTE_USED_FEATURES_OPCODE, (conn_handle))
#endif /* HCI_LE_READ_REMOTE_USED_FEATURES_SUPPORT */

#ifdef HCI_LE_ENCRYPT_SUPPORT
/**
 *  \brief To perform AES Encryption.
 *
 *  \par Description:
 *       The LE_Encrypt command is used to request the Controller to encrypt
 *       the Plaintext_Data in the command using the Key given in the command
 *       and returns the Encrypted_Data to the Host. The AES-128 bit block
 *       cypher is defined in NIST Publication FIPS-197.

 *  \param [in] key
 *         128 bit key for the encryption of the data given in the command.
 *         The most significant octet of the key corresponds to key[0] using
 *         the notation specified in FIPS 197.
 *
 *  \param [in] plain_text
 *         128 bit data block that is requested to be encrypted.
 *         The most significant octet of the PlainText_Data corresponds to
 *         in[0] using the notation specified in FIPS 197.
 *
 *  \param [out] encrypt_cmd_counter
 *         Encryption command counter is a out parameter which species the
 *         encryption counter. This parameter is used by inter stack
 *         to keep track each encryption command initiated and response
 *         received for each of them.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_encrypt
           (
               UCHAR *  key,
               UCHAR *  plain_text,
               UCHAR *  encrypt_cmd_counter
           );
#endif /* HCI_LE_ENCRYPT_SUPPORT */

#ifdef HCI_LE_RAND_SUPPORT
/**
 *  \brief To request for 8 octects of random data to be generated.
 *
 *  \par Description:
 *       This API can be used to request the Controller to generate 8 octects
 *       of random data to be sent to the Host.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_rand()  \
        hci_common_api_handler_no_params \
        (HCI_LE_RAND_OPCODE)
#endif /* HCI_LE_RAND_SUPPORT */

#ifdef HCI_LE_START_ENCRYPTION_SUPPORT
/**
 *  \brief To request for encryption or re-encryption of a BLE connection.
 *
 *  \par Description:
 *       This API is used to authenticate the given encryption key associated
 *       with the remote device specified by the connection handle, and once
 *       authenticated will encrypt the connection.
 *
 *       If the connection is already encrypted then the Controller shall
 *       pause connection encryption before attempting to authenticate the
 *       given encryption key, and then re-encrypt the connection. While
 *       encryption is paused no user data shall be transmitted.
 *
 *       On an authentication failure, the connection shall be automatically
 *       disconnected by the Link Layer. If this command succeeds, then the
 *       connection shall be encrypted.
 *       This command shall only be used when the local device's role is Master.
 *
 *  \param [in] conn_handle
 *         Connection handle to identify the connection
 *
 *  \param [in] random_number
 *         64-bit Random Number
 *
 *  \param [in] encrypted_diversifier
 *         16-bit encryption diversifier (ediv)
 *
 *  \param [in] long_term_key
 *         128-bit long term key (LTK)
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_start_encryption
           (
               UINT16   conn_handle,
               UCHAR  * random_number,
               UINT16   encrypted_diversifier,
               UCHAR  * long_term_key
           );
#endif /* HCI_LE_START_ENCRYPTION_SUPPORT */

#ifdef HCI_LE_LONG_TERM_KEY_REQUESTED_REPLY_SUPPORT
/**
 *  \brief To respond to LE Long Term Key Request.
 *
 *  \par Description:
 *       The LE_Long_Term_Key_Request Reply command is used to reply to an LE
 *       Long Term Key Request event from the Controller, and specifies the
 *       Long_Term_Key parameter that shall be used for this Connection_Handle.
 *
 *  \param [in] connection_handle
 *         Connection handle to identify the connection.
 *
 *  \param [in] long_term_key
 *         128-bit long term key (LTK) for the given connection.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_long_term_key_requested_reply
           (
               UINT16   connection_handle,
               UCHAR  * long_term_key
           );
#endif /* HCI_LE_LONG_TERM_KEY_REQUESTED_REPLY_SUPPORT */

#ifdef HCI_LE_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY_SUPPORT
/**
 *  \brief To reply with a negative response to LE Long Term Key request.
 *
 *  \par Description:
 *       This API can be used to reply to an LE Long Term Key Request event
 *       from the Controller if the Host cannot provide a Long Term Key for
 *       this Connection_Handle.
 *
 *  \param [in] conn_handle
 *         Connection handle to identify the connection.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_long_term_key_requested_negative_reply(conn_handle)  \
        hci_write_command_connection_handle \
        (HCI_LE_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY_OPCODE, (conn_handle))
#endif /* HCI_LE_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY_SUPPORT */

#ifdef HCI_LE_READ_SUPPORTED_STATES_SUPPORT
/**
 *  \brief To read local Bluetooth Controller supported states and state
 *         combinations.
 *
 *  \par Description:
 *       This API can be used to read states and state combinations that local
 *       link layer supports.
 *
 *       LE_States is an 8-octet bit field. If a bit is set to 1 then this
 *       state or state combination is supported by the Controller.
 *       Multiple bits in LE_States may be set to 1 to indicate support for
 *       multiple state and state combinations.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_read_supported_states()  \
        hci_common_api_handler_no_params \
        (HCI_LE_READ_SUPPORTED_STATES_OPCODE)
#endif /* HCI_LE_READ_SUPPORTED_STATES_SUPPORT */

#ifdef HCI_LE_RECEIVER_TEST_COMMAND_SUPPORT
/**
 *  \brief To start a test where the DUT receives test reference packets at a
 *  fixed interval.
 *
 *  \par Description:
 *       This API can be used to  start a test where the DUT receives test
 *       reference packets at a fixed interval. The tester generates the
 *       test reference packets.
 *
 *  \param [in] rx_channel
 *         rx_channel : N = (F - 2402) / 2
 *         Range: 0x00 - 0x27.
 *         Frequency Range : 2402 MHz to 2480 MHz
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_receiver_test_command(rx_channel)  \
        hci_write_command_UCHAR \
        (HCI_LE_RECEIVER_TEST_COMMAND_OPCODE, (rx_channel))
#endif /* HCI_LE_RECEIVER_TEST_COMMAND_SUPPORT */

#ifdef HCI_LE_TRANSMITTER_TEST_COMMAND_SUPPORT
/**
 *  \brief To start a test where the DUT generates test reference
 *         packets at a fixed interval.
 *
 *  \par Description:
 *       This API can be used to start a test where the DUT generates
 *       test reference packets at a fixed interval. The Controller
 *       shall transmit at maximum power.
 *
 *       An LE Controller supporting the LE_Transmitter_Test command shall
 *       support Packet_Payload values 0x00, 0x01 and 0x02. An LE Controller
 *       may support other values of Packet_Payload.
 *
 *  \param [in] tx_frequency
 *         Trasmit Channel<br>
 *         tx_channel = (F - 2402) / 2<br>
 *         Value Range: 0x00 to 0x27.<br>
 *         Frequency Range : 2402 MHz to 2480 MHz
 *  \param [in] test_data_length
 *         Length in bytes of payload data in each packet<br>
 *         Range: 0x00 to 0xFF
 *  \param [in] packet_payload
 *         Packet payload
 *         - 0x00: PRBS9 sequence '11111111100000111101...' (in transmission order)
 *         - 0x01: Repeated '11110000' (in transmission order) sequence
 *         - 0x02: Repeated '10101010' (in transmission order) sequence
 *         - 0x03: PRBS15 sequence
 *         - 0x04: Repeated '11111111' (in transmission order) sequence
 *         - 0x05: Repeated '00000000' (in transmission order) sequence
 *         - 0x06: Repeated '00001111' (in transmission order) sequence
 *         - 0x07: Repeated '01010101' (in transmission order) sequence
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_transmitter_test_command
           (
               UCHAR    tx_frequency,
               UCHAR    test_data_length,
               UCHAR    packet_payload
           );
#endif /* HCI_LE_TRANSMITTER_TEST_COMMAND_SUPPORT */

#ifdef HCI_LE_TEST_END_SUPPORT
/**
 *  \brief To stop any test which is in progress.
 *
 *  \par Description:
 *       This API can be used to stop any test which is in progress.
 *       The Number_Of_Packets for a transmitter test shall be reported as
 *       0x0000. The Number_Of_Packets is an unsigned number and contains
 *       the number of received packets.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_test_end()  \
        hci_common_api_handler_no_params \
        (HCI_LE_TEST_END_OPCODE)
#endif /* HCI_LE_TEST_END_SUPPORT */

#endif /* BT_4_0 */

#ifdef HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT
/**
 *  \brief To read authenticated payload timeout for a connection.
 *
 *  \par Description:
 *       This command reads the Authenticated_Payload_Timeout parameter in the
 *       Primary Controller on the specified Connection_Handle.
 *       When the Connection_Handle is associated with an ACL connection,
 *       the The Link Manager will use this parameter to trigger when to use
 *       the LMP ping sequence. When the Connection_Handle is associated with
 *       an LE connection, the Link Layer will use this parameter to trigger
 *       when to use the LE ping sequence.
 *
 *  \param [in] connection_handle
 *         Connection Handle of the ACL Connection with
 *         the remote Bluetooth device.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_read_authenticated_payload_timeout(connection_handle) \
        hci_write_command_connection_handle \
        ( \
               HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_OPCODE, \
               (connection_handle) \
        )
#endif /* HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT */

#ifdef HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT
/**
 *  \brief To write authenticated payload timeout for a connection.
 *
 *  \par Description:
 *       This command writes the Authenticated_Payload_Timeout parameter in
 *       the Primary Controller for the specified Connection_Handle.
 *       When the Connection_Handle identifies a BR/EDR ACL connection and
 *       the connection is in sniff mode, the Authenticated_Payload_Timeout
 *       shall be equal to or greater than Tsniff. If the connection is in sniff
 *       subrating mode, the Authenticated_Payload_Timeout shall be equal to or
 *       greater than (max subrate)xTsniff. If the connection is put into hold
 *       mode, the Authenticated_Payload_Timeout shall be equal to or greater
 *       than the holdTO value. If the connection is in Park state or if the
 *       Connection_Handle corresponds to a synchronous connection, this command
 *       shall be rejected with the error code Command Disallowed (0x0C).
 *
 *       When the Connection_Handle identifies an LE connection,
 *       the Authenticated_Payload_Timeout shall be equal to or greater than
 *       connInterval * (1 + connSlaveLatency) if the device is master and equal
 *       to or greater than connInterval if the device is slave.
 *
 *       The master of an LE connection may change the connInterval resulting it
 *       to be larger than the Authentication_Payload_Timeout used by LE slave.
 *       In this case, the slave Host may receive one or more Authentication
 *       Payload Timeout Expired events without sending LL_PING_REQ
 *       to the master.
 *
 *  \param [in] connection_handle
 *         Connection Handle of the ACL Connection with
 *         the remote Bluetooth device.
 *
 *  \param [in] authenticated_payload_timeout
 *         Authenticated Payload Timeout.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
API_RESULT BT_hci_write_authenticated_payload_timeout
           (
               /* IN */ UINT16 connection_handle,
               /* IN */ UINT16 authenticated_payload_timeout
           );
#endif /* HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT_SUPPORT */

/* HCI Testing Commands APIs ----------------------------------------------- */

#ifdef HCI_TESTING_COMMANDS
/**
 *  \brief To read the Loopback Mode configuration parameter
 *
 *  \par Description:
 *       This API reads the current value for Loopback Mode configuration
 *       parameter stored in the local Bluetooth device. The setting of the
 *       Loopback Mode will determine the path of information.
 *       In Non-testing Mode operation, the Loopback Mode is set to
 *       Non-testing Mode and the path of the information is as specified by the
 *       Bluetooth specifications. In local Loopback Mode, the local Bluetooth
 *       device sends back every Data Packet (ACL, SCO and eSCO) and
 *       Command Packet that is sent from the Host to the local Bluetooth device.
 *       For details on the Loopback Mode, please refer to the Specification of
 *       the Bluetooth System, v1.2, Vol. 2, Part E - Host Controller Interface
 *       Functional Specification, Section 7.6.1.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_loopback_mode() \
        hci_common_api_handler_no_params (HCI_READ_LOOPBACK_MODE_OPCODE)
/**
 *  \brief To write the Loopback Mode configuration parameter
 *
 *  \par Description:
 *       This API writes the current value for Loopback Mode configuration
 *       parameter to the local Bluetooth device. The setting of the Loopback
 *       Mode will determine the path of information. In Non-testing Mode
 *       operation, the Loopback Mode is set to Non-testing Mode and the path
 *       of the information is as specified by the Bluetooth specifications.
 *       In local Loopback Mode, the local Bluetooth device sends back every
 *       Data Packet (ACL, SCO and eSCO) and Command Packet that is sent from
 *       the Host to the local Bluetooth device.
 *       For details on the Loopback Mode, please refer to the Specification of
 *       the Bluetooth System, v1.2, Vol. 2, Part E - Host Controller Interface
 *       Functional Specification, Section 7.6.2.
 *
 *  \param [in] loopback_mode
 *         The Loopback Mode to be set. The valid values are:
 *         0x00 - No Loopback Mode Enabled
 *         0x01 - Enable Local Loopback Mode
 *         0x02 - Enable Remote Loopback Mode
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 *
 *  \note This API is available only if HCI_TESTING_COMMANDS compilation flag
 *        is enabled during compilation.
 */
API_RESULT BT_hci_write_loopback_mode
           (
               UCHAR     loopback_mode
           );
/**
 *  \brief To allow local Bluetooth device to enter DUT mode.
 *
 *  \par Description:
 *       This API enables the local Bluetooth device to enter Device Under Test
 *       (DUT) mode via the LMP test Commands.
 *       For details on the Device Under Test Mode, please refer to the
 *       Specification of the Bluetooth System, v1.2, Vol. 2,
 *       Part E - Host Controller Interface Functional Specification,
 *       Section 7.6.3.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_enable_device_under_test_mode() \
        hci_common_api_handler_no_params \
        (HCI_ENABLE_DEVICE_UNDER_TEST_MODE_OPCODE)

#endif /* HCI_TESTING_COMMANDS */

#ifdef BT_BRSC_TEST
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_secure_connections_test_mode
           (
               UINT16 connection_handle,
               UCHAR  dm1_acl_u_mode,
               UCHAR  esco_loopback_mode
           );
#endif /* BT_BRSC_TEST */

/* ------------------------------------------------------------------------- */

/* New APIs from Bluetooth Spec 1.2 ---------------------------------------- */

#ifdef BT_HCI_1_2

/* Name Changes in HCI Commands from Bluetooth Specification 1.2 */

#define BT_hci_write_local_name \
        BT_hci_change_local_name

#define BT_hci_read_synchronous_flow_control_enable \
        BT_hci_read_sco_flow_control_enable

#define BT_hci_park_state \
        BT_hci_park_mode

#define BT_hci_exit_park_state \
        BT_hci_exit_park_mode

#define BT_hci_write_synchronous_flow_control_enable \
        BT_hci_write_sco_flow_control_enable

#define BT_hci_set_controller_to_host_flow_control \
        BT_hci_set_host_controller_to_host_flow_control



/* New HCI Commands from Bluetooth Specification 1.2 */
/**
 *  \brief  To cancel ongoing ACL link creation process.
 *
 *  \par Description:
 *       This API can be used to cancel an ongoing ACL link creation process to
 *       a remote Bluetooth device, as started with the
 *       BT_hci_create_connection() API.
 *
 *  \param [in] bd_addr
 *         This is the Bluetooth Device Address for the remote device to which
 *         an attempt was made to create an ACL.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_create_connection_cancel
           (
               UCHAR *   bd_addr
           );
/**
 *  \brief To cancel ongoing remote name request process.
 *
 *  \par Description:
 *       This API cancels ongoing remote name request process to a remote
 *       Bluetooth device, as started with the
 *       BT_hci_remote_name_request() API.
 *
 *  \param [in] bd_addr
 *         This is the Bluetooth Device Address for the remote device to which
 *         an attempt was made to retrieve its name using
 *         BT_hci_remote_name_request() API.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_remote_name_request_cancel
           (
               UCHAR *   bd_addr
           );
/**
 *  \brief To retrieve extended features of the remote Bluetooth device.
 *
 *  \par Description:
 *       This API attempts to retrieve the requested page of the extended LMP
 *       features for the remote device identified by the specified connection
 *       handle.
 *
 *  \param [in] connection_handle
 *         Connection Handle of the ACL Connection with the remote Bluetooth device.
 *
 *  \param [in] page_number
 *         Page Number for LMP Features to retrieve.
 *         The values are:
 *         Value 0x00: Request for normal LMP features as returned by
 *         BT_hci_read_remote_supported_features() API.
 *         Value 0x01-0xFF: Request to return the corresponding page of LMP features.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_read_remote_extended_features
           (
               UINT16    handle,
               UCHAR     page_number
           );
/**
 *  \brief To create or update a Synchronous Connection (eSCO).
 *
 *  \par Description:
 *       This API adds a new or modifies an existing synchronous logical
 *       transport (SCO or eSCO) on a physical link depending on the
 *       Connection Handle parameter specified. If the Connection Handle refers
 *       to an ACL link a new synchronous logical transport will be added.
 *       If the Connection Handle refers to an already existing synchronous
 *       logical transport (eSCO only) this link will be modified.
 *       The parameters are specified per connection. This synchronous
 *       connection can be used to transfer synchronous voice at 64kbps or
 *       transparent synchronous data.
 *       When used to setup a new synchronous logical transport, the Connection
 *       Handle parameter must specify an ACL connection with which the new
 *       synchronous connection will be associated. The other parameters relate
 *       to the negotiation of the link, and may be reconfigured during the
 *       lifetime of the link. Transmit and receive bandwidth specify how much
 *       bandwidth shall be available for transmitting and for receiving data.
 *       While in many cases transmit and receive bandwidth parameters may be
 *       equal, they may be different. The latency specifies an upper limit to
 *       the time in milliseconds between the eSCO (or SCO) instants,
 *       plus the size of the retransmission window, plus the length of the
 *       reserved synchronous slots for this logical transport. The content
 *       format specifies the settings for voice or transparent data on this
 *       connection. The Retransmission Effort specifies the extra resources
 *       that are allocated to this connection if a packet may need to be
 *       retransmitted.
 *       The Retransmission Effort parameter must be set to indicate the
 *       required behaviour, or to "don't care". When used to modify an
 *       existing synchronous logical transport, the Transmit Bandwidth,
 *       Receive Bandwidth and Voice Settings shall be set to the same values
 *       as were used during the initial setup. The Packet Type, Retransmission
 *       Effort and Max Latency parameters may be modified.
 *       The Packet Type field is a bitmap specifying which packet types the
 *       LM shall accept in the negotiation of the link parameters.
 *       Multiple packet types are specified by bitwise OR of the packet
 *       type codes in the table. At least one packet type must be specified
 *       for each negotiation. It is recommended to enable as many packet
 *       types as possible. Note that it is allowed to enable packet types that
 *       are not supported by the local device.
 *       The Connection Handle for the new synchronous connection will be
 *       returned in the synchronous connection complete event.
 *       The local Bluetooth device starts the Link Manager procedure to add or
 *       modify an eSCO link on reception of the HCI Setup Synchronous Connection
 *       Command, and sends a Command Status Event to the HCI. At the end of the
 *       link creation process, the Synchronous Connection Complete Event is received.
 *       For details on the Bluetooth Synchronous link establishment, refer to
 *       the Specification of the Bluetooth System, v1.2, Vol. 2,
 *       Part F - Message Sequence Charts.
 *
 *  \param [in] handle
 *         The Connection Handle of the ACL, or existing SCO/eSCO link
 *
 *  \param [in] sco_in_params
 *         Pointer to caller allocated HCI_SCO_IN_PARAMS variable containing the
 *         input parameters for negotiating the new or existing synchronous link.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 *
 *  \note The link manager may choose any combination of packet types, timing,
 *        and retransmission window sizes that satisfy the parameters given.
 *        This may be achieved by using more frequent transmissions of smaller packets.
 *        The link manager may choose to set up either a SCO or an eSCO connection,
 *        if the parameters allow, using the corresponding LMP sequences.
 *        To modify a SCO connection, use the BT_hci_change_connection_packet_type() API.
 *        If the lower layers cannot achieve the exact transmit and receive
 *        bandwidth requested subject to the other parameters, then the link
 *        will be rejected.
 *        A synchronous connection may only be created when an ACL connection
 *        already exists and when it is not in park state.
 *
 */
API_RESULT BT_hci_setup_synchronous_connection
           (
               UINT16                handle,
               HCI_SCO_IN_PARAMS *   sco_in_params
           );

#ifdef HCI_ENH_SCO
/**
 *  \brief To create or update a Synchronous Connection (eSCO).
 *
 *  \par Description:
 *       This API adds a new or modifies an existing synchronous logical
 *       transport (SCO or eSCO) on a physical link depending on the Connection
 *       Handle parameter specified. If the Connection Handle refers to an ACL
 *       link a new synchronous logical transport will be added.
 *       If the Connection Handle refers to an already existing synchronous
 *       logical transport (eSCO only) this link will be modified.
 *       The parameters are specified per connection.
 *       For details on the Bluetooth Enhanced Synchronous link establishment,
 *       refer to the Specification of the Bluetooth System.
 *
 *  \param [in] handle
 *         The Connection Handle of the ACL, or existing SCO/eSCO link.
 *
 *  \param [in] enh_sco_params
 *         Pointer to caller allocated HCI_ENH_SCO_PARAMS variable containing
 *         the input parameters for negotiating the new or existing
 *         synchronous link.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 *
 *  \note The link manager may choose any combination of packet types, timing,
 *        and retransmission window sizes that satisfy the parameters given.
 *        This may be achieved by using more frequent transmissions of smaller
 *        packets. The link manager may choose to set up either a SCO or an eSCO
 *        connection, if the parameters allow, using the corresponding
 *        LMP sequences.
 *        If the lower layers cannot achieve the exact transmit and receive
 *        bandwidth requested subject to the other parameters, then the link
 *        will be rejected.
 *        A synchronous connection may only be created when an ACL connection
 *        already exists and when it is not in park state.
 */
API_RESULT BT_hci_setup_enh_synchronous_connection
           (
               UINT16                handle,
               HCI_ENH_SCO_PARAMS  * enh_sco_params
           );
#endif /* HCI_ENH_SCO */
/**
 *  \brief To specify the flow parameters for the traffic carried over a connection.
 *
 *  \par Description:
 *       This API enables applications to specify the flow parameters for the
 *       traffic, both outgoing and incoming, carried over an ACL connection
 *       to a remote Bluetooth device.
 *       The local Bluetooth device starts the Flow Specification process to
 *       negotiate flow parameters with the remote device on reception of the
 *       HCI Flow Specification Command
 *
 *  \param [in] connection_handle
 *         Connection Handle of the ACL link for which flow parameters to be
 *         specified.
 *
 *  \param [in] Qos_requested
 *         This parameter must be a pointer to the HCI_QOS data type containing
 *         the requested flow parameters to be negotiated and specified for the
 *         ACL link with the remote Bluetooth device, for the specified flow
 *         direction (incoming or outgoing).
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_flow_specification
           (
               UINT16       handle,
               HCI_QOS *    qos_requested
           );
/**
 *  \brief To read the Default Link Policy Settings of the Bluetooth device.
 *
 *  \par Description:
 *       This API reads the default Link Policy Settings for all new connections.
 *       The Link Policy Settings parameter determines the behavior of the local
 *       Bluetooth device when it receives a request from a remote device or it
 *       decides to change the Master-Slave role or to enter the Hold, Sniff,
 *       or Park mode. The local Bluetooth device will automatically accept or
 *       reject such a request from the remote device, and may even autonomously
 *       request itself based on the value of the Link Policy Settings parameter.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_default_link_policy_settings() \
        hci_common_api_handler_no_params \
        (HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_OPCODE)
/**
 *  \brief To write default Link Policy Settings to the Bluetooth device.
 *
 *  \par Description:
 *       This API writes the default Link Policy setting for all new connections.
 *       The Link Policy Settings parameter determines the behavior of the local
 *       Bluetooth device when it receives a request from a remote device or it
 *       decides to change the Master-Slave role or to enter the Hold, Sniff,
 *       or Park mode. The local Bluetooth device will automatically accept or
 *       reject such a request from the remote device, and may even autonomously
 *       request itself based on the value of the Link Policy Settings parameter.
 *
 *  \param [in] link_policy_settings
 *         The Default Link Policy Settings to be set.
 *         Value 0x0000: Disable all LM modes. (Default)
 *         Value 0x0001: Enable Role switch.
 *         Value 0x0002: Enable Hold Mode.
 *         Value 0x0004: Enable Sniff Mode.
 *         Value 0x0008: Enable Park State.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_write_default_link_policy_settings(p1) \
        hci_write_command_UINT16 \
        (HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_OPCODE, (p1))
/**
 *  \brief To specify local chanel classification.
 *
 *  \par Description:
 *       This API allows user to specify AFH Channel Classification to local
 *       Bluetooth Controller, based on local information. This classification,
 *       once written successfully, persists until overwritten again later.
 *       If this API is used, then updates should be sent within 10 seconds,
 *       of the host knowing that the channel classification has changed.
 *       The interval between two successive updates sent must be at least
 *       1 second.
 *
 *  \param [in] channel_classification
 *         Pointer to an user allocated 10 octet array, containing a bitmap of
 *         79 bits, representing the 79 Bluetooth channels.
 *         The 79 1-bit fields of the bitmap is interpreted in the following
 *         manner - the nth such field (in the range 0 to 78) contains the
 *         value for channel n:
 *         0 = the Channel n is bad
 *         1 = the Channel n is unknown
 *         The most significant bit is reserved and shall be set to 0. At least
 *         20 channels must be marked as unknown.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_set_afh_host_channel_classification
           (
               UCHAR *  channel_params
           );

#ifndef HCI_LITE
/**
 *  \brief To read the Inquiry Scan Type configuration parameter.
 *
 *  \par Description:
 *       This API reads the current value for Inquiry Scan Type configuration
 *       parameter stored in the local Bluetooth device.
 *       The Inquiry Scan Type configuration parameter controls whether the
 *       local Bluetooth device performs Standard Scan or Interlaces Scan
 *       (faster) during inquiry.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_inquiry_scan_type() \
        hci_common_api_handler_no_params (HCI_READ_INQUIRY_SCAN_TYPE_OPCODE)
#endif /* HCI_LITE */

/**
 *  \brief To write the Inquiry Scan Type configuration parameter
 *
 *  \par Description:
 *       This API writes the current value for Inquiry Scan Type configuration
 *       parameter to be stored in the local Bluetooth device. The Inquiry Scan
 *       Type configuration parameter controls whether the local Bluetooth
 *       device performs Standard Scan or Interlaces Scan (faster) during inquiry.
 *
 *  \param [in] inquiry_scan_type
 *         The Inquiry Scan Type to be set. The valid values are:
 *         0x00 - Standard Scan
 *         0x01 - Interlaced Scan
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_write_inquiry_scan_type(p1) \
        hci_write_command_UCHAR (HCI_WRITE_INQUIRY_SCAN_TYPE_OPCODE, (p1))

#ifndef HCI_LITE
/**
 *  \brief To read the Inquiry Mode configuration parameter.
 *
 *  \par Description:
 *       This API reads the current value for Inquiry Mode configuration
 *       parameter stored in the local Bluetooth device. The Inquiry Mode
 *       configuration parameter control whether Bluetooth Inquiry results to be
 *       delivered to host with standard format or with RSSI data.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_inquiry_mode() \
        hci_common_api_handler_no_params (HCI_READ_INQUIRY_MODE_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To write the Inquiry Mode configuration parameter
 *
 *  \par Description:
 *       This API writes the current value for Inquiry Mode configuration
 *       parameter to be stored in the local Bluetooth device. The Inquiry Mode
 *       configuration parameter control whether Bluetooth Inquiry results to be
 *       delivered to host with standard format or with RSSI data.
 *
 *  \param [in] inq_mode
 *         The Inquiry Mode to be set.
 *         The valid values are:
 *         0x00 - Standard Inquiry Result Event format
 *         0x01 - Inquiry Result with RSSI Event format
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_inquiry_mode
           (
               UCHAR     inq_mode
           );

#ifndef HCI_LITE
/**
 *  \brief To read the Page Scan Type configuration parameter
 *
 *  \par Description:
 *       This API reads the current value for Page Scan Type configuration
 *       parameter stored in the local Bluetooth device. The Page Scan Type
 *       configuration parameter controls whether the local Bluetooth device
 *       performs Standard Scan or Interlaces Scan (faster) during paging.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_page_scan_type() \
        hci_common_api_handler_no_params (HCI_READ_PAGE_SCAN_TYPE_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To write the Page Scan Type configuration parameter
 *
 *  \par Description:
 *       This API writes the current value for Page Scan Type configuration
 *       parameter to be stored in the local Bluetooth device.
 *       The Page Scan Type configuration parameter controls whether the
 *       local Bluetooth device performs Standard Scan or Interlaces Scan
 *       (faster) during paging.
 *
 *  \param [in] page_scan_type
 *         The Page Scan Type to be set. The valid values are:
 *         0x00 - Standard Scan
 *         0x01 - Interlaced Scan
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_write_page_scan_type(p1) \
        hci_write_command_UCHAR (HCI_WRITE_PAGE_SCAN_TYPE_OPCODE, (p1))

#ifndef HCI_LITE
/**
 *  \brief To read the AFH Channel Assessment Mode configuration parameter.
 *
 *  \par Description:
 *       This API reads the AFH Channel Assessment Mode configuration parameter
 *       from the local Bluetooth device. The AFH Channel Assessment Mode
 *       configuration parameter controls whether the controller's channel
 *       assessment scheme is enabled or disabled.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_afh_channel_assessment_mode() \
        hci_common_api_handler_no_params \
        (HCI_READ_AFH_CHANNEL_ASSESSMENT_MODE_OPCODE)
#endif /* HCI_LITE */
/**
 *  \brief To write the AFH Channel Assessment Mode configuration parameter.
 *
 *  \par Description:
 *       This API writes the AFH Channel Assessment Mode configuration parameter
 *       to the local Bluetooth device. The AFH Channel Assessment Mode
 *       configuration parameter controls whether the controller's channel
 *       assessment scheme is enabled or disabled.
 *
 *  \param [in] afh_mode
 *         The AFH Channel Assessment mode to be written.
 *         The valid values are:
 *         Value 0x00 - Controller's channel assessment disabled
 *         Value 0x01 - Controller's channel assessment enabled
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_afh_channel_assessment_mode
           (
               UCHAR    afh_mode
           );
/**
 *  \brief To read the AFH Mode and AFH Channel Map for a specified connection handle
 *
 *  \par Description:
 *       This API allows user to read the AFH Mode and AFH Channel Map for a
 *       specified ACL connection handle.
 *
 *  \param [in] connection_handle
 *         The ACL Connection Handle for which the Channel Map to be read.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_read_afh_channel_map
           (
               UINT16    handle
           );

#ifdef HCI_READ_LOCAL_SUPPORTED_COMMANDS_SUPPORT
/**
 *  \brief To read list of supported HCI Commands from the local Bluetooth
 *         device.
 *
 *  \par Description:
 *       This API reads the list of supported HCI commands from the Link
 *       Manager Protocol of the local Bluetooth device.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_read_local_supported_commands() \
        hci_common_api_handler_no_params \
        (HCI_READ_LOCAL_SUPPORTED_COMMANDS_OPCODE)
#endif /* HCI_READ_LOCAL_SUPPORTED_COMMANDS_SUPPORT */
/**
 *  \brief To retrieve extended features of the local Bluetooth device.
 *  \par Description:
 *       This API attempts to retrieve the requested page of the extended LMP
 *       features for the local Bluetooth device.
 *
 *  \param [in] page_number
 *         Page Number for LMP Features to retrieve.
 *         The values are:
 *         Value 0x00: Request for normal LMP features as returned by
 *         BT_hci_read_local_supported_features() API.
 *         Value 0x01-0xFF: Request to return the corresponding page of LMP features.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_read_local_extended_features
           (
               UCHAR     page_number
           );
/**
 *  \brief To retrieve clock offset of the local Bluetooth device with respect
 *         to the remote one.
 *
 *  \par Description:
 *       This API attempts to retrieve the clock offset of the local Bluetooth
 *       device with respect to the remote one, as identified by the supplied
 *       ACL Connection Handle.
 *       Both the System Clock and the Clock Offset to a remote Bluetooth device
 *       is used to determine the hopping frequency used by a remote device for
 *       page scan. This command allows the Host to read clock offset to remote
 *       devices. The clock offset can be used to speed up the paging procedure
 *       when the local device tries to establish a connection with a remote
 *       device, for example, when the local Host has issued Create Connection
 *       or Remote Name Request.
 *
 *  \param [in] connection_handle
 *         Connection Handle of the ACL Connection with the remote Bluetooth
 *         device with respect to which the clock offset needs to be determined.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_read_clock
           (
               UCHAR     which_clock,
               UINT16    handle
           );

#endif /* BT_HCI_1_2 */

#ifdef HCI_LL_TOPOLOGY_CONN_UPDATE_SUPPORT
/**
 *  \brief To accept change connection parameter request
 *
 *  \par Description:
 *       This API can be used by both the master Host and the slave Host
 *       to reply to the HCI LE Remote Connection Parameter Request event.
 *       This indicates that the Host has accepted the remote device's
 *       request to change connection parameters.
 *
 *       The Interval_Min and Interval_Max parameters define the minimum and
 *       maximum allowed connection interval. The Interval_Min parameter shall
 *       not be greater than the Interval_Max parameter.
 *
 *       The Latency parameter shall define the maximum allowed slave latency
 *       for the connection in number of connection events.
 *
 *       The Timeout parameter shall define the link supervision timeout for
 *       the LE link.  The Timeout in milliseconds shall be larger than
 *       (1 + Latency) * Interval_Max * 2, where Interval_Max is given in
 *       milliseconds.
 *
 *       The Minimum_CE_Length and Maximum_CE_Length are information parameters
 *       providing the Controller with a hint about the expected minimum and
 *       maximum length of the connection events. The Minimum_CE_Length shall
 *       be less than or equal to the Maximum_CE_Length.
 *
 *       The actual parameter values selected by the Link Layer may be different
 *       from the parameter values provided by the Host through this command.
 *
 *  \param [in] connection_handle
 *         Connection Handle for the existing BLE connection.
 *
 *  \param [in] conn_interval_min
 *         Minimum value for the connection interval.
 *         This shall be less than or equal to Conn_Interval_Max.
 *
 *  \param [in] conn_interval_max
 *         Maximum value for the connection interval.
 *         This shall be greater than or equal to Conn_Interval_Min.
 *
 *  \param [in] conn_latency
 *         Maximum allowed slave latency for the connection specified as the
 *         number of connection events.
 *
 *  \param [in] supervision_timeout
 *         Supervision Timeout for the connection.
 *
 *  \param [in] minimum_ce_length
 *         Information parameter about the minimum length of connection
 *         needed for this LE connection.
 *
 *  \param [in] maximum_ce_length
 *         Information parameter about the maximum length of connection
 *         needed for this LE connection.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
API_RESULT BT_hci_le_remote_conn_param_req_reply
           (
               UINT16   connection_handle,
               UINT16   conn_interval_min,
               UINT16   conn_interval_max,
               UINT16   conn_latency,
               UINT16   supervision_timeout,
               UINT16   minimum_ce_length,
               UINT16   maximum_ce_length
           );

/**
 *  \brief To reject change connection parameter request
 *
 *  \par Description:
 *       This API can be used by both the master Host and the slave Host
 *       to reply to the HCI LE Remote Connection Parameter Request event.
 *       This indicates that the Host has rejected the remote device's request
 *       to change connection parameters.
 *       The reason for the rejection is given in the Reason parameter.
 *
 *       Instead of issuing this command, the Host should try to provide alternative
 *       connection parameters to the Link Layer via the HCI LE Remote Connection
 *       Parameter Request Reply command
 *
 *  \param [in] connection_handle
 *         Connection Handle for the existing BLE connection.
 *
 *  \param [in] reason
 *         This specifies the reason for rejecting connection parameter
 *         update request.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
API_RESULT BT_hci_le_remote_conn_param_req_negtive_reply
           (
               UINT16   connection_handle,
               UCHAR    reason
           );
#endif /* HCI_LL_TOPOLOGY_CONN_UPDATE_SUPPORT */

/* New APIs from Bluetooth Spec 4.1 ---------------------------------------- */
#ifdef HCI_TRUNCATED_PAGE_SUPPORT
/**
 *  \brief To page the BR/EDR Controller with the specified BD_ADDR and
 *         then abort the paging sequence after an ID response has been
 *         received.
 *
 *  \par Description:
 *       This command is used to page the BR/EDR Controller with the
 *       specified BD_ADDR and then abort the paging sequence after
 *       an ID response has been received.
 *
 *  \param [in] bd_addr
 *              BD_ADDR of the Device to page
 *
 *  \param [in] page_scan_repetition_mode
 *              0x00: R0
 *              0x01: R1
 *              0x02: R2
 *              0x03-0xFF: Reserved
 *
 *  \param [in] clock_offset
 *              Bits 14-0: Bits 16-2 of CLKNslave-CLK
 *              Bit 15: 0 = Clock_Offset is invalid, 1 = Clock_Offset is
 *              valid
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_truncated_page
           (
               UCHAR * bd_addr,
               UCHAR  page_scan_repetition_mode,
               UINT16 clock_offset
           );
#endif /* HCI_TRUNCATED_PAGE_SUPPORT */

#ifdef HCI_TRUNCATED_PAGE_CANCEL_SUPPORT
/**
 *  \brief To request cancellation of an ongoing Truncated_Page process
 *         previously started by a Truncated_Page command.
 *
 *  \par Description:
 *       This is used to request cancellation of an ongoing
 *       Truncated_Page process previously started by a
 *       Truncated_Page command.
 *
 *  \param [in] bd_addr
 *              BD_ADDR of the device to which the Truncated Page command
 *              was previously issued and that is the subject of the
 *              cancellation request.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_truncated_page_cancel
           (
               UCHAR * bd_addr
           );
#endif /* HCI_TRUNCATED_PAGE_CANCEL_SUPPORT */

#ifdef HCI_SET_CONNECTIONLESS_SLAVE_BROADCAST_SUPPORT
/**
 *  \brief To control the Connectionless Slave Broadcast functionality
 *         in the BR/EDR Controller
 *
 *  \par Description:
 *       This command controls the Connectionless Slave Broadcast
 *       functionality in the BR/EDR Controller.
 *
 *  \param [in] enable
 *              0x00: Disabled
 *              0x01: Enabled
 *
 *  \param [in] lt_addr
 *              0x01-0x07: LT_ADDR used for Connectionless Slave Broadcast
 *              0x00, 0x08-0xFF: Reserved for future usage
 *
 *  \param [in] lop_allowed
 *              0x00: BR/EDR Controller shall not sleep (that is, clock
 *              accuracy shall be equal to or better than +|- 20 ppm)
 *              0x01: BR/EDR Controller may sleep (that is, clock accuracy
 *              shall be equal to or better than +|- 250 ppm)
 *
 *  \param [in] packet_type
 *              Packet Type
 *
 *  \param [in] interval_min
 *              Minimum interval between Connectionless Slave Broadcast
 *              packets in slots.
 *              Range: 0x0002-0xFFFE; only even values are valid
 *
 *  \param [in] interval_max
 *              Maximum interval between Connectionless Slave Broadcast
 *              packets in slots.
 *              Range: 0x0002-0xFFFE; only even values are valid
 *
 *  \param [in] csb_supervisionto
 *              Duration in slots after which the BR/EDR Controller reports
 *              a Connectionless Slave Broadcast Timeout event if it is
 *              unable to transmit a Connectionless Slave Broadcast packet.
 *              Range: 0x0002-0xFFFE; only even values are valid
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_set_connectionless_slave_broadcast
           (
               UCHAR  enable,
               UCHAR  lt_addr,
               UCHAR  lop_allowed,
               UINT16 packet_type,
               UINT16 interval_min,
               UINT16 interval_max,
               UINT16 csb_supervisionto
           );
#endif /* HCI_SET_CONNECTIONLESS_SLAVE_BROADCAST_SUPPORT */

#ifdef HCI_SET_CONNECTIONLESS_SLAVE_BROADCAST_RECEIVE_SUPPORT
/**
 *  \brief To control the reception of Connectionless Slave Broadcast
 *         packets in the BR/EDR Controller of a Connectionless Slave
 *         Broadcast slave.
 *
 *  \par Description:
 *       This command controls the reception of Connectionless Slave
 *       Broadcast packets in the BR/EDR Controller of a
 *       Connectionless Slave Broadcast slave.
 *
 *  \param [in] enable
 *              0x00: Disabled
 *              0x01: Enabled
 *
 *  \param [in] bd_addr
 *              BD_ADDR of the Connectionless Slave Broadcast transmitter
 *
 *  \param [in] lt_addr
 *              0x01-0x07: LT_ADDR used for receiving Connectionless Slave
 *              Broadcast messages
 *              0x00, 0x08-0xFF: Reserved for future usage
 *
 *  \param [in] interval
 *              Interval between Connectionless Slave Broadcast packets
 *              instants in slots.
 *              Range: 0x0002-0xFFFE; only even values are valid
 *
 *  \param [in] clock_offset
 *              (CLKNslave - CLK) modulo 2^28 (28 bits meaningful)
 *
 *  \param [in] next_connectionless_slave_broadcast_clock
 *              CLK for next Connectionless Slave Broadcast instant (28 bits
 *              meaningful)
 *
 *  \param [in] csb_supervisionto
 *              Duration in slots to continue listening for Connectionless
 *              Slave Broadcast packets after the last successfully received
 *              Connectionless Slave Broadcast packet.
 *              Range: 0x0002-0xFFFE; only even values are valid
 *
 *  \param [in] remote_timing_accuracy
 *              Timing accuracy of the master in ppm. Typical values are
 *              20ppm and 250ppm.
 *
 *  \param [in] skip
 *              Number of Connectionless Slave Broadcast instants to skip
 *              after successfully receiving a Broadcast packet.
 *
 *  \param [in] packet_type
 *              Packet Type
 *
 *  \param [in] ahf_channel_map
 *              This parameter contains 80 1-bit fields.
 *              The nth such field (in the range 0 to 78) contains the value
 *              for channel n:
 *              Channel n is bad = 0
 *              Channel n is unknown = 1
 *              The most significant bit (bit 79) is reserved and shall be
 *              set to 0
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_set_connectionless_slave_broadcast_receive
           (
               UCHAR  enable,
               UCHAR * bd_addr,
               UCHAR  lt_addr,
               UINT16 interval,
               UINT32 clock_offset,
               UINT32 next_connectionless_slave_broadcast_clock,
               UINT16 csb_supervisionto,
               UCHAR  remote_timing_accuracy,
               UCHAR  skip,
               UINT16 packet_type,
               UCHAR * ahf_channel_map
           );
#endif /* HCI_SET_CONNECTIONLESS_SLAVE_BROADCAST_RECEIVE_SUPPORT */

#ifdef HCI_START_SYNCHRONIZATION_TRAIN_SUPPORT
/**
 *  \brief To control the Synchronization Train functionality in the
 *         BR/EDR Controller
 *
 *  \par Description:
 *       This command controls the Synchronization Train
 *       functionality in the BR/EDR Controller.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
#define BT_hci_start_synchronization_train() \
        hci_common_api_handler_no_params \
        (HCI_START_SYNCHRONIZATION_TRAIN_OPCODE)
#endif /* HCI_START_SYNCHRONIZATION_TRAIN_SUPPORT */

#ifdef HCI_RECEIVE_SYNCHRONIZATION_TRAIN_SUPPORT
/**
 *  \brief To control the Synchronization Train functionality in the
 *         BR/EDR Controller
 *
 *  \par Description:
 *       This command controls the Synchronization Train
 *       functionality in the BR/EDR Controller.
 *
 *  \param [in] bd_addr
 *              BD_ADDR of the Connectionless Slave Broadcast transmitter
 *
 *  \param [in] synchronization_scanto
 *              Duration in slots to search for the synchronization train.
 *              Range: 0x0002-0xFFFE; only even values are valid
 *
 *  \param [in] sync_scan_window
 *              Duration in slots to listen for a synchronization train
 *              packet on a single frequency.
 *              Range: 0x0002-0xFFFE; only even values are valid
 *
 *  \param [in] sync_scan_interval
 *              Duration in slots between the start of consecutive scan
 *              windows.
 *              Range: 0x0002-0xFFFE; only even values are valid
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_receive_synchronization_train
           (
               UCHAR * bd_addr,
               UINT16 synchronization_scanto,
               UINT16 sync_scan_window,
               UINT16 sync_scan_interval
           );
#endif /* HCI_RECEIVE_SYNCHRONIZATION_TRAIN_SUPPORT */

#ifdef HCI_SET_RESERVED_LT_ADDR_SUPPORT
/**
 *  \brief To request that the BR/EDR Controller reserve a specific
 *         LT_ADDR for Connectionless Slave Broadcast.
 *
 *  \par Description:
 *       This command allows the Host to request that the BR/EDR
 *       Controller reserve a specific LT_ADDR for Connectionless
 *       Slave Broadcast.
 *
 *  \param [in] lt_addr
 *              0x01-0x07: LT_ADDR to reserve for Connectionless Slave
 *              Broadcast
 *              0x00, 0x08-0xFF: Reserved for future usage
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_set_reserved_lt_addr
           (
               UCHAR  lt_addr
           );
#endif /* HCI_SET_RESERVED_LT_ADDR_SUPPORT */

#ifdef HCI_DELETE_RESERVED_LT_ADDR_SUPPORT
/**
 *  \brief To request that the BR/EDR Controller cancel the reservation
 *         for a specific LT_ADDR reserved for the purposes of
 *         Connectionless Slave Broadcast.
 *
 *  \par Description:
 *       This command requests that the BR/EDR Controller cancel the
 *       reservation for a specific LT_ADDR reserved for the purposes
 *       of Connectionless Slave Broadcast.
 *
 *  \param [in] lt_addr
 *              0x01-0x07: LT_ADDR currently reserved for Connectionless
 *              Slave Broadcast and for which reservation is to be cancelled
 *              0x00, 0x08-0xFF: Reserved for future usage
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_delete_reserved_lt_addr
           (
               UCHAR  lt_addr
           );
#endif /* HCI_DELETE_RESERVED_LT_ADDR_SUPPORT */

#ifdef HCI_SET_CONNECTIONLESS_SLAVE_BROADCAST_DATA_SUPPORT
/**
 *  \brief To set Connectionless Slave Broadcast data in the BR/EDR
 *         Controller.
 *
 *  \par Description:
 *       This command provides the ability for the Host to set
 *       Connectionless Slave Broadcast data in the BR/EDR
 *       Controller. This command may be issued at any time after an
 *       LT_ADDR has been reserved regardless of whether
 *       connectionless slave broadcast mode has been enabled or
 *       disabled by the Enable parameter in the
 *       Set_Connectionless_Slave_Broadcast command.
 *
 *  \param [in] lt_addr
 *              0x01-0x07: LT_ADDR on which to send Connectionless Slave
 *              Broadcast data
 *              0x00, 0x08-0xFF: Reserved for future usage
 *
 *  \param [in] fragment
 *              0x00: Continuation fragment
 *              0x01: Starting fragment
 *              0x02: Ending fragment
 *              0x03: No fragmentation (single fragment)
 *              0x04-0xFF: Reserved for future use
 *
 *  \param [in] data_length
 *              Length of the Data field
 *
 *  \param [in] data
 *              Data to send in future Connectionless Slave Broadcast
 *              packets.  This data will be repeated in future
 *              Connectionless Slave Broadcast instants until new data is
 *              provided
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_set_connectionless_slave_broadcast_data
           (
               UCHAR  lt_addr,
               UCHAR  fragment,
               UCHAR  data_length,
               UCHAR * data
           );
#endif /* HCI_SET_CONNECTIONLESS_SLAVE_BROADCAST_DATA_SUPPORT */

#ifdef HCI_READ_SYNCHRONIZATION_TRAIN_PARAMETERS_SUPPORT
/**
 *  \brief To read the currently configured values for the
 *         Synchronization Train functionality in the master's BR/EDR
 *         Controller.
 *
 *  \par Description:
 *       This command returns the currently configured values for the
 *       Synchronization Train functionality in the master's BR/EDR
 *       Controller. This command may be issued at any time.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
#define BT_hci_read_synchronization_train_parameters() \
        hci_common_api_handler_no_params \
        (HCI_READ_SYNCHRONIZATION_TRAIN_PARAMETERS_OPCODE)
#endif /* HCI_READ_SYNCHRONIZATION_TRAIN_PARAMETERS_SUPPORT */

#ifdef HCI_WRITE_SYNCHRONIZATION_TRAIN_PARAMETERS_SUPPORT
/**
 *  \brief To configure the Synchronization Train functionality in the
 *         BR/EDR Controller.
 *
 *  \par Description:
 *       This command configures the Synchronization Train
 *       functionality in the BR/EDR Controller. This command may be
 *       issued at any time.
 *
 *  \param [in] interval_min
 *              Minimum value allowed for the interval Sync_Train_Interval
 *              in slots.
 *              Range: 0x0002-0xFFFE; only even values are valid
 *
 *  \param [in] interval_max
 *              Maximum value allowed for the interval Sync_Train_Interval
 *              in slots.
 *              Range: 0x0002-0xFFFE; only even values are valid
 *
 *  \param [in] synchronization_trainto
 *              Duration in slots to continue sending the synchronization
 *              train.
 *              Range: 0x00000002-0x7FFFFFFE; only even values are valid
 *
 *  \param [in] service_data
 *              Host provided value to be included in octet 27 of the
 *              Synchronization Train packet payload body
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_write_synchronization_train_parameters
           (
               UINT16 interval_min,
               UINT16 interval_max,
               UINT32 synchronization_trainto,
               UCHAR  service_data
           );
#endif /* HCI_WRITE_SYNCHRONIZATION_TRAIN_PARAMETERS_SUPPORT */


#ifdef HCI_REMOTE_OOB_EXTENDED_DATA_REQUEST_REPLY_SUPPORT
/**
 *  \brief To reply to an HCI_Remote_OOB_Data_Request event.
 *
 *  \par Description:
 *       This command is used to reply to an
 *       HCI_Remote_OOB_Data_Request event with the C and R values
 *       derived with the P-192 public key and the C and R values
 *       associated with the P-256 public key received via an OOB
 *       transfer from a remote device identified by BD_ADDR.
 *
 *  \param [in] bd_addr
 *              BD_ADDR of remote device from which the C and R values were
 *              received
 *
 *  \param [in] c_192
 *              Simple Pairing Hash C derived from the P-192 public key.
 *
 *  \param [in] r_192
 *              Simple Pairing Randomizer associated with the P-192 public
 *              key.
 *
 *  \param [in] c_256
 *              Simple Pairing Hash C derived from the P-256 public key.
 *
 *  \param [in] r_256
 *              Simple Pairing Randomizer associated with the P-256 public
 *              key.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_remote_oob_extended_data_request_reply
           (
               UCHAR * bd_addr,
               UCHAR * c_192,
               UCHAR * r_192,
               UCHAR * c_256,
               UCHAR * r_256
           );
#endif /* HCI_REMOTE_OOB_EXTENDED_DATA_REQUEST_REPLY_SUPPORT */

#ifdef HCI_SET_MWS_CHANNEL_PARAMETERS_SUPPORT
/**
 *  \brief To inform the Controller of the MWS channel parameters.
 *
 *  \par Description:
 *       This command is used to inform the Controller of the MWS
 *       channel parameters.
 *
 *  \param [in] mws_channel_enable
 *              0x00: MWS channel is disabled.
 *              0x01: MWS channel is enabled.
 *
 *  \param [in] mws_rx_center_frequency
 *              MWS RX center frequency in MHz.
 *
 *  \param [in] mws_tx_center_frequency
 *              MWS TX center frequency in MHz.
 *
 *  \param [in] mws_rx_channel_bandwidth
 *              MWS RX channel bandwidth in kHz.
 *
 *  \param [in] mws_tx_channel_bandwidth
 *              MWS TX channel bandwidth in kHz.
 *
 *  \param [in] mws_channel_type
 *              See Assigned Numbers.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_set_mws_channel_parameters
           (
               UCHAR  mws_channel_enable,
               UINT16 mws_rx_center_frequency,
               UINT16 mws_tx_center_frequency,
               UINT16 mws_rx_channel_bandwidth,
               UINT16 mws_tx_channel_bandwidth,
               UCHAR  mws_channel_type
           );
#endif /* HCI_SET_MWS_CHANNEL_PARAMETERS_SUPPORT */

#ifdef HCI_SET_EXTERNAL_FRAME_CONFIGURATION_SUPPORT
/**
 *  \brief To specify a frame configuration for an external collocated
 *         MWS system.
 *
 *  \par Description:
 *       This command allows the Host to specify a frame
 *       configuration for an external collocated MWS system.
 *
 *  \param [in] mws_frame_duration
 *              External frame duration in microseconds
 *
 *  \param [in] mws_frame_sync_assert_offset
 *              External frame offset in microseconds (signed integer).
 *
 *  \param [in] mws_frame_sync_assert_jitter
 *              External frame sync jitter in microseconds (unsigned
 *              integer)
 *
 *  \param [in] mws_num_periods
 *              Number of specified periods in an external frame.
 *
 *  \param [in] period_duration
 *              Duration of the [i] period in microseconds.
 *
 *  \param [in] period_type
 *              0x00: Downlink
 *              0x01: Uplink
 *              0x02: Bi-Directional
 *              0x03: Guard Period
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_set_external_frame_configuration
           (
               UINT16 mws_frame_duration,
               UINT16 mws_frame_sync_assert_offset,
               UINT16 mws_frame_sync_assert_jitter,
               UCHAR  mws_num_periods,
               UINT16 * period_duration,
               UCHAR * period_type
           );
#endif /* HCI_SET_EXTERNAL_FRAME_CONFIGURATION_SUPPORT */

#ifdef HCI_SET_MWS_SIGNALING_SUPPORT
/**
 *  \brief To inform the Bluetooth Controller of the MWS signaling
 *         interface logical layer parameters.
 *
 *  \par Description:
 *       This command informs the Bluetooth Controller of the MWS
 *       signaling interface logical layer parameters.
 *
 *  \param [in] mws_rx_assert_offset
 *              MWS_RX signal assert offset in microseconds (signed
 *              integer).
 *
 *  \param [in] mws_rx_assert_jitter
 *              MWS_RX signal assert jitter in microseconds (unsigned
 *              integer).
 *
 *  \param [in] mws_rx_deassert_offset
 *              MWS_RX signal de-assert offset in microseconds (signed
 *              integer).
 *
 *  \param [in] mws_rx_deassert_jitter
 *              MWS_RX signal de-assert jitter in microseconds (unsigned
 *              integer).
 *
 *  \param [in] mws_tx_assert_offset
 *              MWS_TX signal assert offset in microseconds (signed
 *              integer).
 *
 *  \param [in] mws_tx_assert_jitter
 *              MWS_TX signal assert jitter in microseconds (unsigned
 *              integer).
 *
 *  \param [in] mws_tx_deassert_offset
 *              MWS_TX signal de-assert offset in microseconds (signed
 *              integer).
 *
 *  \param [in] mws_tx_deassert_jitter
 *              MWS_TX signal de-assert jitter in microseconds (unsigned
 *              integer).
 *
 *  \param [in] mws_pattern_assert_offset
 *              MWS_PATTERN signal assert offset in microseconds (signed
 *              integer).
 *
 *  \param [in] mws_pattern_assert_jitter
 *              MWS_PATTERN signal assert jitter in microseconds (unsigned
 *              integer).
 *
 *  \param [in] mws_inactivity_duration_assert_offset
 *              MWS_INACTIVITY_DURATION signal assert offset in microseconds
 *              (signed integer).
 *
 *  \param [in] mws_inactivity_duration_assert_jitter
 *              MWS_INACTIVITY_DURATION signal assert jitter in microseconds
 *              (unsigned integer).
 *
 *  \param [in] mws_scan_frequency_assert_offset
 *              MWS_SCAN_FREQUENCY signal assert offset in microseconds
 *              (signed integer).
 *
 *  \param [in] mws_scan_frequency_assert_jitter
 *              MWS_SCAN_FREQUENCY signal assert jitter in microseconds
 *              (unsigned integer).
 *
 *  \param [in] mws_priority_assert_offset_request
 *              Minimum advance notification from the beginning of an MWS
 *              Uplink period in microseconds (unsigned integer) before
 *              which the BLUETOOTH_ RX_PRI or 802_RX_PRI signal shall be
 *              asserted to be recognized by the MWS.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_set_mws_signaling
           (
               UINT16 mws_rx_assert_offset,
               UINT16 mws_rx_assert_jitter,
               UINT16 mws_rx_deassert_offset,
               UINT16 mws_rx_deassert_jitter,
               UINT16 mws_tx_assert_offset,
               UINT16 mws_tx_assert_jitter,
               UINT16 mws_tx_deassert_offset,
               UINT16 mws_tx_deassert_jitter,
               UINT16 mws_pattern_assert_offset,
               UINT16 mws_pattern_assert_jitter,
               UINT16 mws_inactivity_duration_assert_offset,
               UINT16 mws_inactivity_duration_assert_jitter,
               UINT16 mws_scan_frequency_assert_offset,
               UINT16 mws_scan_frequency_assert_jitter,
               UINT16 mws_priority_assert_offset_request
           );
#endif /* HCI_SET_MWS_SIGNALING_SUPPORT */

#ifdef HCI_SET_MWS_TRANSPORT_LAYER_SUPPORT
/**
 *  \brief To configure the transport layer between the Bluetooth
 *         Controller and MWS device.
 *
 *  \par Description:
 *       This command configures the transport layer between the
 *       Bluetooth Controller and MWS device.
 *
 *  \param [in] transport_layer
 *              See Assigned Numbers.
 *
 *  \param [in] to_mws_baud_rate
 *              Baud rate in the Bluetooth to MWS direction in Baud.
 *
 *  \param [in] from_mws_baud_rate
 *              Baud rate in the MWS to Bluetooth direction in Baud.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_set_mws_transport_layer
           (
               UCHAR  transport_layer,
               UINT32 to_mws_baud_rate,
               UINT32 from_mws_baud_rate
           );
#endif /* HCI_SET_MWS_TRANSPORT_LAYER_SUPPORT */

#ifdef HCI_SET_MWS_PATTERN_CONFIGURATION_SUPPORT
/**
 *  \brief To specify, in conjunction with the
 *         HCI_Set_External_Frame_Configuration command, local
 *         MWS_PATTERN parameters for an external collocated system.
 *
 *  \par Description:
 *       This command specifies in conjunction with the
 *       HCI_Set_External_Frame_Configuration command, local
 *       MWS_PATTERN parameters for an external collocated system.
 *
 *  \param [in] mws_pattern_index
 *              Index of the MWS_PATTERN instance to be configured.
 *              Range: 0 to 2.
 *
 *  \param [in] mws_pattern_num_intervals
 *              The number of intervals in the following arrays.
 *
 *  \param [in] mws_pattern_interval_duration
 *              The duration of this Bluetooth activity interval in
 *              microseconds.
 *
 *  \param [in] mws_pattern_interval_type
 *              0x00: Neither transmission nor reception is allowed in this
 *              interval.
 *              0x01: Transmission is allowed in this interval.
 *              0x02: Reception is allowed in this interval.
 *              0x03: Both transmission and reception are allowed in this
 *              interval.
 *              0x04: Interval for the MWS frame as defined by the
 *              HCI_Set_External_Frame_Configuration command.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_set_mws_pattern_configuration
           (
               UCHAR  mws_pattern_index,
               UCHAR  mws_pattern_num_intervals,
               UINT16 * mws_pattern_interval_duration,
               UCHAR * mws_pattern_interval_type
           );
#endif /* HCI_SET_MWS_PATTERN_CONFIGURATION_SUPPORT */

#ifdef HCI_SET_MWS_SCAN_FREQUENCY_TABLE_SUPPORT
/**
 *  \brief To configure the MWS scan frequency table in the Controller.
 *
 *  \par Description:
 *       This command configures the MWS scan frequency table in the
 *       Controller.
 *
 *  \param [in] num_scan_frequencies
 *              Number of MWS scan frequencies to be set in the table.
 *
 *  \param [in] scan_frequency_low
 *              Lower edge of the MWS scan frequency in MHz.
 *
 *  \param [in] scan_frequency_high
 *              Upper edge of the MWS scan frequency in MHz.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_set_mws_scan_frequency_table
           (
               UCHAR  num_scan_frequencies,
               UINT16 * scan_frequency_low,
               UINT16 * scan_frequency_high
           );
#endif /* HCI_SET_MWS_SCAN_FREQUENCY_TABLE_SUPPORT */

#ifdef HCI_READ_EXTENDED_PAGE_TIMEOUT_SUPPORT
/**
 *  \brief To read Extended_Page_Timeout configuration parameter.
 *
 *  \par Description:
 *       This command is to read the value for the
 *       Extended_Page_Timeout configuration parameter.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
#define BT_hci_read_extended_page_timeout() \
        hci_common_api_handler_no_params \
        (HCI_READ_EXTENDED_PAGE_TIMEOUT_OPCODE)
#endif /* HCI_READ_EXTENDED_PAGE_TIMEOUT_SUPPORT */

#ifdef HCI_WRITE_EXTENDED_PAGE_TIMEOUT_SUPPORT
/**
 *  \brief To write Extended_Page_Timeout configuration parameter.
 *
 *  \par Description:
 *       This command is to write the value for the
 *       Extended_Page_Timeout configuration parameter.
 *
 *  \param [in] extended_page_timeout
 *              Extended Page Timeout measured in number of Baseband slots.
 *              Interval Length = N * 0.625 ms (1 Baseband slot)
 *              Range: 0x0000 (default) to 0xFFFF
 *              Time Range: 0 to 40.9 s
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_write_extended_page_timeout
           (
               UINT16 extended_page_timeout
           );
#endif /* HCI_WRITE_EXTENDED_PAGE_TIMEOUT_SUPPORT */

#ifdef HCI_READ_EXTENDED_INQUIRY_LENGTH_SUPPORT
/**
 *  \brief To read Extended_Inquiry_Length configuration parameter.
 *
 *  \par Description:
 *       This command is to read the value for the
 *       Extended_Inquiry_Length configuration parameter.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
#define BT_hci_read_extended_inquiry_length() \
        hci_common_api_handler_no_params \
        (HCI_READ_EXTENDED_INQUIRY_LENGTH_OPCODE)
#endif /* HCI_READ_EXTENDED_INQUIRY_LENGTH_SUPPORT */

#ifdef HCI_WRITE_EXTENDED_INQUIRY_LENGTH_SUPPORT
/**
 *  \brief To write Extended_Inquiry_Length configuration parameter.
 *
 *  \par Description:
 *       This command is to write the value for the
 *       Extended_Inquiry_Length configuration parameter.
 *
 *  \param [in] extended_inquiry_length
 *              Interval Length = N * 0.625 ms (1 Baseband slot)
 *              Range: 0x0000 (default) to 0xFFFF
 *              Time Range: 0 to 40.9 s
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_write_extended_inquiry_length
           (
               UINT16 extended_inquiry_length
           );
#endif /* HCI_WRITE_EXTENDED_INQUIRY_LENGTH_SUPPORT */

#ifdef HCI_READ_LOCAL_OOB_EXTENDED_DATA_SUPPORT
/**
 *  \brief To read local OOB extended data.
 *
 *  \par Description:
 *       This command obtains Simple Pairing Hash C_192, Simple
 *       Pairing Randomizer R_192, Simple Pairing Hash C_256, and
 *       Simple Pairing Randomizer R_256, which are intended to be
 *       transferred to a remote device using an OOB mechanism.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
#define BT_hci_read_local_oob_extended_data() \
        hci_common_api_handler_no_params \
        (HCI_READ_LOCAL_OOB_EXTENDED_DATA_OPCODE)
#endif /* HCI_READ_LOCAL_OOB_EXTENDED_DATA_SUPPORT */

#ifdef HCI_GET_MWS_TRANSPORT_LAYER_CONFIGURATION_SUPPORT
/**
 *  \brief To inform the Host of the Baud rates supported by the
 *         Controller for the transport layer.
 *
 *  \par Description:
 *       This command is used to inform the Host of the Baud rates
 *       supported by the Controller for the transport layer.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
#define BT_hci_get_mws_transport_layer_configuration() \
        hci_common_api_handler_no_params \
        (HCI_GET_MWS_TRANSPORT_LAYER_CONFIGURATION_OPCODE)
#endif /* HCI_GET_MWS_TRANSPORT_LAYER_CONFIGURATION_SUPPORT */

#ifdef HCI_SET_TRIGGERED_CLOCK_CAPTURE_SUPPORT
/**
 *  \brief To configure the BR/EDR Controller for triggered clock
 *         capturing.
 *
 *  \par Description:
 *       This command configures the BR/EDR Controller for triggered
 *       clock capturing.
 *
 *  \param [in] connection_handle
 *              Connection_Handle
 *
 *  \param [in] enable
 *              0x00: Disable triggered clock capturing on the specified
 *              Connection_Handle (Default)
 *              0x01: Enable triggered clock capturing on the specified
 *              Connection_Handle
 *
 *  \param [in] which_clock
 *              0x00: Local Clock
 *              0x01: Piconet Clock for the specified connection
 *
 *  \param [in] lpo_allowed
 *              0x00: Controller shall not sleep (that is, clock accuracy
 *              shall be equal to or better than +|- 20 ppm)
 *              0x01: Controller may sleep (that is, clock accuracy shall be
 *              equal to or better than +|- 250 ppm)
 *
 *  \param [in] num_clock_captures_to_filter
 *              0x00: All triggered clock captures result in an
 *              HCI_Triggered_Clock_Capture event sent to the Host
 *              0x01 to 0xFF: Number of triggered clock captures filtered
 *              between sending an HCI_Triggered_Clock_Capture event to the
 *              Host.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_set_triggered_clock_capture
           (
               UINT16 connection_handle,
               UCHAR  enable,
               UCHAR  which_clock,
               UCHAR  lpo_allowed,
               UCHAR  num_clock_captures_to_filter
           );
#endif /* HCI_SET_TRIGGERED_CLOCK_CAPTURE_SUPPORT */

/* New APIs from Bluetooth Spec 4.2 ---------------------------------------- */

/* Data Length Extension Related */
#ifdef HCI_LE_SET_DATA_LENGTH_SUPPORT
/**
 *  \brief To suggest maximum transmission packet size and maximum packet transmission time
 *  to be used for a given connection.
 *
 *  \par Description:
 *       This API is used to suggest maximum transmission packet size and maximum packet
 *       transmission time (connMaxTxOctets and connMaxTxTime) to be used for a given connection.
 *       The Controller may use smaller or larger values based on local information.
 *
 *  \param [in] connection_handle
 *         Connection Handle for the existing BLE connection.
 *
 *  \param [in] tx_octets
 *         This specifies the preferred maximum number of payload octets that the local Controller
 *         should include in a single Link Layer packet for this connection.
 *
 *  \param [in] tx_time
 *         This specifies the preferred maximum number of microseconds that the local Controller
 *         should use to transmit a single Link Layer packet for this connection.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
API_RESULT BT_hci_le_set_data_length
           (
                UINT16   connection_handle,
                UINT16   tx_octets,
                UINT16   tx_time
           );
#endif /* HCI_LE_SET_DATA_LENGTH_SUPPORT */

#ifdef HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LEN_SUPPORT
/**
 *  \brief To read the Host's suggested values for the Controller's maximum
 *  transmitted number of payload octets and maximum packet transmission time
 *  to be used for new connections.
 *
 *  \par Description:
 *       This API is used to read the Host's suggested values
 *       (SuggestedMaxTxOctets and SuggestedMaxTxTime) for the Controller's
 *       maximum transmitted number of payload octets and maximum packet
 *       transmission time to be used for new connections.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_le_read_suggested_default_data_length() \
        hci_common_api_handler_no_params \
        (HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LEN_OPCODE)
#endif /* HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LEN_SUPPORT */

#ifdef HCI_LE_WRITE_SUGGESTED_DEFAULT_DATA_LEN_SUPPORT
/**
 *  \brief To specify preferred values for the Controller's maximum transmission
 *  number of payload octets and maximum packet transmission time to be used for
 *  new connections.
 *
 *  \par Description:
 *       This API is used to specify preferred values for the Controller's
 *       maximum transmission number of payload octets and maximum packet
 *       transmission time to be used for new connections. The Controller may
 *       use smaller or larger values for connInitialMaxTxOctets and
 *       connInitialMaxTxTime based on local information.
 *
 *  \param [in] suggested_max_tx_octets
 *         This specifies the Host's suggested value for the Controller's maximum transmitted
 *         number of payload octets to be used for new connections.
 *
 *  \param [in] suggested_max_tx_time
 *         This specifies the Host's suggested value for the Controller's maximum packet
 *         transmission time to be used for new connections.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
API_RESULT BT_hci_le_write_suggested_default_data_length
           (
               UINT16   suggested_max_tx_octets,
               UINT16   suggested_max_tx_time
           );
#endif /* HCI_LE_WRITE_SUGGESTED_DEFAULT_DATA_LEN_SUPPORT */

/* LE Secure Connections Related */
#ifdef HCI_LE_READ_LOCAL_P_256_PUBLIC_KEY_SUPPORT
/**
 *  \brief To return the local P-256 public key from the Controller.
 *
 *  \par Description:
 *       This API is used to return the local P-256 public key from
 *       the Controller.  The Controller shall generate a new P-256
 *       public/private key pair upon receipt of this command.
 *
 *       The keys returned via this command shall not be used when Secure
 *       Connections is used over the BR/EDR transport.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_le_read_local_p256_public_key() \
        hci_common_api_handler_no_params \
        (HCI_LE_READ_LOCAL_P_256_PUBLIC_KEY_OPCODE)
#endif /* HCI_LE_READ_LOCAL_P_256_PUBLIC_KEY_SUPPORT */

#ifdef HCI_LE_GENERATE_DHKEY_SUPPORT
/**
 *  \brief To initiate generation of a Diffie-Hellman key in the Controller
 *  for use over the LE transport.
 *
 *  \par Description:
 *       This API is used to initiate generation of a Diffie-Hellman key in
 *       the Controller for use over the LE transport. This command takes
 *       the remote P-256 public key as input.  The Diffie-Hellman key
 *       generation uses the private key generated by
 *       LE_Read_Local_P256_Public_Key command.
 *
 *       The Diffie-Hellman key returned via this command shall not be
 *       generated using any keys used for Secure Connections over
 *       the BR/EDR transport.
 *
 *  \param [in] remote_public_key
 *         The remote P-256 public key:
 *             X, Y format
 *             Octets 31-0: X co-ordinate
 *             Octets 63-32: Y co-ordinate
 *             Little Endian Format
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_le_generate_dhkey(remote_public_key) \
        hci_write_command_UCHAR_array \
        (HCI_LE_GENERATE_DHKEY_OPCODE, (remote_public_key), 64)
#endif /* HCI_LE_GENERATE_DHKEY_SUPPORT */

/* Privacy 1.2 related HCI Commands */
#ifdef HCI_LE_ADD_DEV_TO_RESOLVING_LIST_SUPPORT
/**
 *  \brief To add one device to the list of address translations used
 *   to resolve Resolvable Private Addresses in the controller.
 *
 *  \par Description:
 *       This API can be used to add one device to the list of address
 *       translations used to resolve Resolvable Private Addresses in the
 *       Controller.
 *
 *       This command cannot be used when address translation is enabled
 *       in the Controller and:
 *       - Advertising is enabled
 *       - Scanning is enabled
 *       - Create connection command is outstanding
 *
 *       This command can be used at any time when address translation is
 *       disabled in the Controller.
 *
 *       The added device shall be set to Network Privacy mode.
 *
 *       When a Controller cannot add a device to the list because there
 *       is no space available, it shall return the error code Memory Capacity
 *       Exceeded (0x07).
 *
 *  \param [in] peer_id_addr_type
 *         - Public identity Address: \ref BT_BD_PUBLIC_ADDRESS_TYPE (0x00)
 *         - Random (static) identity Address: \ref BT_BD_RANDOM_ADDRESS_TYPE (0x01)
 *
 *  \param [in] peer_id_addr
 *         Public or Random (static) Identity address of the peer device
 *
 *  \param [in] peer_irk
 *         IRK of the peer device
 *
 *  \param [in] local_irk
 *         IRK of the local device
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_add_device_to_resolving_list
           (
               UCHAR    peer_id_addr_type,
               UCHAR *  peer_id_addr,
               UCHAR *  peer_irk,
               UCHAR *  local_irk
           );
#endif /* HCI_LE_ADD_DEV_TO_RESOLVING_LIST_SUPPORT */

#ifdef HCI_LE_REMOVE_DEV_FROM_RESOLVING_LIST_SUPPORT
/**
 *  \brief To remove one device from the list of address translations used
 *   to resolve Resolvable Private Addresses in the controller.
 *
 *  \par Description:
 *       This API can be used to remove one device from the list of address
 *       translations used to resolve Resolvable Private Addresses in the controller.
 *
 *       This command cannot be used when address translation is enabled in the
 *       Controller and:
 *       - Advertising is enabled
 *       - Scanning is enabled
 *       - Create connection command is outstanding
 *
 *       This command can be used at any time when address translation is
 *       disabled in the Controller.
 *
 *       When a Controller cannot remove a device from the resolving list
 *       because it is not found, it shall return the error code
 *       Unknown Connection Identifier (0x02).
 *
 *  \param [in] peer_id_addr_type
 *         - Public identity Address: \ref BT_BD_PUBLIC_ADDRESS_TYPE (0x00)
 *         - Random (static) identity Address: \ref BT_BD_RANDOM_ADDRESS_TYPE (0x01)
 *
 *  \param [in] peer_id_addr
 *         Public or Random (static) Identity Address of the peer device to be removed.
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_remove_device_from_resolving_list(peer_id_addr_type, peer_id_addr)\
        hci_write_command_BD_ADDR_with_TYPE \
        (HCI_LE_REMOVE_DEV_FROM_RESOLVING_LIST_OPCODE , (peer_id_addr_type), (peer_id_addr))
#endif /* HCI_LE_REMOVE_DEV_FROM_RESOLVING_LIST_SUPPORT */

#ifdef HCI_LE_CLR_RESOLVING_LIST_SUPPORT
/**
 *  \brief To remove all devices from the list of address translations
 *   used to resolve Resolvable Private Addresses in the Controller.
 *
 *  \par Description:
 *       This API is used to remove all devices from the list of address
 *       translations used to resolve Resolvable Private Addresses
 *       in the Controller.
 *
 *       This command cannot be used when address translation is enabled
 *       in the Controller and:
 *       - Advertising is enabled
 *       - Scanning is enabled
 *       - Create connection command is outstanding
 *
 *       This command can be used at any time when address translation is
 *       disabled in the Controller.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_le_clear_resolving_list() \
        hci_common_api_handler_no_params \
        (HCI_LE_CLR_RESOLVING_LIST_OPCODE)
#endif /* HCI_LE_CLR_RESOLVING_LIST_SUPPORT */

#ifdef HCI_LE_READ_RESOLVING_LIST_SIZE_SUPPORT
/**
 *  \brief To read the total number of address translation
 *  entries in the resolving list that can be stored in the Controller.
 *
 *  \par Description:
 *       This API is used to read the total number of address translation
 *       entries in the resolving list that can be stored in the Controller.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 *
 *  \note:
 *  The number of entries that can be stored is not fixed and the Controller
 *  can change it at any time (e.g. because the memory used to store the list
 *  can also be used for other purposes).
 */
#define BT_hci_le_read_resolving_list_size() \
        hci_common_api_handler_no_params \
        (HCI_LE_READ_RESOLVING_LIST_SIZE_OPCODE)
#endif /* HCI_LE_READ_RESOLVING_LIST_SIZE_SUPPORT */


#ifdef HCI_LE_READ_PEER_RESOLVABLE_ADDR_SUPPORT
/**
 *  \brief To get the current peer Resolvable Private Address being
 *  used for the corresponding peer Public or Random (static) Identity Address.
 *
 *  \par Description:
 *       This API is used to get the current peer Resolvable Private Address
 *       being used for the corresponding peer Public or Random (static)
 *       Identity Address. The peer's resolvable address being used may change
 *       after the command is called.
 *
 *       This command can be used at any time.
 *
 *       When a Controller cannot find a Resolvable Private Address associated
 *       with the Peer Identity Address, it shall return the error code Unknown
 *       Connection Identifier (0x02).
 *
 *  \param [in] peer_id_addr_type
 *         - Public identity Address: \ref BT_BD_PUBLIC_ADDRESS_TYPE (0x00)
 *         - Random (static) identity Address: \ref BT_BD_RANDOM_ADDRESS_TYPE (0x01)
 *
 *  \param [in] peer_id_addr
 *         Public or Random (static) Identity address of the peer device
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_read_peer_resolvable_address(peer_id_addr_type, peer_id_addr)\
        hci_write_command_BD_ADDR_with_TYPE \
        (HCI_LE_READ_PEER_RESOLVABLE_ADDR_OPCODE, (peer_id_addr_type), (peer_id_addr))
#endif /* HCI_LE_READ_PEER_RESOLVABLE_ADDR_SUPPORT */


#ifdef HCI_LE_READ_LOCAL_RESOLVABLE_ADDR_SUPPORT
/**
 *  \brief To get the current local Resolvable Private Address being used
 *  for the corresponding peer Identity Address.
 *
 *  \par Description:
 *       This API is used to get the current local Resolvable Private Address
 *       being used for the corresponding peer Identity Address. The local's
 *       resolvable address being used may change after the command is called.
 *
 *       This command can be used at any time.
 *
 *       When a Controller cannot find a Resolvable Private Address associated
 *       with the Peer Identity Address, it shall return the error code Unknown
 *       Connection Identifier (0x02).
 *
 *  \param [in] peer_id_addr_type
 *         - Public identity Address: \ref BT_BD_PUBLIC_ADDRESS_TYPE (0x00)
 *         - Random (static) identity Address: \ref BT_BD_RANDOM_ADDRESS_TYPE (0x01)
 *
 *  \param [in] peer_id_addr
 *         Public or Random (static) Identity address of the peer device
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_read_local_resolvable_address(peer_id_addr_type, peer_id_addr)\
        hci_write_command_BD_ADDR_with_TYPE \
        (HCI_LE_READ_LOCAL_RESOLVABLE_ADDR_OPCODE, (peer_id_addr_type), (peer_id_addr))
#endif /* HCI_LE_READ_LOCAL_RESOLVABLE_ADDR_SUPPORT */


#ifdef HCI_LE_SET_ADDR_RESOLUTION_ENABLE_SUPPORT
/**
 *  \brief To enable resolution of Resolvable Private Addresses
 *  in the Controller.
 *
 *  \par Description:
 *       This API can be used to enable resolution of Resolvable Private
 *       Addresses in the Controller.
 *
 *       This can be used at any time except when:
 *       - Advertising is enabled
 *       - Scanning is enabled
 *       - Create connection command is outstanding
 *
 *  \param [in] addr_resolution_enable
 *         Enable/Disable Resolution of of Resolvable Private Addresses
 *         0x00: Address Resolution in controller disabled (default)
 *         0x01: Address Resolution in controller enabled
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note:
 *  Enabling address resolution when it is already enabled, or disabling it
 *  when it is already disabled, has no effect.
 */
#define BT_hci_le_set_address_resolution_enable(addr_resolution_enable)  \
       hci_write_command_UCHAR \
       (HCI_LE_SET_ADDR_RESOLUTION_ENABLE_OPCODE, (addr_resolution_enable))
#endif /* HCI_LE_SET_ADDR_RESOLUTION_ENABLE_SUPPORT */


#ifdef HCI_LE_SET_RESOLVABLE_PRIVATE_ADDR_TIMEOUT_SUPPORT

/**
 *  \brief To set the length of time the controller uses a Resolvable Private Address
 *  before a new resolvable private address is generated and starts being used
 *
 *  \par Description:
 *       This API is used to set the length of time the controller uses a Resolvable Private Address
 *       before a new resolvable private address is generated and starts being used.
 *
 *       This timeout applies to all addresses generated by the controller.
 *
 *  \param [in] rpa_timeout
 *         rpa_timeout measured in seconds
 *         Range for N: 0x0001 - 0xA1B8 (1 sec - approximately 11.5 hours)
 *         Default: N = 0x0384 (900 secs or 15 minutes)
 *
 *  \return API_RESULT
 *  API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_set_resolvable_private_address_timeout(rpa_timeout)\
        hci_write_command_UINT16 \
        (HCI_LE_SET_RESOLVABLE_PRIVATE_ADDR_TIMEOUT_OPCODE , (rpa_timeout))
#endif /* HCI_LE_SET_RESOLVABLE_PRIVATE_ADDR_TIMEOUT_SUPPORT */


#ifdef HCI_LE_READ_MAXIMUM_DATA_LEN_SUPPORT

/**
 *  \brief To read the Controller's maximum supported payload octets and packet duration times
 *  for transmission and reception
 *
 *  \par Description:
 *       This API is used to read the Controller's maximum supported payload octets and
 *       packet duration times for transmission and reception (supportedMaxTxOctets and
 *       supportedMaxTxTime, supportedMaxRxOctets, and supportedMaxRxTime)
 *
 *  \return API_RESULT
 *  API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_read_maximum_data_length() \
        hci_common_api_handler_no_params \
        (HCI_LE_READ_MAXIMUM_DATA_LENGTH_OPCODE)
#endif /* HCI_LE_READ_MAXIMUM_DATA_LEN_SUPPORT */

/* ------------------------------------------------------------------------- */

/* New APIs from Bluetooth Spec 5.0 ---------------------------------------- */
#ifdef HCI_LE_READ_PHY_SUPPORT
/**
 *  \brief To read the current transmitter PHY and receiver PHY.
 *
 *  \par Description:
 *       This API is used to read the current transmitter PHY and receiver PHY
 *       on the connection identified by the Connection_Handle.
 *
 *  \param [in] connection_handle
 *         Connection Handle
 *         Range:0x0000-0x0EFF
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_phy(connection_handle) \
        hci_write_command_connection_handle \
        ( \
            HCI_LE_READ_PHY_OPCODE, \
            (connection_handle) \
        )
#endif /* HCI_LE_READ_PHY_SUPPORT */

#ifdef HCI_LE_SET_DEFAULT_PHY_SUPPORT
/**
 *  \brief To specify preferred values for the transmitter PHY and receiver PHY.
 *
 *  \par Description:
 *       This API is used to specify the preferred values for the transmitter
 *       PHY and receiver PHY to be used for all subsequent connections over
 *       the LE transport.
 *
 *  \param [in] all_phys
 *         The ALL_PHYS parameter is a bit field that allows the Host to specify,
 *         for each direction, whether it has no preference among the PHYs that
 *         the Controller supports in a given direction or whether it will
 *         specify particular PHYs in the TX_PHYS or RX_PHYS parameter.
 *
 *         Bit number      Meaning
 *         ----------      -------
 *         0               The Host has no preference among the transmitter PHYs
 *                         supported by the Controller
 *
 *         1               The Host has no preference among the receiver PHYs
 *                         supported by the Controller
 *
 *  \param [in] tx_phys
 *         The TX_PHYS parameter is a bit field that indicates the transmitter PHYs that the Host
 *         prefers the Controller to use. If the ALL_PHYS parameter specifies that the Host has no
 *         preference, the TX_PHYS parameter is ignored; otherwise at least one bit shall be set
 *         to 1.
 *
 *         Bit number      Meaning
 *         ----------      -------
 *         0               The Host prefers to use the LE 1M transmitter PHY (possibly among others)
 *         1               The Host prefers to use the LE 2M transmitter PHY (possibly among others)
 *         2               The Host prefers to use the LE Coded transmitter PHY (possibly among others)
 *
 *  \param [in] rx_phys
 *         The RX_PHYS parameter is a bit field that indicates the receiver PHYs that the Host
 *         prefers the Controller to use. If the ALL_PHYS parameter specifies that the Host has no
 *         preference, the RX_PHYS parameter is ignored; otherwise at least one bit shall be set
 *         to 1.
 *
 *         Bit number      Meaning
 *         ----------      -------
 *         0               The Host prefers to use the LE 1M transmitter PHY (possibly among others)
 *         1               The Host prefers to use the LE 2M transmitter PHY (possibly among others)
 *         2               The Host prefers to use the LE Coded transmitter PHY (possibly among others)
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_default_phy
           (
               UCHAR    all_phys,
               UCHAR    tx_phys,
               UCHAR    rx_phys
           );
#endif /* HCI_LE_SET_DEFAULT_PHY_SUPPORT */

#ifdef HCI_LE_SET_PHY_SUPPORT
/**
 *  \brief To set the PHY preferences for a connection.
 *
 *  \par Description:
 *       This API is  used to set the PHY preferences for the connection
 *       identified by the Connection_Handle. The Controller might not be able
 *       to make the change (e.g. because the peer does not support
 *       the requested PHY) or may decide that the current PHY is preferable.
 *
 *       If, for at least one direction, the Host has specified a preference
 *       and the current PHY is not one of those preferred, the Controller
 *       shall request a change.  Otherwise the Controller may, but need not,
 *       request a change.
 *
 *       The PHY preferences provided by the LE Set PHY command override those
 *       provided via the LE Set Default PHY command or any preferences
 *       previously set using the LE Set PHY command on the same connection.
 *
 *       The PHY_options parameter is a bit field that allows the Host to
 *       specify options for PHYs. The default value for a new connection shall
 *       be all zero bits. The Controller may override any preferred coding for
 *       transmitting on the LE Coded PHY.
 *
 *       The Host may specify a preferred coding even if it prefers not to use
 *       the LE Coded transmitter PHY since the Controller may override the PHY
 *       preference.
 *
 *  \param [in] connection_handle
 *         Connection Handle
 *         Range:0x0000-0x0EFF
 *
 *  \param [in] all_phys
 *         The ALL_PHYS parameter is a bit field that allows the Host to specify,
 *         for each direction, whether it has no preference among the PHYs that the Controller
 *         supports in a given direction or whether it will specify particular PHYs in
 *         the TX_PHYS or RX_PHYS parameter.
 *
 *         Bit number      Meaning
 *         ----------      -------
 *         0               The Host has no preference among the transmitter PHYs
 *                         supported by the Controller
 *
 *         1               The Host has no preference among the receiver PHYs
 *                         supported by the Controller
 *
 *  \param [in] tx_phys
 *         The TX_PHYS parameter is a bit field that indicates the transmitter PHYs that the Host
 *         prefers the Controller to use. If the ALL_PHYS parameter specifies that the Host has no
 *         preference, the TX_PHYS parameter is ignored; otherwise at least one bit shall be set
 *         to 1.
 *
 *         Bit number      Meaning
 *         ----------      -------
 *         0               The Host prefers to use the LE 1M transmitter PHY (possibly among others)
 *         1               The Host prefers to use the LE 2M transmitter PHY (possibly among others)
 *         2               The Host prefers to use the LE Coded transmitter PHY (possibly among others)
 *
 *  \param [in] rx_phys
 *         The RX_PHYS parameter is a bit field that indicates the receiver PHYs that the Host
 *         prefers the Controller to use. If the ALL_PHYS parameter specifies that the Host has no
 *         preference, the RX_PHYS parameter is ignored; otherwise at least one bit shall be set
 *         to 1.
 *
 *         Bit number      Meaning
 *         ----------      -------
 *         0               The Host prefers to use the LE 1M transmitter PHY (possibly among others)
 *         1               The Host prefers to use the LE 2M transmitter PHY (possibly among others)
 *         2               The Host prefers to use the LE Coded transmitter PHY (possibly among others)
 *
 *  \param [in] phy_options
 *
 *         Bit number      Meaning
 *         ----------      -------
 *         0 - 1           0 = the Host has no preferred coding when transmitting on the LE Coded PHY
 *                         1 = the Host prefers that S=2 coding be used when transmitting on the LE Coded PHY
 *                         2 = the Host prefers that S=8 coding be used when transmitting on the LE Coded PHY
 *                         3 = Reserved for future use
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_phy
           (
                UINT16   connection_handle,
                UCHAR    all_phys,
                UCHAR    tx_phys,
                UCHAR    rx_phys,
                UINT16   phy_options
           );
#endif /* HCI_LE_SET_PHY_SUPPORT */

#ifdef HCI_LE_ENHANCED_RECEIVER_TEST_SUPPORT
/**
 *  \brief To start a test where the DUT receives test reference packets at a
 *  fixed interval.
 *
 *  \par Description:
 *       This API can be used to  start a test where the DUT receives test
 *       reference packets at a fixed interval. The tester generates the
 *       test reference packets.
 *
 *  \param [in] rx_channel Receive channel.
 *                         rx_channel : N = (F - 2402) / 2
 *                         Range: 0x00 - 0x27.
 *                         Frequency Range : 2402 MHz to 2480 MHz
 *
 *  \param [in] phy PHY to test
 *                  0x01: Receiver set to receive data at 1 M PHY
 *                  0x02: Receiver set to receive data at 2 M PHY
 *                  0x03: Receiver set to use the LE Coded PHY
 *
 *  \param [in] modulation_index Modulation Index type used for the test
 *                               0x00: Assume transmitter will have a standard modulation index
 *                               0x01: Assume transmitter will have a stable modulation index
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_enhanced_receiver_test_command
           (
               UCHAR rx_channel,
               UCHAR phy,
               UCHAR modulation_index
           );
#endif /* HCI_LE_ENHANCED_RECEIVER_TEST_SUPPORT */

#ifdef HCI_LE_ENHANCED_TRANSMITTER_TEST_SUPPORT
/**
 *  \brief To start a test where the DUT generates test reference
 *         packets at a fixed interval.
 *
 *  \par Description:
 *       This API can be used to start a test where the DUT generates
 *       test reference packets at a fixed interval. The Controller
 *       shall transmit at maximum power.
 *       An LE Controller supporting the LE_Transmitter_Test command shall
 *       support Packet_Payload values 0x00, 0x01 and 0x02. An LE Controller
 *       supporting the LE Coded PHY shall also support Packet_Payload value
 *       0x04. An LE Controller may support other values of Packet_Payload.
 *
 *  \param [in] tx_channel
 *         Trasmit Channel<br>
 *         tx_channel = (F - 2402) / 2
 *         Value Range: 0x00 to 0x27.
 *         Frequency Range : 2402 MHz to 2480 MHz
 *
 *  \param [in] test_data_length
 *         Length in bytes of payload data in each packet
 *
 *  \param [in] packet_payload
 *         Packet payload
 *         - 0x00: PRBS9 sequence '11111111100000111101...' (in transmission order)
 *         - 0x01: Repeated '11110000' (in transmission order) sequence
 *         - 0x02: Repeated '10101010' (in transmission order) sequence
 *         - 0x03: PRBS15 sequence
 *         - 0x04: Repeated '11111111' (in transmission order) sequence
 *         - 0x05: Repeated '00000000' (in transmission order) sequence
 *         - 0x06: Repeated '00001111' (in transmission order) sequence
 *         - 0x07: Repeated '01010101' (in transmission order) sequence
 *
 *  \param [in] phy PHY to test
 *                  0x01: Transmitter set to use the LE 1M PHY
 *                  0x02: Transmitter set to use the LE 2M PHY
 *                  0x03: Transmitter set to use the LE Coded PHY with S=8 data coding
 *                  0x04: Transmitter set to use the LE Coded PHY with S=2 data coding
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_enhanced_transmitter_test_command
           (
               UCHAR    tx_channel,
               UCHAR    test_data_length,
               UCHAR    packet_payload,
               UCHAR    phy
           );
#endif /* HCI_LE_ENHANCED_TRANSMITTER_TEST_SUPPORT */

/* HCI Advertising Extension Commands APIs ---------------------------------- */
#ifdef HCI_LE_SET_ADVERTISING_SET_RANDOM_ADDRESS_SUPPORT
/**
 *  \brief To set the Random device address for the advertising set
 *
 *  \par Description:
 *       This API is used by the Host to set the random device address specified
 *       by the Random_Address parameter. This address is used in the Controller
 *       for the advertiser's address contained in the advertising PDUs for the
 *       advertising set specified by the Advertising_Handle parameter.
 *
 *       If the Host issues this command while an advertising set using
 *       connectable advertising is enabled, the Controller shall return the
 *       error code Command Disallowed (0x0C). The Host may issue this command
 *       at any other time.
 *
 *       If this command is used to change the address, the new random address
 *       shall take effect for advertising no later than the next successful LE
 *       Extended Set Advertising Enable Command and for periodic advertising no
 *       later than the next successful LE Periodic Advertising Enable Command.
 *
 *  \param [in] advertising_handle
 *         0x00 - 0xEF Used to identify an advertising set
 *
 *  \param [in] random_address
 *         Random Device Address
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_adv_set_random_address
           (
               UCHAR   advertising_handle,
               UCHAR * random_address
           );
#endif /* HCI_LE_SET_ADVERTISING_RANDOM_ADDRESS_SUPPORT */

#ifdef HCI_LE_SET_EXTENDED_ADV_PARAMS_SUPPORT
/**
 *  \brief To set extended advertising parameters
 *
 *  \par Description:
 *       This API is used to is used by the Host to set extended advertising parameters.
 *
 *  \param [in] adv_handle
 *         0x00 - 0xEF: Used to identify the advertising set whose parameters are being configured
 *
 *  \param [in] adv_event_properties
 *         0 Connectable advertising
 *         1 Scannable advertising
 *         2 Directed advertising
 *         3 High Duty Cycle Directed Connectable advertising (<= 3.75 ms Advertising Interval)
 *         4 Use legacy advertising PDUs
 *         5 Omit advertiser's address from all PDUs ("anonymous advertising")
 *         6 Include TxPower in the extended header of the advertising PDU
 *
 *  \param [in] primary_advertising_interval_min
 *         Minimum advertising interval for undirected and low duty cycle directed advertising.
 *         Range: 0x000020 to 0xFFFFFF
 *         Time = N * 0.625 msec
 *         Time Range: 20 ms to 10,485.759375 s
 *
 *  \param [in] primary_advertising_interval_max
 *         Maximum advertising interval for undirected and low duty cycle directed advertising.
 *         Range: 0x000020 to 0xFFFFFF
 *         Time = N * 0.625 msec
 *         Time Range: 20 ms to 10,485.759375 s
 *
 *  \param [in] primary_advertising_channel_map    bit field
 *         Bit Number | Parameter Description
 *         -----------+----------------------
 *            0       | Channel 37 shall be used
 *         -----------+----------------------
 *            1       | Channel 38 shall be used
 *         -----------+----------------------
 *            2       | Channel 39 shall be used
 *
 *  \param [in] own_address_type
 *         0x00    Public Device Address
 *         0x01    Random Device Address
 *         0x02    Controller generates Resolvable Private Address based on the local
 *                 IRK from resolving list. If resolving list contains no matching entry,
 *                 use public address.
 *         0x03    Controller generates Resolvable Private Address based on the local IRK
 *                 from resolving list. If resolving list contains no matching entry, use
 *                 random address from LE_Set_Random_Address.
 *
 *  \param [in] peer_address_type
 *         0x00    Public Device Address or Public Identity Address
 *         0x01    Random Device Address or Random (static) Identity Address
 *
 *  \param [in] peer_address
 *         Public Device Address, Random Device Address, Public Identity Address,
 *         or Random (static) Identity Address of the device to be connected.
 *
 *  \param [in] advertising_filter_policy
 *         0x00    Process scan and connection requests from all devices
 *                 (i.e., the White List is not in use).
 *         0x01    Process connection requests from all devices and only scan requests
 *                 from devices that are in the White List.
 *         0x02    Process scan requests from all devices and only connection requests
 *                 from devices that are in the White List.
 *         0x03    Process scan and connection requests only from devices in the
 *                 White List.
 *
 *  \param [in] advertising_tx_power
 *         Size: 1 Octet (signed integer)
 *         Range: -127 <= N <= +20
 *         Units: dBm
 *         127: Host has no preference
 *
 *  \param [in] primary_advertising_phy
 *         0x01    Primary advertisement PHY is LE 1M
 *         0x03    Primary advertisement PHY is LE Coded
 *
 *  \param [in] secondary_adv_max_skip
 *         0x00 AUX_ADV_IND shall be sent prior to the next advertising event
 *         0x01-0xFF Maximum advertising events the Controller can skip before sending the
 *         AUX_ADV_IND packets on the secondary advertising channel
 *
 *  \param [in] secondary_advertising_phy
 *         0x01    Secondary advertisement PHY is LE 1M
 *         0x02    Secondary advertisement PHY is LE 2M
 *         0x03    Secondary advertisement PHY is LE Coded
 *
 *  \param [in] advertising_set_id
 *         0x00 - 0x0F Value of the Advertising SID subfield in the ADI field of the PDU
 *
 *  \param [in] scan_req_nfy_enable
 *         0x00  Scan request notifications disabled
 *         0x01  Scan request notifications enabled
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_extended_advertising_parameters
           (
               UCHAR   adv_handle,
               UINT16  adv_event_properties,
               UINT32  primary_advertising_interval_min,
               UINT32  primary_advertising_interval_max,
               UCHAR   primary_advertising_channel_map,
               UCHAR   own_address_type,
               UCHAR   peer_address_type,
               UCHAR * peer_address,
               UCHAR   advertising_filter_policy,
               INT8    advertising_tx_power,
               UCHAR   primary_advertising_phy,
               UCHAR   secondary_adv_max_skip,
               UCHAR   secondary_advertising_phy,
               UCHAR   advertising_set_id,
               UCHAR   scan_req_nfy_enable
           );
#endif /* HCI_LE_SET_EXTENDED_ADV_PARAMS_SUPPORT */

#ifdef HCI_LE_SET_EXTENDED_ADVERTISING_DATA_SUPPORT
/**
 *  \brief Used to set the data used in advertising packets
 *
 *  \par Description:
 *       This API is used to set the data used in advertising PDUs that
 *       have a data field. This command may be issued at any time after
 *       an advertising set identified by the Advertising_Handle parameter
 *       has been created using the LE Set Extended Advertising Parameters
 *       Command (see Section 7.8.53), regardless of whether advertising
 *       in that set is enabled or disabled.
 *
 *  \param [in] advertising_handle
 *         Used to identify an advertising set
 *
 *  \param [in] operation
 *         0x00  - Intermediate fragment of fragmented extended advertising data
 *         0x01  - First fragment of fragmented extended advertising data
 *         0x02  - Last fragment of fragmented extended advertising data
 *         0x03  - Complete extended advertising data
 *         0x04  - Unchanged data (just update the Advertising DID)
 *         All other values Reserved for future use

 *  \param [in] frag_pref
 *         0x00 - The Controller may fragment all Host advertising data
 *         0x01 - The Controller should not fragment or should minimize
 *                fragmentation of Host advertising data
 *         All other values Reserved for future use

 *  \param [in] advertising_data_length
 *         0-251 - The number of octets in the Advertising Data parameter
 *
 *  \param [in] advertising_data
 *         Advertising data
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_extended_advertising_data
           (
               /* IN */ UCHAR   advertising_handle,
               /* IN */ UCHAR   operation,
               /* IN */ UCHAR   frag_pref,
               /* IN */ UCHAR   advertising_data_length,
               /* IN */ UCHAR * advertising_data
           );
#endif /* HCI_LE_SET_EXTENDED_ADVERTISING_DATA_SUPPORT */

#ifdef HCI_LE_SET_EXTENDED_SCAN_RESPONSE_DATA_SUPPORT
/**
 *  \brief Used to provide data used in Scanning Packets
 *
 *  \par Description:
 *       This API is used to provide scan response data used in scanning
 *       response PDUs. This command may be issued at any time after the
 *       advertising set identified by the Advertising_Handle parameter
 *       has been created using the LE Set Extended Advertising Parameters
 *       Command regardless of whether advertising in that set is enabled
 *       or disabled.
 *
 *  \param [in] advertising_handle
 *         Used to identify an advertising set
 *
 *  \param [in] operation
 *         0x00   Intermediate fragment of fragmented scan response data
 *         0x01   First fragment of fragmented scan response data
 *         0x02   Last fragment of fragmented scan response data
 *         0x03   Complete scan response data
 *
 *  \param [in] fragment_pref
 *         0x00   The Controller may fragment all scan response data
 *         0x01   The Controller should not fragment or should minimize
 *                fragmentation of scan response data
 *
 *  \param [in] scan_response_data_length
 *         0 - 251 The number of octets in the Scan_Response Data parameter
 *
 *  \param [in] scan_response_data
 *         Scan response data
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_extended_scan_response_data
           (
               UCHAR   advertising_handle,
               UCHAR   operation,
               UCHAR   fragment_pref,
               UCHAR   scan_response_data_length,
               UCHAR * scan_response_data
           );
#endif /* HCI_LE_SET_EXTENDED_SCAN_RESPONSE_DATA_SUPPORT */

#ifdef HCI_LE_SET_EXTENDED_ADVERTISE_ENABLE_SUPPORT
/**
 *  \brief Used to request the Controller to enable or disable one or
 *  more advertising sets
 *
 *  \par Description:
 *       This API is used to request the Controller to enable or disable
 *       one or more advertising sets using the advertising sets identified
 *       by the Advertising_Handle[i] parameter. The Controller manages the
 *       timing of advertisements in accordance with the advertising
 *       parameters given in the LE_Set_Extended_Advertising_Parameters
 *       command. The Number_of_Sets parameter is the number of advertising
 *       sets contained in the parameter arrays.
 *       If Enable and Number_of_Sets are both set to 0x00, then all
 *       advertising sets are disabled.
 *
 *  \param [in] enable
 *         0x00 - Advertising is disabled
 *         0x01 - Advertising is enabled
 *
 *  \param [in] number_of_sets
 *         0x00   Disable all advertising sets
 *         0x01 - 0x3F Number of advertising sets to enable or disable
 *
 *  \param [in] advertising_handle
 *         0x00 - 0xEF Used to identify an advertising set
 *
 *  \param [in] duration
 *         0x0000 No advertising duration. Advertising to continue until
 *                the Host disables it.
 *         N = 0xXXXX Advertising duration
 *         Range: 0x0001 - 0xFFFF
 *         Time = N * 10 ms
 *         Time Range: 10 ms to 655,350 ms
 *
 *  \param [in] max_extd_adv_evts
 *         Maximum number of extended advertising events the Controller shall
 *         attempt to send prior to terminating the extended advertising
 *         0x00    No maximum number of advertising events.
 *         N=0xXX  Maximum number of extended advertising events the Controller
 *                 shall attempt to send prior to terminating the extended advertising
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_extended_advertising_enable
           (
               UCHAR    enable,
               UCHAR    number_of_sets,
               UCHAR  * advertising_handle,
               UINT16 * duration,
               UCHAR  * max_extd_adv_evts
           );
#endif /* HCI_LE_SET_EXTENDED_ADVERTISE_ENABLE_SUPPORT */

#ifdef HCI_LE_READ_MAX_ADV_DATA_LENGTH_SUPPORT
/**
 *  \brief To read the maximum length of advertising data supported by
 *  the advertising Controller
 *
 *  \par Description:
 *       This API is used to used to read the maximum length of data supported
 *       by the Controller for use as advertisement data or scan response data
 *       in an advertising event or as periodic advertisement data.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_read_maximum_advertising_data_length() \
        hci_common_api_handler_no_params \
        (HCI_LE_READ_MAX_ADV_DATA_LENGTH_OPCODE)
#endif /* HCI_LE_READ_MAX_ADV_DATA_LENGTH_SUPPORT */

#ifdef HCI_LE_READ_NUM_SUPPORTED_ADV_SETS_SUPPORT
/**
 *  \brief To read the maximum number of advertising sets.
 *
 *  \par Description:
 *       This API is used to read the maximum number of advertising sets supported
 *       by the advertising Controller at the same time.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *  The number of advertising sets that can be supported is not fixed
 *  and the Controller can change it at any time because the memory
 *  used to store advertising sets can also be used for other purposes.
 */
#define BT_hci_le_read_num_supported_advertising_sets() \
        hci_common_api_handler_no_params \
       (HCI_LE_READ_NUM_SUPPORTED_ADV_SETS_OPCODE)
#endif /* HCI_LE_READ_NUM_SUPPORTED_ADV_SETS_SUPPORT */

#ifdef HCI_LE_REMOVE_ADVERTISING_SET_SUPPORT
/**
 *  \brief To remove an advertising set from the Controller.
 *
 *  \par Description:
 *       This API is used to remove an advertising set from the Controller.
 *
 *       If the advertising set corresponding to the Advertising_Handle parameter
 *       does not exist, then the Controller shall return the error code Unknown
 *       Advertising Identifier (0x42). If advertising on the advertising set
 *       is enabled, then the Controller shall return the error code Command
 *       Disallowed (0x0C).
 *
 *  \param [in] advertising_handle
 *       0x00 - 0xEF   Used to identify an advertising set
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_remove_advertising_set(advertising_handle) \
        hci_write_command_UCHAR \
        (HCI_LE_REMOVE_ADVERTISING_SET_OPCODE, (advertising_handle))
#endif /* HCI_LE_REMOVE_ADVERTISING_SET_SUPPORT */

#ifdef HCI_LE_CLEAR_ADVERTISING_SETS_SUPPORT
/**
 *  \brief To remove all existing advertising sets from the Controller
 *
 *  \par Description:
 *       This API is used to remove all existing advertising sets from the
 *       Controller.
 *
 *       If the advertising set corresponding to the Advertising_Handle
 *       parameter does not exist, then the Controller shall return
 *       the error code Unknown Advertising Identifier (0x42).
 *       If advertising on the advertising set is enabled, then the
 *       Controller shall return the error code Command Disallowed (0x0C).
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *  All advertising sets are cleared on HCI reset.
 */
#define BT_hci_le_clear_advertising_sets() \
        hci_common_api_handler_no_params \
        (HCI_LE_CLEAR_ADVERTISING_SETS_OPCODE)
#endif /* HCI_LE_CLEAR_ADVERTISING_SETS_SUPPORT */

#ifdef HCI_LE_SET_PERIODIC_ADV_PARAMS_SUPPORT
/**
 *  \brief To set the parameters for periodic advertising
 *
 *  \par Description:
 *       This API is used to set the parameters for periodic advertising.
 *
 *       The Advertising_Handle parameter identifies the advertising set whose
 *       periodic advertising parameters are being configured. If the
 *       corresponding advertising set does not already exist, then the
 *       Controller shall return the error code Unknown Advertising Identifier
 *       (0x42).
 *
 *  \param [in] periodic_adv_handle
 *         0x00 - 0xEF  Used to identify a periodic advertisement
 *
 *  \param [in] periodic_advertising_interval_min
 *         Minimum advertising interval for periodic advertising.
 *         Range: 0x0006 to 0xFFFF
 *         Time = N * 1.25 ms
 *         Time Range: 7.5 ms to 81.91875 s
 *
 *  \param [in] periodic_advertising_interval_max
 *         Maximum advertising interval for periodic advertising.
 *         Range: 0x0006 to 0xFFFF
 *         Time = N * 1.25 ms
 *         Time Range: 7.5 ms to 81.91875 s
 *
 *  \param [in] periodic_adv_prty
 *         Bit Number       Parameter Description
 *         ----------       ---------------------
 *         6                Include TxPower in the advertising PDU
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_periodic_advertising_parameters
           (
               UCHAR   periodic_adv_handle,
               UINT16  periodic_advertising_interval_min,
               UINT16  periodic_advertising_interval_max,
               UINT16  periodic_adv_prty
           );
#endif /* HCI_LE_SET_PERIODIC_ADV_PARAMS_SUPPORT */

#ifdef HCI_LE_SET_PERIODIC_ADVERTISING_DATA_SUPPORT
/**
 *  \brief To set the data used in periodic advertising PDUs.
 *
 *  \par Description:
 *       This API is used to set the data used in periodic advertising PDUs.
 *       This command may be issued at any time after the advertising set
 *       identified by the Advertising_Handle parameter has been configured
 *       for periodic advertising using the
 *       LE_Set_Periodic_Advertising_Parameters Command, regardless of
 *       whether advertising in that set is enabled or disabled. If the
 *       advertising set has not been configured for periodic advertising,
 *       then the Controller shall return the error code Command Disallowed
 *       (0x0C).
 *
 *  \param [in] advertising_handle
 *         0x00 - 0xEF Used to identify an advertising set
 *
 *  \param [in] operation
 *         0x00   Intermediate fragment of fragmented scan response data
 *         0x01   First fragment of fragmented scan response data
 *         0x02   Last fragment of fragmented scan response data
 *         0x03   Complete scan response data
 *
 *  \param [in] advertising_data_length
 *         0 - 252 The number of octets in the Advertising Data parameter
 *
 *  \param [in] advertising_data
 *         Periodic advertising data
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_periodic_advertising_data
           (
               UCHAR   advertising_handle,
               UCHAR   operation,
               UCHAR   advertising_data_length,
               UCHAR * advertising_data
           );
#endif /* HCI_LE_SET_PERIODIC_ADVERTISING_DATA_SUPPORT */

#ifdef HCI_LE_SET_PERIODIC_ADVERTISING_ENABLE_SUPPORT
/**
 *  \brief To enable or disable periodic advertising
 *
 *  \par Description:
 *       This api is used to request the Controller to enable or disable
 *       the periodic advertising for the advertising set specified by
 *       the Advertising_Handle parameter (ordinary advertising is not
 *       affected).
 *
 *  \param [in] enable
 *         0x00    Periodic advertising is disabled (default)
 *         0x01    Periodic advertising is enabled
 *
 *  \param [in] advertising_handle
 *         0x00 - 0xEF Used to identify an advertising set
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_periodic_advertising_enable
           (
               UCHAR   enable,
               UCHAR   advertising_handle
           );
#endif /* HCI_LE_SET_PERIODIC_ADVERTISING_ENABLE_SUPPORT */

#ifdef HCI_LE_SET_EXTENDED_SCAN_PARAMETERS_SUPPORT
/**
 *  \brief To set the extended scan parameters
 *
 *  \par Description:
 *       This API is used to set the extended scan parameters to be used
 *       on the primary advertising channels.
 *
 *  \param [in] own_address_type
 *         0x00    Public Device Address
 *         0x01    Random Device Address
 *         0x02    Controller generates Resolvable Private Address based on the local
 *                 IRK from resolving list. If resolving list contains no matching entry,
 *                 use public address.
 *         0x03    Controller generates Resolvable Private Address based on the local IRK
 *                 from resolving list. If resolving list contains no matching entry, use
 *                 random address from LE_Set_Random_Address.
 *
 *  \param [in] scanning_filter_policy
 *         0x00    Accept all
 *                 - advertisement packets except directed advertising packets not
 *                   addressed to this device
 *         0x01    Accept only
 *                 - advertisement packets from devices where the advertiser's address
 *                   is in the White list.
 *                 - Directed advertising packets which are not addressed for this device
 *                   shall be ignored.
 *         0x02    Accept all
 *                 - advertising packets except directed advertising packets where
 *                   the initiator's identity address does not address this device.
 *                 - Note: directed advertising packets where the initiator's address
 *                   is a resolvable private address that cannot be resolved are also accepted.
 *         0x03    Accept all advertising packets except:
 *                 - advertising packets where the advertiser's identity address
 *                   is not in the White List; and
 *                 - directed advertising packets where the initiator's identity
 *                   address does not address this device
 *                 - Note: directed advertising packets where the initiator's
 *                   address is a resolvable private address that cannot be
 *                   resolved are also accepted
 *
 *  \param [in] scanning_phy
 *         Scanning PHYs
 *         Bit number     Parameter Description
 *         ----------     ---------------------
 *         0              Scan advertisements on the LE 1M PHY
 *         2              Scan advertisements on the LE Coded PHY
 *
 *  \param [in] scan_type
 *         0x00    Passive Scanning. No SCAN_REQ PDUs shall be sent.
 *         0x01    Active Scanning. SCAN_REQ PDUs may be sent.
 *
 *  \param [in] scan_interval
 *         This is defined as the time interval from when the Controller started
 *         its last LE scan until it begins the subsequent scan on the primary
 *         advertising channel.
 *         Range: 0x0004 to 0x4000
 *         Time = N * 0.625 msec
 *         Time Range: 2.5 ms to 40.959375 seconds
 *
 *  \param [in] scan_window
 *         The duration of the scan on the primary advertising channel.
 *         LE_Scan_Window shall be less than or equal to LE_Scan_Interval
 *         Range: 0x0004 to 0x4000
 *         Time = N * 0.625 msec
 *         Time Range: 2.5 ms to 40.959375 seconds
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_extended_scan_parameters
           (
               UCHAR    own_address_type,
               UCHAR    scanning_filter_policy,
               UCHAR    scanning_phy,
               UCHAR  * scan_type,
               UINT16 * scan_interval,
               UINT16 * scan_window
           );
#endif /* HCI_LE_SET_EXTENDED_SCAN_PARAMETERS_SUPPORT */

#ifdef HCI_LE_SET_EXTENDED_SCAN_ENABLE_SUPPORT
/**
 *  \brief Used to enable or disable scanning
 *
 *  \par Description:
 *       This API is used to enable or disable scanning.  The Enable parameter
 *       determines whether scanning is enabled or disabled. If it is disabled,
 *       the remaining parameters are ignored.
 *
 *  \param [in] enable
 *         0x00    Scanning disabled
 *         0x01    Scanning enabled
 *
 *  \param [in] filter_duplicates
 *         0x00    Duplicate filtering disabled
 *         0x01    Duplicate filtering enabled
 *         0x02    Duplicate filtering enabled, reset for each scan period
 *
 *  \param [in] duration
 *         0x0000 Scan continuously until explicitly disable
 *         0xXXXX Scan duration.
 *                Range: 0x0001 - 0xFFFF
 *                Time = N * 10 msec
 *                Time Range: 10 ms to 655.35 s
 *
 *  \param [in] period
 *         0x0000 Periodic scanning disabled
 *         N = 0xXXXX Time interval from when the Controller started its last Scan_Duration
 *                    until it begins the subsequent Scan_Duration.
 *         Range: 0x0001 - 0xFFFF
 *         Time = N * 1.28 sec
 *         Time Range: 1.28 s to 83,884.8 s
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_extended_scan_enable
           (
               UCHAR   enable,
               UCHAR   filter_duplicates,
               UINT16  duration,
               UINT16  period
           );
#endif /* HCI_LE_SET_EXTENDED_SCAN_ENABLE_SUPPORT */

#ifdef HCI_LE_EXTENDED_CREATE_CONNECTION_SUPPORT
/**
 *  \brief To create a Link Layer connection to a connectable advertiser.
 *
 *  \par Description:
 *      This api is used to create a Link Layer connection to a connectable
 *      advertiser. LE_Extended_Create_Connection command can be used
 *      in place of LE_Create_Connection command.
 *
 *  \param [in] initiator_filter_policy
 *         0x00  White list is not used to determine which advertiser to connect to.
 *               Peer_Address_Type and Peer_Address shall be used.
 *         0x01  White list is used to determine which advertiser to connect to.
 *               Peer_Address_Type and Peer_Address shall be ignored.
 *
 *  \param [in] own_address_type
 *         0x00 Public Device Address
 *         0x01 Random Device Address
 *         0x02 Controller generates the Resolvable Private Address based on the local
 *              IRK from the resolving list. If the resolving list contains no matching
 *              entry, then use the public address.
 *         0x03 Controller generates the Resolvable Private Address based on the local
 *              IRK from the resolving list. If the resolving list contains no matching
 *              entry, then use the random address from the most recent successful
 *              LE_Set_Random_Address Command.
 *
 *  \param [in] peer_address_type
 *         0x00 Public Device Address or Public Identity Address
 *         0x01 Random Device Address or Random (static) Identity Address
 *
 *  \param [in] peer_address
 *          Public Device Address, Random Device Address, Public Identity Address,
 *          or Random (static) Identity Address of the device to be connected.
 *
 *  \param [in] initiating_phys
 *         Bit number    Parameter Description
 *         ----------    ---------------------
 *         0             Scan connectable advertisements on the LE 1M PHY.
 *                       Connection parameters for the LE 1M PHY are provided.
 *
 *         1             Connection parameters for the LE 2M PHY are provided.
 *
 *         2             Scan connectable advertisements on the LE Coded PHY.
 *                       Connection parameters for the LE Coded PHY are provided.
 *
 *  \param [in] scan_interval
 *         Time interval from when the Controller started its last scan until
 *         it begins the subsequent scan on the primary advertising channel
 *         Range: 0x0004 to 0xFFFF
 *         Time = N * 0.625 ms
 *         Time Range: 2.5 ms to 40.959375 s
 *
 *  \param [in] scan_window
 *         Duration of the scan on the primary advertising channel.
 *         Range: 0x0004 to 0xFFFF
 *         Time = N * 0.625 ms
 *         Time Range: 2.5 ms to 40.959375 s
 *
 *  \param [in] conn_interval_min
 *         Minimum value for the connection interval. This shall be less
 *         than or equal to Conn_Interval_Max.
 *         Range: 0x0006 to 0x0C80
 *         Time = N * 1.25 msec
 *         Time Range: 7.5 msec to 4 s
 *
 *  \param [in] conn_interval_max
 *         Maximum value for the connection interval. This shall be greater
 *         than or equal to Conn_Interval_Min.
 *         Range: 0x0006 to 0x0C80
 *         Time = N * 1.25 msec
 *         Time Range: 7.5 msec to 4 s
 *
 *  \param [in] conn_latency
 *         Slave latency for the connection in number of connection events.
 *         Range: 0x0000 to 0x01F3
 *
 *  \param [in] supervision_timeout
 *         Supervision timeout for the LE Link.
 *         Range: 0x000A to 0x0C80
 *         Time = N * 10 msec
 *         Time Range: 100 msec to 32 s
 *
 *  \param [in] minimum_ce_length
 *         Informative parameter recommending the minimum length of connection
 *         event needed for this LE connection.
 *         Range: 0x0000 - 0xFFFF
 *         Time = N * 0.625 msec
 *
 *  \param [in] maximum_ce_length
 *         Informative parameter recommending the maximum length of connection
 *         event needed for this LE connection.
 *         Range: 0x0000 - 0xFFFF
 *         Time = N * 0.625 msec
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_extended_create_connection
           (
                UCHAR    initiator_filter_policy,
                UCHAR    own_address_type,
                UCHAR    peer_address_type,
                UCHAR  * peer_address,
                UCHAR    initiating_phys,
                UINT16 * scan_interval,
                UINT16 * scan_window,
                UINT16 * conn_interval_min,
                UINT16 * conn_interval_max,
                UINT16 * conn_latency,
                UINT16 * supervision_timeout,
                UINT16 * minimum_ce_length,
                UINT16 * maximum_ce_length
           );
#endif /* HCI_LE_EXTENDED_CREATE_CONNECTION_SUPPORT */

#ifdef HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_SUPPORT
/**
 *  \brief To control the reception of Periodic Advertising packets
 *
 *  \par Description:
 *       This API is used to synchronize with periodic advertising from
 *       an advertiser and begin receiving periodic advertising packets.
 *
 *       This command may be issued whether or not scanning is enabled and
 *       scanning may be enabled and disabled (see the LE Set Extended Scan Enable
 *       Command) while this command is pending. However, synchronization can only
 *       occur when scanning is enabled. While scanning is disabled, no attempt to
 *       synchronize will take place.
 *
 *  \param [in] options  bit field
 *         Bit Number | Parameter Description
 *         -----------+----------------------
 *            0       | 0: Use the Advertising_SID, Advertisier_Address_Type,
 *                    |    and Advertiser_Address parameters to determine
 *                    |    which advertiser to listen to.
 *                    | 1: Use the Periodic Advertiser List to determine
 *                    |    which advertiser to listen to.
 *         -----------+----------------------
 *            1       | 0: Reporting initially enabled
 *                    | 1: Reporting initially disabled
 *         -----------+----------------------
 *     All other bits | Reserved for future use
 *
 *  \param [in] advertising_sid
 *         0x00 - 0x0F  Advertising SID subfield in the ADI field used to
 *                      identify the Periodic Advertising
 *
 *  \param [in] adv_address_type
 *         0x00 Public Device Address
 *         0x01 Random Device Address
 *
 *  \param [in] adv_address
 *         Public Device Address, Random Device Address, Public Identity Address,
 *         or Random (static) Identity Address of the advertiser.
 *
 *  \param [in] skip
 *         The number of periodic advertising packets that can be skipped
 *         after a successful receive.
 *         Range: 0x0000 to 0x1F3
 *
 *  \param [in] sync_timeout
 *         Supervision timeout for the periodic advertising.
 *         Range: 0x000A to 0x4000
 *         Time = N*10 msec
 *         Time Range: 100 ms to 163.84 seconds
 *
 *  \param [in] sync_cte_type    bit field
 *         Bit Number | Parameter Description
 *         -----------+----------------------
 *            0       | Do not sync to packets with an AoA
 *                    | Constant Tone Extension
 *         -----------+----------------------
 *            1       | Do not sync to packets with an AoD
 *                    | Constant Tone Extension with 1 us slots
 *         -----------+----------------------
 *            2       | Do not sync to packets with an AoD
 *                    | Constant Tone Extension with 2 us slots
 *         -----------+----------------------
 *            3       | Do not sync to packets with a type 3
 *                    | Constant Tone Extension (currently
 *                    | reserved for future use)
 *         -----------+----------------------
 *            4       | Do not sync to packets without
 *                    | a Constant Tone Extension
 *         -----------+----------------------
 *     All other bits | Reserved for future use
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_periodic_adv_create_sync
           (
               UCHAR   options,
               UCHAR   advertising_sid,
               UCHAR   adv_address_type,
               UCHAR * adv_address,
               UINT16  skip,
               UINT16  sync_timeout,
               UCHAR   sync_cte_type
           );
#endif /* HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_SUPPORT */

#ifdef HCI_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL_SUPPORT
/**
 *  \brief To cancel the LE_Periodic_Advertising_Create_Sync command
 *
 *  \par Description:
 *      This api is used to cancel the LE_Periodic_Advertising_Create_Sync
 *      command while it is pending.
 *
 *      If the Host issues this command while no
 *      LE_Periodic_Advertising_Create_Sync command is pending, the
 *      Controller shall return the error code Command Disallowed (0x0C).
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_periodic_advertising_create_sync_cancel() \
        hci_common_api_handler_no_params \
       (HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_CANCEL_OPCODE)
#endif /* HCI_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL_SUPPORT */

#ifdef HCI_LE_PERIODIC_ADVERTISING_TERMINATE_SYNC_SUPPORT
/**
 *  \brief To stop reception of periodic advertising
 *
 *  \par Description:
 *      This api is used to stop reception of the periodic advertising
 *      identified by the Sync_Handle parameter.
 *
 *  \param [in] sync_handle
 *         Sync_Handle to be used to identify the periodic advertiser.
 *         Range: 0x0000-0x0EFF
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_periodic_adv_terminate_sync
           (
                UINT16  sync_handle
           );
#endif /* HCI_LE_PERIODIC_ADVERTISING_TERMINATE_SYNC_SUPPORT */

#ifdef HCI_LE_ADD_DEV_TO_PERIODIC_ADVSER_LIST_SUPPORT
/**
 *  \brief To add a single device to the Periodic Advertiser list
 *
 *  \par Description:
 *       This API is used to add a single device to the Periodic Advertiser
 *       list stored in the Controller. Any additions to the Periodic
 *       Advertiser list take effect immediately. If the device is already
 *       on the list, the Controller shall return the error code Invalid HCI
 *       Command Parameters (0x12).
 *
 *       If the Host issues this command when an
 *       LE_Periodic_Advertising_Create_Sync command is pending, the Controller
 *       shall return the error code Command Disallowed (0x0C).
 *
 *       When a Controller cannot add a device to the Periodic Advertiser list
 *       because the list is full, the Controller shall return the error code
 *       Memory Capacity Exceeded (0x07).
 *
 *  \param [in] advertiser_address_type
 *         0x00    Public Device Address or Public Identity Address
 *         0x01    Random Device Address or Random (static) Identity Address
 *
 *  \param [in] advertiser_address
 *         Public Device Address, Random Device Address, Public Identity Address,
 *         or Random (static) Identity Address of the advertiser.
 *
 *  \param [in] advertising_sid
 *         0x00-0x0F Advertising SID subfield in the ADI field used to identify
 *         the Periodic Advertising
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_add_device_to_periodic_advertiser_list
           (
               UCHAR   advertiser_address_type,
               UCHAR * advertiser_address,
               UCHAR   advertising_sid
           );
#endif /* HCI_LE_ADD_DEV_TO_PERIOD_ADVSER_LIST_SUPPORT */

#ifdef HCI_LE_RM_DEV_FROM_PERIODIC_ADVSER_LIST_SUPPORT
/**
 *  \brief To remove a single device to the Periodic Advertiser list
 *
 *  \par Description:
 *       This API is used to remove one device from the list of Periodic
 *       Advertisers stored in the Controller. Removals from the Periodic
 *       Advertisers List take effect immediately.
 *
 *  \param [in] advertiser_address_type
 *         0x00    Public Device Address or Public Identity Address
 *         0x01    Random Device Address or Random (static) Identity Address
 *
 *  \param [in] advertiser_address
 *         Public Device Address, Random Device Address, Public Identity Address,
 *         or Random (static) Identity Address of the advertiser.
 *
 *  \param [in] advertising_sid
 *         0x00-0x0F Advertising SID subfield in the ADI field used to identify
 *         the Periodic Advertising
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_remove_device_from_periodic_advertiser_list
           (
               UCHAR   advertiser_address_type,
               UCHAR * advertiser_address,
               UCHAR   advertising_sid
           );
#endif /* HCI_LE_RM_DEV_FROM_PERIODIC_ADVSER_LIST_SUPPORT */

#ifdef HCI_LE_CLEAR_PERIODIC_ADVSER_LIST_SUPPORT
/**
 *  \brief To remove all devices from the list of Periodic Advertisers
 *  in the Controller.
 *
 *  \par Description:
 *       This API is used to remove all devices from the list of Periodic
 *       Advertisers in the Controller.
 *
 *       If this command is used when an LE_Periodic_Advertising_Create_Sync
 *       command is pending, the Controller shall return the error code Command
 *       Disallowed (0x0C).
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_clear_periodic_advertiser_list() \
        hci_common_api_handler_no_params \
        (HCI_LE_CLEAR_PERIODIC_ADVERTISER_LIST_OPCODE)
#endif /* HCI_LE_CLEAR_PERIODIC_ADVSER_LIST_SUPPORT */

#ifdef HCI_LE_READ_PERIODIC_ADVSER_LIST_SIZE_SUPPORT
/**
 *  \brief To read the total number of Periodic Advertiser list entries that can be
 *  stored in the Controller
 *
 *  \par Description:
 *       This API is used to read the total number of Periodic Advertiser list entries
 *       that can be stored in the Controller
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *  The number of entries that can be stored is not fixed and the Controller
 *  can change it at any time (e.g., because the memory used to store the list
 *  can also be used for other purposes).
 */
#define BT_hci_le_read_periodic_advertiser_list_size() \
        hci_common_api_handler_no_params \
        (HCI_LE_READ_PERIODIC_ADVERTISER_LIST_SIZE_OPCODE)
#endif /* HCI_LE_READ_PERIODIC_ADVSER_LIST_SIZE_SUPPORT */

#ifdef HCI_LE_READ_TRANSMIT_POWER_SUPPORT
/**
 *  \brief To read the minimum and maximum transmit powers.
 *
 *  \par Description:
 *       This API is used read the minimum and maximum transmit powers supported by
 *       the Controller
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_read_transmit_power() \
        hci_common_api_handler_no_params \
        (HCI_LE_READ_TRANSMIT_POWER_OPCODE)
#endif /* HCI_LE_READ_TRANSMIT_POWER_SUPPORT */

#ifdef HCI_LE_READ_RF_PATH_COMPENSATION_SUPPORT
/**
 *  \brief To read the RF Path Compensation Value.
 *
 *  \par Description:
 *       This API is used to read the RF Path Compensation Values parameter used
 *       in the Tx Power Level and RSSI calculation.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
#define BT_hci_le_read_rf_path_compensation() \
        hci_common_api_handler_no_params \
        (HCI_LE_READ_RF_PATH_COMPENSATION_OPCODE)
#endif /* HCI_LE_READ_RF_PATH_COMPENSATION_SUPPORT */

#ifdef HCI_LE_WRITE_RF_PATH_COMPENSATION_SUPPORT
/**
 *  \brief To indicate the RF path gain or loss
 *
 *  \par Description:
 *       This API is used to indicate the RF path gain or loss between
 *       the RF transceiver and the antenna contributed by intermediate
 *       components. A positive value means a net RF path gain and a
 *       negative value means a net RF path loss. The RF Tx Path Compensation
 *       Value parameter shall be used by the Controller to calculate radiative
 *       Tx Power Level used in the TxPower field in the Extended Header using
 *       the following equation:
 *
 *       Radiative Tx Power Level = Tx Power Level at RF transceiver output +
 *                                  RF Tx Path Compensation Value
 *
 *       For example, if the Tx Power Level is +4 (dBm) at RF transceiver output
 *       and the RF Path Compensation Value is -1.5 (dB), the radiative Tx Power
 *       Level is +4+(-1.5) = 2.5 (dBm).
 *
 *       The RF Rx Path Compensation Value parameter shall be used by the
 *       Controller to calculate the RSSI value reported to the Host.
 *
 *  \param [in] rf_tx_path_comp_value
 *         RF tx Path compensation Value.
 *         Range: -128.0 dB (0xFB00) = N = 128.0 dB (0x0500)
 *         Units: 0.1dB
 *
  * \param [in] rf_rx_path_comp_value
 *         RF rx Path compensation Value.
 *         Range: -128.0 dB (0xFB00) = N = 128.0 dB (0x0500)
 *         Units: 0.1dB
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_write_rf_path_compensation
           (
               UINT16   rf_tx_path_comp_value,
               UINT16   rf_rx_path_comp_value
           );
#endif /* HCI_LE_WRITE_RF_PATH_COMPENSATION_SUPPORT */

#ifdef HCI_LE_SET_PRIVACY_MODE_SUPPORT
/**
 *  \brief To set the privacy mode.
 *
 *  \par Description:
 *       This API is used to allow the Host to specify the privacy mode
 *       to be used for a given entry on the resolving list.
 *
 *  \param [in] peer_identity_addr_type
 *         Address type of the peer Bluetooth device identity address
 *         - Public Device or Public Identity Address:
 *           \ref BT_BD_PUBLIC_ADDRESS_TYPE (0x00)
 *         - Random Device or Random (static) Identity Address:
 *           \ref BT_BD_RANDOM_ADDRESS_TYPE (0x01)
 *
 *  \param [in] peer_identity_addr
 *         Public Identity Address or Random (static) Identity Address
 *         of the advertiser
 *
 *  \param [in] privacy_mode
 *         - 0x00: Use Network Privacy Mode for this peer device (default)
 *         - 0x01: Use Device Privacy Mode for this peer device
 *
 *  \return API_RESULT
 *     API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_le_set_privacy_mode
           (
                UCHAR    peer_identity_addr_type,
                UCHAR *  peer_identity_addr,
                UCHAR    privacy_mode
           );
#endif /* HCI_LE_SET_PRIVACY_MODE_SUPPORT */

/* New APIs from Bluetooth Spec 5.1 ---------------------------------------- */
#ifdef HCI_LE_RECEIVER_TEST_V3_SUPPORT
/**
 *  \brief To start a test where the DUT receives test reference
 *         packets
 *
 *  \par Description:
 *       This command is used to start a test where the DUT receives
 *       test reference packets at a fixed interval. The tester
 *       generates the test reference packets.
 *
 *  \param [in] rx_channel
 *              N = (F-2402) / 2
 *              Range: 0x00 to 0x27.
 *              Frequency Range: 2402 MHz to 2480 MHz
 *
 *  \param [in] phy
 *              0x01: Receiver set to use the LE 1M PHY
 *              0x02: Receiver set to use the LE 2M PHY
 *              0x03: Receiver set to use the LE Coded PHY
 *
 *  \param [in] modulation_index
 *              0x00: Assume transmitter will have a standard modulation
 *              index
 *              0x01: Assume transmitter will have a stable modulation index
 *
 *  \param [in] expected_cte_length
 *              0x00: No Constant Tone Extension
 *              0x02 to 0x14: Constant Tone Extension length in 8 us units
 *
 *  \param [in] extension_cte_type
 *              0x00: Expect AoA Constant Tone Extension
 *              0x01: Expect AoD Constant Tone Extension with 1 us slots
 *              0x02: Expect AoD Constant Tone Extension with 2 us slots
 *
 *  \param [in] slot_duration
 *              0x01: Switching and sampling slots are 1 us each
 *              0x02: Switching and sampling slots are 2 us each
 *
 *  \param [in] length_of_switching_pattern
 *              0x02 to 0x4B: The number of Antenna IDs in the pattern
 *
 *  \param [in] antenna_ids
 *              List of Antenna IDs in the pattern
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_receiver_test_v3
           (
               UCHAR  rx_channel,
               UCHAR  phy,
               UCHAR  modulation_index,
               UCHAR  expected_cte_length,
               UCHAR  extension_cte_type,
               UCHAR  slot_duration,
               UCHAR  length_of_switching_pattern,
               UCHAR * antenna_ids
           );
#endif /* HCI_LE_RECEIVER_TEST_V3_SUPPORT */

#ifdef HCI_LE_TRANSMITTER_TEST_V3_SUPPORT
/**
 *  \brief To start a test where the DUT generates test reference
 *         packets at a fixed interval
 *
 *  \par Description:
 *       This command is used to start a test where the DUT generates
 *       test reference packets at a fixed interval. The Controller
 *       shall transmit at maximum power.
 *
 *  \param [in] tx_channel
 *              N = (F-2402) / 2
 *              Range: 0x00 to 0x27.
 *              Frequency Range: 2402 MHz to 2480 MHz
 *
 *  \param [in] length_of_test_data
 *              0x00-0xFF: Length in bytes of payload data in each packet
 *
 *  \param [in] packet_payload
 *              0x00: PRBS9 sequence '11111111100000111101...' (in
 *              transmission order)
 *              0x01: Repeated '11110000' (in transmission order) sequence
 *              0x02: Repeated '10101010' (in transmission order) sequence
 *              0x03: PRBS15 sequence
 *              0x04: Repeated '11111111' (in transmission order) sequence
 *              0x05: Repeated '00000000' (in transmission order) sequence
 *              0x06: Repeated '00001111' (in transmission order) sequence
 *              0x07: Repeated '01010101' (in transmission order) sequence
 *
 *  \param [in] phy
 *              0x01: Transmitter set to use the LE 1M PHY
 *              0x02: Transmitter set to use the LE 2M PHY
 *              0x03: Transmitter set to use the LE Coded PHY with S=8 data
 *              coding
 *              0x04: Transmitter set to use the LE Coded PHY with S=2 data
 *              coding
 *
 *  \param [in] cte_length
 *              0x00: Do not transmit a Constant Tone Extension
 *              0x02 to 0x14: Length of the Constant Tone Extension in 8 us
 *              units
 *
 *  \param [in] cte_type
 *              0x00: AoA Constant Tone Extension
 *              0x01: AoD Constant Tone Extension with 1 us slots
 *              0x02: AoD Constant Tone Extension with 2 us slots
 *
 *  \param [in] length_of_switching_pattern
 *              0x02 to 0x4B: The number of Antenna IDs in the pattern
 *
 *  \param [in] antenna_ids
 *              List of Antenna IDs in the pattern
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_transmitter_test_v3
           (
               UCHAR  tx_channel,
               UCHAR  length_of_test_data,
               UCHAR  packet_payload,
               UCHAR  phy,
               UCHAR  cte_length,
               UCHAR  cte_type,
               UCHAR  length_of_switching_pattern,
               UCHAR * antenna_ids
           );
#endif /* HCI_LE_TRANSMITTER_TEST_V3_SUPPORT */

#ifdef HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_PARAMETERS_SUPPORT
/**
 *  \brief To Set CTE parameters
 *
 *  \par Description:
 *       This API is used to set the type, length, and antenna
 *       switching pattern for the transmission of Constant Tone
 *       Extensions in any periodic advertising on the advertising
 *       set identified by the Advertising_Handle parameter
 *
 *  \param [in] advertising_handle
 *              0x00 to 0xEF: Used to identify an advertising set.
 *
 *  \param [in] cte_length
 *              0x00 No Constant Tone Extension
 *              0x02 to 0x14 Constant Tone Extension length in 8 us units
 *
 *  \param [in] cte_type
 *              0x00 AoA Constant Tone Extension
 *              0x01 AoD Constant Tone Extension with 1 us slots
 *              0x02 AoD Constant Tone Extension with 2 us slots
 *
 *  \param [in] cte_count
 *              The number of Constant Tone Extensions to transmit in each
 *              periodic advertising interval
 *              Range: 0x01 to 0x10
 *
 *  \param [in] length_of_switching_pattern
 *              0x02 to 0x4B The number of Antenna IDs in the pattern
 *
 *  \param [in] antenna_ids
 *              List of Antenna IDs in the pattern
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_connectionless_cte_transmit_parameters
           (
               UCHAR  advertising_handle,
               UCHAR  cte_length,
               UCHAR  cte_type,
               UCHAR  cte_count,
               UCHAR  length_of_switching_pattern,
               UCHAR * antenna_ids
           );
#endif /* HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_PARAMETERS_SUPPORT */

#ifdef HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_ENABLE_SUPPORT
/**
 *  \brief Used to request that the Controller enables or disables the
 *         use of Constant Tone Extensions
 *
 *  \par Description:
 *       The HCI_LE_Set_Connectionless_CTE_Transmit_Enable command is
 *       used to request that the Controller enables or disables the
 *       use of Constant Tone Extensions in any periodic advertising
 *       on the advertising set identified by Advertising_Handle.
 *
 *  \param [in] advertising_handle
 *              0x00 to 0xEF: Identifier for the advertising set in which
 *              Constant Tone Extension is being enabled or disabled.
 *
 *  \param [in] cte_enable
 *              0x00 Advertising with Constant Tone Extension is disabled
 *              (default)
 *              0x01 Advertising with Constant Tone Extension is enabled
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_connectionless_cte_transmit_enable
           (
               UCHAR  advertising_handle,
               UCHAR  cte_enable
           );
#endif /* HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_ENABLE_SUPPORT */

#ifdef HCI_LE_SET_CONNECTIONLESS_IQ_SAMPLING_ENABLE_SUPPORT
/**
 *  \brief Used to request that the Controller enables or disables
 *         capturing IQ samples from the Constant Tone Extension
 *
 *  \par Description:
 *       The HCI_LE_Set_Connectionless_IQ_Sampling_Enable command is
 *       used to request that the Controller enables or disables
 *       capturing IQ samples from the Constant Tone Extension of
 *       periodic advertising packets identified by the Sync_Handle
 *       parameter
 *
 *  \param [in] sync_handle
 *              Sync_Handle identifying the periodic advertising
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] sampling_enable
 *              0x00 Connectionless IQ sampling is disabled (default)
 *              0x01 Connectionless IQ sampling is enabled
 *
 *  \param [in] slot_duration
 *              0x01 Switching and sampling slots are 1 us each
 *              0x02 Switching and sampling slots are 2 us each
 *
 *  \param [in] max_sampled_ctes
 *              0x00 Sample and report all available Constant Tone
 *              Extensions The maximum number of Constant Tone Extensions to
 *              sample and report in each periodic advertising interval
 *              Range: 0x01 to 0x10
 *
 *  \param [in] length_of_switching_pattern
 *              0x02 to 0x4B The number of Antenna IDs in the pattern
 *
 *  \param [in] antenna_ids
 *              List of Antenna IDs in the pattern.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_connectionless_iq_sampling_enable
           (
               UINT16 sync_handle,
               UCHAR  sampling_enable,
               UCHAR  slot_duration,
               UCHAR  max_sampled_ctes,
               UCHAR  length_of_switching_pattern,
               UCHAR * antenna_ids
           );
#endif /* HCI_LE_SET_CONNECTIONLESS_IQ_SAMPLING_ENABLE_SUPPORT */

#ifdef HCI_LE_SET_CONNECTION_CTE_RECEIVE_PARAMETERS_SUPPORT
/**
 *  \brief Used to enable or disable sampling received Constant Tone
 *         Extension fields
 *
 *  \par Description:
 *       The HCI_LE_Set_Connection_CTE_Receive_Parameters command is
 *       used to enable or disable sampling received Constant Tone
 *       Extension fields on the connection identified by the
 *       Connection_Handle parameter and to set the antenna switching
 *       pattern and switching and sampling slot durations to be
 *       used.
 *
 *  \param [in] connection_handle
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] sampling_enable
 *              0x00 Connection IQ sampling is disabled (default)
 *              0x01 Connection IQ sampling is enabled
 *
 *  \param [in] slot_duration
 *              0x01 Switching and sampling slots are 1 us each
 *              0x02 Switching and sampling slots are 2 us each
 *
 *  \param [in] length_of_switching_pattern
 *              0x02 to 0x4B The number of Antenna IDs in the pattern
 *
 *  \param [in] antenna_ids
 *              List of Antenna IDs in the pattern.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_connection_cte_receive_parameters
           (
               UINT16 connection_handle,
               UCHAR  sampling_enable,
               UCHAR  slot_duration,
               UCHAR  length_of_switching_pattern,
               UCHAR * antenna_ids
           );
#endif /* HCI_LE_SET_CONNECTION_CTE_RECEIVE_PARAMETERS_SUPPORT */

#ifdef HCI_LE_SET_CONNECTION_CTE_TRANSMIT_PARAMETERS_SUPPORT
/**
 *  \brief Used to set the antenna switching pattern and permitted
 *         Constant Tone Extension types
 *
 *  \par Description:
 *       The HCI_LE_Set_Connection_CTE_Transmit_Parameters command is
 *       used to set the antenna switching pattern and permitted
 *       Constant Tone Extension types used for transmitting the
 *       Constant Tone Extension requested by the peer device on the
 *       connection identified by the Connection_Handle parameter.
 *
 *  \param [in] connection_handle
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] cte_types
 *              0 Allow AoA Constant Tone Extension Response
 *              1 Allow AoD Constant Tone Extension Response with 1 us slots
 *              2 Allow AoD Constant Tone Extension Response with 2 us slots
 *
 *  \param [in] length_of_switching_pattern
 *              0x02 to 0x4B The number of Antenna IDs in the pattern
 *
 *  \param [in] antenna_ids
 *              List of Antenna IDs in the pattern.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_connection_cte_transmit_parameters
           (
               UINT16 connection_handle,
               UCHAR  cte_types,
               UCHAR  length_of_switching_pattern,
               UCHAR * antenna_ids
           );
#endif /* HCI_LE_SET_CONNECTION_CTE_TRANSMIT_PARAMETERS_SUPPORT */

#ifdef HCI_LE_SET_CONNECTION_CTE_REQUEST_ENABLE_SUPPORT
/**
 *  \brief Used to request the Controller to start or stop initiating
 *         the Constant Tone Extension Request procedure
 *
 *  \par Description:
 *       The HCI_LE_Connection_CTE_Request_Enable command is used to
 *       request the Controller to start or stop initiating the
 *       Constant Tone Extension Request procedure on a connection
 *       identified by the Connection_Handle parameter.
 *
 *  \param [in] connection_handle
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] enable
 *              0x00 Disable Constant Tone Extension Request for the
 *              connection (default)
 *              0x01 Enable Constant Tone Extension Request for the
 *              connection
 *
 *  \param [in] cte_request_interval
 *              0x0000 Initiate the Constant Tone Extension Request
 *              procedure once, at the earliest practical opportunity
 *              0x0001 to 0xFFFF Requested interval for initiating the
 *              Constant Tone Extension Request procedure in number of
 *              connection events.
 *
 *  \param [in] requested_cte_length
 *              0x02 to 0x14 Minimum length of the Constant Tone Extension
 *              being requested in 8 us units
 *
 *  \param [in] requested_cte_type
 *              0x00 AoA Constant Tone Extension
 *              0x01 AoD Constant Tone Extension with 1 us slots
 *              0x02 AoD Constant Tone Extension with 2 us slots
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_connection_cte_request_enable
           (
               UINT16 connection_handle,
               UCHAR  enable,
               UINT16 cte_request_interval,
               UCHAR  requested_cte_length,
               UCHAR  requested_cte_type
           );
#endif /* HCI_LE_SET_CONNECTION_CTE_REQUEST_ENABLE_SUPPORT */

#ifdef HCI_LE_SET_CONNECTION_CTE_RESPONSE_ENABLE_SUPPORT
/**
 *  \brief Used to request the Controller to respond to LL_CTE_REQ PDUs
 *         with LL_CTE_RSP PDUs
 *
 *  \par Description:
 *       The HCI_LE_Connection_CTE_Response_Enable command is used to
 *       request the Controller to respond to LL_CTE_REQ PDUs with
 *       LL_CTE_RSP PDUs on the specified connection.
 *
 *  \param [in] connection_handle
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] enable
 *              0x00 Disable Constant Tone Extension Response for the
 *              connection (default)
 *              0x01 Enable Constant Tone Extension Response for the
 *              connection
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_connection_cte_response_enable
           (
               UINT16 connection_handle,
               UCHAR  enable
           );
#endif /* HCI_LE_SET_CONNECTION_CTE_RESPONSE_ENABLE_SUPPORT */

#ifdef HCI_LE_READ_ANTENNA_INFORMATION_SUPPORT
/**
 *  \brief Allows the Host to read the switching rates, the sampling
 *         rates, the number of antennae, and the maximum length of a
 *         transmitted Constant Tone Extension
 *
 *  \par Description:
 *       This API is used to read the switching rates, the sampling
 *       rates, the number of antennae, and the maximum length of a
 *       transmitted Constant Tone Extension supported by the
 *       Controller.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
#define BT_hci_le_read_antenna_information() \
        hci_common_api_handler_no_params \
        (HCI_LE_READ_ANTENNA_INFORMATION_OPCODE)
#endif /* HCI_LE_READ_ANTENNA_INFORMATION_SUPPORT */

#ifdef HCI_LE_SET_PERIODIC_ADVERTISING_RECEIVE_ENABLE_SUPPORT
/**
 *  \brief To enable or disable reports for the periodic advertising
 *         train
 *
 *  \par Description:
 *       The HCI_LE_Set_Periodic_Advertising_Receive_Enable command
 *       will enable or disable reports for the periodic advertising
 *       train identified by the Sync_Handle parameter.
 *
 *  \param [in] sync_handle
 *              Sync_Handle identifying the periodic advertising train.
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] enable
 *              0x00: Reporting disabled
 *              0x01: Reporting enabled
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_periodic_advertising_receive_enable
           (
               UINT16 sync_handle,
               UCHAR  enable
           );
#endif /* HCI_LE_SET_PERIODIC_ADVERTISING_RECEIVE_ENABLE_SUPPORT */

#ifdef HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_SUPPORT
/**
 *  \brief To instruct the Controller to send synchronization
 *         information
 *
 *  \par Description:
 *       The HCI_LE_Periodic_Advertising_Sync_Transfer command is
 *       used to instruct the Controller to send synchronization
 *       information about the periodic advertising train identified
 *       by the Sync_Handle parameter to a connected device.
 *
 *  \param [in] connection_handle
 *              Connection Handle
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] service_data
 *              A value provided by the Host
 *
 *  \param [in] sync_handle
 *              Sync_Handle identifying the periodic advertising train.
 *              Range: 0x0000 to 0x0EFF
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_periodic_advertising_sync_transfer
           (
               UINT16 connection_handle,
               UINT16 service_data,
               UINT16 sync_handle
           );
#endif /* HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_SUPPORT */

#ifdef HCI_LE_PERIODIC_ADVERTISING_SET_INFO_TRANSFER_SUPPORT
/**
 *  \brief To instruct the Controller to send synchronization
 *         information
 *
 *  \par Description:
 *       The HCI_LE_Periodic_Advertising_Set_Info_Transfer command is
 *       used to instruct the Controller to send synchronization
 *       information about the periodic advertising in an advertising
 *       set to a connected device.
 *
 *  \param [in] connection_handle
 *              Connection Handle
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] service_data
 *              A value provided by the Host
 *
 *  \param [in] advertising_handle
 *              Used to identify an advertising set
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_periodic_advertising_set_info_transfer
           (
               UINT16 connection_handle,
               UINT16 service_data,
               UCHAR  advertising_handle
           );
#endif /* HCI_LE_PERIODIC_ADVERTISING_SET_INFO_TRANSFER_SUPPORT */

#ifdef HCI_LE_SET_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS_SUPPORT
/**
 *  \brief To specify how the Controller will process periodic
 *         advertising synchronization information
 *
 *  \par Description:
 *       The HCI_LE_Set_Periodic_Advertising_Sync_Transfer_Parameters
 *       command is used to specify how the Controller will process
 *       periodic advertising synchronization information received
 *       from the device identified by the Connection_Handle
 *       parameter (the "transfer mode").
 *
 *  \param [in] connection_handle
 *              Connection Handle
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] mode
 *              0x00: No attempt is made to synchronize to the periodic
 *              advertising and no
 *              HCI_LE_Periodic_Advertising_Sync_Transfer_Received event is
 *              sent to the Host.
 *              0x01: An HCI_LE_Periodic_Advertising_Sync_Transfer_Received
 *              event is sent to the Host.
 *              HCI_LE_Periodic_Advertising_Report events will be disabled.
 *              0x02: An HCI_LE_Periodic_Advertising_Sync_Transfer_Received
 *              event is sent to the Host.
 *              HCI_LE_Periodic_Advertising_Report events will be enabled.
 *
 *  \param [in] skip
 *              The number of periodic advertising packets that can be
 *              skipped after a successful receive
 *              Range: 0x0000 to 0x01F3
 *
 *  \param [in] sync_timeout
 *              Synchronization timeout for the the periodic advertising
 *              train
 *              Range: 0x000A to 0x4000
 *              Time = N*10 ms
 *              Time Range: 100 ms to 163.84 s
 *
 *  \param [in] cte_type
 *              Bit number
 *              0: Do not sync to packets with an AoA Constant Tone
 *              Extension
 *              1: Do not sync to packets with an AoD Constant Tone
 *              Extension with 1 us slots
 *              2: Do not sync to packets with an AoD Constant Tone
 *              Extension with 2 us slots
 *              4: Do not sync to packets without a Constant Tone Extension
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_periodic_advertising_sync_transfer_parameters
           (
               UINT16 connection_handle,
               UCHAR  mode,
               UINT16 skip,
               UINT16 sync_timeout,
               UCHAR  cte_type
           );
#endif /* HCI_LE_SET_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS_SUPPORT */

#ifdef HCI_LE_SET_DEFAULT_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS_SUPPORT
/**
 *  \brief To specify the initial value for the mode, skip, timeout,
 *         and Constant Tone Extension type
 *
 *  \par Description:
 *       The HCI_LE_Set_Default_Periodic_Advertising_Sync_Transfer_Pa
 *       rameters command is used to specify the initial value for
 *       the mode, skip, timeout, and Constant Tone Extension type
 *       (set by the
 *       HCI_LE_Set_Periodic_Advertising_Sync_Transfer_Parameters
 *       command) to be used for all subsequent connections over the
 *       LE transport.
 *
 *  \param [in] mode
 *              0x00: No attempt is made to synchronize to the periodic
 *              advertising and no
 *              HCI_LE_Periodic_Advertising_Sync_Transfer_Received event is
 *              sent to the Host (default).
 *              0x01: An HCI_LE_Periodic_Advertising_Sync_Transfer_Received
 *              event is sent to the Host.
 *              HCI_LE_Periodic_Advertising_Report events will be disabled.
 *              0x02: An HCI_LE_Periodic_Advertising_Sync_Transfer_Received
 *              event is sent to the Host.
 *              HCI_LE_Periodic_Advertising_Report events will be enabled.
 *
 *  \param [in] skip
 *              The number of periodic advertising packets that can be
 *              skipped after a successful receive
 *              Range: 0x0000 to 0x01F3
 *
 *  \param [in] sync_timeout
 *              Synchronization timeout for the the periodic advertising
 *              train
 *              Range: 0x000A to 0x4000
 *              Time = N*10 ms
 *              Time Range: 100 ms to 163.84 s
 *
 *  \param [in] cte_type
 *              Bit number
 *              0: Do not sync to packets with an AoA Constant Tone
 *              Extension
 *              1: Do not sync to packets with an AoD Constant Tone
 *              Extension with 1 us slots
 *              2: Do not sync to packets with an AoD Constant Tone
 *              Extension with 2 us slots
 *              4: Do not sync to packets without a Constant Tone Extension
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_default_periodic_advertising_sync_transfer_parameters
           (
               UCHAR  mode,
               UINT16 skip,
               UINT16 sync_timeout,
               UCHAR  cte_type
           );
#endif /* HCI_LE_SET_DEFAULT_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS_SUPPORT */

#ifdef HCI_LE_GENERATE_DHKEY_V2_SUPPORT
/**
 *  \brief To initiate generation of a Diffie-Hellman key in the
 *         Controller
 *
 *  \par Description:
 *       The HCI_LE_Generate_DHKey command is used to initiate
 *       generation of a Diffie-Hellman key in the Controller for use
 *       over the LE transport. This command takes the remote P-256
 *       public key as input. The Diffie-Hellman key generation uses
 *       the private key generated by the HCI_LE_Read_Local_P-
 *       256_Public_Key command or the private debug key.
 *
 *  \param [in] remote_p_256_public_key
 *              The remote P-256 public key:
 *              X, Y format
 *              Octets 31-0: X co-ordinate
 *              Octets 63-32: Y co-ordinate
 *              Little-endian Format
 *
 *  \param [in] key_type
 *              0x00: Use the generated private key
 *              0x01: Use the debug private key
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_generate_dhkey_v2
           (
               UCHAR * remote_p_256_public_key,
               UCHAR  key_type
           );
#endif /* HCI_LE_GENERATE_DHKEY_V2_SUPPORT */

#ifdef HCI_LE_MODIFY_SLEEP_CLOCK_ACCURACY_SUPPORT
/**
 *  \brief To request that the Controller changes its sleep clock
 *         accuracy for testing purposes
 *
 *  \par Description:
 *       This command is used to request that the Controller changes
 *       its sleep clock accuracy for testing purposes. It should not
 *       be used under other circumstances.
 *
 *  \param [in] action
 *              0x00: Switch to a more accurate clock
 *              0x01: Switch to a less accurate clock
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_modify_sleep_clock_accuracy
           (
               UCHAR  action
           );
#endif /* HCI_LE_MODIFY_SLEEP_CLOCK_ACCURACY_SUPPORT */

#ifdef HCI_READ_LOCAL_SIMPLE_PAIRING_OPTIONS_SUPPORT
/**
 *  \brief To read the simple pairing options and the maximum
 *         encryption key size supported.
 *
 *  \par Description:
 *       This command is to read the simple pairing options and the
 *       maximum encryption key size supported.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
#define BT_hci_read_local_simple_pairing_options() \
        hci_common_api_handler_no_params \
        (HCI_READ_LOCAL_SIMPLE_PAIRING_OPTIONS_OPCODE)
#endif /* HCI_READ_LOCAL_SIMPLE_PAIRING_OPTIONS_SUPPORT */

/* New APIs from Bluetooth Spec 5.2 ---------------------------------------- */
#ifdef HCI_LE_READ_ISO_TX_SYNC_SUPPORT
/**
 *  \brief To read the Time_Stamp and Time_Offset of a transmitted
 *         ISO_SDU
 *
 *  \par Description:
 *       This command is used to read the Time_Stamp and Time_Offset
 *       of a transmitted ISO_SDU identified by the
 *       Packet_Sequence_Number on a CIS or BIS identified by the
 *       Connection_Handle parameter on the master or slave.
 *
 *  \param [in] connection_handle
 *              Range: 0x0000 to 0x0EFF
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_read_iso_tx_sync
           (
               UINT16 connection_handle
           );
#endif /* HCI_LE_READ_ISO_TX_SYNC_SUPPORT */

#ifdef HCI_LE_SET_CIG_PARAMETERS_SUPPORT
/**
 *  \brief To set the parameters of one or more CISes that are
 *         associated with a CIG in the Controller
 *
 *  \par Description:
 *       The HCI_LE_Set_CIG_Parameters command is used by a master
 *       Host to set the parameters of one or more CISes that are
 *       associated with a CIG in the Controller. Provided that none
 *       of the CISes in that CIG have been established, this command
 *       may also be used to modify or add CIS(s) to that CIG.
 *
 *  \param [in] cig_id
 *              0x00 to 0xEF: Used to identify a CIG
 *
 *  \param [in] sdu_interval_m_to_s
 *              0x000FF to 0xFFFFF: The interval, in microseconds, of
 *              periodic SDUs.
 *
 *  \param [in] sdu_interval_s_to_m
 *              0x000FF to 0xFFFFF: The interval, in microseconds, of
 *              periodic SDUs.
 *
 *  \param [in] sca
 *              0x00: 251 ppm to 500 ppm
 *              0x01: 151 ppm to 250 ppm
 *              0x02: 101 ppm to 150 ppm
 *              0x03: 76 ppm to 100 ppm
 *              0x04: 51 ppm to 75 ppm
 *              0x05: 31 ppm to 50 ppm
 *              0x06: 21 ppm to 30 ppm
 *              0x07: 0 ppm to 20 ppm
 *
 *  \param [in] packing
 *              0x00: Sequential
 *              0x01: Interleaved
 *
 *  \param [in] framing
 *              0x00: Unframed
 *              0x01: Framed
 *
 *  \param [in] max_transport_latency_m_to_s
 *              0x0005 to 0x0FA0: Maximum time, in milliseconds, for an SDU
 *              to be transported from the master Controller to slave
 *              Controller.
 *
 *  \param [in] max_transport_latency_s_to_m
 *              0x0005 to 0x0FA0: Maximum time, in milliseconds, for an SDU
 *              to be transported from the slave Controller to master
 *              Controller.
 *
 *  \param [in] cis_count
 *              0x00 to 0x10: Total number of CISes in the CIG being added
 *              or modified.
 *              The maximum value of the CIS_Count is limited to 16 (due to
 *              the maximum size of an HCI packet).
 *
 *  \param [in] cis_id
 *              0x00 to 0xEF: Used to identify a CIS
 *
 *  \param [in] max_sdu_m_to_s
 *              0x000 to 0xFFF: Maximum size, in octets, of an SDU from the
 *              master Host.
 *
 *  \param [in] max_sdu_s_to_m
 *              0x000 to 0xFFF: Maximum size, in octets, of an SDU from the
 *              slave Host.
 *
 *  \param [in] phy_m_to_s
 *              Bit number
 *              0: The transmitter PHY of packets from the master is LE 1M
 *              1: The transmitter PHY of packets from the master is LE 2M
 *              2: The transmitter PHY of packets from the master is LE
 *              Coded
 *
 *  \param [in] phy_s_to_m
 *              Bit number
 *              0: The transmitter PHY of packets from the slave is LE 1M
 *              1: The transmitter PHY of packets from the slave is LE 2M
 *              2: The transmitter PHY of packets from the slave is LE Coded
 *
 *  \param [in] rtn_m_to_s
 *              0x00 to 0x0F: Maximum number of times every CIS Data PDU
 *              should be retransmitted from the master to slave
 *
 *  \param [in] rtn_s_to_m
 *              0x00 to 0x0F: Maximum number of times every CIS Data PDU
 *              should be retransmitted from the slave to master
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_cig_parameters
           (
               UCHAR  cig_id,
               UINT32 sdu_interval_m_to_s,
               UINT32 sdu_interval_s_to_m,
               UCHAR  sca,
               UCHAR  packing,
               UCHAR  framing,
               UINT16 max_transport_latency_m_to_s,
               UINT16 max_transport_latency_s_to_m,
               UCHAR  cis_count,
               UCHAR * cis_id,
               UINT16 * max_sdu_m_to_s,
               UINT16 * max_sdu_s_to_m,
               UCHAR * phy_m_to_s,
               UCHAR * phy_s_to_m,
               UCHAR * rtn_m_to_s,
               UCHAR * rtn_s_to_m
           );
#endif /* HCI_LE_SET_CIG_PARAMETERS_SUPPORT */

#ifdef HCI_LE_SET_CIG_PARAMETERS_TEST_SUPPORT
/**
 *  \brief To set the parameters of CISes associated with a CIG, in the
 *         ISO Test Mode.
 *
 *  \par Description:
 *       The HCI_LE_Set_CIG_Parameters_Test command should be used in
 *       the ISO Test mode. The command is used by a master Host to
 *       set the parameters of CISes associated with a CIG. Provided
 *       all CISes in a CIG are not established, this command can
 *       also be used to modify or add additional CIS(s) to that CIG
 *       that is already stored.
 *
 *  \param [in] cig_id
 *              0x00 to 0xEF: Used to identify a CIG
 *
 *  \param [in] sdu_interval_m_to_s
 *              0x000FF to 0xFFFFF: The interval, in microseconds, of
 *              periodic SDUs.
 *
 *  \param [in] sdu_interval_s_to_m
 *              0x000FF to 0xFFFFF: The interval, in microseconds, of
 *              periodic SDUs.
 *
 *  \param [in] ft_m_to_s
 *              0x01 to 0xFF: The flush timeout in multiples of ISO_Interval
 *              for each payload sent from the master to slave.
 *
 *  \param [in] ft_s_to_m
 *              0x01 to 0xFF: The flush timeout in multiples of ISO_Interval
 *              for each payload sent from the slave to master.
 *
 *  \param [in] iso_interval
 *              Time duration of the isochronous interval.
 *              Range: 0x0004 to 0x0C80
 *              Time = N * 1.25 ms
 *              Time Range: 5 ms to 4 s
 *
 *  \param [in] sca
 *              0x00: 251 ppm to 500 ppm
 *              0x01: 151 ppm to 250 ppm
 *              0x02: 101 ppm to 150 ppm
 *              0x03: 76 ppm to 100 ppm
 *              0x04: 51 ppm to 75 ppm
 *              0x05: 31 ppm to 50 ppm
 *              0x06: 21 ppm to 30 ppm
 *              0x07: 0 ppm to 20 ppm
 *
 *  \param [in] packing
 *              0x00: Sequential
 *              0x01: Interleaved
 *
 *  \param [in] framing
 *              0x00: Unframed
 *              0x01: Framed
 *
 *  \param [in] cis_count
 *              0x00 to 0x1F: Total number of CISes in the CIG being added
 *              or modified.
 *
 *  \param [in] cis_id
 *              0x00 to 0xEF: Used to identify a CIS
 *
 *  \param [in] nse
 *              0x01 to 0x1F: Maximum number of subevents in each
 *              isochronous interval of CIS
 *
 *  \param [in] max_sdu_m_to_s
 *              0x0000 to 0x0FFF: Maximum size, in octets, of an SDU from
 *              the master Host.
 *
 *  \param [in] max_sdu_s_to_m
 *              0x0000 to 0x0FFF: Maximum size, in octets, of an SDU from
 *              the slave Host.
 *
 *  \param [in] max_pdu_m_to_s
 *              0x0000 to 0x00FB: Maximum size, in octets, of the payload
 *              from master Link Layer to slave Link Layer.
 *
 *  \param [in] max_pdu_s_to_m
 *              0x0000 to 0x00FB: Maximum size, in octets, of the payload
 *              from slave Link Layer to master Link Layer.
 *
 *  \param [in] phy_m_to_s
 *              Bit number
 *              0: The transmitter PHY of packets from the master is LE 1M
 *              1: The transmitter PHY of packets from the master is LE 2M
 *              2: The transmitter PHY of packets from the master is LE
 *              Coded
 *
 *  \param [in] phy_s_to_m
 *              Bit number
 *              0: The transmitter PHY of packets from the slave is LE 1M
 *              1: The transmitter PHY of packets from the slave is LE 2M
 *              2: The transmitter PHY of packets from the slave is LE Coded
 *
 *  \param [in] bn_m_to_s
 *              0x00: No isochronous data from the master to the slave
 *              0x01 to 0x0F: The burst number for master to slave
 *              transmission
 *
 *  \param [in] bn_s_to_m
 *              0x00: No isochronous data from the slave to the master
 *              0x01 to 0x0F: The burst number for slave to master
 *              transmission
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_cig_parameters_test
           (
               UCHAR  cig_id,
               UINT32 sdu_interval_m_to_s,
               UINT32 sdu_interval_s_to_m,
               UCHAR  ft_m_to_s,
               UCHAR  ft_s_to_m,
               UINT16 iso_interval,
               UCHAR  sca,
               UCHAR  packing,
               UCHAR  framing,
               UCHAR  cis_count,
               UCHAR * cis_id,
               UCHAR * nse,
               UINT16 * max_sdu_m_to_s,
               UINT16 * max_sdu_s_to_m,
               UINT16 * max_pdu_m_to_s,
               UINT16 * max_pdu_s_to_m,
               UCHAR * phy_m_to_s,
               UCHAR * phy_s_to_m,
               UCHAR * bn_m_to_s,
               UCHAR * bn_s_to_m
           );
#endif /* HCI_LE_SET_CIG_PARAMETERS_TEST_SUPPORT */

#ifdef HCI_LE_CREATE_CIS_SUPPORT
/**
 *  \brief To create one or more CISes.
 *
 *  \par Description:
 *       The HCI_LE_Create_CIS command is used by the master Host to
 *       create one or more CISes using the connections identified by
 *       the ACL_Connection_Handle[i] parameter array.
 *
 *  \param [in] cis_count
 *              0x00 to 0x10: Total number of CISes in the CIG being added
 *              or modified.
 *              The maximum value of the CIS_Count is limited to 16 (due to
 *              the maximum size of an HCI packet).
 *
 *  \param [in] cis_connection_handle
 *              List of connection handles of CISes
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] acl_connection_handle
 *              List of connection handles of ACLs
 *              Range: 0x0000 to 0x0EFF
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_create_cis
           (
               UCHAR  cis_count,
               UINT16 * cis_connection_handle,
               UINT16 * acl_connection_handle
           );
#endif /* HCI_LE_CREATE_CIS_SUPPORT */

#ifdef HCI_LE_REMOVE_CIG_SUPPORT
/**
 *  \brief To remove all the CISes associated with the CIG.
 *
 *  \par Description:
 *       The HCI_LE_Remove_CIG command is used by the master Host to
 *       remove all the CISes associated with the CIG identified by
 *       CIG_ID. This command shall not be issued when any CIS in the
 *       CIG has already been established or is pending
 *       establishment. An established CIS or pending establishment
 *       of CIS can be terminated using the HCI_Disconnect command.
 *
 *  \param [in] cig_id
 *              0x00 to 0xEF: Used to identify a CIG
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_remove_cig
           (
               UCHAR  cig_id
           );
#endif /* HCI_LE_REMOVE_CIG_SUPPORT */

#ifdef HCI_LE_ACCEPT_CIS_REQUEST_SUPPORT
/**
 *  \brief To accept the request for the CIS.
 *
 *  \par Description:
 *       The HCI_LE_Accept_CIS_Request command is used by the slave
 *       Host to inform the Controller to accept the request for the
 *       CIS that is identified by the Connection_Handle.
 *
 *  \param [in] connection_handle
 *              Connection handle of the CIS
 *              Range: 0x0000 to 0x0EFF
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_accept_cis_request
           (
               UINT16 connection_handle
           );
#endif /* HCI_LE_ACCEPT_CIS_REQUEST_SUPPORT */

#ifdef HCI_LE_REJECT_CIS_REQUEST_SUPPORT
/**
 *  \brief To reject the request for the CIS.
 *
 *  \par Description:
 *       The HCI_LE_Reject_CIS_Request command is used by the slave
 *       Host to inform the Controller to reject the request for the
 *       CIS that is identified by the Connection_Handle.
 *
 *  \param [in] connection_handle
 *              Connection handle of the CIS
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] reason
 *              Reason the CIS request was rejected. See [Vol 2] Part D,
 *              Error Codes for a list of error codes and descriptions.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_reject_cis_request
           (
               UINT16 connection_handle,
               UCHAR  reason
           );
#endif /* HCI_LE_REJECT_CIS_REQUEST_SUPPORT */

#ifdef HCI_LE_CREATE_BIG_SUPPORT
/**
 *  \brief To create a BIG with one or more BISes.
 *
 *  \par Description:
 *       The HCI_LE_Create_BIG command is used to create a BIG with
 *       one or more BISes. All BISes in a BIG have the same value
 *       for all parameters.
 *
 *  \param [in] big_handle
 *              0x00 to 0xEF: Used to identify the BIG
 *
 *  \param [in] advertising_handle
 *              0x00 to 0xEF: Used to identify the periodic advertising
 *              train.
 *
 *  \param [in] num_bis
 *              0x01 to 0x1F: Total number of BISes in the BIG
 *
 *  \param [in] sdu_interval
 *              0x000100 to 0xFFFFF: The interval, in microseconds, of
 *              periodic SDUs
 *
 *  \param [in] max_sdu
 *              0x000 to 0xFFF: Maximum size of an SDU, in octets.
 *
 *  \param [in] max_transport_latency
 *              0x0000 to 0x0FA0: Maximum time, in milliseconds, for
 *              transmitting an SDU
 *
 *  \param [in] rtn
 *              0x00 to 0x0F: Maximum number of times every BIS Data PDU
 *              should be retransmitted
 *
 *  \param [in] phy
 *              Bit number
 *              0: The transmitter PHY of packets is LE 1M
 *              1: The transmitter PHY of packets is LE 2M
 *              2: The transmitter PHY of packets is LE Coded
 *
 *  \param [in] packing
 *              0x00: Sequential
 *              0x01: Interleaved
 *
 *  \param [in] framing
 *              0x00: Unframed
 *              0x01: Framed
 *
 *  \param [in] encryption
 *              0x00: Unencrypted
 *              0x01: Encrypted
 *
 *  \param [in] broadcast_code
 *              The code used to derive the session key that is used to
 *              encrypt and decrypt BIS payloads.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_create_big
           (
               UCHAR  big_handle,
               UCHAR  advertising_handle,
               UCHAR  num_bis,
               UINT32 sdu_interval,
               UINT16 max_sdu,
               UINT16 max_transport_latency,
               UCHAR  rtn,
               UCHAR  phy,
               UCHAR  packing,
               UCHAR  framing,
               UCHAR  encryption,
               UCHAR * broadcast_code
           );
#endif /* HCI_LE_CREATE_BIG_SUPPORT */

#ifdef HCI_LE_CREATE_BIG_TEST_SUPPORT
/**
 *  \brief To create one or more BISes of a BIG.
 *
 *  \par Description:
 *       The HCI_LE_Create_BIG_Test command should be used in the ISO
 *       Test mode. The command is used to create one or more BISes
 *       of a BIG. All BISes in the BIG have the same values for all
 *       parameters.
 *
 *  \param [in] big_handle
 *              0x00 to 0xEF: Used to identify the BIG
 *
 *  \param [in] advertising_handle
 *              0x00 to 0xEF: Used to identify the periodic advertising
 *              train.
 *
 *  \param [in] num_bis
 *              0x01 to 0x1F: Total number of BISes in the BIG
 *
 *  \param [in] sdu_interval
 *              0x000100 to 0xFFFFF: The interval, in microseconds, of
 *              periodic SDUs
 *
 *  \param [in] iso_interval
 *              The duration of an isochronous interval.
 *              Range: 0x0004 to 0x0C80
 *              Time = N * 1.25 ms
 *              Time Range: 5 ms to 4 s
 *
 *  \param [in] nse
 *              0x01 to 0x1F: The total number of subevents in each interval
 *              of each BIS in the BIG.
 *
 *  \param [in] max_sdu
 *              0x000 to 0xFFF: Maximum size of an SDU, in octets.
 *
 *  \param [in] max_pdu
 *              0x0001 to 0x00FB: Maximum size, in octets, of payload.
 *
 *  \param [in] phy
 *              Bit number
 *              0: The transmitter PHY of packets is LE 1M
 *              1: The transmitter PHY of packets is LE 2M
 *              2: The transmitter PHY of packets is LE Coded
 *
 *  \param [in] packing
 *              0x00: Sequential
 *              0x01: Interleaved
 *
 *  \param [in] framing
 *              0x00: Unframed
 *              0x01: Framed
 *
 *  \param [in] bn
 *              0x01 to 0x07: The number of new payloads in each interval
 *              for each BIS.
 *
 *  \param [in] irc
 *              0x01 to 0x0F: The number of times the scheduled payload(s)
 *              are transmitted in a given event.
 *
 *  \param [in] pto
 *              0x00 to 0x0F: Offset used for pre-transmissions.
 *
 *  \param [in] encryption
 *              0x00: Unencrypted
 *              0x01: Encrypted
 *
 *  \param [in] broadcast_code
 *              The code used to derive the session key that is used to
 *              encrypt and decrypt BIS payloads.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_create_big_test
           (
               UCHAR  big_handle,
               UCHAR  advertising_handle,
               UCHAR  num_bis,
               UINT32 sdu_interval,
               UINT16 iso_interval,
               UCHAR  nse,
               UINT16 max_sdu,
               UINT16 max_pdu,
               UCHAR  phy,
               UCHAR  packing,
               UCHAR  framing,
               UCHAR  bn,
               UCHAR  irc,
               UCHAR  pto,
               UCHAR  encryption,
               UCHAR * broadcast_code
           );
#endif /* HCI_LE_CREATE_BIG_TEST_SUPPORT */

#ifdef HCI_LE_TERMINATE_BIG_SUPPORT
/**
 *  \brief To terminate a BIG.
 *
 *  \par Description:
 *       The HCI_LE_Terminate_BIG command is used to terminate a BIG
 *       identified by the BIG_Handle parameter.  The command also
 *       terminates the transmission of all BISes of the BIG,
 *       destroys the associated connection handles of the BISes in
 *       the BIG and removes the data paths for all BISes in the BIG.
 *
 *  \param [in] big_handle
 *              0x00 to 0xEF: Used to identify the BIG
 *
 *  \param [in] reason
 *              Reason. See [Vol 2] Part D, Error Codes for a list of error
 *              codes and descriptions.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_terminate_big
           (
               UCHAR  big_handle,
               UCHAR  reason
           );
#endif /* HCI_LE_TERMINATE_BIG_SUPPORT */

#ifdef HCI_LE_BIG_CREATE_SYNC_SUPPORT
/**
 *  \brief To synchronize to a BIG
 *
 *  \par Description:
 *       The HCI_LE_BIG_Create_Sync command is used to synchronize to
 *       a BIG described in the periodic advertising train specified
 *       by the Sync_Handle parameter.
 *
 *  \param [in] big_handle
 *              0x00 to 0xEF: Used to identify the BIG
 *
 *  \param [in] sync_handle
 *              Identifier of the periodic advertising train
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] encryption
 *              0x00: Unencrypted
 *              0x01: Encrypted
 *
 *  \param [in] broadcast_code
 *              128-bit code used for deriving the session key for
 *              decrypting payloads of BISes in the BIG.
 *
 *  \param [in] mse
 *              0x00: The Controller can schedule reception of any number of
 *              subevents up to NSE.
 *              0x01 to 0x1F: Maximum number of subevents that should be
 *              used to receive data payloads in each BIS event
 *
 *  \param [in] big_sync_timeout
 *              Synchronization timeout for the BIG
 *              Range: 0x000A to 0x4000
 *              Time = N*10 ms
 *              Time Range: 100 ms to 163.84 s
 *
 *  \param [in] num_bis
 *              0x01 to 0x1F: Total number of BISes to synchronize
 *
 *  \param [in] bis
 *              0x01 to 0x1F: List of indices of BISes
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_big_create_sync
           (
               UCHAR  big_handle,
               UINT16 sync_handle,
               UCHAR  encryption,
               UCHAR * broadcast_code,
               UCHAR  mse,
               UINT16 big_sync_timeout,
               UCHAR  num_bis,
               UCHAR * bis
           );
#endif /* HCI_LE_BIG_CREATE_SYNC_SUPPORT */

#ifdef HCI_LE_BIG_TERMINATE_SYNC_SUPPORT
/**
 *  \brief To stop synchronizing or cancel the process of synchronizing
 *         to the BIG
 *
 *  \par Description:
 *       The HCI_LE_BIG_Terminate_Sync command is used to stop
 *       synchronizing or cancel the process of synchronizing to the
 *       BIG identified by the BIG_Handle parameter.  The command
 *       also terminates the reception of BISes in the BIG specified
 *       in the HCI_LE_BIG_Create_Sync command, destroys the
 *       associated connection handles of the BISes in the BIG and
 *       removes the data paths for all BISes in the BIG.
 *
 *  \param [in] big_handle
 *              0x00 to 0xEF: Used to identify the BIG
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_big_terminate_sync
           (
               UCHAR  big_handle
           );
#endif /* HCI_LE_BIG_TERMINATE_SYNC_SUPPORT */

#ifdef HCI_LE_REQUEST_PEER_SCA_SUPPORT
/**
 *  \brief To read the Sleep Clock Accuracy of the peer device.
 *
 *  \par Description:
 *       This command is used to read the Sleep Clock Accuracy of the
 *       peer device.
 *
 *  \param [in] connection_handle
 *              Connection handle of the ACL
 *              Range: 0x0000 to 0x0EFF
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_request_peer_sca
           (
               UINT16 connection_handle
           );
#endif /* HCI_LE_REQUEST_PEER_SCA_SUPPORT */

#ifdef HCI_LE_SETUP_ISO_DATA_PATH_SUPPORT
/**
 *  \brief To identify and create the isochronous data path between the
 *         Host and the Controller.
 *
 *  \par Description:
 *       The HCI_LE_Setup_ISO_Data_Path command is used to identify
 *       and create the isochronous data path between the Host and
 *       the Controller for an established CIS or BIS identified by
 *       the Connection_Handle parameter.
 *
 *  \param [in] connection_handle
 *              Connection handle of the CIS or BIS
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] data_path_direction
 *              0x00: Input (Host to Controller)
 *              0x01: Output (Controller to Host)
 *
 *  \param [in] data_path_id
 *              0x00: HCI
 *              0x01-0xFE: Logical Channel Number. The meaning of the
 *              logical channel is vendor defined)
 *
 *  \param [in] codec_id
 *              Octet 0: See Assigned Numbers for Coding Format
 *              Octet 1 to 2: Company ID, see Assigned Numbers for Company
 *              Identifier. Shall be ignored if octet 0 is not 0xFF.
 *              Octet 3 to 4: Vendor-defined codec ID. Shall be ignored if
 *              octet 0 is not 0xFF.
 *
 *  \param [in] controller_delay
 *              Controller delay in microseconds
 *              Range: 0x000000 to 0x3D0900
 *              Time range: 0 s to 4 s0x00: HCI
 *
 *  \param [in] codec_configuration_length
 *              Length of codec configuration.
 *
 *  \param [in] codec_configuration
 *              Codec-specific configuration data.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_setup_iso_data_path
           (
               UINT16 connection_handle,
               UCHAR  data_path_direction,
               UCHAR  data_path_id,
               UCHAR * codec_id,
               UINT32 controller_delay,
               UCHAR  codec_configuration_length,
               UCHAR * codec_configuration
           );
#endif /* HCI_LE_SETUP_ISO_DATA_PATH_SUPPORT */

#ifdef HCI_LE_REMOVE_ISO_DATA_PATH_SUPPORT
/**
 *  \brief To remove the input and/or output data path(s) associated
 *         with a CIS or BIS.
 *
 *  \par Description:
 *       The HCI_LE_Remove_ISO_Data_Path command is used to remove
 *       the input and/or output data path(s) associated with a CIS
 *       or BIS identified by the Connection_Handle parameter.
 *
 *  \param [in] connection_handle
 *              Connection handle of a CIS or BIS
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] data_path_direction
 *              Bit-0: Remove input data path
 *              Bit-1: Remove output data path
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_remove_iso_data_path
           (
               UINT16 connection_handle,
               UCHAR  data_path_direction
           );
#endif /* HCI_LE_REMOVE_ISO_DATA_PATH_SUPPORT */

#ifdef HCI_LE_ISO_TRANSMIT_TEST_SUPPORT
/**
 *  \brief To configure an established CIS or BIS to transmit payloads.
 *
 *  \par Description:
 *       The HCI_LE_ISO_Transmit_Test command is used to configure an
 *       established CIS or BIS specified by the Connection_Handle
 *       parameter, and transmit test payloads which are generated by
 *       the Controller.
 *
 *  \param [in] connection_handle
 *              Connection handle of a CIS or BIS
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] payload_type
 *              0x00: Maximum length payload
 *              0x01: Variable length payload
 *              0x02: Zero length payload
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_iso_transmit_test
           (
               UINT16 connection_handle,
               UCHAR  payload_type
           );
#endif /* HCI_LE_ISO_TRANSMIT_TEST_SUPPORT */

#ifdef HCI_LE_ISO_RECEIVE_TEST_SUPPORT
/**
 *  \brief To configure an established CIS or BIS to receive payloads.
 *
 *  \par Description:
 *       The HCI_LE_ISO_Receive_Test command is used to configure an
 *       established CIS or a synchronized BIG specified by the
 *       Connection_Handle parameter to receive payloads.
 *
 *  \param [in] connection_handle
 *              Connection handle of a CIS or BIS
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] payload_type
 *              0x00: Maximum length SDU
 *              0x01: Variable length SDU
 *              0x02: Zero length SDU
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_iso_receive_test
           (
               UINT16 connection_handle,
               UCHAR  payload_type
           );
#endif /* HCI_LE_ISO_RECEIVE_TEST_SUPPORT */

#ifdef HCI_LE_ISO_READ_TEST_COUNTERS_SUPPORT
/**
 *  \brief To read the test counters.
 *
 *  \par Description:
 *       The HCI_LE_ISO_Read_Test_Counters command is used to read
 *       the test counters in the Controller which is configured in
 *       ISO Receive Test mode for a CIS or BIS specified by the
 *       Connection_Handle.
 *
 *  \param [in] connection_handle
 *              Connection handle of a CIS or BIS
 *              Range: 0x0000 to 0x0EFF
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_iso_read_test_counters
           (
               UINT16 connection_handle
           );
#endif /* HCI_LE_ISO_READ_TEST_COUNTERS_SUPPORT */

#ifdef HCI_LE_ISO_TEST_END_SUPPORT
/**
 *  \brief To terminate the ISO Transmit and/or Receive Test mode for a
 *         CIS or BIS.
 *
 *  \par Description:
 *       The HCI_LE_ISO_Test_End command is used to terminate the ISO
 *       Transmit and/or Receive Test mode for a CIS or BIS specified
 *       by the Connection_Handle parameter but does not terminate
 *       the CIS or BIS.
 *
 *  \param [in] connection_handle
 *              Connection handle of a CIS or BIS
 *              Range: 0x0000 to 0x0EFF
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_iso_test_end
           (
               UINT16 connection_handle
           );
#endif /* HCI_LE_ISO_TEST_END_SUPPORT */

#ifdef HCI_LE_SET_HOST_FEATURE_SUPPORT
/**
 *  \brief To set or clear a bit controlled by the Host in the Link
 *         Layer Feature Set stored in the Controller.
 *
 *  \par Description:
 *       The HCI_LE_Set_Host_Feature command is used by the Host to
 *       set or clear a bit controlled by the Host in the Link Layer
 *       FeatureSet stored in the Controller.
 *
 *  \param [in] bit_number
 *              Bit position in the FeatureSet
 *
 *  \param [in] bit_value
 *              0x00: The Host feature is disabled and so the bit in the
 *              FeatureSet shall be set to 0
 *              0x01: The Host feature is enabled and so the bit in the
 *              FeatureSet shall be set to 1
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_host_feature
           (
               UCHAR  bit_number,
               UCHAR  bit_value
           );
#endif /* HCI_LE_SET_HOST_FEATURE_SUPPORT */

#ifdef HCI_LE_READ_ISO_LINK_QUALITY_SUPPORT
/**
 *  \brief To read ISO Link Quality
 *
 *  \par Description:
 *       This command returns the values of various counters related
 *       to link quality that are associated with the isochronous
 *       stream specified by the Connection_Handle parameter.
 *
 *  \param [in] connection_handle
 *              Connection handle of a CIS or BIS
 *              Range: 0x0000 to 0x0EFF
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_read_iso_link_quality
           (
               UINT16 connection_handle
           );
#endif /* HCI_LE_READ_ISO_LINK_QUALITY_SUPPORT */

#ifdef HCI_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL_SUPPORT
/**
 *  \brief To read the current and maximum transmit power levels of the
 *         local Controller
 *
 *  \par Description:
 *       The HCI_LE_Enhanced_Read_Transmit_Power_Level command is
 *       used to read the current and maximum transmit power levels
 *       of the local Controller on the ACL connection identified by
 *       the Connection_Handle parameter and the PHY indicated by the
 *       PHY parameter.
 *
 *  \param [in] connection_handle
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] phy
 *              0x01: LE 1M PHY
 *              0x02: LE 2M PHY
 *              0x03: LE Coded PHY with S=8 data coding
 *              0x04: LE Coded PHY with S=2 data coding
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_enhanced_read_transmit_power_level
           (
               UINT16 connection_handle,
               UCHAR  phy
           );
#endif /* HCI_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL_SUPPORT */

#ifdef HCI_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL_SUPPORT
/**
 *  \brief To read the transmit power level used by the remote
 *         Controller
 *
 *  \par Description:
 *       The HCI_LE_Read_Remote_Transmit_Power_Level command is used
 *       to read the transmit power level used by the remote
 *       Controller on the ACL connection that is identified by the
 *       Connection_Handle parameter and the PHY indicated by the PHY
 *       parameter.
 *
 *  \param [in] connection_handle
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] phy
 *              0x01: LE 1M PHY
 *              0x02: LE 2M PHY
 *              0x03: LE Coded PHY with S=8 data coding
 *              0x04: LE Coded PHY with S=2 data coding
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_read_remote_transmit_power_level
           (
               UINT16 connection_handle,
               UCHAR  phy
           );
#endif /* HCI_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL_SUPPORT */

#ifdef HCI_LE_SET_PATH_LOSS_REPORTING_PARAMETERS_SUPPORT
/**
 *  \brief To set the path loss threshold reporting parameters
 *
 *  \par Description:
 *       The HCI_LE_Set_Path_Loss_Reporting_Parameters command is
 *       used to set the path loss threshold reporting parameters for
 *       the ACL connection identified by the Connection_Handle
 *       parameter.
 *
 *  \param [in] connection_handle
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] high_threshold
 *              High threshold for the path loss
 *              Units: dB
 *              0xFF: High Threshold unused
 *
 *  \param [in] high_hysteresis
 *              Hysteresis value for the high threshold
 *              Units: dB
 *
 *  \param [in] low_threshold
 *              Low threshold for the path loss
 *              Units: dB
 *
 *  \param [in] low_hysteresis
 *              Hysteresis value for the low threshold
 *              Units: dB
 *
 *  \param [in] min_time_spent
 *              Minimum time in number of connection events to be observed
 *              once the path crosses the threshold before an event is
 *              generated
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_path_loss_reporting_parameters
           (
               UINT16 connection_handle,
               UCHAR  high_threshold,
               UCHAR  high_hysteresis,
               UCHAR  low_threshold,
               UCHAR  low_hysteresis,
               UINT16 min_time_spent
           );
#endif /* HCI_LE_SET_PATH_LOSS_REPORTING_PARAMETERS_SUPPORT */

#ifdef HCI_LE_SET_PATH_LOSS_REPORTING_ENABLE_SUPPORT
/**
 *  \brief To enable or disable path loss reporting
 *
 *  \par Description:
 *       The HCI_LE_Set_Path_Loss_Reporting_Enable command is used to
 *       enable or disable path loss reporting for the ACL connection
 *       identified by the Connection_Handle parameter. Path loss
 *       reporting is disabled when the connection is first created.
 *
 *  \param [in] connection_handle
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] enable
 *              0x00: Reporting disabled
 *              0x01: Reporting enabled
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_path_loss_reporting_enable
           (
               UINT16 connection_handle,
               UCHAR  enable
           );
#endif /* HCI_LE_SET_PATH_LOSS_REPORTING_ENABLE_SUPPORT */

#ifdef HCI_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE_SUPPORT
/**
 *  \brief To enable or disable the reporting to the local Host of
 *         transmit power level changes in the local and remote
 *         Controllers
 *
 *  \par Description:
 *       The HCI_LE_Set_Transmit_Power_Reporting_Enable command is
 *       used to enable or disable the reporting to the local Host of
 *       transmit power level changes in the local and remote
 *       Controllers for the ACL connection identified by the
 *       Connection_Handle parameter. Reporting is disabled when the
 *       connection is first created.
 *
 *  \param [in] connection_handle
 *              Range: 0x0000 to 0x0EFF
 *
 *  \param [in] local_enable
 *              0x00: Disable local transmit power reports
 *              0x01: Enable local transmit power reports
 *
 *  \param [in] remote_enable
 *              0x00: Disable remote transmit power reports
 *              0x01: Enable remote transmit power reports
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_le_set_transmit_power_reporting_enable
           (
               UINT16 connection_handle,
               UCHAR  local_enable,
               UCHAR  remote_enable
           );
#endif /* HCI_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE_SUPPORT */

#ifdef HCI_SET_ECOSYSTEM_BASE_INTERVAL_SUPPORT
/**
 *  \brief To give hint to controller about base communication
 *         interval.
 *
 *  \par Description:
 *       This command provides a hint to the Controller specifying
 *       the base communication interval the Controller can expect
 *       current and future communications to use.
 *
 *  \param [in] interval
 *              0x0000: Ignore any previous hint
 *              N = 0xXXXX: Base interval of the ecosystem
 *              Range: 0x0002 to 0x7DF0
 *              Time = N * 1.25 ms
 *              Time Range: 2.5 ms to 40.9 s.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_set_ecosystem_base_interval
           (
               UINT16 interval
           );
#endif /* HCI_SET_ECOSYSTEM_BASE_INTERVAL_SUPPORT */

#ifdef HCI_CONFIGURE_DATA_PATH_SUPPORT
/**
 *  \brief To request the Controller to configure the data transport
 *         path.
 *
 *  \par Description:
 *       This command is used to request the Controller to configure
 *       the data transport path in a given direction between the
 *       Controller and the Host.
 *
 *  \param [in] data_path_direction
 *              0x00: Input (Host to Controller)
 *              0x01: Output (Controller to Host)
 *
 *  \param [in] data_path_id
 *              Logical channel number; the meaning is vendor-specific.
 *
 *  \param [in] vendor_specific_config_length
 *              Length of the vendor-specific configuration data
 *
 *  \param [in] vendor_specific_config
 *              Vendor-specific configuration data for the data path being
 *              configured.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_configure_data_path
           (
               UCHAR  data_path_direction,
               UCHAR  data_path_id,
               UCHAR  vendor_specific_config_length,
               UCHAR * vendor_specific_config
           );
#endif /* HCI_CONFIGURE_DATA_PATH_SUPPORT */

#ifdef HCI_READ_LOCAL_SUPPORTED_CODECS_V2_SUPPORT
/**
 *  \brief To read a list of the Bluetooth SIG defined and vendor
 *         specific codecs supported by the Controller,
 *
 *  \par Description:
 *       This command reads a list of the Bluetooth SIG approved
 *       codecs supported by the Controller, as well as vendor
 *       specific codecs, which are defined by an individual
 *       manufacturer.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
#define BT_hci_read_local_supported_codecs_v2() \
        hci_common_api_handler_no_params \
        (HCI_READ_LOCAL_SUPPORTED_CODECS_V2_OPCODE)
#endif /* HCI_READ_LOCAL_SUPPORTED_CODECS_V2_SUPPORT */

#ifdef HCI_READ_LOCAL_SUPPORTED_CODEC_CAPABILITIES_SUPPORT
/**
 *  \brief To read list of codec capabilities for a given codec.
 *
 *  \par Description:
 *       This command is used to read codec capabilities supported by
 *       the Controller for a given codec.
 *
 *  \param [in] codec_id
 *              Octet 0: See Assigned Numbers for Coding Format
 *              Octets 1 to 2: Company ID, see Assigned Numbers for Company
 *              Identifier. Shall be ignored if octet 0 is not 0xFF.
 *              Octets 3 to 4: Vendor-defined codec ID. Shall be ignored if
 *              octet 0 is not 0xFF.
 *
 *  \param [in] logical_transport_type
 *              0x01: BR/EDR ACL
 *              0x02: BR/EDR SCO or eSCO
 *              0x03: LE CIS
 *              0x04: LE BIS
 *
 *  \param [in] direction
 *              0x00: Input (Host to Controller)
 *              0x01: Output (Controller to Host)
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_read_local_supported_codec_capabilities
           (
               UCHAR * codec_id,
               UCHAR  logical_transport_type,
               UCHAR  direction
           );
#endif /* HCI_READ_LOCAL_SUPPORTED_CODEC_CAPABILITIES_SUPPORT */

#ifdef HCI_READ_LOCAL_SUPPORTED_CONTROLLER_DELAY_SUPPORT
/**
 *  \brief To read the range of supported Controller delays for a given
 *         codec.
 *
 *  \par Description:
 *       This command is used to read the range of supported
 *       Controller delays for a given codec.
 *
 *  \param [in] codec_id
 *              Octet 0: See Assigned Numbers for Coding Format
 *              Octets 1 to 2: Company ID, see Assigned Numbers for Company
 *              Identifier. Shall be ignored if octet 0 is not 0xFF.
 *              Octets 3 to 4: Vendor-defined codec ID. Shall be ignored if
 *              octet 0 is not 0xFF.
 *
 *  \param [in] logical_transport_type
 *              0x01: BR/EDR ACL
 *              0x02: BR/EDR SCO or eSCO
 *              0x03: LE CIS
 *              0x04: LE BIS
 *
 *  \param [in] direction
 *              0x00: Input (Host to Controller)
 *              0x01: Output (Controller to Host)
 *
 *  \param [in] codec_configuration_length
 *              Length of codec configuration.
 *
 *  \param [in] codec_configuration
 *              Codec-specific configuration data.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
API_RESULT BT_hci_read_local_supported_controller_delay
           (
               UCHAR * codec_id,
               UCHAR  logical_transport_type,
               UCHAR  direction,
               UCHAR  codec_configuration_length,
               UCHAR * codec_configuration
           );
#endif /* HCI_READ_LOCAL_SUPPORTED_CONTROLLER_DELAY_SUPPORT */


/* ------------------------------------------------------------------------- */

/* New APIs from Bluetooth Spec 2.1+EDR ------------------------------------ */

#ifdef BT_SSR
API_RESULT BT_hci_sniff_subrating
           (
               /* IN */ UINT16    connection_handle,
               /* IN */ UINT16    maximum_latency,
               /* IN */ UINT16    minimum_remote_timeout,
               /* IN */ UINT16    minimum_local_timeout
           );
#endif /* BT_SSR */

#ifdef BT_EIR
#define BT_hci_read_extended_inquiry_response() \
        hci_common_api_handler_no_params \
        (HCI_READ_EXTENDED_INQUIRY_RESPONSE_OPCODE)

API_RESULT BT_hci_write_extended_inquiry_response
           (
               /* IN */ UCHAR             fec_required,
               /* IN */ HCI_EIR_DATA *    eir_data_field,
               /* IN */ UCHAR             total_eir_fields
           );
/**
 *  \brief To read the Inquiry Mode configuration parameter
 *
 *  \par Description:
 *       This API reads the inquiry Transmit Power level used to transmit the
 *       FHS and EIR data packets. This can be used directly in the Transmit
 *       Power Level EIR data type.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_inquiry_response_transmit_power_level() \
        hci_common_api_handler_no_params \
        (HCI_READ_INQUIRY_RESP_TX_POWER_LEVEL_OPCODE)
/**
 *  \brief To write the Inquiry Mode configuration parameter
 *
 *  \par Description:
 *       This API writes the inquiry transmit power level used to transmit the
 *       inquiry (ID) data packets. The Controller attempts to use the supported
 *       transmit power level closest to the tx_power parameter.
 *
 *  \param [in] tx_power
 *         Signed integer value in the range -70 <= N <= 20 (unit: dbm)
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_inquiry_response_transmit_power_level
           (
               /* IN */ INT8    tx_power
           );
#endif /* BT_EIR */

#ifdef BT_EPR
#define BT_hci_refresh_encryption_key(p1) \
        hci_write_command_UINT16 (HCI_REFRESH_ENCRYPTION_KEY_OPCODE, (p1))
#endif /* BT_EPR */

#ifdef BT_EDR
/**
 *  \brief To read Erroneous Data Reporting parameter from Controller.
 *
 *  \par Description:
 *       This API can be used to read the Erroneous Data Reporting parameter
 *       from controller.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_default_erroneous_data_reporting() \
        hci_common_api_handler_no_params \
        (HCI_READ_DEFAULT_ERRONEOUS_DATA_REPORTING_OPCODE)
/**
 *  \brief  To write Erroneous Data Reporting parameter to Controller.
 *
 *  \par Description:
 *       This API can be used to write the Erroneous Data Reporting parameter to
 *       controller. The new value for the Erroneous Data Reporting parameter
 *       does not apply to existing synchronous connections.
 *
 *  \param [in] erroneous_data_reporting
 *         0x00: Erroneous Data Reporting disabled
 *         0x01: Erroneous Data Reporting enabled
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_default_erroneous_data_reporting
           (
               UCHAR  erroneous_data_reporting
           );
#endif /* BT_EDR */

#ifdef BT_SSP
#define BT_hci_read_simple_pairing_mode() \
        hci_common_api_handler_no_params (HCI_READ_SIMPLE_PAIRING_MODE_OPCODE)

#define BT_hci_write_simple_pairing_mode(p1) \
        hci_write_command_UCHAR (HCI_WRITE_SIMPLE_PAIRING_MODE_OPCODE, (p1))
#endif /* BT_SSP */

#ifdef BT_SSP_PE
API_RESULT BT_hci_send_keypress_notification
           (
               UCHAR *    bd_addr,
               UCHAR      notification_type
           );
#endif /* BT_SSP_PE */

#ifdef BT_SSP_OOB
#define BT_hci_read_local_oob_data() \
        hci_common_api_handler_no_params (HCI_READ_LOCAL_OOB_DATA_OPCODE)
#endif /* BT_SSP_OOB */

#ifdef BT_ENHANCED_FLUSH
/**
 *  \brief To flush all pending ACL data from the Bluetooth device for the
 *         specified ACL Connection Handle, for the specified packet type
 *
 *  \par Description:
 *       This API can be used to discard all L2CAP packets identified by packet_
 *       type that are currently pending for transmission in the Controller for
 *       the specified Connection Handle, even if there currently are chunks of
 *       data that belong to more than one L2CAP packet of the same type in the
 *       Controller.
 *       The only packet type defined is automatically-flushable. Packets not
 *       identified by packet_type will not be flushed and will be processed
 *       normally by the Controller.
 *       After flushing the packets, all data that is sent to the Controller for
 *       the same Connection Handle and packet type will be discarded by the
 *       Controller until an HCI Data Packet with the start
 *       Packet_Boundary_Flag (0x00 or 0x02) is received.
 *       This API will allow higher-level application to control how long the
 *       baseband should try to retransmit a baseband packet of a specific type
 *       for a Connection Handle before all data of that type currently pending
 *       for transmission in the Controller should be flushed.
 *
 *
 *  \param [in] connection_handle
 *         Connection Handle of the ACL connection with a remote Bluetooth device,
 *         data for which needs to be flushed.
 *
 *  \param [in] packet_type
 *         0x00: Automatically flushable packets only.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 *
 *  \note This API is available for ACL packets only, and a valid ACL Connection
 *        Handle must be specified.
 *        The BT_hci_flush() API can be used to flush all packets, irrespective
 *        of packet type. In addition to the BT_hci_flush() and
 *        BT_hci_enhanced_flush() APIs, the applications can make use of
 *        Automatic Flush Timeout in the Bluetooth device to automatically
 *        flush an automatically-flushable L2CAP packet that is currently being
 *        transmitted after the specified flush timer has expired.
 */
API_RESULT BT_hci_enhanced_flush
           (
               /* IN */ UINT16    connection_handle,
               /* IN */ UCHAR     packet_type
           );
#endif /* BT_ENHANCED_FLUSH */

#ifdef BT_SSP_DEBUG
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_write_simple_pairing_debug_mode(p1) \
        hci_write_command_UCHAR \
        (HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE_OPCODE, (p1))
#endif /* BT_SSP_DEBUG */


/* ------------------------------------------------------------------------- */

/* New APIs from Bluetooth Spec 3.0 ---------------------------------------- */
#ifdef BT_3_0

#ifdef HCI_FLOW_CONTROL_COMMANDS
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_flow_control_mode() \
        hci_common_api_handler_no_params (HCI_READ_FLOW_CONTROL_MODE_OPCODE)
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_write_flow_control_mode
           (
                UCHAR  flow_control_mode
           );
#endif /* HCI_FLOW_CONTROL_COMMANDS */
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_read_encryption_key_size
           (
                UINT16   connection_handle
           );
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hci_read_enhanced_transmit_power_level
           (
                UINT16   handle,
                UCHAR    power_type
           );

#ifdef HCI_READ_DATA_BLOCK_SIZE_SUPPORT
/**
 *  \brief To read supported data block size from the controller.
 *
 *  \par Description:
 *       This command is used to read values regarding the maximum
 *       permitted data transfers over the Controller and the data
 *       buffering available in the Controller.
 *
 *  \return
 *      API_SUCCESS or one of the error codes as defined in BT_error.h.
 */
#define BT_hci_read_data_block_size() \
        hci_common_api_handler_no_params \
        (HCI_READ_DATA_BLOCK_SIZE_OPCODE)
#endif /* HCI_READ_DATA_BLOCK_SIZE_SUPPORT */
#endif /* BT_3_0 */

#ifdef BT_BRSC
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_read_secure_connections_host_support()  \
        hci_common_api_handler_no_params (HCI_READ_SECURE_CONNECTIONS_HOST_SUPPORT_OPCODE)
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define BT_hci_write_secure_connections_host_support(enable)  \
        hci_write_command_UCHAR \
        (HCI_WRITE_SECURE_CONNECTIONS_HOST_SUPPORT_OPCODE, (enable))
#endif /* BT_BRSC */

/* ------------------------------------------------------------------------- */

/* HCI Vendor Specific Commands APIs --------------------------------------- */
/**
 *  \brief To send a generic HCI command to the local Bluetooth device.
 *
 *  \par Description:
 *       This API can be used to send generic HCI commands to the local
 *       Bluetooth device. The HCI Group, Command Opcode and parameters are
 *       specific to the command. Refer to the documentation of
 *       the Bluetooth device to find out the details of available commands.
 *
 *  \param [in] ogf
 *         Opcode Group Field for the Opcode.
 *
 *  \param [in] ocf
 *         Opcode Command Field for the Opcode.
 *
 *  \param [in] params
 *         Parameters to be framed for the HCI command.
 *
 *  \param [in] params_length
 *         Total length of the parameters passed.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
API_RESULT BT_hci_send_command
           (
               UCHAR    ogf,
               UINT16   ocf,
               UCHAR *  params,
               UCHAR    params_length
           );

/**
 *  \brief To send a vendor specific HCI command to the local Bluetooth device.
 *
 *  \par Description:
 *       This API can be used to send vendor specific HCI commands to the local
 *       Bluetooth device. The HCI Opcode and parameters are specific to
 *       the vendor specific command. Refer to the documentation of
 *       the Bluetooth device to find out the details of available vendor
 *       specific commands.
 *
 *  \param [in] ocf
 *         Opcode Command Field for the vendor specific Opcode.
 *
 *  \param [in] params
 *         Parameters to be framed for the vendor specific HCI command.
 *
 *  \param [in] params_length
 *         Total length of the parameters passed.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  BT_error.h or Host Controller Error Codes section.
 */
#define BT_hci_vendor_specific_command(ocf, params, params_length) \
        BT_hci_send_command(0x3FU, (ocf), (params), (params_length))

/* ------------------------------------------------------------------------- */


/* APIs for EtherMind Security Manager ------------------------------------- */
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT hci_sm_link_key_request_reply
           (UCHAR *bd_addr, UCHAR *link_key, UCHAR link_key_present);
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT hci_sm_pin_code_request_reply
           (UCHAR *bd_addr, UCHAR *pin_code, UCHAR pin_length);
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT hci_sm_set_pin_type_master_link_key (UINT16 opcode, UCHAR flag);
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define hci_sm_set_pin_type(pt) \
        hci_sm_set_pin_type_master_link_key (HCI_WRITE_PIN_TYPE_OPCODE, (pt))
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
#define hci_sm_master_link_key(kf) \
        hci_sm_set_pin_type_master_link_key (HCI_MASTER_LINK_KEY_OPCODE, (kf))

#ifdef BT_SSP_UC
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT hci_sm_user_confirmation_request_reply
           (
               UCHAR *    bd_addr,
               UCHAR      accept
           );
#endif /* BT_SSP_UC */

#ifdef BT_SSP_PE
/**
 *  \brief
 *  \par Description:
 *
 *
 *
 *
 *  \return
 *      API_RESULT: API_SUCCESS or one of the error codes as defined in
 *                  \ref BLE_ERROR_CODES.
 */
API_RESULT hci_sm_user_passkey_request_reply
           (
               UCHAR *    bd_addr,
               UCHAR *    passkey
           );
#endif /* BT_SSP_PE */

/* ------------------------------------------------------------------------- */


#ifdef __cplusplus
};
#endif

/** \} */
/** \} */
/** \} */
#endif /* _H_BT_HCI_API_ */

