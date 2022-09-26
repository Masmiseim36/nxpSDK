
/**
 *  \file appl_hid_host.c
 *
 *  Source file for HID Host Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_hid_host.h"
#include "appl_utils.h"

#ifdef HID_HOST
/* ----------------------------------------- Exported Global Variables */
static const char hid_host_options[] = "\n\
-------------- HID Host Menu ---------------\n\
    0.  Exit \n\
    1.  Refresh \n\
\n\
   10.  Initialize HID Host \n\
   11.  Shutdown HID Host \n\
   12.  HID Host Registration \n\
\n\
   20.  Add HID Device \n\
   21.  Remove HID Device \n\
\n\
   25.  Perform Device Inquiry\n\
\n\
   30.  Create ACL connection\n\
   31.  Get HID Device SDP record \n\
   32.  Release ACL with HID Device \n\
   33.  Set Sniff Subrating from SDP \n\
   34.  Set Link Supervision Timeout \n\
\n\
   40.  Connect Control Channel with HID Device \n\
   41.  Connect Interrupt Channel with HID Device \n\
   42.  Disconnect from HID Device \n\
   43.  Virtual Cable Unplug \n\
\n\
   50.  Get Protocol \n\
   51.  Set Protocol \n\
\n\
   60.  Get Report \n\
   61.  Set Report \n\
\n\
   70.  Get Idle Rate \n\
   71.  Set Idle Rate \n\
\n\
   80.  Send Control Packet \n\
   81.  Send Interrupt Packet \n\
\n\
  100.  Delete Link Key \n\
\n\
Your Option -> \0";

#ifdef HID_1_0
static char hid_control_options[] = "\n\
-------------- HID Control Type ---------------\n\
    0.  NOP \n\
    1.  HARD_RESET \n\
    2.  SOFT_RESET \n\
    3.  SUSPEND \n\
    4.  EXIT_SUSPEND \n\
    5.  VIRTUAL_CABLE_UNPLUG \n\
Your Choice : \0";
#else
static char hid_control_options[] = "\n\
-------------- HID Control Type ---------------\n\
    3.  SUSPEND \n\
    4.  EXIT_SUSPEND \n\
    5.  VIRTUAL_CABLE_UNPLUG \n\
Your Choice : \0";
#endif /* HID_1_0 */

static char hid_report_types[] = "\n\
-------------- HID Report Type ---------------\n\
    1.  INPUT \n\
    2.  OUTPUT \n\
    3.  FEATURE \n\
Your Choice : \0";


static char hid_protocol_types[] = "\n\
-------------- HID Protocol Type ---------------\n\
    0.  BOOT \n\
    1.  REPORT \n\
Your Choice : \0";

/* ----------------------------------------- Static Global Variables */
/* List of HID device handles. */
DECL_STATIC APPL_HID_HOST_DEVICE_INFO hid_device_handle_list[BT_HID_HOST_MAX_DEVS];

/* Count of Free HID device handles. */
DECL_STATIC UCHAR hid_device_num_handles_free;

/* SDP Handle */
DECL_STATIC SDP_HANDLE sdp_handle ;

/* HID host initialized. */
DECL_STATIC UCHAR hid_host_initialized = 0x0U;

/* HID host started. */
DECL_STATIC UCHAR hid_host_started = 0x0U;

DECL_STATIC UCHAR hid_device_bd_addr[BT_BD_ADDR_SIZE];

/* Virtual Cable Unplug Status Flag */
DECL_STATIC UCHAR   hid_vcu_status_flag;

#ifdef HID_1_1
DECL_STATIC UINT16     ssr_latency, ssr_timeout;
#endif /* HID_1_1 */

/* ------------------------------------------ Functions */

/*
 * This Sample Application Demonstrate usage of various HID Host APIs.
 */

