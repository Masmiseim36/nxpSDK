
/**
 *  \file appl_map_mce.c
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_map.h"
#include "appl_utils.h"

#ifdef MAP_MCE
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/**
 * Flag to register peer bd_addr at the start.
 */
#define   APPL_REGISTER_PEER_BD_ADDR

/* --------------------------------------------- Static Global Variables */
static UCHAR                 mapc_bd_addr[BT_BD_ADDR_SIZE];
static UINT16                connection_handle;

static UCHAR                 map_app_instance;
static UCHAR                 map_server_channel;

static MAP_MAS_INSTANCE      mas_instance[MAP_CLIENT_NUM_INSTANCES];
static MAP_INSTANCE          mns_instance[MAP_CLIENT_NUM_INSTANCES];

static UCHAR                 mapc_file_object[MAPC_MAX_FILE_OBJ_NAME_LEN];
static BT_fops_file_handle   fp;
static UINT32                fsize;
static UINT32                remaining, sent;
static UINT16                map_xchg_size;
static UINT16                map_ns_xchg_size;
static UCHAR                 map_push_started;

#ifdef HAVE_OBJECT_PARSER
static OBJECT_PARSER_CONTEXT map_context;
static UCHAR                 map_parse_buffer[OBEX_MAX_PACKET_LENGTH + APPL_MAP_MAX_TAG_LEN];
static UINT16                map_parser_diff_len;
static BT_fops_file_handle   map_parsed_fp;
static MESSAGE_OBJECT        map_msg_object;
#endif /* HAVE_OBJECT_PARSER */

static BT_fops_file_handle   map_raw_fp;

static UINT16                map_list_index;
static BT_fops_file_handle   map_list_fp;

/* To acount for appl. params size */
static UINT16 appl_map_mce_xchg_size;

/* Indicator for first response packet for any request */
static UINT16 appl_map_mce_start_of_response;

#ifdef MAP_1_3
/* Sample Application Parameter Values */
static UCHAR    appl_last_activity[APPL_PARAM_VALUE_MAX_LEN];
static UCHAR    appl_period_begin[APPL_PARAM_VALUE_MAX_LEN];
static UCHAR    appl_period_end[APPL_PARAM_VALUE_MAX_LEN];
static UCHAR    appl_presence_text[APPL_PARAM_VALUE_MAX_LEN];
static UCHAR    appl_recipient[APPL_PARAM_VALUE_MAX_LEN];
static UCHAR    appl_conversation_id[APPL_PARAM_VALUE_MAX_LEN];
static UCHAR    appl_extended_data[APPL_PARAM_VALUE_MAX_LEN];
static UCHAR    appl_filter_msg_handle[APPL_PARAM_VALUE_MAX_LEN];
#endif /* MAP_1_3 */

#ifdef MAP_1_4
static UCHAR    appl_msg_handle[APPL_PARAM_VALUE_MAX_LEN];
#endif /* MAP_1_4 */

/* TODO: Revert back as auto variable */
static XML_PARSER_OBJECT     xml_obj;

static UCHAR wait_count;

static const UCHAR map_mce_menu[] =
" \n \
-------------- MAP Client Menu --------------\n \
\n \
\t 0. Exit \n \
\t 1. Refresh \n \
\n \
\t 2. Register Peer BD Addrs \n\
\t 3. Get Registered Peer BD Addrs. \n\
\n \
\t 5. MCE Initialize \n \
\t 6. MCE Shutdown \n \
\t 7. MCE Start \n \
\t 8. MCE Stop \n \
\n \
\t10. Create ACL Connection\n\
\t11. ACL Disconnection\n\
\t12. Do SDP Query \n\
\n \
\t15. Connect to MSE \n \
\t16. Disconnect from MSE \n \
\t17. Close transport with MSE \n \
\n \
\t20. Update Inbox \n \
\t21. Set Folder \n \
\t22. Get Folder Listing \n \
\t23. Get Messages Listing \n \
\t24. Get Message \n \
\t25. Set Message Status \n \
\t26. Push Message \n \
\n \
\t30. Register Notification \n \
\t31. Close Notification transport with MSE \n \
\n \
\t35. Abort Operation \n \
\n \
\t40. Get MAS Instance Info \n \
\n \
\t41. Get Coversation Listing \n \
\t42. Set Owner Status \n \
\t43. Get Owner Status \n \
\t44. Set Notification Filter \n \
\n \
\t50. List Directory \n \
\n \
Your Choice: ";

/* --------------------------------------------- Functions */
API_RESULT appl_map_mce_callback
           (
               MAP_HANDLE_INFO * map_handle,
               UINT8             event_type,
               UINT16            event_result,
               MAP_HEADERS     * event_header,
               UINT16            event_hdrlen
           );

void appl_map_mce_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     );

void map_print_appl_instances (void);

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

void appl_map_mce_print_folder_list( XML_DOC_COMPONENT* pTemp);
void appl_map_mce_print_message_list( XML_DOC_COMPONENT* pTemp);

