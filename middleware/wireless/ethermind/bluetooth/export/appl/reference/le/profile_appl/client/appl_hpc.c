
/**
 *  \file appl_hpc.c
 *
 *  This is a HTTP Proxy Collector application.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"

#if (defined ATT && defined HPC)
/**
 * Compilation Feature Flag to allow User Inputs for providing Dynamic
 * values for:
 * - URI
 * - URI Header
 * - URI Entity Body
 *
 * NOTE: If this flag is disabled, default values for the above entities
 *       will be used.
 */
/*#define APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI*/

/**
 * Compilation Feature Flag to allow auto-chain of Control Point Procedures
 */
#define APPL_HPC_HAVE_AUTO_CHAIN_HEADER_BODY_CP_WR

/* ----------------------------------------- Macro Defines */
/**
 * Defines for URI Headers and Entity Body Resource location etc. for
 * HTTP Methods
 */
#define APPL_HPC_SAMPLE_HTTP_BASE_URI_STR                         \
        "http://httpbin.org"

#define APPL_HPC_SAMPLE_HTTP_GET_HEADER_STR                       \
        "Host:httpbin.org\r\nConnection:Close\r\n"

#define APPL_HPC_SAMPLE_HTTP_HEAD_HEADER_STR                      \
        "Host:httpbin.org\r\nConnection:Close\r\n"

#define APPL_HPC_SAMPLE_HTTP_POST_HEADER_STR                      \
        "Host:httpbin.org\r\nContent-Type: text/plain\r\nContent-Length: 22\r\nConnection:Close\r\n"

#define APPL_HPC_SAMPLE_HTTP_PUT_HEADER_STR                       \
        "Host:httpbin.org\r\nContent-Type: text/plain\r\nContent-Length: 21\r\nConnection:Close\r\n"

#define APPL_HPC_SAMPLE_HTTP_DELETE_HEADER_STR                    \
        "Host:httpbin.org\r\nContent-Type: text/plain\r\nContent-Length: 24\r\nConnection:Close\r\n"

#define APPL_HPC_SAMPLE_HTTP_DELETE_EMPTY_BODY_HEADER_STR         \
        "Host:httpbin.org\r\nConnection:Close\r\n"

#define APPL_HPC_SAMPLE_HTTP_POST_ENTITY_BODY_STR                 \
        "POST BODY: HelloWorld!\r\n"

#define APPL_HPC_SAMPLE_HTTP_PUT_ENTITY_BODY_STR                  \
        "PUT BODY: HelloWorld!\r\n"

#define APPL_HPC_SAMPLE_HTTP_DELETE_ENTITY_BODY_STR               \
        "DELETE BODY: HelloWorld!\r\n"

/**
 * Defines for URI Headers and Entity Body Resource location etc for
 * HTTPS Methods
 */
#define APPL_HPC_SAMPLE_HTTPS_BASE_URI_STR                        \
        "https://httpbin.org"

#define APPL_HPC_SAMPLE_HTTPS_GET_HEADER_STR                      \
        "Host:httpbin.org\r\nConnection:Close\r\n"

#define APPL_HPC_SAMPLE_HTTPS_HEAD_HEADER_STR                     \
        "Host:httpbin.org\r\nConnection:Close\r\n"

#define APPL_HPC_SAMPLE_HTTPS_POST_HEADER_STR                     \
        "Host:httpbin.org\r\nContent-Type: text/plain\r\nContent-Length: 22\r\nConnection:Close\r\n"

#define APPL_HPC_SAMPLE_HTTPS_PUT_HEADER_STR                      \
        "Host:httpbin.org\r\nContent-Type: text/plain\r\nContent-Length: 21\r\nConnection:Close\r\n"

#define APPL_HPC_SAMPLE_HTTPS_DELETE_HEADER_STR                   \
        "Host:httpbin.org\r\nContent-Type: text/plain\r\nContent-Length: 24\r\nConnection:Close\r\n"

#define APPL_HPC_SAMPLE_HTTPS_DELETE_EMPTY_BODY_HEADER_STR        \
        "Host:httpbin.org\r\nConnection:Close\r\n"

#define APPL_HPC_SAMPLE_HTTPS_POST_ENTITY_BODY_STR                \
        "POST BODY: HelloWorld!\r\n"

#define APPL_HPC_SAMPLE_HTTPS_PUT_ENTITY_BODY_STR                 \
        "PUT BODY: HelloWorld!\r\n"

#define APPL_HPC_SAMPLE_HTTPS_DELETE_ENTITY_BODY_STR              \
        "DELETE BODY: HelloWorld!\r\n"

/* Default Write Procedures Offset */
#define APPL_HPC_DEFAULT_WR_PROC_OFFSET                      0U

/* Default Read Procedures Offset */
#define APPL_HPC_DEFAULT_RD_PROC_OFFSET                      0U

/* Buffer size for Constructed URI */
#ifdef APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI
#define APPL_HPC_MAX_URI_SIZE                                512U
#else /* APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI */
#define APPL_HPC_MAX_URI_SIZE                                32U
#endif /* APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI */

/* Max buffer for Header and Body */
#define APPL_HPC_MAX_RD_DATA_BUF_LEN                         512U

/* List of HTTP response status codes */
#define APPL_HPC_HTTP_RSP_STATUS_CODE_OK                     200U
#define APPL_HPC_HTTP_RSP_STATUS_CODE_RESRC_NOT_FOUND        404U
#define APPL_HPC_HTTP_RSP_STATUS_CODE_METHOD_NOT_ALLOWED     405U
#define APPL_HPC_HTTP_RSP_STATUS_CODE_CONTINUE               100U
#define APPL_HPC_HTTP_RSP_STATUS_CODE_SWITCHG_PROTOCOL       101U
#define APPL_HPC_HTTP_RSP_STATUS_CODE_MOVED_PERMTLY          301U
#define APPL_HPC_HTTP_RSP_STATUS_CODE_FOUND                  302U
#define APPL_HPC_HTTP_RSP_STATUS_CODE_SEE_OTHER              303U
#define APPL_HPC_HTTP_RSP_STATUS_CODE_USE_PROXY              305U
#define APPL_HPC_HTTP_RSP_STATUS_CODE_TEMP_REDIRECTED        307U
#define APPL_HPC_HTTP_RSP_STATUS_CODE_BAD_REQUEST            400U
#define APPL_HPC_HTTP_RSP_STATUS_CODE_PROC_TIMEOUT           504U

/* List of HTTP response Data Status Codes */
#define APPL_HPC_HTTP_RSP_DATA_STATUS_HEADER_RECEIVED        0x01U
#define APPL_HPC_HTTP_RSP_DATA_STATUS_HEADER_TRUNCATED       0x02U
#define APPL_HPC_HTTP_RSP_DATA_STATUS_BODY_RECEIVED          0x04U
#define APPL_HPC_HTTP_RSP_DATA_STATUS_BODY_TRUNCATED         0x08U

