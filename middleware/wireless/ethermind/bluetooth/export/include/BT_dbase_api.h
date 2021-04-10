
/**
 *  \file BT_dbase_api.h
 *
 *  This module contains the declarations of the structs of each
 *  profile that needed to register its service along with the
 *  exported API functions.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_DBASE_API_
#define _H_BT_DBASE_API_

#include "BT_common.h"

/* ----------------------Global Definitions section ---*/
/**
 * \addtogroup bt_protocol Protocols
 * \{
 */
/**
 * \defgroup  dbase_module  DBASE (SDP Database)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  dbase module to the Application and other upper layers of the stack.
 *
 */
/**
 * \defgroup  dbase_defines Defines
 * \{
 * Describes defines for dbase module.
 */
/**
 * \defgroup dbase_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
#define DB_RECORD_ACTIVE           1
#define DB_RECORD_INACTIVE         0

/* SDP Server Record Handle */
#define SDP_SERVER_RECORD_HANDLE                0x00000000

/* Start of other valid SDP Record Handles */
#define SDP_START_OF_VALID_RECORD_HANDLE        0x00010000

/* PnP Info Attribute IDs */
#define PNP_INFO_SPECIFICATION_ID_ATTR_ID       0x0200
#define PNP_INFO_VENDOR_ID_ATTR_ID              0x0201
#define PNP_INFO_PRODUCT_ID_ATTR_ID             0x0202
#define PNP_INFO_VERSION_ATTRIBUTE_ATTR_ID      0x0203
#define PNP_INFO_PRIMARY_RECORD_ATTR_ID         0x0204
#define PNP_INFO_VENDOR_ID_SOURCE_ATTR_ID       0x0205


#define PNP_INFO_BASE_SERVICE_ID                (0x0620 | BASE_SERVICE_ID)

/* Attribute Identifier Codes Definitions */
#define SERVICE_RECORD_HANDLE                   0x0000
#define SERVICE_CLASS_ID_LIST                   0x0001
#define SERVICE_RECORD_STATE                    0x0002
#define SERVICE_ID                              0x0003
#define PROTOCOL_DESC_LIST                      0x0004
#define BROWSE_GROUP_LIST                       0x0005
#define LANG_BASE_ATTR_ID_LIST                  0x0006
#define SERVICE_INFO_TIME_TO_LIVE               0x0007
#define SERVICE_AVAILABILITY                    0x0008
#define BLUETOOTH_PROFILE_DESC_LIST             0x0009
#define DOCUMENTATION_URL                       0x000A
#define CLIENT_EXECUTABLE_URL                   0x000B
#define ICON_URL                                0x000C
#define ADDITIONAL_PROT_DESC_LIST_ID            0x000D

#define VERSION_NO_LIST                         0x0200
#define SERVICE_DATABASE_STATE                  0x0201
#define GROUP_ID                                0x0200
#define SUPPORTED_FEATURES                      0x0200

#define REMOTE_AUDIO_VOL_CTRL                   0x0302
#define EXTERNAL_NETWORK                        0x0301
#define SERVICE_VERSION                         0x0300
#define SUPPORTED_DATA_STORES_LIST              0x0301
#define SUPPORTED_FORMATS_LIST                  0x0303
#define FAX_1_CLASS_SUPPORT                     0x0302
#define FAX_2_0_CLASS_SUPPORT                   0x0303
#define FAX_2_CLASS_SUPPORT                     0x0304
#define AUDIO_FEEDBACK_SUPPORT                  0x0305

#define SDP_ATTR_ID_SUPPORTED_CAPABILITIES      0x0310
#define SDP_ATTR_ID_SUPPORTED_FUNCTIONS         0x0312
#define SDP_ATTR_ID_TOTAL_IMG_DATA_CAPACITY     0x0313
#define SUPPORTED_REPOSITORIES                  0x0314

#define MAS_INSTANCE_ID                         0x0315
#define SUPPORTED_MESSAGE_TYPES                 0x0316

/* MPS Attribute IDs */
#define MPS_MPSD_SCENARIOS_ATTR_ID              0x0200
#define MPS_MPMD_SCENARIOS_ATTR_ID              0x0201
#define MPS_SUPP_PROF_PROT_DEP_ATTR_ID          0x0202

#define GOEP_L2CAP_PSM_ATTR_ID                  0x0200

/*
 * Below attributes form offsets from the base attr id that is specied
 * under the values of Lang_base_attr_id_list (attrid: 0x0006).
 */