void main_map_mce_operations (void)
{
    API_RESULT retval;
    UINT8 i;
    UINT8 *phandle;
    UCHAR *pbd_addr;
    UINT16 actual;

    MAP_CONNECT_STRUCT connect_info;
    MAP_REQUEST_STRUCT get_info, set_info;
    MAP_HEADER_STRUCT name_req, body_req;
    MAP_APPL_PARAMS appl_param;

    UCHAR   name[64U], msg_file[32U];
    UCHAR   convs_id[34U];
    UCHAR   originator[32U], recipient[32U];
    UCHAR   period_begin[32U], period_end[32U];
    UINT16  size, len, temp;
    UCHAR   more;

    CHAR path[BT_FOPS_MAX_DIRECTORY_SIZE];

    int     choice, menu_choice, cn_handle, sc, val, handle;

    /* Init */
    more          = 0U;
    fp            = NULL;
    map_raw_fp    = NULL;
    map_list_fp   = NULL;

#ifdef HAVE_OBJECT_PARSER
    map_parsed_fp = NULL;
#endif /* HAVE_OBJECT_PARSER */

    BT_LOOP_FOREVER()
    {
        printf ("%s", map_mce_menu);
        scanf ("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
            case 0:
                break; /* return; */

            case 1:
                break;

            case 2: /* Register MAP MSE's BD addrs */
                LOG_DEBUG("Enter MAP MSE's Bluetooth Device Address\n");

                /* Read the BD_ADDR of Remote Device */
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(mapc_bd_addr);
                break;

            case 3:
                /* Get Registered BD_ADDR of Peer Device */
                LOG_DEBUG("\nRegistered Peer BD "\
                BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
                BT_DEVICE_ADDR_ONLY_PRINT_STR(mapc_bd_addr));
                break;

            case 5:
                LOG_DEBUG ("Initializing MAP MCE...\n");
                retval = BT_map_mce_init ();
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                if (API_SUCCESS == retval)
                {
                    /* Reset the MAP handle */
                    for (i = 0U; i < MAP_CLIENT_NUM_INSTANCES; i++)
                    {
                        MAP_RESET_INSTANCE(&(mas_instance[i].instance));
                        mas_instance[i].mns_id = MAP_ENTITY_INVALID;

                        MAP_RESET_INSTANCE(&mns_instance[i]);
                    }

#ifdef HAVE_OBJECT_PARSER
                LOG_DEBUG ("Initializing MAP Parser...\n");
                retval = object_parser_init (&map_context, map_bmsg_parser_cb);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                map_parser_diff_len = 0U;
                map_msg_object.envelope_level = 0U;
                map_msg_object.envelope.content.message = NULL;
#endif /* HAVE_OBJECT_PARSER */
                }

                map_print_appl_instances();
                break;

            case 6:
                LOG_DEBUG ("Shutting down MAP MCE...\n");
                retval = BT_map_mce_shutdown ();
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 7:
                map_print_appl_instances();

                LOG_DEBUG ("Enter Service Type (1-MAS, 2-MNS): ");
                scanf ("%d", &val);

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                phandle = (1U == val)?
                    &mas_instance[handle].instance.handle:
                    &mns_instance[handle].handle;

                if (MAP_ENTITY_INVALID != *phandle)
                {
                    LOG_DEBUG ("Application Instance not free!\n");
                    break;
                }

                LOG_DEBUG ("Starting MAP MCE Instance...\n");
                retval = BT_map_mce_start
                         (
                             (MAP_SERVICE_TYPE)val,
                             appl_map_mce_callback,
                             phandle
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                if (API_SUCCESS != retval)
                {
                    *phandle = MAP_ENTITY_INVALID;
                }

                map_print_appl_instances();
                break;

            case 8:
                map_print_appl_instances();

                LOG_DEBUG ("Enter Service Type (1-MAS, 2-MNS): ");
                scanf ("%d", &val);

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

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

                LOG_DEBUG ("Stopping MAP MCE Service %d...\n", val);
                retval = BT_map_mce_stop ((MAP_SERVICE_TYPE)val, phandle);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                if (API_SUCCESS == retval)
                {
                    *phandle = MAP_ENTITY_INVALID;
                    BT_mem_set (pbd_addr, 0x00, BT_BD_ADDR_SIZE);

                    /* Close any file objects */
                    if (NULL != fp)
                    {
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_fops_file_close (fp);
                        fp = NULL;
                    }

                    if (NULL != map_list_fp)
                    {
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_fops_file_close (map_list_fp);
                        map_list_fp = NULL;
                    }

#ifdef HAVE_OBJECT_PARSER
                    if (NULL != map_parsed_fp)
                    {
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_fops_file_close (map_parsed_fp);
                        map_parsed_fp = NULL;
                    }
#endif /* HAVE_OBJECT_PARSER */

                    if (NULL != map_raw_fp)
                    {
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_fops_file_close (map_raw_fp);
                        map_raw_fp = NULL;
                    }
                }

                map_print_appl_instances();
                break;

            case 10:
                /*
                 * First Application should establish ACL Connection with
                 * the peer Device
                 */
                LOG_DEBUG("Enter MAP MSE's Bluetooth Device Address: ");

    #ifndef APPL_REGISTER_PEER_BD_ADDR
                /* Read the BD_ADDR of Remote Device */
                appl_get_bd_addr(mapc_bd_addr);
    #endif /* APPL_REGISTER_PEER_BD_ADDR */

                retval = BT_hci_create_connection
                         (
                             mapc_bd_addr,
                             LMP_ACL_DH5,
                             0x0U, 0x0U, 0x0U, 0x1U
                         );

                if ( retval != API_SUCCESS )
                {
                    LOG_DEBUG("HCI Connect Attempt FAILED !! Error Code = 0x%04X\n",
                    retval);
                }
                break;

            case 11:
                /* To disconnect read the ACL Connection Handle */
                LOG_DEBUG("Enter Connection Handle (in Hex): "); fflush(stdout);
                scanf("%x", &cn_handle);
                connection_handle = (UINT16)cn_handle;

                LOG_DEBUG("\n");
                LOG_DEBUG("Starting HCI Disconnection ... "); fflush(stdout);

                /* Mention Reason for Disconnect as User Initiated Disconnection */
                retval = BT_hci_disconnect ( connection_handle, 0x13U );
                if ( retval != API_SUCCESS )
                {
                    LOG_DEBUG("HCI Disconnection FAILED !! Error Code = 0x%04X\n",
                    retval);
                }
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

                LOG_DEBUG ("Select MCE Entity Instance: ");
                scanf ("%d", &cn_handle);
                map_app_instance = (UCHAR)cn_handle;

    #ifndef APPL_REGISTER_PEER_BD_ADDR
                LOG_DEBUG ("Enter peer device address: ");
                appl_get_bd_addr (mapc_bd_addr);
    #endif /* APPL_REGISTER_PEER_BD_ADDR */

                mas_instance[map_app_instance].instance.map_sdp_record.map_len_attrib_data =
                      MAP_SDP_RECORD_DATA_SIZE;

                SDP_SET_HANDLE
                (
                    mas_instance[map_app_instance].instance.map_sdp_record.map_sdp_handle,
                    mapc_bd_addr,
                    appl_map_mce_sdp_callback
                );

                retval = BT_sdp_open(&mas_instance[map_app_instance].instance.map_sdp_record.map_sdp_handle);

                if ( retval != API_SUCCESS )
                {
                    LOG_DEBUG("SDP Connect FAILED !! Error Code = 0x%04X\n", retval);
                }

                LOG_DEBUG("Please Wait for SDP Operation to Complete\n");
                break;

            case 15:
                map_print_appl_instances();

                BT_mem_set(&connect_info, 0, sizeof(MAP_CONNECT_STRUCT));

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

#ifndef APPL_REGISTER_PEER_BD_ADDR
                LOG_DEBUG ("Enter peer device address: ");
                appl_get_bd_addr (mapc_bd_addr);
#endif /* APPL_REGISTER_PEER_BD_ADDR */

                connect_info.bd_addr = mapc_bd_addr;

#ifdef MAP_1_2
                LOG_DEBUG ("Enter peer L2CAP PSM (in Hex): ");
                scanf ("%x", &val);
                connect_info.psm = (UINT16)val;
#endif /* MAP_1_2 */

                LOG_DEBUG ("Enter peer server channel (in Hex): ");
                scanf ("%x", &sc);
                connect_info.server_channel = (UCHAR)sc;

                LOG_DEBUG ("Enter Data Exchange Size: ");
                scanf ("%d", &val);
                size = (UINT16) val;
                connect_info.max_recv_size = size;
#ifdef MAP_1_3
                /**
                * Note:
                * MapSupportedFeatures is Mandatory if the MSE advertises a
                * MapSupportedFeatures attribute in its SDP rsp.
                */
                LOG_DEBUG ("Add MAP MCE Supported Features.\n");
                LOG_DEBUG ("    1 -> Yes\n");
                LOG_DEBUG ("    0 -> No\n");
                scanf ("%d", &val);

                connect_info.map_supported_features = 0x00U;
                if (0x00U != val)
                {
                    connect_info.map_supported_features = APPL_MAP_MCE_SUPPORTED_FEATURES;
                }
#endif /* MAP_1_3 */

                LOG_DEBUG ("Connecting...\n");
                retval = BT_map_mce_connect
                         (
                             &mas_instance[handle].instance.handle,
                             &connect_info
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 16:
                map_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                LOG_DEBUG ("Disconnecting on MAP MCE Instance %d\n", handle);
                retval = BT_map_mce_disconnect (&mas_instance[handle].instance.handle);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 17:
                map_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                LOG_DEBUG ("Closing on MAP MCE Instance %d\n", handle);
                retval = BT_map_mce_transport_close (&mas_instance[handle].instance.handle);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 20:
                map_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                LOG_DEBUG ("Requesting to update inbox...\n");
                retval = BT_map_mce_update_inbox (&mas_instance[handle].instance.handle);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 21:
                map_print_appl_instances();

                BT_mem_set(&set_info, 0, sizeof(MAP_REQUEST_STRUCT));

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                LOG_DEBUG ("Enter folder to set, (1:Child, 2:Parent, 3:Root): ");
                scanf ("%d", &choice);

                /* Reset Name params */
                len = 0U;
                name[0U] = '\0';

                if (choice < 1 || choice > 3)
                {
                    LOG_DEBUG ("Invalid Option \n");
                    break;
                }

                /* Get name for child/parent folder */
                if ((1 == choice) || (2 == choice))
                {
                    LOG_DEBUG ("Enter Folder Name: ");
                    fflush(stdin); fflush(stdout);

                    scanf ("%s", name);
                    len = (UINT16) BT_str_n_len(name, sizeof(name));
                    name[len]='\0';
                }

                /* Form request info */
                set_info.setpath_flag = (UCHAR )choice;
                name_req.length = (1U == len)? 0U: len;
                name_req.length ++;
                name_req.value = name;
                set_info.name = &name_req;

                LOG_DEBUG ("Requesting to set folder...\n");
                retval = BT_map_mce_set_folder
                    (&mas_instance[handle].instance.handle, &set_info);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 22:
                map_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
                BT_mem_set(&get_info, 0, sizeof(MAP_REQUEST_STRUCT));

                LOG_DEBUG("Enter number of wait stages: ");
                scanf("%d", &choice);
                wait_count = (UCHAR)choice;

                if (0U < wait_count)
                {
                    get_info.wait = BT_TRUE;
                    wait_count--;
                }
                else
                {
                    get_info.wait = BT_FALSE;
                }

                /* Update request info */
                get_info.appl_params = & appl_param;

                LOG_DEBUG ("Specify Application Parameters? (1/0): ");
                scanf ("%d", &choice);

                if (0 == choice)
                {
                    LOG_DEBUG ("Requesting to get folder listing...\n");
                    retval = BT_map_mce_get_folder_listing
                             (
                                 &mas_instance[handle].instance.handle,
                                 &get_info
                             );
                    LOG_DEBUG ("Retval - 0x%04X\n", retval);

                    break;
                }

                LOG_DEBUG ("Include Appl Param 'MaxListCount' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_MAXLISTCOUNT
                    );

                    LOG_DEBUG ("Enter MaxListCount value: ");
                    scanf ("%d", &val);
                    appl_param.max_list_count = (UINT16) val;
                }

                LOG_DEBUG ("Include Appl Param 'ListStartOffset' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_STARTOFFSET
                    );

                    LOG_DEBUG ("Enter ListStartoffset value: ");
                    scanf ("%d", &val);
                    appl_param.start_offset = (UINT16) val;
                }

                LOG_DEBUG ("Requesting to get folder listing...\n");
                retval = BT_map_mce_get_folder_listing
                         (
                             &mas_instance[handle].instance.handle,
                             &get_info
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 23:
                map_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
                BT_mem_set(&get_info, 0, sizeof(MAP_REQUEST_STRUCT));

                /* Update request info */
                get_info.appl_params = & appl_param;

                LOG_DEBUG ("Enter folder name length ('0' for current): ");
                scanf ("%d", &val);
                len = (UINT16)val;

                if (0U != len)
                {
                    LOG_DEBUG ("Enter child folder name: ");
                    scanf ("%s", name);
                }

                /* Update request info */
                name_req.length = (0U == len)? 0U: (len + 1U);
                name_req.value = name;
                get_info.name = &name_req;

                LOG_DEBUG("Enter number of wait stages: ");
                scanf("%d", &choice);
                wait_count = (UCHAR)choice;

                if (0U < wait_count)
                {
                    get_info.wait = BT_TRUE;
                    wait_count--;
                }
                else
                {
                    get_info.wait = BT_FALSE;
                }

                LOG_DEBUG ("Specify Application Parameters? (1/0): ");
                scanf ("%d", &choice);

                if (0 == choice)
                {
                    LOG_DEBUG ("Requesting to get messages listing...\n");
                    retval = BT_map_mce_get_messages_listing
                             (
                                 &mas_instance[handle].instance.handle,
                                 &get_info
                             );
                    LOG_DEBUG ("Retval - 0x%04X\n", retval);

                    break;
                }

                LOG_DEBUG ("Include Appl Param 'MaxListCount' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_MAXLISTCOUNT
                    );

                    LOG_DEBUG ("Enter MaxListCount value: ");
                    scanf ("%d", &val);
                    appl_param.max_list_count = (UINT16)val;
                }

                LOG_DEBUG ("Include Appl Param 'ListStartOffset' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_STARTOFFSET
                    );

                    LOG_DEBUG ("Enter ListStartoffset value: ");
                    scanf ("%d", &val);
                    appl_param.start_offset = (UINT16)val;
                }

                LOG_DEBUG ("Include Appl Param 'SubjectLength' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_SUBJECTLENGTH
                    );

                    LOG_DEBUG ("Enter SubjectLength value: ");
                    scanf ("%d", &val);
                    appl_param.subject_length = (UINT8)val;
                }

                LOG_DEBUG ("Include Appl Param 'ParameterMask' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_PARAMETERMASK
                    );

                    LOG_DEBUG ("Enter ParameterMask value: ");
                    scanf ("%lu", &appl_param.parameter_mask);
                }

                LOG_DEBUG ("Include Appl Param 'FilterMessageType' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_FILTERMESSAGETYPE
                    );

                    LOG_DEBUG("MessageTypes:\n");
                    LOG_DEBUG("    Bit #0 -> SMS_GSM\n");
                    LOG_DEBUG("    Bit #1 -> SMS_CDMA\n");
                    LOG_DEBUG("    Bit #2 -> EMAIL\n");
                    LOG_DEBUG("    Bit #3 -> MMS\n");
                    LOG_DEBUG("    Bit #4 -> IM\n");
                    LOG_DEBUG ("Enter FilterMessageType value(in Hex): ");
                    scanf ("%x", &val);
                    appl_param.filter_message_type = (UINT8)val;
                }

                LOG_DEBUG ("Include Appl Param 'FilterPeriodBegin' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    appl_param.filter_period_begin.value = period_begin;
                    appl_param.filter_period_begin.length = 0U;

                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_FILTERPERIODBEGIN
                    );

                    LOG_DEBUG ("Enter FilterPeriodBegin value: ");
                    scanf ("%s", appl_param.filter_period_begin.value);
                    appl_param.filter_period_begin.length = (UINT16)BT_str_n_len(period_begin, sizeof(period_begin));
                }

                LOG_DEBUG ("Include Appl Param 'FilterPeriodEnd' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    appl_param.filter_period_end.value = period_end;
                    appl_param.filter_period_end.length = 0U;

                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_FILTERPERIODEND
                    );

                    LOG_DEBUG ("Enter FilterPeriodEnd value: ");
                    scanf ("%s", appl_param.filter_period_end.value);
                    appl_param.filter_period_end.length = (UINT16)BT_str_n_len(period_end, sizeof(period_end));
                }

                LOG_DEBUG ("Include Appl Param 'FilterReadStatus' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_FILTERREADSTATUS
                    );

                    LOG_DEBUG("FilterReadStatus\n");
                    LOG_DEBUG("    Bit #0 -> get unread messages only\n");
                    LOG_DEBUG("    Bit #1 -> get read message only\n");
                    LOG_DEBUG("    0 -> no-filtering\n");
                    LOG_DEBUG ("Enter FilterReadStatus value(in Hex): ");
                    scanf ("%x", &val);
                    appl_param.filter_read_status = (UINT8)val;
                }

                LOG_DEBUG ("Include Appl Param 'FilterRecipient' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    appl_param.filter_recipient.value = recipient;
                    appl_param.filter_recipient.length = 0U;

                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_FILTERRECIPIENT
                    );

                    LOG_DEBUG ("Enter FilterRecipient value: ");
                    scanf ("%s", appl_param.filter_recipient.value);
                    appl_param.filter_recipient.length = (UINT16)BT_str_n_len(recipient, sizeof(recipient));
                }

                LOG_DEBUG ("Include Appl Param 'FilterOriginator' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    appl_param.filter_originator.value = originator;
                    appl_param.filter_originator.length = 0U;

                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_FILTERORIGINATOR
                    );

                    LOG_DEBUG ("Enter FilterOriginator value: ");
                    scanf ("%s", appl_param.filter_originator.value);
                    appl_param.filter_originator.length = (UINT16)BT_str_n_len(originator, sizeof(originator));
                }

                LOG_DEBUG ("Include Appl Param 'FilterPriority' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_FILTERPRIORITY
                    );

                    LOG_DEBUG ("FilterPriority\n");
                    LOG_DEBUG("    Bit #0 -> get high priority messages only\n");
                    LOG_DEBUG("    Bit #1 -> get non-high priority message only\n");
                    LOG_DEBUG("    0 -> no-filtering\n");
                    LOG_DEBUG ("Enter FilterPriority value: ");
                    scanf ("%d", &val);
                    appl_param.filter_priority = (UINT8) val;
                }
#ifdef MAP_1_3
                LOG_DEBUG ("Include Appl Param 'ConversationID' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG_EXT
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_CONVERSATION_ID,
                        1U
                    );

                    LOG_DEBUG ("Enter ConvID value: \n");
                    LOG_DEBUG ("Example: 1234567890123456789ABCDEF1234567\n");
                    scanf("%s", appl_conversation_id);

                    appl_param.conversation_id.value = appl_conversation_id;
                    appl_param.conversation_id.length = (UINT16)BT_str_n_len(appl_conversation_id, sizeof(appl_conversation_id));
                }

                LOG_DEBUG ("Include Appl Param 'FilterMessageHandle' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG_EXT
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_FILTER_MSG_HANDLE,
                        1U
                    );

                    LOG_DEBUG("Enter Message handle value: ");
                    scanf("%s", appl_filter_msg_handle);

                    appl_param.filter_msg_handle.value = appl_filter_msg_handle;
                    appl_param.filter_msg_handle.length = (UINT16)BT_str_n_len(appl_filter_msg_handle, sizeof(appl_filter_msg_handle));
                }
#endif /* MAP_1_3 */

                LOG_DEBUG ("Requesting to get messages listing...\n");
                retval = BT_map_mce_get_messages_listing
                         (
                             &mas_instance[handle].instance.handle,
                             &get_info
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 24:
                map_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
                BT_mem_set(&get_info, 0, sizeof(MAP_REQUEST_STRUCT));

                /* Update request info */
                get_info.appl_params = & appl_param;

                /* TODO: See how to get this 16 gigit UNICODE input */
                LOG_DEBUG ("Enter Message handle to be got: ");
                scanf ("%s", name);

                /* Update request info */
                name_req.length = (UINT16)(BT_str_n_len(name, sizeof(name)) + 1U);
                name_req.value = name;
                get_info.name = &name_req;

                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_ATTACHMENT
                );

                LOG_DEBUG("Attachment:\n");
                LOG_DEBUG("    0 -> OFF\n");
                LOG_DEBUG("    1 -> ON\n");
                LOG_DEBUG ("Enter Attachment value: ");
                scanf ("%d", &val);
                appl_param.attachment = (UINT8)val;

                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_CHARSET
                );

                LOG_DEBUG("Charset:\n");
                LOG_DEBUG("    0 -> native\n");
                LOG_DEBUG("    1 -> UTF-8\n");
                LOG_DEBUG("Enter Charset value: ");
                scanf ("%d", &val);
                appl_param.charset = (UINT8)val;

                LOG_DEBUG ("Include Appl Param 'FractionRequest' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_FRACTIONREQUEST
                    );

                    LOG_DEBUG("FractionRequest:\n");
                    LOG_DEBUG("    0 -> first\n");
                    LOG_DEBUG("    1 -> next\n");
                    LOG_DEBUG ("Enter FractionRequest value: ");
                    scanf ("%d", &val);
                    appl_param.fraction_request = (UINT8)val;
                }

                LOG_DEBUG("Enter number of wait stages: ");
                scanf("%d", &choice);
                wait_count = (UCHAR)choice;

                if (0U < wait_count)
                {
                    get_info.wait = BT_TRUE;
                    wait_count--;
                }
                else
                {
                    get_info.wait = BT_FALSE;
                }

                LOG_DEBUG ("Requesting to get message...\n");
                retval = BT_map_mce_get_message
                         (
                             &mas_instance[handle].instance.handle,
                             &get_info
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 25:
                map_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");;
                    break;
                }

                MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
                BT_mem_set(&set_info, 0, sizeof(MAP_REQUEST_STRUCT));

                /* Update request info */
                set_info.appl_params = & appl_param;

                /* TODO: See how to get this 16 gigit UNICODE input */
                LOG_DEBUG ("Enter Message handle to be set: ");
                scanf ("%s", name);

                /* Update request info */
                name_req.length = (UINT16)(BT_str_n_len(name, sizeof(name)) + 1U);
                name_req.value = name;
                set_info.name = &name_req;

                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_STATUSINDICATOR
                );
                LOG_DEBUG ("Status Indicator: \n");
                LOG_DEBUG ("  0 -> Read Status\n");
                LOG_DEBUG ("  1 -> Deleted Status\n");
