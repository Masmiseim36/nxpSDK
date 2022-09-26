
/**
 *  \file appl_ctn_cce.c
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_ctn.h"
#include "appl_utils.h"

#ifdef CTN_CCE

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
static UCHAR                ctnc_bd_addr[BT_BD_ADDR_SIZE];
static UINT16               connection_handle;

static UCHAR                ctn_app_instance;
static UINT16               ctn_l2cap_psm;
static UINT32               ctn_supp_feat;

static CTN_CAS_INSTANCE     cas_instance[CTN_CLIENT_NUM_INSTANCES];
static CTN_INSTANCE         cns_instance[CTN_CLIENT_NUM_INSTANCES];

static UCHAR                ctn_file_object[CTN_MAX_FILE_OBJ_NAME_LEN];
static BT_fops_file_handle  fp;
static UINT32               fsize;
static UINT32               remaining, sent;
static UINT16               ctn_xchg_size;
static UCHAR                ctn_push_started;

static BT_fops_file_handle  ctn_rx_fp;

static BT_fops_file_handle  ctn_list_fp;

/* Indicator for first response packet for any request */
static UCHAR                appl_ctn_cce_start_of_response;

/* SRMP wait count */
static UCHAR srmp_wait_count;

static const UCHAR ctn_cce_menu[] =
" \n \
-------------- CTN Client Menu --------------\n \
\n \
\t 0. Exit \n \
\t 1. Refresh \n \
\t 2. Register Peer BD Addrs \n\
\t 3. Get Registered Peer BD Addrs. \n\
\n \
\t 5. CCE Initialize \n \
\t 6. CCE Shutdown \n \
\t 7. CCE Start \n \
\t 8. CCE Stop \n \
\n \
\t10. Create ACL Connection\n\
\t11. ACL Disconnection\n\
\t12. Do SDP Query \n\
\n \
\t15. Connect to CSE \n \
\t16. Disconnect from CSE \n \
\t17. Close CAS transport with CSE \n \
\t18. Close CNS transport with CSE \n \
\n \
\t20. Get CTN Listing \n \
\t21. Get CTN Object \n \
\t22. Set CTN Status \n \
\t23. Push CTN Object \n \
\t24. Forward CTN Object \n \
\t25. CTN Get Acount Information \n \
\t26. Sync CTN Acount \n \
\n \
\t28. Register CTN Notification \n \
\n\
\t30. CCE Set number of SRMP wait stages \n\
\n\
\t40. Sync CTN Instance.\n\
\n\
\t50. List Directory \n\
\n \
Your Choice: ";

/* --------------------------------------------- Functions */


