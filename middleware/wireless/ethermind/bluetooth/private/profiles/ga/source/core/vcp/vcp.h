
/**
 *  \file vcp.h
 *
 *  \brief This file defines the Volume Control Profile Interface - includes
 *  Data Structures and Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_VCP_
#define _H_VCP_


/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"
#include "GA_bearer_api.h"
#include "GA_property_config.h"

/* --------------------------------------------- Global Definitions */
/** VCP Roles */
#define VCP_ROLE_CLIENT                   0x01U
#define VCP_ROLE_SERVER                   0x02U

/* TODO: Move this to internal? */
#define VCP_CONTEXT_INVALID               0xFFU
#define VCP_CONTEXT_SVC_INVALID           0xFFU
#define VCP_HANDLE_INVALID                0xFFU

/* --------------------------------------------- Data types /Structures */

/** VCP Handle for the core */
typedef UINT8 VCP_HANDLE;
typedef UINT8 VCP_VOCS_HANDLE;
typedef UINT8 VCP_AICS_HANDLE;

/** VCS Characteristic ID refererence */
#define VCS_CHAR_ID_VOLUME_STATE                    0U
#define VCS_CHAR_ID_VOLUME_CP                       1U
#define VCS_CHAR_ID_VOLUME_FLAGS                    2U

/* Total number of Characteristic IDs */
#define VCS_CHAR_ID_COUNT                           3U

/* Derive the characteristic config mask for the Char ID */
#define VCS_CHAR_CONFIG(id)                         (1U << (id))

/** VOCS Characteristic ID refererence */
#define VOCS_CHAR_ID_VOLUME_OFFSET_STATE             0U
#define VOCS_CHAR_ID_AUDIO_LOCATION                  1U
#define VOCS_CHAR_ID_VOLUME_OFFSET_CP                2U
#define VOCS_CHAR_ID_VOLUME_DESCRIPTION              3U

/* Total number of Characteristic IDs */
#define VOCS_CHAR_ID_COUNT                           4U

/* Derive the characteristic config mask for the Char ID */
#define VOCS_CHAR_CONFIG(id)                         (1U << (id))

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

/* TODO: Move these Context structure to internal? */
/**
 * VCP VCS Contexts
 */
typedef struct _VCP_CONTEXT
{
    /* Device Endpoint */
    GA_ENDPOINT device;

    /**
     * VCS characteristics Info.
     */
    GA_CHAR_INFO char_info[VCS_CHAR_ID_COUNT];

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

}VCP_CONTEXT;

typedef struct _VCP_VOCS_CONTEXT
{
    /**
     * VOCS characteristics Info.
     */
    GA_CHAR_INFO char_info[VOCS_CHAR_ID_COUNT];

    /**
     * Current characteristic ID being configured
     */
    UINT8 char_config_id;

   /**
    * Status Flag
    */
    UCHAR cntx_id;

    /**
     * Associated VCP Context
     */
    VCP_HANDLE vcp_id;

    /** Service Range */
    GA_BRR_SVC_RANGE range;

}VCP_VOCS_CONTEXT;

typedef struct _VCP_AICS_CONTEXT
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
    * Status Flag
    */
    UCHAR cntx_id;

    /**
     * Associated VCP Context
     */
    VCP_HANDLE vcp_id;

    /** Service Range */
    GA_BRR_SVC_RANGE range;
}VCP_AICS_CONTEXT;

/**
 * VCS as Server
 */
typedef struct _VCS_ENTITY_
{
    /**
    * Flag to indicate if allocated.
    */
    UCHAR state;

    /* Service Id */
    GA_BRR_SVC_INST   vcs_id;

    /**
     * Volume State
     */
    GA_BRR_CHR_INST char_id_VOLUME_STATE;

    /**
     * Volume Control Point
     */
    GA_BRR_CHR_INST char_id_VOLUME_CP;

    /**
     * Volume Flags
     */
    GA_BRR_CHR_INST char_id_VOLUME_FLAGS;

    /**
     * Volume Flags Properties
     */
    GA_BRR_CHR_INST   char_id_VOLUME_FLAGS_prpty;
}VCS_ENTTITY;

/**
 *  VOCS as Server
 */
