
/**
 *  \file appl_pbap_pse.c
 *
 *  Source file for PBAP_PSE Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_pbap_pse.h"
#include "appl_utils.h"

#ifdef PBAP_PSE

/*
 * This Sample Application Demonstates usage of various PBAP PSE Role
 * APIs.
 */
static const char pbap_pse_options[] = "\n\
-------------- PBAP PSE Menu ---------------\n\
    0. Exit \n\
    1. Refresh \n\
\n\
    5. PSE Initialize \n\
    6. PSE Shutdown \n\
    7. PSE Start \n\
    8. PSE Stop \n\
\n\
   10. Transport Disconnect \n\
   15. Set Folder Version & Database Identifier \n\
   16. Get Folder Version & Database Identifier \n\
   17. Set New Missed Calls \n\
\n\
   20. PBAP Server set number of wait stages \n\
\n\
   30. Select NewMissedCall header in the response \n\
\n\
   31. Select vCard Propertries\n\
\n\
Your Choice :";

static UINT8                   pbap_pse_handle;
static UINT8                   enable_auth;

static APPL_PBAP_PSE_STRUCT    pbap_appl_instance[PBAP_PSE_MAX_ENTITY];
static APPL_PBAP_PSE_STRUCT  * pbap_appl_inst;

static APPL_PBAP_PSE_USER_INFO user_info;

static UCHAR wait_count;

static UCHAR  user_name[] = "PTS";
static UCHAR  pass_word[] = "0000";

#ifdef PBAP_1_2

/* Sample values */
static UCHAR sample_primay_folder_version[PBAP_FLDR_VER_CNTR_SIZE] =
       {
          0x05U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
          0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x01U
       };

static UCHAR sample_secondary_folder_version[PBAP_FLDR_VER_CNTR_SIZE] =
       {
          0x06U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
          0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x02U
       };

static UCHAR sample_database_identifier[PBAP_DATABASE_IDENTIFIER_SIZE] =
       {
          0x07U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
          0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x03U
       };

#endif /* PBAP_1_2 */

/* File read/write related declarations */
static UINT32               fsize;
static UINT32               sent;
static UINT32               remaining;
static BT_fops_file_handle  fp;
static BT_fops_file_handle  vcard_fp;

static PBAP_VCARD_OBJECT        pbap_vcard;
static OBJECT_PARSER_CONTEXT    vcard_context;
static PBAP_VCARD_LIST          appl_pb_vcard_list[MAX_VCARDS];
static UINT16                   appl_pb_vcard_count;
static UINT8                    appl_pb_new_missed_calls;

static UCHAR       * in_vcard_data;
static UCHAR       * out_vcard_data;
static UINT32        in_vcard_data_len;
static UINT16        out_vcard_data_len;
static UINT16        vcard_data_actual;

/* PBAP object exchange size */
static UINT16               pbap_max_xchg_size;
static PBAP_APPL_PARAMS     pbap_appl_params;
static UINT16               vc_idx; /* vcard index */

static UCHAR                pb_flder_sim1_flag = 0U;
static UINT8                appl_pb_new_missed_calls_flag = 0U;

static UINT8                appl_pb_vcard_prty_flag = 0xFFU;

/* ------------------------------------------ Functions */

/* Entry Point for PBAP Client Application */
void main_pbap_pse_operations (void)
{
    int   choice, menu_choice;
    int   input;

    API_RESULT retval;
    PBAP_EVENT_NTF_CB callback_func;

    /* This application uses only one instance; Set it to the pointer */
    pbap_appl_inst = &pbap_appl_instance[0U];

    /* To enable Authentication: Default Authentication */

    /* Set one user ID and Password */
    user_info.user_id = user_name;
    user_info.passwd = pass_word;

    LOG_DEBUG("Enable Authentication (0/1): ");

#ifndef APPL_INPUT_FROM_STRUCT
    scanf("%d", &input);
    enable_auth = (UCHAR)input;
#else
    scanf("%d", &input);
    enable_auth = (UCHAR)input;
#endif

#if 0 /* User id and pin code configurable */
    if (1 == enable_auth)
    {
        LOG_DEBUG ("User Id required (0/1)?\n");
        scanf ("%d", &input);

        if (1 == input)
        {
            LOG_DEBUG(" Enter the User Id: ");
            if (0 == appl_pbap_pse_read_string (user_name, 20))
            {
                LOG_DEBUG ("Invalid User name\n");
            }
        }
        else
        {
            user_info.user_id = NULL;
        }

        LOG_DEBUG ("Input PIN Code\n");
        if (0 == appl_pbap_pse_read_string (pass_word, 16))
        {
            LOG_DEBUG ("Invalid PIN Code\n");
        }
    }
#endif /* 0 */

    BT_LOOP_FOREVER()
    {
        printf("%s", pbap_pse_options);
        scanf("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0: /* Go Back to the Caller */
            break; /*  return; */

        case 1: /* Refresh the Menu */
            LOG_DEBUG("Refreshing Menu...\n");
            break;

        case 5:
            /*
             *  Initiate the PBAP_PSE Entities; PBAP_PSE_MAX_ENTITY entities
             *  get initialized
             */
             retval = BT_pbap_pse_init();
             if(API_SUCCESS != retval)
             {
                 LOG_DEBUG("Initialization Failed. Reason 0x%04X \n", retval);
             }
             else
             {
                 LOG_DEBUG("Successfully Initialized %d PBAP PSE entities \n",
                 PBAP_PSE_MAX_ENTITY);
             }
             break;

        case 6:
            retval = BT_pbap_pse_shutdown();
            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Shutdown Failed. Reason 0x%04X \n", retval);
            }
            else
            {
                LOG_DEBUG("Shutdown Successful. \n");
            }
            break;

        case 7: /* Start PBAP Server */
            LOG_DEBUG("Starting PBAP Server... ");

            /**
             *  Start an instance of PBAP PSE, Call Connect API to Connect
             */
            callback_func = appl_pbap_pse_event_callback;

            retval = BT_pbap_pse_start_instance
                     (
                         &pbap_pse_handle,
                         callback_func
                     );
            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Failed. Reason 0x%04X \n", retval);
                break;
            }

            /* Set path to root folder */
            BT_str_copy
                (pbap_appl_inst->path, PBAP_ROOT_FOLDER_BASE);

            retval = BT_vfops_set_path_forward
                     (
                         pbap_appl_inst->path,
                         (UCHAR *)PBAP_ROOT_FOLDER
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed to set path to ROOT folder - 0x%04X\n",
                retval);
            }
            else
            {
                pbap_appl_inst->root_depth = 0U;
            }

            LOG_DEBUG("Successfully started PBAP PSE Entity %d \n",
            pbap_pse_handle);

            pbap_appl_inst->pbap_handle = pbap_pse_handle;
            pbap_appl_inst->connect_info.auth_flag = 0U;
            pbap_appl_inst->connect_info.max_recv_size =
            APPL_PBAP_PSE_MAX_SIZE;
            break;

        case 8:
            LOG_DEBUG("Stoping PBAP Server... ");
            retval = BT_pbap_pse_stop_instance(pbap_pse_handle);
            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Stop instance failed. Reason 0x%04X \n", retval);
            }
            else
            {
                LOG_DEBUG("Stop instance Successful. \n");
            }
            break;

        case 10:
            LOG_DEBUG("Initiating Transport Disconnection... \n");
            retval = BT_pbap_pse_trans_disconnect(pbap_pse_handle);
            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Failed. Reason 0x%04X \n", retval);
            }
            else
            {
                LOG_DEBUG("Successful. Wait for the confirmation \n");
            }
            break;

        case 15:
