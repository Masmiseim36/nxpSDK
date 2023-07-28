
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

/* Flag to enable Connection Auto Accept */
#define APPL_LE_L2CAP_CONN_AUTO_ACCEPT

/* Flag to enable sending credits automatically, on reaching Low Water Mark */
#define APPL_LE_L2CAP_AUTO_SEND_CREDIT_ON_LWM

/* Flag to enable sending credits automatically, on receiving a single packet */
#define APPL_LE_L2CAP_AUTO_SEND_INSTANT_CREDIT

/* Flag to enable delay between each packet sent */
#define APPL_LE_L2CAP_DELAY_PKT_SEND

#ifdef L2CAP_SUPPORT_CBFC_MODE

#define APPL_L2CAP_LE_PSM 0x0041U
#define APPL_BUFFER_SIZE  0x0062U

#define APPL_LE_CO_MTU 100U
#define APPL_LE_CO_MPS 50U
#define APPL_LE_CREDIT 10U
#define APPL_L2CA_RECV_CREDIT_LWM 2U

DECL_STATIC L2CAP_PSM_CBFC appl_l2cap_psm =
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

#ifdef L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT
L2CAP_CBFC_CONNECT_PARAM connect_param =
{
    APPL_LE_CO_MTU,
    APPL_LE_CO_MPS,
    APPL_LE_CREDIT,
    0x0000
};
#else /* L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT */
L2CAP_CBFC_CONNECT_PARAM connect_param =
{
    APPL_LE_CO_MTU,
    APPL_LE_CO_MPS,
    APPL_LE_CREDIT
};
#endif /* L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT */

DECL_STATIC UCHAR * appl_data_buffer;

DECL_STATIC DEVICE_HANDLE appl_peer_handle;
DECL_STATIC UINT16        appl_le_co_cid;
DECL_STATIC UCHAR transfer_char = 0x00U;
DECL_STATIC UINT16 appl_le_co_peer_mtu = 0x0000U;

#ifdef APPL_LE_L2CAP_CONN_AUTO_ACCEPT
DECL_STATIC UCHAR appl_l2cap_cbfc_automatic_mode = BT_TRUE;
#endif /* APPL_LE_L2CAP_CONN_AUTO_ACCEPT */

#endif /* L2CAP_SUPPORT_CBFC_MODE */
#ifdef BT_LE

/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
DECL_STATIC DECL_CONST CHAR l2cap_options[] = "\n\
========= LE L2CAP Menu ==================\n\
  0. Exit\n\
  1. Refresh\n\
\n\
  2. Register LE Signalling Channel Callback\n\
  3. Send Connection Update Request\n\
  4. Send Connection Update Response\n\
\n\
 10. Register L2CAP CBFC SPSM\n\
 11. Connect to Remote SPSM\n\
 12. Accept Remote Connection on Local SPSM\n\
 13. Send Data on Channel\n\
 14. Send Transfer Credits\n\
 15. Disconnect Channel\n\
 16. Unregister L2CAP CBFC SPSM\n\
\n\
 20. Set Auto Response Mode\n\
\n\
 30. Set Current CBFC MTU/MPS/Credit Values\n\
 31. Get Current CBFC MTU/MPS/Credit Values\n\
\n\
 50. Send Bulk Data on Channel\n\
Your Option ?";

/* ----------------------------------------- Functions */

