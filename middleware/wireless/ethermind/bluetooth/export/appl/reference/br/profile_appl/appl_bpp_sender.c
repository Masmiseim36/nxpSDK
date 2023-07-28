
/**
 *  \file appl_bpp_sender.c
 *
 *  Source file for BPP Sender Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_bpp.h"
#include "appl_utils.h"

#ifdef BPP_SENDER
/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static UCHAR         appl_bpp_printer_bd_addr[BT_BD_ADDR_SIZE];
static UINT16        connection_handle;

static UCHAR bpp_sample_http_accept_header[] = "Accept: text/plain; q=0.5, text/html";
static UCHAR bpp_sample_http_accept_lang_header[] = "Accept-Language: da,en-gb;q=0.8,en;q=0.7";
static UCHAR bpp_sample_http_authorization_header[] = "Authorization: admin Welcome@123";

static BPP_INSTANCE  bpp_sender_jobch_instance[BPP_NUM_SENDER_INSTANCE];
static BPP_INSTANCE  bpp_sender_statusch_instance[BPP_NUM_SENDER_INSTANCE];
static BPP_INSTANCE  bpp_sender_objectch_instance[BPP_NUM_SENDER_INSTANCE];

static UCHAR bpp_attrib_data[BPP_SDP_RECORD_DATA_SIZE];
static UINT16 bpp_attrib_data_len = BPP_SDP_RECORD_DATA_SIZE;
static SDP_HANDLE bpp_sdp_handle;
static UCHAR bpp_job_server_channel;
static UCHAR bpp_status_server_channel;
static BPP_CONNECT_STRUCT connect_info;
static UCHAR        appl_reject_opn;
static BT_fops_file_handle  fp;
static UINT32       fsize;
static UINT32       remaining, sent;

/* To sent Cancel Job while send doc is ongoing */
static UINT32       _fsize;
static UINT32       _remaining, _sent;

static UINT32       bpp_xchg_size;
static BPP_APPL_PARAMS      appl_param;
static BT_fops_file_handle bpp_rx_fp;
static BT_fops_file_handle  bpp_tx_fp;
static UCHAR file_name[32U] , type_header[64U], file_object[128U];
static UCHAR push_file_description[128U];
static UCHAR current_channel;
static UCHAR channel_type;

/* Flag - if a Job is getting cancelled */
static UCHAR appl_bpp_sender_cancel_job_flag;
static UCHAR appl_bpp_sender_cancel_job_instance;
static UCHAR appl_bpp_sender_cancel_job_channel;
static API_RESULT appl_bpp_sender_cancel_job(UCHAR handle, UCHAR channel);
static API_RESULT appl_bpp_sender_cancel_job_ext(UCHAR handle, UCHAR channel);

static UCHAR appl_bpp_sender_print_ongoing;

static UCHAR appl_bpp_sender_send_doc_stop_flag = 0U;
static UCHAR appl_bpp_sender_send_doc_cnf_cnt = 0U;
static UCHAR appl_bpp_sender_send_doc_cnf_max_cnt = 0U;

static const UCHAR bpp_sender_menu[] =
" \n \
-------------- BPP Sender Menu --------------\n \
\n \
\t 0. Exit \n \
\t 1. Refresh \n \
\n \
\t 2. BPP Sender Initialize \n \
\t 3. BPP Sender Shutdown \n \
\t 4. BPP Sender Start \n \
\t 5. BPP Sender Stop \n \
\n \
\t10. Register BD_ADDR of BPP Printer \n\
\t11. Create ACL Connection\n\
\t12. ACL Disconnection\n\
\t13. Do SDP Query \n\
\n \
\t15. Connect to BPP Printer \n \
\t16. Disconnect from BPP Printer \n \
\t17. Close Transport Connection with BPP Printer \n \
\n \
\t20. Simple Push File \n \
\t21. Simple Reference Push File \n \
\n \
\t22. Create Job request \n\
\t23. Get Printer Attributes\n\
\t24. Get Job Attributes\n\
\t25. Cancel Job\n\
\t26. Create Precise job\n\
\t27. Get Margins\n\
\t28. Send Document\n\
\t29. Get Event\n\
\t30. Send Reference\n\
\n \
\t31. Get RUI\n\
\n \
\t50. Enter Server Pincode\n\
\t60. Set flag to Send Cancel Job while document send is ongoing. \n\
\t61. Set the count to send Cancel Job request(Example: 2). \n\
\n \
Your Choice: ";

/* ----------------------------------------- Functions */
#ifdef APPL_ENABLE_AUTOPROCEDURE
/* Callback registered with HCI */
API_RESULT bppc_appl_hci_event_ind_cb
           (
               UCHAR  event_type,
               UCHAR  * event_data,
               UCHAR  event_datalen
           )
{
    API_RESULT retval;
    UINT16     connection_handle, value_2;
    UCHAR      status, value_1, link_type;

    switch (event_type)
    {
        case HCI_CONNECTION_COMPLETE_EVENT:
            /* Status */
            hci_unpack_1_byte_param(&status, event_data);
            event_data += 1U;

            /* Connection Handle */
            hci_unpack_2_byte_param(&connection_handle, event_data);
            event_data += 8U;

            /* Link Type */
            hci_unpack_1_byte_param(&link_type, event_data);

            if (BT_TRUE == bpp_autoconn)
            {
                /* Set the SDP Handle */
                SDP_SET_HANDLE
                (
                   bpp_sdp_handle,
                   bpp_peer_addr,
                   appl_bpp_sender_sdp_callback
                );

                retval = BT_sdp_open(&bpp_sdp_handle);
                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("SDP Open Failed. Reason: 0x%04X\n",
                    retval);
                }
                else
                {
                    LOG_DEBUG("Started SDP Query\n");
                }
            }
            break;

        default:
            LOG_DEBUG("Invalid Event Type: 0x%02X\n", event_type);
            break;
    }

    return API_SUCCESS;
}

void appl_init_bppc(void)
{
    API_RESULT retval;
    UCHAR i;

    LOG_DEBUG ("Initializing BPP sender...\n");
    retval = BT_bpp_sender_init ();
    LOG_DEBUG ("Retval - 0x%04X\n", retval);
    appl_reject_opn = 0U;
    if (API_SUCCESS == retval)
    {
        /* Reset BD_ADDR & BPP handle */
        for (i = 0U; i < BPP_NUM_SENDER_INSTANCE; i++)
        {
            BPP_RESET_INSTANCE(&bpp_sender_jobch_instance[i]);
            BPP_RESET_INSTANCE(&bpp_sender_statusch_instance[i]);
            BPP_RESET_INSTANCE(&bpp_sender_objectch_instance[i]);
        }
    }

    bpp_autoconn = BT_FALSE;

    /* Register callback with HCI */
    retval = appl_hci_register_callback (bppc_appl_hci_event_ind_cb);
    if (retval != API_SUCCESS)
    {
        LOG_DEBUG ("[ERR] Failed to register callback with HCI\n");
    }
}

void appl_start_bppc(void)
{
    API_RESULT retval;
    UCHAR i;

    for (i = 0U; i < BPP_NUM_SENDER_INSTANCE; i++)
    {
        LOG_DEBUG ("Starting BPP sender...\n");
        retval = BT_bpp_sender_start
                 (
                     &bpp_sender_jobch_instance[i].handle,
                     appl_bpp_sender_callback,
                     0U,0U
                 );/*default set for Direct Printing and job channel*/
        LOG_DEBUG ("Retval - 0x%04X\n", retval);
    }
}
#endif /* APPL_ENABLE_AUTOPROCEDURE */