/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE     appl_gatt_client_handle;
extern BT_DEVICE_ADDR g_bd_addr;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
/* Characteristic and Descriptor Handles */
DECL_STATIC ATT_ATTR_HANDLE hps_uri_hdl;
DECL_STATIC ATT_ATTR_HANDLE hps_http_headers_hdl;
DECL_STATIC ATT_ATTR_HANDLE hps_http_entity_body_hdl;
DECL_STATIC ATT_ATTR_HANDLE hps_https_security_hdl;
DECL_STATIC ATT_ATTR_HANDLE hps_http_cp_hdl;
DECL_STATIC ATT_ATTR_HANDLE hps_status_code_hdl;
DECL_STATIC ATT_ATTR_HANDLE hps_status_code_ccc_hdl;

/* Global Variable to Track the current Read Response */
DECL_STATIC ATT_ATTR_HANDLE appl_hpc_cur_rd_hdl;

/* Global Variable for constructed URI */
DECL_STATIC UCHAR appl_hpc_curr_uri[APPL_HPC_MAX_URI_SIZE];
DECL_STATIC UCHAR appl_hpc_curr_cp_opcode;

/*Global variable for  */
#ifdef APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI
DECL_STATIC UCHAR appl_hpc_header_buf[APPL_HPC_MAX_RD_DATA_BUF_LEN];
DECL_STATIC UCHAR appl_hpc_body_buf[APPL_HPC_MAX_RD_DATA_BUF_LEN];
#endif

DECL_STATIC UCHAR appl_hpc_delete_body_code;

/* State for Control Point Write */
DECL_STATIC UCHAR appl_hpc_cp_wt_state;

/* Global Variable to print the Received Header or Body */
DECL_STATIC UCHAR appl_hpc_rd_buf[APPL_HPC_MAX_RD_DATA_BUF_LEN];

