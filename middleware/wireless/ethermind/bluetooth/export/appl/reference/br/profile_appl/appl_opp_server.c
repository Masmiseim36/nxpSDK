
/**
 *  \file appl_opp_server.c
 *
 *  Source file for OPP Server Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_opp.h"

#ifdef OPP

/** Enable to accept other file types */
#define APPL_OPP_ACCEPT_OTHER_FILE_TYPES

/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static OPP_INSTANCE           opp_server_instance[OPP_NUM_SERVER_INSTANCE];

static UCHAR                  opp_push_started;

static BT_fops_file_handle    opp_rx_fp;
static BT_fops_file_handle    opp_tx_fp;

static UINT32                 fsize;
static UINT32                 remaining, sent;
static UINT32                 opp_xchg_size;

static UCHAR                  appl_push_error;
static UCHAR                  appl_pull_error;
static UCHAR                  file_object[OPP_FOLDER_NAME_LEN * 2U];
/* static UCHAR                  object_name[OPP_OBJECT_NAME_LEN];*/

static UCHAR        wait_count;

static const UCHAR opp_server_menu[] =
" \n \
-------------- OPP Server Menu --------------\n \
\n \
\t 0. Exit \n \
\t 1. Refresh \n \
\n \
\t 2. OPP Server Initialize \n \
\t 3. OPP Server Shutdown \n \
\t 4. OPP Server Start \n \
\t 5. OPP Server Stop \n \
\t 6. OPP Server Accept or Reject Push \n\
\t 7. OPP Server Accept or Reject Pull \n\
\t 10. OPP Server set number of wait stages \n\
\t 17. Close Transport Connection with OPP Client \n\
\n\
\t 20. List Directory \n\
\n\
Your Choice: ";

/* ----------------------------------------- Functions */

