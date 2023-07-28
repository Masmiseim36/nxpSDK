
/**
 *  \file GA_hap_api.h
 *
 *  \brief This file defines the GA Hearing Access Profile(HAP)
 *  Client and Server Entity(SE) Interface - includes Data Structures and
 *  Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_HAP_API_
#define _H_GA_HAP_API_

/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"
#include "GA_bearer_api.h"
#include "GA_property_config.h"

/**
 * \addtogroup bt_ga_appl_profiles Application Profiles
 * \ingroup bt_ga_profiles
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * GA Application Profiles.
 */

/**
 * \defgroup hap_module Hearing Access Profile (HAP)
 * \ingroup bt_ga_appl_profiles
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * GA Hap (HAP) module to the Application and other upper
 * layers of the stack.
 */

/* --------------------------------------------- Global Definitions */
/**
 * \defgroup hap_defines Defines
 * \{
 * \brief This section describes the various Defines in EtherMind GA
 * HAP Layer.
 */

/**
 * \defgroup hap_error_codes Error Codes
 * \{
 * \brief This section lists the Error Codes that are provided as part of
 * return of functionalities of HAP.
 */

/**
 * \name HAP Application Error Codes
 * \{
 */

/** An invalid opcode has been used in a control point operation. */
#define HAP_APPL_ERR_CODE_INVALID_OPCODE            0x80
/**
 * The client executed the Write Preset Name operation with an Index parameter
 * corresponding to a read-only preset record.
 */
#define HAP_APPL_ERR_CODE_WRITE_NAME_NT_ALLOWED     0x81
/**
 * The client executed a Synchronized Locally Hearing Aid Preset Control Point
 * operation to a server that does not support Preset Synchronization.
 */
#define HAP_APPL_ERR_CODE_PRESET_SYNC_NT_SUPP       0x82
/**
 * The requested preset operation cannot be executed at this time.
 * This could happen when the requested preset is incompatible with the state
 * of operation of the hearing aid or the preset record is marked as
 * unavailable.
 */
#define HAP_APPL_ERR_CODE_PRESET_OP_NOT_POSSIBLE    0x83
/**
 * The client requested to write a valid opcode but with parameters
 * of invalid length.
 */
#define HAP_APPL_ERR_CODE_INVALID_PARAM_LENGTH      0x84

/** \} */

/** \} */

/**
 * \defgroup hap_constants Constants
 * \{
 * \brief This section describes the various Constants in EtherMind GA
 * HAP Layer.
 */

/**
 * \name HAP Properties - Preset Writable
 * Name of Preset Writable - Yes/No \n
 * Values(BitMap)
 * \{
 */

/** Bit0: The name of the preset cannot be written by the client */
#define HAP_PROP_NAME_OF_PRESET_WRITABLE_NO         (0x00U << 0U)
/** Bit0: The name of the preset can be written by the client */
#define HAP_PROP_NAME_OF_PRESET_WRITABLE_YES        (0x01U << 0U)

/** \} */

/**
 * \name HAP Properties - Preset Available
 * Preset Available - Yes/No \n
 * Values(BitMap)
 * \{
 */

/** Bit1: The preset is unavailable */
#define HAP_PROP_IS_AVAILABLE_NO                    (0x00U << 1U)
/** Bit1: The preset is available */
#define HAP_PROP_IS_AVAILABLE_YES                   (0x01U << 1U)

/** \} */

/**
 * \name HAP Features - Hearing Aid Type
 * Values(BitMap) - Bit 0 and Bit 1
 * \{
 */

/** Binaural Hearing Aid */
#define HAP_HEARING_AID_TYPE_BINAURAL               (0x00U << 0U)
/** Monaural Hearing Aid */
#define HAP_HEARING_AID_TYPE_MONAURAL               (0x01U << 0U)
/** Banded Hearing Aid */
#define HAP_HEARING_AID_TYPE_BANDED                 (0x01U << 1U)

/** \} */

/**
 * \name HAP Features - Preset Synchronization Support
 * Values(BitMap) - Bit 2
 * \{
 */

/** Preset Synchronization is not supported */
#define HAP_HEARING_AID_PRESET_SYNC_SUPP_NO         (0x00U << 2U)
/** Preset Synchronization is supported */
#define HAP_HEARING_AID_PRESET_SYNC_SUPP_YES        (0x01U << 2U)

/** \} */

/**
 * \name HAP Features - Independent Presets
 * Values(BitMap) - Bit 3
 * \{
 */

/**
 * The list of preset records on this server is identical to the list of
 * preset records in the other server of the coordinated set
 */
#define HAP_HEARING_AID_INDEPENDENT_PRESETS_SAME    (0x00U << 3U)
/**
 * The list of preset records on this server may be different from the list
 * of preset records in the other server of the coordinated set
 */