#define LANG_BASE_ATTR_ID                        0x0100
#define SERVICE_NAME_ATTR_ID                    ( 0x0100 + 0 )
#define SERVICE_DESC_ATTR_ID                    ( 0x0100 + 1 )
#define PROVIDER_NAME_ATTR_ID                   ( 0x0100 + 2 )

/* BPP Attribute Ids */
#define PRINTER_STATUS_CHANNEL                  0x0305
#define RUI_JOB_CHANNEL                         0x0374
/**
 * @name BPP Attribute Ids
 */
/*@{*/
#define BPP_DOCUMENT_FORMATS_SUPPORTED_ATTR_ID              0x0350
#define BPP_CHARACTER_REPERTOIRES_SUPPORTED_ATTR_ID         0x0352
#define BPP_XHTML_PRINT_IMG_FORMATS_SUPPORTED_ATTR_ID       0x0354
#define BPP_COLOR_SUPPORTED_ATTR_ID                         0x0356
#define BPP_1284ID_ATTR_ID                                  0x0358
#define BPP_PRINTER_NAME_ATTR_ID                            0x035A
#define BPP_PRINTER_LOCATION_ATTR_ID                        0x035C
#define BPP_DUPLEX_SUPPORTED_ATTR_ID                        0x035E
#define BPP_MEDIA_TYPES_SUPPORTED_ATTR_ID                   0x0360
#define BPP_MAX_MEDIA_WIDTH_ATTR_ID                         0x0362
#define BPP_MAX_MEDIA_LENGTH_ATTR_ID                        0x0364
#define BPP_ENHANCED_LAYOUT_SUPPORTED_ATTR_ID               0x0366
#define BPP_RUI_FORMATS_SUPPORTED_ATTR_ID                   0x0368
#define BPP_REF_PRINTING_RUI_SUPPORTED_ATTR_ID              0x0370
#define BPP_DIRECT_PRINTING_RUI_SUPPORTED_ATTR_ID           0x0372
#define BPP_REF_PRINTING_TOP_URL_ATTR_ID                    0x0374
#define BPP_DIRECT_PRINTING_TOP_URL_ATTR_ID                 0x0376
#define BPP_PRINTER_ADMIN_RUI_TOP_URL_ATTR_ID               0x0378
/*@}*/


/* HCRP Profile Attribute IDs */
/**
 * @name HCRP Profile Attribute IDs
 */
/*@{*/
#define HCRP_1284ID_ATTR_ID                                 0x0300
#define HCRP_DEVICE_NAME_ATTR_ID                            0x0302
#define HCRP_FRIENDLY_NAME_ATTR_ID                          0x0304
/*@}*/

/* HID Profile Attribute IDs */
/**
 * @name HID Profile Attribute IDs
 */
/*@{*/
#define HID_DEVICE_RELEASE_NUMBER_ATTR_ID                   0x0200
#define HID_PARSER_VERSION_ATTR_ID                          0x0201
#define HID_DEVICE_SUBCLASS_ATTR_ID                         0x0202
#define HID_COUNTRY_CODE_ATTR_ID                            0x0203
#define HID_VIRTUAL_CABLE_ATTR_ID                           0x0204
#define HID_RECONNECT_INITIATE_ATTR_ID                      0x0205
#define HID_DESCRIPTOR_LIST_ATTR_ID                         0x0206
#define HID_LANGID_BASE_LIST_ATTR_ID                        0x0207
#define HID_SDP_DISABLE_ATTR_ID                             0x0208
#define HID_BATTERY_POWER_ATTR_ID                           0x0209
#define HID_REMOTE_WAKE_ATTR_ID                             0x020A
#define HID_PROFILE_VERSION_ATTR_ID                         0x020B
#define HID_SUPERVISION_TIMEOUT_ATTR_ID                     0x020C
#define HID_NORMALLY_CONNECTABLE_ATTR_ID                    0x020D
#define HID_BOOT_DEVICE_ATTR_ID                             0x020E
#define HID_SSR_HOST_MAX_LATENCY_ATTR_ID                    0x020F
#define HID_SSR_HOST_MIN_TIMEOUT_ATTR_ID                    0x0210
/*@}*/

/* PAN Profile Attribute Ids */
/**
 * @name PAN Profile Attribute Ids
 */
