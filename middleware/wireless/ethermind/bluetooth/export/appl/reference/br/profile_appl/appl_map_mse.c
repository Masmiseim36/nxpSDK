
/**
 *  \file appl_map_mse.c
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_map.h"
#include "appl_utils.h"


#ifdef MAP_MSE

#define APPL_MAP_AUTO_NTF_ACTION

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
static UCHAR               bd_addr[BT_BD_ADDR_SIZE];

static UCHAR               map_app_instance;
static UCHAR               map_server_channel;

static MAP_MAS_INSTANCE    mas_instance[MAP_SERVER_NUM_INSTANCES];
static MAP_INSTANCE        mns_instance[MAP_SERVER_NUM_INSTANCES];

/* Static file handle */
static BT_fops_file_handle fd;

#ifdef HAVE_OBJECT_PARSER
static OBJECT_PARSER_CONTEXT    map_context;
static UCHAR               map_parse_buffer[OBEX_MAX_PACKET_LENGTH + APPL_MAP_MAX_TAG_LEN];
static UINT16              map_parser_diff_len;
static BT_fops_file_handle map_parsed_fp;
static BT_fops_file_handle map_raw_fp;
static MESSAGE_OBJECT      map_msg_object;

static UCHAR     map_file_name[MAP_MESSAGEFILE_NAME_LEN];
static UINT16    map_recv_count = 0U;
#endif /* HAVE_OBJECT_PARSER */

/* File indices */
static UINT32 fsize = 0U;
static UINT32 fsize_event = 0U;
static UINT32 remaining = 0U;

/* MAP object exchange size */
static UINT16 map_xchg_size;
static UINT16 map_ns_xchg_size;
static UINT32 sent_len;
static UINT16 actual;

static UCHAR wait_count;

/* To acount for appl. params size */
static UINT16 appl_map_mse_xchg_size;

#ifdef MAP_1_3

/* Sample Application Parameter Values */
static UCHAR    appl_conv_listing_version_counter[APPL_MAP_CONV_LISTING_VER_CNTR_LEN] =
                {
                    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
                    0x00U, 0x00U, 0x00U, 0x00U, 0x46U, 0x54U, 0x32U, 0x01U
                };

static UCHAR    appl_database_identifier[APPL_MAP_DATABASE_IDENTIFIER_LEN] =
                {
                    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
                    0x00U, 0x00U, 0x00U, 0x00U, 0x76U, 0x54U, 0x32U, 0x01U
                };

static UCHAR    appl_folder_version_counter[APPL_MAP_FOLDER_VER_CNTR_LEN] =
                {
                    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
                    0x00U, 0x00U, 0x00U, 0x00U, 0x56U, 0x78U, 0x90U, 0x01U
                };

static UCHAR   appl_mst_time[]        = "20151012T130510+0100";

static MAP_IM_OWNER_STATUS appl_map_im_conversations[MAX_APPL_IM_CONVERSATIONS];
static UCHAR               appl_map_im_converse_index;

#endif /* MAP_1_3 */

#ifdef MAP_SUPPORT_NOTIFICATION
UCHAR ntf_status = 0xFFU;
#endif /* MAP_SUPPORT_NOTIFICATION */

static const UCHAR map_mse_menu[] =
" \n \
------------- MAP Server Menu -------------\n \
\t 0. Exit \n \
\t 1. Refresh \n \
\n \
\t 5. MSE Initialize \n \
\t 6. MSE Shutdown \n \
\t 7. MSE Start \n \
\t 8. MSE Stop \n \
\n \
\t12. Do SDP Query \n \
\n \
\t15. MAP Server set number of wait stages \n\
\n \
\t17. Close Transport with MCE \n \
\n \
\t20. Connect MNS to MCE \n \
\t21. Disconnect MNS from MCE \n \
\t22. Close MNS transport with MCE \n \
\n \
\t25. Send Event to MCE \n \
\n \
\t30. Set the application values \n \
\t31. Get the application values \n \
\n \
Your Choice: ";

/* --------------------------------------------- Functions */
API_RESULT appl_map_mse_callback
           (
               MAP_HANDLE_INFO  * map_handle,
               UINT8              event_type,
               UINT16             event_result,
               MAP_HEADERS      * event_data,
               UINT16             event_datalen
           );

void appl_map_mse_mns_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     );

void map_mse_print_appl_instances (void);

#ifdef HAVE_OBJECT_PARSER
void map_bmsg_parser_cb
     (
         OBJECT_PARSER_CB_PARAM cb_param,
         OBJECT_PARSER_CONTEXT * context
     );

void map_vcard_parser_cb
     (
         OBJECT_PARSER_CB_PARAM cb_param,
         OBJECT_PARSER_CONTEXT * context
     );
#endif /* HAVE_OBJECT_PARSER */


void main_map_mse_operations (void)
{
    API_RESULT retval;
    MAP_HANDLE *phandle;
    UINT8 i;
    UCHAR *pbd_addr;
    UCHAR root[8U];
    CHAR name[128U];
    UCHAR   more;
    UINT16 appl_map_body_hdr_len;

#ifdef MAP_SUPPORT_NOTIFICATION
    MAP_CONNECT_STRUCT connect_info;
    MAP_REQUEST_STRUCT set_info;
    MAP_HEADER_STRUCT body_req;
    MAP_APPL_PARAMS appl_param;
    UINT16 size;
#endif /* MAP_SUPPORT_NOTIFICATION */

    UCHAR file_object[MAP_FOLDER_NAME_LEN * 2U];
    int choice, menu_choice, val, handle, sc;

    /* Init */
    more            = 0x00U;
#ifdef HAVE_OBJECT_PARSER
    map_parsed_fp   = NULL;
    map_raw_fp      = NULL;
#endif /* HAVE_OBJECT_PARSER */

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
#ifdef MAP_SUPPORT_NOTIFICATION
    BT_mem_set(&connect_info, 0, sizeof(MAP_CONNECT_STRUCT));
    BT_mem_set(&set_info, 0, sizeof(MAP_REQUEST_STRUCT));
    BT_mem_set(&body_req, 0, sizeof(MAP_HEADER_STRUCT));
    BT_mem_set(&appl_param, 0, sizeof(MAP_APPL_PARAMS));
#endif /* MAP_SUPPORT_NOTIFICATION*/

    BT_LOOP_FOREVER()
    {
        printf ("%s", map_mse_menu);
        scanf ("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
            case 0:
                break; /* return; */

            case 1:
                break;

            case 5:
                LOG_DEBUG ("Initializing MAP MSE...\n");
                retval = BT_map_mse_init ();
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                /* Reset the MAP handle */
                for (i = 0U; i < MAP_SERVER_NUM_INSTANCES; i++)
                {
                    MAP_RESET_INSTANCE(&mas_instance[i].instance);
                    mas_instance[i].mns_id = MAP_ENTITY_INVALID;

                    MAP_RESET_INSTANCE(&mns_instance[i]);
                }

#ifdef MAP_1_3
                BT_mem_set
                (
                    appl_map_im_conversations,
                    0x00,
                    sizeof (MAP_IM_OWNER_STATUS) * MAX_APPL_IM_CONVERSATIONS
                );
#endif /* MAP_1_3 */

                map_mse_print_appl_instances();

#ifdef HAVE_OBJECT_PARSER
                LOG_DEBUG ("Initializing MAP Parser...\n");
                retval = object_parser_init (&map_context, map_bmsg_parser_cb);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                map_msg_object.envelope_level = 0U;
                map_msg_object.envelope.content.message = NULL;
#endif /* HAVE_OBJECT_PARSER */
                break;

            case 6:
                LOG_DEBUG ("Shutting down MAP MSE...\n");
                retval = BT_map_mse_shutdown ();
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 7:
                map_mse_print_appl_instances();

                LOG_DEBUG ("Enter Service Type (1-MAS, 2-MNS): ");
                scanf ("%d", &val);

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                phandle = ((1U == val)?
                    &mas_instance[handle].instance.handle:
                    &mns_instance[handle].handle);

                if (MAP_ENTITY_INVALID != *phandle)
                {
                    LOG_DEBUG ("Application Instance not free!\n");
                    break;
                }

                LOG_DEBUG ("Starting MAP MSE Instance...\n");
                retval = BT_map_mse_start
                         (
                             (MAP_SERVICE_TYPE)val,
                             appl_map_mse_callback,
                             phandle
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                if (API_SUCCESS != retval)
                {
                    *phandle = MAP_ENTITY_INVALID;
                }
                else
                {
                    /* Set path to root folder */
                    BT_str_copy
                    (mas_instance[handle].path, MAP_ROOT_FOLDER_BASE);

                    /* Form the indexed root folder */
                    BT_str_print((char *)root, "%s_%d", MAP_ROOT_FOLDER, handle);

                    retval = BT_vfops_set_path_forward
                             (
                                 mas_instance[handle].path,
                                 root
                             );
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to set path to ROOT folder - 0x%04X\n",
                        retval);
                    }
                    else
                    {
                        mas_instance[handle].root_depth = 0U;
                    }
                }

                map_mse_print_appl_instances();
                break;

            case 8:
                map_mse_print_appl_instances();

                LOG_DEBUG ("Enter Service Type (1-MAS, 2-MNS): ");
                scanf ("%d", &val);

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (1U == val)
                {
                    phandle = &mas_instance[handle].instance.handle;
                    pbd_addr = mas_instance[handle].instance.bd_addr;
                }
                else
                {
                    phandle = &mns_instance[handle].handle;
                    pbd_addr = mns_instance[handle].bd_addr;
                }

                if (MAP_ENTITY_INVALID == *phandle)
                {
                    LOG_DEBUG ("Application Instance not valid!\n");
                    break;
                }

                LOG_DEBUG ("Stopping MAP MSE Service %d...\n", val);
                retval = BT_map_mse_stop ((MAP_SERVICE_TYPE)val, phandle);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                if (API_SUCCESS == retval)
                {
                    *phandle = MAP_ENTITY_INVALID;
                    BT_mem_set (pbd_addr, 0x00, BT_BD_ADDR_SIZE);
                }

                map_mse_print_appl_instances();
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
                 * Start SDP Query to Find out if the Peer Device is running MAP
                 * Service and if so on which Server Channel the Remote MAP PSE
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

                LOG_DEBUG ("Select MSE Entity Instance: ");
                scanf ("%d", &choice);
                map_app_instance = (UCHAR)choice;

                LOG_DEBUG ("Enter peer device address: ");
                appl_get_bd_addr (bd_addr);

                mns_instance[map_app_instance].map_sdp_record.map_len_attrib_data
                                                      = MAP_SDP_RECORD_DATA_SIZE;

                SDP_SET_HANDLE
                (
                    mns_instance[map_app_instance].map_sdp_record.map_sdp_handle,
                    bd_addr,
                    appl_map_mse_mns_sdp_callback
                );

                retval = BT_sdp_open
                         (
                             &mns_instance[map_app_instance].map_sdp_record.map_sdp_handle
                         );

                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG("SDP Connect FAILED !! Error Code = 0x%04X\n", retval);
                }

                LOG_DEBUG("Please Wait for SDP Operation to Complete\n");
                break;

            case 15:
                LOG_DEBUG("Enter number of wait stages: ");
                scanf("%d", &choice);
                wait_count = (UCHAR)choice;
                break;

            case 17:
                map_mse_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                LOG_DEBUG ("Closing on MAP MSE Instance %d\n", handle);
                retval = BT_map_mse_transport_close
                    (&mas_instance[handle].instance.handle);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

#ifdef MAP_SUPPORT_NOTIFICATION
            case 20:
                map_mse_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                LOG_DEBUG ("Enter peer device address: ");
                appl_get_bd_addr (bd_addr);

                BT_mem_set(&connect_info, 0, sizeof(MAP_CONNECT_STRUCT));

                connect_info.bd_addr = bd_addr;

#ifdef OBEX_OVER_L2CAP
                LOG_DEBUG ("Enter peer L2CAP PSM (in Hex): ");
                scanf ("%x", &val);
                connect_info.psm = (UINT16)val;
#endif /* OBEX_OVER_L2CAP */

                LOG_DEBUG ("Enter peer server channel: ");
                scanf ("%x", &sc);
                connect_info.server_channel = (UCHAR)sc;

                LOG_DEBUG ("Enter Data Exchange Size: ");
                scanf ("%d", &val);
                size = (UINT16) val;
                connect_info.max_recv_size = size;

                LOG_DEBUG ("Connecting...\n");
                retval = BT_map_mse_ns_connect
                         (
                             &mns_instance[handle].handle,
                             &connect_info
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 21:
                map_mse_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                LOG_DEBUG ("Disconnecting on MAP MSE Instance %d\n", handle);
                retval = BT_map_mse_ns_disconnect (&mns_instance[handle].handle);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 22:
                map_mse_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                LOG_DEBUG ("Closing on MAP MSE Instance %d\n", handle);
                retval = BT_map_mse_ns_transport_close (&mns_instance[handle].handle);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 25:
                map_mse_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
                BT_mem_set(&set_info, 0, sizeof(MAP_REQUEST_STRUCT));

                set_info.appl_params = &appl_param;
                set_info.body = &body_req;

                LOG_DEBUG ("Enter The MAS Instance ID: ");
                scanf ("%d", &val);

                appl_param.mas_instance_id = (UCHAR) val;

                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_MASINSTANCEID
                );

                LOG_DEBUG ("Enter the Event Report file name: ");
                scanf ("%s", name);

                fsize_event = 0U;
                remaining = 0U;
                sent_len = 0U;

                /**
                 * For BODY Hdr length, Subtract all other(i.e except BODY header) hdrs length
                 * which will be sent in the first PUT request.
                 * Possible Headers in first 'SendEvent' request:
                 *      - Type Hdr,
                 *      - Application parameter (MASInstanceID)
                 *
                 * So Setting appl_map_body_hdr_len = (map_ns_xchg_size - 48)
                 */
                appl_map_body_hdr_len = (map_ns_xchg_size - 48U);

                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)MAP_ROOT_FOLDER_BASE,
                    (UCHAR *)name,
                    file_object
                );

                retval = BT_fops_file_open (file_object, (UCHAR *)"rb", &fd);
                if ((API_SUCCESS != retval) || (NULL == fd))
                {
                    LOG_DEBUG ("File not found!\n");
                    break;
                }

                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size (fd, &fsize_event);

                remaining = fsize_event;

                if (remaining > appl_map_body_hdr_len)
                {
                    body_req.length = appl_map_body_hdr_len;
                    more = 0x01U;
                }
                else if (0U != remaining)
                {
                    body_req.length =  (UINT16 )remaining;
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
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_read (body_req.value, body_req.length, fd, (UINT16 *)&actual);
                }

                LOG_DEBUG ("Sending Event on MAP MSE Instance %d\n", handle);
                retval = BT_map_mse_ns_send_event
                         (
                             &mns_instance[handle].handle,
                             &set_info,
                             more
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                sent_len +=  actual;
                remaining = fsize_event - sent_len;

                /* If operation has failed or completed, perform cleanup */
                if ((API_SUCCESS != retval) || (0U == remaining))
                {
                    sent_len = 0U;
                    fsize_event = 0U;

                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_close (fd);
                    fd = NULL;
                }

                if (NULL != body_req.value)
                {
                    BT_free_mem (body_req.value);
                    body_req.value = NULL;
                }
                break;
#endif /* MAP_SUPPORT_NOTIFICATION */

            case 30:
#ifdef MAP_1_3
                LOG_DEBUG ("Select options below:\n");
                LOG_DEBUG ("    1. Set Conversation Listing Counter. \n");
                LOG_DEBUG ("    2. Set Database Identifier. \n");
                LOG_DEBUG ("    3. Set Folder Version Counter. \n");
                scanf ("%d", &choice);

                if (1 == choice)
                {
                    LOG_DEBUG ("Enter Conversation Listing Counter(LSB): ");
                    scanf ("%d", &val);

                    appl_conv_listing_version_counter[APPL_MAP_CONV_LISTING_VER_CNTR_LEN -1U] = (UCHAR )val;
                }
                else if (2 == choice)
                {
                    LOG_DEBUG ("Enter Database Identifier(LSB): ");
                    scanf ("%d", &val);

                    appl_database_identifier[APPL_MAP_DATABASE_IDENTIFIER_LEN-1U] = (UCHAR )val;
                }
                else if (3 == choice)
                {
                    LOG_DEBUG ("Enter Folder Version Counter(LSB): ");
                    scanf ("%d", &val);

                    appl_folder_version_counter[APPL_MAP_FOLDER_VER_CNTR_LEN-1U] = (UCHAR )val;
                }
                else
                {
                    LOG_DEBUG ("Invalid Choice\n");
                }
#else
                LOG_DEBUG ("Please Enable MAP_1_3 Feature Flag \n");
#endif /* MAP_1_3 */
                break;

            case 31:
#ifdef MAP_1_3
            {
                LOG_DEBUG ("Conversation Listing Counter\n");
                LOG_DEBUG ("    - 0x");
                for (i = 0U; i < APPL_MAP_CONV_LISTING_VER_CNTR_LEN; i++)
                {
                    LOG_DEBUG ("%02x", appl_conv_listing_version_counter[i]);
                }
                LOG_DEBUG ("\n");

                LOG_DEBUG ("Database Identifier\n");
                LOG_DEBUG ("    - 0x");
                for (i = 0U; i < APPL_MAP_DATABASE_IDENTIFIER_LEN; i++)
                {
                    LOG_DEBUG ("%02x", appl_database_identifier[i]);
                }
                LOG_DEBUG ("\n");

                LOG_DEBUG ("Folder Version Counter\n");
                LOG_DEBUG ("    - 0x");
                for (i = 0U; i < APPL_MAP_FOLDER_VER_CNTR_LEN; i++)
                {
                    LOG_DEBUG ("%02x", appl_folder_version_counter[i]);
                }
                LOG_DEBUG ("\n");
            }
#else
                LOG_DEBUG ("Please Enable MAP_1_3 Feature Flag \n");
#endif /* MAP_1_3 */
                break;

            default:
                LOG_DEBUG ("Invalid Option\n");
                break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }
}