#ifdef MAP_1_3
                LOG_DEBUG ("  2 -> Set Extended Status\n");
#endif /* MAP_1_3 */

                LOG_DEBUG ("Enter StatusIndicator value: ");
                scanf ("%d", &val);
                appl_param.status_indicator = (UINT8)val;

                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_STATUSVALUE
                );
                LOG_DEBUG ("Status Indicator Values: \n");
                LOG_DEBUG ("  1 -> yes\n");
                LOG_DEBUG ("  0 -> no\n");
                LOG_DEBUG ("Enter StatusValue value: ");
                scanf ("%d", &val);
                appl_param.status_value = (UINT8)val;

#ifdef MAP_1_3
                if ((0x02U == appl_param.status_indicator) &&
                    (0x01U == appl_param.status_value))
                {
                    MAP_SET_APPL_PARAM_FLAG_EXT
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_EXTENDED_DATA,
                        1U
                    );

                    LOG_DEBUG ("Enter Extended: \n");
                    LOG_DEBUG ("Example: 0:54;2:48;3:10\n");
                    scanf("%s", appl_extended_data);

                    appl_param.extended_data.value = appl_extended_data;
                    appl_param.extended_data.length = (UINT16)BT_str_n_len(appl_extended_data, sizeof(appl_extended_data));
                }
#endif /* MAP_1_3 */

                LOG_DEBUG ("Requesting to set message status...\n");
                retval = BT_map_mce_set_message_status
                         (
                             &mas_instance[handle].instance.handle,
                             &set_info
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 26:
                map_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                BT_mem_set(&set_info, 0, sizeof(MAP_REQUEST_STRUCT));
                MAP_INIT_HEADER_STRUCT(name_req);
                MAP_INIT_HEADER_STRUCT(body_req);
                MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

                /* Update request info */
                set_info.appl_params = & appl_param;

                fsize = 0U;
                remaining = 0U;
                sent = 0U;
                appl_map_mce_xchg_size = map_xchg_size;

                LOG_DEBUG ("Enter folder name length ('0' for current): ");
                scanf ("%d", &val);
                len = (UINT16) val;
                if (0U != len)
                {
                    LOG_DEBUG ("Enter child folder name: ");
                    scanf ("%s", name);
                }

                /* Update request info */
                name_req.length = (0U == len)? 0U: (len + 1U);
                name_req.value = name;
                set_info.name = &name_req;

                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_CHARSET
                );

                LOG_DEBUG("Charset:\n");
                LOG_DEBUG("    0 -> native\n");
                LOG_DEBUG("    1 -> UTF-8\n");
                LOG_DEBUG ("Enter Charset value: ");
                scanf ("%d", &val);
                appl_param.charset = (UINT8)val;

                appl_map_mce_xchg_size -= 3U;

                LOG_DEBUG ("Include Appl Param 'Transparent' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_TRANSPARENT
                    );

                    LOG_DEBUG("Transparent:\n");
                    LOG_DEBUG("    0 -> OFF\n");
                    LOG_DEBUG("    1 -> ON\n");
                    LOG_DEBUG ("Enter Transparent value: ");
                    scanf ("%d", &val);
                    appl_param.transparent = (UINT8)val;

                    appl_map_mce_xchg_size -= 3U;
                }

                LOG_DEBUG ("Include Appl Param 'Retry' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_RETRY
                    );

                    LOG_DEBUG("Retry:\n");
                    LOG_DEBUG("    0 -> OFF\n");
                    LOG_DEBUG("    1 -> ON\n");
                    LOG_DEBUG ("Enter Retry value: ");
                    scanf ("%d", &val);
                    appl_param.retry = (UINT8)val;

                    appl_map_mce_xchg_size -= 3U;
                }
#ifdef MAP_1_3
                LOG_DEBUG ("Include Appl Param 'ConversationID' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG_EXT
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_CONVERSATION_ID,
                        1U
                    );

                    LOG_DEBUG ("Enter ConvID value: \n");
                    LOG_DEBUG ("Example: 1234567890123456789ABCDEF1234567\n");
                    scanf("%s", appl_conversation_id);

                    appl_param.conversation_id.value = appl_conversation_id;
                    appl_param.conversation_id.length = (UINT16)BT_str_n_len(appl_conversation_id, sizeof(appl_conversation_id));

                    appl_map_mce_xchg_size -= (3U + appl_param.conversation_id.length);

#ifdef MAP_1_4_2
                    /**
                     * If the Conversation Id is added in the request, then the
                     * Name Header shall be empty.
                     */
                    name_req.length = 0U;
#endif /* MAP_1_4_2 */
                }
#endif /* MAP_1_3 */

#ifdef MAP_1_4
                LOG_DEBUG ("Include Appl Param 'Message Handle' (1/0):");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG_EXT
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_MESSAGE_HANDLE,
                        1U
                    );

                    LOG_DEBUG ("Enter Message Handle value: \n");
                    LOG_DEBUG ("Example: 123456789ABC\n");
                    scanf("%s", appl_msg_handle);

                    appl_param.msg_handle.value = appl_msg_handle;
                    appl_param.msg_handle.length = (UINT16)BT_str_n_len(appl_msg_handle, sizeof(appl_msg_handle));

                    appl_map_mce_xchg_size -= (3U + appl_param.msg_handle.length);
                }

                LOG_DEBUG ("Include Appl Param 'Attachment' (1/0):");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_ATTACHMENT
                    );

                    LOG_DEBUG ("Enter Attchment value: \n");
                    LOG_DEBUG ("    0 -> OFF\n");
                    LOG_DEBUG ("    1 -> ON\n");

                    scanf ("%d", &val);
                    appl_param.attachment = (UINT8)val;

                    appl_map_mce_xchg_size -= 3U;
                }

                LOG_DEBUG ("Include Appl Param 'Modify Text' (1/0):");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG_EXT
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_MODIFY_TEXT,
                        1U
                    );

                    LOG_DEBUG ("Enter Modify Text value: \n");
                    LOG_DEBUG ("    0 -> REPLACE\n");
                    LOG_DEBUG ("    1 -> PREPEND\n");

                    scanf ("%d", &val);
                    appl_param.modify_text = (UINT8)val;

                    appl_map_mce_xchg_size -= 3U;
                }
