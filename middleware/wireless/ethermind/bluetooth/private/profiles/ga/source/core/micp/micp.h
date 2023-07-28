
/**
 *  \file micp.h
 *
 *  \brief This file defines the Microphone Control Profile Interface - includes
 *  Data Structures and Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_MICP_
#define _H_MICP_


/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"
#include "GA_bearer_api.h"
#include "GA_property_config.h"

/* --------------------------------------------- Global Definitions */
/** MICP Roles */
#define MICP_ROLE_CLIENT                   0x01U
#define MICP_ROLE_SERVER                   0x02U

/* TODO: Move this to internal? */
#define MICP_CONTEXT_INVALID               0xFFU
#define MICP_CONTEXT_SVC_INVALID           0xFFU
#define MICP_HANDLE_INVALID                0xFFU

/* --------------------------------------------- Data types /Structures */

/**
 * TODO: Move AICS related defines to Common place
 */
/** AICS Characteristic ID refererence */
#define MICS_CHAR_ID_MUTE                            0U

/* Total number of Characteristic IDs */
#define MICS_CHAR_ID_COUNT                           1U

/* Derive the characteristic config mask for the Char ID */
#define MICS_CHAR_CONFIG(id)                         (1U << (id))

/** AICS Characteristic ID refererence */
#define AICS_CHAR_ID_AUDIO_INPUT_STATE               0U
#define AICS_CHAR_ID_GAIN_SETTING_PROPERTIES         1U
#define AICS_CHAR_ID_AUDIO_INPUT_TYPE                2U
#define AICS_CHAR_ID_AUDIO_INPUT_STATUS              3U
#define AICS_CHAR_ID_AUDIO_INPUT_CP                  4U
#define AICS_CHAR_ID_AUDIO_INPUT_DESCRIPTION         5U

/* Total number of Characteristic IDs */
#define AICS_CHAR_ID_COUNT                           6U

/* Derive the characteristic config mask for the Char ID */
#define AICS_CHAR_CONFIG(id)                         (1U << (id))

/** MICP Handle for the core */
typedef UINT8 MICP_HANDLE;
typedef UINT8 MICP_AICS_HANDLE;

/* TODO: Move these Context structure to internal? */
/**
 * MICP MICS Contexts
 */
typedef struct _MICP_CONTEXT_
{
    /* Device Endpoint */
    GA_ENDPOINT device;

    /**
     * MICS characteristics Info.
     */
    GA_CHAR_INFO char_info[MICS_CHAR_ID_COUNT];

    /**
     * Current characteristic ID being configured
     */
    UINT8 char_config_id;

   /**
    * Status Flag
    */
    UCHAR cntx_id;

    /* Context Event for a procedure */
    UINT8 cntx_event;

    /** Service Range */
    GA_BRR_SVC_RANGE range;

}MICP_CONTEXT;

typedef struct _MICP_AICS_CONTEXT_
{
    /**
     * AICS characteristics Info.
     */
    GA_CHAR_INFO char_info[AICS_CHAR_ID_COUNT];

    /**
     * Current characteristic ID being configured
     */
    UINT8 char_config_id;

    /**
     * MICS Service Handle
     */
    MICP_HANDLE mics_hndl;

   /**
    * Status Flag
    */
    UCHAR cntx_id;

    /** Service Range */
    GA_BRR_SVC_RANGE range;
}MICP_AICS_CONTEXT;

/* As Server */
typedef struct _MICS_CONTEXT_
{
   /**
    * MICS Service Handle
    */
    GA_BRR_SVC_INST mics_id;

    /**
     * Mute
     */
    GA_BRR_CHR_INST char_id_MUTE;

    /**
     * Status Flag
     */
    UCHAR status;

}MICS_CONTEXT;