API_RESULT appl_map_mse_callback
           (
               MAP_HANDLE_INFO  * map_handle,
               UINT8              event_type,
               UINT16             event_result,
               MAP_HEADERS      * event_data,
               UINT16             event_datalen
           )
{
    MAP_HEADERS * map_headers;
    MAP_HEADERS map_resp_header;
    MAP_HEADER_STRUCT body, name;
    MAP_RESPONSE_STRUCT response_info;
    MAP_APPL_PARAMS appl_param, *appl_param_rcv;
    API_RESULT retval;
    UCHAR send_response;
    UCHAR tx_response;
    MAP_HANDLE handle;

#ifdef MAP_SUPPORT_NOTIFICATION
    MAP_REQUEST_STRUCT  request_info;
    UCHAR    more;
    UCHAR send_request;
#endif /* MAP_SUPPORT_NOTIFICATION */

    UCHAR file_object[MAP_FOLDER_NAME_LEN * 2U];
    UCHAR curr_folder[MAP_FOLDER_NAME_LEN];
    static UCHAR message_file[MAP_MESSAGEFILE_NAME_LEN];

#ifdef MAP_1_2
    UCHAR mas_instinfo[255U] = "Sample MAP Instance Information Description!";
#endif /* MAP_1_2 */

#ifdef MAP_1_4_2
    UCHAR   conversation_id_flag = 0U;
    UCHAR   conversation_obj_file[MAP_CONVSN_OBJ_FILE_LEN];
#endif /* MAP_1_4_2 */

    UINT16 num_items, i, j, temp;
    INT8  child_folder_depth, curr_folder_depth;

    UINT16 length;
    UCHAR * data;

    static UINT32 sent = 0U;
    static UCHAR push_started = 0U;

    BT_IGNORE_UNUSED_PARAM(event_datalen);

#if ((defined MAP_SUPPORT_NOTIFICATION) && (defined APPL_MAP_AUTO_NTF_ACTION))
    ntf_status = 0xFFU;
#endif /* ((defined MAP_SUPPORT_NOTIFICATION) && (defined APPL_MAP_AUTO_NTF_ACTION)) */

    LOG_DEBUG ("\n\nMAP MSE Event: 0x%02X Result: 0x%04X\n\n",
    event_type, event_result);

    /* Init */
    map_headers = event_data;
    send_response = 0U;
    retval = API_SUCCESS;

    tx_response = (UCHAR) event_result;

    BT_mem_set(&map_resp_header, 0, sizeof(MAP_HEADERS));
    BT_mem_set(&response_info, 0, sizeof(MAP_RESPONSE_STRUCT));

    MAP_INIT_HEADER_STRUCT(body);
    MAP_INIT_HEADER_STRUCT(name);

#ifdef MAP_SUPPORT_NOTIFICATION
    BT_mem_set(&request_info, 0, sizeof(MAP_REQUEST_STRUCT));
    send_request = 0U;
    more = 0U;
#endif /* MAP_SUPPORT_NOTIFICATION */

    handle = map_handle->entity_id;

    for (i = 0U; i < MAP_SERVER_NUM_INSTANCES; i++)
    {
        if (mas_instance[i].instance.handle == handle)
        {
            break;
        }
    }

    if (MAP_SERVER_NUM_INSTANCES == i)
    {
        LOG_DEBUG ("Unknown handle\n");
        /* return API_SUCCESS; */
    }
    else
    {
        switch(event_type)
        {
        case MAP_MSE_CONNECT_IND:
            LOG_DEBUG ("Recvd MAP_MSE_CONNECT_IND - %04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            /* Check for valid event result and parameters */
            if((MAP_SUCCESS_RSP != event_result) ||
               (NULL == map_headers->map_connect_info))
            {
                tx_response = MAP_NOT_ACCEPTABLE_RSP;
                send_response = 1U;
                break;
            }

            LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (map_headers->map_connect_info->bd_addr));

            LOG_DEBUG ("MAX Exchange Size: %d\n",
            map_headers->map_connect_info->max_recv_size);
#ifdef MAP_1_3
            if (0U != map_headers->map_connect_info->map_supported_features)
            {
                LOG_DEBUG ("\nPeer MapSupportedFeatures :0x%08X\n\n",
                    map_headers->map_connect_info->map_supported_features);
            }
#endif /* MAP_1_3 */

            BT_COPY_BD_ADDR(bd_addr, map_headers->map_connect_info->bd_addr);
            map_xchg_size = map_headers->map_connect_info->max_recv_size;
            tx_response = MAP_SUCCESS_RSP;

            /* Set response to be sent */
            send_response = 1U;

            remaining = 0U;
            fsize = 0U;
            sent = 0U;

            /* Get the Handle parameters */
            BT_mem_copy
            (
                mas_instance[i].instance.bd_addr,
                map_headers->map_connect_info->bd_addr,
                BT_BD_ADDR_SIZE
            );

            /* Get the Handle parameters */
            for (j = 0U; j < MAP_SERVER_NUM_INSTANCES; j++)
            {
                if (0 == BT_mem_cmp(mns_instance[j].bd_addr,
                    map_headers->map_connect_info->bd_addr,
                    BT_BD_ADDR_SIZE))
                {
                    mas_instance[i].mns_id = (UCHAR)j;
                    break;
                }
            }

            map_mse_print_appl_instances();

            /* Set path to root folder */
            if (0U != mas_instance[i].root_depth)
            {
                while (0U != mas_instance[i].root_depth)
                {
                    retval = BT_vfops_set_path_backward(mas_instance[i].path);
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to set path backward\n");
                    }
                    mas_instance[i].root_depth --;
                }

                LOG_DEBUG ("Folder set to ROOT\n");
            }
            break;

        case MAP_MSE_DISCONNECT_IND:
            LOG_DEBUG ("Recvd MAP_MSE_DISCONNECT_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);
            break;

        case MAP_MSE_TRANSPORT_CLOSE_CNF:
            LOG_DEBUG ("Recvd MAP_MSE_TRANSPORT_CLOSE_CNF - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            BT_mem_set(&mas_instance[i].instance.bd_addr, 0x00U, BT_BD_ADDR_SIZE);

            map_mse_print_appl_instances();
            break;

        case MAP_MSE_TRANSPORT_CLOSE_IND:
            LOG_DEBUG ("Recvd MAP_MSE_TRANSPORT_CLOSE_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            BT_mem_set(&mas_instance[i].instance.bd_addr, 0x00, BT_BD_ADDR_SIZE);

            map_mse_print_appl_instances();
            break;

        case MAP_MSE_UPDATE_INBOX_IND:
            LOG_DEBUG ("Recvd MAP_MSE_UPDATE_INBOX_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            send_response = 1U;

            /* TODO: Not checking if current path is in Inbox */

            /* Create the listing file with path */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)MAP_ROOT_FOLDER_BASE,
                (UCHAR *)MESSAGESLISTING_FILE,
                file_object
            );

            /* Create the listing file */
            retval = BT_map_create_xml_messages_listing_pl
                     (
                         mas_instance[i].path,
                         file_object,
                         NULL,
                         &num_items
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed to Create Message Listing file\n");
                tx_response = MAP_SERVER_ERROR;
                break;
            }

            retval = BT_map_update_inbox_pl
                     (
                         mas_instance[i].path,
                         (num_items + 1U)
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed to Update Inbox\n");
                tx_response = MAP_SERVER_ERROR;
                break;
            }
            break;

        case MAP_MSE_SET_FOLDER_IND:
            LOG_DEBUG ("Recvd MAP_MSE_SET_FOLDER_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            switch(map_headers->map_req_info->setpath_flag)
            {
            case 0x01U:
                LOG_DEBUG("Invalid Set Path Flag settings received\n");

                /* Set Response to PBAP_BAD_REQ_RSP; Req remains same */
                tx_response = MAP_BAD_REQ_RSP;
                break;

            case 0x02U:
                if ((NULL == map_headers->map_req_info->name) ||
                    (0U == map_headers->map_req_info->name->length))
                {
                    LOG_DEBUG ("Set Path with flag to ROOT FOLDER\n");

                    /* Set path to root folder */
                    if (0U == mas_instance[i].root_depth)
                    {
                        LOG_DEBUG ("Already in ROOT folder\n");
                    }
                    else
                    {
                        while (0U != mas_instance[i].root_depth)
                        {
                            retval = BT_vfops_set_path_backward(mas_instance[i].path);
                            if (API_SUCCESS != retval)
                            {
                                LOG_DEBUG ("Failed to set path backward\n");
                            }
                            mas_instance[i].root_depth --;
                        }

                        LOG_DEBUG ("Folder set to ROOT\n");
                    }
                }
                else if((NULL != map_headers->map_req_info->name) &&
                        (map_headers->map_req_info->name->length > 0U) &&
                        (NULL != map_headers->map_req_info->name->value))
                {
                    LOG_DEBUG ("Set Path with flag to CHILD FOLDER: %s\n",
                    map_headers->map_req_info->name->value);

                    retval = BT_vfops_set_path_forward
                             (
                                 mas_instance[i].path,
                                 map_headers->map_req_info->name->value
                             );
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to set path to folder %s - 0x%04X\n",
                        map_headers->map_req_info->name->value, retval);

                        tx_response = MAP_NOT_FOUND_RSP;
                    }
                    else
                    {
                        mas_instance[i].root_depth++;

                        LOG_DEBUG ("Current PATH: %s\n",
                        map_headers->map_req_info->name->value);
                    }

                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }
                break;

            case 0x03U:
                LOG_DEBUG ("Set Path with flag to PARENT FOLDER");
                if((NULL != map_headers->map_req_info->name) &&
                   (map_headers->map_req_info->name->length > 0U) &&
                   (NULL != map_headers->map_req_info->name->value))
                {
                    LOG_DEBUG (": %s",
                    map_headers->map_req_info->name->value);
                }

                /* Check if already in ROOT */
                if (0U == mas_instance[i].root_depth)
                {
                    LOG_DEBUG ("\nAlready in ROOT\n");
                    tx_response = MAP_BAD_REQ_RSP;
                    break;
                }

                /* Get root depth before moving backward */
                curr_folder_depth = mas_instance[i].root_depth;

                /* Set path backward */
                retval = BT_vfops_set_path_backward(mas_instance[i].path);
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("\nFailed to set path backward - 0x%04X\n", retval);
                    tx_response = MAP_NOT_FOUND_RSP;
                }
                else
                {
                    mas_instance[i].root_depth--;
                    LOG_DEBUG ("\nCurrent PATH: %s\n",
                    mas_instance[i].path);
                }

                /* Get the child folder depth to be set w.r.t root folder */
                if ((NULL == map_headers->map_req_info->name) ||
                    (0U == map_headers->map_req_info->name->length) ||
                    ('\0' == map_headers->map_req_info->name->value[0U]))
                {
                    child_folder_depth = 0U;
                }
                else if ((0 == BT_str_cmp("telecom", map_headers->map_req_info->name->value)))
                {
                    child_folder_depth = 1U;
                }
                else if ((0 == BT_str_cmp("msg", map_headers->map_req_info->name->value)))
                {
                    child_folder_depth = 2U;
                }
                else if ((0 == BT_str_cmp("deleted", map_headers->map_req_info->name->value))||
                         (0 == BT_str_cmp("inbox", map_headers->map_req_info->name->value))  ||
                         (0 == BT_str_cmp("outbox", map_headers->map_req_info->name->value))||
                         (0 == BT_str_cmp("sent", map_headers->map_req_info->name->value)))
                {
                    child_folder_depth = 3U;
                }
                else
                {
                    /* Should not reach here */
                    LOG_DEBUG ("\nChild folder is not found\n");
                    child_folder_depth = 4U;
                }

                /* Set the Child folder */
                if ((MAX_MAP_REPOSITORY_DEPTH == child_folder_depth) &&
                    (MAX_MAP_REPOSITORY_DEPTH == curr_folder_depth))
                {
                    /* Set path forward */
                    retval = BT_vfops_set_path_forward
                             (
                                 mas_instance[i].path,
                                 map_headers->map_req_info->name->value
                             );
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to set path to folder %s - 0x%04X\n",
                        map_headers->map_req_info->name->value, retval);
                        tx_response = MAP_NOT_FOUND_RSP;
                    }
                    else
                    {
                        mas_instance[i].root_depth++;
                        LOG_DEBUG ("\nSetting the child: %s\n",
                        map_headers->map_req_info->name->value);

                        LOG_DEBUG ("\nCurrent PATH: %s\n",
                        mas_instance[i].path);
                    }
                }
                else
                {
                    if (0x00U != child_folder_depth)
                    {
                        /* If parent folder is having invalid child */
                        LOG_DEBUG ("\nInvalide Child Folder\n");
                        tx_response = MAP_SERVER_ERROR;
                    }
                }
                LOG_DEBUG ("\n");

            default:
                LOG_DEBUG("Invalid Flag: %d\n", map_headers->map_req_info->setpath_flag);
                break;
            }

            send_response = 1U;
            break;

        case MAP_MSE_GET_FOLDER_LISTING_IND:
            LOG_DEBUG ("Recvd MAP_MSE_GET_FOLDER_LISTING_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

            response_info.body = &body;
            response_info.appl_params = &appl_param;
            map_resp_header.map_resp_info = &response_info;

            if ((MAP_SUCCESS_RSP != event_result) &&
                (MAP_CONTINUE_RSP != event_result))
            {
                if (NULL != fd)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_close (fd);
                    fd = NULL;
                }

                sent = 0U;
                fsize = 0U;

                break;
            }

            send_response = 1U;

            /*
             * TODO:
             * Headers can be present in first or/and subsequent responses also?
             */
            if (0U == sent)
            {
                tx_response = MAP_SUCCESS_RSP;

                /* Create the listing file with path */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)MAP_ROOT_FOLDER_BASE,
                    (UCHAR *)MAP_FOLDERLISTING_FILE,
                    file_object
                );

                /* Create the listing file */
                retval = BT_map_create_xml_folder_listing_pl
                         (
                             mas_instance[i].path,
                             file_object,
                             &num_items
                         );
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("Failed to Create folder Listing file\n");
                    tx_response = MAP_SERVER_ERROR;
                    break;
                }

                /* Open the listing file */
                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"rb",
                             &fd
                         );
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("Failed to Open File to Create folder Listing\n");
                    tx_response = MAP_SERVER_ERROR;
                    break;
                }

                if (NULL != fd)
                {
                    /* Get the file size */
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_size (fd, &fsize);

                    remaining = fsize;
                }
                else
                {
                    tx_response = MAP_NOT_FOUND_RSP;
                    break;
                }

                /* Set sample listing size */
                appl_param.folder_listing_size = num_items;
                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_FOLDERLISTINGSIZE
                );
            }

            appl_param_rcv = map_headers->map_req_info->appl_params;

            /*
             * TODO: For now, only Max List Count is checked. If '0',
             * then set sample listing size and NewMessage Indicator
             * Else, send sample listing body also
             */
            if (NULL != appl_param_rcv)
            {
                if (0U != (MAP_GET_APPL_PARAM_FLAG
                    (
                        appl_param_rcv->appl_param_flag,
                        MAP_FLAG_MAXLISTCOUNT
                    )))
                {
                    LOG_DEBUG ("Max List Count: %d\n", appl_param_rcv->max_list_count);

                    if (0U == appl_param_rcv->max_list_count)
                    {
                        (BT_IGNORE_RETURN_VALUE) BT_fops_file_close(fd);
                        fd = NULL;
                    }
                }

                if (0U != (MAP_GET_APPL_PARAM_FLAG
                    (
                        appl_param_rcv->appl_param_flag,
                        MAP_FLAG_STARTOFFSET
                    )))
                {
                    LOG_DEBUG ("List Start Offset: %d\n", appl_param_rcv->start_offset);
                }
            }

            /* Get the body of folder listing if required */
            if (NULL != fd)
            {
                if (remaining > map_xchg_size)
                {
                    body.length = map_xchg_size;
                    tx_response = MAP_CONTINUE_RSP;
                }
                else if (0U != remaining)
                {
                    tx_response = MAP_SUCCESS_RSP;
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
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_read (body.value, body.length, fd, &num_items);
                }
            }

            sent += body.length;
            remaining = fsize - sent;

            if (0U == remaining)
            {
                /* Reset the variables */
                sent = 0U;
                fsize = 0U;

                (BT_IGNORE_RETURN_VALUE) BT_fops_file_close (fd);
                fd = NULL;
            }
            break;

        case MAP_MSE_GET_MESSAGES_LISTING_IND:
            LOG_DEBUG ("Recvd MAP_MSE_GET_MESSAGES_LISTING_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

            if ((MAP_SUCCESS_RSP != event_result) &&
                (MAP_CONTINUE_RSP != event_result))
            {
                if (NULL != fd)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_close (fd);
                    fd = NULL;
                }

                sent = 0U;
                fsize = 0U;

                break;
            }

            send_response = 1U;
            appl_map_mse_xchg_size = map_xchg_size;

            response_info.body = &body;
            response_info.appl_params = &appl_param;
            map_resp_header.map_resp_info = &response_info;

            appl_param_rcv = map_headers->map_req_info->appl_params;

            /*
             * Headers are always in the first request/response.
             */
            if (0U == sent)
            {
                if((NULL == map_headers->map_req_info->name) ||
                   (NULL == map_headers->map_req_info->name->value))
                {
                    tx_response = MAP_BAD_REQ_RSP;
                    break;
                }
                else
                {
                    curr_folder[0U] = '\0';

                    if (0U == map_headers->map_req_info->name->length)
                    {
                        LOG_DEBUG ("From Current Folder\n");
                    }
                    else
                    {
                        /* Backup current path */
                        BT_str_copy (curr_folder, mas_instance[i].path);

                        retval = BT_vfops_set_path_forward
                                 (
                                     mas_instance[i].path,
                                     map_headers->map_req_info->name->value
                                 );
                        if (API_SUCCESS != retval)
                        {
                            LOG_DEBUG ("Failed to set path to folder %s - 0x%04X\n",
                            map_headers->map_req_info->name->value, retval);

                            tx_response = MAP_NOT_FOUND_RSP;
                            break;
                        }

                        LOG_DEBUG ("From Child folder %s\n",
                        map_headers->map_req_info->name->value);
                    }

                    tx_response = MAP_SUCCESS_RSP;

                    if (NULL != appl_param_rcv)
                    {
                        LOG_DEBUG("Application Parameters: 0x%08x, 0x%08x\n",
                        appl_param_rcv->appl_param_flag[0U], appl_param_rcv->appl_param_flag[1U]);

                        if ((0U == appl_param_rcv->appl_param_flag[0U]) &&
                            (0U == appl_param_rcv->appl_param_flag[1U]))
                        {
                            appl_param_rcv = NULL;
                        }
                    }

                    /* Create the listing file with path */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)MAP_ROOT_FOLDER_BASE,
                        (UCHAR *)MESSAGESLISTING_FILE,
                        file_object
                    );

                    /* Create the listing file */
                    retval = BT_map_create_xml_messages_listing_pl
                             (
                                 mas_instance[i].path,
                                 file_object,
                                 appl_param_rcv,
                                 &num_items
                             );
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to Create Message Listing file\n");
                        tx_response = MAP_SERVER_ERROR;
                        break;
                    }

                    /* Open the listing file */
                    retval = BT_fops_file_open
                             (
                                 file_object,
                                 (UCHAR *)"rb",
                                 &fd
                             );
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to Open File to Create folder Listing\n");
                        tx_response = MAP_SERVER_ERROR;
                        break;
                    }

                    if (NULL != fd)
                    {
                        /* Get the file size */
                        (BT_IGNORE_RETURN_VALUE) BT_fops_file_size (fd, &fsize);

                        remaining = fsize;

                        /* Set back the path to current */
                        if ('\0' != curr_folder[0U])
                        {
                            retval = BT_vfops_set_path_backward(mas_instance[i].path);
                            if (API_SUCCESS != retval)
                            {
                                LOG_DEBUG ("Failed to set path backward - 0x%04X\n",
                                retval);
                            }
                        }
                    }
                    else
                    {
                        /* Set back the path to current */
                        if ('\0' != curr_folder[0U])
                        {
                            retval = BT_vfops_set_path_backward(mas_instance[i].path);
                            if (API_SUCCESS != retval)
                            {
                                LOG_DEBUG ("Failed to set path backward - 0x%04X\n",
                                retval);
                            }
                        }

                        tx_response = MAP_NOT_FOUND_RSP;
                        break;
                    }
                }

                /* Set sample listing size */
                appl_param.messages_listing_size = num_items;
                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_MESSAGESLISTINGSIZE
                );

                /* Set new message indication */
                appl_param.new_message = 1U;
                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_NEWMESSAGE
                );

                /* Sample MSE Time */
                appl_param.mse_time.value = appl_mst_time;
                appl_param.mse_time.length = sizeof(appl_mst_time);
                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_MSETIME
                );

                appl_map_mse_xchg_size -= (2U + sizeof(appl_mst_time));