void main_bpp_sender_operations (void)
{
    API_RESULT retval;
    UCHAR i;
    UINT8 *phandle;
    BPP_INSTANCE *bpp_instance;
    UINT16 size;
    BPP_REQUEST_STRUCT req_info;
    BPP_HEADER_STRUCT name_info, body_info,type_header_info,description_info;
    BPP_HEADER_STRUCT http_accept_header,http_accept_lang_header ,http_authorization_header;
    UINT32 val;
    int option;

    UCHAR user_code[20U];
    UCHAR pin_code[16U];
    BPP_HEADER_STRUCT  pin;
    BPP_HEADER_STRUCT  userid;
    BPP_CONNECT_STRUCT  connect_req_send;
    BPP_HEADERS         bpp_req_header_send;

    UINT16 file_name_len;
    UINT16 actual;
    UCHAR more;

    int choice, menu_choice, handle, cn_handle, server_ch, input , service_type;

    retval = API_SUCCESS;
    more = 0U;
    phandle = NULL;

    BT_LOOP_FOREVER()
    {
        printf ("%s", bpp_sender_menu);
        scanf ("%d", &choice);

        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            LOG_DEBUG ("Initializing BPP Sender...\n");
            retval = BT_bpp_sender_init ();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                /* Reset BD_ADDR & BPP handle */
                for (i = 0U; i < BPP_NUM_SENDER_INSTANCE; i++)
                {
                    BPP_RESET_INSTANCE(&bpp_sender_jobch_instance[i]);
                    BPP_RESET_INSTANCE(&bpp_sender_statusch_instance[i]);
                    BPP_RESET_INSTANCE(&bpp_sender_objectch_instance[i]);
                }
            }

            bpp_sender_print_appl_instances(BPP_JOB_CHANNEL);
            bpp_sender_print_appl_instances(BPP_STATUS_CHANNEL);
            break;

        case 3:
            LOG_DEBUG ("Shutting down BPP Sender...\n");
            retval = BT_bpp_sender_shutdown ();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 4:
         /* bpp_sender_print_appl_instances();
            bpp_sender_print_appl_instances(); */

            LOG_DEBUG ("Enter BPP Sender instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            LOG_DEBUG("Enter Service Type \n 0. Direct Printing \n 1. Print by Reference \n 2. RUI \n  ");
            scanf ("%d", &service_type);
            LOG_DEBUG("Enter Channel Type \n 0. Job Channel \n 1. Status Channel \n 2. Object Channel \n");
            scanf ("%d", &input);

            channel_type = (UCHAR )input;

            /* BPP applicaion instance handle */
            if(channel_type == 0U)
            {
                phandle = &bpp_sender_jobch_instance[handle].handle;
            }
            else if(channel_type == 1U)
            {
                phandle = &bpp_sender_statusch_instance[handle].handle;
            }
            else if(channel_type == 2U)
            {
                phandle = &bpp_sender_objectch_instance[handle].handle;
            }
            else
            {
                LOG_DEBUG("\nWrong choice for Channel Type\n");
                break;
            }

            if (BPP_ENTITY_INVALID != *phandle)
            {
                LOG_DEBUG ("Application Instance not free!\n");
                break;
            }
            LOG_DEBUG ("Starting BPP Sender...\n");
            retval = BT_bpp_sender_start
                     (
                         phandle,
                         appl_bpp_sender_callback,
                         (UCHAR)service_type,
                         (UCHAR)channel_type
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS != retval)
            {
                *phandle = BPP_ENTITY_INVALID;
            }
            else
            {
                if(2U == channel_type)
                {
                    /* Set path to root folder */
                    BT_str_copy
                    (
                        bpp_sender_objectch_instance[handle].path,
                        BPP_ROOT_FOLDER_BASE
                    );

                    bpp_sender_objectch_instance[handle].root_depth = 0U;
                }
            }

            bpp_sender_print_appl_instances((UCHAR)channel_type);
            break;

        case 5:
            bpp_sender_print_appl_instances((UCHAR)channel_type);

            LOG_DEBUG ("Enter BPP Sender instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            LOG_DEBUG("Enter Channel Type \n 0. Job Channel \n 1. Status Channel \n 2. Object Channel \n  ");
            scanf ("%d", &choice);

            channel_type = (UCHAR)choice;

            /* BPP applicaion instance handle */
            if(channel_type == 0U)
            {
                phandle = &bpp_sender_jobch_instance[handle].handle;
            }
            else if(channel_type == 1U)
            {
                phandle = &bpp_sender_statusch_instance[handle].handle;
            }
            else if(channel_type == 2U)
            {
                phandle = &bpp_sender_objectch_instance[handle].handle;
            }
            else
            {
                LOG_DEBUG("\nWrong choice for Channel Type\n");
                break;
            }

            LOG_DEBUG ("Stopping  BPP Sender...\n");
            retval = BT_bpp_sender_stop
                     (
                         phandle,
                         (UCHAR)channel_type
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                *phandle = BPP_ENTITY_INVALID;

                if(0U == channel_type)
                {
                    BT_mem_set
                    (
                        bpp_sender_jobch_instance[handle].bd_addr,
                        0x00,
                        BT_BD_ADDR_SIZE
                    );
                }
                else if(1U == channel_type)
                {
                    BT_mem_set
                    (
                        bpp_sender_statusch_instance[handle].bd_addr,
                        0x00,
                        BT_BD_ADDR_SIZE
                    );
                }
                else if(2U == channel_type)
                {
                    BT_mem_set
                    (
                        bpp_sender_objectch_instance[handle].bd_addr,
                        0x00,
                        BT_BD_ADDR_SIZE
                    );
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }

                /* Close the files if opened */
                if (NULL != fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (fp);
                    fp = NULL;
                }

                if (NULL != bpp_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (bpp_rx_fp);
                    bpp_rx_fp = NULL;
                }
            }

            bpp_sender_print_appl_instances ((UCHAR)channel_type);
            break;

        case 10: /* Read the BD_ADDR of Remote Device */
            LOG_DEBUG("Please enter BD ADDR of BPP Printer\n");
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(appl_bpp_printer_bd_addr);
            break;

        case 11:
            /*
             * First Application should establish ACL Connection with
             * the peer Device
             */
            retval = BT_hci_create_connection
                     (
                         appl_bpp_printer_bd_addr,
                         LMP_ACL_DH5,
                         0x0U, 0x0U, 0x0U, 0x1U
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("HCI Connect Attempt FAILED !! Error Code = 0x%04X\n",
                        retval);
            }
            break;

        case 12:
            /* To disconnect read the ACL Connection Handle */
            LOG_DEBUG("Enter Connection Handle (in Hex): "); fflush(stdout);
            scanf("%x", &cn_handle);
            connection_handle = (UINT16)cn_handle;

            LOG_DEBUG("\n");
            LOG_DEBUG("Starting HCI Disconnection ... "); fflush(stdout);

            /* Mention Reason for Disconnect as User Initiated Disconnection */
            retval = BT_hci_disconnect (connection_handle, 0x13U);
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("HCI Disconnection FAILED !! Error Code = 0x%04X\n",
                retval);
            }
            break;

        case 13:
            LOG_DEBUG ("Select BPP Entity Instance: ");
            scanf ("%d", &handle);

            LOG_DEBUG("Enter Channel Type \n 0. Job Channel \n 1. Status Channel \n   ");
            scanf ("%d", &choice);

            channel_type = (UCHAR)choice;
            SDP_SET_HANDLE
            (
                bpp_sdp_handle,
                appl_bpp_printer_bd_addr,
                appl_bpp_sender_sdp_callback
            );

            retval = BT_sdp_open(&bpp_sdp_handle);

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("SDP Connect FAILED !! Error Code = 0x%04X\n", retval);
            }

            LOG_DEBUG("Please Wait for SDP Operation to Complete\n");
            break;

        case 15:
            bpp_sender_print_appl_instances(BPP_STATUS_CHANNEL);
            bpp_sender_print_appl_instances(BPP_JOB_CHANNEL);
            LOG_DEBUG ("Enter BPP Sender instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            LOG_DEBUG("Enter Channel Type \n 0. Job Channel \n 1. Status Channel \n   ");
            scanf ("%d", &choice);

            channel_type = (UCHAR)choice;
            /* BPP applicaion instance handle */
            if(channel_type == 0U)
            {
                phandle = &bpp_sender_jobch_instance[handle].handle;
                server_ch = bpp_job_server_channel;
            }
            else if(channel_type == 1U)
            {
                phandle = &bpp_sender_statusch_instance[handle].handle;
                server_ch = bpp_status_server_channel;
            }
            else
            {
                LOG_DEBUG("\nWrong Choice for Channel Type\n");
                break;
            }

            connect_info.bd_addr = appl_bpp_printer_bd_addr;
            connect_info.server_channel = (UINT8 )server_ch;

            LOG_DEBUG ("Enter Data Exchange Size: ");
            scanf ("%d", &val);
            size = (UINT16) val;
            connect_info.max_recv_size = size;

            LOG_DEBUG ("Connecting...\n");
            retval = BT_bpp_sender_connect
                     (
                         phandle/*&bpp_sender_jobch_instance[handle].handle*/,
                         &connect_info,
                         (UCHAR)channel_type
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 16:
            LOG_DEBUG ("Enter BPP Client instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            LOG_DEBUG("Enter Channel Type \n 0. Job Channel \n 1. Status Channel \n   ");
            scanf ("%d", &choice);

            channel_type = (UCHAR)choice;
            /* BPP applicaion instance handle */
            if(channel_type == 0U)
            {
                phandle = &bpp_sender_jobch_instance[handle].handle;
            }
            else if(channel_type == 1U)
            {
                phandle = &bpp_sender_statusch_instance[handle].handle;
            }
            else
            {
                LOG_DEBUG("\nWrong Choice for Channel Type\n");
                break;
            }

            LOG_DEBUG ("Disconnecting BPP Client Instance %d\n", handle);

            retval = BT_bpp_sender_disconnect
                     (
                         phandle,
                         (UCHAR)channel_type
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 17:
            LOG_DEBUG ("Enter BPP Sender instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            LOG_DEBUG("Enter Channel Type \n 0. Job Channel \n 1. Status Channel \n 2. Object Channel \n ");
            scanf ("%d", &choice);

            channel_type = (UCHAR)choice;
            /* BPP applicaion instance handle */

            if(channel_type == 0U)
            {
                phandle = &bpp_sender_jobch_instance[handle].handle;
            }
            else if(channel_type == 1U)
            {
                phandle = &bpp_sender_statusch_instance[handle].handle;
            }
            else if(channel_type == 2U)
            {
                phandle = &bpp_sender_objectch_instance[handle].handle;
            }
            else
            {
                LOG_DEBUG("\nWrong Choice for Channel Type\n");
                break;
            }

            LOG_DEBUG ("Disconnecting BPP Transport with BPP sender\n");
            retval = BT_bpp_sender_transport_close
                     (
                         phandle,
                         (UCHAR)channel_type
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 20:
            bpp_sender_print_appl_instances (BPP_JOB_CHANNEL);

            LOG_DEBUG("Enter BPP Sender instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            current_channel = BPP_JOB_CHANNEL;

            BPP_INIT_HEADER_STRUCT (name_info);
            BPP_INIT_HEADER_STRUCT (body_info);
            BPP_INIT_HEADER_STRUCT (type_header_info);
            BPP_INIT_HEADER_STRUCT (description_info);

            BT_mem_set(&req_info, 0, sizeof(req_info));

#if 1
            LOG_DEBUG("Select Type of file to be sent: \n");
            LOG_DEBUG("1 -> image/jpeg i.e. Image\n");
            LOG_DEBUG("2 -> text/x-vcard:2.1 i.e. vcard\n");
            LOG_DEBUG("3 -> application/vnd.pwg-xhtml-print+xml:0.95\n");
            scanf("%d", &option);

            if (1U == option)
            {
                BT_str_copy(type_header, BPP_TYPE_MIME_MEDIA_JPEG);
            }
            else if (2U == option)
            {
                BT_str_copy(type_header, BPP_TYPE_MIME_MEDIA_VCARD);
            }
            else if (3U == option)
            {
                BT_str_copy(type_header, BPP_TYPE_MIME_MEDIA_XHTML_PRINT);
            }
            else
            {
                LOG_DEBUG("Invalid options\n");
                break;
            }

            type_header_info.value = type_header;
            type_header_info.length = (UINT16)(BT_str_n_len(type_header, sizeof(type_header)) + 1U);
#else
            req_info.put_req_type = BPP_PUT_REQ_TYPE_MIME_MEDIA_DOC;
#endif /* 0 */

            LOG_DEBUG ("Enter the object name to be sent: ");
            scanf ("%s", file_name);
            file_name_len = (UINT16 )BT_str_n_len (file_name, sizeof(file_name));

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BPP_ROOT_FOLDER_BASE,
                file_name,
                file_object
            );

            /* Fill name hdr values */
            name_info.value = file_name;
            name_info.length = (UINT16 )(file_name_len + 1U);

            LOG_DEBUG("Want to send Object description(0/1)?");
            scanf("%d", &input);
            if (0U != input)
            {
                LOG_DEBUG("Enter the object description: ");
                scanf("%s", push_file_description);
                description_info.value = push_file_description;
                description_info.length = (UINT16)(BT_str_n_len(push_file_description, sizeof(push_file_description)) + 1U);
            }

            fsize = 0U;
            remaining = 0U;
            sent = 0U;
            bpp_xchg_size = bpp_sender_jobch_instance[handle].max_xchg_size;

            /* Open the file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            if ((API_SUCCESS == retval) && (NULL != fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);
                remaining = fsize;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
                retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");
                break;
            }

            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16 )bpp_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16 )remaining;
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
            req_info.type_header = &type_header_info;
            req_info.description = &description_info;

            actual = 0;
            LOG_DEBUG ("Requesting to Push file...\n");
            appl_bpp_sender_print_ongoing = BT_TRUE;
            retval = BT_bpp_sender_simple_push
                     (
                         &bpp_sender_jobch_instance[handle].handle,
                         &req_info,
                         more,
                         &actual
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("read length = %d, actual sent = %d\n", body_info.length, actual);
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(fp, sent, SEEK_SET);
            }

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
        {
            UCHAR put_req_type;

            bpp_sender_print_appl_instances(BPP_JOB_CHANNEL);
            LOG_DEBUG("Enter BPP Sender instance: ");
            scanf("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            current_channel = BPP_JOB_CHANNEL;

            BPP_INIT_HEADER_STRUCT(name_info);
            BPP_INIT_HEADER_STRUCT(body_info);
            BPP_INIT_HEADER_STRUCT(type_header_info);
            BPP_INIT_HEADER_STRUCT(description_info);
            BT_mem_set(&req_info, 0, sizeof(req_info));

#if 0
            LOG_DEBUG("Enter the Type of file to be sent: ");
            scanf("%s", type_header);
            /*Fill type hdr values*/
            type_header_info.value = type_header;
            type_header_info.length = (UINT16)(BT_str_len(type_header) + 1);
#else
            put_req_type = 0U;
            LOG_DEBUG("Enter the BPP Header Type:\n");
            LOG_DEBUG("1 -> text/x-ref-simple\n");
            LOG_DEBUG("2 -> text/x-ref-xml\n");
            LOG_DEBUG("3 -> text/x-ref-list\n");
            scanf("%d", &option);

            if (1U == option)
            {
                put_req_type = BPP_PUT_REQ_TYPE_REF_SIMPLE;
                BT_str_copy(type_header, BPP_TYPE_REF_SIMPLE);
            }
            else if (2U == option)
            {
                put_req_type = BPP_PUT_REQ_TYPE_REF_XML;
                BT_str_copy(type_header, BPP_TYPE_REF_XML);
            }
            else if (3U == option)
            {
                put_req_type = BPP_PUT_REQ_TYPE_REF_LIST;
                BT_str_copy(type_header, BPP_TYPE_REF_LIST);
            }
            else
            {
                LOG_DEBUG("Invalid Choice\n");
                break;
            }

            /*Fill type hdr values*/
            type_header_info.value = type_header;
            type_header_info.length = (UINT16)(BT_str_n_len(type_header, sizeof(type_header)) + 1U);

#endif /* 0 */

            LOG_DEBUG("Enter 1 to send http header:\n");
            scanf("%d", &option);
            if (1U == option)
            {
                BPP_INIT_HEADER_STRUCT(http_authorization_header);
                http_authorization_header.value = bpp_sample_http_authorization_header;
                http_authorization_header.length = (UINT16)(BT_str_len(bpp_sample_http_authorization_header) + 1U);
            }
            else
            {
                BPP_INIT_HEADER_STRUCT(http_authorization_header);
                LOG_DEBUG("Not sending Http Header\n");
            }

            fsize = 0U;
            remaining = 0U;
            sent = 0U;
            bpp_xchg_size = bpp_sender_jobch_instance[handle].max_xchg_size;

            if (BPP_PUT_REQ_TYPE_REF_SIMPLE == put_req_type)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BPP_ROOT_FOLDER_BASE,
                    (UCHAR *)"bpp_send_ref_simple_req.txt",
                    file_object
                );
            }
            else if (BPP_PUT_REQ_TYPE_REF_XML == put_req_type)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BPP_ROOT_FOLDER_BASE,
                    (UCHAR *)"sample_ref_push_file.xml",
                    file_object
                );
            }
            else
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BPP_ROOT_FOLDER_BASE,
                    (UCHAR *)"bpp_send_ref_list.txt",
                    file_object
                );
            }

            /* Open the file to be sent */
            retval = BT_fops_file_open(file_object, (UCHAR *)"rb", &fp);
            if ((API_SUCCESS == retval) && (NULL != fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);

                remaining = fsize;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
                retval = BT_fops_file_open(file_object, (UCHAR *)"rb", &fp);
            }
            else
            {
                LOG_DEBUG("Failed to open file\n");
                break;
            }

            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16)bpp_xchg_size;
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
                body_info.value = BT_alloc_mem(body_info.length);
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
            req_info.type_header = &type_header_info;
            req_info.description = &description_info;
            req_info.http_authorization_header = &http_authorization_header;

            LOG_DEBUG ("Requesting to Push file...\n");
            retval = BT_bpp_sender_simple_ref_push
                     (
                         &bpp_sender_jobch_instance[handle].handle,
                         &req_info,
                         more,
                         &actual
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("read length = %d, actual sent = %d\n", body_info.length, actual);
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(fp, sent, SEEK_SET);
            }

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
                BT_free_mem(body_info.value);
                body_info.value = NULL;
            }
        }
            break;

        case 22:
            bpp_sender_print_appl_instances (BPP_JOB_CHANNEL);
            LOG_DEBUG ("Enter BPP Sender instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            current_channel = BPP_JOB_CHANNEL;

            BPP_INIT_HEADER_STRUCT (body_info);
            BT_mem_set(&req_info, 0, sizeof(req_info));

            fsize = 0U;
            remaining = 0U;
            sent = 0U;
            bpp_xchg_size = bpp_sender_jobch_instance[handle].max_xchg_size;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BPP_ROOT_FOLDER_BASE,
                (UCHAR *)"create_job_request.txt",
                file_object
            );

            /* Open the file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            if ((API_SUCCESS == retval) && (NULL != fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);

                remaining = fsize;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
                retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");
                break;
            }

            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16 )bpp_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16 )remaining;
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
            req_info.type_header = &type_header_info;
            req_info.description = &description_info;

            LOG_DEBUG ("Requesting to create job...\n");
            retval = BT_bpp_sender_create_job
                     (
                         &bpp_sender_jobch_instance[handle].handle,
                         &req_info,
                         more,
                         &actual
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("read length = %d, actual sent = %d\n", body_info.length, actual);
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(fp, sent, SEEK_SET);
            }

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
            bpp_sender_print_appl_instances (BPP_JOB_CHANNEL);
            /*current_channel = BPP_JOB_CHANNEL;*/
            LOG_DEBUG ("Enter BPP Sender instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            BPP_INIT_HEADER_STRUCT (body_info);
            BT_mem_set(&req_info, 0, sizeof(req_info));

            LOG_DEBUG("Send Request over \n 0. Job channel \n 1. status channel");
            scanf("%d",&choice);

            current_channel = (UCHAR)choice;
            if(current_channel == 0U)
            {
                phandle = &bpp_sender_jobch_instance[handle].handle;
                bpp_xchg_size = bpp_sender_jobch_instance[handle].max_xchg_size;
            }
            else if(current_channel == 1U)
            {
                phandle = &bpp_sender_statusch_instance[handle].handle;
                bpp_xchg_size = bpp_sender_statusch_instance[handle].max_xchg_size;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            fsize = 0U;
            remaining = 0U;
            sent = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BPP_ROOT_FOLDER_BASE,
                (UCHAR *)"get_prt_attribs_request.txt",
                file_object
            );

            /* Open the file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            if ((API_SUCCESS == retval) && (NULL != fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);

                remaining = fsize;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
                retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");
                break;
            }

            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16 )bpp_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16 )remaining;
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
            req_info.body = &body_info;

            LOG_DEBUG ("Requesting to get printer attributes...\n");
            retval = BT_bpp_sender_get_printer_attrs
                     (
                         phandle,
                         &req_info,
                         current_channel,
                         more,
                         &actual
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("read length = %d, actual sent = %d\n", body_info.length, actual);
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(fp, sent, SEEK_SET);
            }

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

        case 24:
            bpp_sender_print_appl_instances (BPP_JOB_CHANNEL);
            /*current_channel = BPP_JOB_CHANNEL;*/
            LOG_DEBUG ("Enter BPP Sender instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            BPP_INIT_HEADER_STRUCT (body_info);
            BT_mem_set(&req_info, 0, sizeof(req_info));

            LOG_DEBUG("Send Request over \n 0. Job channel \n 1. status channel");
            scanf("%d",&choice);

            current_channel = (UCHAR)choice;
            if(current_channel == 0U)
            {
                phandle = &bpp_sender_jobch_instance[handle].handle;
                bpp_xchg_size = bpp_sender_jobch_instance[handle].max_xchg_size;
            }
            else if(current_channel == 1U)
            {
                phandle = &bpp_sender_statusch_instance[handle].handle;
                bpp_xchg_size = bpp_sender_statusch_instance[handle].max_xchg_size;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            fsize = 0U;
            remaining = 0U;
            sent = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BPP_ROOT_FOLDER_BASE,
                (UCHAR *)"get_job_attribs_request.txt",
                file_object
            );

            /* Open the file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            if ((API_SUCCESS == retval) && (NULL != fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);

                remaining = fsize;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
                retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");
                break;
            }

            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16 )bpp_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16 )remaining;
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
            req_info.body = &body_info;

            LOG_DEBUG ("Requesting to get job attributes...\n");
            retval = BT_bpp_sender_get_job_attrs
                     (
                         phandle,
                         &req_info,
                         current_channel,
                         more,
                         &actual
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("read length = %d, actual sent = %d\n", body_info.length, actual);
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(fp, sent, SEEK_SET);
            }

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

        case 25:
            bpp_sender_print_appl_instances (BPP_JOB_CHANNEL);
            /* current_channel = BPP_JOB_CHANNEL; */
            LOG_DEBUG ("Enter BPP Sender instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            LOG_DEBUG("Send Request over \n 0. Job channel \n 1. status channel");
            scanf("%d", &choice);
            current_channel = (UCHAR)choice;

            if (BT_TRUE == appl_bpp_sender_print_ongoing)
            {
                appl_bpp_sender_cancel_job_instance = (UCHAR)handle;
                appl_bpp_sender_cancel_job_channel = current_channel;
                appl_bpp_sender_cancel_job_flag = BT_TRUE;
            }
            else
            {
                retval = appl_bpp_sender_cancel_job((UCHAR)handle, current_channel);

                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG("Failed to initiate Cancel Job operation. Reason: 0x%04X\n", retval);
                }
            }
            break;

        case 26:
            bpp_sender_print_appl_instances (BPP_JOB_CHANNEL);
            LOG_DEBUG ("Enter BPP Sender instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            current_channel = BPP_JOB_CHANNEL;

            BPP_INIT_HEADER_STRUCT (body_info);
            BT_mem_set(&req_info, 0, sizeof(req_info));

            fsize = 0U;
            remaining = 0U;
            sent = 0U;
            bpp_xchg_size = bpp_sender_jobch_instance[handle].max_xchg_size;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BPP_ROOT_FOLDER_BASE,
                (UCHAR *)"precise_job_request.txt",
                file_object
            );

            /* Open the file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            if ((API_SUCCESS == retval) && (NULL != fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);

                remaining = fsize;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
                retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");
                break;
            }

            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16 )bpp_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16 )remaining;
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
            req_info.type_header = &type_header_info;
            req_info.description = &description_info;

            LOG_DEBUG ("Requesting to create Precise job...\n");
            retval = BT_bpp_sender_create_precise_job
                     (
                         &bpp_sender_jobch_instance[handle].handle,
                         &req_info,
                         more,
                         &actual
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("read length = %d, actual sent = %d\n", body_info.length, actual);
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(fp, sent, SEEK_SET);
            }

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

        case 27:
            bpp_sender_print_appl_instances (BPP_JOB_CHANNEL);
            LOG_DEBUG ("Enter BPP Sender instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            current_channel = BPP_JOB_CHANNEL;

            BPP_INIT_HEADER_STRUCT (body_info);
            BT_mem_set(&req_info, 0, sizeof(req_info));

            fsize = 0U;
            remaining = 0U;
            sent = 0U;
            bpp_xchg_size = bpp_sender_jobch_instance[handle].max_xchg_size;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BPP_ROOT_FOLDER_BASE,
                (UCHAR *)"get_margins_request.txt",
                file_object
            );

            /* Open the file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            if ((API_SUCCESS == retval) && (NULL != fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);

                remaining = fsize;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
                retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");
                break;
            }

            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16 )bpp_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16 )remaining;
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
            req_info.type_header = &type_header_info;
            req_info.description = &description_info;

            LOG_DEBUG ("Requesting to Get Margins...\n");
            retval = BT_bpp_sender_get_margins
                     (
                         &bpp_sender_jobch_instance[handle].handle,
                         &req_info,
                         more,
                         &actual
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("read length = %d, actual sent = %d\n", body_info.length, actual);
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(fp, sent, SEEK_SET);
            }

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

        case 28:
        {
            bpp_sender_print_appl_instances(BPP_JOB_CHANNEL);
            LOG_DEBUG("Enter BPP  Sender instance: ");
            scanf("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            current_channel = BPP_JOB_CHANNEL;

            BPP_INIT_HEADER_STRUCT(name_info);
            BPP_INIT_HEADER_STRUCT(body_info);
            BPP_INIT_HEADER_STRUCT(type_header_info);
            BPP_INIT_HEADER_STRUCT(description_info);
            BPP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
            BT_mem_set(&req_info, 0, sizeof(req_info));

            LOG_DEBUG("Enter the JOB ID: ");
            scanf("%d", &val);
            BPP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BPP_FLAG_JOB_ID
            );

            appl_param.job_id = (UINT32)val;

            LOG_DEBUG("Enter the Type of file to be sent: \n");
            LOG_DEBUG("1 -> image/jpeg i.e. Image\n");
            LOG_DEBUG("2 -> text/x-vcard:2.1 i.e. vcard\n");
            LOG_DEBUG("3 -> application/vnd.pwg-xhtml-print+xml:0.95\n");
            LOG_DEBUG("4 -> text/plain\n");
            LOG_DEBUG("5 -> text/x-vcalendar:1.0\n");
            LOG_DEBUG("6 -> text/x-vmessage:1.1\n");

            scanf("%d", &option);

            if (1U == option)
            {
                BT_str_copy(type_header, BPP_TYPE_MIME_MEDIA_JPEG);
            }
            else if (2U == option)
            {
                BT_str_copy(type_header, BPP_TYPE_MIME_MEDIA_VCARD);
            }
            else if (3U == option)
            {
                BT_str_copy(type_header, BPP_TYPE_MIME_MEDIA_XHTML_PRINT);
            }
            else if (4U == option)
            {
                BT_str_copy(type_header, BPP_TYPE_MIME_MEDIA_BASIC_TEXT);
            }
            else if (5U == option)
            {
                BT_str_copy(type_header, BPP_TYPE_MIME_MEDIA_VCALENDAR);
            }
            else if (6U == option)
            {
                BT_str_copy(type_header, BPP_TYPE_MIME_MEDIA_VMESSAGE);
            }
            else
            {
                LOG_DEBUG("Invalid options\n");
                break;
            }

            /* scanf ("%s", type_header); */

            /* Fill type hdr values */
            type_header_info.value = type_header;
            type_header_info.length = (UINT16)(BT_str_n_len(type_header, sizeof(type_header)) + 1U);

            LOG_DEBUG("Enter the object name to be sent: ");
            scanf("%s", file_name);
            file_name_len = (UINT16)BT_str_n_len(file_name, sizeof(file_name));

            /* Fill name hdr values */
            name_info.value = file_name;
            name_info.length = (UINT16)(file_name_len + 1U);

            fsize = 0U;
            remaining = 0U;
            sent = 0U;
            bpp_xchg_size = bpp_sender_jobch_instance[handle].max_xchg_size;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BPP_ROOT_FOLDER_BASE,
                (UCHAR *)file_name,
                file_object
            );

            /* Open the file to be sent */
            retval = BT_fops_file_open(file_object, (UCHAR *)"rb", &bpp_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != bpp_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bpp_tx_fp, &fsize);

                remaining = fsize;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bpp_tx_fp);
                bpp_tx_fp = NULL;
                retval = BT_fops_file_open(file_object, (UCHAR *)"rb", &bpp_tx_fp);
            }
            else
            {
                LOG_DEBUG("Failed to open file\n");
                break;
            }

            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16)bpp_xchg_size;
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
                body_info.value = BT_alloc_mem(body_info.length);
            }

            if (NULL != body_info.value)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                (
                    body_info.value,
                    body_info.length,
                    bpp_tx_fp,
                    &actual
                );
            }

            /* Update the hdrs  for put request */
            req_info.name = &name_info;
            req_info.body = &body_info;
            req_info.type_header = &type_header_info;
            req_info.description = &description_info;
            req_info.appl_params = &appl_param;

            appl_bpp_sender_send_doc_cnf_cnt = 0U;

            LOG_DEBUG("Requesting to Send Document...\n");
            retval = BT_bpp_sender_send_document
                     (
                         &bpp_sender_jobch_instance[handle].handle,
                         &req_info,
                         more,
                         &actual
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("read length = %d, actual sent = %d\n", body_info.length, actual);
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(bpp_tx_fp, sent, SEEK_SET);
            }

            /* If operation has failed or completed, perform cleanup */
            if ((API_SUCCESS != retval) || (0U == remaining))
            {
                /* Reset the variables */
                sent = 0U;
                fsize = 0U;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bpp_tx_fp);
                bpp_tx_fp = NULL;
            }

            if (NULL != body_info.value)
            {
                BT_free_mem(body_info.value);
                body_info.value = NULL;
            }
        }
            break;

        case 29:
            bpp_sender_print_appl_instances (BPP_STATUS_CHANNEL);
            LOG_DEBUG ("Enter BPP Sender instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            current_channel = BPP_STATUS_CHANNEL;

            BPP_INIT_HEADER_STRUCT (body_info);
            BT_mem_set(&req_info, 0, sizeof(req_info));

            fsize = 0U;
            remaining = 0U;
            sent = 0U;
            bpp_xchg_size = bpp_sender_jobch_instance[handle].max_xchg_size;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BPP_ROOT_FOLDER_BASE,
                (UCHAR *)"get_event_request.txt",
                file_object
            );

            /* Open the file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            if ((API_SUCCESS == retval) && (NULL != fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);

                remaining = fsize;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
                retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");
                break;
            }

            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16 )bpp_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16 )remaining;
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
            req_info.body = &body_info;

            LOG_DEBUG ("Requesting to Get Event...\n");
            retval = BT_bpp_sender_get_event
                     (
                         &bpp_sender_statusch_instance[handle].handle,
                         &req_info,
                         more,
                         &actual
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("read length = %d, actual sent = %d\n", body_info.length, actual);
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(fp, sent, SEEK_SET);
            }

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

        case 30:
        {
            UCHAR put_req_type;

            bpp_sender_print_appl_instances(BPP_JOB_CHANNEL);
            LOG_DEBUG("Enter BPP Sender instance: ");
            scanf("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            current_channel = BPP_JOB_CHANNEL;

            BPP_INIT_HEADER_STRUCT(name_info);
            BPP_INIT_HEADER_STRUCT(body_info);
            BPP_INIT_HEADER_STRUCT(type_header_info);
            BPP_INIT_HEADER_STRUCT(description_info);
            BPP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

            BT_mem_set(&req_info, 0, sizeof(req_info));

            LOG_DEBUG("Enter the JOB ID: ");
            scanf("%d", &val);
            BPP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BPP_FLAG_JOB_ID
            );

            appl_param.job_id = (UINT32)val;

#if 0
            LOG_DEBUG("Enter the Type of file to be sent: ");
            scanf("%s", type_header);
            /*Fill type hdr values*/
            type_header_info.value = type_header;
            type_header_info.length = (UINT16)(BT_str_len(type_header) + 1);
