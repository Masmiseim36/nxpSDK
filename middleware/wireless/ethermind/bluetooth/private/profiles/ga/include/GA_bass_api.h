
/**
 *  \file GA_bass_api.h
 *
 *  \brief This file defines the GA Broadcast Audio Scan Service (BASS)
 *  Client Entity(CE) and Server Entity(SE) Interface -
 *  includes Data Structures and Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_BASS_API_
#define _H_GA_BASS_API_

/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"
#include "GA_bearer_api.h"
#include "GA_property_config.h"

/* --------------------------------------------- Global Definitions */

/**
 * \defgroup ga_bass_module Broadcast Audio Scan Service (BASS)
 * \ingroup ga_cap_module
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * BASS Profile Client module to the Application and other upper
 * layers of the stack.
 */

/**
 * \defgroup ga_bass_defines Defines
 * \{
 * \brief This section describes defines for the BASS CE and SE module.
 */

/**
 * \defgroup ga_bass_ce_module_def BASS CE (Broadcast Audio Scan Service) Client Entity
 * \{
 * \brief This section describes the defines for BASS CE.
 */

/** \} */

/**
 * \defgroup ga_bass_se_module_def BASS SE (Broadcast Audio Scan Service) Server Entity
 * \{
 * \brief This section describes the defines for BASS SE.
 */

/** \} */

/**
 * \defgroup ga_bass_common BASS Common
 * \{
 * \brief Describes common macros for the module.
 */

/**
 * \defgroup ga_bass_constants Constants
 * \{
 * \brief This section describes Constants defined by the module.
 */

/**
 * \name BASS Constants - Control Point Opcode
 * \{
 * \brief This section lists the type of Broadcast Audio Scan Control Point
 * Opcodes used during Control point Write request/Command.
 * As defined in BASS Specification.
 */

/**
 * Remote Scan Stopped - Informs the server that the client is not scanning
 * for Broadcast Sources on behalf of the server.
 */
#define BASS_CP_OPCODE_REMOTE_SCAN_STOPPED  0x00
/**
 * Remote Scan Started - Informs the server that the client is scanning for
 * Broadcast Sources on behalf of the server.
 */
#define BASS_CP_OPCODE_REMOTE_SCAN_STARTED  0x01
/**
 * Add Source - Requests the server to add information including Metadata for
 * a Broadcast Source, and requests the server to synchronize to a
 * PA and/or BIS transmitted by the Broadcast Source.
 */
#define BASS_CP_OPCODE_ADD_SOURCE           0x02
/**
 * Modify Source - Requests the server to update Metadata, to synchronize to,
 * or to stop synchronizing to a PA and/or BIS transmitted by the
 * Broadcast Source identified by the Source_ID.
 */
#define BASS_CP_OPCODE_MODIFY_SOURCE        0x03
/**
 * Set Broadcast_Code - Provides the server with the Broadcast_Code to decrypt
 * a BIS transmitted by a Broadcast Source identified by the Source_ID.
 */
#define BASS_CP_OPCODE_SET_BC_CODE          0x04
/**
 * Remove Source - Requests the server to remove all information for a
 * Broadcast Source identified by the Source_ID.
 */
#define BASS_CP_OPCODE_REMOVE_SOURCE        0x05

/** \} */
/** \} */

/**
 * \defgroup ga_bass_error_codes Error Codes
 * \{
 * \brief This section lists the Error Codes that are provided as part of
 * return of functionalities of BASS
 */

/**
 * \name Application Error Codes: BASS
 * \{
 * \brief This section lists Application error codes,
 * As defined in BASS Specification.
 */

/**
 * An unsupported opcode has been used in a Broadcast Audio Scan Control
 * Point operation.
 */
#define BASS_APPL_ERR_CODE_OPCODE_NOT_SUPP  0x80
/**
 * The Source_ID written by a client does not match any Source_ID exposed in a
 * Broadcast Receive State characteristic value by the server.
 */
#define BASS_APPL_ERR_CODE_INVALID_SRC_ID   0x81

