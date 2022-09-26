
/**
 *  \file appl_hid_device.c
 *
 *  This file implements the HID Device application module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Module Header File Inclusion */
#include "appl_hid_device.h"
#include "appl_utils.h"
#include "appl_hci.h"

/* ----------------------------------------- Global Variables */
#ifdef HID_DEVICE

/* Structure describing callbacks registered with profile */
static HID_DEVICE_CALLBACK_STRUCT hid_appl_device_callback;

/* Structure describing initialization parameters for HID Device Profile */
static HID_DEVICE_INIT_PARAMS hid_device_init_params;

/* Structure describing power schematic parameters for HID Device Profile */
static HID_DEVICE_SNIFF_PARAMS hid_device_sniff_params_appl[2U];

/* Sample report */
static UCHAR hid_appl_output_report_1[] = { 1U,0xFFU };
static UCHAR hid_appl_input_report_1[] = { 1U,0U,1U,2U,3U,4U,5U,6U,7U };
static UCHAR hid_appl_input_report_2[] = { 2U,0U,1U,2U };
static UCHAR hid_appl_feature_report_1[] = { 1U,0U,1U,0U };
static UCHAR *hid_appl_report;

/** HID Device Report length */
static UINT8 hid_appl_report_len;

/** HID Device Protocol; default set to report protocol */
static UCHAR hid_device_appl_protocol = HID_REPORT_PROTOCOL;

/* Idle rate */
static UCHAR device_idle_rate = 0x04U;

/** The BD Address of the HID Host. */
static UCHAR hid_host_bd_addr[BT_BD_ADDR_SIZE];

/** The ACL connection handle. */
static UINT16  hid_device_connection_handle;

/*
 *  Flag to indicate peer or self initiated connection request
 */
static UCHAR app_hid_device_peer_initiated_connection = BT_TRUE;

/* Flag to indicate device initiated connection in progress */
static UCHAR app_hid_device_connect_flag = BT_FALSE;

/**  page scan repetition mode of the host */
static UINT8 hid_device_host_page_scan_repetition_mode = 0U;

/**  page scan mode of the host */
static UINT8 hid_device_host_page_scan_mode = 0U;

/**  host clock offset */
static UINT16 hid_device_host_clock_offset = 0U;

/* host class of device */
static UCHAR hid_host_class_of_device[4U] = {0U};

/*
 *  Variable to indicate device undergoing reset
 *  hid_device_reset = 0x0; default value.
 *  hid_device_reset = HID_CONTROL_SOFT_RESET; implies undergoing soft reset.
 *  hid_device_reset = HID_CONTROL_HARD_RESET; implies undergoing hard reset.
 */
static UINT8 hid_device_reset = 0x0U;

/* Variable to store encryption type */
static UINT8 hid_device_encryption_mode = HID_DEVICE_DEFAULT_ENCRYPTION_MODE;

/* Variable to differentiate between hid_connect and reinitiate_connection. */
static UCHAR hid_connect_type = 0x0U;

/* Variable to differentiate between hid_connect and reinitiate_connection. */
static UCHAR hid_initialized = 0x0U;

/* Variable to differentiate between hid_connect and reinitiate_connection. */
static UCHAR hid_started = 0x0U;

/* Virtual Cable Unplug Status Flag */
static UCHAR   hid_vcu_status_flag;

/* ----------------------------------------- Exported Global Variables */
static const char hid_device_options[] = "\n\
-------------- HID Device Menu ---------------\n\
    0.  Exit \n\
    1.  Refresh \n\
\n\
    2.  Register BD_ADDR of HID Host \n\
    3.  Get Registered BD_ADDR of HID Host\n\
\n\
   4.  Start HID Device\n\
   5.  Stop HID Device\n\
\n\
   10.  Create ACL Connection\n\
   11.  Release ACL Connection with HID Host \n\
\n\
\n\
   20.  Connect Control Channel with HID Host \n\
   21.  Connect Interrupt Channel with HID Host \n\
   22.  Disconnect from HID Host \n\
   23.  Virtual Cable Unplug \n\
\n\
   30.  Send Report\n\
   31.  Set Protocol mode \n\
   32.  Send Test Report \n\
\n\
   50.  Delete Link Key \n\
\n\
   55.  Enter Sniff \n\
   56.  Exit Sniff \n\
   57.  Set Sniff Subrating \n\
\n\
Your Option -> \0";

/** The Database record handle for HID */
static UINT32 hid_dbase_rec_handle;
static UINT32 did_dbase_rec_handle;

/* HID Mouse Database Description */
static UCHAR hid_mouse_virtual_cable[] = {0x28U, 0x01U};
static UCHAR hid_mouse_reconnect_initiate[] = {0x28U, 0x01U};
#if 0
static UCHAR hid_mouse_remote_wake[] = {0x28, 0x00};
#else
static UCHAR hid_mouse_remote_wake[] = {0x28U, 0x01U};
#endif /* 0 */
static UCHAR hid_mouse_normally_connectable[] = {0x28U, 0x01U};
static UCHAR hid_mouse_boot_device[] = {0x28U, 0x01U};
static UCHAR hid_mouse_battery_device[] = {0x28U, 0x01U};
static UCHAR hid_mouse_sdp_disable[] = {0x28U, 0x00U};

static UCHAR hid_mouse_country_code[] = {0x08U, 0x00U};
static UCHAR hid_mouse_sub_class[] = {0x08U, 0xC0U};
static UCHAR hid_mouse_profile_ver[] = { 0x09U, 0x01U, 0x11U};
static UCHAR hid_mouse_rel_num[] = {0x09U, 0x01U, 0x00U};
static UCHAR hid_mouse_supervision_timeout [] = {0x09U, 0x7DU, 0x00U};
static UCHAR hid_mouse_parser_ver[] = { 0x09U, 0x01U, 0x00U};

#ifdef HID_1_1
static UCHAR hid_mouse_ssr_latency[] = { 0x09U, 0x06U, 0x40U };
static UCHAR hid_mouse_ssr_timeout[] = { 0x09U, 0x0CU, 0x80U };
#endif /* HID_1_1 */

static UCHAR hid_mouse_lang_base_id_list[] =
                                      { 0x35U, 0x08U, 0x35U, 0x06U, 0x09U,
                                        0x08U, 0x09U, 0x09U, 0x01U, 0x00U };

static UCHAR hid_protocol_descriptor_list[] =
                                      { 0x35U, 0x0dU, 0x35U, 0x06U,
                                        0x19U, 0x01U, 0x00U, 0x09U,
                                        0x00U, 0x11U, 0x35U, 0x03U,
                                        0x19U, 0x00U, 0x11U };

static UCHAR hid_additional_protocol_descriptor_list[] =
                                                 { 0x35U, 0x0fU, 0x35U, 0x0dU,
                                                   0x35U, 0x06U, 0x19U, 0x01U,
                                                   0x00U, 0x09U, 0x00U, 0x13U,
                                                   0x35U, 0x03U, 0x19U, 0x00U,
                                                   0x11U };


#ifdef HID_MOUSE
static UCHAR hid_mouse_report_desc[] =
{ 0x35U, 0x38U, 0x35U, 0x36U, 0x08U, 0x22U, 0x25U, 0x32U,
  0x05U, 0x01U, 0x09U, 0x02U, 0xa1U, 0x01U, 0x09U, 0x01U, 0xa1U, 0x00U,
  0x05U, 0x09U, 0x19U, 0x01U, 0x29U, 0x03U, 0x15U, 0x00U, 0x25U, 0x01U,
  0x95U, 0x03U, 0x75U, 0x01U, 0x81U, 0x02U, 0x95U,
  0x01U, 0x75U, 0x05U, 0x81U, 0x01U, 0x05U, 0x01U, 0x09U, 0x30U, 0x09U, 0x31U,
  0x15U, 0x81U, 0x25U, 0x7fU, 0x75U, 0x08U, 0x95U, 0x02U, 0x81U, 0x06U,
  0xc0U, 0xc0U };