/*@{*/
#define IP_SUBNET_ID                                        0x0200
#define SECURITY_DESC_ID                                    0x030A
#define NET_ACCESS_TYPE_ID                                  0x030B
#define MAX_NET_ACCESS_RATE                                 0x030C
/*@}*/

/* HFP &Imaging Attribute IDs */
/**
 * @name HFP &Imaging Attribute IDs
 */
/*@{*/
#define SUPPORTED_FEATURES_ATTR_ID                          0x0311
/*@}*/
/* HFP Attrib IDs*/
/**
 * @name HFP &Imaging Attribute IDs
 */
/*@{*/
#define HFP_NETWORK_ATTR_ID                                 0x0301
/*@}*/
/* Few more constants evolve during Lang_base_attr_id_list */
/**
 * @name Lang base Attribute IDs
 */
/*@{*/
#define LANG_ISO639_ID                                      0x656E
#define LANG_CHAR_ENC_IANA_ID                               0x006A
/*@}*/
/* PBAP Attribute IDs */
/**
 * @name PBAP Attribute IDs
 */
/*@{*/
#define PBAP_SUPPORTED_REPOSITORIES                         0x0314
#define PBAP_SUPPORTED_FEATURES_ATTR_ID                     0x0317
/*@}*/
/* MAP Attribute IDs */
/**
 * @name MAP Attribute IDs
 */
/*@{*/
#define MAP_MAS_INSTANCE_ID_ATTR_ID                         0x0315
#define MAP_SUPPORTED_MSG_TYPES_ATTR_ID                     0x0316
#define MAP_SUPPORTED_FEATURES_ATTR_ID                      0x0317
/*@}*/
/* CTN Attribute IDs */
/**
 * @name CTN Attribute IDs
 */
/*@{*/
#define CTN_CAS_INSTANCE_ID_ATTR_ID                         0x0315
#define CTN_SUPPORTED_FEATURES_ATTR_ID                      0x0317
/*@}*/
/* MPS Attribute IDs */
/**
 * @name CTN Attribute IDs
 */
/*@{*/
#define MPS_MPSD_SCENARIOS_ATTR_ID                          0x0200
#define MPS_MPMD_SCENARIOS_ATTR_ID                          0x0201
#define MPS_SUPP_PROF_PROT_DEP_ATTR_ID                      0x0202
/*@}*/
/* 32 bit Protocol UUID's definitions */
/**
 * @name 32 bit Protocol UUID's definitions
 */
/*@{*/
#define SDP_UUID                                0x00000001
#define RFCOMM_UUID                             0x00000003
#define TCSBIN_UUID                             0x00000005
#define ATT_PROTO_UUID                          0x00000007
#define L2CAP_UUID                              0x00000100
#define IP_UUID                                 0x00000009
#define UDP_UUID                                0x00000002
#define TCP_UUID                                0x00000004
#define TCSAT_UUID                              0x00000006
#define OBEX_UUID                               0x00000008
#define FTP_UUID                                0x0000000A
#define HTTP_UUID                               0x0000000C
#define WSP_UUID                                0x0000000E
/*@}*/
#define ROOT_ID                                 0x00001002

/* Service Class UUID's definitions */
/**
 * @name Service Class UUID's definitions
 */
/*@{*/
#define SDPSERVER_SERVICECLASS_UUID             0x00001000
#define BROWSEGROUPDESC_SERVICECLASS_UUID       0x00001001
#define PUBLICBROWSEGROUP_UUID                  0x00001002
#define SERIALPORT_UUID                         0x00001101
#define LANACCESS_USINGPPP_UUID                 0x00001102
#define DIALUP_NETWORKING_UUID                  0x00001103
#define IRMCSYNC_UUID                           0x00001104
#define OBEXOBJECT_PUSH_UUID                    0x00001105
#define OBEXFILETRANSFER_UUID                   0x00001106
#define IRMCSYNC_COMMAND_UUID                   0x00001107
#define HEADSET_UUID                            0x00001108
#define CORDLESS_TELEPHONY_UUID                 0x00001109
#define INTERCOM_UUID                           0x00001110
#define FAX_UUID                                0x00001111
#define HEADSET_AUDIOGATEWAY_UUID               0x00001112
#define PNP_INFO_UUID                           0x00001200
#define GENERIC_NETWORKING_UUID                 0x00001201
#define GENERIC_FILETRANSFER_UUID               0x00001202
#define GENERIC_AUDIO_UUID                      0x00001203
#define GENERIC_TELEPHONY_UUID                  0x00001204
#define HEADSET_HS_UUID                         0x00001131
/*@}*/
/* PAN UUIDs */
/**
 * @name PAN UUIDs
 */
