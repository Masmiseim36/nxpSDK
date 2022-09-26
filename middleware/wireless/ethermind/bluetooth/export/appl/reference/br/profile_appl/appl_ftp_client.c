
/**
 *  \file appl_ftp_client.c
 *
 *  Source file for FTP Client Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_ftp.h"
#include "appl_utils.h"

#ifdef FTP
/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static UCHAR         ftpc_bd_addr[BT_BD_ADDR_SIZE];

static FTP_INSTANCE  ftp_client_instance[FTP_NUM_CLIENT_INSTANCE];

static UCHAR ftp_attrib_data[FTP_SDP_RECORD_DATA_SIZE];
static UINT16 ftp_attrib_data_len = FTP_SDP_RECORD_DATA_SIZE;
static SDP_HANDLE ftp_sdp_handle;
static UCHAR ftp_server_channel;
static UINT16 ftp_l2cap_psm;

static FTP_CONNECT_STRUCT connect_info;

static BT_fops_file_handle  fp;
static UINT32       fsize;
static UINT32       remaining, sent;
static UINT16       ftp_xchg_size;

static BT_fops_file_handle ftp_rx_fp;
static UCHAR file_name[FTP_FILE_NAME_LEN], file_object[128U];

#ifndef OBEX_ASCII_TO_FROM_UNICODE
/**
 * Application has to perform ASCII to/from Unicode Encode/Decode.
 */
static UCHAR file_name_in_unicode_frmt[FTP_FILE_NAME_LEN * 2U];
#endif /* OBEX_ASCII_TO_FROM_UNICODE */

static UCHAR wait_count;

static const UCHAR ftp_client_menu[] =
" \n \
-------------- FTP Client Menu --------------\n \
\n \
\t 0. Exit \n \
\t 1. Refresh \n \
\n \
\t 2. FTP Client Initialize \n \
\t 3. FTP Client Shutdown \n \
\t 4. FTP Client Start \n \
\t 5. FTP Client Stop \n \
\n \
\t10. Create ACL Connection\n\
\t11. ACL Disconnection\n\
\t12. Do SDP Query \n\
\n \
\t15. Connect to FTP Server \n \
\t16. Disconnect from FTP Server \n \
\t17. Close Transport Connection with FTP Server \n \
\n \
\t20. Push File \n \
\t21. Delete File \n \
\t22. Set Folder \n \
\t23. Get FolderListing \n \
\t24. Get File \n \
\t25. Do Action on File \n\
\t26. Abort \n\
\n \
Your Choice: ";

/* ----------------------------------------- Functions */