#else
static UCHAR hid_mouse_report_desc[] =
{ 0x35U, 0x7BU, 0x35U, 0x79U, 0x08U, 0x22U, 0x25U, 0x75U, 0x05U, 0x01U,
  0x09U, 0x06U, 0xA1U, 0x01U, 0x85U, 0x01U, 0x05U, 0x07U, 0x19U, 0xE0U,
  0x29U, 0xE7U, 0x15U, 0x00U, 0x25U, 0x01U, 0x75U, 0x01U, 0x95U, 0x08U,
  0x81U, 0x02U, 0x95U, 0x01U, 0x75U, 0x08U, 0x81U, 0x03U, 0x95U, 0x05U,
  0x75U, 0x01U, 0x05U, 0x08U, 0x19U, 0x01U, 0x29U, 0x05U, 0x91U, 0x02U,
  0x95U, 0x01U, 0x75U, 0x03U, 0x91U, 0x03U, 0x95U, 0x06U, 0x75U, 0x08U,
  0x15U, 0x00U, 0x25U, 0x65U, 0x05U, 0x07U, 0x19U, 0x00U, 0x29U, 0x65U,
  0x81U, 0x00U, 0x95U, 0x05U, 0x75U, 0x08U, 0x26U, 0xFFU, 0x00U, 0x15U,
  0x00U, 0x09U, 0x01U, 0xB1U, 0x02U, 0xC0U, 0x05U, 0x01U, 0x09U, 0x02U,
  0xA1U, 0x01U, 0x85U, 0x02U, 0x09U, 0x01U, 0xA1U, 0x00U, 0x05U, 0x09U,
  0x19U, 0x01U, 0x29U, 0x03U, 0x15U, 0x00U, 0x25U, 0x01U, 0x95U, 0x03U,
  0x75U, 0x01U, 0x81U, 0x02U, 0x95U, 0x01U, 0x75U, 0x05U, 0x81U, 0x03U,
  0x05U, 0x01U, 0x09U, 0x30U, 0x09U, 0x31U, 0x15U, 0x81U, 0x25U, 0x7FU,
  0x75U, 0x08U, 0x95U, 0x02U, 0x81U, 0x06U, 0xC0U, 0xC0U };
#endif

/* HID Device Service Name : "Mouse n Keyboard" */
static UCHAR hid_dev_service_name [] =
{ 0x25U, 0x10U, 'M', 'o', 'u', 's', 'e', ' ', 'n', ' ',
'K', 'e', 'y', 'b', 'o', 'a', 'r', 'd' };

/* HID Device Service Description : "Remote Presentation Controller" */
static UCHAR hid_dev_service_desc [] =
{ 0x25U, 0x1EU, 'R', 'e', 'm', 'o', 't', 'e', ' ', 'P', 'r', 'e',
's', 'e', 'n', 't', 'a', 't', 'i', 'o', 'n', ' ', 'C', 'o', 'n',
't', 'r', 'o', 'l', 'l', 'e', 'r' };

/* HID Device Provider Name : "EtherMind" */
static UCHAR hid_dev_provider_name [] =
{ 0x25U, 0x09U, 'E', 't', 'h', 'e', 'r', 'M', 'i', 'n', 'd' };


/** Function to create an ACL connection */
static API_RESULT hid_device_create_acl_connection ( /* IN */ UCHAR * bd_addr );

void main_hid_device_operations(void)
{
    int        option, menu_option;
    UCHAR      sdp_attrs = 0U;
    API_RESULT retval;
    int        read_val;

    UINT16 hci_connection_handle;

    UINT16  sniff_max_interval;
    UINT16  sniff_min_interval;

    BT_IGNORE_UNUSED_PARAM(hid_connect_type);
    BT_IGNORE_UNUSED_PARAM(hid_device_encryption_mode);
    BT_IGNORE_UNUSED_PARAM(app_hid_device_connect_flag);
    BT_IGNORE_UNUSED_PARAM(hid_device_sniff_params_appl);

    BT_LOOP_FOREVER()
    {
        printf("%s", hid_device_options);
        fflush(stdout);
        fflush(stdin);

        scanf("%d", &option);
        menu_option = option;
        switch(option)
        {
        case 0: /* Exit */
            break; /* return; */

        case 1: /* Refresh */
            LOG_DEBUG("Refreshing ...\n");
            break;

        case 2: /* Set BD_ADDR for HID Host application */
            LOG_DEBUG("Please enter BD ADDR of HID Host\n");
            appl_get_bd_addr(hid_host_bd_addr);
            break;

        case 3:/* Get Registered BD_ADDR of peer device */
            LOG_DEBUG("> Registered Peer BD "\
            BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR(hid_host_bd_addr));
            break;

        case 4: /* Initialize Device */

            /* First sniff setting parameter */
            hid_device_sniff_params_appl[0U].sniff_interval =
                HID_APPL_SNIFF_INTERVAL_1;

            hid_device_sniff_params_appl[1U].sniff_interval =
                HID_APPL_SNIFF_INTERVAL_2;

            /* Second sniff setting parameter */
            hid_device_sniff_params_appl[0U].sniff_duration =
                HID_APPL_SNIFF_DURATION_1;

            hid_device_sniff_params_appl[1U].sniff_duration =
                HID_APPL_SNIFF_DURATION_2;

            /* Initialize device attributes */
            sdp_attrs = HID_APPL_VIRTUAL_CABLED |
                        ( HID_APPL_RECONNECT_INITIATE << 1U ) |
                        ( HID_APPL_BATTERY_DEVICE << 2U ) |
                        ( HID_APPL_REMOTE_WAKE << 3U )  |
                        ( HID_APPL_NORMALLY_CONNECTABLE  << 4U )  |
                        ( HID_APPL_BOOT_DEVICE << 5U );

            hid_device_init_params.device_attrs = sdp_attrs;

            /* Store encryption type */
            hid_device_encryption_mode =
                ( hid_device_init_params.device_attrs & 0xc0U ) >> 6U;


            if (0x0U == hid_initialized)
            {
                LOG_DEBUG ("Registering the HID Device Database...\n");
                    hid_appl_dbase_register();

                LOG_DEBUG ("Initializing HID device...\n");

                /*
                 *  Register routine for handling HCI events
                 */
                retval = appl_hci_register_callback
                         (hid_device_hci_event_ind_cb);

                if(retval != API_SUCCESS)
                {
                    LOG_DEBUG("Failed to register with HCI. Error - 0x%04x",
                    retval);

                    /* return; */
                }
                else
                {

                    /* Initialize hid profile */
                    retval = BT_hid_device_init(&hid_device_init_params);
                    if (retval != API_SUCCESS)
                    {
                        LOG_DEBUG("HID Device failed to initialize; Error = 0x%04X\n",
                            retval);
                    }
                    else
                    {
                        LOG_DEBUG("HID Device Initialized.\n");
                    }

                    hid_initialized = 0x1U;
                }

            }

            if ((0x1U == hid_initialized) &&
                (0x0U == hid_started))
            {
                /* Start hid profile */
                retval = BT_hid_device_start (&hid_device_init_params);
                if (retval != API_SUCCESS)
                {
                    LOG_DEBUG("HID Device failed to start; Error = 0x%04X\n",
                    retval);
                }
                else
                {
                    LOG_DEBUG ("HID Device Started.\n");
                }

                hid_started = 0x1U;

                /* Initialize profile callbacks */
                hid_appl_device_callback.hid_device_get_request =
                    hid_appl_get_request;

                hid_appl_device_callback.hid_device_set_request =
                    hid_appl_set_request;

                hid_appl_device_callback.hid_device_events =
                    hid_appl_get_event;

                /* Register callbacks with the profile */
                retval = BT_hid_device_register (&hid_appl_device_callback);
                if (retval != API_SUCCESS)
                {
                    LOG_DEBUG ("HID Device Register Failed! Error = 0x%02X\n",
                    retval);
                }
                else
                {
                    LOG_DEBUG ("HID Device Registered.\n");
                }

                /* Reset Virtual Cable Unplug Flag */
                hid_vcu_status_flag = 0x00U;
            }
            else
            {
                LOG_DEBUG("HID started already\n");
            }
            break;

        case 5: /* Stop HID */
            if (0x0U == hid_started)
            {
                LOG_DEBUG("HID not started\n");
                break;
            }
            retval = BT_hid_device_stop();
            if (retval != API_SUCCESS)
            {
                LOG_DEBUG ("HID Device Stop Failed! Error = 0x%02X\n",
                retval);
            }
            else
            {
                LOG_DEBUG ("HID Device Stopped.\n");
                hid_started = 0x0U;
            }

            break;

        /*
         * Note: The application writer should take care to make the
         * device discoverable. Currently done from HCI Menu.
         * If required, an API can be used to put the  device in discoverable
         * and connectable mode. The HID host can then connect to this device.
         * To save power, HID devices are generally not in discoverable and
         * connectable mode continuously. The application can decide, how often
         * or when the HID device should be put in discoverable and connectable
         * mode.
         */
        case 10: /* Create ACL Connection */
            LOG_DEBUG ("Confirm the BD address is set earlier.\n");

            /* Set connect flag */
            app_hid_device_connect_flag = BT_TRUE;

            /* Set the connection type. */
            hid_connect_type = HID_APPL_CONNECT;

            retval = hid_device_create_acl_connection (hid_host_bd_addr);
            if (retval != API_SUCCESS)
            {
                LOG_DEBUG ("Connect to HID Host Failed! Error %04X.\n", retval);
            }
            else
            {
                LOG_DEBUG ("Connect to HID Host Initiated...\n");
            }
            break;

        case 11: /* ACL Disconnection */
            retval = BT_hci_get_acl_connection_handle
                     (
                         hid_host_bd_addr,
                         &hci_connection_handle
                     );

            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("Disconnecting ACL Connection 0x%04X\n",
                hci_connection_handle);
            }
            else
            {
                LOG_DEBUG ("ACL Connection for HID Host not found\n");
                LOG_DEBUG("Enter ACL Handle\n");
                scanf("%x",&option);
                hci_connection_handle = (UINT16)option;
            }

            retval = BT_hci_disconnect
                     (
                         hci_connection_handle,
                         0x13U
                     );

            LOG_DEBUG("> API RETVAL ACL disconnect : 0x%04X\n", retval);
            break;

        case 20: /* Control Channel Connection */
            LOG_DEBUG ("HID Control Channel Connect...");
            retval = BT_hid_device_connect(hid_host_bd_addr, HID_CH_TYPE_CTRL);
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed\n");
            }
            else
            {
                LOG_DEBUG ("Success\n");
            }
            break;

        case 21: /* Interrupt Channel Connection */
            LOG_DEBUG ("HID Interrupt Channel Connect...");
            retval = BT_hid_device_connect(hid_host_bd_addr, HID_CH_TYPE_INTR);
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed\n");
            }
            else
            {
                LOG_DEBUG ("Success\n");
            }
            break;

        case 22: /* Disconnect from HID Host */
            LOG_DEBUG ("Select Disconnection type: \n");
            LOG_DEBUG ("    0 -> Both Control & Interrupt Channel \n");
            LOG_DEBUG ("    1 -> Control Channel\n");
            LOG_DEBUG ("    2 -> Interrupt Channel \n");
            scanf ("%d", &read_val);

            if (0x00U == read_val)
            {
                /* Disconnect Both CTRL & INTR Channels */
                retval = BT_hid_device_disconnect
                         (
                             (HID_CH_TYPE_CTRL | HID_CH_TYPE_INTR)
                         );
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("Disconnection Initiation for Both Control & Interrupt Channels Failed, Reason: 0x%04x\n",
                        retval);
                }
                else
                {
                    LOG_DEBUG ("Disconnecting for Both Control & Interrupt Channels.\n");
                }
            }
            else if (0x01U == read_val)
            {
                /* Disconnect CTRL Channel */
                retval = BT_hid_device_disconnect
                         (
                             HID_CH_TYPE_CTRL
                         );
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("Disconnection Initiation for Control Channel Failed, Reason: 0x%04x\n",
                        retval);
                }
                else
                {
                    LOG_DEBUG ("Disconnecting the Control Channel.\n");
                }
            }
            else if (0x02U == read_val)
            {
                /* Disconnect INTR Channel */
                retval = BT_hid_device_disconnect
                         (
                             HID_CH_TYPE_INTR
                         );
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG ("Disconnection Initiation for Interrupt Channel Failed, Reason: 0x%04x\n",
                        retval);
                }
                else
                {
                    LOG_DEBUG ("Disconnecting the Interrupt Channel.\n");
                }
            }
            else
            {
                LOG_DEBUG ("Invalid option\n");
            }

            break;

        case 23: /* Virtual Cable Unplug */

            /* Delete HID Device Link Key */
            (BT_IGNORE_RETURN_VALUE) BT_sm_delete_device_link_key (hid_host_bd_addr);

            retval = BT_hid_device_vc_unplug ();
            if (retval != API_SUCCESS)
            {
                LOG_DEBUG ("Virtual Cable Unplug Failed! Error %04X.\n", retval);
            }
            else
            {
                LOG_DEBUG ("Virtual Cable Unplug Initiated...\n");
            }
            break;

