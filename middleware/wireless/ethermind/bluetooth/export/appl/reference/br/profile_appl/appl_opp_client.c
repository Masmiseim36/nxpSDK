
/**
 *  \file appl_opp_client.c
 *
 *  Source file for OPP_CLIENT Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_opp.h"
#include "appl_utils.h"

#ifdef OPP
/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static UCHAR                  oppc_bd_addr[BT_BD_ADDR_SIZE];

static OPP_INSTANCE           opp_client_instance[OPP_NUM_CLIENT_INSTANCE];

static UCHAR                  opp_attrib_data[OPP_SDP_RECORD_DATA_SIZE];
static UINT16                 opp_attrib_data_len = OPP_SDP_RECORD_DATA_SIZE;
static SDP_HANDLE             opp_sdp_handle;
static UCHAR                  opp_server_channel;
static UINT16                 opp_l2cap_psm;

static OPP_CONNECT_STRUCT     connect_info;

static OPP_REQUEST_STRUCT     opp_req_info;
static OPP_HEADER_STRUCT      name_info, body_info, type_info;

static UCHAR                  file_name[32U], file_object[128U];
static UCHAR                  object_type[32U];
static UCHAR                  object_name[OPP_OBJECT_NAME_LEN];

static UINT16                 file_name_len;
static UINT16                 object_type_len;

static BT_fops_file_handle    opp_rx_fp;
static BT_fops_file_handle    fp;
static UINT32                 fsize;
static UINT32                 remaining, sent;
static UINT32                 opp_xchg_size;

static UCHAR wait_count;

static const UCHAR opp_client_menu[] =
" \n \
-------------- OPP Client Menu --------------\n \
\n \
\t 0. Exit \n \
\t 1. Refresh \n \
\n \
\t 2. OPP Client Initialize \n \
\t 3. OPP Client Shutdown \n \
\t 4. OPP Client Start \n \
\t 5. OPP Client Stop \n \
\n \
\t10. Create ACL Connection\n\
\t11. ACL Disconnection\n\
\t12. Do SDP Query \n\
\n \
\t15. Connect to OPP Server \n \
\t16. Disconnect from OPP Server \n \
\t17. Close Transport Connection with OPP Server \n \
\n \
\t20. Object Push \n \
\t21. Bussiness Card Pull \n \
\t22. Bussiness Card Exchange \n \
\t23. Abort \n\
\n \
Your Choice: ";

/* ----------------------------------------- Functions */