typedef struct _VCS_VOCS_ENTITY_
{
   /**
    * Flag to indicate if allocated.
    */
    UCHAR state;

    /**
     * VOCS Service Handle
     */
    GA_BRR_SVC_INST vocs_id;

    /**
     * Offset State
     */
    GA_BRR_CHR_INST char_id_OFFSET_STATE;

    /**
     * Audio Location
     */
    GA_BRR_CHR_INST char_id_AUDIO_LOCATION;

    /**
     * Volume Offset Control Point
     */
    GA_BRR_CHR_INST char_id_VOLUME_OFFSET_CP;

    /**
     * Audio Output Description
     */
    GA_BRR_CHR_INST char_id_AUDIO_OUTPUT_DESC;
}VCS_VOCS_ENTITY;

/**
 * AICS as a Server
 */
typedef struct _VCS_AICS_ENTITY_
{
   /**
    * Flag to indicate if allocated.
    */
    UCHAR state;

    /**
     * VCS Service Handle
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
    GA_BRR_CHR_INST char_id_AUDIO_INPUT_DESCRIPTION;
}VCS_AICS_ENTITY;

typedef struct _VCP_CHAR_NTF
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
} VCP_CHAR_NTF;

/* VCP Event Info */
typedef struct _VCP_EVENT_INFO
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
     * Service instance. Refer \ref GA_VR_HANDLE.
     */
    UCHAR srvc_inst;

    /**
     * Data
     */
    UCHAR * data;

    /**
     * Data Length
     */
    UCHAR datalen;
} VCP_EVENT_INFO;

/* VCP Response Info. */
typedef struct _VCP_RSP_INFO
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
}VCP_RSP_INF;

/* --------------------------------------------- Macros */

#define VCP_SERVICE_TYPE_VCS                        0x00U
#define VCP_SERVICE_TYPE_VOCS                       0x01U
#define VCP_SERVICE_TYPE_AICS                       0x02U
#define VCP_SERVICE_TYPE_INVALID                    0xFFU

/* Error OpCodes */
#define VCS_ERR_CODE_INVALID_CHNG_CNTR              0x80U
#define VCS_ERR_CODE_OPCODE_NOT_SUPPORTED           0x81U

/* Volume State Char.: Volume Setting Field Min/Max Values */
#define VCS_VOLUME_SETTING_MIN                      0x00U
#define VCS_VOLUME_SETTING_MAX                      0xFFU

/* Volume State Char.: Mute Field values */
#define VCS_MUTE_NO                                 0x00U
#define VCS_MUTE_YES                                0x01U

/* Volume State Char.: Change Counter Field */
#define VCS_CHNG_CNTR_MIN                           0x00U
#define VCS_CHNG_CNTR_MAX                           0xFFU

/* Volume Setting Persistance Related */
#define VCS_VOLUME_SETTING_PERSIST_NO               0x00U
#define VCS_VOLUME_SETTING_PERSIST_YES              0x01U

/**
 * Volume Control Point Opcodes
 */
#define VCS_CP_OP_RELATIVE_VOL_DOWN                 0x00U
#define VCS_CP_OP_RELATIVE_VOL_UP                   0x01U
#define VCS_CP_OP_UNMUTE_RELATIVE_VOL_DOWN          0x02U
#define VCS_CP_OP_UNMUTE_RELATIVE_VOL_UP            0x03U
#define VCS_CP_OP_SET_ABSOLUE_VOL                   0x04U
#define VCS_CP_OP_UNMUTE                            0x05U
#define VCS_CP_OP_MUTE                              0x06U

/* VCS Events */
#define VCS_EVENT_GET_VOLUME_STATE_CNF              0xA0U
#define VCS_EVENT_GET_VOLUME_STATE_IND              0xA1U
#define VCS_EVENT_GET_VOLUME_FLAGS_IND              0xA2U
#define VCS_EVENT_GET_VOLUME_FLAGS_CNF              0xA3U
#define VCS_EVENT_CP_WT_CNF                         0xA4U
#define VCS_EVENT_CP_WT_IND                         0xA5U
#define VCS_EVENT_VOLUME_STATE_NTF                  0xA6U
#define VCS_EVENT_VOLUME_FLAGS_NTF                  0xA7U
#define VCS_EVENT_RD_IND                            0xA8U

/* ====== VOCS ======== */
/* VOCS Related Error Codes */
#define VOCS_ERR_CODE_INVALID_CHNG_CNTR              0x80U
#define VOCS_ERR_CODE_OPCODE_NOT_SUPPORTED           0x81U
#define VOCS_ERR_CODE_VALUE_OUT_OF_RANGE             0x82U

/* Volume Offset Min./Max. */
#define VOCS_VOLUME_OFFSET_MIN                      -255
#define VOCS_VOLUME_OFFSET_MAX                       255

/* Change Counter Min./Max. */
#define VOCS_CHNG_CNTR_MIN                           0x00U
#define VOCS_CHNG_CNTR_MAX                           0xFFU