#if 0
        case 21: /* Device initiated reconnection */
            /* Set the connection type. */
            hid_connect_type = HID_APPL_REINITIATE_CONNECT;

            retval = hid_device_create_acl_connection ( hid_host_bd_addr );
            if (retval != API_SUCCESS)
            {
                LOG_DEBUG ("Reconnect to HID Host Failed! Error %04X.\n", retval);
            }
            else
            {
                printf ("Reconnect to HID Host Initiated...\n");
            }
            break;

        case 22: /* Virtual Cable Unplug */
            retval = BT_hid_device_vc_unplug ();
            if (retval != API_SUCCESS)
            {
                LOG_DEBUG ("Virtual Cable Unplug Failed! Error %04X.\n", retval);
            }
            else
            {
                LOG_DEBUG ("Virtual Cable Unplug Initiated...\n");
            }
            break;

        case 23: /* Disconnect from HID Host */
            retval = BT_hid_device_disconnect();
            if (retval != API_SUCCESS)
            {
                LOG_DEBUG ("Failed to disconnect from Host.\n");
            }
            else
            {
                LOG_DEBUG ("Started disconnection from HID Host.\n");
            }
            break;
#endif /* 0 */

        case 30: /* Send Report. */
            hid_send_ppt_report();
            break;

        case 31:
            LOG_DEBUG("Enter protocol mode: (0-Boot, 1-Report, 2-No Support): ");
            scanf("%d", &read_val);
            hid_device_appl_protocol = (UCHAR)read_val;
            break;

        case 32:
            LOG_DEBUG("Enter Report ID:");
            scanf("%d", &read_val);

            hid_appl_report = NULL;
            hid_appl_report_len = 0U;

            if (1U == read_val)
            {
                LOG_DEBUG("Enter Report Type (1-Input, 2-Output):");
                scanf("%d", &read_val);

                if (1U == read_val)
                {
                    hid_appl_report = hid_appl_input_report_1;
                    hid_appl_report_len = sizeof(hid_appl_input_report_1);
                }
                else if (2U == read_val)
                {
                    hid_appl_report = hid_appl_output_report_1;
                    hid_appl_report_len = sizeof(hid_appl_output_report_1);
                }
                else
                {
                    LOG_DEBUG("Invalid Report Type\n");
                }
            }
            else if (2U == read_val)
            {
                LOG_DEBUG("Enter Report Type (1-Input, 2-Output):");
                scanf("%d", &read_val);

                if (1U == read_val)
                {
                    hid_appl_report = hid_appl_input_report_2;
                    hid_appl_report_len = sizeof(hid_appl_input_report_2);
                }
                else
                {
                    LOG_DEBUG("Invalid Report Type\n");
                }
            }
            else
            {
                LOG_DEBUG("Invalid report ID\n");
            }

            if (NULL != hid_appl_report)
            {
                (BT_IGNORE_RETURN_VALUE) BT_hid_device_send_report(hid_appl_report, hid_appl_report_len);
            }
            break;

        case 50: /* Delete Link Key */
            LOG_DEBUG("Deleting Device Link Key of "\
            BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR(hid_host_bd_addr));

            retval = BT_sm_delete_device_link_key (hid_host_bd_addr);
            if (retval != API_SUCCESS)
            {
                LOG_DEBUG("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                LOG_DEBUG("OK\n");
            }
            break;

        case 55:
            LOG_DEBUG("Sniff Max Interval (in Hex) = "); fflush(stdout);
            scanf("%x", &read_val);
            sniff_max_interval = (UINT16)read_val;

            LOG_DEBUG("Sniff Min Interval (in Hex) = "); fflush(stdout);
            scanf("%x", &read_val);
            sniff_min_interval = (UINT16)read_val;

            retval = BT_hci_sniff_mode
                     (
                         hid_device_connection_handle,
                         sniff_max_interval,
                         sniff_min_interval,
                         1U,
                         0U
                     );
            LOG_DEBUG("HCI Sniff Mode - 0x%04X\n", retval);
            break;

        case 56:
            retval = BT_hci_exit_sniff_mode (hid_device_connection_handle);
            LOG_DEBUG("HCI Exit Sniff Mode - 0x%04X\n", retval);
            break;

        case 57:
            LOG_DEBUG("Updating HCI Sniff Subrating.\n");

            /* Update Sniff subrating */
            (BT_IGNORE_RETURN_VALUE) BT_hci_sniff_subrating(hid_device_connection_handle, 0x0000U, 0x0000U, 0x0000U);
            break;

        default:
            LOG_DEBUG("Invalid choice. Please try again\n");
            break;
        }

        if (0 == menu_option)
        {
            /* return */
            break;
        }
    }

    return;
}


