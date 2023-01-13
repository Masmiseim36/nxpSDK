
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
#define DB_RECORD_ACTIVE           1U
#define DB_RECORD_INACTIVE         0U

/* SDP Server Record Handle */
#define SDP_SERVER_RECORD_HANDLE                0x00000000U

/* Start of other valid SDP Record Handles */
#define SDP_START_OF_VALID_RECORD_HANDLE        0x00010000U

/* PnP Info Attribute IDs */
#define PNP_INFO_SPECIFICATION_ID_ATTR_ID       0x0200U
#define PNP_INFO_VENDOR_ID_ATTR_ID              0x0201U
#define PNP_INFO_PRODUCT_ID_ATTR_ID             0x0202U
#define PNP_INFO_VERSION_ATTRIBUTE_ATTR_ID      0x0203U
#define PNP_INFO_PRIMARY_RECORD_ATTR_ID         0x0204U
#define PNP_INFO_VENDOR_ID_SOURCE_ATTR_ID       0x0205U


#define PNP_INFO_BASE_SERVICE_ID                (0x0620U | BASE_SERVICE_ID)

/* Attribute Identifier Codes Definitions */
#define SERVICE_RECORD_HANDLE                   0x0000U
#define SERVICE_CLASS_ID_LIST                   0x0001U
#define SERVICE_RECORD_STATE                    0x0002U
#define SERVICE_ID                              0x0003U
#define PROTOCOL_DESC_LIST                      0x0004U
#define BROWSE_GROUP_LIST                       0x0005U
#define LANG_BASE_ATTR_ID_LIST                  0x0006U
#define SERVICE_INFO_TIME_TO_LIVE               0x0007U
#define SERVICE_AVAILABILITY                    0x0008U
#define BLUETOOTH_PROFILE_DESC_LIST             0x0009U
#define DOCUMENTATION_URL                       0x000AU
#define CLIENT_EXECUTABLE_URL                   0x000BU
#define ICON_URL                                0x000CU
#define ADDITIONAL_PROT_DESC_LIST_ID            0x000DU

#define VERSION_NO_LIST                         0x0200U
#define SERVICE_DATABASE_STATE                  0x0201U
#define GROUP_ID                                0x0200U
#define SUPPORTED_FEATURES                      0x0200U

#define REMOTE_AUDIO_VOL_CTRL                   0x0302U
#define EXTERNAL_NETWORK                        0x0301U
#define SERVICE_VERSION                         0x0300U
#define SUPPORTED_DATA_STORES_LIST              0x0301U
#define SUPPORTED_FORMATS_LIST                  0x0303U
#define FAX_1_CLASS_SUPPORT                     0x0302U
#define FAX_2_0_CLASS_SUPPORT                   0x0303U
#define FAX_2_CLASS_SUPPORT                     0x0304U
#define AUDIO_FEEDBACK_SUPPORT                  0x0305U

#define SDP_ATTR_ID_SUPPORTED_CAPABILITIES      0x0310U
#define SDP_ATTR_ID_SUPPORTED_FUNCTIONS         0x0312U
#define SDP_ATTR_ID_TOTAL_IMG_DATA_CAPACITY     0x0313U
#define SUPPORTED_REPOSITORIES                  0x0314U

#define MAS_INSTANCE_ID                         0x0315U
#define SUPPORTED_MESSAGE_TYPES                 0x0316U

/* MPS Attribute IDs */
#define MPS_MPSD_SCENARIOS_ATTR_ID              0x0200U
#define MPS_MPMD_SCENARIOS_ATTR_ID              0x0201U
#define MPS_SUPP_PROF_PROT_DEP_ATTR_ID          0x0202U

#define GOEP_L2CAP_PSM_ATTR_ID                  0x0200U

/*
 * Below attributes form offsets from the base attr id that is specied
 * under the values of Lang_base_attr_id_list (attrid: 0x0006).
 */
#define LANG_BASE_ATTR_ID                        0x0100U
#define SERVICE_NAME_ATTR_ID                    ( 0x0100U + 0U )
#define SERVICE_DESC_ATTR_ID                    ( 0x0100U + 1U )
#define PROVIDER_NAME_ATTR_ID                   ( 0x0100U + 2U )

/* BPP Attribute Ids */
#define PRINTER_STATUS_CHANNEL                  0x0305U
#define RUI_JOB_CHANNEL                         0x0374U
/**
 * @name BPP Attribute Ids
 */