DECL_STATIC UCHAR hpc_client_menu[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\
   ----- HPS Discovery procedure ---------\n\
   10 - Discover HTTP Proxy Service\n\
   ----- HPS Status NTF Config procedure ---------\n\
   11 - Enable HTTP Status code Notifications\n\
   12 - Disable HTTP Status code Notification\n\
   ----- HPS Write procedure ---------\n\
   13 - Write URI\n\
   14 - Write HTTP Headers\n\
   15 - Write HTTP Entity\n\
   ----- HPS Control point procedure ---------\n\
   16 - Write HTTP GET/HEAD/POST/PUT/DELETE procedures\n\
   17 - Write HTTP REQ CANCEL\n\
   ----- HPS Read procedure ---------\n\
   18 - Read HTTP Headers\n\
   19 - Read HTTP Entity Body\n\
   20 - Read HTTPS Security\n\
Your Option?\n";

/* ------------------------------- Functions */
API_RESULT appl_hpc_read_op(ATT_ATTR_HANDLE handle);
API_RESULT appl_hpc_write_op
           (
               ATT_ATTR_HANDLE handle,
               UCHAR           * data,
               UINT16          data_len
           );
API_RESULT appl_hpc_write_uri(UCHAR hps_cp_method);
API_RESULT appl_hpc_write_header(void);
API_RESULT appl_hpc_write_body(void);
API_RESULT appl_hpc_write_cp(void);

void hpc_notify_gatt_chardata
     (
         GATT_CHARACTERISTIC_PARAM * characteristic,
         UINT16 size
     )
{
    UINT16 i;
    UINT16 j;

    /* Initialize */
    appl_hpc_curr_cp_opcode   = 0xFFU;
    appl_hpc_cp_wt_state      = 0x00U;
    appl_hpc_delete_body_code = 0x00U;
    BT_mem_set(appl_hpc_rd_buf, 0x0, APPL_HPC_MAX_RD_DATA_BUF_LEN);
    appl_hpc_cur_rd_hdl       = ATT_INVALID_ATTR_HANDLE_VAL;

    /* Parse the Discovery Output and Cache the Handles */
    for (i = 0U; i < size; i++)
    {
        switch (GATT_GET_CHAR_UUID(characteristic))
        {
            case GATT_HPC_URI_CHARACTERISTIC:
                hps_uri_hdl = characteristic->value_handle;
                break;

            case GATT_HPC_HTTP_HEADERS_CHARACTERISTIC:
                hps_http_headers_hdl = characteristic->value_handle;
                break;

            case GATT_HPC_HTTP_ENTITY_BODY_CHARACTERISTIC:
                hps_http_entity_body_hdl = characteristic->value_handle;
                break;

            case GATT_HPC_HTTP_CP_CHARACTERISTIC:
                hps_http_cp_hdl = characteristic->value_handle;
                break;

            case GATT_HPC_HTTP_STATUS_CODE_CHARACTERISTIC:
                hps_status_code_hdl = characteristic->value_handle;
                break;

            case GATT_HPC_HTTPS_SECURITY_CHARACTERISTIC:
                hps_https_security_hdl = characteristic->value_handle;
                break;

            default:
                /* CONSOLE_OUT("Invalid...\n"); */
                break;
        }

        if (0U != characteristic->desc_index)
        {
            for (j = 0U; j < characteristic->desc_index; j++)
            {
                if (GATT_CLIENT_CONFIG ==
                    GATT_GET_CLI_CHAR_DESC_UUID(characteristic,j))
                {
                    switch (GATT_GET_CHAR_UUID(characteristic))
                    {
                    case GATT_HPC_HTTP_STATUS_CODE_CHARACTERISTIC:
                        hps_status_code_ccc_hdl = characteristic->descriptor[j].handle;
                        break;
                    default:
                        /* CONSOLE_OUT("Invalid...\n"); */
                        break;
                    }
                }
            }
        }

        characteristic ++;
    }
}

void hpc_parse_notification_data(UINT16 handle, UCHAR * data, UINT16 datalen)
{
    if (hps_status_code_hdl == handle)
    {
        if (3U == datalen)
        {
            UINT16 http_status;
            UCHAR  data_status;
            CHAR   * tmp_print_val;

            BT_UNPACK_LE_2_BYTE (&http_status, data);
            BT_UNPACK_LE_1_BYTE (&data_status, (&data[2U]));

            /**
             * NOTE: Update the Status and DataStatus Print decoding.
             */
            CONSOLE_OUT("\n[HPC]: HTTP(s) Status Received\n");
            switch(http_status)
            {
                case APPL_HPC_HTTP_RSP_STATUS_CODE_OK:
                    tmp_print_val = (CHAR *)"HTTP_RSP_STATUS_CODE_OK";
                    break;

                case APPL_HPC_HTTP_RSP_STATUS_CODE_RESRC_NOT_FOUND:
                    tmp_print_val = (CHAR *)"HTTP_RSP_STATUS_CODE_RESRC_NOT_FOUND";
                    break;

                case APPL_HPC_HTTP_RSP_STATUS_CODE_METHOD_NOT_ALLOWED:
                    tmp_print_val = (CHAR *)"HTTP_RSP_STATUS_CODE_METHOD_NOT_ALLOWED";
                    break;

                case APPL_HPC_HTTP_RSP_STATUS_CODE_CONTINUE:
                    tmp_print_val = (CHAR *)"HTTP_RSP_STATUS_CODE_CONTINUE";
                    break;

                case APPL_HPC_HTTP_RSP_STATUS_CODE_SWITCHG_PROTOCOL:
                    tmp_print_val = (CHAR *)"HTTP_RSP_STATUS_CODE_SWITCHG_PROTOCOL";
                    break;

                case APPL_HPC_HTTP_RSP_STATUS_CODE_MOVED_PERMTLY:
                    tmp_print_val = (CHAR *)"HTTP_RSP_STATUS_CODE_MOVED_PERMTLY";
                    break;

                case APPL_HPC_HTTP_RSP_STATUS_CODE_FOUND:
                    tmp_print_val = (CHAR *)"HTTP_RSP_STATUS_CODE_RESRC_NOT_FOUND";
                    break;

                case APPL_HPC_HTTP_RSP_STATUS_CODE_SEE_OTHER:
                    tmp_print_val = (CHAR *)"HTTP_RSP_STATUS_CODE_SEE_OTHER";
                    break;

                case APPL_HPC_HTTP_RSP_STATUS_CODE_USE_PROXY:
                    tmp_print_val = (CHAR *)"HTTP_RSP_STATUS_CODE_USE_PROXY";
                    break;

                case APPL_HPC_HTTP_RSP_STATUS_CODE_TEMP_REDIRECTED:
                    tmp_print_val = (CHAR *)"HTTP_RSP_STATUS_CODE_TEMP_REDIRECTED";
                    break;

                case APPL_HPC_HTTP_RSP_STATUS_CODE_BAD_REQUEST:
                    tmp_print_val = (CHAR *)"HTTP_RSP_STATUS_CODE_BAD_REQUEST";
                    break;

                case APPL_HPC_HTTP_RSP_STATUS_CODE_PROC_TIMEOUT:
                    tmp_print_val = (CHAR *)"HTTP_RSP_STATUS_CODE_PROCEDURE_TIMEDOUT";
                    break;

                default:
                    tmp_print_val = (CHAR *)"UNKNOWN";
                    break;
            }

            CONSOLE_OUT("[HPC]: HTTP-status: %4d [%s]\n", http_status, tmp_print_val);
            CONSOLE_OUT("[HPC]: DATA-status: %4d\n", data_status);
        }
    }
}

API_RESULT appl_hpc_read_op(ATT_ATTR_HANDLE handle)
{
    /* Store the Attribute Handle that is being read into the global */
    appl_hpc_cur_rd_hdl = handle;

    /* The Default Read Procedure used here is: READ BLOB */
    return gatt_char_read_long
           (
               &appl_gatt_client_handle,
               handle,
               APPL_HPC_DEFAULT_RD_PROC_OFFSET
           );
}

API_RESULT appl_hpc_write_op
           (
               ATT_ATTR_HANDLE handle,
               UCHAR           * data,
               UINT16          data_len
           )
{
    ATT_PREPARE_WRITE_REQ_PARAM p_wr_req;
    UINT16                      offset;

    offset = APPL_HPC_DEFAULT_WR_PROC_OFFSET;

    /* Send prepare write request */
    p_wr_req.handle_value.handle    = handle;
    p_wr_req.handle_value.value.len = data_len;
    p_wr_req.handle_value.value.val = data;
    p_wr_req.offset                 = offset;

    return gatt_char_wr_long
           (
               &appl_gatt_client_handle,
               &p_wr_req
           );
}

#ifdef APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI
void appl_hpc_read_dyn_header(void)
{
    BT_mem_set(appl_hpc_header_buf,0x0,APPL_HPC_MAX_RD_DATA_BUF_LEN);
    /* Frame the Header According to the Procedure chosen */
    switch(appl_hpc_curr_cp_opcode)
    {
        /* HTTP GET Procedure */
        case 0x01U:
            CONSOLE_OUT("Enter HTTP Header For GET procedure[Max size 512bytes]:\n");
            /* Refer to NOTE-1*/
            CONSOLE_IN("%s",appl_hpc_header_buf);
            break;
        /* HTTP HEAD Procedure */
        case 0x02U:
            CONSOLE_OUT("Enter HTTP Header For HEAD procedure[Max size 512bytes]:\n");
            /* Refer to NOTE-1*/
            CONSOLE_IN("%s",appl_hpc_header_buf);
            break;
        /* HTTP POST Procedure */
        case 0x03U:
            CONSOLE_OUT("Enter HTTP Header For POST procedure[Max size 512bytes]:\n");
            /* Refer to NOTE-1*/
            CONSOLE_IN("%s",appl_hpc_header_buf);
            break;
        /* HTTP PUT Procedure */
        case 0x04U:
            CONSOLE_OUT("Enter HTTP Header For PUT procedure[Max size 512bytes]:\n");
            /* Refer to NOTE-1*/
            CONSOLE_IN("%s",appl_hpc_header_buf);
            break;
        /* HTTP DELETE Procedure */
        case 0x05U:
            /* check if its an Empty body or one with some content */
            CONSOLE_OUT("Enter HTTP Header For DELETE procedure[Max size 512bytes]:\n");
            /* Refer to NOTE-1*/
            CONSOLE_IN("%s",appl_hpc_header_buf);
            break;
        /* HTTPS GET Procedure */
        case 0x06U:
            CONSOLE_OUT("Enter HTTPS Header For GET procedure[Max size 512bytes]:\n");
            /* Refer to NOTE-1*/
            CONSOLE_IN("%s",appl_hpc_header_buf);
            break;
        /* HTTPS HEAD Procedure */
        case 0x07U:
            CONSOLE_OUT("Enter HTTPS Header For HEAD procedure[Max size 512bytes]:\n");
            /* Refer to NOTE-1*/
            CONSOLE_IN("%s",appl_hpc_header_buf);
            break;
        /* HTTPS POST Procedure */
        case 0x08U:
            CONSOLE_OUT("Enter HTTPS Header For POST procedure[Max size 512bytes]:\n");
            /* Refer to NOTE-1*/
            CONSOLE_IN("%s",appl_hpc_header_buf);
            break;
        /* HTTPS PUT Procedure */
        case 0x09U:
            CONSOLE_OUT("Enter HTTPS Header For PUT procedure[Max size 512bytes]:\n");
            /* Refer to NOTE-1*/
            CONSOLE_IN("%s",appl_hpc_header_buf);
            break;
        /* HTTPS DELETE Procedure */
        case 0x0AU:
            /* check if its an Empty body or one with some content */
            CONSOLE_OUT("Enter HTTPS Header For DELETE procedure[Max size 512bytes]:\n");
            /* Refer to NOTE-1*/
            CONSOLE_IN("%s",appl_hpc_header_buf);
            break;
        default:
            CONSOLE_OUT("[HPC]: Invalid Control Point!\n");
            break;
    }

    printf("\n Http header%s\n",&appl_hpc_header_buf[0U]);
    /* Dump the Input for Debug */
    appl_dump_bytes
    (
        &appl_hpc_header_buf[0U],
        (UINT16)BT_str_len((char *)appl_hpc_header_buf)
    );
}

void appl_hpc_read_dyn_body(void)
{
    BT_mem_set(appl_hpc_body_buf,0x0,APPL_HPC_MAX_RD_DATA_BUF_LEN);
    /* Frame the Header According to the Procedure chosen */
    switch(appl_hpc_curr_cp_opcode)
    {
        /* HTTP POST Procedure */
        case 0x03U:
            CONSOLE_OUT("Enter HTTP Body for POST Procedure[Max size 512bytes]:\n");
            /* Refer to NOTE-1*/
            CONSOLE_IN("%s",appl_hpc_body_buf);
            break;
        /* HTTP PUT Procedure */
        case 0x04U:
            CONSOLE_OUT("Enter HTTP Body for PUT Procedure[Max size 512bytes]:\n");
            /* Refer to NOTE-1*/
            CONSOLE_IN("%s",appl_hpc_body_buf);
            break;
        case 0x05U:
            /* HTTP DELETE Procedure */
            /* check if its an Empty body or one with some content */
            if (1U == appl_hpc_delete_body_code)
            {
                CONSOLE_OUT("Enter HTTP Body for DELETE Procedure[Max size 512bytes]:\n");
                /* Refer to NOTE-1*/
                CONSOLE_IN("%s",appl_hpc_body_buf);
            }
            else
            {
                CONSOLE_OUT("[HPC]: Writing HTTP DELETE Empty Entity Body[Max size 512bytes]\n");
            }
            break;
        /* HTTPS POST Procedure */
        case 0x08U:
            CONSOLE_OUT("Enter HTTPS Body for POST Procedure:[Max size 512bytes]\n");
            /* Refer to NOTE-1*/
            CONSOLE_IN("%s",appl_hpc_body_buf);
            break;
        /* HTTPS PUT Procedure */
        case 0x09U:
            CONSOLE_OUT("Enter HTTPS Body for PUT Procedure[Max size 512bytes]:\n");
            /* Refer to NOTE-1*/
            CONSOLE_IN("%s",appl_hpc_body_buf);
            break;
        case 0x0AU:
            /* HTTPS DELETE Procedure */
            /* check if its an Empty body or one with some content */
            if (1U == appl_hpc_delete_body_code)
            {
                CONSOLE_OUT("Enter HTTPS Body for DELETE Procedure[Max size 512bytes]:\n");
                /* Refer to NOTE-1*/
                CONSOLE_IN("%s",appl_hpc_body_buf);
            }
            else
            {
                CONSOLE_OUT("[HPC]: Writing HTTPS DELETE Empty Entity Body\n");
            }
            break;
        default:
            CONSOLE_OUT("[HPC]: Writing Default Empty Entity Body\n");
            break;
    }

    /* Dump the Input for Debug */
    appl_dump_bytes
    (
        &appl_hpc_body_buf[0U],
        (UINT16)BT_str_len((char *)appl_hpc_body_buf)
    );
}

API_RESULT appl_hpc_write_dyn_uri(UCHAR hps_cp_method)
{
    API_RESULT retval;
    UCHAR      * wr_data;
    UINT16     wr_datalen;
    UINT16     uri_len = 0U;

    retval = API_SUCCESS;

    BT_mem_set(appl_hpc_curr_uri, 0x00, APPL_HPC_MAX_URI_SIZE);
    CONSOLE_OUT("Enter URI[Max Len: %dBytes]\n", APPL_HPC_MAX_URI_SIZE);
    /**
     * NOTE-1:
     * Currently using scanf to input the complete string of URI.
     * But using scanf here could lead to unchecked memory overflow esp
     * when the user inputs bytes longer than the desired number.
     */
    CONSOLE_IN("%s",appl_hpc_curr_uri);

    uri_len = BT_str_len(appl_hpc_curr_uri);

    switch(hps_cp_method)
    {
        /* HTTP GET Procedure */
        case 0x01U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[uri_len],
                "/get"
            );
            break;

        /* HTTP HEAD Procedure */
        case 0x02U:
            break;

        /* HTTP POST Procedure */
        case 0x03U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[uri_len],
                "/post"
            );
            break;

        /* HTTP PUT Procedure */
        case 0x04U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[uri_len],
                "/put"
            );
            break;

        /* HTTP DELETE Procedure */
        case 0x05U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[uri_len],
                "/delete"
            );
            break;

        /* HTTPS GET Procedure */
        case 0x06U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[uri_len],
                "/get"
            );
            break;

        /* HTTPS HEAD Procedure */
        case 0x07U:
            break;

        /* HTTPS POST Procedure */
        case 0x08U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[uri_len],
                "/post"
            );
            break;

        /* HTTPS PUT Procedure */
        case 0x09U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[uri_len],
                "/put"
            );
            break;

        /* HTTPS DELETE Procedure */
        case 0x0AU:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[uri_len],
                "/delete"
            );
            break;

        default:
            CONSOLE_OUT("[HPC]: Invalid Option!");
            retval = API_FAILURE;
            break;
    }

    if (API_SUCCESS != retval)
    {
        return retval;
    }

    /* Dump the Input for Debug */
    appl_dump_bytes
    (
        &appl_hpc_curr_uri[0U],
        (UINT16)BT_str_len((char *)appl_hpc_curr_uri)
    );

    CONSOLE_OUT("[HPC]: Send URI: %s\n", (char *)appl_hpc_curr_uri);

    wr_data    = (UCHAR *)appl_hpc_curr_uri;
    wr_datalen = (UINT16)BT_str_len((char *)appl_hpc_curr_uri);

    retval = appl_hpc_write_op
             (
                 hps_uri_hdl,
                 wr_data,
                 wr_datalen
             );
    CONSOLE_OUT("\n[HPC]: Write Op returned 0x%04X\n", retval);

    if (API_SUCCESS == retval )
    {
        /* Mark the global state as Write URI */
        appl_hpc_cp_wt_state = 0x01U;
    }
    else
    {
        appl_hpc_cp_wt_state = 0x00U;
    }

    return retval;
}

