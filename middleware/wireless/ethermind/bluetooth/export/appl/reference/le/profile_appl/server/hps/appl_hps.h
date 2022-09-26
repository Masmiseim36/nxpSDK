
/**
 *  \file appl_hps.h
 *
 *  Application Header File for HTTP Proxy Service.
 */

/*
 *  Copyright (C) 2016. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_HPS_
#define _H_APPL_HPS_

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

#include "gatt_db.h"

/* ------------------------------------------------------------ Macros */
#define APPL_HTTP_GET_REQ                           0x01
#define APPL_HTTP_HEAD_REQ                          0x02
#define APPL_HTTP_POST_REQ                          0x03
#define APPL_HTTP_PUT_REQ                           0x04
#define APPL_HTTP_DELETE_REQ                        0x05
#define APPL_HTTPS_GET_REQ                          0x06
#define APPL_HTTPS_HEAD_REQ                         0x07
#define APPL_HTTPS_POST_REQ                         0x08
#define APPL_HTTPS_PUT_REQ                          0x09
#define APPL_HTTPS_DELETE_REQ                       0x0A
#define APPL_HTTP_REQ_CANCEL                        0x0B

/* HPS relared Error codes */
#define APPL_HPS_INVALID_REQUEST                    0x81
#define APPL_HPS_NETWORK_NOT_AVAILABLE              0x82

#define APPL_HPS_MAX_HTTP_PROXY_DATA                512
#define APPL_HPS_LARGE_HTTP_RSP_DATA_LEN            600

#define APPL_HPS_CONFIGURED_STATE                   0x07

#define APPL_HPS_STATUS_CODE_LEN                    3
#define APPL_HPS_STATUS_CODE_INDEX                  9

/* Utility Macros */
#define APPL_HPS_SUPPORTED_OPCODE(op)\
        (((APPL_HTTP_REQ_CANCEL >= (op)) &&\
        (APPL_HTTP_GET_REQ <= (op)))? BT_TRUE : BT_FALSE)

/* Control Point Procedure Timeout: Default 30 Seconds */
#define APPL_HPS_CP_PROC_TIMEOUT_VAL                30

/* List of HTTP response status codes */
#define HTTP_RSP_STATUS_CODE_OK                     200
#define HTTP_RSP_STATUS_CODE_RESRC_NOT_FOUND        404
#define HTTP_RSP_STATUS_CODE_METHOD_NOT_ALLOWED     405
#define HTTP_RSP_STATUS_CODE_CONTINUE               100
#define HTTP_RSP_STATUS_CODE_SWITCHG_PROTOCOL       101
#define HTTP_RSP_STATUS_CODE_MOVED_PERMTLY          301
#define HTTP_RSP_STATUS_CODE_FOUND                  302
#define HTTP_RSP_STATUS_CODE_SEE_OTHER              303
#define HTTP_RSP_STATUS_CODE_USE_PROXY              305
#define HTTP_RSP_STATUS_CODE_TEMP_REDIRECTED        307
#define HTTP_RSP_STATUS_CODE_BAD_REQUEST            400
#define HTTP_RSP_STATUS_CODE_REQ_TIMEDOUT           504


/* List of HTTP response Data Status Codes */
#define HTTP_RSP_DATA_STATUS_HEADER_RECEIVED        0x01
#define HTTP_RSP_DATA_STATUS_HEADER_TRUNCATED       0x02
#define HTTP_RSP_DATA_STATUS_BODY_RECEIVED          0x04
#define HTTP_RSP_DATA_STATUS_BODY_TRUNCATED         0x08

#define APPL_HPS_HTTP_GET_REQ_LEN                   3
#define APPL_HPS_HTTP_PUT_REQ_LEN                   3
#define APPL_HPS_HTTP_HEAD_REQ_LEN                  4
#define APPL_HPS_HTTP_POST_REQ_LEN                  4
#define APPL_HPS_HTTP_DELETE_REQ_LEN                6
#define APPL_HPS_HTTP_CANCEL_REQ_LEN                6

#define APPL_HPS_MAX_RSP_PARAM_LEN                  15

