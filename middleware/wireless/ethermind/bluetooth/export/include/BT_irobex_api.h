
/**
 *  \file BT_irobex_api.h
 *
 *  Module API Header File for irobex.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_IROBEX_API_
#define _H_BT_IROBEX_API_

/* ----------------------------------- Header File Inclusion */
#include "BT_common.h"
/**
 * \defgroup irobex_module
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  module to the Application and other upper layers of the stack.
 */
/**
 * \defgroup irobex_defines Defines
 * \{
 * Describes defines for the module.
 */
/**
 * \defgroup irobex_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/* Standard header identifiers for irobex objects */
/**
 * @name Standard header identifiers for irobex objects
 *
 * Constant Definitions for Standard header identifiers for irobex objects
 */
/*@{*/
#define         IROBEX_HDR_COUNT                   0xc0U
#define         IROBEX_HDR_NAME                    0x01U
#define         IROBEX_HDR_TYPE                    0x42U
#define         IROBEX_HDR_LENGTH                  0xc3U
#define         IROBEX_HDR_TIME_ISO                0x44U
#define         IROBEX_HDR_TIME                    0xc4U
#define         IROBEX_HDR_DESCRIPTION             0x05U
#define         IROBEX_HDR_TARGET                  0x46U
#define         IROBEX_HDR_HTTP                    0x47U
#define         IROBEX_HDR_BODY                    0x48U
#define         IROBEX_HDR_END_BODY                0x49U
#define         IROBEX_HDR_WHO                     0x4AU
#define         IROBEX_HDR_CONNECT_ID              0xCBU
#define         IROBEX_HDR_APP_PARAM               0x4CU
#define         IROBEX_HDR_AUTH_CHALLENGE          0x4DU
#define         IROBEX_HDR_AUTH_RESPONSE           0x4EU
#define         IROBEX_HDR_OBJECT_CLASS            0x4FU
/*@}*/


/*
 * The below definitions are used as event Ids
 * in the call back function.
 */
/**
 * @name Event IDs of callback functions
 *
 * Constant Definitions for Event IDs of callback functions
 */
/*@{*/
#define         IROBEX_OPEN             0x60U
#define         IROBEX_CLOSE            0x61U
/*@}*/
/*
 *  Irobex request opcodes, which is also used in the
 *  Irobex request packet other than using as the
 *  Event IDs
 */
/**
 * @name Irobex request opcodes
 *
 * Constant Definitions for Irobex request opcodes
 */
/*@{*/
#define         IROBEX_CONNECT          0x80U
#define         IROBEX_DISCONNECT       0x81U
#define         IROBEX_PUT              0x02U
#define         IROBEX_PUT_EOF          0x82U
#define         IROBEX_GET              0x03U
#define         IROBEX_GET_FINAL        0x83U
#define         IROBEX_RESERVED         0x04U
#define         IROBEX_SETPATH          0x85U
#define         IROBEX_ABORT            0xFFU
/*@}*/
/*
 * Irobex response code
 */
/**
 * @name Irobex response opcodes
 *
 * Constant Definitions for Irobex response opcodes
 */
/*@{*/