/*@{*/
#define BNEP_UUID                               0x0000000F
#define PANU_UUID                               0x00001115
#define NETWORK_ACCESS_POINT_UUID               0x00001116
#define GROUP_NETWORK_UUID                      0x00001117
/*@}*/
/* BPP UUIDs */
/**
 * @name BPP UUIDs
 */
/*@{*/
#define BPP_UUID                                0x00001122
#define BPP_DIRECT_PRINTING_UUID                0x00001118
#define BPP_REFERENCE_PRINTING_UUID             0x00001119
#define BPP_DIRECT_PRINT_REF_OBJ_SERVICE_UUID   0x00001120
#define BPP_REFLECTED_UI_UUID                   0x00001121
#define BPP_PRINTING_STATUS_UUID                0x00001123
/*@}*/
/* Imaging UUIDs */
/**
 * @name Imaging UUIDs
 */
/*@{*/
#define IMAGING_UUID                            0x0000111A
#define IMAGING_RESPONDER_UUID                  0x0000111B
#define IMAGING_AUTOMATIC_ARCHIVE_UUID          0x0000111C
#define IMAGING_REFERENCED_OBJECTS_UUID         0x0000111D
/*@}*/
/* HID Profile UUIDs */
/**
 * @name HID Profile UUIDs
 */
/*@{*/
#define HIDP_UUID                               0x00000011
#define HID_SERVICE_UUID                        0x00001124
/*@}*/
/* HCRP UUIDs */
/**
 * @name HCRP UUIDs
 */
/*@{*/
#define HCRP_CONTROL_CH_UUID                    0x00000012
#define HCRP_DATA_CH_UUID                       0x00000014
#define HCRP_NOTIFY_CH_UUID                     0x00000016
#define HCRP_UUID                               0x00001125
#define HCRP_PRINT_UUID                         0x00001126
#define HCRP_SCAN_UUID                          0x00001127
/*@}*/
/* HFP HFU/AG UUIDs */
/**
 * @name  HFP HFU/AG UUIDs
 */
/*@{*/
#define HFP_HFU_HANDS_FREE_UUID                 0x0000111E
#define HFP_AG_AUDIO_GATEWAY_UUID               0x0000111F
#define HFP_GENERIC_AUDIO                       0x00001203
/*@}*/
/* AV Protocol UUIDs */
/**
 * @name  HFP HFU/AG UUIDs
 */
/*@{*/
#define AVCTP_UUID                              0x00000017
#define AVDTP_UUID                              0x00000019
/*@}*/
/* A2DP UUIDs */
/**
 * @name  HFP HFU/AG UUIDs
 */
/*@{*/
#define A2DP_UUID                               0x0000110D
#define A2DP_SOURCE_UUID                        0x0000110A
#define A2DP_SINK_UUID                          0x0000110B
/*@}*/
/* AVRCP UUIDs */
/**
 * @name  AVRCP UUIDs
 */
/*@{*/
#define AVRCP_TARGET_UUID                       0x0000110C
#define AVRCP_UUID                              0x0000110E
#define AVRCP_CONTROLLER_UUID                   0x0000110F
/*@}*/
/* UDIP UUIDs */
/**
 * @name  UDIP UUIDs
 */
/*@{*/
#define UDI_UE_UUID                             0x0000112A
#define UDI_TE_UUID                             0x0000112B
/*@}*/
/* PHONEBOOK UUIDs */
/**
 * @name  PHONEBOOK UUIDs
 */
/*@{*/
#define PHONEBOOK_PCE_UUID                      0x0000112E
#define PHONEBOOK_PSE_UUID                      0x0000112F
#define PHONEBOOK_UUID                          0x00001130
/*@}*/
/* SIM Access Profile UUID */
/**
 * @name  SIM Access Profile UUIDs
 */
/*@{*/
#define SIM_ACCESS_UUID                         0x0000112D
/*@}*/
/* MCAP UUIDs */
/**
 * @name  MCAP UUIDs
 */
/*@{*/
#define MCAP_CONTROL_CHANNEL_UUID               0x0000001E
#define MCAP_DATA_CHANNEL_UUID                  0x0000001F
/*@}*/
/* HDP UUIDs */
/**
 * @name  HDP UUIDs
 */
