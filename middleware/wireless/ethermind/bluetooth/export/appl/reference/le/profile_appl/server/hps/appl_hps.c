
/**
 *  \file appl_hps.c
 *
 *  This file contains the Sample Profile application..
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

/** HPS Platform Abstraction file include */
#include "hps_pl.h"
#include "https_pl.h"

/* #define APPL_HPS_HAVE_DATA_DUMP */

#if (defined ATT && defined HPS)

/* ------------------------------------------------------------ Macros */
#define APPL_HPS_HOST_TAG_LEN                           4U

/* --------------------------------------------- External Global Variables */
extern void gatt_db_hps_config_state_reset(void);

/* --------------------------------------------- Exported Global Variables */
API_RESULT appl_hps_construct_http_req
           (
                APPL_HPS_HTTP_REQUEST_PARAMS * http_request_params,
                UCHAR                        * type_of_http_req,
                UCHAR                        type_of_http_req_len
           );

/* --------------------------------------------- Static Global Variables */
/* The value of this corresponds to "Host" */
DECL_CONST UCHAR appl_hps_host_tag_val[APPL_HPS_HOST_TAG_LEN]  = {0x48U, 0x6fU, 0x73U, 0x74U};

static BT_timer_handle timer_handle;

static GATT_DB_HANDLE  appl_hps_uri_db_handle;
static GATT_DB_HANDLE  appl_hps_http_headers_db_handle;
static GATT_DB_HANDLE  appl_hps_http_entity_body_db_handle;
static GATT_DB_HANDLE  appl_hps_http_status_code_db_handle;
static GATT_DB_HANDLE  appl_hps_http_cp_db_handle;
static GATT_DB_HANDLE  appl_hps_https_security_db_handle;

#define APPL_HPS_INVALID_HANDLE 0xFFFFU

static UCHAR  appl_hps_cntrl_pnt_enabled;
static UCHAR  appl_hps_curr_cntrl_pnt_op;
static UCHAR  appl_hps_http_req_cancel_set;
static UCHAR  appl_hps_in_err_state;
static UCHAR  appl_hps_network_not_available_set;
static UCHAR  appl_hps_status_code_recvd;
static UCHAR  appl_hps_transport_conn_state;
static UINT16 appl_hps_header_length;
static UINT16 appl_hps_msg_entity_len;
static UINT16 appl_hps_uri_length;
static UCHAR  appl_hps_data_status;

static UCHAR  appl_hps_ip_address[APPL_HPS_MAX_IP_ADDR_LEN];

static ATT_ATTR_HANDLE appl_hps_uri_hndl;
static ATT_ATTR_HANDLE appl_hps_http_headers_hndl;
static ATT_ATTR_HANDLE appl_hps_http_entity_bdy_hndl;
static ATT_ATTR_HANDLE appl_hps_http_status_code_hndl;
static ATT_ATTR_HANDLE appl_hps_https_security_hndl;
static APPL_HPS_HTTP_REQUEST_PARAMS http_req_params;

UCHAR http_rsponse_val_len;

APPL_HANDLE appl_handle;

ATT_VALUE http_rsponse_val;

static APPL_TYPE_OF_HTTP_REQUESTS types_of_http_req =
                                  {
                                      /* GET */
                                      {0x47U, 0x45U, 0x54U},
                                      /* HEAD */
                                      {0x48U, 0x45U, 0x41U, 0x44U},
                                      /* POST */
                                      {0x50U, 0x4FU, 0x53U, 0x54U},
                                      /* PUT */
                                      {0x50U, 0x55U, 0x54U},
                                      /* DELETE */
                                      {0x44U, 0x45U, 0x4CU, 0x45U, 0x54U, 0x45U},
                                      /* CANCEL */
                                      {0x43U, 0x41U, 0x4EU, 0x43U, 0x45U, 0x4CU},
                                  };

static UCHAR appl_hps_is_https_req = BT_FALSE;

/* --------------------------------------------- Functions */


void appl_hps_set_https_security_val(UCHAR sec_state)
{
    API_RESULT   retval;
    ATT_VALUE    value;

    value.val = &sec_state;
    value.len = 1U;

    /* Set HTTPS Security State */
    retval = BT_gatt_db_set_char_val
             (
                 &appl_hps_https_security_db_handle,
                 &value
             );

    if (API_FAILURE != retval)
    {
        APPL_TRC(
        "[HPS]: Successfully updated HTTPS Security Char Value to %s\n",
        (BT_TRUE == sec_state) ? "VALID" : "NOT-VALID");
    }
    else
    {
        APPL_TRC(
        "[ERR]: Failed to update  HTTPS Security Char Value, reason 0x%04X\n",
        retval);
    }
}

void appl_hps_init(void)
{
#ifdef GATT_DB_DYNAMIC
#ifdef GATT_DB_HAVE_DB_SIGNATURE
    GATT_DB_SIGNATURE appl_hps_db_sign;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */
    API_RESULT      retval = API_FAILURE;
#endif /* GATT_DB_DYNAMIC */

#ifdef GATT_DB_DYNAMIC
    (BT_IGNORE_RETURN_VALUE) appl_hps_add_gaps();
    (BT_IGNORE_RETURN_VALUE) appl_hps_add_gatts();
    (BT_IGNORE_RETURN_VALUE) appl_hps_add_bas();
    (BT_IGNORE_RETURN_VALUE) appl_hps_add_dis();
    (BT_IGNORE_RETURN_VALUE) appl_hps_add_hps();

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* Set the HPS GATT DB Signature */
    appl_hps_db_sign = APPL_HPS_GATT_DB_SIGN;
    retval = BT_gatt_db_dyn_set_db_signature(appl_hps_db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
    "[HPS]: GATT Database Registration Status: 0x%04X\n", retval);

    /* Fetch and update the Maximum Attribute count in GATT DB */
    GATT_DB_MAX_ATTRIBUTES = BT_gatt_db_get_attribute_count();
#endif /* GATT_DB_DYNAMIC */

    /* Populate the GATT DB HANDLE for HTTP URI */
    appl_hps_uri_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hps_uri_db_handle.service_id = (UCHAR)GATT_SER_HPS_HTTP_PROXY_INST;
    appl_hps_uri_db_handle.char_id    = (UCHAR)GATT_CHAR_HPS_HTTP_URI_INST;

    /* Populate the GATT DB HANDLE for HTTP Headers */
    appl_hps_http_headers_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hps_http_headers_db_handle.service_id = (UCHAR)GATT_SER_HPS_HTTP_PROXY_INST;
    appl_hps_http_headers_db_handle.char_id    = (UCHAR)GATT_CHAR_HPS_HTTP_HEADERS_INST;

    /* Populate the GATT DB HANDLE for HTTP ENtity Body */
    appl_hps_http_entity_body_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hps_http_entity_body_db_handle.service_id = (UCHAR)GATT_SER_HPS_HTTP_PROXY_INST;
    appl_hps_http_entity_body_db_handle.char_id    = (UCHAR)GATT_CHAR_HPS_HTTP_ENTITY_BODY_INST;

    /* Populate the GATT DB HANDLE for HTTP status */
    appl_hps_http_status_code_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hps_http_status_code_db_handle.service_id = (UCHAR)GATT_SER_HPS_HTTP_PROXY_INST;
    appl_hps_http_status_code_db_handle.char_id    = (UCHAR)GATT_CHAR_HPS_HTTP_STATUS_CODE_INST;

    /* Populate the GATT DB HANDLE for HTTP control point */
    appl_hps_http_cp_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hps_http_cp_db_handle.service_id = (UCHAR)GATT_SER_HPS_HTTP_PROXY_INST;
    appl_hps_http_cp_db_handle.char_id    = (UCHAR)GATT_CHAR_HPS_HTTP_CONTROL_POINT_INST;

    /* Populate the GATT DB HANDLE for HTTP Security */
    appl_hps_https_security_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hps_https_security_db_handle.service_id = (UCHAR)GATT_SER_HPS_HTTP_PROXY_INST;
    appl_hps_https_security_db_handle.char_id    = (UCHAR)GATT_CHAR_HPS_HTTP_SECURITY_INST;

#ifndef GATT_DB_DYNAMIC
    appl_hps_gatt_db_register();
#endif /* GATT_DB_DYNAMIC */

    /* Register HPS GATT DB Handler with PL Extension */
    gatt_db_init_pl(gatt_db_hps_gatt_char_handler);

#ifdef BT_DUAL_MODE
    /* Update GAP Service Range in the SDP Records */
    appl_set_gatt_service_in_sdp_record
    (
        (UCHAR)GATT_SER_HPS_GAP_INST,
        DB_RECORD_GAP
    );
    /* Update GATT Service Range in the SDP Records */
    appl_set_gatt_service_in_sdp_record
    (
        (UCHAR)GATT_SER_HPS_GATT_INST,
        DB_RECORD_GATT
    );
            /* Update BAS Service Range in the SDP Records */
    appl_set_gatt_service_in_sdp_record
    (
        (UCHAR)GATT_SER_HPS_BATTERY_INST,
        DB_RECORD_BAS
    );
    /* Update BAS Service Range in the SDP Records */
    appl_set_gatt_service_in_sdp_record
    (
        (UCHAR)GATT_SER_HPS_DEV_INFO_INST,
        DB_RECORD_DIS
    );
    /* Update HPS Service Range in SDP Records */
    appl_set_gatt_service_in_sdp_record
    (
        (UCHAR)GATT_SER_HPS_HTTP_PROXY_INST,
        DB_RECORD_HPS
    );
#endif

    /* Initialize the HPS Transport PL Interface */
    hps_transport_init_pl(appl_hps_cb);
    https_transport_init_pl(appl_hps_cb);

    /**
     * Default GAP Role is Central for this Service.
     * But, during init time setting the role to Peripheral.
     * This needs to be updated from the HPS Menu after the
     * desired Peer Device Address to which this needs to scan and
     * initiate connection is Set.
     * User needs to select the role from the Menu.
     */
    /* NOTE: Printing the Above Text in Application */
    APPL_TRC(
    "\n=====================================================================\n"
    "                          NOTE to USER\n"
    "                          ----------\n");
    APPL_TRC (
    "Default GAP Role is GAP Central for this HPS Service. But, during\n"
    "init time setting the role to Peripheral. This needs to be updated\n"
    "from the HPS Menu after the desired Peer Device Address to which this\n"
    "service needs to scan and auto initiate connection is Set.\n"
    "\n        ** User needs to select the role from the HPS Menu **");
    APPL_TRC(
    "\n=====================================================================\n\n");

    /* To check all the set but unused variables */
    BT_IGNORE_UNUSED_PARAM(appl_hps_http_cp_db_handle);
}