/**
 *  hid_appl_get_request:
 *  The function is used to handle get_ requests from profile. Parameters
 *  passed for a request are interpreted according to the type of
 *  request. The description of the parameters is given
 *  in "BT_hid_device_api.h" file.
 *  The function processes the request and submits the response at the
 *  profile interface. Explanation of response parameters is given the
 *  "BT_hid_device_api.h" file.
 *
 *  @param  req_info : length of requested data.
 *  @param  req_datalen : Length of requested data.
 *  @param  req_data : request data.
 *  @param  req_type : request type.
 *
 *  @return API_RESULT :
 *      - API_SUCCESS, Indicates report send successfully.
 *      - Error Code,  An error code describing the cause of failure.
 *
 */
API_RESULT hid_appl_get_request
           (
               /* IN */ UINT16 req_info,
               /* IN */ UINT16 req_datalen,
               /* IN */ UCHAR *req_data,
               /* IN */ UINT16 *req_type
           )
{
    UINT16 input_report_len;
    UCHAR  report_type, report_id;
    UCHAR response_status;
    UINT16 response_len;
    UCHAR *response_data;

    switch( *req_type )
    {
        case HID_DEVICE_GET_REPORT:
            LOG_DEBUG(" HID Device Get Report\n");

            /* Check Report Type */
            report_type = (UCHAR)(req_info & 0x03U);
            report_id = (UCHAR)(req_info >> 3U);

            LOG_DEBUG ("Report Type 0x%02X, Report ID 0x%02X\n",
            report_type, report_id);

            /* Init */
            response_len = 0U;
            response_data = NULL;
            response_status = (UCHAR)HID_HANDSHAKE_ERR_INVALID_REPORT_ID;

            switch (report_type)
            {
                case HID_INPUT_REPORT:
                    /*
                     * If Input Report is not set from menu option,
                     * use the default Input Report.
                     */
                    response_status = (UCHAR)HID_HANDSHAKE_ERR_INVALID_REPORT_ID;
                    response_len = 0U;
                    response_data = NULL;

                    if (1U == report_id)
                    {
                        hid_appl_report_len = sizeof(hid_appl_input_report_1);
                        hid_appl_report = hid_appl_input_report_1;
                    }
                    else if (2U == report_id)
                    {
                        hid_appl_report_len = sizeof(hid_appl_input_report_2);
                        hid_appl_report = hid_appl_input_report_2;
                    }
                    else
                    {
                        break;
                    }

                    input_report_len = hid_appl_report_len;

                    /* check for optional buffer size field */
                    if (0U != (req_data[0U] & 0x08U))
                    {
                        if (4U == req_datalen)
                        {
                           input_report_len = req_data[3U];
                           input_report_len <<= 8U;

                           input_report_len |= req_data[2U];
                        }
                        else if (3U == req_datalen)
                        {
                           input_report_len = req_data[2U];
                           input_report_len <<= 8U;

                           input_report_len |= req_data[1U];
                        }
                        else
                        {
                            /* MISRA C-2012 Rule 15.7 */
                        }

                        LOG_DEBUG("GET REPORT BufferSize 0x%04X\n",
                        input_report_len);
                    }

                    if(input_report_len > hid_appl_report_len)
                    {
                        input_report_len = hid_appl_report_len;
                    }

                    response_status = (UCHAR)HID_HANDSHAKE_SUCCESSFUL;
                    response_len = input_report_len;
                    response_data = hid_appl_report;
                    break;

                case HID_OUTPUT_REPORT:
                    response_status = (UCHAR)HID_HANDSHAKE_ERR_INVALID_REPORT_ID;
                    response_len = 0U;
                    response_data = NULL;

                    if (1U == report_id)
                    {
                        hid_appl_report_len = sizeof(hid_appl_output_report_1);
                        hid_appl_report = hid_appl_output_report_1;
                    }
                    else
                    {
                        break;
                    }

                    input_report_len = hid_appl_report_len;

                    /* check for optional buffer size field */
                    if (0U != (req_data[0U] & 0x08U))
                    {
                        if (4U == req_datalen)
                        {
                            input_report_len = req_data[3U];
                            input_report_len <<= 8U;

                            input_report_len |= req_data[2U];
                        }
                        else if (3U == req_datalen)
                        {
                            input_report_len = req_data[2U];
                            input_report_len <<= 8U;

                            input_report_len |= req_data[1U];
                        }
                        else
                        {
                            /* MISRA C-2012 Rule 15.7 */
                        }

                        LOG_DEBUG("GET REPORT BufferSize 0x%04X\n",
                            input_report_len);
                    }

                    if (input_report_len > hid_appl_report_len)
                    {
                        input_report_len = hid_appl_report_len;
                    }

                    response_status = (UCHAR)HID_HANDSHAKE_SUCCESSFUL;
                    response_len = input_report_len;
                    response_data = hid_appl_report;
                    break;

                case HID_FEATURE_REPORT:
                    response_status = (UCHAR)HID_HANDSHAKE_ERR_INVALID_PARAMETER;
                    response_len = 0U;
                    response_data = NULL;

                    input_report_len = 0U;

                    if (1U == report_id)
                    {
                        hid_appl_report_len = sizeof(hid_appl_feature_report_1);
                        hid_appl_report = hid_appl_feature_report_1;
                    }
                    else
                    {
                        break;
                    }

                    /* check for optional buffer size field */
                    if (0U != (req_data[0U] & 0x08U))
                    {
                        if (4U == req_datalen)
                        {
                           input_report_len = req_data[3U];
                           input_report_len <<= 8U;

                           input_report_len |= req_data[2U];
                        }
                        else if (3U == req_datalen)
                        {
                           input_report_len = req_data[2U];
                           input_report_len <<= 8U;

                           input_report_len |= req_data[1U];
                        }
                        else
                        {
                            /* MISRA C-2012 Rule 15.7 */
                        }

                        LOG_DEBUG("GET REPORT BufferSize 0x%04X\n",
                        input_report_len);
                    }

                    if(input_report_len > hid_appl_report_len)
                    {
                        input_report_len = hid_appl_report_len;
                    }

                    response_status = (UCHAR)HID_HANDSHAKE_SUCCESSFUL;
                    response_len = input_report_len;
                    response_data = hid_appl_report;

                    break;

                default:
                    LOG_DEBUG("Invalid Report Type: 0x%02X\n", report_type);
                    break;
            }

            /* Send Get Report response */
            (BT_IGNORE_RETURN_VALUE) BT_hid_device_send_response
            (
                HID_DEVICE_GET_REPORT,
                response_status,
                response_len,
                response_data
            );
        break;

        case HID_DEVICE_GET_PROTOCOL:
            LOG_DEBUG(" HID Device Get Protocol\n");
            /* Relay status to profile */

            if (2U > hid_device_appl_protocol)
            {
                (BT_IGNORE_RETURN_VALUE) BT_hid_device_send_response
                (
                    HID_DEVICE_GET_PROTOCOL,
                    (UCHAR)HID_HANDSHAKE_SUCCESSFUL,
                    1U,
                    &hid_device_appl_protocol
                );
            }
            else
            {
                (BT_IGNORE_RETURN_VALUE) BT_hid_device_send_response
                (
                    HID_DEVICE_GET_PROTOCOL,
                    (UCHAR)HID_HANDSHAKE_ERR_UNSUPPORTED_REQUEST,
                    0U,
                    NULL
                );
            }
        break;

        case HID_DEVICE_GET_IDLE_RATE:
            LOG_DEBUG(" HID Device Get Idle Rate\n");
            /* Relay status to profile */

            (BT_IGNORE_RETURN_VALUE) BT_hid_device_send_response
            (
                HID_DEVICE_GET_IDLE_RATE,
                (UCHAR)HID_HANDSHAKE_SUCCESSFUL,
                1U,
                &device_idle_rate
            );
        break;


        default:
        LOG_DEBUG("Invalid Request Type: 0x%02X\n", (*req_type));
        break;
    }

    return API_SUCCESS;
}