#ifdef PBAP_1_2
            LOG_DEBUG ("Select option:\n");
            LOG_DEBUG ("    1. Set DataBase Identifier. \n");
            LOG_DEBUG ("    2. Set Primary Folder Version. \n");
            LOG_DEBUG ("    3. Set Secondary Folder Version. \n");
            scanf ("%d", &input);

            /* Set LSB of DataBase Identifier, Primary/Secondary Folder Version */
            if (1U == input)
            {
                LOG_DEBUG ("Set DaseBase Identifier LSB.\n");
                scanf ("%d", &input);
                sample_database_identifier[15U] = (UCHAR )input;
            }
            else if (2U == input)
            {
                LOG_DEBUG ("Set Primary Version Counter LSB.\n");
                scanf ("%d", &input);
                sample_primay_folder_version[15U] = (UCHAR )input;
            }
            else if (3U == input)
            {
                LOG_DEBUG ("Set Secondary Version Counter LSB.\n");
                scanf ("%d", &input);
                sample_secondary_folder_version[15U] = (UCHAR )input;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
#endif /* PBAP_1_2 */
            break;

        case 16: /* Get Folder Version & Database Identifier */
            {
#ifdef PBAP_1_2
                UCHAR i;

                LOG_DEBUG ("Data Base Identifier.\n");
                LOG_DEBUG ("  - 0x");
                for (i = 0U; i < PBAP_DATABASE_IDENTIFIER_SIZE; i++)
                {
                    LOG_DEBUG ("%02x", sample_database_identifier[i]);
                }
                LOG_DEBUG ("\n");

                LOG_DEBUG ("Primary Folder Version. \n");
                LOG_DEBUG ("  - 0x");
                for (i = 0U; i < PBAP_FLDR_VER_CNTR_SIZE; i++)
                {
                    LOG_DEBUG ("%02x", sample_primay_folder_version[i]);
                }
                LOG_DEBUG ("\n");

                LOG_DEBUG ("Secondary Folder Version. \n");
                LOG_DEBUG ("  - 0x");
                for (i = 0U; i < PBAP_FLDR_VER_CNTR_SIZE; i++)
                {
                    LOG_DEBUG ("%02x", sample_secondary_folder_version[i]);
                }
                LOG_DEBUG ("\n");
#endif /* PBAP_1_2 */
            }
            break;

        case 17: /* Set phonebook newmissed calls */
            LOG_DEBUG ("Enter No. of new missed calls.\n");
            scanf ("%d", &input);
            appl_pb_new_missed_calls = (UINT8 )input;
            break;

        case 20:
            LOG_DEBUG("Enter number of wait stages: ");
            scanf("%d", &choice);
            wait_count = (UCHAR )choice;
            break;

        case 30:
            LOG_DEBUG("Include NewMissedCall in the response(1/0)\n");
            scanf("%d", &input);
            appl_pb_new_missed_calls_flag = (UINT8)input;
            break;

        case 31:
            LOG_DEBUG("Select Below option\n");
            LOG_DEBUG("1 -> Not include X-BT-UCI property in vCard\n");
            LOG_DEBUG("2 -> Not include X-BT-UID  property in vCard\n");
            LOG_DEBUG("3 -> Otherwise\n");
            scanf("%d", &input);

            if (1U == input)
            {
                appl_pb_vcard_prty_flag = 0xFEU;
            }
            else if (2U == input)
            {
                appl_pb_vcard_prty_flag = 0xFDU;
            }
            else
            {
                appl_pb_vcard_prty_flag = 0xFFU;
            }
            break;

        default:
            LOG_DEBUG("Invalid Option. Select Option from the Menu \n");
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


/* Application Notification Callback */
API_RESULT appl_pbap_pse_event_callback
           (
               /* IN */  UINT8            event_type,
               /* IN */  UINT16           event_result,
               /* IN */  PBAP_HANDLE      pbap_handle,
               /* IN */  PBAP_HEADERS   * pbap_headers,
               /* IN */  UINT8            num_headers
           )
{
    UINT8               send_response;
    API_RESULT          retval;

    PBAP_CONNECT_STRUCT connect_resp;
    PBAP_HEADER_STRUCT  pin;
    PBAP_HEADER_STRUCT  userid;
    PBAP_RESPONSE_STRUCT rsp_info;
    PBAP_HEADERS        pbap_resp_header;

    /* Initialize */
    pin.value = user_info.passwd;
    pin.length = (UINT16)BT_str_len(user_info.passwd);

    userid.value = user_info.user_id;
    userid.length = (UINT16)BT_str_len(user_info.user_id);

    connect_resp.pin_info = &pin;
    connect_resp.user_id = &userid;

    /* Response shall not be sent by default: Set to 1 to send response */
    send_response = 0U;

    switch(event_type)
    {
    case PBAP_PSE_CONNECT_IND:
        LOG_DEBUG("Received PBAP_PSE_CONNECT_IND - 0x%04X\n",
                 event_result);

        if(((PBAP_SUCCESS_RSP != event_result) &&
           (PBAP_NOT_AUTHENTICATED != event_result) &&
           (PBAP_UNAUTH_RSP != event_result)) ||
           (NULL == pbap_headers->pbap_connect_info))
        {
            if(PBAP_AUTHENTICATION_FAILED == event_result)
            {
                event_result = PBAP_NOT_AUTHENTICATED;
            }
            else
            {
                event_result = PBAP_NOT_ACCEPTABLE_RSP;
                send_response = 1U;
                break;
            }
        }

        LOG_DEBUG(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR \
        (pbap_headers->pbap_connect_info->bd_addr));

#ifdef PBAP_1_2
        if (0U != pbap_headers->pbap_connect_info->pbap_supported_features)
        {
            LOG_DEBUG ("\nPeer Supported Features :0x%08x\n\n",
                pbap_headers->pbap_connect_info->pbap_supported_features);
        }
#endif /* PBAP_1_2 */

        pbap_appl_inst->connect_info.max_recv_size =
                pbap_headers->pbap_connect_info->max_recv_size;

        /* Max. Recv size */
        pbap_max_xchg_size = pbap_appl_inst->connect_info.max_recv_size;

        /* If Authentication is enabled */
        if(PBAP_NOT_AUTHENTICATED == event_result)
        {
             pin.value = user_info.passwd;
             pin.length = (UINT16)BT_str_len(user_info.passwd);

             userid.value = user_info.user_id;
             userid.length = (UINT16)BT_str_len(user_info.user_id);

             connect_resp.pin_info = &pin;
             connect_resp.user_id = &userid;
             if(1U == enable_auth)
             {
                event_result = PBAP_UNAUTH_RSP;
             }
             else
             {
                event_result = PBAP_SUCCESS_RSP;
             }
        }
        else if(PBAP_UNAUTH_RSP == event_result)
        {
            LOG_DEBUG("PIN VALUE : %s\n", user_info.passwd);
            pin.value = user_info.passwd;
            pin.length = (UINT16)BT_str_len(user_info.passwd);

            LOG_DEBUG("USER ID : %s\n", user_info.user_id);
            userid.value = user_info.user_id;
            userid.length = (UINT16)BT_str_len(user_info.user_id);

            connect_resp.pin_info = &pin;
            connect_resp.user_id = &userid;

            event_result = PBAP_SUCCESS_RSP;
        }
        else
        {
            if(0U == BT_str_n_cmp
                    (
                        pbap_headers->pbap_connect_info->user_id->value,
                        user_info.user_id,
                        pbap_headers->pbap_connect_info->user_id->length
                    ))
            {
                event_result = PBAP_SUCCESS_RSP;
            }
            else
            {
                event_result = PBAP_NOT_ACCEPTABLE_RSP;
            }
        }

        connect_resp.max_recv_size =
                        pbap_appl_inst->connect_info.max_recv_size;

        pbap_resp_header.pbap_connect_info = &connect_resp;

        send_response = 1U;

        if(PBAP_SUCCESS_RSP == event_result)
        {
            if (0U != pbap_appl_inst->root_depth)
            {
                while (0U != pbap_appl_inst->root_depth)
                {
                    retval = BT_vfops_set_path_backward(pbap_appl_inst->path);
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to set path backward\n");
                    }

                    pbap_appl_inst->root_depth--;

                    LOG_DEBUG ("Folder set to ROOT\n");
                }
            }
        }
        break;

    case PBAP_PSE_GET_PHONEBOOK_IND:
        LOG_DEBUG("Received PBAP_PSE_GET_PHONEBOOK_IND - 0x%04X\n",
        event_result);

        (BT_IGNORE_RETURN_VALUE) appl_pbap_pse_handle_get_phonebook_req
        (
            event_result,
            pbap_handle,
            pbap_headers,
            num_headers
        );

        send_response = 0U;
        break;

    case PBAP_PSE_GET_VCARD_LIST_IND:
        LOG_DEBUG("Received PBAP_PSE_GET_VCARD_LIST_IND - 0x%04X\n",
        event_result);

        (BT_IGNORE_RETURN_VALUE) appl_pbap_pse_handle_get_vcard_listing_req
        (
            event_result,
            pbap_handle,
            pbap_headers,
            num_headers
        );

        send_response = 0U;
        break;

    case PBAP_PSE_GET_VCARD_IND:
        LOG_DEBUG("Received PBAP_PSE_GET_VCARD_IND - 0x%04X\n",
        event_result);

        (BT_IGNORE_RETURN_VALUE) appl_pbap_pse_handle_get_vcard_req
        (
            event_result,
            pbap_handle,
            pbap_headers,
            num_headers
        );
        send_response = 0U;
        break;

    case PBAP_PSE_SET_PHONEBOOK_IND:
        LOG_DEBUG("Received PBAP_PSE_SET_PHONEBOOK_IND - 0x%04X\n",
        event_result);

        (BT_IGNORE_RETURN_VALUE) appl_pbap_pse_handle_set_phonebook_req
        (
            event_result,
            pbap_handle,
            pbap_headers,
            num_headers
        );

        send_response = 0U;
        break;

    case PBAP_PSE_DISCONNECT_IND:
        LOG_DEBUG("Received PBAP_PSE_DISCONNECT_IND - 0x%04X\n",
        event_result);
        break;

    case PBAP_PSE_TRANSPORT_CLOSE_CFM: /* Fall Through */
    case PBAP_PSE_TRANSPORT_CLOSE_IND:
        LOG_DEBUG("Received %s - 0x%04X\n", (PBAP_PSE_TRANSPORT_CLOSE_CFM == event_type)?
            "PBAP_PSE_TRANSPORT_CLOSE_CFM" : "PBAP_PSE_TRANSPORT_CLOSE_IND", event_result);

        /* Close all opened files */
        if (NULL != fp)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close (fp);
            fp = NULL;
        }

        if (NULL != vcard_fp)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close (vcard_fp);
            vcard_fp = NULL;
        }

        pb_flder_sim1_flag = 0x00U;
        break;

    case PBAP_PSE_ABORT_IND:
        LOG_DEBUG("Received PBAP_PSE_ABORT_IND - 0x%04X\n",
        event_result);
        break;

    case PBAP_PSE_IDLE_TIMEOUT_IND:
        LOG_DEBUG("Received PBAP_PSE_IDLE_TIMEOUT_IND - 0x%04X\n",
        event_result);

        /**
         * Disconnect Request is sent from the profile itself
         * No request is to be sent from here.
         */
        break;

    case PBAP_PSE_ERROR_IND:
        LOG_DEBUG("Received PBAP_PSE_ERROR_IND - 0x%04X\n",
        event_result);

        /**
         * Disconnect Request is sent from the profile itself
         * No request is to be sent from here.
         */
        break;

    default:
        LOG_DEBUG("Unknown PBAP PSE Event Type: 0x%02x. Result: 0x%04X",
        event_type, event_result);
        break;
    }

    if(1U == send_response)
    {
        if (0U < wait_count)
        {
            rsp_info.wait = BT_TRUE;
            pbap_resp_header.pbap_resp_info = &rsp_info;

            wait_count--;
        }

        retval = BT_pbap_pse_send_response
                 (
                    pbap_handle,
                    event_type,
                    event_result,
                    &pbap_resp_header
                 );

        if(API_SUCCESS != retval)
        {
            LOG_DEBUG("Failed to send the Response. Reason 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("Successfully sent the PBAP Response \n");
        }
    }

    return API_SUCCESS;
}

/* To read the user input string of length "max_len" */
UINT16 appl_pbap_pse_read_string(UCHAR *str, UINT16 max_len)
{
    UINT16 count = 0U;

    if((NULL == str) || (max_len <= 1U))
    {
        LOG_DEBUG(" Invalid parameters to read string \n");
        /* return 0U; */
    }
    else
    {
        scanf("%s", str);
        count = (UINT16)BT_str_len(str);
    }

    return count;
}

API_RESULT appl_pbap_pse_handle_set_phonebook_req
           (
               /* IN */  UINT16           event_result,
               /* IN */  PBAP_HANDLE      pbap_handle,
               /* IN */  PBAP_HEADERS   * pbap_headers,
               /* IN */  UINT8            num_headers
           )
{
    API_RESULT retval;

    PBAP_HEADERS         pbap_rsp_hdrs;

    BT_IGNORE_UNUSED_PARAM(num_headers);

    retval = API_SUCCESS;
    event_result = PBAP_SUCCESS_RSP;

    pbap_rsp_hdrs.pbap_resp_info = NULL;

    switch(pbap_headers->pbap_req_info->setpath_flag)
    {
    case 0x02U:
        if ((NULL == pbap_headers->pbap_req_info->name) ||
            (0x00U == pbap_headers->pbap_req_info->name->length))
        {
            if (0x00U == pbap_appl_inst->root_depth)
            {
                LOG_DEBUG ("PSE is already in root folder\n");
            }
            else
            {
                while (0U != pbap_appl_inst->root_depth)
                {
                    retval = BT_vfops_set_path_backward(pbap_appl_inst->path);
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to set path backward\n");
                    }

                    pbap_appl_inst->root_depth --;
                }

                LOG_DEBUG ("Folder set to ROOT\n");
                pb_flder_sim1_flag = 0x00U;
            }
        }
        else if ((NULL != pbap_headers->pbap_req_info->name) &&
                 (NULL != pbap_headers->pbap_req_info->name->value) &&
                 (0x00U != pbap_headers->pbap_req_info->name->length))
        {
            retval = BT_vfops_set_path_forward
                     (
                         pbap_appl_inst->path,
                         pbap_headers->pbap_req_info->name->value
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed to set path to: %s\n",
                    pbap_headers->pbap_req_info->name->value);

                event_result = PBAP_NOT_FOUND_RSP;
            }
            else
            {
                LOG_DEBUG ("Path Set to: %s\n",
                    pbap_headers->pbap_req_info->name->value);

                pbap_appl_inst->root_depth ++;

                if (0 == BT_str_cmp(pbap_headers->pbap_req_info->name->value, "SIM1"))
                {
                    pb_flder_sim1_flag = 0x01U;
                }
            }
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
        break;

    case 0x03U:
        if (0x00U == pbap_appl_inst->root_depth)
        {
            event_result = PBAP_BAD_REQ_RSP;
            break;
        }

        retval = BT_vfops_set_path_backward(pbap_appl_inst->path);
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG ("Failed to set path backward\n");
        }

        pbap_appl_inst->root_depth --;

        if ((NULL != pbap_headers->pbap_req_info->name) &&
            (NULL != pbap_headers->pbap_req_info->name->value) &&
            (0x00U != pbap_headers->pbap_req_info->name->length))
        {
            retval = BT_vfops_set_path_forward
                     (
                         pbap_appl_inst->path,
                         pbap_headers->pbap_req_info->name->value
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed to set path to: %s\n",
                    pbap_headers->pbap_req_info->name->value);

                event_result = PBAP_NOT_FOUND_RSP;
            }
            else
            {
                LOG_DEBUG ("Path Set to: %s\n",
                    pbap_headers->pbap_req_info->name->value);

                pbap_appl_inst->root_depth ++;
            }
        }
        break;

    default:
        event_result = PBAP_BAD_REQ_RSP;
        break;
    }

    retval = BT_pbap_pse_send_response
             (
                 pbap_handle,
                 PBAP_PSE_SET_PHONEBOOK_IND,
                 event_result,
                 &pbap_rsp_hdrs
             );
    if(API_SUCCESS != retval)
    {
        LOG_DEBUG("Failed to send the Response. Reason 0x%04X\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully sent the PBAP Response \n");
    }

    return retval;
}

