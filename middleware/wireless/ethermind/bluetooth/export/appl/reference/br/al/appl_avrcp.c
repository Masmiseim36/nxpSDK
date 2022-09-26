
/**
 *  \file appl_avrcp.c
 *
 *  Source file for AVRCP command line applicaiton for both CT & TG roles.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_avrcp.h"
#include "appl_avrcp_ct.h"
#include "appl_avrcp_tg.h"
#include "appl_utils.h"

#if (defined AVRCP_CT || defined AVRCP_TG)
/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */

/* Main AVRCP Application Menu */
static const char avrcp_main_menu[] = " \n\
--------------- AVRCP Menu ---------------------- \n\n\
  0.  Exit. \n\
  1.  Refresh. \n\
 \n\
  2.  AVRCP Initialize. \n\
  3.  AVRCP Shutdown. \n\
 \n\
  4.  AVRCP Start Instance. \n\
  5.  AVRCP Stop Instance. \n\
 \n\
  6.  AVRCP Show Handle. \n\
 \n\
  7.  Register Peer BD Addrs. \n\
  8.  Get Registered Peer BD Addrs. \n\
  \n\
 10.  Create ACL Connection. \n\
 11.  Disconnect ACL connection.\n\
 \n\
 12.  AVRCP Control Channel Connect. \n\
 13.  AVRCP Control Channel Disconnect. \n\
 \n\
 14.  AVRCP Browsing Channel Connect.\n\
 15.  AVRCP Browsing Channel Disconnect.\n\
 \n\
 16.  AVRCP Controller Operations.\n\
 17.  AVRCP Target Operations.\n\
 \n\
Your Option -> ";

/* AVRCP Handles */
AVRCP_HANDLE appl_avrcp_handle [AVRCP_MAX_PROFILES];
static UCHAR first_time = 0x0U;

/* HCI ACL Connection List */
static HCI_CONNECTION_LIST appl_avrcp_acl_list [BT_MAX_REMOTE_DEVICES];

/* AVRCP Peer BD Address */
UCHAR appl_avrcp_bd_addr[BT_BD_ADDR_SIZE];

/* ----------------------------------------- Functions */