/* As Server: AICS service instance in MICS */
typedef struct _MICS_AICS_CONTEXT_
{
   /**
    * Flag to indicate if allocated
    */
    UCHAR state;

    /**
     * AICS Service
     */
    GA_BRR_SVC_INST aics_id;

    /**
     * Input State
     */
    GA_BRR_CHR_INST char_id_INPUT_STATE;

    /**
     * Gain Setting Properties
     */
    GA_BRR_CHR_INST char_id_GAIN_SETTING_PROPERTIES;

    /**
     * Input Type
     */
    GA_BRR_CHR_INST char_id_INPUT_TYPE;

    /**
     * Input Status
     */
    GA_BRR_CHR_INST char_id_INPUT_STATUS;

    /**
     * Audio Input Control Point
     */
    GA_BRR_CHR_INST char_id_AUDIO_INPUT_CP;

    /**
     * Audio Input Description
     */
    GA_BRR_CHR_INST char_id_AUDIO_INPUT_DESC;

    /**
     * Audio Input Description Properties
     */
    GA_BRR_CHR_INST char_id_AUDIO_INPUT_DESC_prpty;
}MICS_AICS_CONTEXT;


typedef struct _MICP_EVENT_INFO
{
    /**
     * Peer Device Address
     */
    GA_ENDPOINT * device;

    /**
    * Char Info.
    */
    GA_BRR_CHR_CONTEXT *chr;

    /**
     * Service Type
     */
    UCHAR  srvc_type;

    /**
     * Service instance
     */
    UCHAR srv_inst;

    /**
     * Data
     */
    UCHAR * data;

    /**
     * Data Length
     */
    UCHAR datalen;
} MICP_EVENT_INFO;


typedef struct _MICP_CHAR_NTF
{
    /* Service type */
    UCHAR srvs_type;

    /* Char instance id */
    UINT8 inst;

    /**
     * Notify data
     */
    void * data;

    /**
     * Notify data length
     */
    UINT8 datalen;
} MICP_CHAR_NTF;

/* MICP Response Info. */
typedef struct _MICP_RSP_INFO
{
    GA_BRR_CHR_CONTEXT * chr;

    /**
     * Data
     */
    UCHAR * data;

    /**
     * Data Length
     */
    UCHAR datalen;
}MICP_RSP_INF;

/* --------------------------------------------- Macros */

#define MICP_SERVICE_TYPE_MICS                       0x00U
#define MICP_SERVICE_TYPE_AICS                       0x01U
#define MICP_SERVICE_TYPE_INVALID                    0xFFU

/* MICS MUTE Char values */
#define MICS_CHR_VAL_NOT_MUTED                       0x00U
#define MICS_CHR_VAL_MUTED                           0x01U
#define MICS_CHR_VAL_DISABLED                        0x02U

/* Error Codes */
#define MICS_ERR_CODE_MUTE_DISABLED                  0x80U
#define MICS_ERR_CODE_VALUE_OUT_OF_RANGE             0x81U

/** TODO: ATT Errorcode. Move to seperate file */
#define MICS_ERR_CODE_NOT_ALLOWED                    0x13U

/* ================== */

/* ====== AICS ======== */
/* AICS Related Error Codes */
#define AICS_ERR_CODE_INVALID_CHNG_CNTR              0x80U
#define AICS_ERR_CODE_OPCODE_NOT_SUPPORTED           0x81U
#define AICS_ERR_CODE_MUTE_DISABLED                  0x82U
#define AICS_ERR_CODE_VALUE_OUT_OF_RANGE             0x83U
#define AICS_ERR_CODE_GAIN_MODE_CHANG_NOT_ALLOWED    0x84U

/* AICS Mute Field Values */
#define AICS_NOT_MUTED                               0x00U
#define AICS_MUTED                                   0x01U
#define AICS_DISABLED                                0x02U

/* AICS Gain Mode Field */
#define AICS_GM_MANUAL_ONLY                          0x00U
#define AICS_GM_AUTOMATIC_ONLY                       0x01U
#define AICS_GM_MANUAL                               0x02U
#define AICS_GM_AUTO                                 0x03U