/*
 * This function handles vcard listing request from PBAP Client
 */
API_RESULT appl_pbap_pse_handle_get_vcard_listing_req
           (
               /* IN */  UINT16           event_result,
               /* IN */  PBAP_HANDLE      pbap_handle,
               /* IN */  PBAP_HEADERS   * pbap_headers,
               /* IN */  UINT8            num_headers
           )
{
    API_RESULT retval;

    UCHAR   pb_listing_path[100U];
    UCHAR   file_object[128U];
    UCHAR   vcard_abs_name[128U];

    UINT16 actual;
    UINT16 filtered_vcard_cnt;
    UINT16 filtered_vcard_cnt_1;
    PBAP_APPL_PARAMS *appl_params;
    UINT16 rsp_appl_param_hdr_len;

    PBAP_REQUEST_STRUCT  *pbap_req_hdrs;
    PBAP_HEADERS         pbap_rsp_hdrs;
    PBAP_RESPONSE_STRUCT rsp_info;
    PBAP_HEADER_STRUCT   body_rsp;
    UINT16  i;

    UINT16  actual_vcard_count;

    UINT32  vcard_prop_bit_mask;
    UINT32  vcard_prop_selector_mask;
    UCHAR   select_operator;

    BT_IGNORE_UNUSED_PARAM(num_headers);

    retval = API_SUCCESS;
    event_result = PBAP_SUCCESS_RSP;

    rsp_appl_param_hdr_len = 0U;
    actual          = 0U;
    body_rsp.length = 0U;
    body_rsp.value  = NULL;
    vcard_prop_bit_mask = 0U;
    vcard_prop_selector_mask = 0U;

    rsp_info.body   = NULL;
    actual_vcard_count = 0U;
    filtered_vcard_cnt = 0U;
    filtered_vcard_cnt_1 = 0U;
    select_operator = 0U;

    pbap_rsp_hdrs.pbap_resp_info = &rsp_info;
    pbap_rsp_hdrs.pbap_resp_info->appl_params = NULL;
    pbap_rsp_hdrs.pbap_resp_info->body        = NULL;

    PBAP_RESET_APPL_PARAM_FLAG(pbap_appl_params.appl_param_flag);

    BT_mem_set(pb_listing_path, 0, sizeof(pb_listing_path));

    /* Set pb listing path to root folder */
    BT_str_n_copy (pb_listing_path, PBAP_ROOT_FOLDER_BASE, BT_str_len(PBAP_ROOT_FOLDER_BASE));

    if ((NULL == pbap_headers) ||
        (NULL == pbap_headers->pbap_req_info))
    {
        LOG_DEBUG ("No expected headers \n");

        event_result = PBAP_BAD_REQ_RSP;
        goto SEND_PULL_VCARD_LISTING_RSP;
    }
    else
    {
        appl_params = pbap_headers->pbap_req_info->appl_params;
        pbap_req_hdrs = pbap_headers->pbap_req_info;
    }

    if (0x00U == sent)
    {
        appl_pb_vcard_count = 0U;

        /* Check for name hdr */
        if ((NULL != pbap_req_hdrs->name) &&
            (NULL != pbap_req_hdrs->name->value) &&
            (0x00U != pbap_req_hdrs->name->length))
        {

            if ((0 == BT_str_cmp (pbap_req_hdrs->name->value, "pb")))
            {
                if (0x00U != pb_flder_sim1_flag)
                {
                    BT_str_n_cat(pb_listing_path, "\\root\\SIM1\\telecom\\pb", (sizeof(pb_listing_path) - BT_str_len(pb_listing_path)));
                }
                else
                {
                    BT_str_n_cat(pb_listing_path, "\\root\\telecom\\pb", (sizeof(pb_listing_path) - BT_str_len(pb_listing_path)));
                }
            }
            else if ((0 == BT_str_cmp (pbap_req_hdrs->name->value, "mch")))
            {
                if (0x00U != pb_flder_sim1_flag)
                {
                    BT_str_n_cat(pb_listing_path, "\\root\\SIM1\\telecom\\mch", (sizeof(pb_listing_path) - BT_str_len(pb_listing_path)));
                }
                else
                {
                    BT_str_n_cat(pb_listing_path, "\\root\\telecom\\mch", (sizeof(pb_listing_path) - BT_str_len(pb_listing_path)));
                }
            }
            else if ((0 == BT_str_cmp (pbap_req_hdrs->name->value, "och")))
            {
                if (0x00U != pb_flder_sim1_flag)
                {
                    BT_str_n_cat(pb_listing_path, "\\root\\SIM1\\telecom\\och", (sizeof(pb_listing_path) - BT_str_len(pb_listing_path)));
                }
                else
                {
                    BT_str_n_cat(pb_listing_path, "\\root\\telecom\\och", (sizeof(pb_listing_path) - BT_str_len(pb_listing_path)));
                }
            }
            else if ((0 == BT_str_cmp (pbap_req_hdrs->name->value, "ich")))
            {
                if (0x00U != pb_flder_sim1_flag)
                {
                    BT_str_n_cat(pb_listing_path, "\\root\\SIM1\\telecom\\ich", (sizeof(pb_listing_path) - BT_str_len(pb_listing_path)));
                }
                else
                {
                    BT_str_n_cat(pb_listing_path, "\\root\\telecom\\ich", (sizeof(pb_listing_path) - BT_str_len(pb_listing_path)));
                }
            }
            else if ((0 == BT_str_cmp (pbap_req_hdrs->name->value, "cch")))
            {
                if (0x00U != pb_flder_sim1_flag)
                {
                    BT_str_n_cat(pb_listing_path, "\\root\\SIM1\\telecom\\cch", (sizeof(pb_listing_path) - BT_str_len(pb_listing_path)));
                }
                else
                {
                    BT_str_n_cat(pb_listing_path, "\\root\\telecom\\cch", (sizeof(pb_listing_path) - BT_str_len(pb_listing_path)));
                }
            }
            else if ((0 == BT_str_cmp (pbap_req_hdrs->name->value, "spd")))
            {
                if (0x00U != pb_flder_sim1_flag)
                {
                    BT_str_n_cat(pb_listing_path, "\\root\\SIM1\\telecom\\spd", (sizeof(pb_listing_path) - BT_str_len(pb_listing_path)));
                }
                else
                {
                    BT_str_n_cat(pb_listing_path, "\\root\\telecom\\spd", (sizeof(pb_listing_path) - BT_str_len(pb_listing_path)));
                }
            }
            else if ((0 == BT_str_cmp (pbap_req_hdrs->name->value, "fav")))
            {
                if (0x00U != pb_flder_sim1_flag)
                {
                    BT_str_n_cat(pb_listing_path, "\\root\\SIM1\\telecom\\fav", (sizeof(pb_listing_path) - BT_str_len(pb_listing_path)));
                }
                else
                {
                    BT_str_n_cat(pb_listing_path, "\\root\\telecom\\fav", (sizeof(pb_listing_path) - BT_str_len(pb_listing_path)));
                }
            }
            else
            {
                event_result = PBAP_NOT_FOUND_RSP;

                LOG_DEBUG ("Folder is not found \n");

                goto SEND_PULL_VCARD_LISTING_RSP;
            }
        }
        else if ((NULL != pbap_req_hdrs->name) &&
                 (0x00U == pbap_req_hdrs->name->length))
        {
            LOG_DEBUG ("Current path:%s\n", pbap_appl_inst->path);

            /* Check if current pb path is a valid path */

            if ((NULL == BT_str_str (pbap_appl_inst->path, PBAP_TELECOM_PB_FOLDER)) &&
                (NULL == BT_str_str (pbap_appl_inst->path, PBAP_TELECOM_MCH_FOLDER)) &&
                (NULL == BT_str_str (pbap_appl_inst->path, PBAP_TELECOM_OCH_FOLDER)) &&
                (NULL == BT_str_str (pbap_appl_inst->path, PBAP_TELECOM_ICH_FOLDER)) &&
                (NULL == BT_str_str (pbap_appl_inst->path, PBAP_TELECOM_SPD_FOLDER)) &&
                (NULL == BT_str_str (pbap_appl_inst->path, PBAP_TELECOM_FAV_FOLDER)) &&
                (NULL == BT_str_str (pbap_appl_inst->path, PBAP_TELECOM_CCH_FOLDER)))
            {
                /* Sending error as current pb path is not valide for empty folder name */
                event_result = PBAP_BAD_REQ_RSP;

                LOG_DEBUG ("Current path is not valid \n");

                goto SEND_PULL_VCARD_LISTING_RSP;
            }
            else
            {
                BT_str_copy (pb_listing_path, pbap_appl_inst->path);
            }
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        /* Reset */
        BT_mem_set(appl_pb_vcard_list, 0, sizeof(PBAP_VCARD_LIST)* MAX_VCARDS);

        /* Get List of VCARDS from the given path */
        retval =  BT_pbap_get_vcard_list_pl
                  (
                      pb_listing_path,
                      appl_pb_vcard_list,
                      &appl_pb_vcard_count
                  );

        rsp_info.appl_params = &pbap_appl_params;

        /* Extract N, TEL, UID properties of vcard from list of vcards of given folder */
        for (i = 0U; i < appl_pb_vcard_count; i++)
        {
            /* Create vcard file with path */
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                pb_listing_path,
                appl_pb_vcard_list[i].vcard_file_name,
                vcard_abs_name
            );

            /* Open vcard for parsing */
            retval = BT_fops_file_open(vcard_abs_name, (UCHAR *)"rb", &vcard_fp);
            if ((API_SUCCESS == retval) && (NULL != vcard_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(vcard_fp, &in_vcard_data_len);

                in_vcard_data = BT_alloc_mem(in_vcard_data_len);
                if (NULL == in_vcard_data)
                {
                    LOG_DEBUG("Failed to allocate memory: %d\n", in_vcard_data_len);
                }

                if (NULL != in_vcard_data)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                    (
                        in_vcard_data,
                        (UINT16)in_vcard_data_len,
                        vcard_fp,
                        &vcard_data_actual
                    );
                }

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(vcard_fp);
                vcard_fp = NULL;
            }
            else
            {
                LOG_DEBUG("Failed to open file\n");
                break;
            }

            /* vc_idx is being used in appl_pbab_vcard_list_parser_cb for vcard indexing */
            vc_idx = i;

#if 0
            /* Reset */
            BT_mem_set(appl_pb_vcard_list[i].n, 0x00, MAX_VCARD_NAME_LEN);
            appl_pb_vcard_list[i].n_len = 0;
            BT_mem_set(appl_pb_vcard_list[i].tel, 0x00, MAX_VCARD_TEL_LEN);
            appl_pb_vcard_list[i].tel_len = 0;
            BT_mem_set(appl_pb_vcard_list[i].vcard_bt_uid, 0x00, MAX_VCARD_UID_LEN + 1);
            appl_pb_vcard_list[i].vcard_bt_uid_len = 0;
            appl_pb_vcard_list[i].prop_bit_mask = 0;
#endif /* 0 */

            retval = object_parser_init(&vcard_context, appl_pbab_vcard_list_parser_cb);

            if (API_SUCCESS == retval)
            {
                /* Set the context data and start parsing */
                vcard_context.buffer = in_vcard_data;
                vcard_context.buffer_len = vcard_data_actual;

                retval = object_parser_parse(&vcard_context);

                if ((API_SUCCESS != retval) &&
                    (OBJECT_RESULT_PARTIAL_PARSED != retval))
                {
                    LOG_DEBUG("VCARD Parsing Failed. Result: 0x%04X\n", retval);
                }
            }

            BT_free_mem(in_vcard_data);
#if 0
            /* For Debug */
            LOG_DEBUG("%s\n", appl_pb_vcard_list[i].vcard_file_name);
            LOG_DEBUG("%s\n", appl_pb_vcard_list[i].n);
            LOG_DEBUG("%s\n", appl_pb_vcard_list[i].tel);
            LOG_DEBUG("%s\n", appl_pb_vcard_list[i].vcard_bt_uid);
#endif /* 0 */

            actual_vcard_count += 1U;
        }

        if ((NULL != appl_params) &&
            PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_MAX_LIST_COUNT) &&
            (0x00U == appl_params->max_list_count))
        {

            LOG_DEBUG("appl_params->appl_param_flag: 0x%08x\n", appl_params->appl_param_flag);

            /* Add appl hdr params */
            /* Add Primary Folder Version */
            PBAP_SET_APPL_PARAM_FLAG
            (
                pbap_appl_params.appl_param_flag,
                PBAP_FLAG_PRIMARY_FOLDER_VERSION
            );

            BT_mem_copy
            (
                pbap_appl_params.primary_folder_ver,
                sample_primay_folder_version,
                PBAP_FLDR_VER_CNTR_SIZE
            );

            rsp_appl_param_hdr_len += 18U;


            if ((NULL != BT_str_str(pb_listing_path, "pb")) ||
                (NULL != BT_str_str(pb_listing_path, "fav")) ||
                (NULL != BT_str_str(pb_listing_path, "spd")))
            {
                /* Add Secondary Folder Version */
                PBAP_SET_APPL_PARAM_FLAG
                (
                    pbap_appl_params.appl_param_flag,
                    PBAP_FLAG_SECONDARY_FOLDER_VERSION
                );

                BT_mem_copy
                (
                    pbap_appl_params.secondary_folder_ver,
                    sample_secondary_folder_version,
                    PBAP_FLDR_VER_CNTR_SIZE
                );

                rsp_appl_param_hdr_len += 18U;
            }

            /* Add DataBase Identifier */
            PBAP_SET_APPL_PARAM_FLAG
            (
                pbap_appl_params.appl_param_flag,
                PBAP_FLAG_DATABASE_IDENTIFIER
            );

            BT_mem_copy
            (
                pbap_appl_params.database_identifier,
                sample_database_identifier,
                PBAP_DATABASE_IDENTIFIER_SIZE
            );

            rsp_appl_param_hdr_len += 18U;

            /* Add NewMissedCalls appl. header */
            if (((NULL != BT_str_str(pb_listing_path, "mch")) ||
                (NULL != BT_str_str(pb_listing_path, "cch"))) &&
                (0x00U != appl_pb_new_missed_calls_flag))
            {
                /* Add Primary Folder Version */
                PBAP_SET_APPL_PARAM_FLAG
                (
                    pbap_appl_params.appl_param_flag,
                    PBAP_FLAG_NEW_MISSED_CALLS
                );

                rsp_appl_param_hdr_len += 4U;
                pbap_appl_params.new_missed_calls = appl_pb_new_missed_calls;

                LOG_DEBUG("appl_pb_new_missed_calls_1: 0x%02x\n", appl_pb_new_missed_calls);

                if ((PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_RESET_NEW_MISSED_CALLS)) &&
                    (0x01U == appl_params->reset_new_missed_calls))
                {
                    /* Reset New missed calls */
                    appl_pb_new_missed_calls = 0x00U;
                }

                LOG_DEBUG("appl_pb_new_missed_calls_2: 0x%02x\n", appl_pb_new_missed_calls);
            }

            /* filtered_vcard_cnt = appl_pb_vcard_count; */

            if (0x00U != PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_VCARD_SELECTOR))
            {
                select_operator = 0U;

                if (0x00U != PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_VCARD_SELECTOR_OPERATOR))
                {
                    select_operator = appl_params->vcard_selector_operator;
                }

                filtered_vcard_cnt_1 = 0U;
                for (i = 0U; i < appl_pb_vcard_count; i++)
                {
                    /* Extract initial 32 bits of vcard selector field */
                    vcard_prop_bit_mask = (UINT32)appl_params->vcard_selector[1U];
                    vcard_prop_bit_mask = (vcard_prop_bit_mask << 16U) | (UINT32)appl_params->vcard_selector[0U];

                    vcard_prop_selector_mask = vcard_prop_bit_mask &  appl_pb_vcard_list[i].prop_bit_mask;

                    if (0x00U != vcard_prop_selector_mask)
                    {
                        filtered_vcard_cnt_1 += 1U;
                    }
                }

                filtered_vcard_cnt = filtered_vcard_cnt_1;

