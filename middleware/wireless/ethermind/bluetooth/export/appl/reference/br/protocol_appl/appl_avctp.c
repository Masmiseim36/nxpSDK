
/**
 *  \file appl_avctp.c
 *
 *  Source File for AVCTP Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_avctp.h"
#include "appl_utils.h"

#ifdef AVCTP

static const char main_avctp_options[] = " \n\
====== AVCTP Menu ========= \n\
   0. Exit. \n\
   1. Refresh \n\
  \n\
   2. Register Profile \n\
   3. Deregister Profile \n\
 \n\
   4. AVCTP Connect \n\
   5. AVCTP Disconnect \n\
 \n\
   6. AVCTP Send Message. \n\
 \n\
   100. AVCTP Test Cases.\n\
Your Option ? ";

static const char avctp_test_options[] = " \n\
====== AVCTP Test Menu ========= \n\
   0. Exit. \n\
   1. Refresh \n\
  \n\
   2.  AVCTP_N_1 \n\
   3.  AVCTP_N_2 \n\
   4.  AVCTP_N_3 \n\
   5.  AVCTP_N_4 \n\
   6.  AVCTP_N_5 \n\
   7.  AVCTP_N_6 \n\
   8.  AVCTP_N_7 \n\
   9.  AVCTP_N_8 \n\
   10. AVCTP_N_9 \n\
   11. AVCTP_N_10 \n\
   12. AVCTP_N_11 \n\
   13. AVCTP_N_12 \n\
   14. AVCTP_N_13 \n\
   15. AVCTP_N_14 \n\
   16. AVCTP_N_15 \n\
   17. AVCTP_N_16 \n\
   18. AVCTP_N_17 \n\
   19. AVCTP_N_18 \n\
Your Option ? ";

AVCTP_HANDLE avctp_handle [APPL_MAX_AVCTP_HANDLES];

void appl_avctp_init_handles ( void )
{
    UCHAR index;

    for (index = 0U; index < APPL_MAX_AVCTP_HANDLES; index ++)
    {
        AVCTP_INIT_HANDLE(avctp_handle[index]);
    }
}


void appl_avctp_show_handles ( void )
{
    UCHAR index;

    LOG_DEBUG("List of AVCTP Handles --------------------------\n");
    LOG_DEBUG("Index   Profile Index Sig Index\n");
    LOG_DEBUG("-----   ------------- ---------\n");
    for (index = 0U; index < APPL_MAX_AVCTP_HANDLES; index ++)
    {
        LOG_DEBUG(" %3d       %3d           %3d", index, avctp_handle[index].profile_index,
        avctp_handle[index].ctrl_ch_index);
        LOG_DEBUG("\n");
    }
}


void appl_avctp_choose_handle (UCHAR *retval)
{
    int read_val;

    BT_LOOP_FOREVER()
    {
        appl_avctp_show_handles();

        LOG_DEBUG("\n");
        LOG_DEBUG("Choose Index = "); fflush(stdout);
        scanf("%d", &read_val);
        if ((read_val >= 0) && (read_val < APPL_MAX_AVCTP_HANDLES))
        {
            break;
        }
    }
    *retval = (UCHAR)read_val;
}


void appl_avctp_profile_register ( void )
{
    UCHAR index;
    API_RESULT retval;
    UINT16 profile_id;
    int    read_value;

    appl_avctp_choose_handle(&index);

    LOG_DEBUG("\n");
    LOG_DEBUG("Enter Profile ID (in HEX):");
    scanf("%x", &read_value);
    profile_id = (UINT16)read_value;
    LOG_DEBUG("Registering AVCTP Profile ... "); fflush(stdout);

    retval = BT_avctp_profile_register
             (
                 avctp_ntf_cb,
                 profile_id,
                 &avctp_handle[index]
             );

    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        LOG_DEBUG("Successful. Profile Entity Index = 0x%02X\n",
        avctp_handle[index].profile_index);
    }
}


void main_avctp_operations(void)
{
    int choice, menu_choice;
    static UCHAR first_time = 0x00U;

    if (0x00U == first_time)
    {
        first_time = 0x01U;
        appl_avctp_init_handles();
    }

    BT_LOOP_FOREVER()
    {
        printf("%s", main_avctp_options);
        scanf("%d", &choice);
        menu_choice = choice;

        if (choice < 0)
        {
            LOG_DEBUG("*** Invalid Choice. Try Again.\n");
            continue;
        }

        if(100U == choice)
        {
            LOG_DEBUG("%s", avctp_test_options);
            scanf("%d", &choice);
            appl_avctp_test_menu(choice);
        }
        else
        {
            switch (choice)
            {
            case 0:
                break; /* return; */

            case 1:
                break;

            case 2:
                appl_avctp_profile_register();
                break;

            case 3:
                appl_avctp_profile_deregister();
                break;

            case 4:
                appl_avctp_connect_req();
                break;

            case 5:
                appl_avctp_disconnect_req();
                break;

            case 6:
                appl_avctp_send_message();
                break;

            default:
                LOG_DEBUG("Invalid Option : %d.\n", choice);
                break;
            }

            if (0 == menu_choice)
            {
                /* return */
                break;
            }
        }
    }

    return;
}


