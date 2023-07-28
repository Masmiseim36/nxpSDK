
/**
 *  \file appl_l2cap_ecbfc.c
 *
 *  Source File for L2CAP Enhanced Credit Based Flow Control Mode
 *  Command Line Application.
 */

/*
 *  Copyright (C) 2019. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_l2cap_ecbfc.h"
#include "appl_utils.h"

/* Flag to enable Connection Auto Accept */
#define APPL_L2CAP_ECBFC_CONN_AUTO_ACCEPT

/* Flag to enable sending credits automatically, on reaching Low Water Mark */
#define APPL_L2CAP_ECBFC_AUTO_SEND_CREDIT_ON_LWM

/* Flag to enable sending credits automatically, on receiving a single packet */
#define APPL_L2CAP_ECBFC_AUTO_SEND_INSTANT_CREDIT

/* Flag to enable delay between each packet sent */
#define APPL_L2CAP_ECBFC_DELAY_PKT_SEND

#ifdef L2CAP_SUPPORT_ECBFC_MODE

#define APPL_L2CAP_LE_PSM 0x0041
#define APPL_BUFFER_SIZE  0x0062

#define APPL_LE_CO_MTU 100
#define APPL_LE_CO_MPS 80
#define APPL_LE_CREDIT 10
#define APPL_L2CA_RECV_CREDIT_LWM 1

DECL_STATIC L2CAP_PSM_ECBFC appl_l2cap_ecbfc_psm =
{
    appl_l2ca_ecbfc_connect_ind_cb,
    appl_l2ca_ecbfc_connect_cnf_cb,
    appl_l2ca_ecbfc_reconfig_ind_cb,
    appl_l2ca_ecbfc_reconfig_cnf_cb,
    appl_l2ca_ecbfc_disconnect_ind_cb,
    appl_l2ca_ecbfc_disconnect_cnf_cb,
    appl_l2ca_ecbfc_data_read_cb,
    appl_l2ca_ecbfc_low_rx_credit_ind_cb,
    appl_l2ca_ecbfc_tx_credit_ind_cb,
    APPL_L2CAP_LE_PSM,

#ifdef L2CAP_ECBFC_SUPPORT_UL_CREDIT_LWM
    APPL_L2CA_RECV_CREDIT_LWM,
#endif /* L2CAP_ECBFC_SUPPORT_UL_CREDIT_LWM */

    appl_l2ca_ecbfc_data_write_cb
};

DECL_STATIC L2CAP_ECBFC_CONNECT_PARAM appl_ecbfc_connect_param;
DECL_STATIC L2CAP_ECBFC_RECONFIG_PARAM ecbfc_reconfig_param;

#define APPL_INIT_ECBFC_RECONFIG_STRUCT()\
        BT_mem_set(ecbfc_reconfig_param.cid, 0x00, sizeof(ecbfc_reconfig_param.cid));\
        ecbfc_reconfig_param.mps = appl_ecbfc_connect_param.mps;\
        ecbfc_reconfig_param.mtu = appl_ecbfc_connect_param.mtu;\
        ecbfc_reconfig_param.num_cids = 0x00

DECL_STATIC UCHAR * appl_ecbfc_data_buffer;

DECL_STATIC DEVICE_HANDLE appl_ecbfc_peer_handle;
DECL_STATIC UINT16        appl_ecbfc_le_co_cid[L2CAP_ECBFC_MAX_NUM_CIDS];
DECL_STATIC UCHAR         ecbfc_transfer_char = 0x00;
DECL_STATIC UINT16        appl_ecbfc_le_co_peer_mtu = 0x0000;
DECL_STATIC UINT16        appl_le_num_cids = 0x0000;
DECL_STATIC UINT16        appl_ecbfc_reconfig_cid_list[L2CAP_ECBFC_MAX_NUM_CIDS];
DECL_STATIC UINT16        appl_ecbfc_reconfig_cid_num = 0x0000;

#endif /* L2CAP_SUPPORT_ECBFC_MODE */

#ifdef APPL_L2CAP_ECBFC_CONN_AUTO_ACCEPT
DECL_STATIC UCHAR appl_l2cap_ecbfc_automatic_mode = BT_TRUE;
#endif /* APPL_L2CAP_ECBFC_CONN_AUTO_ACCEPT */

#ifdef BT_LE

/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
DECL_STATIC DECL_CONST CHAR ecbfc_l2cap_options[] = "\n\
============== L2CAP ECBFC Menu ==============\n\
  0. Exit\n\
  1. Refresh\n\
\n\
 10. Register L2CAP ECBFC SPSM\n\
 11. Connect to Remote SPSM\n\
 12. Accept Remote Connection on Local SPSM\n\
 13. Send Data on Channel\n\
 14. Send Transfer Credits\n\
 15. Disconnect Channel\n\
 16. Request Channel(s) Reconfiguration\n\
 17. Respond to Channel(s) Reconfiguration\n\
 18. Unregister L2CAP ECBFC SPSM\n\
\n\
 20. Set Auto Response Mode\n\
\n\
 30. Set Current ECBFC MTU/MPS/Credit Values\n\
 31. Get Current ECBFC MTU/MPS/Credit Values\n\
\n\
 50. Send Bulk Data on Channel\n\
Your Option ?";

