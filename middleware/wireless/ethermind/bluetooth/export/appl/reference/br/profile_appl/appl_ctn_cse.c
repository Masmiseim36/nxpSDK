
/**
 *  \file appl_ctn_cse.c
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_ctn.h"
#include "appl_utils.h"

#ifdef CTN_CSE

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
static UCHAR            bd_addr[BT_BD_ADDR_SIZE];

static UCHAR            ctn_app_instance;
static UINT16           ctn_l2cap_psm;
static UINT32           ctn_supp_feat;

static CTN_CAS_INSTANCE    cas_instance[CTN_SERVER_NUM_INSTANCES];
static CTN_INSTANCE        cns_instance[CTN_SERVER_NUM_INSTANCES];

/* For the sample message file */
static BT_fops_file_handle  ctn_tx_fp;
static BT_fops_file_handle ctn_rx_fp;
static UINT16 actual;
static UCHAR  ctn_push_started;

/**
 * Flag to enable sending a large file for Get Request
 * This is required to pass PTS testcase: CTN/CSE/GOEP/SRMP/BI-02-C.
 */
static UCHAR  appl_send_large_file_flag = 0U;

/* File indices */
static UINT32 fsize = 0U;
static UINT32 remaining = 0U;

/* CTN object exchange size */
static UINT16 ctn_xchg_size;
static UINT16 ctn_ns_xchg_size;

/* SRMP wait count */
static UCHAR srmp_wait_count;

/* TODO: Update this later, by adjusting the sizes at the applicaion */
static UINT16 acount_info_max_xchng_size = 0U;

static BT_fops_file_handle fp = NULL;
static UINT16   sent = 0U;

/* ==== CTN pl related defines ==== */

static UCHAR    appl_ctn_object_path[MAX_CTN_FOLDER_NAME_LEN];
static UCHAR    appl_ctn_cse_obj_listing_file_name[MAX_CTN_OBJECT_NAME_LEN];
static UCHAR    appl_ctn_cse_get_obj_name[MAX_CTN_OBJECT_NAME_LEN];
static UCHAR    appl_ctn_cse_put_obj_name[MAX_CTN_OBJECT_NAME_LEN];
static UCHAR    appl_ctn_cse_event_obj_name[MAX_CTN_OBJECT_NAME_LEN];
static UCHAR    appl_ctn_cse_event_type[40U];
static UCHAR    appl_ctn_cse_event_handle[40U];
static UCHAR    appl_ctn_cse_event_cal_type[MAX_CTN_ATTR_CAL_TYPE_LEN];

/* Contains information for CTN listing response */
static CTN_OBJ_LISTING_INFO listing_obj_info;

/* calendar type: EVENT objects */
static CTN_OBJ_LIST_INFO  appl_ctn_event_obj_list;

/* calendar type: NOTES objects */
static CTN_OBJ_LIST_INFO  appl_ctn_notes_obj_list;

/* calendar type: TASKS objects */
static CTN_OBJ_LIST_INFO  appl_ctn_task_obj_list;

static UCHAR    temp_handle[16U];
static UCHAR    put_rsp_object_handle[MAX_CTN_OBJ_HANDLE_LEN];
static UCHAR    put_rsp_object_handle_len;

static UCHAR    ctn_cse_account_info_file_name[MAX_CTN_OBJECT_NAME_LEN];

static UCHAR    appl_ctn_obj_ref_handle[16U] =
                {
                    0x10U, 0x00U, 0x00U, 0x00U,
                    0x00U, 0x00U, 0x00U, 0x00U,
                    0x00U, 0x00U, 0x00U, 0x00U,
                    0x00U, 0x00U, 0x00U, 0x01U
                };

static const UCHAR ctn_cse_menu[] =
" \n \
------------- CTN Server Menu -------------\n \
\t 0. Exit \n \
\t 1. Refresh \n \
\n \
\t 5. CSE Initialize \n \
\t 6. CSE Shutdown \n \
\t 7. CSE Start \n \
\t 8. CSE Stop \n \
\n \
\t12. Do SDP Query \n \
\n \
\t15. CSE set number of SRMP wait stages \n\
\n \
\t17. Close Transport with CCE \n \
\n \
\t20. Connect CNS to CCE \n \
\t21. Disconnect CNS from CCE \n \
\t22. Close CNS transport with CCE \n \
\n \
\t25. Send Event to CCE \n \
\n \
\t50. List Directory \n\
\n \
\t60. Set to send large file for the Get request.\n \
\t61. Add a new calender file to the respository\n \
\n \
Your Choice: ";

/* --------------------------------------------- Functions */