/*@{*/
#define HDP_UUID                                0x00001400
#define HDP_SOURCE_UUID                         0x00001401
#define HDP_SINK_UUID                           0x00001402
/*@}*/
/* MAP UUID */
/**
 * @name  MAP UUID
 */
/*@{*/
#define MAP_MAS_UUID                            0x00001132
#define MAP_MNS_UUID                            0x00001133
#define MAP_UUID                                0x00001134
/*@}*/
/* GNSS UUID */
/**
 * @name   GNSS UUID
 */
/*@{*/
#define GNSS_UUID                               0x00001135
#define GNSS_SERVER_UUID                        0x00001136
/*@}*/
/* CTN UUID */
/**
 * @name   CTN UUID
 */
/*@{*/
#define CTN_CAS_UUID                            0x0000113C
#define CTN_CNS_UUID                            0x0000113D
#define CTN_UUID                                0x0000113E
/*@}*/

/* New Defines for SDP headers */
/**
 * @name   New Defines for SDP headers
 */
/*@{*/
#define DB_2BYTE_UUID_HEADER                    0x19
#define DB_4BYTE_UUID_HEADER                    0x1A
#define DB_16BYTE_UUID_HEADER                   0x1C
#define DB_DATA_ELEM_ALT_HEADER                 0x36
#define DB_DATA_ELEM_SEQ_HEADER                 0x35
#define DB_4BYTE_INT_HEADER                     0x0A
#define DB_2BYTE_INT_HEADER                     0x09
#define DB_1BYTE_INT_HEADER                     0x08
#define DB_URL_STR_HEADER                       0x45
#define DB_TEXT_STR_HEADER                      0x25
#define DB_BOOLEAN_HEADER                       0x28
/*@}*/
/*
 * All Service IDs for the profiles. Profiles can use a range of xxx0 to
 * xxxF as the service IDs where xxx is the BASE Service ID provided for
 * that particular profile
 */
/**
 * @name   All Service IDs for the profiles.
 */
/*@{*/
#define BASE_SERVICE_ID               0xA000

#define GAP_BASE_SERVICE_ID          (0x0500 | BASE_SERVICE_ID)
#define SDAP_BASE_SERVICE_ID         (0x0510 | BASE_SERVICE_ID)
#define CTP_BASE_SERVICE_ID          (0x0520 | BASE_SERVICE_ID)
#define ICP_BASE_SERVICE_ID          (0x0530 | BASE_SERVICE_ID)
#define SPP_BASE_SERVICE_ID          (0x0540 | BASE_SERVICE_ID)
#define HEADSET_BASE_SERVICE_ID      (0x0550 | BASE_SERVICE_ID)
#define DUNP_BASE_SERVICE_ID         (0x0560 | BASE_SERVICE_ID)
#define FAX_BASE_SERVICE_ID          (0x0570 | BASE_SERVICE_ID)
#define LAP_BASE_SERVICE_ID          (0x0580 | BASE_SERVICE_ID)
#define GOBEX_BASE_SERVICE_ID        (0x0590 | BASE_SERVICE_ID)
#define OPP_BASE_SERVICE_ID          (0x05A0 | BASE_SERVICE_ID)
#define FTP_BASE_SERVICE_ID          (0x05B0 | BASE_SERVICE_ID)
#define SYNC_BASE_SERVICE_ID         (0x05C0 | BASE_SERVICE_ID)
#define HCRP_BASE_SERVICE_ID         (0x05D0 | BASE_SERVICE_ID)
#define HID_BASE_SERVICE_ID          (0x05E0 | BASE_SERVICE_ID)
#define HFP_BASE_SERVICE_ID          (0x05F0 | BASE_SERVICE_ID)
#define PAN_BASE_SERVICE_ID          (0x0600 | BASE_SERVICE_ID)
#define A2DP_BASE_SERVICE_ID         (0x0610 | BASE_SERVICE_ID)

#define MCM_BASE_SERVICE_ID          (0x0800 | BASE_SERVICE_ID)
#define BPP_BASE_SERVICE_ID          (0x0900 | BASE_SERVICE_ID)
/*@}*/
/* ----------- Global structures */
/* --------------------- Enumerators Declarations section ---*/
enum attr_type
{
    DB_NIL = 0,
    DB_UNSIGNED_INT = 1,
    DB_SIGNED_2COM_INT = 2,
    DB_UUID,
    DB_TEXT_STR,
    DB_BOOLEAN,
    DB_DATA_ELEM_SEQ,
    DB_DATA_ELEM_ALT,
    DB_URL
};