/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_bass_ce_module_def
 * \{
 */

/**
 * \defgroup ga_bass_ce_constants Constants
 * \{
 * \brief This section describes Constants defined by the module for Client.
 */

/**
 * \name BASS Client Constants - General Macros
 * \{
 * \brief Initialization and other General Macros offered by the module for Client.
 */

/**
 * Total number of Characteristic IDs,
 * \ref ga_bass_ce_constants
 */
#define BASS_CHAR_ID_COUNT               0x02U

/** \} */

/**
 * \name BASS Client Constants - BASS Char ID
 * \{
 * \brief This section lists the Characteristic ID references.
 */

/**
 * BASS Char ID: Broadcast Audio Scan Control Point,
 * \ref GA_CHAR_PROP_BASS_BCAST_AUDIO_SCAN_CP
 */
#define BASS_CHAR_ID_AUDIO_SCAN_CP                     0U
/** Broadcast Audio Scan Control Point Char Count */
#define BASS_CHAR_ID_AUDIO_SCAN_CP_COUNT               1U
/**
 * BASS Char ID: Broadcast Receive State,
 * \ref GA_CHAR_PROP_BASS_BCAST_RX_STATE
 */
#define BASS_CHAR_ID_RX_STATE                          1U
/** Broadcast Receive State Char Count */
#define BASS_CHAR_ID_RX_STATE_COUNT                    GA_CONFIG_LIMITS(BASS_CE_MAX_BC_RX_STATE_INSTANCES)

/** \} */

/** \} */

/**
 * \defgroup ga_bass_client_events Events
 * \{
 * \brief This section describes the Broadcast Audio Scan Service Events
 * for Client.
 */

/**
 * \name BASS Client Events - Setup
 * \{
 * \brief This section describes the BASS Setup Events for Client.
 */

/**
 * On completion of discovery of service, char and desc, BASS Handle is
 * provided to the application as an array based on the instances along with
 * the event along with the status.
 * In case where service is not found, \ref GA_FAILURE is sent as the event
 * status.
 */
#define BASS_CE_SETUP_CNF                      0x01

/** \} */

/**
 * \name BASS Client Events - Notifications
 * \{
 * \brief This section describes the BASS Notifications Events for Client.
 */

/**
 * This event is notified when the char is configured for any notification
 * change and a notification has occurred from the peer device.
 */
#define BASS_CE_BC_RX_STATE_IND                0x02

/** \} */

/**
 * \name BASS Client Events - Read
 * \{
 * \brief This event is notified whenever read request is successful and a
 * response is received from the peer.
 */

/**
 * This event is triggered whenever read request is successful and a response
 *  is received from the peer.
 * If status is set to \ref GA_CONTINUE, then the event will be triggered 'n'
 * times until completion with status set to \ref GA_SUCCESS or
 * \ref GA_FAILURE.
 */
#define BASS_CE_READ_BC_RX_STATE_CNF           0x03

/** \} */

/**
 * \name BASS Client Events - Write
 * \{
 * \brief The event is notified whenever write is successful and a write
 * response is received from peer.
 */

/**
 * The event is notified whenever write is successful and a write
 * response is received from peer.
 * This applies to only requests that was triggered with write type -
 * Write Request \ref GA_BASS_CE_WRITE_REQUEST.
 */
#define BASS_CE_BAS_CP_CNF                     0x04

/** \} */

/**
 * \name BASS Client Events - Release
 * \{
 * \brief This section describes the BASS Release Events for Client.
 */

/* TODO: Comment */
#define BASS_CE_RELEASE_CNF                    0x05
/** \} */

/** \} */

/**
 * \addtogroup ga_bass_ce_constants
 * \{
 */

/**
 * \name BASS Client Constants - Write Type
 * \{
 * \brief This section lists the type of Write allowed in BASS.
 */

/** Request */
#define GA_BASS_CE_WRITE_REQUEST               0x00
/** Command */
#define GA_BASS_CE_WRITE_WITHOUT_RSP           0x01

/** \} */

/** \} */
/** \} */

/**
 * \addtogroup ga_bass_se_module_def
 * \{
 */

/**
 * \defgroup ga_bass_se_constants Constants
 * \{
 * \brief This section describes Constants defined by the module for Server.
 */

/**
 * \name BASS Server Constants - General Macros
 * \{
 * \brief Initialization and other General Macros offered by the module for server.
 */

/** Maximum Brocast Receive State Instance */
#define BASS_CE_MAX_BC_RX_STATE_INST           0x03
/** \} */
/** \} */

/**
 * \defgroup ga_bass_server_events Events
 * \{
 * \brief This section describes the Broadcast Audio Scan Service Events
 * for Server.
 * Events generated as part of callback from BASS module to the application.
 * \ref BASS_SE_EVT_ID used for representation.
 */

/**
 * \name BASS Server Events - Read
 * \{
 * \brief This section describes the BASS Read Events for Server.
 */

/**
 * The below event is notified whenever a read request from peer is received.
 * Response to this request has to be sent by calling
 * \ref GA_bass_se_send_rsp().
 */
#define BASS_SE_READ_BC_RX_STATE_IND              0x01

/** \} */

/**
 * \name BASS Server Events - Write
 * \{
 * \brief This section describes the BASS Write Events for Server.
 */

/**
 * The below event is notified whenever write request/command is received from
 * peer BASS Client.
 * Response to this request has to be sent by calling
 * \ref GA_mcp_se_send_rsp().
 * Ignore the rx_state_inst field in the \ref BASS_EVT
 */
#define BASS_SE_WRITE_BAS_CP_IND                   0x02

/** \} */

/** \} */

/**
 * \addtogroup ga_bass_se_constants
 * \{
 */

/**
 * \name BASS Server Constants - Address Type
 * \{
 * \brief Describes Advertiser Address Type defined by the module.
 */

/** Public Device Address or Public Identity Address */
#define BASS_ADDR_TYPE_PUBLIC                       0x00U
/** Random Device Address or Random (static) Identity Address */
#define BASS_ADDR_TYPE_RANDOM                       0x01U
/** \} */

/**
 * \name BASS Server Constants - PA Sync
 * \{
 * \brief Describes PA Sync defined by the module.
 */

/** Do not synchronize to PA */
#define BASS_PA_SYNC_DO_NOT_SYNC_TO_PA                  0x00U
/** Synchronize to PA - PAST available */
#define BASS_PA_SYNC_SYNC_TO_PA_PAST_AVL                0x01U
/** Synchronize to PA - PAST not available */
#define BASS_PA_SYNC_SYNC_TO_PA_PAST_NOT_AVL            0x02U
/** \} */

/**
 * \name BASS Server Constants - PA Interval
 * \{
 * \brief Describes PA Interval defined by the module.
 */

/** PA_Interval unknown */
#define BASS_PA_INTERVAL_UNKNOWN                        0xFFFFU
/** \} */

/**
 * \name BASS Server Constants - BIS Sync
 * \{
 * \brief Describes BIS Sync defined by the module.
 */

/** Do not synchronize to BIS_index[x] */
#define BASS_BIS_SYNC_DO_NOT_SYNC_TO_BIS_INDEX         0x00000000U
/** Synchronize to BIS_index[x] */
#define BASS_BIS_SYNC_TO_BIS_INDEX                     (BASS_BIS_SYNC_DO_NOT_SYNC_TO_BIS_INDEX | 1U)
/** No preference */
#define BASS_BIS_SYNC_NO_PREFERENCE                    0xFFFFFFFFU
/** \} */

/**
 * \name BASS Server Constants - PA Sync State
 * \{
 * \brief Describes PA Sync State defined by the module.
 */

/** Not synchronized to PA */
#define BASS_PA_SYNC_STATE_NOT_SYNCHRONIZED             0x00U
/** SyncInfo Request */
#define BASS_PA_SYNC_STATE_SYNC_INFO_REQUEST            0x01U
/** Synchronized to PA */
#define BASS_PA_SYNC_STATE_SYNCHRONIZED_TO_PA           0x02U
/** Failed to synchronize to PA */
#define BASS_PA_SYNC_STATE_FAILED_TO_SYNCHRONIZE_TO_PA  0x03U
/** No PAST */
#define BASS_PA_SYNC_STATE_NOPAST                       0x04U
/** \} */

/**
 * \name BASS Server Constants - BIG Encryption
 * \{
 * \brief Describes BIG Encryption State defined by the module.
 */

/** Not encrypted */
#define BASS_BIG_ENCRYPTION_NOT_ENCRYPTED               0x00U
/** Broadcast_Code required */
#define BASS_BIG_ENCRYPTION_BC_CODE_REQUIRED            0x01U
/** Decrypting */
#define BASS_BIG_ENCRYPTION_DECRYPTING                  0x02U
/** Bad_Code (incorrect encryption key) */
#define BASS_BIG_ENCRYPTION_BAD_CODE                    0x03U
/** \} */

/**
 * \name BASS Server Constants - BIS Sync State
 * \{
 * \brief Describes BIS Sync State defined by the module.
 */

/** Not synchronized to BIS_index[x] */
#define BASS_BIS_SYNC_STATE_UPDATE_NOT_SYNC_TO_BIS_INDEX(bis_sync, bis_index) \
        (bis_sync) = (bis_sync) & ~(1 << (bis_index - 1U));

/** Synchronized to BIS_index[x] */
#define BASS_BIS_SYNC_STATE_UPDATE_SYNC_TO_BIS_INDEX(bis_sync, bis_index) \
        (bis_sync) = (bis_sync) | (1 << (bis_index - 1U));

/** Failed to sync to BIG */
#define BASS_BIS_SYNC_STATE_FAILED_TO_SYNC_TO_BIG    0xFFFFFFFFU

/** Not synchronized to any BIS_index[x] in the SG */
#define BASS_BIS_SYNC_STATE_NOT_SYNCED_TO_BIS        0x00000000U
/** \} */

/**
 * \name BASS Server Constants - Broadcast Code
 * \{
 * \brief Describes Broadcast Code defined by the module.
 */

/** Broadcast_Code Length */
#define BASS_BC_CODE_MAX_LEN                      16U
/** \} */

/**
 * \name BASS Server Constants - Broadcast ID
 * \{
 * \brief Describes Broadcast ID defined by the module.
 */

/** Broadcst ID Length */
#define BASS_BC_ID_LEN                            3U
/** \} */

/** \} */
/** \} */

/**
 * \addtogroup ga_bass_common
 * \{
 */

/**
 * \defgroup ga_bass_macros  Macros
 * \{
 * \brief Initialization and other Macros offered by the module.
 */

/** Derive the characteristic config mask for the Char ID */
#define BASS_CHAR_CONFIG(id)                             (1U << (id))

/** \} */

/* --------------------------------------------- Data types /Structures */

/**
 * \defgroup ga_bass_structures Structures
 * \{
 * \brief This section lists the various data structures and typedefs for use
 * by BASS.
 */

/**
 * This identifier refers to the registered BASS Broadcast Rx State Char
 * instance.
 * The application needs to provide this as a reference for all interface calls
 * to have transaction on the required Broadcast Rx State Char instance.
 */
typedef UINT8 BASS_BC_RX_STATE_INST;

/**
 * The handle indicates the Handle ID for BASS Session.
 * The application needs to provide this as a input param for all the calls
 * to have transaction on the required BASS Session.
 */
typedef UINT8 BASS_HANDLE;

/**
 * BASS CP Opcodes.
 */
typedef UINT8 BASS_BAS_CP_OPCODE;

typedef struct _GA_BASS_BAS_CP_OP
{
    /** Opcode, Refer \ref ga_bass_constants */
    BASS_BAS_CP_OPCODE opcode;

    /** Parameter for the respective Opcodes, Can be Null for certain Opcodes */
    UINT8 * param;

    /** Length of the Parameter */
    UINT8 param_len;
}GA_BASS_BAS_CP_OP;

/* BASS Broadcast Audio Scan CP Add Source Operation Paramaters */
typedef struct _GA_BASS_BAS_CP_ADD_SRC
{
    /**
     * Advertiser Address Type for the Broadcast Source.
     * Refer \ref ga_bass_se_constants
     */
    UINT8 adv_addr_type;

    /** Advertiser Address for the Broadcast Source */
    UINT8 adv_addr[GA_BD_ADDR_SIZE];

    /**
     * Advertising_SID subfield of the ADI containing the SyncInfo that points
     * to the PA transmitted by the Broadcast Source.
     */
    UINT8 adv_SID;

    /** Broadcast ID of the Broadcast Source */
    UINT8 broadcast_ID[BASS_BC_ID_LEN];

    /** PA Sync Values, Refer \ref ga_bass_se_constants */
    UINT8 pa_sync;

    /**
     * SyncInfo field Interval parameter value,
     * Refer \ref ga_bass_se_constants
     */
    UINT16 pa_interval;

    /** Number of subgroups */
    UINT8 num_subgroups;

    /**
     * BIS_Sync parameter for the [ith] subgroup in the BIG,
     * Refer \ref ga_bass_se_constants
     */
    UINT32* bis_sync;

    /** Length of the Metadata parameter value for the [ith] subgroup in the BIG */
    UINT8* metadata_len;

    /** LTV-formatted Metadata for the [ith] subgroup in the BIG */
    UINT8* metadata;
}GA_BASS_BAS_CP_ADD_SRC;

/* BASS Broadcast Audio Scan CP Modify Source Operation Paramaters */
typedef struct _GA_BASS_BAS_CP_MODIFY_SRC
{
    /** Source_ID assigned by the server */
    UINT8 src_ID;

    /** PA Sync Values, Refer \ref ga_bass_se_constants */
    UINT8 pa_sync;

    /**
     * SyncInfo field Interval parameter value,
     * Refer \ref ga_bass_se_constants
     */
    UINT16 pa_interval;

    /** Number of subgroups */
    UINT8 num_subgroups;

    /** BIS_Sync parameter for the [ith] subgroup in the BIG, Refer \ref ga_bass_se_constants */
    UINT32* bis_sync;

    /** Length of the Metadata parameter value for the [ith] subgroup in the BIG */
    UINT8* metadata_len;

    /** LTV-formatted Metadata for the [ith] subgroup in the BIG */
    UINT8* metadata;
}GA_BASS_BAS_CP_MODIFY_SRC;

/* BASS Broadcast Audio Scan CP Set Broadcast Code Paramaters */
typedef struct _GA_BASS_BAS_CP_SET_BC_CODE
{
    /** Source_ID assigned by the server */
    UINT8 src_id;

    /** Broadcast_Code for the Source_ID assigned to a Broadcast Receive State */
    UINT8 br_code[BASS_BC_CODE_MAX_LEN];
}GA_BASS_BAS_CP_SET_BC_CODE;

/* BASS Broadcast Audio Scan CP Remove Source Operation Paramaters */
typedef struct _GA_BASS_BAS_CP_REMOVE_SRC
{
    /** Source_ID assigned by the server */
    UINT8 src_id;
}GA_BASS_BAS_CP_REMOVE_SRC;

/* Broadcast Receive State Source Information */
typedef struct _GA_BASS_BC_RX_STATE_SRC_INFO
{
    /** Source_ID assigned by the server */
    UINT8 id;

    /**
     * Advertiser Address Type of the Broadcast Source.
     * Refer \ref ga_bass_se_constants
     */
    UINT8 addr_type;

    /** Advertiser Address of the Broadcast Source */
    UINT8 addr[GA_BD_ADDR_SIZE];

    /**
     * Advertising_SID subfield of the ADI containing the SyncInfo that points
     * to the PA transmitted by the Broadcast Source.
     */
    UINT8 adv_sid;

    /** Broadcast ID of the Broadcast Source */
    UINT8 broadcast_ID[BASS_BC_ID_LEN];

    /** PA Sync State, Refer \ref ga_bass_se_constants */
    UINT8 pa_sync_state;

    /** Encryption State, Refer \ref ga_bass_se_constants */
    UINT8 big_encryption;

    /** BAD Code*/
    UINT8 bad_code[BASS_BC_CODE_MAX_LEN];

    /** Number of subgroups */
    UINT8 num_subgroups;

    /**
     * BIS_Sync parameter for the [ith] subgroup in the BIG,
     * Refer \ref ga_bass_constants
     */
    UINT32* bis_sync_state;

    /** Length of the Metadata parameter value for the [ith] subgroup in the BIG */
    UINT8* metadata_len;

    /** LTV-formatted Metadata for the [ith] subgroup in the BIG */
    UINT8* metadata;
}GA_BASS_BC_RX_STATE_SRC_INFO;

/* Broadcast Receive State Info */
typedef struct _GA_BASS_BC_RX_STATE
{
    /** Information about the Source in the BC Rx State */
    GA_BASS_BC_RX_STATE_SRC_INFO info;

    /** Length of the Information */
    UINT16 info_len;
}GA_BASS_BC_RX_STATE;

/** BASS Event */
typedef struct _BASS_EVT
{
    /** The handle indicates the Handle ID for BASS Session. */
    BASS_HANDLE            bass_handle;

    /** Registered BASS Broadcast Rx State Char instance */
    BASS_BC_RX_STATE_INST  rx_state_inst;

    /** Data corresponding to the Event */
    void                 * data;

    /** Datalen corresponding to the Event */
    UINT16                 len;
}BASS_EVT;

/** \} */
/** \} */

/**
 * \addtogroup ga_bass_ce_module_def
 * \{
 */

/**
 * \defgroup ga_bass_ce_structures Structures
 * \{
 * \brief This section lists the various data structures and typedefs for use
 * by BASS Client.
 */

/** Event that will be notified by the BASS CE module. */
typedef UINT8 BASS_CE_EVT_ID;

/** Types of write operations available */
typedef UINT8 BASS_CE_WRITE_TYPE;

/** BASS CE Write Request Format */
typedef struct _GA_BASS_CE_WRITE_REQ
{
    /** Write request Value */
    GA_BRR_CHR_VAL     req_data;

    /** Write Type representation */
    BASS_CE_WRITE_TYPE  write_type;

}GA_BASS_CE_WRITE_REQ;

/** \} */
/** \} */
/** \} */

/**
 * \defgroup ga_bass_cb Application Callback
 * \{
 * \brief This section describes the Notification Callback Interfaces offered
 * to the application by BASS.
 */

/**
 * \defgroup ga_bass_ce_module_cb BASS CE (Broadcast Audio Scan Service) Client Entity
 * \{
 * \brief This section describes the callback for BASS CE.
 */

/**
 * BASS CE Callback to be registered by the Application
 *
 * BASS calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param device     peer device info
 * \param evt_id     current event ID
 * \param evt_status status of the current event
 * \param evt_data   current event data
 */
typedef GA_RESULT (* BASS_CE_NTF_CB)
                  (
                      GA_ENDPOINT    * device,
                      BASS_CE_EVT_ID   evt_id,
                      UINT16           evt_status,
                      BASS_EVT       * evt_data
                  );

/** \} */
/** \} */

/**
 * \addtogroup ga_bass_defines
 * \{
 */

/**
 * \addtogroup ga_bass_se_module_def
 * \{
 */

/**
 * \defgroup ga_bass_se_structures Structures
 * \{
 * \brief This section lists the various data structures and typedefs for use
 * by BASS Server.
 */

/** Event that will be notified by the BASS SE module. */
typedef UINT8 BASS_SE_EVT_ID;

/** Context to identify the request and response */
typedef GA_BRR_CHR_CONTEXT GA_BASS_SE_RSP_CONTEXT;

/** \} */
/** \} */
/** \} */

/**
 * \addtogroup ga_bass_cb
 * \{
 */

/**
 * \defgroup ga_bass_se_module_cb BASS SE (Broadcast Audio Scan Service) Server Entity
 * \{
 * \brief This section describes the callback for BASS SE.
 */

/**
 * BASS SE Callback to be registered by the Application
 *
 * BASS calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param device     peer device info
 * \param chr        Response context
 * \param evt_id     current event ID
 * \param evt_status status of the current event
 * \param evt_data   current event data
 */
typedef GA_RESULT (* BASS_SE_NTF_CB)
                  (
                      GA_ENDPOINT            * device,
                      GA_BASS_SE_RSP_CONTEXT * chr,
                      BASS_SE_EVT_ID           evt_id,
                      UINT16                   evt_status,
                      BASS_EVT               * evt_data
                  );

/** \} */
/** \} */

/* --------------------------------------------- Macros */
/* --------------------------------------------- APIs */
/**
 * \defgroup ga_bass_api_defs API Definitions
 * \{
 * \brief This section describes the EtherMind BASS APIs.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup ga_bass_client_api_defs BASS CE (Broadcast Audio Scan Service) Client Entity
 * \{
 * \brief This section describes the Broadcast Audio Scan Service APIs
 * for Client.
 */

/**
 * \name BASS Client APIs - Init
 * \{
 * \brief This section describes the Broadcast Audio Scan Service Init APIs
 * for Client.
 */

/**
 *  \brief To Init the BASS Client Module
 *
 *  \par Description:
 *       This function enables to initialize the BASS Client Entity Module.
 *       This function enables to register a callback with BASS Module.
 *       This callback will be triggered whenever there are events generated
 *       either due to responses or notifications from peer.
 *
 *  \param [in] cb
 *         BASS CE Callback.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_bass_ce_init
          (
              /* IN */ BASS_CE_NTF_CB cb
          );