#define HAP_HEARING_AID_INDEPENDENT_PRESETS_DIFF    (0x01U << 3U)

/** \} */

/**
 * \name HAP Features - Dynamic Presets
 * Values(BitMap) - Bit 4
 * \{
 */

/** The list of preset records does not change */
#define HAP_HEARING_AID_DYNAMIC_PRESETS_DOES_NOT_CHANGE    (0x00U << 4U)
/** The list of preset records may change */
#define HAP_HEARING_AID_DYNAMIC_PRESETS_MAY_CHANGE         (0x01U << 4U)

/** \} */

/**
 * \name HAP Features - Writable Presets Support
 * Values(BitMap) - Bit 5
 * \{
 */

/** The server does not support writable preset records */
#define HAP_HEARING_AID_WRITABLE_PRESETS_NO_SUPP           (0x00U << 5U)
/** The server supports writable preset records */
#define HAP_HEARING_AID_WRITABLE_PRESETS_SUPP              (0x01U << 5U)

/** \} */

/**
 * \name Hearing Aid Preset Control Point operation Opcode
 * \{
 */

/** Read Preset by Index Request */
#define HAP_CP_OPCODE_READ_PRESETS_REQ                     0x01U
/** Read Preset Response */
#define HAP_CP_OPCODE_READ_PRESETS_RSP                     0x02U
/** Preset Changed */
#define HAP_CP_OPCODE_PRESET_CHANGED                       0x03U
/** Write Preset Name  */
#define HAP_CP_OPCODE_WRITE_PRESET_NAME                    0x04U
/** Set Active Preset */
#define HAP_CP_OPCODE_SET_ACTIVE_PRESET                    0x05U
/** Set Next Preset */
#define HAP_CP_OPCODE_SET_NEXT_PRESET                      0x06U
/** Set Previous Preset */
#define HAP_CP_OPCODE_SET_PREV_PRESET                      0x07U
/** Set Active Preset - Synchronized Locally */
#define HAP_CP_OPCODE_SET_ACTIVE_PRESET_SYNC_LOCALLY       0x08U
/** Set Next Preset - Synchronized Locally */
#define HAP_CP_OPCODE_SET_NEXT_PRESET_SYNC_LOCALLY         0x09U
/** Set Previous Preset - Synchronized Locally */
#define HAP_CP_OPCODE_SET_PREV_PRESET_SYNC_LOCALLY         0x0AU

/** \} */

/**
 * \name CP Request operation isLast Field
 * Read All Presets Request operation \n
 * Preset Changed
 * \{
 */

/**
 * Indicates all preset records have been sent and the procedure can be
 * considered concluded.
 */
#define HAP_CP_IS_LAST_NO                                  0x00U
/**
 * Indicates preset records send is still ongoing sent and the
 * procedure is not concluded.
 */
#define HAP_CP_IS_LAST_YES                                 0x01U

/** \} */

/**
 * \name Read Preset by Index Request operation Index Field
 * \{
 */

/** Read Preset by Index Request operation Index - Min */
#define HAP_READ_PRESET_BY_INDEX_REQ_INDEX_MIN             0x01U
/** Read Preset by Index Request operation Index - Max */
#define HAP_READ_PRESET_BY_INDEX_REQ_INDEX_MAX             0xFFU

/** \} */

/**
 * \name ChangeId
 * \{
 */

/** Preset Changed ChangeId - Generic Update */
#define HAP_CP_CHANGE_ID_GENERIC_UPDATE                      0x00U
/** Preset Changed ChangeId - Preset Record Deleted */
#define HAP_CP_CHANGE_ID_PRESET_RECORD_DELETED               0x01U
/** Preset Changed ChangeId - Preset Record Available */
#define HAP_CP_CHANGE_ID_PRESET_RECORD_AVAILABLE             0x02U
/** Preset Changed ChangeId - Preset Record Unavailable */
#define HAP_CP_CHANGE_ID_PRESET_RECORD_UNAVAILABLE           0x03U

/** \} */

/**
 * \name Preset Changed ChangeId
 * \{
 */

/** Preset Changed ChangeId - Generic Update */
#define HAP_CP_CHANGE_ID_GENERIC_UPDATE                      0x00U
/** Preset Changed ChangeId - Preset Record Deleted */
#define HAP_CP_CHANGE_ID_PRESET_RECORD_DELETED               0x01U
/** Preset Changed ChangeId - Preset Record Available */
#define HAP_CP_CHANGE_ID_PRESET_RECORD_AVAILABLE             0x02U
/** Preset Changed ChangeId - Preset Record Unavailable */
#define HAP_CP_CHANGE_ID_PRESET_RECORD_UNAVAILABLE           0x03U