void main_opp_client_operations (void)
{
    API_RESULT retval;
    UCHAR i;
    UINT8 *phandle;
    UINT16 size;
    UINT16 hci_handle;

    UINT16 actual;
    UCHAR more;

    int choice, menu_choice, handle, server_ch, val;

    retval = API_SUCCESS;
    more = 0U;

    BT_LOOP_FOREVER()
    {
        printf ("%s", opp_client_menu);
        scanf ("%d", &choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            LOG_DEBUG ("Initializing OPP Client...\n");
            retval = BT_opp_client_init ();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                /* Reset BD_ADDR & OPP handle */
                for (i = 0U; i < OPP_NUM_CLIENT_INSTANCE; i++)
                {
                    OPP_RESET_INSTANCE(&opp_client_instance[i]);
                }
            }

            opp_client_print_appl_instances ();
            break;

        case 3:
            LOG_DEBUG ("Shutting down OPP Client...\n");
            retval = BT_opp_client_shutdown ();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 4:
            opp_client_print_appl_instances ();

            LOG_DEBUG ("Enter OPP Client instance: ");
            scanf ("%d", &handle);

            /* OPP applicaion instance handle */
            phandle = &opp_client_instance[handle].handle;

            if (OPP_ENTITY_INVALID != *phandle)
            {
                LOG_DEBUG ("Application Instance not free!\n");
                break;
            }

            LOG_DEBUG ("Starting OPP Client...\n");
            retval = BT_opp_client_start
                     (
                         phandle,
                         appl_opp_client_callback
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS != retval)
            {
                *phandle = OPP_ENTITY_INVALID;
            }

            opp_client_print_appl_instances ();
            break;

        case 5:
            opp_client_print_appl_instances ();

            LOG_DEBUG ("Enter OPP Client instance: ");
            scanf ("%d", &handle);

            /* OPP applicaion instance handle */
            phandle = &opp_client_instance[handle].handle;

            LOG_DEBUG ("Stopping  OPP Client...\n");
            retval = BT_opp_client_stop (phandle);
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                *phandle = OPP_ENTITY_INVALID;

                BT_mem_set
                (
                    &opp_client_instance[handle].bd_addr,
                    0x00,
                    BT_BD_ADDR_SIZE
                );

                /* Close the files if opened */
                if (NULL != fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (fp);
                    fp = NULL;
                }

                if (NULL != opp_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (opp_rx_fp);
                    opp_rx_fp = NULL;
                }
            }

            opp_client_print_appl_instances ();
            break;

        case 10:
            /**
             * First Application should establish ACL Connection with
             * the peer Device
             */
            LOG_DEBUG("Enter OPP Server's BD Address: ");

            /* Read the BD_ADDR of Remote Device */
            appl_get_bd_addr(oppc_bd_addr);

            retval = BT_hci_create_connection
                     (
                         oppc_bd_addr,
                         LMP_ACL_DH5,
                         0x0U, 0x0U, 0x0U, 0x1U
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("HCI Connect Attempt FAILED !! Error Code = 0x%04X\n",
                        retval);
            }
            break;

        case 11:
            retval = BT_hci_get_acl_connection_handle
                     (
                         oppc_bd_addr,
                         &hci_handle
                     );

            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("Disconnecting ACL Connection 0x%04X\n",
                hci_handle);
            }
            else
            {
                LOG_DEBUG ("ACL Connection for OPP Client not found\n");
                LOG_DEBUG("Enter ACL Handle\n");
                scanf("%x",&choice);
                hci_handle = (UINT16)choice;
            }

            retval = BT_hci_disconnect
                     (
                         hci_handle,
                         0x13U
                     );

            LOG_DEBUG("> API RETVAL ACL disconnect : 0x%04X\n", retval);
            break;

        case 12:
            SDP_SET_HANDLE
            (
                opp_sdp_handle,
                oppc_bd_addr,
                appl_opp_ce_sdp_callback
            );

            retval = BT_sdp_open(&opp_sdp_handle);

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("SDP Connect FAILED !! Error Code = 0x%04X\n", retval);
            }

            LOG_DEBUG("Please Wait for SDP Operation to Complete\n");
            break;

        case 15:
            opp_client_print_appl_instances();

            LOG_DEBUG ("Enter OPP Client instance: ");
            scanf ("%d", &handle);

            connect_info.bd_addr = oppc_bd_addr;

#ifdef OBEX_OVER_L2CAP
            LOG_DEBUG ("Enter peer L2CAP PSM (in Hex): ");
            scanf ("%x", &val);
            connect_info.psm = (UINT16)val;