void appl_hps_bt_init(void)
{
    APPL_TRC("[HPS]: In appl_hps_bt_init\n");

     APPL_SET_GAP_PERIPHERAL_ROLE();

    /* Invoke common reinitialize routine used after each disconnection */
    appl_hps_server_reinitialize ();
}

void appl_hps_bt_deinit(void)
{
    APPL_TRC("[HPS]: In appl_hps_bt_deinit\n");

    /* Stop Timer */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer(timer_handle);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }
}

void appl_hps_connect(DEVICE_HANDLE  * dq_handle)
{
    ATT_VALUE         value;
    UINT16            cli_cnfg;

    cli_cnfg = 0U;

    appl_hps_uri_db_handle.device_id              = (*dq_handle);
    appl_hps_http_headers_db_handle.device_id     = (*dq_handle);
    appl_hps_http_entity_body_db_handle.device_id = (*dq_handle);
    appl_hps_http_status_code_db_handle.device_id = (*dq_handle);

    appl_get_handle_from_device_handle
    (
        appl_hps_http_status_code_db_handle.device_id,
        &appl_handle
    );

    BT_gatt_db_get_char_val_hndl
    (
        &appl_hps_uri_db_handle,
        &appl_hps_uri_hndl
    );

    BT_gatt_db_get_char_val_hndl
    (
        &appl_hps_http_headers_db_handle,
        &appl_hps_http_headers_hndl
    );

    BT_gatt_db_get_char_val_hndl
    (
        &appl_hps_http_entity_body_db_handle,
        &appl_hps_http_entity_bdy_hndl
    );

    BT_gatt_db_get_char_val_hndl
    (
        &appl_hps_http_status_code_db_handle,
        &appl_hps_http_status_code_hndl
    );
    BT_gatt_db_get_char_cli_cnfg(&appl_hps_http_status_code_db_handle,&value);
    BT_UNPACK_LE_2_BYTE (&cli_cnfg,value.val);

    APPL_TRC (
    "[HPS]: Fetched Client Configuration (0x%04X) for Device (0x%02X)\n",
    cli_cnfg, (*dq_handle));

     if (0U != cli_cnfg)
    {
        appl_manage_trasnfer(appl_hps_http_status_code_db_handle, cli_cnfg);
    }

    appl_hps_https_security_db_handle.device_id = (*dq_handle);
    BT_gatt_db_get_char_val_hndl
    (
        &appl_hps_https_security_db_handle,
        &appl_hps_https_security_hndl
    );
    BT_IGNORE_UNUSED_PARAM(appl_hps_https_security_hndl);
}

void appl_hps_send_http_status_code
    (
        APPL_HANDLE * handle,
        UINT16      rsp_status
    )
{
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    API_RESULT            retval;
    UCHAR                 hps_data[APPL_HPS_STATUS_CODE_LEN];
    UCHAR                 data_status;

     APPL_TRC (
    "[0x%02X]:Sending http status code %d On Handle 0x%04X\n",
    (*handle), rsp_status, appl_hps_http_status_code_hndl);

     /* Initialize */
     BT_mem_set(hps_data, 0, sizeof(hps_data));
     data_status = 0x00U;
     retval      = API_SUCCESS;

     /* Reset */
     appl_hps_in_err_state = BT_FALSE;

    /**
     *  If HTTP Cancel Request received by the server while processing
     *  any other HTTP Request then terminating the executing HTTP request
     *  by without sending the notification.
     */
    if (BT_TRUE == appl_hps_status_code_recvd)
    {
        if (BT_TRUE == appl_hps_http_req_cancel_set)
        {
            /**
             * When an HPS Client uses the Write Characteristic Value
             * GATT sub-procedure with the HTTP Request Cancel opcode,
             * then the HPS Server shall terminate any executing HTTP request
             * from this HPS Client
            .*/
            appl_hps_http_req_cancel_set = BT_FALSE;
            APPL_TRC("\n\nReceived HTTP Cancel request while processing outstanding http req\n");

            retval = API_FAILURE; /* return; */
        }
        else
        {
            /**
             * "Data Status" value needs to be updated according to the length
             * of the Headers Received and the Body Received.
             * The "Data Status" is a Bit field combination of:
             *  - HEADER_RECEIVED  : BIT1 [0x01]
             *  - HEADER_TRUNCATED : BIT2 [0x02]
             *  - BODY_RECEIVED    : BIT3 [0x04]
             *  - BODY_TRUNCATED   : BIT4 [0x08]
             */
            data_status = appl_hps_data_status;

            /* Pack the 2 Bytes of the HTTP Response Status */
            BT_PACK_LE_2_BYTE(&hps_data[0U], &rsp_status);
            /* Pack the 1 Byte of the HTTP Data Status */
            hps_data[2U] = data_status;

            /* Populate the Status Notification Parameters */
            hndl_val_param.handle = appl_hps_http_status_code_hndl;
            hndl_val_param.value.val = hps_data;
            hndl_val_param.value.len = APPL_HPS_STATUS_CODE_LEN;
        }
    }
    else
    {
        APPL_ERR (
        "[** ERR **]: Invalid state to send HTTP RSP Code!\n");

        retval = API_FAILURE; /* return; */
    }

    if (API_SUCCESS == retval)
    {
        /* Send HPS Status Notifications */
        retval = BT_att_send_hndl_val_ntf
                 (
                     &APPL_GET_ATT_INSTANCE(*handle),
                     &hndl_val_param
                 );

        if (API_SUCCESS != retval)
        {
            APPL_ERR (
            "[** ERR **]: Failed to send the status code, reason 0x%04X",
            retval);
        }
    }
}