#endif /* MAP_1_4 */

                LOG_DEBUG ("Enter Message file name to be sent: ");
                scanf ("%s", msg_file);

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                BT_mem_set(mapc_file_object, 0, sizeof(mapc_file_object));
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)MAP_ROOT_FOLDER_BASE,
                    msg_file,
                    mapc_file_object
                );

                /* Get the sample listing from file */
                retval = BT_fops_file_open (mapc_file_object, (UCHAR *)"rb", &fp);
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

                if (remaining > appl_map_mce_xchg_size)
                {
                    body_req.length = appl_map_mce_xchg_size;
                    more = 0x01U;
                }
                else if (remaining != 0U)
                {
                    body_req.length = (UINT16 )remaining;

                    /*
                     * Send message in one packet only,
                     * and no continuation packet. The request will contain
                     * PUT opcode with FINAL bit set and BODY header
                     */
                    more = 0x00U;

                    /* or */

                    /*
                     * Send message in two packets.
                     * The first request will contain PUT opcode and BODY
                     * header and second request sent from the application
                     * callback handling for MAP_MCE_PUSH_MESSAGE_CNF will
                     * have PUT opcode with FINAL bit set and END OF BODY
                     * header
                     */
                    /* more = 0x01; */
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
                    (
                        body_req.value,
                        body_req.length,
                        fp,
                        &actual
                    );
                }

                set_info.body = &body_req;

                LOG_DEBUG ("Requesting to push message...\n");
                retval = BT_map_mce_push_message
                         (
                             &mas_instance[handle].instance.handle,
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

            case 30:
#ifdef MAP_SUPPORT_NOTIFICATION
                map_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
                BT_mem_set(&set_info, 0, sizeof(MAP_REQUEST_STRUCT));

                LOG_DEBUG ("Enter Notification Status (1-Enable, 0-Disable): ");
                scanf ("%d", &choice);

                appl_param.notification_status = (UINT8)choice;

                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_NOTIFICATIONSTATUS
                );

                /* Update request info */
                set_info.appl_params = & appl_param;

                LOG_DEBUG ("Requesting Notification Registration...\n");
                retval = BT_map_mce_set_ntf_registration
                         (
                             &mas_instance[handle].instance.handle,
                             &set_info
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
#else
                LOG_DEBUG ("MAP Notification NOT Enabled\n");
#endif /* MAP_SUPPORT_NOTIFICATION */
                break;

            case 31:
                map_print_appl_instances();

                LOG_DEBUG("Select Application Instance: ");
                scanf("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                LOG_DEBUG("Closing on MAP MCE Notification Instance %d\n", handle);
                retval = BT_map_mce_ns_transport_close(&mas_instance[handle].instance.handle);
                LOG_DEBUG("Retval - 0x%04X\n", retval);
                break;

            case 35:
                map_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                LOG_DEBUG ("Requesting Abort...\n");
                retval = BT_map_mce_abort
                         (
                             &mas_instance[handle].instance.handle
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 40:
#ifdef MAP_1_2
                map_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
                BT_mem_set(&get_info, 0, sizeof(MAP_REQUEST_STRUCT));

                LOG_DEBUG ("Enter MAS Instance : ");
                scanf ("%d", &choice);

                appl_param.mas_instance_id = (UINT8)choice;

                MAP_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_MASINSTANCEID
                );

                /* Update request info */
                get_info.appl_params = & appl_param;

                LOG_DEBUG("Enter number of wait stages: ");
                scanf("%d", &choice);
                wait_count = (UCHAR)choice;

                if (0U < wait_count)
                {
                    get_info.wait = BT_TRUE;
                    wait_count--;
                }
                else
                {
                    get_info.wait = BT_FALSE;
                }

                LOG_DEBUG ("Requesting MAS Instance Information...\n");
                retval = BT_map_mce_get_mas_instance_info
                         (
                             &mas_instance[handle].instance.handle,
                             &get_info
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
#else /* MAP_1_2 */
                LOG_DEBUG ("MAP_1_2 not defined\n");
#endif /* MAP_1_2 */
                break;

#ifdef MAP_1_3
            case 41: /* Get Converstation Listing */
                map_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
                BT_mem_set(&get_info, 0, sizeof(MAP_REQUEST_STRUCT));

                /* Update request info */
                get_info.appl_params = &appl_param;

                LOG_DEBUG("Enter number of wait stages: ");
                scanf("%d", &choice);
                wait_count = (UCHAR)choice;

                if (0U < wait_count)
                {
                    get_info.wait = BT_TRUE;
                    wait_count--;
                }
                else
                {
                    get_info.wait = BT_FALSE;
                }

                /* === Include Required Appl. Params === */
                LOG_DEBUG ("Include Appl Param 'MaxListCount' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_MAXLISTCOUNT
                    );

                    LOG_DEBUG ("Enter 'MaxListCount' value: ");
                    scanf ("%d", &val);
                    appl_param.max_list_count = (UINT16)val;
                }

                LOG_DEBUG ("Include Appl Param 'ListStartOffset' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_STARTOFFSET
                    );

                    LOG_DEBUG ("Enter 'ListStartOffset' value: ");
                    scanf ("%d", &val);
                    appl_param.start_offset = (UINT16)val;
                }

                LOG_DEBUG ("Include Appl Param 'FilterLastActivityBegin' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_FILTERPERIODBEGIN
                    );

                    LOG_DEBUG ("Enter Period Begin: \n");
                    LOG_DEBUG ("Example: 20141012T130510\n");
                    scanf ("%s", appl_period_begin);

                    appl_param.filter_period_begin.value = appl_period_begin;
                    appl_param.filter_period_begin.length = (UINT16)BT_str_n_len(appl_period_begin, sizeof(appl_period_begin));
                }

                LOG_DEBUG ("Include Appl Param 'FilterLastActivityEnd' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_FILTERPERIODEND
                    );

                    LOG_DEBUG ("Enter Period End: \n");
                    LOG_DEBUG ("Example: 20151012T130510\n");
                    scanf ("%s", appl_period_end);

                    appl_param.filter_period_end.value = appl_period_end;
                    appl_param.filter_period_end.length = (UINT16)BT_str_n_len(appl_period_end, sizeof(appl_period_end));
                }

                LOG_DEBUG ("Include Appl Param 'FilterReadStatus' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_FILTERREADSTATUS
                    );

                    LOG_DEBUG ("Enter 'FilterReadStatus' value:\n");
                    LOG_DEBUG ("    1 -> Unread\n");
                    LOG_DEBUG ("    2 -> Read\n");
                    scanf ("%d", &val);
                    appl_param.filter_read_status = (UCHAR)val;
                }

                LOG_DEBUG ("Include Appl Param 'FilterRecipient' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_FILTERRECIPIENT
                    );

                    LOG_DEBUG ("Enter Recipient:\n");
                    LOG_DEBUG ("Example: EtherMind\n");
                    scanf ("%s", appl_recipient);

                    appl_param.filter_recipient.value = appl_recipient;
                    appl_param.filter_recipient.length = (UINT16)BT_str_n_len(appl_recipient, sizeof(appl_recipient));
                }

                LOG_DEBUG ("Include Appl Param 'ConversationID' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG_EXT
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_CONVERSATION_ID,
                        1U
                    );

                    LOG_DEBUG ("Enter ConvID value: \n");
                    LOG_DEBUG ("Example: 1234567890123456789ABCDEF1234567\n");
                    scanf("%s", appl_conversation_id);

                    appl_param.conversation_id.value = appl_conversation_id;
                    appl_param.conversation_id.length = (UINT16)BT_str_n_len(appl_conversation_id, sizeof(appl_conversation_id));
                }

                LOG_DEBUG ("Include Appl Param 'ConvParameterMask' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG_EXT
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_CONV_PARAMETER_MASK,
                        1U
                    );

                    LOG_DEBUG ("ConvParameterMask Bit Mask\n");
                    LOG_DEBUG (" bit  0 -> Conversation name\n");
                    LOG_DEBUG (" bit  1 -> Conversation last_activity\n");
                    LOG_DEBUG (" bit  2 -> Conversation read_status\n");
                    LOG_DEBUG (" bit  3 -> Conversation version_counter\n");
                    LOG_DEBUG (" bit  4 -> Conversattin Summary\n");
                    LOG_DEBUG (" bit  5 -> Participants\n");
                    LOG_DEBUG (" bit  6 -> Participant UCI\n");
                    LOG_DEBUG (" bit  7 -> Participant display_name\n");
                    LOG_DEBUG (" bit  8 -> Participant chat_state\n");
                    LOG_DEBUG (" bit  9 -> Participant last_activity\n");
                    LOG_DEBUG (" bit 10 -> Participant x_bt_uid\n");
                    LOG_DEBUG (" bit 11 -> Participant name\n");
                    LOG_DEBUG (" bit 12 -> Participant presence_availability\n");
                    LOG_DEBUG (" bit 13 -> Participant presence_text\n");
                    LOG_DEBUG (" bit 14 -> Participant priority\n\n");

                    LOG_DEBUG ("Enter 'ConvParameterMask' value(in Hex): ");
                    scanf ("%x", &val);
                    appl_param.conv_parameter_mask = (UINT32)val;
                }

                LOG_DEBUG ("Requesting to get IM coversation listing...\n");
                retval = BT_map_mce_get_conversation_listing
                         (
                             &mas_instance[handle].instance.handle,
                             &get_info
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 42: /* Set Owner Status */
                map_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
                BT_mem_set(&set_info, 0, sizeof(MAP_REQUEST_STRUCT));

                /* Update request info */
                set_info.appl_params = &appl_param;

                /* === Include Required Appl. Params === */
                LOG_DEBUG ("Include Appl Param 'PresenceAvailability' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_PRESENCE_AVAILABILITY
                    );

                    LOG_DEBUG ("Enter 'PresenceAvailability' value\n");
                    LOG_DEBUG ("    0 -> Unknown\n");
                    LOG_DEBUG ("    1 -> Offline\n");
                    LOG_DEBUG ("    2 -> Online\n");
                    LOG_DEBUG ("    3 -> Away\n");
                    LOG_DEBUG ("    4 -> Do not disturb\n");
                    LOG_DEBUG ("    5 -> Busy\n");
                    LOG_DEBUG ("    6 -> In a meeting\n");
                    scanf ("%d", &val);
                    appl_param.presence_availability = (UINT8)val;
                }

                LOG_DEBUG ("Include Appl Param 'PresenceText' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_PRESENCE_TEXT
                    );

                    LOG_DEBUG ("Enter Presense Text: \n");
                    LOG_DEBUG ("Example: @Bengaluru\n");
                    scanf ("%s", appl_presence_text);

                    appl_param.presence_text.value = appl_presence_text;
                    appl_param.presence_text.length = (UINT16)BT_str_n_len(appl_presence_text, sizeof(appl_presence_text));
                }

                LOG_DEBUG ("Include Appl Param 'LastActivity' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_LAST_ACTIVITY
                    );

                    LOG_DEBUG ("Enter LastActivity: \n");
                    LOG_DEBUG ("Example: 20151012T130510\n");
                    scanf ("%s", appl_last_activity);

                    appl_param.last_activity.value = appl_last_activity;
                    appl_param.last_activity.length = (UINT16)BT_str_n_len(appl_last_activity, sizeof(appl_last_activity));
                }

                LOG_DEBUG ("Include Appl Param 'ChatState' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG_EXT
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_CHAT_STATE,
                        1U
                    );

                    LOG_DEBUG ("Enter 'ChatState' value\n");
                    LOG_DEBUG ("    0 -> Unknown\n");
                    LOG_DEBUG ("    1 -> Inactive\n");
                    LOG_DEBUG ("    2 -> Active\n");
                    LOG_DEBUG ("    3 -> Composing\n");
                    LOG_DEBUG ("    4 -> Paused compsing\n");
                    LOG_DEBUG ("    5 -> Gone\n");

                    scanf ("%d", &val);
                    appl_param.chat_state = (UINT8)val;
                }

                LOG_DEBUG ("Include Appl Param 'ConversationID' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG_EXT
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_CONVERSATION_ID,
                        1U
                    );

                    LOG_DEBUG ("Enter Converstation ID :");
                    scanf ("%s", convs_id);

                    temp = (UINT16)BT_str_n_len(convs_id, sizeof(convs_id));

                    appl_param.conversation_id.value = BT_alloc_mem(temp);
                    if (NULL == appl_param.conversation_id.value)
                    {
                        LOG_DEBUG ("Memory allocation failed for Conversation ID\n");
                        break;
                    }

                    BT_mem_copy
                    (
                        appl_param.conversation_id.value,
                        convs_id,
                        temp
                    );

                    appl_param.conversation_id.length = temp;
                }

                LOG_DEBUG ("Requesting to set owner status...\n");
                retval = BT_map_mce_set_owner_status
                         (
                             &mas_instance[handle].instance.handle,
                             &set_info
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 43: /* Get Owner status */
                map_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
                BT_mem_set(&get_info, 0, sizeof(MAP_REQUEST_STRUCT));

                MAP_SET_APPL_PARAM_FLAG_EXT
                (
                    appl_param.appl_param_flag,
                    MAP_FLAG_CONVERSATION_ID,
                    1U
                );

                LOG_DEBUG("Enter number of wait stages: ");
                scanf("%d", &choice);
                wait_count = (UCHAR)choice;

                if (0U < wait_count)
                {
                    get_info.wait = BT_TRUE;
                    wait_count--;
                }
                else
                {
                    get_info.wait = BT_FALSE;
                }

                LOG_DEBUG ("Enter Converstation ID:");
                scanf ("%s", convs_id);

                temp = (UINT16)BT_str_n_len(convs_id, sizeof(convs_id));

                appl_param.conversation_id.value = BT_alloc_mem(temp);
                if (NULL == appl_param.conversation_id.value)
                {
                    LOG_DEBUG ("Memory allocation failed for Conversation ID\n");
                    break;
                }

                BT_mem_copy
                (
                    appl_param.conversation_id.value,
                    convs_id,
                    temp
                );
                appl_param.conversation_id.length = temp;

                /* Update request info */
                get_info.appl_params = &appl_param;

                LOG_DEBUG ("Requesting Get Owner Status...\n");
                retval = BT_map_mce_get_owner_status
                         (
                             &mas_instance[handle].instance.handle,
                             &get_info
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 44: /* Set Notification filter */
                map_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (MAP_CLIENT_NUM_INSTANCES <= handle)
                {
                    printf ("Invalid Application Instance\n");
                    break;
                }

                MAP_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);
                BT_mem_set(&get_info, 0, sizeof(MAP_REQUEST_STRUCT));

                /* Update request info */
                set_info.appl_params = &appl_param;

                /* === Include Required Appl. Params === */
                LOG_DEBUG ("Include Appl Param 'NotificationFilterMask' (1/0): ");
                scanf ("%d", &choice);
                if (0 != choice)
                {
                    MAP_SET_APPL_PARAM_FLAG_EXT
                    (
                        appl_param.appl_param_flag,
                        MAP_FLAG_NOTIFICATION_FILTER_MASK,
                        1U
                    );

                    LOG_DEBUG ("Notification Bit Mask\n");
                    LOG_DEBUG (" bit  0 -> NewMessage\n");
                    LOG_DEBUG (" bit  1 -> MessageDeleted\n");
                    LOG_DEBUG (" bit  2 -> MessageShift\n");
                    LOG_DEBUG (" bit  3 -> SendingSuccess\n");
                    LOG_DEBUG (" bit  4 -> SendingFailure\n");
                    LOG_DEBUG (" bit  5 -> DeliverySuccess\n");
                    LOG_DEBUG (" bit  6 -> DeliveryFailure\n");
                    LOG_DEBUG (" bit  7 -> MemoryFull\n");
                    LOG_DEBUG (" bit  8 -> MemoryAvailable\n");
                    LOG_DEBUG (" bit  9 -> ReadStatusChanged\n");
                    LOG_DEBUG (" bit 10 -> ConversationChanged\n");
                    LOG_DEBUG (" bit 11 -> ParticipantPresenceChanged\n");
                    LOG_DEBUG (" bit 12 -> ParticipantChatStateChanged\n");
                    LOG_DEBUG (" bit 13 -> MessageExtendedDataChanged\n");
                    LOG_DEBUG (" bit 14 -> MessageRemoved\n\n");
                    LOG_DEBUG ("Enter NotificationFilterMask value(in Hex): ");
                    scanf ("%x", &val);
                    appl_param.ntf_filter_mask = (UINT32)val;
                }

                LOG_DEBUG ("Requesting to set notification filter mask...\n");
                retval = BT_map_mce_set_notification_filter
                         (
                             &mas_instance[handle].instance.handle,
                             &set_info
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;
#else /* MAP_1_3 */
                LOG_DEBUG ("MAP_1_3 Feature Flag is Enabled\n");
#endif /* MAP_1_3 */

            case 50:
                LOG_DEBUG("Enter the path: ");
                scanf("%s", path);
                (void)BT_fops_list_directory(path);
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


API_RESULT appl_map_mce_callback
           (
               MAP_HANDLE_INFO * map_handle,
               UINT8             event_type,
               UINT16            event_result,
               MAP_HEADERS     * event_header,
               UINT16            event_hdrlen
           )
{
    API_RESULT          retval;
    MAP_APPL_PARAMS    *appl_param;
    UCHAR               send_request;
    UINT16              i, j;
    MAP_HANDLE          handle;

#ifdef MAP_SUPPORT_NOTIFICATION
    UCHAR               send_response;
#endif /* MAP_SUPPORT_NOTIFICATION */

    UINT16 length;
    UCHAR * data;

    /* XML_PARSER_OBJECT     xml_obj; */
    MAP_HEADERS           map_header;
    MAP_HEADER_STRUCT     body_req;
    MAP_REQUEST_STRUCT    req_info;
    MAP_RESPONSE_STRUCT   rsp_info;
    UCHAR                 more;
    UINT16                actual;

    BT_IGNORE_UNUSED_PARAM(event_hdrlen);

    LOG_DEBUG ("\n\nMAP MCE Event: 0x%02X Result: 0x%04X\n\n", event_type, event_result);

    /* Init */
    retval = API_SUCCESS;
    send_request = 0U;
    BT_mem_set(&map_header, 0, sizeof(MAP_HEADERS));
    BT_mem_set(&req_info, 0, sizeof(MAP_REQUEST_STRUCT));
    BT_mem_set(&rsp_info, 0, sizeof(MAP_RESPONSE_STRUCT));

    MAP_INIT_HEADER_STRUCT (body_req);

#ifdef MAP_SUPPORT_NOTIFICATION
    send_response = 0U;
#endif /* MAP_SUPPORT_NOTIFICATION */

    more = 0x00U;

    handle = map_handle->entity_id;

    switch (event_type)
    {
    case MAP_MCE_CONNECT_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_CONNECT_CNF - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);

        LOG_DEBUG ("MSE BD Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
        event_header->map_connect_info->bd_addr[0U],
        event_header->map_connect_info->bd_addr[1U],
        event_header->map_connect_info->bd_addr[2U],
        event_header->map_connect_info->bd_addr[3U],
        event_header->map_connect_info->bd_addr[4U],
        event_header->map_connect_info->bd_addr[5U]);

        /* TODO: Have to get server channel local/remote info */
        LOG_DEBUG ("MAX Exchange Size: %d\n",
            event_header->map_connect_info->max_recv_size);

        map_xchg_size = event_header->map_connect_info->max_recv_size;

        if (MAP_SUCCESS_RSP == event_result)
        {
            /* Get the Handle parameters */
            for (i = 0U; i < MAP_CLIENT_NUM_INSTANCES; i++)
            {
                if (mas_instance[i].instance.handle == handle)
                {
                    BT_mem_copy
                    (
                        mas_instance[i].instance.bd_addr,
                        event_header->map_connect_info->bd_addr,
                        BT_BD_ADDR_SIZE
                    );

                    break;
                }
            }

            if (MAP_CLIENT_NUM_INSTANCES != i)
            {
                /* Get the Handle parameters */
                for (j = 0U; j < MAP_CLIENT_NUM_INSTANCES; j++)
                {
                    if (0 == BT_mem_cmp(mns_instance[j].bd_addr,
                        event_header->map_connect_info->bd_addr,
                        BT_BD_ADDR_SIZE))
                    {
                        mas_instance[i].mns_id = (UCHAR)j;
                        break;
                    }
                }
            }

            map_print_appl_instances();

            /* Reset */
            appl_map_mce_start_of_response = BT_FALSE;
        }
        break;

    case MAP_MCE_UPDATE_INBOX_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_UPDATE_INBOX_CNF - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);
        break;

    case MAP_MCE_SET_FOLDER_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_SET_FOLDER_CNF - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);
        break;

#ifdef MAP_1_2
    case MAP_MCE_GET_MASINSTINFO_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_GET_MASINSTINFO_CNF - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);

        /* Get MAS Instance Information */
        data = event_header->map_resp_info->body->value;
        length = event_header->map_resp_info->body->length;

        if (0U != length)
        {
            LOG_DEBUG ("MAS Instance Info: %s\n", data);
        }
        else
        {
            LOG_DEBUG ("MAS Instance Info not received\n");
        }
        break;