/** \} */

/**
 * \name HAP Roles
 * \{
 */

/** HAP Role - Hearing Aid (HA) */
#define HAP_ROLE_HA    (0x01 << 0U)
/** HAP Role - Hearing Aid Unicast Client (HAUC) */
#define HAP_ROLE_HAUC  (0x01 << 1U)
/** HAP Role - Hearing Aid Remote Controller (HARC) */
#define HAP_ROLE_HARC  (0x01 << 2U)
/** HAP Role - Immediate Alert Client (IAC) */
#define HAP_ROLE_IAC   (0x01 << 3U)

/** \} */

#ifdef HAP_CLIENT

/**
 * \name HAP Client Constants - General Macros
 * \{
 * \brief Initialization and other General Macros offered by the module.
 */

/**
 * Total number of Characteristic IDs,
 * \ref ga_has_char_prop
 */
#define HAP_CHAR_ID_COUNT               0x03U

/** \} */

/**
 * \name HAP Client Constants - HAP Char ID
 * \{
 * This section lists the Characteristic ID references.
 */

/**
 * HAS Char ID: Hearing Aid Features,
 * \ref GA_CHAR_PROP_HA_FEATURES
 */
#define HAP_CHAR_ID_HA_FEATURES                        0U
/**
 * HAS Char ID: Hearing Aid Preset Control Point,
 * \ref GA_CHAR_PROP_HA_PRESET_CP
 */
#define HAP_CHAR_ID_HA_PRESET_CP                       1U
/**
 * HAS Char ID: Active Preset Index,
 * \ref GA_CHAR_PROP_ACTIVE_PRESET_INDEX
 */
#define HAP_CHAR_ID_ACTIVE_PRESET_INDEX                2U

/** \} */

/** \} */

#endif /* HAP_CLIENT */

/**
 * \defgroup hap_events Events
 * \{
 * \brief This section lists the Asynchronous and Synchronous Events
 * notified to Application by EtherMind GA HAP Layer.
 */

/**
 * \defgroup hap_client_events HAP Client Events
 * \{
 * This section describes the Hearing Access Profile Events
 * for Client.
 */

/**
 * \name HAP Client - Setup Events
 * \{
 */

/**
 * \brief This event indicates the setup of a HAS Session.
 * A handle to the HAS Session is derived and provided to the
 * application, this Handle to be further used during all HAP related
 * requests. This involves discovery of service, char
 * and descriptors. Once done, the configuration for Notifications is
 * also enabled.
 * Callback is triggered with the following values as parameters in the
 * \ref HAP_NTF_CB callback
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ctx  \ref Context NULL \n
 * \param [in] evt_id  \ref HAP_SETUP_CNF
 * \param [in] evt_status  \ref GA_SUCCESS Setup Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] evt_data  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - data: NULL \n
 *                           - datalen: 0 \n
 *                           .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define HAP_SETUP_CNF                            0x01U

/** \} */

/**
 * \name HAP Client - Release Events
 * \{
 */

/**
 * \brief This event is notified when a HAS Service context is released.
 * Configuration of Notifications for char are disabled and followed by release
 * of HAS context.
 * Callback is triggered with the following values as parameters in the
 * \ref HAP_NTF_CB callback
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ctx  \ref Context NULL \n
 * \param [in] evt_id  \ref HAP_RELEASE_CNF
 * \param [in] evt_status  \ref GA_SUCCESS Setup Complete \n
 *                        \ref GA_FAILURE Others \n
 * \param [in] evt_data  If status is \ref GA_SUCCESS or
 *                         \ref GA_FAILURE, \n
 *                           - data: NULL \n
 *                           - datalen: 0 \n
 *                           .
 *
 * \return \ref GA_SUCCESS (always)
 */
#define HAP_RELEASE_CNF                          0x02U

/** \} */

/**
 * \name HAP Client - Read Events
 * \{
 */

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref HAP_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ctx  \ref Context NULL \n
 * \param [in] evt_id  \ref HAP_HA_GET_FEATURES_CNF
 * \param [in] evt_status  \ref GA_SUCCESS Read Response received \n
 * \param [in] evt_data  Pointer to \ref HAP_EVT
 *                           data: Hearing Aid Features(Bit field)
 *                                              represents a \ref UINT8 \n
 *                           datalen: 1 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define HAP_HA_GET_FEATURES_CNF                  0x03U