#if 1
                LOG_DEBUG("PL: actual_vcard_count0x%04x\n", actual_vcard_count);
                LOG_DEBUG("PL: filtered_vcard_cnt%04x\n", filtered_vcard_cnt);
#endif /* 1 */
            }
            else
            {
                filtered_vcard_cnt = actual_vcard_count;
            }

            PBAP_SET_APPL_PARAM_FLAG
            (
                pbap_appl_params.appl_param_flag,
                PBAP_FLAG_PHONE_BOOK_SIZE
            );

            LOG_DEBUG("Phonebook Size: %04x\n", filtered_vcard_cnt);
            pbap_appl_params.phonebook_size = filtered_vcard_cnt;

            event_result = PBAP_SUCCESS_RSP;

            goto SEND_PULL_VCARD_LISTING_RSP;
        }

        /* Create XML vcard listing file */
        retval = BT_pbap_build_xml_vcard_listing_pl
                 (
                     pb_listing_path,
                     (UCHAR *)PBAP_PB_LISTING_FILE_NAME,
                     appl_pb_vcard_list,
                     appl_pb_vcard_count,
                     appl_params,
                     &filtered_vcard_cnt
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("Failed to create vcard listing file\n");

            event_result = PBAP_BAD_REQ_RSP;

            goto SEND_PULL_VCARD_LISTING_RSP;
        }

        /* Create listing object file with path */
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_vfops_create_object_name
        (
            pb_listing_path,
            (UCHAR *)PBAP_PB_LISTING_FILE_NAME,
            file_object
        );

        LOG_DEBUG("file_object: %s\n", file_object);
        /* Open the listing file */
        retval = BT_fops_file_open
                 (
                     file_object,
                     (UCHAR *)"rb",
                     &fp
                 );
        if ((API_SUCCESS == retval) ||
            (NULL != fp))
        {
            /* Get the file size */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);

            remaining = fsize;
        }
        else
        {
            LOG_DEBUG ("Failed to Open vcard listing file\n");

            event_result = PBAP_BAD_REQ_RSP;

            goto SEND_PULL_VCARD_LISTING_RSP;
        }

        /* ============ Add appl hdr params ===== */

        /* Add NewMissedCalls appl. header */
        if (((NULL != BT_str_str(pb_listing_path, "mch")) ||
            (NULL != BT_str_str(pb_listing_path, "cch"))) &&
            (0x00U != appl_pb_new_missed_calls_flag))
        {
            /* Add appl. param hdr & calculate its length */
            rsp_appl_param_hdr_len += 3U;

            /* Add Primary Folder Version */
            PBAP_SET_APPL_PARAM_FLAG
            (
                pbap_appl_params.appl_param_flag,
                PBAP_FLAG_NEW_MISSED_CALLS
            );

            rsp_appl_param_hdr_len += 4U;
            pbap_appl_params.new_missed_calls = appl_pb_new_missed_calls;

            if ((PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_RESET_NEW_MISSED_CALLS)) &&
                (0x01U == appl_params->reset_new_missed_calls))
            {
                /* Reset New missed calls */
                appl_pb_new_missed_calls = 0x00U;
            }
        }

#ifdef PBAP_1_2
        /* appl params length */
        rsp_appl_param_hdr_len += 3U;

        /* Add Primary Folder Version */
        PBAP_SET_APPL_PARAM_FLAG
        (
            pbap_appl_params.appl_param_flag,
            PBAP_FLAG_PRIMARY_FOLDER_VERSION
        );

        BT_mem_copy
        (
            pbap_appl_params.primary_folder_ver,
            sample_primay_folder_version,
            PBAP_FLDR_VER_CNTR_SIZE
        );

        rsp_appl_param_hdr_len += 18U;

        /* Add Secondary Folder Version */
        PBAP_SET_APPL_PARAM_FLAG
        (
            pbap_appl_params.appl_param_flag,
            PBAP_FLAG_SECONDARY_FOLDER_VERSION
        );

        BT_mem_copy
        (
            pbap_appl_params.secondary_folder_ver,
            sample_secondary_folder_version,
            PBAP_FLDR_VER_CNTR_SIZE
        );

        rsp_appl_param_hdr_len += 18U;

        /* Add DataBase Identifier */
        PBAP_SET_APPL_PARAM_FLAG
        (
            pbap_appl_params.appl_param_flag,
            PBAP_FLAG_DATABASE_IDENTIFIER
        );

        BT_mem_copy
        (
            pbap_appl_params.database_identifier,
            sample_database_identifier,
            PBAP_DATABASE_IDENTIFIER_SIZE
        );

        rsp_appl_param_hdr_len += 18U;

#endif /* PBAP_1_2 */

        if (0U != (PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_VCARD_SELECTOR)))
        {
            rsp_appl_param_hdr_len += 4U;

            /* PhoneBook Size */
            PBAP_SET_APPL_PARAM_FLAG
            (
                pbap_appl_params.appl_param_flag,
                PBAP_FLAG_PHONE_BOOK_SIZE
            );

            LOG_DEBUG("PL: actual_vcard_count0x%04x\n", actual_vcard_count);
            LOG_DEBUG("PL: filtered_vcard_cnt%04x\n", filtered_vcard_cnt);

            pbap_appl_params.phonebook_size = filtered_vcard_cnt;
        }

        if (remaining > (UINT32)(pbap_max_xchg_size - rsp_appl_param_hdr_len))
        {
            body_rsp.length = (pbap_max_xchg_size - rsp_appl_param_hdr_len);
            event_result = PBAP_CONTINUE_RSP;
        }
        else if (0U != remaining)
        {
            body_rsp.length = (UINT16)remaining;
            event_result = PBAP_SUCCESS_RSP;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_rsp.length)
        {
            body_rsp.value = BT_alloc_mem(body_rsp.length);
        }

        if (NULL != body_rsp.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read (body_rsp.value, body_rsp.length, fp, &actual);
        }

        sent += body_rsp.length;
        remaining = fsize - sent;

        rsp_info.body = &body_rsp;
    }
    else
    {
        if (remaining > pbap_max_xchg_size)
        {
            event_result = PBAP_CONTINUE_RSP;
            body_rsp.length = pbap_max_xchg_size;
        }
        else if (0U != remaining)
        {
            event_result = PBAP_SUCCESS_RSP;
            body_rsp.length = (UINT16)remaining;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_rsp.length)
        {
            body_rsp.value = BT_alloc_mem(body_rsp.length);
        }

        if (NULL != body_rsp.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read (body_rsp.value, body_rsp.length, fp, &actual);
        }

        sent += body_rsp.length;
        remaining = fsize - sent;

        rsp_info.body = &body_rsp;
    }

SEND_PULL_VCARD_LISTING_RSP:
    retval = BT_pbap_pse_send_response
             (
                  pbap_handle,
                  PBAP_PSE_GET_VCARD_LIST_IND,
                  event_result,
                  &pbap_rsp_hdrs
             );
    if(API_SUCCESS != retval)
    {
        LOG_DEBUG("Failed to send the Response. Reason 0x%04X\n", retval);
    }
    else
    {
        if (NULL != body_rsp.value)
        {
            BT_free_mem(body_rsp.value);
            body_rsp.value = NULL;
        }

        if (0U == remaining)
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close (fp);
            fp = NULL;
        }

        LOG_DEBUG("Successfully sent the PBAP Response \n");
    }

    BT_IGNORE_UNUSED_PARAM(select_operator);

    return retval;
}

/*
 * This function will construct vCard object based on the PBAP Client request
 */