#ifdef MAP_1_3
                /* Sample DatabaseIdentifier */
                appl_param.database_identifier.value = appl_database_identifier;
                appl_param.database_identifier.length = sizeof(appl_database_identifier);
                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_DATABASE_IDENTIFIER
                );

                appl_map_mse_xchg_size -= (2U + sizeof(appl_database_identifier));

                /* Sample Folder Version Counter */
                appl_param.folder_ver_cntr.value = appl_folder_version_counter;
                appl_param.folder_ver_cntr.length = sizeof(appl_folder_version_counter);
                MAP_SET_APPL_PARAM_FLAG_EXT
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_FOLDER_VER_CNTR,
                    1U
                );
                appl_map_mse_xchg_size -= (2U + sizeof(appl_folder_version_counter));
#endif /* MAP_1_3 */
            }

            /*
             * TODO: For now, only Max List Count is checked. If '0',
             * then set sample listing size and NewMessage Indicator
             * Else, send sample listing body also
             */
            if (NULL != appl_param_rcv)
            {
                if ((MAP_GET_APPL_PARAM_FLAG
                     (
                         appl_param_rcv->appl_param_flag,
                         MAP_FLAG_MAXLISTCOUNT
                     )) &&
                    ((0U == appl_param_rcv->max_list_count)))
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_close(fd);
                    fd = NULL;
                }
            }

            /* Get the body of message listing if required */
            if (NULL != fd)
            {
                if (remaining > appl_map_mse_xchg_size)
                {
                    body.length = appl_map_mse_xchg_size;
                    tx_response = MAP_CONTINUE_RSP;
                }
                else if (0U != remaining)
                {
                    tx_response = MAP_SUCCESS_RSP;
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
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_read (body.value, body.length, fd, &num_items);
                }
            }

            sent += body.length;
            remaining = fsize - sent;

            if (0U == remaining)
            {
                /* Reset the variables */
                sent = 0U;
                fsize = 0U;

                (BT_IGNORE_RETURN_VALUE) BT_fops_file_close(fd);
                fd = NULL;
            }
            break;