#endif /* 0 */

            put_req_type = 0U;
            LOG_DEBUG("Enter the BPP Header Type:\n");
            LOG_DEBUG("1 -> text/x-ref-simple\n");
            LOG_DEBUG("2 -> text/x-ref-xml\n");
            LOG_DEBUG("3 -> text/x-ref-list\n");
            scanf("%d", &option);

            if (1U == option)
            {
                put_req_type = BPP_PUT_REQ_TYPE_REF_SIMPLE;
                BT_str_copy(type_header, BPP_TYPE_REF_SIMPLE);
            }
            else if (2U == option)
            {
                put_req_type = BPP_PUT_REQ_TYPE_REF_XML;
                BT_str_copy(type_header, BPP_TYPE_REF_XML);
            }
            else if (3U == option)
            {
                put_req_type = BPP_PUT_REQ_TYPE_REF_LIST;
                BT_str_copy(type_header, BPP_TYPE_REF_LIST);
            }
            else
            {
                LOG_DEBUG("Invalid Choice\n");
                break;
            }

            /*Fill type hdr values*/
            type_header_info.value = type_header;
            type_header_info.length = (UINT16)(BT_str_n_len(type_header, sizeof(type_header)) + 1U);

            fsize = 0U;
            remaining = 0U;
            sent = 0U;
            bpp_xchg_size = bpp_sender_jobch_instance[handle].max_xchg_size;

            if (BPP_PUT_REQ_TYPE_REF_SIMPLE == put_req_type)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BPP_ROOT_FOLDER_BASE,
                    (UCHAR *)"bpp_send_ref_simple_req.txt",
                    file_object
                );
            }
            else if (BPP_PUT_REQ_TYPE_REF_XML == put_req_type)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BPP_ROOT_FOLDER_BASE,
                    (UCHAR *)"sample_ref_push_file.xml",
                    file_object
                );
            }
            else
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BPP_ROOT_FOLDER_BASE,
                    (UCHAR *)"bpp_send_ref_list.txt",
                    file_object
                );
            }

            /* Open the file to be sent */
            retval = BT_fops_file_open(file_object, (UCHAR *)"rb", &fp);
            if ((API_SUCCESS == retval) && (NULL != fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);

                remaining = fsize;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
                retval = BT_fops_file_open(file_object, (UCHAR *)"rb", &fp);
            }
            else
            {
                LOG_DEBUG("Failed to open file\n");
                break;
            }

            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16)bpp_xchg_size;
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
                body_info.value = BT_alloc_mem(body_info.length);
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
            req_info.type_header = &type_header_info;
            req_info.description = &description_info;
            req_info.appl_params = &appl_param;

            LOG_DEBUG ("Requesting to Push file...\n");
            retval = BT_bpp_sender_send_reference
                     (
                         &bpp_sender_jobch_instance[handle].handle,
                         &req_info,
                         more,
                         &actual
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("read length = %d, actual sent = %d\n", body_info.length, actual);
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(fp, sent, SEEK_SET);
            }

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
                BT_free_mem(body_info.value);
                body_info.value = NULL;
            }
        }
            break;

        case 31:
            bpp_sender_print_appl_instances (BPP_JOB_CHANNEL);
            LOG_DEBUG ("Enter BPP Sender instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            current_channel = BPP_JOB_CHANNEL;

            BPP_INIT_HEADER_STRUCT (body_info);
            BPP_INIT_HEADER_STRUCT (name_info);
            BPP_INIT_HEADER_STRUCT (http_accept_header);
            BPP_INIT_HEADER_STRUCT (http_accept_lang_header);

            LOG_DEBUG ("Enter the object name : ");
            scanf ("%s", file_name);
            file_name_len = (UINT16 )BT_str_n_len (file_name, sizeof(file_name));

            /* Fill name hdr values */
            name_info.value = file_name;
            name_info.length = (UINT16 )(file_name_len + 1U);
            /* Fill the http headers values*/
            http_accept_header.value = bpp_sample_http_accept_header;
            http_accept_header.length = (UINT16 )(BT_str_len (bpp_sample_http_accept_header) + 1U);
            http_accept_lang_header.value = bpp_sample_http_accept_lang_header;
            http_accept_lang_header.length = (UINT16 )(BT_str_len (bpp_sample_http_accept_lang_header) + 1U);

            fsize = 0U;
            remaining = 0U;
            sent = 0U;
            bpp_xchg_size = bpp_sender_jobch_instance[handle].max_xchg_size;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)BPP_ROOT_FOLDER_BASE,
                (UCHAR *)"sample_ref_push_file.xml",
                file_object
            );

            /* Open the file to be sent */
            retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            if ((API_SUCCESS == retval) && (NULL != fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);

                remaining = fsize;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
                retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fp);
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");
                break;
            }

            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16 )bpp_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16 )remaining;
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
            req_info.http_accept_header = &http_accept_header;
            req_info.http_accept_lang_header = &http_accept_lang_header;

            LOG_DEBUG ("Requesting to Get RUI...\n");
            retval = BT_bpp_sender_get_rui
                     (
                         &bpp_sender_jobch_instance[handle].handle,
                         &req_info,
                         BPP_JOB_CHANNEL,
                         more,
                         &actual
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("read length = %d, actual sent = %d\n", body_info.length, actual);
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(fp, sent, SEEK_SET);
            }

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

        case 50:
            bpp_sender_print_appl_instances((UCHAR)channel_type);

            LOG_DEBUG("Enter BPP Sender instance: ");
            scanf("%d", &handle);

            if (BPP_NUM_SENDER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            LOG_DEBUG("Enter Channel Type \n 0. Job Channel \n 1. Status Channel \n 2. Object Channel \n ");
            scanf("%d", &choice);

            channel_type = (UCHAR)choice;
            /* BPP applicaion instance handle */
            if (channel_type == 0U)
            {
                bpp_instance = &bpp_sender_jobch_instance[handle];
            }
            else if (channel_type == 1U)
            {
                bpp_instance = &bpp_sender_statusch_instance[handle];
            }
            else if (channel_type == 2U)
            {
                bpp_instance = &bpp_sender_objectch_instance[handle];
            }
            else
            {
                LOG_DEBUG("\nWrong choice for Channel Type\n");
                break;
            }

            LOG_DEBUG(" Enter the PIN code: ");
            scanf("%s", pin_code);

            pin.value = pin_code;
            pin.length = (UINT16)BT_str_len(pin_code);

            LOG_DEBUG ("User Id required (0/1)?\n");
            scanf ("%d", &choice);
            input = (UCHAR )choice;

            if (1U == input)
            {
                LOG_DEBUG(" Enter the User Id: ");
                scanf("%s", user_code);

                userid.value = user_code;
                userid.length = (UINT16)BT_str_len(user_code);
            }
            else
            {
                /* User Id not required */
                userid.value = NULL;
                userid.length = 0U;
            }

            connect_req_send.pin_info = &pin;
            connect_req_send.user_id = &userid;

            /* Copy other Connect parameters from the instance */
            connect_req_send.auth_flag = 0U;
            connect_req_send.bd_addr = bpp_instance->bd_addr;
            connect_req_send.server_channel = bpp_job_server_channel;
            connect_req_send.max_recv_size = 1536U;

            bpp_req_header_send.bpp_connect_info = &connect_req_send;

            retval = BT_bpp_sender_send_request
                     (
                         &bpp_instance->handle,
                         BPP_SENDER_JOBCH_CONNECT_CNF,
                         BPP_UNAUTH_RSP,
                         &bpp_req_header_send,
                         channel_type,
                         0U,
                         NULL
                     );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG(" Failed to send the Connect Request \n");
            }
            else
            {
                LOG_DEBUG(" Succuessfully sent the Connect Request \n");
            }
            break;

        case 60:
            LOG_DEBUG("Want to stop the send cnf in the middle(0/1)\n");
            scanf("%d", &choice);
            appl_bpp_sender_send_doc_stop_flag = (UCHAR )choice;
            break;

        case 61:
            LOG_DEBUG("Set Max. send cnf. count \n");
            scanf("%d", &choice);
            appl_bpp_sender_send_doc_cnf_max_cnt = (UCHAR)choice;
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