void main_ctn_cce_operations (void)
{
    API_RESULT retval;
    UINT8 i;
    UINT8 *phandle;
    UCHAR *pbd_addr;
    UINT16 actual;

    CTN_CONNECT_STRUCT connect_info;
    CTN_REQUEST_STRUCT get_info, set_info, put_info;
    CTN_HEADER_STRUCT name_req, description_req, body_req;
    CTN_APPL_PARAMS appl_param;

    UCHAR name[32U];
    UCHAR appl_ctn_put_object_name[64U];
    UCHAR description_info[64U];
    UCHAR period_begin[32U], period_end[32U];
    UINT16 len;
    UCHAR more;

    CHAR path[BT_FOPS_MAX_DIRECTORY_SIZE];

    int choice, menu_choice, cn_handle, val, handle;

    more = 0U;

    BT_LOOP_FOREVER()
    {
        printf ("%s", ctn_cce_menu);
        scanf ("%d", &choice);

        menu_choice = choice;

        switch (choice)
        {
            case 0:
                break; /* return; */

            case 1:
                break;

            case 2: /* Register CTN CSE's BD addrs */
                LOG_DEBUG("Enter CTN CSE's Bluetooth Device Address\n");

                /* Read the BD_ADDR of Remote Device */
                appl_get_bd_addr(ctnc_bd_addr);
                break;

            case 3:
                /* Get Registered BD_ADDR of Peer Device */
                LOG_DEBUG("\nRegistered Peer BD "\
                BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
                BT_DEVICE_ADDR_ONLY_PRINT_STR(ctnc_bd_addr));
                break;

            case 5:
                LOG_DEBUG ("Initializing CTN CCE...\n");
                retval = BT_ctn_cce_init ();
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                if (API_SUCCESS == retval)
                {
                    /* Reset the CTN handle */
                    for (i = 0U; i < CTN_CLIENT_NUM_INSTANCES; i++)
                    {
                        CTN_RESET_INSTANCE(&(cas_instance[i].instance));
                        cas_instance[i].cns_id = CTN_ENTITY_INVALID;

                        CTN_RESET_INSTANCE(&cns_instance[i]);
                    }
                }

                ctn_cce_print_appl_instances();
                break;

            case 6:
                LOG_DEBUG ("Shutting down CTN CCE...\n");
                retval = BT_ctn_cce_shutdown ();
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 7:
                ctn_cce_print_appl_instances();

                LOG_DEBUG ("Enter Service Type (1-CAS, 2-CNS): ");
                scanf ("%d", &val);

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                if (CTN_CLIENT_NUM_INSTANCES <= handle)
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

                LOG_DEBUG ("Starting CTN CCE Instance...\n");
                retval = BT_ctn_cce_start
                         (
                             (CTN_SERVICE_TYPE)val,
                             appl_ctn_cce_callback,
                             phandle
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                if (API_SUCCESS != retval)
                {
                    *phandle = CTN_ENTITY_INVALID;
                }

                ctn_cce_print_appl_instances();
                break;

            case 8:
                ctn_cce_print_appl_instances();

                LOG_DEBUG ("Enter Service Type (1-CAS, 2-CNS): ");
                scanf ("%d", &val);

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

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

                LOG_DEBUG ("Stopping CTN CCE Service %d...\n", val);
                retval = BT_ctn_cce_stop ((CTN_SERVICE_TYPE)val, phandle);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

                if (API_SUCCESS == retval)
                {
                    *phandle = CTN_ENTITY_INVALID;
                    BT_mem_set (pbd_addr, 0x00, BT_BD_ADDR_SIZE);

                    /* Close any file objects */
                    if (NULL != fp)
                    {
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_fops_file_close (fp);
                        fp = NULL;
                    }

                    if (NULL != ctn_list_fp)
                    {
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_fops_file_close (ctn_list_fp);
                        ctn_list_fp = NULL;
                    }
                }

                ctn_cce_print_appl_instances();
                break;

            case 10:
    #if 0
                /*
                 * First Application should establish ACL Connection with
                 * the peer Device
                 */
                LOG_DEBUG("Enter CTN CSE's Bluetooth Device Address: ");

                /* Read the BD_ADDR of Remote Device */
                appl_get_bd_addr(bd_addr);
    #endif /* 0 */

                retval = BT_hci_create_connection
                         (
                             ctnc_bd_addr,
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

                LOG_DEBUG ("Select CCE Entity Instance: ");
                scanf ("%d", &cn_handle);
                ctn_app_instance = (UCHAR )cn_handle;

    #if 0
                LOG_DEBUG ("Enter peer device address: ");
                appl_get_bd_addr (bd_addr);
    #endif /* 0 */

                cas_instance[ctn_app_instance].instance.ctn_sdp_record.ctn_len_attrib_data =
                      CTN_SDP_RECORD_DATA_SIZE;

                SDP_SET_HANDLE
                (
                    cas_instance[ctn_app_instance].instance.ctn_sdp_record.ctn_sdp_handle,
                    ctnc_bd_addr,
                    appl_ctn_cce_sdp_callback
                );

                retval = BT_sdp_open(&cas_instance[ctn_app_instance].instance.ctn_sdp_record.ctn_sdp_handle);

                if ( retval != API_SUCCESS )
                {
                    LOG_DEBUG("SDP Connect FAILED !! Error Code = 0x%04X\n", retval);
                }

                LOG_DEBUG("Please Wait for SDP Operation to Complete\n");
                break;

            case 15:
                ctn_cce_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

    #if 0
                connect_info.bd_addr = bd_addr;

                /* L2CAP PSM */
                connect_info.l2cap_rpsm = ctn_l2cap_psm;
                connect_info.max_recv_size = 512;
    #else

    #if 0
                LOG_DEBUG ("Enter peer device address: ");
                appl_get_bd_addr (bd_addr);
    #endif /* 0 */

                connect_info.bd_addr = ctnc_bd_addr;

                LOG_DEBUG ("Enter Remote PSM(in Hex): ");
                scanf ("%x", &val);
                connect_info.l2cap_rpsm = (UINT16)val;
                fflush(stdin);

                LOG_DEBUG ("Enter Data Exchange Size: ");
                scanf ("%d", &val);
                connect_info.max_recv_size = (UINT16)val;
    #endif /* 0 */

                LOG_DEBUG ("Connecting...\n");
                retval = BT_ctn_cce_connect
                            (
                                &cas_instance[handle].instance.handle,
                                &connect_info
                            );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 16:
                ctn_cce_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                LOG_DEBUG ("Disconnecting on CTN CCE Instance %d\n", handle);
                retval = BT_ctn_cce_disconnect (&cas_instance[handle].instance.handle);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 17:
                ctn_cce_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                LOG_DEBUG ("Closing on CTN CCE Instance %d\n", handle);
                retval = BT_ctn_cce_transport_close (&cas_instance[handle].instance.handle);
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 18:
                ctn_cce_print_appl_instances();

                LOG_DEBUG("Select Application Instance: ");
                scanf("%d", &handle);

                LOG_DEBUG("Closing on CTN CCE Instance %d\n", handle);
                retval = BT_ctn_cce_ns_transport_close (&cas_instance[handle].instance.handle);
                LOG_DEBUG("Retval - 0x%04X\n", retval);
                break;

            case 20: /* GET CTNListing */
                ctn_cce_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                CTN_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

                LOG_DEBUG("Enter number of SRMP wait stages: ");
                scanf("%d", &choice);
                srmp_wait_count = (UCHAR)choice;

                if (0U < srmp_wait_count)
                {
                    get_info.wait = BT_TRUE;
                    srmp_wait_count--;
                }
                else
                {
                    get_info.wait = BT_FALSE;
                }

                LOG_DEBUG ("Enter Name of the Folder: ");
                scanf ("%s", name);
                len = (UINT16)(BT_str_len (name) + 1U);

                /* Update request info */
                name_req.length = len;
                name_req.value = name;

                /* Update request info */
                get_info.name = &name_req;
                get_info.appl_params = &appl_param;

                LOG_DEBUG ("Specify Application Parameters? (1/0): ");
                scanf ("%d", &choice);
                if (0U == choice)
                {
                    /* Reset app_parmams */
                    get_info.appl_params = NULL;

                    LOG_DEBUG ("Requesting to Get CTN Listing...\n");
                    retval = BT_ctn_cce_get_listing
                                (
                                    &cas_instance[handle].instance.handle,
                                    &get_info
                                );
                    LOG_DEBUG ("Retval - 0x%04X\n", retval);

                    break;
                }

                LOG_DEBUG ("Include Appl Param 'MaxListCount' (1/0): ");
                scanf ("%d", &choice);
                if (0U != choice)
                {
                    CTN_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        CTN_FLAG_MAXLISTCOUNT
                    );

                    LOG_DEBUG ("Enter MaxListCount value: ");
                    scanf ("%d", &val);

                    appl_param.max_list_count = (UINT16) val;
                }

                LOG_DEBUG ("Include Appl Param 'ListStartOffset' (1/0): ");
                scanf ("%d", &choice);
                if (0U != choice)
                {
                    CTN_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        CTN_FLAG_LISTSTARTOFFSET
                    );

                    LOG_DEBUG ("Enter ListStartoffset value: ");
                    scanf ("%d", &val);

                    appl_param.list_start_offset = (UINT16) val;
                }

                LOG_DEBUG ("Include Appl Param 'FilterPeriodBegin' (1/0): ");
                scanf ("%d", &choice);
                if (0U != choice)
                {
                    CTN_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        CTN_FLAG_FILTERPERIODBEGIN
                    );

                    LOG_DEBUG ("Enter FilterPeriodBegin Value: ");
                    scanf ("%s", period_begin);

                    appl_param.filter_period_begin.value = period_begin;
                    appl_param.filter_period_begin.length = (UINT16)(BT_str_len(period_begin) + 1U);
                }

                LOG_DEBUG ("Include Appl Param 'FilterPeriodEnd' (1/0): ");
                scanf ("%d", &choice);
                if (0U != choice)
                {
                    CTN_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        CTN_FLAG_FILTERPERIODEND
                    );

                    LOG_DEBUG ("Enter FilterPeriodEnd Value: ");
                    scanf ("%s", period_end);

                    appl_param.filter_period_end.value = period_end;
                    appl_param.filter_period_end.length = (UINT16)(BT_str_len(period_end) + 1U);
                }

                LOG_DEBUG ("Include Appl Param 'ParameterMask' (1/0): ");
                scanf ("%d", &choice);
                if (0U != choice)
                {
                    CTN_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        CTN_FLAG_PARMETERMASK
                    );

                    /* Enter 0x03FF for all bit masks */
                    LOG_DEBUG ("Enter ParameterMask Value(in Hex): ");
                    scanf ("%x", &appl_param.parameter_mask);
                }

                LOG_DEBUG ("Requesting to Get CTN Listing...\n");
                retval = BT_ctn_cce_get_listing
                            (
                                &cas_instance[handle].instance.handle,
                                &get_info
                            );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 21: /* Get CTN Object */
                ctn_cce_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                CTN_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

                /* Update request info */
                get_info.appl_params = &appl_param;

                LOG_DEBUG("Enter number of SRMP wait stages: ");
                scanf("%d", &choice);
                srmp_wait_count = (UCHAR)choice;

                if (0U < srmp_wait_count)
                {
                    get_info.wait = BT_TRUE;
                    srmp_wait_count--;
                }
                else
                {
                    get_info.wait = BT_FALSE;
                }

                /* TODO: See how to get this 16 digit UNICODE input */
                LOG_DEBUG ("Enter Object handle to be got: ");
                scanf ("%s", name);

                /* Update request info */
                name_req.length = (UINT16)(BT_str_len(name) + 1U);
                name_req.value = name;
                get_info.name = &name_req;

                /* Update application hdr params */
                LOG_DEBUG ("Include Appl Param 'Recurrent' (1/0): ");
                scanf ("%d", &choice);
                if (0U != choice)
                {
                    CTN_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        CTN_FLAG_RECURRENT
                    );

                    LOG_DEBUG ("Enter Recurrent value\n");
                    LOG_DEBUG ("\t0->no\n");
                    LOG_DEBUG ("\t1->yes\n:");
                    scanf ("%d", &val);
                    appl_param.recurrent = (UINT8)val;
                }

                CTN_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    CTN_FLAG_ATTACHMENT
                );

                LOG_DEBUG ("Enter Attachment value\n");
                LOG_DEBUG ("\t0->on\n");
                LOG_DEBUG ("\t1->off\n");
                LOG_DEBUG ("\t2->select\n:");
                scanf ("%d", &val);
                appl_param.attachment = (UINT8)val;

                if (0x02U == appl_param.attachment)
                {
                    CTN_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        CTN_FLAG_ATTACHID
                    );

                    LOG_DEBUG ("Enter Attach ID: ");
                    scanf ("%d", &val);
                    appl_param.attach_id = (UINT8)val;
                }

                LOG_DEBUG ("Requesting to Get CTN Object...\n");
                retval = BT_ctn_cce_get_object
                            (
                                &cas_instance[handle].instance.handle,
                                &get_info
                            );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 22: /* Set CTNStatus */
                ctn_cce_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                CTN_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

                /* Update request info */
                set_info.appl_params = &appl_param;

                /* TODO: See how to get this 16 gigit UNICODE input */
                LOG_DEBUG ("Enter Message handle to be set: ");
                scanf ("%s", name);

                /* Update request info */
                name_req.length = (UINT16)(BT_str_len(name) + 1U);
                name_req.value = name;
                set_info.name = &name_req;

                CTN_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    CTN_FLAG_STATUSINDICATOR
                );

                LOG_DEBUG ("Enter StatusIndicator value.\n");
                LOG_DEBUG ("\t0 -> pStatus\n");
                LOG_DEBUG ("\t1 -> alarmStatus\n");
                LOG_DEBUG ("\t2 -> sendStatus\n");
                LOG_DEBUG ("\t3 -> deleteStatus\n");
                scanf ("%d", &val);
                appl_param.status_indicator = (UINT8)val;

                CTN_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    CTN_FLAG_STATUSVALUE
                );

                LOG_DEBUG ("Enter StatusValue value: \n");
                LOG_DEBUG ("\t0 -> no\n");
                LOG_DEBUG ("\t1 -> yes\n");
                LOG_DEBUG ("\t2 -> postpone\n");
                LOG_DEBUG ("\t3 -> tentative\n");
                LOG_DEBUG ("\t4 -> need-action\n");
                LOG_DEBUG ("\t5 -> accepted\n");
                LOG_DEBUG ("\t6 -> declined\n");
                LOG_DEBUG ("\t7 -> delegated\n");
                LOG_DEBUG ("\t8 -> completed\n");
                LOG_DEBUG ("\t9 -> in-progress\n");
                scanf ("%d", &val);

                appl_param.status_value = (UINT8)val;

                if((CTN_STATUSINDICATOR_ALARMSTATUS == appl_param.status_indicator) &&
                    (CTN_STATUSVALUE_POSTPONE == appl_param.status_value))
                {

                    /* Based on Status Indicator and Status Value, Update the
                        * Postpone Value.
                        */
                    CTN_SET_APPL_PARAM_FLAG
                    (
                        appl_param.appl_param_flag,
                        CTN_FLAG_POSTPONEVAL
                    );
                    LOG_DEBUG ("Enter Postpone value: ");
                    scanf ("%d", &val);
                    appl_param.postpone_val = (UINT32)val;
                }

                LOG_DEBUG ("Requesting Set CTN Status...\n");
                retval = BT_ctn_cce_set_status
                            (
                                &cas_instance[handle].instance.handle,
                                &set_info
                            );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 23: /* PUSH CTN Object */
                ctn_cce_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                CTN_INIT_HEADER_STRUCT(name_req);
                CTN_INIT_HEADER_STRUCT(body_req);
                CTN_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

                /* Update request info */
                put_info.appl_params = &appl_param;

                LOG_DEBUG("Enter number of SRMP wait stages: ");
                scanf("%d", &choice);
                srmp_wait_count = (UCHAR)choice;

                if (0U < srmp_wait_count)
                {
                    put_info.wait = BT_TRUE;
                    srmp_wait_count--;
                }
                else
                {
                    put_info.wait = BT_FALSE;
                }

                LOG_DEBUG ("Enter Folder Name: ");
                scanf ("%s", name);
                len = (UINT16)(BT_str_len(name) + 1U);

                /* Update Name info */
                name_req.length = len;
                name_req.value = name;
                put_info.name = &name_req;

                CTN_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    CTN_FLAG_SEND
                );
                LOG_DEBUG ("Enter Send Value\n");
                LOG_DEBUG ("\t1->Yes\n");
                LOG_DEBUG ("\t0->No\n: ");
                scanf ("%d", &val);
                appl_param.send = (UINT8)val;

                fsize = 0U;
                remaining = 0U;
                sent = 0U;

                LOG_DEBUG ("Enter put object name: ");
                scanf ("%s", appl_ctn_put_object_name);

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)APPL_CTN_ROOT_FOLDER_REFERENCE,
                    appl_ctn_put_object_name,
                    ctn_file_object
                );

                /* Open the file to be pushed */
                retval = BT_fops_file_open (ctn_file_object, (UCHAR *)"rb", &fp);
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

                if (remaining > ctn_xchg_size)
                {
                    body_req.length = ctn_xchg_size;
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

                put_info.body = &body_req;

                actual = 0;
                LOG_DEBUG ("Requesting to Push CTN Object...\n");
                retval = BT_ctn_cce_push_object
                            (
                                &cas_instance[handle].instance.handle,
                                &put_info,
                                more,
                                &actual
                            );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);

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

                if (NULL != body_req.value)
                {
                    BT_free_mem (body_req.value);
                    body_req.value = NULL;
                }
                break;

            case 24: /* Forward CTN Object */
                ctn_cce_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                CTN_INIT_HEADER_STRUCT(name_req);
                CTN_INIT_HEADER_STRUCT(description_req);

                /* TODO: See how to get this 16 gigit UNICODE input */
                LOG_DEBUG ("Enter object handle to forward: ");
                scanf ("%s", name);

                /* Update Name info */
                name_req.length = (UINT16)(BT_str_len(name) + 1U);
                name_req.value = name;
                put_info.name = &name_req;

                /* Update Description info */
                LOG_DEBUG ("Enter Object Description: ");
                scanf ("%s", description_info);
                description_req.length = (UINT16)(BT_str_len(description_info) + 1U);
                description_req.value = description_info;
                put_info.description = &description_req;

                LOG_DEBUG ("Requesting to Forward CTN Object...\n");
                retval = BT_ctn_cce_forward_object
                         (
                             &cas_instance[handle].instance.handle,
                             &put_info
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 25: /* Get CTN Account Information */
                ctn_cce_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                CTN_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

                /* Update request info */
                get_info.appl_params = &appl_param;

                CTN_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    CTN_FLAG_INSTANCEID
                );

                LOG_DEBUG ("Enter InstanceID: ");
                scanf ("%d", &val);
                appl_param.cas_instance_id = (UINT8)val;

                LOG_DEBUG ("Requesting to Get CTN Account Information...\n");
                retval = BT_ctn_cce_get_account_information
                         (
                             &cas_instance[handle].instance.handle,
                             &get_info
                         );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 26: /* Sync CTN Account */
                ctn_cce_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                LOG_DEBUG ("Requesting to Sync Account...\n");
                retval = BT_ctn_cce_sync_account
                            (
                                &cas_instance[handle].instance.handle
                            );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
                break;

            case 28:
    #ifdef CTN_SUPPORT_NOTIFICATION
                ctn_cce_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                CTN_RESET_APPL_PARAM_FLAG(appl_param.appl_param_flag);

                LOG_DEBUG ("Enter Notification Status (1->on, 0->off): ");
                scanf ("%d", &choice);

                appl_param.notification_status = (UINT8)choice;

                CTN_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    CTN_FLAG_NOTIFICATIONSTATUS
                );

                LOG_DEBUG ("Enter Acoustic Alarm Status (1->on, 0->off): ");
                scanf ("%d", &choice);

                appl_param.acoustic_alarm_status = (UINT8)choice;

                CTN_SET_APPL_PARAM_FLAG
                (
                    appl_param.appl_param_flag,
                    CTN_FLAG_ACOUSTICALARMSTATUS
                );

                /* Update request info */
                set_info.appl_params = &appl_param;

                LOG_DEBUG ("Requesting Notification Registration...\n");
                retval = BT_ctn_cce_set_ntf_registration
                            (
                                &cas_instance[handle].instance.handle,
                                &set_info
                            );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
    #else
                LOG_DEBUG ("CTN Notification NOT Enabled\n");
    #endif /* CTN_SUPPORT_NOTIFICATION */
                break;

            case 30:
                LOG_DEBUG("Enter number of SRMP wait stages: ");
                scanf("%d", &choice);
                srmp_wait_count = (UCHAR)choice;
                LOG_DEBUG("SRMP wait count entered  is : %d\n",srmp_wait_count);
                break;

            case 40:
    #ifdef HAVE_CTN_SYNC_INSTANCE
                ctn_cce_print_appl_instances();

                LOG_DEBUG ("Select Application Instance: ");
                scanf ("%d", &handle);

                LOG_DEBUG ("Requesting to Sync Instance...\n");
                retval = BT_ctn_cce_sync_instance
                            (
                                &cas_instance[handle].instance.handle
                            );
                LOG_DEBUG ("Retval - 0x%04X\n", retval);
    #else
                LOG_DEBUG ("Please enable 'HAVE_CTN_SYNC_INSTANCE' feature flag\n");
    #endif /* HAVE_CTN_SYNC_INSTANCE */
                break;

            case 50:
                LOG_DEBUG("Enter the path: ");
                scanf("%s", path);
                EM_fops_list_directory(path);
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

    return;
}