#ifdef MAP_1_2
        case MAP_MSE_GET_MASINSTINFO_IND:
            LOG_DEBUG ("Recvd MAP_MSE_GET_MASINSTINFO_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            send_response = 1U;

            response_info.body = &body;
            map_resp_header.map_resp_info = &response_info;

            appl_param_rcv = map_headers->map_req_info->appl_params;

            if (NULL == appl_param_rcv)
            {
                LOG_DEBUG ("Application Parameter received NULL\n");
                tx_response = MAP_BAD_REQ_RSP;
                break;
            }
            else
            {
                if (0U == (MAP_GET_APPL_PARAM_FLAG
                     (
                         appl_param_rcv->appl_param_flag,
                         MAP_FLAG_MASINSTANCEID
                     )))
                {
                    LOG_DEBUG ("MASInstanceID Parameter Not found\n");
                    tx_response = MAP_BAD_REQ_RSP;
                    break;
                }

                LOG_DEBUG ("MAS Instance ID received: 0x%02X\n",
                appl_param_rcv->mas_instance_id);

                /**
                 * TODO: OwnerUCI
                 */

                /**
                 * MAS Instance Info. shall be NULL terminated UTF-8 encoded string of max 200 bytes.
                 */
                body.value = mas_instinfo;
                body.length = (UINT16)(BT_str_len(mas_instinfo) + 1U);
            }
            break;
#endif /* MAP_1_2 */

#ifdef MAP_1_3
        case MAP_MSE_GET_CONVERSATION_LIST_IND:
            LOG_DEBUG ("Recvd MAP_MSE_GET_CONVERSATION_LIST_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
            send_response = 1U;

            appl_map_mse_xchg_size = map_xchg_size;
            tx_response = MAP_SUCCESS_RSP;

            response_info.body = &body;
            response_info.appl_params = &appl_param;
            map_resp_header.map_resp_info = &response_info;

            appl_param_rcv = map_headers->map_req_info->appl_params;

            if (0U == sent)
            {
                if (appl_param_rcv == NULL)
                {
                    LOG_DEBUG ("Application Parameter received NULL\n");
                    tx_response = MAP_BAD_REQ_RSP;
                    break;
                }
                else
                {
                    if(0U != (MAP_GET_APPL_PARAM_FLAG
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_MAXLISTCOUNT
                            )))
                    {
                        LOG_DEBUG ("MaxListCount: 0x%04x\n", appl_param_rcv->max_list_count);
                    }

                    if(0U != (MAP_GET_APPL_PARAM_FLAG
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_STARTOFFSET
                            )))
                    {
                        LOG_DEBUG ("StartOffset: 0x%04x\n", appl_param_rcv->start_offset);
                    }

                    if(0U != (MAP_GET_APPL_PARAM_FLAG
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_FILTERPERIODBEGIN
                            )))
                    {
                        LOG_DEBUG ("PeriodBegin: %s\n", appl_param_rcv->filter_period_begin.value);
                    }

                    if(0U != (MAP_GET_APPL_PARAM_FLAG
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_FILTERPERIODEND
                            )))
                    {
                        LOG_DEBUG ("PeriodEnd: %s\n", appl_param_rcv->filter_period_end.value);
                    }

                    if(0U != (MAP_GET_APPL_PARAM_FLAG
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_FILTERREADSTATUS
                            )))
                    {
                        LOG_DEBUG ("Status: ");
                        if (0x01U == appl_param_rcv->filter_read_status)
                        {
                            LOG_DEBUG ("Unread\n");
                        }
                        else if (0x02U == appl_param_rcv->filter_read_status)
                        {
                            LOG_DEBUG ("Read\n");
                        }
                        else
                        {
                            /* MISRA C-2012 Rule 15.7 */
                        }
                    }

                    if(0U != (MAP_GET_APPL_PARAM_FLAG
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_FILTERRECIPIENT
                            )))
                    {
                        LOG_DEBUG ("Recipient: %s\n", appl_param_rcv->filter_recipient.value);
                    }

                    if(0U != (MAP_GET_APPL_PARAM_FLAG_EXT
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_CONVERSATION_ID,
                                1U
                            )))
                    {
                        LOG_DEBUG ("ConversationID: \n");
                        for (i = 0U; i < appl_param_rcv->conversation_id.length; i++)
                        {
                            LOG_DEBUG ("%02X", appl_param_rcv->conversation_id.value[i]);
                        }

                        LOG_DEBUG ("\n");
                    }

                    if(0U != (MAP_GET_APPL_PARAM_FLAG_EXT
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_CONV_PARAMETER_MASK,
                                1U
                            )))
                    {
                        LOG_DEBUG ("ConvParameterMask: 0x%08x\n", appl_param_rcv->conv_parameter_mask);

                        if (0U != (MAP_CONV_PARAM_MASK_CONV_NAME & appl_param_rcv->conv_parameter_mask))
                        {
                            LOG_DEBUG ("  - Conversation name\n");
                        }
                        if (0U != (MAP_CONV_PARAM_MASK_CONV_L_ACTVTY & appl_param_rcv->conv_parameter_mask))
                        {
                            LOG_DEBUG ("  - Conversation last_activity\n");
                        }
                        if (0U != (MAP_CONV_PARAM_MASK_CONV_RD_STS & appl_param_rcv->conv_parameter_mask))
                        {
                            LOG_DEBUG ("  - Conversation read_status\n");
                        }
                        if (0U != (MAP_CONV_PARAM_MASK_CONV_VER_CNTR & appl_param_rcv->conv_parameter_mask))
                        {
                            LOG_DEBUG ("  - Conversation version_counter\n");
                        }
                        if (0U != (MAP_CONV_PARAM_MASK_CONV_SUMMARY & appl_param_rcv->conv_parameter_mask))
                        {
                            LOG_DEBUG ("  - Conversattin Summary\n");
                        }
                        if (0U != (MAP_CONV_PARAM_MASK_PARTICIPANTS & appl_param_rcv->conv_parameter_mask))
                        {
                            LOG_DEBUG ("  - Participants\n");
                        }
                        if (0U != (MAP_CONV_PARAM_MASK_PTPNT_UCI & appl_param_rcv->conv_parameter_mask))
                        {
                            LOG_DEBUG ("  - Participant UCI\n");
                        }
                        if (0U != (MAP_CONV_PARAM_MASK_PTPNT_DPLY_NAME & appl_param_rcv->conv_parameter_mask))
                        {
                            LOG_DEBUG ("  - Participant display_name\n");
                        }
                        if (0U != (MAP_CONV_PARAM_MASK_PTPNT_CHT_STATE & appl_param_rcv->conv_parameter_mask))
                        {
                            LOG_DEBUG ("  - Participant chat_state\n");
                        }
                        if (0U != (MAP_CONV_PARAM_MASK_PTPNT_L_ACTVTY & appl_param_rcv->conv_parameter_mask))
                        {
                            LOG_DEBUG ("  - Participant last_activity\n");
                        }
                        if (0U != (MAP_CONV_PARAM_MASK_PTPNT_X_BT_UID & appl_param_rcv->conv_parameter_mask))
                        {
                            LOG_DEBUG ("  - Participant x_bt_uid\n");
                        }
                        if (0U != (MAP_CONV_PARAM_MASK_PTPNT_NAME & appl_param_rcv->conv_parameter_mask))
                        {
                            LOG_DEBUG ("  - Participant name\n");
                        }
                        if (0U != (MAP_CONV_PARAM_MASK_PTPNT_PRNCE_ALTY & appl_param_rcv->conv_parameter_mask))
                        {
                            LOG_DEBUG ("  - Participant presence_availability\n");
                        }
                        if (0U != (MAP_CONV_PARAM_MASK_PTPNT_PRNCE_TEXT & appl_param_rcv->conv_parameter_mask))
                        {
                            LOG_DEBUG ("  - Participant presence_text\n");
                        }
                        if (0U != (MAP_CONV_PARAM_MASK_PTPNT_PRNCE_PRTY & appl_param_rcv->conv_parameter_mask))
                        {
                            LOG_DEBUG ("  - Participant priority\n");
                        }
                    }
                }

                /* === Add the appl params for the rsp ===== */

                /* Sample Conversation Listing Version Counter */
                appl_param.conv_listing_ver_cntr.value = appl_conv_listing_version_counter;
                appl_param.conv_listing_ver_cntr.length = sizeof(appl_conv_listing_version_counter);
                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_CONV_LIST_VER_CNTR
                );

                appl_map_mse_xchg_size -= 18U; /* 2 + 16 */

                /* Sample DatabaseIdentifier */
                appl_param.database_identifier.value = appl_database_identifier;
                appl_param.database_identifier.length = sizeof(appl_database_identifier);
                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_DATABASE_IDENTIFIER
                );

                appl_map_mse_xchg_size -= 18U; /* 2 + 16 */

                /* Sample MSE Time */
                appl_param.mse_time.value = appl_mst_time;
                appl_param.mse_time.length = sizeof(appl_mst_time);
                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_MSETIME
                );

                appl_map_mse_xchg_size -= (2U + sizeof(appl_mst_time));

                appl_param.messages_listing_size = 4U;
                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_MESSAGESLISTINGSIZE
                );

                appl_map_mse_xchg_size -= 4U;

                if(0U != (MAP_GET_APPL_PARAM_FLAG
                   (
                       appl_param_rcv->appl_param_flag,
                       MAP_FLAG_MAXLISTCOUNT
                    ) &&
                    (0x00U == appl_param_rcv->max_list_count)))
                {
                    tx_response = MAP_SUCCESS_RSP;
                    break;
                }

                /* Create the listing file with path */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)MAP_ROOT_FOLDER_BASE,
                    (UCHAR *)CONVLIST_DATA_FILE,
                    file_object
                );

                /* Open the listing file */
                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"rb",
                             &fd
                         );
                if ((API_SUCCESS != retval) || (NULL == fd))
                {
                    LOG_DEBUG ("Failed to Open Message File\n");
                    tx_response = MAP_NOT_FOUND_RSP;
                    break;
                }

                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size (fd, &fsize);

                remaining = fsize;
            }

            /* Get the body data if required */
            if (fd != NULL)
            {
                if (remaining > appl_map_mse_xchg_size)
                {
                    body.length = appl_map_mse_xchg_size;
                    tx_response = MAP_CONTINUE_RSP;
                }
                else if (remaining != 0U)
                {
                    tx_response = MAP_SUCCESS_RSP;
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
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_read (body.value, body.length, fd, &num_items);
                }
            }

            sent += body.length;
            remaining = fsize - sent;

            if (0U == remaining)
            {
                /* Reset the variables */
                sent = 0U;
                fsize = 0U;

                (BT_IGNORE_RETURN_VALUE) BT_fops_file_close(fd);
                fd = NULL;
            }
            break;

        case MAP_MSE_SET_OWNER_STATUS_IND:
            LOG_DEBUG ("Recvd MAP_MSE_SET_OWNER_STATUS_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            send_response = 1U;

            tx_response = MAP_SUCCESS_RSP;

            appl_param_rcv = map_headers->map_req_info->appl_params;
            map_resp_header.map_resp_info = &response_info;

            if (NULL == appl_param_rcv)
            {
                LOG_DEBUG ("Not not received appl. hdr\n");
                tx_response = MAP_BAD_REQ_RSP;
                break;
            }
            else
            {
                appl_map_im_converse_index = 0x00U;
#if 0
                /* TODO: Search for the conversation id */
                for (i = 0; i < MAX_APPL_IM_CONVERSATIONS; i++)
                {
                    if (0x00 == BT_mem_cmp
                                (
                                    appl_map_im_conversations[appl_map_im_converse_index].conversation_id,
                                    appl_param_rcv->conversation_id.value,
                                    appl_param_rcv->conversation_id.length
                                ))
                    {
                        appl_map_im_converse_index = i;
                        break;
                    }
                }

                if (i == MAX_APPL_IM_CONVERSATIONS)
                {
                    LOG_DEBUG ("Invalid Conversation ID\n");
                    tx_response = MAP_NOT_FOUND_RSP;
                    break;
                }
#endif

                if(0U != (MAP_GET_APPL_PARAM_FLAG
                         (
                             appl_param_rcv->appl_param_flag,
                             MAP_FLAG_PRESENCE_AVAILABILITY
                         )))
                {
                    LOG_DEBUG ("Presence Avail: ");
                    if (MAP_IM_PRESENCE_OFFLINE == appl_param_rcv->presence_availability)
                    {
                        LOG_DEBUG ("Offline\n");
                    }
                    else if (MAP_IM_PRESENCE_ONLINE == appl_param_rcv->presence_availability)
                    {
                        LOG_DEBUG ("Online\n");
                    }
                    else if (MAP_IM_PRESENCE_AWAY == appl_param_rcv->presence_availability)
                    {
                        LOG_DEBUG ("Away\n");
                    }
                    else if (MAP_IM_PRESENCE_DO_NOT_DISTURB == appl_param_rcv->presence_availability)
                    {
                        LOG_DEBUG ("Do not distrub\n");
                    }
                    else if (MAP_IM_PRESENCE_BUSY == appl_param_rcv->presence_availability)
                    {
                        LOG_DEBUG ("Busy\n");
                    }
                    else if (MAP_IM_PRESENCE_IN_MEETING == appl_param_rcv->presence_availability)
                    {
                        LOG_DEBUG ("In a meeting\n");
                    }
                    else if (MAP_IM_PRESENCE_UNKNOWN == appl_param_rcv->presence_availability)
                    {
                        LOG_DEBUG ("Unknown\n");
                    }
                    else
                    {
                        LOG_DEBUG ("???\n");
                    }

                    appl_map_im_conversations[appl_map_im_converse_index].presence_availability
                        = appl_param_rcv->presence_availability;
                }

                if(0U != (MAP_GET_APPL_PARAM_FLAG
                        (
                            appl_param_rcv->appl_param_flag,
                            MAP_FLAG_PRESENCE_TEXT
                         )))
                {
                    LOG_DEBUG ("Presence Text: ");
                    for (i = 0U; i < appl_param_rcv->presence_text.length; i++)
                    {
                        LOG_DEBUG ("%c", appl_param_rcv->presence_text.value[i]);
                    }
                    LOG_DEBUG ("\n");

                    BT_mem_copy
                    (
                        appl_map_im_conversations[appl_map_im_converse_index].presence_text,
                        appl_param_rcv->presence_text.value,
                        appl_param_rcv->presence_text.length
                    );

                    appl_map_im_conversations[appl_map_im_converse_index].presence_text_size =
                        (UCHAR )appl_param_rcv->presence_text.length;
                }

                if(0U != (MAP_GET_APPL_PARAM_FLAG
                        (
                            appl_param_rcv->appl_param_flag,
                            MAP_FLAG_LAST_ACTIVITY
                         )))
                {
                    LOG_DEBUG ("Last Activity: ");
                    for (i = 0U; i < appl_param_rcv->last_activity.length; i++)
                    {
                        LOG_DEBUG ("%c", appl_param_rcv->last_activity.value[i]);
                    }
                    LOG_DEBUG ("\n");

                    BT_mem_copy
                    (
                        appl_map_im_conversations[appl_map_im_converse_index].last_activity,
                        appl_param_rcv->last_activity.value,
                        appl_param_rcv->last_activity.length
                    );

                    appl_map_im_conversations[appl_map_im_converse_index].last_activity_size =
                        (UCHAR )appl_param_rcv->last_activity.length;
                }

                if(0U != (MAP_GET_APPL_PARAM_FLAG_EXT
                        (
                            appl_param_rcv->appl_param_flag,
                            MAP_FLAG_CHAT_STATE,
                            1U
                         )))
                {
                    LOG_DEBUG ("Chat State: ");
                    if(MAP_IM_CHAT_STATE_INACTIVE == appl_param_rcv->chat_state)
                    {
                        LOG_DEBUG ("Inactive\n");
                    }
                    else if(MAP_IM_CHAT_STATE_ACTIVE == appl_param_rcv->chat_state)
                    {
                        LOG_DEBUG ("Active\n");
                    }
                    else if(MAP_IM_CHAT_STATE_COMPOSING == appl_param_rcv->chat_state)
                    {
                        LOG_DEBUG ("Composing\n");
                    }
                    else if(MAP_IM_CHAT_STATE_PAUSED_COMPOSING == appl_param_rcv->chat_state)
                    {
                        LOG_DEBUG ("Paused composing\n");
                    }
                    else if(MAP_IM_CHAT_STATE_GONE == appl_param_rcv->chat_state)
                    {
                        LOG_DEBUG ("Gone\n");
                    }
                    else if(MAP_IM_CHAT_STATE_UNKNOWN == appl_param_rcv->chat_state)
                    {
                        LOG_DEBUG ("Unknown\n");
                    }
                    else
                    {
                        LOG_DEBUG ("???\n");
                    }

                    appl_map_im_conversations[appl_map_im_converse_index].chat_state = appl_param_rcv->chat_state;
                }

                if(0U != (MAP_GET_APPL_PARAM_FLAG_EXT
                        (
                            appl_param_rcv->appl_param_flag,
                            MAP_FLAG_CONVERSATION_ID,
                            1U
                        )))
                {
                    LOG_DEBUG ("ConversationID: \n");
                    for (i = 0U; i < appl_param_rcv->conversation_id.length; i++)
                    {
                        LOG_DEBUG ("%c", appl_param_rcv->conversation_id.value[i]);
                    }
                    LOG_DEBUG ("\n");
                }
            }
            break;

        case MAP_MSE_GET_OWNER_STATUS_IND:
            LOG_DEBUG ("Recvd MAP_MSE_GET_OWNER_STATUS_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            send_response = 1U;

            tx_response = MAP_SUCCESS_RSP;

            MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

            response_info.appl_params = &appl_param;
            map_resp_header.map_resp_info = &response_info;

            appl_map_im_converse_index = 0x00U;
            /* TODO: Search for requeste conversation id */

            /* presence availability */
            MAP_SET_APPL_PARAM_FLAG
            (
                appl_param.appl_param_flag,
                MAP_FLAG_PRESENCE_AVAILABILITY
            );

            appl_param.presence_availability = appl_map_im_conversations[appl_map_im_converse_index].presence_availability;

            temp = appl_map_im_conversations[appl_map_im_converse_index].presence_text_size;

            if (0U != temp)
            {
                appl_param.presence_text.value = BT_alloc_mem(temp);
                if (NULL == appl_param.presence_text.value)
                {
                    LOG_DEBUG ("Memory allocation failed for Presence Text.\n");
                    break;
                }

                /* presence text */
                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_PRESENCE_TEXT
                );

                BT_mem_copy
                (
                    appl_param.presence_text.value,
                    appl_map_im_conversations[appl_map_im_converse_index].presence_text,
                    temp
                );

                appl_param.presence_text.length = temp;
            }

            temp = appl_map_im_conversations[appl_map_im_converse_index].last_activity_size;

            if (0U != temp)
            {
                appl_param.last_activity.value = BT_alloc_mem(temp);
                if (NULL == appl_param.last_activity.value)
                {
                    LOG_DEBUG ("Memory allocation failed for Last Activity\n");
                    break;
                }

                /* lastactivity */
                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_LAST_ACTIVITY
                );

                BT_mem_copy
                (
                    appl_param.last_activity.value,
                    appl_map_im_conversations[appl_map_im_converse_index].last_activity,
                    temp
                );

                appl_param.last_activity.length = temp;
            }

            /* chat state */
            MAP_SET_APPL_PARAM_FLAG_EXT
            (
                appl_param.appl_param_flag,
                MAP_FLAG_CHAT_STATE,
                1U
            );

            appl_param.chat_state = appl_map_im_conversations[appl_map_im_converse_index].chat_state;
            break;

        case MAP_MSE_SET_NOTIFICATION_FILTER_IND:
            LOG_DEBUG ("Recvd MAP_MSE_SET_NOTIFICATION_FILTER_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            send_response = 1U;
            map_resp_header.map_resp_info = &response_info;

            tx_response = MAP_SUCCESS_RSP;

            appl_param_rcv = map_headers->map_req_info->appl_params;

            if ((NULL == appl_param_rcv) ||
                (!(MAP_GET_APPL_PARAM_FLAG_EXT
                         (
                             appl_param_rcv->appl_param_flag,
                             MAP_FLAG_NOTIFICATION_FILTER_MASK,
                             1U
                         ))))
            {
                LOG_DEBUG ("NotificationFilterMask Appl. Header Not found\n");
                tx_response = MAP_BAD_REQ_RSP;
                break;
            }
            else
            {
                LOG_DEBUG ("NotificationFilterMask :0x%08x\n", appl_param_rcv->ntf_filter_mask);
                if (0U != (MAP_NTF_FLTR_MASK_NEW_MSG & appl_param_rcv->ntf_filter_mask))
                {
                    LOG_DEBUG ("  - NewMessage\n");
                }
                if (0U != (MAP_NTF_FLTR_MASK_MSG_DELETED & appl_param_rcv->ntf_filter_mask))
                {
                    LOG_DEBUG ("  - MessageDeleted\n");
                }
                if (0U != (MAP_NTF_FLTR_MASK_MSG_SHIFT & appl_param_rcv->ntf_filter_mask))
                {
                    LOG_DEBUG ("  - MessageShift\n");
                }
                if (0U != (MAP_NTF_FLTR_MASK_SENDING_SUCCESS & appl_param_rcv->ntf_filter_mask))
                {
                    LOG_DEBUG ("  - SendingSuccess\n");
                }
                if (0U != (MAP_NTF_FLTR_MASK_SENDING_FAILURE & appl_param_rcv->ntf_filter_mask))
                {
                    LOG_DEBUG ("  - SendingFailure\n");
                }
                if (0U != (MAP_NTF_FLTR_MASK_DELIVERY_SUCCESS & appl_param_rcv->ntf_filter_mask))
                {
                    LOG_DEBUG ("  - DeliverySuccess\n");
                }
                if (0U != (MAP_NTF_FLTR_MASK_DELIVERY_FAILURE & appl_param_rcv->ntf_filter_mask))
                {
                    LOG_DEBUG ("  - DeliveryFailure\n");
                }
                if (0U != (MAP_NTF_FLTR_MASK_MEMORY_FULL & appl_param_rcv->ntf_filter_mask))
                {
                    LOG_DEBUG ("  - MemoryFull\n");
                }
                if (0U != (MAP_NTF_FLTR_MASK_MEMORY_AVAILABLE & appl_param_rcv->ntf_filter_mask))
                {
                    LOG_DEBUG ("  - MemoryAvailable\n");
                }
                if (0U != (MAP_NTF_FLTR_MASK_READ_STS_CHNGD & appl_param_rcv->ntf_filter_mask))
                {
                    LOG_DEBUG ("  - ReadStatusChanged\n");
                }
                if (0U != (MAP_NTF_FLTR_MASK_CONV_CHNGD & appl_param_rcv->ntf_filter_mask))
                {
                    LOG_DEBUG ("  - ConversationChanged\n");
                }
                if (0U != (MAP_NTF_FLTR_MASK_PTPNT_PRNCE_CHNGD & appl_param_rcv->ntf_filter_mask))
                {
                    LOG_DEBUG ("  - ParticipantPresenceChanged\n");
                }
                if (0U != (MAP_NTF_FLTR_MASK_PTPNT_CHT_ST_CHNGD & appl_param_rcv->ntf_filter_mask))
                {
                    LOG_DEBUG ("  - ParticipantChatStateChanged\n");
                }
                if (0U != (MAP_NTF_FLTR_MASK_MSG_EXTD_DATA_CHNGD & appl_param_rcv->ntf_filter_mask))
                {
                    LOG_DEBUG ("  - MessageExtendedDataChanged\n");
                }
                if (0U != (MAP_NTF_FLTR_MASK_MSG_REMOVED & appl_param_rcv->ntf_filter_mask))
                {
                    LOG_DEBUG ("  - MessageRemoved\n");
                }
            }
            break;
#endif /* MAP_1_3 */

        case MAP_MSE_GET_MESSAGE_IND:
            LOG_DEBUG ("Recvd MAP_MSE_GET_MESSAGE_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

            if ((MAP_SUCCESS_RSP != event_result) &&
                (MAP_CONTINUE_RSP != event_result))
            {
                if (NULL != fd)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_close (fd);
                    fd = NULL;
                }

                sent = 0U;
                fsize = 0U;

                break;
            }

            send_response = 1U;

            response_info.body = &body;
            response_info.appl_params = &appl_param;

            appl_param_rcv = map_headers->map_req_info->appl_params;

            /*
             * TODO:
             * Headers can be present in first or/and subsequent responses also?
             */
            if (0U == sent)
            {
                if((NULL == map_headers->map_req_info->name) ||
                    (NULL == map_headers->map_req_info->name->value) ||
                    (0U == map_headers->map_req_info->name->length))
                {
                    LOG_DEBUG ("Invalid Message Handle\n");
                    tx_response = MAP_BAD_REQ_RSP;

                    break;
                }

                LOG_DEBUG ("Message Handle: %s\n",
                map_headers->map_req_info->name->value);

                tx_response = MAP_SUCCESS_RSP;

                /* Get the Message file name from the handle */
                retval = BT_map_get_message_file_pl
                         (
                             map_headers->map_req_info->name->value,
                             mas_instance[i].path,
                             file_object
                         );
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("Message file not found\n");
                    tx_response = MAP_NOT_FOUND_RSP;
                    break;
                }

                /* Open the listing file */
                retval = BT_fops_file_open
                         (
                             file_object,
                             (UCHAR *)"rb",
                             &fd
                         );
                if ((API_SUCCESS != retval) || (NULL == fd))
                {
                    LOG_DEBUG ("Failed to Open Message File\n");
                    tx_response = MAP_NOT_FOUND_RSP;
                    break;
                }

                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size (fd, &fsize);

                remaining = fsize;

                if (NULL == appl_param_rcv)
                {
                    LOG_DEBUG ("Application Parameter received NULL\n");
                    tx_response = MAP_BAD_REQ_RSP;
                    break;
                }
                else
                {
                    if (0U == (MAP_GET_APPL_PARAM_FLAG
                         (
                             appl_param_rcv->appl_param_flag,
                             MAP_FLAG_ATTACHMENT
                         )))
                    {
                        LOG_DEBUG ("Attachment Parameter Not found\n");
                        tx_response = MAP_BAD_REQ_RSP;
                        break;
                    }

                    LOG_DEBUG ("Attachment Value received: 0x%02X\n",
                    appl_param_rcv->attachment);

                    if (0U == (MAP_GET_APPL_PARAM_FLAG
                         (
                             appl_param_rcv->appl_param_flag,
                             MAP_FLAG_CHARSET
                         )))
                    {
                        LOG_DEBUG ("Charset Parameter Not found\n");
                        tx_response = MAP_BAD_REQ_RSP;
                        break;
                    }

                    LOG_DEBUG ("Charset Value received: 0x%02X\n",
                    appl_param_rcv->charset);
                }
            }

            /*
             * TODO: We now only decode the fraction request if sent by client.
             * We do not include fraction deliver in the response which is mandatory
             */
            if (0U != (MAP_GET_APPL_PARAM_FLAG
                     (
                         appl_param_rcv->appl_param_flag,
                         MAP_FLAG_FRACTIONREQUEST
                     )))
            {
                LOG_DEBUG ("Fraction Request Value: 0x%02X\n",
                appl_param_rcv->fraction_request);
            }

            /* Get the body of message listing if required */
            if (NULL != fd)
            {
                if (remaining > map_xchg_size)
                {
                    body.length = map_xchg_size;
                    tx_response = MAP_CONTINUE_RSP;

                    /*
                     * TODO: Currently Fraction handling based on Continue/Success
                     * response. Should be done based on actual bMessage fraction
                     */
                    if (0U != (MAP_GET_APPL_PARAM_FLAG
                             (
                                 appl_param_rcv->appl_param_flag,
                                 MAP_FLAG_FRACTIONREQUEST
                             )))
                    {
                        appl_param.fraction_deliver = 0x00U;
                        MAP_SET_APPL_PARAM_FLAG
                        (
                            appl_param.appl_param_flag,
                            MAP_FLAG_FRACTIONDELIVER
                        );
                    }
                }
                else if (0U != remaining)
                {
                    tx_response = MAP_SUCCESS_RSP;
                    body.length = (UINT16 )remaining;

                    /*
                     * TODO: Currently Fraction handling based on Continue/Success
                     * response. Should be done based on actual bMessage fraction
                     */
                    if (0U != (MAP_GET_APPL_PARAM_FLAG
                             (
                                 appl_param_rcv->appl_param_flag,
                                 MAP_FLAG_FRACTIONREQUEST
                             )))
                    {
                        appl_param.fraction_deliver = 0x01U;
                        MAP_SET_APPL_PARAM_FLAG
                        (
                            appl_param.appl_param_flag,
                            MAP_FLAG_FRACTIONDELIVER
                        );
                    }
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
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_read (body.value, body.length, fd, &num_items);
                }
            }

            sent += body.length;
            remaining = fsize - sent;

            if (0U == remaining)
            {
                /* Reset the variables */
                sent = 0U;
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_close (fd);
                fd = NULL;
            }

            map_resp_header.map_resp_info = &response_info;

            send_response = 1U;
            break;

        case MAP_MSE_SET_MESSAGE_STATUS_IND:
            LOG_DEBUG ("Recvd MAP_MSE_SET_MESSAGE_STATUS_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            if((NULL == map_headers->map_req_info->name) ||
               (NULL == map_headers->map_req_info->name->value) ||
               (0U == map_headers->map_req_info->name->length))
            {
                LOG_DEBUG ("Message Handle Not Found\n");
                tx_response = MAP_BAD_REQ_RSP;

                response_info.body = NULL;
                response_info.appl_params = NULL;
            }
            else
            {
                LOG_DEBUG ("Message Handle: %s\n",
                map_headers->map_req_info->name->value);

                /* Get the Application parameter */
                appl_param_rcv =
                map_headers->map_req_info->appl_params;

                if (!(MAP_GET_APPL_PARAM_FLAG
                      (
                          appl_param_rcv->appl_param_flag,
                          MAP_FLAG_STATUSINDICATOR
                      )) ||
                    !(MAP_GET_APPL_PARAM_FLAG
                      (
                          appl_param_rcv->appl_param_flag,
                          MAP_FLAG_STATUSVALUE
                      )))
                {
#ifdef MAP_1_3
                    if(0U != (MAP_GET_APPL_PARAM_FLAG_EXT
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_EXTENDED_DATA,
                                1U
                             )))
                    {
                        LOG_DEBUG ("Extended Data: \n");
                        LOG_DEBUG ("%s\n", appl_param_rcv->extended_data.value);

                        tx_response = MAP_SUCCESS_RSP;
                    }
                    else
#endif /* MAP_1_3 */
                    {
                        LOG_DEBUG ("Mandatory Appl Params not found\n");
                        tx_response = MAP_NOT_ACCEPTABLE_RSP;
                    }
                }
                else
                {
                    UINT16 msg_status;

                    LOG_DEBUG ("Application Parameter Values: \n");
                    LOG_DEBUG ("Status Indicator - 0x%02X\n",
                    appl_param_rcv->status_indicator);
                    LOG_DEBUG ("Status Value - 0x%02X\n",
                    appl_param_rcv->status_value);

                    msg_status = appl_param_rcv->status_indicator;
                    msg_status <<= 8U;
                    msg_status |= appl_param_rcv->status_value;

                    (BT_IGNORE_RETURN_VALUE) BT_map_set_message_status_pl
                    (
                        mas_instance[i].path,
                        (CHAR *)map_headers->map_req_info->name->value,
                        msg_status
                    );

                    tx_response = MAP_SUCCESS_RSP;
                }

                response_info.body = NULL;
                response_info.appl_params = &appl_param;
            }

            map_resp_header.map_resp_info = &response_info;

            send_response = 1U;
            break;

        case MAP_MSE_PUSH_MESSAGE_IND:
            LOG_DEBUG ("Recvd MAP_MSE_PUSH_MESSAGE_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            send_response = 1U;

            MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

            map_resp_header.map_resp_info = &response_info;

            /* Get the Application parameter */
            appl_param_rcv = map_headers->map_req_info->appl_params;

            response_info.name = NULL;
            response_info.body = NULL;
            response_info.appl_params = NULL;

            if ((MAP_SUCCESS_RSP != event_result) &&
                (MAP_CONTINUE_RSP != event_result))
            {
                if (NULL != fd)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_close (fd);
                    fd = NULL;
                }

                push_started = 0U;

                break;
            }

            /* TODO: Verify this check here */
            if (0U == push_started)
            {
                if((NULL == map_headers->map_req_info->name) ||
                    (NULL == map_headers->map_req_info->name->value))
                {
                    tx_response = MAP_BAD_REQ_RSP;

                    break;
                }
                else
                {
                    /* Check for transparent appl. param */
                    if(0U != (MAP_GET_APPL_PARAM_FLAG
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_TRANSPARENT
                             )))
                    {
                        LOG_DEBUG ("Transparent Param Val: \n");
                        if (MAP_TRANSPARENT_OFF == appl_param_rcv->transparent)
                        {
                            LOG_DEBUG ("    - OFF");
                        }
                        else if (MAP_TRANSPARENT_ON == appl_param_rcv->transparent)
                        {
                            LOG_DEBUG ("    - ON");
                        }
                        else
                        {
                            LOG_DEBUG ("    - ???");
                        }
                        LOG_DEBUG ("\n");
                    }

                    /* Check for retry appl. param */
                    if(0U != (MAP_GET_APPL_PARAM_FLAG
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_RETRY
                             )))
                    {
                        LOG_DEBUG ("Retry Param Val: \n");
                        if (MAP_RETRY_OFF == appl_param_rcv->retry)
                        {
                            LOG_DEBUG ("    - OFF");
                        }
                        else if (MAP_RETRY_ON == appl_param_rcv->retry)
                        {
                            LOG_DEBUG ("    - ON");
                        }
                        else
                        {
                            LOG_DEBUG ("    - ???");
                        }
                        LOG_DEBUG ("\n");
                    }

                    /* Check for charset appl. param */
                    if(0U != (MAP_GET_APPL_PARAM_FLAG
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_CHARSET
                             )))
                    {
                        LOG_DEBUG ("Charset Param Val: \n");
                        if (MAP_CHARSET_NATIVE == appl_param_rcv->charset)
                        {
                            LOG_DEBUG ("    - NATIVE");
                        }
                        else if (MAP_CHARSET_UTF8 == appl_param_rcv->charset)
                        {
                            LOG_DEBUG ("    - UTF8");
                        }
                        else
                        {
                            LOG_DEBUG ("    - ???");
                        }
                        LOG_DEBUG ("\n");
                    }

