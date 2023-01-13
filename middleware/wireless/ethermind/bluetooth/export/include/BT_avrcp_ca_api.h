/**
 *  \file BT_avrcp_ca_api.h
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_AVRCP_CA_API_
#define _H_AVRCP_CA_API_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_obex_api.h"
/* --------------------------------------------- Global Definitions */

#ifdef AVRCP_COVER_ART
/**
 * \defgroup avrcp_ca_module AVRCP CA(Audio Video Remote Control Profile - Cover Art)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Audio Video Remote Control Profile - Cover Art(AVRCP - CA) module to the Application
 *  and other upper layers of the stack.
 */

/**
 * \defgroup avrcp_ca_defines Defines
 * \{
 * Describes defines for the module.
 */

/**
 * \defgroup avrcp_ca_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/* AVRCP Cover Art Initiator Notification Callback Event Types */
/**
 *  \name AVRCP Cover Art Initiator Notification Callback Event Types
 *
 *  Constant Definitions for AVRCP Cover Art Initiator Notification Callback
 *  Event Types
 *
 */
/*@{*/
#define AVRCP_CAI_CONNECT_CNF                           0x01U
#define AVRCP_CAI_DISCONNECT_CNF                        0x02U
#define AVRCP_CAI_TRANSPORT_CLOSE_CNF                   0x03U
#define AVRCP_CAI_TRANSPORT_CLOSE_IND                   0x04U
#define AVRCP_CAI_GET_IMAGE_PROPERTIES_CNF              0x05U
#define AVRCP_CAI_GET_IMAGE_CNF                         0x06U
#define AVRCP_CAI_GET_LINKED_THUMBNAIL_CNF              0x07U
#define AVRCP_CAI_GET_ABORT_CNF                         0x08U
/*@}*/
/* AVRCP Cover Art Responder Notification Callback Event Types */
/**
 *  \name AVRCP Cover Art Responder Notification Callback Event Types
 *
 *  Constant Definitions for AVRCP Cover Art Responder Notification Callback Event Types
 *
 */
/*@{*/
#define AVRCP_CAR_CONNECT_IND                           0x31U
#define AVRCP_CAR_DISCONNECT_IND                        0x32U
#define AVRCP_CAR_TRANSPORT_CLOSE_CNF                   0x33U
#define AVRCP_CAR_TRANSPORT_CLOSE_IND                   0x34U
#define AVRCP_CAR_GET_IMAGE_PROPERTIES_IND              0x35U
#define AVRCP_CAR_GET_IMAGE_IND                         0x36U
#define AVRCP_CAR_GET_LINKED_THUMBNAIL_IND              0x37U
#define AVRCP_CAR_GET_ABORT_IND                         0x38U
/*@}*/
/* Cover Art Target Header Size */
/**
 *  \name AVRCP Cover Art Responder Notification Callback Event Types
 *
 *  Constant Definitions for AVRCP Cover Art Responder Notification Callback Event Types
 *
 */
/*@{*/
#define AVRCP_CA_TARGET_HDR_SIZE                        0x10U
/*@}*/
/* Supported Image Formats */
/**
 *  \name Supported Image Formats
 *
 *  Constant Definitions for Supported Image Formats
 *
 */
/*@{*/
#define COVER_ART_FORMAT_JPEG                           0x01U
#define COVER_ART_FORMAT_GIF                            0x02U
#define COVER_ART_FORMAT_INVALID                        0xFFU
/*@}*/
/* Dynamic PSM used for Cover Art */
/**
 *  \name Dynamic PSM used for Cover Art
 *
 *  Constant Definitions for Dynamic PSM used for Cover Art
 *
 */
 /*@{*/
#define COVER_ART_GOEP_L2CAP_PSM                        0x1005U
/*@}*/
/* AVRCP Cover Art OBEX Response Codes */
/**
 *  \name AVRCP Cover Art OBEX Response Codes
 *
 *  Constant Definitions for AVRCP Cover Art OBEX Response Codes
 *
 */
