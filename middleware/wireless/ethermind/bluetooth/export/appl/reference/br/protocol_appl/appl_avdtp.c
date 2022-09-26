
/**
 *  \file appl_avdtp.c
 *
 *  Source File for AVDTP Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_avdtp.h"
#include "appl_utils.h"

#ifdef AVDTP

static const char main_avdtp_options[] = " \n\
====== AVDTP Menu ========= \n\
   0.  Exit. \n\
   1.  Refresh \n\
 \n\
   2.  Initialize Handle \n\
 \n\
   3.  Register SEP \n\
 \n\
   10. AVDTP Connect \n\
   11. AVDTP Disconnect \n\
 \n\
   13. AVDTP Discover. \n\
   14. AVDTP Get Capability. \n\
   15. AVDTP Set Configuration \n\
   16. AVDTP Get Configuration \n\
   17. AVDTP Open        Stream \n\
   18. AVDTP Start       Stream \n\
   19. AVDTP Suspend     Stream \n\
   20. AVDTP Reconfigure Stream \n\
   21. AVDTP Close       Stream\n\
   22. AVDTP Abort       Stream \n\
   23. AVDTP Media Write \n\
 \n\
Your Option ? ";

UCHAR appl_response_buf[100U];
AVDTP_HANDLE appl_avdtp_handle [APPL_MAX_AVDTP_HANDLES];
UCHAR appl_avdtp_codec_ie[4U] = {0x1AU, 0x2BU, 0x3CU, 0x4DU};


void appl_avdtp_init_handles ( void )
{
    UCHAR index;

    for (index = 0U; index < APPL_MAX_AVDTP_HANDLES; index ++)
    {
        AVDTP_INIT_HANDLE(appl_avdtp_handle[index]);
        AVDTP_SET_HANDLE_CALLBACK(appl_avdtp_handle[index], av_notify_cb);
    }
}


void appl_avdtp_show_handles ( void )
{
    UCHAR index;

    LOG_DEBUG("List of AVDTP Handles --------------------------\n");
    LOG_DEBUG("Index   BD_ADDR\n");
    LOG_DEBUG("-----   -----------------\n");
    for (index = 0U; index < APPL_MAX_AVDTP_HANDLES; index ++)
    {
        LOG_DEBUG(" %3d   %02X:%02X:%02X:%02X:%02X:%02X", index,
        appl_avdtp_handle[index].bd_addr[0U], appl_avdtp_handle[index].bd_addr[1U],
        appl_avdtp_handle[index].bd_addr[2U], appl_avdtp_handle[index].bd_addr[3U],
        appl_avdtp_handle[index].bd_addr[4U], appl_avdtp_handle[index].bd_addr[5U]);

        LOG_DEBUG("\n");
    }
}


void appl_avdtp_choose_handle (UCHAR *retval)
{
    int read_val;

    BT_LOOP_FOREVER()
    {
        appl_avdtp_show_handles();

        LOG_DEBUG("\n");
        LOG_DEBUG("Choose Index = "); fflush(stdout);
        scanf("%d", &read_val);
        if ((read_val >= 0) && (read_val < APPL_MAX_AVDTP_HANDLES))
        {
            break;
        }
    }
    *retval = (UCHAR)read_val;
}


void appl_avdtp_set_handle_params ( void )
{
    int read_val;
    UCHAR index;
    UCHAR bd_addr[BT_BD_ADDR_SIZE];

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    index = 0U;
    BT_mem_set(bd_addr, 0, sizeof(UCHAR)*BT_BD_ADDR_SIZE);

    appl_avdtp_choose_handle(&index);

    LOG_DEBUG("Change BD_ADDR (1/0) ? "); fflush(stdout);
    scanf("%d", &read_val);
    if (1U == read_val)
    {
        LOG_DEBUG("Enter BD_ADDR = ");
        appl_get_bd_addr(bd_addr);
        AVDTP_SET_HANDLE_BD_ADDR(appl_avdtp_handle[index], bd_addr);
    }
    LOG_DEBUG("\n");

    appl_avdtp_show_handles();
    LOG_DEBUG("\n");
}


void appl_avdtp_sep_register ( void )
{
    UCHAR index;
    API_RESULT retval;
    AVDTP_SEP_CAP sep_cap;
    int           read_val;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    index = 0U;
    BT_mem_set(&sep_cap, 0, sizeof(AVDTP_SEP_CAP));

    appl_avdtp_choose_handle(&index);

    LOG_DEBUG("Select Streaming End Point Type.  Source(0)/Sink(1)... "); fflush(stdout);

    scanf("%d", &read_val);

    if (AVDTP_SEP_SOURCE == read_val)
    {
        sep_cap.sep_type = AVDTP_SEP_SOURCE;
    }
    else
    {
        sep_cap.sep_type = AVDTP_SEP_SINK;
    }

    sep_cap.codec_cap.media_type = AVDTP_MEDIA_AUDIO;
    sep_cap.codec_cap.codec_type = AVDTP_CODEC_AUDIO_SBC;
    sep_cap.codec_cap.codec_ie = appl_avdtp_codec_ie;
    sep_cap.codec_cap.codec_ie_len = 4U;

#ifdef AVDTP_HAVE_RECOVERY_SERVICE
    /* Initialize the Streaming End Point Recovery Capability */
    AVDTP_SET_SEP_RECOVERY_CAPABILITY
    (
        sep_cap,
        AVDTP_RECOVERY_TYPE_RFC2733,
        AVDTP_MAX_MRWS,
        AVDTP_MAX_MNMP
    );
