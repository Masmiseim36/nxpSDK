
/**
 *  \file appl_bpp_printer.c
 *
 *  Source file for BPP Printer Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_bpp.h"
#include "appl_utils.h"

#ifdef BPP_PRINTER
/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static UCHAR         bd_addr[BT_BD_ADDR_SIZE];
static BPP_INSTANCE  bpp_printer_jobch_instance[BPP_NUM_PRINTER_INSTANCE];
static BPP_INSTANCE  bpp_printer_statusch_instance[BPP_NUM_PRINTER_INSTANCE];
static BPP_INSTANCE  bpp_printer_objectch_instance[BPP_NUM_PRINTER_INSTANCE];
static UCHAR         bpp_push_started;

static BT_fops_file_handle  bpp_rx_fp;
static BT_fops_file_handle  bpp_tx_fp;

static UINT32       fsize;
static UINT32       remaining, sent;
static UINT32       bpp_xchg_size;
static UINT32       received_bytes;

static UCHAR        appl_reject_opn;
static UCHAR        appl_reject_action;
static UCHAR        appl_bpp_printer_read_only;

static UCHAR                bpp_attrib_data[BPP_SDP_RECORD_DATA_SIZE];
static UINT16               bpp_attrib_data_len = BPP_SDP_RECORD_DATA_SIZE;
static SDP_HANDLE           bpp_sdp_handle;
static UCHAR                bpp_server_channel;
static BPP_APPL_PARAMS      appl_param;
static BPP_CONNECT_STRUCT   connect_info;
static UCHAR                file_name[32U] ;
static UCHAR                channel_type;

static UCHAR                appl_flag_2_accept_push_without_http = 0U;
static UINT8                enable_auth;

static UINT8                appl_get_event_rsp_flag = 0x00U;

#ifdef BPP_HAVE_OBEX_AUTHENTICATION
static APPL_BPP_SER_USER_INFO user_info;

static UCHAR  user_name[] = "PTS";
static UCHAR  pass_word[] = "0000";
#endif /* BPP_HAVE_OBEX_AUTHENTICATION */

static const UCHAR bpp_printer_menu[] =
" \n \
-------------- BPP Printer Menu --------------\n \
\n \
\t 0. Exit \n \
\t 1. Refresh \n \
\n \
\t 2. BPP Printer Initialize \n \
\t 3. BPP Printer Shutdown \n \
\t 4. BPP Printer Start \n \
\t 5. BPP Printer Stop \n \
\n \
\t 6. BPP Printer Reject Operation\n\
\n \
\t10. Set Printer Mode\n\
\n \
\t12. Do SDP Query \n\
\n \
\t15. Connect to BPP Sender \n \
\t16. Disconnect from BPP Sender \n \
\t17. Close Transport Connection with BPP Sender \n \
\n \
\t20. Get Referenced Object \n \
\n \
\t40. Reject the Reference without HTTP Auth. Header \n \
\t41. Set Get_Event Response Status \n \
Your Choice: ";

/* ----------------------------------------- Functions */

