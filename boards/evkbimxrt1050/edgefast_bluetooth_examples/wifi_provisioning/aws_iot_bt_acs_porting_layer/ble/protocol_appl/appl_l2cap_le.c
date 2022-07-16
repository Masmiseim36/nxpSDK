
/**
 *  \file appl_l2cap_le.c
 *
 *  Source File for l2cap Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_l2cap_le.h"
#include "appl_utils.h"

/* Configure logs for the functions in this file. */
#ifdef IOT_LOG_LEVEL_GLOBAL
    #define LIBRARY_LOG_LEVEL    IOT_LOG_LEVEL_GLOBAL
#else
    #define LIBRARY_LOG_LEVEL    IOT_LOG_NONE
#endif

#define LIBRARY_LOG_NAME         ( "APPL_L2CAP_LE" )
#include "iot_logging_setup.h"

/* Flag to enable Connection Auto Accept */
#define APPL_LE_L2CAP_CONN_AUTO_ACCEPT

/* Flag to enable sending credits automatically, on reaching Low Water Mark */
#define APPL_LE_L2CAP_AUTO_SEND_CREDIT_ON_LWM

/* Flag to enable sending credits automatically, on receiving a single packet */
#define APPL_LE_L2CAP_AUTO_SEND_INSTANT_CREDIT

/* Flag to enable delay between each packet sent */
#define APPL_LE_L2CAP_DELAY_PKT_SEND

#ifdef L2CAP_SUPPORT_CBFC_MODE

#define APPL_L2CAP_LE_PSM 0x0041
#define APPL_BUFFER_SIZE  0x0062

#define APPL_LE_CO_MTU 100
#define APPL_LE_CO_MPS 50
#define APPL_LE_CREDIT 10
#define APPL_L2CA_RECV_CREDIT_LWM 1

L2CAP_PSM_CBFC appl_l2cap_psm =
{
    appl_l2ca_connect_ind_cb,
    appl_l2ca_connect_cnf_cb,
    appl_l2ca_disconnect_ind_cb,
    appl_l2ca_disconnect_cnf_cb,
    appl_l2ca_data_read_cb,
    appl_l2ca_low_rx_credit_ind_cb,
    appl_l2ca_tx_credit_ind_cb,
    APPL_L2CAP_LE_PSM,

#ifdef L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM
    APPL_L2CA_RECV_CREDIT_LWM,
#endif /* L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM */

    appl_l2ca_data_write_cb

};

L2CAP_CBFC_CONNECT_PARAM connect_param =
{
    APPL_LE_CO_MTU,
    APPL_LE_CO_MPS,
    APPL_LE_CREDIT
};

DECL_STATIC UCHAR * appl_data_buffer;

DECL_STATIC DEVICE_HANDLE appl_peer_handle;
DECL_STATIC UINT16        appl_le_co_cid;
DECL_STATIC UCHAR transfer_char = 0x00;
DECL_STATIC UINT16 appl_le_co_peer_mtu = 0x0000;
#endif /* L2CAP_SUPPORT_CBFC_MODE */
#ifdef BT_LE

/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */
DECL_STATIC const char l2cap_options[] = "\n\
========= LE L2CAP Menu ================== \n\
    0. Exit. \n\
    1. Refresh \n\
\n\
    2. Register Callback.\n\
    3. Send Connection Update Request. \n\
    4. Send Connection Update Response. \n\
\n\
   10. Register PSM.\n\
   11. Connect to Remote PSM\n\
   12. Accept Remote Connection on Local PSM.\n\
   13. Send Data on Channel\n\
   14. Send Transfer Credits\n\
   15. Disconnect Channel\n\
\n\
   50. Send Bulk Data on Channel\n\
Your Option ? \0";



/* ----------------------------------------- Functions */