void main_ctn_cse_operations (void)
{
    API_RESULT      retval;
    CTN_HANDLE     *phandle;
    UINT8           i;
    UCHAR          *pbd_addr;
    UCHAR           more;

#ifdef CTN_SUPPORT_NOTIFICATION
    CTN_CONNECT_STRUCT  connect_info;
    CTN_REQUEST_STRUCT  set_info;
    CTN_HEADER_STRUCT   body_req;
    CTN_APPL_PARAMS     appl_param;
#endif /* CTN_SUPPORT_NOTIFICATION */

    CHAR path[BT_FOPS_MAX_DIRECTORY_SIZE];

    int choice, menu_choice, val, handle;

    more = 0x00U;

    BT_LOOP_FOREVER()
    {
        printf ("%s", ctn_cse_menu);
        scanf ("%d", &choice);

        menu_choice = choice;

        switch (choice)
        {
            case 0:
                break; /* return; */

            case 1:
                break;

            case 5:
                LOG_DEBUG ("Initializing CTN CSE...\n");
                retval = BT_ctn_cse_init ();
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                /* Reset the CTN handle */
                for (i = 0U; i < CTN_SERVER_NUM_INSTANCES; i++)
                {
                    CTN_RESET_INSTANCE(&cas_instance[i].instance);
                    cas_instance[i].cns_id = CTN_ENTITY_INVALID;

                    CTN_RESET_INSTANCE(&cns_instance[i]);
                }

                ctn_cse_print_appl_instances();
                break;

            case 6:
                LOG_DEBUG ("Shutting down CTN CSE...\n");
                retval = BT_ctn_cse_shutdown ();
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 7:
                ctn_cse_print_appl_instances();

                LOG_DEBUG ("Enter Service Type (1-CAS, 2-CNS): ");
                scanf ("%d", &val);

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);
                if (CTN_SERVER_NUM_INSTANCES <= handle)
                {
                    LOG_DEBUG ("Invalid Application Instance\n");
                    break;
                }

                phandle = (val == 1U)?
                    &cas_instance[handle].instance.handle:
                    &cns_instance[handle].handle;

                if (CTN_ENTITY_INVALID != *phandle)
                {
                    LOG_DEBUG ("Application Instance not free!\n");
                    break;
                }

                LOG_DEBUG ("Starting CTN CSE Instance...\n");
                retval = BT_ctn_cse_start
                         (
                             (CTN_SERVICE_TYPE)val,
                             appl_ctn_cse_callback,
                             phandle
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                if (API_SUCCESS != retval)
                {
                    *phandle = CTN_ENTITY_INVALID;
                }

                ctn_cse_print_appl_instances();
                break;

            case 8:
                ctn_cse_print_appl_instances();

                LOG_DEBUG ("Enter Service Type (1-CAS, 2-CNS): ");
                scanf ("%d", &val);

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (CTN_SERVER_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                if (val == 1U)
                {
                    phandle = &cas_instance[handle].instance.handle;
                    pbd_addr = cas_instance[handle].instance.bd_addr;
                }
                else
                {
                    phandle = &cns_instance[handle].handle;
                    pbd_addr = cns_instance[handle].bd_addr;
                }

                if (CTN_ENTITY_INVALID == *phandle)
                {
                    LOG_DEBUG ("Application Instance not valid!\n");
                    break;
                }

                LOG_DEBUG ("Stopping CTN CSE Service %d...\n", val);
                retval = BT_ctn_cse_stop ((CTN_SERVICE_TYPE)val, phandle);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                if (API_SUCCESS == retval)
                {
                    *phandle = CTN_ENTITY_INVALID;
                    BT_mem_set (pbd_addr, 0x00, BT_BD_ADDR_SIZE);
                }

                ctn_cse_print_appl_instances();
                break;

            case 10:
                /* ACL Connect here */
                break;

            case 11:
                /* ACL Disconnect here */
                break;

            case 12: /* SDP Query */
                /*
                 * SDP Query Should be done only after the Previously Initiated
                 * ACL Connection Completes Successfully.
                 *
                 * Start SDP Query to Find out if the Peer Device is running CTN
                 * Service and if so on which Server Channel the Remote CTN PSE
                 * is Listening to.
                 *
                 * In this Non-Blocking Implemenation, the whole process of
                 * Performing SDP Query is boot-starped by first establishing
                 * an SDP Connection with the Peer.
                 *
                 * At the time of Requesting SDP to Establish a Connection, the
                 * Application also Registers a Callback function with SDP, using
                 * that Callback function SDP Module will keep indicating
                 * Asynchronous SDP events to the Application.
                 *
                 * The Application Callback for SDP is the Third argument of
                 * the SDP_SET_HANDLE() Macro.
                 */

                LOG_DEBUG ("Select CSE Entity Instance: ");
                scanf ("%d", &choice);

                ctn_app_instance = (UCHAR)choice;

                LOG_DEBUG ("Enter peer device address: ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

                cns_instance[ctn_app_instance].ctn_sdp_record.ctn_len_attrib_data
                                                      = CTN_SDP_RECORD_DATA_SIZE;

                SDP_SET_HANDLE
                (
                    cns_instance[ctn_app_instance].ctn_sdp_record.ctn_sdp_handle,
                    bd_addr,
                    appl_ctn_cse_cns_sdp_callback
                );

                retval = BT_sdp_open
                         (
                             &cns_instance[ctn_app_instance].ctn_sdp_record.ctn_sdp_handle
                         );

                if ( retval != API_SUCCESS )
                {
                    LOG_DEBUG("SDP Connect FAILED !! Error Code = 0x%04X\n", retval);
                }

                LOG_DEBUG("Please Wait for SDP Operation to Complete\n");
                break;

            case 15:
                LOG_DEBUG("Enter number of SRMP wait stages: ");
                scanf("%d", &choice);
                srmp_wait_count = (UCHAR )choice;
                LOG_DEBUG("SRMP wait count entered  is : %d\n",srmp_wait_count);
                break;

            case 17:
                ctn_cse_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (CTN_SERVER_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                printf ("Closing on CTN CSE Instance %d\n", handle);
                retval = BT_ctn_cse_transport_close
                         (
                             &cas_instance[handle].instance.handle
                         );
                printf ("Retval - 0x%04X\n", retval);
                break;

#ifdef CTN_SUPPORT_NOTIFICATION
            case 20:
                ctn_cse_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (CTN_SERVER_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

#if 0
                /* bd addrs */
                connect_info.bd_addr = bd_addr;

                /* L2CAP PSM */
                connect_info.l2cap_rpsm = ctn_l2cap_psm;
                connect_info.max_recv_size = 512;
#else
                LOG_DEBUG ("Enter peer device address: ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

                /* bd addrs */
                connect_info.bd_addr = bd_addr;

                fflush(stdin);
                LOG_DEBUG ("Enter Remote PSM(in Hex): ");
                scanf ("%x", &val);
                connect_info.l2cap_rpsm = (UINT16)val;

                fflush(stdin);
                LOG_DEBUG ("Enter Data Exchange Size: ");
                scanf ("%d", &val);
                connect_info.max_recv_size = (UINT16)val;
#endif /* 0 */

                LOG_DEBUG ("Connecting...\n");
                retval = BT_ctn_cse_ns_connect
                         (
                             &cns_instance[handle].handle,
                             &connect_info
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 21:
                ctn_cse_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (CTN_SERVER_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                printf ("Disconnecting on CTN CSE Instance %d\n", handle);
                retval = BT_ctn_cse_ns_disconnect (&cns_instance[handle].handle);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 22:
                ctn_cse_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (CTN_SERVER_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                printf ("Closing on CTN CSE Instance %d\n", handle);
                retval = BT_ctn_cse_ns_transport_close (&cns_instance[handle].handle);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 25: /* Send event */
                ctn_cse_print_appl_instances();

                /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
                handle = 0;

                printf ("Select Application Instance: ");
                retval = appl_validate_params(&handle,1U,0U,CTN_SERVER_NUM_INSTANCES - 1U);
                if (API_SUCCESS != retval)
                {
                    break;
                }

                CTN_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

                set_info.appl_params = &appl_param;
                set_info.body = &body_req;

                LOG_DEBUG ("Enter The CAS Instance ID: ");
                scanf ("%d", &val);

                appl_param.cas_instance_id = (UCHAR) val;

                CTN_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    CTN_FLAG_INSTANCEID
                );

                BT_str_copy(appl_ctn_object_path, APPL_CTN_ROOT_FOLDER_REFERENCE);
                BT_mem_set(appl_ctn_cse_event_handle, 0, sizeof(appl_ctn_cse_event_handle));
                printf ("Enter event type: ");
                scanf ("%s", appl_ctn_cse_event_type);
                LOG_DEBUG ("Enter event handle : ");
                scanf ("%s", appl_ctn_cse_event_handle);
                retval = appl_validate_strparams(appl_ctn_cse_event_handle,40U);
                if (API_SUCCESS != retval)
                {
                    break;
                }
                printf ("Enter cal_type: ");
                scanf ("%s", appl_ctn_cse_event_cal_type);

                /* Build Event file */
                (BT_IGNORE_RETURN_VALUE) BT_ctn_build_event_file_pl
                (
                    appl_ctn_object_path,
                    (UCHAR *)APPL_CTN_EVENT_FILE_NAME,
                    appl_ctn_cse_event_type,
                    appl_ctn_cse_event_handle,
                    appl_ctn_cse_event_cal_type
                );

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    appl_ctn_object_path,
                    (UCHAR *)APPL_CTN_EVENT_FILE_NAME,
                    (UCHAR *)appl_ctn_cse_event_obj_name
                );

                fsize = 0U;
                remaining = 0U;
                sent = 0U;

                /* Get the sample event from file */
                retval = BT_fops_file_open (appl_ctn_cse_event_obj_name, (UCHAR *)"rb", &fp);
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

                body_req.value = NULL;
                body_req.length = 0;

                if (remaining > ctn_ns_xchg_size)
                {
                    body_req.length = ctn_ns_xchg_size;
                    more = 0x01U;
                }
                else if (remaining != 0U)
                {
                    body_req.length = (UINT16) remaining;
                    more = 0x00U;
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }

                if (0U != body_req.length)
                {
                    body_req.value = BT_alloc_mem (body_req.length);
                }

                if (NULL != body_req.value)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                        (body_req.value, body_req.length, fp, &actual);
                }

                actual = 0;
                LOG_DEBUG ("Sending Event on CTN CSE Instance %d\n", handle);
                retval = BT_ctn_cse_ns_send_event
                         (
                             &cas_instance[handle].instance.handle,
                             &set_info,
                             more,
                             &actual
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                sent += actual;
                remaining = fsize - sent;

                /* Adjust the file read pointer to the actual bytes transmitted */
                if (body_req.length != actual)
                {
                    printf("read length = %d, actual sent = %d\n", body_req.length, actual);
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

                if (NULL != body_req.value)
                {
                    BT_free_mem (body_req.value);
                    body_req.value = NULL;
                }
                break;
#endif /* CTN_SUPPORT_NOTIFICATION */

            case 50:
                LOG_DEBUG("Enter the path: ");
                scanf("%s", path);
                (void)BT_fops_list_directory(path);
                break;

            case 60:
                LOG_DEBUG("Send Large file(1/0): ");
                scanf("%d", &val);

                appl_send_large_file_flag = (UCHAR)val;
                break;

            case 61:
            {
                UCHAR src_file_name[256U];
                UCHAR dst_handle_name[256U];
                UCHAR dst_file_name[256U];
                UCHAR dst_folder_name[256U];

                BT_str_copy(appl_ctn_object_path, APPL_CTN_ROOT_FOLDER_REFERENCE);

                LOG_DEBUG("appl_ctn_event_obj_list.num_objects:0x%2x\n", appl_ctn_event_obj_list.num_objects);

                LOG_DEBUG("Enter Handle of new .ics file\n");
                scanf("%s", dst_handle_name);

                BT_str_copy(appl_ctn_object_path, APPL_CTN_ROOT_FOLDER_REFERENCE);
                BT_str_copy(dst_folder_name, APPL_CTN_CALENDAR_FOLDER_REFERENCE);

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    appl_ctn_object_path,
                    (UCHAR *)APPL_NEW_CALENDER_FILE,
                    (UCHAR *)src_file_name
                );

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    dst_folder_name,
                    (UCHAR *)APPL_PUSH_FILE_NAME,
                    (UCHAR *)dst_file_name
                );

                (BT_IGNORE_RETURN_VALUE) BT_fops_file_copy
                (
                    src_file_name,
                    dst_file_name,
                    0x00U
                );

                LOG_DEBUG("src_file_name: %s\n", src_file_name);
                LOG_DEBUG("dst_file_name: %s\n", dst_file_name);

                /**
                 * Update the added object with CTN handle and create
                 * HANDLE.ics file
                 */
                retval = BT_ctn_update_object_handle_pl
                         (
                             dst_folder_name,
                             (UCHAR *)APPL_PUSH_FILE_NAME,
                             dst_handle_name,
                             (UCHAR )BT_str_len(dst_handle_name)
                         );
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("Failed to update the CTN handle to new object\n");
                }

                /* delete */
                retval = BT_fops_object_delete
                         (
                             dst_file_name
                         );
            }
                break;

            default:
                LOG_DEBUG ("Invalid Option\n");
                break;
        }

        if (0 == menu_choice)
        {
            /*  return */
            break;
        }
    }

    return;
}


API_RESULT appl_ctn_cse_callback
           (
               CTN_HANDLE_INFO  * ctn_handle,
               UINT8              event_type,
               UINT16             event_result,
               CTN_HEADERS      * event_data,
               UINT16             event_datalen
           )
{
    CTN_HEADERS         *ctn_headers;
    CTN_HEADERS         ctn_resp_header;
    CTN_HEADER_STRUCT   body, name;

    CTN_RESPONSE_STRUCT response_info;
    CTN_APPL_PARAMS     appl_param, *appl_param_rcv;
    API_RESULT          retval;
    UCHAR               send_response;
    UCHAR               send_request;
    UCHAR               tx_response;
    UINT16              tx_data_offset;
    UCHAR               temp;
    CTN_HANDLE          handle;
    CTN_HEADERS         *ctn_tx_hdrs;
    CTN_HEADERS         ctn_header;
    CTN_HEADER_STRUCT   body_req;
    CTN_REQUEST_STRUCT  req_info;
    UINT16              path_size, obj_size;

#ifdef CTN_SUPPORT_NOTIFICATION
    UCHAR ctn_ntf_status;
    UCHAR acoustic_alarm_status;
#endif /* CTN_SUPPORT_NOTIFICATION */

    UINT16 i, j;
    UINT16 length;
    UCHAR * data;
    UCHAR more;

    BT_IGNORE_UNUSED_PARAM(event_datalen);

    LOG_DEBUG ("\nCTN CSE Event: 0x%02X Result: 0x%04X\n",
    event_type, event_result);

    /* Init */
    ctn_headers = event_data;
    send_response = 0U;
    send_request = 0U;
    more         = 0U;
    ctn_tx_hdrs  = NULL;

    BT_mem_set(&ctn_resp_header, 0, sizeof(CTN_HEADERS));
    BT_mem_set(&req_info, 0, sizeof(CTN_REQUEST_STRUCT));
    BT_mem_set(&response_info, 0, sizeof(CTN_RESPONSE_STRUCT));
    CTN_INIT_HEADER_STRUCT (body_req);
    CTN_INIT_HEADER_STRUCT(body);
    CTN_INIT_HEADER_STRUCT (name);

    tx_response = (UCHAR) event_result;

    tx_data_offset = 0x00U;

    handle = ctn_handle->entity_id;

    switch(event_type)
    {
    case CTN_CSE_CONNECT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_CONNECT_IND.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        /* Check for valid event result and parameters */
        if((CTN_SUCCESS_RSP != event_result) ||
           (NULL == ctn_headers->ctn_connect_info))
        {
            tx_response = CTN_NOT_ACCEPTABLE_RSP;
            send_response = 1U;
            break;
        }

        LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (ctn_headers->ctn_connect_info->bd_addr));

        LOG_DEBUG ("MAX Exchange Size: %d\n",
        ctn_headers->ctn_connect_info->max_recv_size);

        BT_COPY_BD_ADDR(bd_addr, ctn_headers->ctn_connect_info->bd_addr);
        ctn_xchg_size = ctn_headers->ctn_connect_info->max_recv_size;
        tx_response = CTN_SUCCESS_RSP;

        /* Set response to be sent */
        send_response = 1U;

        remaining = 0U;
        fsize = 0U;
        sent = 0U;

        /* Get the Handle parameters */
        for (i = 0U; i < CTN_SERVER_NUM_INSTANCES; i++)
        {
            if (cas_instance[i].instance.handle == handle)
            {
                BT_mem_copy
                (
                    cas_instance[i].instance.bd_addr,
                    ctn_headers->ctn_connect_info->bd_addr,
                    BT_BD_ADDR_SIZE
                );

                break;
            }
        }

        if (CTN_SERVER_NUM_INSTANCES != i)
        {
            /* Get the Handle parameters */
            for (j = 0U; j < CTN_SERVER_NUM_INSTANCES; j++)
            {
                if (0 == BT_mem_cmp(cns_instance[j].bd_addr,
                    ctn_headers->ctn_connect_info->bd_addr,
                    BT_BD_ADDR_SIZE))
                {
                    cas_instance[i].cns_id = (UCHAR)j;
                    break;
                }
            }
        }

        /* refresh event, notes & tasks folder */
        (BT_IGNORE_RETURN_VALUE) appl_ctn_cse_update_folders ();

        ctn_cse_print_appl_instances();
        break;

    case CTN_CSE_DISCONNECT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_DISCONNECT_IND.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (NULL != fp)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fp);
            fp = NULL;
        }
        break;

    case CTN_CSE_TRANSPORT_CLOSE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_TRANSPORT_CLOSE_CNF.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (NULL != fp)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fp);
            fp = NULL;
        }

        for (i = 0U; i < CTN_SERVER_NUM_INSTANCES; i++)
        {
            if (cas_instance[i].instance.handle == handle)
            {
                BT_mem_set(&cas_instance[i].instance.bd_addr, 0x00, BT_BD_ADDR_SIZE);
                break;
            }
        }

        ctn_cse_print_appl_instances();
        break;

    case CTN_CSE_TRANSPORT_CLOSE_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_TRANSPORT_CLOSE_IND.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (NULL != fp)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fp);
            fp = NULL;
        }

        for (i = 0U; i < CTN_SERVER_NUM_INSTANCES; i++)
        {
            if (cas_instance[i].instance.handle == handle)
            {
                BT_mem_set(&cas_instance[i].instance.bd_addr, 0x00, BT_BD_ADDR_SIZE);
                break;
            }
        }

        ctn_cse_print_appl_instances();
        break;

    case CTN_CSE_GET_LISTING_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_GET_LISTING_IND.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        CTN_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

        response_info.body = &body;
        response_info.appl_params = &appl_param;
        ctn_resp_header.ctn_rsp_info = &response_info;

        send_response = 1U;

        /* Get the Handle parameters */
        for (i = 0U; i < CTN_SERVER_NUM_INSTANCES; i++)
        {
            if (cas_instance[i].instance.handle == handle)
            {
                break;
            }
        }

        if (CTN_SERVER_NUM_INSTANCES == i)
        {
            tx_response = CTN_SERVER_ERROR;
            /* Ideally should not reach here */
            break;
        }

        /*
         * TODO:
         * Headers can be present in first or/and subsequent responses also?
         */
        tx_data_offset = ctn_xchg_size;
        if (sent == 0U)
        {
            /* Folder Name requested */
            if ((NULL != ctn_headers->ctn_req_info->name) &&
                (NULL != ctn_headers->ctn_req_info->name->value))
            {
                LOG_DEBUG ("Folder Name: %s\n", ctn_headers->ctn_req_info->name->value);
            }

            BT_mem_set(appl_ctn_object_path, 0x00, sizeof(appl_ctn_object_path));
            path_size = sizeof(appl_ctn_object_path) - 1;

            /* CTN objects base directory */
            BT_str_copy(appl_ctn_object_path, APPL_CTN_ROOT_FOLDER_REFERENCE);

            /* Check if the requested folder is for "calendar" or "notes" or "tasks" */
            if (NULL != BT_str_str(ctn_headers->ctn_req_info->name->value, "calendar"))
            {
                BT_str_n_cat(appl_ctn_object_path, APPL_CTN_CALENDAR_FOLDER_PATH, (path_size - BT_str_n_len(appl_ctn_object_path,sizeof(appl_ctn_object_path)-1)));
            }
            else  if (NULL != BT_str_str(ctn_headers->ctn_req_info->name->value, "notes"))
            {
                BT_str_n_cat(appl_ctn_object_path, APPL_CTN_NOTES_FOLDER_PATH, (path_size - BT_str_n_len(appl_ctn_object_path,sizeof(appl_ctn_object_path)-1)));
            }
            else if (NULL != BT_str_str(ctn_headers->ctn_req_info->name->value, "tasks"))
            {
                BT_str_n_cat(appl_ctn_object_path, APPL_CTN_TASKS_FOLDER_PATH, (path_size - BT_str_n_len(appl_ctn_object_path,sizeof(appl_ctn_object_path)-1)));
            }
            else
            {
                /* send reject rsp */
                tx_response = CTN_NOT_FOUND_RSP;
                break;
            }

            /* Create CTN listing file name with path */
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                appl_ctn_object_path,
                (UCHAR *)APPL_LISTING_FILE,
                appl_ctn_cse_obj_listing_file_name
            );

            /* construct ctn listing object */
            retval = BT_ctn_build_object_listing_pl
                     (
                         appl_ctn_object_path,
                         appl_ctn_cse_obj_listing_file_name,
                         ctn_headers->ctn_req_info->appl_params,
                         &listing_obj_info
                     );

            fsize = 0U;
            remaining = 0U;

            if (0x00U != appl_send_large_file_flag)
            {

                /* CTN objects base directory */
                BT_str_copy(appl_ctn_object_path, APPL_CTN_ROOT_FOLDER_REFERENCE);

                /* Create CTN listing file name with path */
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    appl_ctn_object_path,
                    (UCHAR *)APPL_LARGE_LISTING_FILE,
                    appl_ctn_cse_obj_listing_file_name
                );

                LOG_DEBUG("Opening: %s \n", appl_ctn_cse_obj_listing_file_name);

                /* Open the file to be sent */
                retval = BT_fops_file_open(appl_ctn_cse_obj_listing_file_name, (UCHAR *)"rb", &ctn_tx_fp);

                LOG_DEBUG("retval: 0x%04x\n", retval);
            }
            else
            {
                LOG_DEBUG("Opening: %s \n", appl_ctn_cse_obj_listing_file_name);

                /* Open the file to be sent */
                retval = BT_fops_file_open(appl_ctn_cse_obj_listing_file_name, (UCHAR *)"rb", &ctn_tx_fp);

                LOG_DEBUG("retval: 0x%04x\n", retval);
            }

            if ((API_SUCCESS == retval) && (NULL != ctn_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(ctn_tx_fp, &fsize);

                remaining = fsize;
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");

                break;
            }

            tx_response = CTN_SUCCESS_RSP;

            /* Set sample listing size */
            appl_param.listing_size = listing_obj_info.listing_count;
            CTN_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                CTN_FLAG_LISTINGSIZE
            );
            tx_data_offset -= 4U;

            /* CSE time */
            appl_param.cse_time.value = listing_obj_info.cse_time_offset;
            appl_param.cse_time.length = (UINT16)(BT_str_len(listing_obj_info.cse_time_offset) + 1U);
            CTN_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                CTN_FLAG_CSETIME
            );

            tx_data_offset -= (appl_param.cse_time.length + 2U);
        }

        /* Print Application Parameters */
        appl_param_rcv = ctn_headers->ctn_req_info->appl_params;
        if (appl_param_rcv != NULL)
        {
            if (0 != (CTN_GET_APPL_PARAM_FLAG
                (
                    appl_param_rcv->appl_param_flag,
                    CTN_FLAG_MAXLISTCOUNT
                )))
            {
                LOG_DEBUG ("Max List Count: %d\n", appl_param_rcv->max_list_count);

                if (appl_param_rcv->max_list_count == 0U)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close(ctn_tx_fp);
                    ctn_tx_fp = NULL;
                }
            }

            if (0 != (CTN_GET_APPL_PARAM_FLAG
                (
                    appl_param_rcv->appl_param_flag,
                    CTN_FLAG_LISTSTARTOFFSET
                )))
            {
                LOG_DEBUG ("List Start Offset: %d\n", appl_param_rcv->list_start_offset);
            }

            /* FilterPeriod Begin */
            if (0 != (CTN_GET_APPL_PARAM_FLAG
                (
                    appl_param_rcv->appl_param_flag,
                    CTN_FLAG_FILTERPERIODBEGIN
                )))
            {
                LOG_DEBUG ("Filter Period Begin: ");
                for (i = 0U; i < appl_param_rcv->filter_period_begin.length; i++)
                {
                    LOG_DEBUG ("%c", appl_param_rcv->filter_period_begin.value[i]);
                }
                LOG_DEBUG ("\n");
            }

            /* FilterPeriod End */
            if (0 != (CTN_GET_APPL_PARAM_FLAG
                (
                    appl_param_rcv->appl_param_flag,
                    CTN_FLAG_FILTERPERIODEND
                )))
            {
                LOG_DEBUG ("Filter Period End: ");
                for (i = 0U; i < appl_param_rcv->filter_period_end.length; i++)
                {
                    LOG_DEBUG ("%c", appl_param_rcv->filter_period_end.value[i]);
                }
                LOG_DEBUG ("\n");
            }

            /* Parameter Mask */
            if (0 != (CTN_GET_APPL_PARAM_FLAG
                (
                    appl_param_rcv->appl_param_flag,
                    CTN_FLAG_PARMETERMASK
                )))
            {
                LOG_DEBUG ("Parameter Mast: 0x%04x\n", appl_param_rcv->parameter_mask);
            }
        }

        /* Get the body of CTN listing if required */
        if (ctn_tx_fp != NULL)
        {
            if (remaining > tx_data_offset)
            {
                body.length = tx_data_offset;
                tx_response = CTN_CONTINUE_RSP;
            }
            else if (remaining != 0U)
            {
                tx_response = CTN_SUCCESS_RSP;
                body.length = (UINT16) remaining;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            if (0U != body.length)
            {
                body.value = BT_alloc_mem (body.length);
            }

            if (NULL != body.value)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                    (body.value, body.length, ctn_tx_fp, &actual);

#if 0
                /* Print read data on the console */
                LOG_DEBUG("\n\n ====== CTN GET Listing data ===== \n\n");
                for (i = 0; i < body.length; i++)
                {
                    LOG_DEBUG("%c", body.value[i]);
                }
                LOG_DEBUG("\n\n ================================= \n");
#endif /* 0 */
            }
        }

        sent += body.length;
        remaining = fsize - sent;

        if (remaining == 0U)
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(ctn_tx_fp);
            ctn_tx_fp = NULL;
        }
        break;

    case CTN_CSE_GET_OBJECT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_GET_OBJECT_IND.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        CTN_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

        response_info.body = &body;
        ctn_resp_header.ctn_rsp_info = &response_info;

        send_response = 1U;

        /* Get the Handle parameters */
        for (i = 0U; i < CTN_SERVER_NUM_INSTANCES; i++)
        {
            if (cas_instance[i].instance.handle == handle)
            {
                break;
            }
        }

        /*
         * TODO:
         * Headers can be present in first or/and subsequent responses also?
         */
        if (sent == 0U)
        {
            /* check for object handle */
            if ((NULL == ctn_headers->ctn_req_info->name->value) &&
                (0x00U == ctn_headers->ctn_req_info->name->length))
            {
                /* No valid handle in the request */
                tx_response = CTN_BAD_REQ_RSP;
                break;
            }

            fsize = 0U;
            remaining = 0U;

            /* Update the event, notes & tasks folder info. */
            (BT_IGNORE_RETURN_VALUE) appl_ctn_cse_update_folders ();

            temp = 0x00U;
            /* Get object name for give handle */
            retval = appl_ctn_cse_get_object_name_for_handle
                     (
                         ctn_headers->ctn_req_info->name->value,
                         (UCHAR )ctn_headers->ctn_req_info->name->length,
                         appl_ctn_cse_get_obj_name,
                         &temp
                     );
            if ((APPL_CTN_OBJ_TYPE_INVALIDE == temp) ||
                (API_SUCCESS != retval))
            {
                /* Could not find the requested handle in any folder */
                tx_response = CTN_NOT_FOUND_RSP;
                break;
            }

            /* Open the file to be sent */
            retval = BT_fops_file_open (appl_ctn_cse_get_obj_name, (UCHAR *)"rb", &ctn_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != ctn_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(ctn_tx_fp, &fsize);

                remaining = fsize;
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");

                break;
            }

            tx_response = CTN_SUCCESS_RSP;
        }

        appl_param_rcv = ctn_headers->ctn_req_info->appl_params;
        if (appl_param_rcv != NULL)
        {
            if (0 != (CTN_GET_APPL_PARAM_FLAG
                (
                    appl_param_rcv->appl_param_flag,
                    CTN_FLAG_ATTACHMENT
                )))
            {
                LOG_DEBUG ("Attachment: ");
                if (CTN_ATTACHMENT_ON == appl_param_rcv->attachment)
                {
                    LOG_DEBUG ("on");
                }
                else if (CTN_ATTACHMENT_OFF == appl_param_rcv->attachment)
                {
                    LOG_DEBUG ("off");

                }
                else if (CTN_ATTACHMENT_SELECT == appl_param_rcv->attachment)
                {
                    LOG_DEBUG ("select");
                }
                else
                {
                    LOG_DEBUG ("???");
                }

                LOG_DEBUG ("\n");
            }

            if (0 != (CTN_GET_APPL_PARAM_FLAG
                (
                    appl_param_rcv->appl_param_flag,
                    CTN_FLAG_ATTACHID
                )))
            {
                LOG_DEBUG ("Attachment ID: %d\n", appl_param_rcv->attach_id);
            }

            if (0 != (CTN_GET_APPL_PARAM_FLAG
                (
                    appl_param_rcv->appl_param_flag,
                    CTN_FLAG_RECURRENT
                )))
            {
                LOG_DEBUG ("Recurent: ");
                if (CTN_RECURRENT_NO == appl_param_rcv->recurrent)
                {
                    LOG_DEBUG ("no");
                }
                else if (CTN_RECURRENT_YES == appl_param_rcv->recurrent)
                {
                    LOG_DEBUG ("yes");
                }
                else
                {
                    LOG_DEBUG ("???");
                }
                LOG_DEBUG ("\n");
            }
        }

        /* Get the body of CTN listing if required */
        if (ctn_tx_fp != NULL)
        {
            if (remaining > ctn_xchg_size)
            {
                body.length = ctn_xchg_size;
                tx_response = CTN_CONTINUE_RSP;
            }
            else if (remaining != 0U)
            {
                tx_response = CTN_SUCCESS_RSP;
                body.length = (UINT16) remaining;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            if (0U != body.length)
            {
                body.value = BT_alloc_mem (body.length);
            }

            if (NULL != body.value)
            {

                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                    (body.value, body.length, ctn_tx_fp, &actual);
            }
        }

        sent += body.length;
        remaining = fsize - sent;

        if (0U == remaining)
        {
            /* Reset */
            sent = 0U;
            fsize = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(ctn_tx_fp);
            ctn_tx_fp = NULL;
        }
        break;

    case CTN_CSE_SET_OBJECT_STATUS_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_SET_OBJECT_STATUS_IND.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if((ctn_headers->ctn_req_info->name == NULL) ||
            (ctn_headers->ctn_req_info->name->value == NULL) ||
            (ctn_headers->ctn_req_info->name->length == 0U))
        {
            LOG_DEBUG ("Message Handle Not Found\n");
            tx_response = CTN_BAD_REQ_RSP;

            response_info.body = NULL;
            response_info.appl_params = NULL;
        }
        else
        {
            LOG_DEBUG ("bCalender Handle: %s\n",
            ctn_headers->ctn_req_info->name->value);

            /* Get the Application parameter */
            appl_param_rcv =
            ctn_headers->ctn_req_info->appl_params;

            if (!(CTN_GET_APPL_PARAM_FLAG
                  (
                      appl_param_rcv->appl_param_flag,
                      CTN_FLAG_STATUSINDICATOR
                  )) ||
                !(CTN_GET_APPL_PARAM_FLAG
                  (
                      appl_param_rcv->appl_param_flag,
                      CTN_FLAG_STATUSVALUE
                  )))
            {
                LOG_DEBUG ("Mandatory Appl Params not found\n");
                tx_response = CTN_NOT_ACCEPTABLE_RSP;
            }
            else
            {
                LOG_DEBUG ("Application Parameter Values: \n");
                LOG_DEBUG ("\tStatus Indicator -> ");
                if (CTN_STATUSINDICATOR_PSTATUS == appl_param_rcv->status_indicator)
                {
                    LOG_DEBUG ("pStatus");
                }
                else if (CTN_STATUSINDICATOR_ALARMSTATUS == appl_param_rcv->status_indicator)
                {
                    LOG_DEBUG ("alarmStatus");
                }
                else if (CTN_STATUSINDICATOR_SENDSTATUS == appl_param_rcv->status_indicator)
                {
                    LOG_DEBUG ("sendStatus");
                }
                else if (CTN_STATUSINDICATOR_DELETEDSTATUS == appl_param_rcv->status_indicator)
                {
                    LOG_DEBUG ("deletedStatus");
                }
                else
                {
                    LOG_DEBUG ("???");
                }
                LOG_DEBUG ("\n");

                LOG_DEBUG ("\tStatus Value -> ");
                if (CTN_STATUSVALUE_NO == appl_param_rcv->status_value)
                {
                    LOG_DEBUG ("no");
                }
                else if (CTN_STATUSVALUE_YES == appl_param_rcv->status_value)
                {
                    LOG_DEBUG ("yes");
                }
                else if (CTN_STATUSVALUE_POSTPONE == appl_param_rcv->status_value)
                {
                    LOG_DEBUG ("postpone");
                }
                else if (CTN_STATUSVALUE_TENTATIVE == appl_param_rcv->status_value)
                {
                    LOG_DEBUG ("tentative");
                }
                else if (CTN_STATUSVALUE_NEED_ACTION == appl_param_rcv->status_value)
                {
                    LOG_DEBUG ("need action");
                }
                else if (CTN_STATUSVALUE_ACCEPTED == appl_param_rcv->status_value)
                {
                    LOG_DEBUG ("accepted");
                }
                else if (CTN_STATUSVALUE_DECLINED == appl_param_rcv->status_value)
                {
                    LOG_DEBUG ("declined");
                }
                else if (CTN_STATUSVALUE_DELEGATED == appl_param_rcv->status_value)
                {
                    LOG_DEBUG ("delegated");
                }
                else if (CTN_STATUSVALUE_COMPLETED == appl_param_rcv->status_value)
                {
                    LOG_DEBUG ("completed");
                }
                else if (CTN_STATUSVALUE_IN_PROGRESS == appl_param_rcv->status_value)
                {
                    LOG_DEBUG ("in-progress");
                }
                else
                {
                    LOG_DEBUG ("???");
                }
                LOG_DEBUG ("\n");

                if ((CTN_STATUSINDICATOR_DELETEDSTATUS == appl_param_rcv->status_indicator) &&
                    (CTN_STATUSVALUE_YES == appl_param_rcv->status_value))
                {
                    /* Delete the related object */
                    temp = 0x00U;
                    /* Get object name for give handle */
                    retval = appl_ctn_cse_get_object_name_for_handle
                             (
                                 ctn_headers->ctn_req_info->name->value,
                                 (UCHAR )ctn_headers->ctn_req_info->name->length,
                                 appl_ctn_cse_get_obj_name,
                                 &temp
                             );
                    if ((APPL_CTN_OBJ_TYPE_INVALIDE == temp) ||
                        (API_SUCCESS != retval))
                    {
                        /* Could not find the requested handle in any folder */
                        tx_response = CTN_NOT_FOUND_RSP;
                        break;
                    }

                    /* delete */
                    retval = BT_fops_object_delete
                             (
                                 appl_ctn_cse_get_obj_name
                             );
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("failed to delete object\n");
                        /* send server error rsp? */
                    }
                }

                tx_response = CTN_SUCCESS_RSP;
            }

            if (0 != (CTN_GET_APPL_PARAM_FLAG
                (
                    appl_param_rcv->appl_param_flag,
                    CTN_FLAG_POSTPONEVAL
                )))
            {
                LOG_DEBUG("\tPostpone Val: 0x%08X\n", appl_param_rcv->postpone_val);
            }

            response_info.body = NULL;
            response_info.appl_params = &appl_param;
        }

        ctn_resp_header.ctn_rsp_info = &response_info;

        send_response = 1U;
        break;

    case CTN_CSE_PUSH_OBJECT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_PUSH_OBJECT_IND.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        /* Set response to be sent */
        send_response = 1U;

        ctn_resp_header.ctn_rsp_info = &response_info;
        response_info.name = NULL;

        if ((CTN_SUCCESS_RSP != event_result) &&
            (CTN_CONTINUE_RSP != event_result))
        {
            /**
             * Ideally, should not reach here.
             */
            if (NULL != ctn_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(ctn_rx_fp);
                ctn_rx_fp = NULL;
            }

            ctn_push_started = 0x00U;
            break;
        }

        /* Check for Name hdr in the request */
        if (0U == ctn_push_started)
        {
            /* Get the Application parameter */
            appl_param_rcv =
            ctn_headers->ctn_req_info->appl_params;

            if((NULL == ctn_headers->ctn_req_info->name) ||
               (NULL == ctn_headers->ctn_req_info->name->value) ||
               (0U == ctn_headers->ctn_req_info->name->length))
            {
                tx_response = CTN_BAD_REQ_RSP;

                break;
            }

            BT_mem_set(appl_ctn_object_path, 0x00, sizeof(appl_ctn_object_path));
            path_size = sizeof(appl_ctn_object_path) - 1;

            /* CTN objects base directory */
            BT_str_copy(appl_ctn_object_path, APPL_CTN_ROOT_FOLDER_REFERENCE);

            put_rsp_object_handle_len = 0x00U;

            /* copy ref. handle */
            BT_mem_copy
            (
                temp_handle,
                appl_ctn_obj_ref_handle,
                16U
            );

            /* Check if the requested folder is for "calendar" or "notes" or "tasks" */
            if (NULL != BT_str_str(ctn_headers->ctn_req_info->name->value, "calendar"))
            {
                BT_str_n_cat(appl_ctn_object_path, APPL_CTN_CALENDAR_FOLDER_PATH, (path_size - BT_str_n_len(appl_ctn_object_path,sizeof(appl_ctn_object_path)-1)));

                if (appl_ctn_event_obj_list.num_objects < (MAX_CTN_OBJ_COUNT-1U))
                {
                    LOG_DEBUG("appl_ctn_event_obj_list.num_objects:0x%2x\n", appl_ctn_event_obj_list.num_objects);

                    temp = appl_ctn_event_obj_list.num_objects;
                    obj_size = sizeof(appl_ctn_event_obj_list.obj_details[temp].name) - 1;

                    /* update handle value for the received object */
                    temp_handle[15U] = temp + 1U;

                    /* convert to bytes to chars */
                    (BT_IGNORE_RETURN_VALUE) BT_ctn_convert_bytes_to_chars
                    (
                        temp_handle,
                        16U,
                        put_rsp_object_handle,
                        &put_rsp_object_handle_len
                    );

                    /* update list */
                    appl_ctn_event_obj_list.obj_details[temp].handle_len = put_rsp_object_handle_len;

                    if (0U != put_rsp_object_handle_len)
                    {
                        BT_mem_copy
                        (
                            appl_ctn_event_obj_list.obj_details[temp].handle,
                            put_rsp_object_handle,
                            put_rsp_object_handle_len
                        );
                    }

                    appl_ctn_event_obj_list.obj_details[temp].handle[put_rsp_object_handle_len] = '\0';

                    /* allocate a name for the received object */
                    BT_str_n_copy
                    (
                        appl_ctn_event_obj_list.obj_details[temp].name,
                        appl_ctn_event_obj_list.obj_details[temp].handle,
                        obj_size
                    );

                    BT_str_n_cat
                    (
                        appl_ctn_event_obj_list.obj_details[temp].name,
                        ".ics",
                        (obj_size - BT_str_len(appl_ctn_event_obj_list.obj_details[temp].name))
                    );

                    appl_ctn_event_obj_list.obj_details[temp].name_len = (put_rsp_object_handle_len + 4U);

                    appl_ctn_event_obj_list.num_objects += 1U;
                }
                else
                {
                    /* Max. Count reached, Send reject rsp */
                    tx_response = CTN_SERVER_ERROR;
                    break;
                }
            }
            else  if (NULL != BT_str_str(ctn_headers->ctn_req_info->name->value, "notes"))
            {
                BT_str_n_cat(appl_ctn_object_path, APPL_CTN_NOTES_FOLDER_PATH, (path_size - BT_str_n_len(appl_ctn_object_path,sizeof(appl_ctn_object_path)-1)));

                if (appl_ctn_notes_obj_list.num_objects < (MAX_CTN_OBJ_COUNT - 1U))
                {
                    temp = appl_ctn_notes_obj_list.num_objects;
                    obj_size = sizeof(appl_ctn_event_obj_list.obj_details[temp].name) - 1;

                    LOG_DEBUG("appl_ctn_notes_obj_list.num_objects:0x%2x\n", appl_ctn_notes_obj_list.num_objects);

                    /* update handle value for the received object */
                    temp_handle[0U] = 0x20U;
                    temp_handle[15U] = temp + 1U;

                    /* convert to bytes to chars */
                    (BT_IGNORE_RETURN_VALUE) BT_ctn_convert_bytes_to_chars
                    (
                        temp_handle,
                        16U,
                        put_rsp_object_handle,
                        &put_rsp_object_handle_len
                    );

                    /* update list */
                    appl_ctn_notes_obj_list.obj_details[temp].handle_len = put_rsp_object_handle_len;

                    if (0U != put_rsp_object_handle_len)
                    {
                        BT_mem_copy
                        (
                            appl_ctn_notes_obj_list.obj_details[temp].handle,
                            put_rsp_object_handle,
                            put_rsp_object_handle_len
                        );
                    }

                    appl_ctn_notes_obj_list.obj_details[temp].handle[put_rsp_object_handle_len] = '\0';

                    BT_str_n_copy
                    (
                        appl_ctn_notes_obj_list.obj_details[temp].name,
                        appl_ctn_notes_obj_list.obj_details[temp].handle,
                        obj_size
                    );

                    BT_str_n_cat
                    (
                        appl_ctn_notes_obj_list.obj_details[temp].name,
                        ".ics",
                        (obj_size - BT_str_len(appl_ctn_event_obj_list.obj_details[temp].name))
                    );

                    appl_ctn_notes_obj_list.obj_details[temp].name_len = put_rsp_object_handle_len + 4U;
                    appl_ctn_notes_obj_list.num_objects += 1U;
                }
                else
                {
                    /* Max. Count reached, Send reject rsp */
                    tx_response = CTN_SERVER_ERROR;
                    break;
                }
            }
            else if (NULL != BT_str_str(ctn_headers->ctn_req_info->name->value, "tasks"))
            {
                BT_str_n_cat(appl_ctn_object_path, APPL_CTN_TASKS_FOLDER_PATH, (path_size - BT_str_n_len(appl_ctn_object_path,sizeof(appl_ctn_object_path)-1)));

                if (appl_ctn_task_obj_list.num_objects < (MAX_CTN_OBJ_COUNT - 1U))
                {
                    temp = appl_ctn_task_obj_list.num_objects;
                    obj_size = sizeof(appl_ctn_event_obj_list.obj_details[temp].name) - 1;

                    LOG_DEBUG("appl_ctn_task_obj_list.num_objects:0x%2x\n", appl_ctn_task_obj_list.num_objects);

                    /* update handle value for the received object */
                    temp_handle[0U] = 0x30U;
                    temp_handle[15U] = temp + 1U;

                    /* convert to bytes to chars */
                    (BT_IGNORE_RETURN_VALUE) BT_ctn_convert_bytes_to_chars
                    (
                        temp_handle,
                        16U,
                        put_rsp_object_handle,
                        &put_rsp_object_handle_len
                    );

                    /* update list */
                    appl_ctn_task_obj_list.obj_details[temp].handle_len = put_rsp_object_handle_len;

                    if (0U != put_rsp_object_handle_len)
                    {
                        BT_mem_copy
                        (
                            appl_ctn_task_obj_list.obj_details[temp].handle,
                            put_rsp_object_handle,
                            put_rsp_object_handle_len
                        );
                    }

                    appl_ctn_task_obj_list.obj_details[temp].handle[put_rsp_object_handle_len] = '\0';

                    /* allocate a name for the received object */
                    BT_str_n_copy
                    (
                        appl_ctn_task_obj_list.obj_details[temp].name,
                        appl_ctn_task_obj_list.obj_details[temp].handle,
                        obj_size
                    );

                    BT_str_n_cat
                    (
                        appl_ctn_task_obj_list.obj_details[temp].name,
                        ".ics",
                        (obj_size - BT_str_len(appl_ctn_event_obj_list.obj_details[temp].name))
                    );

                    appl_ctn_task_obj_list.obj_details[temp].name_len = put_rsp_object_handle_len + 4U;
                    appl_ctn_task_obj_list.num_objects += 1U;
                }
                else
                {
                    /* Max. Count reached, Send reject rsp */
                    tx_response = CTN_SERVER_ERROR;
                    break;
                }
            }
            else
            {
                /* Send error response */
                tx_response = CTN_BAD_REQ_RSP;
                break;
            }

            /* Create CTN push object name with path */
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                appl_ctn_object_path,
                (UCHAR *)APPL_PUSH_FILE_NAME,
                appl_ctn_cse_put_obj_name
            );

            LOG_DEBUG ("Folder Name: %s\n", ctn_headers->ctn_req_info->name->value);

            if (0 != (CTN_GET_APPL_PARAM_FLAG
                (
                    appl_param_rcv->appl_param_flag,
                    CTN_FLAG_SEND
                )))
            {
                LOG_DEBUG("Send Val -> ");
                if(CTN_SEND_NO == appl_param_rcv->send)
                {
                    LOG_DEBUG ("no");
                }
                else if(0 != appl_param_rcv->send)
                {
                    LOG_DEBUG ("yes");
                }
                else
                {
                    LOG_DEBUG ("???");
                }

                LOG_DEBUG ("\n");
            }

            name.value = put_rsp_object_handle;
            name.length = (UINT16)(BT_str_n_len(put_rsp_object_handle, sizeof(put_rsp_object_handle)) + 1U);

            response_info.name = &name;

            ctn_push_started = 0x01U;
        }

        if ((NULL == ctn_headers->ctn_req_info->body->value) &&
            (0U == ctn_headers->ctn_req_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            length = ctn_headers->ctn_req_info->body->length;
            data = ctn_headers->ctn_req_info->body->value;

            /* Print received data on console */
            LOG_DEBUG ("\n\n ====== CTN Push Object data ===== \n\n");
            for (i = 0U; i < length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n\n ================================= \n");

            /* Open file PUT request */
            if (NULL == ctn_rx_fp)
            {
                retval = BT_fops_file_open
                            (
                                appl_ctn_cse_put_obj_name,
                                (UCHAR *)"wb",
                                &ctn_rx_fp
                            );
                if ((API_SUCCESS != retval) || (NULL == ctn_rx_fp))
                {
                    LOG_DEBUG ("Failed to open PUSH Object file\n");

                    /* Ideally, Should not reach here */
                    ctn_push_started = 0x00U;
                    tx_response = CTN_SERVER_ERROR;
                }
            }

            /* write to file */
            if (NULL != ctn_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, ctn_rx_fp, &actual);
            }
        }

        /* Reset */
        if (CTN_SUCCESS_RSP == event_result)
        {
            if (NULL != ctn_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (ctn_rx_fp);
                ctn_rx_fp = NULL;
            }

            ctn_push_started = 0x00U;

            /**
             * Update the received object with CTN handle and create
             * HANDLE.ics file
             */
            retval = BT_ctn_update_object_handle_pl
                     (
                         appl_ctn_object_path,
                         (UCHAR *)APPL_PUSH_FILE_NAME,
                         put_rsp_object_handle,
                         put_rsp_object_handle_len
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed to update the CTN hanlde to received object\n");
            }

            /* delete the received file */
            retval = BT_fops_object_delete
                     (
                         appl_ctn_cse_put_obj_name
                     );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("Failed to delete object\n");
            }
        }
        break;

    case CTN_CSE_FORWARD_OBJECT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_FORWARD_OBJECT_IND.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        tx_response = CTN_SUCCESS_RSP;

        if((ctn_headers->ctn_req_info->name == NULL) ||
            (ctn_headers->ctn_req_info->name->value == NULL) ||
            (ctn_headers->ctn_req_info->name->length == 0U))
        {
            LOG_DEBUG ("Object Handle Not Found\n");
            tx_response = CTN_BAD_REQ_RSP;

            response_info.body = NULL;
            response_info.appl_params = NULL;
        }
        else
        {
            LOG_DEBUG ("Object Handle: %s\n",
                ctn_headers->ctn_req_info->name->value);

            if((NULL != ctn_headers->ctn_req_info->description) &&
               (NULL != ctn_headers->ctn_req_info->description->value) &&
               (0U != ctn_headers->ctn_req_info->description->length))
            {
                LOG_DEBUG ("Object Description: %s\n",
                    ctn_headers->ctn_req_info->description->value);
            }
        }

        ctn_resp_header.ctn_rsp_info = &response_info;

        send_response = 1U;
        break;

    case CTN_CSE_SYNC_ACCOUNT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_SYNC_ACCOUNT_IND.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        tx_response = CTN_SUCCESS_RSP;

        ctn_resp_header.ctn_rsp_info = &response_info;

        send_response = 1U;
        break;

