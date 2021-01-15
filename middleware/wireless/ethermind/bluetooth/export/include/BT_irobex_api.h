
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

/* Standard header identifiers for irobex objects */

#define         IROBEX_HDR_COUNT                   0xc0
#define         IROBEX_HDR_NAME                    0x01
#define         IROBEX_HDR_TYPE                    0x42
#define         IROBEX_HDR_LENGTH                  0xc3
#define         IROBEX_HDR_TIME_ISO                0x44
#define         IROBEX_HDR_TIME                    0xc4
#define         IROBEX_HDR_DESCRIPTION             0x05
#define         IROBEX_HDR_TARGET                  0x46
#define         IROBEX_HDR_HTTP                    0x47
#define         IROBEX_HDR_BODY                    0x48
#define         IROBEX_HDR_END_BODY                0x49
#define         IROBEX_HDR_WHO                     0x4A
#define         IROBEX_HDR_CONNECT_ID              0xCB
#define         IROBEX_HDR_APP_PARAM               0x4C
#define         IROBEX_HDR_AUTH_CHALLENGE          0x4D
#define         IROBEX_HDR_AUTH_RESPONSE           0x4E
#define         IROBEX_HDR_OBJECT_CLASS            0x4F



/*
 * The below definitions are used as event Ids
 * in the call back function.
 */
#define         IROBEX_OPEN             0x60
#define         IROBEX_CLOSE            0x61

/*
 *  Irobex request opcodes, which is also used in the
 *  Irobex request packet other than using as the
 *  Event IDs
 */

#define         IROBEX_CONNECT          0x80
#define         IROBEX_DISCONNECT       0x81
#define         IROBEX_PUT              0x02
#define         IROBEX_PUT_EOF          0x82
#define         IROBEX_GET              0x03
#define         IROBEX_GET_FINAL        0x83
#define         IROBEX_RESERVED         0x04
#define         IROBEX_SETPATH          0x85
#define         IROBEX_ABORT            0xFF

/*
 * Irobex response code
 */


#define         IROBEX_CONTINUE_RSP     0x90  /* Continue */
#define         IROBEX_SUCCESS_RSP      0xA0  /* OK, Success */
#define         IROBEX_CREATED_RSP      0xA1  /* Created */
#define         IROBEX_ACCEPTED_RSP     0xA2  /* Accepted */
#define         IROBEX_NON_AUTH_RSP     0xA3  /* Non Authorative information */
#define         IROBEX_NO_CONTENT_RSP   0xA4  /* No content */
#define         IROBEX_RESET_CONT_RSP   0xA5  /* Reset Content */
#define         IROBEX_PART_CONT_RSP    0xA6  /* Partial Content */
#define         IROBEX_MULTI_CHOICE_RSP 0xB0  /* Multiple Choices */
#define         IROBEX_MOVE_P_RSP       0xB1  /* Moved Permenently */
#define         IROBEX_MOVE_T_RSP       0xB2  /* Moved temporarily  */
#define         IROBEX_SEE_OTHER_RSP    0xB3  /* See Other */
#define         IROBEX_NOT_MODI_RSP     0xB4  /* Not Modified */
#define         IROBEX_USE_PROXY        0xB5  /* Use proxy */
#define         IROBEX_BAD_REQ_RSP      0xC0  /* Bad Request */
#define         IROBEX_UNAUTH_RSP       0xC1  /* Unathorized */
#define         IROBEX_PAY_RSP          0xC2  /* Payment required */
#define         IROBEX_FORBIDDEN_RSP    0xC3  /* Forbidden ; operation refused*/
#define         IROBEX_NO_FOUND_RSP     0xC4  /* Not found */
#define         IROBEX_METHOD_RSP       0xC5  /* Method Not allowed */
#define         IROBEX_NOT_ACCEPT_RSP   0xC6  /* Not Acceptable */
#define         IROBEX_PROXY_AUTH_RSP   0xC7  /* Proxy Authentication Required */
#define         IROBEX_TIME_OUT_RSP     0xC8  /* Request Time out */
#define         IROBEX_CONFLICT_RSP     0xC9  /* Conflict */
#define         IROBEX_GONE_RSP         0xCA  /* Gone */
#define         IROBEX_LENGTH_RSP       0xCB  /* Length required */
#define         IROBEX_PRECOND_RSP      0xCC  /* Precondition Failed */
#define         IROBEX_REQ_ENTITY_RSP   0xCD  /* Requested entity too large */
#define         IROBEX_REQ_URL_RSP      0xCE  /* Requested URL too large */
#define         IROBEX_MEDIA_RSP        0xCF  /* Unsupported media type */
#define         IROBEX_SERVER_ERR_RSP   0xD0  /* Internal Server error */
#define         IROBEX_NOT_IMP_RSP      0xD1  /* Not implemented */
#define         IROBEX_BAD_GATE_RSP     0xD2  /* Bad Gateway */
#define         IROBEX_NO_SERVICE_RSP   0xD3  /* Service Unavailable */
#define         IROBEX_GATE_TIME_RSP    0xD4  /* Gateway Timeout */
#define         IROBEX_HTTP_RSP         0xD5  /* HTTP version not supported */
#define         IROBEX_DATA_FULL_RSP    0xE0  /* Database Full */
#define         IROBEX_DATA_LOCK_RSP    0xE1  /* Database Locked */
#define         IROBEX_FINAL_BIT_SET    0x80
#define         IROBEX_CONTINUE_FINAL   0x91


 /* Internal errors defined for IROBEX */