/**
 *  hid_appl_set_request:
 *  The function is used to handle set_ and hid_control requests from
 *  profile. Parameters passed for a request are interpreted according
 *  to the type of request. The description of the parameters is given
 *  in "BT_hid_device_api.h" file.
 *  The function processes the request and submits the response at the
 *  profile interface. Explanation of response parameters is given the
 *  "BT_hid_device_api.h" file.
 *
 *  @param  req_info : length of Reqeust Data.
 *  @param  set_req_info : set and out Request specific information.
 *  @param  req_data : request data.
 *  @param  req_type : request Type.
 *
 *  @return API_RESULT :
 *      - API_SUCCESS, Indicates report send successfully.
 *      - Error Code,  An error code describing the cause of failure.
 *
 */
API_RESULT hid_appl_set_request
           (
               /* IN */ UINT16   req_info,
               /* IN */ UCHAR    set_req_info,
               /* IN */ UCHAR  * req_data,
               /* IN */ UINT16 * req_type
           )
{
    UINT16 i;
    UCHAR response;

    BT_IGNORE_UNUSED_PARAM(set_req_info);

    switch ( *req_type )
    {
        case HID_DEVICE_SET_REPORT:
            LOG_DEBUG(" HID Device Set Report. Type: 0x%02X\n", set_req_info);

            LOG_DEBUG("Set Report Data:\n");
            for (i = 0U; i < req_info; i++)
            {
                LOG_DEBUG("0x%02x ", req_data[i]);
            }
            LOG_DEBUG("\n");

            response = (UCHAR)HID_HANDSHAKE_ERR_INVALID_REPORT_ID;

            switch (set_req_info)
            {
            case HID_OUTPUT_REPORT:
                if (1U == req_data[0U])
                {
                    if (req_info == sizeof(hid_appl_output_report_1))
                    {
                        BT_mem_copy(hid_appl_output_report_1, req_data, req_info);
                        hid_appl_report_len = (UCHAR)req_info;
                        response = (UCHAR)HID_HANDSHAKE_SUCCESSFUL;
                    }
                    else
                    {
                        response = (UCHAR)HID_HANDSHAKE_ERR_INVALID_PARAMETER;
                    }
                }
                break;

            case HID_INPUT_REPORT:
                if (1U == req_data[0U])
                {
                    if (req_info == sizeof(hid_appl_input_report_1))
                    {
                        BT_mem_copy(hid_appl_input_report_1, req_data, req_info);
                        hid_appl_report_len = (UCHAR)req_info;
                        response = (UCHAR)HID_HANDSHAKE_SUCCESSFUL;
                    }
                    else
                    {
                        response = (UCHAR)HID_HANDSHAKE_ERR_INVALID_PARAMETER;
                    }
                }
                else if (2U == req_data[0U])
                {
                    if (req_info == sizeof(hid_appl_input_report_2))
                    {
                        BT_mem_copy(hid_appl_input_report_2, req_data, req_info);
                        hid_appl_report_len = (UCHAR)req_info;
                        response = (UCHAR)HID_HANDSHAKE_SUCCESSFUL;
                    }
                    else
                    {
                        response = (UCHAR)HID_HANDSHAKE_ERR_INVALID_PARAMETER;
                    }
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }

                break;

            case HID_FEATURE_REPORT:
                response = (UCHAR)HID_HANDSHAKE_ERR_INVALID_PARAMETER;
                break;

            default:
                LOG_DEBUG("Invalid Set Request: 0x%02X\n", set_req_info);
                break;
            }

            /* Relay status to profile */
            (BT_IGNORE_RETURN_VALUE) BT_hid_device_send_response
            (
                HID_DEVICE_SET_REPORT,
                response,
                0U,
                NULL
            );
        break;

        case HID_DEVICE_SET_IDLE_RATE:
            LOG_DEBUG(" HID Device Set Idle Rate\n");

            device_idle_rate = (UCHAR)req_info;

            /* Relay status to profile */
            (BT_IGNORE_RETURN_VALUE) BT_hid_device_send_response
            (
                HID_DEVICE_SET_IDLE_RATE,
                (UCHAR)HID_HANDSHAKE_SUCCESSFUL,
                0U,
                NULL
            );
        break;

        case HID_DEVICE_SET_PROTOCOL:
            LOG_DEBUG(" HID Device Set Protocol\n");
            if((HID_REPORT_PROTOCOL == req_info)||
                (HID_BOOT_PROTOCOL == req_info))
            {
                if (2U > hid_device_appl_protocol)
                {
                    hid_device_appl_protocol = (UCHAR)req_info;

                    /* Relay status to profile */
                    (BT_IGNORE_RETURN_VALUE) BT_hid_device_send_response
                    (
                        HID_DEVICE_SET_PROTOCOL,
                        (UCHAR)HID_HANDSHAKE_SUCCESSFUL,
                        0U,
                        NULL
                    );

                    break;
                }
            }

            /* Relay status to profile */
            (BT_IGNORE_RETURN_VALUE) BT_hid_device_send_response
            (
                HID_DEVICE_SET_PROTOCOL,
                (UCHAR)HID_HANDSHAKE_ERR_UNSUPPORTED_REQUEST,
                0U,
                NULL
            );
        break;

        case HID_DEVICE_OUT_REPORT:
            LOG_DEBUG(" HID Device Out Report: ");
            appl_dump_bytes (req_data, req_info);

            switch (set_req_info)
            {
            case HID_INPUT_REPORT:
                if ((1U == req_data[0U]) &&
                   (req_info == sizeof(hid_appl_input_report_1)))
                {
                    LOG_DEBUG("Valid Input report for Report ID 1\n");
                    req_data = NULL;
                    break;
                }
                else if ((2U == req_data[0U]) &&
                    (req_info == sizeof(hid_appl_input_report_2)))
                {
                    LOG_DEBUG("Valid Input report for Report ID 2\n");
                    req_data = NULL;
                    break;
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }

                break;

            case HID_OUTPUT_REPORT:
                if ((1U == req_data[0U]) &&
                    (req_info == sizeof(hid_appl_output_report_1)))
                {
                    LOG_DEBUG("Valid Output report for Report ID 1\n");
                    req_data = NULL;
                    break;
                }
                break;

            default:
                LOG_DEBUG("Invalid Set Request: 0x%02X\n", set_req_info);
                break;
            }

            if (NULL != req_data)
            {
                LOG_DEBUG("Invalid report received\n");
            }

        break;

        case HID_DEVICE_CONTROL:
            LOG_DEBUG(" HID Device Control Data\n");

            switch ( req_info )
            {
                case HID_CONTROL_NOP:
                    LOG_DEBUG(" HID Device Control NOP\n");

                break;

                case HID_CONTROL_HARD_RESET:
                    LOG_DEBUG(" HID Device Hard Reset\n");
                    /* Set reset flag */
                    hid_device_reset = HID_CONTROL_HARD_RESET;
                break;

                case HID_CONTROL_SOFT_RESET:
                    LOG_DEBUG(" HID Device Soft Reset\n");
                    /* Set reset flag */
                    hid_device_reset = HID_CONTROL_SOFT_RESET;

                    (BT_IGNORE_RETURN_VALUE) BT_hid_device_send_response
                    (
                        HID_DEVICE_SET_PROTOCOL,
                        (UCHAR)HID_HANDSHAKE_ERR_UNSUPPORTED_REQUEST,
                        0U,
                        NULL
                    );
                break;

                case HID_CONTROL_SUSPEND:
                    LOG_DEBUG(" HID Device Control Suspend\n");

                break;

                case HID_CONTROL_EXIT_SUSPEND:
                    LOG_DEBUG(" HID Device Exit Suspend\n");

                break;

                case HID_CONTROL_VIRTUAL_CABLE_UNPLUG:
                    LOG_DEBUG(" HID Device Virtual Cable Unplug\n");
                    /* TBD: Remove Pairing Information */
                break;

                default:
                    LOG_DEBUG(" HID Device Unsupported Request\n");
                    (BT_IGNORE_RETURN_VALUE) BT_hid_device_send_response
                    (
                        HID_DEVICE_SET_PROTOCOL,
                        (UCHAR)HID_HANDSHAKE_ERR_UNSUPPORTED_REQUEST,
                        0U,
                        NULL
                    );
                break;
            }

            break;

        default:
           LOG_DEBUG("Invalid Request Type: 0x%04X\n", (*req_type));
           break;
    }

    /* Request not Identified */
    return  HID_HANDSHAKE_ERR_UNSUPPORTED_REQUEST;
}