API_RESULT appl_hpc_write_dyn_header(void)
{
    API_RESULT retval;
    UCHAR      *wr_data;
    UINT16     wr_datalen;

    retval = API_SUCCESS;

    if(BT_str_len(appl_hpc_header_buf) == 0U)
    {
            retval     = API_FAILURE;
            wr_data    = NULL;
            wr_datalen = 0U;
    }
    else
    {
        wr_data = (UCHAR *)appl_hpc_header_buf;
        wr_datalen = (UINT16)BT_str_len(appl_hpc_header_buf);
    }

    if (API_SUCCESS != retval)
    {
        return retval;
    }

    retval = appl_hpc_write_op
             (
                 hps_http_headers_hdl,
                 wr_data,
                 wr_datalen
             );
    CONSOLE_OUT("\n[HPC]: Write Op returned 0x%04X\n", retval);

    if (API_SUCCESS == retval)
    {
        /* Mark the global state as Write Header */
        appl_hpc_cp_wt_state = 0x02U;
    }
    else
    {
        appl_hpc_cp_wt_state = 0x00U;
    }

    return retval;
}

API_RESULT appl_hpc_write_dyn_body(void)
{
    API_RESULT retval;
    UCHAR      * wr_data;
    UINT16     wr_datalen;

    retval = API_SUCCESS;

    if(BT_str_len(appl_hpc_body_buf) == 0U)
    {
        wr_data    = NULL;
        wr_datalen = 0U;
    }
    else
    {
        wr_data = (UCHAR *)appl_hpc_body_buf;
        wr_datalen = (UINT16)BT_str_len(appl_hpc_body_buf);
    }

    retval = appl_hpc_write_op
             (
                 hps_http_entity_body_hdl,
                 wr_data,
                 wr_datalen
             );
    CONSOLE_OUT("\n[HPC]: Write Op returned 0x%04X\n", retval);

    if (API_SUCCESS == retval)
    {
        /* Mark the global state as Write Entity Body */
        appl_hpc_cp_wt_state = 0x03U;
    }
    else
    {
        appl_hpc_cp_wt_state = 0x00U;
    }

    return retval;
}
#else /* APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI */
API_RESULT appl_hpc_write_uri(UCHAR hps_cp_method)
{
    API_RESULT retval;
    UCHAR      * wr_data;
    UINT16     wr_datalen;

    retval = API_SUCCESS;

    BT_mem_set(appl_hpc_curr_uri, 0x00, APPL_HPC_MAX_URI_SIZE);

    switch(hps_cp_method)
    {
        /* HTTP GET Procedure */
        case 0x01U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[0U],
                "%s/get",
                APPL_HPC_SAMPLE_HTTP_BASE_URI_STR
            );
            break;

        /* HTTP HEAD Procedure */
        case 0x02U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[0U],
                "%s",
                APPL_HPC_SAMPLE_HTTP_BASE_URI_STR
            );
            break;

        /* HTTP POST Procedure */
        case 0x03U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[0U],
                "%s/post",
                APPL_HPC_SAMPLE_HTTP_BASE_URI_STR
            );
            break;

        /* HTTP PUT Procedure */
        case 0x04U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[0U],
                "%s/put",
                APPL_HPC_SAMPLE_HTTP_BASE_URI_STR
            );
            break;

        /* HTTP DELETE Procedure */
        case 0x05U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[0U],
                "%s/delete",
                APPL_HPC_SAMPLE_HTTP_BASE_URI_STR
            );
            break;

        /* HTTPS GET Procedure */
        case 0x06U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[0U],
                "%s/get",
                APPL_HPC_SAMPLE_HTTPS_BASE_URI_STR
            );
            break;

        /* HTTPS HEAD Procedure */
        case 0x07U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[0U],
                "%s",
                APPL_HPC_SAMPLE_HTTPS_BASE_URI_STR
            );
            break;

        /* HTTPS POST Procedure */
        case 0x08U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[0U],
                "%s/post",
                APPL_HPC_SAMPLE_HTTPS_BASE_URI_STR
            );
            break;

        /* HTTPS PUT Procedure */
        case 0x09U:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[0U],
                "%s/put",
                APPL_HPC_SAMPLE_HTTPS_BASE_URI_STR
            );
            break;

        /* HTTPS DELETE Procedure */
        case 0x0AU:
            BT_str_print
            (
                (char *)&appl_hpc_curr_uri[0U],
                "%s/delete",
                APPL_HPC_SAMPLE_HTTPS_BASE_URI_STR
            );
            break;

        default:
            CONSOLE_OUT("[HPC]: Invalid Option!");
            retval = API_FAILURE;
            break;
    }

    if (API_SUCCESS != retval)
    {
        return retval;
    }

    /* Dump the Input for Debug */
    appl_dump_bytes
    (
        &appl_hpc_curr_uri[0U],
        (UINT16)BT_str_len((char *)appl_hpc_curr_uri)
    );

    CONSOLE_OUT("[HPC]: Send URI: %s\n", (char *)appl_hpc_curr_uri);

    wr_data    = (UCHAR *)appl_hpc_curr_uri;
    wr_datalen = (UINT16)BT_str_len((char *)appl_hpc_curr_uri);

    retval = appl_hpc_write_op
             (
                 hps_uri_hdl,
                 wr_data,
                 wr_datalen
             );
    CONSOLE_OUT("\n[HPC]: Write Op returned 0x%04X\n", retval);

    if (API_SUCCESS == retval )
    {
        /* Mark the global state as Write URI */
        appl_hpc_cp_wt_state = 0x01U;
    }
    else
    {
        appl_hpc_cp_wt_state = 0x00U;
    }

    return retval;
}