#ifdef HAVE_CTN_SYNC_INSTANCE
    case CTN_CCE_SYNC_INSTANCE_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_SYNC_INSTANCE_IND.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        /* tx_response = CTN_SUCCESS_RSP; */

        tx_response = CTN_NOT_IMPLEMENTED_RSP;

        ctn_resp_header.ctn_rsp_info = &response_info;

        send_response = 1U;
        break;
#endif /* HAVE_CTN_SYNC_INSTANCE */

    case CTN_CSE_GET_ACCOUNT_INFO_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_GET_ACCOUNT_INFO_IND.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        CTN_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

        response_info.body = &body;
        response_info.appl_params = &appl_param;
        ctn_resp_header.ctn_rsp_info = &response_info;

        send_response = 1U;

        /* Get the Handle parameters */
        for (i = 0U; i < CTN_SERVER_NUM_INSTANCES; i++)
        {
            if (cas_instance[i].instance.handle == handle)
            {
                break;
            }
        }

        if (CTN_SERVER_NUM_INSTANCES == i)
        {
            tx_response = CTN_FORBIDDEN_RSP;

            break;
        }

        appl_param_rcv = ctn_headers->ctn_req_info->appl_params;

        /*
        * TODO: For now, only Max List Count is checked. If '0',
        * then set sample listing size and NewMessage Indicator
        * Else, send sample listing body also
        */
        if (appl_param_rcv != NULL)
        {
            if (0 != (CTN_GET_APPL_PARAM_FLAG
            (
                appl_param_rcv->appl_param_flag,
                CTN_FLAG_INSTANCEID
            )))
            {
                LOG_DEBUG("Instant ID: %x", appl_param_rcv->cas_instance_id);

                if (handle != appl_param_rcv->cas_instance_id)
                {
                    tx_response = CTN_NOT_FOUND_RSP;
                    break;
                }
            }
        }

        /*
         * TODO:
         * Headers can be present in first or/and subsequent responses also?
         */
        if (sent == 0U)
        {
            fsize = 0U;
            remaining = 0U;
            acount_info_max_xchng_size = 0U;

            BT_str_copy(appl_ctn_object_path, APPL_CTN_ROOT_FOLDER_REFERENCE);

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                appl_ctn_object_path,
                (UCHAR *)APPL_CTN_ACCOUNT_INFO_FILE,
                (UCHAR *)ctn_cse_account_info_file_name
            );

            /* Open the file to be sent */
            retval = BT_fops_file_open (ctn_cse_account_info_file_name, (UCHAR *)"rb", &ctn_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != ctn_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(ctn_tx_fp, &fsize);

                remaining = fsize;
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");

                break;
            }

            tx_response = CTN_SUCCESS_RSP;

            /* TODO: delete later */
            acount_info_max_xchng_size = (ctn_xchg_size - 100U);

            /* Email Url */
            appl_param.emailuri.value = (UCHAR *)APPL_CTN_CSE_EMAIL_URL_NAME;
            appl_param.emailuri.length = (UINT16)(BT_str_len(APPL_CTN_CSE_EMAIL_URL_NAME) + 1U);

            CTN_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                CTN_FLAG_EMAILURI
            );

            /* Last Update */
            appl_param.lastupdate.value = (UCHAR *)APPL_CTN_UPDATE_TIME;
            appl_param.lastupdate.length = (UINT16)BT_str_len(APPL_CTN_UPDATE_TIME);

            CTN_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                CTN_FLAG_LASTUPDATE
            );
        }

        /* Get the body of CTN Acount Info if required */
        if (ctn_tx_fp != NULL)
        {
            if (remaining > acount_info_max_xchng_size)
            {
                body.length = acount_info_max_xchng_size;
                tx_response = CTN_CONTINUE_RSP;
            }
            else if (remaining != 0U)
            {
                tx_response = CTN_SUCCESS_RSP;
                body.length = (UINT16 )remaining;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            if (0U != body.length)
            {
                body.value = BT_alloc_mem (body.length);
            }

            if (NULL != body.value)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                    (body.value, body.length, ctn_tx_fp, &actual);
            }
        }

        sent += body.length;
        remaining = fsize - sent;

        if (remaining == 0U)
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(ctn_tx_fp);
            ctn_tx_fp = NULL;
        }
        break;

    case CTN_CSE_SET_NTF_REGISTRATION_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_SET_NTF_REGISTRATION_IND.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        send_response = 1U;
        ctn_resp_header.ctn_rsp_info = &response_info;