/* Control Point Opcodes */
#define VOCS_CP_OP_SET_VOLUME_OFFSET                 0x01U

/* VOCS Events */
#define VOCS_EVENT_GET_OFFSET_STATE_CNF              0xB0U
#define VOCS_EVENT_GET_AUDIO_LOCATION_CNF            0xB1U
#define VOCS_EVENT_SET_AUDIO_LOCATION_IND            0xB2U
#define VOCS_EVENT_GET_AUDIO_OUT_DESC_CNF            0xB3U
#define VOCS_EVENT_SET_AUDIO_OUT_DESC_IND            0xB4U
#define VOCS_EVENT_RD_IND                            0xB5U
#define VOCS_EVENT_CP_WT_CNF                         0xB6U
#define VOCS_EVENT_CP_WT_IND                         0xB7U
#define VOCS_EVENT_OFFSET_STATE_NTF                  0xB8U
#define VOCS_EVENT_AUDIO_LOCATION_NTF                0xB9U
#define VOCS_EVENT_AUDIO_OUT_DESC_NTF                0xBAU

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

/* AICS Events */
#define AICS_EVENT_GET_INPUT_STATE_CNF                  0xC0U
#define AICS_EVENT_GET_GAIN_SETTING_PROPRTY_CNF         0xC1U
#define AICS_EVENT_GET_INPUT_TYPE_CNF                   0xC2U
#define AICS_EVENT_GET_INPUT_STATUS_CNF                 0xC3U
#define AICS_EVENT_GET_AUDIO_INPUT_DESC_CNF             0xC4U
#define AICS_EVENT_SET_AUDIO_INPUT_DESC_IND             0xC5U
#define AICS_EVENT_RD_IND                               0xC6U
#define AICS_EVENT_CP_WT_CNF                            0xC7U
#define AICS_EVENT_CP_WT_IND                            0xC8U
#define AICS_EVENT_CP_INPUT_STATE_NTF                   0xC9U
#define AICS_EVENT_CP_INPUT_STATUS_NTF                  0xCAU
#define AICS_EVENT_AUDIO_INPUT_DESC_NTF                 0xCBU

/* Common Event */
#define  VCP_SETUP_VCS_CNF                              0xD0U
#define  VCP_DISCVER_OPTIONAL_SERVICES_CNF              0xD1U
#define  VCP_SETUP_VOCS_CNF                             0xD2U
#define  VCP_SETUP_AICS_CNF                             0xD3U
#define  VCP_RELEASE_VCS_CNF                            0xD4U
#define  VCP_RELEASE_VOCS_CNF                           0xD5U
#define  VCP_RELEASE_AICS_CNF                           0xD6U
#define  VCP_EVENT_INVALID                              0xFFU

/* --------------------------------------------- APIs */
/* ============= VCP Controller ============= */
#ifdef VCP_CONTROLLER
/**
 *  \brief To Init the VCP Client Module
 *
 *  \par Description:
 *       This function enables to initialize the GA VCP Client Module;
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT VCP_client_init(void);

/**
 *  \brief To Shutdown the VCP Client Module
 *
 *  \par Description:
 *       This function enables to Shutdown the GA VCP Client Module;
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT VCP_client_shutdown(void);

/**
 *  \brief To Setup Volume Control Service(VCS) Context
 *
 *  \par Description:
 *       This function enables to setup VCS context with given device.
 *       Internally, VCS service is discovered.
 *
 *  \param [in] device
 *        Peer Device with which the context is setup.
 *
 *  \param [out] vcs_hndl
 *        Allocated VCS Context Handle.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 *          If API_SUCCESS, VCP_DISCOVER_SRVS_CNF is notified.
 */
GA_RESULT VCP_setup_context
          (
              /* IN */ GA_ENDPOINT * device,
              /* OUT */ VCP_HANDLE * handle
          );

/**
 * Note:
 * TODO: Discover the Included Services from the remoted device.
 */
GA_RESULT VCP_discover_optional_services
          (
              /* IN */ VCP_HANDLE handle
          );

/**
 *  \brief To Setup Volume Offset Control Service Context
 *
 *  \par Description:
 *       This function enables to setup a context with given VCS.
 *
 *  \param [in] vcs_hndl
 *        VCS Handle associated with VOCS
 *
 *  \param [out] vocs_hndl
 *        Allocated VOCS Context Handle.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 *         If API_SUCCESS, VCP_DISCOVER_SRVS_CONTINUE_CNF or VCP_DISCOVER_SRVS_CNF.
 */