API_RESULT appl_hpc_write_header(void)
{
    API_RESULT retval;
    UCHAR      * wr_data;
    UINT16     wr_datalen;

    retval = API_SUCCESS;

    /* Frame the Header According to the Procedure chosen */
    switch(appl_hpc_curr_cp_opcode)
    {
        /* HTTP GET Procedure */
        case 0x01U:
            CONSOLE_OUT("[HPC]: Writing HTTP Headers: %s\n",
            APPL_HPC_SAMPLE_HTTP_GET_HEADER_STR);
            wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTP_GET_HEADER_STR;
            wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTP_GET_HEADER_STR);
            break;
        /* HTTP HEAD Procedure */
        case 0x02U:
            CONSOLE_OUT("[HPC]: Writing HTTP Headers: %s\n",
            APPL_HPC_SAMPLE_HTTP_HEAD_HEADER_STR);
            wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTP_HEAD_HEADER_STR;
            wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTP_HEAD_HEADER_STR);
            break;
        /* HTTP POST Procedure */
        case 0x03U:
            CONSOLE_OUT("[HPC]: Writing HTTP Headers: %s\n",
            APPL_HPC_SAMPLE_HTTP_POST_HEADER_STR);
            wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTP_POST_HEADER_STR;
            wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTP_POST_HEADER_STR);
            break;
        /* HTTP PUT Procedure */
        case 0x04U:
            CONSOLE_OUT("[HPC]: Writing HTTP Headers: %s\n",
            APPL_HPC_SAMPLE_HTTP_PUT_HEADER_STR);
            wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTP_PUT_HEADER_STR;
            wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTP_PUT_HEADER_STR);
            break;
        /* HTTP DELETE Procedure */
        case 0x05U:
            /* check if its an Empty body or one with some content */
            if (1U == appl_hpc_delete_body_code)
            {
                CONSOLE_OUT("[HPC]: Writing HTTP Headers: %s\n",
                APPL_HPC_SAMPLE_HTTP_DELETE_HEADER_STR);
                wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTP_DELETE_HEADER_STR;
                wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTP_DELETE_HEADER_STR);
            }
            else
            {
                CONSOLE_OUT("[HPC]: Writing HTTP Headers: %s\n",
                APPL_HPC_SAMPLE_HTTP_DELETE_EMPTY_BODY_HEADER_STR);
                wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTP_DELETE_EMPTY_BODY_HEADER_STR;
                wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTP_DELETE_EMPTY_BODY_HEADER_STR);
            }
            break;
        /* HTTPS GET Procedure */
        case 0x06U:
            CONSOLE_OUT("[HPC]: Writing HTTPS Headers: %s\n",
            APPL_HPC_SAMPLE_HTTPS_GET_HEADER_STR);
            wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTPS_GET_HEADER_STR;
            wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTPS_GET_HEADER_STR);
            break;
        /* HTTPS HEAD Procedure */
        case 0x07U:
            CONSOLE_OUT("[HPC]: Writing HTTPS Headers: %s\n",
            APPL_HPC_SAMPLE_HTTPS_HEAD_HEADER_STR);
            wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTPS_HEAD_HEADER_STR;
            wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTPS_HEAD_HEADER_STR);
            break;
        /* HTTPS POST Procedure */
        case 0x08U:
            CONSOLE_OUT("[HPC]: Writing HTTPS Headers: %s\n",
            APPL_HPC_SAMPLE_HTTPS_POST_HEADER_STR);
            wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTPS_POST_HEADER_STR;
            wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTPS_POST_HEADER_STR);
            break;
        /* HTTPS PUT Procedure */
        case 0x09U:
            CONSOLE_OUT("[HPC]: Writing HTTPS Headers: %s\n",
            APPL_HPC_SAMPLE_HTTPS_PUT_HEADER_STR);
            wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTPS_PUT_HEADER_STR;
            wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTPS_PUT_HEADER_STR);
            break;
        /* HTTPS DELETE Procedure */
        case 0x0AU:
            /* check if its an Empty body or one with some content */
            if (1U == appl_hpc_delete_body_code)
            {
                CONSOLE_OUT("[HPC]: Writing HTTPS Headers: %s\n",
                APPL_HPC_SAMPLE_HTTPS_DELETE_HEADER_STR);
                wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTPS_DELETE_HEADER_STR;
                wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTPS_DELETE_HEADER_STR);
            }
            else
            {
                CONSOLE_OUT("[HPC]: Writing HTTPS Headers: %s\n",
                APPL_HPC_SAMPLE_HTTPS_DELETE_EMPTY_BODY_HEADER_STR);
                wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTPS_DELETE_EMPTY_BODY_HEADER_STR;
                wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTPS_DELETE_EMPTY_BODY_HEADER_STR);
            }
            break;
        default:
            CONSOLE_OUT("[HPC]: Invalid Control Point!\n");
            retval     = API_FAILURE;
            wr_data    = NULL;
            wr_datalen = 0U;
            break;
    }

    if (API_SUCCESS != retval)
    {
        return retval;
    }

    retval = appl_hpc_write_op
             (
                 hps_http_headers_hdl,
                 wr_data,
                 wr_datalen
             );
    CONSOLE_OUT("\n[HPC]: Write Op returned 0x%04X\n", retval);

    if (API_SUCCESS == retval)
    {
        /* Mark the global state as Write Header */
        appl_hpc_cp_wt_state = 0x02U;
    }
    else
    {
        appl_hpc_cp_wt_state = 0x00U;
    }

    return retval;
}