/*@{*/
#define BPP_DOCUMENT_FORMATS_SUPPORTED_ATTR_ID              0x0350U
#define BPP_CHARACTER_REPERTOIRES_SUPPORTED_ATTR_ID         0x0352U
#define BPP_XHTML_PRINT_IMG_FORMATS_SUPPORTED_ATTR_ID       0x0354U
#define BPP_COLOR_SUPPORTED_ATTR_ID                         0x0356U
#define BPP_1284ID_ATTR_ID                                  0x0358U
#define BPP_PRINTER_NAME_ATTR_ID                            0x035AU
#define BPP_PRINTER_LOCATION_ATTR_ID                        0x035CU
#define BPP_DUPLEX_SUPPORTED_ATTR_ID                        0x035EU
#define BPP_MEDIA_TYPES_SUPPORTED_ATTR_ID                   0x0360U
#define BPP_MAX_MEDIA_WIDTH_ATTR_ID                         0x0362U
#define BPP_MAX_MEDIA_LENGTH_ATTR_ID                        0x0364U
#define BPP_ENHANCED_LAYOUT_SUPPORTED_ATTR_ID               0x0366U
#define BPP_RUI_FORMATS_SUPPORTED_ATTR_ID                   0x0368U
#define BPP_REF_PRINTING_RUI_SUPPORTED_ATTR_ID              0x0370U
#define BPP_DIRECT_PRINTING_RUI_SUPPORTED_ATTR_ID           0x0372U
#define BPP_REF_PRINTING_TOP_URL_ATTR_ID                    0x0374U
#define BPP_DIRECT_PRINTING_TOP_URL_ATTR_ID                 0x0376U
#define BPP_PRINTER_ADMIN_RUI_TOP_URL_ATTR_ID               0x0378U
/*@}*/


/* HCRP Profile Attribute IDs */
/**
 * @name HCRP Profile Attribute IDs
 */
/*@{*/
#define HCRP_1284ID_ATTR_ID                                 0x0300U
#define HCRP_DEVICE_NAME_ATTR_ID                            0x0302U
#define HCRP_FRIENDLY_NAME_ATTR_ID                          0x0304U
/*@}*/

/* HID Profile Attribute IDs */
/**
 * @name HID Profile Attribute IDs
 */
/*@{*/
#define HID_DEVICE_RELEASE_NUMBER_ATTR_ID                   0x0200U
#define HID_PARSER_VERSION_ATTR_ID                          0x0201U
#define HID_DEVICE_SUBCLASS_ATTR_ID                         0x0202U
#define HID_COUNTRY_CODE_ATTR_ID                            0x0203U
#define HID_VIRTUAL_CABLE_ATTR_ID                           0x0204U
#define HID_RECONNECT_INITIATE_ATTR_ID                      0x0205U
#define HID_DESCRIPTOR_LIST_ATTR_ID                         0x0206U
#define HID_LANGID_BASE_LIST_ATTR_ID                        0x0207U
#define HID_SDP_DISABLE_ATTR_ID                             0x0208U
#define HID_BATTERY_POWER_ATTR_ID                           0x0209U
#define HID_REMOTE_WAKE_ATTR_ID                             0x020AU
#define HID_PROFILE_VERSION_ATTR_ID                         0x020BU
#define HID_SUPERVISION_TIMEOUT_ATTR_ID                     0x020CU
#define HID_NORMALLY_CONNECTABLE_ATTR_ID                    0x020DU
#define HID_BOOT_DEVICE_ATTR_ID                             0x020EU
#define HID_SSR_HOST_MAX_LATENCY_ATTR_ID                    0x020FU
#define HID_SSR_HOST_MIN_TIMEOUT_ATTR_ID                    0x0210U
/*@}*/

/* PAN Profile Attribute Ids */
/**
 * @name PAN Profile Attribute Ids
 */
/*@{*/
#define IP_SUBNET_ID                                        0x0200U
#define SECURITY_DESC_ID                                    0x030AU
#define NET_ACCESS_TYPE_ID                                  0x030BU
#define MAX_NET_ACCESS_RATE                                 0x030CU
/*@}*/

/* HFP &Imaging Attribute IDs */
/**
 * @name HFP &Imaging Attribute IDs
 */
/*@{*/
#define SUPPORTED_FEATURES_ATTR_ID                          0x0311U
/*@}*/
/* HFP Attrib IDs*/
/**
 * @name HFP &Imaging Attribute IDs
 */