#define         IROBEX_CONTINUE_RSP     0x90U  /**< Continue */
#define         IROBEX_SUCCESS_RSP      0xA0U  /**< OK, Success */
#define         IROBEX_CREATED_RSP      0xA1U  /**< Created */
#define         IROBEX_ACCEPTED_RSP     0xA2U  /**< Accepted */
#define         IROBEX_NON_AUTH_RSP     0xA3U  /**< Non Authorative information */
#define         IROBEX_NO_CONTENT_RSP   0xA4U  /**< No content */
#define         IROBEX_RESET_CONT_RSP   0xA5U  /**< Reset Content */
#define         IROBEX_PART_CONT_RSP    0xA6U  /**< Partial Content */
#define         IROBEX_MULTI_CHOICE_RSP 0xB0U  /**< Multiple Choices */
#define         IROBEX_MOVE_P_RSP       0xB1U  /**< Moved Permenently */
#define         IROBEX_MOVE_T_RSP       0xB2U  /**< Moved temporarily  */
#define         IROBEX_SEE_OTHER_RSP    0xB3U  /**< See Other */
#define         IROBEX_NOT_MODI_RSP     0xB4U  /**< Not Modified */
#define         IROBEX_USE_PROXY        0xB5U  /**< Use proxy */
#define         IROBEX_BAD_REQ_RSP      0xC0U  /**< Bad Request */
#define         IROBEX_UNAUTH_RSP       0xC1U  /**< Unathorized */
#define         IROBEX_PAY_RSP          0xC2U  /**< Payment required */
#define         IROBEX_FORBIDDEN_RSP    0xC3U  /**< Forbidden ; operation refused*/
#define         IROBEX_NO_FOUND_RSP     0xC4U  /**< Not found */
#define         IROBEX_METHOD_RSP       0xC5U  /**< Method Not allowed */
#define         IROBEX_NOT_ACCEPT_RSP   0xC6U  /**< Not Acceptable */
#define         IROBEX_PROXY_AUTH_RSP   0xC7U  /**< Proxy Authentication Required */
#define         IROBEX_TIME_OUT_RSP     0xC8U  /**< Request Time out */
#define         IROBEX_CONFLICT_RSP     0xC9U  /**< Conflict */
#define         IROBEX_GONE_RSP         0xCAU  /**< Gone */
#define         IROBEX_LENGTH_RSP       0xCBU  /**< Length required */
#define         IROBEX_PRECOND_RSP      0xCCU  /**< Precondition Failed */
#define         IROBEX_REQ_ENTITY_RSP   0xCDU  /**< Requested entity too large */
#define         IROBEX_REQ_URL_RSP      0xCEU  /**< Requested URL too large */
#define         IROBEX_MEDIA_RSP        0xCFU  /**< Unsupported media type */
#define         IROBEX_SERVER_ERR_RSP   0xD0U  /**< Internal Server error */
#define         IROBEX_NOT_IMP_RSP      0xD1U  /**< Not implemented */
#define         IROBEX_BAD_GATE_RSP     0xD2U  /**< Bad Gateway */
#define         IROBEX_NO_SERVICE_RSP   0xD3U  /**< Service Unavailable */
#define         IROBEX_GATE_TIME_RSP    0xD4U  /**< Gateway Timeout */
#define         IROBEX_HTTP_RSP         0xD5U  /**< HTTP version not supported */
#define         IROBEX_DATA_FULL_RSP    0xE0U  /**< Database Full */
#define         IROBEX_DATA_LOCK_RSP    0xE1U  /**< Database Locked */
#define         IROBEX_FINAL_BIT_SET    0x80U
#define         IROBEX_CONTINUE_FINAL   0x91U
/*@}*/

 /* Internal errors defined for IROBEX */
/**
 * @name Internal errors defined for IROBEX
 *
 * Constant Definitions for Internal errors defined for IROBEX
 */
/*@{*/
#define       IROBEX_WHO_MISMATCH        0xF1U
#define       IROBEX_CONNECTID_MISMATCH  0xF2U
#define       IROBEX_BAD_HEADER          0xF3U
/*@}*/

/* define the operation mode */
/**
 * @name Operation mode
 *
 * Constant Definitions for Operation mode
 */
/*@{*/
#define        IROBEX_SERVER           1U
#define        IROBEX_CLIENT           2U
/*@}*/

/* Macro to set the IROBEX CB. This is only for NON_BLOCKING */
#define IROBEX_REGISTER_CB(session_id,cb) \
        (session_id)->irobex_notify_cb = (cb)

/** \} */

/**
 * \defgroup irobex_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
/*
 * Application requires this structure to create a list of headers
 * if necessery before calling any irobex request including connect
 */

typedef struct irobex_object_header
{
    /**
     *  Pointer to the header value. If the header value is 4 byte/
     *  1 byte value assign the address to hv;
     */
     union
     {
         UCHAR* value;
         UINT32 four_byte;
         UCHAR  byte;
     }hv;

    /**
     * Size of header value. if the header value is of type UINT32
     * size is equal to 4.
     */
    UINT16  size;

    /** Header Identifier */
    UCHAR hi;

    /** Dummy variable */
    UCHAR dummy;

}IROBEX_OBJ_HEADER;