/* Entry Point for HID Host Application */
void main_hid_host_operations (void)
{
    int option, menu_option;
    API_RESULT retval;
    HID_PROTOCOL_TYPE protocol;
    HID_REPORT_TYPE report_type;
    UCHAR report_id;
    HID_HOST_REPORT report;

#if ((defined HID_SUPPORT_GET_IDLE) || (defined HID_SUPPORT_SET_IDLE))
    UCHAR new_idle_rate;
#endif /* (defined HID_SUPPORT_GET_IDLE) || (defined HID_SUPPORT_SET_IDLE) */

    HID_CONTROL_OP_CODE op_code;
    int input_read;
    UINT16 index;
    HID_HOST_DEV_HANDLE hid_device_dev_handle;

    UINT16 hci_connection_handle;
    UINT16 lsto;

    BT_LOOP_FOREVER()
    {
        printf("%s", hid_host_options);
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

        case 10: /* Initialize Host */
            if (0x1U == hid_host_started)
            {
                LOG_DEBUG("HID Host started already...\n");
                break;
            }

            LOG_DEBUG("Starting HID Host...\n");

            /* Check if HID host initalized already. */
            if (0x0U == hid_host_initialized)
            {
                retval = BT_hid_host_init();

                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("Initializing HID Host Failed. Reason: 0x%04X\n",
                        retval);
                    break;
                }
                else
                {
#ifndef BT_DRIVER_MODE
                    hid_uinput_init();
#endif /* BT_DRIVER_MODE */

                    hid_host_initialized = 0x01U;
                }
            }

            /* Start the HID Host. */
            retval = BT_hid_host_start();
            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Starting HID Host Failed. Reason: 0x%04X\n",
                retval);
                break;
            }
            else
            {
                LOG_DEBUG("HID Host Started Successfully\n");
                hid_host_started = 0x1U;

                /* Reset Virtual Cable Unplug status flag */
                hid_vcu_status_flag = 0x00U;
            }

            /* Initially all the handles are free for use. */
            hid_device_num_handles_free = BT_HID_HOST_MAX_DEVS;

            /* Initialse all handles to Unused state [BT_HID_INVALID_HANDLE]. */
            for ( index = 0U; index < BT_HID_HOST_MAX_DEVS; index++ )
            {
                hid_device_handle_list[index].handle = BT_HID_INVALID_HANDLE;
            }
            break;

        case 11: /* Shutdown HID Host */
            if (0x0U == hid_host_started)
            {
                LOG_DEBUG("HID Host not started...\n");
                break;
            }

            LOG_DEBUG("Stopping HID Host...\n");
            retval = BT_hid_host_stop();

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Stopping HID Host Failed. Reason: 0x%04X\n",
                retval);
                break;
            }
            else
            {
                LOG_DEBUG("HID Host Stopped Successfully\n");
                hid_host_started = 0x0U;
            }

            /* Reset all the handles as free. */
            hid_device_num_handles_free = BT_HID_HOST_MAX_DEVS;

            /* Reset all handles to unused state [BT_HID_INVALID_HANDLE]. */
            for ( index = 0U; index < BT_HID_HOST_MAX_DEVS; index++ )
            {
                hid_device_handle_list[index].handle = BT_HID_INVALID_HANDLE;
            }
            break;

        case 12: /* HID Host Registration. */

            LOG_DEBUG("Registering HID Host ... "); fflush (stdout);

            retval = BT_hid_host_register
                     (
                         BT_HID_DEFAULT_TRANS_MTU,
                         BT_HID_DEFAULT_RECV_MTU,
                         appl_hid_host_report_ind_cb,
                         appl_hid_host_event_ind_cb
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("FAILED !! Reason = 0x%04x.\n", retval);
            }
            else
            {
                LOG_DEBUG("SUCCEEDED !\n");
            }
            break;

        case 20: /* ADD HID Device */

            /* Check if any free handles are available.*/
            if (0U == hid_device_num_handles_free)
            {
                LOG_DEBUG("Error: No free handles available\n");
                break;
            }

            LOG_DEBUG("Enter Bluetooth Device Address of HID Device\n");
            retval = appl_get_bd_addr(hid_device_bd_addr);

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Invalid Bluetooth Device Address Entered\n");
                break;
            }

            retval = BT_hid_host_add_device
                     (
                         hid_device_bd_addr,
                         BT_HID_DEFAULT_TRANS_MTU,
                         BT_HID_DEFAULT_RECV_MTU,
                         appl_hid_host_report_ind_cb,
                         appl_hid_host_event_ind_cb,
                         &hid_device_dev_handle
                     );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Adding HID Device has Failed. Reason: 0x%04X\n",
                retval);
            }
            else
            {
                LOG_DEBUG("HID Device Added Successfully.\n");
                LOG_DEBUG("HID Device Handle: 0x%02X\n", hid_device_dev_handle);

                /* Save the handle and address in the added devices list. */
                hid_device_add_to_handle_list
                (
                    hid_device_dev_handle
                );
            }

            break;

        case 21: /* Remove HID Device */

            hid_device_dev_handle = hid_device_get_handle();

            if (BT_HID_INVALID_HANDLE == hid_device_dev_handle)
            {
                LOG_DEBUG("ERROR: Invalid handle\n");
                break;
            }

            retval = BT_hid_host_remove_device
                     (
                         hid_device_dev_handle
                     );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Removal of HID Device has Failed. Reason: 0x%04X\n",
                retval);
            }
            else
            {
                LOG_DEBUG("HID Device Removed Successfully\n");

                /* Remove the handle from the added devices list. */
                hid_device_remove_from_handle_list(hid_device_dev_handle);
            }
            break;

        case 25: /* HCI Inquiry */
        {
            UINT32 lap;
            UCHAR  inq_length;
            UCHAR  num_resp;

            lap = BT_GIAC;
            inq_length = 0x10U;
            num_resp = 0U;

            LOG_DEBUG("Starting HCI Inquiry\n");
            retval = BT_hci_inquiry(lap, inq_length, num_resp);

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
            }
            else
            {
                LOG_DEBUG("Successfully started.\n");
                LOG_DEBUG("Please wait for Inquiry Results & Inquiry Complete.\n");
            }
        }
        break;

        case 30: /* Create ACL connection */
            /**
             * Select the handle from the added device list
             */
            hid_device_dev_handle = hid_device_get_handle();
            if (BT_HID_INVALID_HANDLE == hid_device_dev_handle)
            {
                LOG_DEBUG("ERROR: Invalid handle\n");
                break;
            }

            /* Get BD address from given handle */
            retval = BT_hid_host_get_addr_from_handle
                     (
                         hid_device_dev_handle,
                         hid_device_bd_addr
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG( " Failed to get the address of the device\n");
            }
            else
            {
                LOG_DEBUG(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER,
                BT_DEVICE_ADDR_ONLY_PRINT_STR (hid_device_bd_addr));
            }

            retval =  BT_hci_create_connection
                      (
                          hid_device_bd_addr,
                          LMP_ACL_DM1|LMP_ACL_DM3|LMP_ACL_DM5|
                          LMP_ACL_DH1|LMP_ACL_DH3|LMP_ACL_DH5,
                          0U, 0U, 0U,
                          0U
                      );

            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("HCI create connection success\n");
            }
            else
            {
                LOG_DEBUG("Error creating ACL , API RETVAL 0x%04X\n",retval);
            }
            break;

        case 31: /* Perform SDP Query */
            hid_device_dev_handle = hid_device_get_handle();
            if (BT_HID_INVALID_HANDLE == hid_device_dev_handle)
            {
                LOG_DEBUG("ERROR: Invalid handle\n");
                break;
            }

            /* Perform the SDP query for the device added. */

            retval = BT_hid_host_get_addr_from_handle
                     (
                         hid_device_dev_handle,
                         hid_device_bd_addr
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG( " Failed to get the address of the device\n");
            }
            else
            {
                LOG_DEBUG(" Performing SDP Query on device with BD adrress:\n");

                LOG_DEBUG(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER,
                BT_DEVICE_ADDR_ONLY_PRINT_STR (hid_device_bd_addr));
            }

            /* Set the SDP Handle */
            SDP_SET_HANDLE
            (
                sdp_handle,
                hid_device_bd_addr,
                hid_host_sdp_cb
            );

            retval = BT_sdp_open(&sdp_handle);
            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("SDP Open Failed. Reason: 0x%04X\n",
                retval);
            }
            else
            {
                LOG_DEBUG("Started SDP Query\n");
            }
            break;

        case 32:/* Release ACL with HID Device */
            retval = BT_hci_get_acl_connection_handle
                     (
                         hid_device_bd_addr,
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

        case 33:
            retval = BT_hci_get_acl_connection_handle
                     (
                         hid_device_bd_addr,
                         &hci_connection_handle
                     );

            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("ACL Connection Handle: 0x%04X\n",
                hci_connection_handle);
            }
            else
            {
                LOG_DEBUG ("ACL Connection for HID Host not found\n");
                LOG_DEBUG("Enter ACL Handle\n");
                scanf("%x",&option);
                hci_connection_handle = (UINT16)option;
            }

            LOG_DEBUG("Updating HCI Sniff Subrating. Latency: 0x%04X, Timeout: 0x%04X\n",
                ssr_latency, ssr_timeout);

            /* Update Sniff subrating */
            (BT_IGNORE_RETURN_VALUE) BT_hci_sniff_subrating
            (
                hci_connection_handle,
                ssr_latency,
                ssr_timeout,
                ssr_timeout
            );
            break;

        case 34:
            retval = BT_hci_get_acl_connection_handle
                     (
                         hid_device_bd_addr,
                         &hci_connection_handle
                     );

            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("ACL Connection Handle: 0x%04X\n",
                hci_connection_handle);
            }
            else
            {
                LOG_DEBUG ("ACL Connection for HID Host not found\n");
                LOG_DEBUG("Enter ACL Handle\n");
                scanf("%x",&option);
                hci_connection_handle = (UINT16)option;
            }

            LOG_DEBUG("Enter Link Supervision Timeout: 0x");
            scanf("%x", &option);
            lsto = (UINT16)option;

            /* Update LSTO */
            (BT_IGNORE_RETURN_VALUE) BT_hci_write_link_supervision_timeout
            (
                hci_connection_handle,
                lsto
            );
            break;

        case 40: /* Connect with HID Device */
            hid_device_dev_handle = hid_device_get_handle();
            if (BT_HID_INVALID_HANDLE == hid_device_dev_handle)
            {
                LOG_DEBUG("ERROR: Invalid handle\n");
                break;
            }

            retval = BT_hid_host_connect_ctrl_ch_to_device
                     (
                         hid_device_dev_handle
                     );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Connection Initiation Failed. Reason: 0x%04X\n",
                retval);
            }
            else
            {
                LOG_DEBUG("Connecting ...\n");
            }
            break;

        case 41: /* Connect with HID Device */
            hid_device_dev_handle = hid_device_get_handle();
            if (BT_HID_INVALID_HANDLE == hid_device_dev_handle)
            {
                LOG_DEBUG("ERROR: Invalid handle\n");
                break;
            }

            retval = BT_hid_host_connect_intr_ch_to_device
                     (
                         hid_device_dev_handle
                     );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("Connection Initiation Failed. Reason: 0x%04X\n",
                    retval);
            }
            else
            {
                LOG_DEBUG("Connecting ...\n");
            }
            break;

        case 42: /* Disconnect from HID Device */
            hid_device_dev_handle = hid_device_get_handle();
            if (BT_HID_INVALID_HANDLE == hid_device_dev_handle)
            {
                LOG_DEBUG("ERROR: Invalid handle\n");
                break;
            }

            LOG_DEBUG ("Enter the Channel: \n");
            LOG_DEBUG ("  0 -> Both the Channels \n");
            LOG_DEBUG ("  1 -> Control Channel \n");
            LOG_DEBUG ("  2 -> Interrupt Channel \n");
            scanf ("%d", &option);

            if (0U == option)
            {
                retval = BT_hid_host_disconnect_device
                         (
                             hid_device_dev_handle,
                             (HID_HOST_CTRL_DISCONNECTION | HID_HOST_INTR_DISCONNECTION)
                         );

                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("Disconnection Initiation for Control & Interrupt Channel Failed. Reason: 0x%04X\n",
                    retval);
                }
                else
                {
                    LOG_DEBUG("Disconnecting Control & Interrupt Channel...\n");
                }
            }
            else if (1U == option)
            {
                retval = BT_hid_host_disconnect_device
                         (
                             hid_device_dev_handle,
                             HID_HOST_CTRL_DISCONNECTION
                         );

                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("Disconnection Initiation for Control Channel Failed. Reason: 0x%04X\n",
                    retval);
                }
                else
                {
                    LOG_DEBUG("Disconnecting Control Channel...\n");
                }
            }
            else if (2U == option)
            {
                retval = BT_hid_host_disconnect_device
                         (
                             hid_device_dev_handle,
                             HID_HOST_INTR_DISCONNECTION
                         );

                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("Disconnection Initiation for Interrupt Channel Failed. Reason: 0x%04X\n",
                    retval);
                }
                else
                {
                    LOG_DEBUG("Disconnecting Interrupt Channel...\n");
                }
            }
            else
            {
                LOG_DEBUG ("Invalid Option: %d\n", option);
            }

            break;

        case 43: /* Virtual Cable Unplug */
            hid_device_dev_handle = hid_device_get_handle();
            if (BT_HID_INVALID_HANDLE == hid_device_dev_handle)
            {
                LOG_DEBUG("ERROR: Invalid handle\n");
                break;
            }

            /* Delete HID Device Link Key */
            (BT_IGNORE_RETURN_VALUE) BT_sm_delete_device_link_key (hid_device_bd_addr);

            retval = BT_hid_host_virtual_cable_unplug
                     (
                         hid_device_dev_handle
                     );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Virtual Cable Unplug Failed. Reason: 0x%04X\n",
                retval);
            }
            else
            {
                LOG_DEBUG("Virtual Cable Unplug In-progress ...\n");
            }
            break;

        case 50:  /* Get Protocol */
            hid_device_dev_handle = hid_device_get_handle();
            if (BT_HID_INVALID_HANDLE == hid_device_dev_handle)
            {
                LOG_DEBUG("ERROR: Invalid handle\n");
                break;
            }

            retval = BT_hid_host_get_protocol
                     (
                          hid_device_dev_handle
                     );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Get Protocol Failed. Reason: 0x%04X\n",
                retval);
            }
            else
            {
                LOG_DEBUG("Get Protocol In-progress ...\n");
            }
            break;

        case 51:  /* Set Protocol */
            hid_device_dev_handle = hid_device_get_handle();
            if (BT_HID_INVALID_HANDLE == hid_device_dev_handle)
            {
                LOG_DEBUG("ERROR: Invalid handle\n");
                break;
            }

            LOG_DEBUG("%s", hid_protocol_types);
            scanf("%d", &input_read);
            protocol = (HID_REPORT_TYPE)input_read;

            retval = BT_hid_host_set_protocol
                     (
                          hid_device_dev_handle,
                          protocol
                     );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Set Protocol Failed. Reason: 0x%04X\n",
                retval);
            }
            else
            {
                LOG_DEBUG("Set Protocol In-progress ...\n");
            }
            break;

        case 60:  /* Get Report */
            hid_device_dev_handle = hid_device_get_handle();
            if (BT_HID_INVALID_HANDLE == hid_device_dev_handle)
            {
                LOG_DEBUG("ERROR: Invalid handle\n");
                break;
            }

            LOG_DEBUG("%s", hid_report_types);
            scanf("%d", &input_read);
            report_type = (HID_REPORT_TYPE)input_read;

            LOG_DEBUG("Enter Report ID\n");
            scanf("%d", &input_read);
            report_id = (UCHAR)input_read;

            report.report_type = (HID_REPORT_TYPE)input_read;

            LOG_DEBUG("Enter Report Length\n");
            scanf("%d", &input_read);
            report.report_len = (UINT16)input_read;

            retval = BT_hid_host_get_report
                     (
                          hid_device_dev_handle,
                          report_type,
                          &report,
                          report_id
                     );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Get Report Failed. Reason: 0x%04X\n",
                retval);
            }
            else
            {
                LOG_DEBUG("Get Report In-progress ...\n");
            }
            break;

        case 61:  /* Set Report */
            hid_device_dev_handle = hid_device_get_handle();
            if (BT_HID_INVALID_HANDLE == hid_device_dev_handle)
            {
                LOG_DEBUG("ERROR: Invalid handle\n");
                break;
            }

            LOG_DEBUG("%s", hid_report_types);
            scanf("%d", &input_read);
            report.report_type = (HID_REPORT_TYPE)input_read;

            LOG_DEBUG("Enter Report Length\n");
            scanf("%d", &input_read);
            report.report_len = (UINT16)input_read;

            if( report.report_len > BT_HID_HOST_MAX_REPORT_LEN)
            {
                LOG_DEBUG("Report Length (%d)> BT_HID_HOST_MAX_REPORT_LEN (%d)\n",
                report.report_len, BT_HID_HOST_MAX_REPORT_LEN);
                break;
            }

            LOG_DEBUG("Enter Report Stream (in HEX)\n");

            for(index = 0U; index < report.report_len; index++)
            {
                scanf("%x", &input_read);
                report.report[index] = (UCHAR)input_read;
            }
            retval = BT_hid_host_set_report
                     (
                          hid_device_dev_handle,
                          &report
                     );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Set Report Failed. Reason: 0x%04X\n",
                retval);
            }
            else
            {
                LOG_DEBUG("Set Report In-progress ...\n");
            }
            break;

        case 70:  /* Get Idle Rate */