/** \} */

/**
 * \name BASS Client APIs - De-Init
 * \{
 * \brief This section describes the Broadcast Audio Scan Service De-Init APIs
 * for Client.
 */

/**
 *  \brief To Shutdown the BASS Client Module
 *
 *  \par Description:
 *       This function enables to Shutdown the BASS Client Entity Module.
 *       This function de-references the callback registered with GA BASS
 *       Module. No events generated at the BASS layer will be triggered
 *       post this function call.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_bass_ce_shutdown(void);

/** \} */

/**
 * \name BASS Client APIs - Setup
 * \{
 * \brief This section describes the Broadcast Audio Scan Service Setup APIs
 * for Client.
 */

/**
 *  \brief To Setup BASS Context
 *
 *  \par Description:
 *       This function enables to setup BASS context with given device.
 *       Internally, BASS service, char and descriptors for the BASS service
 *       instance will be discovered.
 *       bass_handle to be used by application for further calls to have
 *       transaction on this BASS Handle.
 *
 *  \param [in] device
 *         Peer Device with which the context needs to be set-up.
 *
 *  \param [in] srv_info
 *         Service Handle range to be provided for BASS Service Instance Search
 *
 *  \param [out] bass_handle
 *         BASS Handle.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure
 *          If GA_SUCCESS, BASS_CE_SETUP_CNF is notified on
 *          completion with status as success or failure.
 */