void main_l2cap_le_operations(void)
{
    unsigned int read, choice;

    BT_DEVICE_ADDR bd_addr;
    DEVICE_HANDLE  device_handle;
    UCHAR  result;
    UINT16 min_interval, max_interval, conn_latency, supervision_timeout;
    API_RESULT retval;

#ifdef L2CAP_SUPPORT_CBFC_MODE
    UINT16 remote_psm, lcid, response;
    UINT32 send_count;
#endif /* L2CAP_SUPPORT_CBFC_MODE */

    BT_LOOP_FOREVER()
    {
        IotLogInfo("%s", l2cap_options);
        (void)l2cap_options;    /*fix build warning: set but never used.*/
        CONSOLE_IN("%d", &choice);

        switch(choice)
        {
        case 0:
            return;

        case 1:
            break;

        case 2:
            retval = BT_l2cap_register_le_event_cb(appl_l2cap_le_callback);

            if(retval != API_SUCCESS)
            {
                IotLogInfo("ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                IotLogInfo("SUCCESS!!\n");
            }
            break;

        case 3:
            IotLogInfo("Enter BD_ADDR : ");
            appl_get_bd_addr(BT_BD_ADDR(&bd_addr));

            IotLogInfo("Enter bd_addr_type : ");
            CONSOLE_IN("%X", &read);
            BT_BD_ADDR_TYPE(&bd_addr) = (UCHAR) read;

            IotLogInfo("Enter min interval : ");
            CONSOLE_IN("%X", &read);
            min_interval = (UINT16) read;

            IotLogInfo("Enter max interval : ");
            CONSOLE_IN("%X", &read);
            max_interval = (UINT16) read;

            IotLogInfo("Enter conn latency : ");
            CONSOLE_IN("%X", &read);
            conn_latency = (UINT16) read;

            IotLogInfo("Enter supervision timeout : ");
            CONSOLE_IN("%X", &read);
            supervision_timeout = (UINT16) read;

            retval = device_queue_search_le_remote_addr (&device_handle,&bd_addr);

            retval = BT_l2cap_le_connection_param_update_request
                     (
                         &device_handle,
                         min_interval,
                         max_interval,
                         conn_latency,
                         supervision_timeout
                     );

            if(retval != API_SUCCESS)
            {
                IotLogInfo("ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                IotLogInfo("SUCCESS!!\n");
            }
            break;

        case 4:
            IotLogInfo("Enter BD_ADDR : ");
            appl_get_bd_addr(BT_BD_ADDR(&bd_addr));

            IotLogInfo("Enter bd_addr_type : ");
            CONSOLE_IN("%X", &read);
            BT_BD_ADDR_TYPE(&bd_addr) = (UCHAR) read;

            IotLogInfo("Enter Result : ");
            CONSOLE_IN("%X", &read);
            result = (UCHAR) read;

            retval = device_queue_search_le_remote_addr(&device_handle, &bd_addr);

            retval = BT_l2cap_le_connection_param_update_response
                     (
                         &device_handle,
                         result
                     );

            if(retval != API_SUCCESS)
            {
                IotLogInfo("ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                IotLogInfo("SUCCESS!!\n");
            }
            break;

#ifdef L2CAP_SUPPORT_CBFC_MODE
        case 10:
            IotLogDebug("Enter the PSM to register (0x): ");
            scanf("%x", &read);

            appl_l2cap_psm.psm = (UINT16)read;
            retval = l2cap_cbfc_register_psm (&appl_l2cap_psm);
            if(retval != API_SUCCESS)
            {
                IotLogInfo("ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                IotLogInfo("SUCCESS!!\n");
            }
            break;

        case 11:
            IotLogInfo("Enter BD_ADDR : ");
            appl_get_bd_addr(BT_BD_ADDR(&bd_addr));

            IotLogInfo("Enter bd_addr_type : ");
            CONSOLE_IN("%X", &read);
            BT_BD_ADDR_TYPE(&bd_addr) = (UCHAR) read;

            IotLogInfo ("Enter Remote PSM (in Hex)\n");
            CONSOLE_IN ("%x",&read);

            remote_psm = (UINT16) read;

            retval = device_queue_search_le_remote_addr(&device_handle, &bd_addr);

            retval = l2ca_cbfc_connect_req
                     (
                         &device_handle,
                         remote_psm,
                         appl_l2cap_psm.psm,
                         &connect_param
                     );
            if(retval != API_SUCCESS)
            {
                IotLogInfo("ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                IotLogInfo("SUCCESS!!\n");
            }
            break;

        case 12:
            IotLogInfo ("Enter LCID (in Hex)\n");
            CONSOLE_IN ("%x",&read);

            lcid = (UINT16) read;

            IotLogInfo ("Enter Response Code (in Hex)\n");
            CONSOLE_IN ("%x",&read);

            response = (UINT16) read;
            appl_le_co_cid = lcid;

            retval = l2ca_cbfc_connect_rsp
                     (
                         lcid,
                         response,
                         &connect_param
                     );
            if(retval != API_SUCCESS)
            {
                IotLogInfo("ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                IotLogInfo("SUCCESS!!\n");
            }
            break;

        case 13:
            appl_le_l2cap_co_send_data
            (
                &appl_peer_handle,
                appl_le_co_cid,
                appl_le_co_peer_mtu, /* APPL_BUFFER_SIZE, */
                1
            );

            break;

        case 14:
            retval = l2ca_cbfc_send_flow_control_credit
                     (
                         appl_le_co_cid,
                         APPL_LE_CREDIT
                     );
            if(retval != API_SUCCESS)
            {
                IotLogInfo("ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                IotLogInfo("SUCCESS!!\n");
            }
            break;

        case 15:
            retval = l2ca_cbfc_disconnect_req (appl_le_co_cid);
            if(retval != API_SUCCESS)
            {
                IotLogInfo("ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                IotLogInfo("SUCCESS!!\n");
            }
            break;

        case 50:
            IotLogInfo ("Enter number of times LE L2CAP CO packet to be sent\n");
            CONSOLE_IN ("%d", &read);

            send_count = (UINT32)read;

            IotLogInfo ("Sending %d times\n", send_count);
            appl_le_l2cap_co_send_data
            (
                &appl_peer_handle,
                appl_le_co_cid,
                appl_le_co_peer_mtu, /* APPL_BUFFER_SIZE, */
                send_count
            );
            break;

#else /* L2CAP_SUPPORT_CBFC_MODE */
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 50:
            IotLogInfo("Enable L2CAP_SUPPORT_CBFC_MODE Feature Flag!");
            break;
#endif /* L2CAP_SUPPORT_CBFC_MODE */

        default:
            IotLogInfo("Invalid choice. Try agin...\n");
            break;
        }
    }
}

void appl_l2cap_le_callback
     (
         DEVICE_HANDLE    * handle,
         UCHAR              event_code,
         UCHAR            * data,
         UINT16             packet_len
     )
{
    UINT16 length, min_interval, max_interval, slave_latency;
    UINT16 supervision_timeout, reason, result;
    API_RESULT retval;
    UINT16   connection_handle;
    BT_DEVICE_ADDR peer_bd_addr;
    UCHAR   * bd_addr;
    UCHAR     bd_addr_type;

    retval = device_queue_get_remote_addr (handle,&peer_bd_addr);

    if (API_SUCCESS != retval)
    {
        IotLogError ("Received LE-L2CAP Callback. No device found\n");
        return;
    }

    bd_addr = BT_BD_ADDR(&peer_bd_addr);
    bd_addr_type = BT_BD_ADDR_TYPE (&peer_bd_addr);
    (void)bd_addr;         /*fix build warning: set but never used.*/
    (void)bd_addr_type;    /*fix build warning: set but never used.*/

    /*
     * This event is sent to application from Stack only when the
     * Local Device is Master. Application doesn't have to check
     * the Local device role again.
     */
    if (L2CAP_CONNECTION_UPDATE_REQUEST_EVENT == event_code)
    {
        /* Default - Set Connection Parameter Rejected as Result */
        result = L2CAP_CONNECTION_PARAMETERS_REJECTED;

        /* Data Packet Length Validation */
        if (0 != packet_len)
        {
            /* Extract L2CAP packet Length */
            BT_UNPACK_LE_2_BYTE(&length, &data[0]);

            /**
             *  Connection Parameter Update Request shall have
             *  8 octet of data fields
             */
            if (8 == length)
            {
                BT_UNPACK_LE_2_BYTE(&min_interval, &data[2]);
                BT_UNPACK_LE_2_BYTE(&max_interval, &data[4]);
                BT_UNPACK_LE_2_BYTE(&slave_latency, &data[6]);
                BT_UNPACK_LE_2_BYTE(&supervision_timeout, &data[8]);

#ifdef APPL_MENU_OPS
        IotLogInfo("L2CAP Connection Update Request\n");
#endif /* APPL_MENU_OPS */
                IotLogDebug("Received : L2CAP_CONNECTION_UPDATE\n");
                IotLogDebug("\tBD_ADDR      : %02X:%02X:%02X:%02X:%02X:%02X\n",
                bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);
                IotLogDebug("\tBD_ADDR_TYPE : %02X\n", bd_addr_type);
                IotLogDebug("\tLength : 0x%04X\n", length);
                IotLogDebug("\tMinimum Interval : 0x%04X\n", min_interval);
                IotLogDebug("\tMaximum Interval : 0x%04X\n", max_interval);
                IotLogDebug("\tSlave Latency : 0x%04X\n", slave_latency);
                IotLogDebug("\tSupervision Timeout : 0x%04X\n", supervision_timeout);

                /* Verify received parameters */
                if ((min_interval < HCI_LE_MIN_CONN_INTRVL_MIN_RANGE) ||
                    (min_interval > HCI_LE_MIN_CONN_INTRVL_MAX_RANGE) ||
                    (max_interval < HCI_LE_MAX_CONN_INTRVL_MIN_RANGE) ||
                    (max_interval > HCI_LE_MAX_CONN_INTRVL_MAX_RANGE) ||
                    (min_interval > max_interval) ||
                    /*(slave_latency < HCI_LE_CONN_LATENCY_MIN_RANGE) || */
                    (slave_latency > HCI_LE_CONN_LATENCY_MAX_RANGE) ||
                    (supervision_timeout < HCI_LE_SUPERVISION_TO_MIN_RANGE) ||
                    (supervision_timeout > HCI_LE_SUPERVISION_TO_MAX_RANGE))
                {
                    IotLogError ("Invalid Parameter\n");
                }
                else
                {
                    result = L2CAP_CONNECTION_PARAMETERS_ACCEPTED;
                }
            }
        }

        /* Send Response */
        retval = BT_l2cap_le_connection_param_update_response
                 (
                     handle,
                     result
                 );

        if (L2CAP_CONNECTION_PARAMETERS_ACCEPTED == result)
        {
            /* Get LE Connection Handle */
            retval = BT_hci_get_le_connection_handle (&peer_bd_addr, &connection_handle);

            if (API_SUCCESS == retval)
            {
                BT_hci_le_connection_update
                (
                     connection_handle,
                     min_interval,
                     max_interval,
                     slave_latency,
                     supervision_timeout,
                     0x0000, /* minimum_ce_length, */
                     0xFFFF  /* maximum_ce_length */
                );
            }
        }
    }
    else if (L2CAP_CONNECTION_UPDATE_RESPONSE_EVENT == event_code)
    {
        BT_UNPACK_LE_2_BYTE(&length, &data[0]);
        BT_UNPACK_LE_2_BYTE(&result, &data[2]);

#ifdef APPL_MENU_OPS
        IotLogInfo("L2CAP Connection Update Response. Result: 0x%04X\n",
            result);
#endif /* APPL_MENU_OPS */
        IotLogDebug("Received : L2CAP_CONNECTION_UPDATE\n");
        IotLogDebug("\tBD_ADDR      : %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);
        IotLogDebug("\tbd_addr_type : %02X\n", bd_addr_type);
        IotLogDebug("\tLength       : %04X\n", length);
        IotLogDebug("\tResult       : %04X\n", result);
    }
    else if (L2CAP_COMMAND_REJECTED_EVENT == event_code)
    {
        BT_UNPACK_LE_2_BYTE(&reason, &data[0]);

#ifdef APPL_MENU_OPS
        IotLogInfo("L2CAP Command Rejected. Reason: 0x%04X\n", reason);
#endif /* APPL_MENU_OPS */
        IotLogDebug("Received : L2CAP_COMMAND_REJ\n");
        IotLogDebug("\tBD_ADDR      : %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);
        IotLogDebug("\tbd_addr_type : %02X\n", bd_addr_type);
        IotLogDebug("\tReason       : %04X\n", reason);
    }
    else
    {
        IotLogDebug("Received Invalid Event. Event = 0x%02X\n", event_code);
        IotLogDebug("\tBD_ADDR      : %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);
        IotLogDebug("\tbd_addr_type : %02X\n", bd_addr_type);
    }
}

#ifdef L2CAP_SUPPORT_CBFC_MODE
API_RESULT appl_l2ca_connect_ind_cb
           (
               DEVICE_HANDLE        * handle,
               UINT16                     lcid,
               UINT16                     psm,
               L2CAP_CBFC_CONNECT_PARAM * param
           )
{
    API_RESULT retval;
    UINT16 response;

    IotLogInfo (
    "\n -> appl_l2ca_connect_ind_cb from Device %02X, lcid %04X, PSM 0x%04X\n",
    (*handle), lcid, psm);

    IotLogInfo (
    "\n MTU:0x%04X MPS:0x%04X Credit:0x%04X\n", param->mtu, param->mps, param->credit);

    appl_peer_handle = (*handle);
    appl_le_co_cid = lcid;
    appl_le_co_peer_mtu = param->mtu;

    retval = API_SUCCESS;

#ifdef APPL_LE_L2CAP_CONN_AUTO_ACCEPT
    response = 0x0000;
    retval = l2ca_cbfc_connect_rsp
             (
                 lcid,
                 response,
                 &connect_param
             );
#endif /* APPL_LE_L2CAP_CONN_AUTO_ACCEPT */

    return retval;
}

API_RESULT appl_l2ca_connect_cnf_cb
           (
               DEVICE_HANDLE        * handle,
               UINT16                     lcid,
               UINT16                     response,
               L2CAP_CBFC_CONNECT_PARAM * param
           )
{
    IotLogInfo (
     "\n -> appl_l2ca_connect_cnf_cb from Device %02X, lcid %04X, response 0x%04X\n",
    (*handle), lcid, response);

    /* Print MTU, MPS and Credit, on success response */
    if (L2CAP_CONNECTION_SUCCESSFUL == response)
    {
        IotLogInfo (
        "\n MTU:0x%04X MPS:0x%04X Credit:0x%04X\n", param->mtu, param->mps, param->credit);

        appl_peer_handle = (*handle);
        appl_le_co_cid = lcid;
        appl_le_co_peer_mtu = param->mtu;
    }

    return API_SUCCESS;
}

API_RESULT appl_l2ca_disconnect_ind_cb(UINT16 lcid)
{
    IotLogInfo ("\n -> appl_l2ca_disconnect_ind_cb, lcid %04X\n",lcid);

    /* RESET peer MTU */
    appl_le_co_peer_mtu = 0x0000;

    /* RESET transfer_char after disconnection */
    transfer_char = 0x00;

#ifndef L2CAP_ALLOC_FOR_CBFC_SDU
    if (NULL != appl_data_buffer)
    {
        /* Free buffer. L2CAP has allocated and copied the data */
        BT_free_mem (appl_data_buffer);

        /* Resetting global data pointer after free */
        appl_data_buffer = NULL;
    }
#endif /* L2CAP_ALLOC_FOR_CBFC_SDU */

    return API_SUCCESS;
}

API_RESULT appl_l2ca_disconnect_cnf_cb(UINT16 lcid, UINT16 reason)
{
    BT_IGNORE_UNUSED_PARAM(reason);

    IotLogInfo ("\n -> appl_l2ca_disconnect_cnf_cb lcid %04X\n",lcid);

    /* RESET peer MTU */
    appl_le_co_peer_mtu = 0x0000;

    /* RESET transfer_char after disconnection */
    transfer_char = 0x00;

#ifndef L2CAP_ALLOC_FOR_CBFC_SDU
    if (NULL != appl_data_buffer)
    {
        /* Free buffer. L2CAP has allocated and copied the data */
        BT_free_mem (appl_data_buffer);

        /* Resetting global data pointer after free */
        appl_data_buffer = NULL;
    }
#endif /* L2CAP_ALLOC_FOR_CBFC_SDU */

    return API_SUCCESS;
}

API_RESULT appl_l2ca_data_read_cb (UINT16 lcid, UINT16 result, UCHAR * data, UINT16 datalen)
{
    API_RESULT retval;
    UINT16 no_of_credits;

    IotLogInfo (
    "\n -> appl_l2ca_data_read_cb, lcid %04X, data len %04X, result 0x%04X\n",
    lcid, datalen, result);

    appl_dump_bytes(data, datalen);

    retval = API_SUCCESS;

#ifdef APPL_LE_L2CAP_AUTO_SEND_INSTANT_CREDIT
    /**
     * Calculate possible number of LE L2CAP Frames.
     *
     * Maximum size of each frame shall not exceed local MPS.
     * Account for the 2 octet of SDU length.
     */
    no_of_credits = (datalen + 2 + APPL_LE_CO_MPS - 1)/APPL_LE_CO_MPS;

    retval = l2ca_cbfc_send_flow_control_credit
             (
                 lcid,
                 no_of_credits
             );
#endif /* APPL_LE_L2CAP_AUTO_SEND_INSTANT_CREDIT */

    return retval;
}

API_RESULT appl_l2ca_low_rx_credit_ind_cb (UINT16 lcid, UINT16 credit)
{
    API_RESULT retval;

    IotLogInfo (
    "\n -> appl_l2ca_low_rx_credit_ind_cb, lcid %04X, Credit %04X\n",
    lcid,credit);

    retval = API_SUCCESS;

#ifdef APPL_LE_L2CAP_AUTO_SEND_CREDIT_ON_LWM
    retval = l2ca_cbfc_send_flow_control_credit
             (
                 lcid,
                 APPL_LE_CREDIT
             );
#endif /* APPL_LE_L2CAP_AUTO_SEND_CREDIT_ON_LWM */

    return retval;
}

API_RESULT appl_l2ca_tx_credit_ind_cb (UINT16 lcid, UINT16 result, UINT16 credit)
{
    IotLogInfo (
    "\n -> appl_l2ca_tx_credit_cb, lcid %04X, Credit %04X, Result %04X\n",
    lcid, credit, result);

    /**
     * Disconnect channel if credit overflow has occured. This is in accordance
     * text in section 10.1 of the Core 5.0 specification
     */
    if (L2CAP_CREDIT_OVERFLOW == result)
    {
        l2ca_cbfc_disconnect_req(lcid);
    }

    return API_SUCCESS;
}

API_RESULT appl_l2ca_data_write_cb(UINT16 lcid, UINT16 result, UCHAR *buffer, UINT16 buffer_len)
{
    BT_IGNORE_UNUSED_PARAM(buffer);
    BT_IGNORE_UNUSED_PARAM(buffer_len);

#if 0
    IotLogInfo(
        "\n -> appl_l2ca_data_write_cb, lcid %04X, Result %04X\n",
        lcid, result);
#else
    BT_IGNORE_UNUSED_PARAM(lcid);
    BT_IGNORE_UNUSED_PARAM(result);

#ifndef L2CAP_ALLOC_FOR_CBFC_SDU
    if (NULL != appl_data_buffer)
    {
        /* Free buffer. L2CAP has allocated and copied the data */
        BT_free_mem (appl_data_buffer);

        /* Resetting global data pointer after free */
        appl_data_buffer = NULL;
    }
#endif /* L2CAP_ALLOC_FOR_CBFC_SDU */
#endif /* 0 */

    return API_SUCCESS;
}

API_RESULT appl_le_l2cap_co_send_data
           (
               /* IN */ DEVICE_HANDLE * device_handle,
               /* IN */ UINT16          cid,
               /* IN */ UINT16          buffer_len,
               /* IN */ UINT32          count
           )
{
    UINT32  index;
    API_RESULT retval;
    UCHAR   additional_l2cap_hdr_len;

    retval = API_SUCCESS;

    /*
     * If L2CAP_ALLOC_FOR_CBFC_SDU is defined,
     * do not allocate additional buffer for L2CAP Header.
     */
#ifdef L2CAP_ALLOC_FOR_CBFC_SDU
    additional_l2cap_hdr_len = 0;
#else
    additional_l2cap_hdr_len = L2CAP_CREDIT_MODE_SDU_HDR_LEN;
#endif /* L2CAP_ALLOC_FOR_CBFC_SDU */

    for (index = 0; index < count; index ++)
    {
        appl_data_buffer = BT_alloc_mem (buffer_len + additional_l2cap_hdr_len);

        if (NULL == appl_data_buffer)
        {
            IotLogInfo ("Memory Allocation Failed!\n");
        }
        else
        {
#ifndef L2CAP_ALLOC_FOR_CBFC_SDU
            /* If L2CAP_ALLOC_FOR_CBFC_SDU is not defined, add SDU Header Length */
            BT_PACK_LE_2_BYTE_VAL (appl_data_buffer, buffer_len);
#endif /* L2CAP_ALLOC_FOR_CBFC_SDU */

            transfer_char++;

            /* Fill with pattern */
            BT_mem_set
            (
                appl_data_buffer+additional_l2cap_hdr_len,
                transfer_char,
                buffer_len
            );

            IotLogInfo (
            "\n -> Sending data over LE L2CAP CO Channel\n");

            /* Display Information Payload on console */
            appl_dump_bytes(appl_data_buffer+additional_l2cap_hdr_len, buffer_len);

#ifndef L2CAP_ALLOC_FOR_CBFC_SDU
            /*
             * If L2CAP_ALLOC_FOR_CBFC_SDU is not defined,
             * include length of 'SDU Header Length' field (2 Octets)
             * in the buffer length.
             */
            buffer_len += L2CAP_CREDIT_MODE_SDU_HDR_LEN;
#endif /* L2CAP_ALLOC_FOR_CBFC_SDU */

            retval = l2ca_channel_data_write
                     (
                          device_handle,
                          cid,
                          appl_data_buffer,
                          buffer_len
                     );

            if(retval != API_SUCCESS)
            {
                IotLogInfo("ERROR!!! retval = 0x%04X", retval);
                break;
            }
            else
            {
                IotLogInfo("SUCCESS!!\n");

            #ifdef APPL_LE_L2CAP_DELAY_PKT_SEND
                BT_sleep(1);
            #endif /* APPL_LE_L2CAP_DELAY_PKT_SEND */
            }
    #ifdef L2CAP_ALLOC_FOR_CBFC_SDU
            /* Free buffer. L2CAP has allocated and copied the data */
            BT_free_mem (appl_data_buffer);
    #endif /* L2CAP_ALLOC_FOR_CBFC_SDU */
        }
    }

    return retval;
}

#endif /* L2CAP_SUPPORT_CBFC_MODE */

void appl_l2cap_le_register(void)
{
    BT_l2cap_register_le_event_cb(appl_l2cap_le_callback);
}

#endif /* BT_LE */