/* Input Status */
#define AICS_IN_STATUS_INACTIVE                      0x00U
#define AICS_IN_STATUS_ACTIVE                        0x01U

/* AICS Gain Setting Min./Max. */
#define AICS_GAIN_SETTING_MIN                       -128
#define AICS_GAIN_SETTING_MAX                        127

/* Control Point Opcodes */
#define AICS_CP_OP_SET_GAIN_SETTING                  0x01U
#define AICS_CP_OP_UNMUTE                            0x02U
#define AICS_CP_OP_MUTE                              0x03U
#define AICS_CP_OP_SET_MANUAL_GAIN_MODE              0x04U
#define AICS_CP_OP_SET_AUTO_GAIN_MODE                0x05U


/* Common Event */
#define MC_SETUP_MICS_CNF                               0xE0U
#define MC_DISCOVER_MICS_OPTIONAL_SRVC_CNF              0xE1U
#define MC_SETUP_AICS_CNF                               0xE2U
#define MC_RELEASE_MICS_CNF                             0xE3U
#define MC_RELEASE_AICS_CNF                             0xE4U

#define MC_EVENT_GET_MUTE_CNF                           0xE5U
#define MC_EVENT_GET_MUTE_IND                           0xE6U
#define MC_EVENT_SET_MUTE_CNF                           0xE7U
#define MC_EVENT_SET_MUTE_IND                           0xE8U
#define MC_EVENT_MUTE_NTF                               0xE9U


/* AICS Events */
#define MC_AICS_EVENT_GET_INPUT_STATE_CNF               0xEAU
#define MC_AICS_EVENT_GET_GAIN_SETTING_PROPRTY_CNF      0xEBU
#define MC_AICS_EVENT_GET_INPUT_TYPE_CNF                0xECU
#define MC_AICS_EVENT_GET_INPUT_STATUS_CNF              0xEDU
#define MC_AICS_EVENT_GET_AUDIO_INPUT_DESC_CNF          0xEEU
#define MC_AICS_EVENT_SET_AUDIO_INPUT_DESC_IND          0xEFU
#define MC_AICS_EVENT_RD_IND                            0xF0U
#define MC_AICS_EVENT_CP_WT_CNF                         0xF1U
#define MC_AICS_EVENT_CP_WT_IND                         0xF2U
#define MC_AICS_EVENT_CP_INPUT_STATE_NTF                0xF3U
#define MC_AICS_EVENT_CP_INPUT_STATUS_NTF               0xF4U
#define MC_AICS_EVENT_AUDIO_INPUT_DESC_NTF              0xF5U
#define MC_EVENT_INVALID                                0xFFU

/** \} */
/* --------------------------------------------- APIs */

/* ============= MICP Controller ============= */
#ifdef MICP_CONTROLLER
/**
 *  \brief To Init the MICP Controller Module
 *
 *  \par Description:
 *       This function enables to initialize the GA MICP Controller Module;
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT MICP_client_init(void);

/**
 *  \brief To Shutdown the MICP Controller Module
 *
 *  \par Description:
 *       This function enables to Shutdown the GA MICP Controller Module;
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT MICP_client_shutdown(void);

/**
 *  \brief To Setup Microphone Control Service(MICS) Context
 *
 *  \par Description:
 *       This function enables to setup MICS context with given device.
 *       Internally, MICS and Service Characterstics and enable notifications is done.
 *
 *  \param [in] device
 *        Peer Device with which the context is setup.
 *
 *  \param [out] mics_hndl
 *        Allocated MICS Context Handle.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT MICP_setup_context
          (
              /* IN */  GA_ENDPOINT   *device,
              /* OUT */ MICP_HANDLE   *handle
          );

/**
 *  \brief To Discover the Included Services from the remoted device.
 *
 *  \par Description:
 *       This function enables to discover the included services (i.e. AICS)
 *       with the remote device.
 *
 *  \param [in] mics_hndl
 *       Handle of MICS Service.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT MICP_discover_optional_services
          (
              /* IN */ MICP_HANDLE  handle
          );