#endif /* AVDTP_HAVE_RECOVERY_SERVICE */

    LOG_DEBUG("\n");
    LOG_DEBUG("Registering AVDTP SEP ... "); fflush(stdout);
    retval = BT_avdtp_sep_register (&sep_cap,
                                    av_notify_cb,
                                    &appl_avdtp_handle[index]);
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        LOG_DEBUG("Successful. SEID = 0x%02X\n", appl_avdtp_handle[index].local_seid);
    }
}


void appl_avdtp_connect_req ( void )
{
    UCHAR index;
    API_RESULT retval;

    appl_avdtp_choose_handle(&index);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVDTP Connect ... "); fflush(stdout);
    retval = BT_avdtp_connect_req(&appl_avdtp_handle[index]);
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully Started.\n");
    }
}


void appl_avdtp_disconnect_req ( void )
{
    UCHAR index;
    API_RESULT retval;

    appl_avdtp_choose_handle(&index);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVDTP Disconnect ... "); fflush(stdout);
    retval = BT_avdtp_disconnect_req(&appl_avdtp_handle[index]);
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully Started.\n");
    }
}


void appl_avdtp_set_configuration ( void )
{
    UCHAR index, remote_seid;
    API_RESULT retval;
    AVDTP_SEP_CONF sep_conf;
    int seid;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    index = 0U;
    remote_seid = 0U;
    BT_mem_set(&sep_conf, 0, sizeof(AVDTP_SEP_CONF));

    appl_avdtp_choose_handle(&index);

    sep_conf.service_flag               = 0x01U;
    sep_conf.recovery_cap.recovery_type = 0x00U;
    sep_conf.recovery_cap.mrws          = 0x00U;
    sep_conf.recovery_cap.mnmp          = 0x00U;
    sep_conf.rohc_conf                  = 0x00U;

    sep_conf.codec_cap.media_type = AVDTP_MEDIA_AUDIO;
    sep_conf.codec_cap.codec_type = AVDTP_CODEC_AUDIO_SBC;
    sep_conf.codec_cap.codec_ie = appl_avdtp_codec_ie;
    sep_conf.codec_cap.codec_ie_len = 4U;

    LOG_DEBUG("Enter Remote SEID: "); fflush(stdout);
    scanf("%d",&seid);

    remote_seid = (UCHAR)seid;
    AVDTP_SET_HANDLE_REMOTE_SEID(appl_avdtp_handle[index], remote_seid);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVDTP Set Configuration ... "); fflush(stdout);
    retval = BT_avdtp_set_configuration_req(&appl_avdtp_handle[index], &sep_conf);
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully Started.\n");
    }
}