#endif /* MAP_1_2 */

#ifdef MAP_1_3
    case MAP_MCE_GET_CONVERSATION_LIST_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_GET_CONVERSATION_LIST_CNF - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);

        if ((MAP_SUCCESS_RSP != event_result) &&
            (MAP_CONTINUE_RSP != event_result))
        {
            if (NULL != map_list_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (map_list_fp);
                map_list_fp = NULL;
            }

            /* Reset */
            appl_map_mce_start_of_response = BT_FALSE;

            break;
        }

        if (MAP_CONTINUE_RSP == event_result)
        {
            send_request = 1U;
        }

        appl_param = event_header->map_resp_info->appl_params;

        if (BT_TRUE != appl_map_mce_start_of_response)
        {
            /* Reset */
            appl_map_mce_start_of_response = BT_TRUE;

            if (appl_param != NULL)
            {
                /* print the appl. params */
                if (0U != (MAP_GET_APPL_PARAM_FLAG
                (
                    appl_param->appl_param_flag,
                    MAP_FLAG_CONV_LIST_VER_CNTR
                )))
                {

                    LOG_DEBUG("Conversation List Version Counter:\n");
                    for (i = 0U; i < appl_param->conv_listing_ver_cntr.length; i++)
                    {
                        LOG_DEBUG("%02X", appl_param->conv_listing_ver_cntr.value[i]);
                    }
                    LOG_DEBUG("\n");
                }

                if (0U != (MAP_GET_APPL_PARAM_FLAG
                    (
                        appl_param->appl_param_flag,
                        MAP_FLAG_MESSAGESLISTINGSIZE
                    )))
                {
                    LOG_DEBUG("Listing Size: %02X\n", appl_param->messages_listing_size);
                }

                if (0U != (MAP_GET_APPL_PARAM_FLAG
                    (
                        appl_param->appl_param_flag,
                        MAP_FLAG_DATABASE_IDENTIFIER
                    )))
                {
                    LOG_DEBUG("DatabaseIdentifer: \n");
                    for (i = 0U; i < appl_param->database_identifier.length; i++)
                    {
                        LOG_DEBUG("%02X", appl_param->database_identifier.value[i]);
                    }
                    LOG_DEBUG("\n");
                }

                if (0U != (MAP_GET_APPL_PARAM_FLAG
                    (
                        appl_param->appl_param_flag,
                        MAP_FLAG_MSETIME
                    )))
                {
                    LOG_DEBUG("MSE Time: \n");
                    LOG_DEBUG("%s\n", appl_param->mse_time.value);
                }
            }
            else
            {
                LOG_DEBUG("Appl Param NULL received\n");
            }
        }

        /* Get Body content - Messages Listing Object */
        if ((event_header->map_resp_info->body == NULL) ||
            (event_header->map_resp_info->body->value == NULL) ||
            (event_header->map_resp_info->body->length == 0U))
        {
            LOG_DEBUG ("No Body Content found\n");
        }
        else
        {
            /* Get MAP Conversation Listing Information */
            data = event_header->map_resp_info->body->value;
            length = event_header->map_resp_info->body->length;

            if ((NULL != data) && (0U != length))
            {
                LOG_DEBUG ("\n\n==== MAP Conversation Listing ====\n");
                for (i = 0U; i < length; i++)
                {
                    LOG_DEBUG ("%c", data[i]);
                }
                LOG_DEBUG ("\n====================================\n");

                if (NULL == map_list_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    BT_mem_set(mapc_file_object, 0, sizeof(mapc_file_object));
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)MAP_ROOT_FOLDER_BASE,
                        (UCHAR *)APPL_MAPC_CONV_LISTING_FILE,
                        mapc_file_object
                    );

                    retval = BT_fops_file_open
                             (
                                 mapc_file_object,
                                 (UCHAR *)"wb",
                                 &map_list_fp
                             );
                    if ((API_SUCCESS != retval) || (NULL == map_list_fp))
                    {
                        LOG_DEBUG ("Failed to open Conversation Listing file\n");
                    }
                }

                if (NULL != map_list_fp)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, map_list_fp, &actual);
                }
            }
        }

        if (MAP_SUCCESS_RSP == event_result)
        {
            appl_map_mce_start_of_response = BT_FALSE;

            if(NULL != map_list_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (map_list_fp);
                map_list_fp = NULL;
            }
        };

        break;

    case MAP_MCE_SET_OWNER_STATUS_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_SET_OWNER_STATUS_CNF - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);
        break;

    case MAP_MCE_GET_OWNER_STATUS_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_GET_OWNER_STATUS_CNF - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);

        /* Received application parameters */
        appl_param = event_header->map_resp_info->appl_params;

        if (NULL != appl_param)
        {
            /* print the appl. params */
            if(0U != (MAP_GET_APPL_PARAM_FLAG
                 (
                     appl_param->appl_param_flag,
                     MAP_FLAG_PRESENCE_AVAILABILITY
                 )))
            {
                LOG_DEBUG ("Presence Avail: ");
                if (MAP_IM_PRESENCE_OFFLINE == appl_param->presence_availability)
                {
                    LOG_DEBUG ("Offline\n");
                }
                else if (MAP_IM_PRESENCE_ONLINE == appl_param->presence_availability)
                {
                    LOG_DEBUG ("Online\n");
                }
                else if (MAP_IM_PRESENCE_AWAY == appl_param->presence_availability)
                {
                    LOG_DEBUG ("Away\n");
                }
                else if (MAP_IM_PRESENCE_DO_NOT_DISTURB == appl_param->presence_availability)
                {
                    LOG_DEBUG ("Do not distrub\n");
                }
                else if (MAP_IM_PRESENCE_BUSY == appl_param->presence_availability)
                {
                    LOG_DEBUG ("Busy\n");
                }
                else if (MAP_IM_PRESENCE_IN_MEETING == appl_param->presence_availability)
                {
                    LOG_DEBUG ("In a meeting\n");
                }
                else if (MAP_IM_PRESENCE_UNKNOWN == appl_param->presence_availability)
                {
                    LOG_DEBUG ("Unknown\n");
                }
                else
                {
                    LOG_DEBUG ("???\n");
                }
            }

            if(0U != (MAP_GET_APPL_PARAM_FLAG
                (
                    appl_param->appl_param_flag,
                    MAP_FLAG_PRESENCE_TEXT
                 )))
            {
                LOG_DEBUG ("Presence Text:");
                for (i = 0U; i < appl_param->presence_text.length; i++)
                {
                    LOG_DEBUG ("%c", appl_param->presence_text.value[i]);
                }
                LOG_DEBUG ("\n");
            }

            if(0U != (MAP_GET_APPL_PARAM_FLAG_EXT
                (
                    appl_param->appl_param_flag,
                    MAP_FLAG_CHAT_STATE,
                    1U
                 )))
            {
                LOG_DEBUG ("Chat State: ");
                if(MAP_IM_CHAT_STATE_INACTIVE == appl_param->chat_state)
                {
                    LOG_DEBUG ("Inactive\n");
                }
                else if(MAP_IM_CHAT_STATE_ACTIVE == appl_param->chat_state)
                {
                    LOG_DEBUG ("Active\n");
                }
                else if(MAP_IM_CHAT_STATE_COMPOSING == appl_param->chat_state)
                {
                    LOG_DEBUG ("Composing\n");
                }
                else if(MAP_IM_CHAT_STATE_PAUSED_COMPOSING == appl_param->chat_state)
                {
                    LOG_DEBUG ("Paused composing\n");
                }
                else if(MAP_IM_CHAT_STATE_GONE == appl_param->chat_state)
                {
                    LOG_DEBUG ("Gone\n");
                }
                else if(MAP_IM_CHAT_STATE_UNKNOWN == appl_param->chat_state)
                {
                    LOG_DEBUG ("Unknown\n");
                }
                else
                {
                    LOG_DEBUG ("???\n");
                }
            }
        }
        else
        {
            LOG_DEBUG ("Appl Param NULL received\n");
        }
        break;

    case MAP_MCE_SET_NOTIFICATION_FILTER_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_SET_NOTIFICATION_FILTER_CNF - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);
        break;
#endif /* MAP_1_3 */

    case MAP_MCE_GET_FOLDER_LISTING_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_GET_FOLDER_LISTING_CNF - 0x%04X\n",
        event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);

        if ((MAP_SUCCESS_RSP != event_result) &&
            (MAP_CONTINUE_RSP != event_result))
        {
            if (NULL != map_list_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (map_list_fp);
                map_list_fp = NULL;
            }

            /* Reset */
            appl_map_mce_start_of_response = BT_FALSE;

            break;
        }

        if (MAP_CONTINUE_RSP == event_result)
        {
            send_request = 1U;
        }

        appl_param = event_header->map_resp_info->appl_params;

        if (BT_TRUE != appl_map_mce_start_of_response)
        {
            /* Reset */
            appl_map_mce_start_of_response = BT_TRUE;

            if (appl_param != NULL)
            {
                /* Get Folder listing size */
                if (0U != (MAP_GET_APPL_PARAM_FLAG
                (
                    appl_param->appl_param_flag,
                    MAP_FLAG_FOLDERLISTINGSIZE
                )))
                {
                    LOG_DEBUG("Folder Listing Size: %d\n",
                        appl_param->folder_listing_size);
                }
                else
                {
                    LOG_DEBUG("Folder Listing Size not received\n");
                }
            }
            else
            {
                LOG_DEBUG("Appl Param NULL received\n");
            }
        }

        /* Get Body content - Folder Listing Object */
        if ((event_header->map_resp_info->body == NULL) ||
            (event_header->map_resp_info->body->value == NULL) ||
            (event_header->map_resp_info->body->length == 0U))
        {
            LOG_DEBUG ("No Body Content found\n");
        }
        else
        {
            data = event_header->map_resp_info->body->value;
            length = event_header->map_resp_info->body->length;

#if 0
            LOG_DEBUG ("\n\n================= BODY PACKET =================\n");

            /* Dump the Body stream */
            for (i = 0; i < length; i++)
            {
                LOG_DEBUG ("%02X ", data[i]);
            }

#else
            LOG_DEBUG ("\n================= BODY PACKET =================\n");

            LOG_DEBUG ("\n========== MSE Current Folder Listing ==========\n");

            /* Print the stream */
            for (i = 0U; i < length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }

            LOG_DEBUG ("\n================================================\n");

            if (NULL == map_list_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                BT_mem_set(mapc_file_object, 0, sizeof(mapc_file_object));
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)MAP_ROOT_FOLDER_BASE,
                    (UCHAR *)APPL_MAPC_FOLDER_LISTING_FILE,
                    mapc_file_object
                );

                retval = BT_fops_file_open
                         (
                             mapc_file_object,
                             (UCHAR *)"wb",
                             &map_list_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == map_list_fp))
                {
                    LOG_DEBUG ("Failed to open Folder Listing file\n");
                }
            }

            if (NULL != map_list_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, map_list_fp, &actual);
            }