#define       IROBEX_WHO_MISMATCH        0xF1
#define       IROBEX_CONNECTID_MISMATCH  0xF2
#define       IROBEX_BAD_HEADER          0xF3


/* define the operation mode */
#define        IROBEX_SERVER           1
#define        IROBEX_CLIENT           2


/* Macro to set the IROBEX CB. This is only for NON_BLOCKING */
#define IROBEX_REGISTER_CB(session_id,cb) \
        (session_id)->irobex_notify_cb = (cb)

/*
 * Application requires this structure to create a list of headers
 * if necessery before calling any irobex request including connect
 */

typedef struct irobex_object_header
{
    /*
     *  pointer to the header value. If the header value is 4 byte/
     *  1 byte value assign the address to hv;
     */
     union
     {
         UCHAR* value;
         UINT32 four_byte;
         UCHAR  byte;
     }hv;

    /*
     * size of header value. if the header value is of type UINT32
     * size is equal to 4.
     */
    UINT16  size;

    /* header Identifier */
    UCHAR hi;

    /* Dummy variable */
    UCHAR dummy;

}IROBEX_OBJ_HEADER;


/*
 * Irobex structure contains the identifier to indicate the irobex session
 * that had opened for the application. The bd_addr field indicates the
 * bd_addr of the peer device with which the obex session has been established.
 * The value of index should fall in the range of 0 to IROBEX_INSTANCES,
 * which is a tunable parameter.
 */

typedef struct irobex_session_id
{
    /*
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

    /* Flags used in Connect & Setpath */
    UCHAR flag;

    /* Dummy variable */
    UCHAR dummy;

    /* Identifier for the obex session */
    UCHAR session_index;

    /* Server channel */
    UCHAR server_channel;

    /* BD_ADDR of the peer device */
    UCHAR bd_addr[BT_BD_ADDR_SIZE];

}IROBEX;


/*The IROBEX  Notification Callback Data Type (Non-Blocking Mode)*/
typedef API_RESULT (* IROBEX_NOTIFY_CB)
               (
                    UCHAR                      /* Event Identifier */,
                    IROBEX * /* IROBEX SessionID */,
                    UINT16                     /* Result/Response */,
                    IROBEX_OBJ_HEADER* /*  receiving header list */,
                    UCHAR /* header_num; number of tx/rx headers */,
                    UINT16 /* maximum transmitting size*/
               );




/*----------------------------------  IrOBEX API's----------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Open a new irobex session. mode defines the the client/ server functionality
 * of the new session. who parameter value can set as NULL if the
 * profile doesn't require the direct connection. This API provides
 * a session handle to identify the opened irobex session and must used for all
 * the following transactions over this session.
 */
API_RESULT BT_irobex_session_open(IROBEX*  /* irobex_session Identifier */,
                                  UCHAR    /*client/server mode*/,
                                  UCHAR*   /* who value*/,
                                  UINT16   /* length of who value*/);


/*
 * close the irobex session. This must be the final API call
 * using the same session handle.
 */
API_RESULT BT_irobex_session_close(IROBEX*  /* irobex session handle */);


/*
 * Register the bd addr, server channel and reception capability of the
 * application to  OBEX layer. A server session doesn't require the BD ADDR value,
 * hence this value can be NULL.
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
API_RESULT BT_irobex_send_response(IROBEX*            /* session handle */,
                                   IROBEX_OBJ_HEADER* /* tx header list */,
                                   UCHAR              /* number of tx headrs */,
                                   UCHAR              /* recieved request */,
                                   UCHAR              /* response for sending */);



/* ------------------Authentication API's ------------------------- */


API_RESULT  BT_irobex_generate_digest_challenge(UCHAR* /* challenge */);

API_RESULT  BT_irobex_generate_digest_response(UCHAR* /* challenge */,
                                               UCHAR* /* response */,
                                               UCHAR* /* password */,
                                               UCHAR  /* length */);

API_RESULT  BT_irobex_check_authentication(UCHAR*   /* challenge */,
                                           UCHAR*   /* response */,
                                           UCHAR*   /* password */,
                                           UCHAR    /* length */);

#ifdef __cplusplus
};
#endif

#endif /* _H_BT_IROBEX_API_ */