#ifdef CTN_SUPPORT_NOTIFICATION
        if ((CTN_SUCCESS_RSP == event_result) ||
            (CTN_CONTINUE_RSP == event_result))
        {
            if (ctn_headers->ctn_req_info->appl_params != NULL)
            {
                ctn_ntf_status =
                ctn_headers->ctn_req_info->appl_params->notification_status;

                /* Check for the Notification status */
                if (0 != (CTN_GET_APPL_PARAM_FLAG
                     (
                         ctn_headers->ctn_req_info->appl_params->appl_param_flag,
                         CTN_FLAG_NOTIFICATIONSTATUS
                     )))
                {
                    if (0x01U == ctn_ntf_status)
                    {
                        LOG_DEBUG ("Notification Enabled\n");
                    }
                    else if (0x00U == ctn_ntf_status)
                    {
                        LOG_DEBUG ("Notification Disabled\n");
                    }
                    else
                    {
                        LOG_DEBUG ("Unsupported Notification Status\n");
                        tx_response = CTN_BAD_REQ_RSP;
                    }
                }

                acoustic_alarm_status =
                ctn_headers->ctn_req_info->appl_params->acoustic_alarm_status;

                /* Check for the Notification status */
                if (0 != (CTN_GET_APPL_PARAM_FLAG
                     (
                         ctn_headers->ctn_req_info->appl_params->appl_param_flag,
                         CTN_FLAG_ACOUSTICALARMSTATUS
                     )))
                {
                    if (0x01U == acoustic_alarm_status)
                    {
                        LOG_DEBUG ("Acoustic Alarm Enabled\n");
                    }
                    else if (0x00U == acoustic_alarm_status)
                    {
                        LOG_DEBUG ("Acoustic Alarm Disabled\n");
                    }
                    else
                    {
                        LOG_DEBUG ("Unsupported Notification Status\n");
                        tx_response = CTN_BAD_REQ_RSP;
                    }
                }
            }
        }