#ifdef MAP_1_3
                    /* Check for Conversation ID appl. param */
                    if(0U != (MAP_GET_APPL_PARAM_FLAG_EXT
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_CONVERSATION_ID,
                                1U
                             )))
                    {
                        LOG_DEBUG ("Coversation ID Param Val:\n");
                        LOG_DEBUG ("    - 0x");
                        for (i = 0U; i < appl_param_rcv->conversation_id.length; i++)
                        {
                            LOG_DEBUG ("%c", appl_param_rcv->conversation_id.value[i]);
                        }
                        LOG_DEBUG ("\n");

#ifdef MAP_1_4_2
                        /* Form the Converstion Message Object file with conversation_id */
                        conversation_id_flag = 0x01U;
                        BT_mem_copy
                        (
                            conversation_obj_file,
                            appl_param_rcv->conversation_id.value,
                            appl_param_rcv->conversation_id.length
                        );
                        conversation_obj_file[appl_param_rcv->conversation_id.length] = '\0';
                        BT_str_n_cat(conversation_obj_file, "_cnvsn_msg.txt", (sizeof(conversation_obj_file) - BT_str_len(conversation_obj_file)));
#endif /* MAP_1_4_2 */
                    }
#endif /* MAP_1_3 */

#ifdef MAP_1_4
                    /* Check for Message Handle appl. param */
                    if(0U != (MAP_GET_APPL_PARAM_FLAG_EXT
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_MESSAGE_HANDLE,
                                1U
                             )))
                    {
                        LOG_DEBUG ("Message Handle Param Val:\n");
                        LOG_DEBUG ("    - 0x");
                        for (i = 0U; i < appl_param_rcv->msg_handle.length; i++)
                        {
                            LOG_DEBUG ("%c", appl_param_rcv->msg_handle.value[i]);
                        }
                        LOG_DEBUG ("\n");
                    }

                    /* Check for Attachment appl. param */
                    if(0U != (MAP_GET_APPL_PARAM_FLAG
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_ATTACHMENT
                             )))
                    {
                        LOG_DEBUG ("Attachment Param Val: \n");
                        if (MAP_ATTACHMENT_OFF == appl_param_rcv->attachment)
                        {
                            LOG_DEBUG ("    - OFF");
                        }
                        else if (MAP_ATTACHMENT_ON == appl_param_rcv->attachment)
                        {
                            LOG_DEBUG ("    - ON");
                        }
                        else
                        {
                            LOG_DEBUG ("    - ???");
                        }
                        LOG_DEBUG ("\n");
                    }

                    /* Check for ModifyText appl. param */
                    if(0U != (MAP_GET_APPL_PARAM_FLAG_EXT
                            (
                                appl_param_rcv->appl_param_flag,
                                MAP_FLAG_MODIFY_TEXT,
                                1U
                             )))
                    {
                        LOG_DEBUG ("ModifyText Param Val: \n");
                        if (MAP_FRWD_MODIFY_TEXT_REPLACE == appl_param_rcv->modify_text)
                        {
                            LOG_DEBUG ("    - REPLACE");
                        }
                        else if (MAP_FRWD_MODIFY_TEXT_PREPEND == appl_param_rcv->modify_text)
                        {
                            LOG_DEBUG ("    - PREPEND");
                        }
                        else
                        {
                            LOG_DEBUG ("    - ???");
                        }
                        LOG_DEBUG ("\n\n");
                    }
