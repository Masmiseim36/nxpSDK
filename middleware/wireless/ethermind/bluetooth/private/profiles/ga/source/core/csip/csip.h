
/**
 *  \file csip.h
 *
 *  \brief This file defines the Coordinated Set Identification Profile Interface - includes
 *  Data Structures and Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_CSIP_
#define _H_CSIP_


/* --------------------------------------------- Header File Inclusion */
#include "GA_common.h"
#include "GA_bearer_api.h"
#include "GA_property_config.h"

/* --------------------------------------------- Global Definitions */
/** CSIP Roles */
#define CSIP_ROLE_CLIENT                   0x01U
#define CSIP_ROLE_SERVER                   0x02U

/** Invalid CSIP Handle */
#define CSIP_HANDLE_INVALID                0xFFU

/** SIRK Length */
#define CSIP_SIRK_LEN                      16U

/** CSIS Characteristic ID refererence */
#define CSIS_CHAR_ID_SIRK                            0U
#define CSIS_CHAR_ID_SET_SIZE                        1U
#define CSIS_CHAR_ID_SET_MEMBER_LOCK                 2U
#define CSIS_CHAR_ID_SET_MEMBER_RANK                 3U

/* Total number of Characteristic IDs */
#define CSIS_CHAR_ID_COUNT                           4U

/* Derive the characteristic config mask for the Char ID */
#define CSIS_CHAR_CONFIG(id)                         (1U << (id))

/* --------------------------------------------- Data types /Structures */
/** CSIP Handle for the CSIP context */
typedef UINT8 CSIP_HANDLE;

/** CSIP Set Handle */
typedef UINT8 CSIP_SET_HANDLE;

/** CSIP Member Handle */
typedef UINT8 CSIP_MEMBER_HANDLE;

/** CSIP Event Information */
typedef struct _CSIP_EVENT_INFO
{
    /** CS Member Handle */
    CSIP_MEMBER_HANDLE handle;

    /** Context */
    GA_BRR_CHR_CONTEXT * context;

    /** Data */
    void * data;

    /* Data Size */
    UINT16 datalen;

} CSIP_EVENT_INFO;


/** CSIP Event response */
typedef struct _CSIP_RSP_INFO
{
    /** Context */
    GA_BRR_CHR_CONTEXT * context;

    /** Response data */
    void * data;

    /** Response data size */
    UINT16 datalen;

} CSIP_RSP_INFO;

/** SIRK information */
typedef struct _CSIP_SIRK
{
    /**
     *  Type
     *  0 - Encrypted
     *  1 - Plain
     */
    UCHAR type;

    /**
     *  SIRK Value
     */
    UCHAR value[CSIP_SIRK_LEN];

} CSIP_SIRK;

/** SIRK Procedure callback */
typedef void (* CSIP_SIRK_CB)
             (
                 UCHAR * data,
                 UINT16  datalen
             );

/* --------------------------------------------- Macros */
/* Error Codes */
#define CSIS_ERR_LOCK_DENIED                        0x80U
#define CSIS_ERR_LOCK_RELEASE_NOT_ALLOWED           0x81U
#define CSIS_ERR_INVALID_LOCK_VALUE                 0x82U
#define CSIS_ERR_OOB_SIRK_ONLY                      0x83U
#define CSIS_ERR_LOCK_ALREADY_GRANTED               0x84U

/* CSIP Asynchronous Events */
#define CSIP_SIRK_GET_IND                           0xD0U
#define CSIP_SETSIZE_GET_IND                        0xD1U
#define CSIP_SETMEMBER_LOCK_SET_IND                 0xD2U
#define CSIP_SETMEMBER_LOCK_GET_IND                 0xD3U
#define CSIP_SETMEMBER_RANK_GET_IND                 0xD4U
#define CSIP_SIRK_NTF                               0xD5U
#define CSIP_SET_SIZE_NTF                           0xD6U
#define CSIP_SETMEMBER_LOCK_STATUS_NTF              0xD7U

/* CSIP Synchronous Events */
#define CSIP_DISCOVER_CNF                           0xD8U
#define CSIP_SETUP_CNF                              0xD9U
#define CSIP_RELEASE_CNF                            0xDAU
#define CSIP_SIRK_GET_CNF                           0xDBU
#define CSIP_SETSIZE_GET_CNF                        0xDCU
#define CSIP_SETMEMBER_LOCK_SET_CNF                 0xDDU
#define CSIP_SETMEMBER_LOCK_GET_CNF                 0xDEU
#define CSIP_SETMEMBER_RANK_GET_CNF                 0xDFU

#define CSIP_EVENT_INVALID                          0xFFU

/**
 * \defgroup ga_csip_set_member_lock CSIS Set Member Lock Values.
 * \{
 * This section lists the CSIS Set Member Lock Values.
 * As defined in CSIS Specification.
 */

 /** Set Member Lock - Unlocked */