/*@{*/
#define AVRCP_CA_SUCCESS_RSP                            OBEX_SUCCESS_RSP
#define AVRCP_CA_CONTINUE_RSP                           OBEX_CONTINUE_RSP
#define AVRCP_CA_BAD_REQ_RSP                            OBEX_REQ_IS_BAD_RSP
#define AVRCP_CA_NOT_IMPLEMENTED_RSP                    OBEX_NOT_IMP_RSP
#define AVRCP_CA_UNAUTH_RSP                             OBEX_UNAUTHORIZED_RSP
#define AVRCP_CA_PRECOND_FAILED_RSP                     OBEX_PRECOND_FAIL_RSP
#define AVRCP_CA_NOT_FOUND_RSP                          OBEX_NOT_FOUND_RSP
#define AVRCP_CA_NOT_ACCEPTABLE_RSP                     OBEX_NOT_ACCEPTABLE_RSP
#define AVRCP_CA_NO_SERVICE_RSP                         OBEX_NO_SERVICE_RSP
#define AVRCP_CA_FORBIDDEN_RSP                          OBEX_FORBIDDEN_RSP
#define AVRCP_CA_SERVER_ERROR                           OBEX_SERVER_ERR_RSP
/*@}*/
/* User Defined OBEX Headers for BIP */
/**
 *  \name User Defined OBEX Headers for BIP
 *
 *  Constant Definitions for User Defined OBEX Headers for BIP
 *
 */
/*@{*/
#define OBEX_HDR_IMG_HANDLE                             0x30U
#define OBEX_HDR_IMG_DESCRIPTOR                         0x71U
/*@}*/
#define AVRCP_CA_EVENT_INVALID                          0xFFU
#define AVRCP_CA_ENTITY_INVALID                         0xFFU
#define AVRCP_CA_INVALID_OBEX_CONNECTION_ID             0xFFFFFFFFU

/* To initialize a Header Structure */
#define CA_INIT_HEADER_STRUCT(hs)    \
        (hs).value = NULL;           \
        (hs).length = 0
/** \} */
/* --------------------------------------------- Structures/Data Types */
/**
 * \defgroup avrcp_ca_structures AVRCP CA Structures
 * \{
 * Describes AVRCP CA Structures defined by the module.
 */
/**
 * AVRCP Cover Art Handle type
 */
typedef UINT8 AVRCP_CA_HANDLE;

/**
 * Cover Art OBEX Header structure to exchange data with application
 */
typedef struct _AVRCP_CA_HEADER_STRUCT
{
    /**
     * Pointer to data
     */
    UCHAR   * value;

    /**
     * Data Length
     */
    UINT16    length;

} AVRCP_CA_HEADER_STRUCT;

/**
 * AVRCP Cover Art Connection Structure
 */
typedef struct _AVRCP_CA_CONNECT_STRUCT
{
    /**
     * Bluetooth Address of the peer device
     */
    UCHAR      * bd_addr;

    /**
     *  Maximum Receive Size
     */
    UINT16       max_recv_size;

#ifdef OBEX_OVER_L2CAP
    /**
     * L2CAP PSM
     */
    UINT16      l2cap_rpsm;
#endif /* OBEX_OVER_L2CAP */

} AVRCP_CA_CONNECT_STRUCT;

/**
 * Cover Art Request Structure
 */
typedef struct _AVRCP_CA_REQUEST_STRUCT
{
    /**
     * Handle of the Image
     */
    AVRCP_CA_HEADER_STRUCT   * img_handle;

    /**
     * Descriptor of the Image
     */
    AVRCP_CA_HEADER_STRUCT   * img_descriptor;

    /**
     * Image Content
     */
    AVRCP_CA_HEADER_STRUCT   * body;

    /**
     * Wait specifier during SRM transaction
     */
    UCHAR                 wait;
} AVRCP_CA_REQUEST_STRUCT;

/**
 * Cover Art Response Structure
 */
typedef struct _AVRCP_CA_RESPONSE_STRUCT
{
     /* Image data or Image properties data */
    AVRCP_CA_HEADER_STRUCT   * body;

     /* Image Length */
    AVRCP_CA_HEADER_STRUCT   * length;

    /**
     * Wait specifier during SRM transaction
     */
    UCHAR                 wait;
} AVRCP_CA_RESPONSE_STRUCT;

/* AVRCP Cover Art Common Headers */
typedef union _AVRCP_CA_HEADERS
{
    /**
     * Request structure, used to send the Cover Art request
     */
    AVRCP_CA_REQUEST_STRUCT   * ca_req_info;

    /**
     * Response structure, used to send the Cover Art response
     */
    AVRCP_CA_RESPONSE_STRUCT  * ca_rsp_info;

    /**
     * Connect structure, used to send the Cover Art connect request
     */
    AVRCP_CA_CONNECT_STRUCT   * ca_connect_info;

} AVRCP_CA_HEADERS;

