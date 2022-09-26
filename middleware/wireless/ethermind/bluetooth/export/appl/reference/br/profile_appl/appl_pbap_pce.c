
/**
 *  \file appl_pbap_pce.c
 *
 *  Source file for PBAP_PCE Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_pbap_pce.h"
#include "appl_utils.h"

/*
 * This Sample Application demonstrates the usage of various PBAP PCE Role APIs
 */

#ifdef PBAP_PCE
/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */

static const char pbap_pce_options[] = "\n\
-------------- PBAP PCE Menu ---------------\n\
    0. Exit \n\
    1. Refresh \n\
    2. Register Peer BD ADDR \n\
    3. Get Registered Peer BD ADDR. \n\
\n\
    5. PCE Initialize \n\
    6. PCE Shutdown \n\
    7. PCE Start \n\
    8. PCE Stop \n\
\n\
   10. Create ACL Connection\n\
   11. ACL Disconnection\n\
   12. Do SDP Query \n\
\n\
   15. Connect to PBAP PSE \n\
   16. Disconnect from PBAP PSE \n\
   17. Transport Disconnect \n\
\n\
   20. Download Phonebook \n\
   21. Set Phonebook \n\
   22. Get Phonebook List\n\
   23. Pull Business Card \n\
   24. Abort \n\
\n\
   30. Input Server Pincode \n\
\n\
   50. List Directory \n\
\n\
Your Choice : \0";
/* PUT */

static UCHAR                   pbapc_bd_addr[6U];
static UINT8                   pbap_pce_handle;
static APPL_PBAP_PCE_STRUCT    pbap_appl_instance[2U];

static APPL_PBAP_PCE_USER_INFO user_info;

static SDP_HANDLE              sdp_pbap_handle;
static UCHAR                   pbap_ser_channel;
#ifdef PBAP_1_2
static UINT16                  pbap_ser_psm;
#endif /* PBAP_1_2 */
static UCHAR                   user_name[] = "PTS";
static UCHAR                   pass_word[] = "0000";
static APPL_PBAP_PCE_STRUCT  * pbap_appl_inst;
static PBAP_APPL_PARAMS        pbap_appl_params;

static UCHAR                   pbap_file_object[MAX_PBAP_FILE_OBJ_NAME_LEN];
static BT_fops_file_handle     get_file_ptr;
static UINT8                   get_file_ptr_active;
static UINT8                   auth_reqd;

static UCHAR wait_count;

/* ----------------------------------------- Functions */