#endif /* MAP_1_4 */

#ifndef MAP_1_4_2
                    if (0U == map_headers->map_req_info->name->length)
#else /* MAP_1_4_2 */
                    /* Ignore Name Header field if Conversation Id is set */
                    if ((0x01 == conversation_id_flag) ||
                        (0 == map_headers->map_req_info->name->length))
#endif /* MAP_1_4_2 */
                    {
                        /* Backup current path */
                        BT_str_copy (curr_folder, mas_instance[i].path);
                        LOG_DEBUG ("To Current Folder\n");
                    }

                    /* TODO: Not required if Conv List ID present */
                    if (0U != map_headers->map_req_info->name->length)
                    {
                        LOG_DEBUG ("To Child folder %s\n",
                        map_headers->map_req_info->name->value);

                        /* Backup current path */
                        BT_str_copy (curr_folder, mas_instance[i].path);

                        /* Set path to child */
                        retval = BT_vfops_set_path_forward(mas_instance[i].path, map_headers->map_req_info->name->value);
                    }

                    /* If the folder being pushed is outbox, simulate delivery by pushing to sent */
                    if (NULL != BT_str_str (mas_instance[i].path, "outbox"))
                    {
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void) BT_vfops_set_path_backward(mas_instance[i].path);
                        retval = BT_vfops_set_path_forward(mas_instance[i].path, (UCHAR *)"sent");
                    }

                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG("Failed to set to folder\n");
                        tx_response = MAP_SERVER_ERROR;
                        break;
                    }

                    /* Get the message listing in the folder */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)MAP_ROOT_FOLDER_BASE,
                        (UCHAR *)MESSAGESLISTING_FILE,
                        file_object
                    );

                    /* Create the listing file */
                    retval = BT_map_create_xml_messages_listing_pl
                             (
                                 mas_instance[i].path,
                                 file_object,
                                 NULL,
                                 &num_items
                             );
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to Create Message Listing file\n");
                        tx_response = MAP_SERVER_ERROR;
                        break;
                    }

                    /* Create message file */
                    (BT_IGNORE_RETURN_VALUE) BT_map_push_message_pl (mas_instance[i].path, (num_items + 1U), message_file);

                    (void)BT_vfops_create_object_name
                    (
                        mas_instance[i].path,
                        message_file,
                        file_object
                    );

                    /* Create the listing file */
                    retval = BT_fops_file_open
                             (
                                 file_object,
                                 (UCHAR *)"wb",
                                 &fd
                             );
                    if (API_SUCCESS != retval)
                    {
                        LOG_DEBUG ("Failed to Open File to Push Message\n");
                        tx_response = MAP_SERVER_ERROR;
                        break;
                    }

                    /* Restore current path */
                    BT_str_copy (mas_instance[i].path, curr_folder);

                    response_info.body = NULL;
                    response_info.appl_params = &appl_param;
                }

                push_started = 1U;
            }

            length = map_headers->map_req_info->body->length;
            data = map_headers->map_req_info->body->value;

            (BT_IGNORE_RETURN_VALUE) BT_fops_file_write (data, length, fd, &num_items);

#if 0
                LOG_DEBUG ("\n\n================= BODY PACKET =================\n");

                /* Dump the Body stream */
                for (i = 0; i < length; i++)
                {
                    LOG_DEBUG ("%02X ", data[i]);
                }

                LOG_DEBUG ("\n================= BODY PACKET =================\n\n");
#else
                LOG_DEBUG ("\n\n========== MCE Message Received ==========\n\n");

                /* Print the stream */
                for (i = 0U; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }

                LOG_DEBUG ("\n\n================================================\n\n");

#ifdef HAVE_OBJECT_PARSER
                /* Open file to dump raw message */
                if (NULL == map_raw_fp)
                {
                    map_recv_count++;
                    BT_str_print(map_file_name, "map_objects\\map_raw_%d.msg", map_recv_count);

                    retval = BT_fops_file_open
                             (
                                 map_file_name,
                                 (UCHAR *)"wb",
                                 &map_raw_fp
                             );
                    if ((API_SUCCESS != retval) || (NULL == map_raw_fp))
                    {
                        LOG_DEBUG ("Failed to open Message file\n");
                    }
                }

                /* Dump the raw message */
                if (NULL != map_raw_fp)
                {
                    BT_fops_file_write(data, length, map_raw_fp, &actual);
                }

                /* Open file to dump parsed message */
                if (NULL == map_parsed_fp)
                {
                    BT_str_print(map_file_name, "map_objects\\map_parsed_%d.msg", map_recv_count);

                    retval = BT_fops_file_open
                             (
                                 map_file_name,
                                 (UCHAR *)"wb",
                                 &map_parsed_fp
                             );
                    if ((API_SUCCESS != retval) || (NULL == map_raw_fp))
                    {
                        LOG_DEBUG ("Failed to open Message file to parse\n");
                    }

                    map_parser_diff_len = 0U;
                }

                /* Get the message parsed */
                BT_mem_copy
                (
                    (&map_parse_buffer[map_parser_diff_len]),
                    data,
                    length
                );

                /* Set the context data and call the parser */
                map_context.buffer = map_parse_buffer;
                map_context.buffer_len = length + map_parser_diff_len;

                retval = object_parser_parse (&map_context);

                BT_mem_move
                (
                    map_parse_buffer,
                    &map_parse_buffer[map_context.total_parsed_length],
                    (map_context.buffer_len - map_context.total_parsed_length)
                );

                map_parser_diff_len = map_context.buffer_len - map_context.total_parsed_length;

                /* Close the file if message received fully */
                if (MAP_SUCCESS_RSP == event_result)
                {
                    BT_fops_file_close (map_parsed_fp);
                    map_parsed_fp = NULL;

                    BT_fops_file_close (map_raw_fp);
                    map_raw_fp = NULL;

                    LOG_DEBUG ("\n=================== Received Message Object ==================\n\n");
                    LOG_DEBUG ("VERSION: %s\n", map_msg_object.property.version);
                    LOG_DEBUG ("STATUS: %s\n",
                        (READ == map_msg_object.property.status)? "READ": "UNREAD");
                    LOG_DEBUG ("TYPE: %x\n", map_msg_object.property.type);
                    LOG_DEBUG ("FOLDER: %s\n", map_msg_object.property.folder);

                    LOG_DEBUG ("ENCODING: %x\n", map_msg_object.envelope.content.property.encoding);
                    LOG_DEBUG ("CHARSET: %x\n", map_msg_object.envelope.content.property.charset);
                    LOG_DEBUG ("LANGUAGE: %s\n", map_msg_object.envelope.content.property.language);
                    LOG_DEBUG ("CONTENT LENGTH: %d\n", map_msg_object.envelope.content.property.content_length);

                    LOG_DEBUG ("ORIGINATOR: %s\n",
                    ('\0' == map_msg_object.originator.property->param[0U].param_value[0U])? "UNKNOWN":
                    map_msg_object.originator.property->param[0U].param_value);

                    for (i = 0U; i < map_msg_object.num_envelopes; i++)
                    {
                        LOG_DEBUG ("RECEPIENT %d: %s\n", i,
                        ('\0' == map_msg_object.envelope.recepient[i].property[0U].param[0U].param_value[0U])? "UNKNOWN":
                        map_msg_object.envelope.recepient[i].
                        property[0U].param[0U].param_value);
                    }

                    if (NULL != map_msg_object.envelope.content.message)
                    {
                        LOG_DEBUG ("MESSAGE:\n%s\n\n", map_msg_object.envelope.content.message);
                        BT_free_mem(map_msg_object.envelope.content.message);
                        map_msg_object.envelope.content.message = NULL;

                    }
                    LOG_DEBUG ("\n=================== Received Message Object ==================\n");

                    map_msg_object.num_envelopes = 0U;

                    LOG_DEBUG ("Reinitializing MAP Parser...\n");
                    retval = object_parser_init (&map_context, map_bmsg_parser_cb);
                    LOG_DEBUG ("Retval - 0x%04X\n", retval);
                }