API_RESULT appl_bpp_sender_callback
           (
               BPP_HANDLE      * bpp_handle,
               UINT8             event_type,
               UINT16            event_result,
               BPP_HEADERS     * event_header,
               UINT16            event_hdrlen,
               UCHAR             bpp_channel_type
           )
{
    API_RESULT retval;
    BPP_HANDLE handle;
    BPP_HEADERS *bpp_rx_hdrs;
    UINT16 i;
    UINT8 *phandle;
    UCHAR send_response;
    UCHAR send_request;
    UCHAR tx_response;
    BPP_HEADERS bpp_tx_hdrs;
    BPP_HEADER_STRUCT body_info;
    BPP_REQUEST_STRUCT req_info;
    UCHAR more;
    UINT16 actual;
    BPP_RESPONSE_STRUCT bpp_rsp_info;

    /* Initialize to default */
    handle = *bpp_handle;
    retval = API_SUCCESS;
    bpp_rx_hdrs = event_header;

    send_response = 0U;
    send_request = 0x00U;
    more = 0x00U;
    tx_response = 0x00U;
    actual      = 0x00U;
    phandle = NULL;

    BT_mem_set(&bpp_tx_hdrs, 0, sizeof(BPP_HEADERS));
    BT_mem_set(&req_info, 0, sizeof(BPP_REQUEST_STRUCT));
    BT_mem_set(&bpp_rsp_info, 0, sizeof(BPP_RESPONSE_STRUCT));
    BPP_INIT_HEADER_STRUCT (body_info);

    BT_IGNORE_UNUSED_PARAM (event_hdrlen);

    LOG_DEBUG("channel type ---> %d\n", bpp_channel_type);

    for (i = 0U; i < BPP_NUM_SENDER_INSTANCE; i++)
    {
        if(BPP_JOB_CHANNEL == bpp_channel_type)
        {
            if (bpp_sender_jobch_instance[i].handle == handle)
            {
                break;
            }
        }
        else if(BPP_STATUS_CHANNEL == bpp_channel_type)
        {
            if (bpp_sender_statusch_instance[i].handle == handle)
            {
                break;
            }
        }
        else if(BPP_OBJECT_CHANNEL == bpp_channel_type)
        {
            if (bpp_sender_objectch_instance[i].handle == handle)
            {
                break;
            }
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }

    if(BPP_JOB_CHANNEL == bpp_channel_type)
    {
        phandle = &bpp_sender_jobch_instance[handle].handle;
    }
    else if(BPP_STATUS_CHANNEL == bpp_channel_type)
    {
        phandle = &bpp_sender_statusch_instance[handle].handle;
    }
    else if(BPP_OBJECT_CHANNEL == bpp_channel_type)
    {
        phandle = &bpp_sender_objectch_instance[handle].handle;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    /* Fix for build warning. TODO: Check usage */
    BT_IGNORE_UNUSED_PARAM(phandle);

    switch(event_type)
    {
    case BPP_SENDER_STATUSCH_TRANSPORT_CLOSE_IND: /* Fall Through */
    case BPP_SENDER_STATUSCH_TRANSPORT_CLOSE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received %s\n", (BPP_SENDER_STATUSCH_TRANSPORT_CLOSE_IND == event_type) ?
        "BPP_SENDER_STATUSCH_TRANSPORT_CLOSE_IND" : "BPP_SENDER_STATUSCH_TRANSPORT_CLOSE_CNF");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        /* Reset bd_addr */
        for (i = 0U; i < BPP_NUM_SENDER_INSTANCE; i++)
        {
            if (bpp_sender_statusch_instance[i].handle == handle)
            {
                BT_mem_set (bpp_sender_statusch_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);
            }
        }

        bpp_sender_print_appl_instances(BPP_STATUS_CHANNEL);
        break;

    case BPP_SENDER_JOBCH_TRANSPORT_CLOSE_IND: /* Fall Through */
    case BPP_SENDER_JOBCH_TRANSPORT_CLOSE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received %s\n", (BPP_SENDER_JOBCH_TRANSPORT_CLOSE_IND == event_type) ?
        "BPP_SENDER_JOBCH_TRANSPORT_CLOSE_IND" : "BPP_SENDER_JOBCH_TRANSPORT_CLOSE_CNF");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        /* Reset bd_addr */
        for (i = 0U; i < BPP_NUM_SENDER_INSTANCE; i++)
        {
            if (bpp_sender_jobch_instance[i].handle == handle)
            {
                BT_mem_set (bpp_sender_jobch_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);
            }
        }

        bpp_sender_print_appl_instances(BPP_JOB_CHANNEL);
        break;

    case BPP_SENDER_OBJECTCH_TRANSPORT_CLOSE_IND: /* Fall Through */
    case BPP_SENDER_OBJECTCH_TRANSPORT_CLOSE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received %s\n", (BPP_SENDER_OBJECTCH_TRANSPORT_CLOSE_IND == event_type) ?
        "BPP_SENDER_OBJECTCH_TRANSPORT_CLOSE_IND" : "BPP_SENDER_OBJECTCH_TRANSPORT_CLOSE_CNF");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        /* Reset bd_addr */
        for (i = 0U; i < BPP_NUM_SENDER_INSTANCE; i++)
        {
            if (bpp_sender_objectch_instance[i].handle == handle)
            {
                BT_mem_set (bpp_sender_objectch_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);
            }
        }

        bpp_sender_print_appl_instances(BPP_OBJECT_CHANNEL);
        break;

    case BPP_SENDER_STATUSCH_CONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_STATUSCH_CONNECT_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (bpp_rx_hdrs->bpp_connect_info->bd_addr));

        LOG_DEBUG ("MAX Exchange Size: %d\n",
        bpp_rx_hdrs->bpp_connect_info->max_recv_size);

        /* Get the Handle parameters */
        for (i = 0U; i < BPP_NUM_SENDER_INSTANCE; i++)
        {
            if (bpp_sender_statusch_instance[i].handle == handle)
            {
                BT_mem_copy
                (
                    bpp_sender_statusch_instance[i].bd_addr,
                    bpp_rx_hdrs->bpp_connect_info->bd_addr,
                    BT_BD_ADDR_SIZE
                );

                bpp_sender_statusch_instance[i].max_xchg_size =
                    bpp_rx_hdrs->bpp_connect_info->max_recv_size;

                break;
            }
        }

        bpp_sender_print_appl_instances(BPP_STATUS_CHANNEL);
        break;

    case BPP_SENDER_JOBCH_CONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_JOBCH_CONNECT_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (bpp_rx_hdrs->bpp_connect_info->bd_addr));

        LOG_DEBUG ("MAX Exchange Size: %d\n",
        bpp_rx_hdrs->bpp_connect_info->max_recv_size);

        /* Get the Handle parameters */
        for (i = 0U; i < BPP_NUM_SENDER_INSTANCE; i++)
        {
            if (bpp_sender_jobch_instance[i].handle == handle)
            {
                BT_mem_copy
                (
                    bpp_sender_jobch_instance[i].bd_addr,
                    bpp_rx_hdrs->bpp_connect_info->bd_addr,
                    BT_BD_ADDR_SIZE
                );

                bpp_sender_jobch_instance[i].max_xchg_size =
                    bpp_rx_hdrs->bpp_connect_info->max_recv_size;

                break;
            }
        }

        bpp_sender_print_appl_instances(BPP_JOB_CHANNEL);

        if (BPP_UNAUTH_RSP == event_result)
        {
            LOG_DEBUG("\nAuthentication Challenged. Choose 50 to enter the User ID and the Password\n");
        }
        break;

    case BPP_SENDER_OBJECTCH_CONNECT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_OBJECTCH_CONNECT_IND\n");
        LOG_DEBUG("BPP SENDER Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        /* Set response to be sent */
        send_response = 1U;

        if (BPP_SUCCESS_RSP != event_result)
        {
            tx_response = BPP_NOT_ACCEPTABLE_RSP;
            break;
        }

        LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (bpp_rx_hdrs->bpp_connect_info->bd_addr));

        LOG_DEBUG ("MAX Exchange Size: %d\n",
        bpp_rx_hdrs->bpp_connect_info->max_recv_size);
        tx_response = BPP_SUCCESS_RSP;

        /* Get the Handle parameters */
        BT_mem_copy
        (
            bpp_sender_objectch_instance[i].bd_addr,
            bpp_rx_hdrs->bpp_connect_info->bd_addr,
            BT_BD_ADDR_SIZE
        );

        bpp_sender_objectch_instance[i].max_xchg_size =
            bpp_rx_hdrs->bpp_connect_info->max_recv_size;

        bpp_sender_print_appl_instances (BPP_OBJECT_CHANNEL);

        /* Set path to root folder */
        if (0U != bpp_sender_objectch_instance[i].root_depth)
        {
            while (0U != bpp_sender_objectch_instance[i].root_depth)
            {
                retval = BT_vfops_set_path_backward(bpp_sender_objectch_instance[i].path);
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("Failed to set path backward\n");
                }
                bpp_sender_objectch_instance[i].root_depth --;
            }

            LOG_DEBUG ("Folder set to ROOT\n");
        }
        break;

    case BPP_SENDER_STATUSCH_DISCONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_STATUSCH_DISCONNECT_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        bpp_sender_print_appl_instances(BPP_STATUS_CHANNEL);
        break;

    case BPP_SENDER_JOBCH_DISCONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_JOBCH_DISCONNECT_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        bpp_sender_print_appl_instances(BPP_JOB_CHANNEL);
        break;

    case BPP_SENDER_OBJECTCH_DISCONNECT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_OBJECTCH_DISCONNECT_IND\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        bpp_sender_print_appl_instances (BPP_OBJECT_CHANNEL);
        break;

    case BPP_SENDER_FILE_PUSH_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_FILE_PUSH_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((BT_TRUE == appl_bpp_sender_cancel_job_flag) ||
            (BPP_SUCCESS_RSP == event_result) ||
            (BPP_CONTINUE_RSP != event_result))
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fp);
            fp = NULL;

            appl_bpp_sender_print_ongoing = BT_FALSE;

            if (BT_TRUE == appl_bpp_sender_cancel_job_flag)
            {
                (BT_IGNORE_RETURN_VALUE) appl_bpp_sender_cancel_job
                (
                    appl_bpp_sender_cancel_job_instance,
                    appl_bpp_sender_cancel_job_channel
                );
            }

            break;
        }

        /* Fill body hdr values */
        if (remaining > bpp_xchg_size)
        {
            body_info.length = (UINT16 )bpp_xchg_size;
            more = 0x01U;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
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

        req_info.body = &body_info;

        bpp_tx_hdrs.bpp_req_info = &req_info;

        send_request = 0x01U;
        break;

    case BPP_SENDER_SEND_DOC_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_SEND_DOC_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        appl_bpp_sender_send_doc_cnf_cnt += 1U;

        LOG_DEBUG("remainig: %d\n", remaining);

        if ((BPP_SUCCESS_RSP == event_result) ||
            (BPP_CONTINUE_RSP != event_result))
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;
            remaining = 0U;
            actual = 0U;
            send_request = 0x00U;

            if (NULL != bpp_tx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bpp_tx_fp);
                bpp_tx_fp = NULL;
            }

            if (NULL != bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bpp_rx_fp);
                bpp_rx_fp = NULL;
            }

            break;
        }

        if ((appl_bpp_sender_send_doc_cnf_cnt > appl_bpp_sender_send_doc_cnf_max_cnt) &&
            (0U != appl_bpp_sender_send_doc_stop_flag))
        {
            /* Use option #60, #61 to reset */
            appl_bpp_sender_send_doc_cnf_cnt = 0x00U;
            /* appl_bpp_sender_send_doc_stop_flag = 0x00; */
#if 1
            LOG_DEBUG("====== appl_bpp_sender_cancel_job ======\n");
            retval = appl_bpp_sender_cancel_job_ext(0x00U, 0x01U);
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("Failed to initiate Cancel Job operation. Reason: 0x%04X\n", retval);
            }