/* ----------------------------------------- Functions */

void main_l2cap_ecbfc_operations(void)
{
    unsigned int   read_val, choice;
    UINT32         cid_index;
    BT_DEVICE_ADDR bd_addr;
    DEVICE_HANDLE  device_handle;
    API_RESULT     retval;

#ifdef L2CAP_SUPPORT_ECBFC_MODE
    UINT16 remote_psm, response, result;
    UINT32 send_count;
    UCHAR  link_type;
#endif /* L2CAP_SUPPORT_ECBFC_MODE */

    /* Initialize */
    appl_ecbfc_connect_param.mps    = APPL_LE_CO_MPS;
    appl_ecbfc_connect_param.mtu    = APPL_LE_CO_MTU;
    appl_ecbfc_connect_param.credit = APPL_LE_CREDIT;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT("%s", ecbfc_l2cap_options);
        CONSOLE_IN("%d", &choice);

        switch(choice)
        {
        case 0:
            return;

        case 1:
            break;

#ifdef L2CAP_SUPPORT_ECBFC_MODE
        case 10:
            CONSOLE_OUT("Enter the PSM to register (0x): ");
            CONSOLE_IN("%x", &read_val);

            appl_l2cap_ecbfc_psm.psm = (UINT16)read_val;
            retval = l2cap_ecbfc_register_psm (&appl_l2cap_ecbfc_psm);
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

            CONSOLE_OUT("Enter Link Type (0: BR/EDR, 1: LE): ");
            CONSOLE_IN("%X", &read_val);
            link_type = (UCHAR)read_val;

            if (0 == link_type)
            {
                /* Initialize address-type to Public Address for BREDR Link */
                BT_BD_ADDR_TYPE(&bd_addr) = BT_BD_PUBLIC_ADDRESS_TYPE;
                retval = device_queue_search_br_edr_remote_addr(&device_handle, &bd_addr);
            }
            else
            {
                CONSOLE_OUT("Enter bd_addr_type : ");
                CONSOLE_IN("%X", &read_val);
                BT_BD_ADDR_TYPE(&bd_addr) = (UCHAR) read_val;

                retval = device_queue_search_le_remote_addr(&device_handle, &bd_addr);
            }

            if (API_SUCCESS != retval)
            {
                CONSOLE_OUT("ERROR: Invalid BD_ADDR!!! retval = 0x%04X", retval);
                break;
            }

            CONSOLE_OUT ("Enter Remote PSM (in Hex)\n");
            CONSOLE_IN ("%x",&read_val);

            remote_psm = (UINT16) read_val;

            CONSOLE_OUT("Enter Number of Channels to be created (in range [1,5])\n");
            CONSOLE_IN("%x", &read_val);
            appl_ecbfc_connect_param.num_cids = (UINT8)read_val;

            retval = l2ca_ecbfc_connect_req
                     (
                         &device_handle,
                         remote_psm,
                         appl_l2cap_ecbfc_psm.psm,
                         &appl_ecbfc_connect_param
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

        case 12:
            CONSOLE_OUT ("Enter Response Code (in Hex)\n");
            CONSOLE_IN ("%x",&read_val);
            response = (UINT16)read_val;

            retval = l2ca_ecbfc_connect_rsp
                     (
                         appl_ecbfc_le_co_cid,
                         1,
                         response,
                         &appl_ecbfc_connect_param
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
                CONSOLE_OUT("Enter BD_ADDR : ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(BT_BD_ADDR(&bd_addr));

                CONSOLE_OUT("Enter Link Type (0: BR/EDR, 1: LE): ");
                CONSOLE_IN("%X", &read_val);
                link_type = (UCHAR)read_val;

                if (0 == link_type)
                {
                    /* Initialize address-type to Public Address for BREDR Link */
                    BT_BD_ADDR_TYPE(&bd_addr) = BT_BD_PUBLIC_ADDRESS_TYPE;
                    retval = device_queue_search_br_edr_remote_addr(&device_handle, &bd_addr);
                }
                else
                {
                    CONSOLE_OUT("Enter bd_addr_type : ");
                    CONSOLE_IN("%X", &read_val);
                    BT_BD_ADDR_TYPE(&bd_addr) = (UCHAR)read_val;

                    retval = device_queue_search_le_remote_addr(&device_handle, &bd_addr);
                }

                if (API_SUCCESS != retval)
                {
                    CONSOLE_OUT("ERROR: Invalid BD_ADDR!!! retval = 0x%04X", retval);
                    break;
                }
#if 0
                /**
                 * Removing the CID printing here as in multiple channel
                 * creation scenario the appl_ecbfc_le_co_cid entires are
                 * over-written.
                 * Ideally, appl_ecbfc_le_co_cid needs to be size of L2CAP_MAX_CHANNELS
                 * and there needs to be routine for free space look-up and CID
                 * addition and removal to this list. Also this list might need to have
                 * Device Handle and Link Type reference?
                 * For example, the over-writing of application maintained CID list
                 * can be observed in L2CAP/ECFC/BV-17-C and L2CAP/ECFC/BV-53-C
                 * where multiple channels of ECBFC are being established through
                 * 2 diffferent Req-Rsp commands.
                 */
                CONSOLE_OUT("List of Connected CIDs\n");
                for (cid_index = 0; cid_index < appl_le_num_cids; cid_index++)
                {
                    CONSOLE_OUT("[%02X] 0x%04X\n", (UINT8)cid_index, appl_ecbfc_le_co_cid[cid_index]);
                }
#endif /* 0 */

                CONSOLE_OUT("Enter the cid on which data to be send\n");
                CONSOLE_IN("%x", &choice);
                (BT_IGNORE_RETURN_VALUE)appl_l2cap_ecbfc_send_data
                (
                    &appl_ecbfc_peer_handle,
                    (UINT16)choice,
                    appl_ecbfc_le_co_peer_mtu, /* APPL_BUFFER_SIZE, */
                    1
                );
            }
            break;

        case 14:
            CONSOLE_OUT("Enter the cid\n");
            CONSOLE_IN("%x", &choice);
            retval = l2ca_ecbfc_send_flow_control_credit
                     (
                         (UINT16)choice,
                         APPL_LE_CREDIT
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

        case 15:
            {
                CONSOLE_OUT("List of Connected CIDs\n");
                for (cid_index = 0; cid_index < appl_le_num_cids; cid_index++)
                {
                    CONSOLE_OUT("[%02X] 0x%04X\n", (UINT8)cid_index, appl_ecbfc_le_co_cid[cid_index]);
                }

                CONSOLE_OUT("Enter the cid to disconnect\n");
                CONSOLE_IN("%x", &choice);

                retval = l2ca_ecbfc_disconnect_req((UINT16)choice);
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

        case 16:
            {
                UINT32 i;
                UCHAR  t_config_option;

                CONSOLE_OUT("Enter BD_ADDR : ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(BT_BD_ADDR(&bd_addr));

                CONSOLE_OUT("Enter Link Type (0: BR/EDR, 1: LE): ");
                CONSOLE_IN("%X", &read_val);
                link_type = (UCHAR)read_val;

                if (0 == link_type)
                {
                    /* Initialize address-type to Public Address for BREDR Link */
                    BT_BD_ADDR_TYPE(&bd_addr) = BT_BD_PUBLIC_ADDRESS_TYPE;
                    retval = device_queue_search_br_edr_remote_addr(&device_handle, &bd_addr);
                }
                else
                {
                    CONSOLE_OUT("Enter bd_addr_type : ");
                    CONSOLE_IN("%X", &read_val);
                    BT_BD_ADDR_TYPE(&bd_addr) = (UCHAR) read_val;

                    retval = device_queue_search_le_remote_addr(&device_handle, &bd_addr);
                }

                if (API_SUCCESS != retval)
                {
                    CONSOLE_OUT("ERROR: Invalid BD_ADDR!!! retval = 0x%04X", retval);
                    break;
                }

                /* Set Default values of MTU and MPS */
                APPL_INIT_ECBFC_RECONFIG_STRUCT();

                CONSOLE_OUT("Reconfig what?\n1. MTU\n2. MPS:\n3. Both:\n");
                CONSOLE_IN("%d", &read_val);
                t_config_option = (UCHAR)read_val;

                if (1 == (t_config_option & 0x01))
                {
                    CONSOLE_OUT("Enter new MTU [in Dec]:\n");
                    CONSOLE_IN("%d", &read_val);
                    ecbfc_reconfig_param.mtu = (UINT16)read_val;
                }
                if (2 == (t_config_option & 0x02))
                {
                    CONSOLE_OUT("Enter new MPS [in Dec]:\n");
                    CONSOLE_IN("%d", &read_val);
                    ecbfc_reconfig_param.mps = (UINT16)read_val;
                }

                CONSOLE_OUT("Enter Number of Channels to be Reconfig with new MTU or MPS(in range [1,5])\n");
                CONSOLE_IN("%d", &read_val);
                ecbfc_reconfig_param.num_cids = (UINT8)read_val;

                /* TODO: Check the num_cids is not exceeding 5 */

                appl_dump_bytes((UCHAR *)appl_ecbfc_le_co_cid, sizeof(appl_ecbfc_le_co_cid));

                CONSOLE_OUT("Enter CID's (in Hex 16-bit value)\n");
                for (i = 0; i < ecbfc_reconfig_param.num_cids; i++)
                {
                    CONSOLE_IN("%x", &read_val);
                    ecbfc_reconfig_param.cid[i] = (UINT16)read_val;
                }

                retval = l2ca_ecbfc_reconfig_req
                         (
                             &device_handle,
                             &ecbfc_reconfig_param
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

        case 17:
            {
                CONSOLE_OUT ("Enter Reconfiguration Response Result (in Hex)\n");
                CONSOLE_IN ("%x",&read_val);
                result = (UINT16)read_val;

                retval = l2ca_ecbfc_reconfig_rsp
                         (
                             appl_ecbfc_reconfig_cid_list,
                             appl_ecbfc_reconfig_cid_num,
                             result
                         );

                if (retval != API_SUCCESS)
                {
                    CONSOLE_OUT("ERROR!!! retval = 0x%04X", retval);
                }
                else
                {
                    CONSOLE_OUT("SUCCESS!!\n");

                    /* Clear the global Reconfig CID cache list */
                    BT_mem_set(appl_ecbfc_reconfig_cid_list, 0x00, sizeof(appl_ecbfc_reconfig_cid_list));
                    appl_ecbfc_reconfig_cid_num = 0x0000;
                }
            }
            break;

        case 18:
            {
#ifndef L2CAP_NO_UNREGISTER_LE_PSM
                CONSOLE_OUT("Enter the PSM to Unregister (0x): ");
                CONSOLE_IN("%x", &read_val);

                appl_l2cap_ecbfc_psm.psm = (UINT16)read_val;
                retval = l2cap_ecbfc_unregister_psm (appl_l2cap_ecbfc_psm.psm);

                if(retval != API_SUCCESS)
                {
                    CONSOLE_OUT("ERROR!!! retval = 0x%04X", retval);
                }
                else
                {
                    CONSOLE_OUT("PSM 0x%04X Unregister Successful!!\n", appl_l2cap_ecbfc_psm.psm);
                }
#else /* L2CAP_NO_UNREGISTER_LE_PSM */
                CONSOLE_OUT("Unregister L2CAP ECBFC PSM Feature Not Enabled!\n");
#endif /* L2CAP_NO_UNREGISTER_LE_PSM */
            }
            break;

        case 20:
#ifdef APPL_L2CAP_ECBFC_CONN_AUTO_ACCEPT
            CONSOLE_OUT("Enter mode (0: Non-automatic, 1: Automatic) : ");
            CONSOLE_IN("%d", &read_val);
            appl_l2cap_ecbfc_automatic_mode = (0 == (UCHAR)read_val) ? BT_FALSE : BT_TRUE;
#else /* APPL_L2CAP_ECBFC_CONN_AUTO_ACCEPT */
            CONSOLE_OUT("Compilation flag APPL_L2CAP_ECBFC_CONN_AUTO_ACCEPT Disabled!\n");
#endif /* APPL_L2CAP_ECBFC_CONN_AUTO_ACCEPT */
            break;

        case 30:
            {
                UCHAR t_config_option;

                CONSOLE_OUT(
                "\nCurrent L2CAP ECBFC MTU=%d, MPS=%d, Credits=%d\n",
                appl_ecbfc_connect_param.mtu, appl_ecbfc_connect_param.mps,
                appl_ecbfc_connect_param.credit);

                CONSOLE_OUT("Set New Values for What?\n1. MTU\n2. MPS:\n3. Credits:\n");
                CONSOLE_IN("%d", &read_val);
                t_config_option = (UCHAR)read_val;

                /* Set MTU value */
                if (1 == t_config_option)
                {
                    CONSOLE_OUT("Enter Desired MTU Value to be used in ECBFC req/res [in Dec]: ");
                    CONSOLE_IN("%d", &read_val);
                    if (0 != read_val)
                    {
                        appl_ecbfc_connect_param.mtu = (UINT16)read_val;
                    }
                }
                /* Set MPS value */
                if (2 == t_config_option)
                {
                    CONSOLE_OUT("Enter Desired MPS Value to be used in ECBFC req/res [in Dec]: ");
                    CONSOLE_IN("%d", &read_val);
                    if (0 != read_val)
                    {
                        appl_ecbfc_connect_param.mps = (UINT16)read_val;
                    }
                }
                /* Set Credit value */
                if (3 == t_config_option)
                {
                    CONSOLE_OUT("Enter Desired Credit Value to be used in ECBFC req/res [in Dec]: ");
                    CONSOLE_IN("%d", &read_val);
                    if (0 != read_val)
                    {
                        appl_ecbfc_connect_param.credit = (UINT16)read_val;
                    }
                }

                CONSOLE_OUT(
                "\nUpdated values of L2CAP ECBFC MTU=%d, MPS=%d, Credits=%d\n",
                appl_ecbfc_connect_param.mtu, appl_ecbfc_connect_param.mps,
                appl_ecbfc_connect_param.credit);
            }
            break;

        case 31:
            {
                CONSOLE_OUT(
                "\nCurrent L2CAP ECBFC MTU=%d, MPS=%d, Credits=%d\n",
                appl_ecbfc_connect_param.mtu, appl_ecbfc_connect_param.mps,
                appl_ecbfc_connect_param.credit);
            }
            break;

        case 50:
            {
                CONSOLE_OUT("List of Connected CIDs\n");
                for (cid_index = 0; cid_index < appl_le_num_cids; cid_index++)
                {
                    CONSOLE_OUT("[%02X] 0x%04X\n", (UINT8)cid_index, appl_ecbfc_le_co_cid[cid_index]);
                }

                CONSOLE_OUT("Enter the cid to send data\n");
                CONSOLE_IN("%x", &choice);

                CONSOLE_OUT("Enter number of times LE L2CAP CO packet to be sent\n");
                CONSOLE_IN("%d", &read_val);

                send_count = (UINT32)read_val;

                CONSOLE_OUT("Sending %d times\n", send_count);
                (BT_IGNORE_RETURN_VALUE)appl_l2cap_ecbfc_send_data
                (
                    &appl_ecbfc_peer_handle,
                    (UINT16)choice,
                    appl_ecbfc_le_co_peer_mtu, /* APPL_BUFFER_SIZE, */
                    send_count
                );
            }
            break;

#else /* L2CAP_SUPPORT_ECBFC_MODE */
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 50:
            CONSOLE_OUT("Enable L2CAP_SUPPORT_ECBFC_MODE Feature Flag!");
            break;
#endif /* L2CAP_SUPPORT_ECBFC_MODE */

        default:
            CONSOLE_OUT("Invalid choice. Try agin...\n");
            break;
        }
    }
}


#ifdef L2CAP_SUPPORT_ECBFC_MODE
API_RESULT appl_l2ca_ecbfc_connect_ind_cb
           (
               DEVICE_HANDLE             * handle,
               UINT16                    * lcid,
               UINT16                      psm,
               L2CAP_ECBFC_CONNECT_PARAM * param
           )
{
    API_RESULT retval;
    UINT16 response;
    UINT32 index;

    /* TODO: review */
    if ((NULL == handle) ||
        (NULL == lcid) ||
        (NULL == param))
    {
        /* Should not reach here */
        return API_FAILURE;
    }

    CONSOLE_OUT (
    "\n -> appl_l2ca_ecbfc_connect_ind_cb from Device %02X, PSM 0x%04X, Num CIDs: 0x%02X\n",
    (*handle), psm, param->num_cids);

    for (index = 0; index < param->num_cids; index++)
    {
        CONSOLE_OUT("CID [%02X]: 0x%04X\n", (UINT8)index, lcid[index]);
    }

    CONSOLE_OUT (
    "\n MTU:0x%04X MPS:0x%04X Credit:0x%04X\n", param->mtu, param->mps, param->credit);

    /* BT_mem_set(appl_ecbfc_le_co_cid, 0x00, sizeof(appl_ecbfc_le_co_cid)); */

    appl_ecbfc_peer_handle = (*handle);

    /* TODO: Accpeting all requested channels */
    appl_le_num_cids = param->num_cids;

    /* BT_mem_copy(appl_ecbfc_le_co_cid, lcid, (appl_le_num_cids*2)); */

    BT_mem_copy(appl_ecbfc_le_co_cid, lcid, sizeof(appl_ecbfc_le_co_cid));
    appl_ecbfc_le_co_peer_mtu = param->mtu;

    retval = API_SUCCESS;

#ifdef APPL_L2CAP_ECBFC_CONN_AUTO_ACCEPT
    if (BT_TRUE == appl_l2cap_ecbfc_automatic_mode)
    {
        response = 0x0000;
        retval = l2ca_ecbfc_connect_rsp
                 (
                     lcid,
                     appl_le_num_cids,
                     response,
                     &appl_ecbfc_connect_param
                 );
    }
#endif /* APPL_L2CAP_ECBFC_CONN_AUTO_ACCEPT */

    return retval;
}

API_RESULT appl_l2ca_ecbfc_connect_cnf_cb
           (
               DEVICE_HANDLE             * handle,
               UINT16                    * lcid,
               UINT16                      response,
               L2CAP_ECBFC_CONNECT_PARAM * param
           )
{
    UINT32 index;

    /* TODO: review */
    if ((NULL == handle)||
        (NULL == lcid) ||
        (NULL == param))
    {
        /* Should not reach here */
        return API_FAILURE;
    }

    CONSOLE_OUT(
    "\n -> appl_l2ca_ecbfc_connect_cnf_cb from Device %02X, lcid count %02X, response 0x%04X\n",
    (*handle), param->num_cids, response);

    /* Print MTU, MPS and Credit, on success response */
    if ((L2CAP_CONNECTION_SUCCESSFUL                             == response) ||
        (L2CAP_SOME_CONNECTION_REFUSED_INSUF_RESOURCE            == response) ||
        (L2CAP_CONNECTION_REFUSED_SOME_INVALID_SRC_CID           == response) ||
        (L2CAP_CONNECTION_REFUSED_SOME_SRC_CID_ALREADY_ALLOCATED == response))
    {
        for (index = 0; index < param->num_cids; index++)
        {
            CONSOLE_OUT("Local CID [%02X]: 0x%04X\n", (UCHAR)index, lcid[index]);
        }

        CONSOLE_OUT (
        "\n MTU:0x%04X MPS:0x%04X Credit:0x%04X\n", param->mtu, param->mps, param->credit);

        appl_ecbfc_peer_handle = (*handle);

        appl_le_num_cids = param->num_cids;
        BT_mem_copy(appl_ecbfc_le_co_cid, lcid, sizeof(appl_ecbfc_le_co_cid));
        appl_ecbfc_le_co_peer_mtu = param->mtu;
    }

    return API_SUCCESS;
}

API_RESULT appl_l2ca_ecbfc_reconfig_ind_cb
           (
               DEVICE_HANDLE            * handle,
               UINT16                   * lcid,
               L2CAP_ECBFC_RECONFIG_PARAM * param
           )
{
    API_RESULT retval;
    UINT32 index;
    UINT16 result;

    /* TODO: review */
    if ((NULL == handle) ||
        (NULL == lcid) ||
        (NULL == param))
    {
        /* Should not reach here */
        return API_FAILURE;
    }

    CONSOLE_OUT(
    "\n -> appl_l2ca_ecbfc_reconfig_ind_cb from Device %02X\n",
    (*handle));

    /* Initialize the global Reconfig CID cache list */
    BT_mem_set(appl_ecbfc_reconfig_cid_list, 0x00, sizeof(appl_ecbfc_reconfig_cid_list));
    appl_ecbfc_reconfig_cid_num = 0x0000;

    for (index = 0; index < param->num_cids; index++)
    {
        CONSOLE_OUT("CID [%02X]: 0x%04X\n", (UCHAR)index, lcid[index]);
        appl_ecbfc_reconfig_cid_list[index] = lcid[index];
    }

    CONSOLE_OUT(
    "\n MTU:0x%04X MPS:0x%04X\n", param->mtu, param->mps);

    appl_ecbfc_peer_handle = (*handle);
    BT_mem_copy(appl_ecbfc_le_co_cid, lcid, sizeof(appl_ecbfc_le_co_cid));
    appl_ecbfc_le_co_peer_mtu = param->mtu;
    appl_ecbfc_reconfig_cid_num = param->num_cids;

    retval = API_SUCCESS;

    result = L2CAP_ECBFC_RECONFIG_SUCCESSFUL;

#ifdef APPL_L2CAP_ECBFC_CONN_AUTO_ACCEPT

    if (BT_TRUE == appl_l2cap_ecbfc_automatic_mode)
    {
        retval = l2ca_ecbfc_reconfig_rsp
                 (
                     lcid,
                     appl_ecbfc_reconfig_cid_num,
                     result
                 );

        if (API_SUCCESS == retval)
        {
            /* Clear the global Reconfig CID cache list */
            BT_mem_set(appl_ecbfc_reconfig_cid_list, 0x00, sizeof(appl_ecbfc_reconfig_cid_list));
            appl_ecbfc_reconfig_cid_num = 0x0000;
        }
    }
#endif /* APPL_L2CAP_ECBFC_CONN_AUTO_ACCEPT */

    return retval;
}

API_RESULT appl_l2ca_ecbfc_reconfig_cnf_cb
           (
               DEVICE_HANDLE            * handle
           )
{
    CONSOLE_OUT(
    "\n -> appl_l2ca_ecbfc_reconfig_cnf_cb from Device %02X\n",
    (*handle));

    appl_ecbfc_peer_handle = (*handle);

    return API_SUCCESS;
}

API_RESULT appl_l2ca_ecbfc_disconnect_ind_cb(UINT16 lcid)
{
    UINT32 index;

    CONSOLE_OUT ("\n -> appl_l2ca_ecbfc_disconnect_ind_cb, lcid %04X\n",lcid);

    /* RESET peer MTU */
    /* appl_ecbfc_le_co_peer_mtu = 0x0000; */

    /* RESET ecbfc_transfer_char after disconnection */
    ecbfc_transfer_char = 0x00;

    /* Decrement the Number of connected CIDs */
    /* Check for validation of appl_le_num_cids going < 0 ? */
    if (0 < appl_le_num_cids)
    {
        appl_le_num_cids -= 1;

        if (0 == appl_le_num_cids)
        {
            BT_mem_set(appl_ecbfc_le_co_cid, 0x0, sizeof(appl_ecbfc_le_co_cid));
        }
    }

    /* Search for the Disconnected CID and Reset in the global */
    for (index = 0; index < L2CAP_ECBFC_MAX_NUM_CIDS; index++)
    {
        if (appl_ecbfc_le_co_cid[index] == lcid)
        {
            /* RESET */
            appl_ecbfc_le_co_cid[index] = 0x0000;
        }
    }

#ifndef L2CAP_ALLOC_FOR_CBFC_SDU
    if (NULL != appl_ecbfc_data_buffer)
    {
        /* Free buffer. L2CAP has allocated and copied the data */
        BT_free_mem (appl_ecbfc_data_buffer);

        /* Resetting global data pointer after free */
        appl_ecbfc_data_buffer = NULL;
    }
#endif /* L2CAP_ALLOC_FOR_CBFC_SDU */

    return API_SUCCESS;
}

API_RESULT appl_l2ca_ecbfc_disconnect_cnf_cb(UINT16 lcid, UINT16 reason)
{
    UINT32 index;

    BT_IGNORE_UNUSED_PARAM(reason);

    CONSOLE_OUT ("\n -> appl_l2ca_ecbfc_disconnect_cnf_cb lcid %04X\n",lcid);

    /* RESET peer MTU */
    /* appl_ecbfc_le_co_peer_mtu = 0x0000; */

    /* RESET ecbfc_transfer_char after disconnection */
    ecbfc_transfer_char = 0x00;

    /* Decrement the Number of connected CIDs */
    /* Check for validation of appl_le_num_cids going < 0 ? */
    if (0 < appl_le_num_cids)
    {
        appl_le_num_cids -= 1;

        if (0 == appl_le_num_cids)
        {
            BT_mem_set(appl_ecbfc_le_co_cid, 0x0, sizeof(appl_ecbfc_le_co_cid));
        }
    }

    /* Search for the Disconnected CID and Reset in the global */
    for (index = 0; index < L2CAP_ECBFC_MAX_NUM_CIDS; index++)
    {
        if (appl_ecbfc_le_co_cid[index] == lcid)
        {
            /* RESET */
            appl_ecbfc_le_co_cid[index] = 0x0000;
        }
    }

#ifndef L2CAP_ALLOC_FOR_CBFC_SDU
    if (NULL != appl_ecbfc_data_buffer)
    {
        /* Free buffer. L2CAP has allocated and copied the data */
        BT_free_mem (appl_ecbfc_data_buffer);

        /* Resetting global data pointer after free */
        appl_ecbfc_data_buffer = NULL;
    }
#endif /* L2CAP_ALLOC_FOR_CBFC_SDU */

    return API_SUCCESS;
}

API_RESULT appl_l2ca_ecbfc_data_read_cb (UINT16 lcid, UINT16 result, UCHAR * data, UINT16 datalen)
{
    API_RESULT retval;
    UINT16 no_of_credits;

    CONSOLE_OUT (
    "\n -> appl_l2ca_ecbfc_data_read_cb, lcid %04X, data len %04X, result 0x%04X\n",
    lcid, datalen, result);

    appl_dump_bytes(data, datalen);

    retval = API_SUCCESS;

#ifdef APPL_L2CAP_ECBFC_AUTO_SEND_INSTANT_CREDIT
    /**
     * Calculate possible number of LE L2CAP Frames.
     *
     * Maximum size of each frame shall not exceed local MPS.
     * Account for the 2 octet of SDU length.
     */
    no_of_credits = (datalen + 2 + APPL_LE_CO_MPS - 1)/APPL_LE_CO_MPS;

    retval = l2ca_ecbfc_send_flow_control_credit
             (
                 lcid,
                 no_of_credits
             );
#endif /* APPL_L2CAP_ECBFC_AUTO_SEND_INSTANT_CREDIT */

    return retval;
}

API_RESULT appl_l2ca_ecbfc_low_rx_credit_ind_cb (UINT16 lcid, UINT16 credit)
{
    API_RESULT retval;

    CONSOLE_OUT (
    "\n -> appl_l2ca_ecbfc_low_rx_credit_ind_cb, lcid %04X, Credit %04X\n",
    lcid,credit);

    retval = API_SUCCESS;

#ifdef APPL_L2CAP_ECBFC_AUTO_SEND_CREDIT_ON_LWM
    retval = l2ca_ecbfc_send_flow_control_credit
             (
                 lcid,
                 APPL_LE_CREDIT
             );
#endif /* APPL_L2CAP_ECBFC_AUTO_SEND_CREDIT_ON_LWM */

    return retval;
}

API_RESULT appl_l2ca_ecbfc_tx_credit_ind_cb (UINT16 lcid, UINT16 result, UINT16 credit)
{
    CONSOLE_OUT (
    "\n -> appl_l2ca_ecbfc_tx_credit_ind_cb, lcid %04X, Credit %04X, Result %04X\n",
    lcid, credit, result);

    /**
     * Disconnect channel if credit overflow has occured. This is in accordance
     * text in section 10.1 of the Core 5.0 specification
     */
    if (L2CAP_CREDIT_OVERFLOW == result)
    {
        (BT_IGNORE_RETURN_VALUE)l2ca_ecbfc_disconnect_req(lcid);
    }

    return API_SUCCESS;
}

API_RESULT appl_l2ca_ecbfc_data_write_cb(UINT16 lcid, UINT16 result, UCHAR *buffer, UINT16 buffer_len)
{
    BT_IGNORE_UNUSED_PARAM(buffer);
    BT_IGNORE_UNUSED_PARAM(buffer_len);
    BT_IGNORE_UNUSED_PARAM(lcid);
    BT_IGNORE_UNUSED_PARAM(result);

    CONSOLE_OUT(
    "\n -> appl_l2ca_ecbfc_data_write_cb, lcid %04X, Result %04X\n",
    lcid, result);

#ifndef L2CAP_ALLOC_FOR_CBFC_SDU
    if (NULL != appl_ecbfc_data_buffer)
    {
        /* Free buffer. L2CAP has allocated and copied the data */
        BT_free_mem (appl_ecbfc_data_buffer);

        /* Resetting global data pointer after free */
        appl_ecbfc_data_buffer = NULL;
    }
#endif /* L2CAP_ALLOC_FOR_CBFC_SDU */

    return API_SUCCESS;
}

API_RESULT appl_l2cap_ecbfc_send_data
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
#ifdef L2CAP_ALLOC_FOR_ECBFC_SDU
    additional_l2cap_hdr_len = 0;
#else
    additional_l2cap_hdr_len = L2CAP_CREDIT_MODE_SDU_HDR_LEN;
#endif /* L2CAP_ALLOC_FOR_ECBFC_SDU */

    /**
     * TODO: Remove loop based data send and have Write Cnf CB based Data send
     */
    for (index = 0; index < count; index ++)
    {
        appl_ecbfc_data_buffer = BT_alloc_mem (buffer_len + additional_l2cap_hdr_len);

        if (NULL == appl_ecbfc_data_buffer)
        {
            CONSOLE_OUT ("Memory Allocation Failed!\n");
        }
        else
        {
#ifndef L2CAP_ALLOC_FOR_ECBFC_SDU
            /* If L2CAP_ALLOC_FOR_CBFC_SDU is not defined, add SDU Header Length */
            BT_PACK_LE_2_BYTE_VAL (appl_ecbfc_data_buffer, buffer_len);
#endif /* L2CAP_ALLOC_FOR_ECBFC_SDU */

            ecbfc_transfer_char++;

            /* Fill with pattern */
            BT_mem_set
            (
                appl_ecbfc_data_buffer+additional_l2cap_hdr_len,
                ecbfc_transfer_char,
                buffer_len
            );

            CONSOLE_OUT (
            "\n -> Sending data over LE L2CAP CO Channel\n");

            /* Display Information Payload on console */
            appl_dump_bytes(appl_ecbfc_data_buffer+additional_l2cap_hdr_len, buffer_len);

#ifndef L2CAP_ALLOC_FOR_ECBFC_SDU
            /*
             * If L2CAP_ALLOC_FOR_CBFC_SDU is not defined,
             * include length of 'SDU Header Length' field (2 Octets)
             * in the buffer length.
             */
            buffer_len += L2CAP_CREDIT_MODE_SDU_HDR_LEN;
#endif /* L2CAP_ALLOC_FOR_ECBFC_SDU */

            retval = l2ca_ecbfc_data_write
                     (
                          device_handle,
                          cid,
                          appl_ecbfc_data_buffer,
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

            #ifdef APPL_L2CAP_ECBFC_DELAY_PKT_SEND
                BT_sleep(1);
            #endif /* APPL_L2CAP_ECBFC_DELAY_PKT_SEND */
            }
    #ifdef L2CAP_ALLOC_FOR_ECBFC_SDU
            /* Free buffer. L2CAP has allocated and copied the data */
            BT_free_mem (appl_ecbfc_data_buffer);
    #endif /* L2CAP_ALLOC_FOR_ECBFC_SDU */
        }
    }

    return retval;
}

#endif /* L2CAP_SUPPORT_ECBFC_MODE */

#endif /* BT_LE */