API_RESULT appl_ctn_cce_callback
           (
               CTN_HANDLE_INFO * ctn_handle,
               UINT8             event_type,
               UINT16            event_result,
               CTN_HEADERS     * event_header,
               UINT16            event_hdrlen
           )
{
    API_RESULT retval;
    CTN_APPL_PARAMS * appl_param;
    UCHAR send_request;
    UINT16 i, j;
    CTN_HANDLE handle;

#ifdef CTN_SUPPORT_NOTIFICATION
    UCHAR send_response;
#endif /* CTN_SUPPORT_NOTIFICATION */

    UINT16 length;
    UCHAR * data;

    CTN_HEADERS *ctn_rx_hdrs;
    CTN_HEADERS ctn_header;
    CTN_HEADER_STRUCT body_req;
    CTN_REQUEST_STRUCT req_info;
    CTN_RESPONSE_STRUCT rsp_info;
    UCHAR more;
    UINT16 actual;

    BT_IGNORE_UNUSED_PARAM(event_hdrlen);

    LOG_DEBUG ("\nCTN CCE Event: 0x%02X Result: 0x%04X\n", event_type, event_result);

    /* Init */
    send_request = 0U;
    BT_mem_set(&ctn_header, 0, sizeof(CTN_HEADERS));
    BT_mem_set(&req_info, 0, sizeof(CTN_REQUEST_STRUCT));
    BT_mem_set(&rsp_info, 0, sizeof(CTN_RESPONSE_STRUCT));
    CTN_INIT_HEADER_STRUCT (body_req);

#ifdef CTN_SUPPORT_NOTIFICATION
    send_response = 0U;
#endif /* CTN_SUPPORT_NOTIFICATION */

    more = 0x00U;
    ctn_rx_hdrs = event_header;

    handle = ctn_handle->entity_id;

    switch (event_type)
    {
    case CTN_CCE_CONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_CONNECT_CNF\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        LOG_DEBUG ("CSE BD Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
        event_header->ctn_connect_info->bd_addr[0U],
        event_header->ctn_connect_info->bd_addr[1U],
        event_header->ctn_connect_info->bd_addr[2U],
        event_header->ctn_connect_info->bd_addr[3U],
        event_header->ctn_connect_info->bd_addr[4U],
        event_header->ctn_connect_info->bd_addr[5U]);

        /* TODO: Have to get server channel local/remote info */

        LOG_DEBUG ("MAX Exchange Size: %d\n",
            event_header->ctn_connect_info->max_recv_size);

        ctn_xchg_size = event_header->ctn_connect_info->max_recv_size;

        if (CTN_SUCCESS_RSP == event_result)
        {
            /* Get the Handle parameters */
            for (i = 0U; i < CTN_CLIENT_NUM_INSTANCES; i++)
            {
                if (cas_instance[i].instance.handle == handle)
                {
                    BT_mem_copy
                    (
                        cas_instance[i].instance.bd_addr,
                        event_header->ctn_connect_info->bd_addr,
                        BT_BD_ADDR_SIZE
                    );

                    break;
                }
            }

            if (CTN_CLIENT_NUM_INSTANCES != i)
            {
                /* Get the Handle parameters */
                for (j = 0U; j < CTN_CLIENT_NUM_INSTANCES; j++)
                {
                    if (0 == BT_mem_cmp(cns_instance[j].bd_addr,
                        event_header->ctn_connect_info->bd_addr,
                        BT_BD_ADDR_SIZE))
                    {
                        cas_instance[i].cns_id = (UCHAR)j;
                        break;
                    }
                }
            }

            ctn_cce_print_appl_instances();

            /* Reset */
            appl_ctn_cce_start_of_response = BT_FALSE;
        }
        break;

    case CTN_CCE_GET_LISTING_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_GET_LISTING_CNF\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((CTN_SUCCESS_RSP != event_result) &&
            (CTN_CONTINUE_RSP != event_result))
        {
            /* In normal case, should not reach here */
            if (NULL != ctn_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(ctn_rx_fp);
                ctn_rx_fp = NULL;
            }

            /* Reset */
            appl_ctn_cce_start_of_response = BT_FALSE;

            break;
        }

        if (CTN_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }

        appl_param = ctn_rx_hdrs->ctn_rsp_info->appl_params;

        if (BT_TRUE != appl_ctn_cce_start_of_response)
        {
            appl_ctn_cce_start_of_response = BT_TRUE;

            if (NULL != appl_param)
            {
                /* Get CTN listing size */
                if (0U != (CTN_GET_APPL_PARAM_FLAG
                (
                    appl_param->appl_param_flag,
                    CTN_FLAG_LISTINGSIZE
                )))
                {
                    LOG_DEBUG("CTN Listing Size: %d\n",
                        appl_param->listing_size);
                }
                else
                {
                    LOG_DEBUG("CTN Listing Size not received\n");
                }

                /* Get CTN CSE time */
                if (0U != (CTN_GET_APPL_PARAM_FLAG
                (
                    appl_param->appl_param_flag,
                    CTN_FLAG_CSETIME
                )))
                {
                    LOG_DEBUG("CTN CSE time: %s\n",
                        appl_param->cse_time.value);
                }
                else
                {
                    LOG_DEBUG("CTN CSE time not received\n");
                }
            }
            else
            {
                LOG_DEBUG("Appl Param NULL received\n");
            }
        }

        if ((NULL == ctn_rx_hdrs->ctn_rsp_info->body) ||
            (NULL == ctn_rx_hdrs->ctn_rsp_info->body->value) ||
            (0U == ctn_rx_hdrs->ctn_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            data = ctn_rx_hdrs->ctn_rsp_info->body->value;
            length = ctn_rx_hdrs->ctn_rsp_info->body->length;

#if 0
            /* print received data on console */
            LOG_DEBUG ("\n\n ====== CTN Get Listing Object data ===== \n\n");
            for (i = 0; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n\n ================================= \n");
#endif /* 0 */

            /* Open file for dumping received GetFolderListing request data */
            if (NULL == ctn_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)APPL_CTN_ROOT_FOLDER_REFERENCE,
                    (UCHAR *)APPL_LISTING_FILE,
                    ctn_file_object
                );

                retval = BT_fops_file_open
                         (
                             (UCHAR *)ctn_file_object,
                             (UCHAR *)"wb",
                             &ctn_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == ctn_rx_fp))
                {
                    LOG_DEBUG ("Failed to open CTN Listing file\n");
                }
            }

            /* Write to file */
            if (NULL != ctn_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, ctn_rx_fp, &actual);
            }
        }

        /* Close the file if final chunk is received */
        if (CTN_SUCCESS_RSP == event_result)
        {
            if (NULL != ctn_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(ctn_rx_fp);
                ctn_rx_fp = NULL;

                LOG_DEBUG("\nReceived complete CTN-listing object: %s\n", APPL_LISTING_FILE);
                LOG_DEBUG("Check in: %s folder\n", APPL_CTN_ROOT_FOLDER_REFERENCE);
            }

            /* Reset */
            appl_ctn_cce_start_of_response = BT_FALSE;
        }
        break;

    case CTN_CCE_GET_OBJECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_GET_OBJECT_CNF\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((CTN_SUCCESS_RSP != event_result) &&
            (CTN_CONTINUE_RSP != event_result))
        {
            /* In normal case, should not reach here */
            if (NULL != ctn_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(ctn_rx_fp);
                ctn_rx_fp = NULL;
            }
            break;
        }

        if (CTN_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }

        if ((NULL == ctn_rx_hdrs->ctn_rsp_info->body) ||
            (NULL == ctn_rx_hdrs->ctn_rsp_info->body->value) ||
            (0U == ctn_rx_hdrs->ctn_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            data = ctn_rx_hdrs->ctn_rsp_info->body->value;
            length = ctn_rx_hdrs->ctn_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG ("\n\n ====== CTN Get Object data ===== \n\n");
            for (i = 0U; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n\n ================================= \n");

            /* Open file for dumping received Get Object request data */
            if (NULL == ctn_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)APPL_CTN_ROOT_FOLDER_REFERENCE,
                    (UCHAR *)APPL_GET_FILE_NAME,
                    ctn_file_object
                );

                retval = BT_fops_file_open
                         (
                             (UCHAR *)ctn_file_object,
                             (UCHAR *)"wb",
                             &ctn_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == ctn_rx_fp))
                {
                    LOG_DEBUG ("Failed to open CTN GetObject file\n");
                }
            }

            /* Write to file */
            if (NULL != ctn_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, ctn_rx_fp, &actual);
            }

            /* Close the file if final chunk is received */
            if (CTN_SUCCESS_RSP == event_result)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close (ctn_rx_fp);
                ctn_rx_fp = NULL;
            }
        }
        break;

    case CTN_CCE_SET_OBJECT_STATUS_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_SET_OBJECT_STATUS_CNF\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        break;

    case CTN_CCE_PUSH_OBJECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_PUSH_OBJECT_CNF\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((CTN_SUCCESS_RSP == event_result) ||
            (CTN_CONTINUE_RSP != event_result))
        {
            /* In normal case, should not reach here */
            if (NULL != fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fp);
                fp = NULL;
            }

            break;
        }

        /* Get Name content - CTN object folder */
        if ((NULL != event_header->ctn_rsp_info->name->value) &&
            (event_header->ctn_rsp_info->name->length > 0U))
        {
            data = event_header->ctn_rsp_info->name->value;
            length = event_header->ctn_rsp_info->name->length;

            LOG_DEBUG ("CTN Object Handle: ");
            for (i = 0U; i < length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }

            LOG_DEBUG ("\n");
        }

        /* Fill body hdr values */
        if (remaining > ctn_xchg_size)
        {
            body_req.length = ctn_xchg_size;
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

        send_request = 1U;
        break;

    case CTN_CCE_FORWARD_OBJECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_FORWARD_OBJECT_CNF\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        break;

    case CTN_CCE_GET_ACCOUNT_INFO_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_GET_ACCOUNT_INFO_CNF\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((CTN_SUCCESS_RSP != event_result) &&
            (CTN_CONTINUE_RSP != event_result))
        {
            /* In normal case, should not reach here */
            if (NULL != ctn_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(ctn_rx_fp);
                ctn_rx_fp = NULL;
            }

            /* Reset */
            appl_ctn_cce_start_of_response = BT_FALSE;

            break;
        }

        if (CTN_CONTINUE_RSP == event_result)
        {
            send_request = 0x01U;
        }

        appl_param = ctn_rx_hdrs->ctn_rsp_info->appl_params;

        if (BT_TRUE != appl_ctn_cce_start_of_response)
        {
            appl_ctn_cce_start_of_response = BT_TRUE;

            if (appl_param != NULL)
            {
                /* Get CTN listing size */
                if (0U != (CTN_GET_APPL_PARAM_FLAG
                (
                    appl_param->appl_param_flag,
                    CTN_FLAG_EMAILURI
                )))
                {
                    LOG_DEBUG("CTN Email URL: %s\n",
                        appl_param->emailuri.value);
                }
                else
                {
                    LOG_DEBUG("CTN Email URL not received\n");
                }

                /* Get CTN listing size */
                if (0U != (CTN_GET_APPL_PARAM_FLAG
                (
                    appl_param->appl_param_flag,
                    CTN_FLAG_LASTUPDATE
                )))
                {
                    LOG_DEBUG("CTN Last Update: %s\n",
                        appl_param->lastupdate.value);
                }
                else
                {
                    LOG_DEBUG("CTN Last Update not received\n");
                }
            }
            else
            {
                LOG_DEBUG("Appl Param NULL received\n");
            }
        }

        if ((NULL == ctn_rx_hdrs->ctn_rsp_info->body) ||
            (NULL == ctn_rx_hdrs->ctn_rsp_info->body->value) ||
            (0U == ctn_rx_hdrs->ctn_rsp_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            data = ctn_rx_hdrs->ctn_rsp_info->body->value;
            length = ctn_rx_hdrs->ctn_rsp_info->body->length;

            /* print received data on console */
            LOG_DEBUG ("\n\n ====== CTN Get Account Info data ===== \n\n");
            for (i = 0U; i<length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }
            LOG_DEBUG ("\n\n ================================= \n");

            /* Open file for dumping received GetFolderListing request data */
            if (NULL == ctn_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)APPL_CTN_ROOT_FOLDER_REFERENCE,
                    (UCHAR *)APPL_CTN_CCE_ACCOUNT_INFO_FILE,
                    ctn_file_object
                );

                LOG_DEBUG("The contents sent by CSE will be dumped to file %s\n",
                    ctn_file_object);

                retval = BT_fops_file_open
                         (
                             (UCHAR *)ctn_file_object,
                             (UCHAR *)"wb",
                             &ctn_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == ctn_rx_fp))
                {
                    LOG_DEBUG ("Failed to open CTN Acount info file\n");
                }
            }

            /* Write to file */
            if (NULL != ctn_rx_fp)
            {
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, ctn_rx_fp, &actual);
            }
        }

        /* Close the file if final chunk is received */
        if (CTN_SUCCESS_RSP == event_result)
        {
            if (NULL != ctn_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(ctn_rx_fp);
                ctn_rx_fp = NULL;
            }

            /* Reset */
            appl_ctn_cce_start_of_response = BT_FALSE;
        }
        break;

    case CTN_CCE_SYNC_ACCOUNT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_SYNC_ACCOUNT_CNF\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        break;