void main_avrcp_operations (void)
{
    int choice, menu_choice, read_val;
    UINT16 connection_handle;
    API_RESULT retval;

    if (first_time == 0x0U)
    {
        first_time = 1U;
        appl_avrcp_init_handles();
    }

    BT_LOOP_FOREVER()
    {
        printf ("%s", avrcp_main_menu);
        scanf("%d", &choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            /* Init */
            appl_avrcp_init ();
            break;

        case 3:
            /* Shutdown */
            appl_avrcp_shutdown ();
            break;

        case 4:
            /* Start */
            appl_avrcp_start ();
            break;

        case 5:
            /* Stop */
            appl_avrcp_stop ();
            break;

        case 6:
            appl_avrcp_show_handles();
            break;

        case 7:

            /* Register Peer BD address */
             LOG_DEBUG("Enter Peer BD Address:");
             appl_get_bd_addr(appl_avrcp_bd_addr);

            /* BT_mem_copy(appl_avrcp_bd_addr, bd_addr, BT_BD_ADDR_SIZE);  */
            break;

        case 8:

            /* Get Registered BD_ADDR of Peer Device */
            LOG_DEBUG("\nRegistered Peer BD "\
            BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR(appl_avrcp_bd_addr));
            break;

        case 10:
            /**
             * First Application should establish ACL Connection with
             * the peer Device
             */
            retval = BT_hci_create_connection
                     (
                         appl_avrcp_bd_addr,
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
            scanf("%x", &read_val);
            connection_handle = (UINT16)read_val;

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

        case 12:
            /* Control Channel Connect */
            appl_avrcp_ctl_ch_connect_req ();
            break;

        case 13:
            /* Control Channel Disconnect */
            appl_avrcp_ctl_ch_disconnect_req ();
            break;

        case 14:
            /* Browsing Channel Connect */
            appl_avrcp_brow_ch_connect_req ();
            break;

        case 15:
            /* Browsing Channel Disconnect */
            appl_avrcp_brow_ch_disconnect_req ();
            break;

        case 16:
#ifdef AVRCP_CT
            /* AVRCP Controller operations */
            main_avrcp_ct_operations ();
#else
            LOG_DEBUG ("AVRCP_CT flag is not defined\n");
#endif /* AVRCP_CT */
            break;

        case 17:
#ifdef AVRCP_TG
            /* AVRCP Target operations */
            main_avrcp_tg_operations ();
#else
            LOG_DEBUG ("AVRCP_TG flag is not defined\n");
#endif /* AVRCP_TG */
            break;

        default:
            LOG_DEBUG ("Invalid Choice\n");
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


/* Init AVRCP application handles */
void appl_avrcp_init_handles (void)
{
    UCHAR index;

    for (index = 0U; index < AVRCP_MAX_PROFILES; index ++)
    {
        AVRCP_INIT_HANDLE (appl_avrcp_handle[index]);
    }

    return;
}

void appl_avrcp_init (void)
{
    API_RESULT retval;

    LOG_DEBUG("\n");
    LOG_DEBUG("Initializing AVRCP ... "); fflush(stdout);

    retval = BT_avrcp_al_init();
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully Done.\n");
    }

    /* CT Role init */
#ifdef AVRCP_CT
    appl_avrcp_ct_init();
#endif /* AVRCP_CT */

    /* TG Role init */
#ifdef AVRCP_TG
    appl_avrcp_tg_init ();
#endif /* AVRCP_TG */

    return;
}

void appl_avrcp_shutdown (void)
{
    API_RESULT retval;

    LOG_DEBUG("\n");
    LOG_DEBUG("Shutting down AVRCP ... "); fflush(stdout);

    retval = BT_avrcp_al_shutdown();
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully Done.\n");
    }

#ifdef AVRCP_CT
    /* CT Role Shutdown */
    appl_avrcp_ct_shutdown ();
#endif /* AVRCP_CT */

#ifdef AVRCP_TG
    /* TG Role Shutdown */
    appl_avrcp_tg_shutdown ();
#endif  /* AVRCP_TG */

    return;
}

void appl_avrcp_start (void)
{
    UCHAR index;

    API_RESULT retval;

    /* Choose Handle */
    appl_avrcp_choose_handle(&index);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVRCP Instance ... "); fflush(stdout);

    retval = BT_avrcp_al_start
             (
                 &appl_avrcp_handle[index],
                 appl_avrcp_ntf_cb
             );
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully Done.\n");
    }

#ifdef AVRCP_CT
    /* CT Role Start */
    appl_avrcp_ct_start (index);
#endif /* AVRCP_CT */

#ifdef AVRCP_TG
    /* TG Role Start */
     appl_avrcp_tg_start (index);
#endif /* AVRCP_TG */

    return;
}

void appl_avrcp_stop (void)
{
    UCHAR index;
    API_RESULT retval;

    /* Choose Handle */
    appl_avrcp_choose_handle(&index);

    LOG_DEBUG("\n");
    LOG_DEBUG("Stopping AVRCP Instance ... "); fflush(stdout);

    retval = BT_avrcp_al_stop (&appl_avrcp_handle[index]);
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully Done.\n");
    }

 #ifdef AVRCP_CT
    /* CT Role Stop */
    appl_avrcp_ct_stop (index);
#endif /* AVRCP_CT */

#ifdef AVRCP_TG
    /* TG Role Stop */
   appl_avrcp_tg_stop (index);
#endif /* AVRCP_TG */

     return;
}

void appl_avrcp_ctl_ch_connect_req (void)
{
    UCHAR index;
    UINT16 acl_indx;
    API_RESULT retval;

    /* Choose Handle */
    appl_avrcp_choose_handle(&index);

    /* Get Remote BD_ADDR */
    retval = appl_avrcp_choose_acl_connection (&acl_indx);
    if (API_SUCCESS == retval)
    {
        LOG_DEBUG("\n");
        LOG_DEBUG("Starting AVRCP Control Channel Connect ... "); fflush(stdout);

        retval = BT_avrcp_al_connect_req
                 (
                     &appl_avrcp_handle[index],
                     appl_avrcp_acl_list[acl_indx].bd_addr
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("Successfully Started.\n");
        }
    }

    return;
}

void appl_avrcp_ctl_ch_disconnect_req (void)
{
    UCHAR index;
    API_RESULT retval;

    /* Choose Handle */
    appl_avrcp_choose_handle(&index);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVRCP Control Channel Disconnect ... "); fflush(stdout);

    retval = BT_avrcp_al_disconnect_req (&appl_avrcp_handle[index]);
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully Started.\n");
    }

    return;
}

#ifdef AVRCP_1_4
void appl_avrcp_brow_ch_connect_req (void)
{
    UCHAR index;
    UINT16 acl_indx;
    API_RESULT retval;

    /* Choose Handle */
    appl_avrcp_choose_handle(&index);

    /* Get Remote BD_ADDR */
    retval = appl_avrcp_choose_acl_connection (&acl_indx);
    if (API_SUCCESS == retval)
    {
        LOG_DEBUG("\n");
        LOG_DEBUG("Starting AVRCP Browsing Channel Connect ... "); fflush(stdout);

        retval = BT_avrcp_al_channel_connect_req
                 (
                     &appl_avrcp_handle[index],
                     appl_avrcp_acl_list[acl_indx].bd_addr,
                     AVCTP_CHANNEL_TYPE_BROWSING
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("Successfully Started.\n");
        }
    }

    return;
}