void main_ftp_client_operations (void)
{
    API_RESULT retval;
    UCHAR i;

#ifndef OBEX_ASCII_TO_FROM_UNICODE
    UCHAR j;
#endif /* OBEX_ASCII_TO_FROM_UNICODE */

    UINT8 *phandle;
    UINT16 size;
    UINT16 hci_handle;

    FTP_REQUEST_STRUCT req_info;
    FTP_HEADER_STRUCT name_info, body_info;

#ifdef OBEX_ENH_SUPPORT
    FTP_HEADER_STRUCT dest_name_info;
    UCHAR dest_file_name[32U];
#endif /* OBEX_ENH_SUPPORT */

    UINT16 file_name_len;
    UINT16 actual;
    UCHAR more;

    int choice, menu_choice, handle, server_ch, val;

    /* Init */
    retval = API_SUCCESS;
    more = 0U;

    BT_LOOP_FOREVER()
    {
        printf ("%s", ftp_client_menu);
        scanf ("%d", &choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /*  return; */

        case 1:
            break;

        case 2:
            LOG_DEBUG ("Initializing FTP Client...\n");
            retval = BT_ftp_client_init ();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                /* Reset BD_ADDR & FTP handle */
                for (i = 0U; i < FTP_NUM_CLIENT_INSTANCE; i++)
                {
                    FTP_RESET_INSTANCE(&ftp_client_instance[i]);
                }
            }

            ftp_client_print_appl_instances ();
            break;

        case 3:
            LOG_DEBUG ("Shutting down FTP Client...\n");
            retval = BT_ftp_client_shutdown ();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 4:
            ftp_client_print_appl_instances ();

            LOG_DEBUG ("Enter FTP Client instance: ");
            scanf ("%d", &handle);

            if (FTP_NUM_CLIENT_INSTANCE <= handle)
            {
                LOG_DEBUG("Invalid instance, Try Again\n");
                break;
            }

            /* FTP applicaion instance handle */
            phandle = &ftp_client_instance[handle].handle;

            if (FTP_ENTITY_INVALID != *phandle)
            {
                LOG_DEBUG ("Application Instance not free!\n");
                break;
            }

            LOG_DEBUG ("Starting FTP Client...\n");
            retval = BT_ftp_client_start
                     (
                         phandle,
                         appl_ftp_client_callback
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS != retval)
            {
                *phandle = FTP_ENTITY_INVALID;
            }

            ftp_client_print_appl_instances ();
            break;

        case 5:
            ftp_client_print_appl_instances ();

            LOG_DEBUG ("Enter FTP Client instance: ");
            scanf ("%d", &handle);

            if (FTP_NUM_CLIENT_INSTANCE <= handle)
            {
                LOG_DEBUG("Invalid instance, Try Again\n");
                break;
            }

            /* FTP applicaion instance handle */
            phandle = &ftp_client_instance[handle].handle;

            LOG_DEBUG ("Stopping  FTP Client...\n");
            retval = BT_ftp_client_stop (phandle);
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                *phandle = FTP_ENTITY_INVALID;

                BT_mem_set
                (
                    &ftp_client_instance[handle].bd_addr,
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

                if (NULL != ftp_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (ftp_rx_fp);
                    ftp_rx_fp = NULL;
                }
            }

            ftp_client_print_appl_instances ();
            break;

        case 10:
            /*
             * First Application should establish ACL Connection with
             * the peer Device
             */
            LOG_DEBUG("Enter FTP Server's BD Address: ");

            /* Read the BD_ADDR of Remote Device */
            appl_get_bd_addr(ftpc_bd_addr);

            retval = BT_hci_create_connection
                     (
                         ftpc_bd_addr,
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
                         ftpc_bd_addr,
                         &hci_handle
                     );

            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("Disconnecting ACL Connection 0x%04X\n",
                hci_handle);
            }
            else
            {
                LOG_DEBUG ("ACL Connection for FTP Client not found\n");
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
                ftp_sdp_handle,
                ftpc_bd_addr,
                appl_ftp_ce_sdp_callback
            );

            retval = BT_sdp_open(&ftp_sdp_handle);

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("SDP Connect FAILED !! Error Code = 0x%04X\n", retval);
            }

            LOG_DEBUG("Please Wait for SDP Operation to Complete\n");
            break;

        case 15:
            ftp_client_print_appl_instances();

            LOG_DEBUG ("Enter FTP Client instance: ");
            scanf ("%d", &handle);

            if (FTP_NUM_CLIENT_INSTANCE <= handle)
            {
                LOG_DEBUG("Invalid instance, Try Again\n");
                break;
            }

            connect_info.bd_addr = ftpc_bd_addr;

#ifdef OBEX_OVER_L2CAP
            LOG_DEBUG ("Enter peer L2CAP PSM (in Hex): ");
            scanf ("%x", &val);
            connect_info.psm = (UINT16)val;
