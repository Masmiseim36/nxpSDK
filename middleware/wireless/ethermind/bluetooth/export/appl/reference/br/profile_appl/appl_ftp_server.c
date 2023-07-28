
/**
 *  \file appl_ftp_server.c
 *
 *  Source file for FTP Server Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_ftp.h"
#include "ftp_pl.h"
#include "appl_utils.h"

#ifdef FTP
/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static FTP_INSTANCE  ftp_server_instance[FTP_NUM_SERVER_INSTANCE];

static UCHAR  ftp_push_started;

static BT_fops_file_handle ftp_rx_fp;
static BT_fops_file_handle  ftp_tx_fp;

static UINT32       fsize;
static UINT32       remaining, sent;
static UINT16       ftp_xchg_size;

static UCHAR        appl_reject_opn;
static UCHAR        appl_reject_action;
static UCHAR        appl_ftp_server_read_only;

static UCHAR        wait_count;

/* Flag to check if name header is received for GET operation */
static UCHAR        appl_name_hdr_in_get;

#ifndef OBEX_ASCII_TO_FROM_UNICODE
/**
 * Application has to perform ASCII to/from Unicode Encode/Decode.
 */
static UCHAR file_name[FTP_FILE_NAME_LEN];
#endif /* OBEX_ASCII_TO_FROM_UNICODE */

static const UCHAR ftp_server_menu[] =
" \n \
-------------- FTP Server Menu --------------\n \
\n \
\t 0. Exit \n \
\t 1. Refresh \n \
\n \
\t 2. FTP Server Initialize \n \
\t 3. FTP Server Shutdown \n \
\t 4. FTP Server Start \n \
\t 5. FTP Server Stop \n \
\n \
\t 6. FTP Server Reject Operation \n\
\t 7. FTP Server Reject Action \n\
\n \
\t 10.FTP Server set/unset as Read Only \n\
\n \
\t 12.FTP Server set number of wait stages \n\
\n \
\t 17. Close Transport Connection with FTP Client \n\
\n \
\t 50: List Directory \n\
\n \
Your Choice: ";

/* ----------------------------------------- Functions */