#ifdef HAVE_CTN_SYNC_INSTANCE
    case CTN_CCE_SYNC_INSTANCE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_SYNC_INSTANCE_CNF\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        break;
#endif /* HAVE_CTN_SYNC_INSTANCE */

    case CTN_CCE_SET_NTF_REGISTRATION_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_SET_NTF_REGISTRATION_CNF\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        break;

    case CTN_CCE_ABORT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_ABORT_CNF\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        /* Reset */
        appl_ctn_cce_start_of_response = BT_FALSE;

        break;

    case CTN_CCE_DISCONNECT_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_DISCONNECT_CNF\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        break;

    case CTN_CCE_TRANSPORT_CLOSE_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_TRANSPORT_CLOSE_IND\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        for (i = 0U; i < CTN_CLIENT_NUM_INSTANCES; i++)
        {
            if (cas_instance[i].instance.handle == handle)
            {
                BT_mem_set(&cas_instance[i].instance.bd_addr, 0x00, BT_BD_ADDR_SIZE);
                break;
            }
        }

        if (NULL != ctn_rx_fp)
        {
            /* In normal case, should not reach here */

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(ctn_rx_fp);
            ctn_rx_fp = NULL;
        }

        if (NULL != fp)
        {
            /* In normal case, should not reach here */

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fp);
            fp = NULL;
        }

        ctn_cce_print_appl_instances();
        break;

    case CTN_CCE_TRANSPORT_CLOSE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_TRANSPORT_CLOSE_CNF\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        for (i = 0U; i < CTN_CLIENT_NUM_INSTANCES; i++)
        {
            if (cas_instance[i].instance.handle == handle)
            {
                BT_mem_set(&cas_instance[i].instance.bd_addr, 0x00, BT_BD_ADDR_SIZE);
                break;
            }
        }

        if (NULL != ctn_rx_fp)
        {
            /* In normal case, should not reach here */

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(ctn_rx_fp);
            ctn_rx_fp = NULL;
        }

        if (NULL != fp)
        {
            /* In normal case, should not reach here */

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(fp);
            fp = NULL;
        }

        ctn_cce_print_appl_instances();
        break;