/**
 * \brief This event is notified when a read response is received for a char
 * from peer device with the following values as parameters in the
 * \ref HAP_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ctx  \ref Context NULL \n
 * \param [in] evt_id  \ref HAP_HA_GET_ACTIVE_PRESET_INDEX_CNF
 * \param [in] evt_status  \ref GA_SUCCESS Read Response received \n
 * \param [in] evt_data  Pointer to \ref HAP_EVT
 *                           data: Active Preset Index
 *                                            represents a \ref UINT8 \n
 *                           datalen: 1 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define HAP_HA_GET_ACTIVE_PRESET_INDEX_CNF       0x04U

/** \} */

/**
 * \name HAP Client - Write Events
 * \{
 */

/**
 * \brief This event is notified when a write response is received for a char
 * from peer device with the following values as parameters in the
 * \ref HAP_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ctx  \ref Context NULL \n
 * \param [in] evt_id  \ref HAP_HA_PRESET_CP_CNF
 * \param [in] evt_status  \ref GA_SUCCESS Write Response received \n
 * \param [in] evt_data  Pointer to \ref HAP_EVT
 *                           data: NULL \n
 *                           datalen: 0 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define HAP_HA_PRESET_CP_CNF                     0x05U

/** \} */

/**
 * \name HAP Client - Notification Events
 * \{
 */

/**
 * \brief This event is notified when a notification is received for Hearing
 * Aid Features characteristic from peer device with the following values as
 * parameters in the \ref HAP_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ctx  \ref Context NULL \n
 * \param [in] evt_id  \ref HAP_HA_FEATURES_NTF
 * \param [in] evt_status  \ref GA_SUCCESS Notification received \n
 * \param [in] evt_data  Pointer to \ref HAP_EVT
 *                           data: Hearing Aid Features(Bit field)
 *                                              represents a \ref UINT8 \n
 *                           datalen: 1 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define HAP_HA_FEATURES_NTF                      0x06U

/**
 * \brief This event is notified when a notification is received for Hearing
 * Aid Features characteristic from peer device with the following values as
 * parameters in the \ref HAP_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ctx  \ref Context NULL \n
 * \param [in] evt_id  \ref HAP_HA_PRESET_CP_NTF
 * \param [in] evt_status  \ref GA_SUCCESS Notification received \n
 * \param [in] evt_data  Pointer to \ref HAP_EVT
 *                           data: Opcode ( \ref UINT8 )
 *                                 \ref hap_constants
 *                                 Varies depending upon Opcode
 *                                  represents a \ref UINT8 \n
 *                           datalen: Varies \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define HAP_HA_PRESET_CP_NTF                     0x07U

/**
 * \brief This event is notified when a notification is received for Hearing
 * Aid Features characteristic from peer device with the following values as
 * parameters in the \ref HAP_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ctx  \ref Context NULL \n
 * \param [in] evt_id  \ref HAP_HA_GET_ACTIVE_PRESET_INDEX_NTF
 * \param [in] evt_status  \ref GA_SUCCESS Notification received \n
 * \param [in] evt_data  Pointer to \ref HAP_EVT
 *                           data: Active Preset Index
 *                                            represents a \ref UINT8 \n
 *                           datalen: 1 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define HAP_HA_GET_ACTIVE_PRESET_INDEX_NTF       0x08U

/** \} */

/** \} */

/**
 * \defgroup hap_server_events HAP Server Events
 * \{
 * This section describes the Hearing Access Profile Events
 * for Server.
 */

/**
 * \name HAP Server - Read Events
 * \{
 */

/**
 * \brief This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref HAP_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ctx  \ref Context \n
 *                      Use it to send response via \ref GA_hap_send_rsp()
 * \param [in] evt_id  \ref HAP_HA_GET_FEATURES_IND
 * \param [in] evt_status  \ref GA_SUCCESS Read Request received \n
 * \param [in] evt_data  Pointer to \ref HAP_EVT
 *                           data: NULL \n
 *                           datalen: 0 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define HAP_HA_GET_FEATURES_IND                  0x09U

/**
 * \brief This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref HAP_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ctx  \ref Context \n
 *                      Use it to send response via \ref GA_hap_send_rsp()
 * \param [in] evt_id  \ref HAP_HA_GET_ACTIVE_PRESET_INDEX_IND
 * \param [in] evt_status  \ref GA_SUCCESS Read Request received \n
 * \param [in] evt_data  Pointer to \ref HAP_EVT
 *                           data: NULL \n
 *                           datalen: 0 \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define HAP_HA_GET_ACTIVE_PRESET_INDEX_IND       0x0AU

/** \} */

/**
 * \name HAP Server - Write Events
 * \{
 */