void main_bpp_printer_operations (void)
{
    API_RESULT retval;
    UINT16 i;
    UINT8 *phandle;
    UINT16 size;
    int choice, menu_choice, handle, input , service_type, server_ch, val;
    BPP_REQUEST_STRUCT req_info;
    BPP_HEADER_STRUCT name_info;
    UINT16 file_name_len;

    BT_IGNORE_UNUSED_PARAM(appl_reject_action);

    retval = API_SUCCESS;
    phandle = NULL;

#ifdef BPP_HAVE_OBEX_AUTHENTICATION
    /* Set one user ID and Password */
    user_info.user_id = user_name;
    user_info.passwd = pass_word;

    printf("Enable Authentication (0/1): ");

#ifndef APPL_INPUT_FROM_STRUCT
    scanf("%d", &input);
    enable_auth = (UCHAR)input;
#else
    scanf("%d", &input);
    enable_auth = (UCHAR)input;
#endif

#else
    enable_auth = 0x00U;
#endif /* BPP_HAVE_OBEX_AUTHENTICATION */

    BT_LOOP_FOREVER()
    {
        printf ("%s", bpp_printer_menu);
        scanf ("%d", &choice);

        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            printf ("Initializing BPP Server...\n");
            retval = BT_bpp_printer_init ();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                /* Reset BD_ADDR & BPP handle */
                for (i = 0U; i < BPP_NUM_PRINTER_INSTANCE; i++)
                {
                    BPP_RESET_INSTANCE(&bpp_printer_jobch_instance[i]);
                    BPP_RESET_INSTANCE(&bpp_printer_statusch_instance[i]);
                    BPP_RESET_INSTANCE(&bpp_printer_objectch_instance[i]);
                }
            }

            appl_reject_opn = 0U;
            appl_reject_action = 0U;
            appl_bpp_printer_read_only = 0U;

            bpp_printer_print_appl_instances (BPP_JOB_CHANNEL);
            bpp_printer_print_appl_instances (BPP_STATUS_CHANNEL);
            break;

        case 3:
            LOG_DEBUG ("Shutting down BPP Server...\n");
            retval = BT_bpp_printer_shutdown ();
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 4:
            LOG_DEBUG ("Enter BPP Server instance:");
            scanf ("%d", &handle);

            if (BPP_NUM_PRINTER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            LOG_DEBUG ("Enter Service Type \n");
            LOG_DEBUG ("    0 -> Direct Printing \n");
            LOG_DEBUG ("    1 -> Print by Reference \n");
            LOG_DEBUG ("    2 -> RUI \n ");
            scanf ("%d", &service_type);

            LOG_DEBUG ("Enter Channel Type \n");
            LOG_DEBUG ("    0 -> Job Channel \n");
            LOG_DEBUG ("    1 -> Status Channel \n");
            LOG_DEBUG ("    2 -> Object Channel \n");
            scanf ("%d", &input);

            channel_type = (UCHAR ) input;

            /* BPP applicaion instance handle */

            if(0U == channel_type)
            {
                phandle = &bpp_printer_jobch_instance[handle].handle;
            }
            else if(1U == channel_type)
            {
                phandle = &bpp_printer_statusch_instance[handle].handle;
            }
            else if(2U == channel_type)
            {
                phandle = &bpp_printer_objectch_instance[handle].handle;
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

            LOG_DEBUG ("Starting BPP Server...\n");
            retval = BT_bpp_printer_start
                     (
                         phandle,
                         appl_bpp_printer_callback,
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
                if(0U == channel_type)
                {
                    /* Set path to root folder */
                    BT_str_copy
                    (
                        bpp_printer_jobch_instance[handle].path,
                        BPP_ROOT_FOLDER_BASE
                    );

                    bpp_printer_jobch_instance[handle].root_depth = 0U;
                }
                else if(1U == channel_type)
                {
                    /* Set path to root folder */
                    BT_str_copy
                    (
                        bpp_printer_statusch_instance[handle].path,
                        BPP_ROOT_FOLDER_BASE
                    );

                    bpp_printer_statusch_instance[handle].root_depth = 0U;
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }
            }

            bpp_printer_print_appl_instances((UCHAR)channel_type );
            break;

        case 5:
            bpp_printer_print_appl_instances((UCHAR)channel_type);

            LOG_DEBUG ("Enter BPP Printer instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_PRINTER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            LOG_DEBUG ("Enter Channel Type \n");
            LOG_DEBUG ("    0 -> Job Channel \n");
            LOG_DEBUG ("    1 -> Status Channel \n");
            LOG_DEBUG ("    2 -> Object Channel \n");

            scanf ("%d", &input);

            channel_type = (UCHAR)input;

            /* BPP applicaion instance handle */
            if(0U == channel_type)
            {
                phandle = &bpp_printer_jobch_instance[handle].handle;
            }
            else if(1U == channel_type)
            {
                phandle = &bpp_printer_statusch_instance[handle].handle;
            }
            else if(2U == channel_type)
            {
                phandle = &bpp_printer_objectch_instance[handle].handle;
            }
            else
            {
                LOG_DEBUG("\nWrong choice for Channel Type\n");
                break;
            }

            LOG_DEBUG ("Stopping  BPP printer...\n");
            retval = BT_bpp_printer_stop
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
                        bpp_printer_jobch_instance[handle].bd_addr,
                        0x00,
                        BT_BD_ADDR_SIZE
                    );
                }
                else if(1U == channel_type)
                {
                    BT_mem_set
                    (
                        bpp_printer_statusch_instance[handle].bd_addr,
                        0x00,
                        BT_BD_ADDR_SIZE
                    );
                }
                else if(2U == channel_type)
                {
                    BT_mem_set
                    (
                        bpp_printer_objectch_instance[handle].bd_addr,
                        0x00,
                        BT_BD_ADDR_SIZE
                    );
                }
                else
                {
                    LOG_DEBUG("\nWrong choice for Channel Type\n");
                    break;
                }

                /* Close the files if opened */
                if (NULL != bpp_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (bpp_rx_fp);
                    bpp_rx_fp = NULL;
                }

                /* Close the files if opened */
                if (NULL != bpp_tx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (bpp_tx_fp);
                    bpp_tx_fp = NULL;
                }
            }

            bpp_printer_print_appl_instances ((UCHAR)channel_type);
            break;

        case 6:
            LOG_DEBUG ("  0x<Err> -> Error code\n");
            scanf("%x",&input);
            appl_reject_opn = (UCHAR)input;
            break;

        case 10: /* Set Printer Mode */
            LOG_DEBUG("Select Printer Mode: (0: Private Online, 1: Public Online)\n");
            scanf("%d", &input);

            if (0U == input)
            {
                /* Private Online */
                /* Non-discoverable */
                (BT_IGNORE_RETURN_VALUE) BT_hci_write_scan_enable(0x02U);

                LOG_DEBUG("Setting in Private Online Mode ...\n");
            }
            else
            {
                UINT32 iac_lap[2U] = { BT_GIAC, BT_LIAC };

                /* Public Online */
                /* General and Limited Discoverable */
                (BT_IGNORE_RETURN_VALUE) BT_hci_write_current_iac_lap(2U, iac_lap);
                (BT_IGNORE_RETURN_VALUE) BT_hci_write_scan_enable(0x03U);

                LOG_DEBUG("Setting in Public Online Mode ...\n");
            }
            break;

        case 12:
            LOG_DEBUG ("Select BPP Entity Instance: ");
            scanf ("%d", &handle);

            LOG_DEBUG ("Enter peer device address: ");
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

            SDP_SET_HANDLE
            (
                bpp_sdp_handle,
                bd_addr,
                appl_bpp_printer_sdp_callback
            );

            retval = BT_sdp_open(&bpp_sdp_handle);

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("SDP Connect FAILED !! Error Code = 0x%04X\n", retval);
            }

            LOG_DEBUG("Please Wait for SDP Operation to Complete\n");
            break;

        case 15:
            bpp_printer_print_appl_instances(BPP_OBJECT_CHANNEL);
            LOG_DEBUG ("Enter BPP Printer instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_PRINTER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            LOG_DEBUG("Enter Channel Type \n 2. Object Channel \n   ");
            scanf ("%d", &input);

            channel_type = (UCHAR)input;

            /* BPP applicaion instance handle */
            if(channel_type == 2U)
            {
                phandle = &bpp_printer_objectch_instance[handle].handle;
            }
            else
            {
                LOG_DEBUG("\nWrong choice for Channel Type\n");
                break;
            }

            LOG_DEBUG ("Enter peer device address: ");
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

            connect_info.bd_addr = bd_addr;

            LOG_DEBUG ("Enter peer server channel (in Hex): ");
            scanf ("%x", &server_ch);
            connect_info.server_channel = (UCHAR )server_ch;

            LOG_DEBUG ("Enter Data Exchange Size: ");
            scanf ("%d", &val);
            size = (UINT16) val;
            connect_info.max_recv_size = size;

            LOG_DEBUG ("Connecting...\n");
            retval = BT_bpp_printer_connect
                     (
                         phandle,
                         &connect_info,
                         (UCHAR)channel_type
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 16:
            LOG_DEBUG ("Enter BPP Client instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_PRINTER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            LOG_DEBUG ("Enter Channel Type \n");
            LOG_DEBUG ("    2 -> Object Channel \n");
            scanf ("%d", &input);

            channel_type = (UCHAR)input;

            /* BPP applicaion instance handle */
            if(channel_type == 2U)
            {
                phandle = &bpp_printer_objectch_instance[handle].handle;
            }
            else
            {
                LOG_DEBUG("\nWrong choice for Channel Type\n");
                break;
            }

            LOG_DEBUG ("Disconnecting BPP Client Instance %d\n", handle);
            retval = BT_bpp_printer_disconnect
                    (
                        phandle,
                        (UCHAR)channel_type
                    );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 17:
            LOG_DEBUG ("Enter BPP Client instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_PRINTER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            LOG_DEBUG ("Enter Channel Type \n");
            LOG_DEBUG ("    0 -> Job Channel \n");
            LOG_DEBUG ("    1 -> Status Channel \n");
            LOG_DEBUG ("    2 -> Object Channel \n");
            scanf ("%d", &input);

            channel_type = (UCHAR)input;

            /* BPP applicaion instance handle */
            if(0U == channel_type)
            {
                phandle = &bpp_printer_jobch_instance[handle].handle;
            }
            else if(1U == channel_type)
            {
                phandle = &bpp_printer_statusch_instance[handle].handle;
            }
            else if(2U == channel_type)
            {
                phandle = &bpp_printer_objectch_instance[handle].handle;
            }
            else
            {
                LOG_DEBUG("\nWrong choice for Channel Type\n");
                break;
            }

            LOG_DEBUG ("Disconnecting BPP Transport with BPP printer\n");
            retval = BT_bpp_printer_transport_close
                    (
                        phandle,
                        (UCHAR)channel_type
                    );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 20:
            bpp_printer_print_appl_instances (BPP_OBJECT_CHANNEL);
            LOG_DEBUG ("Enter BPP Client instance: ");
            scanf ("%d", &handle);

            if (BPP_NUM_PRINTER_INSTANCE <= handle)
            {
                printf ("Invalid Application Instance\n");
                break;
            }

            BPP_INIT_HEADER_STRUCT (name_info);
            BPP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

            LOG_DEBUG("Enter the Offset value : ");
            scanf("%d", &val);

            BPP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BPP_FLAG_OFFSET
            );
            appl_param.offset = (UINT32)val;

            LOG_DEBUG("Enter the Count value : ");
            scanf("%d", &val);

            BPP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BPP_FLAG_COUNT
            );
            appl_param.count = (INT32)val;

            LOG_DEBUG("Enter the File Size : ");
            scanf("%d", &val);

            BPP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BPP_FLAG_FILE_SIZE
            );
            appl_param.file_size = (INT32)val;

            LOG_DEBUG ("Enter the object name: ");
            scanf ("%s", file_name);
            file_name_len = (UINT16 )BT_str_n_len (file_name, sizeof(file_name));

            /* Fill name hdr values */
            name_info.value = file_name;
            name_info.length = (UINT16 )(file_name_len + 1U);

            req_info.name = &name_info;
            req_info.appl_params = &appl_param;

            received_bytes = 0U;

            LOG_DEBUG ("Requesting to Get Referenced Object...\n");
            retval = BT_bpp_printer_get_ref_obj
                     (
                         &bpp_printer_objectch_instance[handle].handle,
                         &req_info
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
            break;

        case 40:
            LOG_DEBUG("Reject Reference Push without HTTP(0/1)\n");
            scanf("%d", &input);
            appl_flag_2_accept_push_without_http = (UCHAR)input;
            break;

        case 41:
            /* This is required for PTS testcase: BPP/PR/DPS/BV-09-I */
            printf("Set Get_Event response\n");
            printf("    0 -> None\n");
            printf("    1 -> Stopped\n");
            printf("    3 -> Completed\n");
            scanf("%d", &input);
            appl_get_event_rsp_flag = (UCHAR)input;

            break;

        default:
            LOG_DEBUG ("Invalid  Choice.\n");
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


API_RESULT appl_bpp_printer_callback
           (
               BPP_HANDLE   * bpp_handle,
               UINT8          event_type,
               UINT16         event_result,
               BPP_HEADERS  * event_header,
               UINT16         event_hdrlen,
               UCHAR          bpp_channel_type
           )
{
    API_RESULT retval;
    BPP_HANDLE handle;
    UCHAR send_response;
    UCHAR send_request;
    UCHAR more;
    BPP_CONNECT_STRUCT connect_resp;
    BPP_RESPONSE_STRUCT bpp_rsp_info;
    BPP_HEADER_STRUCT body_info;
    BPP_HEADERS *bpp_rx_hdrs;
    BPP_HEADERS bpp_tx_hdrs;
    UCHAR tx_response;
    UINT16 i;
    UINT32 sample_job_id;
    UINT16 length;
    UINT16 actual;
    UCHAR *data;

#ifdef BPP_HAVE_OBEX_AUTHENTICATION
    BPP_HEADER_STRUCT  pin;
    BPP_HEADER_STRUCT  userid;
#endif /* BPP_HAVE_OBEX_AUTHENTICATION */

#ifdef GET_REF_OBJ_HEADERS
    BPP_HEADER_STRUCT name_info;
    UINT16 file_name_len;
    BPP_REQUEST_STRUCT req_info;
#endif /* GET_REF_OBJ_HEADERS */

    static UCHAR file_object[BPP_FOLDER_NAME_LEN * 2U];

    /* Initialize to default values */
    handle = *bpp_handle;
    retval = API_SUCCESS;
    sample_job_id = 12345U;

    BT_mem_set(&bpp_rsp_info, 0, sizeof(BPP_RESPONSE_STRUCT));
    BT_mem_set(&connect_resp, 0, sizeof(BPP_CONNECT_STRUCT));
    BT_mem_set(&bpp_tx_hdrs, 0, sizeof(BPP_HEADERS));
    BPP_INIT_HEADER_STRUCT(body_info);

    send_response = 0U;
    bpp_rx_hdrs = event_header;
    tx_response = (UCHAR )event_result;

    data = NULL;
    length = 0x00U;
    actual = 0x00U;
    send_request = 0x00U;
    more = 0x00U;

#ifdef BPP_HAVE_OBEX_AUTHENTICATION
    /* Initialize */
    pin.value = user_info.passwd;
    pin.length = (UINT16)BT_str_len(user_info.passwd);

    userid.value = user_info.user_id;
    userid.length = (UINT16)BT_str_len(user_info.user_id);

    connect_resp.pin_info = &pin;
    connect_resp.user_id = &userid;
#endif /* BPP_HAVE_OBEX_AUTHENTICATION */

    BT_IGNORE_UNUSED_PARAM(event_hdrlen);

    LOG_DEBUG("channel type ---> %d\n", bpp_channel_type);

    for (i = 0U; i < BPP_NUM_PRINTER_INSTANCE; i++)
    {
        if (bpp_printer_jobch_instance[i].handle == handle)
        {
            break;
        }
        else if (bpp_printer_statusch_instance[i].handle == handle)
        {
            break;
        }
        else if (bpp_printer_objectch_instance[i].handle == handle)
        {
            break;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }

    if (BPP_NUM_PRINTER_INSTANCE == i)
    {
        LOG_DEBUG ("Failed to find application FTO printer instance!\n");
        retval = API_SUCCESS; /* return API_SUCCESS; */
    }
    else
    {
        switch(event_type)
        {
        case BPP_PRINTER_STATUSCH_TRANSPORT_CLOSE_IND: /* Fall Through */
        case BPP_PRINTER_STATUSCH_TRANSPORT_CLOSE_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received %s\n", (BPP_PRINTER_STATUSCH_TRANSPORT_CLOSE_IND == event_type) ?
            "BPP_PRINTER_STATUSCH_TRANSPORT_CLOSE_IND" : "BPP_PRINTER_STATUSCH_TRANSPORT_CLOSE_CNF");
            LOG_DEBUG("BPP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            BT_mem_set (bpp_printer_statusch_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);

            bpp_printer_print_appl_instances (BPP_STATUS_CHANNEL);
            break;

        case BPP_PRINTER_JOBCH_TRANSPORT_CLOSE_IND: /* Fall Through */
        case BPP_PRINTER_JOBCH_TRANSPORT_CLOSE_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received %s\n", (BPP_PRINTER_JOBCH_TRANSPORT_CLOSE_IND == event_type) ?
            "BPP_PRINTER_JOBCH_TRANSPORT_CLOSE_IND" : "BPP_PRINTER_JOBCH_TRANSPORT_CLOSE_CNF");
            LOG_DEBUG("BPP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            BT_mem_set (bpp_printer_jobch_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);

            bpp_printer_print_appl_instances (BPP_JOB_CHANNEL);
            break;

        case BPP_PRINTER_OBJECTCH_TRANSPORT_CLOSE_IND: /* Fall Through */
        case BPP_PRINTER_OBJECTCH_TRANSPORT_CLOSE_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received %s\n", (BPP_PRINTER_OBJECTCH_TRANSPORT_CLOSE_IND == event_type) ?
            "BPP_PRINTER_OBJECTCH_TRANSPORT_CLOSE_IND" : "BPP_PRINTER_OBJECTCH_TRANSPORT_CLOSE_CNF");
            LOG_DEBUG("BPP Printer Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* Reset bd_addr */
            for (i = 0U; i < BPP_NUM_PRINTER_INSTANCE; i++)
            {
                if (bpp_printer_objectch_instance[i].handle == handle)
                {
                    BT_mem_set (bpp_printer_objectch_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);
                }
            }

            bpp_printer_print_appl_instances(BPP_OBJECT_CHANNEL);
            break;

        case BPP_PRINTER_STATUSCH_CONNECT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_STATUSCH_CONNECT_IND\n");
            LOG_DEBUG("BPP Printer Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (bpp_rx_hdrs->bpp_connect_info->bd_addr));

            LOG_DEBUG ("MAX Exchange Size: %d\n",
            bpp_rx_hdrs->bpp_connect_info->max_recv_size);
            tx_response = BPP_SUCCESS_RSP;

            /* Set response to be sent */
            send_response = 1U;

            /* Get the Handle parameters */
            BT_mem_copy
            (
                bpp_printer_statusch_instance[i].bd_addr,
                bpp_rx_hdrs->bpp_connect_info->bd_addr,
                BT_BD_ADDR_SIZE
            );

            bpp_printer_statusch_instance[i].max_xchg_size =
                bpp_rx_hdrs->bpp_connect_info->max_recv_size;

            bpp_printer_print_appl_instances (BPP_STATUS_CHANNEL);

            /* Set path to root folder */
            if (0U != bpp_printer_statusch_instance[i].root_depth)
            {
                while (0U != bpp_printer_statusch_instance[i].root_depth)
                {
                    retval = BT_vfops_set_path_backward(bpp_printer_statusch_instance[i].path);
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to set path backward\n");
                    }
                    bpp_printer_statusch_instance[i].root_depth --;
                }

                LOG_DEBUG ("Folder set to ROOT\n");
            }
            break;

        case BPP_PRINTER_JOBCH_CONNECT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_JOBCH_CONNECT_IND\n");
            LOG_DEBUG("BPP Printer Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            if ((BPP_SUCCESS_RSP != event_result) &&
                (BPP_NOT_AUTHENTICATED != event_result))
            {
                if (BPP_AUTHENTICATION_FAILED == event_result)
                {
                    event_result = BPP_NOT_AUTHENTICATED;
                }
                else
                {
                    event_result = BPP_NOT_ACCEPTABLE_RSP;
                    send_response = 1U;
                    break;
                }
            }

            /* Reject if operation reject */
            if (0U != appl_reject_opn)
            {
                tx_response = appl_reject_opn;
                send_response = 1U;

                LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);
                break;
            }

            LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (bpp_rx_hdrs->bpp_connect_info->bd_addr));

            LOG_DEBUG ("MAX Exchange Size: %d\n",
            bpp_rx_hdrs->bpp_connect_info->max_recv_size);
            tx_response = BPP_SUCCESS_RSP;

            /* Set response to be sent */
            send_response = 1U;

            /* Get the Handle parameters */
            BT_mem_copy
            (
                bpp_printer_jobch_instance[i].bd_addr,
                bpp_rx_hdrs->bpp_connect_info->bd_addr,
                BT_BD_ADDR_SIZE
            );

            bpp_printer_jobch_instance[i].max_xchg_size =
                bpp_rx_hdrs->bpp_connect_info->max_recv_size;

            bpp_printer_print_appl_instances (BPP_JOB_CHANNEL);

            /* Set path to root folder */
            if (0U != bpp_printer_jobch_instance[i].root_depth)
            {
                while (0U != bpp_printer_jobch_instance[i].root_depth)
                {
                    retval = BT_vfops_set_path_backward(bpp_printer_jobch_instance[i].path);
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to set path backward\n");
                    }
                    bpp_printer_jobch_instance[i].root_depth --;
                }

                LOG_DEBUG ("Folder set to ROOT\n");
            }

            if (BPP_NOT_AUTHENTICATED == event_result)
            {
                bpp_tx_hdrs.bpp_connect_info = &connect_resp;

#ifdef BPP_HAVE_OBEX_AUTHENTICATION
                pin.value = user_info.passwd;
                pin.length = (UINT16)BT_str_len(user_info.passwd);

                userid.value = user_info.user_id;
                userid.length = (UINT16)BT_str_len(user_info.user_id);

                connect_resp.pin_info = &pin;
                connect_resp.user_id = &userid;
#endif /* BPP_HAVE_OBEX_AUTHENTICATION */
                if (1U == enable_auth)
                {
                    tx_response = BPP_UNAUTH_RSP;
                }
                else
                {
                    tx_response = BPP_SUCCESS_RSP;
                }
            }
            break;

        case BPP_PRINTER_OBJECTCH_CONNECT_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_OBJECTCH_CONNECT_CNF\n");
            LOG_DEBUG("BPP Sender Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (bpp_rx_hdrs->bpp_connect_info->bd_addr));

            LOG_DEBUG ("MAX Exchange Size: %d\n",
            bpp_rx_hdrs->bpp_connect_info->max_recv_size);

            /* Get the Handle parameters */
            for (i = 0U; i < BPP_NUM_PRINTER_INSTANCE; i++)
            {
                if (bpp_printer_objectch_instance[i].handle == handle)
                {
                    BT_mem_copy
                    (
                        bpp_printer_objectch_instance[i].bd_addr,
                        bpp_rx_hdrs->bpp_connect_info->bd_addr,
                        BT_BD_ADDR_SIZE
                    );

                    bpp_printer_objectch_instance[i].max_xchg_size =
                        bpp_rx_hdrs->bpp_connect_info->max_recv_size;

                    break;
                }
            }

            bpp_printer_print_appl_instances(BPP_OBJECT_CHANNEL);
            break;

        case BPP_PRINTER_STATUSCH_DISCONNECT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_STATUSCH_DISCONNECT_IND\n");
            LOG_DEBUG("BPP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            bpp_printer_print_appl_instances (BPP_STATUS_CHANNEL);
            break;

        case BPP_PRINTER_JOBCH_DISCONNECT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_JOBCH_DISCONNECT_IND\n");
            LOG_DEBUG("BPP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            bpp_printer_print_appl_instances (BPP_JOB_CHANNEL);
            break;

        case BPP_PRINTER_OBJECTCH_DISCONNECT_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_OBJECTCH_DISCONNECT_CNF\n");
            LOG_DEBUG("BPP Printer Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            bpp_printer_print_appl_instances(BPP_OBJECT_CHANNEL);
            break;

        case BPP_PRINTER_FILE_PUSH_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_FILE_PUSH_IND\n");
            LOG_DEBUG("BPP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* Set response to be sent */
            send_response = 1U;

            bpp_tx_hdrs.bpp_rsp_info = &bpp_rsp_info;

            if ((BPP_SUCCESS_RSP != event_result) &&
                (BPP_CONTINUE_RSP != event_result))
            {
                break;
            }

            /* Check if push already ongoing */
            if (0U == bpp_push_started)
            {
                bpp_push_started = 0x01U;

                /* Check for Type hdr in the request */
                if((NULL == bpp_rx_hdrs->bpp_req_info->type_header) ||
                   (NULL == bpp_rx_hdrs->bpp_req_info->type_header->value) ||
                   (0U == bpp_rx_hdrs->bpp_req_info->type_header->length))
                {
                    tx_response = BPP_BAD_REQ_RSP;

                    break;
                }
                else
                {
                    LOG_DEBUG("\nType header : %s\n",bpp_rx_hdrs->bpp_req_info->type_header->value);

                    if (0 != BT_mem_cmp (bpp_rx_hdrs->bpp_req_info->type_header->value,
                                         "image/jpeg",
                                         bpp_rx_hdrs->bpp_req_info->type_header->length))
                    {
                        LOG_DEBUG("\nunsupported format\n");
                        /*tx_response = BPP_UNSUPP_MEDIA_RSP;*/
                    }
                }

                if(NULL != bpp_rx_hdrs->bpp_req_info->name->value)
                {
                    LOG_DEBUG("\nName : %s\n",bpp_rx_hdrs->bpp_req_info->name->value);
                }
                else
                {
                    LOG_DEBUG("\nName not sent\n");
                }

                if(NULL != bpp_rx_hdrs->bpp_req_info->description->value)
                {
                    LOG_DEBUG("\nDescription : %s\n",bpp_rx_hdrs->bpp_req_info->description->value);
                }
                else
                {
                    LOG_DEBUG("\nDescription not sent\n");
                }

                /* Reject if operation reject is set or if Read ONly is set */
                if (0U != appl_bpp_printer_read_only)
                {
                    tx_response = appl_bpp_printer_read_only;
                    LOG_DEBUG("Rejected: Reason - 0x%02X (Read Only)\n", tx_response);

                    break;
                }

                if (0U != appl_reject_opn)
                {
                    tx_response = appl_reject_opn;
                    LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);

                    break;
                }
                else
                {
                    if(NULL != bpp_rx_hdrs->bpp_req_info->name->value)
                    {
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_vfops_create_object_name
                        (
                            (UCHAR *)BPP_ROOT_RX_FOLDER_BASE,
                            bpp_rx_hdrs->bpp_req_info->name->value,
                            file_object
                        );
                    }
                    else
                    {
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_vfops_create_object_name
                        (
                            (UCHAR *)BPP_ROOT_RX_FOLDER_BASE,
                            (UCHAR *)"simple_push_file.txt",
                            file_object
                        );
                    }
                }
            }

            if ((NULL == bpp_rx_hdrs->bpp_req_info->body->value) &&
                (0U == bpp_rx_hdrs->bpp_req_info->body->length))
            {
                LOG_DEBUG ("No Body Content found ");
            }
            else
            {
                length = bpp_rx_hdrs->bpp_req_info->body->length;
                data = bpp_rx_hdrs->bpp_req_info->body->value;

#if 0
                /* Print received data on console */
                LOG_DEBUG ("\n\n ====== BPP Push Object data ===== \n\n");
                for (i = 0; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n\n ================================= \n");
#endif /* 0 */

                /* Open file PUT request */
                if (NULL == bpp_rx_fp)
                {
                    retval = BT_fops_file_open
                             (
                                 (UCHAR *)file_object,
                                 (UCHAR *)"wb",
                                 &bpp_rx_fp
                             );
                    if ((API_SUCCESS != retval) || (NULL == bpp_rx_fp))
                    {
                        LOG_DEBUG ("Failed to open PUSH Object file\n");
                    }
                }

                /* write to file */
                if (NULL != bpp_rx_fp)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bpp_rx_fp, &actual);
                }
            }

            /* Reset */
            if (BPP_SUCCESS_RSP == event_result)
            {
                if (NULL != bpp_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (bpp_rx_fp);
                    bpp_rx_fp = NULL;
                }

                bpp_push_started = 0x00U;
            }
            break;

        case BPP_PRINTER_SEND_DOC_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_SEND_DOC_IND\n");
            LOG_DEBUG("BPP Server Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* Set response to be sent */
            send_response = 1U;

            bpp_tx_hdrs.bpp_rsp_info = &bpp_rsp_info;

            if ((BPP_SUCCESS_RSP != event_result) &&
                (BPP_CONTINUE_RSP != event_result))
            {
                break;
            }

            /* Check if push already ongoing */
            if (0U == bpp_push_started)
            {
                bpp_push_started = 0x01U;

                /* Check for Type hdr in the request */
                if((NULL == bpp_rx_hdrs->bpp_req_info->type_header) ||
                   (NULL == bpp_rx_hdrs->bpp_req_info->type_header->value) ||
                   (0U == bpp_rx_hdrs->bpp_req_info->type_header->length))
                {
                    tx_response = BPP_BAD_REQ_RSP;

                    break;
                }
                else
                {
                    LOG_DEBUG("Type header : %s\n",bpp_rx_hdrs->bpp_req_info->type_header->value);
#if 0
                    if (0 != BT_mem_cmp (bpp_rx_hdrs->bpp_req_info->type_header->value,
                                         "image/jpeg",
                                         bpp_rx_hdrs->bpp_req_info->type_header->length))
                       {
                            LOG_DEBUG("\nUnsupported Format\n");
                       }
#endif /* 0 */
                }

                if(NULL != bpp_rx_hdrs->bpp_req_info->name->value)
                {
                    LOG_DEBUG("\nName : %s\n",bpp_rx_hdrs->bpp_req_info->name->value);
                }
                else
                {
                    LOG_DEBUG("\nName not sent\n");
                }

                if(NULL != bpp_rx_hdrs->bpp_req_info->description->value)
                {
                    LOG_DEBUG("\n Description : %s\n",bpp_rx_hdrs->bpp_req_info->description->value);
                }
                else
                {
                    LOG_DEBUG("\nDescription not sent\n");
                }

                LOG_DEBUG("JOB ID : %d\n", bpp_rx_hdrs->bpp_req_info->appl_params->job_id);

                /* Reject if operation reject is set or if Read ONly is set */
                if (0U != appl_bpp_printer_read_only)
                {
                    tx_response = appl_bpp_printer_read_only;
                    LOG_DEBUG("Rejected: Reason - 0x%02X (Read Only)\n", tx_response);

                    break;
                }

                if (0U != appl_reject_opn)
                {
                    tx_response = appl_reject_opn;
                    LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);

                    break;
                }
                else
                {
                    if(NULL != bpp_rx_hdrs->bpp_req_info->name->value)
                    {
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_vfops_create_object_name
                        (
                            (UCHAR *)BPP_ROOT_RX_FOLDER_BASE,
                            bpp_rx_hdrs->bpp_req_info->name->value,
                            file_object
                        );
                    }
                    else
                    {
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_vfops_create_object_name
                        (
                            (UCHAR *)BPP_ROOT_RX_FOLDER_BASE,
                            (UCHAR *)"send_doc_request.txt",
                            file_object
                        );
                    }

                    /* Open file PUT request */
                    if (NULL == bpp_rx_fp)
                    {
                        LOG_DEBUG("Opening: %s\n", file_object);
                        retval = BT_fops_file_open
                                 (
                                     (UCHAR *)file_object,
                                     (UCHAR *)"wb",
                                     &bpp_rx_fp
                                 );
                        if ((API_SUCCESS != retval) || (NULL == bpp_rx_fp))
                        {
                            LOG_DEBUG("Failed to open PUSH Object file\n");
                        }
                    }
                }
            }

            if ((NULL == bpp_rx_hdrs->bpp_req_info->body->value) &&
                (0U == bpp_rx_hdrs->bpp_req_info->body->length))
            {
                LOG_DEBUG ("No Body Content found ");
            }
            else
            {
                length = bpp_rx_hdrs->bpp_req_info->body->length;
                data = bpp_rx_hdrs->bpp_req_info->body->value;

#if 1
                /* Print received data on console */
                LOG_DEBUG ("\n====== BPP Push Object data =====\n");
                for (i = 0U; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n=================================\n");
#endif /* 0 */

                /* write to file */
                if (NULL != bpp_rx_fp)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bpp_rx_fp, &actual);
                }
            }

            /* Reset */
            if (BPP_SUCCESS_RSP == event_result)
            {
                if (NULL != bpp_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (bpp_rx_fp);
                    bpp_rx_fp = NULL;
                }

                bpp_push_started = 0x00U;
            }

            break;

        case BPP_PRINTER_SIMPLE_REF_PUSH_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_SIMPLE_REF_PUSH_IND\n");
            LOG_DEBUG("BPP printer Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* Set response to be sent */
            send_response = 1U;

            bpp_tx_hdrs.bpp_rsp_info = &bpp_rsp_info;

            if ((BPP_SUCCESS_RSP != event_result) &&
                (BPP_CONTINUE_RSP != event_result))
            {
                break;
            }

            /* Check if push already ongoing */
            if (0U == bpp_push_started)
            {
                /* Check for Type hdr in the request */
                if((NULL == bpp_rx_hdrs->bpp_req_info->type_header) ||
                   (NULL == bpp_rx_hdrs->bpp_req_info->type_header->value) ||
                   (0U == bpp_rx_hdrs->bpp_req_info->type_header->length))
                {
                    tx_response = BPP_BAD_REQ_RSP;
                    break;
                }
                else
                {
                    LOG_DEBUG("Type header : %s\n",bpp_rx_hdrs->bpp_req_info->type_header->value);
                }

                if ((NULL == bpp_rx_hdrs->bpp_req_info->http_authorization_header) ||
                    (NULL == bpp_rx_hdrs->bpp_req_info->http_authorization_header->value) ||
                    (0U == bpp_rx_hdrs->bpp_req_info->http_authorization_header->length))
                {

                    LOG_DEBUG("http header not recvd\n");

                    if (0x01U == appl_flag_2_accept_push_without_http)
                    {
                        LOG_DEBUG("Sending:BPP_UNAUTH_RSP\n");
                        tx_response = BPP_UNAUTH_RSP;
                        break;
                    }

                    /* tx_response = BPP_BAD_REQ_RSP;*/
                    /* break; */
                }
                else
                {
                    LOG_DEBUG("http_authorization_header  - %s\n", bpp_rx_hdrs->bpp_req_info->http_authorization_header->value);
                }

                /* Reject if operation reject is set or if Read ONly is set */
                if (0U != appl_bpp_printer_read_only)
                {
                    tx_response = appl_bpp_printer_read_only;
                    LOG_DEBUG("Rejected: Reason - 0x%02X (Read Only)\n", tx_response);
                    break;
                }

                if (0U != appl_reject_opn)
                {
                    tx_response = appl_reject_opn;
                    LOG_DEBUG("Rejected: Reason - 0x%02X\n", tx_response);
                    break;
                }
                else
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)BPP_ROOT_RX_FOLDER_BASE,
                        (UCHAR *)"bpp_simple_ref_push_soap_msg.txt",
                        file_object
                    );
                }

                bpp_push_started = 0x01U;
            }

            if ((NULL == bpp_rx_hdrs->bpp_req_info->body->value) &&
                (0U == bpp_rx_hdrs->bpp_req_info->body->length))
            {
                LOG_DEBUG ("No Body Content found ");
            }
            else
            {
                length = bpp_rx_hdrs->bpp_req_info->body->length;
                data = bpp_rx_hdrs->bpp_req_info->body->value;

#if 1
                /* Print received data on console */
                LOG_DEBUG ("\n====== BPP Push Object data, length:%d =====\n", length);
                for (i = 0U; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n=================================\n");
#endif /* 0 */

                /* Open file PUT request */
                if (NULL == bpp_rx_fp)
                {
                    retval = BT_fops_file_open
                             (
                                 (UCHAR *)file_object,
                                 (UCHAR *)"wb",
                                 &bpp_rx_fp
                             );
                    if ((API_SUCCESS != retval) || (NULL == bpp_rx_fp))
                    {
                        LOG_DEBUG ("Failed to open PUSH Object file\n");
                    }
                }

                /* write to file */
                if (NULL != bpp_rx_fp)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bpp_rx_fp, &actual);
                }
            }

            /* Reset */
            if (BPP_SUCCESS_RSP == event_result)
            {
                if (NULL != bpp_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (bpp_rx_fp);
                    bpp_rx_fp = NULL;
                }

                bpp_push_started = 0x00U;
            }

            break;

        case BPP_PRINTER_SEND_REF_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_SEND_REF_IND\n");
            LOG_DEBUG("BPP printer Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            /* Set response to be sent */
            send_response = 1U;

            bpp_tx_hdrs.bpp_rsp_info = &bpp_rsp_info;

            if ((BPP_SUCCESS_RSP != event_result) &&
                (BPP_CONTINUE_RSP != event_result))
            {
                break;
            }

            /* Check if push already ongoing */
            if (0U == bpp_push_started)
            {
                bpp_push_started = 0x01U;

                /* Check for Type hdr in the request */
                if((NULL == bpp_rx_hdrs->bpp_req_info->type_header) ||
                   (NULL == bpp_rx_hdrs->bpp_req_info->type_header->value) ||
                   (0U == bpp_rx_hdrs->bpp_req_info->type_header->length))
                {
                    tx_response = BPP_BAD_REQ_RSP;

                    break;
                }
                else
                {
                    LOG_DEBUG("\nType header : %s\n",bpp_rx_hdrs->bpp_req_info->type_header->value);
                }
                LOG_DEBUG("JOB ID : %d\n", bpp_rx_hdrs->bpp_req_info->appl_params->job_id);

                /* Reject if operation reject is set or if Read ONly is set */
                if (0U != appl_bpp_printer_read_only)
                {
                    tx_response = appl_bpp_printer_read_only;
                    LOG_DEBUG("Rejected: Reason - 0x%02X (Read Only)\n", tx_response);

                    break;
                }
                if (0U != appl_reject_opn)
                {
                    tx_response = appl_reject_opn;
                    LOG_DEBUG("Rejected  : Reason - 0x%02X\n", tx_response);

                    break;
                }
                else
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        bpp_printer_jobch_instance[i].path,
                        (UCHAR *)"bpp_send_ref_request.txt",
                        file_object
                    );
                }
            }

            if ((NULL == bpp_rx_hdrs->bpp_req_info->body->value) &&
                (0U == bpp_rx_hdrs->bpp_req_info->body->length))
            {
                LOG_DEBUG ("No Body Content found ");
            }
            else
            {
                length = bpp_rx_hdrs->bpp_req_info->body->length;
                data = bpp_rx_hdrs->bpp_req_info->body->value;

#if 1
                /* Print received data on console */
                LOG_DEBUG ("\n\n ====== BPP Push Object data ===== \n\n");
                for (i = 0U; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n\n ================================= \n");
#endif /* 0 */

                /* Open file PUT request */
                if (NULL == bpp_rx_fp)
                {
                    retval = BT_fops_file_open
                             (
                                 (UCHAR *)file_object,
                                 (UCHAR *)"wb",
                                 &bpp_rx_fp
                             );
                    if ((API_SUCCESS != retval) || (NULL == bpp_rx_fp))
                    {
                        LOG_DEBUG ("Failed to open PUSH Object file\n");
                    }
                }

                /* write to file */
                if (NULL != bpp_rx_fp)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, bpp_rx_fp, &actual);
                }
            }

            /* Reset */
            if (BPP_SUCCESS_RSP == event_result)
            {
                if (NULL != bpp_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (bpp_rx_fp);
                    bpp_rx_fp = NULL;
                }

                bpp_push_started = 0x00U;
            }
            break;

        case BPP_PRINTER_CREATE_JOB_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_CREATE_JOB_IND \n");
            LOG_DEBUG("BPP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            BPP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
            bpp_rsp_info.body             = &body_info;
            bpp_rsp_info.appl_params      = &appl_param;
            bpp_tx_hdrs.bpp_rsp_info      = &bpp_rsp_info;


            /* set application parameter */
            BPP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BPP_FLAG_JOB_ID
            );
            appl_param.job_id = (UINT32) sample_job_id;

            LOG_DEBUG("JOB ID: %d\n", bpp_rsp_info.appl_params->job_id);

            if ((NULL != bpp_rx_hdrs->bpp_req_info->body->value) &&
                (0U != bpp_rx_hdrs->bpp_req_info->body->length))
            {
                length = bpp_rx_hdrs->bpp_req_info->body->length;
                data = bpp_rx_hdrs->bpp_req_info->body->value;

                /* Print received data on console */
                LOG_DEBUG ("\n ====== BPP CREATE JOB REQUEST  ===== \n");
                for (i = 0U; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n ================================= \n");
            }

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
                bpp_xchg_size = bpp_printer_jobch_instance[handle].max_xchg_size;
                tx_response = BPP_SUCCESS_RSP;

                /* Create the listing file with path */
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    bpp_printer_jobch_instance[handle].path,
                    (UCHAR *)"create_job_response.txt",
                    file_object
                );

                /* Get the Sample listing from file */
                retval = BT_fops_file_open ((UCHAR *)file_object, (UCHAR *)"rb", &bpp_tx_fp);
                if ((API_SUCCESS == retval) && (NULL != bpp_tx_fp))
                {
                    /* Get the file size */
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bpp_tx_fp, &fsize);

                    remaining = fsize;
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
                if ( BPP_CONTINUE_RSP == event_result)
                {
                    tx_response = BPP_CONTINUE_RSP;
                }
                else
                {
                    tx_response = BPP_SUCCESS_RSP;
                }

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

        case BPP_PRINTER_CREATE_PR_JOB_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_CREATE_PR_JOB_IND \n");
            LOG_DEBUG("BPP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            BPP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
            bpp_rsp_info.body             = &body_info;
            bpp_rsp_info.appl_params      = &appl_param;
            bpp_tx_hdrs.bpp_rsp_info      = &bpp_rsp_info;

            /* set application parameter */
            BPP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                BPP_FLAG_JOB_ID
            );
            appl_param.job_id = (UINT32) sample_job_id;

            LOG_DEBUG("JOB ID: %d\n", bpp_rsp_info.appl_params->job_id);

            if ((NULL != bpp_rx_hdrs->bpp_req_info->body->value) &&
                (0U != bpp_rx_hdrs->bpp_req_info->body->length))
            {
                length = bpp_rx_hdrs->bpp_req_info->body->length;
                data = bpp_rx_hdrs->bpp_req_info->body->value;

                /* Print received data on console */
                LOG_DEBUG ("\n ====== BPP CREATE PRECISE JOB REQUEST,legth:%d  ===== \n", length);
                for (i = 0U; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n ================================= \n");
            }

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
                bpp_xchg_size = bpp_printer_jobch_instance[handle].max_xchg_size;
                tx_response = BPP_SUCCESS_RSP;

                /* Create the listing file with path */
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    bpp_printer_jobch_instance[handle].path,
                    (UCHAR *)"create_precise_job_response.txt",
                    file_object
                );

                /* Get the Sample listing from file */
                retval = BT_fops_file_open ((UCHAR *)file_object, (UCHAR *)"rb", &bpp_tx_fp);
                if ((API_SUCCESS == retval) && (NULL != bpp_tx_fp))
                {
                    /* Get the file size */
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bpp_tx_fp, &fsize);
                    remaining = fsize;
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
                if ( BPP_CONTINUE_RSP == event_result)
                {
                    tx_response = BPP_CONTINUE_RSP;
                }
                else
                {
                    tx_response = BPP_SUCCESS_RSP;
                }
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

        case BPP_PRINTER_GET_PRINTER_ATTRS_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_GET_PRINTER_ATTRS_IND \n");
            LOG_DEBUG("BPP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            bpp_rsp_info.body             = &body_info;
            bpp_tx_hdrs.bpp_rsp_info      = &bpp_rsp_info;
#if 0
            /*check if event_result is countinue response */
            if(BPP_CONTINUE_RSP == event_result)
            {
                tx_response = BPP_CONTINUE_RSP;
                break;
            }
#endif
            if ((NULL != bpp_rx_hdrs->bpp_req_info->body->value) &&
                (0U != bpp_rx_hdrs->bpp_req_info->body->length))
            {
                length = bpp_rx_hdrs->bpp_req_info->body->length;
                data = bpp_rx_hdrs->bpp_req_info->body->value;

                /* Print received data on console */
                LOG_DEBUG ("\n ====== BPP GET PRINTER ATTRS REQUEST  ===== \n\n");
                for (i = 0U; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n ================================= \n");
            }

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
                bpp_xchg_size = bpp_printer_jobch_instance[handle].max_xchg_size;
                tx_response = BPP_SUCCESS_RSP;

                /* Create the listing file with path */
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    bpp_printer_jobch_instance[handle].path,
                    (UCHAR *)"get_prt_attribs_response.txt",
                    file_object
                );

                /* Get the Sample listing from file */
                retval = BT_fops_file_open ((UCHAR *)file_object, (UCHAR *)"rb", &bpp_tx_fp);
                if ((API_SUCCESS == retval) && (NULL != bpp_tx_fp))
                {
                    /* Get the file size */
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bpp_tx_fp, &fsize);

                    remaining = fsize;
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
                if ( BPP_CONTINUE_RSP == event_result)
                {
                    tx_response = BPP_CONTINUE_RSP;
                }
                else
                {
                    tx_response = BPP_SUCCESS_RSP;
                }
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

        case BPP_PRINTER_GET_JOB_ATTRS_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_GET_JOB_ATTRS_IND \n");
            LOG_DEBUG("BPP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            bpp_rsp_info.body             = &body_info;
            bpp_tx_hdrs.bpp_rsp_info      = &bpp_rsp_info;

#if 0
            /*check if event_result is countinue response */
            if(BPP_CONTINUE_RSP == event_result)
            {
                tx_response = BPP_CONTINUE_RSP;
                break;
            }
#endif
            if ((NULL != bpp_rx_hdrs->bpp_req_info->body->value) &&
                (0U != bpp_rx_hdrs->bpp_req_info->body->length))
            {
                length = bpp_rx_hdrs->bpp_req_info->body->length;
                data = bpp_rx_hdrs->bpp_req_info->body->value;

                /* Print received data on console */
                LOG_DEBUG ("\n ====== BPP GET JOB ATTRS REQUEST  ===== \n");
                for (i = 0U; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n ================================= \n");
            }

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
                bpp_xchg_size = bpp_printer_jobch_instance[handle].max_xchg_size;
                tx_response = BPP_SUCCESS_RSP;

                /* Create the listing file with path */
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    bpp_printer_jobch_instance[handle].path,
                    (UCHAR *)"get_job_attribs_response.txt",
                    file_object
                );

                /* Get the Sample listing from file */
                retval = BT_fops_file_open ((UCHAR *)file_object, (UCHAR *)"rb", &bpp_tx_fp);
                if ((API_SUCCESS == retval) && (NULL != bpp_tx_fp))
                {
                    /* Get the file size */
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bpp_tx_fp, &fsize);

                    remaining = fsize;
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
                if ( BPP_CONTINUE_RSP == event_result)
                {
                    tx_response = BPP_CONTINUE_RSP;
                }
                else
                {
                    tx_response = BPP_SUCCESS_RSP;
                }
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

        case BPP_PRINTER_CANCEL_JOB_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_CANCEL_JOB_IND \n");
            LOG_DEBUG("BPP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            bpp_rsp_info.body             = &body_info;
            bpp_tx_hdrs.bpp_rsp_info      = &bpp_rsp_info;
#if 0
            /*check if event_result is countinue response */
            if(BPP_CONTINUE_RSP == event_result)
            {
                tx_response = BPP_CONTINUE_RSP;
                break;
            }
#endif
            if ((NULL != bpp_rx_hdrs->bpp_req_info->body->value) &&
                (0U != bpp_rx_hdrs->bpp_req_info->body->length))
            {
                length = bpp_rx_hdrs->bpp_req_info->body->length;
                data = bpp_rx_hdrs->bpp_req_info->body->value;

                /* Print received data on console */
                LOG_DEBUG ("\n ====== BPP CANCEL JOB REQUEST  ===== \n\n");
                for (i = 0U; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n ================================= \n");
            }

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
                bpp_xchg_size = bpp_printer_jobch_instance[handle].max_xchg_size;
                tx_response = BPP_SUCCESS_RSP;

                /* Create the listing file with path */
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    bpp_printer_jobch_instance[handle].path,
                    (UCHAR *)"cancel_job_response.txt",
                    file_object
                );

                /* Get the Sample listing from file */
                retval = BT_fops_file_open ((UCHAR *)file_object, (UCHAR *)"rb", &bpp_tx_fp);
                if ((API_SUCCESS == retval) && (NULL != bpp_tx_fp))
                {
                    /* Get the file size */
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bpp_tx_fp, &fsize);

                    remaining = fsize;
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
                if ( BPP_CONTINUE_RSP == event_result)
                {
                    tx_response = BPP_CONTINUE_RSP;
                }
                else
                {
                    tx_response = BPP_SUCCESS_RSP;
                }

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

        case BPP_PRINTER_GET_MARGINS_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_GET_MARGINS_IND \n");
            LOG_DEBUG("BPP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            bpp_rsp_info.body             = &body_info;
            bpp_tx_hdrs.bpp_rsp_info      = &bpp_rsp_info;
#if 0
            /*check if event_result is countinue response */
            if(BPP_CONTINUE_RSP == event_result)
            {
                tx_response = BPP_CONTINUE_RSP;
                break;
            }
#endif
            if ((NULL != bpp_rx_hdrs->bpp_req_info->body->value) &&
                (0U != bpp_rx_hdrs->bpp_req_info->body->length))
            {
                length = bpp_rx_hdrs->bpp_req_info->body->length;
                data = bpp_rx_hdrs->bpp_req_info->body->value;

                /* Print received data on console */
                LOG_DEBUG ("\n ====== BPP GET MARGINS REQUEST  ===== \n\n");
                for (i = 0U; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n ================================= \n");
            }

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
                bpp_xchg_size = bpp_printer_jobch_instance[handle].max_xchg_size;
                tx_response = BPP_SUCCESS_RSP;

                /* Create the listing file with path */
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    bpp_printer_jobch_instance[handle].path,
                    (UCHAR *)"get_margins_response.txt",
                    file_object
                );

                /* Get the Sample listing from file */
                retval = BT_fops_file_open ((UCHAR *)file_object, (UCHAR *)"rb", &bpp_tx_fp);
                if ((API_SUCCESS == retval) && (NULL != bpp_tx_fp))
                {
                    /* Get the file size */
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bpp_tx_fp, &fsize);

                    remaining = fsize;
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
                if ( BPP_CONTINUE_RSP == event_result)
                {
                    tx_response = BPP_CONTINUE_RSP;
                }
                else
                {
                    tx_response = BPP_SUCCESS_RSP;
                }
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

        case BPP_PRINTER_GET_EVENT_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_GET_EVENT_IND \n");
            LOG_DEBUG("BPP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            bpp_rsp_info.body             = &body_info;
            bpp_tx_hdrs.bpp_rsp_info      = &bpp_rsp_info;
#if 0
            /*check if event_result is countinue response */
            if(BPP_CONTINUE_RSP == event_result)
            {
                tx_response = BPP_CONTINUE_RSP;
                break;
            }
#endif
            /**
             * After sending print stop status.
             * Send empty response with Status: BPP_CONTINUE_RSP.
             */
            if (0x02U == appl_get_event_rsp_flag)
            {
                tx_response = BPP_CONTINUE_RSP;

                /* send response */
                send_response = 1U;

                break;
            }

            if ((NULL != bpp_rx_hdrs->bpp_req_info->body->value) &&
                (0U != bpp_rx_hdrs->bpp_req_info->body->length))
            {
                length = bpp_rx_hdrs->bpp_req_info->body->length;
                data = bpp_rx_hdrs->bpp_req_info->body->value;

                /* Print received data on console */
                LOG_DEBUG ("\n ====== BPP GET EVENT REQUEST  ===== \n\n");
                for (i = 0U; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n ================================= \n");
            }

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
                bpp_xchg_size = bpp_printer_statusch_instance[handle].max_xchg_size;

                if (0x01U == appl_get_event_rsp_flag)
                {
                    tx_response = BPP_CONTINUE_RSP;

                    /**
                     * Set to 0x02, to send empty Get response with status BPP_CONTINUE_RSP.
                     */
                    appl_get_event_rsp_flag = 0x02U;

                    /* Send the Get Event response with Job state as Stopped */

                    /* Create the listing file with path */
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        bpp_printer_statusch_instance[handle].path,
                        (UCHAR *)"get_event_rsp_job_stopped.txt",
                        file_object
                    );
                }
                else if (0x3U == appl_get_event_rsp_flag)
                {
                    tx_response = BPP_CONTINUE_RSP;

                    /* Send the Get Event response with Job state as Completed */

                    /* Create the listing file with path */
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        bpp_printer_statusch_instance[handle].path,
                        (UCHAR *)"get_event_rsp_job_completed.txt",
                        file_object
                    );
                }
                else
                {
                    tx_response = BPP_SUCCESS_RSP;

                    /* Create the listing file with path */
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        bpp_printer_statusch_instance[handle].path,
                        (UCHAR *)"get_event_response.txt",
                        file_object
                    );
                }

                /* Get the Sample listing from file */
                retval = BT_fops_file_open ((UCHAR *)file_object, (UCHAR *)"rb", &bpp_tx_fp);
                if ((API_SUCCESS == retval) && (NULL != bpp_tx_fp))
                {
                    /* Get the file size */
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bpp_tx_fp, &fsize);

                    remaining = fsize;
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
                if ( BPP_CONTINUE_RSP == event_result)
                {
                    tx_response = BPP_CONTINUE_RSP;
                }
                else
                {
                    tx_response = BPP_SUCCESS_RSP;
                }
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

        case BPP_PRINTER_GET_RUI_IND:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_GET_RUI_IND \n");
            LOG_DEBUG("BPP Responder Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            bpp_rsp_info.body             = &body_info;
            bpp_tx_hdrs.bpp_rsp_info      = &bpp_rsp_info;
#if 0
            /*check if event_result is countinue response */
            if(BPP_CONTINUE_RSP == event_result)
            {
                tx_response = BPP_CONTINUE_RSP;
                break;
            }
#endif
            if ((NULL != bpp_rx_hdrs->bpp_req_info->body->value) &&
                (0U != bpp_rx_hdrs->bpp_req_info->body->length))
            {
                length = bpp_rx_hdrs->bpp_req_info->body->length;
                data = bpp_rx_hdrs->bpp_req_info->body->value;

                /* Print received data on console */
                LOG_DEBUG ("\n ====== BPP GET RUI  REQUEST  ===== \n");
                for (i = 0U; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n ================================= \n");
            }
            if(NULL != bpp_rx_hdrs->bpp_req_info->name->value)
            {
                LOG_DEBUG("\nName : %s\n",bpp_rx_hdrs->bpp_req_info->name->value);
            }
            else
            {
                LOG_DEBUG("\nName not sent\n");
            }

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
                LOG_DEBUG("\nhttp_accept_header  - %s\n", bpp_rx_hdrs->bpp_req_info->http_accept_header->value);
                LOG_DEBUG("\nhttp_accept_lang_header  - %s\n", bpp_rx_hdrs->bpp_req_info->http_accept_lang_header->value);

                bpp_xchg_size = bpp_printer_jobch_instance[handle].max_xchg_size;
                tx_response = BPP_SUCCESS_RSP;

                /* Create the listing file with path */
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    bpp_printer_jobch_instance[handle].path,
                    (UCHAR *)"get_event_response.txt",
                    file_object
                );

                /* Get the Sample listing from file */
                retval = BT_fops_file_open ((UCHAR *)file_object, (UCHAR *)"rb", &bpp_tx_fp);
                if ((API_SUCCESS == retval) && (NULL != bpp_tx_fp))
                {
                    /* Get the file size */
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(bpp_tx_fp, &fsize);

                    remaining = fsize;
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
                if ( BPP_CONTINUE_RSP == event_result)
                {
                    tx_response = BPP_CONTINUE_RSP;
                }
                else
                {
                    tx_response = BPP_SUCCESS_RSP;
                }
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

        case BPP_PRINTER_GET_REF_OBJ_CNF:
            LOG_DEBUG("\n");
            LOG_DEBUG("Received BPP_PRINTER_GET_REF_OBJ_CNF\n");
            LOG_DEBUG("BPP Printer Instance: %d\n", handle);
            LOG_DEBUG("Result = 0x%04X\n", event_result);

            if ((BPP_CONTINUE_RSP != event_result) &&
                (BPP_SUCCESS_RSP != event_result))
            {
                if (NULL != bpp_rx_fp)
                {
                    (void)BT_fops_file_close(bpp_rx_fp);
                    bpp_rx_fp = NULL;
                }

                break;
            }

            if ((NULL == bpp_rx_hdrs->bpp_rsp_info->body) ||
                (NULL == bpp_rx_hdrs->bpp_rsp_info->body->value) ||
                (0U == bpp_rx_hdrs->bpp_rsp_info->body->length))
            {
                LOG_DEBUG ("No Body Content found ");
            }
            else
            {
                data = bpp_rx_hdrs->bpp_rsp_info->body->value;
                length = bpp_rx_hdrs->bpp_rsp_info->body->length;
#if 0
                /* print received data on console */
                LOG_DEBUG ("\n ====== BPP GET REF OBJ RESPONSE ===== \n");
                for (i = 0; i<length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n ================================= \n");
#endif /* 0 */

                LOG_DEBUG("File size : %d\n", bpp_rx_hdrs->bpp_rsp_info->appl_params->file_size);

                /* Open file for dumping received GetImageListing request data */
                if (NULL == bpp_rx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)BPP_ROOT_RX_FOLDER_BASE,
                        file_name, /* TBD: What is this filename? */
                        file_object
                    );

                    retval = BT_fops_file_open
                             (
                                 (UCHAR *)file_object,
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

                if (BPP_CONTINUE_RSP == event_result)
                {
                    received_bytes += length;
                    LOG_DEBUG("received bytes so far = %d\n", received_bytes);

#ifdef GET_REF_OBJ_HEADERS
                    BPP_INIT_HEADER_STRUCT(name_info);
                    file_name_len = 0U;
#endif /* GET_REF_OBJ_HEADERS */

                    BPP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

                    BPP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        BPP_FLAG_OFFSET
                    );
                    appl_param.offset = (UINT32)received_bytes;

                    BPP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        BPP_FLAG_COUNT
                    );
                    appl_param.count = (INT32)length;

                    BPP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        BPP_FLAG_FILE_SIZE
                    );

                    appl_param.file_size = (INT32)bpp_rx_hdrs->bpp_rsp_info->appl_params->file_size;
                    LOG_DEBUG("file_size = %d\n", appl_param.file_size);

#ifdef GET_REF_OBJ_HEADERS
                    file_name_len = (UINT16)BT_str_len(file_name);

                    /**
                     * TODO: Check if name info has to be sent in subsequent requests also
                     */
                    name_info.value = file_name;
                    name_info.length = (UINT16)(file_name_len + 1);

                    req_info.name = &name_info;
                    req_info.appl_params = &appl_param;

                    LOG_DEBUG("Requesting to Get Referenced Object...\n");
                    LOG_DEBUG ("name_info.value = %s\n", name_info.value);
                    LOG_DEBUG("count = %d\n", appl_param.count);

                    bpp_tx_hdrs.bpp_req_info = &req_info;
                    event_header = &bpp_tx_hdrs;
#endif /* GET_REF_OBJ_HEADERS */

                    send_request = 0x01U;

                    /* GET request for GetReferencedObj has to be sent with Final bit set */

                    more = 0x00U;
                }
                else
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    LOG_DEBUG("Closing the file...\n");

                    if (NULL != bpp_rx_fp)
                    {
                        (void)BT_fops_file_close(bpp_rx_fp);
                        bpp_rx_fp = NULL;
                    }
                }
            }
            break;

        default:
            LOG_DEBUG("Invalid Event Type: 0x%02X\n", event_type);
            break;
        }

        /* Send request if required */
        if (0U != send_request)
        {
            retval = BT_bpp_printer_send_request
                     (
                         &handle,
                          event_type,
                          event_result,
                          event_header,
                          bpp_channel_type, /* current_channel, */
                          more,
                         &actual
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("BPP  Sender Send Request Failed - 0x%04X\n", retval);
            }
        }

        /* Send response if required */
        if (0U != send_response)
        {
            retval = BT_bpp_printer_send_response
                     (
                         &handle,
                         event_type,
                         tx_response,
                         &bpp_tx_hdrs,
                         bpp_channel_type
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed to send BPP printer Response - 0x%04x\n", retval);
            }

            /* Free the allocated memory */
            if (NULL != body_info.value)
            {
                BT_free_mem(body_info.value);
                body_info.value = NULL;

                /* MISRA C - 2012 Rule 2.2 */
                /* body_info.length = 0U; */
            }
        }
    }

    return retval;
}