API_RESULT appl_hps_set_http_entity_body_char_val
           (
               UCHAR   * http_rsp_recv,
               UINT16    http_rsp_recv_len
           )
{
    API_RESULT      retval;

    retval = API_SUCCESS;

    /* Max http message entity size should be 512 bytes*/
    if (http_rsp_recv_len > APPL_HPS_MAX_HTTP_PROXY_DATA)
    {
        http_req_params.http_entity_body_len = APPL_HPS_MAX_HTTP_PROXY_DATA;
        appl_hps_data_status                |= HTTP_RSP_DATA_STATUS_BODY_TRUNCATED;
    }
    else
    {
        http_req_params.http_entity_body_len = http_rsp_recv_len;
        appl_hps_data_status                |= HTTP_RSP_DATA_STATUS_BODY_RECEIVED;
    }

    appl_hps_msg_entity_len = http_req_params.http_entity_body_len;

    /* Copy the http message entity received in http response to http_entity_bdy_buffer */
    BT_mem_copy
    (
        http_req_params.http_entity_body,
        http_rsp_recv,
        http_req_params.http_entity_body_len
    );

    return retval;
}

API_RESULT appl_hps_set_http_header_char_val
           (
               UCHAR   * http_rsp_recv,
               UINT16  http_rsp_recv_len,
               UINT16  * offset
           )
{
    API_RESULT      retval;
    unsigned int    index;
    unsigned int    strg_found_idx;
    unsigned int    total_header_len;
    UCHAR           string_chk[4U];
    UCHAR           serh_string[4U] = {0x0DU, 0x0AU, 0x0DU, 0x0AU}; /* CRLFCRLF */

    retval = API_SUCCESS;
    index = 0U;
    total_header_len = 0U;
    strg_found_idx = 0U;

    /* NULL Check for Offset */

    (*offset) = (UINT16) total_header_len;

    /* Initialize the HTTP URI/HEADER/BODY Structure */
    appl_hps_request_params_init();

    while(index <= http_rsp_recv_len)
    {
        BT_mem_copy(string_chk, &http_rsp_recv[index], 4U);
        if(0 == BT_mem_cmp(string_chk, serh_string, 4U))
        {
            strg_found_idx = index;

            /* Adding 2 bytes for Return carriage and New Line */
            total_header_len = (strg_found_idx + 4U);

            if (total_header_len == 0U)
            {
                APPL_TRC (
                "[HPS]: No HTTP headers in the received HTTP Response\n");

                retval = API_FAILURE;
                break; /* return API_FAILURE; */
            }

            /* Max Http headers size should be 512 bytes */
            if (total_header_len > APPL_HPS_MAX_HTTP_PROXY_DATA)
            {
                http_req_params.http_headers_len = APPL_HPS_MAX_HTTP_PROXY_DATA;
                appl_hps_data_status             = HTTP_RSP_DATA_STATUS_HEADER_TRUNCATED;
            }
            else
            {
                http_req_params.http_headers_len = (UINT16) total_header_len;
                appl_hps_data_status             = HTTP_RSP_DATA_STATUS_HEADER_RECEIVED;
            }

            appl_hps_header_length = http_req_params.http_headers_len;

            /**
             *  Copy http headers[after the status line] received in the http response to
             *  http_header_buffer
             */
            BT_mem_copy
            (
                http_req_params.http_headers,
                &http_rsp_recv[0U],
                appl_hps_header_length
            );
            break;
        }
        index++;
    }

    /* Append the Offset */
    (*offset) = (UINT16) total_header_len;

    return retval;
}