/**
 * \brief This event is notified when a read request is received for a char from
 * peer device with the following values as parameters in the
 * \ref HAP_NTF_CB callback.
 *
 * \param [in] device  Pointer to peer device handle \ref
 * \param [in] ctx  \ref Context \n
 *                      Use it to send response via \ref GA_hap_send_rsp()
 * \param [in] evt_id  \ref HAP_HA_PRESET_CP_IND
 * \param [in] evt_status  \ref GA_SUCCESS Read Request received \n
 * \param [in] evt_data  Pointer to \ref HAP_EVT
 *                           data: Opcode ( \ref UINT8 )
 *                                 \ref hap_constants
 *                                 Varies depending upon Opcode
 *                                  represents a \ref UINT8 \n
 *                           datalen: Varies \n
 *
 * \return \ref GA_SUCCESS (always)
 */
#define HAP_HA_PRESET_CP_IND                     0x0BU

/** \} */

/** \} */

/** \} */

/**
 * \defgroup hap_macros Macros
 * \{
 * \brief Initialization and other Macros offered by the module.
 */

/**
 * \name HAP - Char Config
 * \{
 */

/** Derive the characteristic config mask for the Char ID */
#define HAS_CHAR_CONFIG(id)                             (1U << (id))

/** \} */

/** \} */

/* --------------------------------------------- Data types /Structures */

/**
 * \defgroup hap_structures Structures
 * \{
 * \brief This section describes the various Data-Types and Structures in
 * EtherMind GA HAP Layer.
 */

/**
 * The field in the handle indicates the Handle ID.
 * The application needs to provide this as a input param for all the calls
 * to have transaction on the required HAP handle.
 */
typedef UINT8 HAP_HANDLE;

/** Event that will be notified by the HAP module. */
typedef UINT8 HAP_EVT_ID;

/** Roles defined for HAP */
typedef UINT8 HAP_ROLE;

/** HAP Features */
typedef UINT8 HAP_FEATURES;

/** Char Value: Use during Notification/Read Rsp/Write Rsp */
typedef GA_BRR_CHR_VAL HAP_CHR_VAL;

/** HAP Event */
typedef struct _HAP_EVT_
{
    /** HAP Handle on which HAP event is receievd */
    HAP_HANDLE   hap_handle;

    /** Data associated with the event */
    void       * data;

    /** Length of the event data */
    UINT16       len;

}HAP_EVT;

#ifdef HAP_CLIENT

/** Write Type: Request/Command */
typedef GA_BRR_WRITE_TYPE HAP_CE_WRITE_TYPE;

/** HAP Client Write Request */
typedef struct _HAP_CE_WRITE_REQ_
{
    /** Write Type */
    HAP_CE_WRITE_TYPE  type;

    /** Write Value and Length */
    HAP_CHR_VAL        value;

}HAP_CE_WRITE_REQ;

#endif /* HAP_CLIENT */

#ifdef HAP_SERVER

/** HAP Char Context */
typedef GA_BRR_CHR_CONTEXT GA_HAP_SE_RSP_CONTEXT;

#endif /* HAP_SERVER */

/** \} */

/* --------------------------------------------- Macros */

/**
 * \defgroup hap_macros Macros
 * \{
 * \brief This section describes the various Utility and functional Macros in
 * EtherMind GA HAP Layer.
 */

/** \} */

/** \} */

/* --------------------------------------------- Callback Interfaces */
/**
 * \defgroup hap_cb Application Callback
 * \{
 * \brief This section describes the Notification Callback Interfaces offered
 * to the application by EtherMind GA HAP Layer.
 */

/**
 * HAP Callback to be registered by the Application
 *
 * HAP Layer calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param device     peer device info
 * \param ctx        Context is NULL, Ignore for Client Callback.
 *                   Use it to send response via \ref GA_hap_send_rsp() in
 *                   Server Callback.
 * \param evt_id     current event ID
 * \param evt_status status of the current event
 * \param evt_data   current event data
 */
typedef GA_RESULT (* HAP_NTF_CB)
                  (
                      GA_ENDPOINT * device,
                      void        * ctx,
                      HAP_EVT_ID    evt_id,
                      UINT16        evt_status,
                      HAP_EVT     * evt_data
                  );

/** \} */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hap_api_defs API Definitions
 * \{
 * \brief This section describes the various APIs exposed by
 * EtherMind GA HAP Layer to the Application and other upper
 * layers of the stack.
 */

/**
 * \defgroup hap_common_api HAP Common APIs
 * \{
 * \brief This section describes the Hearing Access Profile APIs
 * common for Client and Server.
 */

/**
 * \name HAP Init
 * This section describes the Hearing Access Profile Init APIs
 * \{
 */

/**
 *  \brief To Init the HAP Client and Server Module
 *
 *  \par Description:
 *       This function enables to initialize the HAP Client and Server Entity Module.
 *       This function enables to register a callback with HAP Module.
 *       This callback will be triggered whenever there are events generated
 *       either due to requests/responses or notifications from peer.
 *
 *  \param [in] cb
 *         HAP Callback.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_hap_init
          (
              /* IN */ HAP_NTF_CB cb
          );