API_RESULT appl_hpc_write_body(void)
{
    API_RESULT retval;
    UCHAR      * wr_data;
    UINT16     wr_datalen;

    retval = API_SUCCESS;

    /* Frame the Header According to the Procedure chosen */
    switch(appl_hpc_curr_cp_opcode)
    {
        /* HTTP POST Procedure */
        case 0x03U:
            CONSOLE_OUT("[HPC]: Writing HTTP POST Entity: %s\n",
            APPL_HPC_SAMPLE_HTTP_POST_ENTITY_BODY_STR);
            wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTP_POST_ENTITY_BODY_STR;
            wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTP_POST_ENTITY_BODY_STR);
            break;
        /* HTTP PUT Procedure */
        case 0x04U:
            CONSOLE_OUT("[HPC]: Writing HTTP PUT Entity: %s\n",
            APPL_HPC_SAMPLE_HTTP_PUT_ENTITY_BODY_STR);
            wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTP_PUT_ENTITY_BODY_STR;
            wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTP_PUT_ENTITY_BODY_STR);
            break;
        case 0x05U:
            /* HTTP DELETE Procedure */
            /* check if its an Empty body or one with some content */
            if (1U == appl_hpc_delete_body_code)
            {
                CONSOLE_OUT("[HPC]: Writing HTTP DELETE Entity: %s\n",
                APPL_HPC_SAMPLE_HTTP_DELETE_ENTITY_BODY_STR);
                wr_data = (UCHAR *)APPL_HPC_SAMPLE_HTTP_DELETE_ENTITY_BODY_STR;
                wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTP_DELETE_ENTITY_BODY_STR);
            }
            else
            {
                CONSOLE_OUT("[HPC]: Writing HTTP DELETE Empty Entity Body\n");
                wr_data    = NULL;
                wr_datalen = 0U;
            }
            break;
        /* HTTPS POST Procedure */
        case 0x08U:
            CONSOLE_OUT("[HPC]: Writing HTTPS POST Entity: %s\n",
            APPL_HPC_SAMPLE_HTTPS_POST_ENTITY_BODY_STR);
            wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTPS_POST_ENTITY_BODY_STR;
            wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTPS_POST_ENTITY_BODY_STR);
            break;
        /* HTTPS PUT Procedure */
        case 0x09U:
            CONSOLE_OUT("[HPC]: Writing HTTPS PUT Entity: %s\n",
            APPL_HPC_SAMPLE_HTTPS_PUT_ENTITY_BODY_STR);
            wr_data    = (UCHAR *)APPL_HPC_SAMPLE_HTTPS_PUT_ENTITY_BODY_STR;
            wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTPS_PUT_ENTITY_BODY_STR);
            break;
        case 0x0AU:
            /* HTTPS DELETE Procedure */
            /* check if its an Empty body or one with some content */
            if (1U == appl_hpc_delete_body_code)
            {
                CONSOLE_OUT("[HPC]: Writing HTTPS DELETE Entity: %s\n",
                APPL_HPC_SAMPLE_HTTPS_DELETE_ENTITY_BODY_STR);
                wr_data = (UCHAR *)APPL_HPC_SAMPLE_HTTPS_DELETE_ENTITY_BODY_STR;
                wr_datalen = (UINT16)BT_str_len(APPL_HPC_SAMPLE_HTTPS_DELETE_ENTITY_BODY_STR);
            }
            else
            {
                CONSOLE_OUT("[HPC]: Writing HTTPS DELETE Empty Entity Body\n");
                wr_data    = NULL;
                wr_datalen = 0U;
            }
            break;
        default:
            CONSOLE_OUT("[HPC]: Writing Default Empty Entity Body\n");
            wr_data    = NULL;
            wr_datalen = 0U;
            break;
    }

    retval = appl_hpc_write_op
             (
                 hps_http_entity_body_hdl,
                 wr_data,
                 wr_datalen
             );
    CONSOLE_OUT("\n[HPC]: Write Op returned 0x%04X\n", retval);

    if (API_SUCCESS == retval)
    {
        /* Mark the global state as Write Entity Body */
        appl_hpc_cp_wt_state = 0x03U;
    }
    else
    {
        appl_hpc_cp_wt_state = 0x00U;
    }

    return retval;
}
#endif /* APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI */

API_RESULT appl_hpc_write_cp(void)
{
    API_RESULT retval;

    retval = API_SUCCESS;

    CONSOLE_OUT("\n[HPC]: Issuing Control Point Wt for Opcode %d\n",
    appl_hpc_curr_cp_opcode);

    gatt_char_wr
    (
        &appl_gatt_client_handle,
        hps_http_cp_hdl,
        &appl_hpc_curr_cp_opcode,
        sizeof(UCHAR),
        0x01U
    );

    /* Resetting the Current Control Point Opcode */
    appl_hpc_curr_cp_opcode   = 0xFFU;

    /* Resetting Entity Body related Code */
    appl_hpc_delete_body_code = 0x00U;

    /* Reset the Global State */
    appl_hpc_cp_wt_state = 0x00U;

    return retval;
}

void hpc_parse_read_data(UCHAR * data, UINT16 datalen)
{
    static UINT16 marker = 0U;

    BT_IGNORE_UNUSED_PARAM(data);
    BT_IGNORE_UNUSED_PARAM(datalen);

    if (((appl_hpc_cur_rd_hdl != hps_http_headers_hdl) &&
        (appl_hpc_cur_rd_hdl != hps_http_entity_body_hdl)) ||
        (ATT_INVALID_ATTR_HANDLE_VAL == appl_hpc_cur_rd_hdl))
    {
        /* Skipping any other read apart from
         * URI Header and Entity Body Characteristics.
         * Also, skipping if the read is not initiated from HPC.
         */
        return;
    }

    if(APPL_HPC_MAX_RD_DATA_BUF_LEN > (marker + datalen))
    {
        /* Copy the data to the buffer */
        BT_mem_copy(&appl_hpc_rd_buf[marker], data, datalen);
        marker += datalen;

        /* Empty Function */
        if ((datalen + 1U) < ATT_DEFAULT_MTU)
        {
            /**
             * Read Procedure is Complete.
             * The Rationale here is that, the underlying
             * GATT Client layer will internally trigger for subsequent
             * Read Blobs with the calculated offset when ever the Server responds
             * with completely filled up MTU.
             */
            if (hps_http_entity_body_hdl == appl_hpc_cur_rd_hdl)
            {
                CONSOLE_OUT("\n[HPC]: HTTP(S) Entity Body read from Remote :\n");
            }
            else
            {
                CONSOLE_OUT("\n[HPC]: HTTP(S) Header read from Remote :\n");
            }

            CONSOLE_OUT("****************************************************************\n");
            CONSOLE_OUT("%s\n", appl_hpc_rd_buf);

            marker = 0U;
            appl_hpc_cur_rd_hdl = ATT_INVALID_ATTR_HANDLE_VAL;
            BT_mem_set(appl_hpc_rd_buf, 0x0, APPL_HPC_MAX_RD_DATA_BUF_LEN);
        }
    }
    else
    {
        /* Something failed. Clear the variables */
        marker = 0U;
        appl_hpc_cur_rd_hdl = ATT_INVALID_ATTR_HANDLE_VAL;
        BT_mem_set(appl_hpc_rd_buf, 0x0, APPL_HPC_MAX_RD_DATA_BUF_LEN);
    }
}

void hpc_notify_write_rsp(void)
{
    /* Empty Function */
}

void hpc_notify_execute_write_rsp(void)
{
#ifdef APPL_HPC_HAVE_AUTO_CHAIN_HEADER_BODY_CP_WR
    CONSOLE_OUT("[HPC]: HPC Execute Write RSP received\n");

    switch(appl_hpc_cp_wt_state)
    {
        /* HTTP URI is written */
        case 0x01U:
            /* Initiate HTTP Header Write */
#ifndef APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI
            appl_hpc_write_header();
#else/* APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI */
            appl_hpc_write_dyn_header();
#endif/* APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI */
            break;

        /* HTTP Header is written */
        case 0x02U:
            /* Initiate HTTP Entity Body Write */
#ifndef APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI
            appl_hpc_write_body();
#else/* APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI */
            appl_hpc_write_dyn_body();
#endif/* APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI */
            break;

        /* HTTP Entity Body is written */
        case 0x03U:
            /* Initiate HTTP Control Point Write */
            appl_hpc_write_cp();
            break;

        default:
            CONSOLE_OUT("Invalid State\n");
            break;
    }
#else /* APPL_HPC_HAVE_AUTO_CHAIN_HEADER_BODY_CP_WR */

    /* Empty Function */

#endif /* APPL_HPC_HAVE_AUTO_CHAIN_HEADER_BODY_CP_WR */
}