typedef struct _AVRCP_CA_IMAGE_DESCRIPTOR
{
    /**
     *  Image format
     *     - JPEG
     *     - GIF
     */
    UCHAR format;

    /* Image height (No. of pixels) */
    UINT16 img_height;

    /* Image width (No. of pixels) */
    UINT16 img_width;

}AVRCP_CA_IMAGE_DESCRIPTOR;

/* AVRCP Cover Art  Application Event Notification Callback Type */
typedef API_RESULT (* AVRCP_CA_EVENT_NTF_CB)
                   (
                        AVRCP_CA_HANDLE     * avrcp_ca_handle,
                        UINT8                 event_type,
                        UINT16                event_result,
                        AVRCP_CA_HEADERS    * avrcp_ca_headers,
                        UINT16                num_headers
                   );
/** \} */
/** \} */
/* --------------------------------------------- Macros */

/* --------------------------------------------- Functions */

/* =========== AVRCP  Cover Art Initiator APIs ==== */
/**
 * \defgroup avrcp_ca_api AVRCP CA API definitions
 * \{
 * Describes API definitions of the module.
 */
#ifdef AVRCP_COVER_ART_INITIATOR
/**
 *  \fn BT_avrcp_cai_init
 *
 *  \brief To initialize the AVRCP  Cover Art Initiator Module.
 *
 *  \par Description:
 *       This API is used to initialize the AVRCP  Cover Art Initiator Module.
 *       It initializes global data structures and synchronization primitives used
 *       in this module.
 *
 *  \param None
 *
 *  \return API_RESULT: API_SUCCESS or an Error Code describing
 *                      the cause of failure.
 *
 */
API_RESULT BT_avrcp_cai_init (void);

/**
 *  \fn BT_avrcp_cai_shutdown
 *
 *  \brief To shutdown AVRCP  Cover Art Initiator Module.
 *
 *  \par Description:
 *        This API shuts down the AVRCP  Cover Art Initiator Module
 *        if it is already initialized.
 *
 *  \param None
 *
 *  \return API_RESULT: API_SUCCESS or an Error Code describing
 *                      the cause of failure.
 *
 */
API_RESULT BT_avrcp_cai_shutdown (void);

/**
 *  \fn BT_avrcp_cai_start
 *
 *  \brief To start an instance of AVRCP  Cover Art Initiator.
 *
 *  \par Description:
 *       This API starts an instance of AVRCP  Cover Art Initiator. On the successful
 *       instantiation, this API returns a Cover Art handle which is an index to the
 *       AVRCP  Cover Art Initiator. This also registers the callback function that
 *       Cover Art needs to call when any event occurs for this instance. Now the AVRCP
 *        Cover Art Initiator is ready to initiate a connection to AVRCP
 *       Cover Art Responder.
 *
 *  \param [out] ca_handle
 *         Index to the Covert Art Initiator entity that is started.
 *
 *  \param [in] avrcp_cai_cb
 *         The application callback function pointer to be registered.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \note All further communication exchanged between the application and profile
 *        for the AVRCP Cover Art instance will be identified using the assigned
 *        Cover Art handle.
 */
API_RESULT BT_avrcp_cai_start
           (
               /* OUT */ AVRCP_CA_HANDLE       * ca_handle,
               /* IN */  AVRCP_CA_EVENT_NTF_CB   avrcp_cai_cb
           );

/**
 *  \fn BT_avrcp_cai_stop
 *
 *  \brief To stop an active AVRCP  Cover Art Initiator instance.
 *
 *  \par Description:
 *       This API stops the AVRCP  Cover Art Initiator instance corresponding
 *       to the specified handle.
 *
 *  \param [in] ca_handle
 *         Index to the AVRCP  Cover Art entity that has to
 *         be stopped.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \note In case an active connection exists in the specified instance to be stopped,
 *        the transport for the connection is closed and the instance is stopped.
 */
API_RESULT BT_avrcp_cai_stop
           (
               /* IN */ AVRCP_CA_HANDLE * ca_handle
           );