#ifdef SDP_HAVE_LIB_INDEPENDENT_DBASE_SUPPORT
/**
 * @name   Constant Definitions of DB Record
 */
/*@{*/
#define DB_RECORD_A2DP_SINK                          0x80
#define DB_RECORD_A2DP_SOURCE                        0x81
#define DB_RECORD_AVRCP_TARGET                       0x82
#define DB_RECORD_AVRCP_CONTROLLER                   0x83
#define DB_RECORD_RO                                 0x84
#define DB_RECORD_ARCHIVED_OBJECTS                   0x85
#define DB_RECORD_BIP                                0x86
#define DB_RECORD_BPP                                0x87
#define DB_RECORD_BASIC_PRINTING_REFERENCED_OBJECTS  0x88
#define DB_RECORD_FTP                                0x89
#define DB_RECORD_MAP_MCE                            0x8A
#define DB_RECORD_MAP_MSE                            0x8B
#define DB_RECORD_OPP                                0x8C
#define DB_RECORD_PBAP_PCE                           0x8D
#define DB_RECORD_PBAP_PSE                           0x8E
#define DB_RECORD_SAP                                0x8F
#define DB_RECORD_UDI_UE                             0x90
#define DB_RECORD_UDI_TE                             0x91
#define DB_RECORD_CTN_CCE                            0x92
#define DB_RECORD_CTN_CSE                            0x93
/*@}*/
#endif /* SDP_HAVE_LIB_INDEPENDENT_DBASE_SUPPORT */
/** \} */
/* ----------------------Structure Declarations section ---*/
/**
 * \defgroup dbase_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
struct attr
{
    UINT16 attr_id;
    UINT16 len;
    UCHAR *value;
};


typedef struct sdp_record
{
    UINT32 rec_handle;
    UINT16 active;
    UINT16 num_uuids;
    UINT16 num_attrs;
    UINT16 attr_offset;
}SDP_RECORD;
/** \} */
/** \} */
/*---------------------------------Function Declarations */
#ifdef __cplusplus
extern "C" {
#endif
/**
 * \defgroup dbase_api API Definitions
 * \{
 * Describes API definitions of the module.
 */
/**
 *  \brief Ethermind SDP dbase initializer
 *
 *  \par Description:
 *  This function initializes SDP dbase records statically.
 *  It is called as a part of EtherMind stack initialization.
 *
 *  \param None
 *
 *  \return None
 *
 */
void BT_dbase_init
     (
         void
     );
/**
 *
 *  \brief To get SDDB Record Handles for an UUID search pattern
 *
 *  \par Description:
 *    This function provides an Interface to get the SDDB Record Handles
 *    that match a list of UUIDs which are passed as input parameters.
 *
 *  \param [in] uuids_array
 *         Specifies the array of 128-bit UUIDs as the search pattern
 *         based on which the record handles are to be matched.
 *  \param [in] num_uuids
 *         Number of UUIDs in the search pattern.
 *  \param [in,out] num_handles
 *         As IN parameter it indicates the Maximum Number of Record Handles
 *         that can be returned (by filling in the buffer provided by the
 *         caller for this purpose).
 *         As OUT parameter it indicates the Actual Number of Records which
 *         has matched the given search pattern.
 *  \param [out] handle_array
 *         Array of Record Handles for which match is found.
 *         Memory for this shall be allocated allocated by caller.
 *         If handle_array is NULL, the caller wants to know how many
 *         record handle matches. Caller will use the matching record handle
 *         information to allocate appropriate memory, in sub-sequent call
 *         to this API.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_get_rec_handles
           (
               /* IN */    UCHAR  * uuids_array,
               /* IN */    UCHAR    num_uuids,
               /* INOUT */ UCHAR  * num_handles,
               /* OUT */   UINT32 * handle_array
           );

/**
 *
 *  \brief To get an Attribute Value from an SDDB Record
 *
 *  \par Description:
 *      This function provides an Interface to get the value of
 *      an attribute that is referred by attribute Id,
 *      and also conforms to the record handle
 *
 *  \param [in] record_handle
 *         Record Handle in which attribute value has to be found.
 *  \param [in,out] attr_id
 *         Attribute Id that identifies specific attribute.
 *         The return value contains the next available attribute ID
 *  \param [out] attr_value
 *         Buffer provided by the caller, to be filled in this function.
 *  \param [in] attr_buf_len
 *         Length of Attribute Value Buffer.
 *  \param [out] attr_val_len
 *         Actual length of the attribute value corresponding
 *         Attribute ID.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_get_attr_value
           (
               /* IN */    UINT32   record_handle,
               /* INOUT */ UINT16 * attr_id,
               /* OUT */   UCHAR  * attr_value,
               /* IN */    UINT16   attr_buf_len,
               /* OUT */   UINT32 * attr_val_len
           );