void hpc_profile_operations (void)
{
    UINT32     choice, menu_choice;
    UINT16     cli_cfg;
    UCHAR      cfg_val[GATT_CLI_CFG_VAL_LEN];
    API_RESULT retval;
    ATT_UUID   uuid;

    /* Initialize */
    retval = API_SUCCESS;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT(
        "%s",hpc_client_menu);

        CONSOLE_IN ( "%u",&choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /*  return; */

        case 1:
            break;

        case 10:
            uuid.uuid_16 = GATT_HPS_SERVICE;
            gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 11:
            cli_cfg = GATT_CLI_CNFG_NOTIFICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hps_status_code_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 12:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hps_status_code_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 13:
            CONSOLE_OUT("Enter the desired HTTP Procedure to be performed:\n");
            CONSOLE_OUT(" 0x01 - HTTP  GET Request\n");
            CONSOLE_OUT(" 0x02 - HTTP  HEAD Request\n");
            CONSOLE_OUT(" 0x03 - HTTP  POST Request\n");
            CONSOLE_OUT(" 0x04 - HTTP  PUT Request\n");
            CONSOLE_OUT(" 0x05 - HTTP  DELETE Request\n");
            CONSOLE_OUT(" 0x06 - HTTPS GET Request\n");
            CONSOLE_OUT(" 0x07 - HTTPs HEAD Request\n");
            CONSOLE_OUT(" 0x08 - HTTPS POST Request\n");
            CONSOLE_OUT(" 0x09 - HTTPs PUT Request\n");
            CONSOLE_OUT(" 0x0A - HTTPs DELETE Request\n");
            CONSOLE_IN("%x", &choice);

            retval = API_SUCCESS;

            /* Store the Current CP Opcode in Global */
            appl_hpc_curr_cp_opcode = (UCHAR)choice;

            /**
             * For Delete Procedures, check if which Entity Body to be used.
             * The Application can choose to either
             * 1. Delete the previous Entity Body used in "HTTP:PUT"/"HTTPS:PUT"
             * 2. Delete the previous Entity Body used in "HTTP:POST"/"HTTPS:POST"
             * 3. Empty Body
             */
            if ((0x05U == appl_hpc_curr_cp_opcode) ||
                (0x0AU == appl_hpc_curr_cp_opcode))
            {
                CONSOLE_OUT("Enter the desired Entity Body for Delete:\n");
                CONSOLE_OUT(" 0x00 - Use Empty Body\n");
                CONSOLE_OUT(" 0x01 - Use Sample Body\n");

                CONSOLE_IN("%x", &choice);

                appl_hpc_delete_body_code = (UCHAR)choice;

                /* Set Empty Body if invalid value is chosen */
                if (0x01U < appl_hpc_delete_body_code)
                {
                    appl_hpc_delete_body_code = 0x00U;
                }
            }

#ifdef APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI
            /*Reading header and body so that callback context thread should
             * not be blocked during process
             */
            appl_hpc_read_dyn_header();
            appl_hpc_read_dyn_body();
            appl_hpc_write_dyn_uri(appl_hpc_curr_cp_opcode);
#else/* APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI */
            /* Initiate URI Send */
            appl_hpc_write_uri(appl_hpc_curr_cp_opcode);
#endif/* APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI */
            break;

        case 14:
#ifndef APPL_HPC_HAVE_AUTO_CHAIN_HEADER_BODY_CP_WR
#ifndef APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI
            /* Initiate Header Send */
            appl_hpc_write_header();
#else/* APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI */
            appl_hpc_read_dyn_header();
            appl_hpc_write_dyn_header();
#endif/* APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI */
#else /* APPL_HPC_HAVE_AUTO_CHAIN_HEADER_BODY_CP_WR */
            CONSOLE_OUT("[HPC]: Auto Chaining of HTTP Header, Body and Control Point is enabled!\n");
#endif /* APPL_HPC_HAVE_AUTO_CHAIN_HEADER_BODY_CP_WR */
            break;

        case 15:
#ifndef APPL_HPC_HAVE_AUTO_CHAIN_HEADER_BODY_CP_WR
#ifndef APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI
            /* Initiate Body Send */
            appl_hpc_write_body();
#else/* APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI */
            appl_hpc_read_dyn_body();
            appl_hpc_write_dyn_body();
#endif/* APPL_HPC_SUPPORT_DYNAMIC_INPUT_URI */
#else /* APPL_HPC_HAVE_AUTO_CHAIN_HEADER_BODY_CP_WR */
            CONSOLE_OUT("[HPC]: Auto Chaining of HTTP Header, Body and Control Point is enabled!\n");
#endif /* APPL_HPC_HAVE_AUTO_CHAIN_HEADER_BODY_CP_WR */
            break;

        case 16:
#ifndef APPL_HPC_HAVE_AUTO_CHAIN_HEADER_BODY_CP_WR
            /* Initiate Control Point Send */
            appl_hpc_write_cp();
#else /* APPL_HPC_HAVE_AUTO_CHAIN_HEADER_BODY_CP_WR */
            CONSOLE_OUT("[HPC]: Auto Chaining of HTTP Header, Body and Control Point is enabled!\n");
#endif /* APPL_HPC_HAVE_AUTO_CHAIN_HEADER_BODY_CP_WR */
            break;

        case 17:
            CONSOLE_OUT("[HPC]: Sending HTTP REQ CANCEL Control Point\n");
            /* Setting Opcode to REQ CANCEL */
            appl_hpc_curr_cp_opcode = 0x0BU;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hps_http_cp_hdl,
                &appl_hpc_curr_cp_opcode,
                sizeof(UCHAR),
                0x01U
            );

            /* Resetting the Current Control Point Opcode */
            appl_hpc_curr_cp_opcode = 0xFFU;

            /* Reset the Global State */
            appl_hpc_cp_wt_state = 0x00U;
            break;

        case 18:
            CONSOLE_OUT("[HPC]: Reading HTTP Headers\n");
            retval = appl_hpc_read_op(hps_http_headers_hdl);
            CONSOLE_OUT("\n[HPC]: Read Op returned 0x%04X\n", retval);
            break;

        case 19:
            CONSOLE_OUT("[HPC]: Reading HTTP Entity Body\n");
            retval = appl_hpc_read_op(hps_http_entity_body_hdl);
            CONSOLE_OUT("\n[HPC]: Read Op returned 0x%04X\n", retval);
            break;

        case 20:
            CONSOLE_OUT("[HPC]: Reading HTTPS Security\n");
            retval = appl_hpc_read_op(hps_https_security_hdl);
            CONSOLE_OUT("\n[HPC]: Read Op returned 0x%04X\n", retval);
            break;

        default:
            CONSOLE_OUT("[HPC]: Invalid choice!!\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}

#endif /* (defined ATT && defined HPC) */