/* Entry Point for PBAP Client Application */
void main_pbap_pce_operations (void)
{
    int   choice, menu_choice;
    int   cn_handle;
    UINT8 input;

    UCHAR file_name[64U];
    UCHAR file_name_listing[32U];

    UCHAR user_code[20U];
    UCHAR pin_code[16U];
    UCHAR search_val[20U];

    CHAR path[BT_FOPS_MAX_DIRECTORY_SIZE];

    API_RESULT retval;
    PBAP_EVENT_NTF_CB callback_func;
    PBAP_HEADER_STRUCT  name_req;
    PBAP_HEADER_STRUCT  pin;
    PBAP_HEADER_STRUCT  userid;
    PBAP_REQUEST_STRUCT req_info;

    PBAP_CONNECT_STRUCT  connect_req_send;
    PBAP_HEADERS         pbap_req_header_send;

    UINT16 connection_handle = HCI_INVALID_CONNECTION_HANDLE;

    BT_IGNORE_UNUSED_PARAM(auth_reqd);

    /* This application uses only one instance; Set it to the pointer */
    pbap_appl_inst = &pbap_appl_instance[0U];
    user_info.user_id = user_name;
    user_info.passwd = pass_word;

    name_req.value = NULL;
    name_req.length = 0U;

    pin.value = NULL;
    pin.length = 0U;

    userid.value = NULL;
    userid.length = 0U;

    req_info.name = NULL;
    req_info.appl_params = NULL;
    req_info.user_id = NULL;
    req_info.pin_info = NULL;

    get_file_ptr_active = 0x00U;
    get_file_ptr = NULL;

    pbap_req_header_send.pbap_connect_info = NULL;
    connect_req_send.pin_info = NULL;
    connect_req_send.user_id = NULL;

    BT_LOOP_FOREVER()
    {
        printf("%s", pbap_pce_options);
        scanf("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0: /* Go Back to the Caller */
            break; /* return; */

        case 1: /* Refresh the Menu */
            LOG_DEBUG("Refreshing Menu...\n");
            break;

        case 2: /* Register PBAP PSE's BD addrs */
            LOG_DEBUG("Enter PBAP PSE's Bluetooth Device Address\n");

            /* Read the BD_ADDR of Remote Device */
            appl_get_bd_addr(pbapc_bd_addr);
            break;

        case 3:
            /* Get Registered BD_ADDR of Peer Device */
            LOG_DEBUG("\nRegistered Peer BD "\
                BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
                BT_DEVICE_ADDR_ONLY_PRINT_STR(pbapc_bd_addr));
            break;

        case 5:
          /*
           *  Initiate the PBAP_PCE Entities; PBAP_PCE_MAX_ENTITY entities
           *  get initialized
           */
          retval = BT_pbap_pce_init();

          if(API_SUCCESS != retval)
          {
              LOG_DEBUG("Initialization Failed. Reason 0x%04X \n", retval);
          }
          else
          {
              LOG_DEBUG("Successfully Initialized %d PBAP PCE entities \n",
              PBAP_PCE_MAX_ENTITY);
          }
          break;

        case 6:
            retval = BT_pbap_pce_shutdown();

            if(API_SUCCESS != retval)
            {
              LOG_DEBUG("Shutdown Failed. Reason 0x%04X \n", retval);
              break;
            }
            LOG_DEBUG("Shutdown successful.\n");
            break;

        case 7:
          /*
           *  Start an instance of PBAP PCE, Call Connect API to Connect
           */
          callback_func = appl_pbap_pce_event_callback;

          retval = BT_pbap_pce_start_instance
                   (
                       &pbap_pce_handle,
                       callback_func
                   );

          if(API_SUCCESS != retval)
          {
              LOG_DEBUG("Start Failed. Reason 0x%04X \n", retval);
              break;
          }

          LOG_DEBUG("Successfully started PBAP PCE Entity %d \n",
          pbap_pce_handle);
          break;

        case 8:
           /*
            *  Stop an instance of PBAP PCE
            */
          retval = BT_pbap_pce_stop_instance
                   (
                       pbap_pce_handle
                   );

          if(API_SUCCESS != retval)
          {
              LOG_DEBUG("Stop Failed. Reason 0x%04X \n", retval);
              break;
          }

          LOG_DEBUG("Successfully stopped PBAP PCE Entity %d \n",
          pbap_pce_handle);
          break;

        case 10:
            /*
             * First Application should establish ACL Connection with
             * the peer Device
             */
            retval = BT_hci_create_connection
                     (
                         pbapc_bd_addr,
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
            /* To disconnect read the ACL Connection Handle */
            LOG_DEBUG("Enter Connection Handle (in Hex) = "); fflush(stdout);
            scanf("%x", &cn_handle);
            connection_handle = (UINT16)cn_handle;

            LOG_DEBUG("\n");
            LOG_DEBUG("Starting HCI Disconnection ... "); fflush(stdout);

            /* Mention Reason for Disconnect as User Initiated Disconnection */
            retval = BT_hci_disconnect ( connection_handle, 0x13U );
            if (API_SUCCESS != retval)
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
             * Start SDP Query to Find out if the Peer Device is running PBAP
             * Service and if so on which Server Channel the Remote PBAP PSE
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

            LOG_DEBUG("Querying Remote Device 0x%02X:%02X:%02X:%02X:%02X:%02X\n",
            pbapc_bd_addr[0U], pbapc_bd_addr[1U], pbapc_bd_addr[2U], pbapc_bd_addr[3U], pbapc_bd_addr[4U],
            pbapc_bd_addr[5U]);

            SDP_SET_HANDLE
            (
                sdp_pbap_handle,
                pbapc_bd_addr,
                appl_pbap_pce_sdp_callback
            );

            retval = BT_sdp_open(&sdp_pbap_handle);

            if ( retval != API_SUCCESS )
            {
                LOG_DEBUG("SDP Connect FAILED !! Error Code = 0x%04X\n", retval);
            }

            LOG_DEBUG("Please Wait for SDP Operation to Complete\n");
            break;

        case 15:
            LOG_DEBUG("Authenticate the Server(0/1): ");
            scanf("%d", &choice);

            auth_reqd = 0U;
            if(1 == choice)
            {
                auth_reqd = 1U;
                pin.value = user_info.passwd;
                pin.length = (UINT16)BT_str_len(user_info.passwd);

                userid.value = user_info.user_id;
                userid.length = (UINT16)BT_str_len(user_info.user_id);

                pbap_appl_inst->connect_info.pin_info = &pin;
                pbap_appl_inst->connect_info.user_id = &userid;
            }

            pbap_appl_inst->pbap_handle = pbap_pce_handle;

            pbap_appl_inst->acl_connect_handle = connection_handle;

            pbap_appl_inst->connect_info.bd_addr = pbapc_bd_addr;
            pbap_appl_inst->connect_info.auth_flag = (UINT8)choice;

#ifdef PBAP_1_2
            /**
             * Note:
             * PbapSupportedFeatures is Mandatory if the PSE advertises a
             * PbapSupportedFeatures attribute in its SDP rsp.
             */
            LOG_DEBUG ("Add PBAP Supported Features.\n");
            LOG_DEBUG ("    1 -> Yes\n");
            LOG_DEBUG ("    0 -> No\n");
            scanf ("%d", &choice);

            pbap_appl_inst->connect_info.pbap_supported_features = 0x00U;
            if (0x00 != choice)
            {
                pbap_appl_inst->connect_info.pbap_supported_features = APPL_PBAP_SUPPORTED_FEATURES;
            }

#if 0
            pbap_appl_inst->connect_info.psm = pbap_ser_psm;
#else
            LOG_DEBUG ("Enter peer L2CAP PSM (in Hex): ");
            scanf ("%x", &choice);
            pbap_appl_inst->connect_info.psm = (UINT16)choice;
#endif /* 0 */
#endif /* PBAP_1_2 */

            LOG_DEBUG("Enter peer RFCOMM Server Channel (in Hex): ");
            scanf("%x", &choice);
            pbap_ser_channel = (UINT8)choice;

            pbap_appl_inst->connect_info.server_ch_no = pbap_ser_channel;
            pbap_appl_inst->connect_info.max_recv_size = APPL_PBAP_PCE_MAX_SIZE;

            LOG_DEBUG("Sending Connect Request... \n");
            retval = BT_pbap_pce_connect
                    (
                        pbap_pce_handle,
                        &pbap_appl_inst->connect_info
                    );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Failed. Reason 0x%04X. \n", retval);
            }
            else
            {
                LOG_DEBUG("Successful.\n Wait for Connection complete Event. \n");
            }
            break;

        case 16:
            /*
             * Disconnect from the PBAP Server and stop the instance after
             * getting the Disconnect Confirmation.
             */
            retval = BT_pbap_pce_disconnect
                    (
                        pbap_pce_handle
                    );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Stop Failed. Reason 0x%04X \n", retval);
                break;
            }
            break;

        case 17: /* Transport Disconnect */
            retval = BT_pbap_pce_trans_disconnect(pbap_pce_handle);

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("PBAP PCE Disconnect Attempt FAILED !! Error Code = 0x%04X\n",
                retval);
            }
            break;

        case 20: /* Phonebook download */
            LOG_DEBUG("Enter the Phone Book Name to be downloaded: ");

            BT_mem_set(&req_info, 0, sizeof(PBAP_REQUEST_STRUCT));

            name_req.length = appl_pbap_pce_read_string(file_name,30U);
            name_req.value = file_name;
            req_info.name = &name_req;

            LOG_DEBUG("Enter number of wait stages: ");
            scanf("%d", &choice);
            wait_count = (UCHAR)choice;

            if (0U < wait_count)
            {
                req_info.wait = BT_TRUE;
                wait_count--;
            }
            else
            {
                req_info.wait = BT_FALSE;
            }

            PBAP_RESET_APPL_PARAM_FLAG(pbap_appl_params.appl_param_flag);

            /*
             * Set Mandatory 'MaxList' parameter flag and
             * assign default max value
             */
            PBAP_SET_APPL_PARAM_FLAG
            (
                pbap_appl_params.appl_param_flag,
                PBAP_FLAG_MAX_LIST_COUNT
            );
            pbap_appl_params.max_list_count = 0xFFFFU;

            LOG_DEBUG("Do you want to specify application parameters?(1/0): ");
            scanf("%d", &choice);
            if (1 == choice)
            {
                /* APPL_PARAM 1 : PropertySelector */
                LOG_DEBUG("Include appl. param 'PropertySelector'? (1/0): ");
                scanf("%d", &choice);
                if(1 == choice)
                {
                    PBAP_SET_APPL_PARAM_FLAG
                    (
                        pbap_appl_params.appl_param_flag,
                        PBAP_FLAG_FILTER
                    );

                    PBAP_RESET_FILTER_FLAG
                    (
                        pbap_appl_params.filter
                    );

                    LOG_DEBUG ("Select property 'N'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_N\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.filter,
                            PBAP_FLAG_N
                        );
                    }

                    LOG_DEBUG ("Select property 'TEL'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_TEL\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.filter,
                            PBAP_FLAG_TEL
                        );
                    }

                    LOG_DEBUG ("Select property 'EMAIL'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_EMAIL\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.filter,
                            PBAP_FLAG_EMAIL
                        );
                    }

                    LOG_DEBUG ("Select property 'ORG'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_ORG\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.filter,
                            PBAP_FLAG_ORG
                        );
                    }

                    LOG_DEBUG ("Select property 'PHOTO'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_PHOTO\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.filter,
                            PBAP_FLAG_PHOTO
                        );
                    }

                    LOG_DEBUG ("Select property 'X-IRMC-CALL-DATETIME'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_X_IRMC_CALL_DATETIME\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.filter,
                            PBAP_FLAG_X_IRMC_CALL_DATETIME
                        );
                    }

                    LOG_DEBUG ("Select property 'X-BT-UCI'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_X_BT_UCI\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.filter,
                            PBAP_FLAG_X_BT_UCI
                        );
                    }

                    LOG_DEBUG ("Select property 'X-BT-UID'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_X_BT_UID\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.filter,
                            PBAP_FLAG_X_BT_UID
                        );
                    }
                 }

                /* APPL_PARAM 2 : Format */
                LOG_DEBUG("Include appl. param 'Format'? (1/0): ");
                scanf("%d", &choice);
                if(1 == choice)
                {
                    PBAP_SET_APPL_PARAM_FLAG
                    (
                        pbap_appl_params.appl_param_flag,
                        PBAP_FLAG_FORMAT
                    );

                    LOG_DEBUG("Enter value for\n");
                    LOG_DEBUG ("\t 0-> v2.1\n");
                    LOG_DEBUG ("\t 1-> v3.0\n");

                    choice = 0;
                    scanf("%d", &choice);
                    pbap_appl_params.format = (UINT8)choice;
                }

                /* APPL_PARAM 3 : MaxListCount */
                LOG_DEBUG("Include appl. param 'MaxListCount'? (1/0): ");
                scanf("%d", &choice);
                if(1 == choice)
                {
                    LOG_DEBUG("Enter the Count: \n");
                    LOG_DEBUG ("    0 -> Phone Book Size\n");
                    /* LOG_DEBUG("Enter value for MaxListCount (0-65535): "); */
                    scanf("%d", &choice);

                    pbap_appl_params.max_list_count = (UINT16)choice;
                }

                /* APPL_PARAM 4 : ListStartOffset */
                LOG_DEBUG("Include appl. param 'ListStartOffset'? (1/0): ");
                scanf("%d", &choice);
                if(1 == choice)
                {
                    PBAP_SET_APPL_PARAM_FLAG
                    (
                        pbap_appl_params.appl_param_flag,
                        PBAP_FLAG_LIST_START_OFFSET
                    );

                    LOG_DEBUG("Enter ListStartOffset value: ");
                    scanf("%d", &choice);
                    pbap_appl_params.list_start_offset = (UINT16)choice;
                }

