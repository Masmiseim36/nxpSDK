
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



/* HCRP Profile Attribute IDs */
#define HCRP_1284ID_ATTR_ID                                 0x0300
#define HCRP_DEVICE_NAME_ATTR_ID                            0x0302
#define HCRP_FRIENDLY_NAME_ATTR_ID                          0x0304


/* HID Profile Attribute IDs */
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


/* PAN Profile Attribute Ids */
#define IP_SUBNET_ID                                        0x0200
#define SECURITY_DESC_ID                                    0x030A
#define NET_ACCESS_TYPE_ID                                  0x030B
#define MAX_NET_ACCESS_RATE                                 0x030C


/* HFP &Imaging Attribute IDs */
#define SUPPORTED_FEATURES_ATTR_ID                          0x0311

/* HFP Attrib IDs*/
#define HFP_NETWORK_ATTR_ID                                 0x0301

/* Few more constants evolve during Lang_base_attr_id_list */
#define LANG_ISO639_ID                                      0x656E
#define LANG_CHAR_ENC_IANA_ID                               0x006A

/* PBAP Attribute IDs */
#define PBAP_SUPPORTED_REPOSITORIES                         0x0314
#define PBAP_SUPPORTED_FEATURES_ATTR_ID                     0x0317

/* MAP Attribute IDs */
#define MAP_MAS_INSTANCE_ID_ATTR_ID                         0x0315
#define MAP_SUPPORTED_MSG_TYPES_ATTR_ID                     0x0316
#define MAP_SUPPORTED_FEATURES_ATTR_ID                      0x0317

/* CTN Attribute IDs */
#define CTN_CAS_INSTANCE_ID_ATTR_ID                         0x0315
#define CTN_SUPPORTED_FEATURES_ATTR_ID                      0x0317

/* MPS Attribute IDs */
#define MPS_MPSD_SCENARIOS_ATTR_ID                          0x0200
#define MPS_MPMD_SCENARIOS_ATTR_ID                          0x0201
#define MPS_SUPP_PROF_PROT_DEP_ATTR_ID                      0x0202

/* 32 bit Protocol UUID's definitions */
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

#define ROOT_ID                                 0x00001002

/* Service Class UUID's definitions */
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

/* PAN UUIDs */
#define BNEP_UUID                               0x0000000F
#define PANU_UUID                               0x00001115
#define NETWORK_ACCESS_POINT_UUID               0x00001116
#define GROUP_NETWORK_UUID                      0x00001117

/* BPP UUIDs */
#define BPP_UUID                                0x00001122
#define BPP_DIRECT_PRINTING_UUID                0x00001118
#define BPP_REFERENCE_PRINTING_UUID             0x00001119
#define BPP_DIRECT_PRINT_REF_OBJ_SERVICE_UUID   0x00001120
#define BPP_REFLECTED_UI_UUID                   0x00001121
#define BPP_PRINTING_STATUS_UUID                0x00001123

/* Imaging UUIDs */
#define IMAGING_UUID                            0x0000111A
#define IMAGING_RESPONDER_UUID                  0x0000111B
#define IMAGING_AUTOMATIC_ARCHIVE_UUID          0x0000111C
#define IMAGING_REFERENCED_OBJECTS_UUID         0x0000111D

/* HID Profile UUIDs */
#define HIDP_UUID                               0x00000011
#define HID_SERVICE_UUID                        0x00001124

/* HCRP UUIDs */
#define HCRP_CONTROL_CH_UUID                    0x00000012
#define HCRP_DATA_CH_UUID                       0x00000014
#define HCRP_NOTIFY_CH_UUID                     0x00000016
#define HCRP_UUID                               0x00001125
#define HCRP_PRINT_UUID                         0x00001126
#define HCRP_SCAN_UUID                          0x00001127

/* HFP HFU/AG UUIDs */
#define HFP_HFU_HANDS_FREE_UUID                 0x0000111E
#define HFP_AG_AUDIO_GATEWAY_UUID               0x0000111F
#define HFP_GENERIC_AUDIO                       0x00001203

/* AV Protocol UUIDs */
#define AVCTP_UUID                              0x00000017
#define AVDTP_UUID                              0x00000019

/* A2DP UUIDs */
#define A2DP_UUID                               0x0000110D
#define A2DP_SOURCE_UUID                        0x0000110A
#define A2DP_SINK_UUID                          0x0000110B

