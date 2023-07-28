
/**
 *  \file appl_sap_client.c
 *
 *  Source file for SAP Client Command Line Test Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_sap_client.h"
#include "appl_utils.h"

#ifdef SAP_CLIENT
/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static const char sap_client_menu[] = "\n\
-------- SIM Access Client Menu ------- \n\
    0.   Exit \n\
    1.   Refresh menu \n\
\n\
    2.   Register BD_ADDR of SAP Server \n\
    3.   Get Registered BD_ADDR of SAP Server \n\
\n\
   10.   Create ACL Connection\n\
   11.   ACL Disconnection\n\
   12.   Do SDP Query \n\
\n\
   20.   Start SIM Access Client \n\
   21.   Connect to SIM Access Server \n\
   22.   Disconnect from SIM Access Server \n\
   23.   Transfer ATR \n\
   24.   Power SIM Off \n\
   25.   Power SIM On \n\
   26.   Reset SIM \n\
   27.   Transfer APDU \n\
\n\
   30.   Get Card Reader Status \n\
   31.   Set Transport Protocol \n\
Your Option -> \0";

/* SAP Server BD_ADDR */
static UCHAR appl_sap_server_bd_addr[BT_BD_ADDR_SIZE];

static SDP_HANDLE sap_client_sdp_handle;
static UCHAR sap_server_channel;
static UCHAR sap_attrib_data[50U];

/* HCI ACL Connection List */
static HCI_CONNECTION_LIST sap_client_appl_acl_list [BT_MAX_REMOTE_DEVICES];