/*@{*/
#define HFP_NETWORK_ATTR_ID                                 0x0301U
/*@}*/
/* Few more constants evolve during Lang_base_attr_id_list */
/**
 * @name Lang base Attribute IDs
 */
/*@{*/
#define LANG_ISO639_ID                                      0x656EU
#define LANG_CHAR_ENC_IANA_ID                               0x006AU
/*@}*/
/* PBAP Attribute IDs */
/**
 * @name PBAP Attribute IDs
 */
/*@{*/
#define PBAP_SUPPORTED_REPOSITORIES                         0x0314U
#define PBAP_SUPPORTED_FEATURES_ATTR_ID                     0x0317U
/*@}*/
/* MAP Attribute IDs */
/**
 * @name MAP Attribute IDs
 */
/*@{*/
#define MAP_MAS_INSTANCE_ID_ATTR_ID                         0x0315U
#define MAP_SUPPORTED_MSG_TYPES_ATTR_ID                     0x0316U
#define MAP_SUPPORTED_FEATURES_ATTR_ID                      0x0317U
/*@}*/
/* CTN Attribute IDs */
/**
 * @name CTN Attribute IDs
 */
/*@{*/
#define CTN_CAS_INSTANCE_ID_ATTR_ID                         0x0315U
#define CTN_SUPPORTED_FEATURES_ATTR_ID                      0x0317U
/*@}*/
/* MPS Attribute IDs */
/**
 * @name CTN Attribute IDs
 */
/*@{*/
#define MPS_MPSD_SCENARIOS_ATTR_ID                          0x0200U
#define MPS_MPMD_SCENARIOS_ATTR_ID                          0x0201U
#define MPS_SUPP_PROF_PROT_DEP_ATTR_ID                      0x0202U
/*@}*/
/* 32 bit Protocol UUID's definitions */
/**
 * @name 32 bit Protocol UUID's definitions
 */
/*@{*/
#define SDP_UUID                                0x00000001U
#define RFCOMM_UUID                             0x00000003U
#define TCSBIN_UUID                             0x00000005U
#define ATT_PROTO_UUID                          0x00000007U
#define L2CAP_UUID                              0x00000100U
#define IP_UUID                                 0x00000009U
#define UDP_UUID                                0x00000002U
#define TCP_UUID                                0x00000004U
#define TCSAT_UUID                              0x00000006U
#define OBEX_UUID                               0x00000008U
#define FTP_UUID                                0x0000000AU
#define HTTP_UUID                               0x0000000CU
#define WSP_UUID                                0x0000000EU
/*@}*/
#define ROOT_ID                                 0x00001002U

/* Service Class UUID's definitions */
/**
 * @name Service Class UUID's definitions
 */
/*@{*/
#define SDPSERVER_SERVICECLASS_UUID             0x00001000U
#define BROWSEGROUPDESC_SERVICECLASS_UUID       0x00001001U
#define PUBLICBROWSEGROUP_UUID                  0x00001002U
#define SERIALPORT_UUID                         0x00001101U
#define LANACCESS_USINGPPP_UUID                 0x00001102U
#define DIALUP_NETWORKING_UUID                  0x00001103U
#define IRMCSYNC_UUID                           0x00001104U
#define OBEXOBJECT_PUSH_UUID                    0x00001105U
#define OBEXFILETRANSFER_UUID                   0x00001106U
#define IRMCSYNC_COMMAND_UUID                   0x00001107U
#define HEADSET_UUID                            0x00001108U
#define CORDLESS_TELEPHONY_UUID                 0x00001109U
#define INTERCOM_UUID                           0x00001110U
#define FAX_UUID                                0x00001111U
#define HEADSET_AUDIOGATEWAY_UUID               0x00001112U
#define PNP_INFO_UUID                           0x00001200U
#define GENERIC_NETWORKING_UUID                 0x00001201U
#define GENERIC_FILETRANSFER_UUID               0x00001202U
#define GENERIC_AUDIO_UUID                      0x00001203U
#define GENERIC_TELEPHONY_UUID                  0x00001204U
#define HEADSET_HS_UUID                         0x00001131U
/*@}*/
/* PAN UUIDs */
/**
 * @name PAN UUIDs
 */
/*@{*/
#define BNEP_UUID                               0x0000000FU
#define PANU_UUID                               0x00001115U
#define NETWORK_ACCESS_POINT_UUID               0x00001116U
#define GROUP_NETWORK_UUID                      0x00001117U
/*@}*/
/* BPP UUIDs */
/**
 * @name BPP UUIDs
 */