#ifdef PBAP_1_2
                /* APPL_PARAM 5 : ResetNewMissedCalls */
                LOG_DEBUG("Include appl. param 'ResetNewMissedCalls'? (1/0): ");
                scanf("%d", &choice);
                if(1 == choice)
                {
                    PBAP_SET_APPL_PARAM_FLAG
                    (
                        pbap_appl_params.appl_param_flag,
                        PBAP_FLAG_RESET_NEW_MISSED_CALLS
                    );

                    pbap_appl_params.reset_new_missed_calls = 0x01U; /* reset */
                }

                /* APPL_PARAM 6 : vCardSelector */
                LOG_DEBUG("Include appl. param 'vCardSelector'? (1/0): ");
                scanf("%d", &choice);
                if(1 == choice)
                {
                    PBAP_SET_APPL_PARAM_FLAG
                    (
                        pbap_appl_params.appl_param_flag,
                        PBAP_FLAG_VCARD_SELECTOR
                    );

                    PBAP_RESET_FILTER_FLAG
                    (
                        pbap_appl_params.vcard_selector
                    );

                    LOG_DEBUG ("vCardSelector property 'TEL'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_TEL\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.vcard_selector,
                            PBAP_FLAG_TEL
                        );
                    }

                    LOG_DEBUG ("vCardSelector property 'EMAIL'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_EMAIL\n");
                        PBAP_SET_FILTER_FLAG
                        (
                              pbap_appl_params.vcard_selector,
                              PBAP_FLAG_EMAIL
                        );
                    }

                    LOG_DEBUG ("vCardSelector property 'PHOTO'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_PHOTO\n");
                        PBAP_SET_FILTER_FLAG
                        (
                              pbap_appl_params.vcard_selector,
                              PBAP_FLAG_PHOTO
                        );
                    }
                } /* End: vCardSelector */

                /* APPL_PARAM 7 : vCardSelectorOperator */
                LOG_DEBUG("Include appl. param 'vCardSelectorOperator'? (1/0): ");
                scanf("%d", &choice);
                if(1 == choice)
                {
                    PBAP_SET_APPL_PARAM_FLAG
                    (
                        pbap_appl_params.appl_param_flag,
                        PBAP_FLAG_VCARD_SELECTOR_OPERATOR
                    );

                    LOG_DEBUG("Enter value for vCardSelectorOperator\n");
                    LOG_DEBUG ("\t 0-> OR\n");
                    LOG_DEBUG ("\t 1-> AND\n");
                    LOG_DEBUG (": ");
                    scanf("%d", &choice);

                    pbap_appl_params.vcard_selector_operator = (UINT8)choice;
                }