API_RESULT appl_pbap_pse_handle_get_vcard_req
           (
               /* IN */  UINT16           event_result,
               /* IN */  PBAP_HANDLE      pbap_handle,
               /* IN */  PBAP_HEADERS   * pbap_headers,
               /* IN */  UINT8            num_headers
           )
{
    API_RESULT retval;

    UCHAR       file_object[128U];
    UCHAR       vcard_abs_name[128U];
    UINT16      actual;
    UINT32      vcard_prop_filter_mask;

    PBAP_APPL_PARAMS *appl_params;
    UINT16 rsp_appl_param_hdr_len;

    PBAP_HEADERS         pbap_rsp_hdrs;
    PBAP_RESPONSE_STRUCT rsp_info;
    PBAP_HEADER_STRUCT   body_rsp;

    BT_IGNORE_UNUSED_PARAM(num_headers);

    retval = API_SUCCESS;
    event_result = PBAP_SUCCESS_RSP;

    actual          = 0U;
    body_rsp.length = 0U;
    body_rsp.value  = NULL;

    rsp_info.body   = NULL;

    pbap_rsp_hdrs.pbap_resp_info = &rsp_info;
    pbap_rsp_hdrs.pbap_resp_info->appl_params = NULL;
    pbap_rsp_hdrs.pbap_resp_info->body        = NULL;

    PBAP_RESET_APPL_PARAM_FLAG(pbap_appl_params.appl_param_flag);

    appl_params = pbap_headers->pbap_req_info->appl_params;
    vcard_prop_filter_mask = 0U;
    rsp_appl_param_hdr_len = 0U;
    BT_mem_set(&vcard_abs_name[0U], 0, sizeof(vcard_abs_name));

    if (0x00U == sent)
    {
        /* Check vcard name */
        if((NULL == pbap_headers->pbap_req_info->name) ||
           (NULL == pbap_headers->pbap_req_info->name->value) ||
           (0x00U == pbap_headers->pbap_req_info->name->length))
        {
            /* No, vCard Name found so send bad req  */
            event_result = PBAP_BAD_REQ_RSP;
            goto SEND_PULL_VCARD_RSP;
        }

        /* Create the vcard file with path */
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_vfops_create_object_name
        (
            pbap_appl_inst->path,
            (UCHAR *)PBAP_VCARD_NAME,
            file_object
        );

        LOG_DEBUG("file_object: %s\n", file_object);

        /* Create vcard by appling filtering */
        retval = BT_fops_file_open
                 (
                     file_object,
                     (UCHAR *)"wb",
                     &fp
                 );

        if (NULL != BT_str_str(pbap_headers->pbap_req_info->name->value, "X-BT-UID:"))
        {
            UINT16 i;

            /* Request has X-BT-UID */
            for (i = 0U; i < appl_pb_vcard_count; i++)
            {
                /* Check if UID matches in the list */
                if (0 == BT_mem_cmp
                         (
                             pbap_headers->pbap_req_info->name->value + 9U, /* indexing to UID */
                             appl_pb_vcard_list[i].vcard_bt_uid,
                             appl_pb_vcard_list[i].vcard_bt_uid_len
                         ))
                {
                    pbap_headers->pbap_req_info->name->value = appl_pb_vcard_list[i].vcard_file_name;

                    /**
                     * For PTS validation:
                     * 1. PTS sets to pb folder, then does phonebook download.
                     * 2. After this, it sets to telecom folder and does get vcard with 'X-BT-UID' value
                     *
                     *  Below set path forward is to handle above scenario.
                     */
                    if ((0x01U == pbap_appl_inst->root_depth) ||
                        ((0x02U == pbap_appl_inst->root_depth) &&
                         (NULL != BT_str_str(pbap_appl_inst->path, "SIM1"))))
                    {
                        retval = BT_vfops_set_path_forward
                                 (
                                     pbap_appl_inst->path,
                                     (UCHAR *)"pb"
                                 );
                        if (API_SUCCESS == retval)
                        {
                            /* Increase the depth */
                            pbap_appl_inst->root_depth += 1U;
                        }
                    }

                    break;
                }
            }

            /* Not found the requested UID in vCard list, send error rsp */
            if (i == appl_pb_vcard_count)
            {
                event_result = PBAP_NOT_FOUND_RSP;
                goto SEND_PULL_VCARD_RSP;
            }
        }

        /* Create the requested vcard file with path */
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_vfops_create_object_name
        (
            pbap_appl_inst->path,
            pbap_headers->pbap_req_info->name->value,
            vcard_abs_name
        );

        /* Open reqested vCard */
        retval = BT_fops_file_open (vcard_abs_name, (UCHAR *)"rb", &vcard_fp);
        if ((API_SUCCESS == retval) && (NULL != vcard_fp))
        {
            /* Get the vCard size */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(vcard_fp, &in_vcard_data_len);

            in_vcard_data = BT_alloc_mem (in_vcard_data_len);
            if (NULL == in_vcard_data)
            {
               LOG_DEBUG("Failed to allocate size: %d\n", in_vcard_data_len);
            }

            if (NULL != in_vcard_data)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                (
                    in_vcard_data,
                    (UINT16 )in_vcard_data_len,
                    vcard_fp,
                    &vcard_data_actual
                );
            }

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(vcard_fp);
            vcard_fp = NULL;
        }
        else
        {
            LOG_DEBUG ("Failed to open file:%s\n", vcard_abs_name);

            event_result = PBAP_NOT_FOUND_RSP;
            goto SEND_PULL_VCARD_RSP;
        }

        /* Reset pbap_vcard */
        BT_mem_set(&pbap_vcard, 0, sizeof(PBAP_VCARD_OBJECT));

        BT_str_copy(pbap_vcard.vcard_file_name, pbap_headers->pbap_req_info->name->value);

        retval = object_parser_init (&vcard_context, appl_pbab_vcard_parser_cb);

        /* Set the context data and call the parser */
        vcard_context.buffer = in_vcard_data;
        vcard_context.buffer_len = vcard_data_actual;

        /* Alllocating buffer for vcard to be built */
        out_vcard_data = BT_alloc_mem (in_vcard_data_len);
        if (NULL == out_vcard_data)
        {
            LOG_DEBUG("Failed to allocate size: %d\n", in_vcard_data_len);
        }

        retval = object_parser_parse (&vcard_context);

        /* Apply filtering */
        if (NULL != appl_params)
        {
            /* Apply vcard property selector filtering */
            vcard_prop_filter_mask = 0xFFFFFFFFU;
            if (0U != (PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_FILTER)))
            {
                /* Extract initial 32 bits of filter field */
                vcard_prop_filter_mask = (UINT32 )appl_params->filter[1U];
                vcard_prop_filter_mask =  (vcard_prop_filter_mask << 16U) |(UINT32 )appl_params->filter[0U];
            }

            /* TODO: Format */
        }

        /* Build vcard by applying filtering */
        retval =  BT_pbap_build_vcard_pl
                  (
                      &pbap_vcard,
                      vcard_prop_filter_mask,
                      in_vcard_data,
                      vcard_data_actual,
                      out_vcard_data,
                      &out_vcard_data_len
                  );

        if (NULL != out_vcard_data)
        {
            /* Write to file */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(out_vcard_data, out_vcard_data_len, fp, &vcard_data_actual);
        }

        /* Free memory */
        BT_free_mem (in_vcard_data);
        BT_free_mem (out_vcard_data);

        /* Close built vcard file */
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_fops_file_close(fp);
        fp = NULL;

        /* Open created vcard file for sending */
        retval = BT_fops_file_open
                 (
                     file_object,
                     (UCHAR *)"rb",
                     &fp
                 );
        if ((API_SUCCESS == retval) ||
            (NULL != fp))
        {
            /* Get the file size */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);

            remaining = fsize;
        }
        else
        {
            LOG_DEBUG ("Failed to Open vcard file\n");

            event_result = PBAP_NOT_FOUND_RSP;
            goto SEND_PULL_VCARD_RSP;
        }

        rsp_info.appl_params = &pbap_appl_params;

#ifdef PBAP_1_2
        rsp_appl_param_hdr_len = 3U;

        /* Add Data Base Identifier */
        PBAP_SET_APPL_PARAM_FLAG
        (
            pbap_appl_params.appl_param_flag,
            PBAP_FLAG_DATABASE_IDENTIFIER
        );

        BT_mem_copy
        (
            pbap_appl_params.database_identifier,
            sample_database_identifier,
            PBAP_DATABASE_IDENTIFIER_SIZE
        );

        rsp_appl_param_hdr_len += 18U;
#endif /* PBAP_1_2 */

        if (remaining > (UINT32)(pbap_max_xchg_size - rsp_appl_param_hdr_len))
        {
            body_rsp.length = (pbap_max_xchg_size - rsp_appl_param_hdr_len);
            event_result = PBAP_CONTINUE_RSP;
        }
        else if (0U != remaining)
        {
            body_rsp.length = (UINT16)remaining;
            event_result = PBAP_SUCCESS_RSP;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_rsp.length)
        {
            body_rsp.value = BT_alloc_mem(body_rsp.length);
        }

        if (NULL != body_rsp.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read (body_rsp.value, body_rsp.length, fp, &actual);
        }

        sent += body_rsp.length;
        remaining = fsize - sent;

        rsp_info.body = &body_rsp;
    }
    else
    {
        if (remaining > pbap_max_xchg_size)
        {
            event_result = PBAP_CONTINUE_RSP;
            body_rsp.length = pbap_max_xchg_size;
        }
        else if (0U != remaining)
        {
            event_result = PBAP_SUCCESS_RSP;
            body_rsp.length = (UINT16)remaining;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_rsp.length)
        {
            body_rsp.value = BT_alloc_mem(body_rsp.length);
        }

        if (NULL != body_rsp.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read (body_rsp.value, body_rsp.length, fp, &actual);
        }

        sent += body_rsp.length;
        remaining = fsize - sent;

        rsp_info.body = &body_rsp;
    }

SEND_PULL_VCARD_RSP:
    retval = BT_pbap_pse_send_response
             (
                 pbap_handle,
                 PBAP_PSE_GET_VCARD_IND,
                 event_result,
                 &pbap_rsp_hdrs
             );
    if(API_SUCCESS != retval)
    {
        LOG_DEBUG("Failed to send the Response. Reason 0x%04X\n", retval);
    }
    else
    {
        if (NULL != body_rsp.value)
        {
            BT_free_mem(body_rsp.value);
            body_rsp.value = NULL;
        }

        if (0U == remaining)
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close (fp);
            fp = NULL;
        }

        LOG_DEBUG("Successfully sent the PBAP Response \n");
    }

    return retval;
}

/**
 * This function will build phone book object based on PBAP Client request
 */