/**
 *  \brief To update the value of an attribute in an SDP Database Record.
 *
 *  \par Description:
 *       This API updates the attribute value of a SDP Service Record, as
 *       identified by the given record handle. The new attribute value
 *       overwrites the existing memory that holds the current attribute
 *       value for the given attribute identifier in the specified SDP Database Record.
 *       This API can only be used in cases where some fixed octets of an
 *       existing attribute value needs to be changed and the size (in octets)
 *       of the new attribute value is not more than the existing
 *       attribute value size.
 *
 *  \param [in] record_handle
 *         Identifies the SDP Database Service Record.
 *
 *  \param [in] attr_id
 *         The Attribute ID whose value to be changed.
 *
 *  \param [in] attr_value
 *         Caller allocated UCHAR buffer containing the octets of the new value
 *         for the specified attribute.
 *
 *  \param [in] attr_value_length
 *         Size, in octets, of the buffer containing the new value.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 */
API_RESULT BT_dbase_update_attr_value
           (
               /* IN */ UINT32 record_handle,
               /* IN */ UINT16 attr_id,
               /* IN */ UCHAR *attr_value,
               /* IN */ UINT16 attr_value_length
           );
/**
 *  \brief To cahnge the value of an attribute in an SDP Database Record.
 *
 *  \par Description:
 *       This API changes the attribute value of a SDP Service Record,
 *       as identified by the given record handle.
 *       This API can only be used in cases where existing attribute value
 *       needs to be changed and the size (in octets) of the new attribute
 *       value is more than the existing attribute value size.
 *
 *  \param [in] record_handle
 *         Identifies the SDP Database Service Record.
 *
 *  \param [in] attr_id
 *         The Attribute ID whose value to be changed.
 *
 *  \param [in] attr_value
 *         Caller allocated UCHAR buffer containing the octets of the new value
 *         for the specified attribute. This parameter must be resident,
 *         and should not be freed by the caller, as it will be used by the
 *         SDP Database to provide the attribute value.
 *
 *  \param [in] attr_value_length
 *         Size, in octets, of the buffer containing the new value.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_dbase_change_attr_value
           (
               /* IN */ UINT32 record_handle,
               /* IN */ UINT16 attr_id,
               /* IN */ UCHAR *attr_value,
               /* IN */ UINT16 attr_value_length
           );

/**
 *  \brief To activate an SDP Database Service Record.
 *
 *  \par Description:
 *       This API activates a given SDP Service Record in the SDP Database.
 *       Once activated, the Service is now discoverable by other remote
 *       Bluetooth devices (SDP client devices).
 *
 *  \param [in] record_handle
 *         Identifies the Service Record in SDP Database.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_dbase_activate_record
           (
               /* IN */  UINT32    record_handle
           );

/**
 *  \brief To inactivate given SDP record in SDP database
 *
 *  \par Description:
 *       This API inactivates a given SDP Service Record in the SDP Database.
 *       Once deactivated, the Service will no longer be discoverable by other
 *       remote Bluetooth devices (SDP client devices).
 *
 *  \param [in] record_handle
 *         Identifies the Service Record in SDP Database.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_dbase_inactivate_record
           (
               /* IN */  UINT32    record_handle
           );

/**
 *  \brief To get the SDDB Record Handle, if present.
 *
 *  \par Description:
 *       This API returns the SDP Record Handle from the SDP Databse that is
 *       associated with a particular service type and its instance.
 *
 *  \param [in] service_type
 *         Identifies the service type (for example, for Serial Port Profile
 *         "DB_RECORD_SPP"). All the service types are defined in the "db_gen.h".
 *
 *  \param [in] service_instance
 *         Identifies the instance of  a particular service type (for example,
 *         if Serial Port Profile has two instances, then to get the Record
 *         Handle of the first instance it should be 0 and for the next
 *         instance it should be 1).
 *
 *  \param [out] rec_hdl
 *         Pointer to a caller allocated UINT32 variable where the associated
 *         SDP Record Handle will be copied on return.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 *
 */