#endif /* PBAP_1_2 */
            }

            req_info.appl_params = &pbap_appl_params;

            LOG_DEBUG("Sending Get Phone book Request... ");
            retval = BT_pbap_pce_get_phonebook
                     (
                         pbap_pce_handle,
                         &req_info
                     );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG(" Failed. Reason 0x%04X \n", retval);
            }
            else
            {
                LOG_DEBUG(" Successful. \n Wait for GET Response... \n");

                /* If the request is not for phonebook size */
                if(PBAP_GET_APPL_PARAM_FLAG(
                    pbap_appl_params.appl_param_flag,
                    PBAP_FLAG_MAX_LIST_COUNT) &&
                    (0U == pbap_appl_params.max_list_count))
                {
                    LOG_DEBUG("Request is for phonebook size\n");
                }
                else
                {
                    appl_replace_special_chars(file_name);

                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)APPL_PBAP_FOLDER_REFERENCE,
                        file_name,
                        pbap_file_object
                    );

                    LOG_DEBUG("Phonebook contents sent by PSE will be dumped to file %s\n",
                        pbap_file_object);

                    /* Open a file to write the contents */
                    retval = BT_fops_file_open
                             (
                                 pbap_file_object,
                                 (UCHAR *)"wb",
                                 &get_file_ptr
                             );

                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG(" Failed to open the file\n");
                        break;
                    }
                    else
                    {
                        get_file_ptr_active = 0x01U;
                    }
                }
            }
            break;

        case 21: /* Set Phone Book */
            LOG_DEBUG("Select Below Set Path Options\n");
            LOG_DEBUG("    1 -> Child Folder \n");
            LOG_DEBUG("    2 -> Previous Folder (Parent Folder)\n");
            LOG_DEBUG("    3 -> Root Folder \n");

            scanf("%d", &choice);

            BT_mem_set(&req_info, 0x00, sizeof (PBAP_REQUEST_STRUCT));

            switch(choice)
            {
            case 1:
                LOG_DEBUG(" Enter the path to be set: ");

                name_req.length = appl_pbap_pce_read_string(file_name, 20U);
                name_req.value = file_name;
                req_info.name = &name_req;
                req_info.setpath_flag    = PBAP_SET_CHILD_FOLDER;
                break;

            case 2:
                req_info.setpath_flag    = PBAP_SET_PARENT_FOLDER;
                break;

            case 3:
                req_info.setpath_flag    = PBAP_SET_ROOT_FOLDER;
                break;

            default:
                LOG_DEBUG(" Invalid option \n");
                break;
            }

            if((choice > 0) && (choice < 4))
            {
                LOG_DEBUG("Sending Set Phone book Path Request... ");
                retval = BT_pbap_pce_set_phonebook
                        (
                            pbap_pce_handle,
                            &req_info
                        );

                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG(" Failed. Reason 0x%04X \n", retval);
                }
                else
                {
                    LOG_DEBUG(" Successful. \n Wait for SETPATH Response... \n");
                }
            }
            break;

        case 22: /* Get Phone Book List */
            LOG_DEBUG("Enter the Phone Book Name to be browsed: (1/0)");
            scanf("%d", &choice);

            BT_mem_set(&req_info, 0, sizeof(PBAP_REQUEST_STRUCT));

            BT_str_copy(file_name,"0");
            if(1 == choice)
            {
                LOG_DEBUG ("Enter PB Folder Name: ");
                name_req.length = appl_pbap_pce_read_string(file_name,20U);
                name_req.value = file_name;
                req_info.name = &name_req;
            }
            else
            {
                req_info.name = NULL;
            }

            LOG_DEBUG("Enter number of wait stages: ");
            scanf("%d", &choice);
            wait_count = (UCHAR)choice;

            if (0U < wait_count)
            {
                req_info.wait = BT_TRUE;
                wait_count--;
            }
            else
            {
                req_info.wait = BT_FALSE;
            }

            PBAP_RESET_APPL_PARAM_FLAG(pbap_appl_params.appl_param_flag);

            /*
            * Set Mandatory 'MaxList' parameter flag and
            * assign default max value
            */
            PBAP_SET_APPL_PARAM_FLAG
            (
                pbap_appl_params.appl_param_flag,
                PBAP_FLAG_MAX_LIST_COUNT
            );

            pbap_appl_params.max_list_count = 0xFFFFU;


            LOG_DEBUG("Do you want to specify application parameters?(1/0): ");
            scanf("%d", &choice);
            if(1 == choice)
            {
                /* APPL_PARAM 1 : Order */
                LOG_DEBUG("Include appl. param 'Order'? (1/0): ");
                scanf("%d", &choice);
                if(1 == choice)
                {
                    PBAP_SET_APPL_PARAM_FLAG
                    (
                        pbap_appl_params.appl_param_flag,
                        PBAP_FLAG_ORDER
                    );

                    LOG_DEBUG("Select Oder param Value\n");
                    LOG_DEBUG ("    0 -> Indexed\n");
                    LOG_DEBUG ("    1 -> Alphanumeric\n");
                    LOG_DEBUG ("    2 -> Phonetic\n");

                    scanf("%d", &choice);

                    pbap_appl_params.order = (UINT8)choice;
                }

                LOG_DEBUG("Include appl. param 'Search' Attributes? (1/0): ");
                scanf("%d", &choice);
                if (1 == choice)
                {
                    /* APPL_PARAM 2 : Search Property */
                    PBAP_SET_APPL_PARAM_FLAG
                    (
                        pbap_appl_params.appl_param_flag,
                        PBAP_FLAG_SEARCH_ATTRIBUTE
                    );

                    LOG_DEBUG ("Select  Search property\n");
                    LOG_DEBUG ("    0 -> Name\n");
                    LOG_DEBUG ("    1 -> Number\n");
                    LOG_DEBUG ("    2 -> Sound\n");

                    scanf("%d", &choice);

                    pbap_appl_params.search_attr = (UINT8)choice;

                    /* APPL_PARAM 3 : Search Property */
                    PBAP_SET_APPL_PARAM_FLAG
                    (
                        pbap_appl_params.appl_param_flag,
                        PBAP_FLAG_SEARCH_VALUE
                    );

                    LOG_DEBUG("Enter the Search Value: ");
                    (BT_IGNORE_RETURN_VALUE) appl_pbap_pce_read_string(search_val, 20U);


                    pbap_appl_params.search_value.value = search_val;
                    pbap_appl_params.search_value.length =
                                                    (UINT16)BT_str_len(search_val);
                }

                /* APPL_PARAM 4 : MaxListCount */
                LOG_DEBUG("Include appl. param 'MaxListCount'? (1/0): ");
                scanf("%d", &choice);
                if (1 == choice)
                {
                    LOG_DEBUG("Enter the Count: \n");
                    LOG_DEBUG ("    0 -> Phone Book Size\n");
                    /* LOG_DEBUG("Enter value for MaxListCount (0-65535): "); */
                    scanf("%d", &choice);

                    pbap_appl_params.max_list_count = (UINT16)choice;

                }

                /* APPL_PARAM 5 : ListStartOffset */
                LOG_DEBUG("Include appl. param 'ListStartOffset'? (1/0): ");
                scanf("%d", &choice);
                if (1 == choice)
                {
                    PBAP_SET_APPL_PARAM_FLAG
                    (
                        pbap_appl_params.appl_param_flag,
                        PBAP_FLAG_LIST_START_OFFSET
                    );

                    LOG_DEBUG("Enter ListStartOffset value: ");
                    scanf("%d", &choice);
                    pbap_appl_params.list_start_offset = (UINT16)choice;
                }

#ifdef PBAP_1_2
                /* APPL_PARAM 6 : ResetNewMissedCalls */
                LOG_DEBUG("Include appl. param 'ResetNewMissedCalls'? (1/0): ");
                scanf("%d", &choice);
                if (1 == choice)
                {
                    PBAP_SET_APPL_PARAM_FLAG
                    (
                        pbap_appl_params.appl_param_flag,
                        PBAP_FLAG_RESET_NEW_MISSED_CALLS
                    );

                    /* LOG_DEBUG("Enter value for ResetNewMissedCalls: ");
                    scanf("%d", &choice); */
                    pbap_appl_params.reset_new_missed_calls = 0x01U; /* reset */
                }

                /* APPL_PARAM 7 : vCardSelector */
                LOG_DEBUG("Include appl. param 'vCardSelector'? (1/0): ");
                scanf("%d", &choice);
                if (1 == choice)
                {
                    PBAP_SET_APPL_PARAM_FLAG
                    (
                        pbap_appl_params.appl_param_flag,
                        PBAP_FLAG_VCARD_SELECTOR
                    );

                    PBAP_RESET_FILTER_FLAG
                    (
                        pbap_appl_params.vcard_selector
                    );

                    LOG_DEBUG("vCardSelector property 'FN'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_FN\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.vcard_selector,
                            PBAP_FLAG_FN
                        );
                    }

                    LOG_DEBUG("vCardSelector property 'N'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_N\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.vcard_selector,
                            PBAP_FLAG_N
                        );
                    }

                    LOG_DEBUG ("vCardSelector property 'TEL'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_TEL\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.vcard_selector,
                            PBAP_FLAG_TEL
                        );
                    }

                    LOG_DEBUG ("vCardSelector property 'EMAIL'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_EMAIL\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.vcard_selector,
                            PBAP_FLAG_EMAIL
                        );
                    }

                    LOG_DEBUG ("vCardSelector property 'PHOTO'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_PHOTO\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.vcard_selector,
                            PBAP_FLAG_PHOTO
                        );
                    }

                    LOG_DEBUG("Select property 'X-BT-UCI'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_X_BT_UCI\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.vcard_selector,
                            PBAP_FLAG_X_BT_UCI
                        );
                    }

                    LOG_DEBUG("Select property 'X-BT-UID'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_X_BT_UID\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.vcard_selector,
                            PBAP_FLAG_X_BT_UID
                        );
                    }
                }

                /* APPL_PARAM 8 : vCardSelectorOperator */
                LOG_DEBUG("Include appl. param 'vCardSelectorOperator'? (1/0): ");
                scanf("%d", &choice);
                if (1 == choice)
                {
                    PBAP_SET_APPL_PARAM_FLAG
                    (
                        pbap_appl_params.appl_param_flag,
                        PBAP_FLAG_VCARD_SELECTOR_OPERATOR
                    );

                    LOG_DEBUG("Enter value for vCardSelectorOperator\n");
                    LOG_DEBUG ("\t 0-> OR\n");
                    LOG_DEBUG ("\t 1-> AND\n");
                    LOG_DEBUG (": ");

                    scanf("%d", &choice);
                    pbap_appl_params.vcard_selector_operator = (UINT8)choice;
                }