/**
 *  \brief To Setup Audio Input Control Service Context
 *
 *  \par Description:
 *       This function enables to setup a AICS context with given MICS Instance.
 *
 *  \param [in] mics_hndl
 *        MICS Handle.
 *
 *  \param [in] svc
 *
 *  \param [out] aics_hndl
 *        Allocated AICS Context Handle.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT MICP_setup_aics_cntx
          (
              /* IN  */ MICP_HANDLE   handle,
              /* IN */  GA_BRR_SVC_INFO *svc,
              /* OUT */ MICP_AICS_HANDLE  *aics
          );

GA_RESULT MICP_release_context
          (
              /* IN */ MICP_HANDLE   handle,
              /* IN */ UCHAR free
          );

GA_RESULT MICP_get_micp_handle_from_aics
          (
              /* IN */  MICP_AICS_HANDLE   micp_aics_handle,
              /* OUT */ MICP_HANDLE      * micp_handle
          );

/**
 *  \brief To Release a MICP Service context.
 *
 *  \par Description:
 *       This function enables to release context of given MICS
 *       service type & service handle.
 *
 *  \param [in] mics_srvs_type
 *        Flag to indicate the Service type.
 *
 *  \param [in] mics_hndl
 *        Handle associated with the given service type.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT MICP_release_aics_context
          (
              /* IN */ MICP_AICS_HANDLE   handle,
              /* IN */ UCHAR free
          );

/**
 *  \brief To Read MICS Characteristic Value.
 *
 *  \par Description:
 *       This function enables to read MICS Char. Value with given MICS Handle.
 *
 *  \param [in] mics_hndl
 *        MICS Handle.
 *
 *  \param [in] e_event
 *        Expected Event after completion of this procedure.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 *
 * Note: TODO:
 *  - Remove 'e_event' dependency. Beacuse only one characteristic is present.
 */
GA_RESULT MICP_read_request
          (
              /* IN  */ MICP_HANDLE  handle,
              /* IN  */ UCHAR  cevent
          );

/**
 *  \brief To Set MICS Characteristic Value.
 *
 *  \par Description:
 *       This function enables to set MICS Char. Value with given MICS Handle.
 *
 *  \param [in] mics_hndl
 *        MICS Handle.
 *
 *  \param [in] mics_mute_val
 *        Mute char value.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT MICP_set_mute
          (
              /* IN  */ MICP_HANDLE  handle,
              /* IN */  UINT8        mics_mute_val
          );

/**
 *  \brief To Read AICS Characteristic Value.
 *
 *  \par Description:
 *       This function enables to read AICS Char. Value with given AICS instance.
 *
 *  \param [in] mics_hndl
 *        MICS Handle.
 *
 *  \param [in] e_event
 *        Expected Event after completion of this procedure.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT MICP_aics_read_request
          (
              /* IN  */ MICP_AICS_HANDLE  handle,
              /* IN  */ UCHAR  cevent
          );

/**
 *  \brief To Set Audio Input Description.
 *
 *  \par Description:
 *       This function enables to Set Audio Input Description.
 *
 *  \param [in] aics_hndl
 *        AICS Handle
 *
 *  \param [in] aics_audio_in_desc
 *        Audio Input Description.
 *
 *  \param [in] aics_audio_in_desc_len
 *        Audio Input Description Length.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT MICP_aics_set_audio_input_description
          (
              /* IN  */ MICP_AICS_HANDLE   handle,
              /* IN */  UCHAR  *aics_audio_in_desc,
              /* IN */  UCHAR   aics_audio_in_desc_len
          );