/** \} */

/**
 * \name HAP De-Init
 * This section describes the Hearing Access Profile De-Init APIs
 * \{
 */

/**
 *  \brief To Shutdown the HAP Client and Server Module
 *
 *  \par Description:
 *       This function enables to Shutdown the HAP Client and Server Entity
 *       module.
 *       This function de-references the callback registered with GA HAP
 *       Module. No events generated at the HAP layer will be triggered
 *       post this function call.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_hap_shutdown(void);

/** \} */

/**
 * \name HAP Init
 * This section describes the Hearing Access Profile Init APIs
 * \{
 */
/**
 *  \brief To register HAP Role.
 *
 *  \par Description:
 *       This function registers HAP Service and Char based on the HAP role
 *       registered.
 *
 *  \param [in] role
 *         Refer \ref HAP_ROLE.
 *
 *  \param [in] features
 *         Refer \ref HAP_FEATURES. Valid only when role is \ref HAP_ROLE_HA.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 */
GA_RESULT GA_hap_register_role
          (
              /* IN */  HAP_ROLE     role,
              /* IN */  HAP_FEATURES features
          );
/** \} */

/** \} */

#ifdef HAP_CLIENT

/**
 * \defgroup hap_client_api HAP Client APIs
 * \{
 * This section describes the Hearing Access Profile APIs
 * for Client.
 */

/**
 * \name HAP Client - Setup
 * \{
 */

/**
 * \cond EXCLUDE_PART
 */
#ifdef HAP_SUPPORT_CONFIG_SELECTION
/**
 *  \brief To update the configuration mask
 *
 *  \par Description:
 *       This function helps to update the configuration setting of which
 *       of the HAS characteristics of a peer needs configuration at setup.
 *
 *  \param [in] config
 *         Bitmask of the Characteristic IDs for configuration
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 *  \ref
 */
GA_RESULT GA_hap_update_ntf_configuration(UINT32 config);

#endif /* HAP_SUPPORT_CONFIG_SELECTION */

#ifdef HAP_SUPPORT_CONTEXT_MANAGE
/**
 *  \brief To manage the GA HAP characteristic handles of a device.
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
 *         Service range array for HAS
 *
 *  \param [inout] info
 *         Array of characteristic handles for HAS
 *
 *  \param [inout] info_count
 *         Count of characteristics handles in above parameter
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
GA_RESULT GA_hap_manage_context_info
          (
              /* IN     */ UINT8              set,
              /* IN     */ GA_ENDPOINT      * device,
              /* INOUT  */ HAP_HANDLE       * handle,
              /* INOUT  */ GA_BRR_SVC_RANGE ** range,
              /* INOUT  */ GA_CHAR_INFO     ** info,
              /* INOUT  */ UINT16           * info_count
          );

/**
 *  \brief To get the GA HAP characteristic handles of a device.
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
 *         Service range array for HAS
 *
 *  \param [inout] i
 *         Array of characteristic handles for HAS
 *
 *  \param [inout] c
 *         Count of characteristics handles in above parameter
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_hap_get_context_info(d, h, r, i, c) \
        GA_hap_manage_context_info(GA_FALSE, (d), (h), (r), (i), (c))

/**
 *  \brief To set the GA HAP characteristic handles of a device.
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
 *         Service range array for HAS
 *
 *  \param [inout] i
 *         Array of characteristic handles for HAS
 *
 *  \param [inout] c
 *         Count of characteristics handles in above parameter
 *
 *  \return \ref GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
#define GA_hap_set_context_info(d, h, r, i, c) \
        GA_hap_manage_context_info(GA_TRUE, (d), (h), (r), (i), (c))

#endif /* HAP_SUPPORT_CONTEXT_MANAGE */

/**
 * \endcond
 */

/**
 *  \brief To Setup Hearing Aid Service(HAS) Context
 *
 *  \par Description:
 *       This function enables to setup HAS context with given device.
 *       Internally, HAP service, char and descriptors for the service
 *       is discovered. The configuration of Notifications and Indications to
 *       the chars that expose the properties are internally performed.
 *       Also, HAP Features are read from the peer device.
 *
 *  \param [in] device
 *         Peer Device with which the context needs to be set-up.
 *
 *  \param [out] hap_handle
 *         HAP Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref HAP_SETUP_CNF is notified on
 *          completion with status as success or failure.
 */
GA_RESULT GA_hap_setup_context
          (
              /* IN */   GA_ENDPOINT * device,
              /* OUT */  HAP_HANDLE  * hap_handle
          );