/**
 *  hid_appl_get_event:
 *  The function is used to handle device events. Example of possible
 *  events are notification of connection status, connectability mode,
 *  power mode etc. Failure status of host contoller initialization
 *  commands like the support for power modes, authentication and encryption
 *  support are also reported as events in this function. The funtion is
 *  also used to request the application for user PIN and link keys,
 *  Parameters passed for an event are interpreted according to the
 *  type of event. The description of the parameters is given in
 *  "BT_hid_device_api.h" file.
 *  The function processes the event and submits the response
 *  at the profile interface. Explanation of response parameters is
 *  given the "BT_hid_device_api.h" file.
 *
 *  @param  event_info : event Type.
 *  @param  event_data : event Data.
 *  @param  event_data_len : length of event data.
 *
 *  @return API_RESULT :
 *      - API_SUCCESS, Indicates report send successfully.
 *      - Error Code,  An error code describing the cause of failure.
 *
 */
API_RESULT hid_appl_get_event
           (
               /* IN */ UCHAR    event_info,
               /* IN */ UCHAR  * event_data,
               /* IN */ UINT16 * event_data_len
           )
{
#if 1
    API_RESULT  retval;
    UINT16      hci_connection_handle;

    BT_IGNORE_UNUSED_PARAM(event_data_len);
    retval = API_SUCCESS;

    /* Determine event type */
    switch ( event_info )
    {
    case HID_DEVICE_CTRL_CH_CONNECTION:
        /* HID Control Channel Connected */
        LOG_DEBUG("\n[HID Device Appl.] HID Connection Complete Event for Control Channel\n");

        BT_mem_copy(hid_host_bd_addr, event_data, BT_BD_ADDR_SIZE);

        break;

    case HID_DEVICE_INTR_CH_CONNECTION:
        /* HID Interrupt Channel Connected */
        LOG_DEBUG("\n[HID Device Appl.] HID Connection Complete Event for Interrupt Channel\n");
        BT_mem_copy(hid_host_bd_addr, event_data, BT_BD_ADDR_SIZE);

         break;

    case HID_DEVICE_CTRL_CH_DISCONNECTION :
        /* HID Control Channel Disconnected */
        LOG_DEBUG("\n[HID Device Appl.] HID Disconnection Complete Event for Control Channel\n");

        if (0x01 == hid_vcu_status_flag)
        {
            /* Disable Virtual Cable Unplug Flag */
            hid_vcu_status_flag = 0x00;

            /* Initiate ACL Disconnection */
            retval = BT_hci_get_acl_connection_handle
                     (
                         hid_host_bd_addr,
                         &hci_connection_handle
                     );

            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("Disconnecting ACL Connection 0x%04X\n",
                hci_connection_handle);

                retval = BT_hci_disconnect
                         (
                             hci_connection_handle,
                             0x13
                         );

                LOG_DEBUG("> API RETVAL ACL disconnect : 0x%04X\n", retval);
            }
        }

        break;

    case HID_DEVICE_INTR_CH_DISCONNECTION:
        /* HID Interrupt Channel Disconnected */
        LOG_DEBUG("\n[HID Device Appl.] HID Disconnection Complete Event for Interrupt Channel\n");
        break;

    case HID_DEVICE_VIRTUAL_CABLE_UNPLUG_IND:
        LOG_DEBUG("[HID HOST APPL] HID Virtual Cable Unplug Indication\n");

        /* Delete HID Device Link Key */
        (BT_IGNORE_RETURN_VALUE) BT_sm_delete_device_link_key (hid_host_bd_addr);

        /* Enable Virtual Cable Unplug Flag */
        hid_vcu_status_flag = 0x01;

        /* Disconnect Both the Channels */
        retval = BT_hid_device_disconnect
                 (
                     (HID_CH_TYPE_CTRL | HID_CH_TYPE_INTR)
                 );

        break;
    case HID_DEVICE_SEND_REPORT:
        LOG_DEBUG("\n[HID Device Appl.] HID Send Report Complete Event \n");
        break;

    default:
        LOG_DEBUG("Invalid Event Type: 0x%02X\n", event_info);
        break;
    }

#if 0
    case HID_DEVICE_CONNECTED:
        /* Device connected */
        LOG_DEBUG ("HID connected.\n");

        /* Get master information */
        BT_mem_copy(hid_host_bd_addr, event_data, BT_BD_ADDR_SIZE);
        BT_mem_copy(hid_host_class_of_device,event_data + 6, 4);
    break;


    case HID_DEVICE_DISCONNECTED:
        /* Device disconnected */
        LOG_DEBUG ("HID Disconnected.\n");
    break;
#endif  /* 0 */

#else
    BT_IGNORE_UNUSED_PARAM(event_data_len);

    /* Determine event type */
    switch ( event_info )
    {
        case HID_DEVICE_CONNECTED:
            /* Device connected */
            LOG_DEBUG ("HID connected.\n");

            /* Get master information */
            BT_mem_copy(hid_host_bd_addr, event_data, BT_BD_ADDR_SIZE);
            BT_mem_copy(hid_host_class_of_device,event_data + 6, 4);
        break;

        case HID_DEVICE_DISCONNECTED:
            /* Device disconnected */
            LOG_DEBUG ("HID Disconnected.\n");
        break;
    }
#endif /* 0 */

    /* Return */
    return API_SUCCESS;
}



void hid_appl_dbase_register ( void )
{
    /* Get record handle */
    (BT_IGNORE_RETURN_VALUE) BT_dbase_get_record_handle ( DB_RECORD_HID, 0U, &hid_dbase_rec_handle);
    (BT_IGNORE_RETURN_VALUE) BT_dbase_get_record_handle ( DB_RECORD_DID, 0U, &did_dbase_rec_handle);

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0004U,
                                 hid_protocol_descriptor_list,
                                 15U);

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x000DU,
                                 hid_additional_protocol_descriptor_list,
                                 17U);

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0100U,
                                 hid_dev_service_name,
                                 18U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0101U,
                                 hid_dev_service_desc,
                                 32U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0102U,
                                 hid_dev_provider_name,
                                 11U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0200U,
                                 hid_mouse_rel_num,
                                 3U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0201U,
                                 hid_mouse_profile_ver,
                                 3U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0202U,
                                 hid_mouse_sub_class,
                                 2U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0203U,
                                 hid_mouse_country_code,
                                 2U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0204U,
                                 hid_mouse_virtual_cable,
                                 2U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0205U,
                                 hid_mouse_reconnect_initiate,
                                 2U) ;