API_RESULT appl_pbap_pse_handle_get_phonebook_req
           (
               /* IN */  UINT16           event_result,
               /* IN */  PBAP_HANDLE      pbap_handle,
               /* IN */  PBAP_HEADERS   * pbap_headers,
               /* IN */  UINT8            num_headers
           )
{
    API_RESULT  retval;
    UCHAR       file_object[128U];
    UCHAR       vcard_abs_name[128U];
    UINT16      i;
    UINT16      vcard_start_index;
    UINT16      actual_vcard_count;
    UINT16      actual;
    UINT16      rsp_appl_param_hdr_len;

    PBAP_APPL_PARAMS    * appl_params;
    PBAP_HEADERS          pbap_rsp_hdrs;
    PBAP_RESPONSE_STRUCT  rsp_info;
    PBAP_HEADER_STRUCT    body_rsp;

    UINT32  vcard_prop_bit_mask;
    UINT32  vcard_prop_selector_mask;
    UINT32  vcard_prop_filter_mask;

    BT_IGNORE_UNUSED_PARAM(num_headers);

    retval       = API_SUCCESS;
    event_result = PBAP_SUCCESS_RSP;

    rsp_appl_param_hdr_len = 0U;
    actual = 0U;
    body_rsp.length = 0U;
    body_rsp.value = NULL;

    rsp_info.body = NULL;
    vcard_prop_bit_mask = 0x00U;
    vcard_prop_selector_mask = 0x00U;

    pbap_rsp_hdrs.pbap_resp_info                = &rsp_info;
    pbap_rsp_hdrs.pbap_resp_info->appl_params   = NULL;
    pbap_rsp_hdrs.pbap_resp_info->body          = NULL;

    PBAP_RESET_APPL_PARAM_FLAG(pbap_appl_params.appl_param_flag);

    appl_pb_vcard_count = 0U;
    actual_vcard_count = 0U;

    vcard_start_index = 0U;
    appl_params = pbap_headers->pbap_req_info->appl_params;

    if (0x00U == sent)
    {
        /* Validate the file name */
        if((NULL == pbap_headers->pbap_req_info->name) ||
           (NULL == pbap_headers->pbap_req_info->name->value) ||
           (0x00U == pbap_headers->pbap_req_info->name->length))
        {
            /* No name send  bad req rsp */
            event_result = PBAP_BAD_REQ_RSP;

            LOG_DEBUG("No Name Header in the request \n");

            goto SEND_PULL_PHONE_BOOK_RSP;
        }

        /* Take backup of path */
        BT_str_copy(pbap_appl_inst->tpath, pbap_appl_inst->path);
        pbap_appl_inst->troot_depth = pbap_appl_inst->root_depth;

        /**
         * Set the path to phonebook requested folder and also get
         * phonebook object name(including path)
         */
        retval = appl_pbab_set_to_get_phonebook_req_path
                 (
                     pbap_appl_inst,
                     pbap_headers->pbap_req_info->name->value,
                     file_object
                 );
        if (API_SUCCESS != retval)
        {
            event_result = PBAP_NOT_FOUND_RSP;

            LOG_DEBUG("Failed appl_pbab_set_to_get_phonebook_req_path \n");

            goto SEND_PULL_PHONE_BOOK_RSP;
        }

        if (NULL != appl_params)
        {
            if (0U != (PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_LIST_START_OFFSET)))
            {
                vcard_start_index = appl_params->list_start_offset;
            }

        }

        /* Get List of VCARDS from the given folder path */
        retval =  BT_pbap_get_vcard_list_pl
                  (
                      pbap_appl_inst->path,
                      appl_pb_vcard_list,
                      &appl_pb_vcard_count
                  );

#if 0
        if ((NULL != appl_params) &&
            (PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_MAX_LIST_COUNT)) &&
            (0x00 != appl_params->max_list_count))
#endif /* 0 */
        {
            LOG_DEBUG("file_object: %s\n", file_object);

            /* Open the phone book object to create */
            retval = BT_fops_file_open
                     (
                         file_object,
                         (UCHAR *)"wb",
                         &fp
                     );

            if ((NULL != appl_params) &&
                (PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_MAX_LIST_COUNT)) &&
                (0x00U != appl_params->max_list_count) &&
                (appl_params->max_list_count < appl_pb_vcard_count))
             {
                 appl_pb_vcard_count = appl_params->max_list_count;
             }
        }

        /* Parse vcards in given folder and build vcard based on the request */
        for (i = vcard_start_index; i < appl_pb_vcard_count; i++)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                pbap_appl_inst->path,
                appl_pb_vcard_list[i].vcard_file_name,
                vcard_abs_name
            );

            /* open vcard from the list */
            retval = BT_fops_file_open (vcard_abs_name, (UCHAR *)"rb", &vcard_fp);
            if ((API_SUCCESS == retval) && (NULL != vcard_fp))
            {
                /* Get the vcard size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(vcard_fp, &in_vcard_data_len);

                in_vcard_data = BT_alloc_mem (in_vcard_data_len);
                if (NULL == in_vcard_data)
                {
                    LOG_DEBUG("Failed to allocate size: %d\n", in_vcard_data_len);
                }

                if (NULL != in_vcard_data)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                    (
                        in_vcard_data,
                        (UINT16)in_vcard_data_len,
                        vcard_fp,
                        &vcard_data_actual
                    );
                }

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(vcard_fp);
                vcard_fp = NULL;
            }
            else
            {
                LOG_DEBUG ("Failed to open file\n");
                continue;
            }

            /* Reset pbap_vcard */
            BT_mem_set(&pbap_vcard, 0, sizeof(PBAP_VCARD_OBJECT));

            BT_str_copy(pbap_vcard.vcard_file_name, appl_pb_vcard_list[i].vcard_file_name);

            retval = object_parser_init (&vcard_context, appl_pbab_vcard_parser_cb);
            /* Set the context data and call the parser */
            vcard_context.buffer = in_vcard_data;
            vcard_context.buffer_len = vcard_data_actual;

            /* Alllocating buffer for vcard to be built */
            out_vcard_data = BT_alloc_mem (in_vcard_data_len);
            if (NULL == out_vcard_data)
            {
                LOG_DEBUG("Failed to allocate size: %d\n", in_vcard_data_len);
            }

            /* Parse vcard using object parser */
            retval = object_parser_parse (&vcard_context);

            vcard_prop_filter_mask = 0xFFFFFFFFU;

            /* Apply filtering if applicable */
            if (NULL != appl_params)
            {
#ifdef PBAP_1_2
               /**
                * if vCardSelect operator is not received in the request,
                * then set it to OR as default operator.
                */
                if (0x00U == PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_VCARD_SELECTOR_OPERATOR))
                {
                    appl_params->vcard_selector_operator = 0x00U;
                }

                /* Apply VCARD selector filtering */
                if (0U != (PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_VCARD_SELECTOR)))
                {
                   /* Extract initial 32 bits of vcard selector field */
                    vcard_prop_bit_mask = (UINT32 )appl_params->vcard_selector[1U];
                    vcard_prop_bit_mask =  (vcard_prop_bit_mask << 16U) |(UINT32 )appl_params->vcard_selector[0U];

                    vcard_prop_selector_mask =   vcard_prop_bit_mask &  pbap_vcard.prop_bit_mask;

                    if (((0x00U == appl_params->vcard_selector_operator) &&
                         (0x00U == vcard_prop_selector_mask )) ||
                        ((0x01U == appl_params->vcard_selector_operator) &&
                         (vcard_prop_selector_mask != vcard_prop_bit_mask)))
                    {
                        /*
                         * Skip adding vcard to phone book and start processing
                         * next vcard
                         */

                        /* Free the memories */
                        BT_free_mem(out_vcard_data);
                        BT_free_mem(in_vcard_data);

                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_fops_file_close(vcard_fp);
                        vcard_fp = NULL;
                        continue;
                    }
                    actual_vcard_count += 1U;
                }
                else
                {
                    actual_vcard_count += 1U;
                }
#endif /* PBAP_1_2 */

                /* Apply vcard property selector filtering */
                if (0U != (PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_FILTER)))
                {
                    vcard_prop_filter_mask = (UINT32 )appl_params->filter[1U];
                    vcard_prop_filter_mask =  (vcard_prop_filter_mask << 16U) |(UINT32 )appl_params->filter[0U];
                }
            }

#if 0
            if ((NULL != appl_params) &&
                (PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_MAX_LIST_COUNT)) &&
                (0x00 != appl_params->max_list_count))
#endif /* 0 */
            {
                /* Build vcard with filtering */
                retval =  BT_pbap_build_vcard_pl
                          (
                              &pbap_vcard,
                              vcard_prop_filter_mask,
                              in_vcard_data,
                              vcard_data_actual,
                              out_vcard_data,
                              &out_vcard_data_len
                          );

                if (API_SUCCESS == retval)
                {
                    /* Write to file */
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(out_vcard_data, out_vcard_data_len, fp, &vcard_data_actual);
                }
            }

            /* Free memory */
            BT_free_mem (in_vcard_data);
            BT_free_mem (out_vcard_data);
        }

        /* Close built phone book object file */
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_fops_file_close(fp);
        fp = NULL;

        if ((NULL != appl_params) &&
            (PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_MAX_LIST_COUNT)) &&
            (0x00U == appl_params->max_list_count))
        {
            /* vcard_count = appl_params->list_start_offset; */
#if 1
        /* Add NewMissedCalls appl. header */
        if (((NULL != BT_str_str(pbap_headers->pbap_req_info->name->value, "mch.vcf")) ||
             (NULL != BT_str_str(pbap_headers->pbap_req_info->name->value, "cch.vcf"))) &&
            (0x00U != appl_pb_new_missed_calls_flag))
        {
            /* Add appl. param hdr & calculate its length */
            rsp_appl_param_hdr_len += 3U;

            /* Add Primary Folder Version */
            PBAP_SET_APPL_PARAM_FLAG
            (
                pbap_appl_params.appl_param_flag,
                PBAP_FLAG_NEW_MISSED_CALLS
            );

            rsp_appl_param_hdr_len += 4U;
            pbap_appl_params.new_missed_calls = appl_pb_new_missed_calls;
        }
#endif /* 1 */

#ifdef PBAP_1_2

            /* Add Primary Folder Version */
            PBAP_SET_APPL_PARAM_FLAG
            (
                pbap_appl_params.appl_param_flag,
                PBAP_FLAG_PRIMARY_FOLDER_VERSION
            );

            BT_mem_copy
            (
                pbap_appl_params.primary_folder_ver,
                sample_primay_folder_version,
                PBAP_FLDR_VER_CNTR_SIZE
            );

            /* Add Secondary Folder Version */
            PBAP_SET_APPL_PARAM_FLAG
            (
                pbap_appl_params.appl_param_flag,
                PBAP_FLAG_SECONDARY_FOLDER_VERSION
            );

            BT_mem_copy
            (
                pbap_appl_params.secondary_folder_ver,
                sample_secondary_folder_version,
                PBAP_FLDR_VER_CNTR_SIZE
            );

            /* Add Data Base Identifier */
            PBAP_SET_APPL_PARAM_FLAG
            (
                pbap_appl_params.appl_param_flag,
                PBAP_FLAG_DATABASE_IDENTIFIER
            );

            BT_mem_copy
            (
                pbap_appl_params.database_identifier,
                sample_database_identifier,
                PBAP_DATABASE_IDENTIFIER_SIZE
            );
#endif /* PBAP_1_2 */

            /* PhoneBook Size */
            PBAP_SET_APPL_PARAM_FLAG
            (
                pbap_appl_params.appl_param_flag,
                PBAP_FLAG_PHONE_BOOK_SIZE
            );

            /* pbap_appl_params.phonebook_size = appl_pb_vcard_count; */
            LOG_DEBUG("pbap_appl_params.phonebook_size: 0x%04x \n", actual_vcard_count);
            pbap_appl_params.phonebook_size = actual_vcard_count;

            rsp_info.appl_params = &pbap_appl_params;

            event_result = PBAP_SUCCESS_RSP;

            if ((PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_RESET_NEW_MISSED_CALLS)) &&
                (0x01U == appl_params->reset_new_missed_calls))
            {
                /* Reset New missed calls */
                appl_pb_new_missed_calls = 0x00U;
            }

            LOG_DEBUG("Sending Success response_0 \n");
            goto SEND_PULL_PHONE_BOOK_RSP;
        }

        LOG_DEBUG("file_object: %s\n", file_object);

        /* Open the same phone book object file and send */
        retval = BT_fops_file_open
                 (
                     file_object,
                     (UCHAR *)"rb",
                     &fp
                 );
        if ((API_SUCCESS == retval) ||
            (NULL != fp))
        {
            /* Get the phone book size */
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(fp, &fsize);

            remaining = fsize;
        }
        else
        {
            LOG_DEBUG ("Failed to Open Phone Book file\n");

            event_result = PBAP_BAD_REQ_RSP;
            goto SEND_PULL_PHONE_BOOK_RSP;
        }

        rsp_info.appl_params = &pbap_appl_params;

        /* For TC_PSE_PBD_BV_19_C, PBAP_FLAG_NEW_MISSED_CALLS should not be present in the rsp */