#endif /* OBEX_OVER_L2CAP */

            LOG_DEBUG ("Enter peer server channel (in Hex): ");
            scanf ("%x", &server_ch);
            connect_info.server_channel = (UINT8)server_ch;

            LOG_DEBUG ("Enter Data Exchange Size: ");
            scanf ("%d", &val);
            size = (UINT16) val;
            connect_info.max_recv_size = size;

            LOG_DEBUG ("Connecting...\n");
            retval = BT_ftp_client_connect
                     (
                         &ftp_client_instance[handle].handle,
                         &connect_info
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 16:
            LOG_DEBUG ("Enter FTP Client instance: ");
            scanf ("%d", &handle);

            if (FTP_NUM_CLIENT_INSTANCE <= handle)
            {
                LOG_DEBUG("Invalid instance, Try Again\n");
                break;
            }

            LOG_DEBUG ("Disconnecting FTP Client Instance %d\n", handle);
            retval = BT_ftp_client_disconnect (&ftp_client_instance[handle].handle);
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 17:
            LOG_DEBUG ("Enter FTP Client instance: ");
            scanf ("%d", &handle);

            if (FTP_NUM_CLIENT_INSTANCE <= handle)
            {
                LOG_DEBUG("Invalid instance, Try Again\n");
                break;
            }

            LOG_DEBUG ("Disconnecting FTP Transport with FTP server\n");
            retval = BT_ftp_client_transport_close (&ftp_client_instance[handle].handle);
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 20:
            ftp_client_print_appl_instances ();

            LOG_DEBUG ("Enter FTP Client instance: ");
            scanf ("%d", &handle);

            if (FTP_NUM_CLIENT_INSTANCE <= handle)
            {
                LOG_DEBUG("Invalid instance, Try Again\n");
                break;
            }

            FTP_INIT_HEADER_STRUCT (name_info);
            FTP_INIT_HEADER_STRUCT (body_info);
            req_info.obj_size = 0U;
            BT_mem_set(file_name, 0, sizeof(file_name));
            BT_mem_set(&req_info, 0, sizeof(FTP_REQUEST_STRUCT));

            LOG_DEBUG ("Enter the object name to be sent: ");
            scanf ("%s", file_name);
            file_name_len = (UINT16)BT_str_n_len (file_name, sizeof(file_name)) + 1U /* NULL Char */;

            if (file_name_len > FTP_FILE_NAME_LEN)
            {
                LOG_DEBUG ("\nMax File Name Length Limit error\n");
                break; /* return; */
            }

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)FTP_ROOT_FOLDER_BASE,
                file_name,
                file_object
            );

#ifndef OBEX_ASCII_TO_FROM_UNICODE
            /**
             * Encode ASCII Text to UNICODE Text
             */
            BT_mem_set(file_name_in_unicode_frmt, 0, sizeof(file_name_in_unicode_frmt));
            j = 0U;
            for (i = 0U; i < file_name_len; i++)
            {
                file_name_in_unicode_frmt[j + 1U] = file_name[i];
                j += 2U;
            }

            /* Fill name hdr values */
            name_info.value = file_name_in_unicode_frmt;
            name_info.length = file_name_len * 2U;
#else
            /* Fill name hdr values */
            name_info.value = file_name;
            name_info.length = file_name_len;