void appl_bpp_printer_sdp_callback
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

    BT_IGNORE_UNUSED_PARAM(length);

#ifndef OBEX_OVER_L2CAP
    UINT16 attrib_id[1U] = {PROTOCOL_DESC_LIST};

    num_attrib_ids = 1U;
#else /* OBEX_OVER_L2CAP */
    UINT16 attrib_id[2U] = {PROTOCOL_DESC_LIST,
                           GOEP_L2CAP_PSM_ATTR_ID};

    num_attrib_ids = 2U;
#endif /* OBEX_OVER_L2CAP */

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
    uuid[0U].uuid_union.uuid_16= BPP_DIRECT_PRINT_REF_OBJ_SERVICE_UUID; /* BPP_REFLECTED_UI_UUID */
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

                /* Check the BPP server channel number */
                retval = BT_sdp_get_channel_number
                            (
                                sdp_query_result,
                                &bpp_server_channel
                            );

                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("SDP FAILED to find BPP Server Channel\n");
                }
                else
                {
                    LOG_DEBUG("Server Channel: 0x%02X\n", bpp_server_channel);

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

void bpp_printer_print_appl_instances (UCHAR bpp_channel_type)
{
    UINT8 i;
    BPP_INSTANCE *bpp_printer_instance;

    bpp_printer_instance = NULL;

    for (i = 0U; i < BPP_NUM_PRINTER_INSTANCE; i++)
    {
        if(BPP_JOB_CHANNEL == bpp_channel_type)
        {
            LOG_DEBUG ("\n\n=========================JOB CHANNEL========================\n");
            bpp_printer_instance = &bpp_printer_jobch_instance[i];
            break;
        }
        else if(BPP_STATUS_CHANNEL == bpp_channel_type)
        {
            LOG_DEBUG ("\n\n=========================STATUS CHANNEL=====================\n");
            bpp_printer_instance = &bpp_printer_statusch_instance[i];
            break;
        }
        else if(BPP_OBJECT_CHANNEL == bpp_channel_type)
        {
            LOG_DEBUG ("\n\n=========================OBJECT CHANNEL=====================\n");
            bpp_printer_instance = &bpp_printer_objectch_instance[i];
            break;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }

    LOG_DEBUG ("Instance  BPP Handle\t BD Addr");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    if (NULL != bpp_printer_instance)
    {
        for (i = 0U; i < BPP_NUM_PRINTER_INSTANCE; i++)
        {
            LOG_DEBUG ("%d\t  ", i);
            LOG_DEBUG ("%02x\t\t ", bpp_printer_instance[i].handle);
            LOG_DEBUG ("%02x:%02x:%02x:%02x:%02x:%02x\t ",
            bpp_printer_instance[i].bd_addr[0U], bpp_printer_instance[i].bd_addr[1U],
            bpp_printer_instance[i].bd_addr[2U], bpp_printer_instance[i].bd_addr[3U],
            bpp_printer_instance[i].bd_addr[4U], bpp_printer_instance[i].bd_addr[5U]);

            LOG_DEBUG ("\n");
        }
    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    return;
}

#endif /* BPP_PRINTER */