#else /* CTN_SUPPORT_NOTIFICATION */
        tx_response = CTN_NOT_IMPLEMENTED_RSP;
#endif /* CTN_SUPPORT_NOTIFICATION */
        break;

#ifdef CTN_SUPPORT_NOTIFICATION
    case CTN_CSE_NS_CONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_NS_CONNECT_CNF.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (NULL != ctn_headers->ctn_connect_info)
        {
            LOG_DEBUG("Peer " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (ctn_headers->ctn_connect_info->bd_addr));

            LOG_DEBUG("MAX Exchange Size: %d\n",
            ctn_headers->ctn_connect_info->max_recv_size);

            ctn_ns_xchg_size = ctn_headers->ctn_connect_info->max_recv_size;
        }

        /* Check for valid event result and parameters */
        if((CTN_SUCCESS_RSP != event_result) ||
           (NULL == ctn_headers->ctn_connect_info))
        {
            tx_response = CTN_NOT_ACCEPTABLE_RSP;
            send_response = 1U;

            break;
        }

        /* Get the Handle parameters */
        for (i = 0U; i < CTN_SERVER_NUM_INSTANCES; i++)
        {
            if (cns_instance[i].handle == handle)
            {
                BT_COPY_BD_ADDR(cns_instance[i].bd_addr,
                ctn_headers->ctn_connect_info->bd_addr)

                break;
            }
        }

        if (i != CTN_SERVER_NUM_INSTANCES)
        {
            for (j = 0U; j < CTN_SERVER_NUM_INSTANCES; j++)
            {
                if (0 == BT_mem_cmp(cas_instance[j].instance.bd_addr,
                     ctn_headers->ctn_connect_info->bd_addr, BT_BD_ADDR_SIZE))
                {
                    cas_instance[j].cns_id = (UCHAR)i;
                }
            }
        }

        ctn_cse_print_appl_instances();
        break;

    case CTN_CSE_NS_DISCONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_NS_DISCONNECT_CNF.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        break;

    case CTN_CSE_NS_TRANSPORT_CLOSE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_NS_TRANSPORT_CLOSE_CNF.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        for (i = 0U; i < CTN_SERVER_NUM_INSTANCES; i++)
        {
            if (cns_instance[i].handle == handle)
            {
                BT_mem_set(cns_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);
                break;
            }
        }

        for (j = 0U; j < CTN_SERVER_NUM_INSTANCES; j++)
        {
            if (cas_instance[j].cns_id == i)
            {
                cas_instance[j].cns_id = CTN_ENTITY_INVALID;
            }
        }

        ctn_cse_print_appl_instances();
        break;

    case CTN_CSE_NS_TRANSPORT_CLOSE_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_NS_TRANSPORT_CLOSE_IND.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        for (i = 0U; i < CTN_SERVER_NUM_INSTANCES; i++)
        {
            if (cns_instance[i].handle == handle)
            {
                BT_mem_set(cns_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);
                break;
            }
        }

        for (j = 0U; j < CTN_SERVER_NUM_INSTANCES; j++)
        {
            if (cas_instance[j].cns_id == i)
            {
                cas_instance[j].cns_id = CTN_ENTITY_INVALID;
            }
        }

        ctn_cse_print_appl_instances();
        break;

    case CTN_CSE_NS_SEND_EVENT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CSE_NS_SEND_EVENT_CNF.\n");
        LOG_DEBUG("CTN Server Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((CTN_SUCCESS_RSP == event_result) ||
            (CTN_CONTINUE_RSP != event_result))
        {
            break;
        }

        /* Fill body hdr values */
        if (remaining > ctn_ns_xchg_size)
        {
            body_req.length = ctn_ns_xchg_size;
            more = 0x01U;
        }
        else if (remaining != 0U)
        {
            body_req.length = (UINT16) remaining;
            more = 0x00U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_req.length)
        {
            body_req.value = BT_alloc_mem (body_req.length);
        }

        if (NULL != body_req.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                (body_req.value, body_req.length, fp, &actual);
        }

        req_info.body = &body_req;
        ctn_header.ctn_req_info = &req_info;

        ctn_tx_hdrs = &ctn_header;

        send_request = 1U;
        break;