#else
            /**
             * Experiment to sent Abort.
             */
            LOG_DEBUG("###### BT_bpp_send_abort ########## \n");
            retval = BT_bpp_sender_send_abort(phandle, current_channel);
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("Failed to send BT_bpp_send_abort. Reason: 0x%04X\n", retval);
            }
#endif /* 0 */
        }

        /* Fill body hdr values */
        if (remaining > bpp_xchg_size)
        {
            body_info.length = (UINT16 )bpp_xchg_size;
            more = 0x01U;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
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
                bpp_tx_fp,
                &actual
            );
        }

        req_info.body = &body_info;

#if 0
        /* Print received data on console */
        LOG_DEBUG("\n====== BPP Push Object data =====\n");
        for (i = 0; i < body_info.length; i++)
        {
            LOG_DEBUG("%c", body_info.value[i]);
        }
        LOG_DEBUG("\n================================= \n");
#endif /* 0 */

        bpp_tx_hdrs.bpp_req_info = &req_info;

        send_request = 0x01U;
        break;

    case BPP_SENDER_SIMPLE_REF_PUSH_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_SIMPLE_REF_PUSH_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((BPP_SUCCESS_RSP == event_result) ||
            (BPP_CONTINUE_RSP != event_result))
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;
            remaining = 0U;
            actual = 0U;
            send_request = 0x00U;

            if (NULL != fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }
            break;
        }

        /* Fill body hdr values */
        if (remaining > bpp_xchg_size)
        {
            body_info.length = (UINT16 )bpp_xchg_size;
            more = 0x01U;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
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

        req_info.body = &body_info;

        bpp_tx_hdrs.bpp_req_info = &req_info;

        send_request = 0x01U;
        break;

    case BPP_SENDER_SEND_REF_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_SEND_REF_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((BPP_SUCCESS_RSP == event_result) ||
            (BPP_CONTINUE_RSP != event_result))
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;
            remaining = 0U;
            actual = 0U;
            send_request = 0x00U;

            if (NULL != fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }
            break;
        }

        /* Fill body hdr values */
        if (remaining > bpp_xchg_size)
        {
            body_info.length = (UINT16 )bpp_xchg_size;
            more = 0x01U;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
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

        req_info.body = &body_info;

        bpp_tx_hdrs.bpp_req_info = &req_info;

        send_request = 0x01U;
        break;

    case BPP_SENDER_CREATE_JOB_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_CREATE_JOB_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((BPP_CONTINUE_RSP != event_result) &&
            (BPP_SUCCESS_RSP != event_result))
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;
            remaining = 0U;
            actual = 0U;
            send_request = 0x00U;

            if (NULL != fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }

            if (NULL != bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bpp_rx_fp);
                bpp_rx_fp = NULL;
            }

            break;
        }

        if(remaining > 0U)
        {
            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16 )bpp_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body_info.length = (UINT16 )remaining;
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

            req_info.body = &body_info;

            bpp_tx_hdrs.bpp_req_info = &req_info;

            send_request =  0x01U;
        }
        else if (BPP_CONTINUE_RSP == event_result)
        {
            req_info.body = NULL;
            bpp_tx_hdrs.bpp_req_info = &req_info;
            send_request = 0x01U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if ((NULL == bpp_rx_hdrs->bpp_rsp_info->body) ||
            (NULL == bpp_rx_hdrs->bpp_rsp_info->body->value) ||
            (0U == bpp_rx_hdrs->bpp_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            UINT16 length;
            UCHAR *data;

            data = bpp_rx_hdrs->bpp_rsp_info->body->value;
            length = bpp_rx_hdrs->bpp_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG ("\n ====== BPP CREATE JOB RESPONSE ===== \n");
            for (i = 0U; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n ================================= \n");

            LOG_DEBUG("JOB ID : %d\n", bpp_rx_hdrs->bpp_rsp_info->appl_params->job_id);

            /* Open file for dumping received GetImageListing request data */
            if (NULL == bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BPP_ROOT_FOLDER_BASE,
                    (UCHAR *)"bpp_create_job_response.txt",
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &bpp_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == bpp_rx_fp))
                {
                    LOG_DEBUG ("Failed to open Folder Listing file\n");
                }
            }

            /* Write to file */
            if (NULL != bpp_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bpp_rx_fp, &actual);
            }
        }

        /* Close the file if final chunk is received */
        if ((BPP_SUCCESS_RSP == event_result) &&
            (NULL != bpp_rx_fp))

        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close (bpp_rx_fp);
            bpp_rx_fp = NULL;
        }
        break;

    case BPP_SENDER_GET_PRINTER_ATTRS_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_GET_PRINTER_ATTRS_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((BPP_CONTINUE_RSP != event_result) &&
            (BPP_SUCCESS_RSP != event_result))
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;
            remaining = 0U;
            actual = 0U;
            send_request = 0x00U;

            if (NULL != fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }

            if (NULL != bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bpp_rx_fp);
                bpp_rx_fp = NULL;
            }

            break;
        }

        if (remaining > 0U)
        {
            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16)bpp_xchg_size;
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
                body_info.value = BT_alloc_mem(body_info.length);
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

            req_info.body = &body_info;

            bpp_tx_hdrs.bpp_req_info = &req_info;

            send_request = 0x01U;
        }
        else if (BPP_CONTINUE_RSP == event_result)
        {
            req_info.body = NULL;
            bpp_tx_hdrs.bpp_req_info = &req_info;
            send_request = 0x01U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if ((NULL == bpp_rx_hdrs->bpp_rsp_info->body) ||
            (NULL == bpp_rx_hdrs->bpp_rsp_info->body->value) ||
            (0U == bpp_rx_hdrs->bpp_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            UINT16 length;
            UCHAR *data;

            data = bpp_rx_hdrs->bpp_rsp_info->body->value;
            length = bpp_rx_hdrs->bpp_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG ("\n ====== BPP GET PRINTER ATTRS RESPONSE ===== \n");
            for (i = 0U; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n ================================= \n");

            /* Open file for dumping received GetImageListing request data */
            if (NULL == bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BPP_ROOT_FOLDER_BASE,
                    (UCHAR *)"bpp_get_printer_attrs_response.txt",
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &bpp_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == bpp_rx_fp))
                {
                    LOG_DEBUG ("Failed to open file\n");
                }
            }

            /* Write to file */
            if (NULL != bpp_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bpp_rx_fp, &actual);
            }
        }

        /* Close the file if final chunk is received */
        if ((BPP_SUCCESS_RSP == event_result) &&
            (NULL != bpp_rx_fp))
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close (bpp_rx_fp);
            bpp_rx_fp = NULL;
        }
        break;

    case BPP_SENDER_GET_JOB_ATTRS_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_GET_JOB_ATTRS_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((BPP_CONTINUE_RSP != event_result) &&
            (BPP_SUCCESS_RSP != event_result))
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;
            remaining = 0U;
            actual = 0U;
            send_request = 0x00U;

            if (NULL != fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }

            if (NULL != bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bpp_rx_fp);
                bpp_rx_fp = NULL;
            }

            break;
        }

        if (remaining > 0U)
        {
            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16)bpp_xchg_size;
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
                body_info.value = BT_alloc_mem(body_info.length);
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

            req_info.body = &body_info;

            bpp_tx_hdrs.bpp_req_info = &req_info;

            send_request = 0x01U;
        }
        else if (BPP_CONTINUE_RSP == event_result)
        {
            req_info.body = NULL;
            bpp_tx_hdrs.bpp_req_info = &req_info;
            send_request = 0x01U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if ((NULL == bpp_rx_hdrs->bpp_rsp_info->body) ||
            (NULL == bpp_rx_hdrs->bpp_rsp_info->body->value) ||
            (0U == bpp_rx_hdrs->bpp_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            UINT16 length;
            UCHAR *data;

            data = bpp_rx_hdrs->bpp_rsp_info->body->value;
            length = bpp_rx_hdrs->bpp_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG ("\n ====== BPP GET JOB ATTRS RESPONSE ===== \n");
            for (i = 0U; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n ================================= \n");

            /* Open file for dumping received GetImageListing request data */
            if (NULL == bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BPP_ROOT_FOLDER_BASE,
                    (UCHAR *)"bpp_get_job_attrs_response.txt",
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &bpp_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == bpp_rx_fp))
                {
                    LOG_DEBUG ("Failed to open file\n");
                }
            }

            /* Write to file */
            if (NULL != bpp_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bpp_rx_fp, &actual);
            }
        }

        /* Close the file if final chunk is received */
        if ((BPP_SUCCESS_RSP == event_result) &&
            (NULL != bpp_rx_fp))
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close (bpp_rx_fp);
            bpp_rx_fp = NULL;
        }
        break;

    case BPP_SENDER_CANCEL_JOB_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_CANCEL_JOB_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (0U != appl_bpp_sender_send_doc_stop_flag)
        {
            LOG_DEBUG("==== appl_bpp_sender_send_doc_stop_flag ====\n");

            appl_bpp_sender_send_doc_stop_flag = 0U;
            send_request = 0x00U;
            break;
        }

        if ((BPP_CONTINUE_RSP != event_result) &&
            (BPP_SUCCESS_RSP != event_result))
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;
            remaining = 0U;
            actual = 0U;
            send_request = 0x00U;

            if (NULL != fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }

            if (NULL != bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bpp_rx_fp);
                bpp_rx_fp = NULL;
            }

            break;
        }

        if (remaining > 0U)
        {
            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16)bpp_xchg_size;
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
                body_info.value = BT_alloc_mem(body_info.length);
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

            req_info.body = &body_info;

            bpp_tx_hdrs.bpp_req_info = &req_info;

            send_request = 0x01U;
        }
        else if (BPP_CONTINUE_RSP == event_result)
        {
            req_info.body = NULL;
            bpp_tx_hdrs.bpp_req_info = &req_info;
            send_request = 0x01U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if ((NULL == bpp_rx_hdrs->bpp_rsp_info->body) ||
            (NULL == bpp_rx_hdrs->bpp_rsp_info->body->value) ||
            (0U == bpp_rx_hdrs->bpp_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            UINT16 length;
            UCHAR *data;

            data = bpp_rx_hdrs->bpp_rsp_info->body->value;
            length = bpp_rx_hdrs->bpp_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG ("\n ====== BPP CANCEL JOB RESPONSE ===== \n");
            for (i = 0U; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n ================================= \n");

            /* Open file for dumping received GetImageListing request data */
            if (NULL == bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BPP_ROOT_FOLDER_BASE,
                    (UCHAR *)"bpp_cancel_job_response.txt",
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &bpp_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == bpp_rx_fp))
                {
                    LOG_DEBUG ("Failed to open file\n");
                }
            }

            /* Write to file */
            if (NULL != bpp_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bpp_rx_fp, &actual);
            }
        }

        /* Close the file if final chunk is received */
        if ((BPP_SUCCESS_RSP == event_result) &&
            (NULL != bpp_rx_fp))
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close (bpp_rx_fp);
            bpp_rx_fp = NULL;
            appl_bpp_sender_cancel_job_flag = BT_FALSE;
        }
        break;

    case BPP_SENDER_CREATE_PR_JOB_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_CREATE_PR_JOB_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((BPP_CONTINUE_RSP != event_result) &&
            (BPP_SUCCESS_RSP != event_result))
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;
            remaining = 0U;
            actual = 0U;
            send_request = 0x00U;

            if (NULL != fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }

            if (NULL != bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bpp_rx_fp);
                bpp_rx_fp = NULL;
            }

            break;
        }

        if(remaining > 0U)
        {
            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16)bpp_xchg_size;
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

            req_info.body = &body_info;

            bpp_tx_hdrs.bpp_req_info = &req_info;

            send_request =  0x01U;
        }
        else if (BPP_CONTINUE_RSP == event_result)
        {
            req_info.body = NULL;
            bpp_tx_hdrs.bpp_req_info = &req_info;
            send_request = 0x01U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if ((NULL == bpp_rx_hdrs->bpp_rsp_info->body) ||
            (NULL == bpp_rx_hdrs->bpp_rsp_info->body->value) ||
            (0U == bpp_rx_hdrs->bpp_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            UINT16 length;
            UCHAR *data;

            data = bpp_rx_hdrs->bpp_rsp_info->body->value;
            length = bpp_rx_hdrs->bpp_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG ("\n ====== BPP CREATE PRECISE JOB RESPONSE ===== \n");
            for (i = 0U; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n ================================= \n");

            LOG_DEBUG("JOB ID : %d\n", bpp_rx_hdrs->bpp_rsp_info->appl_params->job_id);


            /* Open file for dumping received GetImageListing request data */
            if (NULL == bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BPP_ROOT_FOLDER_BASE,
                    (UCHAR *)"bpp_create_precise_job_response.txt",
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &bpp_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == bpp_rx_fp))
                {
                    LOG_DEBUG ("Failed to open Folder Listing file\n");
                }
            }

            /* Write to file */
            if (NULL != bpp_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bpp_rx_fp, &actual);
            }
        }

        /* Close the file if final chunk is received */
        if ((BPP_SUCCESS_RSP == event_result) &&
            (NULL != bpp_rx_fp))
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close (bpp_rx_fp);
            bpp_rx_fp = NULL;
        }
        break;

    case BPP_SENDER_GET_MARGINS_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_GET_MARGINS_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((BPP_CONTINUE_RSP != event_result) &&
            (BPP_SUCCESS_RSP != event_result))
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;
            remaining = 0U;
            actual = 0U;
            send_request = 0x00U;

            if (NULL != fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }

            if (NULL != bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bpp_rx_fp);
                bpp_rx_fp = NULL;
            }

            break;
        }

        if (remaining > 0U)
        {
            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16)bpp_xchg_size;
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
                body_info.value = BT_alloc_mem(body_info.length);
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

            req_info.body = &body_info;

            bpp_tx_hdrs.bpp_req_info = &req_info;

            send_request = 0x01U;
        }
        else if (BPP_CONTINUE_RSP == event_result)
        {
            req_info.body = NULL;
            bpp_tx_hdrs.bpp_req_info = &req_info;
            send_request = 0x01U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if ((NULL == bpp_rx_hdrs->bpp_rsp_info->body) ||
            (NULL == bpp_rx_hdrs->bpp_rsp_info->body->value) ||
            (0U == bpp_rx_hdrs->bpp_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            UINT16 length;
            UCHAR *data;

            data = bpp_rx_hdrs->bpp_rsp_info->body->value;
            length = bpp_rx_hdrs->bpp_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG ("\n ====== BPP GET MARGINS RESPONSE ===== \n");
            for (i = 0U; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n ================================= \n");


            /* Open file for dumping received GetImageListing request data */
            if (NULL == bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BPP_ROOT_FOLDER_BASE,
                    (UCHAR *)"bpp_get_margins_response.txt",
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &bpp_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == bpp_rx_fp))
                {
                    LOG_DEBUG ("Failed to open file\n");
                }
            }

            /* Write to file */
            if (NULL != bpp_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bpp_rx_fp, &actual);
            }
        }

        /* Close the file if final chunk is received */
        if ((BPP_SUCCESS_RSP == event_result) &&
            (NULL != bpp_rx_fp))
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(bpp_rx_fp);
            bpp_rx_fp = NULL;
        }
        break;

    case BPP_SENDER_GET_RUI_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_GET_RUI_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((BPP_CONTINUE_RSP != event_result) &&
            (BPP_SUCCESS_RSP != event_result))
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;
            remaining = 0U;
            actual = 0U;
            send_request = 0x00U;

            if (NULL != bpp_tx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bpp_tx_fp);
                bpp_tx_fp = NULL;
            }

            if (NULL != bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bpp_rx_fp);
                bpp_rx_fp = NULL;
            }

            break;
        }

        if (remaining > 0U)
        {
            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16)bpp_xchg_size;
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
                body_info.value = BT_alloc_mem(body_info.length);
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

            req_info.body = &body_info;
            bpp_tx_hdrs.bpp_req_info = &req_info;

            send_request = 0x01U;
        }
        else if (BPP_CONTINUE_RSP == event_result)
        {
            req_info.body = NULL;
            bpp_tx_hdrs.bpp_req_info = &req_info;
            send_request = 0x01U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if ((NULL == bpp_rx_hdrs->bpp_rsp_info->body) ||
            (NULL == bpp_rx_hdrs->bpp_rsp_info->body->value) ||
            (0U == bpp_rx_hdrs->bpp_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            UINT16 length;
            UCHAR *data;

            data = bpp_rx_hdrs->bpp_rsp_info->body->value;
            length = bpp_rx_hdrs->bpp_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG ("\n ====== BPP GET RUI RESPONSE ===== \n");
            for (i = 0U; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n ================================= \n");


            /* Open file for dumping received GetImageListing request data */
            if (NULL == bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BPP_ROOT_FOLDER_BASE,
                    (UCHAR *)"bpp_get_rui_response.txt",
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &bpp_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == bpp_rx_fp))
                {
                    LOG_DEBUG ("Failed to open file\n");
                }
            }

            /* Write to file */
            if (NULL != bpp_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bpp_rx_fp, &actual);
            }
        }

        /* Close the file if final chunk is received */
        if ((BPP_SUCCESS_RSP == event_result) &&
            (NULL != bpp_rx_fp))
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(bpp_rx_fp);
            bpp_rx_fp = NULL;
        }
        break;

    case BPP_SENDER_GET_EVENT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_GET_EVENT_CNF\n");
        LOG_DEBUG("BPP Sender Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((BPP_CONTINUE_RSP != event_result) &&
            (BPP_SUCCESS_RSP != event_result))
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;
            remaining = 0U;
            actual = 0U;
            send_request = 0x00U;

            if (NULL != fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }

            if (NULL != bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bpp_rx_fp);
                bpp_rx_fp = NULL;
            }

            break;
        }

        if (remaining > 0U)
        {
            /* Fill body hdr values */
            if (remaining > bpp_xchg_size)
            {
                body_info.length = (UINT16)bpp_xchg_size;
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
                body_info.value = BT_alloc_mem(body_info.length);
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

            req_info.body = &body_info;

            bpp_tx_hdrs.bpp_req_info = &req_info;

            send_request = 0x01U;
        }
        else if (BPP_CONTINUE_RSP == event_result)
        {
            req_info.body = NULL;
            bpp_tx_hdrs.bpp_req_info = &req_info;
            send_request = 0x01U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if ((NULL == bpp_rx_hdrs->bpp_rsp_info->body) ||
            (NULL == bpp_rx_hdrs->bpp_rsp_info->body->value) ||
            (0U == bpp_rx_hdrs->bpp_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            UINT16 length;
            UCHAR *data;

            data = bpp_rx_hdrs->bpp_rsp_info->body->value;
            length = bpp_rx_hdrs->bpp_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG ("\n ====== BPP GET EVENT RESPONSE ===== \n");
            for (i = 0U; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n ================================= \n");

            /* Open file for dumping received GetImageListing request data */
            if (NULL == bpp_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)BPP_ROOT_FOLDER_BASE,
                    (UCHAR *)"bpp_get_event_response.txt",
                    file_object
                );

                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"wb",
                             &bpp_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == bpp_rx_fp))
                {
                    LOG_DEBUG ("Failed to open file\n");
                }
            }

            /* Write to file */
            if (NULL != bpp_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bpp_rx_fp, &actual);
            }
        }

        /* Close the file if final chunk is received */
        if ((BPP_SUCCESS_RSP == event_result) &&
            (NULL != bpp_rx_fp))
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(bpp_rx_fp);
            bpp_rx_fp = NULL;
        }
        break;

    case BPP_SENDER_GET_REF_OBJ_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received BPP_SENDER_GET_REF_OBJ_IND \n");
        LOG_DEBUG("BPP Responder Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        bpp_rsp_info.body             = &body_info;
        BPP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
        bpp_rsp_info.appl_params      = &appl_param;
        bpp_tx_hdrs.bpp_rsp_info      = &bpp_rsp_info;


        /*set application parameter */
        BPP_SET_APPL_PARAM_FLAG
        (
            appl_param.appl_param_flag,
            BPP_FLAG_FILE_SIZE
        );

#if 0
        /*check if event_result is countinue response */
        if(BPP_CONTINUE_RSP == event_result)
        {
            tx_response = BPP_CONTINUE_RSP;
            break;
        }
#endif
        if(NULL != bpp_rx_hdrs->bpp_req_info->name->value)
        {
             LOG_DEBUG("\nName : %s\n", bpp_rx_hdrs->bpp_req_info->name->value);
        }
        else
        {
             LOG_DEBUG("\nName not sent\n");
        }

        LOG_DEBUG("OFFSET VALUE : %d\n", bpp_rx_hdrs->bpp_req_info->appl_params->offset);
        LOG_DEBUG("COUNT : %d\n", bpp_rx_hdrs->bpp_req_info->appl_params->count);
        LOG_DEBUG("FILE SIZE : %d\n", bpp_rx_hdrs->bpp_req_info->appl_params->file_size);
        appl_param.file_size  = (INT32)bpp_rx_hdrs->bpp_req_info->appl_params->file_size;
        /* send response */
        send_response = 1U;

        /* Reject if operation reject */
        if (0U != appl_reject_opn)
        {
            tx_response = appl_reject_opn;
            LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);
            break;
        }

        if (0U == sent)
        {
            bpp_xchg_size = bpp_sender_objectch_instance[handle].max_xchg_size;
            tx_response = BPP_SUCCESS_RSP;

            LOG_DEBUG("\nbpp_xcng_size --> %d\n",bpp_xchg_size);

            /* Create the listing file with path */
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                bpp_sender_objectch_instance[handle].path,
                (UCHAR *)bpp_rx_hdrs->bpp_req_info->name->value,
                file_object
            );
            LOG_DEBUG("\nfile object --> %s\n",file_object);
            /* Get the Sample listing from file */
            retval = BT_fops_file_open ((UCHAR *)file_object, (UCHAR *)"rb", &bpp_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != bpp_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bpp_tx_fp, &fsize);

                remaining = fsize;
                appl_param.file_size  = (INT32)fsize;
            }
            else
            {
                tx_response = BPP_NOT_FOUND_RSP;
                break;
            }
        }

        /* Fill body hdr values */
        if (remaining > bpp_xchg_size)
        {
            body_info.length = (UINT16 )bpp_xchg_size;
            tx_response = BPP_CONTINUE_RSP;
        }
        else if (0U != remaining)
        {
            body_info.length = (UINT16 )remaining;
            tx_response = BPP_SUCCESS_RSP;
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
                bpp_tx_fp,
                &actual
            );
        }

        /* Update object size sent & remaining to send */
        sent += actual;
        remaining = fsize - sent;

        /* Adjust the file read pointer to the actual bytes transmitted */
        if (body_info.length != actual)
        {
            printf("read length = %d, actual sent = %d\n", body_info.length, actual);
            printf("Adjusting the file read pointer\n");
            (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(bpp_tx_fp, sent, SEEK_SET);
        }

        if (0U == remaining)
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(bpp_tx_fp);
            bpp_tx_fp = NULL;
        }
        break;

    case BPP_SENDER_ABORT_CNF:
        LOG_DEBUG("Received BPP_SENDER_ABORT_CNF \n");
        LOG_DEBUG("BPP Responder Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

#if 0
        /* current_channel = 0x01; */
        LOG_DEBUG("====== appl_bpp_sender_cancel_job_ext ======\n");
        /* retval = appl_bpp_sender_cancel_job_ext(0x00, 0x01); */  /* Status Channel */
        retval = appl_bpp_sender_cancel_job_ext(0x00, 0x00); /* Job Channel */
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("Failed to initiate Cancel Job operation. Reason: 0x%04X\n", retval);
        }