/*@{*/
#define BPP_UUID                                0x00001122U
#define BPP_DIRECT_PRINTING_UUID                0x00001118U
#define BPP_REFERENCE_PRINTING_UUID             0x00001119U
#define BPP_DIRECT_PRINT_REF_OBJ_SERVICE_UUID   0x00001120U
#define BPP_REFLECTED_UI_UUID                   0x00001121U
#define BPP_PRINTING_STATUS_UUID                0x00001123U
/*@}*/
/* Imaging UUIDs */
/**
 * @name Imaging UUIDs
 */
/*@{*/
#define IMAGING_UUID                            0x0000111AU
#define IMAGING_RESPONDER_UUID                  0x0000111BU
#define IMAGING_AUTOMATIC_ARCHIVE_UUID          0x0000111CU
#define IMAGING_REFERENCED_OBJECTS_UUID         0x0000111DU
/*@}*/
/* HID Profile UUIDs */
/**
 * @name HID Profile UUIDs
 */
/*@{*/
#define HIDP_UUID                               0x00000011U
#define HID_SERVICE_UUID                        0x00001124U
/*@}*/
/* HCRP UUIDs */
/**
 * @name HCRP UUIDs
 */
/*@{*/
#define HCRP_CONTROL_CH_UUID                    0x00000012U
#define HCRP_DATA_CH_UUID                       0x00000014U
#define HCRP_NOTIFY_CH_UUID                     0x00000016U
#define HCRP_UUID                               0x00001125U
#define HCRP_PRINT_UUID                         0x00001126U
#define HCRP_SCAN_UUID                          0x00001127U
/*@}*/
/* HFP HFU/AG UUIDs */
/**
 * @name  HFP HFU/AG UUIDs
 */
/*@{*/
#define HFP_HFU_HANDS_FREE_UUID                 0x0000111EU
#define HFP_AG_AUDIO_GATEWAY_UUID               0x0000111FU
#define HFP_GENERIC_AUDIO                       0x00001203U
/*@}*/
/* AV Protocol UUIDs */
/**
 * @name  HFP HFU/AG UUIDs
 */
/*@{*/
#define AVCTP_UUID                              0x00000017U
#define AVDTP_UUID                              0x00000019U
/*@}*/
/* A2DP UUIDs */
/**
 * @name  HFP HFU/AG UUIDs
 */
/*@{*/
#define A2DP_UUID                               0x0000110DU
#define A2DP_SOURCE_UUID                        0x0000110AU
#define A2DP_SINK_UUID                          0x0000110BU
/*@}*/
/* AVRCP UUIDs */
/**
 * @name  AVRCP UUIDs
 */
/*@{*/
#define AVRCP_TARGET_UUID                       0x0000110CU
#define AVRCP_UUID                              0x0000110EU
#define AVRCP_CONTROLLER_UUID                   0x0000110FU
/*@}*/
/* UDIP UUIDs */
/**
 * @name  UDIP UUIDs
 */
/*@{*/
#define UDI_UE_UUID                             0x0000112AU
#define UDI_TE_UUID                             0x0000112BU
/*@}*/
/* PHONEBOOK UUIDs */
/**
 * @name  PHONEBOOK UUIDs
 */
/*@{*/
#define PHONEBOOK_PCE_UUID                      0x0000112EU
#define PHONEBOOK_PSE_UUID                      0x0000112FU
#define PHONEBOOK_UUID                          0x00001130U
/*@}*/
/* SIM Access Profile UUID */
/**
 * @name  SIM Access Profile UUIDs
 */
/*@{*/
#define SIM_ACCESS_UUID                         0x0000112DU
/*@}*/
/* MCAP UUIDs */
/**
 * @name  MCAP UUIDs
 */
/*@{*/
#define MCAP_CONTROL_CHANNEL_UUID               0x0000001EU
#define MCAP_DATA_CHANNEL_UUID                  0x0000001FU
/*@}*/
/* HDP UUIDs */
/**
 * @name  HDP UUIDs
 */
/*@{*/
#define HDP_UUID                                0x00001400U
#define HDP_SOURCE_UUID                         0x00001401U
#define HDP_SINK_UUID                           0x00001402U
/*@}*/
/* MAP UUID */
/**
 * @name  MAP UUID
 */
/*@{*/
#define MAP_MAS_UUID                            0x00001132U
#define MAP_MNS_UUID                            0x00001133U
#define MAP_UUID                                0x00001134U
/*@}*/
/* GNSS UUID */
/**
 * @name   GNSS UUID
 */