#endif /* CTN_SUPPORT_NOTIFICATION */

    default:
        LOG_DEBUG("Invalid Event Type: 0x%02X\n", event_type);
        break;
    }

    /* Send response if required */
    if (0 != send_response)
    {
        /* OBEX over L2CAP is Mandatory for CTN */
        if (0U < srmp_wait_count)
        {
            response_info.wait = BT_TRUE;
            ctn_resp_header.ctn_rsp_info = &response_info;
            srmp_wait_count--;
        }

        retval = BT_ctn_cse_send_response
                 (
                     &handle,
                     event_type,
                     tx_response,
                     &ctn_resp_header
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG ("Failed to send CSE Response - 0x%05X\n", retval);
        }

        if (body.value != NULL)
        {
            BT_free_mem (body.value);
            body.value = NULL;

            /* MISRA C - 2012 Rule 2.2 */
            /* body.length = 0U; */
        }
    }

#ifdef CTN_SUPPORT_NOTIFICATION
    if (0 != send_request)
    {
        retval = BT_ctn_cse_ns_send_request
                 (
                     &handle,
                     event_type,
                     event_result,
                     ctn_tx_hdrs,
                     more,
                     &actual
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG ("CTN CSE Send Request Failed - 0x%04X\n", retval);
        }

        if (CTN_CSE_NS_SEND_EVENT_CNF == event_type)
        {
            if (NULL != body_req.value)
            {
                BT_free_mem (body_req.value);
                body_req.value = NULL;
            }

            sent += actual;
            remaining = fsize - sent;

            /* Adjust the file read pointer to the actual bytes transmitted */
            if (body_req.length != actual)
            {
                printf("read length = %d, actual sent = %d\n", body_req.length, actual);
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
        }
    }
#endif  /* CTN_SUPPORT_NOTIFICATION */

    return API_SUCCESS;
}


