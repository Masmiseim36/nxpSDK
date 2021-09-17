
/**
 *  \file avrcp_al_internal.c
 *
 *  This source code implements callback function for handling incoming
 *  events and notifications from AVRCP Layer and Exacting event data and
 *  passing to the applicaion
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "bt_pal_avrcp_al_internal.h"

#if ((defined AVRCP_TG) || (defined AVRCP_CT))

/* --------------------------------------------- External Global Variables */
/* Global MAP MCE mutex variable */
BT_DEFINE_MUTEX_TYPE(extern, avrcp_al_mutex);

extern AVRCP_AL_EVENT_NTF_CB al_event_ntf_cb;

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */
/* Callback registered with AVRCP Profile */
API_RESULT avrcp_al_ntf_cb
           (
               /* IN */ UCHAR  event_type,
               /* IN */ UINT16 event_result,
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ void  * event_data,
               /* IN */ UINT16 event_datalen
           )
{
    API_RESULT          retval;
    void               *avrcp_al_event_data;
    UINT16              avrcp_al_event_datalen;
    AVRCP_AL_CT_CMD_INFO   avrcp_cmd_info;
    AVRCP_AL_CT_RSP_INFO   avrcp_rsp_info;
    AVRCP_AL_VD_CMD_INFO   avrcp_vd_cmd_info;
    AVRCP_AL_BROW_CMD_INFO avrcp_brow_cmd_info;
    AVRCP_AL_BROW_RSP_INFO avrcp_brow_rsp_info;


    retval = API_SUCCESS;
    avrcp_al_event_data = NULL;
    avrcp_al_event_datalen = 0;

    /* Lock */
    avrcp_al_lock();

    switch(event_type)
    {
    case AVRCP_CONNECT_IND:
    case AVRCP_CONNECT_CNF:         /* Fall Through */
    case AVRCP_DISCONNECT_IND:      /* Fall Through */
    case AVRCP_DISCONNECT_CNF:      /* Fall Through */
#ifdef AVRCP_1_4
    case AVRCP_BOW_CONNECT_IND:     /* Fall Through */
    case AVRCP_BOW_CONNECT_CNF:     /* Fall Through */
    case AVRCP_BOW_DISCONNECT_IND:  /* Fall Through */
    case AVRCP_BOW_DISCONNECT_CNF:  /* Fall Through */
#endif /* AVRCP_1_4 */

        avrcp_al_event_data     = event_data;
        avrcp_al_event_datalen  = event_datalen;
        break;

    case AVRCP_MESSAGE_SEND_CNF:

        /* Free Allocated Memory */
        if (event_data != NULL)
        {
            BT_free_mem (event_data);
        }
        break;

    case AVRCP_MESSAGE_IND:

        /* Extract AVRCP Control Command opcode */
        AVRCP_GET_MSG_OPCODE ((UCHAR *)event_data, avrcp_cmd_info.cmd_opcode);

        /* Transaction label */
        avrcp_cmd_info.cmd_tl = *(UCHAR *)event_data & 0xF0;

        avrcp_cmd_info.cmd_info = event_data;
        avrcp_cmd_info.cmd_info_len = event_datalen;

        avrcp_al_event_data     = &avrcp_cmd_info;
        avrcp_al_event_datalen  = sizeof(avrcp_cmd_info);
        break;

    case AVRCP_MESSAGE_CNF:

        /* Extract Control Response */
        AVRCP_GET_MSG_RESPONSE_TYPE ((UCHAR *)event_data, avrcp_rsp_info.rsp_type);

        AVRCP_GET_MSG_OPCODE ((UCHAR *)event_data, avrcp_rsp_info.cmd_opcode);

        avrcp_rsp_info.rsp_info = event_data;
        avrcp_rsp_info.rsp_info_len = event_datalen;

        avrcp_al_event_data = &avrcp_rsp_info;
        avrcp_al_event_datalen = sizeof(avrcp_rsp_info);
        break;

#ifdef AVRCP_1_3

    case AVRCP_METADATA_CMD: /* Fall through */
    case AVRCP_METADATA_INTERIM_RSP:
    case AVRCP_METADATA_RSP:

        avrcp_vd_cmd_info.tl = ((UCHAR *)event_data)[0] & 0xF0;

         /* Extract the cmd_type of the Metadata command */
        AVRCP_GET_MSG_COMMAND_TYPE
        (
           (UCHAR *)event_data,
            avrcp_vd_cmd_info.cmd_type
        );

        /* Extract the PDU ID */
        AVRCP_GET_METADATA_PDU_ID
        (
            (UCHAR *)event_data,
            &avrcp_vd_cmd_info.pdu_id
        );

        /* Extract the Packet Type */
        AVRCP_GET_METADATA_PACKET_TYPE
        (
            (UCHAR *)event_data,
            &avrcp_vd_cmd_info.packet_type
        );

        /* Extract the Parameter Length */
        AVRCP_GET_METADATA_PARAM_LEN
        (
            (UCHAR *)event_data,
            &avrcp_vd_cmd_info.param_len
        );

        avrcp_vd_cmd_info.vd_cmd_data    = event_data;
        avrcp_vd_cmd_info.vd_cmd_datalen = event_datalen;

        avrcp_al_event_data     = &avrcp_vd_cmd_info;
        avrcp_al_event_datalen  = sizeof(avrcp_vd_cmd_info);

        break;

#endif /* AVRCP_1_3 */

#ifdef AVRCP_1_4

    case AVRCP_BOW_MESSAGE_IND:

        /* Browsing cmd PDU ID */
        avrcp_brow_cmd_info.pdu_id  = ((UCHAR *)event_data)[1];

        /* Transaction label */
        avrcp_brow_cmd_info.tl       = ((UCHAR *)event_data)[0] & 0xF0;

        /* Sending complete data */
        avrcp_brow_cmd_info.brow_cmd_info = event_data;
        avrcp_brow_cmd_info.brow_cmd_info_len = event_datalen;

        avrcp_al_event_data     = &avrcp_brow_cmd_info;
        avrcp_al_event_datalen  = sizeof(avrcp_brow_cmd_info);

        break;

    case AVRCP_BOW_MESSAGE_RSP:

        /* Browsing rsp PDU ID */
        avrcp_brow_rsp_info.pdu_id  = ((UCHAR *)event_data)[1];

        /* Transaction label */
        avrcp_brow_rsp_info.tl       = ((UCHAR *)event_data)[0] & 0xF0;

        /* Sending complete data */
        avrcp_brow_rsp_info.param_info = event_data;
        avrcp_brow_rsp_info.param_info_len = event_datalen;

        avrcp_al_event_data     = &avrcp_brow_rsp_info;
        avrcp_al_event_datalen  = sizeof(avrcp_brow_rsp_info);

        break;

#endif /* AVRCP_1_4 */

    default:
        break;
    }

    /* Unlock */
    avrcp_al_unlock();

   retval =  al_event_ntf_cb
             (
                 avrcp_handle,
                 event_type,
                 event_result,
                 avrcp_al_event_data,
                 avrcp_al_event_datalen
             );

    return retval;
}

#endif /* ((defined AVRCP_TG) || (defined AVRCP_CT)) */