void main_l2cap_le_operations(void)
{
    unsigned int read_val, choice, menu_choice;

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
        CONSOLE_OUT("%s", l2cap_options);
        CONSOLE_IN("%d", &choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            retval = BT_l2cap_register_le_event_cb(appl_l2cap_le_callback);

            if(retval != API_SUCCESS)
            {
                CONSOLE_OUT("ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                CONSOLE_OUT("SUCCESS!!\n");
            }
            break;

        case 3:
            CONSOLE_OUT("Enter BD_ADDR : ");
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(BT_BD_ADDR(&bd_addr));

            CONSOLE_OUT("Enter bd_addr_type : ");
            CONSOLE_IN("%X", &read_val);
            BT_BD_ADDR_TYPE(&bd_addr) = (UCHAR) read_val;

            CONSOLE_OUT("Enter min interval : ");
            CONSOLE_IN("%X", &read_val);
            min_interval = (UINT16) read_val;

            CONSOLE_OUT("Enter max interval : ");
            CONSOLE_IN("%X", &read_val);
            max_interval = (UINT16) read_val;

            CONSOLE_OUT("Enter conn latency : ");
            CONSOLE_IN("%X", &read_val);
            conn_latency = (UINT16) read_val;

            CONSOLE_OUT("Enter supervision timeout : ");
            CONSOLE_IN("%X", &read_val);
            supervision_timeout = (UINT16) read_val;

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
                CONSOLE_OUT("ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                CONSOLE_OUT("SUCCESS!!\n");
            }
            break;

        case 4:
            CONSOLE_OUT("Enter BD_ADDR : ");
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(BT_BD_ADDR(&bd_addr));

            CONSOLE_OUT("Enter bd_addr_type : ");
            CONSOLE_IN("%X", &read_val);
            BT_BD_ADDR_TYPE(&bd_addr) = (UCHAR) read_val;

            CONSOLE_OUT("Enter Result : ");
            CONSOLE_IN("%X", &read_val);
            result = (UCHAR) read_val;

            retval = device_queue_search_le_remote_addr(&device_handle, &bd_addr);

            retval = BT_l2cap_le_connection_param_update_response
                     (
                         &device_handle,
                         result
                     );

            if(retval != API_SUCCESS)
            {
                CONSOLE_OUT("ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                CONSOLE_OUT("SUCCESS!!\n");
            }
            break;

#ifdef L2CAP_SUPPORT_CBFC_MODE
        case 10:
            printf("Enter the PSM to register (0x): ");
            scanf("%x", &read_val);

            appl_l2cap_psm.psm = (UINT16)read_val;
            retval = l2cap_cbfc_register_psm (&appl_l2cap_psm);
            if(retval != API_SUCCESS)
            {
                CONSOLE_OUT("ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                CONSOLE_OUT("SUCCESS!!\n");
            }
            break;

        case 11:
            CONSOLE_OUT("Enter BD_ADDR : ");
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(BT_BD_ADDR(&bd_addr));

            CONSOLE_OUT("Enter bd_addr_type : ");
            CONSOLE_IN("%X", &read_val);
            BT_BD_ADDR_TYPE(&bd_addr) = (UCHAR) read_val;

            CONSOLE_OUT ("Enter Remote PSM (in Hex)\n");
            CONSOLE_IN ("%x",&read_val);

            remote_psm = (UINT16) read_val;

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
                CONSOLE_OUT("ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                CONSOLE_OUT("SUCCESS!!\n");

#ifdef L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT
                CONSOLE_OUT("Local CID Allocated - 0x%04X\n", connect_param.cid);
#endif /* L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT */
            }
            break;

        case 12:
            CONSOLE_OUT ("Enter LCID (in Hex)\n");
            CONSOLE_IN ("%x",&read_val);

            lcid = (UINT16) read_val;

            CONSOLE_OUT ("Enter Response Code (in Hex)\n");
            CONSOLE_IN ("%x",&read_val);

            response = (UINT16) read_val;
            appl_le_co_cid = lcid;

            retval = l2ca_cbfc_connect_rsp
                     (
                         lcid,
                         response,
                         &connect_param
                     );
            if(retval != API_SUCCESS)
            {
                CONSOLE_OUT("ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                CONSOLE_OUT("SUCCESS!!\n");
            }
            break;

        case 13:
            {
                UINT16 datalen_to_send;

                CONSOLE_OUT ("Enter Length of Data to transmit (in Decimal)\n");
                CONSOLE_IN ("%d", &read_val);

                datalen_to_send = (UINT16)read_val;

                if (appl_le_co_peer_mtu < datalen_to_send)
                {
                    datalen_to_send = appl_le_co_peer_mtu;
                }

                (BT_IGNORE_RETURN_VALUE) appl_le_l2cap_co_send_data
                (
                    &appl_peer_handle,
                    appl_le_co_cid,
                    datalen_to_send,
                    1U
                );
            }
            break;

        case 14:
            {
                UINT16 credits_to_send;

                CONSOLE_OUT ("Enter Number of Creadits to send (in Decimal)\n");
                CONSOLE_IN ("%d", &read_val);

                credits_to_send = (UINT16)read_val;

                retval = l2ca_cbfc_send_flow_control_credit
                         (
                             appl_le_co_cid,
                             credits_to_send
                         );
                if (retval != API_SUCCESS)
                {
                    CONSOLE_OUT("ERROR!!! retval = 0x%04X", retval);
                }
                else
                {
                    CONSOLE_OUT("SUCCESS!!\n");
                }
            }
            break;

        case 15:
            retval = l2ca_cbfc_disconnect_req (appl_le_co_cid);
            if(retval != API_SUCCESS)
            {
                CONSOLE_OUT("ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                CONSOLE_OUT("SUCCESS!!\n");
            }
            break;

        case 16:
            {
#ifndef L2CAP_NO_UNREGISTER_LE_PSM
                CONSOLE_OUT("Enter the PSM to Unregister (0x): ");
                CONSOLE_IN("%x", &read_val);

                appl_l2cap_psm.psm = (UINT16)read_val;
                retval = l2cap_cbfc_unregister_psm (appl_l2cap_psm.psm);

                if(retval != API_SUCCESS)
                {
                    CONSOLE_OUT("ERROR!!! retval = 0x%04X", retval);
                }
                else
                {
                    CONSOLE_OUT("PSM 0x%04X Unregister Successful!!\n", appl_l2cap_psm.psm);
                }
#else /* L2CAP_NO_UNREGISTER_LE_PSM */
                CONSOLE_OUT("Unregister L2CAP CBFC PSM Feature Not Enabled!\n");
#endif /* L2CAP_NO_UNREGISTER_LE_PSM */
            }
            break;

        case 20:
#ifdef APPL_LE_L2CAP_CONN_AUTO_ACCEPT
            CONSOLE_OUT("Enter mode (0: Non-automatic, 1: Automatic) : ");
            CONSOLE_IN("%d", &read_val);
            appl_l2cap_cbfc_automatic_mode = (0U == (UCHAR)read_val) ? BT_FALSE : BT_TRUE;
#else /* APPL_LE_L2CAP_CONN_AUTO_ACCEPT */
            CONSOLE_OUT("Compilation flag APPL_LE_L2CAP_CONN_AUTO_ACCEPT Disabled!\n");
#endif /* APPL_LE_L2CAP_CONN_AUTO_ACCEPT */
            break;

        case 30:
            {
                UCHAR t_config_option;

                CONSOLE_OUT(
                "\nCurrent L2CAP CBFC MTU=%d, MPS=%d, Credits=%d\n",
                connect_param.mtu, connect_param.mps, connect_param.credit);

                CONSOLE_OUT("Set New Values for What?\n1. MTU\n2. MPS:\n3. Credits:\n");
                CONSOLE_IN("%d", &read_val);
                t_config_option = (UCHAR)read_val;

                /* Set MTU value */
                if (1 == t_config_option)
                {
                    CONSOLE_OUT("Enter Desired MTU Value to be used in CoC req/res [in Dec]: ");
                    CONSOLE_IN("%d", &read_val);
                    if (0 != read_val)
                    {
                        connect_param.mtu = (UINT16)read_val;
                    }
                }

                /* Set MPS value */
                if (2 == t_config_option)
                {
                    CONSOLE_OUT("Enter Desired MPS Value to be used in CoC req/res [in Dec]: ");
                    CONSOLE_IN("%d", &read_val);
                    if (0 != read_val)
                    {
                        connect_param.mps = (UINT16)read_val;
                    }
                }

                /* Set Credits value */
                if (3 == t_config_option)
                {
                    CONSOLE_OUT("Enter Desired Credit Value to be used in CoC req/res [in Dec]: ");
                    CONSOLE_IN("%d", &read_val);
                    if (0 != read_val)
                    {
                        connect_param.credit = (UINT16)read_val;
                    }
                }

                CONSOLE_OUT(
                "\nUpdated values of L2CAP CBFC MTU=%d, MPS=%d, Credits=%d\n",
                connect_param.mtu, connect_param.mps, connect_param.credit);
            }
            break;

        case 31:
            {
                CONSOLE_OUT(
                "\nCurrent L2CAP CBFC MTU=%d, MPS=%d, Credits=%d\n",
                connect_param.mtu, connect_param.mps, connect_param.credit);
            }
            break;

        case 50:
            CONSOLE_OUT ("Enter number of times LE L2CAP CO packet to be sent\n");
            CONSOLE_IN ("%d", &read_val);

            send_count = (UINT32)read_val;

            CONSOLE_OUT ("Sending %d times\n", send_count);
            (BT_IGNORE_RETURN_VALUE) appl_le_l2cap_co_send_data
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
            CONSOLE_OUT("Enable L2CAP_SUPPORT_CBFC_MODE Feature Flag!");
            break;
#endif /* L2CAP_SUPPORT_CBFC_MODE */

        default:
            CONSOLE_OUT("Invalid choice. Try again...\n");
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

    retval = device_queue_get_remote_addr (handle, &peer_bd_addr);

    if (API_SUCCESS != retval)
    {
        APPL_ERR ("Received LE-L2CAP Callback. No device found\n");
        /* return; */
    }
    else
    {
        bd_addr = BT_BD_ADDR(&peer_bd_addr);
        bd_addr_type = BT_BD_ADDR_TYPE (&peer_bd_addr);

        /* Initialize locals */
        min_interval = 0U;
        max_interval = 0U;
        slave_latency = 0U;
        supervision_timeout = 0U;

        /**
         * This event is sent to application from Stack only when the
         * Local Device is Master. Application doesn't have to check
         * the Local device role again.
         */
        if (L2CAP_CONNECTION_UPDATE_REQUEST_EVENT == event_code)
        {
            /* Default - Set Connection Parameter Rejected as Result */
            result = L2CAP_CONNECTION_PARAMETERS_REJECTED;

            /* Data Packet Length Validation */
            if (0U != packet_len)
            {
                /* Extract L2CAP packet Length */
                BT_UNPACK_LE_2_BYTE(&length, &data[0U]);

                /**
                 *  Connection Parameter Update Request shall have
                 *  8 octet of data fields
                 */
                if (8U == length)
                {
                    BT_UNPACK_LE_2_BYTE(&min_interval, &data[2U]);
                    BT_UNPACK_LE_2_BYTE(&max_interval, &data[4U]);
                    BT_UNPACK_LE_2_BYTE(&slave_latency, &data[6U]);
                    BT_UNPACK_LE_2_BYTE(&supervision_timeout, &data[8U]);

#ifdef APPL_MENU_OPS
                    CONSOLE_OUT("L2CAP Connection Update Request\n");
#endif /* APPL_MENU_OPS */
                    APPL_TRC("Received : L2CAP_CONNECTION_UPDATE\n");
                    APPL_TRC("\tBD_ADDR      : %02X:%02X:%02X:%02X:%02X:%02X\n",
                    bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
                    APPL_TRC("\tBD_ADDR_TYPE : %02X\n", bd_addr_type);
                    APPL_TRC("\tLength : 0x%04X\n", length);
                    APPL_TRC("\tMinimum Interval : 0x%04X\n", min_interval);
                    APPL_TRC("\tMaximum Interval : 0x%04X\n", max_interval);
                    APPL_TRC("\tSlave Latency : 0x%04X\n", slave_latency);
                    APPL_TRC("\tSupervision Timeout : 0x%04X\n", supervision_timeout);

                    /* Verify received parameters */
                    if ((min_interval < HCI_LE_MIN_CONN_INTRVL_MIN_RANGE) ||
                        (min_interval > HCI_LE_MIN_CONN_INTRVL_MAX_RANGE) ||
                        (max_interval < HCI_LE_MAX_CONN_INTRVL_MIN_RANGE) ||
                        (max_interval > HCI_LE_MAX_CONN_INTRVL_MAX_RANGE) ||
                        (min_interval > max_interval) ||
                        /* (slave_latency < HCI_LE_CONN_LATENCY_MIN_RANGE) || */ /* fix for warning: pointless comparison */
                        (slave_latency > HCI_LE_CONN_LATENCY_MAX_RANGE) ||
                        (supervision_timeout < HCI_LE_SUPERVISION_TO_MIN_RANGE) ||
                        (supervision_timeout > HCI_LE_SUPERVISION_TO_MAX_RANGE))
                    {
                        APPL_ERR ("Invalid Parameter\n");
                    }
                    else
                    {
                        result = L2CAP_CONNECTION_PARAMETERS_ACCEPTED;
                    }
                }
                else
                {
                    APPL_ERR("Invalid Parameter Length: %d\n", length);
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
                    (BT_IGNORE_RETURN_VALUE) BT_hci_le_connection_update
                    (
                        connection_handle,
                        min_interval,
                        max_interval,
                        slave_latency,
                        supervision_timeout,
                        0x0000U, /* minimum_ce_length, */
                        0xFFFFU  /* maximum_ce_length */
                    );
                }
            }
        }
        else if (L2CAP_CONNECTION_UPDATE_RESPONSE_EVENT == event_code)
        {
            BT_UNPACK_LE_2_BYTE(&length, &data[0U]);
            BT_UNPACK_LE_2_BYTE(&result, &data[2U]);

#ifdef APPL_MENU_OPS
            CONSOLE_OUT("L2CAP Connection Update Response. Result: 0x%04X\n",
            result);
#endif /* APPL_MENU_OPS */
            APPL_TRC("Received : L2CAP_CONNECTION_UPDATE\n");
            APPL_TRC("\tBD_ADDR      : %02X:%02X:%02X:%02X:%02X:%02X\n",
            bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
            APPL_TRC("\tbd_addr_type : %02X\n", bd_addr_type);
            APPL_TRC("\tLength       : %04X\n", length);
            APPL_TRC("\tResult       : %04X\n", result);
        }
        else if (L2CAP_COMMAND_REJECTED_EVENT == event_code)
        {
            /**
             * Not extracting and validating the "length" parameter.
             * Extracting and printing only the "reason".
             */
            BT_UNPACK_LE_2_BYTE(&reason, &data[2U]);

#ifdef APPL_MENU_OPS
            CONSOLE_OUT("L2CAP Command Rejected. Reason: 0x%04X\n", reason);
#endif /* APPL_MENU_OPS */
            APPL_TRC("Received : L2CAP_COMMAND_REJ\n");
            APPL_TRC("\tBD_ADDR      : %02X:%02X:%02X:%02X:%02X:%02X\n",
            bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
            APPL_TRC("\tbd_addr_type : %02X\n", bd_addr_type);
            APPL_TRC("\tReason       : %04X\n", reason);
        }
        else
        {
            APPL_TRC("Received Invalid Event. Event = 0x%02X\n", event_code);
            APPL_TRC("\tBD_ADDR      : %02X:%02X:%02X:%02X:%02X:%02X\n",
            bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
            APPL_TRC("\tbd_addr_type : %02X\n", bd_addr_type);
        }
    }
}

#ifdef L2CAP_SUPPORT_CBFC_MODE
API_RESULT appl_l2ca_connect_ind_cb
           (
               DEVICE_HANDLE            * handle,
               UINT16                     lcid,
               UINT16                     psm,
               L2CAP_CBFC_CONNECT_PARAM * param
           )
{
    API_RESULT retval;
    UINT16 response;

    CONSOLE_OUT (
    "\n -> appl_l2ca_connect_ind_cb from Device %02X, lcid %04X, PSM 0x%04X\n",
    (*handle), lcid, psm);

#ifdef L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT
    CONSOLE_OUT("Remote CID - 0x%04X\n", param->cid);
#endif /* L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT */

    CONSOLE_OUT (
    "\n MTU:0x%04X MPS:0x%04X Credit:0x%04X\n", param->mtu, param->mps, param->credit);

    appl_peer_handle = (*handle);
    appl_le_co_cid = lcid;
    appl_le_co_peer_mtu = param->mtu;

    retval = API_SUCCESS;

#ifdef APPL_LE_L2CAP_CONN_AUTO_ACCEPT
    if (BT_TRUE == appl_l2cap_cbfc_automatic_mode)
    {
        response = 0x0000U;
        retval = l2ca_cbfc_connect_rsp
                 (
                     lcid,
                     response,
                     &connect_param
                 );
    }
#endif /* APPL_LE_L2CAP_CONN_AUTO_ACCEPT */

    return retval;
}

API_RESULT appl_l2ca_connect_cnf_cb
           (
               DEVICE_HANDLE            * handle,
               UINT16                     lcid,
               UINT16                     response,
               L2CAP_CBFC_CONNECT_PARAM * param
           )
{
    CONSOLE_OUT (
     "\n -> appl_l2ca_connect_cnf_cb from Device %02X, lcid %04X, response 0x%04X\n",
    (*handle), lcid, response);

    /* Print MTU, MPS and Credit, on success response */
    if (L2CAP_CONNECTION_SUCCESSFUL == response)
    {
#ifdef L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT
        CONSOLE_OUT("Remote CID - 0x%04X\n", param->cid);
#endif /* L2CAP_CBFC_CONNECT_WITH_CID_CONTEXT */

        CONSOLE_OUT (
        "\n MTU:0x%04X MPS:0x%04X Credit:0x%04X\n", param->mtu, param->mps, param->credit);

        appl_peer_handle = (*handle);
        appl_le_co_cid = lcid;
        appl_le_co_peer_mtu = param->mtu;
    }

    return API_SUCCESS;
}

API_RESULT appl_l2ca_disconnect_ind_cb(UINT16 lcid)
{
    CONSOLE_OUT ("\n -> appl_l2ca_disconnect_ind_cb, lcid %04X\n",lcid);

    /* RESET peer MTU */
    appl_le_co_peer_mtu = 0x0000U;

    /* RESET transfer_char after disconnection */
    transfer_char = 0x00U;

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

    CONSOLE_OUT ("\n -> appl_l2ca_disconnect_cnf_cb lcid %04X\n",lcid);

    /* RESET peer MTU */
    appl_le_co_peer_mtu = 0x0000U;

    /* RESET transfer_char after disconnection */
    transfer_char = 0x00U;

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

    CONSOLE_OUT (
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
    no_of_credits = (datalen + 2U + APPL_LE_CO_MPS - 1U)/APPL_LE_CO_MPS;

    retval = l2ca_cbfc_send_flow_control_credit
             (
                 lcid,
                 no_of_credits
             );

    CONSOLE_OUT (
    "\nSending %d Credits on Data RX, retval 0x%04X\n",
    no_of_credits, retval);
#endif /* APPL_LE_L2CAP_AUTO_SEND_INSTANT_CREDIT */

    return retval;
}

API_RESULT appl_l2ca_low_rx_credit_ind_cb (UINT16 lcid, UINT16 credit)
{
    API_RESULT retval;
    UINT16     no_of_credits;

    CONSOLE_OUT (
    "\n -> appl_l2ca_low_rx_credit_ind_cb, lcid %04X, Credit %04X\n",
    lcid, credit);

    retval = API_SUCCESS;

#ifdef APPL_LE_L2CAP_AUTO_SEND_CREDIT_ON_LWM
    no_of_credits = APPL_LE_CREDIT; /* (APPL_LE_CREDIT - credit) */
    retval = l2ca_cbfc_send_flow_control_credit
             (
                 lcid,
                 no_of_credits
             );

    CONSOLE_OUT (
    "\nSending %d Credits on Low Credit Indication, retval 0x%04X\n",
    no_of_credits, retval);
#endif /* APPL_LE_L2CAP_AUTO_SEND_CREDIT_ON_LWM */

    return retval;
}

API_RESULT appl_l2ca_tx_credit_ind_cb (UINT16 lcid, UINT16 result, UINT16 credit)
{
    CONSOLE_OUT (
    "\n -> appl_l2ca_tx_credit_cb, lcid %04X, Received Credit %04X, Result %04X\n",
    lcid, credit, result);

    /**
     * Disconnect channel if credit overflow has occurred. This is in accordance
     * text in section 10.1 of the Core 5.0 specification
     */
    if (L2CAP_CREDIT_OVERFLOW == result)
    {
        (BT_IGNORE_RETURN_VALUE) l2ca_cbfc_disconnect_req(lcid);
    }

    return API_SUCCESS;
}

API_RESULT appl_l2ca_data_write_cb(UINT16 lcid, UINT16 result, UCHAR *buffer, UINT16 buffer_len)
{
    BT_IGNORE_UNUSED_PARAM(buffer);
    BT_IGNORE_UNUSED_PARAM(buffer_len);

#if 0
    CONSOLE_OUT(
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
    additional_l2cap_hdr_len = 0U;
#else
    additional_l2cap_hdr_len = L2CAP_CREDIT_MODE_SDU_HDR_LEN;
#endif /* L2CAP_ALLOC_FOR_CBFC_SDU */

    for (index = 0U; index < count; index ++)
    {
        appl_data_buffer = BT_alloc_mem (buffer_len + additional_l2cap_hdr_len);

        if (NULL == appl_data_buffer)
        {
            CONSOLE_OUT ("Memory Allocation Failed!\n");
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
                &appl_data_buffer[additional_l2cap_hdr_len],
                transfer_char,
                buffer_len
            );

            CONSOLE_OUT (
            "\n -> Sending data over LE L2CAP CO Channel\n");

            /* Display Information Payload on console */
            appl_dump_bytes(&appl_data_buffer[additional_l2cap_hdr_len], buffer_len);

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
                CONSOLE_OUT("ERROR!!! retval = 0x%04X", retval);
                break;
            }
            else
            {
                CONSOLE_OUT("SUCCESS!!\n");

            #ifdef APPL_LE_L2CAP_DELAY_PKT_SEND
                BT_sleep(1U);
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
    (BT_IGNORE_RETURN_VALUE) BT_l2cap_register_le_event_cb(appl_l2cap_le_callback);
}

#endif /* BT_LE */