#ifdef HID_MOUSE
    BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0206U,
                                 hid_mouse_report_desc,
                                 58U) ;
#else
    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0206U,
                                 hid_mouse_report_desc,
                                 125U);
#endif
    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0207U,
                                 hid_mouse_lang_base_id_list,
                                 10U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0208U,
                                 hid_mouse_sdp_disable,
                                 2U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0209U,
                                 hid_mouse_battery_device,
                                 2U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x020AU,
                                 hid_mouse_remote_wake,
                                 2U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x020BU,
                                 hid_mouse_parser_ver,
                                 3U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x020CU,
                                 hid_mouse_supervision_timeout,
                                 3U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x020DU,
                                 hid_mouse_normally_connectable,
                                 2U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x020EU,
                                 hid_mouse_boot_device,
                                 2U) ;

#ifdef HID_1_1
    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x020FU,
                                 hid_mouse_ssr_latency,
                                 3U) ;

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value ( hid_dbase_rec_handle,
                                 0x0210U,
                                 hid_mouse_ssr_timeout,
                                 3U) ;
#endif /* HID_1_1 */

    (BT_IGNORE_RETURN_VALUE) BT_dbase_activate_record ( hid_dbase_rec_handle );
    (BT_IGNORE_RETURN_VALUE) BT_dbase_activate_record ( did_dbase_rec_handle );

    /* return */
    return;
}

/**
 *
 *  The function is used to receive HCI Event notifications.
 *
 *  Input Parameters:
 *
 *  @param  event_type : HCI Event type.
 *  @param  *event_data : The data send.
 *  @param  event_data_len : The length of the data string.
 *
 *  @return API_RESULT :
 *      - API_SUCCESS, Indicates event handled successfully.
 *      - Error Code,  An error code describing the cause of failure.
 *
 *  @note  following to be registered with BT_bluetooth_on()
 *
 */
API_RESULT hid_device_hci_event_ind_cb
           (
               /* IN */ UCHAR event_type,
               /* IN */ UCHAR *event_data,
               /* IN */ UCHAR event_data_len
           )
{
    /* The status of the mode change event */
    UCHAR status;

    /* The current mode of the connection */
    UCHAR mode;

    /* Retun value */
    API_RESULT api_result;

    /* link type */
    UCHAR link_type = 0U;

    /* HCI command opcode */
    UINT16 command = 0U;

    /* discoverability modes for the device */
    UINT32 iac_lap[2U] = { BT_GIAC, BT_LIAC };

    BT_IGNORE_UNUSED_PARAM(event_data_len);

    /* Switch on the Event Code */
    switch ( event_type )
    {
    case HCI_CONNECTION_COMPLETE_EVENT:

        hci_unpack_1_byte_param(&status, (&event_data[0U]));

        if(0x00U == status) /* SUCCESS */
        {
            hci_unpack_1_byte_param( &link_type, (&event_data[9U]) );
            if (0x01U == link_type) /* ACL TYPE */
            {
                /* Extract connection handle */
                hci_unpack_2_byte_param
                (
                    &hid_device_connection_handle,
                    (&event_data[1U])
                );

#if 0
                /* ACL connection done; establish BT-HID connection */
                if (BT_FALSE == app_hid_device_peer_initiated_connection)
                {
                    /* Verify if connection process aborted */
                    if (BT_FALSE == app_hid_device_connect_flag)
                    {
                        BT_hci_disconnect
                        (
                            hid_device_connection_handle,
                            0x13
                        );
                        break;
                    }

                    LOG_DEBUG("Initiated HID Connect."\
                           "Please wait for HID connect confirm\n");

                    if (HID_APPL_CONNECT == hid_connect_type)
                    {
                        LOG_DEBUG ("HID Control Channel Connect...");
                        api_result = BT_hid_device_connect(hid_host_bd_addr, HID_CH_TYPE_CTRL);
                        if (API_SUCCESS != api_result)
                        {
                            LOG_DEBUG ("Failed\n");
                        }
                        else
                        {
                            LOG_DEBUG ("Success\n");
                        }
                    }
                    else if (HID_APPL_REINITIATE_CONNECT == hid_connect_type)
                    {
                        api_result = BT_hid_device_initiate_reconnection();
                    }
                    else
                    {
                        /* MISRA C-2012 Rule 15.7 */
                    }
                }
                else
                {
                    LOG_DEBUG(" Host initiated connection "\
                           "Waiting for HID connection\n");
                }

#endif /* 0 */

                /* Reset set connect flag */
                app_hid_device_connect_flag = BT_FALSE;

                /* Reset flag */
                app_hid_device_peer_initiated_connection = BT_TRUE;
            }
        }

    break;

    case HCI_DISCONNECTION_COMPLETE_EVENT:

        /* check if device undergoing reset */
        if (0U != hid_device_reset)
        {
            (BT_IGNORE_RETURN_VALUE) BT_hci_reset();
        }

        /* Reset host CoD Information */
        BT_mem_set(hid_host_class_of_device, 0x0, 4U);

    break;

    case HCI_NUMBER_OF_COMPLETED_PACKETS_EVENT:
        /*LOG_DEBUG("HCI_NUMBER_OF_COMPLETED_PACKETS_EVENT received\n");*/
        break;

    case HCI_PAGE_SCAN_REPETITION_MODE_CHANGE_EVENT:

        /* Refresh page scan repetition mode of the host */
        mode = event_data[6U];
        if (mode < 4U)
        {
            hid_device_host_page_scan_repetition_mode
                  = event_data[6U];
        }
    break;


    case HCI_PAGE_SCAN_MODE_CHANGE_EVENT:

        /* Refresh page scan repetition mode of the host */
        mode = event_data[6U];
        if (mode < 4U)
        {
            hid_device_host_page_scan_mode
                  = event_data[6U];
        }
    break;

    case HCI_READ_CLOCK_OFFSET_COMPLETE_EVENT:
        /* extract status */
        status = *event_data;

        /* refresh clock offset of the host */
        if (0U == status)
        {
            hid_device_host_clock_offset = 0U;

            hci_unpack_2_byte_param(
                     &hid_device_host_clock_offset,
                     &event_data[3U]);
        }
    break;

    case HCI_COMMAND_STATUS_EVENT:
       status = (*(event_data));

       command = ((UINT16)(event_data[3U]) << 8U);
       command |= (event_data[2U]);

       LOG_DEBUG("[hid] Received Command Status 0x%02x for Opcode 0x%04x.\n",
       status, command);

       /* determine command opcode */
       if (HCI_DISCONNECTION_COMPLETE_EVENT == command)
       {
           if ( status != 0x0U )
           {
               LOG_DEBUG("[hid] disconnect Req Failed; \
               command status: 0x%x\n", status );

               /* check if device undergoing reset */
               if (0U != hid_device_reset)
               {
                   LOG_DEBUG("[hid] issuing reset request\n");

                   (BT_IGNORE_RETURN_VALUE) BT_hci_reset();
               }
           }
           else
           {
               LOG_DEBUG("[hid] disconnect req pending\n");
           }
       }
    break;

    case HCI_COMMAND_COMPLETE_EVENT:
        BT_mem_copy(&command, &event_data[1U], 2U);

        status = event_data[3U];

        /* Determine command opcode */
        if (HCI_WRITE_CLASS_OF_DEVICE_OPCODE == command)
        {
            api_result = BT_hci_write_current_iac_lap (2U, iac_lap );

            if ( api_result != API_SUCCESS )
            {
                LOG_DEBUG("Lim Disc mode setting Failed; Err Code: %x.\
                init aborted\n",  api_result);
            }
            else
            {
                LOG_DEBUG("Req submitted to put device in Lim Disc mode\n");
            }

            if ( status != 0x0U )
            {
                LOG_DEBUG(" Write COD Req Failed; status: 0x%x",
                status );
            }
            else
            {
                LOG_DEBUG("Write COD Req successful\n" );
            }
        }
    break;

    default:
        LOG_DEBUG("Invalid Event Type: 0x%02X\n", event_type);
    break;
    }

    BT_IGNORE_UNUSED_PARAM(hid_connect_type);
    BT_IGNORE_UNUSED_PARAM(app_hid_device_connect_flag);
    BT_IGNORE_UNUSED_PARAM(app_hid_device_peer_initiated_connection);
    return API_SUCCESS;
}