GA_RESULT VCP_setup_vocs_context
          (
              /* IN */  VCP_HANDLE        handle,
              /* IN */  GA_BRR_SVC_INFO * svc,
              /* OUT */ VCP_VOCS_HANDLE * vocs
          );

/**
 *  \brief To Setup Audio Input Control Service Context
 *
 *  \par Description:
 *       This function enables to setup a context with given VCS.
 *
 *  \param [in] vcs_hndl
 *        VCS Handle associated with VOCS
 *
 *  \param [out] aics_hndl
 *        Allocated AICS Context Handle.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 *         If API_SUCCESS, VCP_DISCOVER_SRVS_CONTINUE_CNF or VCP_DISCOVER_SRVS_CNF.
 */
GA_RESULT VCP_setup_aics_context
          (
              /* IN */  VCP_HANDLE        handle,
              /* IN */  GA_BRR_SVC_INFO * svc,
              /* OUT */ VCP_AICS_HANDLE * aics
          );

GA_RESULT VCP_get_vcp_handle_from_opt_svc
          (
              /* IN */  UCHAR        vcp_srvc_type,
              /* IN */  UINT8        vcp_opt_srvc_handle,
              /* OUT */ VCP_HANDLE * vcp_handle
          );

/**
 *  \brief To Release a VCP context.
 *
 *  \par Description:
 *       This function enables to release context of given service.
 *
 *  \param [in] vcs_srvs_type
 *        Flag to indicate the Service type.
 *
 *  \param [in] vcs_hndl
 *        Handle associated with the given service
 *
 *  \param [out] aics_hndl
 *        Allocated AICS Context Handle.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 *         If API_SUCCESS, VCP_DISCOVER_SRVS_CONTINUE_CNF or VCP_DISCOVER_SRVS_CNF.
 */

GA_RESULT VCP_release_context
          (
              /* IN */ VCP_HANDLE   handle,
              /* IN */ UCHAR   free
          );

GA_RESULT VCP_release_vocs_context
          (
              /* IN */ VCP_VOCS_HANDLE   handle,
              /* IN */ UCHAR   free
          );

GA_RESULT VCP_release_aics_context
          (
              /* IN */ VCP_AICS_HANDLE   handle,
              /* IN */ UCHAR   free
          );

GA_RESULT VCP_update_ntf_configuration
          (
              /* IN */ UCHAR srvs_type,
              /* IN */ UINT32 config
          );

/**
 *  \brief  To send VCS Control Point Request.
 *
 *  \par Description:
 *       This routine enables to request VCS control point write.
 *
 *  \param [in] vcs_hndl
 *         VCS Handle
 *
 *  \param [in] cp_op
 *         VCS Control Point Opcode
 *
 *  \param [in] cp_req_data
 *              Control Point Write Request Data
 *
 *  \param [in] valen
 *              Control Point Write Request Data Length.
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
GA_RESULT VCP_cp_request
          (
              /* IN */ VCP_HANDLE  handle,
              /* IN */ UINT8   cp_op,
              /* IN */ void   *cp_req_data,
              /* IN */ UINT16  cp_req_datalen
          );

/* ================= VOCS Related =============== */

GA_RESULT VCP_read_request
          (
              /* IN  */ VCP_HANDLE  handle,
              /* IN  */ UCHAR       cevent
          );


/* TODO: Use this read char interface and remove other vocs reads char apis */
GA_RESULT VCP_vocs_read_request
          (
              /* IN */ VCP_VOCS_HANDLE  handle,
              /* IN */ UCHAR            cevent
          );

/* TODO: Use this read char interface and remove other vocs reads char apis */
GA_RESULT VCP_aics_read_request
          (
              /* IN */ VCP_AICS_HANDLE  handle,
              /* IN */ UCHAR            cevent
          );

/**
 *  \brief To Set Audio Location.
 *
 *  \par Description:
 *       This function enables to set Audio Location.
 *
 *  \param [in] vocs_hndl
 *        VOCS Handle
 *
 *  \param [in] vocs_audio_loc
 *        VOCS Audio Location.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT VCP_vocs_set_audio_location
          (
              /* IN  */ VCP_VOCS_HANDLE  handle,
              /* IN */  UINT32           vocs_audio_loc
          );

/**
 *  \brief To Set Audio Output Description.
 *
 *  \par Description:
 *       This function enables to set Audio Output Description.
 *
 *  \param [in] vocs_hndl
 *        VOCS Handle
 *
 *  \param [in] vocs_audio_out_desc
 *        Audio Output Description.
 *
 *  \param [in] vocs_audio_out_desc_len
 *        Audio Output Description Length
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT VCP_vocs_set_audio_output_description
          (
              /* IN  */ VCP_VOCS_HANDLE  handle,
              /* IN */  UCHAR  *vocs_audio_out_desc,
              /* IN */  UCHAR  vocs_audio_out_desc_len
          );