GA_RESULT GA_bass_ce_setup_context
          (
              /* IN */   GA_ENDPOINT * device,
              /* OUT */  BASS_HANDLE * bass_ce_handle
          );

/**
 * \cond EXCLUDE_PART
 */
#ifdef BASS_SUPPORT_CONFIG_SELECTION
/**
 *  \brief To update the configuration mask
 *
 *  \par Description:
 *       This function helps to update the configuration setting of which
 *       of the BASS characteristics of a peer needs configuration at setup.
 *
 *  \param [in] config
 *         Bitmask of the Characteristic IDs for configuration
 *
 *  \return GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 *  \ref
 */
GA_RESULT GA_bass_ce_update_ntf_configuration(UINT32 config);

#endif /* BASS_SUPPORT_CONFIG_SELECTION */

#ifdef BASS_SUPPORT_CONTEXT_MANAGE
/**
 *  \brief To manage the BASS characteristic handles of a device.
 *
 *  \par Description
 *       The routine enables the application to get/set the service and
 *       characteristic handle ranges from/to a device context.
 *
 *  \param [in] set
 *         \ref GA_TRUE for Context SET and \ref GA_FALSE for Context GET
 *
 *  \param [in] device
 *         Remote device endpoint
 *
 *  \param [inout] handle
 *         Context handle for the endpoint
 *
 *  \param [inout] range
 *         Service range array for BASS
 *
 *  \param [inout] info
 *         Array of characteristic handles for BASS
 *
 *  \param [inout] info_count
 *         Count of characteristics handles in above parameter
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
GA_RESULT GA_bass_ce_manage_context_info
          (
              /* IN     */ UINT8              set,
              /* IN     */ GA_ENDPOINT      * device,
              /* INOUT  */ BASS_HANDLE      * handle,
              /* INOUT  */ GA_BRR_SVC_RANGE ** range,
              /* INOUT  */ GA_CHAR_INFO     ** info,
              /* INOUT  */ UINT16           * info_count
          );