void appl_avdtp_get_configuration ( void )
{
    UCHAR index, remote_seid;
    API_RESULT retval;
    int seid;

    appl_avdtp_choose_handle(&index);

    LOG_DEBUG("Enter Remote SEID: "); fflush(stdout);
    scanf("%d",&seid);

    remote_seid = (UCHAR)seid;
    AVDTP_SET_HANDLE_REMOTE_SEID(appl_avdtp_handle[index], remote_seid);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVDTP Get Configuration ... ");
    retval = BT_avdtp_get_configuration_req
             (
                 &appl_avdtp_handle[index],
                 appl_response_buf,
                 100U
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


void appl_avdtp_close ( void )
{
    UCHAR index, remote_seid;
    API_RESULT retval;
    int seid;

    appl_avdtp_choose_handle(&index);

    LOG_DEBUG("Enter Remote SEID: "); fflush(stdout);
    scanf("%d",&seid);

    remote_seid = (UCHAR)seid;
    AVDTP_SET_HANDLE_REMOTE_SEID(appl_avdtp_handle[index], remote_seid);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVDTP Close Stream ... "); fflush(stdout);
    retval = BT_avdtp_close_req(&appl_avdtp_handle[index]);
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully Started.\n");
    }

}

void appl_avdtp_abort ( void )
{
    UCHAR index, remote_seid;
    API_RESULT retval;
    int seid;

    appl_avdtp_choose_handle(&index);

    LOG_DEBUG("Enter Remote SEID: "); fflush(stdout);
    scanf("%d",&seid);

    remote_seid = (UCHAR)seid;
    AVDTP_SET_HANDLE_REMOTE_SEID(appl_avdtp_handle[index], remote_seid);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVDTP Abort ... "); fflush(stdout);
    retval = BT_avdtp_abort_req(&appl_avdtp_handle[index]);
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully Started.\n");
    }

}