#define CSIP_SET_MEMBER_LOCK_UNLOCKED    0x01

/** Set Member Lock - Locked */
#define CSIP_SET_MEMBER_LOCK_LOCKED      0x02

/** \} */

/**
 * \defgroup ga_csip_sirk CSIS SIRK Type.
 * \{
 * This section lists the CSIS SIRK Type.
 * As defined in CSIS Specification.
 */

/** SIRK Type - Encrypted SIRK */
#define CSIP_SIRK_TYPE_ENCRYPTED         0x00

/** SIRK Type - Plain text SIRK */
#define CSIP_SIRK_TYPE_PLAIN_TEXT        0x01

/** \} */

/**
 * \defgroup ga_csip_coord_set_size CSIS Coordinated Set Size Values.
 * \{
 * This section lists the CSIS Coordinated Set Size Values.
 * As defined in CSIS Specification.
 */

 /** Coordinated Set Size - Minimum */
#define CSIP_SET_SIZE_MIN          0x02

/** Coordinated Set Size - Maximum */
#define CSIP_SET_SIZE_MAX          0xFF

/** \} */

/* --------------------------------------------- APIs */
GA_RESULT CSIP_client_init(void);
GA_RESULT CSIP_client_shutdown(void);
GA_RESULT CSIP_server_init(void);
GA_RESULT CSIP_server_shutdown(void);

/**
 *  \brief To register a local CSIP set member instance.
 *
 *  \par Description:
 *       This function registers a local CSIP Set Member instance
 *
 *  \param [out] handle
 *        Address to hold assigned member handle
 *
 *  \param [out] service_id
 *        Address to hold assigned service ID
 *
 *  \return GA_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT CSIP_register_member
          (
              /* OUT */ CSIP_MEMBER_HANDLE * handle,
              /* OUT */ GA_BRR_SVC_INST * service_id
          );

#ifdef CSIP_SUPPORT_CONTEXT_MANAGE
/**
 *  \brief Interface to manage the CSIP CSIS Context Information.
 *
 *  \par Description:
 *       This function enables to get/set the context information of CSIP CSIS.
 *
 *  \param [in] set
 *         Flag to indicate get/set the context information.
 *
 *  \param [in] ep
 *         Remote Device Address.
 *
 *  \param [inout] hndl
 *         CSIP Handle which is allocated during CSIS context setup.
 *
 *  \param [inout] rng
 *         CSIS characteristics handle range.
 *
 *  \param [inout] c_info
 *          CSIS characteristics information.
 *
 *  \param [inout] c_info_cnt
 *          CSIS characteristics count.
 *
 *  \return GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
GA_RESULT CSIP_manage_csis_context_info
          (
              /* IN     */ UINT8              set,
              /* IN     */ GA_ENDPOINT       * device,
              /* INOUT  */ CSIP_HANDLE       * handle,
              /* INOUT  */ GA_BRR_SVC_RANGE  ** range,
              /* INOUT  */ GA_CHAR_INFO      ** info,
              /* INOUT  */ UINT16            * info_count
          );
#endif /* CSIP_SUPPORT_CONTEXT_MANAGE */

/* ============= CSIP Set ============= */

/**
 * \ignore
 * \{
 */

/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param [in] set_sirk
 *
 *  \param [in] set_handle
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT CSIP_set_create
          (
              /* IN */ UINT8 * set_sirk,
              /* OUT */ CSIP_SET_HANDLE * set_handle
          );

/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param [in] set_handle
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT CSIP_set_release
          (
              /* IN */ CSIP_SET_HANDLE set_handle
          );

/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param [in] set_handle
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT CSIP_set_add_member
          (
              /* IN */ CSIP_SET_HANDLE set_handle,
              /* IN */ CSIP_HANDLE     handle
          );

/**
 *  \brief
 *
 *  \par Description:
 *
 *  \param [in] set_handle
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT CSIP_set_remove_member
          (
              /* IN */ CSIP_SET_HANDLE set_handle,
              /* IN */ CSIP_HANDLE     handle
          );

/** \} */

/**
 *  \brief To discover Coordinated Set Idenitfucation Service (CSIS)
 *
 *  \par Description:
 *       This function discovers the CSIS in a given device.
 *
 *  \param [in] device
 *        Peer Device on which the discovery is performed.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT CSIP_discover_csis
          (
              /* IN */  GA_ENDPOINT * device
          );

/**
 *  \brief To update the configuration mask
 *
 *  \par Description:
 *       This function helps to update the configuration setting of which
 *       of the CSIS characteristics of a peer needs configuration at setup.
 *
 *  \param [in] config
 *         Bitmask of the Characteristic IDs for configuration
 *
 *  \return GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 *  \ref
 */