#endif /* PBAP_1_2 */
            }

            req_info.appl_params = &pbap_appl_params;

            LOG_DEBUG("Sending Get Phone book List Request... ");
            retval = BT_pbap_pce_get_vcard_list
                    (
                        pbap_pce_handle,
                        &req_info
                    );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG(" Failed. Reason 0x%04X \n", retval);
            }
            else
            {
                LOG_DEBUG(" Successful. \n Wait for GET LIST Response... \n");

                /* If the request is not for Size, then open file for writing */
                if(PBAP_GET_APPL_PARAM_FLAG(
                    pbap_appl_params.appl_param_flag,
                    PBAP_FLAG_MAX_LIST_COUNT) &&
                    (0U == pbap_appl_params.max_list_count))
                {
                    LOG_DEBUG("Request is for phonebook size\n");
                }
                else
                {
                    appl_replace_special_chars(file_name);
                    BT_str_n_copy(file_name_listing, "listing_", sizeof(file_name_listing));
                    BT_str_n_cat(file_name_listing, file_name, (sizeof(file_name_listing) - BT_str_len(file_name_listing)));
                    BT_str_n_cat(file_name_listing, ".xml", (sizeof(file_name_listing) - BT_str_len(file_name_listing)));

                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)APPL_PBAP_FOLDER_REFERENCE,
                        file_name_listing,
                        pbap_file_object
                    );

                    LOG_DEBUG("Get List Response will be logged to file %s\n",
                        pbap_file_object);

                    /* Open a file to write the contents */
                    retval = BT_fops_file_open
                            (
                                pbap_file_object,
                                (UCHAR *)"wb",
                                &get_file_ptr
                            );
                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG(" Failed to open the file\n");
                        break;
                    }
                    else
                    {
                        get_file_ptr_active = 0x01U;
                    }
                }
            }
            break;

        case 23: /* Get vCard */
            LOG_DEBUG("Enter the vCard Name: ");

            BT_mem_set(&req_info, 0, sizeof(PBAP_REQUEST_STRUCT));
            name_req.length = appl_pbap_pce_read_string(file_name, 64U);
            name_req.value = file_name;
            req_info.name = &name_req;

            LOG_DEBUG("Enter number of wait stages: ");
            scanf("%d", &choice);
            wait_count = (UCHAR)choice;

            if (0U < wait_count)
            {
                req_info.wait = BT_TRUE;
                wait_count--;
            }
            else
            {
                req_info.wait = BT_FALSE;
            }

            /* Reset */
            PBAP_RESET_APPL_PARAM_FLAG(pbap_appl_params.appl_param_flag);

            LOG_DEBUG(" Do you want to specify application parameters?(1/0): ");
            scanf("%d", &choice);
            if(1 == choice)
            {
                /* APPL_PARAM 1 : Format */
                LOG_DEBUG("Include appl. param 'Format'? (1/0): ");
                scanf("%d", &choice);
                if(1 == choice)
                {
                    LOG_DEBUG("Select below\n");
                    LOG_DEBUG ("\t 0-> v2.1\n");
                    LOG_DEBUG ("\t 1-> v3.0\n");

                    scanf("%d", &choice);
                    pbap_appl_params.format = (UINT8)choice;
                    PBAP_SET_APPL_PARAM_FLAG
                    (
                        pbap_appl_params.appl_param_flag,
                        PBAP_FLAG_FORMAT
                    );
                }

                /* APPL_PARAM 2: Filter */
                LOG_DEBUG("Include appl. param 'PropertySelector'? (1/0): ");
                scanf("%d", &choice);
                if(1 == choice)
                {
                    PBAP_SET_APPL_PARAM_FLAG
                    (
                        pbap_appl_params.appl_param_flag,
                        PBAP_FLAG_FILTER
                    );

                    PBAP_RESET_FILTER_FLAG
                    (
                        pbap_appl_params.filter
                    );

                    LOG_DEBUG ("Select property 'N'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_N\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.filter,
                            PBAP_FLAG_N
                        );
                    }

                    LOG_DEBUG ("Select property 'TEL'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_TEL\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.filter,
                            PBAP_FLAG_TEL
                        );
                    }

                    LOG_DEBUG ("Select property 'EMAIL'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_EMAIL\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.filter,
                            PBAP_FLAG_EMAIL
                        );
                    }

                    LOG_DEBUG ("Select property 'ORG'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_ORG\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.filter,
                            PBAP_FLAG_ORG
                        );
                    }

                    LOG_DEBUG ("Select property 'PHOTO'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_PHOTO\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.filter,
                            PBAP_FLAG_PHOTO
                        );
                    }

                    LOG_DEBUG("Select property 'X-BT-UCI'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_X_BT_UCI\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.filter,
                            PBAP_FLAG_X_BT_UCI
                        );
                    }

                    LOG_DEBUG("Select property 'X-BT-UID'?(1/0): ");
                    scanf("%d", &choice);
                    if (1 == choice)
                    {
                        LOG_DEBUG("Setting Filter PBAP_FLAG_X_BT_UID\n");
                        PBAP_SET_FILTER_FLAG
                        (
                            pbap_appl_params.filter,
                            PBAP_FLAG_X_BT_UID
                        );
                    }

                    LOG_DEBUG("The Filter Flag is set to: Filter[0] 0x%04x \n\
                            Filter[1] 0x%04x\n Filter[2] 0x%04x\n Filter[3] 0x%04x\n",
                            pbap_appl_params.filter[0U],
                            pbap_appl_params.filter[1U],
                            pbap_appl_params.filter[2U],
                            pbap_appl_params.filter[3U]);
                }
            }

            req_info.appl_params = &pbap_appl_params;

            LOG_DEBUG("Sending Get vCard Request... ");
            retval = BT_pbap_pce_get_vcard
                     (
                         pbap_pce_handle,
                         &req_info
                     );
            if(API_SUCCESS != retval)
            {
                LOG_DEBUG(" Failed. Reason 0x%04X \n", retval);
            }
            else
            {
                LOG_DEBUG(" Successful. \n Wait for GET vCard Response... \n");
                appl_replace_special_chars(file_name);

                if (NULL == BT_str_str(file_name, ".vcf"))
                {
                    /* Name is UID */
                    BT_str_n_cat(file_name, ".vcf", (sizeof(file_name) - BT_str_len(file_name)));
                }

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)APPL_PBAP_FOLDER_REFERENCE,
                    file_name,
                    pbap_file_object
                );

                LOG_DEBUG("Phonebook contents sent by PSE will be dumped to file %s\n",
                        pbap_file_object);

                /* Open a file to write the contents */
                retval = BT_fops_file_open
                         (
                             pbap_file_object,
                             (UCHAR *)"wb",
                             &get_file_ptr
                         );
                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG(" Failed to open the file\n");
                    break;
                }
                else
                {
                    get_file_ptr_active = 0x01U;
                }
            }
            break;

        case 24: /* Abort */
            retval = BT_pbap_pce_abort(pbap_pce_handle);

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Abort Failed. Reason 0x%04X \n", retval);
            }
            else
            {
                LOG_DEBUG("Successfully sent the Abort Request \n");
            }
            break;

        case 30:
            LOG_DEBUG(" Enter the PIN code: ");

            if(0U == appl_pbap_pce_read_string(pin_code, 16U))
            {
                LOG_DEBUG("Invalid PIN \n");
                LOG_DEBUG("Choose 23 to disonnect the Transport connection.\n");
                break;
            }

            BT_mem_set(&connect_req_send, 0, sizeof(PBAP_CONNECT_STRUCT));
            pin.value = pin_code;
            pin.length = (UINT16)BT_str_len(pin_code);

            LOG_DEBUG ("User Id required (0/1)?\n");
            scanf ("%d", &choice);
            input = (UCHAR)choice;

            if (1U == input)
            {
                LOG_DEBUG(" Enter the User Id: ");

                if(0U == appl_pbap_pce_read_string(user_code, 20U))
                {
                    LOG_DEBUG("Invalid User Name \n");
                    LOG_DEBUG("Choose 23 to disonnect the Transport connection.\n");
                    break;
                }

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
            connect_req_send.bd_addr = pbap_appl_inst->connect_info.bd_addr;
            connect_req_send.server_ch_no =
                                pbap_appl_inst->connect_info.server_ch_no;

            connect_req_send.max_recv_size = APPL_PBAP_PCE_MAX_SIZE;

#ifdef PBAP_1_2
            pbap_appl_inst->connect_info.pbap_supported_features = APPL_PBAP_SUPPORTED_FEATURES;
#endif /* PBAP_1_2 */

            pbap_req_header_send.pbap_connect_info = &connect_req_send;

            retval = BT_pbap_pce_send_request
                    (
                        pbap_pce_handle,
                        PBAP_PCE_CONNECT_CFM,
                        PBAP_UNAUTH_RSP,
                        &pbap_req_header_send
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

        case 50:
            LOG_DEBUG("Enter the path: ");
            scanf("%s", path);
            EM_fops_list_directory(path);
            break;

        default:
            LOG_DEBUG("Invalid Option. Select Option from the Menu \n");
            break;
        } /* switch(choice) */

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    } /* BT_LOOP_FOREVER() */

    return;
}