void main_avdtp_operations(void)
{
    int choice, menu_choice;
    static UCHAR first_time = 0x00U;

    if (0x00U == first_time)
    {
        appl_avdtp_init_handles();
        (BT_IGNORE_RETURN_VALUE) BT_avdtp_callback_register (av_notify_cb);

        first_time = 0x01U;
    }

    BT_LOOP_FOREVER()
    {
        printf("%s", main_avdtp_options);
        scanf("%d", &choice);
        if (choice < 0)
        {
            LOG_DEBUG("*** Invalid Choice. Try Again.\n");
            continue;
        }

        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            appl_avdtp_set_handle_params();
            break;

        case 3:
            appl_avdtp_sep_register();
            break;

        case 10:
            appl_avdtp_connect_req();
            break;

        case 11:
            appl_avdtp_disconnect_req();
            break;

        case 13:
            appl_avdtp_discover ();
            break;

        case 14:
            appl_avdtp_get_cap ();
            break;

        case 15:
            appl_avdtp_set_configuration();
            break;

        case 16:
            appl_avdtp_get_configuration();
            break;

        case 17:
            appl_avdtp_open ();
            break;

        case 18:
            appl_avdtp_start ();
            break;

        case 19:
            appl_avdtp_suspend ();
            break;

        case 20:
            appl_avdtp_reconfigure ();
            break;

        case 21:
            appl_avdtp_close();
            break;

        case 22:
            appl_avdtp_abort();
            break;

        case 23:
        #ifdef A2DP_SOURCE
            appl_avdtp_write ();
        #endif /* A2DP_SOURCE */
            break;

        case 30:
            (BT_IGNORE_RETURN_VALUE) l2ca_disconnect_req(0x0041U);
            break;

        case 31:
        {
            UCHAR mph[12U];

            AVDTP_SET_MEDIA_PACKET_HEADER
            (mph, 0x02U, 0x0U, 0x0U, 0x0U, 0x09U, 0xAABBCCDDU, 0xF1F2F3F4U);
            AVDTP_SET_MEDIA_PACKET_HEADER_SEQ(mph, 5U);
            appl_dump_bytes(mph, 12U);

            AVDTP_SET_MEDIA_PACKET_HEADER
            (mph, 0x02U, 0x1U, 0x1U, 0x0U, 0x09U, 0xAABBCCDDU, 0xF1F2F3F4U);
            AVDTP_SET_MEDIA_PACKET_HEADER_SEQ(mph, 5U);
            appl_dump_bytes(mph, 12U);

            AVDTP_SET_MEDIA_PACKET_HEADER
            (mph, 0x02U, 0x1U, 0x1U, 0x1U, 0x09U, 0xAABBCCDDU, 0xF1F2F3F4U);
            AVDTP_SET_MEDIA_PACKET_HEADER_SEQ(mph, 5U);
            appl_dump_bytes(mph, 12U);

            AVDTP_SET_MEDIA_PACKET_HEADER
            (mph, 0x02U, 0x1U, 0x0U, 0x1U, 0x09U, 0xAABBCCDDU, 0xF1F2F3F4U);
            AVDTP_SET_MEDIA_PACKET_HEADER_SEQ(mph, 5U);
            appl_dump_bytes(mph, 12U);

            AVDTP_SET_MEDIA_PACKET_HEADER_SEQ(mph, 6U);
            appl_dump_bytes(mph, 12U);

            AVDTP_SET_MEDIA_PACKET_HEADER_SEQ(mph, 256U);
            appl_dump_bytes(mph, 12U);
        }
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

    return;
}


API_RESULT av_notify_cb
           (
               UCHAR           event_type,
               UINT16          event_result,
               AVDTP_HANDLE  * avdtp_handle,
               void          * event_data,
               UINT16          event_datalen
           )
{
    API_RESULT result;
    UINT16 i,j,losc = 0U;
    AVDTP_SEP_CONF  sep_conf;
    AVDTP_SEP_INFO  sep_info;
    UCHAR *evnt_data = (UCHAR *)event_data;

    /* Init */
    result = API_SUCCESS;

    LOG_DEBUG("\n");
    switch (event_type)
    {
    case AVDTP_CONNECT_CNF:
        LOG_DEBUG("Received AVDTP_CONNECT_CNF.\n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        break;

    case AVDTP_DISCONNECT_CNF:
        LOG_DEBUG("Received AVDTP_DISCONNECT_CNF.\n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        break;

    case AVDTP_SET_CONFIGURATION_CNF:
        LOG_DEBUG("Received AVDTP_SET_CONFIGURATION_CNF. \n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        break;

    case AVDTP_SET_CONFIGURATION_IND:
        LOG_DEBUG("Received AVDTP_SET_CONFIGURATION_IND. \n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);

        result = BT_avdtp_decode_sep_configuration(event_data,
                                                   event_datalen,
                                                   &sep_conf);
        if(API_SUCCESS != result)
        {
            LOG_DEBUG("Set Conf Command Decode Failed. Reason = 0x%04X\n",result);
            result = API_FAILURE; /* return API_FAILURE; */
        }

        break;

    case AVDTP_DISCOVER_CNF:
        LOG_DEBUG("Received AVDTP_DISCOVER_CNF.\n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        if (API_SUCCESS == event_result)
        {
            LOG_DEBUG("\nDiscover Info:\n==============\n");
            for(i = 0U; i < event_datalen; i += 2U)
            {
                result = BT_avdtp_decode_sep_information
                (&evnt_data[i], (UINT16)(event_datalen - i), &sep_info);

                if(API_SUCCESS != result)
                {
                    LOG_DEBUG("FAILED to decode Discover Response. Reason = 0x%04X\n",
                    result);
                }
                else
                {
                    LOG_DEBUG("\tIN USE   = 0x%02X "\
                           "SEP TYPE    = 0x%02X\n"\
                           "\tACP SEID = 0x%02X, Media Type = 0x%02X\n",
                           sep_info.in_use,
                           sep_info.sep_type,
                           sep_info.acp_seid,
                           sep_info.media_type);

                    LOG_DEBUG("\n");
                }
            }
        }
        else
        {
            LOG_DEBUG("Received Discover Reject.\nError Code:0x%04X\n",
                                                        event_result);
        }

        break;

    case AVDTP_GET_CAPABILITIES_CNF:
        LOG_DEBUG("Received AVDTP_GET_CAPABILITIES_CNF.\n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        if (API_SUCCESS == event_result)
        {
            LOG_DEBUG("\nGet Cap Info:\n=============\n");
            for ( i = 0U; i < event_datalen;  )
            {
                LOG_DEBUG("\tService Category: %02d\n",appl_response_buf[i++]);
                losc = appl_response_buf[i++];
                LOG_DEBUG("\tLOSC: %02d\n",losc);
                if (0U < losc)
                {
                    LOG_DEBUG("\tService Cat Info Bytes: ");
                    for (j = 0U; j < losc; j++)
                    {
                        LOG_DEBUG("0x%02X ", appl_response_buf[i++]);
                    }
                    LOG_DEBUG("\n");
                }
                LOG_DEBUG ("\n");
            }
        }
        else
        {
            LOG_DEBUG("Received Get Cap Reject.\nError Code:0x%04X\n",
                                                       event_result);
        }

        break;

    case AVDTP_GET_CONFIGURATION_CNF:
        LOG_DEBUG("Received AVDTP_GET_CONFIGURATION_CNF.\n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        if (API_SUCCESS == event_result)
        {
            LOG_DEBUG("\nGet Conf Info:\n==============\n");
            for ( i = 0U; i < event_datalen;  )
            {
                LOG_DEBUG("\tService Category: %02d\n",appl_response_buf[i++]);
                losc = appl_response_buf[i++];
                LOG_DEBUG("\tLOSC: %02d\n",losc);
                if (0U < losc)
                {
                    LOG_DEBUG("\tService Cat Info Bytes: ");
                    for (j = 0U; j < losc; j++)
                    {
                        LOG_DEBUG("0x%02X ", appl_response_buf[i++]);
                    }
                    LOG_DEBUG("\n");
                }
                LOG_DEBUG ("\n");
            }
        }
        else
        {
            LOG_DEBUG("Received Get Cap Reject.\nError Code:0x%04X\n",
                                                       event_result);
        }

        break;

    case AVDTP_START_IND:
        LOG_DEBUG("Received AVDTP_START_IND.\n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        if (API_SUCCESS == event_result)
        {
            LOG_DEBUG("START Accepted. OutMTU = 0x%04X (%d)\n",
            *evnt_data, *evnt_data);
        }
        else
        {
            LOG_DEBUG("START rejected!\n");
        }

        break;

    case AVDTP_START_CNF:
        LOG_DEBUG("Received AVDTP_START_CNF.\n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        if (API_SUCCESS == event_result)
        {
            LOG_DEBUG("STREAMING STARTED. OutMTU = 0x%04X (%d)\n",
            *evnt_data, *evnt_data);
        }
        else
        {
            LOG_DEBUG("STREAMING FAILED\n");
        }

        break;

    case AVDTP_OPEN_IND:
        LOG_DEBUG("Received AVDTP_OPEN_IND.\n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        if (API_SUCCESS == event_result)
        {
            LOG_DEBUG("OPEN Accepted.\n");
        }
        else
        {
            LOG_DEBUG("OPEN rejected!\n");
        }

        break;

    case AVDTP_OPEN_CNF:
        LOG_DEBUG("Received AVDTP_OPEN_CNF.\n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        if (API_SUCCESS == event_result)
        {
            LOG_DEBUG("Stream OPEN.\n");
        }
        else
        {
            LOG_DEBUG("OPEN failed.\n");
        }

        break;

    case AVDTP_SUSPEND_IND:
        LOG_DEBUG("Received AVDTP_SUSPEND_IND.\n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        if (API_SUCCESS == event_result)
        {
            LOG_DEBUG("SUSPEND Accepted.\n");
        }
        else
        {
            LOG_DEBUG("SUSPEND rejected!\n");
        }

        break;

    case AVDTP_SUSPEND_CNF:
        LOG_DEBUG("Received AVDTP_SUSPEND_CNF.\n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        if (API_SUCCESS == event_result)
        {
            LOG_DEBUG("Stream SUSPENDED.\n");
        }
        else
        {
            LOG_DEBUG("SUSPEND failed.\n");
        }

        break;

    case AVDTP_RECONFIGURE_IND:
        LOG_DEBUG("Received AVDTP_RECONFIGURE_IND.\n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        if (API_SUCCESS == event_result)
        {
            LOG_DEBUG("\nReconfigure Info:\n================\n");
            for ( i = 1U; i < event_datalen;  )
            {
                LOG_DEBUG("\tService Category: %02d\n",evnt_data[i++]);
                losc = evnt_data[i++];
                LOG_DEBUG("\tLOSC: %02d\n",losc);
                if (0U < losc)
                {
                    LOG_DEBUG("\tService Cat Info Bytes: ");
                    for (j = 0U; j < losc; j++)
                    {
                        LOG_DEBUG("0x%02X ", evnt_data[i++]);
                    }
                    LOG_DEBUG("\n");
                }
                LOG_DEBUG ("\n");
            }
        }
        else
        {
            LOG_DEBUG("RECONFIGURE rejected!\n");
            result = API_FAILURE; /* return API_FAILURE; */
        }

        break;

    case AVDTP_RECONFIGURE_CNF:
        LOG_DEBUG("Received AVDTP_RECONFIGURE_CNF.\n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        if (API_SUCCESS == event_result)
        {
            LOG_DEBUG("RECONFIGURE DONE\n");
        }
        else
        {
            LOG_DEBUG("RECONFIGURE FAILED\n");
        }

        break;

    case AVDTP_MEDIA_DATA_IND:
        LOG_DEBUG("Received AVDTP_MEDIA_DATA_IND.\n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);
        if (API_SUCCESS == event_result)
        {
            LOG_DEBUG("\nMedia Data(%d bytes):\n==============\n",event_datalen);
            for ( i = 0U; i < event_datalen;  )
            {
                LOG_DEBUG("0x%02X ",evnt_data[i++]);
            }
            LOG_DEBUG("\n");
            result = API_SUCCESS; /* return API_SUCCESS; */
        }
        else
        {
            LOG_DEBUG("Some Failure!!\n");
            result = API_FAILURE; /* return API_FAILURE; */
        }
        break;

    case AVDTP_CLOSE_CNF:
        LOG_DEBUG("Received AVDTP_CLOSE_CNF. \n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        break;

    case AVDTP_CLOSE_IND:
        LOG_DEBUG("Received AVDTP_CLOSE_IND. \n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        break;

    case AVDTP_ABORT_CNF:
        LOG_DEBUG("Received AVDTP_ABORT_CNF. \n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        break;

    case AVDTP_ABORT_IND:
        LOG_DEBUG("Received AVDTP_ABORT_IND. \n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        break;

    case AVDTP_CONNECT_IND:
        LOG_DEBUG("Received AVDTP_CONNECT_IND. \n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        break;

    case AVDTP_DISCONNECT_IND:
        LOG_DEBUG("Received AVDTP_DISCONNECT_IND. \n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        break;

    case AVDTP_RESET_IND:
        LOG_DEBUG("Received AVDTP_RESET_IND. \n");
        LOG_DEBUG("Handle:\n");
        LOG_DEBUG("    BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n",
        avdtp_handle->bd_addr[0U], avdtp_handle->bd_addr[1U],
        avdtp_handle->bd_addr[2U], avdtp_handle->bd_addr[3U],
        avdtp_handle->bd_addr[4U], avdtp_handle->bd_addr[5U]);
        LOG_DEBUG("    Local SEID: %02d\n", avdtp_handle->local_seid);
        LOG_DEBUG("    Remote SEID: %02d\n", avdtp_handle->remote_seid);
        LOG_DEBUG("Result = 0x%04X\n", event_result);

        break;

    default:
        LOG_DEBUG("Received Unknown Event 0x%02X. Result = 0x%04X\n",
        event_type, event_result);
        break;
    }

    return result;
}


void appl_avdtp_discover ( void )
{
    API_RESULT retval;

    retval =  BT_avdtp_discover_req
              (
                  &appl_avdtp_handle[0U],
                  appl_response_buf,
                  100U
              );

    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        LOG_DEBUG("DICOVER SEND Successful.\n");
    }
}

void appl_avdtp_get_cap ( void )
{
    API_RESULT retval;
    UCHAR index, remote_seid;
    int seid;

    appl_avdtp_choose_handle(&index);

    LOG_DEBUG("Enter Remote SEID: "); fflush(stdout);
    scanf("%d",&seid);

    remote_seid = (UCHAR)seid;
    AVDTP_SET_HANDLE_REMOTE_SEID(appl_avdtp_handle[index], remote_seid);


    LOG_DEBUG("\n");

    LOG_DEBUG("Starting AVDTP Get Cap ... ");
    retval = BT_avdtp_get_capabilities_req
              (
                  &appl_avdtp_handle[index],
                  appl_response_buf,
                  100U
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


void appl_avdtp_open ( void )
{
    API_RESULT retval;
    UCHAR index;

    appl_avdtp_choose_handle(&index);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVDTP Open ... ");
    retval = BT_avdtp_open_req
              (
                  &appl_avdtp_handle[index]
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


void appl_avdtp_start ( void )
{
    API_RESULT retval;
    UCHAR index;

    appl_avdtp_choose_handle(&index);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVDTP Start ... ");
    retval = BT_avdtp_start_req
              (
                  &appl_avdtp_handle[index]
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

void appl_avdtp_suspend ( void )
{
    API_RESULT retval;
    UCHAR index;

    appl_avdtp_choose_handle(&index);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVDTP Suspend ... ");
    retval = BT_avdtp_suspend_req
              (
                  &appl_avdtp_handle[index]
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

#ifdef A2DP_SOURCE
void appl_avdtp_write ( void )
{
    API_RESULT retval;
    UCHAR index;
    int read_val, pkt_len;
    UCHAR media_header[12U] = {1U,2U,3U,4U,5U,6U,7U,8U,9U,1U,2U,3U};
    UCHAR *appl_write_buf = NULL;

    appl_avdtp_choose_handle(&index);

    LOG_DEBUG("\n");
    LOG_DEBUG("Construct pkt...\n");

    LOG_DEBUG("Pkt len(>10)? "); fflush(stdout);
    scanf("%d", &pkt_len);
    appl_write_buf = BT_alloc_mem (pkt_len);
    if (NULL == appl_write_buf)
    {
        LOG_DEBUG("Mem alloc for write buf failed!!\n");
        /* return; */
    }
    else
    {
        LOG_DEBUG("Mux support ?(1/0)"); fflush(stdout);
        scanf("%d", &read_val);
        if (1U == read_val)
        {
            BT_mem_copy(&appl_write_buf[L2CAP_HDR_LEN + 1U], media_header, 12U );
        }
        else
        {
            BT_mem_copy( appl_write_buf + L2CAP_HDR_LEN, media_header, 12U );
        }

        LOG_DEBUG("Starting AVDTP Media write..\n");
        retval = BT_avdtp_media_write
                 (
                     &appl_avdtp_handle[index],
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
}
#endif /* A2DP_SOURCE */

void appl_avdtp_reconfigure ( void )
{
    UCHAR index, remote_seid;
    API_RESULT retval;
    AVDTP_SEP_CONF sep_conf;
    int seid;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    index = 0U;
    remote_seid = 0U;
    BT_mem_set(&sep_conf, 0, sizeof(AVDTP_SEP_CONF));

    appl_avdtp_choose_handle(&index);

#if 0
    sep_conf.service_flag = 0x01;
    sep_conf.recovery_cap.recovery_type = 0x00;
    sep_conf.recovery_cap.mrws = 0x00;
    sep_conf.recovery_cap.mnmp = 0x00;
    sep_conf.rohc_conf = 0x00;
#endif /* 0 */

    sep_conf.service_flag = 0x00U;
    sep_conf.codec_cap.media_type = AVDTP_MEDIA_AUDIO;
    sep_conf.codec_cap.codec_type = AVDTP_CODEC_AUDIO_SBC;
    sep_conf.codec_cap.codec_ie = appl_avdtp_codec_ie;
    sep_conf.codec_cap.codec_ie_len = 4U;

    LOG_DEBUG("Enter Remote SEID: "); fflush(stdout);
    scanf("%d",&seid);

    remote_seid = (UCHAR)seid;
    AVDTP_SET_HANDLE_REMOTE_SEID(appl_avdtp_handle[index], remote_seid);

    LOG_DEBUG("\n");
    LOG_DEBUG("Starting AVDTP Reconfiguration ... "); fflush(stdout);
    retval = BT_avdtp_reconfigure_req(&appl_avdtp_handle[index], &sep_conf);

    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Error Code = 0x%04x\n", retval);
    }
    else
    {
        LOG_DEBUG("Successfully Started.\n");
    }
}
#endif /* AVDTP */