/*@{*/
#define GNSS_UUID                               0x00001135U
#define GNSS_SERVER_UUID                        0x00001136U
/*@}*/
/* CTN UUID */
/**
 * @name   CTN UUID
 */
/*@{*/
#define CTN_CAS_UUID                            0x0000113CU
#define CTN_CNS_UUID                            0x0000113DU
#define CTN_UUID                                0x0000113EU
/*@}*/

/* New Defines for SDP headers */
/**
 * @name   New Defines for SDP headers
 */
/*@{*/
#define DB_2BYTE_UUID_HEADER                    0x19U
#define DB_4BYTE_UUID_HEADER                    0x1AU
#define DB_16BYTE_UUID_HEADER                   0x1CU
#define DB_DATA_ELEM_ALT_HEADER                 0x36U
#define DB_DATA_ELEM_SEQ_HEADER                 0x35U
#define DB_4BYTE_INT_HEADER                     0x0AU
#define DB_2BYTE_INT_HEADER                     0x09U
#define DB_1BYTE_INT_HEADER                     0x08U
#define DB_URL_STR_HEADER                       0x45U
#define DB_TEXT_STR_HEADER                      0x25U
#define DB_BOOLEAN_HEADER                       0x28U
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
#define BASE_SERVICE_ID               0xA000U

#define GAP_BASE_SERVICE_ID          (0x0500U | BASE_SERVICE_ID)
#define SDAP_BASE_SERVICE_ID         (0x0510U | BASE_SERVICE_ID)
#define CTP_BASE_SERVICE_ID          (0x0520U | BASE_SERVICE_ID)
#define ICP_BASE_SERVICE_ID          (0x0530U | BASE_SERVICE_ID)
#define SPP_BASE_SERVICE_ID          (0x0540U | BASE_SERVICE_ID)
#define HEADSET_BASE_SERVICE_ID      (0x0550U | BASE_SERVICE_ID)
#define DUNP_BASE_SERVICE_ID         (0x0560U | BASE_SERVICE_ID)
#define FAX_BASE_SERVICE_ID          (0x0570U | BASE_SERVICE_ID)
#define LAP_BASE_SERVICE_ID          (0x0580U | BASE_SERVICE_ID)
#define GOBEX_BASE_SERVICE_ID        (0x0590U | BASE_SERVICE_ID)
#define OPP_BASE_SERVICE_ID          (0x05A0U | BASE_SERVICE_ID)
#define FTP_BASE_SERVICE_ID          (0x05B0U | BASE_SERVICE_ID)
#define SYNC_BASE_SERVICE_ID         (0x05C0U | BASE_SERVICE_ID)
#define HCRP_BASE_SERVICE_ID         (0x05D0U | BASE_SERVICE_ID)
#define HID_BASE_SERVICE_ID          (0x05E0U | BASE_SERVICE_ID)
#define HFP_BASE_SERVICE_ID          (0x05F0U | BASE_SERVICE_ID)
#define PAN_BASE_SERVICE_ID          (0x0600U | BASE_SERVICE_ID)
#define A2DP_BASE_SERVICE_ID         (0x0610U | BASE_SERVICE_ID)

#define MCM_BASE_SERVICE_ID          (0x0800U | BASE_SERVICE_ID)
#define BPP_BASE_SERVICE_ID          (0x0900U | BASE_SERVICE_ID)
/*@}*/
/* ----------- Global structures */
/* --------------------- Enumerators Declarations section ---*/
enum attr_type
{
    DB_NIL = 0U,
    DB_UNSIGNED_INT = 1U,
    DB_SIGNED_2COM_INT = 2U,
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
#define DB_RECORD_A2DP_SINK                          0x80U
#define DB_RECORD_A2DP_SOURCE                        0x81U
#define DB_RECORD_AVRCP_TARGET                       0x82U
#define DB_RECORD_AVRCP_CONTROLLER                   0x83U
#define DB_RECORD_RO                                 0x84U
#define DB_RECORD_ARCHIVED_OBJECTS                   0x85U
#define DB_RECORD_BIP                                0x86U
#define DB_RECORD_BPP                                0x87U
#define DB_RECORD_BASIC_PRINTING_REFERENCED_OBJECTS  0x88U
#define DB_RECORD_FTP                                0x89U
#define DB_RECORD_MAP_MCE                            0x8AU
#define DB_RECORD_MAP_MSE                            0x8BU
#define DB_RECORD_OPP                                0x8CU
#define DB_RECORD_PBAP_PCE                           0x8DU
#define DB_RECORD_PBAP_PSE                           0x8EU
#define DB_RECORD_SAP                                0x8FU
#define DB_RECORD_UDI_UE                             0x90U
#define DB_RECORD_UDI_TE                             0x91U
#define DB_RECORD_CTN_CCE                            0x92U
#define DB_RECORD_CTN_CSE                            0x93U
/*@}*/
#endif /* SDP_HAVE_LIB_INDEPENDENT_DBASE_SUPPORT */
/** \} */
/* ----------------------Structure Declarations section ---*/
/**
 * \defgroup dbase_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
struct SDP_ATTR
{
    UINT16 attr_id;
    UINT16 len;
    UCHAR *value;
};


typedef struct _SDP_RECORD
{
    UINT32 rec_handle;
    UINT16 active;
    UINT16 num_uuids;
    UINT16 num_attrs;
    UINT16 attr_offset;
#ifdef SDP_DYNAMIC_DB
    UINT8  service_type;
    UINT8  service_instance;
#endif /* SDP_DYNAMIC_DB */
}SDP_RECORD;