/**
 *  \brief  To send AICS Control Point Request.
 *
 *  \par Description:
 *       This routine enables to request AICS Control Point Write.
 *
 *  \param [in] aics_hndl
 *         AICS Handle
 *
 *  \param [in] aics_op
 *         AICS Control Point Opcode
 *
 *  \param [in] aics_req_data
 *              Control Point Write Request Data
 *
 *  \param [in] aics_req_datalen
 *              Control Point Write Request Data Length.
 *
 * *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
GA_RESULT MICP_aics_cp_request
          (
              /* IN  */ MICP_AICS_HANDLE  handle,
              /* IN */  UINT8   aics_op,
              /* IN */  void   *aics_req_data,
              /* IN */  UINT16  aics_req_datalen
          );

GA_RESULT MICP_update_ntf_configuration
          (
              /* IN */ UCHAR srvs_type,
              /* IN */ UINT32 config
          );

#ifdef MICP_SUPPORT_CONTEXT_MANAGE
GA_RESULT MICP_manage_mics_context_info
          (
              /* IN     */ UINT8              set,
              /* IN     */ GA_ENDPOINT       * device,
              /* INOUT  */ MICP_HANDLE       * handle,
              /* INOUT  */ GA_BRR_SVC_RANGE  ** range,
              /* INOUT  */ GA_CHAR_INFO      ** info,
              /* INOUT  */ UINT16             * info_count
          );

GA_RESULT MICP_manage_aics_context_info
          (
              /* IN     */ UINT8                 set,
              /* IN     */ MICP_HANDLE           handle,
              /* INOUT  */ MICP_AICS_HANDLE    * aics_handle,
              /* INOUT  */ GA_BRR_SVC_RANGE    ** range,
              /* INOUT  */ GA_CHAR_INFO        ** info,
              /* INOUT  */ UINT16               * info_count
          );
#endif /* MICP_SUPPORT_CONTEXT_MANAGE */
#endif /* MICP_CONTROLLER */

/* ============= MICP Device ============= */

#ifdef MICP_DEVICE
/**
 *  \brief To Init the MICP Device Module
 *
 *  \par Description:
 *       This function enables to initialize the GA MICP Controller Module;
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT MICP_server_init(void);

/**
 *  \brief To Shutdown the MICP Device Module
 *
 *  \par Description:
 *       This function enables to Shutdown the GA MICP Controller Module;
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT MICP_server_shutdown(void);

/**
 *  \brief To register AICS
 *
 *  \par Description:
 *       This function enables to register AICS with MICS.
 *
 *  \param [out] aics_hndl
 *         AICS Handle
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT MICP_register_aics
          (
              /* OUT */ UCHAR *aics_hndl
          );

/**
 *  \brief To unregister AICS
 *
 *  \par Description:
 *       This function enables to unregister AICS with MICS
 *
 *  \param [out] aics_hndl
 *         AICS Handle
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT MICP_unregister_aics
          (
              /* IN */ UCHAR aics_inst
          );

/**
 *  \brief To Notify a char value to remote devices.
 *
 *  \par Description:
 *       This function enables to notify char value.
 *
 *  \param [in] device
 *         Remote Device Address
 *
 *  \param [in] srvs_inst
 *         MICS/AICS service instance.
 *
 *  \param [in] srvs_type
 *         Flag to indicate the Service Type.
 *
 *  \param [in] char_uuid
 *         Flag to indicate the Service Type.
 *
 *  \param [in] char_info
 *         Char. value.
 *
 *  \param [out] aics_hndl
 *         Char Value Length.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT MICP_notify_char
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UCHAR         srvs_inst,
              /* IN */ UCHAR         srvs_type,
              /* IN */ UINT16        char_uuid,
              /* IN */ void         *char_info,
              /* IN */ UCHAR         char_info_len
          );

/* To Send Response */
GA_RESULT MICP_send_rsp
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UCHAR         event_type,
              /* IN */ UINT16        event_status,
              /* IN */ void         *rsp_info,
              /* IN */ UCHAR         rsp_info_len
          );

#endif /* MICP_RENDERER */

#endif /* _H_MICP_ */