#endif /* 0 */

        break;

    default:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received  BPP_EVENT_INVALID\n");
        break;
    }

    if (0U != send_request)
    {
        retval = BT_bpp_sender_send_request
                 (
                     phandle, /* &bpp_sender_jobch_instance[handle].handle, */
                     event_type,
                     event_result,
                     &bpp_tx_hdrs,
                     bpp_channel_type, /* current_channel, */
                     more,
                     &actual
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG ("BPP  Sender Send Request Failed - 0x%04X\n", retval);
        }
#if 1
        if ((BPP_SENDER_FILE_PUSH_CNF == event_type)||
            (BPP_SENDER_SEND_DOC_CNF == event_type)||
            (BPP_SENDER_SEND_REF_CNF == event_type)||
            (BPP_SENDER_SIMPLE_REF_PUSH_CNF == event_type) ||
            (BPP_SENDER_CREATE_JOB_CNF == event_type) ||
            (BPP_SENDER_GET_PRINTER_ATTRS_CNF == event_type) ||
            (BPP_SENDER_GET_JOB_ATTRS_CNF == event_type) ||
            (BPP_SENDER_CANCEL_JOB_CNF == event_type) ||
            (BPP_SENDER_CREATE_PR_JOB_CNF == event_type) ||
            (BPP_SENDER_GET_MARGINS_CNF == event_type) ||
            (BPP_SENDER_GET_EVENT_CNF == event_type) ||
            (BPP_SENDER_GET_RUI_CNF == event_type))
        {
            /* Free memory */
            if (NULL != body_info.value)
            {
                BT_free_mem (body_info.value);
                body_info.value = NULL;
            }

            if (0U != remaining)
            {
                /* Update object size sent & remaining to send */
                sent += actual;
                remaining = fsize - sent;

                /* Adjust the file read pointer to the actual bytes transmitted */
                if (body_info.length != actual)
                {
                    if (BPP_SENDER_SEND_DOC_CNF == event_type)
                    {
                        printf("read length = %d, actual sent = %d\n", body_info.length, actual);
                        printf("Adjusting the file read pointer\n");
                        (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(bpp_tx_fp, sent, SEEK_SET);
                    }
                    else
                    {
                        printf("read length = %d, actual sent = %d\n", body_info.length, actual);
                        printf("Adjusting the file read pointer\n");
                        (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(fp, sent, SEEK_SET);
                    }
                }
            }

            /* If operation has failed or completed, perform cleanup */
            if ((API_SUCCESS != retval) || (0U == remaining))
            {
                /* Reset the variables */
                sent = 0U;
                fsize = 0U;

                if (BPP_SENDER_SEND_DOC_CNF == event_type)
                {
                    if (NULL != bpp_tx_fp)
                    {
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_fops_file_close(bpp_tx_fp);
                        bpp_tx_fp = NULL;
                    }
                }
                else
                {
                    if (NULL != fp)
                    {
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_fops_file_close(fp);
                        fp = NULL;
                        appl_bpp_sender_print_ongoing = BT_FALSE;
                    }
                }
            }
        }
#endif
    }

    /* Send response if required */
    if (0U != send_response)
    {
        retval = BT_bpp_sender_send_response
                 (
                     &handle,
                     event_type,
                     tx_response,
                     &bpp_tx_hdrs,
                     bpp_channel_type
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG ("Failed to send BPP sender Response - 0x%04x\n", retval);
            tx_response = 0xFFU;
        }

        if ((BPP_CONTINUE_RSP != tx_response) &&
            (BPP_SUCCESS_RSP != tx_response))
        {
            if (NULL != bpp_tx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(bpp_tx_fp);
                bpp_tx_fp = NULL;
            }
        }
    }

    return retval;
}

void appl_bpp_sender_sdp_callback
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
    UINT16 attrib_id[3U] = {PROTOCOL_DESC_LIST,
                           ADDITIONAL_PROT_DESC_LIST_ID,
                           GOEP_L2CAP_PSM_ATTR_ID};

    num_attrib_ids = 3U;
#endif /* OBEX_OVER_L2CAP */

    BT_IGNORE_UNUSED_PARAM (length);

    /*
     * Frame the SDP Search pattern with following UUIDs
     * - BPP_UUID
     * - RFCOMM_UUID
     * - L2CAP_UUID
     * and Attribute ID "PROTOCOL_DESC_LIST"
     * (For getting the RFCOMM Server Channel on which Peer BPP Service
     *  is Listening)
     *
     */
    uuid[0U].uuid_type = UUID_16;
    uuid[1U].uuid_type = UUID_16;
    uuid[2U].uuid_type = UUID_16;
    uuid[0U].uuid_union.uuid_16 = ((0U == channel_type)?BPP_DIRECT_PRINTING_UUID: BPP_PRINTING_STATUS_UUID); /* BPP_REFERENCE_PRINTING_UUID */
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
                     &bpp_sdp_handle,
                     uuid,
                     3U,
                     attrib_id,
                     num_attrib_ids,
                     NULL,
                     0U,
                     bpp_attrib_data,
                     &bpp_attrib_data_len
                 );

        if(API_SUCCESS != retval)
        {
            LOG_DEBUG("SDP SEARCH Failed for BPP..Closing SDP\n");
            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&bpp_sdp_handle);
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

                /* Check the BPP server channel number for Job Channel */
                retval = BT_sdp_get_channel_number
                            (
                                sdp_query_result,
                                &bpp_job_server_channel
                            );

                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("SDP FAILED to find BPP Server Channel for Job Channel\n");
                }
                else
                {
                    LOG_DEBUG("Server Channel [Job]: 0x%02X\n", bpp_job_server_channel);

                    /*
                        * Store the Server Channel globally.
                        * Later it will be used while Creating BPP Connection
                        */
                }

                /* No PSMs found, check the BPP server channel number for Status Channel */
                retval = BT_sdp_get_additional_channel
                            (
                                sdp_query_result,
                                &bpp_status_server_channel
                            );

                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("SDP FAILED to find BPP Server Channel for Status Channel\n");
                }
                else
                {
                    LOG_DEBUG("Server Channel [Status]: 0x%02X\n", bpp_status_server_channel);

                    /*
                        * Store the Server Channel globally.
                        * Later it will be used while Creating BPP Connection
                        */
                }

                /* BPP PSM or Server channel found */
                if (API_SUCCESS == retval)
                {
                    LOG_DEBUG ("BPP Service Found\n");
                }
                else
                {
                    LOG_DEBUG ("Failed to find BPP Service.\n");
                }

                no_server_channels++;
            }
        }

        /* Now Close the SDP Connection */
        (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&bpp_sdp_handle);
        break;

    default: /* Events not needed to be Handled */
        LOG_DEBUG("Unhandled SDP Event (0x%02X)\n", command);
        break;
    }/* switch */

    return;
}