/* ----------------------------------------- Functions */
void main_sap_client_operations(void)
{
    int choice, menu_choice;
    int connection_handle;
    API_RESULT retval;
    UCHAR * apdu;
    UCHAR indx, type;

    BT_LOOP_FOREVER()
    {
        printf("%s",sap_client_menu);

        scanf("%d",&choice);

        menu_choice = choice;

        switch(choice)
        {
        case 0: /* Exit */
            break; /* return; */

        case 1: /* Refresh */
            break;

        case 2: /* Register BD_ADDR of peer device */
            LOG_DEBUG("Please enter BD ADDR of SAP Server\n");
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(appl_sap_server_bd_addr);
            break;

        case 3:/* Get Registered BD_ADDR of peer device */
            LOG_DEBUG("> Registered Peer BD_ADDR "\
                "%02X:%02X:%02X:%02X:%02X:%02X\n",
                appl_sap_server_bd_addr[0U],
                appl_sap_server_bd_addr[1U],
                appl_sap_server_bd_addr[2U],
                appl_sap_server_bd_addr[3U],
                appl_sap_server_bd_addr[4U],
                appl_sap_server_bd_addr[5U]);
            break;

        case 10:
            /*
             * First Application should establish ACL Connection with
             * the peer Device
             */
            retval = BT_hci_create_connection
                     (
                         appl_sap_server_bd_addr,
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
            scanf("%x", &connection_handle);

            LOG_DEBUG("\n");
            LOG_DEBUG("Starting HCI Disconnection ... "); fflush(stdout);

            /* Mention Reason for Disconnect as User Initiated Disconnection */
            retval = BT_hci_disconnect ((UINT16)connection_handle, 0x13U );

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
             * Start SDP Query to Find out if the Peer Device is running OPP
             * Service and if so on which Server Channel the Remote OPP Server
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

            if(API_FAILURE == sap_client_appl_get_acl_connection(&indx))
            {
                LOG_DEBUG("No ACL link with SAP Server\n");
                break;
            }

            LOG_DEBUG("Querying Remote Device 0x%02X:%02X:%02X:%02X:%02X:%02X\n",
            sap_client_appl_acl_list[indx].bd_addr[0U],
            sap_client_appl_acl_list[indx].bd_addr[1U],
            sap_client_appl_acl_list[indx].bd_addr[2U],
            sap_client_appl_acl_list[indx].bd_addr[3U],
            sap_client_appl_acl_list[indx].bd_addr[4U],
            sap_client_appl_acl_list[indx].bd_addr[5U]);

            SDP_SET_HANDLE
            (
                sap_client_sdp_handle,
                sap_client_appl_acl_list[indx].bd_addr,
                appl_sap_client_sdp_callback
            );

            retval = BT_sdp_open(&sap_client_sdp_handle);

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("SDP Connect FAILED !! Error Code = 0x%04X\n", retval);
            }

            LOG_DEBUG("Please Wait for SDP Operation to Complete\n");

            break;

        case 20: /* Start SIM Access Client */
            retval = BT_sap_client_start(appl_sap_client_appl_cb);
            if(API_SUCCESS == retval)
            {
                LOG_DEBUG("SIM Access Client Started Successfully\n");
            }
            else
            {
                LOG_DEBUG("Error starting SIM Access Client\n");
            }
            break;

        case 21: /* Connect to SIM Access Server */
            if(API_FAILURE == sap_client_appl_get_acl_connection(&indx))
            {
                LOG_DEBUG("No ACL link with SAP Server\n");
                break;
            }

            LOG_DEBUG("Connecting to the SAP Server\n");
            /* Using the same bd_addr that used to create ACL connection */
            (BT_IGNORE_RETURN_VALUE) BT_sap_client_connect
            (
                sap_client_appl_acl_list[indx].bd_addr,
                sap_server_channel
            );
            break;

        case 22: /* Disconnect from SIM Access Server */
            LOG_DEBUG("Disconnecting from SAP Server\n");
            (BT_IGNORE_RETURN_VALUE) BT_sap_client_disconnect();
            break;

        case 23: /* Transfer ATR */
            LOG_DEBUG("Client requests for ATR from SIM\n");
            (BT_IGNORE_RETURN_VALUE) BT_sap_client_trasfer_atr();
            break;

        case 24: /* Power SIM Off */
            LOG_DEBUG("Client requests for power SIM off\n");
            (BT_IGNORE_RETURN_VALUE) BT_sap_client_power_sim_off();
            break;

        case 25: /* Power SIM On */
            LOG_DEBUG("Client requests for power SIM on\n");
            (BT_IGNORE_RETURN_VALUE) BT_sap_client_power_sim_on();
            break;

        case 26: /* Reset SIM */
            LOG_DEBUG("Client requests to reset SIM\n");
            (BT_IGNORE_RETURN_VALUE) BT_sap_client_reset_sim();
            break;

        case 27: /* Transfer APDU */
            LOG_DEBUG("Enter APDU Type (0x04: Command, 0x10: Command7816): \n");
            scanf("%x", &choice);
            type = (UCHAR) choice;

            LOG_DEBUG("Enter APDU length: \n");
            scanf("%d", &choice);

            /* Allocate memory for APDU */
            apdu = BT_alloc_mem(choice);

            if (NULL == apdu)
            {
                LOG_DEBUG("Failed to allocate memory for APDU\n");
                break;
            }

            /* TODO: Length Check */
            LOG_DEBUG("Enter APDU bytes (Hex: 43 E3 ...): ");
            for (indx = 0; indx < choice; indx++)
            {
                scanf("%x", &apdu[indx]);
            }

            LOG_DEBUG("Sending APDU Command...\n");
            retval = BT_sap_client_transfer_apdu_command(type, apdu, (UINT16)choice);
            LOG_DEBUG("Retval - 0x%04X\n", retval);

            /* Free allocated memory */
            BT_free_mem(apdu);
            apdu = NULL;
            break;

        case 30: /* Get Card Reader Status */
            LOG_DEBUG("Client requests for Card Reader Status\n");
            (BT_IGNORE_RETURN_VALUE) BT_sap_client_transfer_card_reader_status();
            break;

        case 31: /* Set Transport Protocol */
            {
                UCHAR transport_protocol;
                int   read_val;

                LOG_DEBUG("Enter Transport Protocol\n");
                scanf("%d", &read_val);

                transport_protocol = (UCHAR)read_val;

                (BT_IGNORE_RETURN_VALUE) BT_sap_client_set_transport_protocol(transport_protocol);
            }
            break;

        default:
            LOG_DEBUG("Invalid Choice\n");
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


API_RESULT appl_sap_client_appl_cb
           (
               UCHAR    event_type,
               UINT16   event_result,
               void   * event_data,
               UINT16   event_datalen
           )
{
    UINT16 index;
    UCHAR *data;
    SAP_MESSAGE sap_msg;
    UCHAR msg_id;
    UCHAR *temp;
    INT32 i;
    UCHAR status_change;
    UCHAR result_code;
    API_RESULT  retval;

    /* Init */
    retval = API_SUCCESS;

    if (API_SUCCESS != event_result)
    {
        LOG_DEBUG("Received SAP Client Callback\n");
        LOG_DEBUG("Event 0x%02X, Result 0x%04X\n",
        event_type, event_result);
        /* return API_SUCCESS; */
    }
    else
    {

        data = (UCHAR *)event_data;
        BT_mem_set(&sap_msg, 0x00, sizeof(SAP_MESSAGE));

        switch(event_type)
        {
        case SAP_RFCOMM_OPEN:
            LOG_DEBUG("SAP_RFCOMM_OPEN->\n");
            LOG_DEBUG("Peer BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
            data[0U], data[1U], data[2U],
            data[3U], data[4U], data[5U]);
            break;

        case SAP_RFCOMM_CLOSE:
            LOG_DEBUG("SAP_RFCOMM_CLOSE->\n");

            LOG_DEBUG("Peer BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
            data[0U], data[1U], data[2U],
            data[3U], data[4U], data[5U]);
            break;

        case SAP_RFCOMM_WRITE:
            LOG_DEBUG("SAP_RFCOMM_WRITE->\n");

            if (NULL == event_data)
            {
                LOG_DEBUG("No event data\n");
                break;
            }

            LOG_DEBUG("Data:\n");

            for (index = 0U; index < event_datalen; index ++)
            {
                LOG_DEBUG("%02X ", data[index]);
            }

            LOG_DEBUG("\n");

            break;

        case SAP_RFCOMM_READ:
            LOG_DEBUG("SAP_RFCOMM_READ->\n");

            if (NULL == event_data)
            {
                LOG_DEBUG("No event data\n");
                break;
            }

            LOG_DEBUG("Data:\n");

            for (index = 0U; index < event_datalen; index ++)
            {
                LOG_DEBUG("%02X ", data[index]);
            }

            LOG_DEBUG("\n");
            msg_id = data[0U];
            switch(msg_id)
            {
                case SAP_MSG_CONNECT_RESP:
                    (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message
                    (
                        &sap_msg,
                        data
                    );
                    LOG_DEBUG("MsgID: CONNECT_RESP\n");
                    LOG_DEBUG("No_of_PARAMs: %02X\n",
                    sap_msg.no_of_parameters);
                    LOG_DEBUG("PARAM ID: ConnectionStatus\n");

                    LOG_DEBUG("PARAM Len: %04X\n",
                    sap_msg.param[0U].param_length);
                break;

                case SAP_MSG_DISCONNECT_RESP:
                    (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message
                    (
                        &sap_msg,
                        data
                    );
                    LOG_DEBUG("MsgID: DISCONNECT_RESP\n");
                    LOG_DEBUG("No_of_PARAMs: %02X\n",
                    sap_msg.no_of_parameters);
                break;

                case SAP_MSG_STATUS_IND:
                    (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message
                    (
                        &sap_msg,
                        data
                    );

                    LOG_DEBUG("MsgID: STATUS_IND\n");
                    LOG_DEBUG("No_of_PARAMs: %02X\n",
                    sap_msg.no_of_parameters);
                    LOG_DEBUG("PARAM ID: StatusChange\n");

                    LOG_DEBUG("PARAM Len: %04X\n",
                    sap_msg.param[0U].param_length);

                    /* TBD: Has to look for the other case */
                    /*
                       sap_srv_max_msg_size = 0;
                     */

                    temp = &data[8U];

                    status_change = 0U;

                    for(i = sap_msg.param[0U].param_length - 1; i >= 0; i--)
                    {
                        status_change |= (*temp) << (8U * i);
                        temp++;
                    }

                    LOG_DEBUG("StatusChange: %02x\n", status_change);

                    /*handling for different status changes*/
                    switch(status_change)
                    {
                        case SAP_STATUS_CHANGE_UNKNOWN_ERROR:
                        {
                            break;
                        }
                        case SAP_STATUS_CHANGE_CARD_RESET:
                        case SAP_STATUS_CHANGE_CARD_NOT_ACCESSIBLE:
                        {
			    (BT_IGNORE_RETURN_VALUE) BT_sap_client_trasfer_atr();
                            break;
                        }
                        case SAP_STATUS_CHANGE_CARD_REMOVED:
                        {
                            break;
                        }
                        case SAP_STATUS_CHANGE_CARD_INSERTED:
                        {
                            break;
                        }
                        case SAP_STATUS_CHANGE_CARD_RECOVERED:
                        {
                            break;
                        }

                        default:
                            LOG_DEBUG("Invalid Status: 0x%02X\n", status_change);
                            break;
                    }

                    break;

                case SAP_MSG_TRANSFER_ATR_RESP:
                    (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message
                    (
                        &sap_msg,
                        data
                    );
                    LOG_DEBUG("MsgID: TRANSFER_ATR_RESP\n");
                    LOG_DEBUG("No_of_PARAMs: %02X\n",
                    sap_msg.no_of_parameters);

                    LOG_DEBUG("PARAM ID: ConnectionStatus\n");

                    LOG_DEBUG("PARAM Len: %04X\n",
                    sap_msg.param[0U].param_length);

                    temp = &data[8U];

                    result_code = 0U;

                    for(i = sap_msg.param[0U].param_length - 1; i >= 0; i--)
                    {
                        result_code |= (*temp) << (8U * i);
                        temp++;
                    }

                    LOG_DEBUG("ResultCode: %02x\n", result_code);

                break;

                case SAP_MSG_TRANSFER_APDU_RESP:
                    (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message
                    (
                        &sap_msg,
                        data
                    );
                    LOG_DEBUG("MsgID: TRANSFER_APDU_RESP\n");
                    LOG_DEBUG("No_of_PARAMs: %02X\n",
                    sap_msg.no_of_parameters);
                    break;

                case SAP_MSG_POWER_SIM_OFF_RESP:
                    (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message
                    (
                        &sap_msg,
                        data
                    );
                    LOG_DEBUG("MsgID: POWER_SIM_OFF_RESP\n");
                    LOG_DEBUG("No_of_PARAMs: %02X\n",
                    sap_msg.no_of_parameters);

                    LOG_DEBUG("PARAM ID: ResultCode\n");

                    LOG_DEBUG("PARAM Len: %04X\n",
                    sap_msg.param[0U].param_length);

                    temp = &data[8U];

                    result_code = 0U;

                    for(i = sap_msg.param[0U].param_length - 1; i >= 0; i--)
                    {
                        result_code |= (*temp) << (8U * i);
                        temp++;
                    }

                    LOG_DEBUG("ResultCode: %02x\n", result_code);
                break;

                case SAP_MSG_POWER_SIM_ON_RESP:
                    (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message
                    (
                        &sap_msg,
                        data
                    );

                    LOG_DEBUG("MsgID: POWER_SIM_ON_RESP\n");
                    LOG_DEBUG("No_of_PARAMs: %02X\n",
                    sap_msg.no_of_parameters);

                    LOG_DEBUG("PARAM ID: ResultCode\n");

                    LOG_DEBUG("PARAM Len: %04X\n",
                    sap_msg.param[0U].param_length);

                    temp = &data[8U];

                    result_code = 0U;

                    for(i = sap_msg.param[0U].param_length - 1; i >= 0; i--)
                    {
                        result_code |= (*temp) << (8U * i);
                        temp++;
                    }

                    LOG_DEBUG("ResultCode: %02x\n", result_code);

                    LOG_DEBUG("Client requests for ATR from SIM\n");
                    (BT_IGNORE_RETURN_VALUE) BT_sap_client_trasfer_atr();
                break;

                case SAP_MSG_RESET_SIM_RESP:
                    (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message
                    (
                        &sap_msg,
                        data
                    );
                    LOG_DEBUG("MsgID: RESET_SIM_RESP\n");
                    LOG_DEBUG("No_of_PARAMs: %02X\n",
                    sap_msg.no_of_parameters);

                    LOG_DEBUG("PARAM ID: ResultCode\n");

                    LOG_DEBUG("PARAM Len: %04X\n",
                    sap_msg.param[0U].param_length);

                    temp = &data[8U];

                    result_code = 0U;

                    for(i = sap_msg.param[0U].param_length - 1; i >= 0; i--)
                    {
                        result_code |= (*temp) << (8U * i);
                        temp++;
                    }

                    LOG_DEBUG("ResultCode: %02x\n", result_code);

		    LOG_DEBUG("Client requests for ATR from SIM\n");
                    (BT_IGNORE_RETURN_VALUE) BT_sap_client_trasfer_atr();
                break;

                case SAP_MSG_TRANSFER_CARD_READER_STATUS_RESP:
                    (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message
                    (
                        &sap_msg,
                        data
                    );

                    LOG_DEBUG("MsgID: SAP_MSG_TRANSFER_CARD_READER_STATUS_RESP\n");
                    LOG_DEBUG("No_of_PARAMs: %02X\n",
                    sap_msg.no_of_parameters);

                    LOG_DEBUG("PARAM ID: ResultCode\n");

                    LOG_DEBUG("PARAM Len: %04X\n",
                    sap_msg.param[0U].param_length);

                    temp = &data[8U];

                    result_code = 0U;

                    for(i = sap_msg.param[0U].param_length - 1; i >= 0; i--)
                    {
                        result_code |= (*temp) << (8U * i);
                        temp++;
                    }

                    LOG_DEBUG("ResultCode: %02x\n", result_code);
                break;

                case SAP_MSG_SET_TRANSPORT_PROTOCOL_RESP:
                    (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message
                    (
                        &sap_msg,
                        data
                    );

                    LOG_DEBUG("MsgID: SET_TRANSPORT_PROTOCOL_RESP\n");
                    LOG_DEBUG("No_of_PARAMs: %02X\n",
                    sap_msg.no_of_parameters);

                    LOG_DEBUG("PARAM ID: ResultCode\n");

                    LOG_DEBUG("PARAM Len: %04X\n",
                    sap_msg.param[0U].param_length);

                    temp = &data[8U];

                    result_code = 0U;

                    for(i = sap_msg.param[0U].param_length - 1; i >= 0; i--)
                    {
                        result_code |= (*temp) << (8U * i);
                        temp++;
                    }

                    LOG_DEBUG("ResultCode: %02x\n", result_code);
                break;

            case SAP_MSG_DISCONNECT_IND:
                (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message
                (
                    &sap_msg,
                    data
                );

                LOG_DEBUG("MsgID: SAP_MSG_DISCONNECT_IND\n");
                LOG_DEBUG("No_of_PARAMs: %02X\n",
                sap_msg.no_of_parameters);

                if ((1U == sap_msg.no_of_parameters) &&
                    (SAP_PID_DISCONNECT_TYPE == sap_msg.param[0U].param_id))
                {
                    LOG_DEBUG("PARAM ID: SAP_PID_DISCONNECT_TYPE\n");

                    if (1U == sap_msg.param[0U].param_length)
                    {
                        UCHAR disc_type;

                        disc_type = *((UCHAR *)sap_msg.param[0U].parameter_value);

                        if (SAP_GRACEFUL_DISCONNECTION == disc_type)
                        {
                            (BT_IGNORE_RETURN_VALUE) BT_sap_client_disconnect();
                        }
                        else
                        {
                            /* Immediate Disconnect */
                        }
                    }
                }
                break;

            default:
                LOG_DEBUG("Unhandled Message ID: 0x%02X\n", msg_id);
                break;
            }

            break;

        default:
            LOG_DEBUG("Unexpected SAP Client Event Type: 0x%02X\n",
            event_type);
            break;
        }
    }

    return retval;
}


/*
 * Callback Function Registered with SDP,
 * Using SDP_SET_HANDLE() and BT_sdp_open ()
 */
void appl_sap_client_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     )
{
    S_UUID  uuid;
    UINT16 num_uuids;
    UINT16  attrib_id;
    UINT16 num_attribute_ids;
    API_RESULT retval = API_FAILURE;
    UINT16 sap_len_attrib_data;

    BT_IGNORE_UNUSED_PARAM(length);

    uuid.uuid_type = UUID_16;
    uuid.uuid_union.uuid_16 = SIM_ACCESS_UUID;
    num_uuids = 0x01U;

    attrib_id = PROTOCOL_DESC_LIST;
    num_attribute_ids = 1U ;

    LOG_DEBUG("SDP Callback Response = 0x%02X status= 0x%04X\n", command, status);

    switch(command)
    {
    case SDP_Open :
        sap_len_attrib_data = sizeof(sap_attrib_data);
        retval = BT_sdp_servicesearchattributerequest
                 (
                     &sap_client_sdp_handle,
                     &uuid,
                     num_uuids,
                     &attrib_id,
                     num_attribute_ids,
                     NULL,
                     0x00U,
                     sap_attrib_data,
                     &sap_len_attrib_data
                 );

        if(API_SUCCESS != retval)
        {
            LOG_DEBUG("BT_sdp_servicesearchattributerequest unsuccessful\n");
            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sap_client_sdp_handle);
            /* return; */
        }
        else
        {
            LOG_DEBUG("BT_sdp_servicesearchattributerequest successful\n");
        }
        break;

    case SDP_Close:
        LOG_DEBUG("SDP Close Return Value : %d\n", status);
        break;

    case SDP_ErrorResponse:
        /* LOG_DEBUG("Error Response Recieved\n"); */
        break;

    case SDP_ServiceSearchAttributeResponse:
        LOG_DEBUG("Service Search Attribute Request Successful.\n");

        /* Get the Channel Number */
        LOG_DEBUG("BT_sdp_get_channel_number\n");

        retval = BT_sdp_get_channel_number
                 (
                     data,
                     &sap_server_channel
                 );

       if(API_SUCCESS != retval)
       {
           LOG_DEBUG("[Appl-ERR] - BT_sdp_serviceattributerequest failed \n");
           /* return; */
       }
       else
       {
           LOG_DEBUG("server_channel: %d\n", sap_server_channel);

           (BT_IGNORE_RETURN_VALUE)BT_sdp_close(&sap_client_sdp_handle);
       }

       break;

    default:
        LOG_DEBUG("Unhandled Event: 0x%02X\n", command);
        break;

    }/* switch */

    return;
}


API_RESULT sap_client_appl_get_acl_connection (UCHAR * indx)
{
    int choice;
    API_RESULT retval;
    UCHAR i, num_list;

    /* Init */
    retval = API_SUCCESS;

    retval = BT_hci_get_connection_details
             (sap_client_appl_acl_list, BT_MAX_REMOTE_DEVICES, &num_list);

    if (API_SUCCESS != retval)
    {
         LOG_DEBUG("*** HCI Get Connection Details FAILED !! "\
         "Error Code = 0x%04X\n", retval);
         retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        if (0U == num_list)
        {
            LOG_DEBUG("*** ACL Connection to a Remote Device NOT Available\n");
            retval = API_FAILURE; /* return API_FAILURE; */
        }
        else
        {
            LOG_DEBUG("\n");
            LOG_DEBUG("==========================================================\n");
            LOG_DEBUG("Active ACL Connections\n");
            LOG_DEBUG("----------------------------------------------------------\n");
            for (i = 0U; i < num_list; i ++)
            {
                LOG_DEBUG("  [%d]  ", i);
                LOG_DEBUG("BD_ADDR %02X:%02X:%02X:%02X:%02X:%02X  ",
                sap_client_appl_acl_list[i].bd_addr[0U],
                sap_client_appl_acl_list[i].bd_addr[1U],
                sap_client_appl_acl_list[i].bd_addr[2U],
                sap_client_appl_acl_list[i].bd_addr[3U],
                sap_client_appl_acl_list[i].bd_addr[4U],
                sap_client_appl_acl_list[i].bd_addr[5U]);

                LOG_DEBUG("ACL Handle = 0x%04X\n",
                sap_client_appl_acl_list[i].acl_handle);
            }
            LOG_DEBUG("==========================================================\n");
            LOG_DEBUG("\n");
        }
    }

    if (API_SUCCESS == retval)
    {
        LOG_DEBUG("Choose ACL Connection Index = "); fflush(stdout);
        scanf("%d", &choice);
        if ((UINT16)choice >= BT_MAX_REMOTE_DEVICES)
        {
            retval = API_FAILURE; /* return API_FAILURE; */
        }
        else
        {
            *indx = (UCHAR)choice;
        }
    }

    return retval;
}
#endif /* SAP_CLIENT */