#endif /* 0 */
        }

        if (MAP_SUCCESS_RSP == event_result)
        {
            appl_map_mce_start_of_response = BT_FALSE;

            if (NULL != map_list_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (map_list_fp);
                map_list_fp = NULL;
            }

            BT_mem_set(xml_obj.file, 0x00, sizeof(xml_obj.file));
            BT_str_n_copy(xml_obj.file, mapc_file_object,sizeof(mapc_file_object)-1);
            xml_obj.use_buffer = BT_FALSE;
            xml_obj.pStart = NULL;

            retval = xml_parser(&xml_obj);

            if ((API_SUCCESS != retval) &&
                (NULL != xml_obj.pStart))
            {
                LOG_DEBUG(" ERR ! DOCUMENT NOT VALID!! \n" );

                xml_free_parse_struct(xml_obj.pStart);

                retval = API_FAILURE; /* return API_FAILURE; */
            }
            else if(NULL == xml_obj.pStart)
            {
                LOG_DEBUG(" ERR ! DOCUMENT NOT VALID!! \n" );
                retval = API_FAILURE; /* return API_FAILURE; */
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            if (API_FAILURE != retval)
            {
                map_list_index = 1U;

                LOG_DEBUG("\n\n=================== MSE Current Folder Listing ===================\n");
                appl_map_mce_print_folder_list(xml_obj.pStart);
                LOG_DEBUG("\n----------------------------------------------------------------------\n");

                xml_free_parse_struct(xml_obj.pStart);
            }
        }
        break;

    case MAP_MCE_GET_MESSAGES_LISTING_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_GET_MESSAGES_LISTING_CNF - 0x%04X\n",
        event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);

        if ((MAP_SUCCESS_RSP != event_result) &&
            (MAP_CONTINUE_RSP != event_result))
        {
            if (NULL != map_list_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (map_list_fp);
                map_list_fp = NULL;
            }

            /* Reset */
            appl_map_mce_start_of_response = BT_FALSE;

            break;
        }

        if (MAP_CONTINUE_RSP == event_result)
        {
            send_request = 1U;
        }

        appl_param = event_header->map_resp_info->appl_params;

        if (BT_TRUE != appl_map_mce_start_of_response)
        {
            appl_map_mce_start_of_response = BT_TRUE;

            if (appl_param != NULL)
            {
                /* Get New Message indicator */
                if (0U != (MAP_GET_APPL_PARAM_FLAG
                    (
                        appl_param->appl_param_flag,
                        MAP_FLAG_NEWMESSAGE
                    )))
                {
                    LOG_DEBUG("New Message Indicator: %d\n",
                        appl_param->new_message);
                }
                else
                {
                    LOG_DEBUG("New Message Indicator not received\n");
                }

                /* Get MSE Time */
                if (0U != (MAP_GET_APPL_PARAM_FLAG
                    (
                        appl_param->appl_param_flag,
                        MAP_FLAG_MSETIME
                    )))
                {
                    LOG_DEBUG("MSE Time: %s\n",
                        appl_param->mse_time.value);
                }
                else
                {
                    LOG_DEBUG("MSE Time not received\n");
                }

                /* Get Messages Listing Size */
                if (0U != (MAP_GET_APPL_PARAM_FLAG
                    (
                        appl_param->appl_param_flag,
                        MAP_FLAG_MESSAGESLISTINGSIZE
                    )))
                {
                    LOG_DEBUG("Messages Listing Size: %d\n",
                        appl_param->messages_listing_size);
                }
                else
                {
                    LOG_DEBUG("Messages Listing Size not received\n");
                }
            }
            else
            {
                LOG_DEBUG("Appl Param NULL received\n");
            }
        }

        /* Get Body content - Messages Listing Object */
        if ((event_header->map_resp_info->body == NULL) ||
            (event_header->map_resp_info->body->value == NULL) ||
            (event_header->map_resp_info->body->length == 0U))
        {
            LOG_DEBUG ("No Body Content found\n");
        }
        else
        {
            data = event_header->map_resp_info->body->value;
            length = event_header->map_resp_info->body->length;

#if 0
            LOG_DEBUG ("\n\n================= BODY PACKET =================\n");

            /* Dump the Body stream */
            for (i = 0; i < length; i++)
            {
                LOG_DEBUG ("%02X ", data[i]);
            }

#else
            LOG_DEBUG ("\n================= BODY PACKET =================\n");

            LOG_DEBUG ("\n========== MSE Current Message Listing ==========\n");

            /* Print the stream */
            for (i = 0U; i < length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }

            LOG_DEBUG ("\n================================================\n");

            if (NULL == map_list_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                BT_mem_set(mapc_file_object, 0, sizeof(mapc_file_object));
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)MAP_ROOT_FOLDER_BASE,
                    (UCHAR *)APPL_MAPC_MSG_LISTING_FILE,
                    mapc_file_object
                );

                retval = BT_fops_file_open
                         (
                             mapc_file_object,
                             (UCHAR *)"wb",
                             &map_list_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == map_list_fp))
                {
                    LOG_DEBUG ("Failed to open Messages Listing file\n");
                }
            }

            if (NULL != map_list_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, map_list_fp, &actual);
            }
#endif /* 0 */
        }

        if (MAP_SUCCESS_RSP == event_result)
        {
            appl_map_mce_start_of_response = BT_FALSE;

            if (NULL != map_list_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (map_list_fp);
                map_list_fp = NULL;
            }

#if 0
            BT_str_copy(xml_obj.file, mapc_file_object);
            xml_obj.use_buffer = BT_FALSE;
            xml_obj.pStart = NULL;

            retval = xml_parser(&xml_obj);

            if ((API_SUCCESS != retval) &&
                (NULL != xml_obj.pStart))
            {
                LOG_DEBUG(" ERR ! DOCUMENT NOT VALID!! \n");

                xml_free_parse_struct(xml_obj.pStart);

                retval = API_FAILURE; /* return API_FAILURE; */
            }
            else if (NULL == xml_obj.pStart)
            {
                LOG_DEBUG(" ERR ! DOCUMENT NOT VALID!! \n");
                retval = API_FAILURE; /* return API_FAILURE; */
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            if (API_FAILURE != retval)
            {
                map_list_index = 1;

                LOG_DEBUG ("\n\n=================== MSE Current Message Listing ======================\n");
                LOG_DEBUG ("======================================================================\n");
                LOG_DEBUG ("No. %-17s%-20s%-10s%-09s%-04s%-04s%-04s\n",
                "Handle", "Subject", "Type", "Size", "Rd", "St", "Pd");
                LOG_DEBUG ("\n----------------------------------------------------------------------\n");
                appl_map_mce_print_message_list (xml_obj.pStart);
                LOG_DEBUG ("\n----------------------------------------------------------------------\n");

                xml_free_parse_struct(xml_obj.pStart);
            }
#endif /* 0 */
        }
        break;

    case MAP_MCE_GET_MESSAGE_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_GET_MESSAGE_CNF - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);

        if ((MAP_SUCCESS_RSP != event_result) &&
            (MAP_CONTINUE_RSP != event_result))
        {
            /* Reset */
            appl_map_mce_start_of_response = BT_FALSE;

            break;
        }

        if (MAP_CONTINUE_RSP == event_result)
        {
            send_request = 1U;
        }

        appl_param = event_header->map_resp_info->appl_params;

        if (BT_TRUE != appl_map_mce_start_of_response)
        {
            appl_map_mce_start_of_response = BT_TRUE;

            if (appl_param != NULL)
            {
                /* Get Fraction Deliver */
                if (0U != (MAP_GET_APPL_PARAM_FLAG
                (
                    appl_param->appl_param_flag,
                    MAP_FLAG_FRACTIONDELIVER
                )))
                {
                    LOG_DEBUG("Fraction Deliver: %d\n",
                        appl_param->fraction_deliver);

                    if (!appl_param->fraction_deliver)
                    {
                        appl_param->fraction_request = 0x01U;

                        MAP_SET_APPL_PARAM_FLAG
                        (
                            appl_param->appl_param_flag,
                            MAP_FLAG_FRACTIONREQUEST
                        );
                    }
                }
                else
                {
                    LOG_DEBUG("Fraction Deliver not received\n");
                }
            }
            else
            {
                LOG_DEBUG("Appl Param NULL received\n");
            }
        }

        /* Get Body content - bMessage Object */
        if ((event_header->map_resp_info->body->value == NULL) ||
            (event_header->map_resp_info->body->length == 0U))
        {
            LOG_DEBUG ("No Body Content found\n");
        }
        else
        {
            data = event_header->map_resp_info->body->value;
            length = event_header->map_resp_info->body->length;

            LOG_DEBUG ("\n================= BODY PACKET =================\n");
            LOG_DEBUG ("\n========== MSE Get Message ==========\n");

            /* Print the stream */
            for (i = 0U; i < length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }

            LOG_DEBUG ("\n======================================\n");

            /* Open file to dump raw message */
            if (NULL == map_raw_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                BT_mem_set(mapc_file_object, 0, sizeof(mapc_file_object));
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)MAP_ROOT_FOLDER_BASE,
                    (UCHAR *)APPL_MAPC_MSG_FILE,
                    mapc_file_object
                );

                retval = BT_fops_file_open
                         (
                             mapc_file_object,
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
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, map_raw_fp, &actual);
            }

#ifdef HAVE_OBJECT_PARSER
            /* Open file to dump parsed message */
            if (NULL == map_parsed_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                BT_mem_set(mapc_file_object, 0, sizeof(mapc_file_object));
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)MAP_ROOT_FOLDER_BASE,
                    (UCHAR *)APPL_MAPC_MSG_FILE,
                    mapc_file_object
                );

                retval = BT_fops_file_open
                         (
                             mapc_file_object,
                             (UCHAR *)"wb",
                             &map_parsed_fp
                         );

                if ((API_SUCCESS != retval) || (NULL == map_parsed_fp))
                {
                    LOG_DEBUG ("Failed to open Message file for Parsing\n");
                }
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

            if (OBJECT_RESULT_PARTIAL_PARSED == retval)
            {
                BT_mem_move
                (
                    map_parse_buffer,
                    &map_parse_buffer[map_context.total_parsed_length],
                    (map_context.buffer_len - map_context.total_parsed_length)
                );

                map_parser_diff_len = map_context.buffer_len - map_context.
                    total_parsed_length;
            }