/** \} */

/**
 * \name HAP Client - Release
 * \{
 */

/**
 *  \brief Release or free the given HAP context.
 *
 *  \par Description
 *  When 'free' is set to \ref GA_FALSE, this routine initiates the release
 *  procedure for the context. Once release is done, the context is freed up
 *  and the setup must be freshly done by calling
 *  \ref GA_hap_setup_context if required for
 *  the same device again.
 *
 *  If the 'free' parameter is set to \ref GA_TRUE,
 *  this API just frees up the context without the release procedure.
 *
 *  \param [in] context Context to be released/freed
 *  \param [in] free Indicate free only without release
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref HAP_RELEASE_CNF is notified on
 *          completion with status as success or failure.
 */
GA_RESULT GA_hap_terminate
          (
              /* IN */ HAP_HANDLE handle,
              /* IN */ UCHAR      free
          );

/**
 *  \brief Release the given GA HAP context.
 *
 *  \par Description
 *  This routine initiates the release procedure for the context. Once release
 *  is done, the context is freed up and the setup must be freshly done by
 *  calling \ref GA_hap_setup_context if required for the same device again.
 *
 *  \param [in] context Context to be released
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 *          If \ref GA_SUCCESS, \ref HAP_RELEASE_CNF is notified on
 *          completion with status as success or failure.
 */
#define GA_hap_release(ctx) \
        GA_hap_terminate((ctx), GA_FALSE)

/**
 *  \brief Free the given HAP context.
 *
 *  \par Description
 *  This routine frees up the given context of the GA layer.
 *
 *  \param [in] context Context to be freed
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
#define GA_hap_close(ctx) \
        GA_hap_terminate((ctx), GA_TRUE)

/** \} */

/**
 * \name HAP Client - Read
 * \{
 */

/**
 *  \brief To perform Read operation on a Char.
 *
 *  \par Description:
 *       This function triggers read request.
 *
 *  \param [in] hap_handle
 *         HAP Handle.
 *
 *  \param [in] hap_exp_evt
 *         Expected event ID to be generated once this operation is performed.
 *
 *  \param [in] char_uuid
 *         Char UUID on which read is requested.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer, HAP_READ_XXX_CNF is
 *          notified, here XXX indicates the name of the char.
 */
GA_RESULT GA_hap_read_request
          (
              /* IN */  HAP_HANDLE hap_handle,
              /* IN */  HAP_EVT_ID hap_exp_evt,
              /* IN */  UINT16     char_uuid
          );

/** \} */

/**
 * \name HAP Client - Write
 * \{
 */

/**
 *  \brief To perform Write operation on a Char.
 *
 *  \par Description:
 *       This function triggers write request.
 *
 *  \param [in] hap_handle
 *         HAP Handle.
 *
 *  \param [in] hap_exp_evt
 *         Expected event ID to be generated once this operation is performed.
 *
 *  \param [in] char_uuid
 *         Char UUID on which write is requested.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When char allows write request and if it is a write request,
 *          once the response is received from peer,
 *          HAP_XXX_CNF is notified, here XXX is the name of the char.
 */
GA_RESULT GA_hap_write_request
          (
              /* IN */  HAP_HANDLE         hap_handle,
              /* IN */  HAP_EVT_ID         hap_exp_evt,
              /* IN */  UINT16             char_uuid,
              /* IN */  HAP_CE_WRITE_REQ * req
          );

/** \} */

/** \} */

#endif /* HAP_CLIENT*/

#ifdef HAP_SERVER

/**
 * \defgroup hap_server_api HAP Server APIs
 * \{
 * This section describes the Hearing Access Profile APIs
 * for Server.
 */

/**
 * \name HAP Server - Notify
 * \{
 */

/**
 *  \brief To send Notifications to the peer HAP device.
 *
 *  \par Description:
 *       This function send notifications to the peer device based on the
 *       address and if configuration is enabled.
 *
 *  \param [in] device
 *         If NULL, Ntf to all connected HAP sessions will be passed.
 *         Else, to the corresponsing address passed.
 *
 *  \param [in] char_uuid
 *         Char UUID.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification.
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_hap_send_ntf
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT16        char_uuid,
              /* IN */ HAP_CHR_VAL * val
          );

/** \} */

/**
 * \name HAP Server - Read/Write
 * \{
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
 *  \param [in] hap_inst
 *         HAP Service Instance on which request is receieved.
 *
 *  \param [in] hap_evt_id
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
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT GA_hap_send_rsp
          (
              /* IN */ GA_ENDPOINT           * device,
              /* IN */ GA_HAP_SE_RSP_CONTEXT * ctx,
              /* IN */ HAP_EVT_ID              hap_evt_id,
              /* IN */ GA_RESULT               rsp_status,
              /* IN */ HAP_CHR_VAL           * val
          );