#define APPL_HPS_MAX_IP_ADDR_LEN                    50
#define APPL_HPS_MAX_URI_LEN                        100

#define APPL_HPS_GET_CUR_MTU(h) \
        appl_hps_get_curr_mtu((h))

/* --------------------------------------------- Structures */
typedef struct _APPL_HPS_CNTRL_POINT_RSP_PARAM_
{
    APPL_HANDLE appl_handle;
    UCHAR       rsp_param[APPL_HPS_MAX_RSP_PARAM_LEN];
    UINT16      rsp_param_len;
}APPL_HPS_CNTRL_POINT_RSP_PARAM;


typedef struct _APPL_HPS_HTTP_REQUEST_PARAMS_
{
    UCHAR  uri             [APPL_HPS_MAX_HTTP_PROXY_DATA];
    UINT16 uri_len;
    UCHAR  http_headers    [APPL_HPS_MAX_HTTP_PROXY_DATA];
    UINT16 http_headers_len;
    UCHAR  http_entity_body[APPL_HPS_MAX_HTTP_PROXY_DATA];
    UINT16 http_entity_body_len;
}APPL_HPS_HTTP_REQUEST_PARAMS;

typedef struct _APPL_TYPE_OF_HTTP_REQUESTS_
{
    UCHAR http_get_req   [APPL_HPS_HTTP_GET_REQ_LEN];
    UCHAR http_head_req  [APPL_HPS_HTTP_HEAD_REQ_LEN];
    UCHAR http_post_req  [APPL_HPS_HTTP_POST_REQ_LEN];
    UCHAR http_put_req   [APPL_HPS_HTTP_PUT_REQ_LEN];
    UCHAR http_delete_req[APPL_HPS_HTTP_DELETE_REQ_LEN];
    UCHAR http_cancel_req[APPL_HPS_HTTP_CANCEL_REQ_LEN];
}APPL_TYPE_OF_HTTP_REQUESTS;

typedef struct _APPL_TYPE_OF_HTTPS_REQUESTS_
{
    UCHAR https_get_req   [APPL_HPS_HTTP_GET_REQ_LEN];
    UCHAR https_head_req  [APPL_HPS_HTTP_HEAD_REQ_LEN];
    UCHAR https_post_req  [APPL_HPS_HTTP_POST_REQ_LEN];
    UCHAR https_put_req   [APPL_HPS_HTTP_PUT_REQ_LEN];
    UCHAR https_delete_req[APPL_HPS_HTTP_DELETE_REQ_LEN];
}APPL_TYPE_OF_HTTPS_REQUESTS;

/* HPS Database Signature */
/* NOTE: Currently having a dummy signature where
 * - the 2 Upper Bytes are 0x1234
 * - the 2 Lower Bytes are HPS UUID(0x1823)
 * Application can have any logic for choosing 4Bytes of signature
 */
#ifdef GATT_DB_HAVE_DB_SIGNATURE
#define APPL_HPS_GATT_DB_SIGN                     (0x12340000U | GATT_HPS_SERVICE)
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

/* --------------------------------------------- Functions */
void appl_hps_init(void);
void appl_hps_bt_init(void);
void appl_hps_bt_deinit(void);

API_RESULT gatt_db_hps_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           );

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_hps_gatt_db_register(void);
#else
#define appl_hps_gatt_db_register()
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

void appl_hps_connect(DEVICE_HANDLE  * dq_handle);
void appl_manage_trasnfer (GATT_DB_HANDLE handle, UINT16 config);
void appl_hps_server_reinitialize (void);
void appl_hsc_cnfg_handler (UINT16 config);
API_RESULT appl_hps_cp_handler
           (
               GATT_DB_HANDLE  * handle,
               ATT_VALUE       * value,
               UCHAR             hps_config_state
           );
API_RESULT appl_hps_get_hostname
           (
              APPL_HPS_HTTP_REQUEST_PARAMS * http_req,
              UCHAR * type_of_http_req,
              UCHAR   type_of_http_req_len,
              UCHAR * ip_address
           );