#ifndef HID_1_0
            LOG_DEBUG ("Get Idle is only supported for HID 1.0\n");
#else
#ifndef HID_SUPPORT_GET_IDLE
            LOG_DEBUG ("Define HID_SUPPORT_GET_IDLE feature flag\n");
#else
            hid_device_dev_handle = hid_device_get_handle();
            if (BT_HID_INVALID_HANDLE == hid_device_dev_handle)
            {
                LOG_DEBUG("ERROR: Invalid handle\n");
                break;
            }

            retval = BT_hid_host_get_idle_rate
                     (
                          hid_device_dev_handle
                     );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Get Idle Rate Failed. Reason: 0x%04X\n",
                retval);
            }
            else
            {
                LOG_DEBUG("Get Idle Rate In-progress ...\n");
            }
#endif /* HID_SUPPORT_GET_IDLE */
#endif /* HID_1_0 */
            break;

        case 71:  /* Set Idle Rate */
#ifndef HID_1_0
            LOG_DEBUG ("Set Idle is only supported for HID 1.0\n");
#else
#ifndef HID_SUPPORT_SET_IDLE
            LOG_DEBUG ("Define HID_SUPPORT_SET_IDLE feature flag\n");
#else
            hid_device_dev_handle = hid_device_get_handle();
            if (BT_HID_INVALID_HANDLE == hid_device_dev_handle)
            {
                LOG_DEBUG("ERROR: Invalid handle\n");
                break;
            }

            /* Zero (0) represents Infinite Idle Rate */
            LOG_DEBUG("Enter New Idle Rate\n");
            scanf("%d", &input_read);
            new_idle_rate = (UCHAR)input_read;

            retval = BT_hid_host_set_idle_rate
                     (
                          hid_device_dev_handle,
                          new_idle_rate
                     );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Set Idle Rate Failed. Reason: 0x%04X\n",
                retval);
            }
            else
            {
                LOG_DEBUG("Set Idle Rate In-progress ...\n");
            }