#if 1
        /* Add NewMissedCalls appl. header */
        if (((NULL != BT_str_str(pbap_headers->pbap_req_info->name->value, "mch.vcf")) ||
             (NULL != BT_str_str(pbap_headers->pbap_req_info->name->value, "cch.vcf"))) &&
             (0x00U != appl_pb_new_missed_calls_flag))
        {
            /* Add appl. param hdr & calculate its length */
            rsp_appl_param_hdr_len += 3U;

            /* Add Primary Folder Version */
            PBAP_SET_APPL_PARAM_FLAG
            (
                pbap_appl_params.appl_param_flag,
                PBAP_FLAG_NEW_MISSED_CALLS
            );

            rsp_appl_param_hdr_len += 4U;

            LOG_DEBUG("--> appl_pb_new_missed_calls_1: 0x%02x <--\n", appl_pb_new_missed_calls);

            if ((PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_RESET_NEW_MISSED_CALLS)) &&
                (0x01U == appl_params->reset_new_missed_calls))
            {
                pbap_appl_params.new_missed_calls = appl_pb_new_missed_calls;
                appl_pb_new_missed_calls = 0x00U;
            }
            else
            {
                pbap_appl_params.new_missed_calls = appl_pb_new_missed_calls;
            }

            LOG_DEBUG("--> appl_pb_new_missed_calls_2: 0x%02x <--\n", appl_pb_new_missed_calls);
        }
#endif /* 1 */

#ifdef PBAP_1_2

        /* Add appl. param hdr & calculate its length */
        rsp_appl_param_hdr_len += 3U;

        /* Add Primary Folder Version */
        PBAP_SET_APPL_PARAM_FLAG
        (
            pbap_appl_params.appl_param_flag,
            PBAP_FLAG_PRIMARY_FOLDER_VERSION
        );

        BT_mem_copy
        (
            pbap_appl_params.primary_folder_ver,
            sample_primay_folder_version,
            PBAP_FLDR_VER_CNTR_SIZE
        );

        rsp_appl_param_hdr_len += 18U;

        /* Add Secondary Folder Version */
        PBAP_SET_APPL_PARAM_FLAG
        (
            pbap_appl_params.appl_param_flag,
            PBAP_FLAG_SECONDARY_FOLDER_VERSION
        );

        BT_mem_copy
        (
            pbap_appl_params.secondary_folder_ver,
            sample_secondary_folder_version,
            PBAP_FLDR_VER_CNTR_SIZE
        );

        rsp_appl_param_hdr_len += 18U;

        /* Add Data Base Identifier */
        PBAP_SET_APPL_PARAM_FLAG
        (
            pbap_appl_params.appl_param_flag,
            PBAP_FLAG_DATABASE_IDENTIFIER
        );

        BT_mem_copy
        (
            pbap_appl_params.database_identifier,
            sample_database_identifier,
            PBAP_DATABASE_IDENTIFIER_SIZE
        );

        rsp_appl_param_hdr_len += 18U;

        if (0U != (PBAP_GET_APPL_PARAM_FLAG(appl_params->appl_param_flag, PBAP_FLAG_VCARD_SELECTOR)))
        {
            rsp_appl_param_hdr_len += 4U;

            /* PhoneBook Size */
            PBAP_SET_APPL_PARAM_FLAG
            (
                pbap_appl_params.appl_param_flag,
                PBAP_FLAG_PHONE_BOOK_SIZE
            );

            LOG_DEBUG("PD: actual_vcard_count0x%04x\n", actual_vcard_count);
            pbap_appl_params.phonebook_size = actual_vcard_count;
        }

#endif /* PBAP_1_2 */

        /* Calculate body hdr length in 1st rsp */
        if (remaining > (UINT32 )(pbap_max_xchg_size - rsp_appl_param_hdr_len))
        {
            body_rsp.length = (pbap_max_xchg_size - rsp_appl_param_hdr_len);
            event_result = PBAP_CONTINUE_RSP;
        }
        else if (0U != remaining)
        {
            body_rsp.length = (UINT16)remaining;
            event_result = PBAP_SUCCESS_RSP;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_rsp.length)
        {
            body_rsp.value = BT_alloc_mem(body_rsp.length);
        }

        if (NULL != body_rsp.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read (body_rsp.value, body_rsp.length, fp, &actual);
        }

        sent += body_rsp.length;
        remaining = fsize - sent;

        rsp_info.body = &body_rsp;
    }
    else
    {
        if (remaining > pbap_max_xchg_size)
        {
            event_result = PBAP_CONTINUE_RSP;
            body_rsp.length = pbap_max_xchg_size;
        }
        else if (0U != remaining)
        {
            event_result = PBAP_SUCCESS_RSP;
            body_rsp.length = (UINT16)remaining;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (0U != body_rsp.length)
        {
            body_rsp.value = BT_alloc_mem(body_rsp.length);
        }

        if (NULL != body_rsp.value)
        {
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read (body_rsp.value, body_rsp.length, fp, &actual);
        }

        sent += body_rsp.length;
        remaining = fsize - sent;

        rsp_info.body = &body_rsp;
    }

SEND_PULL_PHONE_BOOK_RSP:
    retval = BT_pbap_pse_send_response
             (
                 pbap_handle,
                 PBAP_PSE_GET_PHONEBOOK_IND,
                 event_result,
                 &pbap_rsp_hdrs
             );
    if(API_SUCCESS != retval)
    {
        LOG_DEBUG("Failed to send the Response. Reason 0x%04X\n", retval);
    }
    else
    {
        if (NULL != body_rsp.value)
        {
            BT_free_mem(body_rsp.value);
            body_rsp.value = NULL;
        }

        if (0U == remaining)
        {
            /* Reset the variables */
            sent = 0U;
            fsize = 0U;

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close (fp);
            fp = NULL;
        }

        LOG_DEBUG("Successfully sent the PBAP Response \n");
    }

    /* Take backup of path */
    BT_str_copy(pbap_appl_inst->path, pbap_appl_inst->tpath);
    pbap_appl_inst->root_depth = pbap_appl_inst->troot_depth;

    return retval;
}

/**
 * This callback will store all the vcard properties and required information
 * for a building a vCard.
 */