#endif /* HAVE_OBJECT_PARSER */
        }

        /* Close the file if message received fully */
        if (MAP_SUCCESS_RSP == event_result)
        {
            appl_map_mce_start_of_response = BT_FALSE;

            if (NULL != map_raw_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (map_raw_fp);
                map_raw_fp = NULL;
            }

#ifdef HAVE_OBJECT_PARSER
            if (NULL != map_parsed_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (map_parsed_fp);
                map_parsed_fp = NULL;
            }

            LOG_DEBUG ("\n=================== Received Message Object ==================\n\n");
            LOG_DEBUG ("VERSION: %s\n", map_msg_object.property.version);
            LOG_DEBUG ("STATUS: %s\n",
                (map_msg_object.property.status == READ)? "READ": "UNREAD");
            LOG_DEBUG ("TYPE: %x\n", map_msg_object.property.type);
            LOG_DEBUG ("FOLDER: %s\n", map_msg_object.property.folder);

            LOG_DEBUG ("ENCODING: %x\n", map_msg_object.envelope.content.property.encoding);
            LOG_DEBUG ("CHARSET: %x\n", map_msg_object.envelope.content.property.charset);
            LOG_DEBUG ("LANGUAGE: %s\n", map_msg_object.envelope.content.property.language);
            LOG_DEBUG ("CONTENT LENGTH: %d\n", map_msg_object.envelope.content.property.content_length);

            LOG_DEBUG ("ORIGINATOR: %s\n",
            (map_msg_object.originator.property->param[0U].param_value[0U] == '\0')?
            (UCHAR *)"UNKNOWN":
            map_msg_object.originator.property->param[0U].param_value);

            for (i = 0U; i < map_msg_object.num_envelopes; i++)
            {
                LOG_DEBUG ("RECEPIENT %d: %s\n", i,
                (map_msg_object.envelope.recepient[i].
                property[0U].param[0U].param_value[0U] == '\0')? (UCHAR *)"UNKNOWN":
                map_msg_object.envelope.recepient[i].
                property[0U].param[0U].param_value);
            }

            if (map_msg_object.envelope.content.message != NULL)
            {
                LOG_DEBUG ("MESSAGE:\n%s\n\n", map_msg_object.envelope.content.message);
                BT_free_mem(map_msg_object.envelope.content.message);
                map_msg_object.envelope.content.message = NULL;

            }
            LOG_DEBUG ("\n=================== Received Message Object ==================\n");

            LOG_DEBUG ("Reinitializing MAP Parser...\n");
            retval = object_parser_init (&map_context, map_bmsg_parser_cb);
            LOG_DEBUG ("Retval - 0x%04X\n", retval);

            map_msg_object.num_envelopes = 0U;
            map_parser_diff_len = 0U;
#endif /* HAVE_OBJECT_PARSER */
        }
        break;

    case MAP_MCE_SET_MESSAGE_STATUS_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_SET_MESSAGE_STATUS_CNF - 0x%04X\n",
        event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);
        break;

    case MAP_MCE_PUSH_MESSAGE_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_PUSH_MESSAGE_CNF - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);

        if ((MAP_SUCCESS_RSP == event_result) ||
            (MAP_CONTINUE_RSP != event_result))
        {
            break;
        }

        /* Get Name content - Message Handle */
        if ((event_header->map_resp_info->name->value != NULL) &&
            (event_header->map_resp_info->name->length > 0U))
        {
            data = event_header->map_resp_info->name->value;
            length = event_header->map_resp_info->name->length;

            LOG_DEBUG ("Message Handle: ");
            for (i = 0U; i < length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n");
        }

        /* Transfer the remainder if any */
        if (remaining > map_xchg_size)
        {
            body_req.length = map_xchg_size;
            more = 0x01U;
        }
        else if (remaining != 0U)
        {
            body_req.length = (UINT16)remaining;
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
            (BT_IGNORE_RETURN_VALUE) BT_fops_file_read (body_req.value, body_req.length, fp, &actual);
        }

        req_info.body = &body_req;
        map_header.map_req_info = &req_info;

        send_request = 1U;
        break;

    case MAP_MCE_SET_NTF_REGISTRATION_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_SET_NTF_REGISTRATION_CNF - 0x%04X\n",
        event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);
        break;

    case MAP_ABORT_CNF:
        LOG_DEBUG ("Recvd MAP_ABORT_CNF - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);

        /* Reset */
        appl_map_mce_start_of_response = BT_FALSE;
        break;

    case MAP_MCE_DISCONNECT_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_DISCONNECT_CNF - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);
        break;

    case MAP_MCE_TRANSPORT_CLOSE_IND:
        LOG_DEBUG ("Recvd MAP_MCE_TRANSPORT_CLOSE_IND - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);

        for (i = 0U; i < MAP_CLIENT_NUM_INSTANCES; i++)
        {
            if (mas_instance[i].instance.handle == handle)
            {
                BT_mem_set(&mas_instance[i].instance.bd_addr, 0x00, BT_BD_ADDR_SIZE);
                break;
            }
        }

        map_print_appl_instances();
        break;

    case MAP_MCE_TRANSPORT_CLOSE_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_TRANSPORT_CLOSE_CNF - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Handle - 0x%02X\n", handle);

        for (i = 0U; i < MAP_CLIENT_NUM_INSTANCES; i++)
        {
            if (mas_instance[i].instance.handle == handle)
            {
                BT_mem_set(&mas_instance[i].instance.bd_addr, 0x00, BT_BD_ADDR_SIZE);
                break;
            }
        }

        map_print_appl_instances();
        break;

#ifdef MAP_SUPPORT_NOTIFICATION
    case MAP_MCE_NS_CONNECT_IND:
        LOG_DEBUG ("Recvd MAP_MCE_MNS_CONNECT_IND - %04X\n", event_result);
        LOG_DEBUG ("MCE Instance Handle - 0x%02X\n", handle);

        if (NULL != event_header->map_connect_info)
        {
            LOG_DEBUG("Peer Address - %02X %02X %02X %02X %02X %02X\n",
            event_header->map_connect_info->bd_addr[0U],
            event_header->map_connect_info->bd_addr[1U],
            event_header->map_connect_info->bd_addr[2U],
            event_header->map_connect_info->bd_addr[3U],
            event_header->map_connect_info->bd_addr[4U],
            event_header->map_connect_info->bd_addr[5U]);

            LOG_DEBUG ("MAX Exchange Size: %d\n",
            event_header->map_connect_info->max_recv_size);

            map_ns_xchg_size = event_header->map_connect_info->max_recv_size;
        }

        /* Check for valid event result and parameters */
        if((MAP_SUCCESS_RSP != event_result) ||
           (NULL == event_header->map_connect_info))
        {
            event_result = MAP_NOT_ACCEPTABLE_RSP;
            send_response = 1U;
            break;
        }

        /* Get the Handle parameters */
        for (i = 0U; i < MAP_CLIENT_NUM_INSTANCES; i++)
        {
            if (mns_instance[i].handle == handle)
            {
                BT_COPY_BD_ADDR(mns_instance[i].bd_addr,
                event_header->map_connect_info->bd_addr);
                break;
            }
        }

        if (i != MAP_CLIENT_NUM_INSTANCES)
        {
            for (j = 0U; j < MAP_CLIENT_NUM_INSTANCES; j++)
            {
                if (0 == BT_mem_cmp(mas_instance[j].instance.bd_addr,
                     event_header->map_connect_info->bd_addr, BT_BD_ADDR_SIZE))
                {
                    mas_instance[j].mns_id = (UCHAR)i;
                }
            }
        }

        map_print_appl_instances();

        event_result = MAP_SUCCESS_RSP;

        /* Set response to be sent */
        send_response = 1U;
        break;

    case MAP_MCE_NS_DISCONNECT_IND:
        LOG_DEBUG ("Recvd MAP_MCE_MNS_DISCONNECT_IND - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Instance Handle - 0x%02X\n", handle);
        break;

    case MAP_MCE_NS_TRANSPORT_CLOSE_CNF:
        LOG_DEBUG ("Recvd MAP_MCE_MNS_TRANSPORT_CLOSE_CNF - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Instance Handle - 0x%02X\n", handle);

        for (i = 0U; i < MAP_CLIENT_NUM_INSTANCES; i++)
        {
            if (mns_instance[i].handle == handle)
            {
                BT_mem_set (mns_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);
                break;
            }
        }

        for (j = 0U; j < MAP_CLIENT_NUM_INSTANCES; j++)
        {
            if (mas_instance[j].mns_id == i)
            {
                mas_instance[j].mns_id = MAP_ENTITY_INVALID;
            }
        }

        map_print_appl_instances();
        break;

    case MAP_MCE_NS_TRANSPORT_CLOSE_IND:
        LOG_DEBUG ("Recvd MAP_MCE_MNS_TRANSPORT_CLOSE_IND - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Instance Handle - 0x%02X\n", handle);

        for (i = 0U; i < MAP_CLIENT_NUM_INSTANCES; i++)
        {
            if (mns_instance[i].handle == handle)
            {
                BT_mem_set (mns_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);
                break;
            }
        }

        for (j = 0U; j < MAP_CLIENT_NUM_INSTANCES; j++)
        {
            if (mas_instance[j].mns_id == i)
            {
                mas_instance[j].mns_id = MAP_ENTITY_INVALID;
            }
        }

        map_print_appl_instances();
        break;

    case MAP_MCE_NS_EVENT_REPORT_IND:
        LOG_DEBUG ("Recvd MAP_MCE_MNS_EVENT_REPORT_IND - 0x%04X\n", event_result);
        LOG_DEBUG ("MCE Instance Handle - 0x%02X\n", handle);

        if ((MAP_SUCCESS_RSP != event_result) &&
            (MAP_CONTINUE_RSP != event_result))
        {
            break;
        }

       /* Set response to be sent */
        send_response = 1U;

        if (!map_push_started)
        {
            map_push_started = 0x01U;

            if ((event_header->map_req_info->body == NULL) ||
                (event_header->map_req_info->appl_params == NULL) ||
                (event_header->map_req_info->body->value == NULL) ||
                (event_header->map_req_info->body->length == 0U))
            {
                LOG_DEBUG ("Body/Appl Param Header/Value not present\n");

                /**
                 * From MAP/MCE/MMN/BV-01-I for IM Event PTS is sending empty body and expected send success respone.
                 */

                /* event_result = MAP_BAD_REQ_RSP; */
                event_result = MAP_SUCCESS_RSP;

                break;
            }

            appl_param = event_header->map_req_info->appl_params;

            if (0U != (MAP_GET_APPL_PARAM_FLAG
                (
                    appl_param->appl_param_flag,
                    MAP_FLAG_MASINSTANCEID
                )))
            {
                LOG_DEBUG ("MAS Instance ID: %d\n", appl_param->mas_instance_id);
            }
            else
            {
                LOG_DEBUG ("MAS Instance ID Not Found\n");
                event_result = MAP_BAD_REQ_RSP;

                break;
            }
        }

        if ((NULL == event_header->map_req_info->body->value) &&
            (0U == event_header->map_req_info->body->length))
        {
           LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            length = event_header->map_req_info->body->length;
            data = event_header->map_req_info->body->value;

            LOG_DEBUG ("\n\n========== Event Report IND ==========\n\n");

            /* Print the stream */
            for (i = 0U; i < length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }

            LOG_DEBUG ("\n\n======================================\n\n");

            /* Open file for PUT request */
            if (NULL == fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                BT_mem_set(mapc_file_object, 0, sizeof(mapc_file_object));
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)MAP_ROOT_FOLDER_BASE,
                    (UCHAR *)APPL_MAPC_EVENT_RPRT_FILE,
                    mapc_file_object
                );

                retval = BT_fops_file_open
                         (
                             mapc_file_object,
                             (UCHAR *)"wb",
                             &fp
                         );
                if ((API_SUCCESS != retval) || (NULL == fp))
                {
                    LOG_DEBUG ("Failed to open EVENT Report Object file\n");
                }
            }

            /* Write to file */
            if (NULL != fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, fp, &actual);
            }
        }

        if (MAP_SUCCESS_RSP == event_result)
        {
            if (NULL != fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (fp);
                fp = NULL;
            }

            map_push_started = 0x00U;
        }

        break;
#endif /* MAP_SUPPORT_NOTIFICATION */

    default:
        LOG_DEBUG ("Unknown MAP MCE Event\n");
        break;
    }

    if (0U != send_request)
    {
        if (0U < wait_count)
        {
            req_info.wait = BT_TRUE;
            map_header.map_req_info = &req_info;

            wait_count --;
        }

        retval = BT_map_mce_send_request
                 (
                     &handle,
                     event_type,
                     event_result,
                     &map_header,
                     more,
                     &actual
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG ("MAP MCE Send Request Failed - 0x%04X\n", retval);
        }

        if (MAP_MCE_PUSH_MESSAGE_CNF == event_type)
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

#ifdef MAP_SUPPORT_NOTIFICATION
    if (0U != send_response)
    {
        /* TODO: Check if to be protected with SRMP flag */
        if (0U < wait_count)
        {
            rsp_info.wait = BT_TRUE;
            map_header.map_resp_info = &rsp_info;
            wait_count--;
        }

        retval = BT_map_mce_ns_send_response
                 (
                     &handle,
                     event_type,
                     (UCHAR)event_result,
                     &map_header
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG ("MAP MCE Send Request Failed - 0x%04X\n", retval);
        }
    }
#endif /* MAP_SUPPORT_NOTIFICATION */

    BT_IGNORE_UNUSED_PARAM(map_ns_xchg_size);

    return retval;
}


void appl_map_mce_sdp_callback
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

#ifndef MAP_1_2
    UINT16 attrib_id[3U] = {PROTOCOL_DESC_LIST,
                           MAP_MAS_INSTANCE_ID_ATTR_ID,
                           MAP_SUPPORTED_MSG_TYPES_ATTR_ID};

    num_attrib_ids = 3U;
#else /* MAP_1_2 */
    UINT16 attrib_id[5U] = {PROTOCOL_DESC_LIST,
                           GOEP_L2CAP_PSM_ATTR_ID,
                           MAP_MAS_INSTANCE_ID_ATTR_ID,
                           MAP_SUPPORTED_MSG_TYPES_ATTR_ID,
                           MAP_SUPPORTED_FEATURES_ATTR_ID};

    num_attrib_ids = 5U;
#endif /* MAP_1_2 */

    BT_IGNORE_UNUSED_PARAM(length);

    /**
     * Frame the SDP Search pattern with following UUIDs
     * - MAP_MAS_UUID
     * - RFCOMM_UUID
     * - L2CAP_UUID
     * and Attribute ID "PROTOCOL_DESC_LIST"
     * (For getting the RFCOMM Server Channel on which Peer MAP Service
     *  is Listening)
     */
    uuid[0U].uuid_type = UUID_16;
    uuid[1U].uuid_type = UUID_16;
    uuid[2U].uuid_type = UUID_16;
    uuid[0U].uuid_union.uuid_16 = MAP_MAS_UUID;
    uuid[1U].uuid_union.uuid_16 = L2CAP_UUID;
    uuid[2U].uuid_union.uuid_16 = RFCOMM_UUID;

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
                     &mas_instance[map_app_instance].instance.map_sdp_record.map_sdp_handle,
                     uuid,
                     3U,
                     attrib_id,
                     num_attrib_ids,
                     NULL,
                     0U,
                     mas_instance[map_app_instance].instance.map_sdp_record.map_attrib_data,
                     &mas_instance[map_app_instance].instance.map_sdp_record.map_len_attrib_data
                 );

        if(API_SUCCESS != retval)
        {
            LOG_DEBUG("SDP SEARCH Failed for MAP..Closing SDP\n");
            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&mas_instance[map_app_instance].instance.map_sdp_record.map_sdp_handle);
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
            UCHAR attr_val_1B;
            UINT16 attr_size;
            UCHAR attr_val[4U];

#ifdef MAP_1_2
            UINT32 attr_val_4B;
#endif /* MAP_1_2 */

#ifdef MAP_1_2
            UINT16 attr_val_2B;
            UINT16 psm;
#endif /* MAP_1_2 */

            sdp_query_result = data;
            no_server_channels = 1U;

            while (sdp_query_result != NULL)
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

#ifdef MAP_1_2
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
                    }
                }

                /* if ((API_SUCCESS != retval) && (RFCOMM_PSM == psm)) */
#endif /* MAP_1_2 */
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

                    attr_val_1B = 0U;
                    attr_size = 1U;
                    /* Get the MAS Instance ID */
                    retval = BT_sdp_get_attr_value
                             (
                                 MAP_MAS_INSTANCE_ID_ATTR_ID,
                                 sdp_query_result,
                                 attr_val,
                                 &attr_size
                             );
                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG("SDP FAILED to find MAP Instance ID\n");
                    }
                    else
                    {
                        BT_UNPACK_LE_1_BYTE(&attr_val_1B, attr_val);
                        LOG_DEBUG("MAP Instance: 0x%02X\n", attr_val_1B);
                    }

                    attr_val_1B = 0U;
                    attr_size = 1U;
                    /* Get the SupportedMessageTypes */
                    retval = BT_sdp_get_attr_value
                             (
                                 MAP_SUPPORTED_MSG_TYPES_ATTR_ID,
                                 sdp_query_result,
                                 attr_val,
                                 &attr_size
                             );
                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG("SDP FAILED to find MAP Supported Message Type\n");
                    }
                    else
                    {
                        BT_UNPACK_LE_1_BYTE(&attr_val_1B, attr_val);
                        LOG_DEBUG("MAP Supported Message Type: 0x%02X\n", attr_val_1B);
                    }

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
                }
                else
                {
                    LOG_DEBUG ("Failed to find MAP Service.\n");
                }

                no_server_channels++;
            }
        }

        /* Now Close the SDP Connection */
        (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&mas_instance[map_app_instance].instance.map_sdp_record.map_sdp_handle);
        break;

    default: /* Events not needed to be Handled */
        LOG_DEBUG("Unhandled SDP Event (0x%02X)\n", command);
        break;
    }/* switch */

    return;
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
#ifdef HAVE_OBJECT_PARSER
    int i;
#endif /* HAVE_OBJECT_PARSER */

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
#ifdef HAVE_OBJECT_PARSER
        fprintf (map_parsed_fp, "GROUP NAME ---> ");
#endif /* HAVE_OBJECT_PARSER */
        break;

    case OBJECT_TOKEN_PROP_NAME:
#ifdef HAVE_OBJECT_PARSER
        fprintf (map_parsed_fp, "Property NAME ---> ");
#endif /* HAVE_OBJECT_PARSER */

        param_id = cb_param.keyword_id;
        break;

    case OBJECT_TOKEN_PROP_VAL:
#ifdef HAVE_OBJECT_PARSER
        fprintf (map_parsed_fp, "Property VALUE ---> ");