API_RESULT appl_hps_write_char_value
           (
               ATT_ATTR_HANDLE  handle,
               ATT_VALUE        * att_value,
               UINT16           offset
           );
void appl_hps_request_params_init(void);
API_RESULT appl_hps_cb
           (
               UCHAR  * data,
               UINT16   data_len
           );
void appl_hps_send_http_status_code
    (
        APPL_HANDLE * handle,
        UINT16      rsp_status
    );
API_RESULT appl_hps_set_http_header_char_val
           (
               UCHAR   * http_rsp_recv,
               UINT16  http_rsp_recv_len,
               UINT16  * offset
           );
API_RESULT appl_hps_set_http_entity_body_char_val
           (
               UCHAR   * http_rsp_recv,
               UINT16    http_rsp_recv_len
           );
API_RESULT appl_hps_read_char_value
           (
               GATT_DB_HANDLE  * handle,
               ATT_ATTR_HANDLE attr_handle,
               UINT16          offset,
               UCHAR           is_rd_blob
           );
API_RESULT appl_hps_send_err_rsp
           (
               GATT_DB_HANDLE   * handle,
               ATT_ATTR_HANDLE   attr_hndl
           );
API_RESULT appl_hps_send_read_rsp
           (
               GATT_DB_HANDLE   * handle,
               ATT_ATTR_HANDLE   attr_hndl
           );
API_RESULT appl_hps_send_read_blob_rsp
           (
               GATT_DB_HANDLE   * handle,
               ATT_ATTR_HANDLE   attr_hndl,
               UINT16            offset
           );
void appl_hps_set_network_availability(UCHAR flag);
void appl_hps_set_gap_central_role(void);
API_RESULT appl_hps_init_socket_to_http_server(UCHAR * ipaddr);
API_RESULT appl_hps_handle_cp_req
           (
               APPL_HPS_HTTP_REQUEST_PARAMS * http_req,
               UCHAR                        * type_of_http_req,
               UCHAR                        type_of_http_req_len
           );
UINT16 appl_hps_get_curr_mtu(ATT_HANDLE * handle);

void appl_hps_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     );
void appl_hps_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     );
void appl_hps_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     );
API_RESULT appl_hps_cp_start_timer(void);
API_RESULT appl_hps_cp_stop_timer(void);
void appl_hps_cp_timer_expiry_handler (void *data_param, UINT16 datalen);

/* Profile handling */
/* Profile Initialization */
#define APPL_PROFILE_INIT() appl_hps_init()
#define APPL_PROFILE_BT_INIT() appl_hps_bt_init()
#define APPL_PROFILE_BT_DEINIT() appl_hps_bt_deinit()

/* Profile Connection */
#define APPL_PROFILE_CONNECT(x) \
        appl_hps_connect(&APPL_GET_DEVICE_HANDLE((*x)))

/* Profile Trigger for Measurements */
#define APPL_SEND_MEASUREMENT(x)

/* Profile Disconnect Handler */
#define APPL_PROFILE_DISCONNECT_HANDLER(x)

/* Transport Disconnect Handler */
#define APPL_TRANSPORT_DISCONNECT_HANDLER(x, r) appl_hps_server_reinitialize()

/* GATT Database Specific Handler */
#define GATT_DB_PROFILE_HANDLER(h, p)                      \
        gatt_db_hps_gatt_char_handler((h),(p))

/* Handle Value Notification Complete Handler */
#define APPL_PROFILE_HV_NTF_COMPLETE_HANDLER(x,y,z)\
        appl_hps_handle_ntf_complete((x),(y),(z))

/* Handle Value Indication Complete Handler */
#define APPL_PROFILE_HV_IND_COMPLETE_HANDLER(x,y)\
         appl_hps_handle_ind_complete((x),(y))

/* Profile ATT MTU Update Complete Handler */
#define APPL_PROFILE_MTU_UPDT_COMPLETE_HANDLER(x,y)\
        appl_hps_handle_mtu_update_complete((x),(y))

#define APPL_USE_IDLE_TIMER
#define APPL_IDLE_TIMEOUT           30

#endif /* _H_APPL_HPS_ */