#endif /* OBEX_OVER_L2CAP */

            LOG_DEBUG ("Enter peer RFCOMM Server Channel (in Hex): ");
            scanf ("%x", &server_ch);
            connect_info.server_channel = (UINT8)server_ch;

            LOG_DEBUG ("Enter Data Exchange Size: ");
            scanf ("%d", &val);
            size = (UINT16) val;
            connect_info.max_rx_size = size;

            LOG_DEBUG ("Connecting...\n");
            retval = BT_opp_client_connect
                     (
                         &opp_client_instance[handle].handle,
                         &connect_info
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 16:
            LOG_DEBUG ("Enter OPP Client instance: ");
            scanf ("%d", &handle);

            LOG_DEBUG ("Disconnecting OPP Client Instance %d\n", handle);
            retval = BT_opp_client_disconnect (&opp_client_instance[handle].handle);
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 17:
            LOG_DEBUG ("Enter OPP Client instance: ");
            scanf ("%d", &handle);

            LOG_DEBUG ("Disconnecting Transport with OPP Server\n");
            retval = BT_opp_client_transport_close (&opp_client_instance[handle].handle);
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 20:
            opp_client_print_appl_instances ();

            LOG_DEBUG ("Enter OPP Client instance: ");
            scanf ("%d", &handle);

            OPP_INIT_HEADER_STRUCT (type_info);
            OPP_INIT_HEADER_STRUCT (name_info);
            OPP_INIT_HEADER_STRUCT (body_info);

            LOG_DEBUG ("Specify Type Header? (1/0): ");
            scanf ("%d", &choice);
            if (0 != choice)
            {
                LOG_DEBUG("Enter the Object Type:\n");
                LOG_DEBUG("    1-> text/x-vcard\n");
                LOG_DEBUG("    2-> text/x-vcalender\n");
                LOG_DEBUG("    3-> text/x-vmsg\n");
                scanf("%d", &val);
                if (1 == val)
                {
                    BT_str_n_copy(object_type, OPP_TYPE_HDR_VCARD, sizeof(OPP_TYPE_HDR_VCARD));
                }
                else if (2 == val)
                {
                    BT_str_n_copy(object_type, OPP_TYPE_HDR_VCALENDER, sizeof(OPP_TYPE_HDR_VCALENDER));
                }
                else if (3 == val)
                {
                    BT_str_n_copy(object_type, OPP_TYPE_HDR_VMSG, sizeof(OPP_TYPE_HDR_VMSG));
                }
                else
                {
                    LOG_DEBUG("Invalid Choice, Try Again\n");
                    break;
                }

                /* Fill type hdr values */
                type_info.value = object_type;
                object_type_len = (UINT16)BT_str_n_len(object_type, sizeof(object_type));
                type_info.length = (UINT16)(object_type_len + 1U);
            }

            LOG_DEBUG ("Enter the object name to be sent: ");
            scanf ("%s", file_name);
            file_name_len = (UINT16)BT_str_n_len(file_name, sizeof(file_name));

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)OPP_ROOT_FOLDER_BASE,
                file_name,
                file_object
            );

            /* Fill name hdr values */
            name_info.value = file_name;
            name_info.length = (UINT16)(file_name_len + 1U);

            fsize = 0U;
            remaining = 0U;
            sent = 0U;
            opp_xchg_size = opp_client_instance[handle].max_xchg_size;

            /* Open the file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            if ((API_SUCCESS == retval) && (NULL != fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);

                remaining = fsize;
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");
                break;
            }

            /* Fill body hdr values */
            if (remaining > opp_xchg_size)
            {
                body_info.length = (UINT16)opp_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16)remaining;
                more = 0x00U;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            if (0U != body_info.length)
            {
                body_info.value = BT_alloc_mem (body_info.length);
            }

            if (NULL != body_info.value)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                    (body_info.value, body_info.length, fp, &actual);
            }

            /* Update the hdrs  for put request */
            opp_req_info.obj_len_info = remaining; /* Object Size */
            opp_req_info.name = &name_info;
            opp_req_info.body = &body_info;
            if (NULL != type_info.value)
            {
                opp_req_info.type = &type_info;
            }

            /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
            actual = 0U;

            LOG_DEBUG ("Requesting to Push object...\n");
            retval = BT_opp_client_push_object
                     (
                         &opp_client_instance[handle].handle,
                         &opp_req_info,
                         more,
                         &actual
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            /* Update object size sent & remaining to send */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_seek(fp, actual, SEEK_SET);
            sent += actual;
            remaining = fsize - sent;

            /* If operation has failed or completed, perform cleanup */
            if ((API_SUCCESS != retval) || (0U == remaining))
            {
                /* Reset the variables */
                sent = 0U;
                fsize = 0U;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }

            if (NULL != body_info.value)
            {
                BT_free_mem (body_info.value);
                body_info.value = NULL;
            }
            break;

        case 21:
            opp_client_print_appl_instances ();

            LOG_DEBUG ("Enter OPP Client instance: ");
            scanf ("%d", &handle);


            OPP_INIT_HEADER_STRUCT (type_info);
            BT_mem_set(&opp_req_info, 0, sizeof(OPP_REQUEST_STRUCT));

            LOG_DEBUG("Enter number of wait stages: ");
            scanf("%d", &choice);
            wait_count = (UCHAR)choice;

            if (0U < wait_count)
            {
                opp_req_info.wait = BT_TRUE;
                wait_count--;
            }
            else
            {
                opp_req_info.wait = BT_FALSE;
            }

            opp_xchg_size = opp_client_instance[handle].max_xchg_size;

            LOG_DEBUG ("Requesting to Pull Business Card...\n");
            retval = BT_opp_client_pull_business_card
                     (
                         &opp_client_instance[handle].handle,
                         &opp_req_info
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            break;

        case 22:
            opp_client_print_appl_instances ();

            LOG_DEBUG ("Enter OPP Client instance: ");
            scanf ("%d", &handle);

            BT_mem_set(&opp_req_info, 0, sizeof(OPP_REQUEST_STRUCT));
            OPP_INIT_HEADER_STRUCT (type_info);
            OPP_INIT_HEADER_STRUCT (name_info);
            OPP_INIT_HEADER_STRUCT (body_info);

            LOG_DEBUG ("Enter the object name to be sent: ");
            scanf ("%s", file_name);
            file_name_len = (UINT16)BT_str_n_len(file_name, sizeof(file_name));

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)OPP_ROOT_FOLDER_BASE,
                file_name,
                file_object
            );

            /* Fill name hdr values */
            name_info.value = file_name;
            name_info.length = (UINT16)(file_name_len + 1U);

            fsize = 0U;
            remaining = 0U;
            sent = 0U;
            opp_xchg_size = opp_client_instance[handle].max_xchg_size;

            /* Open the file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            if ((API_SUCCESS == retval) && (NULL != fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);

                remaining = fsize;
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");
                break;
            }

            /* Fill body hdr values */
            if (remaining > opp_xchg_size)
            {
                body_info.length = (UINT16)opp_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16)remaining;
                more = 0x00U;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            if (0U != body_info.length)
            {
                body_info.value = BT_alloc_mem (body_info.length);
            }

            if (NULL != body_info.value)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                (
                    body_info.value,
                    body_info.length,
                    fp,
                    &actual
                );
            }

            /* Update the hdrs  for put request */
            opp_req_info.obj_len_info = remaining;
            opp_req_info.name = &name_info;
            opp_req_info.type = NULL;
            opp_req_info.body = &body_info;

            /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
            actual = 0U;

            LOG_DEBUG ("Requesting to Push object...\n");
            retval = BT_opp_client_exchange_business_card
                     (
                         &opp_client_instance[handle].handle,
                         &opp_req_info,
                         more,
                         &actual
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            /* Update object size sent & remaining to send */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_seek(fp, actual, SEEK_SET);
            sent += actual;
            remaining = fsize - sent;

            /* If operation has failed or completed, perform cleanup */
            if ((API_SUCCESS != retval) || (0U == remaining))
            {
                /* Reset the variables */
                sent = 0U;
                fsize = 0U;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }

            if (NULL != body_info.value)
            {
                BT_free_mem (body_info.value);
                body_info.value = NULL;
            }

            break;

        case 23:
            opp_client_print_appl_instances ();

            LOG_DEBUG ("Enter OPP Client instance to Abort: ");
            scanf ("%d", &handle);

            LOG_DEBUG ("aborting the push operation.. \n");
            retval = BT_opp_client_abort(&opp_client_instance[handle].handle);
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        default:
            LOG_DEBUG ("Invalid Choice.\n");
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

API_RESULT appl_opp_client_callback
           (
               OPP_HANDLE      * opp_handle,
               UINT8             event_type,
               UINT16            event_result,
               OPP_HEADERS     * event_header,
               UINT16            event_hdrlen
           )
{
    API_RESULT retval;
    OPP_HANDLE handle;
    OPP_HEADERS *opp_rx_hdrs;
    UINT16 i;

    UCHAR send_request;

    OPP_HEADERS opp_tx_hdrs;
    OPP_REQUEST_STRUCT req_info;
    UCHAR more;
    UINT16 actual;

    BT_IGNORE_UNUSED_PARAM(event_hdrlen);

    /* Initialize to default */
    handle = *opp_handle;
    retval = API_SUCCESS;
    opp_rx_hdrs = event_header;

    send_request = 0x00U;
    more = 0x00U;
    BT_mem_set(&opp_tx_hdrs, 0, sizeof(OPP_HEADERS));
    BT_mem_set(&req_info, 0, sizeof(OPP_REQUEST_STRUCT));
    OPP_INIT_HEADER_STRUCT (body_info);

    switch(event_type)
    {
    case OPP_CLIENT_TRANSPORT_CLOSE_IND: /* Fall Through */
    case OPP_CLIENT_TRANSPORT_CLOSE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received %s\n", (OPP_CLIENT_TRANSPORT_CLOSE_IND == event_type) ?
        "OPP_CLIENT_TRANSPORT_CLOSE_IND" : "OPP_CLIENT_TRANSPORT_CLOSE_CNF");
        LOG_DEBUG("OPP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        /* Reset bd_addr */
        for (i = 0U; i < OPP_NUM_CLIENT_INSTANCE; i++)
        {
            if (opp_client_instance[i].handle == handle)
            {
                BT_mem_set (opp_client_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);
            }
        }

        opp_client_print_appl_instances ();
        break;

    case OPP_CLIENT_CONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received OPP_CLIENT_CONNECT_CNF\n");
        LOG_DEBUG("OPP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (opp_rx_hdrs->opp_connect_info->bd_addr));

        LOG_DEBUG ("MAX Tx. Size: %d\n",
        opp_rx_hdrs->opp_connect_info->max_tx_size);

        /* Get the Handle parameters */
        for (i = 0U; i < OPP_NUM_CLIENT_INSTANCE; i++)
        {
            if (opp_client_instance[i].handle == handle)
            {
                BT_mem_copy
                (
                    opp_client_instance[i].bd_addr,
                    opp_rx_hdrs->opp_connect_info->bd_addr,
                    BT_BD_ADDR_SIZE
                );

                opp_client_instance[i].max_xchg_size =
                    opp_rx_hdrs->opp_connect_info->max_tx_size;

                break;
            }
        }

        opp_client_print_appl_instances ();
        break;

    case OPP_CLIENT_DISCONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received OPP_CLIENT_DISCONNECT_CNF\n");
        LOG_DEBUG("OPP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        opp_client_print_appl_instances ();
        break;

    case OPP_CLIENT_PUSH_OBJECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received OPP_CLIENT_PUSH_OBJECT_CNF\n");
        LOG_DEBUG("OPP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((OPP_SUCCESS_RSP != event_result) &&
            (OPP_CONTINUE_RSP != event_result))
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fp);
            fp = NULL;
            break;
        }

        if (OPP_SUCCESS_RSP == event_result)
        {
            break;
        }

        /* Fill body hdr values */
        if (remaining > opp_xchg_size)
        {
            body_info.length = (UINT16)opp_xchg_size;
            more = 0x01U;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16)remaining;
            more = 0x00U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_info.length)
        {
            body_info.value = BT_alloc_mem (body_info.length);
        }

        /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
        actual = 0U;

        if (NULL != body_info.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
            (
                body_info.value,
                body_info.length,
                fp,
                &actual
            );
        }

        req_info.body = &body_info;

        opp_tx_hdrs.opp_req_info = &req_info;

        send_request = 0x01U;
        break;

    case OPP_CLIENT_EXCH_BUSINESS_CARD_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received OPP_CLIENT_EXCH_BUSINESS_CARD_CNF\n");
        LOG_DEBUG("OPP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((OPP_SUCCESS_RSP != event_result) &&
            (OPP_CONTINUE_RSP != event_result))
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fp);
            fp = NULL;
            break;
        }

        if (OPP_SUCCESS_RSP == event_result)
        {
            /**
             * Request for Business Card Pull After Pushing
             *  Business Card from OPP Client
             */
            opp_req_info.body = NULL;
            opp_req_info.type = &type_info;

            LOG_DEBUG ("Requesting to Pull Business Card...\n");
            retval = BT_opp_client_pull_business_card
                     (
                         &opp_client_instance[handle].handle,
                         &opp_req_info
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            break;
        }

        /* Fill body hdr values */
        if (remaining > opp_xchg_size)
        {
            body_info.length = (UINT16)opp_xchg_size;
            more = 0x01U;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16)remaining;
            more = 0x00U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_info.length)
        {
            body_info.value = BT_alloc_mem (body_info.length);
        }

        /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
        actual = 0U;

        if (NULL != body_info.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
            (
                body_info.value,
                body_info.length,
                fp,
                &actual
            );
        }

        req_info.body = &body_info;

        opp_tx_hdrs.opp_req_info = &req_info;

        send_request = 0x01U;
        break;

    case OPP_CLIENT_PULL_BUSINESS_CARD_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received OPP_CLIENT_PULL_BUSINESS_CARD_CNF\n");
        LOG_DEBUG("OPP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (OPP_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }

        if ((NULL == opp_rx_hdrs->opp_rsp_info->body) ||
            (NULL == opp_rx_hdrs->opp_rsp_info->body->value) ||
            (0U == opp_rx_hdrs->opp_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            UINT16 length;
            UCHAR *data;

            /* Open file for dumping received Object Data */
            if (NULL == opp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)OPP_ROOT_RX_FOLDER_BASE,
                    (UCHAR *)object_name,
                    file_object
                );

                LOG_DEBUG("Created file object for pull request -> %s\n", file_object);

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &opp_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == opp_rx_fp))
                {
                    LOG_DEBUG ("Failed to open Received Business Card file\n");
                }
            }

            data = opp_rx_hdrs->opp_rsp_info->body->value;
            length = opp_rx_hdrs->opp_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG("\n ====== OPP Pull Object data ===== \n");
            for (i = 0U; i<length; i++)
            {
                LOG_DEBUG("%c", data[i]);
            }
            LOG_DEBUG("\n ================================= \n");

            /* Write to file */
            if (NULL != opp_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, opp_rx_fp, &actual);
            }

            /* Close the file if final chunk is received */
            if (OPP_SUCCESS_RSP == event_result)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (opp_rx_fp);
                opp_rx_fp = NULL;
            }
        }
        break;

    case OPP_CLIENT_ABORT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received OPP_CLIENT_ABORT_CNF\n");
        LOG_DEBUG("OPP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (NULL != opp_rx_fp)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(opp_rx_fp);
            opp_rx_fp = NULL;
        }

        if (NULL != fp)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fp);
            fp = NULL;
        }
        break;

    default:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received OPP_EVENT_INVALID\n");
        break;
    }

    if (0U != send_request)
    {
        if (0U < wait_count)
        {
            req_info.wait = BT_TRUE;
            opp_tx_hdrs.opp_req_info = &req_info;

            wait_count--;
        }

        retval = BT_opp_client_send_request
                 (
                     &opp_client_instance[handle].handle,
                     event_type,
                     event_result,
                     &opp_tx_hdrs,
                     more,
                     &actual
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG ("OPP Client Send Request Failed - 0x%04X\n", retval);
        }

        if ((OPP_CLIENT_PUSH_OBJECT_CNF == event_type) ||
            (OPP_CLIENT_EXCH_BUSINESS_CARD_CNF == event_type))
        {
            /* Free memory */
            if (NULL != body_info.value)
            {
                BT_free_mem (body_info.value);
                body_info.value = NULL;
            }

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            /* If operation has failed or completed, perform cleanup */
            if ((API_SUCCESS != retval) || (0U == remaining))
            {
                /* Reset the variables */
                sent = 0U;
                fsize = 0U;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }
        }
    }

    return retval;
}


