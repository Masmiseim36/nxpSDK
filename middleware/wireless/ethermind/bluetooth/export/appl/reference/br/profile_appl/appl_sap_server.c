
/**
 *  \file appl_sap_server.c
 *
 *  Source file for SAP Server Command Line Test Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_sap_server.h"
#include "appl_utils.h"

#ifdef SAP_SERVER
/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static const char sap_server_menu[] = "\n\
-------- SIM Access Server Menu ------- \n\
    0.  Exit \n\
    1.  Refresh menu \n\n\
    2.  Start SIM Access Server \n\
    3.  Set SAP Server Class of Device \n\
 \n\
   10.  Report Status \n\
 \n\
   15.  Send APDU Response \n\
 \n\
   20.  Disconnect SAP Client \n\
Your Option -> \0";

/* ----------------------------------------- Functions */
void main_sap_server_operations(void)
{
    int choice, menu_choice;
    API_RESULT retval = 0U;
    UCHAR * response;
    UCHAR i;

    BT_LOOP_FOREVER()
    {
        printf("%s",sap_server_menu);

        scanf("%d",&choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0: /* Exit */
            break; /* return; */

        case 1: /* Refresh */
            break;

        case 2: /* Start SIM Access Server */
            retval = BT_sap_server_start(appl_sap_server_appl_cb);
            if(API_SUCCESS == retval)
            {
                LOG_DEBUG("SIM Access Server Started Successfully\n");
            }
            else
            {
                LOG_DEBUG("Error starting SIM Access Server\n");
            }
            break;

        case 3:
            /* Set the Class of Device */
            /*
             * Set Class of Device:
             *    Major Service Class: "Telephony"
             *    Major Device Class: "Phone"
             *    Minor Device Class: "SIM Card Reader"
             */
            retval = BT_hci_write_class_of_device
                     (
                         BT_MSC_TELEPHONY|BT_MDC_PERIPHERAL|
                         BT_PPMC_CARD_READER
                     );
            if(API_SUCCESS == retval)
            {
                LOG_DEBUG("SIM Access Server CoD Set Successfully\n");
            }
            else
            {
                LOG_DEBUG("Error setting SIM Access Server CoD\n");
            }
            break;

        case 10:
            {
                UCHAR status;
                int read_val;

                LOG_DEBUG("Enter Card Startus\n");

                LOG_DEBUG("Unknown Error: %d\n", SAP_STATUS_CHANGE_UNKNOWN_ERROR);
                LOG_DEBUG("Reset: %d\n", SAP_STATUS_CHANGE_CARD_RESET);
                LOG_DEBUG("Card Not Accessible: %d\n", SAP_STATUS_CHANGE_CARD_NOT_ACCESSIBLE);
                LOG_DEBUG("Card Removed: %d\n", SAP_STATUS_CHANGE_CARD_REMOVED);
                LOG_DEBUG("Card Inserted: %d\n", SAP_STATUS_CHANGE_CARD_INSERTED);
                LOG_DEBUG("Card Recovered: %d\n", SAP_STATUS_CHANGE_CARD_RECOVERED);

                scanf("%d", &read_val);

                status = (UCHAR)read_val;

                retval = BT_sap_server_report_status(status);
                if (API_SUCCESS == retval)
                {
                    LOG_DEBUG("Report Status Operation started successfully\n");
                }
                else
                {
                    LOG_DEBUG("Error in report status. Result 0x%04X\n", retval);
                }
            }
            break;

        case 15:
            LOG_DEBUG("Enter APDU Response length: \n");
            scanf("%d", &choice);

            /* Allocate memory for APDU Response */
            response = BT_alloc_mem(choice);

            if (NULL == response)
            {
                LOG_DEBUG("Failed to allocate memory for APDU response\n");
                break;
            }

            LOG_DEBUG("Enter APDU Response bytes (Hex: 43 E3 ...): \n");
            for (i = 0; i < choice; i++)
            {
                scanf("%x", &response[i]);
            }

            LOG_DEBUG("Sending APDU Response...\n");
            retval = BT_sap_server_transfer_apdu_response(response, (UINT16)choice);
            LOG_DEBUG("Retval - 0x%04X\n", retval);

            /* Free allocated memory */
            BT_free_mem(response);
            response = NULL;
            break;

        case 20:
            /* Disconnect SAP Client */
            {
                int read_val;
                UCHAR disc_type;

                LOG_DEBUG("Enter disconnection type. Graceful (0), Immediate (1)\n");
                scanf("%d", &read_val);

                if (0U == read_val)
                {
                    disc_type = SAP_GRACEFUL_DISCONNECTION;
                }
                else
                {
                    disc_type = SAP_IMMEDIATE_DISCONNECTION;
                }

                retval = BT_sap_server_disconnect(disc_type);

                if (API_SUCCESS == retval)
                {
                    LOG_DEBUG("Disconnection Operation started successfully\n");
                }
                else
                {
                    LOG_DEBUG("Error in disconnection. Result 0x%04X\n", retval);
                }
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
}


API_RESULT appl_sap_server_appl_cb
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
    UINT32 max_msg_size;

    if (API_SUCCESS != event_result)
    {
        LOG_DEBUG("Received SAP Server Callback\n");
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
        case SAP_RFCOMM_ACCEPT:
            LOG_DEBUG("SAP_RFCOMM_ACCEPT->\n");
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
            case SAP_MSG_CONNECT_REQ:
                (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message
                (
                    &sap_msg,
                    event_data
                );

                LOG_DEBUG("MsgID: CONNECT_REQ\n");
                LOG_DEBUG("No_of_PARAMs: %02X\n",
                     sap_msg.no_of_parameters);
                LOG_DEBUG("PARAM ID: MaxMsgSize\n");

                LOG_DEBUG("PARAM Len: %04X\n",
                      sap_msg.param[0U].param_length);

                max_msg_size = 0U;
                temp = &((UCHAR *)event_data)[8U];

                /* It is just two byte value */
                for(i = sap_msg.param[0U].param_length - 1; i >= 0; i--)
                {
                    max_msg_size |= (*temp) << (8U * i);
                    temp++;
                }

                LOG_DEBUG("MaxMsgSize: %08x\n", max_msg_size);
                break;

            case SAP_MSG_DISCONNECT_REQ:
                (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message(&sap_msg, event_data);
                LOG_DEBUG("MsgID: DISCONNECT_REQ\n");
                LOG_DEBUG("No_of_PARAMs: %02X\n",
                sap_msg.no_of_parameters);

                break;

            case SAP_MSG_TRANSFER_ATR_REQ:
                (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message(&sap_msg, event_data);
                LOG_DEBUG("MsgID: TRANSFER_ATR_REQ\n");
                LOG_DEBUG("No_of_PARAMs: %02X\n",
                sap_msg.no_of_parameters);

                break;

           case SAP_MSG_TRANSFER_APDU_REQ:
               (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message(&sap_msg, event_data);
               LOG_DEBUG("MsgID: TRANSFER_APDU_REQ\n");
               LOG_DEBUG("No_of_PARAMs: %02X\n",
               sap_msg.no_of_parameters);

               break;

            case SAP_MSG_POWER_SIM_OFF_REQ:
                (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message(&sap_msg,
                event_data);
                LOG_DEBUG("MsgID: POWER_SIM_OFF_REQ\n");
                LOG_DEBUG("No_of_PARAMs: %02X\n",
                sap_msg.no_of_parameters);

                break;

            case SAP_MSG_POWER_SIM_ON_REQ:
                (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message(&sap_msg, event_data);
                LOG_DEBUG("MsgID: POWER_SIM_ON_REQ\n");
                LOG_DEBUG("No_of_PARAMs: %02X\n",
                sap_msg.no_of_parameters);

                break;

            case SAP_MSG_RESET_SIM_REQ:
                (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message(&sap_msg, event_data);
                LOG_DEBUG("MsgID: RESET_SIM_REQ\n");
                LOG_DEBUG("No_of_PARAMs: %02X\n",
                sap_msg.no_of_parameters);

                break;

           case SAP_MSG_TRANSFER_CARD_READER_STATUS_REQ:
               (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message(&sap_msg, event_data);
               LOG_DEBUG("MsgID: CARD_READER_REQ\n");
               LOG_DEBUG("No_of_PARAMs: %02X\n",
               sap_msg.no_of_parameters);
               break;

           case SAP_MSG_SET_TRANSPORT_PROTOCOL_REQ:
               (BT_IGNORE_RETURN_VALUE) BT_sap_parse_message(&sap_msg, event_data);
               LOG_DEBUG("MsgID: TRANSPORT_PROTOCOL_REQ\n");
               LOG_DEBUG("No_of_PARAMs: %02X\n",
               sap_msg.no_of_parameters);
               break;

            default:
                LOG_DEBUG("Client is misbehaving\n");
                break;
            }

            break;

        default:
            LOG_DEBUG("Unexpected SAP Client Event Type: 0x%02X\n",
            event_type);
            break;
        }
    }

    return API_SUCCESS;
}


/**
 *  Generates passkey of requested length.
 *
 *  @param
 *      OUT UCHAR *passwd: Generated Passkey.
 *
 *  @param
 *      IN UCHAR len: Length of the passkey to be generated.
 *
 *  @return  void
 */
void sap_server_appl_passwd_generator( /* OUT */ UCHAR *passwd, /* IN */ UCHAR len)
{
    /*
     * Our Character set [a-z, A-Z, 0-9],
     * but currently we are using only 0-9
     */
    int m = 0U;
    UCHAR i;

    for(i=0U; i < len; i++)
    {
        m++;
        srand((unsigned int)(BT_get_time_ms() / m));

        /* Genrating alphanumric password */
        if(0 != (rand() % 2U)) /* odd */
        {
            if(0 != (rand() % 2U)) /* odd */
            {
                m = rand();
                passwd[i] = (m % 26U) + 'a';
            }
            else
            {
                m = rand();
                passwd[i] = (m % 26U) + 'A';
            }
        }
        else
        {
            m = rand();
            passwd[i] = (m % 10U) + '0';
        }
    }
}
#endif /* SAP_SERVER */