/** \} */

/** \} */

/** \} */

#endif /* HAP_SERVER */
#ifdef __cplusplus
};
#endif

/**
 * \addtogroup hap_defines
 * \{
 */

/**
 * \addtogroup hap_macros
 * \{
 */

#ifdef HAP_CLIENT
/**
 * \name HAP Client - Read
 * \{
 */

/**
 *  \brief To Read Hearing Aid Features.
 *
 *  \par Description:
 *       This function reads the Hearing Aid Features of the peer.
 *
 *  \param [in] hap_handle
 *         HAP Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref HAP_HA_GET_FEATURES_CNF is notified.
 */
#define GA_hap_read_ha_features(hap_handle) \
        GA_hap_read_request((hap_handle), (HAP_HA_GET_FEATURES_CNF), (GA_CHAR_HA_FEATURES))

/**
 *  \brief To Read Active Preset Index.
 *
 *  \par Description:
 *       This function reads the Active Preset Index of the peer.
 *
 *  \param [in] hap_handle
 *         HAP Handle.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When read response is received from peer,
 *          \ref HAP_HA_GET_ACTIVE_PRESET_INDEX_CNF is notified.
 */
#define GA_hap_read_active_preset_index(hap_handle) \
        GA_hap_read_request((hap_handle), (HAP_HA_GET_ACTIVE_PRESET_INDEX_CNF), (GA_CHAR_ACTIVE_PRESET_INDEX))

/** \} */

/**
 * \name HAP Client - Write
 * \{
 */

/**
 *  \brief To Write Hearing Aid Preset Control Point.
 *
 *  \par Description:
 *       This function writes Hearing Aid Preset Control Point.
 *
 *  \param [in] hap_handle
 *         HAP Handle.
 *
 *  \param [in] req
 *         Write request indicating data, len and type of write.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 *          When write response is received from peer,
 *          \ref HAP_HA_PRESET_CP_CNF is notified.
 */
#define GA_hap_write_ha_preset_cp(hap_handle, req) \
        GA_hap_write_request((hap_handle), (HAP_HA_PRESET_CP_CNF), (GA_CHAR_HA_PRESET_CP), (req))

/** \} */

#endif /* HAP_CLIENT */

#ifdef HAP_SERVER

/**
 * \name HAP Server - Notify
 * \{
 */

/**
 *  \brief To send Notifications - HAP Features.
 *
 *  \par Description:
 *       This function sends notifications if configured for HAP Features.
 *
 *  \param [in] device
 *         If NULL, Ntf to all connected HAP sessions will be passed.
 *         Else, to the corresponsing address passed.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref HAP_FEATURES.
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 */
#define GA_hap_notify_features(device, val) \
        GA_hap_send_ntf((device), (GA_CHAR_HA_FEATURES), (val))

/**
 *  \brief To send Notifications - HAP Active Preset Index.
 *
 *  \par Description:
 *       This function sends notifications if configured for Active Preset Index.
 *
 *  \param [in] device
 *         If NULL, Ntf to all connected HAP sessions will be passed.
 *         Else, to the corresponsing address passed.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification - \ref UINT8.
 *         Data Value to be in the range of
 *         \ref HAP_READ_PRESET_BY_INDEX_REQ_INDEX_MIN and
 *         \ref HAP_READ_PRESET_BY_INDEX_REQ_INDEX_MAX.
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 */
#define GA_hap_notify_active_preset_index(device, val) \
        GA_hap_send_ntf((device), (GA_CHAR_ACTIVE_PRESET_INDEX), (val))

/**
 *  \brief To send Indications - Hearing Aid Preset Control Point.
 *         To send Notifications - When EATT supported.
 *
 *  \par Description:
 *       This function sends notifications if configured for Hearing Aid
 *       Preset Control Point.
 *
 *  \param [in] device
 *         If NULL, Ntf to all connected HAP sessions will be passed.
 *         Else, to the corresponsing address passed.
 *
 *  \param [in] char_info
 *         Data to be sent as part of the notification.
 *
 *  \param [in] char_info_len
 *         Length of the data to be sent.
 *
 *  \return \ref GA_SUCCESS or an error code indicating reason for failure.
 */
#define GA_hap_notify_ha_preset_cp(device, val) \
        GA_hap_send_ntf((device), (GA_CHAR_HA_PRESET_CP), (val))
/** \} */

#endif /* HAP_SERVER */

/** \} */

/** \} */

/** \} */

/** \} */

#endif /* _H_GA_HAP_API_ */