/**
 *  \fn BT_avrcp_cai_connect
 *
 *  \brief To establish the AVRCP  Cover Art Service connection.
 *
 *  \par Description:
 *       This API establishes the OBEX connection with the remote AVRCP Target for the
 *       AVRCP Cover Art Service. Cover Art Initiator calls the registered
 *       application callback upon the completion of the connection procedure (when the
 *       Cover Art Responder responds for the connection request) with the notification
 *       AVRCP_CAI_CONNECT_CNF event
 *
 *  \param [in] ca_handle
 *          Handle of the Cover Art Initiator instance that has to be connected.
 *
 *  \param [in] avrcp_ca_connect_info
 *         Structure containing the Bluetooth Device Address of the Cover Art Responder,
 *         the local maximum receive capacity (in Bytes) and L2CAP psm of
 *         Cover Art Responder.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_avrcp_cai_connect
           (
               /* IN */ AVRCP_CA_HANDLE          * ca_handle,
               /* IN */ AVRCP_CA_CONNECT_STRUCT  * avrcp_ca_connect_info
           );

/**
 *  \fn BT_avrcp_cai_disconnect
 *
 *  \brief To disconnect the established Cover Art Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the active Cover Art Service Connection
 *       with the remote AVRCP  Cover Art Responder. Cover Art Initiator calls
 *       the registered application callback upon completion of this request
 *       with the notification AVRCP_CAI_DISCONNECT_CNF event.
 *
 *  \param [in] ca_handle
 *         Handle of the Cover Art Initiator instance that has to be disconnected.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_avrcp_cai_disconnect
           (
               /* IN */ AVRCP_CA_HANDLE * ca_handle
           );

/**
 *  \fn BT_avrcp_cai_get_image_properties
 *
 *  \brief To retrieve the Cover Art Image characteristics from Cover Art Responder.
 *
 *  \par Description:
 *       This API retrieves the Cover Art Image characteristics from Cover Art
 *       Responder. The application shall include the handle of Cover Art Image. The
 *       Cover Art Responder will return the image properties of the
 *       referenced image.
 *
 *       The completion of this procedure is notified through AVRCP_CAI_GET_IMAGE_PROPERTIES_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] ca_handle
 *         Handle of the Cover Art Initiator instance.
 *
 *  \param [in] avrcp_ca_get_info
 *         Structure containing the Cover Art Image handle.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_avrcp_cai_get_image_properties
           (
               /* IN */ AVRCP_CA_HANDLE         * ca_handle,
               /* IN */ AVRCP_CA_REQUEST_STRUCT * avrcp_ca_get_info
           );

/**
 *  \fn BT_avrcp_cai_get_image
 *
 *  \brief To retrieve the Cover Art Image from AVRCP  Cover Art Responder.
 *
 *  \par Description:
 *       This API retrieves the Cover Art Image from Cover Art Responder.
 *       The application shall include the handle of Cover Art and the image
 *       descriptor object.

 *       The completion of this procedure is notified through AVRCP_CAI_GET_IMAGE_CNF
 *       event along with the status of the procedure.
 *
 *  \param [in] ca_handle
 *         Handle of the Cover Art Initiator instance.
 *
 *  \param [in] avrcp_ca_get_info
 *         Structure containing the Cover Art Image handle and its image descriptor object.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_avrcp_cai_get_image
           (
               /* IN */ AVRCP_CA_HANDLE          * ca_handle,
               /* IN */ AVRCP_CA_REQUEST_STRUCT  * avrcp_ca_get_info
           );

/**
 *  \fn BT_avrcp_cai_get_linked_thumbnail
 *
 *  \brief To retrieve the Linked Thumbnail of the Cover Art Image from
 *         Cover Art Responder.
 *
 *  \par Description:
 *       This API retrieves the Cover Art Image from AVRCP  Cover Art Responder.
 *       The application shall include the Cover Art image handle and its image
 *       descriptor object.

 *       The completion of this procedure is notified through
 *       AVRCP_CAI_GET_LINKED_THUMBNAIL_CNF  event along with the status of
 *       the procedure.
 *
 *  \param [in] ca_handle
 *         Handle of the Cover Art Initiator instance.
 *
 *  \param [in] avrcp_ca_get_info
 *         Structure containing the Cover Art Image handle.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_avrcp_cai_get_linked_thumbnail
           (
               /* IN */ AVRCP_CA_HANDLE          * ca_handle,
               /* IN */ AVRCP_CA_REQUEST_STRUCT  * avrcp_ca_get_info
           );