#ifdef CTN_SUPPORT_NOTIFICATION
    case CTN_CCE_NS_CONNECT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_NS_CONNECT_IND\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (NULL != event_header->ctn_connect_info)
        {
            LOG_DEBUG("Peer Address - %02X %02X %02X %02X %02X %02X\n",
            event_header->ctn_connect_info->bd_addr[0U],
            event_header->ctn_connect_info->bd_addr[1U],
            event_header->ctn_connect_info->bd_addr[2U],
            event_header->ctn_connect_info->bd_addr[3U],
            event_header->ctn_connect_info->bd_addr[4U],
            event_header->ctn_connect_info->bd_addr[5U]);

            LOG_DEBUG ("MAX Exchange Size: %d\n",
            event_header->ctn_connect_info->max_recv_size);
        }

        /* Check for valid event result and parameters */
        if((CTN_SUCCESS_RSP != event_result) ||
           (NULL == event_header->ctn_connect_info))
        {
            event_result = CTN_NOT_ACCEPTABLE_RSP;
            send_response = 1U;
            break;
        }

        /* Get the Handle parameters */
        for (i = 0U; i < CTN_CLIENT_NUM_INSTANCES; i++)
        {
            if (cns_instance[i].handle == handle)
            {
                BT_COPY_BD_ADDR(cns_instance[i].bd_addr,
                event_header->ctn_connect_info->bd_addr);
                break;
            }
        }

        if (i != CTN_CLIENT_NUM_INSTANCES)
        {
            for (j = 0U; j < CTN_CLIENT_NUM_INSTANCES; j++)
            {
                if (0 == BT_mem_cmp(cas_instance[j].instance.bd_addr,
                     event_header->ctn_connect_info->bd_addr, BT_BD_ADDR_SIZE))
                {
                    cas_instance[j].cns_id = (UCHAR)i;
                }
            }
        }

        ctn_cce_print_appl_instances();

        event_result = CTN_SUCCESS_RSP;

        /* Set response to be sent */
        send_response = 1U;
        break;

    case CTN_CCE_NS_DISCONNECT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_NS_DISCONNECT_IND\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        break;

    case CTN_CCE_NS_TRANSPORT_CLOSE_CNF:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_NS_TRANSPORT_CLOSE_CNF\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        for (i = 0U; i < CTN_CLIENT_NUM_INSTANCES; i++)
        {
            if (cns_instance[i].handle == handle)
            {
                BT_mem_set (cns_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);
                break;
            }
        }

        for (j = 0U; j < CTN_CLIENT_NUM_INSTANCES; j++)
        {
            if (cas_instance[j].cns_id == i)
            {
                cas_instance[j].cns_id = CTN_ENTITY_INVALID;
            }
        }

        ctn_cce_print_appl_instances();
        break;

    case CTN_CCE_NS_TRANSPORT_CLOSE_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_NS_TRANSPORT_CLOSE_IND\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        for (i = 0U; i < CTN_CLIENT_NUM_INSTANCES; i++)
        {
            if (cns_instance[i].handle == handle)
            {
                BT_mem_set (cns_instance[i].bd_addr, 0x00, BT_BD_ADDR_SIZE);
                break;
            }
        }

        for (j = 0U; j < CTN_CLIENT_NUM_INSTANCES; j++)
        {
            if (cas_instance[j].cns_id == i)
            {
                cas_instance[j].cns_id = CTN_ENTITY_INVALID;
            }
        }

        ctn_cce_print_appl_instances();
        break;

    case CTN_CCE_NS_EVENT_REPORT_IND:
        LOG_DEBUG("\n");
        LOG_DEBUG("Received CTN_CCE_NS_EVENT_REPORT_IND\n");
        LOG_DEBUG("CTN Client Instance: %d\n", handle);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if ((CTN_SUCCESS_RSP != event_result) &&
            (CTN_CONTINUE_RSP != event_result))
        {
            /* In normal case, should not reach here */
            if (NULL != ctn_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(ctn_rx_fp);
                ctn_rx_fp = NULL;
            }

            ctn_push_started = 0x00U;
            break;
        }

        /* Set response to be sent */
        send_response = 1U;

        if (0U == ctn_push_started)
        {
            ctn_push_started = 0x01U;

            if ((event_header->ctn_req_info->body == NULL) ||
                (event_header->ctn_req_info->appl_params == NULL) ||
                (event_header->ctn_req_info->body->value == NULL) ||
                (event_header->ctn_req_info->body->length == 0U))
            {
                LOG_DEBUG ("Body/Appl Param Header/Value not present\n");
                event_result = CTN_BAD_REQ_RSP;

                break;
            }

            appl_param = event_header->ctn_req_info->appl_params;

            if (0U != (CTN_GET_APPL_PARAM_FLAG
                (
                    appl_param->appl_param_flag,
                    CTN_FLAG_INSTANCEID
                )))
            {
                LOG_DEBUG ("CAS Instance ID: %d\n", appl_param->cas_instance_id);
            }
            else
            {
                LOG_DEBUG ("CAS Instance ID Not Found\n");
                event_result = CTN_BAD_REQ_RSP;

                break;
            }
        }

        if ((NULL == event_header->ctn_req_info->body->value) &&
            (0U == event_header->ctn_req_info->body->length))
        {
            LOG_DEBUG ("No Body Content found ");
        }
        else
        {
            length = event_header->ctn_req_info->body->length;
            data = event_header->ctn_req_info->body->value;

            LOG_DEBUG ("\n\n========== Event Report IND ==========\n\n");

            /* Print the stream */
            for (i = 0U; i < length; i++)
            {
                LOG_DEBUG ("%c", data[i]);
            }

            LOG_DEBUG ("\n\n======================================\n\n");

            /* Open file PUT request */
            if (NULL == ctn_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_vfops_create_object_name
                (
                    (UCHAR *)APPL_CTN_ROOT_FOLDER_REFERENCE,
                    (UCHAR *)APPL_CTN_CCE_EVENT_FILE_NAME,
                    ctn_file_object
                );

                retval = BT_fops_file_open
                         (
                             (UCHAR *)ctn_file_object,
                             (UCHAR *)"wb",
                             &ctn_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == ctn_rx_fp))
                {
                    LOG_DEBUG ("Failed to open EVENT Report Object file\n");
                }
            }

            /* Write to file */
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
        }
        break;