API_RESULT avctp_ntf_cb
           ( UCHAR  event_type,
             UINT16 event_result,
             AVCTP_HANDLE * handle,
             void  * event_data,
             UINT16 event_datalen )
{
    UINT16 i;
    UCHAR *evnt_data = (UCHAR *)event_data;
    UCHAR tl, mt;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    for(i = 0U; i < APPL_MAX_AVCTP_HANDLES; i++)
    {
        if(avctp_handle[i].profile_index == handle->profile_index)
        {
            avctp_handle[i].ctrl_ch_index = handle->ctrl_ch_index;
        }
    }

    LOG_DEBUG("\n");
    switch (event_type)
    {
    case AVCTP_CONNECT_IND:
        LOG_DEBUG("-------------- AVCTP --------------\n");
        LOG_DEBUG("Received AVCTP_CONNECT_IND.\n");
        LOG_DEBUG("AVCTP Profile Index = %d, Sig Index = %d, Result = 0x%04X\n",
        handle->profile_index, handle->ctrl_ch_index, event_result);
        LOG_DEBUG("-----------------------------------\n");

        break;

    case AVCTP_CONNECT_CNF:
        LOG_DEBUG("-------------- AVCTP --------------\n");
        LOG_DEBUG("Received AVCTP_CONNECT_CNF.\n");
        LOG_DEBUG("AVCTP Profile Index = %d, Sig Index = %d, Result = 0x%04X\n",
        handle->profile_index, handle->ctrl_ch_index, event_result);
        LOG_DEBUG("-----------------------------------\n");

        break;

    case AVCTP_DISCONNECT_IND:
        LOG_DEBUG("-------------- AVCTP --------------\n");
        LOG_DEBUG("Received AVCTP_DISCONNECT_IND.\n");
        LOG_DEBUG("AVCTP Profile Index = %d, Sig Index = %d, Result = 0x%04X\n",
        handle->profile_index, handle->ctrl_ch_index, event_result);
        LOG_DEBUG("-----------------------------------\n");

        break;

    case AVCTP_DISCONNECT_CNF:
        LOG_DEBUG("-------------- AVCTP --------------\n");
        LOG_DEBUG("Received AVCTP_DISCONNECT_CNF.\n");
        LOG_DEBUG("AVCTP Profile Index = %d, Sig Index = %d, Result = 0x%04X\n",
        handle->profile_index, handle->ctrl_ch_index, event_result);
        LOG_DEBUG("-----------------------------------\n");

        break;

    case AVCTP_MESSAGE_IND:
        LOG_DEBUG("-------------- AVCTP --------------\n");
        LOG_DEBUG("Received AVCTP_MESSAGE_IND.\n");
        LOG_DEBUG("AVCTP Profile Index = %d, Sig Index = %d, Result = 0x%04X\n",
        handle->profile_index, handle->ctrl_ch_index, event_result);

        AVCTP_GET_MSG_HDR(evnt_data, tl, mt);
        LOG_DEBUG("Messaget Type = %s, Transaction Lable = %d\n",
        (0U == mt) ? "Command" : "Response", tl);
        LOG_DEBUG("-----------------------------------\n");

        if (API_SUCCESS == event_result)
        {
            LOG_DEBUG("\nAVCTP Message Data(%d bytes):\n==============\n",event_datalen);
            for ( i = 0U; i < event_datalen;  )
            {
                LOG_DEBUG("0x%02X ",evnt_data[i++]);
            }
            LOG_DEBUG("\n");
            retval = API_SUCCESS; /* return API_SUCCESS; */
        }
        else
        {
            LOG_DEBUG("Some Failure!!\n");
            retval = API_FAILURE; /* return API_FAILURE; */
        }
        break;

    default:
        LOG_DEBUG("Received Unknown Event 0x%02X. Result = 0x%04X\n",
        event_type, event_result);
        break;
    }

    return retval;
}

void appl_avctp_connect_req ( void )
{
    UCHAR index;
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    API_RESULT retval;

    appl_avctp_choose_handle(&index);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVCTP Connect ... "); fflush(stdout);
    LOG_DEBUG("\nEnter Remote Device BD_ADDR:");

    appl_get_bd_addr(bd_addr);

    retval = BT_avctp_connect_req
             (
                 &avctp_handle[index],
                 bd_addr
             );

    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully Started.\n");
    }
}