#endif /* OBEX_ASCII_TO_FROM_UNICODE */

            fsize = 0U;
            remaining = 0U;
            sent = 0U;
            ftp_xchg_size = ftp_client_instance[handle].max_xchg_size;

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

            /* Size of the object */
            req_info.obj_size = fsize;

            LOG_DEBUG ("Object Size: 0x%08X\n", req_info.obj_size);

            /* Fill body hdr values */
            if (remaining > ftp_xchg_size)
            {
                body_info.length = ftp_xchg_size;
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
            req_info.name = &name_info;
            req_info.body = &body_info;

            /* MISRA Rule 9.1 */
            actual = 0U;

            LOG_DEBUG ("Requesting to Push file...\n");
            retval = BT_ftp_client_push_file
                     (
                         &ftp_client_instance[handle].handle,
                         &req_info,
                         more,
                         &actual
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

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

            if (NULL != body_info.value)
            {
                BT_free_mem (body_info.value);
                body_info.value = NULL;
            }
            break;

        case 21:
            ftp_client_print_appl_instances ();

            LOG_DEBUG ("Enter FTP Client instance: ");
            scanf ("%d", &handle);

            if (FTP_NUM_CLIENT_INSTANCE <= handle)
            {
                LOG_DEBUG("Invalid instance, Try Again\n");
                break;
            }

            BT_mem_set(&req_info, 0, sizeof(FTP_REQUEST_STRUCT));
            FTP_INIT_HEADER_STRUCT (name_info);

            LOG_DEBUG ("Enter the object name to delete: ");
            scanf ("%s", file_name);
            file_name_len = (UINT16)BT_str_n_len (file_name, sizeof(file_name));

            /* Fill name hdr values */
            name_info.value = file_name;
            name_info.length = (UINT16)(file_name_len + 1U);

            ftp_xchg_size = ftp_client_instance[handle].max_xchg_size;

            /* Update the hdrs in get request */
            req_info.name = &name_info;

            LOG_DEBUG ("Requesting to Delete a file...\n");
            retval = BT_ftp_client_delete_file
                     (
                         &ftp_client_instance[handle].handle,
                         &req_info
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 22:
            ftp_client_print_appl_instances ();

            LOG_DEBUG ("Enter FTP Client instance: ");
            scanf ("%d", &handle);

            if (FTP_NUM_CLIENT_INSTANCE <= handle)
            {
                LOG_DEBUG("Invalid instance, Try Again\n");
                break;
            }

            LOG_DEBUG ("Select Folder to set: \n");
            LOG_DEBUG ("\t0 -> New\n");
            LOG_DEBUG ("\t1 -> Child\n");
            LOG_DEBUG ("\t2 -> Parent\n");
            LOG_DEBUG ("\t3 -> Root\n");
            scanf ("%d", &choice);

            if (choice > 3U)
            {
                LOG_DEBUG ("Invalid Option \n");
                break;
            }

            BT_mem_set(&req_info, 0, sizeof(FTP_REQUEST_STRUCT));
            FTP_INIT_HEADER_STRUCT (name_info);

            /* Get name for child/root/new folder */
            if ((0U == choice) || (1U == choice))
            {
                LOG_DEBUG ("Enter Folder Name: ");
                scanf ("%s", file_name);

                /* Fill name hdr values */
                name_info.value = file_name;
                name_info.length = (UINT16)(BT_str_n_len (file_name, sizeof(file_name)) + 1U);
            }

            ftp_xchg_size = ftp_client_instance[handle].max_xchg_size;

            /* Update the hdrs in get request */
            req_info.name = &name_info;
            req_info.setpath_flag = (UCHAR )choice;

            LOG_DEBUG ("Requesting to Set folder...\n");
            retval = BT_ftp_client_set_folder
                     (
                         &ftp_client_instance[handle].handle,
                         &req_info
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 23:
            ftp_client_print_appl_instances ();

            BT_mem_set(&req_info, 0, sizeof(FTP_REQUEST_STRUCT));

            LOG_DEBUG ("Enter FTP Client instance: ");
            scanf ("%d", &handle);

            if (FTP_NUM_CLIENT_INSTANCE <= handle)
            {
                LOG_DEBUG("Invalid instance, Try Again\n");
                break;
            }

            LOG_DEBUG ("Enter Folder Name len('0' for current)\n");
            scanf ("%d", &choice);

            file_name_len = 0U;
            if (0U != choice)
            {
                LOG_DEBUG ("Enter Folder Name: ");
                scanf ("%s", file_name);
                file_name_len = (UINT16)BT_str_n_len (file_name, sizeof(file_name));
            }

            /* Fill name hdr values */
            name_info.value = file_name;
            name_info.length =  file_name_len + 1U;

            /* Update the hdrs for Getfolderlisting request */
            req_info.name = &name_info;

            LOG_DEBUG ("Requesting to Get FolderListing.....\n");
            retval = BT_ftp_client_get_folder_listing
                     (
                         &ftp_client_instance[handle].handle,
                         &req_info
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 24:
            ftp_client_print_appl_instances ();

            LOG_DEBUG ("Enter FTP Client instance: ");
            scanf ("%d", &handle);

            if (FTP_NUM_CLIENT_INSTANCE <= handle)
            {
                LOG_DEBUG("Invalid instance, Try Again\n");
                break;
            }

            FTP_INIT_HEADER_STRUCT (name_info);
            BT_mem_set(&req_info, 0, sizeof(FTP_REQUEST_STRUCT));

            LOG_DEBUG ("Enter the object name to get: ");
            scanf ("%s", file_name);
            file_name_len = (UINT16)BT_str_n_len (file_name, sizeof(file_name))  + 1U /* NULL Char */;

            if (file_name_len > FTP_FILE_NAME_LEN)
            {
                LOG_DEBUG ("\nMax File Name Length Limit error\n");
                break; /* return; */
            }

#ifndef OBEX_ASCII_TO_FROM_UNICODE
            /**
             * Encode ASCII Text to UNICODE Text
             */
            BT_mem_set (file_name_in_unicode_frmt, 0, sizeof (file_name_in_unicode_frmt));
            j = 0U;
            for (i = 0U; i < file_name_len; i++)
            {
                file_name_in_unicode_frmt[j + 1U] = file_name[i];
                j += 2U;
            }

            /* Fill name hdr values */
            name_info.value = file_name_in_unicode_frmt;
            name_info.length = file_name_len * 2U;
#else
            /* Fill name hdr values */
            name_info.value = file_name;
            name_info.length = file_name_len;
#endif /* OBEX_ASCII_TO_FROM_UNICODE */

            ftp_xchg_size = ftp_client_instance[handle].max_xchg_size;

            /* Update the hdrs for PULL file request */
            req_info.name = &name_info;

            LOG_DEBUG("Enter number of wait stages: ");
            scanf("%d", &choice);
            wait_count = (UCHAR)choice;

            if (0U < wait_count)
            {
                req_info.wait = BT_TRUE;
                wait_count --;
            }
            else
            {
                req_info.wait = BT_FALSE;
            }

            LOG_DEBUG ("Requesting to pull object...\n");
            retval = BT_ftp_client_pull_file
                     (
                         &ftp_client_instance[handle].handle,
                         &req_info
                     );

            /* Open file for dumping Get file request with the respective file name */
            if (API_SUCCESS == retval)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)FTP_ROOT_FOLDER_BASE,
                    file_name,
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &ftp_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == ftp_rx_fp))
                {
                    LOG_DEBUG ("Failed to File Open\n");
                }
            }
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

#ifdef OBEX_ENH_SUPPORT
        case 25:
            ftp_client_print_appl_instances ();

            BT_mem_set(&req_info, 0, sizeof(FTP_REQUEST_STRUCT));

            LOG_DEBUG ("Enter FTP Client instance: ");
            scanf ("%d", &handle);

            if (FTP_NUM_CLIENT_INSTANCE <= handle)
            {
                LOG_DEBUG("Invalid instance, Try Again\n");
                break;
            }

            LOG_DEBUG ("Select Action to Perform: \n");
            LOG_DEBUG ("\t0 -> Copy\n");
            LOG_DEBUG ("\t1 -> Move/Rename\n");
            LOG_DEBUG ("\t2 -> Set Permissions\n");
            scanf ("%d", &choice);

            if (choice > 2U)
            {
                LOG_DEBUG("Invalid Choice...\n");

                break;
            }

            LOG_DEBUG ("Enter Source Name: ");
            scanf ("%s", file_name);

            /* Fill src name hdr values */
            name_info.value = file_name;
            name_info.length = (UINT16)(BT_str_n_len (file_name, sizeof(file_name)) + 1U);

            req_info.action_id = (UCHAR )choice;
            req_info.permissions = 0xFFFFFFFFU; /* Invalide */

            if ((0U == choice) || (1U == choice))
            {
                LOG_DEBUG ("Enter Dest Name: ");
                scanf ("%s", dest_file_name);

                dest_name_info.value = dest_file_name;
                dest_name_info.length = (UINT16)(BT_str_n_len (dest_file_name, sizeof(dest_file_name)) + 1U);
            }
            else
            {
                LOG_DEBUG ("Enter Permission to be set: ");
                scanf ("%d", &choice);
                req_info.permissions = (UINT32)choice;
                LOG_DEBUG ("Permissions need to be set: 0x%08x\n", req_info.permissions);
            }

            req_info.name = &name_info;
            req_info.dest_name = &dest_name_info;

            LOG_DEBUG ("Requesting ACTION on a file...\n");
            retval = BT_ftp_client_manipulate_file
                     (
                         &ftp_client_instance[handle].handle,
                         &req_info
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;
#endif /* OBEX_ENH_SUPPORT */

        case 26:
            ftp_client_print_appl_instances ();

            LOG_DEBUG ("Enter FTP Client instance: ");
            scanf ("%d", &handle);

            if (FTP_NUM_CLIENT_INSTANCE <= handle)
            {
                LOG_DEBUG("Invalid instance, Try Again\n");
                break;
            }

            LOG_DEBUG ("aborting the push operation.. \n");
            retval = BT_ftp_client_abort(&ftp_client_instance[handle].handle);
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

API_RESULT appl_ftp_client_callback
           (
               FTP_HANDLE      * ftp_handle,
               UINT8             event_type,
               UINT16            event_result,
               FTP_HEADERS     * event_header,
               UINT16            event_hdrlen
           )
{
    API_RESULT retval;
    FTP_HANDLE handle;
    FTP_HEADERS *ftp_rx_hdrs;
    UINT16 i;

    UCHAR send_request;

    FTP_HEADERS ftp_tx_hdrs;
    FTP_HEADER_STRUCT body_info;
    FTP_REQUEST_STRUCT req_info;
    UCHAR more;
    UINT16 actual;

    BT_IGNORE_UNUSED_PARAM(event_hdrlen);

    /* Initialize to default */
    handle = *ftp_handle;
    retval = API_SUCCESS;
    ftp_rx_hdrs = event_header;

    BT_mem_set(&ftp_tx_hdrs, 0, sizeof(FTP_HEADERS));
    BT_mem_set(&req_info, 0, sizeof(FTP_REQUEST_STRUCT));
    FTP_INIT_HEADER_STRUCT(body_info);

    send_request = 0x00U;
    more = 0x00U;
    req_info.obj_size = 0U;

    switch(event_type)
    {
    case FTP_CLIENT_TRANSPORT_CLOSE_IND: /* Fall Through */
    case FTP_CLIENT_TRANSPORT_CLOSE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received %s\n", (FTP_CLIENT_TRANSPORT_CLOSE_IND == event_type) ?
        "FTP_CLIENT_TRANSPORT_CLOSE_IND" : "FTP_CLIENT_TRANSPORT_CLOSE_CNF");
        LOG_DEBUG("FTP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        /* Reset bd_addr */
        for (i = 0U; i < FTP_NUM_CLIENT_INSTANCE; i++)
        {
            if (ftp_client_instance[i].handle == handle)
            {
                BT_mem_set (ftp_client_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);
            }
        }

        ftp_client_print_appl_instances ();

        break;

    case FTP_CLIENT_CONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received FTP_CLIENT_CONNECT_CNF\n");
        LOG_DEBUG("FTP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (ftp_rx_hdrs->ftp_connect_info->bd_addr));

        LOG_DEBUG ("MAX Exchange Size: %d\n",
        ftp_rx_hdrs->ftp_connect_info->max_recv_size);

        /* Get the Handle parameters */
        for (i = 0U; i < FTP_NUM_CLIENT_INSTANCE; i++)
        {
            if (ftp_client_instance[i].handle == handle)
            {
                BT_mem_copy
                (
                    ftp_client_instance[i].bd_addr,
                    ftp_rx_hdrs->ftp_connect_info->bd_addr,
                    BT_BD_ADDR_SIZE
                );

                ftp_client_instance[i].max_xchg_size =
                    ftp_rx_hdrs->ftp_connect_info->max_recv_size;

                break;
            }
        }

        ftp_client_print_appl_instances ();
        break;

    case FTP_CLIENT_DISCONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received FTP_CLIENT_DISCONNECT_CNF\n");
        LOG_DEBUG("FTP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        ftp_client_print_appl_instances ();
        break;

    case FTP_CLIENT_PUSH_OBJ_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received FTP_CLIENT_PUSH_OBJ_CNF\n");
        LOG_DEBUG("FTP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (FTP_CONTINUE_RSP != event_result)
        {
            if (FTP_SUCCESS_RSP == event_result)
            {
                LOG_DEBUG("File Push Complete\n");
            }
            else
            {
                LOG_DEBUG("Error in File Push\n");
            }
            break;
        }

        /* Fill body hdr values */
        if (remaining > ftp_xchg_size)
        {
            body_info.length = ftp_xchg_size;
            more = 0x01U;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16) remaining;
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

        ftp_tx_hdrs.ftp_req_info = &req_info;

        send_request = 0x01U;
        break;

    case FTP_CLIENT_DELETE_OBJ_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received FTP_CLIENT_DELETE_OBJ_CNF\n");
        LOG_DEBUG("FTP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        if (FTP_SUCCESS_RSP != event_result)
        {
            LOG_DEBUG("Error in File Deletion\n");
        }
        break;

    case FTP_CLIENT_GET_FOLDER_LISTING_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received FTP_CLIENT_GET_FOLDER_LISTING_CNF\n");
        LOG_DEBUG("FTP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (FTP_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }

        if ((NULL == ftp_rx_hdrs->ftp_rsp_info->body) ||
            (NULL == ftp_rx_hdrs->ftp_rsp_info->body->value) ||
            (0U == ftp_rx_hdrs->ftp_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            UINT16 length;
            UCHAR *data;

            data = ftp_rx_hdrs->ftp_rsp_info->body->value;
            length = ftp_rx_hdrs->ftp_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG ("\n\n ====== FTP Pull Object data ===== \n\n");
            for (i = 0U; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n\n ================================= \n");

            /* Open file for dumping received GetFolderListing request data */
            if (NULL == ftp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)FTP_ROOT_FOLDER_BASE,
                    file_name,
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             (UCHAR *)FOLDERLISTING_FILE,
                             (UCHAR *)"wb",
                             &ftp_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == ftp_rx_fp))
                {
                    LOG_DEBUG ("Failed to open Folder Listing file\n");
                }
            }

            /* Write to file */
            if (NULL != ftp_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, ftp_rx_fp, &actual);
            }

            /* Close the file if final chunk is received */
            if (FTP_SUCCESS_RSP == event_result)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (ftp_rx_fp);
                ftp_rx_fp = NULL;
            }
        }
        break;

    case FTP_CLIENT_PULL_OBJ_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received FTP_CLIENT_PULL_OBJ_CNF\n");
        LOG_DEBUG("FTP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (FTP_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }

        /* Object size */
        if (0U != ftp_rx_hdrs->ftp_rsp_info->obj_size)
        {
            LOG_DEBUG ("Object size: 0x%08X\n", ftp_rx_hdrs->ftp_rsp_info->obj_size);
        }

        if ((NULL == ftp_rx_hdrs->ftp_rsp_info->body) ||
            (NULL == ftp_rx_hdrs->ftp_rsp_info->body->value) ||
            (0U == ftp_rx_hdrs->ftp_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            UINT16 length;
            UCHAR *data;

            data = ftp_rx_hdrs->ftp_rsp_info->body->value;
            length = ftp_rx_hdrs->ftp_rsp_info->body->length;

#if 0
            /* Print received data on console */
            LOG_DEBUG ("\n\n ====== FTP Pull Object data ===== \n\n");
            for (i = 0; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n\n ================================= \n");
#endif /* 0 */

            /* write to file */
            if (NULL != ftp_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, ftp_rx_fp, &actual);
            }

            /* Close the file if final chunk is received */
            if (FTP_SUCCESS_RSP == event_result)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (ftp_rx_fp);
                ftp_rx_fp = NULL;
            }
        }
        break;

    case FTP_CLIENT_ABORT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received FTP_CLIENT_ABORT_CNF\n");
        LOG_DEBUG("FTP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (NULL != fp)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fp);
            fp = NULL;
        }

        if (NULL != ftp_rx_fp)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(ftp_rx_fp);
            ftp_rx_fp = NULL;

            (BT_IGNORE_RETURN_VALUE) BT_fops_object_delete(file_name);
        }
        break;

    case FTP_CLIENT_SET_FOLDER_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received FTP_CLIENT_SET_FOLDER_CNF\n");
        LOG_DEBUG("FTP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        if (FTP_SUCCESS_RSP != event_result)
        {
            LOG_DEBUG("Error in Set Folder Operation\n");
        }
        break;

    case FTP_CLIENT_ACTION_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received FTP_CLIENT_ACTION_CNF\n");
        LOG_DEBUG("FTP Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        if (FTP_SUCCESS_RSP != event_result)
        {
            LOG_DEBUG("Error in File Operation\n");
        }
        break;

    default:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received FTP_EVENT_INVALID\n");
        break;
    }

    if (0U != send_request)
    {
        if (0U < wait_count)
        {
            req_info.wait = BT_TRUE;
            ftp_tx_hdrs.ftp_req_info = &req_info;

            wait_count--;
        }

        retval = BT_ftp_client_send_request
                 (
                     &ftp_client_instance[handle].handle,
                     event_type,
                     event_result,
                     &ftp_tx_hdrs,
                     more,
                     &actual
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG ("FTP Client Send Request Failed - 0x%04X\n", retval);
        }

        if (FTP_CLIENT_PUSH_OBJ_CNF == event_type)
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

void appl_ftp_ce_sdp_callback
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
                           GOEP_L2CAP_PSM_ATTR_ID};

    BT_IGNORE_UNUSED_PARAM(length);

    num_attrib_ids = 2U;
#endif /* OBEX_OVER_L2CAP */

    /*
     * Frame the SDP Search pattern with following UUIDs
     * - FTP_UUID
     * - RFCOMM_UUID
     * - L2CAP_UUID
     * and Attribute ID "PROTOCOL_DESC_LIST"
     * (For getting the RFCOMM Server Channel on which Peer FTP Service
     *  is Listening)
     *
     */
    uuid[0U].uuid_type = UUID_16;
    uuid[1U].uuid_type = UUID_16;
    uuid[2U].uuid_type = UUID_16;
    uuid[0U].uuid_union.uuid_16= OBEXFILETRANSFER_UUID;
    uuid[1U].uuid_union.uuid_16 =L2CAP_UUID;
    uuid[2U].uuid_union.uuid_16 =RFCOMM_UUID;

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
                     &ftp_sdp_handle,
                     uuid,
                     3U,
                     attrib_id,
                     num_attrib_ids,
                     NULL,
                     0U,
                     ftp_attrib_data,
                     &ftp_attrib_data_len
                 );

        if(API_SUCCESS != retval)
        {
            LOG_DEBUG("SDP SEARCH Failed for FTP..Closing SDP\n");
            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&ftp_sdp_handle);
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

                /* Check for OBEX L2CAP PSM for FTP in the Protocol Descriptor List */
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
                        BT_UNPACK_LE_2_BYTE(&ftp_l2cap_psm, attr_val_2B);
                        LOG_DEBUG("GOEP L2CAP PSM: 0x%04X\n", ftp_l2cap_psm);
                    }
                }

                /* if ((API_SUCCESS != retval) && (RFCOMM_PSM == psm)) */
#endif /* OBEX_OVER_L2CAP */
                {
                    /* No PSMs found, check the FTP server channel number */
                    retval = BT_sdp_get_channel_number
                             (
                                 sdp_query_result,
                                 &ftp_server_channel
                             );

                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG("SDP FAILED to find FTP Server Channel\n");
                    }
                    else
                    {
                        LOG_DEBUG("Server Channel: 0x%02X\n", ftp_server_channel);

                        /*
                         * Store the Server Channel globally.
                         * Later it will be used while Creating FTP Connection
                         */
                    }
                }

                /* FTP PSM or Server channel found */
                if (API_SUCCESS == retval)
                {
                    LOG_DEBUG ("FTP Service Found\n");
                }
                else
                {
                    LOG_DEBUG ("Failed to find FTP Service.\n");
                }

                no_server_channels++;
            }
        }

        /* Now Close the SDP Connection */
        (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&ftp_sdp_handle);
        break;

    default: /* Events not needed to be Handled */
        LOG_DEBUG("Unhandled SDP Event (0x%02X)\n", command);
        break;
    }/* switch */

    return;
}


void ftp_client_print_appl_instances (void)
{
    UINT8 i;

    LOG_DEBUG ("\n\n===========================================================\n");
    LOG_DEBUG ("Instance  FTP Handle\t BD Addr");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < FTP_NUM_CLIENT_INSTANCE; i++)
    {
        LOG_DEBUG ("%d\t  ", i);
        LOG_DEBUG ("%02x\t\t ", ftp_client_instance[i].handle);
        LOG_DEBUG ("%02x:%02x:%02x:%02x:%02x:%02x\t ",
        ftp_client_instance[i].bd_addr[0U], ftp_client_instance[i].bd_addr[1U],
        ftp_client_instance[i].bd_addr[2U], ftp_client_instance[i].bd_addr[3U],
        ftp_client_instance[i].bd_addr[4U], ftp_client_instance[i].bd_addr[5U]);

        LOG_DEBUG ("\n");
    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    return;
}
#endif /* FTP */