/**
 *  \brief  To send VOCS Control Point Request.
 *
 *  \par Description:
 *       This routine enables to request VOCS control point write.
 *
 *  \param [in] vocs_hndl
 *         VOCS Handle
 *
 *  \param [in] cp_op
 *         VOCS Control Point Opcode
 *
 *  \param [in] cp_req_data
 *              Control Point Write Request Data
 *
 *  \param [in] cp_req_datalen
 *              Control Point Write Request Data Length.
 *
 *
 *  \return  API_RESULT:
 *           API_SUCCESS on success, or, an Error Code
 *           describing the cause of failure.
 */
GA_RESULT VCP_vocs_cp_request
          (
              /* IN */ VCP_VOCS_HANDLE  handle,
              /* IN */ UINT8   cp_op,
              /* IN */ void   *cp_req_data,
              /* IN */ UINT16  cp_req_datalen
          );

/* ================= AICS Related =============== */
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
GA_RESULT VCP_aics_set_audio_input_description
          (
              /* IN  */ VCP_AICS_HANDLE   handle,
              /* IN */  UCHAR  *aics_audio_in_desc,
              /* IN */  UCHAR   aics_audio_in_desc_len
          );

/**
 *  \brief  To send AICS Control Point Request.
 *
 *  \par Description:
 *       This routine enables to request VOCS control point write.
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
GA_RESULT VCP_aics_cp_request
          (
              /* IN */ VCP_AICS_HANDLE  handle,
              /* IN */ UINT8   aics_op,
              /* IN */ void   *aics_req_data,
              /* IN */ UINT16  aics_req_datalen
          );

#ifdef VCP_SUPPORT_CONTEXT_MANAGE
GA_RESULT VCP_manage_vcs_context_info
          (
              /* IN     */ UINT8              set,
              /* IN     */ GA_ENDPOINT      * device,
              /* INOUT  */ VCP_HANDLE       * handle,
              /* INOUT  */ GA_BRR_SVC_RANGE ** range,
              /* INOUT  */ GA_CHAR_INFO     ** info,
              /* INOUT  */ UINT16           * info_count
          );

GA_RESULT VCP_manage_vocs_context_info
          (
              /* IN     */ UINT8                set,
              /* IN     */ VCP_HANDLE           handle,
              /* INOUT  */ VCP_VOCS_HANDLE    * vocs_handle,
              /* INOUT  */ GA_BRR_SVC_RANGE   ** range,
              /* INOUT  */ GA_CHAR_INFO       ** info,
              /* INOUT  */ UINT16             * info_count
          );

GA_RESULT VCP_manage_aics_context_info
          (
              /* IN     */ UINT8                set,
              /* IN     */ VCP_HANDLE           handle,
              /* INOUT  */ VCP_AICS_HANDLE    * aics_handle,
              /* INOUT  */ GA_BRR_SVC_RANGE   ** range,
              /* INOUT  */ GA_CHAR_INFO       ** info,
              /* INOUT  */ UINT16             * info_count
          );
#endif /* VCP_SUPPORT_CONTEXT_MANAGE */
#endif /* VCP_CONTROLLER */

/* ============= VCP Renderer ============= */
#ifdef VCP_RENDERER
GA_RESULT VCP_server_init(void);
GA_RESULT VCP_server_shutdown(void);
GA_RESULT VCP_register_vocs(/* OUT */ UINT8 *vocs_hndle);
GA_RESULT VCP_unregister_vocs(/* IN */ UINT8 vocs_hndle);
GA_RESULT VCP_register_aics(/* OUT */ UINT8 *aics_hndle);
GA_RESULT VCP_unregister_aics(/* IN */ UINT8 aics_hndle);
GA_RESULT VCP_notify_char
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UCHAR         srvs_inst,
              /* IN */ UCHAR         srvs_type,
              /* IN */ UINT16        char_uuid,
              /* IN */ void         *char_info,
              /* IN */ UCHAR         char_info_len
          );

GA_RESULT VCP_send_rsp
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UCHAR         event_type,
              /* IN */ UINT16        event_status,
              /* IN */ void         *rsp_info,
              /* IN */ UCHAR         rsp_info_len
          );
#endif /* VCP_RENDERER */

#endif /* _H_VCP_ */