/* Application Notification Callback */
API_RESULT appl_pbap_pce_event_callback
           (
               /* IN */  UINT8            event_type,
               /* IN */  UINT16           event_result,
               /* IN */  PBAP_HANDLE      pbap_handle,
               /* IN */  PBAP_HEADERS   * pbap_headers,
               /* IN */  UINT8            num_headers
           )
{
    UINT8                   send_request;
    UINT16                  bytes_written;
    API_RESULT              retval;
    PBAP_HEADERS            pbap_req_header;
    PBAP_REQUEST_STRUCT     req_info;

#ifdef PBAP_1_2
    UINT16 i;
#endif /* PBAP_1_2 */

    BT_IGNORE_UNUSED_PARAM(num_headers);

    /* Initialize */
    send_request = 0x01U;

    BT_mem_set(&pbap_req_header, 0, sizeof(PBAP_HEADERS));
    BT_mem_set(&req_info, 0, sizeof(PBAP_REQUEST_STRUCT));

    switch(event_type)
    {
    case PBAP_PCE_CONNECT_CFM:
        LOG_DEBUG("Received PBAP_PCE_CONNECT_CFM - ");
        print_pbap_event_result(event_result);

        if((PBAP_SUCCESS_RSP != event_result) &&
            (PBAP_UNAUTH_RSP != event_result))
        {
            break;
        }

        LOG_DEBUG("BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        pbap_headers->pbap_connect_info->bd_addr[0U],
        pbap_headers->pbap_connect_info->bd_addr[1U],
        pbap_headers->pbap_connect_info->bd_addr[2U],
        pbap_headers->pbap_connect_info->bd_addr[3U],
        pbap_headers->pbap_connect_info->bd_addr[4U],
        pbap_headers->pbap_connect_info->bd_addr[5U]);

#ifdef PBAP_1_2
        pbap_ser_psm = 0x0000U;
#endif /* PBAP_1_2 */

        if(PBAP_UNAUTH_RSP == event_result)
        {
            LOG_DEBUG("Choose 30 to enter the User ID and the Password:");
        }

        send_request = 0x00U;

        break;

    case PBAP_PCE_GET_PHONEBOOK_CFM:
        LOG_DEBUG("Received PBAP_PCE_GET_PHONEBOOK_CFM - ");
        print_pbap_event_result(event_result);

        if(NULL != pbap_headers->pbap_resp_info->appl_params)
        {
            if(0U != (PBAP_GET_APPL_PARAM_FLAG(
                pbap_headers->pbap_resp_info->appl_params->appl_param_flag,
                PBAP_FLAG_PHONE_BOOK_SIZE)))
            {
                LOG_DEBUG("PBAP_FLAG_PHONE_BOOK_SIZE: %d\n",
                    pbap_headers->pbap_resp_info->appl_params->phonebook_size);
            }

            if(0U != (PBAP_GET_APPL_PARAM_FLAG(
                pbap_headers->pbap_resp_info->appl_params->appl_param_flag,
                PBAP_FLAG_NEW_MISSED_CALLS)))
            {
                LOG_DEBUG("PBAP_FLAG_NEW_MISSED_CALLS: %d\n",
                  pbap_headers->pbap_resp_info->appl_params->new_missed_calls);
            }

#ifdef PBAP_1_2
            /* Primary Folder Version */
            if(0U != (PBAP_GET_APPL_PARAM_FLAG(
                pbap_headers->pbap_resp_info->appl_params->appl_param_flag,
                PBAP_FLAG_PRIMARY_FOLDER_VERSION)))
            {
                LOG_DEBUG("PBAP_FLAG_PRIMARY_FOLDER_VERSION: \n");

                LOG_DEBUG ("    - 0x");
                for (i = 0U; i < 16U; i++)
                {
                    LOG_DEBUG ("%02x",
                        ((UCHAR *)pbap_headers->pbap_resp_info->appl_params->primary_folder_ver)[i]);
                }
                LOG_DEBUG ("\n");
            }

            /* Secondary Folder Version */
            if(0U != (PBAP_GET_APPL_PARAM_FLAG(
                pbap_headers->pbap_resp_info->appl_params->appl_param_flag,
                PBAP_FLAG_SECONDARY_FOLDER_VERSION)))
            {
                LOG_DEBUG("PBAP_FLAG_SECONDARY_FOLDER_VERSION.\n");

                LOG_DEBUG ("    - 0x");
                for (i = 0U; i < 16U; i++)
                {
                    LOG_DEBUG ("%02x",
                        ((UCHAR *)pbap_headers->pbap_resp_info->appl_params->secondary_folder_ver)[i]);
                }
                LOG_DEBUG ("\n");
            }

            /* Database Identifier */
            if(0U != (PBAP_GET_APPL_PARAM_FLAG(
                pbap_headers->pbap_resp_info->appl_params->appl_param_flag,
                PBAP_FLAG_DATABASE_IDENTIFIER)))
            {
                LOG_DEBUG("PBAP_FLAG_DATABASE_IDENTIFIER.\n");

                LOG_DEBUG ("    - 0x");
                for (i = 0U; i < 16U; i++)
                {
                    LOG_DEBUG ("%02x",
                        ((UCHAR *)pbap_headers->pbap_resp_info->appl_params->database_identifier)[i]);
                }
                LOG_DEBUG ("\n");
            }
#endif /* PBAP_1_2 */
        }

        if((PBAP_SUCCESS_RSP == event_result) ||
           (PBAP_CONTINUE_RSP == event_result))
        {
            if((NULL != pbap_headers->pbap_resp_info->body) &&
               (pbap_headers->pbap_resp_info->body->length > 0U) &&
               (NULL != pbap_headers->pbap_resp_info->body->value))
            {
                if (0U != get_file_ptr_active) /* if 0x01 */
                {
                    retval = BT_fops_file_write
                             (
                                 pbap_headers->pbap_resp_info->body->value,
                                 pbap_headers->pbap_resp_info->body->length,
                                 get_file_ptr,
                                 &bytes_written
                             );

                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG(
                        "Error writing into file. Abort the operation.\n");

                        send_request = 0x00U;
                        break;
                    }
                }
            }
            if(PBAP_CONTINUE_RSP == event_result)
            {
                send_request = 0x01U;
            }
            else
            {
                send_request = 0x00U;
            }
        }
        else
        {
            send_request = 0x00U;
        }

        if((0U != get_file_ptr_active) &&
           (0x00U == send_request))
        {
             /* Close the file and inactivale the pointer */
             /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
             (void)BT_fops_file_close(get_file_ptr);
             get_file_ptr_active = 0x00U;
        }

        event_result = PBAP_SUCCESS_RSP;
        break;

    case PBAP_PCE_GET_VCARD_LIST_CFM:
        LOG_DEBUG("Received PBAP_PCE_GET_VCARD_LIST_CFM -");
        print_pbap_event_result(event_result);

        if(NULL != pbap_headers->pbap_resp_info->appl_params)
        {
            if(0U != (PBAP_GET_APPL_PARAM_FLAG(
                pbap_headers->pbap_resp_info->appl_params->appl_param_flag,
                PBAP_FLAG_PHONE_BOOK_SIZE)))
            {
                LOG_DEBUG("PBAP_FLAG_PHONE_BOOK_SIZE: %d\n",
                    pbap_headers->pbap_resp_info->appl_params->phonebook_size);

                /* found during upf-day-3-s-3-with-moto-watertown */
#if 0
                send_request = 0x00;
                break;
#endif /* 0 */
            }

            if(0U != (PBAP_GET_APPL_PARAM_FLAG(
                pbap_headers->pbap_resp_info->appl_params->appl_param_flag,
                PBAP_FLAG_NEW_MISSED_CALLS)))
            {
                LOG_DEBUG("PBAP_FLAG_NEW_MISSED_CALLS: %d\n",
                pbap_headers->pbap_resp_info->appl_params->new_missed_calls);
            }

#ifdef PBAP_1_2
            /* Primary Folder Version */
            if(0U != (PBAP_GET_APPL_PARAM_FLAG(
                pbap_headers->pbap_resp_info->appl_params->appl_param_flag,
                PBAP_FLAG_PRIMARY_FOLDER_VERSION)))
            {
                LOG_DEBUG("PBAP_FLAG_PRIMARY_FOLDER_VERSION.\n");

                LOG_DEBUG ("    - 0x");
                for (i = 0U; i < 16U; i++)
                {
                    LOG_DEBUG ("%02x",
                        ((UCHAR *)pbap_headers->pbap_resp_info->appl_params->primary_folder_ver)[i]);
                }
                LOG_DEBUG ("\n");
            }

            /* Secondary Folder Version */
            if(0U != (PBAP_GET_APPL_PARAM_FLAG(
                pbap_headers->pbap_resp_info->appl_params->appl_param_flag,
                PBAP_FLAG_SECONDARY_FOLDER_VERSION)))
            {
                LOG_DEBUG("PBAP_FLAG_SECONDARY_FOLDER_VERSION.\n");

                LOG_DEBUG ("    - 0x");
                for (i = 0U; i < 16U; i++)
                {
                    LOG_DEBUG ("%02x",
                        ((UCHAR *)pbap_headers->pbap_resp_info->appl_params->secondary_folder_ver)[i]);
                }
                LOG_DEBUG ("\n");
            }

            /* Database Identifier */
            if(0U != (PBAP_GET_APPL_PARAM_FLAG(
                pbap_headers->pbap_resp_info->appl_params->appl_param_flag,
                PBAP_FLAG_DATABASE_IDENTIFIER)))
            {
                LOG_DEBUG("PBAP_FLAG_DATABASE_IDENTIFIER.\n");

                LOG_DEBUG ("    - 0x");
                for (i = 0U; i < 16U; i++)
                {
                    LOG_DEBUG ("%02x",
                        ((UCHAR *)pbap_headers->pbap_resp_info->appl_params->database_identifier)[i]);
                }
                LOG_DEBUG ("\n");
            }
#endif /* PBAP_1_2 */
        }

        if((PBAP_SUCCESS_RSP == event_result) ||
           (PBAP_CONTINUE_RSP == event_result))
        {
            if((NULL != pbap_headers->pbap_resp_info->body) &&
               (pbap_headers->pbap_resp_info->body->length > 0U) &&
               (NULL != pbap_headers->pbap_resp_info->body->value))
            {
                if (0U != get_file_ptr_active) /* if 0x01 */
                {
                    retval = BT_fops_file_write
                             (
                                 pbap_headers->pbap_resp_info->body->value,
                                 pbap_headers->pbap_resp_info->body->length,
                                 get_file_ptr,
                                 &bytes_written
                             );

                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG(
                        "Error writing into file. Abort the operation.\n");

                        send_request = 0x00U;
                        break;
                    }
                    else
                    {
                        LOG_DEBUG("Successfully written into the File. \n");
                    }
                }
            }
        }

        if(PBAP_SUCCESS_RSP == event_result)
        {
            send_request = 0x00U;
        }
        else if(PBAP_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }
        else
        {
            send_request = 0x00U;
        }

        if((0U != get_file_ptr_active) && /* if 0x01 */
           (0x00U == send_request))
        {
             /* Close the file and inactivale the pointer */
             /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
             (void)BT_fops_file_close(get_file_ptr);
             get_file_ptr_active = 0x00U;
        }

        event_result = PBAP_SUCCESS_RSP;
        break;

    case PBAP_PCE_GET_VCARD_CFM:
        LOG_DEBUG("Received PBAP_PCE_GET_VCARD_CFM - ");
        print_pbap_event_result(event_result);

        if((NULL != pbap_headers->pbap_resp_info->body) &&
           (pbap_headers->pbap_resp_info->body->length > 0U) &&
           (NULL != pbap_headers->pbap_resp_info->body->value))
        {
            if (0U != get_file_ptr_active) /* if 0x01 */
            {
                retval = BT_fops_file_write
                         (
                             pbap_headers->pbap_resp_info->body->value,
                             pbap_headers->pbap_resp_info->body->length,
                             get_file_ptr,
                             &bytes_written
                         );

                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("Error writing into file. Abort the operation.\n");
                    send_request = 0x00U;
                }
            }
        }