void appl_avrcp_brow_ch_disconnect_req (void)
{
    UCHAR index;
    API_RESULT retval;

    /* Choose Handle */
    appl_avrcp_choose_handle(&index);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVRCP Browsing Channel Disconnect ... "); fflush(stdout);

    retval = BT_avrcp_al_channel_disconnect_req
             (
                 &appl_avrcp_handle[index],
                 AVCTP_CHANNEL_TYPE_BROWSING
             );
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully Started.\n");
    }

    return;
}

#endif /* AVRCP_1_4 */


/* Application callback registered with AVRCP Abstraction Layer */
API_RESULT appl_avrcp_ntf_cb
           (
                AVRCP_HANDLE * avrcp_handle,
                UCHAR          event_type,
                UINT16         event_result,
                void  *        event_data,
                UINT16         event_datalen
           )
{
    UCHAR index;
#ifdef AVRCP_CT
    AVRCP_AL_GET_FOLDER_ITEMS_PARAM get_folder_items_param;
#endif /* AVRCP_CT */

      LOG_DEBUG("\n");

    switch (event_type)
    {
    case AVRCP_CONNECT_IND:
        LOG_DEBUG("Received AVRCP_CONNECT_IND.\n");
        LOG_DEBUG("    Event Result   = 0x%04X\n", event_result);
        LOG_DEBUG("    AVRCP Instance = %d (0x%02X)\n",
        avrcp_handle->profile_inst, avrcp_handle->profile_inst);
        LOG_DEBUG("    Remote BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avrcp_handle->bd_addr[0U], avrcp_handle->bd_addr[1U],
        avrcp_handle->bd_addr[2U], avrcp_handle->bd_addr[3U],
        avrcp_handle->bd_addr[4U], avrcp_handle->bd_addr[5U]);

        /* Update AVRCP Handle */
        for (index = 0U; index < AVRCP_MAX_PROFILES; index ++)
        {
            if (avrcp_handle->profile_inst ==
                              appl_avrcp_handle[index].profile_inst)
            {
                AVRCP_SET_HANDLE_BD_ADDR
                (
                    appl_avrcp_handle[index],
                    avrcp_handle->bd_addr
                );
            }
        }

        break;

    case AVRCP_CONNECT_CNF:
        LOG_DEBUG("Received AVRCP_CONNECT_CNF.\n");
        LOG_DEBUG("    Event Result   = 0x%04X\n", event_result);
        LOG_DEBUG("    AVRCP Instance = %d (0x%02X)\n",
        avrcp_handle->profile_inst, avrcp_handle->profile_inst);
        LOG_DEBUG("    Remote BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avrcp_handle->bd_addr[0U], avrcp_handle->bd_addr[1U],
        avrcp_handle->bd_addr[2U], avrcp_handle->bd_addr[3U],
        avrcp_handle->bd_addr[4U], avrcp_handle->bd_addr[5U]);

        if (API_SUCCESS != event_result)
        {
            /* Update AVRCP Handle */
            for (index = 0U; index < AVRCP_MAX_PROFILES; index++)
            {
                if (avrcp_handle->profile_inst ==
                    appl_avrcp_handle[index].profile_inst)
                {
                    AVRCP_RESET_HANDLE_BD_ADDR
                    (
                        appl_avrcp_handle[index]
                    );
                }
            }
        }

        break;

    case AVRCP_DISCONNECT_IND:
        LOG_DEBUG("Received AVRCP_DISCONNECT_IND.\n");
        LOG_DEBUG("    Event Result   = 0x%04X\n", event_result);
        LOG_DEBUG("    AVRCP Instance = %d (0x%02X)\n",
        avrcp_handle->profile_inst, avrcp_handle->profile_inst);
        LOG_DEBUG("    Remote BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avrcp_handle->bd_addr[0U], avrcp_handle->bd_addr[1U],
        avrcp_handle->bd_addr[2U], avrcp_handle->bd_addr[3U],
        avrcp_handle->bd_addr[4U], avrcp_handle->bd_addr[5U]);

        /* Update AVRCP Handle */
        for (index = 0U; index < AVRCP_MAX_PROFILES; index ++)
        {
            if (avrcp_handle->profile_inst ==
                              appl_avrcp_handle[index].profile_inst)
            {
                AVRCP_RESET_HANDLE_BD_ADDR
                (
                    appl_avrcp_handle[index]
                );
            }
        }

        break;

    case AVRCP_DISCONNECT_CNF:
        LOG_DEBUG("Received AVRCP_DISCONNECT_CNF.\n");
        LOG_DEBUG("    Event Result   = 0x%04X\n", event_result);
        LOG_DEBUG("    AVRCP Instance = %d (0x%02X)\n",
        avrcp_handle->profile_inst, avrcp_handle->profile_inst);
        LOG_DEBUG("    Remote BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avrcp_handle->bd_addr[0U], avrcp_handle->bd_addr[1U],
        avrcp_handle->bd_addr[2U], avrcp_handle->bd_addr[3U],
        avrcp_handle->bd_addr[4U], avrcp_handle->bd_addr[5U]);

        /* Update AVRCP Handle */
        for (index = 0U; index < AVRCP_MAX_PROFILES; index ++)
        {
            if (avrcp_handle->profile_inst ==
                              appl_avrcp_handle[index].profile_inst)
            {
                AVRCP_RESET_HANDLE_BD_ADDR
                (
                    appl_avrcp_handle[index]
                );
            }
        }

        break;

#ifdef AVRCP_1_4

    case AVRCP_BOW_CONNECT_IND:
        LOG_DEBUG("Received AVRCP_BOW_CONNECT_IND.\n");
        LOG_DEBUG("    Event Result   = 0x%04X\n", event_result);
        LOG_DEBUG("    AVRCP Instance = %d (0x%02X)\n",
        avrcp_handle->profile_inst, avrcp_handle->profile_inst);

        break;

    case AVRCP_BOW_CONNECT_CNF:
        LOG_DEBUG("Received AVRCP_BOW_CONNECT_CNF.\n");
        LOG_DEBUG("    Event Result   = 0x%04X\n", event_result);
        LOG_DEBUG("    AVRCP Instance = %d (0x%02X)\n",
        avrcp_handle->profile_inst, avrcp_handle->profile_inst);

#ifdef AVRCP_CT
        /**
         * Get Media Player list
         */
        get_folder_items_param.scope = AVRCP_SCOPE_MEDIA_PLAYER_LIST;
        get_folder_items_param.start_item = 0U;
        get_folder_items_param.end_item = 10U;
        get_folder_items_param.attr_count = 0xFFU;

        (BT_IGNORE_RETURN_VALUE) BT_avrcp_al_get_folder_items
        (
            avrcp_handle,
            &get_folder_items_param
        );
#endif /* AVRCP_CT */

        break;

    case AVRCP_BOW_DISCONNECT_IND:
        LOG_DEBUG("Received AVRCP_BOW_DISCONNECT_IND.\n");
        LOG_DEBUG("    Event Result   = 0x%04X\n", event_result);
        LOG_DEBUG("    AVRCP Instance = %d (0x%02X)\n",
        avrcp_handle->profile_inst, avrcp_handle->profile_inst);

        break;

    case AVRCP_BOW_DISCONNECT_CNF:
        LOG_DEBUG("Received AVRCP_BOW_DISCONNECT_CNF.\n");
        LOG_DEBUG("    Event Result   = 0x%04X\n", event_result);
        LOG_DEBUG("    AVRCP Instance = %d (0x%02X)\n",
        avrcp_handle->profile_inst, avrcp_handle->profile_inst);

        break;

#endif /* AVRCP_1_4 */

    case AVRCP_MESSAGE_SEND_CNF:
        LOG_DEBUG("Received AVRCP_MESSAGE_SEND_CNF.\n");
        LOG_DEBUG("    Event Result   = 0x%04X\n", event_result);
        LOG_DEBUG("    AVRCP Instance = %d (0x%02X)\n",
        avrcp_handle->profile_inst, avrcp_handle->profile_inst);
        LOG_DEBUG("    Remote BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avrcp_handle->bd_addr[0U], avrcp_handle->bd_addr[1U],
        avrcp_handle->bd_addr[2U], avrcp_handle->bd_addr[3U],
        avrcp_handle->bd_addr[4U], avrcp_handle->bd_addr[5U]);

        break;

        /* TG related events */
#ifdef AVRCP_TG
    case AVRCP_MESSAGE_IND:
        LOG_DEBUG("Received AVRCP_MESSAGE_IND.\n");
        LOG_DEBUG("    Event Result   = 0x%04X\n", event_result);
        LOG_DEBUG("    AVRCP Instance = %d (0x%02X)\n",
        avrcp_handle->profile_inst, avrcp_handle->profile_inst);
        LOG_DEBUG("    Remote BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avrcp_handle->bd_addr[0U], avrcp_handle->bd_addr[1U],
        avrcp_handle->bd_addr[2U], avrcp_handle->bd_addr[3U],
        avrcp_handle->bd_addr[4U], avrcp_handle->bd_addr[5U]);

        if ((event_data != NULL) && (event_datalen > 0U))
        {
            appl_avrcp_tg_handle_message_cmd
            (
                    avrcp_handle->profile_inst,
                    event_data,
                    event_datalen
            );
        }

        break;

#ifdef AVRCP_1_3

    case AVRCP_METADATA_CMD:
        LOG_DEBUG("Received AVRCP_METADATA_CMD.\n");
        LOG_DEBUG("    Event Result   = 0x%04X\n", event_result);
        LOG_DEBUG("    AVRCP Instance = %d (0x%02X)\n",
        avrcp_handle->profile_inst, avrcp_handle->profile_inst);
        LOG_DEBUG("    Remote BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avrcp_handle->bd_addr[0U], avrcp_handle->bd_addr[1U],
        avrcp_handle->bd_addr[2U], avrcp_handle->bd_addr[3U],
        avrcp_handle->bd_addr[4U], avrcp_handle->bd_addr[5U]);

        if ((event_data != NULL) && (event_datalen > 0U))
        {
            appl_avrcp_tg_handle_vendor_dependent_cmd
                (
                    avrcp_handle->profile_inst,
                    event_data,
                    event_datalen
                );
        }

        break;

#endif /* AVRCP_1_3 */

#ifdef AVRCP_1_4
    case AVRCP_BOW_MESSAGE_IND:

        LOG_DEBUG("Received AVRCP Browsing PDU IND.\n");
        LOG_DEBUG("    Event Result   = 0x%04X\n", event_result);
        LOG_DEBUG("    AVRCP Instance = %d (0x%02X)\n",
        avrcp_handle->profile_inst, avrcp_handle->profile_inst);
        LOG_DEBUG("    Remote BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avrcp_handle->bd_addr[0U], avrcp_handle->bd_addr[1U],
        avrcp_handle->bd_addr[2U], avrcp_handle->bd_addr[3U],
        avrcp_handle->bd_addr[4U], avrcp_handle->bd_addr[5U]);

        if ((event_data != NULL) && (event_datalen > 0U))
        {
            appl_avrcp_tg_handle_browsing_cmd (event_data, event_datalen);
        }

        break;

#endif /* AVRCP_1_4 */

#endif /* AVRCP_TG */

#ifdef AVRCP_CT
    case AVRCP_MESSAGE_CNF:
        LOG_DEBUG("Received AVRCP_MESSAGE_CNF.\n");
        LOG_DEBUG("    Event Result   = 0x%04X\n", event_result);
        LOG_DEBUG("    AVRCP Instance = %d (0x%02X)\n",
        avrcp_handle->profile_inst, avrcp_handle->profile_inst);
        LOG_DEBUG("    Remote BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avrcp_handle->bd_addr[0U], avrcp_handle->bd_addr[1U],
        avrcp_handle->bd_addr[2U], avrcp_handle->bd_addr[3U],
        avrcp_handle->bd_addr[4U], avrcp_handle->bd_addr[5U]);

        if ((event_result == API_SUCCESS) &&
            (event_data != NULL) && (event_datalen > 0U))
        {
            appl_avrcp_ct_handle_message_cnf (event_data, event_datalen);
        }

        break;

#ifdef AVRCP_1_3
    case AVRCP_METADATA_INTERIM_RSP:
    case AVRCP_METADATA_RSP:

        LOG_DEBUG("Received %s.\n", ((AVRCP_METADATA_RSP == event_type)?
                "AVRCP_METADATA_RSP" : "AVRCP_METADATA_INTERIM_RSP"));
        LOG_DEBUG("    Event Result   = 0x%04X\n", event_result);
        LOG_DEBUG("    AVRCP Instance = %d (0x%02X)\n",
        avrcp_handle->profile_inst, avrcp_handle->profile_inst);
        LOG_DEBUG("    Remote BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avrcp_handle->bd_addr[0U], avrcp_handle->bd_addr[1U],
        avrcp_handle->bd_addr[2U], avrcp_handle->bd_addr[3U],
        avrcp_handle->bd_addr[4U], avrcp_handle->bd_addr[5U]);

        if ((event_data != NULL) && (event_datalen > 0U))
        {
            appl_avrcp_ct_handle_vendor_dependent_cmd_rsp
                (
                    avrcp_handle->profile_inst,
                    event_data,
                    event_datalen
                );
        }

        break;

#endif /* AVRCP_1_3 */

#ifdef AVRCP_1_4
    case AVRCP_BOW_MESSAGE_RSP:
        LOG_DEBUG("Received AVRCP_BOW_MESSAGE_RSP.\n");
        LOG_DEBUG("    Event Result   = 0x%04X\n", event_result);
        LOG_DEBUG("    AVRCP Instance = %d (0x%02X)\n",
        avrcp_handle->profile_inst, avrcp_handle->profile_inst);
        LOG_DEBUG("    Remote BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avrcp_handle->bd_addr[0U], avrcp_handle->bd_addr[1U],
        avrcp_handle->bd_addr[2U], avrcp_handle->bd_addr[3U],
        avrcp_handle->bd_addr[4U], avrcp_handle->bd_addr[5U]);

        if ((event_data != NULL) && (event_datalen > 0U))
        {
            appl_avrcp_ct_handle_brow_cmd_rsp
                (
                    avrcp_handle->profile_inst,
                    event_data,
                    event_datalen
                );
        }

        break;

#endif /* AVRCP_1_4 */

#endif /* AVRCP_CT */

    default:
        LOG_DEBUG("Received Unknown AVRCP Event = 0x%02X. Result = 0x%04X\n",
        event_type, event_result);
        break;
    }

    return API_SUCCESS;
}