void main_ftp_server_operations (void)
{
    API_RESULT retval;
    UINT16 i;
    UINT8 *phandle;
    int choice, menu_choice, handle, option;
    char path[BT_FOPS_MAX_DIRECTORY_SIZE];

    retval = API_SUCCESS;

    BT_LOOP_FOREVER()
    {
        printf ("%s", ftp_server_menu);
        scanf ("%d", &choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            LOG_DEBUG ("Initializing FTP Server...\n");
            retval = BT_ftp_server_init ();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                /* Reset BD_ADDR & FTP handle */
                for (i = 0U; i < FTP_NUM_SERVER_INSTANCE; i++)
                {
                    FTP_RESET_INSTANCE(&ftp_server_instance[i]);
                }
            }

            appl_reject_opn = 0U;
            appl_reject_action = 0U;
            appl_ftp_server_read_only = 0U;

            ftp_server_print_appl_instances ();
            break;

        case 3:
            LOG_DEBUG ("Shutting down FTP Server...\n");
            retval = BT_ftp_server_shutdown ();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 4:
            ftp_server_print_appl_instances ();

            LOG_DEBUG ("Enter FTP Server instance:");
            scanf ("%d", &handle);

            if (FTP_NUM_SERVER_INSTANCE <= handle)
            {
                LOG_DEBUG("Invalid instance, Try Again\n");
                break;
            }

            /* FTP application instance handle */
            phandle = &ftp_server_instance[handle].handle;

            if (FTP_ENTITY_INVALID != *phandle)
            {
                LOG_DEBUG ("Application Instance not free!\n");
                break;
            }

            LOG_DEBUG ("Starting FTP Server...\n");
            retval = BT_ftp_server_start
                     (
                         phandle,
                         appl_ftp_server_callback
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS != retval)
            {
                *phandle = FTP_ENTITY_INVALID;
            }
            else
            {
                /* Set path to root folder */
                BT_str_copy
                (
                    ftp_server_instance[handle].path,
                    FTP_ROOT_FOLDER_BASE
                );

                ftp_server_instance[handle].root_depth = 0U;
            }

            ftp_server_print_appl_instances();
            break;

        case 5:
            ftp_server_print_appl_instances();

            LOG_DEBUG ("Enter FTP Server instance: ");
            scanf ("%d", &handle);

            if (FTP_NUM_SERVER_INSTANCE <= handle)
            {
                LOG_DEBUG("Invalid instance, Try Again\n");
                break;
            }

            /* FTP applicaion instance handle */
            phandle = &ftp_server_instance[handle].handle;

            LOG_DEBUG ("Stopping  FTP Server...\n");
            retval = BT_ftp_server_stop (phandle);
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                *phandle = FTP_ENTITY_INVALID;

                BT_mem_set
                (
                    &ftp_server_instance[handle].bd_addr,
                    0x00,
                    BT_BD_ADDR_SIZE
                );
            }

            ftp_server_print_appl_instances ();
            break;

        case 6:
            LOG_DEBUG (" 0: Accept Operation 0x<Err>: Reject Operation \n");
            scanf("%x",&option);
            appl_reject_opn = (UCHAR )option;
            break;

        case 7:
            LOG_DEBUG(" 0: Accept Action 0x<Err>: Reject Action \n");
            scanf("%x", &option);
            appl_reject_action = (UCHAR )option;
            break;

        case 10:
            LOG_DEBUG("Set/Unset Server as Read Only 0: Unset 0x<Err>: Read Only Error Code ?");
            scanf("%X",&option);
            appl_ftp_server_read_only = (UCHAR )option;
            break;

        case 12:
            LOG_DEBUG("Enter number of wait stages: ");
            scanf("%d", &choice);
            wait_count = (UCHAR)choice;
            LOG_DEBUG("number of wait stages %d\n ", wait_count);
            break;

        case 17:
            LOG_DEBUG ("Enter FTP Server instance: ");
            scanf ("%d", &handle);

            if (FTP_NUM_SERVER_INSTANCE <= handle)
            {
                LOG_DEBUG("Invalid instance, Try Again\n");
                break;
            }

            LOG_DEBUG ("Disconnecting FTP Transport with FTP client\n");
            retval = BT_ftp_server_transport_close (&ftp_server_instance[handle].handle);
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 50:
            LOG_DEBUG ("Enter the path: ");
            scanf("%s", path);

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_list_directory(path);
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


API_RESULT appl_ftp_server_callback
           (
               FTP_HANDLE   * ftp_handle,
               UINT8          event_type,
               UINT16         event_result,
               FTP_HEADERS  * event_header,
               UINT16         event_hdrlen
           )
{
    API_RESULT retval;
    FTP_HANDLE handle;
    UCHAR send_response;
    UCHAR * to_free;

    FTP_RESPONSE_STRUCT ftp_rsp_info;
    FTP_HEADER_STRUCT body_info;
    FTP_HEADERS *ftp_rx_hdrs;
    FTP_HEADERS ftp_tx_hdrs;
    UCHAR tx_response;
    UINT16 i;

#ifndef OBEX_ASCII_TO_FROM_UNICODE
    UINT16 j;
#endif /* OBEX_ASCII_TO_FROM_UNICODE */

    UINT16 length;
    UINT16 actual;
    UCHAR *data;

    UCHAR object_name[64U];
    UINT32 file_attribute;

    static UCHAR file_object[FTP_FOLDER_NAME_LEN * 2U];
    static BT_fops_file_handle fp = NULL;

    BT_IGNORE_UNUSED_PARAM(event_hdrlen);

    /* Initialize to default values */
    handle = *ftp_handle;
    retval = API_SUCCESS;

    ftp_rsp_info.name = NULL;
    ftp_rsp_info.body = NULL;
    ftp_rsp_info.obj_size = 0U;

    BT_mem_set(&ftp_tx_hdrs, 0, sizeof(FTP_HEADERS));
    BT_mem_set(&ftp_rsp_info, 0, sizeof(FTP_RESPONSE_STRUCT));
    FTP_INIT_HEADER_STRUCT (body_info);

    to_free = NULL;
    send_response = 0U;
    ftp_rx_hdrs = event_header;
    tx_response = (UCHAR )event_result;

    data = NULL;
    length = 0x00U;
    actual = 0x00U;
    i = 0U;

    for (i = 0U; i < FTP_NUM_SERVER_INSTANCE; i++)
    {
        if (ftp_server_instance[i].handle == handle)
        {
            break;
        }
    }

    if (FTP_NUM_SERVER_INSTANCE == i)
    {
        LOG_DEBUG ("Failed to find application FTO server instance!\n");
        retval = API_SUCCESS; /* return API_SUCCESS; */
    }
    else
    {
        switch(event_type)
        {
        case FTP_SERVER_TRANSPORT_CLOSE_IND: /* Fall Through */
        case FTP_SERVER_TRANSPORT_CLOSE_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received %s\n", (FTP_SERVER_TRANSPORT_CLOSE_IND == event_type) ?
            "FTP_SERVER_TRANSPORT_CLOSE_IND" : "FTP_SERVER_TRANSPORT_CLOSE_CNF");
            LOG_DEBUG("FTP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            BT_mem_set (ftp_server_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);

            /* Reset */
            appl_name_hdr_in_get = 0U;

            ftp_server_print_appl_instances ();
            break;

        case FTP_SERVER_CONNECT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received FTP_SERVER_CONNECT_IND\n");
            LOG_DEBUG("FTP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            if (FTP_SUCCESS_RSP != event_result)
            {
                tx_response = FTP_NOT_ACCEPTABLE_RSP;

                send_response = 1U;

                break;
            }

            LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (ftp_rx_hdrs->ftp_connect_info->bd_addr));

            LOG_DEBUG ("MAX Exchange Size: %d\n",
            ftp_rx_hdrs->ftp_connect_info->max_recv_size);
            tx_response = FTP_SUCCESS_RSP;

            /* Set response to be sent */
            send_response = 1U;

            /* Get the Handle parameters */
            BT_mem_copy
            (
                ftp_server_instance[i].bd_addr,
                ftp_rx_hdrs->ftp_connect_info->bd_addr,
                BT_BD_ADDR_SIZE
            );

            ftp_server_instance[i].max_xchg_size =
                ftp_rx_hdrs->ftp_connect_info->max_recv_size;

            ftp_server_print_appl_instances ();

            /* Reset */
            appl_name_hdr_in_get = 0U;

            /* Set path to root folder */
            if (0U != ftp_server_instance[i].root_depth)
            {
                while (0U != ftp_server_instance[i].root_depth)
                {
                    retval = BT_vfops_set_path_backward(ftp_server_instance[i].path);
                    if (API_SUCCESS != retval)
                    {
                        tx_response = FTP_SERVER_ERROR;
                        LOG_DEBUG ("Failed to set path backward\n");
                        break;
                    }
                    ftp_server_instance[i].root_depth --;
                }

                LOG_DEBUG ("Folder set to ROOT\n");
            }
            break;

        case FTP_SERVER_DISCONNECT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received FTP_SERVER_DISCONNECT_IND\n");
            LOG_DEBUG("FTP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            ftp_server_print_appl_instances ();
            break;

        case FTP_SERVER_PUSH_FILE_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received FTP_SERVER_PUSH_FILE_IND\n");
            LOG_DEBUG("FTP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* Set response to be sent */
            send_response = 1U;

            ftp_tx_hdrs.ftp_rsp_info = &ftp_rsp_info;

            if ((FTP_SUCCESS_RSP != event_result) &&
                (FTP_CONTINUE_RSP != event_result))
            {
                /* tx_response = FTP_SERVER_ERROR; */

                /**
                 * Ideally, should not reach here.
                 */
                ftp_push_started = 0x00U;

                if (NULL != ftp_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close(ftp_rx_fp);
                    ftp_rx_fp = NULL;
                }
                break;
            }

            /* Check if push already ongoing */
            if (0U == ftp_push_started)
            {
                /* Object size */
                if (0U != ftp_rx_hdrs->ftp_req_info->obj_size)
                {
                    LOG_DEBUG ("Object size = 0x%08X\n", ftp_rx_hdrs->ftp_req_info->obj_size);
                }

                /* Check for Name hdr in the request */
                if((NULL == ftp_rx_hdrs->ftp_req_info->name) ||
                   (NULL == ftp_rx_hdrs->ftp_req_info->name->value) ||
                   (0U == ftp_rx_hdrs->ftp_req_info->name->length))
                {
                    tx_response = FTP_BAD_REQ_RSP;

                    break;
                }

                /* Reject if operation reject is set or if Read ONly is set */
                else if (0U != appl_ftp_server_read_only)
                {
                    tx_response = appl_ftp_server_read_only;
                    LOG_DEBUG("Rejected: Reason - 0x%02X (Read Only)\n", tx_response);

                    break;
                }
                else if (0U != appl_reject_opn)
                {
                    tx_response = appl_reject_opn;
                    LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);

                    break;
                }
                else
                {
#ifndef OBEX_ASCII_TO_FROM_UNICODE
                    /* Coverting Unicode to ASCII */
                    for (j = 0U; j < (ftp_rx_hdrs->ftp_req_info->name->length / 2U); j++)
                    {
                        file_name[j] = ftp_rx_hdrs->ftp_req_info->name->value[j * 2U + 1U];
                    }

                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        ftp_server_instance[i].path,
                        file_name,
                        file_object
                    );

                    LOG_DEBUG ("Push File Name: %s\n", file_name);
#else
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        ftp_server_instance[i].path,
                        ftp_rx_hdrs->ftp_req_info->name->value,
                        file_object
                    );
#endif /* OBEX_ASCII_TO_FROM_UNICODE */
                }

                ftp_push_started = 0x01U;
            }

            if ((NULL == ftp_rx_hdrs->ftp_req_info->body->value) &&
                (0U == ftp_rx_hdrs->ftp_req_info->body->length))
            {
                /**
                 * Ideally Should not reach here.
                 */
                tx_response = FTP_NOT_FOUND_RSP;
                LOG_DEBUG ("No Body Content found ");

                ftp_push_started = 0x00U;

                if (NULL != ftp_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close(ftp_rx_fp);
                    ftp_rx_fp = NULL;
                }
                break;
            }
            else
            {
                length = ftp_rx_hdrs->ftp_req_info->body->length;
                data = ftp_rx_hdrs->ftp_req_info->body->value;

#if 0
                /* Print received data on console */
                LOG_DEBUG ("\n\n ====== FTP Push Object data ===== \n\n");
                for (i = 0; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n\n ================================= \n");
#endif /* 0 */

                /* Open file PUT request */
                if (NULL == ftp_rx_fp)
                {
                    retval = BT_fops_file_open
                             (
                                 (UCHAR *)file_object,
                                 (UCHAR *)"wb",
                                 &ftp_rx_fp
                             );
                    if ((API_SUCCESS != retval) || (NULL == ftp_rx_fp))
                    {
                        tx_response = FTP_NOT_FOUND_RSP;
                        LOG_DEBUG ("Failed to open PUSH Object file\n");

                        ftp_push_started = 0x00U;
                    }
                }

                /* write to file */
                if (NULL != ftp_rx_fp)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, ftp_rx_fp, &actual);
                }
            }

            /* Reset */
            if (FTP_SUCCESS_RSP == event_result)
            {
                if (NULL != ftp_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (ftp_rx_fp);
                    ftp_rx_fp = NULL;
                }

                ftp_push_started = 0x00U;
            }
            break;

        case FTP_SERVER_DELETE_FILE_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received FTP_SERVER_DELETE_FILE_IND\n");
            LOG_DEBUG("FTP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* send response */
            send_response = 1U;

            tx_response = FTP_SUCCESS_RSP;

            ftp_tx_hdrs.ftp_rsp_info = &ftp_rsp_info;

            /* Delete File Name */
            if ((NULL != ftp_rx_hdrs->ftp_req_info) &&
                (NULL != ftp_rx_hdrs->ftp_req_info->name) &&
                (NULL != ftp_rx_hdrs->ftp_req_info->name->value) &&
                (0U != ftp_rx_hdrs->ftp_req_info->name->length))
            {
                LOG_DEBUG ("Delete File Name: %s\n", ftp_rx_hdrs->ftp_req_info->name->value);

                /* Reject if operation reject is set or if Read ONly is set */
                if (0U != appl_ftp_server_read_only)
                {
                    tx_response = appl_ftp_server_read_only;
                    LOG_DEBUG("Rejected: Reason - 0x%02X (Read Only)\n", tx_response);

                    break;
                }
                else if (0U != appl_reject_opn)
                {
                    tx_response = appl_reject_opn;
                    LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);

                    break;
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    ftp_server_instance[i].path,
                    ftp_rx_hdrs->ftp_req_info->name->value,
                    file_object
                );

                retval = BT_fops_get_file_attributes
                         (
                             file_object,
                             &file_attribute
                         );

                if (API_SUCCESS != retval)
                {
                    tx_response = OBEX_NOT_FOUND_RSP;
                }
                else
                {
                    if (BT_FOPS_MASK_READONLY == (file_attribute & BT_FOPS_MASK_READONLY))
                    {
                        printf ("object cannot be deleted : %s",object_name);
                        tx_response = OBEX_UNAUTHORIZED_RSP;
                    }
                    else
                    {
                       (BT_IGNORE_RETURN_VALUE) BT_fops_object_delete(file_object);
                    }
                }
            }
            break;

        case FTP_SERVER_GET_FOLDER_LISTING_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received FTP_SERVER_GET_FOLDER_LISTING_IND\n");
            LOG_DEBUG("FTP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            ftp_rsp_info.body = &body_info;
            ftp_tx_hdrs.ftp_rsp_info = &ftp_rsp_info;

            /* send response */
            send_response = 1U;

            /* Reject if operation reject */
            if (0U != appl_reject_opn)
            {
                tx_response = appl_reject_opn;
                LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);

                break;
            }

            if (FTP_CONTINUE_RSP == event_result)
            {
                tx_response = FTP_CONTINUE_RSP;

                break;
            }

            if (0U == sent)
            {
                ftp_xchg_size = ftp_server_instance[handle].max_xchg_size;
                tx_response = FTP_SUCCESS_RSP;

                /* Create the listing file */
                retval = BT_fops_file_open
                         (
                             (UCHAR *)FOLDERLISTING_FILE,
                             (UCHAR *)"wb",
                             &fp
                         );
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("Failed to Open File to Create folder Listing\n");
                    tx_response = FTP_SERVER_ERROR;
                    break;
                }

                retval = BT_ftp_server_create_xml_dir_listing
                         (
                             ftp_server_instance[i].path,
                             fp
                         );
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("Failed to Create folder Listing file\n");
                    tx_response = FTP_SERVER_ERROR;
                    break;
                }

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);

                /* Get the sample listing from file */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_open ((UCHAR *)FOLDERLISTING_FILE, (UCHAR *)"rb", &fp);
                if (NULL != fp)
                {
                    /* Get the file size */
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_seek(fp, 0U, BT_FOPS_SEEK_SET);

                    remaining = fsize;
                }
                else
                {
                    tx_response = FTP_NOT_FOUND_RSP;
                    break;
                }
            }

            /* Fill body hdr values */
            if (remaining > ftp_xchg_size)
            {
                body_info.length = ftp_xchg_size;
                tx_response = FTP_CONTINUE_RSP;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16 )remaining;
                tx_response = FTP_SUCCESS_RSP;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            if (0U != body_info.length)
            {
                body_info.value = BT_alloc_mem (body_info.length);
                to_free = body_info.value;
            }

            if (NULL != body_info.value)
            {
                /* fread(body_info.value, body_info.length, 1, fp); */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read(body_info.value, body_info.length, fp, &actual);
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
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }
            break;

        case FTP_SERVER_PULL_FILE_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received FTP_SERVER_PULL_FILE_IND\n");
            LOG_DEBUG("FTP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* send response */
            send_response = 1U;

            ftp_tx_hdrs.ftp_rsp_info = &ftp_rsp_info;

            ftp_rsp_info.body = &body_info;

            if (FTP_CONTINUE_RSP == event_result)
            {
                tx_response = FTP_CONTINUE_RSP;

                if (0x01U == appl_name_hdr_in_get)
                {
                    /**
                    * Name Header might have received in earlier
                    * FTP_SERVER_PULL_FILE_IND Events
                    */
                    break;
                }

                /* Check for Name hdr in the request */
                if ((NULL != ftp_rx_hdrs->ftp_req_info->name) ||
                    (NULL != ftp_rx_hdrs->ftp_req_info->name->value) ||
                    (0U != ftp_rx_hdrs->ftp_req_info->name->length))
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        ftp_server_instance[i].path,
                        ftp_rx_hdrs->ftp_req_info->name->value,
                        file_object
                    );

                    ftp_xchg_size = ftp_server_instance[handle].max_xchg_size;

                    /* Open the file to be sent */
                    retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &ftp_tx_fp);
                    if ((API_SUCCESS == retval) && (NULL != ftp_tx_fp))
                    {
                        /* Get the file size */
                        (BT_IGNORE_RETURN_VALUE) BT_fops_file_size (ftp_tx_fp, &fsize);

                        /* Update the object size */
                        ftp_rsp_info.obj_size = fsize;

                        LOG_DEBUG ("Object size: 0x%08X\n", ftp_rsp_info.obj_size);

                        remaining = fsize;
                    }
                    else
                    {
                        LOG_DEBUG ("Failed to open file\n");

                        tx_response = FTP_NOT_FOUND_RSP;

                        break;
                    }

                    appl_name_hdr_in_get = 0x01U;
                }

                /* Wait for FTP_SUCCESS_RSP */
                break;
            }

            if (0U == sent)
            {
                /**
                 * Check if Name Header is already received and processed
                 */
                if (0x01U != appl_name_hdr_in_get)
                {
                    /**
                     * No, Name Header is not received in previous FTP_SERVER_PULL_FILE_IND Events.
                     * Check if received in the current FTP_SERVER_PULL_FILE_IND Event
                     */
                    if((NULL == ftp_rx_hdrs->ftp_req_info->name) ||
                       (NULL == ftp_rx_hdrs->ftp_req_info->name->value) ||
                       (0U == ftp_rx_hdrs->ftp_req_info->name->length))
                    {
                        LOG_DEBUG ("Invalid Object Name.\n");

                        tx_response = FTP_BAD_REQ_RSP;

                        break;
                    }

                    /* Check if operation to be rejected */
                    if (0U != appl_reject_opn)
                    {
                        tx_response = appl_reject_opn;
                        LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);

                        break;
                    }
                    else
                    {
#ifndef OBEX_ASCII_TO_FROM_UNICODE
                        /* Coverting Unicode to ASCII */
                        for (j = 0U; j < (ftp_rx_hdrs->ftp_req_info->name->length / 2U); j++)
                        {
                            file_name[j] = ftp_rx_hdrs->ftp_req_info->name->value[j * 2U + 1U];
                        }

                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_vfops_create_object_name
                        (
                            ftp_server_instance[i].path,
                            file_name,
                            file_object
                        );

                        LOG_DEBUG ("Pull File Name: %s\n", file_name);
#else
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_vfops_create_object_name
                        (
                            ftp_server_instance[i].path,
                            ftp_rx_hdrs->ftp_req_info->name->value,
                            file_object
                        );
#endif /* OBEX_ASCII_TO_FROM_UNICODE */
                    }

                    ftp_xchg_size = ftp_server_instance[handle].max_xchg_size;

                    /* Open the file to be sent */
                    retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &ftp_tx_fp);
                    if ((API_SUCCESS == retval) && (NULL != ftp_tx_fp))
                    {
                        /* Get the file size */
                        (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(ftp_tx_fp, &fsize);

                        /* Update the object size */
                        ftp_rsp_info.obj_size = fsize;

                        LOG_DEBUG ("Object size: 0x%08X\n", ftp_rsp_info.obj_size);

                        remaining = fsize;
                    }
                    else
                    {
                        LOG_DEBUG ("Failed to open file\n");

                        tx_response = FTP_NOT_FOUND_RSP;

                        break;
                    }
                }
            }

            /* Fill body hdr values */
            if (remaining > ftp_xchg_size)
            {
                body_info.length = ftp_xchg_size;
                tx_response = FTP_CONTINUE_RSP;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16)remaining;
                tx_response = FTP_SUCCESS_RSP;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            if (0U != body_info.length)
            {
                body_info.value = BT_alloc_mem (body_info.length);
                to_free = body_info.value;
            }

            if (NULL != body_info.value)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read(body_info.value, body_info.length, ftp_tx_fp, &actual);
            }

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            if (0U == remaining)
            {
                /* Reset the variables */
                sent = 0U;
                fsize = 0U;
                actual = 0U;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(ftp_tx_fp);
                ftp_tx_fp = NULL;

                /* Reset */
                appl_name_hdr_in_get = 0x00U;
            }
            break;

        case FTP_SERVER_ABORT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received FTP_SERVER_ABORT_IND\n");
            LOG_DEBUG("FTP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            if (NULL != ftp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(ftp_rx_fp);
                ftp_rx_fp = NULL;
                ftp_push_started = 0x00U;
                (BT_IGNORE_RETURN_VALUE) BT_fops_object_delete(file_object);
            }

            if (NULL != ftp_tx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(ftp_tx_fp);
                ftp_rx_fp = NULL;
                sent = 0U;
            }
            break;

        case FTP_SERVER_SET_FOLDER_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received FTP_SERVER_SET_FOLDER_IND\n");
            LOG_DEBUG("FTP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* send response */
            send_response = 1U;

            tx_response = FTP_SUCCESS_RSP;

            LOG_DEBUG("\n\nIN Path: %s\n", ftp_server_instance[i].path);
            LOG_DEBUG("IN Depth: %d\n", ftp_server_instance[i].root_depth);

            if (NULL == ftp_rx_hdrs->ftp_req_info)
            {
                tx_response = FTP_BAD_REQ_RSP;
                break;
            }

            /* Based on the setpath_flag, decide the operation */
            switch (ftp_rx_hdrs->ftp_req_info->setpath_flag)
            {
            case 0x00U:
                /* New Folder Creation */
                if ((NULL == ftp_rx_hdrs->ftp_req_info->name) ||
                    (NULL == ftp_rx_hdrs->ftp_req_info->name->value) ||
                    (0U == ftp_rx_hdrs->ftp_req_info->name->length))
                {
                    tx_response = FTP_BAD_REQ_RSP;
                }
                else if (0U != appl_ftp_server_read_only)
                {
                    tx_response = appl_ftp_server_read_only;
                    LOG_DEBUG("Rejected: Reason - 0x%02X (Read Only)\n", tx_response);

                    break;
                }
                else
                {
                    LOG_DEBUG ("New Folder: %s", ftp_rx_hdrs->ftp_req_info->name->value);

                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        ftp_server_instance[i].path,
                        ftp_rx_hdrs->ftp_req_info->name->value,
                        file_object
                    );

                    (BT_IGNORE_RETURN_VALUE) BT_fops_create_folder (file_object);

                    /* Enter the folder after creating */
                    retval = BT_vfops_set_path_forward
                             (
                                 ftp_server_instance[i].path,
                                 ftp_rx_hdrs->ftp_req_info->name->value
                             );
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to set path to folder %s - 0x%04X\n",
                        ftp_rx_hdrs->ftp_req_info->name->value, retval);

                        tx_response = FTP_NOT_FOUND_RSP;
                    }
                    else
                    {
                        ftp_server_instance[i].root_depth++;

                        LOG_DEBUG ("Current PATH: %s\n",
                        ftp_rx_hdrs->ftp_req_info->name->value);
                    }
                }
                break;

            case 0x02U:
                /* Child/Root Folder */
                if ((NULL == ftp_rx_hdrs->ftp_req_info->name) ||
                    (0U == ftp_rx_hdrs->ftp_req_info->name->length) ||
                    ('\0' == ftp_rx_hdrs->ftp_req_info->name->value[0U]))
                {
                    LOG_DEBUG ("Set Path with flag to ROOT FOLDER\n");

                    /* Set path to root folder */
                    if (0U == ftp_server_instance[i].root_depth)
                    {
                        LOG_DEBUG ("Already in ROOT folder\n");
                    }
                    else
                    {
                        while (0U != ftp_server_instance[i].root_depth)
                        {
                            retval = BT_vfops_set_path_backward(ftp_server_instance[i].path);
                            if (API_SUCCESS != retval)
                            {
                                LOG_DEBUG ("Failed to set path backward\n");
                            }
                            ftp_server_instance[i].root_depth --;
                        }

                        LOG_DEBUG ("Folder set to ROOT\n");
                    }
                }
                else if ((NULL != ftp_rx_hdrs->ftp_req_info->name) &&
                         (NULL != ftp_rx_hdrs->ftp_req_info->name->value) &&
                         (0U != ftp_rx_hdrs->ftp_req_info->name->length))
                {
                    LOG_DEBUG ("Set Path with flag to CHILD FOLDER: %s\n",
                    ftp_rx_hdrs->ftp_req_info->name->value);

                    /* Reject if operation reject is set */
                    if (0U != appl_reject_opn)
                    {
                        tx_response = appl_reject_opn;
                        LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);

                        break;
                    }

                    retval = BT_vfops_set_path_forward
                             (
                                 ftp_server_instance[i].path,
                                 ftp_rx_hdrs->ftp_req_info->name->value
                             );
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to set path to folder %s - 0x%04X\n",
                        ftp_rx_hdrs->ftp_req_info->name->value, retval);

                        tx_response = FTP_NOT_FOUND_RSP;
                    }
                    else
                    {
                        ftp_server_instance[i].root_depth++;

                        LOG_DEBUG ("Current PATH: %s\n",
                        ftp_rx_hdrs->ftp_req_info->name->value);
                    }
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }

                break;

            case 0x03U:
                LOG_DEBUG ("Set Path with flag to PARENT FOLDER");

                /* Check if already in ROOT */
                if (0U == ftp_server_instance[i].root_depth)
                {
                    LOG_DEBUG ("\nAlready in ROOT\n");
                    tx_response = FTP_BAD_REQ_RSP;
                    break;
                }

                /* Set path backward */
                retval = BT_vfops_set_path_backward(ftp_server_instance[i].path);
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("\nFailed to set path backward - 0x%04X\n", retval);
                    tx_response = FTP_NOT_FOUND_RSP;
                }
                else
                {
                    ftp_server_instance[i].root_depth--;
                    LOG_DEBUG ("\nCurrent PATH: %s\n",
                    ftp_server_instance[i].path);
                }
                break;

            default:
                /* Should not reach here */
                tx_response = FTP_BAD_REQ_RSP;
                break;
            }

            LOG_DEBUG("\nOUT Path: %s\n", ftp_server_instance[i].path);
            LOG_DEBUG("OUT Depth: %d\n\n", ftp_server_instance[i].root_depth);
            break;

        case FTP_SERVER_ACTION_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received FTP_SERVER_ACTION_IND\n");
            LOG_DEBUG("FTP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            if (0U != appl_reject_action)
            {
                tx_response = appl_reject_action;
                LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);
                send_response = 1U;

                break;
            }

            /* Print ACTION ID */
            LOG_DEBUG ("ACTION-ID : %d\n", ftp_rx_hdrs->ftp_req_info->action_id);

            /* Print SOURCE Name */
            if ((NULL != ftp_rx_hdrs->ftp_req_info->name->value) &&
                (0U != ftp_rx_hdrs->ftp_req_info->name->length))
            {
                LOG_DEBUG ("Source Name: %s\n", ftp_rx_hdrs->ftp_req_info->name->value);
            }

            /* Print DEST Name */
            if ((NULL != ftp_rx_hdrs->ftp_req_info->dest_name->value) &&
                (0U != ftp_rx_hdrs->ftp_req_info->dest_name->length))
            {
                LOG_DEBUG ("Dest Name: %s\n", ftp_rx_hdrs->ftp_req_info->dest_name->value);
            }

            switch (ftp_rx_hdrs->ftp_req_info->action_id)
            {
                UCHAR src[128U], dst[128U];

                case 0U: /* Copy */
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        ftp_server_instance[i].path,
                        ftp_rx_hdrs->ftp_req_info->name->value,
                        src
                    );

                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        ftp_server_instance[i].path,
                        ftp_rx_hdrs->ftp_req_info->dest_name->value,
                        dst
                    );

                    retval = BT_fops_file_copy
                    (
                        src,
                        dst,
                        BT_FALSE
                    );
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to copy file %s - 0x%04X\n",
                        src, retval);

                        tx_response = FTP_NOT_FOUND_RSP;
                    }
                    break;

                case 1U: /* Move */
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        ftp_server_instance[i].path,
                        ftp_rx_hdrs->ftp_req_info->name->value,
                        src
                    );

                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        ftp_server_instance[i].path,
                        ftp_rx_hdrs->ftp_req_info->dest_name->value,
                        dst
                    );

                    retval = BT_fops_file_move
                             (
                                 src,
                                 dst
                             );
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to move file %s - 0x%04X\n",
                        src, retval);

                        tx_response = FTP_NOT_FOUND_RSP;
                    }
                    break;

                case 2U: /* Set Permission */
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        ftp_server_instance[i].path,
                        ftp_rx_hdrs->ftp_req_info->name->value,
                        src
                    );
                    /* Open the Source file */
                    retval = BT_fops_file_open
                             (
                                 (UCHAR *)src,
                                 (UCHAR *)"rb",
                                 &fp
                             );
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to Open Source File\n");
                        tx_response = FTP_NOT_FOUND_RSP;
                        break;
                    }
                    LOG_DEBUG("Permissions: 0x%08lx\n", ftp_rx_hdrs->ftp_req_info->permissions);
                    (BT_IGNORE_RETURN_VALUE) BT_fops_set_file_attributes(src, ftp_rx_hdrs->ftp_req_info->permissions);
                    break;

                default:
                    LOG_DEBUG("Invalid Action Type\n");
                    break;
            }

            /* send response */
            send_response = 1U;
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
                ftp_rsp_info.wait = BT_TRUE;
                ftp_tx_hdrs.ftp_rsp_info = &ftp_rsp_info;

                wait_count--;
            }

            retval = BT_ftp_server_send_response
                     (
                         &handle,
                         event_type,
                         tx_response,
                         &ftp_tx_hdrs
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed to send FTP server Response - 0x%04x\n", retval);
            }
        }

        if (NULL != to_free)
        {
            BT_free_mem(to_free);
        }
    }

    return retval;
}

void ftp_server_print_appl_instances (void)
{
    UINT8 i;

    LOG_DEBUG ("\n\n===========================================================\n");
    LOG_DEBUG ("Instance  FTP Handle\t BD Addr");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < FTP_NUM_SERVER_INSTANCE; i++)
    {
        LOG_DEBUG ("%d\t  ", i);
        LOG_DEBUG ("%02x\t\t ", ftp_server_instance[i].handle);
        LOG_DEBUG ("%02x:%02x:%02x:%02x:%02x:%02x\t ",
        ftp_server_instance[i].bd_addr[0U], ftp_server_instance[i].bd_addr[1U],
        ftp_server_instance[i].bd_addr[2U], ftp_server_instance[i].bd_addr[3U],
        ftp_server_instance[i].bd_addr[4U], ftp_server_instance[i].bd_addr[5U]);

        LOG_DEBUG ("\n");

    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    return;
}

#endif /* FTP */