#endif /* HID_SUPPORT_SET_IDLE */
#endif /* HID_1_0 */
            break;

        case 80:  /* Send Control Packet */
            hid_device_dev_handle = hid_device_get_handle();
            if (BT_HID_INVALID_HANDLE == hid_device_dev_handle)
            {
                LOG_DEBUG("ERROR: Invalid handle\n");
                break;
            }

            LOG_DEBUG("%s", hid_control_options);
            scanf("%d", &input_read);
            op_code = (HID_CONTROL_OP_CODE)input_read;

            if(op_code == (HID_CONTROL_OP_CODE)HID_HOST_VIRTUAL_CABLE_UNPLUG)
            {
                /* Delete HID Device Link Key */
                (BT_IGNORE_RETURN_VALUE) BT_sm_delete_device_link_key (hid_device_bd_addr);
            }

            /* A HID_CONTROL request does not generate a HANDSHAKE response */
            retval = BT_hid_host_send_hid_control
                     (
                          hid_device_dev_handle,
                          op_code
                     );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Sending Control Packet Failed. Reason: 0x%04X\n",
                retval);
            }
            else
            {
                LOG_DEBUG("Sending Control Packet In-progress ...\n");
            }
            break;

        case 81:  /* Send Interrupt Packet */
            hid_device_dev_handle = hid_device_get_handle();
            if (BT_HID_INVALID_HANDLE == hid_device_dev_handle)
            {
                LOG_DEBUG("ERROR: Invalid handle\n");
                break;
            }

            /* Get HID_HOST_REPORT from user */
            LOG_DEBUG("%s", hid_report_types);
            scanf("%d", &input_read);
            report.report_type = (HID_REPORT_TYPE)input_read;

            LOG_DEBUG("Enter Report Length\n");
            scanf("%d", &input_read);
            report.report_len = (UINT16)input_read;

            if( report.report_len > BT_HID_HOST_MAX_REPORT_LEN)
            {
                LOG_DEBUG("Report Length > BT_HID_HOST_MAX_REPORT_LEN \n");
                break;
            }

            LOG_DEBUG("Enter Report Stream (in HEX)\n");

            for(index = 0U; index < report.report_len; index++)
            {
                scanf("%x", &input_read);
                report.report[index] = (UCHAR)input_read;
            }

            retval = BT_hid_host_send_report_on_interrupt
                     (
                          hid_device_dev_handle,
                          &report
                     );

            if(API_SUCCESS != retval)
            {
                LOG_DEBUG("Sending Interrupt Packet Failed. Reason: 0x%04X\n",
                retval);
            }
            else
            {
                LOG_DEBUG("Sending Interrupt Packet In-progress ...\n");
            }
            break;

        case 100: /* Delete Link Key */
            LOG_DEBUG("Deleting Device Link Key of "\
                BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
                BT_DEVICE_ADDR_ONLY_PRINT_STR(hid_device_bd_addr));

            retval = BT_sm_delete_device_link_key(hid_device_bd_addr);
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
}