void appl_avrcp_choose_handle (UCHAR * retval)
{
    int read_val;

    BT_LOOP_FOREVER()
    {
        appl_avrcp_show_handles();

        LOG_DEBUG("Choose Index = "); fflush(stdout);
        scanf("%d", &read_val);

        if ((read_val >= 0) && (read_val < AVRCP_MAX_PROFILES))
        {
            break;
        }
    }
    *retval = (UCHAR)read_val;

    return;
}

void appl_avrcp_show_handles (void)
{
    UCHAR index;

    LOG_DEBUG("\n");
    LOG_DEBUG("Index  Remote BD_ADDR     Instance\n");
    LOG_DEBUG("-----  -----------------  --------\n");
    for (index = 0U; index < AVRCP_MAX_PROFILES; index ++)
    {
        if (appl_avrcp_handle[index].profile_inst == 0xFFU)
        {
            LOG_DEBUG("%3d    NOT ALLOCATED\n", index);
            continue;
        }

        LOG_DEBUG("%3d    ", index);
        LOG_DEBUG("%02X:%02X:%02X:%02X:%02X:%02X  ",
        appl_avrcp_handle[index].bd_addr[0U],
        appl_avrcp_handle[index].bd_addr[1U],
        appl_avrcp_handle[index].bd_addr[2U],
        appl_avrcp_handle[index].bd_addr[3U],
        appl_avrcp_handle[index].bd_addr[4U],
        appl_avrcp_handle[index].bd_addr[5U]);

        LOG_DEBUG("  0x%02X\n", appl_avrcp_handle[index].profile_inst);
    }
    LOG_DEBUG("\n");

    return;
}