API_RESULT appl_hps_read_char_value
           (
               GATT_DB_HANDLE  * handle,
               ATT_ATTR_HANDLE attr_handle,
               UINT16          offset,
               UCHAR           is_rd_blob
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;

    if ((appl_hps_http_headers_hndl == attr_handle) ||
       (appl_hps_http_entity_bdy_hndl == attr_handle))
    {
        if ((0x0000U == offset) && (BT_FALSE == is_rd_blob))
        {
            /* Sending Read Response if offset is not set and if not Read Blob */
            appl_hps_send_read_rsp(handle, attr_handle);
        }
        else
        {
            /* Sending Read Blob Response if offset is set */
            if (attr_handle == appl_hps_http_headers_hndl)
            {
                if (offset > appl_hps_header_length)
                {
                    appl_hps_send_err_rsp(handle, attr_handle);
                }
                else
                {
                    appl_hps_send_read_blob_rsp(handle, attr_handle, offset);
                }
            }
            else if (attr_handle == appl_hps_http_entity_bdy_hndl)
            {
                if (offset > appl_hps_msg_entity_len)
                {
                    appl_hps_send_err_rsp(handle, attr_handle);
                }
                else
                {
                    appl_hps_send_read_blob_rsp(handle, attr_handle, offset);
                }
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
        }

        /**
         * Response is already initiated above,
         * so avoid sending response from the below GATT
         * server application layer.
         */
        retval = GATT_DB_ALREADY_RESPONDED;
    }

    return retval;
}

API_RESULT appl_hps_send_err_rsp
           (
               GATT_DB_HANDLE   *  handle,
               ATT_ATTR_HANDLE  attr_hndl
           )
{
    API_RESULT retval;
    ATT_ERROR_RSP_PARAM err_param;
    ATT_HANDLE  att_handle;

    /* Init */
    retval = API_SUCCESS;

    retval = appl_get_handle_from_device_handle
             (
                 handle->device_id,
                 &appl_handle
             );
    if (API_SUCCESS != retval)
    {
        APPL_TRC(
        "[HPS]: Failed to get APPL Handle from 0x%02X\n",
        handle->device_id);

        /* return retval; */
    }
    else
    {
        att_handle = APPL_GET_ATT_INSTANCE(appl_handle);

        err_param.error_code = ATT_INVALID_OFFSET;
        err_param.op_code = ATT_READ_BLOB_REQ;
        err_param.handle = attr_hndl;

        /* Send Error Response */
        retval = BT_att_send_error_rsp
                 (
                     &att_handle,
                     &err_param
                 );
    }

    return retval;
}

API_RESULT appl_hps_send_read_rsp
           (
               GATT_DB_HANDLE   * handle,
               ATT_ATTR_HANDLE  attr_hndl
           )
{
    API_RESULT retval;
    ATT_VALUE   rsp_params;
    ATT_HANDLE  att_handle;

    /* Init */
    retval = API_SUCCESS;

    retval = appl_get_handle_from_device_handle
             (
                 handle->device_id,
                 &appl_handle
             );
    if (API_SUCCESS != retval)
    {
        APPL_TRC(
        "[HPS]: Failed to get APPL Handle from 0x%02X\n",
        handle->device_id);

        /* return retval; */
    }
    else
    {
        att_handle = APPL_GET_ATT_INSTANCE(appl_handle);

        /* Sending Read Response if offset is not set */
        if (appl_hps_http_headers_hndl == attr_hndl)
        {
            rsp_params.val = http_req_params.http_headers;
            rsp_params.len = (http_req_params.http_headers_len > (APPL_HPS_GET_CUR_MTU(&att_handle) - 1U)) ?
                (APPL_HPS_GET_CUR_MTU(&att_handle) - 1U) : http_req_params.http_headers_len;
        }
        else if (appl_hps_http_entity_bdy_hndl == attr_hndl)
        {
            rsp_params.val = http_req_params.http_entity_body;
            rsp_params.len = (http_req_params.http_entity_body_len > (APPL_HPS_GET_CUR_MTU(&att_handle) - 1U)) ?
                (APPL_HPS_GET_CUR_MTU(&att_handle) - 1U) : http_req_params.http_entity_body_len;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        /* Read Response */
        retval = BT_att_read_rsp
                 (
                     &att_handle,
                     &rsp_params
                 );
    }

    return retval;
}

API_RESULT appl_hps_send_read_blob_rsp
           (
               GATT_DB_HANDLE   * handle,
               ATT_ATTR_HANDLE    attr_hndl,
               UINT16             offset
           )
{
    API_RESULT retval;
    ATT_VALUE   rsp_params;
    ATT_HANDLE  att_handle;

    /* Init */
    retval = API_SUCCESS;

    retval = appl_get_handle_from_device_handle
             (
                 handle->device_id,
                 &appl_handle
             );
    if (API_SUCCESS != retval)
    {
        APPL_TRC(
        "[HPS]: Failed to get APPL Handle from 0x%02X\n",
        handle->device_id);

        return retval;
    }
    else
    {
        att_handle = APPL_GET_ATT_INSTANCE(appl_handle);

        if (appl_hps_http_headers_hndl == attr_hndl)
        {
            rsp_params.val = &http_req_params.http_headers[offset];

            rsp_params.len = ((http_req_params.http_headers_len - offset) > (APPL_HPS_GET_CUR_MTU(&att_handle) - 1U)) ?
                (APPL_HPS_GET_CUR_MTU(&att_handle) - 1U) : (http_req_params.http_headers_len - offset);
        }
        else if (appl_hps_http_entity_bdy_hndl == attr_hndl)
        {
            rsp_params.val = &http_req_params.http_entity_body[offset];

            rsp_params.len = ((http_req_params.http_entity_body_len - offset) > (APPL_HPS_GET_CUR_MTU(&att_handle) - 1U)) ?
                (APPL_HPS_GET_CUR_MTU(&att_handle) - 1U) : (http_req_params.http_entity_body_len - offset);
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        /* Read Blob Response */
        retval = BT_att_read_blob_rsp
                 (
                     &att_handle,
                     &rsp_params
                 );
    }

    return retval;
}

void appl_manage_trasnfer (GATT_DB_HANDLE handle, UINT16 config)
{
    /* Empty Function */

    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(config);
}

void appl_hps_server_reinitialize (void)
{
    /* Initialize */
    appl_hps_header_length             = 0U;
    appl_hps_msg_entity_len            = 0U;
    appl_hps_uri_length                = 0U;
    appl_hps_data_status               = 0U;
    appl_hps_curr_cntrl_pnt_op         = 0xFFU;
    appl_hps_http_req_cancel_set       = BT_FALSE;
    appl_hps_in_err_state              = BT_FALSE;
    appl_hps_network_not_available_set = BT_FALSE;
    appl_hps_status_code_recvd         = BT_FALSE;
    appl_hps_transport_conn_state      = BT_FALSE;
    appl_hps_cntrl_pnt_enabled         = BT_FALSE;

    BT_mem_set(appl_hps_ip_address, 0x0, APPL_HPS_MAX_IP_ADDR_LEN);

    appl_hps_request_params_init();
    gatt_db_hps_config_state_reset();

#if ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL))
    if (BT_TRUE == APPL_IS_GAP_CENTRAL_ROLE())
    {
        /* Configure and Enable Scanning */
        appl_service_configure_scan (APPL_GAP_PROC_NORMAL, 0x00U, 0x00U, 0x00U);
        appl_service_enable_scan(0x01U);
    }
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */

#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
    if (BT_TRUE == APPL_IS_GAP_PERIPHERAL_ROLE())
    {
        /* Configure and Enable Advertising */
        appl_service_configure_adv(APPL_GAP_PROC_NORMAL, HCI_ADV_IND, 0x00U, 0x00U, NULL, 0x00U);
        appl_service_enable_adv(0x01U);
    }
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */
}

void appl_hsc_cnfg_handler(UINT16 config)
{
    if (GATT_CLI_CNFG_NOTIFICATION == config)
    {
        appl_hps_cntrl_pnt_enabled = BT_TRUE;
    }
    else
    {
        appl_hps_cntrl_pnt_enabled = BT_FALSE;
    }
}

void appl_hps_request_params_init()
{
    BT_mem_set(&http_req_params, 0x0, sizeof(APPL_HPS_HTTP_REQUEST_PARAMS));
}

API_RESULT appl_hps_write_char_value
           (
               ATT_ATTR_HANDLE          handle,
               ATT_VALUE              * att_value,
               UINT16                   offset
           )
 {
    if (appl_hps_uri_hndl == handle)
    {
        /* Reset the globals when write offset is 0 */
        if (0U == offset)
        {
            BT_mem_set(http_req_params.uri, 0, sizeof(http_req_params.uri));
            appl_hps_uri_length = 0U;
        }

        /* Copy the incoming URI value into global based on offset */
        BT_mem_copy
        (
            &http_req_params.uri[offset],
            &att_value->val[0U],
            att_value->len
        );

        /* adjust the URI length on each write */
        appl_hps_uri_length += att_value->len;

        /* Set the URI Length in Global */
        http_req_params.uri_len = appl_hps_uri_length;
    }
    else if (appl_hps_http_headers_hndl == handle)
    {
        /* Reset the Globals when write offset is 0 */
        if (0U == offset)
        {
            BT_mem_set(http_req_params.http_headers, 0, sizeof(http_req_params.http_headers));
            appl_hps_header_length = 0U;
        }

        /* Copy the incoming HEADER value into global based on offset */
        BT_mem_copy
        (
            &http_req_params.http_headers[offset],
            &att_value->val[0U],
            att_value->len
        );

        /* adjust the HEADER length on each write */
        appl_hps_header_length += att_value->len;

        /* Set the HEADER Length in Global */
        http_req_params.http_headers_len = appl_hps_header_length;
    }
    else if (appl_hps_http_entity_bdy_hndl == handle)
    {
        /* Reset the Globals when write offset is 0 */
        if (0U == offset)
        {
            BT_mem_set(http_req_params.http_entity_body, 0, sizeof(http_req_params.http_entity_body));
            appl_hps_msg_entity_len = 0U;
        }

        /* Copy the incoming ENTITY BODY value into global based on offset */
        BT_mem_copy
        (
            &http_req_params.http_entity_body[offset],
            &att_value->val[0U],
            att_value->len
        );

        /* adjust the ENTITY BODY length on each write */
        appl_hps_msg_entity_len += att_value->len;

        /* Set the ENTITY BODY Length in Global */
        http_req_params.http_entity_body_len = appl_hps_msg_entity_len;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return API_SUCCESS;
}

void appl_hps_adjust_uri_header(void)
{
    /**
     * This function check the last two Bytes of URI Header for "\r\n".
     * If the Terminating Characters are not found. Then append the same
     * and adjust the length.
     */
    if (2U < http_req_params.http_headers_len)
    {
        /* TODO: Validate the boundary conditions for the header array here */
        if (('\r' != http_req_params.http_headers[(http_req_params.http_headers_len - 2U)]) &&
            ('\n' != http_req_params.http_headers[(http_req_params.http_headers_len - 1U)]))
        {
            http_req_params.http_headers[(http_req_params.http_headers_len)] = '\r';
            http_req_params.http_headers_len++;
            http_req_params.http_headers[(http_req_params.http_headers_len)] = '\n';
            http_req_params.http_headers_len++;
        }
    }
}

API_RESULT appl_hps_cp_handler
           (
                GATT_DB_HANDLE * handle,
                ATT_VALUE      * value,
                UCHAR          hps_config_state
           )
{
    API_RESULT    retval;
    UCHAR         hps_cp_opcode;

    retval = API_SUCCESS;

    /* Copying the opcode to the local variable */
    hps_cp_opcode = value->val[0U];

    if (BT_FALSE == appl_hps_cntrl_pnt_enabled)
    {
        APPL_TRC("[ERR]: HPS Status CCCD Not Configured!\n");

        retval = (ATT_CCD_IMPROPERLY_CONFIGURED | APPL_ERR_ID);
    }
    /**
     * According to HPS Spec:
     * "The HPS Client must configure URI, HTTP Headers and HTTP Entity Body
     * by using appropriate GATT write procedure each time before it issues
     * any HTTP Request except the HTTP Request Cancel.
     * The HPS Server shall reject an attempt to Write Characteristic Value
     * of the HTTP Control Point Characteristic with error code
     * 'INVALID REQUEST' if the request is received before URI, HTTP Headers
     * and HTTP Entity Body characteristics have been configured after startup,
     * or after the previous HTTP Request. If a particular characteristic is
     * not used in executing an HTTP Request, then it shall be configured with
     * a null (zero length) value."
     */
    else if (APPL_HPS_CONFIGURED_STATE != hps_config_state)
    {
        APPL_TRC("[ERR]: HPS either of URI/HEADER/BODY not set (again)!\n");

        retval = APPL_HPS_INVALID_REQUEST;
    }
    /**
     *  HTTP status code, URI, HTTP Headers and HTTP Entity body characteristics
     *  should be configured by the HPS client before issuing
     *  any HTTP request on HTTP control point
     */
    else if (BT_TRUE == appl_hps_network_not_available_set)
    {
        APPL_TRC("[ERR]: HPS NETWORK NOT AVAILABLE!\n");

        retval = (APPL_HPS_NETWORK_NOT_AVAILABLE | APPL_ERR_ID);
    }
    /**
     *  IUT enters into ER state once it accepts the control point command
     *  It IUT is in ER state and received once more control command other
     *  than the HTTP Cancel IUT sends procedure already in progess err code
     */
    else if ( (BT_TRUE == appl_hps_in_err_state) &&
            ((APPL_HTTPS_DELETE_REQ >= hps_cp_opcode) && (APPL_HTTP_GET_REQ <= hps_cp_opcode)) )
    {
        APPL_TRC("[ERR]: HPS Procedure Already In Progress!\n");

        retval = (ATT_PROCEDURE_ALREADY_IN_PROGRESS | APPL_ERR_ID);
    }
    /* Check the control point opcode */
    else if (BT_FALSE != APPL_HPS_SUPPORTED_OPCODE(hps_cp_opcode))
    {
        /* Store the Valid Control Point in the Global */
        appl_hps_curr_cntrl_pnt_op = hps_cp_opcode;

        if (APPL_HTTP_REQ_CANCEL == hps_cp_opcode)
        {
            appl_hps_in_err_state           = BT_FALSE;
            appl_hps_http_req_cancel_set    = BT_TRUE;
        }
        else
        {
            /* Set HPS server is in ER State */
            appl_hps_in_err_state = BT_TRUE;
        }

        /* Adjust the URI Header */
        appl_hps_adjust_uri_header();

        appl_hps_is_https_req = BT_TRUE; /* We assume it is a https req first. */

        switch (hps_cp_opcode)
        {
        /* Fall Through */
        case APPL_HTTP_GET_REQ:
            appl_hps_is_https_req = BT_FALSE;
        case APPL_HTTPS_GET_REQ:
            if (0U != http_req_params.http_headers_len)
            {
                retval = appl_hps_handle_cp_req
                         (
                             &http_req_params,
                             types_of_http_req.http_get_req,
                             sizeof(types_of_http_req.http_get_req)
                         );
            }
            else
            {
                APPL_TRC("HTTP Headers is not set\n");
            }
            break;
        /* Fall Through */
        case APPL_HTTP_HEAD_REQ:
            appl_hps_is_https_req = BT_FALSE;
        case APPL_HTTPS_HEAD_REQ:
            if (0U != http_req_params.http_headers_len)
            {
                retval = appl_hps_handle_cp_req
                         (
                             &http_req_params,
                             types_of_http_req.http_head_req,
                             sizeof(types_of_http_req.http_head_req)
                         );
            }
            else
            {
                APPL_TRC("HTTP Headers is not set\n");
            }
            break;
        /* Fall Through */
        case APPL_HTTP_POST_REQ:
            appl_hps_is_https_req = BT_FALSE;
        case APPL_HTTPS_POST_REQ:
            if (0U != http_req_params.http_headers_len)
            {
                retval = appl_hps_handle_cp_req
                         (
                             &http_req_params,
                             types_of_http_req.http_post_req,
                             sizeof(types_of_http_req.http_post_req)
                         );
            }
            else
            {
                APPL_TRC("HTTP Headers is not set\n");
            }
            break;
        /* Fall Through */
        case APPL_HTTP_PUT_REQ:
            appl_hps_is_https_req = BT_FALSE;
        case APPL_HTTPS_PUT_REQ:
            if (0U != http_req_params.http_headers_len)
            {
                retval = appl_hps_handle_cp_req
                         (
                             &http_req_params,
                             types_of_http_req.http_put_req,
                             sizeof(types_of_http_req.http_put_req)
                         );
            }
            else
            {
                APPL_TRC("HTTP Headers is not set\n");
            }
            break;
        /* Fall Through */
        case APPL_HTTP_DELETE_REQ:
            appl_hps_is_https_req = BT_FALSE;
        case APPL_HTTPS_DELETE_REQ:
            if (0U != http_req_params.http_headers_len)
            {
                retval = appl_hps_handle_cp_req
                         (
                             &http_req_params,
                             types_of_http_req.http_delete_req,
                             sizeof(types_of_http_req.http_delete_req)
                         );
            }
            else
            {
                APPL_TRC("HTTP Headers is not set\n");
            }
            break;

        case APPL_HTTP_REQ_CANCEL:
            APPL_TRC("Processing HTTP Cancel Request\n");
            /**
             * If Transport is not connected and directly a CANCEL is requested
             * The Send an error that its an Invalid request.
             * TODO: Check if there is anything mentioned in HPS Sepc for this.
             */
            if (BT_FALSE == appl_hps_transport_conn_state)
            {
                retval = (APPL_HPS_INVALID_REQUEST | APPL_ERR_ID);

                /* Reset the Cancel State */
                appl_hps_http_req_cancel_set    = BT_FALSE;
            }
            break;

        default:
                /* This place should not Hit! */
                APPL_TRC("[ERR]: Invalid Control Point Opcode!\n");
                retval = (APPL_HPS_INVALID_REQUEST | APPL_ERR_ID);
            break;
        }
    }
    else
    {
        APPL_TRC("[ERR]: Unsupported/Invalid Control Point Opcode!\n");
        retval = (APPL_HPS_INVALID_REQUEST | APPL_ERR_ID);
    }

    /**
     * Reset the HPS URI/HEADER/BODY Config State when Control Point Error
     * has occurred.
     */
    if (API_SUCCESS != retval)
    {
        gatt_db_hps_config_state_reset();

        /* Stop the timer if its running */
        appl_hps_cp_stop_timer();
    }

    BT_IGNORE_UNUSED_PARAM(handle);

    return retval;
}

API_RESULT appl_hps_cp_start_timer(void)
{
    API_RESULT retval;

    retval = API_SUCCESS;

    /* Start timer for Control Point Procedures */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        APPL_TRC (
        "[HPS]: Timer %p Already Running! Unable to start new timer!\n",
        timer_handle);

        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Start a Control Point procedure timer */
        retval = BT_start_timer
                 (
                     &timer_handle,
                     APPL_HPS_CP_PROC_TIMEOUT_VAL,
                     appl_hps_cp_timer_expiry_handler,
                     NULL,
                     0U
                 );

        APPL_TRC (
        "[HPS]: Control Point Procedure Timer %p Started, result 0x%04X!\n",
        timer_handle, retval);
    }

    return retval;
}

API_RESULT appl_hps_cp_stop_timer(void)
{
    API_RESULT retval;

    retval = API_SUCCESS;

    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        retval = BT_stop_timer(timer_handle);

        APPL_TRC(
        "[HPS]: Stopping Control Point Procedure Timer %p, result 0x%04X!\n",
        timer_handle, retval);

        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    return retval;
}

void appl_hps_cp_timer_expiry_handler (void *data_param, UINT16 datalen)
{
    UINT16 rsp_status;

    BT_IGNORE_UNUSED_PARAM(data_param);
    BT_IGNORE_UNUSED_PARAM(datalen);

    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        APPL_TRC (
        "[HPS]: Control Point Timeout Occurred: %p\n", timer_handle);

        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    /**
     * Reset the HPS URI/HEADER/BODY Config State when Control Point Error
     * has occurred.
     */
    gatt_db_hps_config_state_reset();

    /**
     * If the Control Point timeout has occured,
     * then the Status is Request Timeout.
     */
    rsp_status = HTTP_RSP_STATUS_CODE_REQ_TIMEDOUT;

    /* Set the Status Code Received Flag to True */
    appl_hps_status_code_recvd = BT_TRUE;

    appl_hps_send_http_status_code(&appl_handle, rsp_status);
}

API_RESULT appl_hps_handle_cp_req
           (
               /* IN */  APPL_HPS_HTTP_REQUEST_PARAMS * http_req,
               /* IN */  UCHAR                        * type_of_http_req,
               /* IN */  UCHAR                        type_of_http_req_len
           )
{
    API_RESULT retval;

    BT_mem_set(appl_hps_ip_address, 0, sizeof(appl_hps_ip_address));

    /* Start Control Point Procedure timer */
    appl_hps_cp_start_timer();

    /* Get the HostName/IP Address from the HTTP Header */
    retval = appl_hps_get_hostname
             (
                 http_req,
                 type_of_http_req,
                 type_of_http_req_len,
                 &appl_hps_ip_address[0U]
             );

    if (API_SUCCESS == retval)
    {
        /**
         * Check if already a TCP connection which is open
         * is not existing.
         */
        if (BT_FALSE == appl_hps_transport_conn_state)
        {
            /* Have TCP connection with HTTP server */
            retval = appl_hps_init_socket_to_http_server(appl_hps_ip_address);
        }

        if (API_SUCCESS == retval)
        {
            retval = appl_hps_construct_http_req
                     (
                         http_req,
                         type_of_http_req,
                         type_of_http_req_len
                     );
        }
    }

    if (API_SUCCESS != retval)
    {
        /**
         * Reset the state for further processing of next commands
         * once failure is encountered.
         */
        appl_hps_in_err_state = BT_FALSE;

        /**
         * NOTE:
         * Here the application can use either
         * 1. ATT_INSUFFICIENT_RESOURCES
         * 2. ATT_UNLIKELY_ERROR
         * 3. Generically use APPL_HPS_NETWORK_NOT_AVAILABLE
         *
         * Currently the application is set to use the Network Error.
         */
        APPL_ERR(
        "[HPS]: **ERR** Translating application error 0x%04X as "
        "Network Error!\n", retval);

        retval = (APPL_HPS_NETWORK_NOT_AVAILABLE | APPL_ERR_ID);
    }

    return retval;
}

API_RESULT appl_hps_get_hostname
           (
              /* IN */  APPL_HPS_HTTP_REQUEST_PARAMS * http_req,
              /* IN */  UCHAR                        * type_of_http_req,
              /* IN */  UCHAR                        type_of_http_req_len,
              /* OUT */ UCHAR                        * ip_address
           )
{
    API_RESULT retval;
    UCHAR      start_idx;
    UCHAR      str_found_idx;
    UCHAR      char_check;
    UCHAR      index;
    UCHAR      total_http_headers_len;
    UCHAR      http_header_len;
    UCHAR      * data;
    UCHAR      * pch;

    BT_IGNORE_UNUSED_PARAM(type_of_http_req);
    BT_IGNORE_UNUSED_PARAM(type_of_http_req_len);

    retval                 = API_SUCCESS;
    index                  = 0U;
    start_idx              = 0U;
    str_found_idx          = 0U;
    total_http_headers_len = (UCHAR) http_req->http_headers_len;

    data = NULL;
    if (0U != total_http_headers_len)
    {
        while (index <= total_http_headers_len)
        {
            /**
             * Take every byte of http headers char value
             * and compare with "\n".
             * This is because, some Clients might send Headers with
             * "\r\n" some might send with "\n".
             */
            char_check = http_req->http_headers[index];

            if ('\n' == char_check)
            {
                /* Store the index where '\n' is found */
                str_found_idx = index;

                /* Check if the preceding Octet is "\r" */
                if (0U < str_found_idx)
                {
                    if ('\r' == http_req->http_headers[str_found_idx - 1U])
                    {
                        /* correct header len will not include "\r" too */
                        http_header_len = (str_found_idx - start_idx - 1U);
                    }
                    else
                    {
                        /* No "\r" present only "\n" */
                        http_header_len = (str_found_idx - start_idx);
                    }
                }
                else
                {
                    /* Error Scenario, "\n" found at Index 0! */
                    APPL_ERR("[HPS]: **ERR** Invalid URI Header Format!\n");

                    retval = API_FAILURE;
                    break; /* return API_FAILURE; */
                }

#ifdef APPL_HPS_HAVE_DATA_DUMP
                appl_dump_bytes
                (
                    &http_req->http_headers[start_idx],
                    (UINT16)http_header_len
                );
#endif /* APPL_HPS_HAVE_DATA_DUMP */

                /* Since NULL is added later, Allocate 1 Extra Byte */
                data = BT_alloc_mem(((UINT16)http_header_len) + 1U);

                if (NULL == data)
                {
                    APPL_ERR("[HPS]: **ERR** Memory Allocation Failed in "
                    "appl_hps_get_hostname!\n");

                    retval = API_FAILURE;
                    break; /* return API_FAILURE; */
                }

                BT_mem_copy
                (
                    data,
                    &http_req->http_headers[start_idx],
                    (UINT32)http_header_len
                );

                /* To Check if this needs to be moved to platforms */
                /**
                 * Tokenize the first part of the Header with ":" as delimiter.
                 */
                pch = (UCHAR *)strtok((CHAR *)data, ":");
                pch[http_header_len] = '\0';

                /* Check if the Tag Matches "Host" */
                if (0 == BT_mem_cmp(pch, appl_hps_host_tag_val, APPL_HPS_HOST_TAG_LEN))
                {
                    /**
                     * TODO: Check if the loop can be removed
                     * and only the first tokenized string to be
                     * treated as the Hostname to look for.
                     */
                    while (NULL != pch)
                    {
                        /**
                         * Tokenize the first part of the remaining Header with
                         * ":" as delimiter. Typically, till the end of the
                         * remaining part of the string.
                         */
                        pch = (UCHAR *)strtok(NULL, ": ");

                        /* If Tokenized value is NULL then break the loop */
                        if (NULL == pch)
                        {
                            break;
                        }

                        APPL_TRC("[HPS]: Trying to resolve Host name: %s\n", pch);

                        /* Get the IP Address of the HTTP Server using Host */
                        retval = hps_fetch_ip_from_hostname_pl
                                 (
                                     pch,
                                     ip_address
                                 );

                        if (API_SUCCESS == retval)
                        {
                            APPL_TRC("[HPS]: Host name: %s resolved to: %s\n",
                            pch, ip_address);

                            break;
                        }
                        else
                        {
                            APPL_ERR(
                            "[HPS]: **ERR** Failed to convert Host-name %s to"
                            "IP Address\n", pch);

                            retval = API_FAILURE;
                        }
                    }
                }
                else
                {
                    APPL_ERR(
                    "[HPS]: **ERR** Failed to find a Valid Host Name!\n");

                    retval = API_FAILURE;
                }

                /* Free the allocated memory */
                BT_free_mem(data);

                /**
                 * Break the loop, if an IP Address for a Valid Hostname is
                 * found!
                 */
                if (API_SUCCESS == retval)
                {
                    break;
                }
            }

            index++;
        }
    }

    return retval;
}

API_RESULT appl_hps_init_socket_to_http_server(UCHAR * ipaddr)
{
    API_RESULT retval;

    retval = API_SUCCESS;

    /**
     *  Call the function to start the HPS Transport connection
     *  with the HTTP server on the ipaddr.
     */
    if (appl_hps_is_https_req)
    {
        retval = https_transport_open_pl(ipaddr);

        /* Set the HTTPS Server Security Char Value */
        (API_SUCCESS == retval) ?
        appl_hps_set_https_security_val(BT_TRUE):
        appl_hps_set_https_security_val(BT_FALSE);
    }
    else
    {
        retval = hps_transport_open_pl(ipaddr);
    }

    if (API_SUCCESS != retval)
    {
        APPL_ERR("[HPS]: **ERR** Transport Establishment failed\n");
        retval = API_FAILURE;
    }
    else
    {
        APPL_TRC("[HPS]: Transport Established Successfully!\n");

        appl_hps_transport_conn_state = BT_TRUE;
    }
    return retval;
}

API_RESULT appl_hps_fetch_http_status
           (
               UCHAR   * http_rsp,
               UINT16  http_rsp_len,
               UINT16  * status,
               UINT16  * offset
           )
{
    UINT16     http_rsp_status_code;
    UCHAR      * status_line;
    UINT16     status_line_len;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    BT_IGNORE_UNUSED_PARAM(http_rsp_len);

    /* NULL Check */
    if ((NULL == status) || (NULL == offset))
    {
        APPL_ERR(
        "[HPS]: **ERR** NULL Value provided for Status and Offset!\n");

        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /**
         * The Status Line of the HTTP Response is the
         * Line till first occurrence of "\r\n".
         * Tokenize and fetch that first line.
         */
        status_line = (UCHAR *)strtok((CHAR *)http_rsp, "\r\n");

        /* Fetch the status line length */
        status_line_len = (UINT16)BT_str_len(status_line);

    #ifdef APPL_HPS_HAVE_DATA_DUMP
        APPL_TRC("[HPS]: Received HTTP Status Line is\n");
        appl_dump_bytes(status_line, status_line_len);
    #endif /* APPL_HPS_HAVE_DATA_DUMP */

        APPL_TRC("\n[HPS]: Received HTTP Status Line of length %d is:\n%s\n",
        status_line_len, status_line);

        /**
         * The HTTP response status code value is located after the "HTTP/1.1 "
         * HTTP Protocol Version string at the offset location defined by
         * APPL_HPS_STATUS_CODE_INDEX.
         */
        /* Converting received 3 bytes of status code to single integer value */
        http_rsp_status_code = (UINT16)(atoi((const char *)(&status_line[APPL_HPS_STATUS_CODE_INDEX])));

        /* Copying status to OUT param */
        (* status) = http_rsp_status_code;

        /**
         * The Offset for next data in the HTTP response is
         * status line length + 2 bytes of "\r\n".
         */
        (* offset) = (status_line_len + 2U);
    }

    return retval;
}

API_RESULT appl_hps_construct_http_req
           (
                APPL_HPS_HTTP_REQUEST_PARAMS * http_request_params,
                UCHAR                        * type_of_http_req,
                UCHAR                        type_of_http_req_len
           )
{
    API_RESULT retval;
    UCHAR      * packet;
    UINT16     offset;
    UINT16     packet_length, alloc_len;
    /* HTTP/1.1 */
    UCHAR     http_version[10U] =
              {
                  0x48U, 0x54U, 0x54U, 0x50U,
                  0x2fU, 0x31U, 0x2eU, 0x31U,
                  0x0dU, 0x0aU
              };
    UCHAR     ascii_for_space    = 0x20U;
    UINT16    ascii_for_crlf     = 0x0a0dU;

    retval = API_SUCCESS;
    packet_length = 0U;

    /**
     *  Once the TCP socket connection done with the first data that
     *  HTTP server expects to receive from the client is
     *  the "Request-Line" production
     *  appl_hps_construct_http_req() function with construct
     *  http request with 1st line of HTTP request will be
     *  "Request Line"
     */

    /**
     *  Construct the http request
     *  HTTP Request Format is
     *     1. A Request-line = Method (space) Request-URI (space) HTTP-Version CRLF
     *     2. Zero or more header (General|Request|Entity) fields followed by CRLF
     *     3. An empty line (i.e., a line with nothing preceding the CRLF)
     *        indicating the end of the header fields
     *     4. Optionally a message-body
     */

    /**
     * Allocate the memory of
     *  length of type of request
     *  1 byte of Space
     *  length of URI
     *  1 byte Space
     *  8 bytes of HTTP/1.1
     *  2 bytes of Carriage Return, line feed
     *  Length of http Header
     *  2 bytes for Carriage Return, line feed
     */
    alloc_len = 0U;
    alloc_len += type_of_http_req_len;
    alloc_len += 1U;
    alloc_len += http_request_params->uri_len;
    alloc_len += 1U;
    alloc_len += 8U;
    alloc_len += http_request_params->http_headers_len;
    alloc_len += 2U;
    alloc_len += 2U;

    switch(appl_hps_curr_cntrl_pnt_op)
    {
        /* Add the Entity Body too */
        case APPL_HTTP_POST_REQ:
        case APPL_HTTP_PUT_REQ:
        case APPL_HTTP_DELETE_REQ:
        case APPL_HTTPS_POST_REQ:
        case APPL_HTTPS_PUT_REQ:
        case APPL_HTTPS_DELETE_REQ:
            alloc_len += http_request_params->http_entity_body_len;
            break;

        default:
            CONSOLE_OUT("Invalid Operation\n");
            break;
    }

    offset = 0U;

    packet = BT_alloc_mem(alloc_len);

    if (NULL == packet)
    {
        APPL_ERR(
        "[HPS]: **ERR** Memory Allocation Failed in "
        "appl_hps_construct_http_req!\n");

        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Initialize the Allocated Memory */
        BT_mem_set(packet, 0x0, alloc_len);

        /* Copy  Method */
        BT_mem_copy(&packet[offset], type_of_http_req, type_of_http_req_len);

        /* Space */
        offset = offset + type_of_http_req_len;
        BT_PACK_LE_1_BYTE(&packet[offset], &ascii_for_space);

        /* Copy Request URI */
        offset = offset + 1U;
        BT_mem_copy
        (
            &packet[offset],
            http_request_params->uri,
            http_request_params->uri_len
        );

        /* Copy Space */
        offset = offset + http_request_params->uri_len;
        BT_PACK_LE_1_BYTE(&packet[offset], &ascii_for_space);

        /* Copy HTTP Version */
        offset = offset + 1U;
        BT_mem_copy
        (
            &packet[offset],
            http_version,
            sizeof(http_version)
        );

        /* Copy HTTP headers */
        /* Each header will be seaprated by \r\n */
        offset += sizeof(http_version);
        BT_mem_copy
        (
            &packet[offset],
            http_request_params->http_headers,
            http_request_params->http_headers_len
        );

        /* Copy CRLF */
        offset += http_request_params->http_headers_len;
        BT_PACK_LE_2_BYTE(&packet[offset], &ascii_for_crlf);
        offset += 2U;

        switch(appl_hps_curr_cntrl_pnt_op)
        {
            /* Add the Entity Body too */
            case APPL_HTTP_POST_REQ:
            case APPL_HTTP_PUT_REQ:
            case APPL_HTTP_DELETE_REQ:
            case APPL_HTTPS_POST_REQ:
            case APPL_HTTPS_PUT_REQ:
            case APPL_HTTPS_DELETE_REQ:
                BT_mem_copy
                (
                    &packet[offset],
                    http_request_params->http_entity_body,
                    http_request_params->http_entity_body_len
                );
                offset += http_request_params->http_entity_body_len;
                break;

            default:
                CONSOLE_OUT("Invalid Operation\n");
                break;
        }

        /* Assign the Total Packet Length */
        packet_length = offset;

        /**
         * Constructed request will in the below form
         *  Example HTTP/HTTPS Request
         *  HTTP Format is will be available in (https://www.tutorialspoint.com/http/http_requests.htm)
         *  "GET Request-URI HTTP/1.1\r\n Host: Host_name\r\nConnection: Close\r\n\r\n"
         */

        /* printing the constructed http/https request */
#ifdef APPL_HPS_HAVE_DATA_DUMP
        APPL_TRC("[HPS]: Constructed HTTP Request is:\n");
        appl_dump_bytes(packet, packet_length);
#endif /* APPL_HPS_HAVE_DATA_DUMP */

        /* TODO: Check if using %.*s is a portable code? */
        APPL_TRC("[HPS]: Constructed HTTP Request:\n\n%.*s\n", packet_length, packet);

        /* Send the constructed request on socket */
        if (appl_hps_is_https_req)
        {
            retval = https_transport_write_data_pl(packet, packet_length);
        }
        else
        {
            retval = hps_transport_write_data_pl(packet, packet_length);
        }

        /* Free the allocated memory */
        BT_free_mem(packet);
    }

    return retval;
}

/* This cb is called once IUT received any data from the network interface */
API_RESULT appl_hps_cb
           (
               UCHAR  * data,
               UINT16 data_len
           )
{
    API_RESULT retval;
    UINT16     http_rsp_status;
    UINT16     offset;

    retval = API_SUCCESS;

#ifdef APPL_HPS_HAVE_DATA_DUMP
    APPL_TRC("[HPS]: Received Data from TCP:\n");
    appl_dump_bytes(data, data_len);
#endif /* APPL_HPS_HAVE_DATA_DUMP */

    /**
     * NOTE:
     * Typically, the Transport Close, needs to happen
     * depending on the data received from the URL.
     * Check for the Status
     * Check and extract the Headers
     * Extract the message/entity body if present.
     */
    /** Shut down the current TCP connection */
    if (appl_hps_is_https_req)
    {
        https_transport_close_pl();

        /*
         * On HTTPS Transport Close, Reset the
         * HTTPS Server Security char Value.
         */
        appl_hps_set_https_security_val(BT_FALSE);
    }
    else
    {
        hps_transport_close_pl();
    }

    appl_hps_transport_conn_state = BT_FALSE;

    /* Validate the incoming data length */
    if (0U == data_len)
    {
        /**
         * On failure, this function just returns.
         * The HTTP status will be sent out on Procedure timeout.
         */
        APPL_ERR(
        "\n[HPS]: **ERR** ZERO Bytes received from Transport. "
        "Dropping the Packet!\nWait for status to be sent after Timeout of %d "
        "seconds!\n", APPL_HPS_CP_PROC_TIMEOUT_VAL);

        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Dumping the received HTTP Response data as String */
        APPL_TRC("[HPS]: Received HTTP Response is:\n%s\n", data);

        /* Parse and find the HTTP Status code from HTTP Response */
        retval = appl_hps_fetch_http_status
                 (
                     data,
                     data_len,
                     &http_rsp_status,
                     &offset
                 );

        if (API_SUCCESS != retval)
        {
            /**
             * On failure, this function just returns.
             * The HTTP status will be sent out on Procedure timeout.
             */
            APPL_ERR(
            "\n[HPS]: **ERR** Failures 0x%04X during HTTP Status parsing!\nWait "
            "for status to be sent after Timeout of %d seconds!\n",
            APPL_HPS_CP_PROC_TIMEOUT_VAL);

            /* return retval; */
        }
        else
        {
            appl_hps_status_code_recvd = BT_TRUE;

            /**
             * Adjust the incoming transport data and datalen according to
             * length of the status line, to Parse the Header and the Body.
             */
            data     += offset;
            data_len -= offset;

            /* Reset the Offset */
            offset = 0U;

            /**
             * Store the first 512 octets of the received HTTP Headers from the
             * http response into http_header_buffer
             */
            retval = appl_hps_set_http_header_char_val(data, data_len, &offset);
            if (retval != API_SUCCESS)
            {
                /**
                 * On failure, this function just returns.
                 * The HTTP status will be sent out on Procedure timeout.
                 */
                APPL_ERR(
                "\n[HPS]: **ERR** Failures 0x%04X during HTTP Header parsing!\n"
                "Wait for status to be sent after Timeout of %d seconds!\n",
                APPL_HPS_CP_PROC_TIMEOUT_VAL);

                /* return retval; */
            }
            else
            {
#ifdef APPL_HPS_HAVE_DATA_DUMP
                APPL_TRC("\n[HPS]: Received HTTP Response Header is\n");
                appl_dump_bytes
                (
                    http_req_params.http_headers,
                    http_req_params.http_headers_len
                );
#endif /* APPL_HPS_HAVE_DATA_DUMP */

                APPL_TRC("\n[HPS]: Received HTTP Response Header is:\n%s",
                http_req_params.http_headers);

                /**
                 *  Store the first 512 octets of the received HTTP Message Body from the
                 *  http response into http_entity_bdy_buffer
                 */
                retval = appl_hps_set_http_entity_body_char_val
                         (
                             (&data[offset]),
                             (data_len - offset)
                         );

                if (retval != API_SUCCESS)
                {
                    /**
                     * On failure, this function just returns.
                     * The HTTP status will be sent out on Procedure timeout.
                     */
                    APPL_ERR(
                    "\n[HPS]: **ERR** Failures 0x%04X during HTTP Entity Body parsing!\n"
                    "Wait for status to be sent after Timeout of %d seconds!\n",
                    APPL_HPS_CP_PROC_TIMEOUT_VAL);

                    /* return retval; */
                }
                else
                {
#ifdef APPL_HPS_HAVE_DATA_DUMP
                    APPL_TRC("\n[HPS]: Received HTTP Response Entity Body is\n");
                    appl_dump_bytes(http_req_params.http_entity_body,
                    http_req_params.http_entity_body_len);
#endif /* APPL_HPS_HAVE_DATA_DUMP */

                    APPL_TRC("\n[HPS]: Received HTTP Response Entity Body is:\n%s\n",
                    http_req_params.http_entity_body);

                    /* Stop the Control Point timer */
                    appl_hps_cp_stop_timer();

                    /**
                     * Reset the HPS URI/HEADER/BODY Config State when Control Point Error
                     * has occurred.
                     */
                    gatt_db_hps_config_state_reset();

                    /* Send status code notification to HPS client */
                    appl_hps_send_http_status_code(&appl_handle, http_rsp_status);
                }
            }
        }
    }

    return retval;
}

void appl_hps_set_network_availability(UCHAR flag)
{
    appl_hps_network_not_available_set = (BT_TRUE == flag) ? BT_FALSE : BT_TRUE;
}

void appl_hps_set_gap_central_role(void)
{
    APPL_SET_GAP_CENTRAL_ROLE();

#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
    appl_service_enable_adv(0x00U);
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */

#if ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL))
    if (BT_TRUE == APPL_IS_GAP_CENTRAL_ROLE())
    {
        /* Configure and Enable Scanning */
        appl_service_configure_scan (APPL_GAP_PROC_NORMAL, 0x00U, 0x00U, 0x00U);
        appl_service_enable_scan(0x01U);
    }
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */
}

void appl_hps_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     )
{
    APPL_TRC("\n[HPS]: IND Completed for Appl Handle 0x%02X with result 0x%04X\n",
    *handle, evt_result);
}

void appl_hps_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     )
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(event_data);
    BT_IGNORE_UNUSED_PARAM(datalen);

#ifdef APPL_HPS_HAVE_DATA_DUMP
    APPL_TRC("\n[HPS]: NTF Sent for Appl Handle 0x%02X\n", *handle);
    appl_dump_bytes(event_data, datalen);
#endif /* APPL_HPS_HAVE_DATA_DUMP */
}

void appl_hps_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     )
{
    APPL_TRC("\n[HPS]: Updated MTU is %d for Appl Handle 0x%02X\n",
    mtu, *handle);
}

UINT16 appl_hps_get_curr_mtu(ATT_HANDLE * handle)
{
    APPL_HANDLE appl_hndl;
    API_RESULT  retval;
    UINT16      t_mtu;

    t_mtu = ATT_DEFAULT_MTU;

    retval = appl_get_handle_from_device_handle(handle->device_id, &appl_hndl);

    if (API_SUCCESS == retval)
    {
        /* Fetch the MTU for the APPL Handle */
        t_mtu = APPL_GET_MTU(appl_hndl);

        /* TODO: Validate the t_mtu returned for this Handle Handle */
    }

    return t_mtu;
}

#endif /* (defined ATT && defined HPS) */