#endif /* CTN_SUPPORT_NOTIFICATION */

    default:
        LOG_DEBUG ("Unknown CTN CCE Event\n");
        break;
    }

    if (0U != send_request)
    {
        /* OBEX over L2CAP is Mandatory for CTN */
        if (0U < srmp_wait_count)
        {
            req_info.wait = BT_TRUE;
            ctn_header.ctn_req_info = &req_info;
            srmp_wait_count--;
        }

        retval = BT_ctn_cce_send_request
                 (
                     &handle,
                     event_type,
                     event_result,
                     &ctn_header,
                     more,
                     &actual
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG ("CTN CCE Send Request Failed - 0x%04X\n", retval);
        }

        if (CTN_CCE_PUSH_OBJECT_CNF == event_type)
        {
            if (NULL != body_req.value)
            {
                BT_free_mem (body_req.value);
                body_req.value = NULL;
            }

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

#ifdef CTN_SUPPORT_NOTIFICATION
    if (0U != send_response)
    {
        /* OBEX over L2CAP is Mandatory for CTN */
        if (0U < srmp_wait_count)
        {
            rsp_info.wait = BT_TRUE;
            ctn_header.ctn_rsp_info = &rsp_info;
            srmp_wait_count--;
        }

        retval = BT_ctn_cce_ns_send_response
                 (
                     &handle,
                     event_type,
                     (UCHAR)event_result,
                     &ctn_header
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG ("CTN CCE Send Request Failed - 0x%04X\n", retval);
        }
    }
#endif /* CTN_SUPPORT_NOTIFICATION */

    return API_SUCCESS;
}


void appl_ctn_cce_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     )
{
    S_UUID uuid[2U];

    UINT16 attrib_id[3U] = {PROTOCOL_DESC_LIST,
                           CTN_CAS_INSTANCE_ID_ATTR_ID,
                           CTN_SUPPORTED_FEATURES_ATTR_ID
                           };
    API_RESULT retval;

    BT_IGNORE_UNUSED_PARAM(length);

    /*
     * Frame the SDP Search pattern with following UUIDs
     * - CTN_CAS_UUID
     * - L2CAP_UUID
     * and Attribute ID "PROTOCOL_DESC_LIST"
     * (For getting the L2CAP PSM on which Peer CTN Service
     *  is Listening)
     *
     */
    uuid[0U].uuid_type = UUID_16;
    uuid[1U].uuid_type = UUID_16;
    uuid[0U].uuid_union.uuid_16 = CTN_CAS_UUID;
    uuid[1U].uuid_union.uuid_16 = L2CAP_UUID;

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
                     &cas_instance[ctn_app_instance].instance.ctn_sdp_record.ctn_sdp_handle,
                     uuid,
                     2U,
                     attrib_id,
                     3U,
                     NULL,
                     0U,
                     cas_instance[ctn_app_instance].instance.ctn_sdp_record.ctn_attrib_data,
                     &cas_instance[ctn_app_instance].instance.ctn_sdp_record.ctn_len_attrib_data
                 );

        if(API_SUCCESS != retval)
        {
            LOG_DEBUG("SDP SEARCH Failed for CTN..Closing SDP\n");
            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&cas_instance[ctn_app_instance].instance.ctn_sdp_record.ctn_sdp_handle);
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
            UCHAR ctn_server_record_no;
            UCHAR * sdp_query_result;
            UCHAR attr_val_4[4U];
            UCHAR attr_val;
            UINT16 attr_size_4;
            UINT16 attr_size;

            sdp_query_result = data;
            ctn_server_record_no = 1U;
            attr_size = 1U;
            attr_size_4 = 4U;

            while (sdp_query_result != NULL)
            {
                sdp_query_result =
                    BT_sdp_get_Pointer_to_next_AttributeList
                    (
                        data,
                        ctn_server_record_no
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
                    LOG_DEBUG("CTN Server PSM: 0x%04X\n", ctn_l2cap_psm);
                }

                /* Get the CAS Instance ID */
                retval = BT_sdp_get_attr_value
                         (
                             CTN_CAS_INSTANCE_ID_ATTR_ID,
                             sdp_query_result,
                             &attr_val,
                             &attr_size
                         );
                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("SDP FAILED to find CTN Instance ID\n");
                }
                else
                {
                    LOG_DEBUG("CTN Server Instance ID: 0x%02X\n", attr_val);
                }

                /* Get the CTN Server Supported Features */
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
                    LOG_DEBUG("CTN Server Supported Features: 0x%08X\n", ctn_supp_feat);
                }

                LOG_DEBUG ("\n==============================\n");

                ctn_server_record_no++;
            }
        }

        /* Now Close the SDP Connection */
        (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&cas_instance[ctn_app_instance].instance.ctn_sdp_record.ctn_sdp_handle);
        break;

    default: /* Events not needed to be Handled */
        LOG_DEBUG("Unhandled SDP Event (0x%02X)\n", command);
        break;
    }/* switch */

    return;
}