/**
 * Irobex structure contains the identifier to indicate the irobex session
 * that had opened for the application. The bd_addr field indicates the
 * bd_addr of the peer device with which the obex session has been established.
 * The value of index should fall in the range of 0 to IROBEX_INSTANCES,
 * which is a tunable parameter.
 */
typedef struct irobex_session_id
{
    /**
     *  Upper Layer Callback Handler for IROBEX Notifications
     *  in Non-Blocking mode of operation.
     */
    API_RESULT (* irobex_notify_cb)
               (
                    UCHAR                      /* Event Identifier */,
                    struct irobex_session_id * /* IROBEX SessionID */,
                    UINT16                     /* Result/Response */,
                    IROBEX_OBJ_HEADER* /*  receiving header list */,
                    UCHAR /* header_num; number of tx/rx headers */,
                    UINT16 /* maximum transmitting size*/
               );

    /** Flags used in Connect & Setpath */
    UCHAR flag;

    /** Dummy variable */
    UCHAR dummy;

    /** Identifier for the obex session */
    UCHAR session_index;

    /** Server channel */
    UCHAR server_channel;

    /** BD_ADDR of the peer device */
    UCHAR bd_addr[BT_BD_ADDR_SIZE];

}IROBEX;


/** The IROBEX  Notification Callback Data Type (Non-Blocking Mode) */
typedef API_RESULT (* IROBEX_NOTIFY_CB)
               (
                    UCHAR                      /* Event Identifier */,
                    IROBEX * /* IROBEX SessionID */,
                    UINT16                     /* Result/Response */,
                    IROBEX_OBJ_HEADER* /*  receiving header list */,
                    UCHAR /* header_num; number of tx/rx headers */,
                    UINT16 /* maximum transmitting size*/
               );


/** \} */
/** \} */

/*----------------------------------  IrOBEX API's----------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/**
 * \defgroup irobex_api IROBEX API definitions
 * \{
 * Describes API definitions defined by the module.
 */
/*
 * Open a new irobex session. mode defines the the client/ server functionality
 * of the new session. who parameter value can set as NULL if the
 * profile doesn't require the direct connection. This API provides
 * a session handle to identify the opened irobex session and must used for all
 * the following transactions over this session.
 */
/**
 *  \brief To create a new IrOBEX session.
 *
 *  \par Description:
 *       This API allocates a new IrOBEX session for the application with client
 *       or server functionality as specified by the mode parameter. This API
 *       returns immediately after allocating a new session, in both Blocking
 *       and Non-Blocking modes.
 *
 *  \param [in] mode
 *         Functionality of the session i.e., 1=IROBEX_SERVER, 2=IROBEX_CLIENT
 *
 *  \param [in] who
 *         The Who header that is a string to identify the host Application.
 *         This value could be NULL for the application that does not require
 *         any Who value.
 *
 *  \param [in] length
 *         The length of the who header
 *
 *  \param [out] ir_session_id
 *         The Identifier for this session will be returned in the Handle
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note Who value is required if the server expects a target header in the
 *        CONNECT request Currently all profiles other than Object Push Profile
 *        requires the who value to be send as part of the CONNECT request,
 *        which can be used by the remote server for multiplexing if it is
 *        running multiple services on the same session.
 *        In Non-Blocking mode, IrOBEX will not indicate the application upon
 *        completion of this API.
 */
API_RESULT BT_irobex_session_open(IROBEX*  /* irobex_session Identifier */,
                                  UCHAR    /*client/server mode*/,
                                  UCHAR*   /* who value*/,
                                  UINT16   /* length of who value*/);


/*
 * close the irobex session. This must be the final API call
 * using the same session handle.
 */
/**
 *  \brief To close a IrOBEX session.
 *
 *  \par Description:
 *       This API closes the requested IrOBEX session. If transport connection
 *       exists, transport disconnection is initiated.  On successful completion
 *       of transport disconnection, IROBEX_CLOSE callback is called.
 *
 *  \param [in] ir_session_id
 *         The Identifier for the session to be closed
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note This must be the final API call using the same session handle.
 */
API_RESULT BT_irobex_session_close(IROBEX*  /* irobex session handle */);


/*
 * Register the bd addr, server channel and reception capability of the
 * application to  OBEX layer. A server session doesn't require the BD ADDR value,
 * hence this value can be NULL.
 */