#endif /* HAVE_OBJECT_PARSER */
#endif /* 0 */

            /* Send the Message Handle */
            if (MAP_SUCCESS_RSP == event_result)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_close (fd);
                fd = NULL;

                push_started = 0U;
                response_info.name = &name;

                /* Put the message handle */
                message_file[17U] = '\0';
                name.value = &message_file[1U];
                name.length = 17U;
            }
            break;

        case MAP_MSE_SET_NTF_REGISTRATION_IND:
            LOG_DEBUG ("Recvd MAP_MSE_SET_NTF_REGISTRATION_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            send_response = 1U;

            map_resp_header.map_resp_info = &response_info;

#ifdef MAP_SUPPORT_NOTIFICATION
            if ((MAP_SUCCESS_RSP == event_result) ||
                (MAP_CONTINUE_RSP == event_result))
            {
                if (NULL != map_headers->map_req_info->appl_params)
                {
                    ntf_status =
                    map_headers->map_req_info->appl_params->notification_status;

                    /* Check for the Notification status */
                    if (0U != (MAP_GET_APPL_PARAM_FLAG
                             (
                                 map_headers->map_req_info->appl_params->appl_param_flag,
                                 MAP_FLAG_NOTIFICATIONSTATUS
                             )))
                    {
                        if (0x01U == ntf_status)
                        {
                            LOG_DEBUG ("Notification Enabled\n");
                        }
                        else if (0x00U == ntf_status)
                        {
                            LOG_DEBUG ("Notification Disabled\n");
                        }
                        else
                        {
                            LOG_DEBUG ("Unsupported Notification Status\n");
                            tx_response = MAP_BAD_REQ_RSP;
                        }
                    }
                }
            }
#else /* MAP_SUPPORT_NOTIFICATION */
            tx_response = MAP_NOT_IMPLEMENTED_RSP;
#endif /* MAP_SUPPORT_NOTIFICATION */
            break;

#ifdef MAP_SUPPORT_NOTIFICATION
        case MAP_MSE_NS_CONNECT_CNF:
            LOG_DEBUG ("Recvd MAP_MSE_MNS_CONNECT_CNF - %04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            if (NULL != map_headers->map_connect_info)
            {
                LOG_DEBUG("Peer " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
                BT_DEVICE_ADDR_ONLY_PRINT_STR (map_headers->map_connect_info->bd_addr));

                LOG_DEBUG ("MAX Exchange Size: %d\n",
                map_headers->map_connect_info->max_recv_size);
                map_ns_xchg_size = map_headers->map_connect_info->max_recv_size;
            }

            /* Check for valid event result and parameters */
            if((MAP_SUCCESS_RSP != event_result) ||
               (NULL == map_headers->map_connect_info))
            {
                tx_response = MAP_NOT_ACCEPTABLE_RSP;
                send_response = 1U;

                break;
            }

            /* Get the Handle parameters */
            for (i = 0U; i < MAP_SERVER_NUM_INSTANCES; i++)
            {
                if (mns_instance[i].handle == handle)
                {
                    BT_COPY_BD_ADDR(mns_instance[i].bd_addr,
                    map_headers->map_connect_info->bd_addr)

                    break;
                }
            }

            if (i != MAP_SERVER_NUM_INSTANCES)
            {
                for (j = 0U; j < MAP_SERVER_NUM_INSTANCES; j++)
                {
                    if (0 == BT_mem_cmp(mas_instance[j].instance.bd_addr,
                         map_headers->map_connect_info->bd_addr, BT_BD_ADDR_SIZE))
                    {
                        mas_instance[j].mns_id = (UCHAR)i;
                    }
                }
            }

            map_mse_print_appl_instances();
            break;

        case MAP_MSE_NS_DISCONNECT_CNF:
            LOG_DEBUG ("Recvd MAP_MSE_MNS_DISCONNECT_CNF - %04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);
            break;

        case MAP_MSE_NS_TRANSPORT_CLOSE_CNF:
            LOG_DEBUG ("Recvd MAP_MSE_MNS_TRANSPORT_CLOSE_CNF: - %04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            for (i = 0U; i < MAP_SERVER_NUM_INSTANCES; i++)
            {
                if (mns_instance[i].handle == handle)
                {
                    BT_mem_set(mns_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);
                    break;
                }
            }

            for (j = 0U; j < MAP_SERVER_NUM_INSTANCES; j++)
            {
                if (mas_instance[j].mns_id == i)
                {
                    mas_instance[j].mns_id = MAP_ENTITY_INVALID;
                }
            }

            map_mse_print_appl_instances();
            break;

        case MAP_MSE_NS_TRANSPORT_CLOSE_IND:
            LOG_DEBUG ("Recvd MAP_MSE_MNS_TRANSPORT_CLOSE_IND: - %04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            for (i = 0U; i < MAP_SERVER_NUM_INSTANCES; i++)
            {
                if (mns_instance[i].handle == handle)
                {
                    BT_mem_set(mns_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);
                    break;
                }
            }

            for (j = 0U; j < MAP_SERVER_NUM_INSTANCES; j++)
            {
                if (mas_instance[j].mns_id == i)
                {
                    mas_instance[j].mns_id = MAP_ENTITY_INVALID;
                }
            }

            map_mse_print_appl_instances();
            break;

        case MAP_MSE_NS_SEND_EVENT_CNF:
            LOG_DEBUG ("Recvd MAP_MSE_MNS_SEND_EVENT_CNF - %04X\n", event_result);
            LOG_DEBUG ("MSE Instance Handle - 0x%02X\n", handle);

            if ((MAP_SUCCESS_RSP == event_result) ||
                (MAP_CONTINUE_RSP != event_result))
            {
                break;
            }

            send_request = 1U;

            request_info.body = &body;
            map_headers->map_req_info = &request_info;

            /* Fill body hdr values */
            if (remaining > map_ns_xchg_size)
            {
                body.length = map_ns_xchg_size;
                more = 0x01U;
            }
            else if (0U != remaining)
            {
                body.length = (UINT16 )remaining;
                more = 0x00U;
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
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read (body.value, body.length, fd, (UINT16 *)&actual);
            }
            break;
#endif /* MAP_SUPPORT_NOTIFICATION */

        default:
            LOG_DEBUG("Invalid Event Type: 0x%02X\n", event_type);
            break;
        }

        /* Send response if required */
        if (0U != send_response)
        {
            if (0U < wait_count)
            {
                response_info.wait = BT_TRUE;
                map_resp_header.map_resp_info = &response_info;

                wait_count--;
            }

            retval = BT_map_mse_send_response
                     (
                         &handle,
                         event_type,
                         tx_response,
                         &map_resp_header
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed to send MSE Response - 0x%05X\n", retval);
            }

            if ((NULL != body.value)
#ifdef MAP_1_2
                &&
                (MAP_MSE_GET_MASINSTINFO_IND != event_type)
#endif /* MAP_1_2 */
                )
            {
                BT_free_mem (body.value);
                body.value = NULL;

                /* MISRA C - 2012 Rule 2.2 */
                /* body.length = 0U; */
            }
        }
#ifdef  MAP_SUPPORT_NOTIFICATION
        else if (0U != send_request)
        {
            retval = BT_map_mse_ns_send_request
                     (
                         &handle,
                         event_type,
                         tx_response,
                         map_headers,
                         more,
                         &actual
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed to send MSE Request - 0x%05X\n", retval);
            }

            sent_len += actual;
            remaining = fsize_event - sent_len;

            /* If operation has failed or completed, perform cleanup */
            if ((API_SUCCESS != retval) || (0U == remaining))
            {
                sent_len = 0U;
                fsize_event = 0U;

                (BT_IGNORE_RETURN_VALUE) BT_fops_file_close (fd);
                fd = NULL;
            }

            if (NULL != body.value)
            {
                BT_free_mem (body.value);
                body.value = NULL;

                /* MISRA C - 2012 Rule 2.2 */
                /* body.length = 0U; */
            }
        }
#endif /* MAP_SUPPORT_NOTIFICATION */
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

#if ((defined MAP_SUPPORT_NOTIFICATION) && (defined APPL_MAP_AUTO_NTF_ACTION))
        if (0x01U == ntf_status)
        {
            map_app_instance = 0U;

            for (i = 0U; i < MAP_SERVER_NUM_INSTANCES; i++)
            {
                if (mas_instance[i].instance.handle == handle)
                {
                    BT_COPY_BD_ADDR(mns_instance[map_app_instance].bd_addr, mas_instance[i].instance.bd_addr);
                    break;
                }
            }

            mns_instance[map_app_instance].map_sdp_record.map_len_attrib_data
                                                  = MAP_SDP_RECORD_DATA_SIZE;

            SDP_SET_HANDLE
            (
                mns_instance[map_app_instance].map_sdp_record.map_sdp_handle,
                mns_instance[map_app_instance].bd_addr,
                appl_map_mse_mns_sdp_callback
            );

            retval = BT_sdp_open(
                &mns_instance[map_app_instance].map_sdp_record.map_sdp_handle
                );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("SDP Connect FAILED !! Error Code = 0x%04X\n", retval);
            }
            else
            {
                LOG_DEBUG("Please Wait for SDP Operation to Complete\n");
            }
        }
        else if (0x00U == ntf_status)
        {
            LOG_DEBUG ("Disconnecting...\n");
            retval = BT_map_mse_ns_disconnect
                     (
                         &mns_instance[map_app_instance].handle
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
#endif /* ((defined MAP_SUPPORT_NOTIFICATION) && (defined APPL_MAP_AUTO_NTF_ACTION)) */
    }

    return API_SUCCESS;
}


void appl_map_mse_mns_sdp_callback
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

#if ((defined MAP_SUPPORT_NOTIFICATION) && (defined APPL_MAP_AUTO_NTF_ACTION))
    MAP_CONNECT_STRUCT connect_info;
#endif /* ((defined MAP_SUPPORT_NOTIFICATION) && (defined APPL_MAP_AUTO_NTF_ACTION)) */

#ifndef OBEX_OVER_L2CAP
    UINT16 attrib_id[1U] = { PROTOCOL_DESC_LIST };
    num_attrib_ids = 1U;
#else /* OBEX_OVER_L2CAP */
    UINT16 attrib_id[3U] = {PROTOCOL_DESC_LIST,
                           GOEP_L2CAP_PSM_ATTR_ID,
                           MAP_SUPPORTED_FEATURES_ATTR_ID};

    num_attrib_ids = 3U;
#endif /* OBEX_OVER_L2CAP */

    BT_IGNORE_UNUSED_PARAM(length);

    /*
     * Frame the SDP Search pattern with following UUIDs
     * - MAP_MAS_UUID
     * - RFCOMM_UUID
     * - L2CAP_UUID
     * and only one Attribute ID "PROTOCOL_DESC_LIST"
     * (For getting the RFCOMM Server Channel on which Peer MAP Service
     *  is Listening)
     *
     */
    uuid[0U].uuid_type = UUID_16;
    uuid[1U].uuid_type = UUID_16;
    uuid[2U].uuid_type = UUID_16;
    uuid[0U].uuid_union.uuid_16= MAP_MNS_UUID;
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
                     &mns_instance[map_app_instance].map_sdp_record.map_sdp_handle,
                     uuid,
                     3U,
                     attrib_id /* Protocol Desc*/,
                     num_attrib_ids,
                     NULL,
                     0U,
                     mns_instance[map_app_instance].map_sdp_record.map_attrib_data,
                     &mns_instance[map_app_instance].map_sdp_record.map_len_attrib_data
                 );

        if(API_SUCCESS != retval)
        {
            LOG_DEBUG("SDP SEARCH Failed for MAP..Closing SDP\n");
            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&mns_instance[map_app_instance].map_sdp_record.map_sdp_handle);
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

#ifdef MAP_1_2
            UINT32 attr_val_4B;
            UINT16 attr_size;
#endif /* MAP_1_2 */

#ifdef OBEX_OVER_L2CAP
            UINT16 attr_val_2B;
            UINT16 psm;
#endif /* OBEX_OVER_L2CAP */

#if ((defined MAP_1_2) || (defined OBEX_OVER_L2CAP))
            UCHAR attr_val[4U];
#endif /* MAP_1_2 || OBEX_OVER_L2CAP */

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

                /* Check for OBEX L2CAP PSM for MAP in the Protocol Descriptor List */
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
                    attr_val_2B = 0U;
                    attr_size = 2U;
                    /* Get the GOEP L2CAP PSM */
                    retval = BT_sdp_get_attr_value
                             (
                                 GOEP_L2CAP_PSM_ATTR_ID,
                                 sdp_query_result,
                                 attr_val,
                                 &attr_size
                             );

                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG("SDP FAILED to find GOEP L2CAP PSM\n");
                    }
                    else
                    {
                        BT_UNPACK_LE_2_BYTE(&attr_val_2B, attr_val);
                        LOG_DEBUG("GOEP L2CAP PSM: 0x%04X\n", attr_val_2B);

                        /* Update the PSM */
                        psm = attr_val_2B;
                    }
                }