/* AVRCP UUIDs */
#define AVRCP_TARGET_UUID                       0x0000110C
#define AVRCP_UUID                              0x0000110E
#define AVRCP_CONTROLLER_UUID                   0x0000110F

/* UDIP UUIDs */
#define UDI_UE_UUID                             0x0000112A
#define UDI_TE_UUID                             0x0000112B

/* PHONEBOOK UUIDs */
#define PHONEBOOK_PCE_UUID                      0x0000112E
#define PHONEBOOK_PSE_UUID                      0x0000112F
#define PHONEBOOK_UUID                          0x00001130

/* SIM Access Profile UUID */
#define SIM_ACCESS_UUID                         0x0000112D

/* MCAP UUIDs */
#define MCAP_CONTROL_CHANNEL_UUID               0x0000001E
#define MCAP_DATA_CHANNEL_UUID                  0x0000001F

/* HDP UUIDs */
#define HDP_UUID                                0x00001400
#define HDP_SOURCE_UUID                         0x00001401
#define HDP_SINK_UUID                           0x00001402

/* MAP UUID */
#define MAP_MAS_UUID                            0x00001132
#define MAP_MNS_UUID                            0x00001133
#define MAP_UUID                                0x00001134

/* GNSS UUID */
#define GNSS_UUID                               0x00001135
#define GNSS_SERVER_UUID                        0x00001136

/* CTN UUID */
#define CTN_CAS_UUID                            0x0000113C
#define CTN_CNS_UUID                            0x0000113D
#define CTN_UUID                                0x0000113E


/* New Defines for SDP headers */
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

/*
 * All Service IDs for the profiles. Profiles can use a range of xxx0 to
 * xxxF as the service IDs where xxx is the BASE Service ID provided for
 * that particular profile
 */
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

#endif /* SDP_HAVE_LIB_INDEPENDENT_DBASE_SUPPORT */

/* ----------------------Structure Declarations section ---*/

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

/*---------------------------------Function Declarations */
#ifdef __cplusplus
extern "C" {
#endif

void BT_dbase_init
     (
         void
     );

API_RESULT BT_dbase_get_rec_handles
           (
               UCHAR * /* uuids_array */,
               UCHAR /* num_uuids */,
               UCHAR * /*num_handles*/,
               UINT32 * /*han_array*/
           );

API_RESULT BT_dbase_get_attr_value
           (
               UINT32 /*record_handle */,
               UINT16* /*attr_id*/,
               UCHAR * /*att_value buffer*/,
               UINT16  /* Length of attr_value buffer */,
               UINT32 * /*att_val_len*/
           );

API_RESULT BT_dbase_update_attr_value
           (
               UINT32 /* record_handle */,
               UINT16 /* attr_id */,
               UCHAR * /* attr_value */,
               UINT16 /* attr_value_length */
           );

API_RESULT BT_dbase_change_attr_value
           (
               UINT32 /* record_handle */,
               UINT16 /* attr_id */,
               UCHAR * /* attr_value */,
               UINT16 /* attr_value_length */
           );


API_RESULT BT_dbase_activate_record
           (
               /* IN */  UINT32    record_handle
           );

API_RESULT BT_dbase_inactivate_record
           (
               /* IN */  UINT32    record_handle
           );

API_RESULT BT_dbase_get_record_handle
           (
               /* IN */ UCHAR service_type,
               /* IN */ UCHAR service_instance,
               /* OUT */ UINT32 *rec_hdl
           );

API_RESULT BT_dbase_get_server_channel
           (
               /* IN */  UINT32 record_handle,
               /* IN */  UINT16 attr_id,
               /* OUT */ UCHAR *server_channel
           );

API_RESULT BT_dbase_get_obex_psm
           (
               /* IN */  UINT32   record_handle,
               /* IN */  UINT16   attr_id,
               /* OUT */ UINT16 * psm
           );

API_RESULT BT_dbase_get_num_service_class_uuids
           (
               /* OUT */ UINT32 *    num_serv_class_uuids
           );

API_RESULT BT_dbase_get_service_class_uuids
           (
               /* OUT */   UCHAR *     uuid_buffer,
               /* INOUT */ UINT32 *    uuid_buffer_length
           );

#ifdef __cplusplus
};
#endif

#endif /* _H_BT_DBASE_API_ */