/**
 *  \brief To configure and register the session
 *
 *  \par Description:
 *       This API is used to set the maximum receiving capability of the
 *       Application, remote Bluetooth Device Address and RFCOMM Server Channel.
 *       The IrOBEX Notification Callback must be populated with the handle
 *       using IROBEX_REGISTER_CB in case of Non-Blocking mode.
 *       This API returns immediately after allocating a new session,
 *       in both Blocking and Non-Blocking modes.
 *
 *  \param [in,out] ir_session_id
 *         IN parameters: Handle of a valid IrOBEX session.
 *         OUT parameters: The BD_ADDR and server_channel fields get modified as
 *                         per the Configuration parameters.
 *
 *  \param [in] max_rx_size
 *         Maximum data receiving capability of the Application. This value is
 *         negotiated as part of the connection.
 *
 *  \param [in] bd_addr
 *         Remote BD_ADDR. This value could be NULL for IrOBEX server session.
 *
 *  \param [in] server_channel
 *         A valid RFCOMM server channel for establishing the transport Connection.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note The BT_irobex_session_open() API should be called before calling this API.
 */
API_RESULT BT_irobex_register_service(IROBEX*  /* irobex session handle*/,
                                      UINT16   /* maximum receiving packet size */,
                                      UCHAR*   /* bd_addr */,
                                      UCHAR    /* server_channel*/);

/*
 * This API is to open the RFCOMM Transport Connection for the
 * IROBEX. For NON_BLOCKING mode this API shall be called before
 * Calling the Request/Read/Response APIs. For the Blocking mode
 * This call could be optional as Request/Read API will
 * Establish the connection if it does not exist
 */
/**
 *  \brief To establish the transport connection for  IrOBEX session
 *
 *  \par Description:
 *       This API establishes the Transport connection (RFCOMM channel) for the
 *       IrOBEX client session.
 *
 *  \param [in] ir_session_id
 *         Handle of a valid IrOBEX session.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note The BT_irobex_session_open() and BT_irobex_register_service() APIs
 *        must be call before calling this API.
 *        In Non-Blocking mode, IrOBEX calls the callback with event_id as
 *        IROBEX_OPEN upon completion of the request.
 */
API_RESULT  BT_irobex_open(IROBEX* ir_session_id);

/*
 * This API is required by the Client session to send an Irobex request.
 * All the transmitting headers  should be added to the transmitting
 * array of object headers and the header_num indicates the number of transmitting
 * headers. This API uses an array of object headers as of the  out parameter
 * and provide the list of headers that was recieved
 * in the response. header_num will get updated to the number of received headers.
 * The application must allocate the array for the tx & rx  header list.
 * An array of 16 can satify most of the requirement.
 * if the request is connect, this API also provides the  maximimum size of packet
 * that the application can send through irobex as one of the out parameters.
 */
/**
 *  \brief To Send an IrOBEX request
 *
 *  \par Description:
 *       This API is required by a client application to send the IrOBEX request
 *       to the server application.  The commonly used requests are Connect, Put,
 *       Get, Disconnect, Abort and Setpath. These IrOBEX opcodes are defined in
 *       BT_irobex_api.h Header file.
 *
 *  \param [in] ir_session_id
 *         Handle of a valid IrOBEX session.
 *
 *  \param [in] opcode
 *         Opcode for the IrOBEX request.
 *         The valid Opcode values are:
 *         - IROBEX_CONNECT
 *         - IROBEX_DISCONNECT
 *         - IROBEX_PUT
 *         - IROBEX_PUT_EOF
 *         - IROBEX_GET
 *         - IROBEX_GET_FINAL
 *         - IROBEX_SETPATH
 *         - IROBEX_ABORT
 *
 *  \param [in] flags
 *         This parameter is required only for IROBEX_SETPATH request. The flag values for connect must be 0x00. The IROBEX_SETPATH request uses the following values:
 *         - IROBEX_SETPATH_CREAT
 *         - IROBEX_SETPATH_CURRENT
 *         - IROBEX_SETPATH_FORWARD
 *         - IROBEX_SETPATH_BACKWARD
 *
 *  \param [in] tx_header
 *         Pointer to the list of transmitting headers. The list could be an
 *         array of IROBEX_OBJECT_HEADER objects.
 *
 *  \param [in, out] header_num
            IN: User allocated buffer or array to store the list of received headers.
 *         OUT: Number of headers in the transmitting list
 *
 *  \param [in, out]
            IN: Size of the rx_header buffer
 *         OUT: Pointer to the received list of headers.
 *
 *  \param [out] max_trans_size
 *         This value is significant in case of connect request, which indicates
 *         the maximum size of IrOBEX packet.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note Upon completion of the transaction in Non-Blocking mode, the IrOBEX
 *        Notification Callback function is called with the corresponding opcode
 *        as the event identifier.
 */