API_RESULT appl_avrcp_choose_acl_connection (UINT16 * indx)
{
    int choice;
    API_RESULT retval;
    UCHAR i, num_list;

    LOG_DEBUG("Getting Connection Details from HCI ... "); fflush(stdout);
    retval = BT_hci_get_connection_details
             (
                 appl_avrcp_acl_list,
                 BT_MAX_REMOTE_DEVICES,
                 &num_list
             );
    if (API_SUCCESS != retval)
    {
         LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
    }
    else
    {
        LOG_DEBUG("Succeeded.\n");

        if (0U == num_list)
        {
            retval = API_FAILURE; /* return API_FAILURE; */
        }

        if (API_FAILURE != retval)
        {
            LOG_DEBUG("\n");
            LOG_DEBUG("==========================================================\n");
            LOG_DEBUG("Active ACL Connections\n");
            LOG_DEBUG("----------------------------------------------------------\n");
            for (i = 0U; i < num_list; i++)
            {
                LOG_DEBUG("  [%2d]  ", i);
                LOG_DEBUG("BD_ADDR %02X:%02X:%02X:%02X:%02X:%02X  ",
                    appl_avrcp_acl_list[i].bd_addr[0U],
                    appl_avrcp_acl_list[i].bd_addr[1U],
                    appl_avrcp_acl_list[i].bd_addr[2U],
                    appl_avrcp_acl_list[i].bd_addr[3U],
                    appl_avrcp_acl_list[i].bd_addr[4U],
                    appl_avrcp_acl_list[i].bd_addr[5U]);
                LOG_DEBUG("ACL Handle = 0x%04X\n",
                    appl_avrcp_acl_list[i].acl_handle);
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
            *indx = (UINT16)choice;
        }
    }

    return retval;
}


void appl_avrcp_print_opcode (UCHAR opcode)
{
    switch (opcode)
    {
    case AVRCP_OPCODE_UNIT_INFO:
        LOG_DEBUG("    Opcode = UNIT INFO\n");
        break;

    case AVRCP_OPCODE_SUBUNIT_INFO:
        LOG_DEBUG("    Opcode = SUBUNIT INFO\n");
        break;

    case AVRCP_OPCODE_PASS_THROUGH:
        LOG_DEBUG("    Opcode = PASS THROUGH\n");
        break;

    default:
        LOG_DEBUG("    Opcode = ???? (0x%02X)\n", opcode);
        break;
    }

    return;
}

void appl_avrcp_print_response_type (UCHAR rsp_type)
{
    switch (rsp_type)
    {
    case AVRCP_RESPONSE_TYPE_NOT_IMPLEMENTED:
        LOG_DEBUG("    Response Type = NOT IMPLEMENTED\n");
        break;

    case AVRCP_RESPONSE_TYPE_ACCEPTED:
        LOG_DEBUG("    Response Type = ACCEPTED\n");
        break;

    case AVRCP_RESPONSE_TYPE_REJECTED:
        LOG_DEBUG("    Response Type = REJECTED\n");
        break;

    case AVRCP_RESPONSE_TYPE_INTERIM:
        LOG_DEBUG("    Response Type = INTERIM\n");
        break;

    case AVRCP_RESPONSE_TYPE_STABLE:
        LOG_DEBUG("    Response Type = STABLE\n");
        break;

    default:
        LOG_DEBUG("    Response Type = ???? (0x%02X)\n", rsp_type);
        break;
    }

    return;
}


void appl_avrcp_print_operation_id (UCHAR operation_id)
{
    switch (operation_id)
    {
    case AVRCP_OPERATION_ID_SELECT:
        LOG_DEBUG("    Operation ID = SELECT\n");
        break;

    case AVRCP_OPERATION_ID_UP:
        LOG_DEBUG("    Operation ID = UP\n");
        break;

    case AVRCP_OPERATION_ID_DOWN:
        LOG_DEBUG("    Operation ID = DOWN\n");
        break;

    case AVRCP_OPERATION_ID_LEFT:
        LOG_DEBUG("    Operation ID = LEFT\n");
        break;

    case AVRCP_OPERATION_ID_RIGHT:
        LOG_DEBUG("    Operation ID = RIGHT\n");
        break;

    case AVRCP_OPERATION_ID_RIGHT_UP:
        LOG_DEBUG("    Operation ID = RIGHT UP\n");
        break;

    case AVRCP_OPERATION_ID_RIGHT_DOWN:
        LOG_DEBUG("    Operation ID = RIGHT DOWN\n");
        break;

    case AVRCP_OPERATION_ID_LEFT_UP:
        LOG_DEBUG("    Operation ID = LEFT UP\n");
        break;

    case AVRCP_OPERATION_ID_LEFT_DOWN:
        LOG_DEBUG("    Operation ID = LEFT DOWN\n");
        break;

    case AVRCP_OPERATION_ID_ROOT_MENU:
        LOG_DEBUG("    Operation ID = ROOT MENU\n");
        break;

    case AVRCP_OPERATION_ID_SETUP_MENU:
        LOG_DEBUG("    Operation ID = SETUP MENU\n");
        break;

    case AVRCP_OPERATION_ID_CONTENTS_MENU:
        LOG_DEBUG("    Operation ID = CONTENTS MENU\n");
        break;

    case AVRCP_OPERATION_ID_FAVORITE_MENU:
        LOG_DEBUG("    Operation ID = FAVORITE MENU\n");
        break;

    case AVRCP_OPERATION_ID_EXIT:
        LOG_DEBUG("    Operation ID = EXIT\n");
        break;

    case AVRCP_OPERATION_ID_0:
        LOG_DEBUG("    Operation ID = 0\n");
        break;

    case AVRCP_OPERATION_ID_1:
        LOG_DEBUG("    Operation ID = 1\n");
        break;

    case AVRCP_OPERATION_ID_2:
        LOG_DEBUG("    Operation ID = 2\n");
        break;

    case AVRCP_OPERATION_ID_3:
        LOG_DEBUG("    Operation ID = 3\n");
        break;

    case AVRCP_OPERATION_ID_4:
        LOG_DEBUG("    Operation ID = 4\n");
        break;

    case AVRCP_OPERATION_ID_5:
        LOG_DEBUG("    Operation ID = 5\n");
        break;

    case AVRCP_OPERATION_ID_6:
        LOG_DEBUG("    Operation ID = 6\n");
        break;

    case AVRCP_OPERATION_ID_7:
        LOG_DEBUG("    Operation ID = 7\n");
        break;

    case AVRCP_OPERATION_ID_8:
        LOG_DEBUG("    Operation ID = 8\n");
        break;

    case AVRCP_OPERATION_ID_9:
        LOG_DEBUG("    Operation ID = 9\n");
        break;

    case AVRCP_OPERATION_ID_DOT:
        LOG_DEBUG("    Operation ID = DOT\n");
        break;

    case AVRCP_OPERATION_ID_ENTER:
        LOG_DEBUG("    Operation ID = ENTER\n");
        break;

    case AVRCP_OPERATION_ID_CLEAR:
        LOG_DEBUG("    Operation ID = CLEAR\n");
        break;

    case AVRCP_OPERATION_ID_CHANNEL_UP:
        LOG_DEBUG("    Operation ID = CHANNEL UP\n");
        break;

    case AVRCP_OPERATION_ID_CHANNEL_DOWN:
        LOG_DEBUG("    Operation ID = CHANNEL DOWN\n");
        break;

    case AVRCP_OPERATION_ID_PREVIOUS_CHANNEL:
        LOG_DEBUG("    Operation ID = PREVIOUS CHANNEL\n");
        break;

    case AVRCP_OPERATION_ID_SOUND_SELECT:
        LOG_DEBUG("    Operation ID = SOUND SELECT\n");
        break;

    case AVRCP_OPERATION_ID_INPUT_SELECT:
        LOG_DEBUG("    Operation ID = INPUT SELECT\n");
        break;

    case AVRCP_OPERATION_ID_DISPLAY_INFORMATION:
        LOG_DEBUG("    Operation ID = DISPLAY INFORMATION\n");
        break;

    case AVRCP_OPERATION_ID_HELP:
        LOG_DEBUG("    Operation ID = HELP\n");
        break;

    case AVRCP_OPERATION_ID_PAGE_UP:
        LOG_DEBUG("    Operation ID = PAGE UP\n");
        break;

    case AVRCP_OPERATION_ID_PAGE_DOWN:
        LOG_DEBUG("    Operation ID = PAGE DOWN\n");
        break;

    case AVRCP_OPERATION_ID_POWER:
        LOG_DEBUG("    Operation ID = POWER\n");
        break;

    case AVRCP_OPERATION_ID_VOLUME_UP:
        LOG_DEBUG("    Operation ID = VOLUME UP\n");
        break;

    case AVRCP_OPERATION_ID_VOLUME_DOWN:
        LOG_DEBUG("    Operation ID = VOLUME DOWN\n");
        break;

    case AVRCP_OPERATION_ID_MUTE:
        LOG_DEBUG("    Operation ID = MUTE\n");
        break;

    case AVRCP_OPERATION_ID_PLAY:
        LOG_DEBUG("    Operation ID = PLAY\n");
        break;

    case AVRCP_OPERATION_ID_STOP:
        LOG_DEBUG("    Operation ID = STOP\n");
        break;

    case AVRCP_OPERATION_ID_PAUSE:
        LOG_DEBUG("    Operation ID = PAUSE\n");
        break;

    case AVRCP_OPERATION_ID_RECORD:
        LOG_DEBUG("    Operation ID = RECORD\n");
        break;

    case AVRCP_OPERATION_ID_REWIND:
        LOG_DEBUG("    Operation ID = REWIND\n");
        break;

    case AVRCP_OPERATION_ID_FAST_FORWARD:
        LOG_DEBUG("    Operation ID = FAST FORWARD\n");
        break;

    case AVRCP_OPERATION_ID_EJECT:
        LOG_DEBUG("    Operation ID = EJECT\n");
        break;

    case AVRCP_OPERATION_ID_FORWARD:
        LOG_DEBUG("    Operation ID = FORWARD\n");
        break;

    case AVRCP_OPERATION_ID_BACKWARD:
        LOG_DEBUG("    Operation ID = BACKWARD\n");
        break;

    case AVRCP_OPERATION_ID_ANGLE:
        LOG_DEBUG("    Operation ID = ANGLE\n");
        break;

    case AVRCP_OPERATION_ID_SUBPICTURE:
        LOG_DEBUG("    Operation ID = SUBPICTURE\n");
        break;

    case AVRCP_OPERATION_ID_F1:
        LOG_DEBUG("    Operation ID = F1\n");
        break;

    case AVRCP_OPERATION_ID_F2:
        LOG_DEBUG("    Operation ID = F2\n");
        break;

    case AVRCP_OPERATION_ID_F3:
        LOG_DEBUG("    Operation ID = F3\n");
        break;

    case AVRCP_OPERATION_ID_F4:
        LOG_DEBUG("    Operation ID = F4\n");
        break;

    case AVRCP_OPERATION_ID_F5:
        LOG_DEBUG("    Operation ID = F5\n");
        break;

    case AVRCP_OPERATION_ID_VENDOR_UNIQUE:
        LOG_DEBUG("    Operation ID = VENDOR UNIQUE\n");
        break;

    default:
        LOG_DEBUG("    Operation ID = ???? (0x%02X)\n", operation_id);
        break;
    }

    return;
}

#endif /* #if (defined AVRCP_CT || defined AVRCP_TG) */