API_RESULT BT_dbase_get_record_handle
           (
               /* IN */  UCHAR service_type,
               /* IN */  UCHAR service_instance,
               /* OUT */ UINT32 *rec_hdl
           );
/**
 *  \brief To get a RFCOMM Server Channel present in an SDP Database Record.
 *
 *  \par Description:
 *       This function returns the RFCOMM Server Channel corresponding to a SDP
 *       Service Record and an Attribute ID. This API is useful only in the case
 *       of profiles/application running over RFCOMM.
 *
 *  \param [in] record_handle
 *         Identifies the SDP Database Service Record.
 *
 *  \param [in] attr_id
 *         Attribute ID that the RFCOMM Server Channel is associated with.
 *         For most of the cases, as per the Bluetooth specification Profiles over RFCOMM,
 *         the Attribute ID needs to be the "Protocol Descriptor List".
 *         Some profile (such as, the Basic Printing Profile), more than one RFCOMM Server Channel
 *         may be used. This additional RFCOMM Server Channels, if any, are usually associated
 *         with the "Additional Protocol Descriptor List" Attribute ID.
 *
 *  \param [out] server_channel
 *         Pointer to a caller allocated UCHAR variable, that is filled with the
 *         RFCOMM Server Channel value on return.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 */
API_RESULT BT_dbase_get_server_channel
           (
               /* IN */  UINT32 record_handle,
               /* IN */  UINT16 attr_id,
               /* OUT */ UCHAR *server_channel
           );

/**
 *  \brief To get OBEX PSM present in an SDDB Record
 *
 *  \par Description:
 *    This function returns the PSM corresponding
 *    to a SDP Service Record and an Attribute ID.
 *    This API is useful only in the case of those profiles/application
 *    which are running OBEX over L2CAP.
 *
 *  \param [in] record_handle
 *         Identifies the service record in DataBase.
 *
 *  \param [in] attr_id
 *         Attribute ID the PSM is associated with.
 *         Currently Attribute ID will be either ProtocolDescriptorList
 *         or GoepL2CapPsm
 *
 *  \param [out] psm
 *         This is filled with the assocatiated L2CAP PSM (if any).
 *         The memory shall be provided by the caller.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 */
API_RESULT BT_dbase_get_obex_psm
           (
               /* IN */  UINT32   record_handle,
               /* IN */  UINT16   attr_id,
               /* OUT */ UINT16 * psm
           );

/**
 *  \brief To extract the number of Service Class UUIDs present in the SDDB
 *
 *  \par Description:
 *       This function returns the number of Service Class UUIDs
 *       present in all the SDP Service Records (active and inactive).
 *
 *  \param [out] num_serv_class_uuids
 *         Caller provided stoarge to fill the number of Service Class UUIDs
 *         present in the SDP Service Record.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *          - DB_NULL_PARAM_NOT_ALLOWED
 *              If the parameters are not valid.
 *          - API_SUCCESS
 *              Otherwise
 *
 *  \note
 */
API_RESULT BT_dbase_get_num_service_class_uuids
           (
               /* OUT */ UINT32 *    num_serv_class_uuids
           );

/**
 *  \brief To extract Service Class UUIDs present in the SDDB
 *
 *  \par Description:
 *       This function returns the Service Class UUIDs present in
 *       all the SDP Service Records (active and inactive).
 *
 *  \param [in] uuid_buffer
 *         Caller provided buffer, where the UUIDs to be filled.
 *  \param [in, out] uuid_buffer_length
 *         IN:  Caller indicates the legth of UUID buffer (in Octet).
 *         OUT: This function modifies the value with the number of octets
 *              filled by the UUIDs before returning.
 *         Note: The length provided by the caller should be always even.
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *          - API_SUCCESS
 *              If all the available Service Class UUIDs are filled
 *              in the provided buffer.
 *          - DB_NULL_PARAM_NOT_ALLOWED
 *              If the parameters are not valid.
 *          - DB_MORE_SERVICE_CLASS_UUIDS_AVAILABLE
 *              If the uuid buffer does not have suffitient space
 *              for all the available uuids.
 */
API_RESULT BT_dbase_get_service_class_uuids
           (
               /* OUT */   UCHAR *     uuid_buffer,
               /* INOUT */ UINT32 *    uuid_buffer_length
           );

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_DBASE_API_ */