void appl_hid_host_report_ind_cb
     (
         HID_HOST_DEV_HANDLE   handle,
         HID_HOST_REPORT     * report
     )
{
#ifndef BT_DRIVER_MODE

    UCHAR *data;
    UINT16 datalen;

#endif /* BT_DRIVER_MODE */

    BT_IGNORE_UNUSED_PARAM(handle);

    LOG_DEBUG("[HID HOST APPL] Report Indication Callback\n");

#ifndef BT_DRIVER_MODE

    if ((NULL == report) || (0U == report->report_len))
    {
        LOG_DEBUG("[HID HOST APPL] (Report == NULL) OR (Report Length == 0)");
        /* return; */
    }
    else
    {
        LOG_DEBUG("Report Type = 0x%02X, Report Length = %d\n",
        report->report_type, report->report_len);

        appl_dump_bytes(report->report, report->report_len);

        /*
         * Keyboard Boot Report will be 9 Bytes long
         * Mouse Boot Report will be 4 Bytes long
         */
         /* Mouse Report */
        if (0x02U == report->report[0U])
        {
            if (report->report_len >= 4U)
            {
                data = (UCHAR *)&(report->report[1U]);
                datalen = (UINT16)(report->report_len - 1U);
                hid_uinput_mouse_irq(data, datalen);
            }
        }
        /* Keyboard Report */
        else if (0x01U == report->report[0U])
        {
            if (report->report_len >= 9U)
            {
                data = (UCHAR *)&(report->report[1U]);
                datalen = (UINT16)(report->report_len - 1U);

                hid_uinput_kbd_irq(data, datalen);
            }
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
#endif /* BT_DRIVER_MODE */

    BT_IGNORE_UNUSED_PARAM(data);
    BT_IGNORE_UNUSED_PARAM(datalen);
    return;
}

void appl_hid_host_event_ind_cb
     (
         HID_HOST_EVT_TYPE event_type,
         HID_HOST_DEV_HANDLE handle,
         API_RESULT result,
         UCHAR *event_data,
         UINT16 event_data_len
     )
{
    UINT16 index;
    API_RESULT retval;
    UINT16 hci_connection_handle;

    LOG_DEBUG("HID Event Indication Callback \n");
    LOG_DEBUG("Event Type 0x%02X\n", event_type);
    LOG_DEBUG("Device Handle %d\n", handle);
    LOG_DEBUG("Result 0x%04X\n", result);
    LOG_DEBUG("Event Data Length %d\n", event_data_len);

    switch(event_type)
    {
        case HID_HOST_CTRL_CONNECTION:
            LOG_DEBUG("[HID HOST APPL] HID Connection Complete Event for Control Channel\n");
            LOG_DEBUG("Result: 0x%04X\n", result);

            /* TBD: Review */
            hid_device_add_to_handle_list(handle);
            break;

        case HID_HOST_CTRL_DISCONNECTION:
            LOG_DEBUG("[HID HOST APPL] HID Disconnection Complete Event for Control Channel\n");
            LOG_DEBUG("Result: 0x%04X\n", result);

            if (0x00U != hid_vcu_status_flag)
            {
                /* Reset Virtual Cable Unplug status flag */
                hid_vcu_status_flag = 0x00U;

                /* Initiate ACL disconnection if no other profile is connected */
                LOG_DEBUG ("Initiating ACL Connection for HID Virtual Cable Unplug Indication.\n");

                retval = BT_hci_get_acl_connection_handle
                         (
                             hid_device_bd_addr,
                             &hci_connection_handle
                         );
                if (API_SUCCESS == retval)
                {
                    LOG_DEBUG("Disconnecting ACL Connection 0x%04X\n",
                    hci_connection_handle);

                    retval = BT_hci_disconnect
                             (
                                 hci_connection_handle,
                                 0x13U
                             );

                    LOG_DEBUG("> API RETVAL ACL disconnect : 0x%04X\n", retval);
                }
            }

            break;

        case HID_HOST_INTR_CONNECTION:
            LOG_DEBUG("[HID HOST APPL] HID Connection Complete Event for Interrupt Channel\n");
            LOG_DEBUG("Result: 0x%04X\n", result);

            /* hid_device_add_to_handle_list(handle); */
            break;

        case HID_HOST_INTR_DISCONNECTION:
            LOG_DEBUG("[HID HOST APPL] HID Disconnection Complete Event for Interrupt Channel\n");
            LOG_DEBUG("Result: 0x%04X\n", result);
            break;

        case HID_HOST_VIRTUAL_CABLE_UNPLUG_IND:
            LOG_DEBUG("[HID HOST APPL] HID Virtual Cable Unplug Indication\n");
            LOG_DEBUG("Result: 0x%04X\n", result);

            if (API_SUCCESS == result)
            {
                /* Delete HID Device Link Key */
                (BT_IGNORE_RETURN_VALUE) BT_sm_delete_device_link_key (hid_device_bd_addr);

                /* Mark the Virtual Cable Unplug is on-going */
                hid_vcu_status_flag = 0x01U;

                /* Initiate disconnection for both control and interrupt channel */
                retval = BT_hid_host_disconnect_device
                         (
                             handle,
                             (HID_HOST_CTRL_DISCONNECTION | HID_HOST_INTR_DISCONNECTION)
                         );
                if(API_SUCCESS != retval)
                {
                    LOG_DEBUG("Disconnection Initiation for Control & Interrupt Channel Failed. Reason: 0x%04X\n",
                    retval);
                }
                else
                {
                    LOG_DEBUG("Disconnecting Control & Interrupt Channel...\n");
                }
            }

            break;

        case HID_HOST_GET_PROTOCOL_RSP:
            LOG_DEBUG("[HID HOST APPL] HID GET_PROTOCOL Response\n");

            if(API_SUCCESS != result)
            {
                LOG_DEBUG("GET_PROTOCOL Failed\n");
                break;
            }

            if(1U != event_data_len)
            {
                LOG_DEBUG("Wrong Length for GET_PROTOCOL Response (%d)\n",
                event_data_len);
            }
            else
            {
                if(HID_HOST_BOOT_PROTOCOL == (*event_data))
                {
                    LOG_DEBUG("BOOT Protocol\n");
                }
                else if(HID_HOST_REPORT_PROTOCOL == (*event_data))
                {
                    LOG_DEBUG("REPORT Protocol\n");
                }
                else
                {
                    LOG_DEBUG("Undefined HID Protocol (0x%02X)\n",
                    (*event_data));
                }
            }

            break;

#ifdef HID_SUPPORT_GET_IDLE
        case HID_HOST_GET_IDLE_RSP:
            LOG_DEBUG("[HID HOST APPL] HID GET_IDLE Response\n");

            if(API_SUCCESS != result)
            {
                LOG_DEBUG("GET_IDLE Failed\n");
                break;
            }

            if(1U != event_data_len)
            {
                LOG_DEBUG("Wrong Length for GET_PROTOCOL Response (%d)\n",
                event_data_len);
            }
            else
            {
                LOG_DEBUG("Current Idle Rate (0x%02X)\n",
                (*event_data));
            }
            break;
#endif /* HID_SUPPORT_GET_IDLE */

        case HID_HOST_GET_REPORT_RSP:
            LOG_DEBUG("[HID HOST APPL] HID GET_REPORT Response\n");

            if(API_SUCCESS != result)
            {
                LOG_DEBUG("GET_REPORT Failed\n");
                break;
            }

            /* Print the Report */
            LOG_DEBUG("Get Report Successful\n");

            /* Print Report */
            for(index = 0U; index < event_data_len; index ++)
            {
                /* Print 10 Bytes in a single line */
                if(0U == index % 10U)
                {
                    LOG_DEBUG("\n");
                }

                LOG_DEBUG("%02X ", event_data[index]);
            }

            LOG_DEBUG("\n");

            break;

        case HID_HOST_SET_REPORT_RSP:
            LOG_DEBUG("[HID HOST APPL] HID SET_REPORT Response\n");

            if(API_SUCCESS == result)
            {
                LOG_DEBUG("SET_REPORT Successful\n");
            }
            else
            {
                LOG_DEBUG("SET_REPORT Failed. Result Code (0x%02X)\n",
                (*event_data));
            }
            break;

        case HID_HOST_SET_PROTOCOL_RSP:
            LOG_DEBUG("[HID HOST APPL] HID SET_PROTOCOL Response\n");

            if(API_SUCCESS == result)
            {
                LOG_DEBUG("SET_PROTOCOL Successful\n");
            }
            else
            {
                LOG_DEBUG("SET_PROTOCOL Failed. Result Code (0x%02X)\n",
                (*event_data));
            }
            break;

#ifdef HID_SUPPORT_SET_IDLE
        case HID_HOST_SET_IDLE_RSP:
            LOG_DEBUG("[HID HOST APPL] HID SET_IDLE Response\n");

            if(API_SUCCESS == result)
            {
                LOG_DEBUG("SET_IDLE Successful\n");
            }
            else
            {
                LOG_DEBUG("SET_IDLE Failed. Result Code (0x%02X)\n",
                (*event_data));
            }
            break;
#endif /* HID_SUPPORT_SET_IDLE */

        default:
            LOG_DEBUG("[HID HOST APPL] Unknown Event Type: 0x%02X\n", event_type);
            break;
    }

    return;
}

/**
 *  This function is SDP Callback handler for HID Host
 *
 *  \param command (IN)
 *
 *  \param data (IN)
 *  \param length (IN)
 *  \param status (IN)
 *
 *  \return None
 */
void hid_host_sdp_cb
     (
          /* IN */ UCHAR command,
          /* IN */ UCHAR * data,
          /* IN */ UINT16 length,
          /* IN */ UINT16 status
     )
{
    UCHAR *attrib_data = NULL;
    API_RESULT retval;

    S_UUID     uuid[2U];
    UCHAR      num_uuids                = 2U;

    UINT16     attrib_ids[10U];
    UINT16     num_attribs              = 0U;

    UINT16     len_attrib_data          = 1000U;

    UCHAR      reconnect_init           = 0U;
    UINT16     reconnect_init_len       = 0U;

    UCHAR      normally_connectable     = 0U;
    UINT16     normally_connectable_len = 0U;

#ifdef HID_1_1
    UCHAR      ssr_val[2U];
    UINT16     ssr_len;
#endif /* HID_1_1 */

    HID_HOST_DEV_HANDLE handle;

    retval = API_SUCCESS;

    if(API_SUCCESS != status)
    {
        LOG_DEBUG ("SDP Operation Failed.\n");
        retval = API_FAILURE; /* return; */
    }
    else
    {
        retval = BT_hid_host_get_handle_from_addr(sdp_handle.bd_addr, &handle);
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("Failed to get a valid handle\n");
            retval = API_FAILURE; /* return; */
        }
    }

    if (API_SUCCESS == retval)
    {
        switch(command)
        {
            case SDP_Open : /* SDP open callback */
                attrib_data = BT_alloc_mem(1000U);

                if(NULL == attrib_data)
                {
                    LOG_DEBUG ("Allocation for Attribute data Failed.\n");
                    (BT_IGNORE_RETURN_VALUE) BT_sdp_close (&sdp_handle);
                    break;
                }

                uuid[0U].uuid_type = UUID_16;
                uuid[1U].uuid_type = UUID_16;

                uuid[0U].uuid_union.uuid_16 = HID_SERVICE_UUID;
                uuid[1U].uuid_union.uuid_16 = L2CAP_UUID;

                /* Attribute ID for Protocol Descriptor List */
                attrib_ids[0U] = PROTOCOL_DESC_LIST;

                /* Attribute ID for Additional Protocol Descriptor List */
                attrib_ids[1U] = ADDITIONAL_PROT_DESC_LIST_ID;

                /* Attribute ID for HIDReconnectInit boolean value */
                attrib_ids[2U] = HID_RECONNECT_INITIATE_ATTR_ID;

                /* Attribute ID for HID Descriptor List */
                attrib_ids[3U] = HID_DESCRIPTOR_LIST_ATTR_ID;

                /* Attribute ID for HID Normally Connectable boolean value */
                attrib_ids[4U] = HID_NORMALLY_CONNECTABLE_ATTR_ID;

                num_attribs = 5U;

    #ifdef HID_1_1
                /* Attribute ID for SSR Host Max. Latency Attribute ID */
                attrib_ids[5U] = HID_SSR_HOST_MAX_LATENCY_ATTR_ID;

                /* Attribute ID for SSR Host Min. Timeout Attribute ID */
                attrib_ids[6U] = HID_SSR_HOST_MIN_TIMEOUT_ATTR_ID;

                num_attribs += 2U;
    #endif /* HID_1_1 */

                /* Do Service Search Request */
                retval = BT_sdp_servicesearchattributerequest
                         (
                             &sdp_handle,
                             uuid,
                             num_uuids,
                             attrib_ids,
                             num_attribs,
                             NULL,
                             0x00U,
                             attrib_data,
                             &len_attrib_data
                         );

                if(API_SUCCESS != retval)
                {
                    BT_free_mem(attrib_data);
                    (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_handle);
                }
                break;

            case SDP_Close: /* SDP Close callback */
                LOG_DEBUG ("SDP Closed.\n");
                break;

            case SDP_ServiceSearchAttributeResponse:
                /* Obtain the HIDReconnectInit attribute value */
                reconnect_init_len = length;
                if (API_SUCCESS != BT_sdp_get_attr_value(HID_RECONNECT_INITIATE_ATTR_ID, data, &reconnect_init, &reconnect_init_len))
                {
                    LOG_DEBUG ("BT_sdp_get_attr_value Failed\n" );
                    BT_free_mem(data);
                    break;
                }

                LOG_DEBUG ("HIDReconnectInit value = %d, Length = %d.\n",
                reconnect_init, reconnect_init_len );

                hid_device_handle_list[handle].dev_attr_hid_reconnect_initiate =
                    (BOOLEAN)reconnect_init;

                /* Obtain the Report Descriptor values */
                hid_device_handle_list[handle].report_desc_len = 0U;

                BT_mem_set(hid_device_handle_list[handle].report_desc, 0, APPL_HID_HOST_MAX_DESC_LEN);

                hid_device_handle_list[handle].report_desc_len = length;
                if (API_SUCCESS != BT_sdp_get_DataSequence_attr_val(HID_DESCRIPTOR_LIST_ATTR_ID, data, hid_device_handle_list[handle].report_desc, &hid_device_handle_list[handle].report_desc_len))
                {
                    LOG_DEBUG ("BT_sdp_get_DataSequence_attr_val Failed\n");
                    BT_free_mem(data);
                    break;
                }

                LOG_DEBUG ("Report Descriptor Length = %d.\n",
                hid_device_handle_list[handle].report_desc_len );

                /* Obtain the HIDNormallyConnectable attribute value */
                normally_connectable_len = length;
                if (API_SUCCESS != BT_sdp_get_attr_value(HID_NORMALLY_CONNECTABLE_ATTR_ID, data, &normally_connectable, &normally_connectable_len))
                {
                    LOG_DEBUG ("BT_sdp_get_attr_val unsuccessful.\n" );
                    BT_free_mem(data);
                    break;
                }

                LOG_DEBUG ("BT_sdp_get_attr_val successful.\n" );

                LOG_DEBUG ("HIDNormallyConnectable value = %d Len = %d.\n",
                        normally_connectable, normally_connectable_len );

                hid_device_handle_list[handle].dev_attr_hid_normally_connectable
                    = (BOOLEAN)normally_connectable;

#ifdef HID_1_1
                /* TBD - Get necessary information (if required) from database */
                /* Obtain the HIDSSRLatency attribute value */
                ssr_len = sizeof(ssr_val);
                if (API_SUCCESS != BT_sdp_get_attr_value(HID_SSR_HOST_MAX_LATENCY_ATTR_ID, data, ssr_val, &ssr_len))
                {
                    LOG_DEBUG("BT_sdp_get_attr_val unsuccessful.\n");
                    BT_free_mem(data);
                    break;
                }
                BT_UNPACK_BE_2_BYTE(&ssr_latency, ssr_val);

                /* Obtain the HIDSSRTimeout attribute value */
                ssr_len = sizeof(ssr_val);
                if (API_SUCCESS != BT_sdp_get_attr_value(HID_SSR_HOST_MIN_TIMEOUT_ATTR_ID, data, ssr_val, &ssr_len))
                {
                    LOG_DEBUG("BT_sdp_get_attr_val unsuccessful.\n");
                    BT_free_mem(data);
                    break;
                }
                BT_UNPACK_BE_2_BYTE(&ssr_timeout, ssr_val);
#endif /* HID_1_1 */

                (BT_IGNORE_RETURN_VALUE) BT_sdp_close ( &sdp_handle );
                break;

            case SDP_ErrorResponse:
                break;

            default: /* Invalid: Nothing to do */
                LOG_DEBUG("Invalid SDP Command Type: 0x%02X\n", command);
                break;
        }
    }
    return;
}

void hid_device_add_to_handle_list
     (
         HID_HOST_DEV_HANDLE handle
     )
{
    /* Ensure if it is a valid handle. */
    if (handle < BT_HID_HOST_MAX_DEVS)
    {
        /* Check if handle is free */
        if (BT_HID_INVALID_HANDLE == hid_device_handle_list[handle].handle)
        {
            hid_device_handle_list[handle].handle = handle;


            /* Decrement the free handles count. */
            hid_device_num_handles_free--;
        }

        LOG_DEBUG("Added handle %d to the list\n",handle);
    }
}


HID_HOST_DEV_HANDLE hid_device_get_handle(void)
{
    HID_HOST_DEV_HANDLE handle;
    int read_val;
    UINT16 index;

    API_RESULT retval;

    UCHAR bd_addr[BT_BD_ADDR_SIZE];

    retval = API_SUCCESS;

    if (BT_HID_HOST_MAX_DEVS == hid_device_num_handles_free)
    {
        LOG_DEBUG("Error: No devices are added in the list. \n");
        retval = BT_HID_INVALID_HANDLE; /* return BT_HID_INVALID_HANDLE; */
    }
    else
    {
        LOG_DEBUG("List of Added Devices:\n");
        for (index = 0U; index < BT_HID_HOST_MAX_DEVS; index++)
        {
            if (BT_HID_INVALID_HANDLE != hid_device_handle_list[index].handle)
            {
                LOG_DEBUG("\t %d\t", hid_device_handle_list[index].handle);

                retval = BT_hid_host_get_addr_from_handle
                         (
                             (UCHAR)index,
                             bd_addr
                         );

                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG( " Failed to get the address of the device\n");
                }
                else
                {
                    LOG_DEBUG(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER,
                    BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));
                }
                LOG_DEBUG ("\n");
            }
        }

        LOG_DEBUG("Choose the handle to be used:\n");
        scanf("%d",&read_val);

        handle = (HID_HOST_DEV_HANDLE)read_val;

        if (handle < BT_HID_HOST_MAX_DEVS)
        {
            /* Check if a valid handle is eneterd. */
            if (BT_HID_INVALID_HANDLE == hid_device_handle_list[handle].handle)
            {
                /* Invalid handle. */
                retval = BT_HID_INVALID_HANDLE; /*  return BT_HID_INVALID_HANDLE; */
            }
            else
            {
                retval = handle; /* return handle; */
            }
        }
        else
        {
            /* Invalid handle. */
            retval = BT_HID_INVALID_HANDLE; /* return BT_HID_INVALID_HANDLE; */
        }
    }

    return (HID_HOST_DEV_HANDLE )retval;
}

void hid_device_remove_from_handle_list(HID_HOST_DEV_HANDLE handle)
{
    if ( handle == hid_device_handle_list[handle].handle )
    {
        /* Found the matching handle. Free it. */
        hid_device_handle_list[handle].handle = BT_HID_INVALID_HANDLE;
        hid_device_num_handles_free++;
    }
    else
    {
        LOG_DEBUG(" ERROR: handle not found in the added device list.\n");
    }

    return;
}
#endif /* HID_HOST */