API_RESULT  BT_irobex_request(IROBEX*  /* irobex session handle */,
                              UCHAR    /* opcode */,
                              UCHAR    /* flags required for connect & setpath */,
                              IROBEX_OBJ_HEADER* /* transmitting header list */,
                              IROBEX_OBJ_HEADER* /*  receiving header list */,
                              UCHAR* /* header_num; number of tx/rx headers */,
                              UINT16* /* maximum transmitting size*/);

/*
 * Server session use this API to send response for the request that had already
 * received. All the headers required for the response should  be added to
 * the transmitting array of object headers and the number of headers indicates
 * the total number of headers added to it. An array of 16 object headers
 * can satisfy most of the requirement.
 */
/**
 *  \brief To send response for the received request.
 *
 *  \par Description:
 *       This API is required by the server application to send the response for
 *       the received IrOBEX request.
 *
 *  \param [in] ir_session_id
 *         Handle of a valid IrOBEX session.
 *
 *  \param [in] tx_header
 *         Pointer to the list of transmitting headers. The list could be an array
 *
 *  \param [in] header_num
 *         Number of headers in the transmitting list
 *
 *  \param [in] req_code
 *         Opcode of the received IrOBEX request
 *
 *  \param [in] resp_code
 *         Opcode for the IrOBEX response.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note This API should call after receiving an IrOBEX request from any client session.
 *        Use of this API is same for blocking as well as Non-Blocking.
 */
API_RESULT BT_irobex_send_response(IROBEX*            /* session handle */,
                                   IROBEX_OBJ_HEADER* /* tx header list */,
                                   UCHAR              /* number of tx headrs */,
                                   UCHAR              /* recieved request */,
                                   UCHAR              /* response for sending */);



/* ------------------Authentication API's ------------------------- */

/**
 *  \brief To generate a challenge string for authentication.
 *
 *  \par Description:
 *       This API can be used to generate digest challenge string for the
 *       Authentication challenge header that is required for IrOBEX
 *       Authentication.
 *
 *  \param [out] challenge
 *         Digest challenge string
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT  BT_irobex_generate_digest_challenge(UCHAR* /* challenge */);
/**
 *  \brief To send a response to an authentication challenge request.
 *
 *  \par Description:
 *       This API can be used to generate digest response string from the
 *       received challenge for the Authentication response header that is
 *       required for IrOBEX Authentication.
 *
 *  \param [in] challenge
 *         Digest challenge string received from the remote end.
 *
 *  \param [in] passwd
 *         Shared password required for authentication.
 *
 *  \param [in] length
 *         The length of the password.
 *
 *  \param [out] response
 *         Digest response string generated using md5 algorithm.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT  BT_irobex_generate_digest_response(UCHAR* /* challenge */,
                                               UCHAR* /* response */,
                                               UCHAR* /* password */,
                                               UCHAR  /* length */);
/**
 *  \brief To check if the remote IrOBEX session is authenticated or not.
 *
 *  \par Description:
 *       This API is required to check whether the remote IrOBEX application is
 *       authenticated or not.
 *
 *  \param [in] challenge
 *         Transmitted Digest challenge string.
 *
 *  \param [in] response
 *         Digest response string received from the remote end.
 *
 *  \param [in] passwd
 *         Shared password required for authentication.
 *
 *  \param [in] length
 *         Length of the password.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT  BT_irobex_check_authentication(UCHAR*   /* challenge */,
                                           UCHAR*   /* response */,
                                           UCHAR*   /* password */,
                                           UCHAR    /* length */);

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
#endif /* _H_BT_IROBEX_API_ */