/**
 *  \brief To get the BASS characteristic handles of a device.
 *
 *  \par Description
 *       The routine enables the application to get the service and
 *       characteristic handle ranges from a device context.
 *
 *  \param [in] d
 *         Remote device endpoint
 *
 *  \param [inout] h
 *         Context handle for the endpoint
 *
 *  \param [inout] r
 *         Service range array for BASS
 *
 *  \param [inout] i
 *         Array of characteristic handles for BASS
 *
 *  \param [inout] c
 *         Count of characteristics handles in above parameter
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_bass_ce_get_context_info(d, h, r, i, c) \
        GA_bass_ce_manage_context_info(GA_FALSE, (d), (h), (r), (i), (c))

/**
 *  \brief To set the BASS characteristic handles of a device.
 *
 *  \par Description
 *       The routine enables the application to set the service and
 *       characteristic handle ranges to a device context.
 *
 *  \param [in] d
 *         Remote device endpoint
 *
 *  \param [inout] h
 *         Context handle for the endpoint
 *
 *  \param [inout] r
 *         Service range array for BASS
 *
 *  \param [inout] i
 *         Array of characteristic handles for BASS
 *
 *  \param [inout] c
 *         Count of characteristics handles in above parameter
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_bass_ce_set_context_info(d, h, r, i, c) \
        GA_bass_ce_manage_context_info(GA_TRUE, (d), (h), (r), (i), (c))

#endif /* BASS_SUPPORT_CONTEXT_MANAGE */