#ifdef PBAP_1_2
        if(NULL != pbap_headers->pbap_resp_info->appl_params)
        {
            /* Database Identifier */
            if(0U != (PBAP_GET_APPL_PARAM_FLAG(
                pbap_headers->pbap_resp_info->appl_params->appl_param_flag,
                PBAP_FLAG_DATABASE_IDENTIFIER)))
            {
                LOG_DEBUG("PBAP_FLAG_DATABASE_IDENTIFIER.\n");

                LOG_DEBUG ("    - 0x");
                for (i = 0U; i < 16U; i++)
                {
                    LOG_DEBUG ("%02x",
                        ((UCHAR *)pbap_headers->pbap_resp_info->appl_params->database_identifier)[i]);
                }
                LOG_DEBUG ("\n");
            }
        }
#endif /* PBAP_1_2 */

        if(PBAP_SUCCESS_RSP == event_result)
        {
            LOG_DEBUG("Successfully written into the File. \n");
            send_request = 0x00U;
        }
        else if(PBAP_CONTINUE_RSP != event_result)
        {
            send_request = 0x00U;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if((0U != get_file_ptr_active) && /* if 0x01 */
           (0x00U == send_request))
        {
             /* Close the file and inactivale the pointer */
             /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
             (void)BT_fops_file_close(get_file_ptr);
             get_file_ptr_active = 0x00U;
        }

        event_result = PBAP_SUCCESS_RSP;
        break;

    case PBAP_PCE_SET_PHONEBOOK_CFM:
        LOG_DEBUG("Received PBAP_PCE_SET_PHONEBOOK_CFM - ");
        print_pbap_event_result(event_result);

        /* This is the Final response; no subsequent request shall be sent */
        send_request = 0x00U;
        break;

    case PBAP_PCE_ABORT_CFM:
        LOG_DEBUG("Received PBAP_PCE_ABORT_CFM -");
        print_pbap_event_result(event_result);

        /* This is the Final response; no subsequent request shall be sent */
        send_request = 0x00U;
        break;

    case PBAP_PCE_DISCONNECT_CFM:
        LOG_DEBUG("Received PBAP_PCE_DISCONNECT_CFM - ");
        print_pbap_event_result(event_result);

        /* This is the Final response; no subsequent request shall be sent */
        send_request = 0x00U;
        break;

    case PBAP_PCE_TRANSPORT_CLOSE_CFM: /* Fall Through */
    case PBAP_PCE_TRANSPORT_CLOSE_IND:

        LOG_DEBUG("Received %s - ", (PBAP_PCE_TRANSPORT_CLOSE_CFM == event_type) ?
             "PBAP_PCE_TRANSPORT_CLOSE_CFM":"PBAP_PCE_TRANSPORT_CLOSE_IND");
        print_pbap_event_result(event_result);

        /* This is the Final response; no subsequent request shall be sent */
        send_request = 0x00U;

        /* Close the file if opened */
        if (0U != get_file_ptr_active)
        {
            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close (get_file_ptr);
            get_file_ptr_active = 0x00U;
        }

        break;

    case PBAP_PCE_IDLE_TIMEOUT_IND:
        LOG_DEBUG("Received PBAP_PCE_IDLE_TIMEOUT_IND - ");
        print_pbap_event_result(event_result);

        /*
         * Disconnect Request is sent from the profile itself
         * No request is to be sent from here.
         */
        send_request = 0x00U;
        break;

    case PBAP_PCE_ERROR_IND:
        LOG_DEBUG("Received PBAP_PCE_ERROR_IND - ");
        print_pbap_event_result(event_result);
        send_request = 0x00U;
        break;

    default:
        LOG_DEBUG("Received Unknown PBAP PCE Event Type: 0x%02x. Result: 0x%04X",
        event_type, event_result);

        send_request = 0x00U;
        break;
    }

    if(0x01U == send_request)
    {
        if (0U < wait_count)
        {
            req_info.wait = BT_TRUE;
            pbap_req_header.pbap_req_info = &req_info;

            wait_count--;
        }

        retval = BT_pbap_pce_send_request
                 (
                     pbap_handle,
                     event_type,
                     event_result,
                     &pbap_req_header
                 );

        if(API_SUCCESS != retval)
        {
            LOG_DEBUG("Send Request Result: 0x%04X", retval);
        }
    }

    return API_SUCCESS;
}