#ifdef SDP_DYNAMIC_DB
typedef struct _DB_SERVICE_CLASS_UUID_ELEM
{
    UINT16      uuid_len;
    UINT16      uuid_16;
    UINT8       uuid_128[16U];
} DB_SERVICE_CLASS_UUID_ELEM;

typedef struct
{
    UINT16      protocol_uuid;
    UINT16      num_params;
    /* Sub-params always towards the end of list */
    UINT16      num_sub_params;
    UINT16      params[DB_MAX_PROTOCOL_PARAMS];
} DB_PROTOCOL_ELEM;

typedef struct
{
    UINT16              num_elems;
    DB_PROTOCOL_ELEM    elem[DB_MAX_LIST_ELEMS];
} DB_PROTO_LIST_ELEM;

typedef struct
{
    UINT8     * desc;
    UINT16      desc_len;
    UINT16      data_type;

    UINT8       mdep_id;
    UINT8       role;
} DB_MDEP_ELEM;

#endif /* SDP_DYNAMIC_DB */

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
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
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
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
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
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
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
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
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
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
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
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
 */
#ifdef SDP_SERVER_ACCESS_RFCOMM_CH
#define BT_dbase_get_server_channel(rh, aid, ch)                \
        BT_dbase_access_server_channel((rh), (aid), 0x00, (ch))
#else
API_RESULT BT_dbase_get_server_channel
           (
               /* IN */  UINT32 record_handle,
               /* IN */  UINT16 attr_id,
               /* OUT */ UCHAR *server_channel
           );
#endif /* SDP_SERVER_ACCESS_RFCOMM_CH */

#ifdef SDP_SERVER_ACCESS_RFCOMM_CH
/**
 *  \brief To set a RFCOMM Server Channel present in an SDP Database Record.
 *
 *  \par Description:
 *       This function enables to update the RFCOMM Server Channel corresponding to a SDP
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
 *  \param [in] server_channel
 *         Pointer to a caller allocated UCHAR variable, that is filled with the
 *         RFCOMM Server Channel value to be updated.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
 */

#define BT_dbase_set_server_channel(rh, aid, ch)                \
        BT_dbase_access_server_channel((rh), (aid), 0x01, (ch))

/**
 *  \brief To access a RFCOMM Server Channel present in an SDP Database Record.
 *
 *  \par Description:
 *       This function enabables to access the RFCOMM Server Channel corresponding to a SDP
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
 *  \param [in] flag
 *         Flag that indicate to get or set the RFCOMM channel number.
 *          0x00 -> To get the rfcomm channel.
 *          0x01 -> To set the rfcomm channel.
 *
 *  \param [inout] server_channel
 *         Pointer to a caller allocated UCHAR variable, that is filled with the
 *         RFCOMM Server Channel value on return or to be set.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
 *
 *  \note
 */
API_RESULT BT_dbase_access_server_channel
           (
               /* IN    */  UINT32     record_handle,
               /* IN    */  UINT16     attr_id,
               /* IN    */  UCHAR      flag,
               /* INOUT */ UCHAR *    server_channel
           );
#endif /* SDP_SERVER_ACCESS_RFCOMM_CH */

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
 *       API_SUCCESS or one of the error codes as defined in \ref BT_error.h.
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