void bpp_sender_print_appl_instances (UCHAR bpp_channel_type)
{
    UINT8 i;
    BPP_INSTANCE *bpp_sender_instance;

    bpp_sender_instance = NULL;

    for (i = 0U; i < BPP_NUM_SENDER_INSTANCE; i++)
    {
        if(BPP_JOB_CHANNEL == bpp_channel_type)
        {
            LOG_DEBUG ("\n\n=========================JOB CHANNEL========================\n");
            bpp_sender_instance = &bpp_sender_jobch_instance[i];
            break;
        }
        else if(BPP_STATUS_CHANNEL == bpp_channel_type)
        {
            LOG_DEBUG ("\n\n=========================STATUS CHANNEL=====================\n");
            bpp_sender_instance = &bpp_sender_statusch_instance[i];
            break;
        }
        else if(BPP_OBJECT_CHANNEL == bpp_channel_type)
        {
            LOG_DEBUG ("\n\n=========================OBJECT CHANNEL=====================\n");
            bpp_sender_instance = &bpp_sender_objectch_instance[i];
            break;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }

    LOG_DEBUG ("Instance  BPP Handle\t BD Addr");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    if (NULL != bpp_sender_instance)
    {
        for (i = 0U; i < BPP_NUM_SENDER_INSTANCE; i++)
        {
            LOG_DEBUG ("%d\t  ", i);
            LOG_DEBUG ("%02x\t\t ", bpp_sender_instance[i].handle);
            LOG_DEBUG ("%02x:%02x:%02x:%02x:%02x:%02x\t ",
            bpp_sender_instance[i].bd_addr[0U], bpp_sender_instance[i].bd_addr[1U],
            bpp_sender_instance[i].bd_addr[2U], bpp_sender_instance[i].bd_addr[3U],
            bpp_sender_instance[i].bd_addr[4U], bpp_sender_instance[i].bd_addr[5U]);

            LOG_DEBUG ("\n");
        }
    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    return;
}

static API_RESULT appl_bpp_sender_cancel_job(UCHAR handle, UCHAR channel)
{
    API_RESULT retval;
    UINT8 *phandle;
    BPP_REQUEST_STRUCT req_info;
    BPP_HEADER_STRUCT body_info;

    UINT16 i;
    UINT16 actual;
    UCHAR more;

    /* Init */
    more = 0U;
    retval = API_SUCCESS;
    phandle = NULL;

    BPP_INIT_HEADER_STRUCT(body_info);
    if (channel == 0U)
    {
        phandle = &bpp_sender_jobch_instance[handle].handle;
        bpp_xchg_size = bpp_sender_jobch_instance[handle].max_xchg_size;
    }
    else if (channel == 1U)
    {
        phandle = &bpp_sender_statusch_instance[handle].handle;
        bpp_xchg_size = bpp_sender_statusch_instance[handle].max_xchg_size;
    }
    else
    {
        LOG_DEBUG("Invalid Channel Type: %d\n", channel);
        retval = API_FAILURE; /* return API_FAILURE; */
    }

    if (API_SUCCESS == retval)
    {
        fsize = 0U;
        remaining = 0U;
        sent = 0U;

        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_vfops_create_object_name
        (
            (UCHAR *)BPP_ROOT_FOLDER_BASE,
            (UCHAR *)"cancel_job_request.txt",
            file_object
        );

        /* Open the file to be sent */
        retval = BT_fops_file_open(file_object, (UCHAR *)"rb", &fp);
        if ((API_SUCCESS == retval) && (NULL != fp))
        {
            /* Get the file size */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);

            remaining = fsize;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fp);
            fp = NULL;
            retval = BT_fops_file_open(file_object, (UCHAR *)"rb", &fp);
        }
        else
        {
            LOG_DEBUG("Failed to open file\n");
            /* return retval; */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Fill body hdr values */
        if (remaining > bpp_xchg_size)
        {
            body_info.length = (UINT16)bpp_xchg_size;
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
            body_info.value = BT_alloc_mem(body_info.length);
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

#if 1
            /* Print received data on console */
            LOG_DEBUG("\n ====== BPP Job cancel Object data ===== \n");
            for (i = 0U; i < body_info.length; i++)
            {
                LOG_DEBUG("%c", body_info.value[i]);
            }
            LOG_DEBUG("\n ================================= \n");
#endif /* 0 */
        }

        /* Update the hdrs for put request */
        req_info.body = &body_info;

        LOG_DEBUG("Requesting to cancel the given job...\n");
        appl_bpp_sender_cancel_job_flag = BT_TRUE;
        retval = BT_bpp_sender_cancel_job
                 (
                     phandle,
                     &req_info,
                     current_channel,
                     more,
                     &actual
                 );
        LOG_DEBUG("Retval - 0x%04X\n", retval);

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("Failed to initiate Cancel Job operation. Reason: 0x%04X\n", retval);
            appl_bpp_sender_cancel_job_flag = BT_FALSE;

            /* return retval; */
        }
        else
        {
            /* Update object size sent & remaining to send */
            sent += actual;
            remaining = fsize - sent;

            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("read length = %d, actual sent = %d\n", body_info.length, actual);
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(fp, sent, SEEK_SET);
            }

            if (0U == remaining)
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
                BT_free_mem(body_info.value);
                body_info.value = NULL;
            }
        }
    }

    return retval;
}