void appl_ctn_cse_cns_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     )
{
    S_UUID uuid[2U];

    UINT16 attrib_id[2U] = {PROTOCOL_DESC_LIST,
                           CTN_SUPPORTED_FEATURES_ATTR_ID};
    API_RESULT retval;

    BT_IGNORE_UNUSED_PARAM(length);

    /*
     * Frame the SDP Search pattern with following UUIDs
     * - CTN_CAS_UUID
     * - L2CAP_UUID
     * and only one Attribute ID "PROTOCOL_DESC_LIST"
     * (For getting the L2CAP PSM on which Peer CTN Service
     *  is Listening)
     *
     */
    uuid[0U].uuid_type = UUID_16;
    uuid[1U].uuid_type = UUID_16;
    uuid[0U].uuid_union.uuid_16= CTN_CNS_UUID;
    uuid[1U].uuid_union.uuid_16 =L2CAP_UUID;

    switch(command)
    {
    case SDP_Open:
        if(status != API_SUCCESS)
        {
            LOG_DEBUG("SDP OPEN FAILED!\n");
            break;
        }
        LOG_DEBUG("SDP OPEN\n\n");

        LOG_DEBUG("Initiating SDP Service Attribute Request ....\n");
        retval = BT_sdp_servicesearchattributerequest
                 (
                     &cns_instance[ctn_app_instance].ctn_sdp_record.ctn_sdp_handle,
                     uuid,
                     2U,
                     attrib_id /* Protocol Desc */,
                     2U,
                     NULL,
                     0U,
                     cns_instance[ctn_app_instance].ctn_sdp_record.ctn_attrib_data,
                     &cns_instance[ctn_app_instance].ctn_sdp_record.ctn_len_attrib_data
                 );

        if(API_SUCCESS != retval)
        {
            LOG_DEBUG("SDP SEARCH Failed for CTN..Closing SDP\n");
            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&cns_instance[ctn_app_instance].ctn_sdp_record.ctn_sdp_handle);
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
            UCHAR ctn_client_record_no;
            UCHAR * sdp_query_result;
            UCHAR attr_val_4[4U];
            UINT16 attr_size_4;

            sdp_query_result = data;
            ctn_client_record_no = 1U;
            attr_size_4 = 4U;

            while (sdp_query_result != NULL)
            {
                sdp_query_result =
                    BT_sdp_get_Pointer_to_next_AttributeList
                    (
                        data,
                        ctn_client_record_no
                    );

                if (sdp_query_result == NULL)
                {
                    continue;
                }

                ctn_l2cap_psm = 0x0000U;

                LOG_DEBUG ("\n==============================\n");

                /* Check for OBEX L2CAP PSM for CTN in the Protocol Descriptor List */
                retval = BT_sdp_get_psm (sdp_query_result, &ctn_l2cap_psm);
                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("SDP FAILED to find PSM in Protocol Descriptor List\n");
                }
                else
                {
                    LOG_DEBUG("CTN Client PSM: 0x%04X\n", ctn_l2cap_psm);
                }

                /* Get the CTN Client Supported Features */
                retval = BT_sdp_get_attr_value
                         (
                             CTN_SUPPORTED_FEATURES_ATTR_ID,
                             sdp_query_result,
                             attr_val_4,
                             &attr_size_4
                         );
                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("SDP FAILED to find CTN Supported Features\n");
                }
                else
                {
                    BT_UNPACK_LE_4_BYTE(&ctn_supp_feat, attr_val_4);
                    LOG_DEBUG("CTN Client Supported Features: 0x%08X\n", ctn_supp_feat);
                }
                LOG_DEBUG ("\n==============================\n");
                ctn_client_record_no++;
            }
        }

        /* Now Close the SDP Connection */
        (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&cns_instance[ctn_app_instance].ctn_sdp_record.ctn_sdp_handle);
        break;

    default: /* Events not needed to be Handled */
        LOG_DEBUG("Unhandled SDP Event (0x%02X)\n", command);
        break;

    }/* switch */

    return;
}