#ifdef SDP_DYNAMIC_DB
/**
 *  \brief To create an SDP database record at runtime.
 *
 *  \par Description:
 *       This function can be used by the upper layer/application to create
 *       SDP database record at runtime.
 *
 *  \param [in] service_type
 *         SDP Service Type
 *
 *  \param [in] service_instance
 *         Instance of SDP Service. Useful when there are multiple SDP records
 *         for same profiles/services (like say for Hands-free, A2DP etc.)
 *
 *  \param [out] record_handle
 *         Returns the Service Record Handle associated with created SDP record
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_create_record
           (
               UINT8 service_type,
               UINT8 service_instance,
               UINT32 * record_handle
           );

/**
 *  \brief To delete an SDP database record at runtime.
 *
 *  \par Description:
 *       This function deletes an existing SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to be deleted.
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_delete_record(UINT32 record_handle);

/**
 *  \brief To add an attibute to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds an SDP attribute to an existing SDP database
 *       record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] attr_id
 *         SDP attribute ID
 *
 *  \param [in] attr_len
 *         Length of attribute
 *
 *  \param [in] attr_val
 *         Pointer to the buffer containing attribute value
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_attribute
           (
               UINT32 record_handle,
               UINT16 attr_id,
               UINT32 attr_len,
               UINT8 *attr_val
           );

/**
 *  \brief To delete an attribute from an SDP database record at runtime.
 *
 *  \par Description:
 *       This function deletes an existing attribure from an SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record from where the attribute to be deleted.
 *
 *  \param [in] attr_id
 *         Identifies the attribute to be deleted.
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_delete_attribute
           (
               UINT32 record_handle,
               UINT16 attr_id
           );

/**
 *  \brief To add an ServiceClassIdList attibute to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds an ServiceClassIdList attribute to an existing SDP database
 *       record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] num_services
 *         Number of UUIDs in ServiceClassIdList
 *
 *  \param [in] service_uuids
 *         List of 16-bit UUIDs in ServiceClassIdList
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_service_class_id_list
           (
               UINT32 record_handle,
               UINT16 num_services,
               UINT16 * service_uuids
           );

/**
 *  \brief To add an ServiceClassIdList attibute to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds an ServiceClassIdList attribute to an existing SDP database
 *       record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] num_services
 *         Number of UUIDs in ServiceClassIdList
 *
 *  \param [in] service_uuids
 *         List of 16-bit and/or 128-bit UUIDs in ServiceClassIdList
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_service_class_id_list_ex
           (
               UINT32 record_handle,
               UINT16 num_services,
               DB_SERVICE_CLASS_UUID_ELEM * service_uuids
           );

/**
 *  \brief To add ProtocolDescriptorList attibute to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds ProtocolDescriptorList attribute to an existing SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] num_elems
 *         Number of elements in the sequence of ProtocolDescriptorList elements
 *
 *  \param [in] elems
 *         Pointer to sequence of ProtocolDescriptorList elements
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_proto_desc_list
           (
               UINT32 record_handle,
               UINT16 num_elems,
               DB_PROTOCOL_ELEM * elems
           );

/**
 *  \brief To add AdditionalProtocolDescriptorList attibute to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds AdditionalProtocolDescriptorList attribute to an existing SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] num_elems
 *         Number of elements in the sequence of ProtocolDescriptorList elements
 *
 *  \param [in] elems
 *         Pointer to sequence of ProtocolDescriptorList elements
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_additional_proto_desc_list
           (
               UINT32 record_handle,
               UINT16 num_elems,
               DB_PROTO_LIST_ELEM * elems
           );

/**
 *  \brief To add BrowseGroupList attibute to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds BrowseGroupList attribute to an existing SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] num_uuids
 *         Number of UUIDs representing browse groups
 *
 *  \param [in] uuids
 *         Pointer to sequence of UUIDs representing browse groups
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_browse_group_list
           (
               UINT32 record_handle,
               UINT16 num_uuids,
               UINT16 * uuids
           );

/**
 *  \brief To add LanguageBaseAttributeIDList attibute to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds LanguageBaseAttributeIDList attribute to an existing SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] attr_id
 *         SDP attribute ID
 *
 *  \param [in] language
 *         Identifier representing the natural language
 *
 *  \param [in] char_enc
 *         Identifier that specifies a character encoding used for the language
 *
 *  \param [in] base_id
 *         An attribute ID that serves as the base attribute ID for
 *         the natural language in the service record
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_languagebase_attr_id_list
           (
               UINT32 record_handle,
               UINT16 language,
               UINT16 char_enc,
               UINT16 base_id
           );

/**
 *  \brief To add BluetoothProfileDescriptorList attibute to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds BluetoothProfileDescriptorList attribute to an existing SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] profile_uuid
 *         UUID assigned to the profile
 *
 *  \param [in] version
 *         16-bit profile version number
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_profile_descriptor_list
           (
               UINT32 record_handle,
               UINT16 profile_uuid,
               UINT16 version
           );

/**
 *  \brief To add ServiceName attibute to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds ServiceName attribute to an existing SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] service_name_length
 *         Length of Service Name
 *
 *  \param [in] service_name
 *         Pointer to buffer containing Service Name
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_service_name
           (
               UINT32 record_handle,
               UINT16 service_name_length,
               UINT8 *service_name
           );

/**
 *  \brief To add ServiceDescription attibute to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds ServiceDescription attribute to an existing SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] service_desc_length
 *         Length of Service Description
 *
 *  \param [in] service_desc
 *         Pointer to buffer containing Service Description
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_service_description
           (
               UINT32 record_handle,
               UINT16 service_desc_length,
               UINT8 *service_desc
           );

/**
 *  \brief To add ProviderName attibute to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds ProviderName attribute to an existing SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] provider_name_length
 *         Length of Provider Name
 *
 *  \param [in] provider_name
 *         Pointer to buffer containing Provider Name
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_provider_name
           (
               UINT32 record_handle,
               UINT16 provider_name_length,
               UINT8 *provider_name
           );

/**
 *  \brief To add VersionNumber attibute to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds VersionNumber attribute to an existing SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] version_number
 *         Version Number of the profile/service
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_version_numder
           (
               UINT32 record_handle,
               UINT16 version_number
           );

/**
 *  \brief To add SupportedFeatures attibute to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds SupportedFeatures attribute to an existing SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] supported_features
 *         Supported Features value - typically a bit-mask defined in profile specifications.
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_supported_features
           (
               UINT32 record_handle,
               UINT16 supported_features
           );

/**
 *  \brief To add an attibute of type text to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds an attribute of type text to an existing SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] attr_id
 *         SDP attribute ID
 *
 *  \param [in] length
 *         Length of the attribute value
 *
 *  \param [in] text
 *         Pointer to buffer containing attribute type text
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_attribute_type_text
           (
               UINT32 record_handle,
               UINT16 attr_id,
               UINT16 length,
               UINT8 *text
           );

/**
 *  \brief To add an attibute of type URL to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds an attribute of type URL to an existing SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] attr_id
 *         SDP attribute ID
 *
 *  \param [in] length
 *         Length of the attribute value
 *
 *  \param [in] url
 *         Pointer to buffer containing attribute type URL
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_attribute_type_url
           (
               UINT32 record_handle,
               UINT16 attr_id,
               UINT16 length,
               UINT8 *url
           );

/**
 *  \brief To add an attibute of type UINT to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds an attribute of type UINT to an existing SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] attr_id
 *         SDP attribute ID
 *
 *  \param [in] length
 *         Length of the attribute value
 *
 *  \param [in] value
 *         Pointer to buffer containing attribute type UINT
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_attribute_type_uint
           (
               UINT32 record_handle,
               UINT16 attr_id,
               UINT16 length,
               UINT8  * value
           );

/**
 *  \brief To add an attibute of type boolean to an SDP database record at runtime.
 *
 *  \par Description:
 *       This function adds an attribute of type boolean to an existing SDP database record.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] attr_id
 *         SDP attribute ID
 *
 *  \param [in] value
 *         Pointer to buffer containing attribute type boolean
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_attribute_type_boolean
           (
               UINT32 record_handle,
               UINT16 attr_id,
               UINT8  value
           );

/**
 *  \brief To add SupportedFeaturesList attibute to an SDP database record for HDP at runtime.
 *
 *  \par Description:
 *       This function adds SupportedFeaturesList attribute to an existing SDP database record
 *       for HDP.
 *
 *  \param [in] record_handle
 *         Identifies the SDP record to which the attribute to be added
 *
 *  \param [in] num_mdeps
 *         Number of MCAP Data End Points (MDEPs) in corresponding HDP database record
 *
 *  \param [in] mdep
 *         Pointer to sequence of MDEPs
 *
 *  \return
 *       API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_dbase_add_mdep_supported_features_list
           (
               UINT32 record_handle,
               UINT16 num_mdeps,
               DB_MDEP_ELEM * mdep
           );
#endif /* SDP_DYNAMIC_DB */

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_DBASE_API_ */