void appl_opp_ce_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     )
{
    API_RESULT retval;
    UINT16 num_attrib_ids;

    S_UUID uuid[3U];

#ifndef OBEX_OVER_L2CAP
    UINT16 attrib_id[1U] = {PROTOCOL_DESC_LIST};

    num_attrib_ids = 1U;
#else /* OBEX_OVER_L2CAP */
    UINT16 attrib_id[2U] = {PROTOCOL_DESC_LIST,
                           GOEP_L2CAP_PSM_ATTR_ID
                           };

    num_attrib_ids = 2U;
#endif /* OBEX_OVER_L2CAP */

    BT_IGNORE_UNUSED_PARAM(length);

    /*
     * Frame the SDP Search pattern with following UUIDs
     * - OPP_UUID
     * - RFCOMM_UUID
     * - L2CAP_UUID
     * and Attribute ID "PROTOCOL_DESC_LIST"
     * (For getting the RFCOMM Server Channel on which Peer OPP Service
     *  is Listening)
     *
     */
    uuid[0U].uuid_type = UUID_16;
    uuid[1U].uuid_type = UUID_16;
    uuid[2U].uuid_type = UUID_16;
    uuid[0U].uuid_union.uuid_16 = OBEXOBJECT_PUSH_UUID;
    uuid[1U].uuid_union.uuid_16 = L2CAP_UUID;
    uuid[2U].uuid_union.uuid_16 = RFCOMM_UUID;

    switch(command)
    {
    case SDP_Open:
        if(API_SUCCESS != status)
        {
            LOG_DEBUG("SDP OPEN FAILED!\n");
            break;
        }
        LOG_DEBUG("SDP OPEN\n\n");

        LOG_DEBUG("Initiating SDP Service Attribute Request ....\n");
        retval = BT_sdp_servicesearchattributerequest
                 (
                     &opp_sdp_handle,
                     uuid,
                     3U,
                     attrib_id,
                     num_attrib_ids,
                     NULL,
                     0U,
                     opp_attrib_data,
                     &opp_attrib_data_len
                 );

        if(API_SUCCESS != retval)
        {
            LOG_DEBUG("SDP SEARCH Failed for OPP..Closing SDP\n");
            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&opp_sdp_handle);
        }
        break;

    case SDP_Close:
        LOG_DEBUG("SDP CLOSE\n\n");
        break;

    case SDP_ErrorResponse:
        LOG_DEBUG("SDP ERROR RESPONSE\n\n");
        break;

    case SDP_ServiceSearchAttributeResponse:
        LOG_DEBUG("\n");

        {
            UCHAR no_server_channels;
            UCHAR * sdp_query_result;

#ifdef OBEX_OVER_L2CAP
            UCHAR attr_val_2B[2U];
            UINT16 attr_size;
            UINT16 psm;
#endif /* OBEX_OVER_L2CAP */

            sdp_query_result = data;
            no_server_channels = 1U;

            while (NULL != sdp_query_result)
            {
                sdp_query_result =
                BT_sdp_get_Pointer_to_next_AttributeList
                (
                    data,
                    no_server_channels
                );

                if (NULL == sdp_query_result)
                {
                    continue;
                }

#ifdef OBEX_OVER_L2CAP
                psm = 0x0000U;

                /* Check for OBEX L2CAP PSM for OPP in the Protocol Descriptor List */
                retval = BT_sdp_get_psm (sdp_query_result, &psm);
                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("SDP FAILED to find PSM in Protocol Descriptor List\n");
                }
                else
                {
                    LOG_DEBUG("PSM: 0x%04X\n", psm);
                }

                /*
                 * PSM not found in Protocol Descriptor List.
                 * Check for GOEP L2CAP PSM attribute
                 */
                if ((API_SUCCESS != retval) || (RFCOMM_PSM == psm))
                {
                    attr_size = 2U;
                    /* Get the GOEP L2CAP PSM */
                    retval = BT_sdp_get_attr_value
                             (
                                 GOEP_L2CAP_PSM_ATTR_ID,
                                 sdp_query_result,
                                 attr_val_2B,
                                 &attr_size
                             );

                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG("SDP FAILED to find GOEP L2CAP PSM\n");
                    }
                    else
                    {
                        BT_UNPACK_LE_2_BYTE(&opp_l2cap_psm, attr_val_2B);
                        LOG_DEBUG("GOEP L2CAP PSM: 0x%04X\n", opp_l2cap_psm);
                    }
                }

                /* if ((API_SUCCESS != retval) && (RFCOMM_PSM == psm)) */
#endif /* OBEX_OVER_L2CAP */
                {
                    /* No PSMs found, check the OPP server channel number */
                    retval = BT_sdp_get_channel_number
                             (
                                 sdp_query_result,
                                 &opp_server_channel
                             );

                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG("SDP FAILED to find OPP Server Channel\n");
                    }
                    else
                    {
                        LOG_DEBUG("Server Channel: 0x%02X\n", opp_server_channel);

                        /*
                         * Store the Server Channel globally.
                         * Later it will be used while Creating OPP Connection
                         */
                    }
                }

                /* OPP PSM or Server channel found */
                if (API_SUCCESS == retval)
                {
                    LOG_DEBUG ("OPP Service Found\n");
                }
                else
                {
                    LOG_DEBUG ("Failed to find OPP Service.\n");
                }

                no_server_channels++;
            }
        }

        /* Now Close the SDP Connection */
        (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&opp_sdp_handle);
        break;

    default: /* Events not needed to be Handled */
        LOG_DEBUG("Unhandled SDP Event (0x%02X)\n", command);
        break;
    }/* switch */

    return;
}


void opp_client_print_appl_instances (void)
{
    UINT8 i;

    LOG_DEBUG ("\n\n===========================================================\n");
    LOG_DEBUG ("Instance  OPP Handle\t BD Addr");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < OPP_NUM_CLIENT_INSTANCE; i++)
    {
        LOG_DEBUG ("%d\t  ", i);
        LOG_DEBUG ("%02x\t\t ", opp_client_instance[i].handle);
        LOG_DEBUG ("%02x:%02x:%02x:%02x:%02x:%02x\t ",
        opp_client_instance[i].bd_addr[0U], opp_client_instance[i].bd_addr[1U],
        opp_client_instance[i].bd_addr[2U], opp_client_instance[i].bd_addr[3U],
        opp_client_instance[i].bd_addr[4U], opp_client_instance[i].bd_addr[5U]);

        LOG_DEBUG ("\n");

    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    return;
}

#endif /* OPP */