/**
 *  hid_device_create_acl_connection:
 *  The function is used to submit request to create ACL connection.
 *
 *  @param  bd_addr : remote device address.
 *
 *  @return API_RESULT :
 *      - API_SUCCESS, Indicates event handled successfully.
 *      - Error Code,  An error code describing the cause of failure.
 */
static API_RESULT hid_device_create_acl_connection ( /* IN */ UCHAR * bd_addr )
{
   API_RESULT retval;

   /* Set connection initiated flag */
   app_hid_device_peer_initiated_connection = BT_FALSE;

   /* Submit request to create acl connection */
   retval = BT_hci_create_connection
            (
                bd_addr,
                HID_DEVICE_PACKET_TYPES,
                hid_device_host_page_scan_repetition_mode,
                hid_device_host_page_scan_mode,
                hid_device_host_clock_offset,
                HID_DEVICE_ROLE_SWITCH
            );
   if (HCI_STATE_ALREADY_CONNECTED == retval)
   {
#if 0
        /* Create hid connection */
        if (HID_APPL_CONNECT == hid_connect_type)
        {
            retval = BT_hid_device_connect(bd_addr, HID_CH_TYPE_CTRL);
        }
        else if (HID_APPL_REINITIATE_CONNECT == hid_connect_type)
        {
            retval = BT_hid_device_initiate_reconnection();
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
#endif /* 0 */
   }

   BT_IGNORE_UNUSED_PARAM(hid_connect_type);
   BT_IGNORE_UNUSED_PARAM(app_hid_device_peer_initiated_connection);
   return retval;
}



/**
 *  hid_send_ppt_report:
 *  The function is used to generate ppt reports based on user's choice.
 *
 *  @param  bd_addr : void
 *
 *  @return void
 */
void hid_send_ppt_report(void)
{
    /* Keys defined for different keyboard and mouse operations. */
    enum
    {
        KEY_SEND = 0x01U,
        KEY_END,
        KEY_1,
        KEY_3,
        KEY_4,
        KEY_6,
        KEY_VOL_UP,
        KEY_VOL_DOWN,
        KEY_SOFT_LEFT,
        KEY_SOFT_RIGHT,
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_CENTER
    };

    static UCHAR report_kbd_make[9U] = {0x1U, 0U,0U, 0x4aU, 0U,0U,0U,0U,0U};
    static UCHAR report_kbd_break[9U] = {0x1U, 0U,0U, 0U, 0U,0U,0U,0U,0U};
    static UCHAR report_mse_make[4U] = {0x02U, 0U, 0U, 0U};
    static UCHAR report_mse_break[4U] = {0x02U, 0U, 0U, 0U};

    UINT16 report_length;
    UCHAR key;
    int read_val;
    API_RESULT retval;

    LOG_DEBUG("Send Report: Enter The Key\n");
    LOG_DEBUG(" Key Mapping:\n");
    LOG_DEBUG(" %2d : Start the PPT.\n",KEY_SEND);
    LOG_DEBUG(" %2d : Exit the PPT.\n",KEY_END);
    LOG_DEBUG(" %2d : Home \n",KEY_1);
    LOG_DEBUG(" %2d : End \n",KEY_3);
    LOG_DEBUG(" %2d : Black Screen \n",KEY_4);
    LOG_DEBUG(" %2d : White Screen \n",KEY_6);
    LOG_DEBUG(" %2d : Next Screen \n",KEY_VOL_UP);
    LOG_DEBUG(" %2d : Previous Screen \n",KEY_VOL_DOWN);
    LOG_DEBUG(" %2d : Mouse Left Button \n",KEY_SOFT_LEFT);
    LOG_DEBUG(" %2d : Mouse Right Button \n",KEY_SOFT_RIGHT);
    LOG_DEBUG(" %2d : Mouse Move Up \n",KEY_UP);
    LOG_DEBUG(" %2d : Mouse Move Down \n",KEY_DOWN);
    LOG_DEBUG(" %2d : Mouse Move Left \n",KEY_LEFT);
    LOG_DEBUG(" %2d : Mouse Move Right \n",KEY_RIGHT);
    LOG_DEBUG(" %2d : Mouse Enter \n",KEY_CENTER);

    while (getchar() != '\n')
    {
        /* MISRA C-2012 Rule 15.6 */
    }

    scanf ("%d", &read_val);

    key = (UINT8)read_val;

    /* Init */
    retval = API_SUCCESS;
    report_length = 0U;

    switch (key)
    {
        case KEY_SEND:
        report_kbd_make[3U] = 0x3EU; /* Start */
        report_length = 9U;
        LOG_DEBUG ("Start            \r");
        break;

        case KEY_END:
        report_kbd_make[3U] = 0x29U; /* Exit */
        report_length = 9U;
        LOG_DEBUG ("End              \r");
        break;

    case KEY_1:
        report_kbd_make[3U] = 0x4AU; /* KEY_1 */
        report_length = 9U;
        LOG_DEBUG ("Home             \r");
        break;

    case KEY_3:
        report_kbd_make[3U] = 0x4DU; /* End */
        report_length = 9U;
        LOG_DEBUG ("End              \r");
        break;

    case KEY_4:
        report_kbd_make[3U] = 0x05U; /* Black */
        report_length = 9U;
        LOG_DEBUG ("Black On/Off     \r");
        break;

    case KEY_6:
        report_kbd_make[3U] = 0x1AU; /* White */
        report_length = 9U;
        LOG_DEBUG ("White On/Off    \r");
        break;

    case KEY_VOL_UP:
        report_kbd_make[3U] = 0x4EU; /* Next */
        report_length = 9U;
        LOG_DEBUG ("Next             \r");
        break;

    case KEY_VOL_DOWN:
        report_kbd_make[3U] = 0x4BU; /* Back */
        report_length = 9U;
        LOG_DEBUG ("Back             \r");
        break;

    case KEY_SOFT_LEFT:
        report_mse_make[1U] = 0x01U;  /* Mouse left button */
        report_length = 4U;
        LOG_DEBUG ("Mouse Left       \r");
        break;

    case KEY_SOFT_RIGHT:
        report_mse_make[1U] = 0x02U;  /* Mouse right button */
        report_length = 4U;
        LOG_DEBUG ("Mouse Right      \r");
        break;

    case KEY_UP:
        report_mse_make[3U] = 0xF6U;  /* Up */
        report_length = 4U;
        LOG_DEBUG ("Up               \r");
        break;

    case KEY_DOWN:
        report_mse_make[3U] = 0x0AU;  /* Down */
        report_length = 4U;
        LOG_DEBUG ("Down             \r");
        break;

    case KEY_LEFT:
        report_mse_make[2U] = 0xF6U; /* left */
        report_length = 4U;
        LOG_DEBUG ("Left             \r");
        break;

    case KEY_RIGHT:
        report_mse_make[2U] = 0x0AU; /* right */
        report_length = 4U;
        LOG_DEBUG ("Right            \r");
        break;

    case KEY_CENTER:
        report_mse_make[1U] = 0x01U;  /* Enter */
        report_length = 4U;
        LOG_DEBUG ("Center           \r");
        break;

    default :
        LOG_DEBUG ("No operation     \r");
        retval = API_FAILURE; /* return; */
    }

    if (API_SUCCESS == retval)
    {
        if(9U == report_length)
        {
            (BT_IGNORE_RETURN_VALUE) BT_hid_device_send_report (report_kbd_make, report_length);
            report_kbd_make[3U]=0U;
            (BT_IGNORE_RETURN_VALUE) BT_hid_device_send_report (report_kbd_break, report_length);
        }
        else
        {
            (BT_IGNORE_RETURN_VALUE) BT_hid_device_send_report (report_mse_make, report_length);
            report_mse_make[1U]=0U;
            report_mse_make[2U]=0U;
            report_mse_make[3U]=0U;
            (BT_IGNORE_RETURN_VALUE) BT_hid_device_send_report (report_mse_break, report_length);
        }
    }

    return;
}

#endif /* HID_DEVICE */