/**
 *  \fn BT_avrcp_cai_abort
 *
 *  \brief To abort the ongoing Cover Art operation.
 *
 *  \par Description:
 *       This API aborts the ongoing OBEX Get operation that was initiated by the Cover Art Initiator.
 *       The completion of the Abort operation is notified through AVRCP_CAI_GET_ABORT_CNF notification
 *       event along with the status of the procedure.
 *
 *  \param [in] ca_handle
 *         Handle of the Cover Art Initiator instance of which the operation is to be aborted.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_avrcp_cai_abort
           (
               /* IN */ AVRCP_CA_HANDLE      * ca_handle
           );

/**
 *  \fn BT_avrcp_cai_send_req
 *
 *  \brief To send a request to the Cover Art responder
 *
 *  \par Description:
 *       This API sends the request to the Cover Art responder with the corresponding
 *       Cover Art event and the data given by the application contained in the cover_art_headers.
 *       The application shall call this API to continue any operation initiated and to
 *       which the first notification has been received. Thus this API shall be called from the
 *       AVRCP  Cover Art Initiator application callback function with the event_type set
 *       to the current received event.
 *
 *  \param [in] ca_handle
 *         Handle of the Cover Art Initiator instance.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback, to indicate that the request is
 *         the continuation of the operation for which it was notified.
 *
 *  \param [in] event_result
 *         Status of the request.
 *
 *  \param [in] cover_art_hdrs
 *           Structure containing Get request information, in case of Get operation.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_avrcp_cai_send_req
           (
               /* IN */ AVRCP_CA_HANDLE     * ca_handle,
               /* IN */ UINT8                 event_type,
               /* IN */ UINT16                event_result,
               /* IN */ AVRCP_CA_HEADERS    * cover_art_hdrs
           );

/**
 *  \fn BT_avrcp_cai_transport_close
 *
 *  \brief To disconnect the underlying transport of AVRCP Cover Art Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the transport connection of the AVRCP Cover Art
 *       Service with the remote AVRCP Cover Art responder. AVRCP Cover Art Initiator
 *       calls the registered  application callback upon completion of this procedure with the
 *       notification AVRCP_CAI_TRANSPORT_CLOSE_CNF event.
 *
 *  \param [in] ca_handle
 *         Handle of the Cover Art Initiator instance.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_avrcp_cai_transport_close
           (
               /* IN */ AVRCP_CA_HANDLE   * ca_handle
           );
#endif /* AVRCP_COVER_ART_INITIATOR */

/* =========== AVRCP  Cover Art Responder APIs =========== */
#ifdef AVRCP_COVER_ART_RESPONDER
/**
 *  \fn BT_avrcp_car_init
 *
 *  \brief To initialize the AVRCP  Cover Art Responder Module.
 *
 *  \par Description:
 *       This API is used to initialize the AVRCP  Cover Art Module.
 *       It initializes global data structures and synchronization primitives used
 *       in this module.
 *
 *  \param None
 *
 *  \return API_RESULT: API_SUCCESS or an Error Code describing
 *                      the cause of failure.
 *
 */
API_RESULT BT_avrcp_car_init (void);

/**
 *  \fn BT_avrcp_car_shutdown
 *
 *  \brief To shutdown AVRCP  Cover Art Responder Module.
 *
 *  \par Description:
 *        This API shuts down the AVRCP  Cover Art Responder Module
 *        if it is already initialized.
 *
 *  \param None
 *
 *  \return API_RESULT: API_SUCCESS or an Error Code describing
 *                      the cause of failure.
 *
 */
API_RESULT BT_avrcp_car_shutdown (void);

/**
 *  \fn BT_avrcp_car_start
 *
 *  \brief To start an instance of AVRCP  Cover Art Responder.
 *
 *  \par Description:
 *       This API starts an instance of AVRCP  Cover Art Responder. On the successful
 *       instantiation, this API returns a Cover Art handle which is an index to the
 *       AVRCP  Cover Art. This also registers the callback function that
 *       Cover Art needs to call when any event occurs for this instance. Now the AVRCP
 *        Cover Art Responder is ready to accept a connection from AVRCP
 *       Cover Art Responder.
 *
 *  \param [out] ca_handle
 *         Index to the Cover Art Responder entity that is started.
 *
 *  \param [in] avrcp_car_cb
 *         The application callback function pointer to be registered.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \note All further communication exchanged between the application and profile
 *        for the AVRCP Cover Art instance will be identified using the assigned
 *        Cover Art handle.
 */