/**
 * \endcond
 */

/** \} */

/**
 * \name BASS Client APIs - Read
 * \{
 * \brief This section describes the Broadcast Audio Scan Service Read APIs
 * for Client.
 */

/**
 *  \brief To perform Read operation on a Char.
 *
 *  \par Description:
 *       This function triggers read request.
 *
 *  \param [in] bass_handle
 *         BASS Handle.
 *
 *  \param [in] bass_se_bc_rx_state_inst
 *         BASS Broadcast Receive State Char Instance Identifier.
 *
 *  \param [in] bass_ce_exp_evt
 *         Expected event ID to be generated once this operation is performed.
 *
 *  \param [in] char_uuid
 *         Char UUID on which read is requested.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer, BASS_CE_READ_XXX_CNF is
 *          notified, here XXX is the name of the char.
 */
GA_RESULT GA_bass_ce_read_request
          (
              /* IN */  BASS_HANDLE           bass_ce_handle,
              /* IN */  BASS_BC_RX_STATE_INST bass_ce_bc_rx_state_inst,
              /* IN */  BASS_CE_EVT_ID        bass_ce_exp_evt,
              /* IN */  UINT16                char_uuid
          );

/** \} */

/**
 * \name BASS Client APIs - Write
 * \{
 * \brief This section describes the Broadcast Audio Scan Service Write APIs
 * for Client.
 */

/**
 *  \brief To perform Write operation on a Char.
 *
 *  \par Description:
 *       This function triggers write request.
 *
 *  \param [in] bass_handle
 *         BASS Handle.
 *
 *  \param [in] bass_se_bc_rx_state_inst
 *         BASS Broadcast Receive State Char Instance Identifier.
 *
 *  \param [in] bass_ce_exp_evt
 *         Expected event ID to be generated once this operation is performed.
 *
 *  \param [in] char_uuid
 *         Char UUID on which write is requested.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure.
 *          When char allows write request and if it is a write request,
 *          once the response is received from peer,
 *          BASS_CE_XXX_CNF is notified, here XXX is the name of the char.
 */
GA_RESULT GA_bass_ce_write_request
          (
              /* IN */  BASS_HANDLE            bass_ce_handle,
              /* IN */  BASS_BC_RX_STATE_INST  bass_ce_bc_rx_state_inst,
              /* IN */  BASS_CE_EVT_ID         bass_ce_exp_evt,
              /* IN */  UINT16                 char_uuid,
              /* IN */  GA_BASS_CE_WRITE_REQ * req
          );

/** \} */

/**
 * \name BASS Client APIs - Read
 * \{
 * \brief This section describes the Broadcast Audio Scan Service Read APIs
 * for Client.
 */

/**
 *  \brief To Read Broadcast Receive State.
 *
 *  \par Description:
 *       This function reads Broadcast Receive State.
 *
 *  \param [in] bass_ce_handle
 *         BASS Handle.
 *
 *  \param [in] bass_ce_bc_rx_state_handle
 *         BASS Broadcast Receive State Char Handle.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          BASS_CE_READ_BC_RX_STATE_CNF is notified.
 */