#endif /* HAVE_OBJECT_PARSER */

        if (cb_param.parser_result & OBJECT_RESULT_PARTIAL_PARSED)
        {
#ifdef HAVE_OBJECT_PARSER
            fprintf (map_parsed_fp, "<Partial value> ");
#endif /* HAVE_OBJECT_PARSER */
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
            map_msg_object.envelope.content.property.content_length =
                atoi((CHAR *)cb_param.value);

            /* Allocate memory to copy the incoming message */
            map_msg_object.envelope.content.message = NULL;
            map_msg_object.envelope.content.message =
            BT_alloc_mem
            ((map_msg_object.envelope.content.property.content_length -
              BMSG_CONTENT_MARKER_LENGTH) + 1U);
            break;

        default:
            LOG_DEBUG("Invalid Param ID: 0x%04X\n", param_id);
            break;
        }
        break;

    case OBJECT_TOKEN_PROP_PARAM_NAME:
#ifdef HAVE_OBJECT_PARSER
        fprintf (map_parsed_fp, "Property PARAM NAME ---> ");
#endif /* HAVE_OBJECT_PARSER */
        break;

    case OBJECT_TOKEN_PROP_PARAM_VAL:
#ifdef HAVE_OBJECT_PARSER
        fprintf (map_parsed_fp, "Property PARAM VALUE ---> ");
#endif /* HAVE_OBJECT_PARSER */
        break;

    case OBJECT_TOKEN_ERROR:
#ifdef HAVE_OBJECT_PARSER
        fprintf (map_parsed_fp, "***ERROR*** ---> ");
#endif /* HAVE_OBJECT_PARSER */
        break;

    default:
        printf("Invalid Keyword Type: %d\n", cb_param.keyword_type);
        break;
    }

    /* Copy if it is expected message */
    if (BMSG_STATE_IN_MSG == state)
    {
        if (0U == msg_continue)
        {
            msg_dest_index = 0U;
            msg_remaining =
                (map_msg_object.envelope.content.property.content_length -
                            BMSG_CONTENT_MARKER_LENGTH);

            context->total_parsed_length += context->cur_parsed_length;
            msg_src_index = context->total_parsed_length;

            to_copy =
            ((context->buffer_len - context->total_parsed_length) <
            (map_msg_object.envelope.content.property.content_length -
            BMSG_CONTENT_MARKER_LENGTH))?
            (context->buffer_len - context->total_parsed_length):
            (map_msg_object.envelope.content.property.content_length -
            BMSG_CONTENT_MARKER_LENGTH);
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
             (map_msg_object.envelope.content.property.content_length -
             BMSG_CONTENT_MARKER_LENGTH)) &&
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

#ifdef HAVE_OBJECT_PARSER
    fprintf (map_parsed_fp, "[KEY : 0x%04x] ", cb_param.keyword_id);

    for (i = 0U; i < cb_param.value_len; i++)
    {
        fprintf (map_parsed_fp, "%c", cb_param.value [i]);
    }
    fprintf (map_parsed_fp, "\n");

    if (0U != (cb_param.parser_result & 0x01U))
    {
        fprintf (map_parsed_fp, "Folded Line Detected\n");
    }
#endif /* HAVE_OBJECT_PARSER */
}


void map_vcard_parser_cb
     (
         OBJECT_PARSER_CB_PARAM cb_param,
         OBJECT_PARSER_CONTEXT * context
     )
{
#ifdef HAVE_OBJECT_PARSER
    UINT8 i;
#endif /* HAVE_OBJECT_PARSER */

    static UINT16 vcard_param_id = BEGIN;
    UINT16 token, token_len;

    token = 0U;
    token_len = 0U;

    switch (cb_param.keyword_type)
    {
    case OBJECT_TOKEN_GRP_NAME:
#ifdef HAVE_OBJECT_PARSER
        fprintf (map_parsed_fp, "GROUP NAME ---> ");
#endif /* HAVE_OBJECT_PARSER */
        break;

    case OBJECT_TOKEN_PROP_NAME:
#ifdef HAVE_OBJECT_PARSER
        fprintf (map_parsed_fp, "Property NAME ---> ");
#endif /* HAVE_OBJECT_PARSER */
        vcard_param_id = cb_param.keyword_id;
        break;

    case OBJECT_TOKEN_PROP_VAL:
#ifdef HAVE_OBJECT_PARSER
        fprintf (map_parsed_fp, "Property VALUE ---> ");

        if (cb_param.parser_result & OBJECT_RESULT_PARTIAL_PARSED)
        {
            fprintf (map_parsed_fp, "<Partial value> ");
        }
#endif /* HAVE_OBJECT_PARSER */

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
#ifdef HAVE_OBJECT_PARSER
        fprintf (map_parsed_fp, "Property PARAM NAME ---> ");
#endif /* HAVE_OBJECT_PARSER */
        break;

    case OBJECT_TOKEN_PROP_PARAM_VAL:
#ifdef HAVE_OBJECT_PARSER
        fprintf (map_parsed_fp, "Property PARAM VALUE ---> ");
#endif /* HAVE_OBJECT_PARSER */
        break;

    case OBJECT_TOKEN_ERROR:
#ifdef HAVE_OBJECT_PARSER
        fprintf (map_parsed_fp, "***ERROR*** ---> ");
#endif /* HAVE_OBJECT_PARSER */
        break;

    default:
        printf("Invalid Keyword: %d\n", cb_param.keyword_type);
        break;
    }

#ifdef HAVE_OBJECT_PARSER
    fprintf (map_parsed_fp, "[KEY : 0x%04x] ", cb_param.keyword_id);

    for (i = 0U; i < cb_param.value_len; i++)
    {
        fprintf (map_parsed_fp, "%c", cb_param.value [i]);
    }
    fprintf (map_parsed_fp, "\n");

    if (cb_param.parser_result & 0x01U)
    {
        fprintf (map_parsed_fp, "Folded Line Detected\n");
    }
#endif /* HAVE_OBJECT_PARSER*/
}
#endif /* HAVE_OBJECT_PARSER */

void appl_map_mce_print_folder_list( XML_DOC_COMPONENT* pTemp)
{
    UINT16 i = 0U;
    UINT16 j = 0U;
    UINT16 flag = 0U;

    if(NULL == pTemp)
    {
        LOG_DEBUG("Invalid Parameter\n");
        /* return; */
    }
    else
    {
        if((0 == (BT_str_cmp(pTemp->element_name, "file"))) ||
           (0 == (BT_str_cmp(pTemp->element_name, "folder"))))
        {
            /* LOG_DEBUG("[ %02d ] %-10s", list_index, pTemp->element_name); */
            LOG_DEBUG("[ %02d ]", map_list_index);
            flag = 1U;
            map_list_index ++;
        }

        if ( pTemp->no_of_attribs != -1 )
        {
            for ( j = 0U; j <= pTemp->no_of_attribs; j++ )
            {
                if (1U == flag)
                {
                    if(0 == (BT_str_cmp(pTemp->list[j].attrib, "name")))
                    {
                        LOG_DEBUG("\t%s", pTemp->list[j].value);
                    }
#if 0
                    else if((BT_str_cmp(pTemp->list[j].attrib,"type"))==0)
                    {
                        LOG_DEBUG(" %s",pTemp->list[j].value);
                    }
                    else if((BT_str_cmp(pTemp->list[j].attrib,"user-perm"))==0)
                    {
                        LOG_DEBUG(" %s",pTemp->list[j].value);
                    }
                    else if((BT_str_cmp(pTemp->list[j].attrib,"group-perm"))==0)
                    {
                        LOG_DEBUG(" %s",pTemp->list[j].value);
                    }
                    else if((BT_str_cmp(pTemp->list[j].attrib,"other-perm"))==0)
                    {
                        LOG_DEBUG(" %s",pTemp->list[j].value);
                    }
                    else if((BT_str_cmp(pTemp->list[j].attrib,"size"))==0)
                    {
                        LOG_DEBUG(" %10s",pTemp->list[j].value);
                    }
                    else if((BT_str_cmp(pTemp->list[j].attrib,"modified"))==0)
                    {
                        LOG_DEBUG(" %30s",pTemp->list[j].value);
                    }
                    else
                    {
                        /* MISRA C-2012 Rule 15.7 */
                    }
#endif /* 0 */
                }
            }
            LOG_DEBUG("\n");
        }

        for ( i = 0U; i <= pTemp->no_of_sub_elems ; i++ )
        {
            appl_map_mce_print_folder_list( pTemp->sub_elems [i]);
        }
    }
}


void appl_map_mce_print_message_list( XML_DOC_COMPONENT* pTemp)
{
    UINT16 i = 0U;
    UINT16 j = 0U;
    UINT16 flag = 0U;

    if(NULL == pTemp)
    {
        LOG_DEBUG("Invalid Parameter\n");
        /* return; */
    }
    else
    {
        if (0 == (BT_str_cmp(pTemp->element_name, "msg")))
        {
            /* LOG_DEBUG("[ %02d ] %-10s", list_index, pTemp->element_name); */
            LOG_DEBUG("%02d. ", map_list_index);
            flag = 1U;
            map_list_index++;
        }

        if (pTemp->no_of_attribs != -1)
        {
            for (j = 0U; j <= pTemp->no_of_attribs; j++)
            {
                if (1U == flag)
                {
                    if (0 == (BT_str_cmp(pTemp->list[j].attrib, "handle")))
                    {
                        LOG_DEBUG("%-17s", pTemp->list[j].value);
                    }
                    else if (0 == (BT_str_cmp(pTemp->list[j].attrib, "subject")))
                    {
                        LOG_DEBUG("%-20s", pTemp->list[j].value);
                    }
                    else if (0 == (BT_str_cmp(pTemp->list[j].attrib, "type")))
                    {
                        LOG_DEBUG("%-10s", pTemp->list[j].value);
                    }
                    else if (0 == (BT_str_cmp(pTemp->list[j].attrib, "size")))
                    {
                        LOG_DEBUG("%-09s", pTemp->list[j].value);
                    }
                    else if (0 == (BT_str_cmp(pTemp->list[j].attrib, "read")))
                    {
                        LOG_DEBUG("%-04s", pTemp->list[j].value);
                    }
                    else if (0 == (BT_str_cmp(pTemp->list[j].attrib, "sent")))
                    {
                        LOG_DEBUG("%-04s", pTemp->list[j].value);
                    }
                    else if (0 == (BT_str_cmp(pTemp->list[j].attrib, "protected")))
                    {
                        LOG_DEBUG("%-04s", pTemp->list[j].value);
                    }
#if 0
                    else if ((BT_str_cmp(pTemp->list[j].attrib, "datetime")) == 0)
                    {
                        LOG_DEBUG(" %s", pTemp->list[j].value);
                    }
                    else if ((BT_str_cmp(pTemp->list[j].attrib, "sender_name")) == 0)
                    {
                        LOG_DEBUG(" %s", pTemp->list[j].value);
                    }
                    else if ((BT_str_cmp(pTemp->list[j].attrib, "sender_addressing")) == 0)
                    {
                        LOG_DEBUG(" %s", pTemp->list[j].value);
                    }
                    else if ((BT_str_cmp(pTemp->list[j].attrib, "recipient_addressing")) == 0)
                    {
                        LOG_DEBUG(" %s", pTemp->list[j].value);
                    }
                    else if ((BT_str_cmp(pTemp->list[j].attrib, "attachment_size")) == 0)
                    {
                        LOG_DEBUG(" %10s", pTemp->list[j].value);
                    }
                    else if ((BT_str_cmp(pTemp->list[j].attrib, "priority")) == 0)
                    {
                        LOG_DEBUG(" %10s", pTemp->list[j].value);
                    }
#endif /* 0 */
                    else
                    {
                        /* MISRA C-2012 Rule 15.7 */
                    }
                }
            }
            LOG_DEBUG("\n");
        }

        for (i = 0U; i <= pTemp->no_of_sub_elems; i++)
        {
            appl_map_mce_print_message_list(pTemp->sub_elems[i]);
        }
    }
}

void map_print_appl_instances (void)
{
    UINT8 i;

    LOG_DEBUG ("\n\n===========================================================\n");
    LOG_DEBUG ("Instance\t MAS Handle\t BD Addr\t MNS Id");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < MAP_CLIENT_NUM_INSTANCES; i++)
    {
        LOG_DEBUG ("%d\t\t ", i);
        LOG_DEBUG ("%02x\t ", mas_instance[i].instance.handle);
        LOG_DEBUG ("%02x:%02x:%02x:%02x:%02x:%02x\t ",
        mas_instance[i].instance.bd_addr[0U], mas_instance[i].instance.bd_addr[1U],
        mas_instance[i].instance.bd_addr[2U], mas_instance[i].instance.bd_addr[3U],
        mas_instance[i].instance.bd_addr[4U], mas_instance[i].instance.bd_addr[5U]);
        LOG_DEBUG ("%02x\n", mas_instance[i].mns_id);
    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    LOG_DEBUG ("\n\n===========================================================\n");
    LOG_DEBUG ("Instance\t MNS Handle\t BD Addr");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < MAP_CLIENT_NUM_INSTANCES; i++)
    {
        LOG_DEBUG ("%d\t\t ", i);
        LOG_DEBUG ("%02x\t ", mns_instance[i].handle);
        LOG_DEBUG ("%02x:%02x:%02x:%02x:%02x:%02x\n",
        mns_instance[i].bd_addr[0U], mns_instance[i].bd_addr[1U],
        mns_instance[i].bd_addr[2U], mns_instance[i].bd_addr[3U],
        mns_instance[i].bd_addr[4U], mns_instance[i].bd_addr[5U]);
    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");
}

#endif /* MAP_MCE */