#endif /* OBEX_OVER_L2CAP */

                /* if ((API_SUCCESS != retval) && (RFCOMM_PSM == psm)) */
                {
                    /* No PSMs found, check the MAP server channel number */
                    retval = BT_sdp_get_channel_number
                             (
                                 sdp_query_result,
                                 &map_server_channel
                             );

                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG("SDP FAILED to find MAP Server Channel\n");
                    }
                    else
                    {
                        LOG_DEBUG("Server Channel: 0x%02X\n", map_server_channel);

                        /*
                         * Store the Server Channel globally.
                         * Later it will be used while Creating MAP Connection
                         */
                    }
                }

                /* MAP PSM or Server channel found */
                if (API_SUCCESS == retval)
                {
                    LOG_DEBUG ("MAP Service Found, Getting Attributes... \n");

#ifdef MAP_1_2
                    attr_val_4B = 0U;
                    attr_size = 4U;
                    /* Get the SupportedFeatures */
                    retval = BT_sdp_get_attr_value
                             (
                                 MAP_SUPPORTED_FEATURES_ATTR_ID,
                                 sdp_query_result,
                                 attr_val,
                                 &attr_size
                             );
                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG("SDP FAILED to find MAP Supported Features\n");
                    }
                    else
                    {
                        BT_UNPACK_LE_4_BYTE(&attr_val_4B, attr_val);
                        LOG_DEBUG("MAP Supported Features: 0x%08X\n", attr_val_4B);
                    }
#endif /* MAP_1_2 */

#if ((defined MAP_SUPPORT_NOTIFICATION) && (defined APPL_MAP_AUTO_NTF_ACTION))
                    if (0x01U == ntf_status)
                    {
                        connect_info.bd_addr = mns_instance[map_app_instance].bd_addr;
                        connect_info.server_channel = map_server_channel;
#ifdef OBEX_OVER_L2CAP
                        connect_info.psm = psm;
#endif /* OBEX_OVER_L2CAP */
                        connect_info.max_recv_size = 512U;

                        LOG_DEBUG ("Connecting...\n");
                        retval = BT_map_mse_ns_connect
                                 (
                                     &mns_instance[map_app_instance].handle,
                                     &connect_info
                                 );
                        LOG_DEBUG ("Retval - 0x%04X\n", retval);
                    }
#endif /* ((defined MAP_SUPPORT_NOTIFICATION) && (defined APPL_MAP_AUTO_NTF_ACTION)) */
                }
                else
                {
                    LOG_DEBUG ("Failed to find MAP Service.\n");
                }

                no_server_channels++;
            }
        }

        /* Now Close the SDP Connection */
        (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&mns_instance[map_app_instance].map_sdp_record.map_sdp_handle);
        break;

    default: /* Events not needed to be Handled */
        LOG_DEBUG("Unhandled SDP Event (0x%02X)\n", command);
        break;
    }/* switch */

    return;
}


void map_mse_print_appl_instances(void)
{
    UINT8 i;

    LOG_DEBUG ("\n\n===========================================================\n");
    LOG_DEBUG ("Instance\t MAS Handle\t BD Addr\t MNS Id");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < MAP_SERVER_NUM_INSTANCES; i++)
    {
        LOG_DEBUG ("%d\t\t ", i);
        LOG_DEBUG ("%02x\t ", mas_instance[i].instance.handle);
        LOG_DEBUG (BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\t ",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (mas_instance[i].instance.bd_addr));
        LOG_DEBUG ("%02x\n", mas_instance[i].mns_id);
    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");


    LOG_DEBUG ("\n\n===========================================================\n");
    LOG_DEBUG ("Instance\t MNS Handle\t BD Addr");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < MAP_SERVER_NUM_INSTANCES; i++)
    {
        LOG_DEBUG ("%d\t\t ", i);
        LOG_DEBUG ("%02x\t ", mns_instance[i].handle);
        LOG_DEBUG (BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER,
        BT_DEVICE_ADDR_ONLY_PRINT_STR (mns_instance[i].bd_addr));
    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");
}


#ifdef HAVE_OBJECT_PARSER
#define BMSG_STATE_OUT               0x00U
#define BMSG_STATE_IN                0x01U
#define BMSG_STATE_IN_BENV           0x02U
#define BMSG_STATE_IN_BBODY          0x03U
#define BMSG_STATE_IN_MSG            0x04U

static UCHAR begin_transition_table[] =
{
    BMSG_STATE_IN,
    BMSG_STATE_IN_BENV,
    BMSG_STATE_IN_BBODY,
    BMSG_STATE_IN_MSG
};

static UCHAR end_transition_table[] =
{
    0x00U,
    BMSG_STATE_OUT,
    BMSG_STATE_IN,
    BMSG_STATE_IN_BENV,
    BMSG_STATE_IN_BBODY
};

void map_bmsg_parser_cb
     (
         OBJECT_PARSER_CB_PARAM cb_param,
         OBJECT_PARSER_CONTEXT * context
     )
{
    int i;
    static UCHAR state = BMSG_STATE_OUT;
    static UINT16 param_id;

    static UCHAR msg_continue;
    static UINT32 msg_dest_index, msg_remaining;
    UINT32 msg_src_index, to_copy;

    UINT16 token, token_len;

    token = 0U;
    token_len = 0U;

    switch (cb_param.keyword_type)
    {
    case OBJECT_TOKEN_GRP_NAME:
        fLOG_DEBUG (map_parsed_fp, "GROUP NAME ---> ");
        break;

    case OBJECT_TOKEN_PROP_NAME:
        fLOG_DEBUG (map_parsed_fp, "Property NAME ---> ");

        param_id = cb_param.keyword_id;
        break;

    case OBJECT_TOKEN_PROP_VAL:
        fLOG_DEBUG (map_parsed_fp, "Property VALUE ---> ");

        if (0U != (cb_param.parser_result & OBJECT_RESULT_PARTIAL_PARSED))
        {
            fLOG_DEBUG (map_parsed_fp, "<Partial value> ");
        }

        /* Try to identify the property param name token */
        token = token_match
                (
                    cb_param.value,
                    cb_param.value_len,
                    &token_len
                );

        switch (param_id)
        {
        case BEGIN:
            if (VCARD == token)
            {
                context->callback = map_vcard_parser_cb;
                break;
            }

            if (BENV == token)
            {
                map_msg_object.envelope_level ++;
                map_msg_object.num_envelopes ++;
            }

            if ((1U < map_msg_object.envelope_level) && (BENV == token))
            {
                /* Dont do anything */
            }
            else
            {
                /* Set the BEGIN state of the working block */
                state = begin_transition_table[state];
            }
            break;

        case END:
            if ((1U < map_msg_object.envelope_level) && (BENV == token))
            {
                /* Dont do anything */
            }
            else
            {
                /* Set the END state of the working block */
                state = end_transition_table[state];
            }

            if (BENV == token)
            {
                map_msg_object.envelope_level--;
            }
            break;

        case VERSION:
            BT_mem_copy
            (map_msg_object.property.version, cb_param.value, cb_param.value_len);
            map_msg_object.property.version[cb_param.value_len] = '\0';
            break;

        case STATUS:
            map_msg_object.property.status = token;
            break;

        case TYPE:
            map_msg_object.property.type = token;
            break;

        case FOLDER:
            BT_mem_copy
            (map_msg_object.property.folder, cb_param.value, cb_param.value_len);
            map_msg_object.property.folder[cb_param.value_len] = '\0';
            break;

        case ENCODING:
            map_msg_object.envelope.content.property.encoding = token;
            break;

        case CHARSET:
            map_msg_object.envelope.content.property.charset = token;
            break;

        case LANGUAGE:
            BT_mem_copy
            (map_msg_object.envelope.content.property.language,
            cb_param.value, cb_param.value_len);
            map_msg_object.envelope.content.property.language[cb_param.value_len] = '\0';
            break;

        case LENGTH:
            map_msg_object.envelope.content.property.content_length = atoi(cb_param.value);

            /* Allocate memory to copy the incoming message */
            map_msg_object.envelope.content.message = NULL;
            map_msg_object.envelope.content.message =
            BT_alloc_mem (map_msg_object.envelope.content.property.content_length + 1U);

            break;

        default:
            LOG_DEBUG("Invalid Param ID: %d\n", param_id);
            break;
        }

        break;

    case OBJECT_TOKEN_PROP_PARAM_NAME:
        fLOG_DEBUG (map_parsed_fp, "Property PARAM NAME ---> ");
        break;

    case OBJECT_TOKEN_PROP_PARAM_VAL:
        fLOG_DEBUG (map_parsed_fp, "Property PARAM VALUE ---> ");
        break;

    case OBJECT_TOKEN_ERROR:
        fLOG_DEBUG (map_parsed_fp, "***ERROR*** ---> ");
        break;

    default:
        LOG_DEBUG("Invalid Keyword Type: %d\n", cb_param.keyword_type);
        break;
    }

    /* Copy if it is expected message */
    if (BMSG_STATE_IN_MSG == state)
    {
        if (0U == msg_continue)
        {
            msg_dest_index = 0U;
            msg_remaining = map_msg_object.envelope.content.property.content_length;

            context->total_parsed_length += context->cur_parsed_length;
            msg_src_index = context->total_parsed_length;

            to_copy =
            ((context->buffer_len - context->total_parsed_length) <
            map_msg_object.envelope.content.property.content_length)?
            (context->buffer_len - context->total_parsed_length):
            map_msg_object.envelope.content.property.content_length;
        }
        else
        {
            msg_src_index = 0U;

            to_copy =
            (context->buffer_len < msg_remaining)?
            context->buffer_len: msg_remaining;
        }

        if (NULL != map_msg_object.envelope.content.message)
        {
            BT_mem_copy
            (
                (&map_msg_object.envelope.content.message[msg_dest_index]),
                (context->buffer + msg_src_index),
                to_copy
            );
        }

        msg_dest_index += to_copy;
        msg_remaining -= to_copy;

        if ((msg_dest_index ==
             map_msg_object.envelope.content.property.content_length) &&
            (NULL != map_msg_object.envelope.content.message))
        {
            map_msg_object.envelope.content.message[msg_dest_index] = '\0';
        }

        context->total_parsed_length += to_copy;
        context->cur_parsed_length = 0U;
    }

    if (MSG == token)
    {
        if (BEGIN == param_id)
        {
            msg_continue = 0x01U;
        }
        else if (END == param_id)
        {
            msg_continue = 0x00U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }

    fLOG_DEBUG (map_parsed_fp, "[KEY : 0x%04x] ", cb_param.keyword_id);

    for (i = 0U; i < cb_param.value_len; i++)
    {
        fLOG_DEBUG (map_parsed_fp, "%c", cb_param.value [i]);
    }
    fLOG_DEBUG (map_parsed_fp, "\n");

    if (0U != (cb_param.parser_result & 0x01U))
    {
        fLOG_DEBUG (map_parsed_fp, "Folded Line Detected\n");
    }
}


void map_vcard_parser_cb
     (
         OBJECT_PARSER_CB_PARAM cb_param,
         OBJECT_PARSER_CONTEXT * context
     )
{
    static UINT16 vcard_param_id = BEGIN;

    UINT8 i;
    UINT16 token, token_len;

    token = 0U;
    token_len = 0U;

    switch (cb_param.keyword_type)
    {
    case OBJECT_TOKEN_GRP_NAME:
        fLOG_DEBUG (map_parsed_fp, "GROUP NAME ---> ");
        break;

    case OBJECT_TOKEN_PROP_NAME:
        fLOG_DEBUG (map_parsed_fp, "Property NAME ---> ");

        vcard_param_id = cb_param.keyword_id;
        break;

    case OBJECT_TOKEN_PROP_VAL:
        fLOG_DEBUG (map_parsed_fp, "Property VALUE ---> ");

        if (0U != (cb_param.parser_result & OBJECT_RESULT_PARTIAL_PARSED))
        {
            fLOG_DEBUG (map_parsed_fp, "<Partial value> ");
        }

        /* Try to identify the property param name token */
        token = token_match
                (
                    cb_param.value,
                    cb_param.value_len,
                    &token_len
                );

        switch (vcard_param_id)
        {
        case BEGIN:
            break;

        case END:
            if (VCARD == token)
            {
                context->callback = map_bmsg_parser_cb;
                vcard_param_id = BEGIN;
            }
            break;

        case VERSION:
            break;

        case N:
            if (!map_msg_object.envelope_level)
            {
                BT_mem_copy
                (
                    map_msg_object.originator.property->param[0U].param_value,
                    cb_param.value,
                    cb_param.value_len
                );

                map_msg_object.originator.
                property->param[0U].param_value[cb_param.value_len] = '\0';
            }
            else
            {
                BT_mem_copy
                (
                    map_msg_object.envelope.recepient[map_msg_object.envelope_level - 1U].
                    property[0U].param[0U].param_value,
                    cb_param.value,
                    cb_param.value_len
                );

                map_msg_object.envelope.recepient[map_msg_object.envelope_level - 1U].
                property[0U].param[0U].param_value[cb_param.value_len] = '\0';
            }
            break;

        case ENCODING:
            break;

        case CHARSET:
            break;

        default:
            LOG_DEBUG("Invalid Param ID: %d\n", vcard_param_id);
            break;
        }

        break;

    case OBJECT_TOKEN_PROP_PARAM_NAME:
        fLOG_DEBUG (map_parsed_fp, "Property PARAM NAME ---> ");
        break;

    case OBJECT_TOKEN_PROP_PARAM_VAL:
        fLOG_DEBUG (map_parsed_fp, "Property PARAM VALUE ---> ");
        break;

    case OBJECT_TOKEN_ERROR:
        fLOG_DEBUG (map_parsed_fp, "***ERROR*** ---> ");
        break;

    default:
        LOG_DEBUG("Invalid Keyword Type: %d\n", cb_param.keyword_type);
        break;
    }

    fLOG_DEBUG (map_parsed_fp, "[KEY : 0x%04x] ", cb_param.keyword_id);

    for (i = 0U; i < cb_param.value_len; i++)
    {
        fLOG_DEBUG (map_parsed_fp, "%c", cb_param.value [i]);
    }
    fLOG_DEBUG (map_parsed_fp, "\n");

    if (0U != (cb_param.parser_result & 0x01U))
    {
        fLOG_DEBUG (map_parsed_fp, "Folded Line Detected\n");
    }
}
#endif /* HAVE_OBJECT_PARSER */

#endif /* MAP_MSE */