#define GA_bass_ce_read_bc_rx_state(bass_ce_handle, bass_ce_bc_rx_state_handle) \
        GA_bass_ce_read_request((bass_ce_handle), (bass_ce_bc_rx_state_handle), (BASS_CE_READ_BC_RX_STATE_CNF), (GA_CHAR_BASS_BCAST_RECEIVE_STATE))

/** \} */

/**
 * \name BASS Client APIs - Write
 * \{
 * \brief This section describes the Broadcast Audio Scan Service Write APIs
 * for Client.
 */

/**
 *  \brief To Write BAS CP - Remote Scan Stopped/Started.
 *
 *  \par Description:
 *       This function writes BAS CP - Remote Scan Stopped/Started.
 *
 *  \param [in] bass_ce_handle
 *         BASS Handle.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure.
 *          When write response is received from peer,
 *          BASS_CE_BAS_CP_CNF is notified based on \GA_BASS_CE_WRITE_REQUEST.
 */
#define GA_bass_ce_inform_remote_scan_status(bass_ce_handle, req) \
        GA_bass_ce_write_request((bass_ce_handle), 0xFF, (BASS_CE_BAS_CP_CNF), (GA_CHAR_BASS_BCAST_AUDIO_SCAN_CP), (req))

/**
 *  \brief To Write BAS CP - Add Source.
 *
 *  \par Description:
 *       This function writes BAS CP - Add Source.
 *
 *  \param [in] bass_ce_handle
 *         BASS Handle.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure.
 *          When write response is received from peer,
 *          BASS_CE_BAS_CP_CNF is notified.
 */
#define GA_bass_ce_request_to_add_source(bass_ce_handle, req) \
        GA_bass_ce_write_request((bass_ce_handle), 0xFF, (BASS_CE_BAS_CP_CNF), (GA_CHAR_BASS_BCAST_AUDIO_SCAN_CP), (req))

/**
 *  \brief To Write BAS CP - Modify Source.
 *
 *  \par Description:
 *       This function writes BAS CP - Modify Source.
 *
 *  \param [in] bass_ce_handle
 *         BASS Handle.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure.
 *          When write response is received from peer,
 *          BASS_CE_BAS_CP_CNF is notified.
 */
#define GA_bass_ce_request_to_modify_source(bass_ce_handle, req) \
        GA_bass_ce_write_request((bass_ce_handle), 0xFF, (BASS_CE_BAS_CP_CNF), (GA_CHAR_BASS_BCAST_AUDIO_SCAN_CP), (req))

/**
 *  \brief To Write BAS CP - Set Broadcast Code.
 *
 *  \par Description:
 *       This function writes BAS CP - Set Broadcast Code.
 *
 *  \param [in] bass_ce_handle
 *         BASS Handle.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure.
 *          When write response is received from peer,
 *          BASS_CE_BAS_CP_CNF is notified.
 */
#define GA_bass_ce_set_bc_code(bass_ce_handle, req) \
        GA_bass_ce_write_request((bass_ce_handle), 0xFF, (BASS_CE_BAS_CP_CNF), (GA_CHAR_BASS_BCAST_AUDIO_SCAN_CP), (req))

/**
 *  \brief To Write BAS CP - Remove Source.
 *
 *  \par Description:
 *       This function writes BAS CP - Remove Source.
 *
 *  \param [in] bass_ce_handle
 *         BASS Handle.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure.
 *          When write response is received from peer,
 *          BASS_CE_BAS_CP_CNF is notified.
 */
#define GA_bass_ce_request_to_remove_source(bass_ce_handle, req) \
        GA_bass_ce_write_request((bass_ce_handle), 0xFF, (BASS_CE_BAS_CP_CNF), (GA_CHAR_BASS_BCAST_AUDIO_SCAN_CP), (req))

/** \} */

/**
 * \name BASS Client APIs - Release
 * \{
 * \brief This section describes the Broadcast Audio Scan Service Release APIs
 * for Client.
 */

/**
 *  \brief To release/free BASS Context
 *
 *  \par Description:
 *       This function enables to release/free BASS Context with the peer device.
 *
 *  \param [in] bass_ce_handle BASS Handle.
 *  \param [in] free Release/Free the context.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref BASS_CE_RELEASE_CNF is notified on
 *          completion with status as success or failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
GA_RESULT GA_bass_ce_terminate
          (
              /* IN */  BASS_HANDLE  bass_ce_handle,
              /* IN */  UCHAR  free
          );

/**
 *  \brief To release BASS Context
 *
 *  \par Description:
 *       This function enables to release BASS Context with the peer device.
 *
 *  \param [in] bass_ce_handle
 *         BASS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref BASS_CE_RELEASE_CNF is notified on
 *          completion with status as success or failure.
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_bass_ce_release(bass_ce_handle) \
        GA_bass_ce_terminate((bass_ce_handle), GA_FALSE)

/**
 *  \brief To close BASS Context
 *
 *  \par Description:
 *       This function enables to close BASS Context with the peer device.
 *
 *  \param [in] bass_ce_handle
 *         BASS Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *
 *  \sa ga_mcp_and_otp_error_code
 */
#define GA_bass_ce_close(bass_ce_handle) \
        GA_bass_ce_terminate((bass_ce_handle), GA_TRUE)

/** \} */

/** \} */

/**
 * \defgroup ga_bass_server_api_defs BASS SE (Broadcast Audio Scan Service) Server Entity
 * \{
 * \brief This section describes the Broadcast Audio Scan Service APIs
 * for Server.
 */