static API_RESULT appl_bpp_sender_cancel_job_ext(UCHAR handle, UCHAR channel)
{
    API_RESULT retval;
    UINT8 *phandle;
    BPP_REQUEST_STRUCT req_info;
    BPP_HEADER_STRUCT body_info;

    UINT16 i;
    UINT16 actual;
    UCHAR more;

    /* Init */
    more = 0U;
    retval = API_SUCCESS;
    phandle = NULL;

    BPP_INIT_HEADER_STRUCT(body_info);
    if (channel == 0U)
    {
        phandle = &bpp_sender_jobch_instance[handle].handle;
        bpp_xchg_size = bpp_sender_jobch_instance[handle].max_xchg_size;
    }
    else if (channel == 1U)
    {
        phandle = &bpp_sender_statusch_instance[handle].handle;
        bpp_xchg_size = bpp_sender_statusch_instance[handle].max_xchg_size;
    }
    else
    {
        LOG_DEBUG("Invalid Channel Type: %d\n", channel);
        retval = API_FAILURE; /* return API_FAILURE; */
    }

    if (API_SUCCESS == retval)
    {
        _fsize = 0U;
        _remaining = 0U;
        _sent = 0U;

        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_vfops_create_object_name
        (
            (UCHAR *)BPP_ROOT_FOLDER_BASE,
            (UCHAR *)"cancel_job_request.txt",
            file_object
        );

        /* Open the file to be sent */
        retval = BT_fops_file_open(file_object, (UCHAR *)"rb", &fp);
        if ((API_SUCCESS == retval) && (NULL != fp))
        {
            /* Get the file size */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &_fsize);

            _remaining = _fsize;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fp);
            fp = NULL;
            retval = BT_fops_file_open(file_object, (UCHAR *)"rb", &fp);
        }
        else
        {
            LOG_DEBUG("Failed to open file\n");
            /* return retval; */
        }
    }

    if (API_SUCCESS == retval)
    {
        /* Fill body hdr values */
        if (_remaining > bpp_xchg_size)
        {
            body_info.length = (UINT16)bpp_xchg_size;
            more = 0x01U;
        }
        else if (0U != _remaining)
        {
            body_info.length = (UINT16)_remaining;
            more = 0x00U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_info.length)
        {
            body_info.value = BT_alloc_mem(body_info.length);
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

#if 1
            /* Print received data on console */
            LOG_DEBUG("\n\n ====== BPP Job cancel Object data ===== \n\n");
            for (i = 0U; i < body_info.length; i++)
            {
                LOG_DEBUG("%c", body_info.value[i]);
            }
            LOG_DEBUG("\n ================================= \n");
#endif /* 0 */
        }

        /* Update the hdrs for put request */
        req_info.body = &body_info;

        LOG_DEBUG("Requesting to cancel the given job...\n");
        appl_bpp_sender_cancel_job_flag = BT_TRUE;
        retval = BT_bpp_sender_cancel_job
                 (
                     phandle,
                     &req_info,
                     channel /* current_channel */,
                     more,
                     &actual
                 );
        LOG_DEBUG("Retval - 0x%04X\n", retval);

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("Failed to initiate Cancel Job operation. Reason: 0x%04X\n", retval);
            appl_bpp_sender_cancel_job_flag = BT_FALSE;

            /* return retval; */
        }
        else
        {
            /* Update object size sent & remaining to send */
            _sent += actual;
            _remaining = _fsize - _sent;

            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_info.length != actual)
            {
                printf("read length = %d, actual sent = %d\n", body_info.length, actual);
                printf("Adjusting the file read pointer\n");
                (BT_IGNORE_RETURN_VALUE)BT_fops_file_seek(fp, _sent, SEEK_SET);
            }

            LOG_DEBUG("=== appl_bpp_sender_cancel_job, _remaining: %d\n ==== ", _remaining);

            if (0U == _remaining)
            {
                /* Reset the variables */
                _sent = 0U;
                _fsize = 0U;

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }

            if (NULL != body_info.value)
            {
                BT_free_mem(body_info.value);
                body_info.value = NULL;
            }
        }
    }

    return retval;
}

#endif /* BPP_SENDER */