void ctn_cce_print_appl_instances (void)
{
    UINT8 i;

    LOG_DEBUG ("\n\n===========================================================\n");
    LOG_DEBUG ("Instance\t CAS Handle\t BD Addr\t CNS Id");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < CTN_CLIENT_NUM_INSTANCES; i++)
    {
        LOG_DEBUG ("%d\t\t ", i);
        LOG_DEBUG ("%02x\t ", cas_instance[i].instance.handle);
        LOG_DEBUG ("%02x:%02x:%02x:%02x:%02x:%02x\t ",
        cas_instance[i].instance.bd_addr[0U], cas_instance[i].instance.bd_addr[1U],
        cas_instance[i].instance.bd_addr[2U], cas_instance[i].instance.bd_addr[3U],
        cas_instance[i].instance.bd_addr[4U], cas_instance[i].instance.bd_addr[5U]);
        LOG_DEBUG ("%02x\n", cas_instance[i].cns_id);
    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    LOG_DEBUG ("\n\n===========================================================\n");
    LOG_DEBUG ("Instance\t CNS Handle\t BD Addr");
    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");

    for (i = 0U; i < CTN_CLIENT_NUM_INSTANCES; i++)
    {
        LOG_DEBUG ("%d\t\t ", i);
        LOG_DEBUG ("%02x\t ", cns_instance[i].handle);
        LOG_DEBUG ("%02x:%02x:%02x:%02x:%02x:%02x\n",
        cns_instance[i].bd_addr[0U], cns_instance[i].bd_addr[1U],
        cns_instance[i].bd_addr[2U], cns_instance[i].bd_addr[3U],
        cns_instance[i].bd_addr[4U], cns_instance[i].bd_addr[5U]);
    }

    LOG_DEBUG ("\n-----------------------------------------------------------\n\n");
}

#endif /* CTN_CCE */