/**
 * \name BASS Server APIs - Init
 * \{
 * \brief This section describes the Broadcast Audio Scan Service Init APIs
 * for Server.
 */

/**
 *  \brief To Init the BASS Server Module.
 *
 *  \par Description:
 *       This function enables to initialize the GA BASS - Server Module with
 *       the GA Bearer Module. This enables to register a callback with the
 *       GA BASS SE module, this callback will be triggered when there are
 *       events generated either due to responses or notifications from peer.
 *       By default, a single instance of BR Receive State is added and the
 *       instance is 0U.
 *
 *  \param [in] cb
 *         BASS SE Callback.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_bass_se_init(BASS_SE_NTF_CB cb);

/** \} */

/**
 * \name BASS Server APIs - De-Init
 * \{
 * \brief This section describes the Broadcast Audio Scan Service De-Init APIs
 * for Server.
 */

/**
 *  \brief To Shutdown the BASS Server Module.
 *
 *  \par Description:
 *       This function enables to Shutdown the GA BASS Server Module and
 *       de-registers the callback.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_bass_se_shutdown(void);

/** \} */

/**
 * \name BASS Server APIs - Setup
 * \{
 * \brief This section describes the Broadcast Audio Scan Service Init APIs
 * for Server.
 */

/**
 *  \brief To add an instance of Broadcast Receive State Char.
 *
 *  \par Description:
 *       This function adds the instance to the DB. Maximum
 *       number of instances of Broadcast Receive State Char that is supported
 *       is defined by BASS_SE_MAX_BC_RX_STATE_ENTITIES. The
 *       bass_se_bc_rx_state_inst must be conserved by the App to be used in
 *       all Interface calls that involves transaction over Broadcast Receive
 *       State Char Instance.
 *       Note: By default, 0U is indicated for the default char instance added
 *       during GA_bass_se_init().
 *
 *  \param [out] bass_se_bc_rx_state_inst
 *         BASS Broadcast Receive State Char Instance Identifier.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure.
 *          If the max num of instances supported is reached, GA_FAILURE occurs,
 */
GA_RESULT GA_bass_se_add_bc_rx_state_inst
          (
              BASS_BC_RX_STATE_INST * bass_se_bc_rx_state_inst
          );

/** \} */

/**
 * \name BASS Server APIs - Notify
 * \{
 * \brief This section describes the Broadcast Audio Scan Service Notify APIs
 * for Server.
 */

/**
 *  \brief To send Notifications to the peer BASS CE.
 *
 *  \par Description:
 *       This function send notifications to the peer device based on the
 *       address and if configuration is enabled.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected BASS sessions will be passed.
 *         Else, to the corresponsing address passed.
 *
 *  \param [in] bass_se_bc_rx_state_inst
 *         BASS Session Instance Identifier - Broadcast Rx State.
 *
 *  \param [in] char_uuid
 *         Char UUID reference.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification.
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_bass_se_send_ntf
          (
              /* IN */ GA_ENDPOINT          * ga_dev,
              /* IN */ BASS_BC_RX_STATE_INST  bass_se_bc_rx_state_inst,
              /* IN */ UINT16                 char_uuid,
              /* IN */ void                 * char_info,
              /* IN */ UINT8                  char_info_len
          );

/**
 *  \brief To send Notifications - Broadcast Receive State.
 *
 *  \par Description:
 *       This function sends notifications if configured for Broadcast Receive State.
 *
 *  \param [in] ga_dev
 *         If NULL, Ntf to all connected BASS sessions will be passed.
 *         Else, to the corresponsing address passed.
 *
 *  \param [in] bass_se_bc_rx_state_inst
 *         BASS Broadcast Receive State Char Instance Identifier.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification.
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure.
 */
#define GA_bass_se_notify_bc_rx_state(ga_dev, bass_se_bc_rx_state_inst, char_info, char_info_len) \
        GA_bass_se_send_ntf((ga_dev), (bass_se_bc_rx_state_inst), (GA_CHAR_BASS_BCAST_RECEIVE_STATE), (char_info), (char_info_len))

/** \} */

/**
 * \name BASS Server APIs - Read/Write
 * \{
 * \brief This section describes the Broadcast Audio Scan Service Read/Write APIs
 * for Server.
 */

/**
 *  \brief To trigger sending of response for read or write request.
 *
 *  \par Description:
 *       This function triggers sending of response for read/write.
 *
 *  \param [in] dev
 *         Remote device address
 *
 *  \param [in] ctx
 *         Context to be passed back as sent.
 *
 *  \param [in] bass_se_evt_id
 *         Event ID to indicate the request.
 *
 *  \param [in] rsp_status
 *         Response Status for read/write request.
 *
 *  \param [in] rsp_info
 *         Response packet to be sent - Valid for Read request event type only.
 *         NULL - For Write request.
 *
 *  \param [in] rsp_info_len
 *         Response Packet length - Valid for Read request event type only.
 *         0 - For Write request.
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_bass_se_send_rsp
          (
              /* IN */ GA_ENDPOINT            * device,
              /* IN */ GA_BASS_SE_RSP_CONTEXT * ctx,
              /* IN */ BASS_SE_EVT_ID           bass_se_evt_id,
              /* IN */ GA_RESULT                rsp_status,
              /* IN */ void                   * rsp_info,
              /* IN */ UINT16                   rsp_info_len
          );

/** \} */
/** \} */

#ifdef __cplusplus
};
#endif

/** \} */
/** \} */

#endif /* _H_GA_BASS_API_ */