void main_opp_server_operations (void)
{
    API_RESULT retval;
    UINT16 i;
    UINT8 *phandle;
    int choice, menu_choice, handle, option;
    char path[BT_FOPS_MAX_DIRECTORY_SIZE];

    retval = API_SUCCESS;

    BT_LOOP_FOREVER()
    {
        printf ("%s", opp_server_menu);
        scanf ("%d", &choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            LOG_DEBUG ("Initializing OPP Server...\n");
            retval = BT_opp_server_init ();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                /* Reset BD_ADDR & OPP handle */
                for (i = 0U; i < OPP_NUM_SERVER_INSTANCE; i++)
                {
                    OPP_RESET_INSTANCE(&opp_server_instance[i]);
                }
            }

            appl_push_error = 0U;
            appl_pull_error = 0U;
            opp_server_print_appl_instances ();
            break;

        case 3:
            LOG_DEBUG ("Shutting down OPP Server...\n");
            retval = BT_opp_server_shutdown ();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 4:
            opp_server_print_appl_instances ();

            LOG_DEBUG ("Enter OPP Server instance:");
            scanf ("%d", &handle);

            /* OPP application instance handle */
            phandle = &opp_server_instance[handle].handle;

            if (OPP_ENTITY_INVALID != *phandle)
            {
                LOG_DEBUG ("Application Instance not free!\n");
                break;
             }

            LOG_DEBUG ("Starting OPP Server...\n");
            retval = BT_opp_server_start
                     (
                         phandle,
                         appl_opp_server_callback
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS != retval)
            {
                *phandle = OPP_ENTITY_INVALID;
            }
            else
            {
                /* Set path to root folder */
                BT_str_copy
                (
                    opp_server_instance[handle].path,
                    OPP_ROOT_FOLDER_BASE
                );
            }

            opp_server_print_appl_instances();
            break;

        case 5:
            opp_server_print_appl_instances();

            LOG_DEBUG ("Enter OPP Server instance: ");
            scanf ("%d", &handle);

            /* OPP applicaion instance handle */
            phandle = &opp_server_instance[handle].handle;

            LOG_DEBUG ("Stopping  OPP Server...\n");
            retval = BT_opp_server_stop (phandle);
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                *phandle = OPP_ENTITY_INVALID;

                BT_mem_set
                (
                    &opp_server_instance[handle].bd_addr,
                    0x00,
                    BT_BD_ADDR_SIZE
                );
            }

            opp_server_print_appl_instances ();
            break;

        case 6:
            LOG_DEBUG (" 0: Accept Push 0x<Err>: Reject Push\n");
            scanf("%x",&option);
            appl_push_error = (UCHAR)option;
            break;

        case 7:
            LOG_DEBUG(" 0: Accept Pull 0x<Err>: Reject Pull\n");
            scanf("%x", &option);
            appl_pull_error = (UCHAR)option;
            break;

        case 10:
            LOG_DEBUG("Enter number of wait stages: ");
            scanf("%d", &choice);
            wait_count = (UCHAR)choice;
            LOG_DEBUG("number of wait stages %d\n ", wait_count);
            break;

        case 17:
            LOG_DEBUG ("Enter OPP Server instance: ");
            scanf ("%d", &handle);

            LOG_DEBUG ("Disconnecting Transport with OPP Server\n");
            retval = BT_opp_server_transport_close (&opp_server_instance[handle].handle);
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 20:
            LOG_DEBUG ("Enter the path: ");
            scanf("%s", path);
            EM_fops_list_directory(path);
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


API_RESULT appl_opp_server_callback
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
    UCHAR send_response;

    OPP_RESPONSE_STRUCT opp_rsp_info;
    OPP_HEADER_STRUCT body_info;
    OPP_HEADERS *opp_rx_hdrs;
    OPP_HEADERS opp_tx_hdrs;
    UCHAR tx_response;
    UINT16 /* num_folders, */ i;

    UCHAR      rx_object_type[OPP_OBJECT_NAME_LEN];

#ifndef APPL_OPP_ACCEPT_OTHER_FILE_TYPES
    UCHAR      opp_obj_type_str_len;
#endif

    UINT32 length;
    UINT16 actual;
    UCHAR *data;

    BT_IGNORE_UNUSED_PARAM(event_hdrlen);

    /* Initialize to default values */
    handle = *opp_handle;
    retval = API_SUCCESS;

    opp_rsp_info.name = NULL;
    opp_rsp_info.body = NULL;

#ifndef APPL_OPP_ACCEPT_OTHER_FILE_TYPES
    opp_obj_type_str_len = 0U;
#endif
    send_response = 0U;
    opp_rx_hdrs = event_header;
    tx_response = (UCHAR )event_result;

    data = NULL;
    length = 0x00U;
    actual = 0x00U;

    for (i = 0U; i < OPP_NUM_SERVER_INSTANCE; i++)
    {
        if (opp_server_instance[i].handle == handle)
        {
            break;
        }
    }

    if (OPP_NUM_SERVER_INSTANCE == i)
    {
        LOG_DEBUG ("Failed to find application FTO server instance!\n");
        /* return API_SUCCESS; */
    }
    else
    {
        switch(event_type)
        {
        case OPP_SERVER_TRANSPORT_CLOSE_IND: /* Fall Through */
        case OPP_SERVER_TRANSPORT_CLOSE_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received %s\n", (OPP_SERVER_TRANSPORT_CLOSE_IND == event_type) ?
            "OPP_SERVER_TRANSPORT_CLOSE_IND" : "OPP_SERVER_TRANSPORT_CLOSE_CNF");
            LOG_DEBUG("OPP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            BT_mem_set (opp_server_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);

            opp_server_print_appl_instances ();
            break;

        case OPP_SERVER_CONNECT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received OPP_SERVER_CONNECT_IND\n");
            LOG_DEBUG("OPP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            if (OPP_SUCCESS_RSP != event_result)
            {
                tx_response = OPP_NOT_ACCEPTABLE_RSP;

                send_response = 1U;

                break;
            }

            LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (opp_rx_hdrs->opp_connect_info->bd_addr));

            LOG_DEBUG ("MAX Tx. Size: %d\n",
            opp_rx_hdrs->opp_connect_info->max_tx_size);
            tx_response = OPP_SUCCESS_RSP;

            /* Set response to be sent */
            send_response = 1U;

            /* Get the Handle parameters */
            BT_mem_copy
            (
                opp_server_instance[i].bd_addr,
                opp_rx_hdrs->opp_connect_info->bd_addr,
                BT_BD_ADDR_SIZE
            );

            opp_server_instance[i].max_xchg_size =
                opp_rx_hdrs->opp_connect_info->max_tx_size;

            opp_server_print_appl_instances ();
            break;

        case OPP_SERVER_DISCONNECT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received OPP_SERVER_DISCONNECT_IND\n");
            LOG_DEBUG("OPP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            opp_server_print_appl_instances ();
            break;

        case OPP_SERVER_PUSH_OBJECT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received OPP_SERVER_PUSH_OBJECT_IND\n");
            LOG_DEBUG("OPP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            if (0U != appl_push_error)
            {
                tx_response = appl_push_error;
                LOG_DEBUG("Sending %02X Error Code\n ", appl_push_error);
                send_response = 1U;

                 break;
            }

            /* Set response to be sent */
            send_response = 1U;

            opp_tx_hdrs.opp_rsp_info = &opp_rsp_info;

            if ((OPP_SUCCESS_RSP != event_result) &&
                (OPP_CONTINUE_RSP != event_result))
            {
                break;
            }

            /* Check for Name hdr in the request */
            if (0U == opp_push_started)
            {
                if ((NULL != opp_rx_hdrs->opp_req_info->type) &&
                    (NULL != opp_rx_hdrs->opp_req_info->type->value) &&
                    (0U != opp_rx_hdrs->opp_req_info->type->length))
                {
                    LOG_DEBUG("OPP Push Object Type: %s\n", opp_rx_hdrs->opp_req_info->type->value);

                    if (0 == BT_mem_cmp
                             (
                                 opp_rx_hdrs->opp_req_info->type->value,
                                 OPP_TYPE_HDR_VCARD,
                                 opp_rx_hdrs->opp_req_info->type->length
                             ))
                    {
                        BT_str_n_copy(rx_object_type, OPP_OBJECT_TYPE_VCARD, sizeof(OPP_OBJECT_TYPE_VCARD));
                    }
                    else if (0 == BT_mem_cmp
                                  (
                                      opp_rx_hdrs->opp_req_info->type->value,
                                      OPP_TYPE_HDR_VCALENDER,
                                      opp_rx_hdrs->opp_req_info->type->length
                                  ))
                    {
                        BT_str_n_copy(rx_object_type, OPP_OBJECT_TYPE_VCAL, sizeof(OPP_OBJECT_TYPE_VCAL));
                    }
                    else if (0 == BT_mem_cmp
                                  (
                                      opp_rx_hdrs->opp_req_info->type->value,
                                      OPP_TYPE_HDR_VMSG,
                                      opp_rx_hdrs->opp_req_info->type->length
                                  ))
                    {
                        BT_str_n_copy(rx_object_type, OPP_OBJECT_TYPE_VMAG, sizeof(OPP_OBJECT_TYPE_VMAG));
                    }
#ifdef APPL_OPP_ACCEPT_OTHER_FILE_TYPES
                    else
                    {
                        LOG_DEBUG ("Received Object type - %s\n", opp_rx_hdrs->opp_req_info->type->value);
                    }
#else /* APPL_OPP_ACCEPT_OTHER_FILE_TYPES */
                    else
                    {
                        tx_response = OPP_UNSUPP_MEDIA_RSP;
                        break;
                    }

                    /* Check if the received object has the string corresponding to object type */
                    opp_obj_type_str_len = (UCHAR)BT_str_n_len(rx_object_type, sizeof(rx_object_type));

                    if ((NULL != opp_rx_hdrs->opp_req_info->body->value) &&
                        (opp_rx_hdrs->opp_req_info->body->length > opp_obj_type_str_len))
                    {
                        if (NULL == BT_str_str(opp_rx_hdrs->opp_req_info->body->value, rx_object_type))
                        {
                            /* No, Body of the OPP object does not match with the type hdr */
                            tx_response = OPP_BAD_REQ_RSP;
                            break;
                        }
                    }
#endif /* APPL_OPP_ACCEPT_OTHER_FILE_TYPES */
                }

                if((NULL == opp_rx_hdrs->opp_req_info->name) ||
                   (NULL == opp_rx_hdrs->opp_req_info->name->value) ||
                   (0U == opp_rx_hdrs->opp_req_info->name->length))
                {
                    tx_response = OPP_BAD_REQ_RSP;
                    break;
                }
                else
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)OPP_ROOT_RX_FOLDER_BASE,
                        opp_rx_hdrs->opp_req_info->name->value,
                        file_object
                    );

                    LOG_DEBUG("Created file object for push request -> %s\n", file_object);
                }

                opp_push_started = 0x01U;
            }

            if ((NULL == opp_rx_hdrs->opp_req_info->body->value) &&
                (0U == opp_rx_hdrs->opp_req_info->body->length))
            {
                LOG_DEBUG ("No Body Content found ");
            }
            else
            {
                length = opp_rx_hdrs->opp_req_info->body->length;
                data = opp_rx_hdrs->opp_req_info->body->value;

    #if 0
                /* Print received data on console */
                LOG_DEBUG ("\n\n ====== OPP Push Object data ===== \n\n");
                for (i = 0; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n\n ================================= \n");
    #endif /* 0 */

                /* Open file PUT request */
                if (NULL == opp_rx_fp)
                {
                    retval = BT_fops_file_open
                             (
                                 file_object,
                                 (UCHAR *)"wb",
                                 &opp_rx_fp
                             );

                    if ((API_SUCCESS != retval) || (NULL == opp_rx_fp))
                    {
                        LOG_DEBUG ("Failed to open PUSH Object file\n");
                        tx_response = OPP_NOT_FOUND_RSP;
                        break;
                    }
                }

                /* write to file */
                if (NULL != opp_rx_fp)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, (UINT16)length, opp_rx_fp, (UINT16 *)&actual);
                }
            }

            /* Reset */
            if (OPP_SUCCESS_RSP == event_result)
            {
                if (NULL != opp_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (opp_rx_fp);
                    opp_rx_fp = NULL;
                }

                opp_push_started = 0x00U;
            }
            break;

        case OPP_SERVER_PULL_BUSINESS_CARD_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received OPP_SERVER_PULL_BUSINESS_CARD_IND\n");
            LOG_DEBUG("OPP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            opp_rsp_info.body = &body_info;
            opp_tx_hdrs.opp_rsp_info = &opp_rsp_info;

            /* send response */
            send_response = 1U;

            if (0U != appl_pull_error)
            {
                tx_response = appl_pull_error;
                LOG_DEBUG("Sending %02X Error Code\n ", appl_pull_error);

                break;
            }

            if (OPP_SUCCESS_RSP != event_result)
            {
                /* Wait for OPP_SUCCESS_RSP */
                tx_response = (UCHAR)event_result;
                break;
            }

            if (0U == sent)
            {
                opp_xchg_size = opp_server_instance[handle].max_xchg_size;
                tx_response = OPP_SUCCESS_RSP;

                /* Create the file object in the path */
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)OPP_ROOT_FOLDER_BASE,
                    (UCHAR *)OPP_OBJECT_VCARD,
                    file_object
                );

                /* Create the listing file */
                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"rb",
                             &opp_tx_fp
                         );
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("Failed to Open File to Pull\n");
                    tx_response = OPP_NOT_FOUND_RSP;
                    break;
                }

                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(opp_tx_fp, &fsize);
                remaining = fsize;
            }

            /* Fill body hdr values */
            if (remaining > opp_xchg_size)
            {
                body_info.length = (UINT16)opp_xchg_size;
                tx_response = OPP_CONTINUE_RSP;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16)remaining;
                tx_response = OPP_SUCCESS_RSP;
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
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read(body_info.value, body_info.length, opp_tx_fp, &actual);
            }

            /* Update object size sent & remaining to send */
            sent += body_info.length;
            remaining = fsize - sent;

            if (0U == remaining)
            {
                /* Reset the variables */
                sent = 0U;
                fsize = 0U;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(opp_tx_fp);
                opp_tx_fp = NULL;
            }
            break;

        case OPP_SERVER_ABORT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received OPP_SERVER_ABORT_IND\n");
            LOG_DEBUG("OPP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            if (NULL != opp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(opp_rx_fp);
                opp_rx_fp = NULL;
                opp_push_started = 0U;

                (BT_IGNORE_RETURN_VALUE) BT_fops_object_delete(file_object);
            }

            if (NULL != opp_tx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(opp_tx_fp);
                opp_tx_fp = NULL;
            }
            break;

        default:
            LOG_DEBUG("Invalid Event Type: 0x%02X\n", event_type);
            break;
        }

        /* Send response if required */
        if (0U != send_response)
        {
            if (0U < wait_count)
            {
                opp_rsp_info.wait = BT_TRUE;
                opp_tx_hdrs.opp_rsp_info = &opp_rsp_info;

                wait_count--;
            }

            retval = BT_opp_server_send_response
                     (
                         &handle,
                         event_type,
                         tx_response,
                         &opp_tx_hdrs
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed to send OPP server Response - 0x%04x\n", retval);
            }
        }
    }

    return retval;
}


void opp_server_print_appl_instances (void)
{
    UINT8 i;

    LOG_DEBUG ("\n\n===========================================================\n");
    LOG_DEBUG ("Instance  OPP Handle\t BD Addr");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < OPP_NUM_SERVER_INSTANCE; i++)
    {
        LOG_DEBUG ("%d\t  ", i);
        LOG_DEBUG ("%02x\t\t ", opp_server_instance[i].handle);
        LOG_DEBUG ("%02x:%02x:%02x:%02x:%02x:%02x\t ",
        opp_server_instance[i].bd_addr[0U], opp_server_instance[i].bd_addr[1U],
        opp_server_instance[i].bd_addr[2U], opp_server_instance[i].bd_addr[3U],
        opp_server_instance[i].bd_addr[4U], opp_server_instance[i].bd_addr[5U]);

        LOG_DEBUG ("\n");

    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    return;
}

#endif  /* OPP */