GA_RESULT CSIP_update_ntf_configuration(UINT32 config);

/**
 *  \brief To Setup Coordinated Set Idenitfucation Profile (CSIP) Context
 *
 *  \par Description:
 *       This function enables to setup CSIP context with given device.
 *       Internally, CSIS service is discovered.
 *
 *  \param [in] device
 *        Peer Device with which the context is setup.
 *
 *  \param [out] ci
 *        Allocated CSIP Context Handle.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT CSIP_setup_context
          (
              /* IN */  GA_ENDPOINT * device,
              /* IN */  GA_BRR_SVC_INFO * svc_info,
              /* OUT */ CSIP_HANDLE * ci
          );

/**
 *  \brief To Release Coordinated Set Idenitfucation Profile (CSIP) Context
 *
 *  \par Description:
 *       This function enables to release the given CSIP context.
 *
 *  \param [in] ci
 *       CSIP Context Handle.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 *
 *  \note This function internally disables notifications and invokes CSIP_RELEASE_CNF
 */
GA_RESULT CSIP_release_context
          (
              /* IN */ CSIP_HANDLE ci,
              /* IN */ UCHAR free
          );


/**
 *  \brief To get value of an attribute of a CSIP context
 *
 *  \par Description:
 *       This function enables to read an attribute value in CSIP context
 *
 *  \param [in] ci
 *        CSIP Context Handle
 *
 *  \param [in] cevent
 *        Attribute event to be generated
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT CSIP_get_value
          (
              /* IN */ CSIP_HANDLE ci,
              /* IN */ UCHAR cevent
          );


/**
 *  \brief To set value of an attribute of a CSIP context
 *
 *  \par Description:
 *       This function enables to write an attribute value in CSIP context
 *
 *  \param [in] ci
 *        CSIP Context Handle
 *
 *  \param [in] cevent
 *        Attribute event to be generated
 *
 *  \param [in] data
 *        Data for the attribute
 *
 *  \param [in] cevent
 *        Data size
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT CSIP_set_value
          (
              /* IN */ CSIP_HANDLE ci,
              /* IN */ UCHAR cevent,
              /* IN */ void * data,
              /* IN */ UINT16 size
          );

/**
 *  \brief To send Notification.
 *
 *  \par Description:
 *       This function enables to send notification for a char
 *
 *  \param [in] device
 *        Device Endpoint on which Ntf needs to be sent to
 *
 *  \param [in] ci
 *        CSIP Context Handle
 *
 *  \param [in] char_uuid
 *        Char UUID
 *
 *  \param [in] ntf_data
 *        Data for the attribute
 *
 *  \param [in] len
 *        Notification Data size
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT CSIP_send_ntf
          (
              /* IN */ GA_ENDPOINT  * device,
              /* IN */ CSIP_MEMBER_HANDLE ci,
              /* IN */ UINT16         char_uuid,
              /* IN */ void         * ntf_data,
              /* IN */ UINT16         len
          );

/**
 *  \brief To send response to incoming request.
 *
 *  \par Description:
 *       This function enables to send response to incoming request from the peer.
 *
 *  \param [in] device
 *         Peer Device Address Information.
 *
 *  \param [in] event_type
 *         Event type for which response is sent.
 *
 *  \param [in] event_status
 *         Response status for the read/write operation.
 *
 *  \param [in] rsp_info
 *         Response information for read/write operatios \ref CISP_RSP_INFO.
 *
 *  \param [in] rsp_info_len
 *         Response information size.
 *
 *  \return GA_SUCCESS or one of the error codes as defined in \ref GA_error.h.
 */
GA_RESULT CSIP_send_rsp
          (
              /* IN */ GA_ENDPOINT   * device,
              /* IN */ UCHAR           event_type,
              /* IN */ UINT16          event_status,
              /* IN */ CSIP_RSP_INFO * rsp_info,
              /* IN */ UCHAR           rsp_info_len
          );

/**
 *  \brief To encrypt/decrypt a given SIRK
 *
 *  \par Description:
 *       This function encrypts/decrypts a given SIRK with the Key
 *
 *  \param [in] sirk
 *        128 Bit SIRK to be encrypted/decrypted
 *
 *  \param [in] key
 *        128 Bit Key for encryption/decryption
 *
 *  \param [in] encrypt
 *        1 - Encrypt, 0 - Decrypt
 *
 *  \param [in] cb
 *        Callback to deliver the result
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
GA_RESULT CSIP_sirk_process
          (
              /* IN */ UCHAR * sirk,
              /* IN */ UCHAR * key,
              /* IN */ UCHAR encrypt,
              /* IN */ CSIP_SIRK_CB cb
          );

#endif /* _H_CSIP_ */