void appl_avctp_disconnect_req ( void )
{
    UCHAR index;
    API_RESULT retval;

    appl_avctp_choose_handle(&index);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVCTP Disconnect ... "); fflush(stdout);

    retval = BT_avctp_disconnect_req(&avctp_handle[index]);

    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully Started.\n");
    }
}


void appl_avctp_send_message ( void )
{
    API_RESULT retval;
    UCHAR index, tl, mt;
    INT32 pkt_len;
    UINT16 counter;
    UCHAR *appl_write_buf = NULL;
    int    read_value;

    appl_avctp_choose_handle(&index);

    LOG_DEBUG("\n");
    LOG_DEBUG("Construct pkt...\n");

    LOG_DEBUG("Pkt len(>10)? "); fflush(stdout);
    scanf("%d", &read_value);
    pkt_len = (INT32)read_value;
    appl_write_buf = BT_alloc_mem(pkt_len);

    if (NULL == appl_write_buf)
    {
        LOG_DEBUG("Memory allocation for failed!!\n");
        /* return; */
    }
    else
    {
        LOG_DEBUG("\nEnter Transcation Lable:");
        scanf("%d", &read_value);
        tl = (UCHAR)read_value;

        LOG_DEBUG("\nEnter Message Type:");
        scanf("%d", &read_value);
        mt = (UCHAR)read_value;

        if(0x00U == mt)
        {
            /* Update Transaction Lable, Message Type */
            AVCTP_UPDATE_MSG_HDR(appl_write_buf, tl, AVCTP_CTR_MSG_COMMAND);
        }
        else if (0x01U == mt)
        {
            /* Update Transaction Lable, Message Type */
            AVCTP_UPDATE_MSG_HDR(appl_write_buf, tl, AVCTP_CTR_MSG_RESPONSE);
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        for(counter = 1U; counter < pkt_len; counter++)
        {
            appl_write_buf[counter] = (UCHAR)counter;
        }

        for(counter = 0U; counter < pkt_len; counter++)
        {
             LOG_DEBUG("data[%d] - 0x%02X\n", counter, appl_write_buf[counter]);
        }

        LOG_DEBUG("Starting AVCTP Message Write..\n");
        retval = BT_avctp_send_message
                 (
                     &avctp_handle[index],
                     appl_write_buf,
                     (UINT16)pkt_len
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
        }
        else
        {
            LOG_DEBUG("Successfully Started.\n");
        }
    }

    return;
}

void appl_avctp_profile_deregister (void)
{
    UCHAR index;
    API_RESULT retval;

    appl_avctp_choose_handle(&index);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVCTP Profile Deregistration ... "); fflush(stdout);

    retval = BT_avctp_profile_deregister
             (
                 &avctp_handle[index]
             );

    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully Done.\n");
    }
}


void appl_avctp_test_menu (int choice)
{
    UCHAR index;
    UINT16 profile_id;
    API_RESULT retval;
    int read_value;

    switch(choice)
    {
    case 2:
        appl_avctp_choose_handle(&index);
        LOG_DEBUG("\n");
        LOG_DEBUG("Enter Profile ID:");
        scanf("%d", &read_value);
        profile_id = (UINT16)read_value;
        LOG_DEBUG("Registering AVCTP Profile with NTF CB as NULL\n");
        fflush(stdout);

        retval = BT_avctp_profile_register
                 (
                     NULL,
                     profile_id,
                     &avctp_handle[index]
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("Test Case %d PASSED, as AVCTP has rejected this REQ\n",
            (choice - 1U));
        }
        else
        {
            LOG_DEBUG("Test Case Failed.\n");
        }
        break;

    case 4:
        appl_avctp_choose_handle(&index);
        LOG_DEBUG("\n");
        LOG_DEBUG("Enter Profile ID:");
        scanf("%d", &read_value);
        profile_id = (UINT16)read_value;
        LOG_DEBUG("Registering AVCTP Profile with AVCTP Handle as NULL\n");
        fflush(stdout);

        retval = BT_avctp_profile_register
                 (
                     avctp_ntf_cb,
                     profile_id,
                     NULL
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("Test Case %d PASSED, as AVCTP has rejected this REQ\n",
            (choice - 1U));
        }
        else
        {
            LOG_DEBUG("Test Case Failed.\n");
        }
        break;

    default:
        LOG_DEBUG("Invalid Option: %d\n", choice);
        break;
    }
}
#endif /* AVCTP */