void ctn_cse_print_appl_instances (void)
{
    UINT8 i;

    LOG_DEBUG ("\n\n===========================================================\n");
    LOG_DEBUG ("Instance\t CAS Handle\t BD Addr\t CNS Id");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < CTN_SERVER_NUM_INSTANCES; i++)
    {
        LOG_DEBUG ("%d\t\t ", i);
        LOG_DEBUG ("%02x\t ", cas_instance[i].instance.handle);
        LOG_DEBUG (BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\t ",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (cas_instance[i].instance.bd_addr));
        LOG_DEBUG ("%02x\n", cas_instance[i].cns_id);
    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    LOG_DEBUG ("\n\n===========================================================\n");
    LOG_DEBUG ("Instance\t CNS Handle\t BD Addr");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < CTN_SERVER_NUM_INSTANCES; i++)
    {
        LOG_DEBUG ("%d\t\t ", i);
        LOG_DEBUG ("%02x\t ", cns_instance[i].handle);
        LOG_DEBUG (BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER,
        BT_DEVICE_ADDR_ONLY_PRINT_STR (cns_instance[i].bd_addr));
        LOG_DEBUG ("\n");
    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");
}

/**
 * This function will update the CTN object list information
 * in 'event', 'notes', 'tasks' folder
 */
API_RESULT appl_ctn_cse_update_folders(void)
{
    API_RESULT retval;
    UINT16 path_size;

    path_size = sizeof(appl_ctn_object_path) - 1;

    /* Refresh the object details in calendar folder */
    BT_mem_set(appl_ctn_object_path, 0x00, sizeof(appl_ctn_object_path));
    BT_str_copy(appl_ctn_object_path, APPL_CTN_ROOT_FOLDER_REFERENCE);
    BT_str_n_cat(appl_ctn_object_path, APPL_CTN_CALENDAR_FOLDER_PATH, (path_size - BT_str_n_len(appl_ctn_object_path,sizeof(appl_ctn_object_path)-1)));

    retval = BT_ctn_get_object_list
             (
                 appl_ctn_object_path,
                 &appl_ctn_event_obj_list
             );
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("BT_ctn_get_object_list failed for: %s \n", appl_ctn_object_path);
    }

    /* Refresh the object details in notes folder */
    BT_mem_set(appl_ctn_object_path, 0x00, sizeof(appl_ctn_object_path));
    BT_str_copy(appl_ctn_object_path, APPL_CTN_ROOT_FOLDER_REFERENCE);
    BT_str_n_cat(appl_ctn_object_path, APPL_CTN_NOTES_FOLDER_PATH, (path_size - BT_str_n_len(appl_ctn_object_path,sizeof(appl_ctn_object_path)-1)));

    retval = BT_ctn_get_object_list
             (
                 appl_ctn_object_path,
                 &appl_ctn_notes_obj_list
             );
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("BT_ctn_get_object_list failed for: %s \n", appl_ctn_object_path);
    }

    /* Refresh the object details in task folder */
    BT_mem_set(appl_ctn_object_path, 0x00, sizeof(appl_ctn_object_path));
    BT_str_copy(appl_ctn_object_path, APPL_CTN_ROOT_FOLDER_REFERENCE);
    BT_str_n_cat(appl_ctn_object_path, APPL_CTN_TASKS_FOLDER_PATH, (path_size - BT_str_n_len(appl_ctn_object_path,sizeof(appl_ctn_object_path)-1)));

    retval = BT_ctn_get_object_list
             (
                 appl_ctn_object_path,
                 &appl_ctn_task_obj_list
             );
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("BT_ctn_get_object_list failed for: %s \n", appl_ctn_object_path);
    }

    return retval;
}

/**
 * This function is to get CTN object name and object folder type for a
 * given CTN handle.
 */
API_RESULT appl_ctn_cse_get_object_name_for_handle
           (
               /* IN */  UCHAR  *obj_handle,
               /* IN */  UCHAR   obj_handle_len,
               /* OUT */ UCHAR  *obj_name,
               /* OUT */ UCHAR  *obj_folder_type
           )
{
    API_RESULT  retval;
    UINT16      i, path_size;

    /* Init */
    retval = API_FAILURE;

    /* param check */
    if ((NULL == obj_handle) ||
        (0x00U == obj_handle_len) ||
        (NULL == obj_name) ||
        (NULL == obj_folder_type))
    {
        LOG_DEBUG("Invalid Parameters\n");
    }
    else
    {
        *obj_folder_type = APPL_CTN_OBJ_TYPE_INVALIDE;

        path_size = sizeof(appl_ctn_object_path) - 1;

        BT_mem_set(appl_ctn_object_path, 0x00, sizeof(appl_ctn_object_path));
        BT_str_copy(appl_ctn_object_path, APPL_CTN_ROOT_FOLDER_REFERENCE);

        /* Search for the request handle in calendar folder */
        for (i = 0U; i < appl_ctn_event_obj_list.num_objects; i++)
        {
            if ((0x00U != appl_ctn_event_obj_list.obj_details[i].handle_len) &&
                (0 == BT_mem_cmp
                      (
                          obj_handle,
                          appl_ctn_event_obj_list.obj_details[i].handle,
                          appl_ctn_event_obj_list.obj_details[i].handle_len
                      ))
                )
            {
                BT_str_n_cat(appl_ctn_object_path, APPL_CTN_CALENDAR_FOLDER_PATH, (path_size - BT_str_n_len(appl_ctn_object_path,sizeof(appl_ctn_object_path)-1)));

                /* Create object name with path */
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    appl_ctn_object_path,
                    (UCHAR *)appl_ctn_event_obj_list.obj_details[i].name,
                    obj_name
                );

                /* Yes, Found the requested handle in calender folder */
                *obj_folder_type = APPL_CTN_OBJ_TYPE_VEVENT;
                retval = API_SUCCESS; break; /* return API_SUCCESS; */
            }
        }

        if (API_FAILURE == retval)
        {
            /* Could not find in calender folder, Search in notes folder */
            for (i = 0U; i < appl_ctn_notes_obj_list.num_objects; i++)
            {
                if (0 == BT_mem_cmp
                         (
                             obj_handle,
                             appl_ctn_notes_obj_list.obj_details[i].handle,
                             appl_ctn_notes_obj_list.obj_details[i].handle_len
                         ))
                {
                    BT_str_n_cat(appl_ctn_object_path, APPL_CTN_NOTES_FOLDER_PATH, (path_size - BT_str_n_len(appl_ctn_object_path,sizeof(appl_ctn_object_path)-1)));

                    /* Create object name with path */
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        appl_ctn_object_path,
                        (UCHAR *)appl_ctn_notes_obj_list.obj_details[i].name,
                        obj_name
                    );

                    /* Yes, Found the requested handle in notes folder */
                    *obj_folder_type = APPL_CTN_OBJ_TYPE_VNOTE;
                    retval = API_SUCCESS; break; /* return API_SUCCESS; */
                }
            }
        }

        if (API_FAILURE == retval)
        {
            /* Could not find in notes folder, Search in task folder */
            for (i = 0U; i < appl_ctn_task_obj_list.num_objects; i++)
            {
                if (0 == BT_mem_cmp
                         (
                             obj_handle,
                             appl_ctn_task_obj_list.obj_details[i].handle,
                             appl_ctn_task_obj_list.obj_details[i].handle_len
                         ))
                {
                    BT_str_n_cat(appl_ctn_object_path, APPL_CTN_TASKS_FOLDER_PATH, (path_size - BT_str_n_len(appl_ctn_object_path,sizeof(appl_ctn_object_path)-1)));

                    /* Create object name with path */
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        appl_ctn_object_path,
                        (UCHAR *)appl_ctn_task_obj_list.obj_details[i].name,
                        obj_name
                    );

                    /* Yes, Found the requested handle in task folder */
                    *obj_folder_type = APPL_CTN_OBJ_TYPE_VTODO;
                    retval = API_SUCCESS; break; /* return API_SUCCESS; */
                }
            }
        }
    }

    return retval;
}

#endif /* CTN_CSE */