/*
 * Callback Function Registered with SDP,
 * Using SDP_SET_HANDLE() and BT_sdp_open ()
 */
void appl_pbap_pce_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     )
{
    static UCHAR pbap_attrib_data[50U];
    UINT16 pbap_len_attrib_data = 50U;

    API_RESULT retval;
    UINT16 num_attrib_ids;

    S_UUID uuid[3U];

#ifndef PBAP_1_2
    UINT16 attrib_id[2U] = {PROTOCOL_DESC_LIST,
                           PBAP_SUPPORTED_REPOSITORIES};

    num_attrib_ids = 2U;
#else /* PBAP_1_2 */
    UINT16 attrib_id[4U] = {PROTOCOL_DESC_LIST,
                           GOEP_L2CAP_PSM_ATTR_ID,
                           PBAP_SUPPORTED_REPOSITORIES,
                           PBAP_SUPPORTED_FEATURES_ATTR_ID};

    num_attrib_ids = 4U;
#endif /* PBAP_1_2 */

    BT_IGNORE_UNUSED_PARAM(length);

    /*
     * Frame the SDP Search pattern with following UUIDs
     * - PHONEBOOK_PSE_UUID
     * - RFCOMM_UUID
     * - L2CAP_UUID
     * and only one Attribute ID "PROTOCOL_DESC_LIST"
     * (For getting the RFCOMM Server Channel on which Peer PBAP Service
     *  is Listening)
     *
     */
    uuid[0U].uuid_type = UUID_16;
    uuid[1U].uuid_type = UUID_16;
    uuid[2U].uuid_type = UUID_16;
    uuid[0U].uuid_union.uuid_16= PHONEBOOK_PSE_UUID;
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
                     &sdp_pbap_handle,
                     uuid,
                     3U,
                     attrib_id /* Protocol Desc*/,
                     num_attrib_ids,
                     NULL,
                     0U,
                     pbap_attrib_data,
                     &pbap_len_attrib_data
                 );
        if(API_SUCCESS != retval)
        {
            LOG_DEBUG("SDP SEARCH Failed for PBAP..Closing SDP\n");
            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_pbap_handle);
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
            UINT32 attr_val_4B;
            UINT16 attr_size;
            UCHAR attr_val[4U];

#ifdef PBAP_1_2
            UINT16 attr_val_2B;
            UINT16 psm;
#endif /* PBAP_1_2 */

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

#ifdef PBAP_1_2
                psm = 0x0000U;

                /* Check for OBEX L2CAP PSM for PBAP in the Protocol Descriptor List */
                retval = BT_sdp_get_psm (sdp_query_result, &psm);
                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("SDP FAILED to find PSM in Protocol Descriptor List\n");
                }
                else
                {
                    pbap_ser_psm = psm;
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

                        pbap_ser_psm = attr_val_2B;
                    }
                }

                /* if ((API_SUCCESS != retval) && (RFCOMM_PSM == psm)) */
#endif /* PBAP_1_2 */
                {
                    /* No PSMs found, check the PBAP server channel number */
                    retval = BT_sdp_get_channel_number
                             (
                                 sdp_query_result,
                                 &pbap_ser_channel
                             );
                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG("SDP FAILED to find PBAP Server Channel\n");
                    }
                    else
                    {
                        LOG_DEBUG("Server Channel: 0x%02X\n", pbap_ser_channel);

                        /*
                         * Store the Server Channel globally.
                         * Later it will be used while Creating PBAP Connection
                         */
                    }
                }

                /* PBAP PSM or Server channel found */
                if (API_SUCCESS == retval)
                {
                    LOG_DEBUG ("PBAP Service Found, Getting Attributes... \n");

                    attr_val_1B = 0U;
                    attr_size = 1U;
                    /* Get the PBAP Supported Repositories */
                    retval = BT_sdp_get_attr_value
                             (
                                 PBAP_SUPPORTED_REPOSITORIES,
                                 sdp_query_result,
                                 attr_val,
                                 &attr_size
                             );
                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG("SDP FAILED to find PBAP Supported Repos\n");
                    }
                    else
                    {
                        BT_UNPACK_LE_1_BYTE(&attr_val_1B, attr_val);
                        LOG_DEBUG("Supported Repos: 0x%02X\n", attr_val_1B);
                    }

                    attr_val_4B = 0U;
                    attr_size = 4U;
                    /* Get the PBAP Supported Features */
                    retval = BT_sdp_get_attr_value
                             (
                                 PBAP_SUPPORTED_FEATURES_ATTR_ID,
                                 sdp_query_result,
                                 attr_val,
                                 &attr_size
                             );
                    if(API_SUCCESS != retval)
                    {
                        LOG_DEBUG("SDP FAILED to find PBAP Supported Features\n");
                    }
                    else
                    {
                        BT_UNPACK_LE_4_BYTE(&attr_val_4B, attr_val);
                        LOG_DEBUG("Supported Features: 0x%08X\n", attr_val_4B);
                    }
                }
                else
                {
                    LOG_DEBUG ("Failed to find PBAP Service.\n");
                }

                no_server_channels++;
            }
        }

        /* Now Close the SDP Connection */
        (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_pbap_handle);
        break;

    default: /* Events not needed to be Handled */
        LOG_DEBUG("Unhandled SDP Event (0x%02X)\n", command);
        break;

    }/* switch */

    BT_IGNORE_UNUSED_PARAM(pbap_ser_psm);
    return;
}


/* To read the user input string of length "max_len" */
UINT16 appl_pbap_pce_read_string(UCHAR *str, UINT16 max_len)
{
    UINT16 count;

    BT_IGNORE_UNUSED_PARAM(max_len);

    /* Init */
    count = 0U;

    if((NULL == str) || (max_len <= 1U))
    {
        LOG_DEBUG("Invalid parameters to read string \n");
        /* return 0U; */
    }
    else
    {
        scanf("%s", str);
        count = (UINT16)BT_str_len(str);
    }

    return count;
}

void appl_replace_special_chars(UCHAR* str)
{
    UINT32 n;

    for(n = 0U; n < BT_str_len(str); n++)
    {
        if(('/' == str[n]) ||
           ('-' == str[n]) ||
           (':' == str[n]))
        {
            str[n] = '_';
        }
    }

    return;
}

void print_pbap_event_result(UINT16 event_result)
{
    switch(event_result)
    {
    case PBAP_SUCCESS_RSP:
        LOG_DEBUG("Success(0x%04X)\n", event_result);
        break;
    case PBAP_CONTINUE_RSP:
        LOG_DEBUG("Continue(0x%04X)\n", event_result);
        break;
    case PBAP_BAD_REQ_RSP:
        LOG_DEBUG("Bad Request(0x%04X)\n", event_result);
        break;
    case PBAP_NOT_IMPLEMENTED_RSP:
        LOG_DEBUG("Not Implemented(0x%04X)\n", event_result);
        break;
    case PBAP_UNAUTH_RSP:
        LOG_DEBUG("Unauthorized(0x%04X)\n", event_result);
        break;
    case PBAP_PRECOND_FAILED_RSP:
        LOG_DEBUG("Pre-condition Failed(0x%04X)\n", event_result);
        break;
    case PBAP_NOT_FOUND_RSP:
        LOG_DEBUG("Not Found(0x%04X)\n", event_result);
        break;
    case PBAP_NOT_ACCEPTABLE_RSP:
        LOG_DEBUG("Not Acceptable(0x%04X)\n", event_result);
        break;
    case PBAP_NO_SERVICE_RSP:
        LOG_DEBUG("No Service(0x%04X)\n", event_result);
        break;
    case PBAP_FORBIDDEN_RSP:
        LOG_DEBUG("Forbidden(0x%04X)\n", event_result);
        break;
    default:
        LOG_DEBUG("Error(0x%04X)\n", event_result);
        break;
    }

    return;
}

#endif /* PBAP_PCE */