API_RESULT BT_avrcp_car_start
           (
               /* OUT */ AVRCP_CA_HANDLE        * ca_handle,
               /* IN */  AVRCP_CA_EVENT_NTF_CB    avrcp_car_cb
           );

/**
 *  \fn BT_avrcp_car_stop
 *
 *  \brief To stop an active AVRCP  Cover Art Responder instance.
 *
 *  \par Description:
 *       This API stops the AVRCP  Cover Art Responder instance corresponding
 *       to the specified handle.
 *
 *  \param [in] ca_handle
 *         Index to the AVRCP  Cover Art entity that has to
 *         be stopped.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \note In case an active connection exists in the specified instance to be stopped,
 *        the transport for the connection is closed and the instance is stopped.
 */
API_RESULT BT_avrcp_car_stop
           (
               /* IN */ AVRCP_CA_HANDLE * ca_handle
           );

/**
 *  \fn BT_avrcp_car_send_response
 *
 *  \brief To send response to the latest request received from the Cover Art Initiator.
 *
 *  \par Description:
 *       This API is used to send a response to the requests made by the Cover Art
 *       Initiator. Response to Connect, GetImageProperties, GetImage, GetLinkedThumbnail
 *       shall be sent through this API. The application shall call this API with the event
 *       type of the latest notification received. This indicates the response is for the
 *       latest indication received. Cover Art Responder frames the OBEX specific packets
 *       based on the response and associated response data and send it to the Cover Art Initiator.
 *
 *  \param [in] ca_handle
 *         Handle of the Cover Art Responder instance.
 *
 *  \param [in] event_type
 *         Event type that was received in the application callback.
 *         This is to indicate that the response being sent in response to the
 *         request for which the application was notified.
 *
 *  \param [in] event_result
 *         This is the OBEX response code.
 *
 *  \param [in] cover_art_hdrs
 *         Structure containing either the connect information, in case of connect operation
 *         or the response data, in case of Get operation.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_avrcp_car_send_response
           (
               /* IN */ AVRCP_CA_HANDLE     * ca_handle,
               /* IN */ UINT8                 event_type,
               /* IN */ UCHAR                 event_result,
               /* IN */ AVRCP_CA_HEADERS    * cover_art_hdrs
           );

/**
 *  \fn BT_avrcp_car_get_obex_connection_status
 *
 *  \brief To get Cover Art OBEX connection status.
 *
 *  \par Description:
 *       This API used to get the current Cover Art OBEX connection status. If Cover Art OBEX
 *       connection status is true, then Cover Art Attribute can be sent for GetElementAttribute,
 *       GetFolderItems etc. requests.
 *
 *  \param [in] ca_handle
 *         Handle of the Cover Art Responder instance.
 *
 *  \param [out] status
 *         Status of Cover Art OBEX connection, Set to BT_TRUE for an Active Cover Art
 *         Connection else BT_FALSE
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 */
API_RESULT BT_avrcp_car_get_obex_connection_status
           (
               /* IN */  AVRCP_CA_HANDLE handle,
               /* OUT */ UCHAR * status
           );

/**
 *  \fn BT_avrcp_car_transport_close
 *
 *  \brief To disconnect the underlying transport of AVRCP Cover Art Service Connection.
 *
 *  \par Description:
 *       This API is to disconnect the transport connection of the AVRCP Cover Art
 *       Service with the remote AVRCP Cover Art Initiator. AVRCP Cover Art Responder
 *       calls the registered  application callback upon completion of this procedure with the
 *       notification AVRCP_CAR_TRANSPORT_CLOSE_CNF event.
 *
 *  \param [in] ca_handle
 *         Handle of the Cover Art Responder instance.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
API_RESULT BT_avrcp_car_transport_close
           (
               /* IN */ AVRCP_CA_HANDLE   * ca_handle
           );
/** \} */
/** \} */
#endif /* AVRCP_COVER_ART_RESPONDER */

#endif /* AVRCP_COVER_ART */

#endif /* _H_AVRCP_CA_API_  */