void appl_pbab_vcard_parser_cb
     (
         /* IN */ OBJECT_PARSER_CB_PARAM cb_param,
         /* IN */ OBJECT_PARSER_CONTEXT * context
     )
{
    static UINT16   vcard_param_id = BEGIN;
    static UINT16   vcp_idx = 0U; /* vcard property index */
    static UCHAR    vcpp_idx = 0U; /* vcard property parameter index */
    static UCHAR    num_prop = 0U; /* no. of vcard properties */
    static UCHAR    param_flag = 0U;
#ifdef OBJ_PARSER_CONSOLE_LOGS
    UINT16 i;
#endif /*n OBJ_PARSER_CONSOLE_LOGS */

    BT_IGNORE_UNUSED_PARAM(context);

    switch (cb_param.keyword_type)
    {
    case OBJECT_TOKEN_GRP_NAME:
        #ifdef OBJ_PARSER_CONSOLE_LOGS
        LOG_DEBUG ("GROUP NAME ---> ");
        #endif /* OBJ_PARSER_CONSOLE_LOGS */
        break;

    case OBJECT_TOKEN_PROP_NAME:
        #ifdef OBJ_PARSER_CONSOLE_LOGS
        LOG_DEBUG ( "Property NAME ---> ");
        #endif /* OBJ_PARSER_CONSOLE_LOGS */
        vcard_param_id = cb_param.keyword_id;

        vcp_idx = num_prop;

        pbap_vcard.info[vcp_idx].prop_name_id = vcard_param_id;

        pbap_vcard.info[vcp_idx].prop_name_len = cb_param.value_len;

        pbap_vcard.info[vcp_idx].prop_name = cb_param.value;

        /* Set property bit mask */
        switch (vcard_param_id)
        {
            case VERSION:
                pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_VERSION;
                pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_VERSION;
                break;

            case FN:
                pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_FN;
                pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_FN;
                break;

            case N:
                pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_N;
                pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_N;
                break;

            case PHOTO:
                pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_PHOTO;
                pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_PHOTO;
                break;

            case TEL:
                pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_TEL;
                pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_TEL;
                break;

            case EMAIL:
                pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_EMAIL;
                pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_EMAIL;
                break;

            case ORG:
                pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_ORG;
                pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_ORG;
                break;

            case BDAY:
                pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_BDAY;
                pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_BDAY;
                break;

            case ADR:
                pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_ADR;
                pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_ADR;
                break;

            case REV:
                pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_REV;
                pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_REV;
                break;

            case TITLE:
                pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_TITLE;
                pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_TITLE;
                break;

            case NOTE:
                pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_NOTE;
                pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_NOTE;
                break;

            case LABEL:
                pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_LABEL;
                pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_LABEL;
                break;

            case X_BT_SPEEDDIALKEY:
                pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_X_BT_SPEEDDIALKEY;
                pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_X_BT_SPEEDDIALKEY;
                break;

            case X_BT_UID:
                if (0x00U != (appl_pb_vcard_prty_flag & 0x02U))
                {
                    pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_X_BT_UID;
                    pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_X_BT_UID;
                }
                break;

            case X_BT_UCI:
                if (0x00U != (appl_pb_vcard_prty_flag & 0x01U))
                {
                    pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_X_BT_UCI;
                    pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_X_BT_UCI;
                }
                break;

            case X_IRMC_CALL_DATETIME:
                pbap_vcard.prop_bit_mask |= VCARD_PROP_BIT_MASK_X_IRMC_CALL_DATETIME;
                pbap_vcard.info[vcp_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_X_IRMC_CALL_DATETIME;
                break;

            default:
                /* LOG_DEBUG("Invalid Param ID: %d\n", vcard_param_id); */
                break;
        }

        num_prop ++;
        vcpp_idx = 0U;
        param_flag = 0x00U;

        break;

    case OBJECT_TOKEN_PROP_VAL:
        #ifdef OBJ_PARSER_CONSOLE_LOGS
        LOG_DEBUG ("Property VALUE ---> ");

        if (0U != (cb_param.parser_result & OBJECT_RESULT_PARTIAL_PARSED))
        {
            LOG_DEBUG ( "<Partial value> ");
        }
        #endif /* OBJ_PARSER_CONSOLE_LOGS */

        pbap_vcard.info[vcp_idx].prop_val_len = cb_param.value_len;
        pbap_vcard.info[vcp_idx].prop_val = cb_param.value;

        pbap_vcard.info[vcp_idx].num_params = vcpp_idx;

        switch (vcard_param_id)
        {
        case BEGIN:
        #ifdef OBJ_PARSER_CONSOLE_LOGS
            LOG_DEBUG ("BEGIN Property VALUE ---> ");
        #endif /* OBJ_PARSER_CONSOLE_LOGS */
            break;

        case END:
            pbap_vcard.num_properties = num_prop;

            num_prop = 0U;
            vcp_idx = 0U;
            break;

        case VERSION:
            break;

        case N:
        #ifdef OBJ_PARSER_CONSOLE_LOGS
            LOG_DEBUG ("NAME Property VALUE ---> ");
        #endif /* OBJ_PARSER_CONSOLE_LOGS */
            break;

        case ENCODING:
            break;

        case CHARSET:
            break;

        default:
            /* LOG_DEBUG("Invalid Param ID: %d\n", vcard_param_id); */
            break;
        }
        break;

    case OBJECT_TOKEN_PROP_PARAM_NAME:
        #ifdef OBJ_PARSER_CONSOLE_LOGS
        LOG_DEBUG ("Property PARAM NAME ---> ");
        #endif /* OBJ_PARSER_CONSOLE_LOGS */

        pbap_vcard.info[vcp_idx].parm_info[vcpp_idx].param_name_len =
                                    cb_param.value_len;

        pbap_vcard.info[vcp_idx].parm_info[vcpp_idx].param_name = cb_param.value;

        vcpp_idx++;
        param_flag = 0xFFU;
        break;

    case OBJECT_TOKEN_PROP_PARAM_VAL:
        #ifdef OBJ_PARSER_CONSOLE_LOGS
        LOG_DEBUG ("Property PARAM VALUE ---> ");
        #endif /* OBJ_PARSER_CONSOLE_LOGS */

        if (0xFFU == param_flag)
        {
           vcpp_idx --;
        }

        pbap_vcard.info[vcp_idx].parm_info[vcpp_idx].param_value_len =
             cb_param.value_len;

        pbap_vcard.info[vcp_idx].parm_info[vcpp_idx].param_value = cb_param.value;

        vcpp_idx ++;
        param_flag = 0x00U;
        break;

    case OBJECT_TOKEN_ERROR:
        #ifdef OBJ_PARSER_CONSOLE_LOGS
        LOG_DEBUG ("***ERROR*** ---> ");
        #endif /* OBJ_PARSER_CONSOLE_LOGS */
        break;

    default:
        LOG_DEBUG("Invalid Keyword Type: %d\n", cb_param.keyword_type);
        break;
    }

#ifdef OBJ_PARSER_CONSOLE_LOGS
    LOG_DEBUG ("[KEY : 0x%04x] ", cb_param.keyword_id);

    for (i = 0U; i < cb_param.value_len; i++)
    {
        LOG_DEBUG ("%c", cb_param.value [i]);
    }
    LOG_DEBUG ("\n");

    if (0U != (cb_param.parser_result & 0x01U))
    {
        LOG_DEBUG ("Folded Line Detected\n");
    }

#endif /* OBJ_PARSER_CONSOLE_LOGS */

    return;
}

/**
 * This callback will extract the N, TEL, UID properties of the vCard.
 */
void appl_pbab_vcard_list_parser_cb
     (
         /* IN */ OBJECT_PARSER_CB_PARAM cb_param,
         /* IN */ OBJECT_PARSER_CONTEXT * context
     )
{
    static UINT16 vcard_param_id = BEGIN;

#ifdef OBJ_PARSER_CONSOLE_LOGS
    UINT16 i;
#endif /* OBJ_PARSER_CONSOLE_LOGS */

    BT_IGNORE_UNUSED_PARAM(context);

    switch (cb_param.keyword_type)
    {
    case OBJECT_TOKEN_GRP_NAME:
#ifdef OBJ_PARSER_CONSOLE_LOGS
        LOG_DEBUG ("GROUP NAME ---> ");
#endif /* OBJ_PARSER_CONSOLE_LOGS */
        break;

    case OBJECT_TOKEN_PROP_NAME:
#ifdef OBJ_PARSER_CONSOLE_LOGS
        LOG_DEBUG ( "Property NAME ---> ");
#endif /* OBJ_PARSER_CONSOLE_LOGS */
        vcard_param_id = cb_param.keyword_id;

        /* Set property bit mask */
        switch (vcard_param_id)
        {
            case VERSION:
                appl_pb_vcard_list[vc_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_VERSION;
                break;

            case FN:
                appl_pb_vcard_list[vc_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_FN;
                break;

            case N:
                appl_pb_vcard_list[vc_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_N;
                break;

            case PHOTO:
                appl_pb_vcard_list[vc_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_PHOTO;
                break;

            case TEL:
                appl_pb_vcard_list[vc_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_TEL;
                break;

            case EMAIL:
                appl_pb_vcard_list[vc_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_EMAIL;
                break;

            case ADR:
                appl_pb_vcard_list[vc_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_ADR;
                break;

            case X_BT_SPEEDDIALKEY:
                appl_pb_vcard_list[vc_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_X_BT_SPEEDDIALKEY;
                break;

            case X_BT_UCI:
                appl_pb_vcard_list[vc_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_X_BT_UCI;
                break;

            case X_BT_UID:
                appl_pb_vcard_list[vc_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_X_BT_UID;
                break;

            case X_IRMC_CALL_DATETIME:
                appl_pb_vcard_list[vc_idx].prop_bit_mask |= VCARD_PROP_BIT_MASK_X_IRMC_CALL_DATETIME;
                break;

            default:
                /* LOG_DEBUG("Invalid Param ID: %d\n", vcard_param_id); */
                break;
        }

        break;

    case OBJECT_TOKEN_PROP_VAL:
#ifdef OBJ_PARSER_CONSOLE_LOGS
        LOG_DEBUG ("Property VALUE ---> ");

        if (0U != (cb_param.parser_result & OBJECT_RESULT_PARTIAL_PARSED))
        {
            LOG_DEBUG ( "<Partial value> ");
        }
#endif  /* OBJ_PARSER_CONSOLE_LOGS */

        switch (vcard_param_id)
        {
        case N:
        #ifdef OBJ_PARSER_CONSOLE_LOGS
            LOG_DEBUG ("NAME Property VALUE ---> ");
        #endif  /* OBJ_PARSER_CONSOLE_LOGS */
            BT_mem_copy
            (
                appl_pb_vcard_list[vc_idx].n,
                cb_param.value,
                cb_param.value_len
            );

            appl_pb_vcard_list[vc_idx].n_len = (UCHAR )cb_param.value_len;
            break;

        case TEL:
        #ifdef OBJ_PARSER_CONSOLE_LOGS
            LOG_DEBUG ("TEL Property VALUE ---> ");
        #endif  /* OBJ_PARSER_CONSOLE_LOGS */

            /* Storing only first occurance of TEL property value */
            if(0x00U == appl_pb_vcard_list[vc_idx].tel_len)
            {
                BT_mem_copy
                (
                    appl_pb_vcard_list[vc_idx].tel,
                    cb_param.value,
                    cb_param.value_len
                );

                appl_pb_vcard_list[vc_idx].tel_len = (UCHAR )cb_param.value_len;
            }
            break;

        case X_BT_UID:
        #ifdef OBJ_PARSER_CONSOLE_LOGS
            LOG_DEBUG ("X_BT_UID Property VALUE ---> ");
        #endif  /* OBJ_PARSER_CONSOLE_LOGS */
            BT_mem_copy
            (
                appl_pb_vcard_list[vc_idx].vcard_bt_uid,
                cb_param.value,
                cb_param.value_len
            );

            appl_pb_vcard_list[vc_idx].vcard_bt_uid_len = (UCHAR )cb_param.value_len;
            break;

        default:
            /* LOG_DEBUG("Invalid Param ID: %d\n", vcard_param_id); */
            break;
        }
        break;

    case OBJECT_TOKEN_PROP_PARAM_NAME:
        #ifdef OBJ_PARSER_CONSOLE_LOGS
        LOG_DEBUG ("Property PARAM NAME ---> ");
        #endif  /* OBJ_PARSER_CONSOLE_LOGS */
        break;

    case OBJECT_TOKEN_PROP_PARAM_VAL:
        #ifdef OBJ_PARSER_CONSOLE_LOGS
        LOG_DEBUG ("Property PARAM VALUE ---> ");
        #endif  /* OBJ_PARSER_CONSOLE_LOGS */
        break;

    case OBJECT_TOKEN_ERROR:
        #ifdef OBJ_PARSER_CONSOLE_LOGS
        LOG_DEBUG ("***ERROR*** ---> ");
        #endif  /* OBJ_PARSER_CONSOLE_LOGS */
        break;

    default:
        LOG_DEBUG("Invalid Keyword Type: %d\n", cb_param.keyword_type);
        break;
    }

#ifdef OBJ_PARSER_CONSOLE_LOGS
    LOG_DEBUG ("[KEY : 0x%04x] ", cb_param.keyword_id);

    for (i = 0U; i < cb_param.value_len; i++)
    {
        LOG_DEBUG ("%c", cb_param.value [i]);
    }
    LOG_DEBUG ("\n");

    if (0U != (cb_param.parser_result & 0x01U))
    {
        LOG_DEBUG ("Folded Line Detected\n");
    }
#endif  /* OBJ_PARSER_CONSOLE_LOGS */

    return;
}

/**
 * This function will change current phonebook path to requested folder and
 * will create requested phonebook object(including folder path) name.
 */
API_RESULT appl_pbab_set_to_get_phonebook_req_path
           (
               /* INOUT */ APPL_PBAP_PSE_STRUCT  * pb_appl_inst,
               /* IN    */ UCHAR * pb_file_name,
               /* OUT   */ UCHAR * pb_object
           )
{
    API_RESULT  retval;
    UCHAR       *pb_ptr;
    UCHAR       index;
    UCHAR       pb_name[10U], pb_folder[10U];

    retval  = API_SUCCESS;
    index   = 0U;

    /* Validate params */
    if ((NULL == pb_file_name) ||
        (NULL == pb_object) ||
        (NULL == pb_appl_inst))
    {
        LOG_DEBUG ("Invalid input params\n");
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Set to root folder */
        if (0U != pb_appl_inst->root_depth)
        {
             while (0U != pb_appl_inst->root_depth)
             {
                retval = BT_vfops_set_path_backward(pb_appl_inst->path);
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("Failed to set path backward\n");
                }

                pb_appl_inst->root_depth--;
             }
        }

        /* Check if requested phonebook file has 'SIMI' */
        pb_ptr = (UCHAR *)BT_str_str(pb_file_name, "SIM1");
        if (NULL == pb_ptr)
        {
            /* pb_file_name does not have 'SIM1', check for 'telecom' */
            pb_ptr = (UCHAR *)BT_str_str(pb_file_name, "telecom");
            if (NULL != pb_ptr)
            {
                index += 8U;
                BT_str_n_copy (pb_name, &pb_file_name[index], (sizeof(pb_name) - 1));
            }
            else
            {
                /* Requested pb_file_name is not valid, Send Error */
                retval = API_FAILURE; /* return API_FAILURE; */
            }
        }
        else
        {
            index += 5U;

            pb_ptr = (UCHAR *)BT_str_str(&pb_file_name[index], (UCHAR *)"telecom");
            if (NULL != pb_ptr)
            {
                index += 8U;
                BT_str_n_copy (pb_name, &pb_file_name[index], (sizeof(pb_name) - 1));
            }
            else
            {
                /* Requested pb_file_name is not valid, Send Error */
                retval = API_FAILURE; /* return API_FAILURE; */
            }
        }

        if (API_SUCCESS == retval)
        {
            UCHAR i;

            for (i = 0U; i < (UINT16)BT_str_len(pb_name); i++)
            {
                if ('.' == pb_name[i])
                {
                    pb_folder[i] = '\0';

                    break;
                }
                else
                {
                    pb_folder[i] = pb_name[i];
                }
            }

            /* Set path to 'SIMI1' folder */
            if (13U == index)
            {
                /* Change the path to 'SIMI' folder */
                retval = BT_vfops_set_path_forward
                         (
                             pbap_appl_inst->path,
                             (UCHAR *)"SIM1"
                         );

                if (API_SUCCESS != retval)
                {
                    retval = API_FAILURE; /* return API_FAILURE; */
                }
                else
                {
                    pb_appl_inst->root_depth++;
                }
            }

            if (API_SUCCESS == retval)
            {
                /* Change the path to 'telecom' folder */
                retval = BT_vfops_set_path_forward
                         (
                             pbap_appl_inst->path,
                             (UCHAR *)"telecom"
                         );
                if (API_SUCCESS != retval)
                {
                    retval = API_FAILURE; /* return API_FAILURE; */
                }
                else
                {
                    pb_appl_inst->root_depth++;
                }
            }

            if (API_SUCCESS == retval)
            {
                /* Create the phone book file with path */
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    pbap_appl_inst->path,
                    pb_name,
                    pb_object
                );

                /* Change the path to requested pb folder(pb, ich, och, cch, sdp, fav) */
                retval = BT_vfops_set_path_forward
                         (
                             pbap_appl_inst->path,
                             pb_folder
                         );
                if (API_SUCCESS != retval)
                {
                    retval = API_FAILURE; /* return API_FAILURE; */
                }
                else
                {
                    pb_appl_inst->root_depth ++;
                }
            }
        }
    }

    return retval;
}

#endif /* PBAP_PSE */

