
/**
 *  \file avrcp_al_api.c
 *
 *  This source code implements hight level AVRCP APIs,
 *  using basic AVRCP exposed interfaces.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "bt_pal_avrcp_al_internal.h"

#if ((defined AVRCP_TG) || (defined AVRCP_CT))

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */
/** Global Mutex for AVRCP module */
BT_DEFINE_MUTEX (avrcp_al_mutex)

/* --------------------------------------------- Static Global Variables */
/**
 *  This is the callback function used by AVRCP to notify application
 *  of AVRCP synchronous and asynchronous events.
 */
AVRCP_AL_EVENT_NTF_CB al_event_ntf_cb;

UCHAR  avrcp_al_init_state = AVRCP_AL_STATE_INVALID;

/* --------------------------------------------- Functions */

API_RESULT BT_avrcp_al_init (void)
{
    API_RESULT retval;

    retval = API_SUCCESS;

    if (AVRCP_AL_STATE_INVALID == avrcp_al_init_state)
    {
        /* Initialize Mutex for AVRCP Abstraction Module */
        BT_MUTEX_INIT (avrcp_al_mutex, AVRCP);

        avrcp_al_init_state = AVRCP_AL_STATE_INITIALIZED;
    }

    /* Lock */
    avrcp_al_lock();

    retval = BT_avrcp_init();
    if (API_SUCCESS != retval)
    {
        AVRCP_AL_ERR(
            "[AVRCP_AL]: Failed BT_avrcp_init\n");
    }

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}

API_RESULT BT_avrcp_al_shutdown (void)
{
    API_RESULT retval;

    retval = API_SUCCESS;

    avrcp_al_lock();

    retval = BT_avrcp_shutdown();
    if (API_SUCCESS != retval)
    {
        AVRCP_AL_ERR(
            "[AVRCP_AL]: Failed BT_avrcp_shutdown\n");
    }

    avrcp_al_unlock();

    return retval;
}

API_RESULT BT_avrcp_al_start
           (
               /* OUT */ AVRCP_HANDLE *handle,
               /* IN  */ AVRCP_AL_EVENT_NTF_CB al_nty_cb
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;

    /* Validate input params */
    if ((NULL == handle) ||
        (NULL == al_nty_cb))
    {
        AVRCP_AL_ERR(
        "[AVRCP_AL] Incorrect Parameters for AVRCP AL Start\n");

        return AVRCP_INVALID_PARAMETERS;
    }

    avrcp_al_lock();

    al_event_ntf_cb = al_nty_cb;

    retval = BT_avrcp_start
             (
                 handle,
                 avrcp_al_ntf_cb
             );
    if (API_SUCCESS != retval)
    {
        AVRCP_AL_ERR(
        "[AVRCP_AL] Failed BT_avrcp_start\n");
    }

    avrcp_al_unlock();

    return retval;
}

API_RESULT BT_avrcp_al_stop
           (
               /* IN */ AVRCP_HANDLE *handle
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;

    avrcp_al_lock();

    retval = BT_avrcp_stop (handle);
    if (API_SUCCESS != retval)
    {
        AVRCP_AL_ERR(
        "[AVRCP_AL] Failed BT_avrcp_stop\n");
    }

    avrcp_al_unlock();

    return retval;
}


/**
 *  \fn BT_avrcp_al_send_info_cmd
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param info_opcode
 *
 *  \return None
 */
API_RESULT BT_avrcp_al_send_info_cmd
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          sub_unit
           )
{
    API_RESULT  retval;
    UCHAR     * data;
    UINT16      datalen;
    UINT16      marker;

    retval = API_FAILURE;

    /* Lock */
    avrcp_al_lock();

    /*
     * Allocate Memory as required for command.
     * This should also include space for AVCTP protocol header
     */
    datalen = AVRCP_AL_FIXED_CMD_LEN_UNIT_INFO + AVCTP_HDR_LEN;
    data = BT_alloc_mem (datalen);
    if (NULL == data)
    {
        /* Unlock */
        avrcp_al_unlock();

        /* Log Error */
        return retval;
    }


    /* Update marker with the header length */
    marker = AVCTP_HDR_LEN + AVRCP_COMMAND_HDR_LEN;

    /* Check for Unit-Info or Subunit-Info */
    if (AVRCP_OPCODE_SUBUNIT_INFO == sub_unit)
    {
        /* Update SubUnit Header */
        AVRCP_UPDATE_SUBUNIT_HDR
        (
            &data[AVCTP_HDR_LEN],
            AVRCP_COMMAND_TYPE_STATUS,
            AVRCP_SUBUNIT_TYPE_UNIT,
            0x07
        );

        /* Add additional info */
        data[marker] = 0x07;
        marker ++;
    }
    else
    {
        /* Update Unit Header */
        AVRCP_UPDATE_UNIT_HDR
        (
            &data[AVCTP_HDR_LEN],
            AVRCP_COMMAND_TYPE_STATUS,
            AVRCP_SUBUNIT_TYPE_UNIT,
            0x07
        );
    }

    /* Fill remaining bytes of data as required */
    do
    {
        data[marker] = 0xFF;
        marker ++;

    } while (marker < datalen);

    /* Send data over the control channel */
    retval = BT_avrcp_send_control_cmd
             (
                 avrcp_handle,
                 data,
                 datalen
             );

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}

/**
 *  \fn BT_avrcp_al_send_info_rsp
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param avctp_tl_msg_type
 *  \param sub_unit
 *  \param sub_unit_type
 *
 *  \return None
 */
API_RESULT BT_avrcp_al_send_info_rsp
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          avctp_tl_msg_type,
               /* IN */ UCHAR          sub_unit,
               /* IN */ UCHAR          sub_unit_type
           )
{
    API_RESULT  retval;
    UCHAR     * data;
    UINT16      datalen;
    UINT16      marker;

    retval = API_FAILURE;

    /* Lock */
    avrcp_al_lock();

    /*
     * Allocate Memory as required for command.
     * This should also include space for AVCTP protocol header
     */
    datalen = AVRCP_AL_FIXED_CMD_LEN_UNIT_INFO + AVCTP_HDR_LEN;
    data = BT_alloc_mem (datalen);
    if (NULL == data)
    {
        /* Unlock */
        avrcp_al_unlock();

        /* Log Error */
        return retval;
    }

    /* Add Transaction Label & AVRCP msg type (command or response) in AVCTP Header */
    data[0] = avctp_tl_msg_type;

    /* Update marker with the header length */
    marker = AVCTP_HDR_LEN + AVRCP_COMMAND_HDR_LEN;

    /* Check for Unit-Info or Subunit-Info */
    if (AVRCP_OPCODE_SUBUNIT_INFO == sub_unit)
    {
        /* Update SubUnit Header */
        AVRCP_UPDATE_SUBUNIT_HDR
        (
            &data[AVCTP_HDR_LEN],
            AVRCP_RESPONSE_TYPE_STABLE,
            AVRCP_SUBUNIT_TYPE_UNIT,
            0x07
        );

        /* Add additional info */
        data[marker++] = 0x07;

        /* Add subunit type info */
        data[marker++] =  sub_unit_type << 3;

        /* Filling remaining bytes with 0xFF */
        data[marker++] = 0xFF;
        data[marker++] = 0xFF;
        data[marker++] = 0xFF;
    }
    else
    {
        /* Update Unit Header */
        AVRCP_UPDATE_UNIT_HDR
        (
            &data[AVCTP_HDR_LEN],
            AVRCP_RESPONSE_TYPE_STABLE,
            AVRCP_SUBUNIT_TYPE_UNIT,
            0x07
        );

        /* Add additional info */
        data[marker++] = 0x07;

        /* Add Unit Type info */
        data[marker++] = sub_unit_type << 3;

        /* Company ID */
        data[marker++] = (UCHAR )(BT_SIG_REGISTERED_COMPANY_ID >> 16);

        data[marker++] = (UCHAR )(BT_SIG_REGISTERED_COMPANY_ID >> 8);

        data[marker++] = (UCHAR )(BT_SIG_REGISTERED_COMPANY_ID);

    }

    /* Send data over the control channel */
    retval = BT_avrcp_send_control_rsp
             (
                 avrcp_handle,
                 data,
                 datalen
             );

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}


/**
 *  \fn BT_avrcp_al_send_passthrough_cmd_rsp
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param avctp_tl_msg_type
 *  \param cmd_rsp_type
 *  \param operation_id
 *  \param state
 *  \param vunique
 *
 *  \return None
 */
API_RESULT BT_avrcp_al_send_passthrough_cmd_rsp
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          avctp_tl_msg_type,
               /* IN */ UCHAR          cmd_rsp_type,
               /* IN */ UCHAR          operation_id,
               /* IN */ UCHAR          state,
               /* IN */ UINT16         vu_op_id
           )
{
    API_RESULT  retval;
    UCHAR     * data;
    UINT16      datalen;

    retval = API_FAILURE;

    /* Lock */
    avrcp_al_lock();

    /* Check if Vendor-Unique Passthrough */
    if (AVRCP_OPERATION_ID_VENDOR_UNIQUE == operation_id)
    {
        /*
         * Allocate Memory as required for command.
         * This should also include space for AVCTP protocol header
         */
        datalen = AVRCP_AL_FIXED_CMD_LEN_VU_PASSTHROUGH + AVCTP_HDR_LEN;
        data = BT_alloc_mem (datalen);
        if (NULL == data)
        {
            /* Unlock */
            avrcp_al_unlock();

            /* Log Error */
            return retval;
        }

        /* Reset memory */
        BT_mem_set(data, 0x00, datalen);

        /*
         * For Control Commands, Transaction label & Message type in AVCTP Header are updated
         * internally in AVRCP layer. But for Response packet, these has to be provided by application.
         */
        if (AVRCP_COMMAND_TYPE_CONTROL != cmd_rsp_type)
        {
            /* Add Transaction Label & AVRCP msg type (command or response) in AVCTP Header */
            data[0] = avctp_tl_msg_type;
        }

        /* Yes, Form AVRCP Vendor-Unique Passthrough Command */
        AVRCP_UPDATE_PASSTHROUGH_VU_PKT
        (
            &data[AVCTP_HDR_LEN],
            cmd_rsp_type,
            AVRCP_SUBUNIT_TYPE_PANEL,
            0x00,
            state,
            vu_op_id
        );
    }
    else
    {
        /* Update the operation with state(press or release) */
        state = ((state << 7) | (operation_id & 0x7F));

        /*
         * Allocate Memory as required for command.
         * This should also include space for AVCTP protocol header
         */
        datalen = AVRCP_AL_FIXED_CMD_LEN_PASSTHROUGH + AVCTP_HDR_LEN;
        data = BT_alloc_mem (datalen);
        if (NULL == data)
        {
            /* Unlock */
            avrcp_al_unlock();

            /* Log Error */
            return retval;
        }

        /* Reset memory */
        BT_mem_set(data, 0x00, datalen);

        /*
         * For Control Commands, Transaction label & Message type in AVCTP Header are updated
         * internally in AVRCP layer. But for Response packet, these has to be provided by application.
         */
        if (AVRCP_COMMAND_TYPE_CONTROL != cmd_rsp_type)
        {
            /* Update Transaction Label & AVRCP msg type in AVCTP Header */
            data[0] = avctp_tl_msg_type;
        }

        /* No, Form AVRCP Passthrough Command */
        AVRCP_UPDATE_PASSTHROUGH_HDR
        (
            &data[AVCTP_HDR_LEN],
            cmd_rsp_type,
            AVRCP_SUBUNIT_TYPE_PANEL,
            0x00,
            state
        );
    }

    if (AVRCP_COMMAND_TYPE_CONTROL == cmd_rsp_type)
    {
        /* Send command data over the control channel */
        retval = BT_avrcp_send_control_cmd
                 (
                     avrcp_handle,
                     data,
                     datalen
                 );
    }
    else
    {
        /* Send response data over the control channel */
        retval = BT_avrcp_send_control_rsp
                 (
                     avrcp_handle,
                     data,
                     datalen
                 );
    }

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}

/**
 *  \fn BT_avrcp_al_set_absolute_volume
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param volume
 *
 *  \return None
 */
API_RESULT BT_avrcp_al_set_absolute_volume
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          volume
           )
{
    API_RESULT      retval;
    UCHAR           param;
    UINT16          paramlen;
    AVRCP_AL_VD_CMD_INFO pdu_info;

    retval = API_FAILURE;

    /* Lock */
    avrcp_al_lock();

    /* Update parameters */
    param = (volume & 0x7F);
    paramlen = 1;

    /* Update PDU parameters */
    pdu_info.tl              = 0x00;
    pdu_info.cmd_type        = AVRCP_COMMAND_TYPE_CONTROL;
    pdu_info.packet_type     = AVRCP_METADATA_PACKET_TYPE_SINGLE;
    pdu_info.pdu_id          = AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME;
    pdu_info.vd_cmd_data     = &param;
    pdu_info.vd_cmd_datalen  = paramlen;

    retval = BT_avrcp_al_send_metadata_pdu
             (
                 avrcp_handle,
                 &pdu_info,
                 0x00
             );

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}


/**
 *  \fn BT_avrcp_al_get_capabilities
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param cap_id
 *
 *  \return None
 */
API_RESULT BT_avrcp_al_get_capabilities
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          cap_id
           )
{
    API_RESULT          retval;
    UCHAR               param;
    UINT16              paramlen;
    AVRCP_AL_VD_CMD_INFO   vd_cmd_info;

    retval = API_FAILURE;

    /* Lock */
    avrcp_al_lock();

    /* Update parameters */
    param = cap_id;
    paramlen = 1;

    vd_cmd_info.tl          = 0x00;
    vd_cmd_info.cmd_type    = AVRCP_COMMAND_TYPE_STATUS;
    vd_cmd_info.packet_type = AVRCP_METADATA_PACKET_TYPE_SINGLE;
    vd_cmd_info.pdu_id      = AVRCP_METADATA_PDU_ID_GET_CAPABILITY;
    vd_cmd_info.vd_cmd_data = &param;
    vd_cmd_info.vd_cmd_datalen = paramlen;

    retval = BT_avrcp_al_send_metadata_pdu
             (
                 avrcp_handle,
                 &vd_cmd_info,
                 0x00
             );

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}


/**
 *  \fn BT_avrcp_al_send_set_addressed_player_cmd
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param player_id
 *
 *  \return None
 */
API_RESULT BT_avrcp_al_send_set_addressed_player_cmd
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UINT16         player_id
           )
{
    API_RESULT          retval;
    UCHAR               param[2];
    UINT16              paramlen;
    AVRCP_AL_VD_CMD_INFO   vd_cmd_info;

    retval = API_FAILURE;

    /* Lock */
    avrcp_al_lock();

    /* Update parameters */
    avrcp_pack_2_byte_metadata_param
    (
        param,
        &player_id
    );

    paramlen = 2;

    vd_cmd_info.tl              = 0x00;
    vd_cmd_info.cmd_type        = AVRCP_COMMAND_TYPE_CONTROL;
    vd_cmd_info.packet_type     = AVRCP_METADATA_PACKET_TYPE_SINGLE;
    vd_cmd_info.pdu_id          = AVRCP_PDU_ID_SET_ADDRESSED_PLAYER;
    vd_cmd_info.vd_cmd_data     = param;
    vd_cmd_info.vd_cmd_datalen  = paramlen;

    retval = BT_avrcp_al_send_metadata_pdu
             (
                 avrcp_handle,
                 &vd_cmd_info,
                 0x00
             );

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}

API_RESULT BT_avrcp_al_send_set_addressed_player_rsp
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          tl,
               /* IN */ UCHAR          rsp_type,
               /* IN */ UCHAR          status
           )
{
    API_RESULT          retval;
    UCHAR               param;
    UINT16              paramlen;
    AVRCP_AL_VD_CMD_INFO   vd_cmd_info;

    /* Lock */
    avrcp_al_lock();

    retval      = API_SUCCESS;
    param       = status;
    paramlen    = 1;

    vd_cmd_info.tl              = tl;
    vd_cmd_info.cmd_type        = rsp_type;
    vd_cmd_info.packet_type     = AVRCP_METADATA_PACKET_TYPE_SINGLE;
    vd_cmd_info.pdu_id          = AVRCP_PDU_ID_SET_ADDRESSED_PLAYER;
    vd_cmd_info.vd_cmd_data     = &param;
    vd_cmd_info.vd_cmd_datalen  = paramlen;

    retval = BT_avrcp_al_send_metadata_pdu
             (
                 avrcp_handle,
                 &vd_cmd_info,
                 0x01
             );

    /* Unlock */
    avrcp_al_unlock();

    return retval;

}


API_RESULT BT_avrcp_al_send_metadata_pdu
           (
               /* IN */ AVRCP_HANDLE      * avrcp_handle,
               /* IN */ AVRCP_AL_VD_CMD_INFO * pdu_info,
               /* IN */ UCHAR               dir
           )
{
    API_RESULT      retval;
    UCHAR          *data;
    UINT16          datalen;
    UCHAR           packet_type;
    UCHAR           vd_pkt_hdr_len;
    static  UCHAR  *vd_data = NULL;
    static  UINT16  vd_datalen, vd_pkt_offset;
    UINT16          max_vd_pdu_paramlen, vd_pdu_paramlen;


    retval = API_SUCCESS;

    /* Validate input parameters */
    if (NULL == pdu_info)
    {
        /* Log error */

        return API_FAILURE;
    }

    vd_pkt_hdr_len = AVRCP_COMMAND_HDR_LEN + AVRCP_METADATA_PDU_HDR_LEN;
    max_vd_pdu_paramlen = (AVRCP_AL_MAX_AVC_PACKET_LEN - vd_pkt_hdr_len - AVCTP_HDR_LEN);

    /* Allocate buffer for Max. AV/C packet length */
    data = (UCHAR *) BT_alloc_mem (AVRCP_AL_MAX_AVC_PACKET_LEN);
    if (NULL == data)
    {
        return API_FAILURE;
    }

    /**
     * Send reject rsp, if Request Continue or Abort Continue is received before
     * any command with start or single response packet_type from the application.
     */
    if (((NULL == vd_data) ||
         (0x00 == vd_datalen)) &&
        ((AVRCP_METADATA_PACKET_TYPE_CONTINUE == pdu_info->packet_type) ||
         (AVRCP_METADATA_PACKET_TYPE_END == pdu_info->packet_type)))
    {
        /* Form AVRCP Vendor Dependant Command header */
        AVRCP_UPDATE_VD_METADATA_HDR
        (
            &data[AVCTP_HDR_LEN],
            AVRCP_RESPONSE_TYPE_REJECTED,
            AVRCP_SUBUNIT_TYPE_PANEL,
            0X00
        );

        pdu_info->vd_cmd_datalen = 0x01;

        /* Form AVRCP Vendor Dependant PDU header */
        AVRCP_UPDATE_METADATA_INFO
        (
            &data[AVCTP_HDR_LEN],
            pdu_info->pdu_id,
            pdu_info->packet_type,
            pdu_info->vd_cmd_datalen
        );

        if (AVRCP_METADATA_PACKET_TYPE_CONTINUE == pdu_info->packet_type)
        {
            /* PDU ID */
            data[AVCTP_HDR_LEN + 6] = AVRCP_METADATA_PDU_ID_REQUEST_CONTINUING_RESPONSE;
        }
        else
        {
            /* PDU ID */
            data[AVCTP_HDR_LEN + 6] = AVRCP_METADATA_PDU_ID_ABORT_CONTINUING_RESPONSE;
        }

        /* Update the packet type and vd_param lenght */
        data[AVCTP_HDR_LEN + 7] = AVRCP_METADATA_PACKET_TYPE_SINGLE;

        data[AVCTP_HDR_LEN + vd_pkt_hdr_len] = AVRCP_METADATA_ERROR_INVALID_COMMAND;

        datalen = AVCTP_HDR_LEN + vd_pkt_hdr_len + 1;

        goto SEND_CMD_RSP;
    }

    if ((AVRCP_METADATA_PACKET_TYPE_SINGLE == pdu_info->packet_type) ||
        (AVRCP_METADATA_PACKET_TYPE_START  == pdu_info->packet_type))
    {
        /**
         * Check if unfinished Vendor Dependent PDU command/response is
         * pending to send.
         */

        if (NULL != vd_data)
        {
            /**
             * Yes, then free the memory & reset the parameters. Process new VD PDU packet.
             */

            BT_free_mem(vd_data);
            vd_data = NULL;
            vd_datalen = 0;
            vd_pkt_offset = 0;
        }

        if(pdu_info->vd_cmd_datalen > (AVRCP_AL_MAX_AVC_PACKET_LEN - vd_pkt_hdr_len - AVCTP_HDR_LEN))
        {
            /**
             * Allocating buffer for complete VD PDU packet.
             */
            vd_datalen = AVCTP_HDR_LEN + vd_pkt_hdr_len + pdu_info->vd_cmd_datalen;
            vd_data = (UCHAR *) BT_alloc_mem (vd_datalen);
            if (NULL == vd_data)
            {
                /* data is not NULL. Checked after allocation */
                BT_free_mem(data);

                return API_FAILURE;
            }

            if (NULL != pdu_info->vd_cmd_data)
            {
                /* Copy param info to allocated buffer */
                BT_mem_copy(&vd_data[AVCTP_HDR_LEN + vd_pkt_hdr_len], pdu_info->vd_cmd_data, pdu_info->vd_cmd_datalen);
            }

            /* Update the vd packet type */
            packet_type = AVRCP_METADATA_PACKET_TYPE_START;

            /* Form AVRCP Vendor Dependant Command header */
            AVRCP_UPDATE_VD_METADATA_HDR
            (
                &vd_data[AVCTP_HDR_LEN],
                pdu_info->cmd_type,
                AVRCP_SUBUNIT_TYPE_PANEL,
                0X00
            );

            /* Form AVRCP Vendor Dependant PDU header */
            AVRCP_UPDATE_METADATA_INFO
            (
                &vd_data[AVCTP_HDR_LEN],
                pdu_info->pdu_id,
                packet_type,
                max_vd_pdu_paramlen
            );

            datalen = AVCTP_HDR_LEN + vd_pkt_hdr_len + max_vd_pdu_paramlen;
            vd_pkt_offset = datalen;

            /* Copy the data */
            BT_mem_copy
            (
                &data[AVCTP_HDR_LEN],
                &vd_data[AVCTP_HDR_LEN],
                (datalen - AVCTP_HDR_LEN)
            );
        }
        else
        {
            /* Form AVRCP Vendor Dependant Command header */
            AVRCP_UPDATE_VD_METADATA_HDR
            (
                &data[AVCTP_HDR_LEN],
                pdu_info->cmd_type,
                AVRCP_SUBUNIT_TYPE_PANEL,
                0X00
            );

            /* Form AVRCP Vendor Dependant PDU header */
            AVRCP_UPDATE_METADATA_INFO
            (
                &data[AVCTP_HDR_LEN],
                pdu_info->pdu_id,
                pdu_info->packet_type,
                pdu_info->vd_cmd_datalen
            );

            datalen = AVCTP_HDR_LEN + vd_pkt_hdr_len + pdu_info->vd_cmd_datalen;

            /* Copy the PDU parameters */
            if (NULL != pdu_info->vd_cmd_data)
            {
                BT_mem_copy
                (
                    &data[AVCTP_HDR_LEN + vd_pkt_hdr_len],
                    pdu_info->vd_cmd_data,
                    pdu_info->vd_cmd_datalen
                );
            }
        }
    }
    else if (AVRCP_METADATA_PACKET_TYPE_CONTINUE == pdu_info->packet_type)
    {
        /* Validate PDU id for continue response */
        if (pdu_info->pdu_id != vd_data[AVCTP_HDR_LEN + 6])
        {
            /* PDU ID does not match for continue response */

            /* TODO: Send error and return */
        }

        /* Copy the vd packet hdr  */
        BT_mem_copy(&data[AVCTP_HDR_LEN], &vd_data[AVCTP_HDR_LEN], vd_pkt_hdr_len);

        if ((vd_datalen - vd_pkt_offset) > max_vd_pdu_paramlen)
        {
            /*
             * send continue packet type
             */

            /* Update the packet type and vd_param lenght */
            data[AVCTP_HDR_LEN + 7] = AVRCP_METADATA_PACKET_TYPE_CONTINUE;

            /* Update the param length */
            avrcp_pack_2_byte_metadata_param
            (
                &data [AVCTP_HDR_LEN + 8],
                &max_vd_pdu_paramlen
            );

            /* Copy the data */
            BT_mem_copy(&data[AVCTP_HDR_LEN + vd_pkt_hdr_len], &vd_data[vd_pkt_offset], max_vd_pdu_paramlen);

            datalen = AVCTP_HDR_LEN + vd_pkt_hdr_len + max_vd_pdu_paramlen;
            vd_pkt_offset += max_vd_pdu_paramlen;
        }
        else
        {
            UINT16 i;
            /**
             * send end packet type
             */

            /* Update the packet type and vd_param lenght */
            data[AVCTP_HDR_LEN + 7] = AVRCP_METADATA_PACKET_TYPE_END;

            /* parameter length */
            vd_pdu_paramlen = vd_datalen - vd_pkt_offset;

            /* Update the param length */
            avrcp_pack_2_byte_metadata_param
            (
                &data [AVCTP_HDR_LEN + 8],
                &vd_pdu_paramlen
            );

            /* Copy the data */
            BT_mem_copy(&data[AVCTP_HDR_LEN + vd_pkt_hdr_len], &vd_data[vd_pkt_offset], vd_pdu_paramlen);

            datalen = AVCTP_HDR_LEN + vd_pkt_hdr_len + vd_pdu_paramlen;
            vd_pkt_offset += vd_pdu_paramlen;

            for (i = 0; i < vd_pdu_paramlen; i++)
            {
                AVRCP_AL_INF (
                "%x", data[AVCTP_HDR_LEN + vd_pkt_hdr_len + i]);
            }

            /**
             * Complete vd pdu data sent. Free the vd_data memory and reset the
             * parameters.
             */

            BT_free_mem(vd_data);
            vd_data = NULL;
            vd_datalen = 0;
            vd_pkt_offset = 0;
        }
    }
    else
    {
        /* ========== PDU ID -> Abort continue respone received ===== */

        /* Copy the vd packet hdr  */
        BT_mem_copy(&data[AVCTP_HDR_LEN], &vd_data[AVCTP_HDR_LEN], vd_pkt_hdr_len);

        data[AVCTP_HDR_LEN] = pdu_info->cmd_type;

        /* PDU ID */
        data[AVCTP_HDR_LEN + 6] = AVRCP_METADATA_PDU_ID_ABORT_CONTINUING_RESPONSE;

        /* Update the packet type and vd_param lenght */
        data[AVCTP_HDR_LEN + 7] = AVRCP_METADATA_PACKET_TYPE_SINGLE;

        /* parameter length */
        vd_pdu_paramlen = 0x00;

        /* Update the param length */
        avrcp_pack_2_byte_metadata_param
        (
            &data [AVCTP_HDR_LEN + 8],
            &vd_pdu_paramlen
        );

        /* Data length */
        datalen = AVCTP_HDR_LEN + vd_pkt_hdr_len;

        /* Free the Memory */
        BT_free_mem(vd_data);
        vd_data = NULL;
        vd_datalen = 0;
        vd_pkt_offset = 0;
    }

SEND_CMD_RSP:

    /* Send command or response */
    if (0x00 == dir)
    {
        /* Send data over the control channel */
        retval = BT_avrcp_send_control_cmd
                 (
                     avrcp_handle,
                     data,
                     datalen
                 );
    }
    else
    {
        /* Set AVCTP Header */
        data[0] = (pdu_info->tl | AVCTP_CTR_MSG_RESPONSE);

        /* Send the metadata rsp */
        retval = BT_avrcp_send_control_rsp
                 (
                     avrcp_handle,
                     data,
                     datalen
                 );
    }

    return retval;
}


/**
 *  \fn BT_avrcp_al_get_play_status
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *
 *  \return None
 */
API_RESULT BT_avrcp_al_get_play_status
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle
           )
{
    AVRCP_AL_VD_CMD_INFO   pdu_info;
    API_RESULT          retval;

    retval = API_FAILURE;

    /* Lock */
    avrcp_al_lock();

    /* Update PDU parameters */
    pdu_info.tl              = 0x00;
    pdu_info.cmd_type        = AVRCP_COMMAND_TYPE_STATUS;
    pdu_info.packet_type     = AVRCP_METADATA_PACKET_TYPE_SINGLE;
    pdu_info.pdu_id          = AVRCP_METADATA_PDU_ID_GET_PLAY_STATUS;
    pdu_info.vd_cmd_data     = NULL;
    pdu_info.vd_cmd_datalen  = 0;

    retval = BT_avrcp_al_send_metadata_pdu
             (
                 avrcp_handle,
                 &pdu_info,
                 0x00
             );

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}


/**
 *  \fn BT_avrcp_al_register_notification
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param event_id
 *  \param interval
 *
 *  \return None
 */
API_RESULT BT_avrcp_al_register_notification
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          event_id,
               /* IN */ UINT32         interval
           )
{
    API_RESULT          retval;
    UCHAR               param[5];
    UINT16              paramlen;
    AVRCP_AL_VD_CMD_INFO   pdu_info;

    retval = API_FAILURE;

    /* Lock */
    avrcp_al_lock();

    /* Update parameters */
    param[0] = event_id;

    avrcp_pack_4_byte_metadata_param
    (
        &param[1],
        &interval
    );

    paramlen = 5;

    /* Update PDU parameters */
    pdu_info.tl              = 0x00;
    pdu_info.cmd_type        = AVRCP_COMMAND_TYPE_NOTIFY;
    pdu_info.packet_type     = AVRCP_METADATA_PACKET_TYPE_SINGLE;
    pdu_info.pdu_id          = AVRCP_METADATA_PDU_ID_REGISTER_NOTIFICATION;
    pdu_info.vd_cmd_data     = param;
    pdu_info.vd_cmd_datalen  = paramlen;

    retval = BT_avrcp_al_send_metadata_pdu
             (
                 avrcp_handle,
                 &pdu_info,
                 0x00
             );

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}

/**
 *  \fn BT_avrcp_al_send_continuation_pdu
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param pdu_id
 *  \param param
 *
 *  \return None
 */

API_RESULT BT_avrcp_al_send_continuation_pdu
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          pdu_id,
               /* IN */ UCHAR          param
           )
{
    API_RESULT          retval;
    UINT16              paramlen;
    AVRCP_AL_VD_CMD_INFO   pdu_info;

    retval = API_FAILURE;

    /* Lock */
    avrcp_al_lock();

    /* Update parameters */
    paramlen = 1;

    /* Update PDU parameters */
    pdu_info.tl              = 0x00;
    pdu_info.cmd_type        = AVRCP_COMMAND_TYPE_CONTROL;
    pdu_info.packet_type     = AVRCP_METADATA_PACKET_TYPE_SINGLE;
    pdu_info.pdu_id          = pdu_id;
    pdu_info.vd_cmd_data     = &param;
    pdu_info.vd_cmd_datalen  = paramlen;

    retval = BT_avrcp_al_send_metadata_pdu
             (
                 avrcp_handle,
                 &pdu_info,
                 0x00
             );

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}


/***************************** AVRCP BROWSING CHANNEL APIs ******/
/**
 *  \fn BT_avrcp_al_send_browsing_pdu
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param pdu_id
 *  \param data
 *  \param datalen
 *  \param dir
 *
 *  \return None
 */

API_RESULT BT_avrcp_al_send_browsing_pdu
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UCHAR          pdu_id,
               /* IN */ UCHAR        * data,
               /* IN */ UINT16         datalen,
               /* IN */ UCHAR          dir
           )
{
    API_RESULT      retval;
    UCHAR         * param;
    UINT16          paramlen;
    UINT16          offset;

    retval = API_FAILURE;

    /*
     * Allocate Memory as required for command.
     * This should also include space for AVCTP protocol header
     */
    paramlen = AVCTP_HDR_LEN + AVRCP_COMMAND_HDR_LEN + datalen;
    param    = BT_alloc_mem (paramlen);
    if (NULL == param)
    {
        /* Log Error */
        return retval;
    }

    /* Reset memory */
    BT_mem_set(param, 0x00, paramlen);

    /* Start packing the data */
    offset = AVCTP_HDR_LEN;

    /* Set the PDU ID */
    param[offset++] = pdu_id;

    /* Set the paramter length */
    avrcp_pack_2_byte_metadata_param
    (
        (param + offset),
        &datalen
    );

    offset += 2;

    /* TODO: Check for Max data length */

    /* Copy the data */
    BT_mem_copy ((param + offset), data, datalen);

    if (0x00 == dir)
    {
        /* Send it to the peer device */
        retval = BT_avrcp_send_cmd
                 (
                     avrcp_handle,
                     param,
                     paramlen,
                     AVCTP_CHANNEL_TYPE_BROWSING
                 );
    }
    else
    {
        /* Send it to the peer device */
        retval = BT_avrcp_send_rsp
                 (
                     avrcp_handle,
                     param,
                     paramlen,
                     AVCTP_CHANNEL_TYPE_BROWSING
                 );
    }

    return retval;
}

/**
 *  \fn BT_avrcp_al_send_browsing_cmd_rsp
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param brow_rsp_info
 *
 *  \return None
 */
API_RESULT BT_avrcp_al_send_browsing_cmd_rsp
           (
               /* IN */ AVRCP_HANDLE         * avrcp_handle,
               /* IN */ AVRCP_AL_BROW_RSP_INFO  * brow_rsp_info
           )
{
    API_RESULT      retval;
    UCHAR         * param;
    UINT16          paramlen;
    UINT16          offset;

    retval = API_FAILURE;

    if (NULL == brow_rsp_info)
    {
        /* return error */
        return retval;
    }

    /* Lock */
    avrcp_al_lock();

    /*
     * Allocate Memory as required for command.
     * This should also include space for AVCTP protocol header
     */
    paramlen = AVCTP_HDR_LEN + AVRCP_COMMAND_HDR_LEN + brow_rsp_info->param_info_len;
    param    = BT_alloc_mem (paramlen);
    if (NULL == param)
    {
        /* Unlock */
        avrcp_al_unlock();

        /* Log Error */
        return retval;
    }

    /* Reset memory */
    BT_mem_set(param, 0x00, paramlen);

    /* Start packing the data */
    offset = AVCTP_HDR_LEN;

    /* Set the PDU ID */
    param[offset++] = brow_rsp_info->pdu_id;

    /* Set the paramter length */
    avrcp_pack_2_byte_metadata_param
    (
        (param + offset),
        &brow_rsp_info->param_info_len
    );

    offset += 2;

    /* TODO: Check for Max data length */

    /* Copy the data */
    BT_mem_copy ((param + offset), brow_rsp_info->param_info, brow_rsp_info->param_info_len);

    /* Set AVCTP Header */
    param[0] = (brow_rsp_info->tl | AVCTP_CTR_MSG_RESPONSE);

    /* Send it to the peer device */
    retval = BT_avrcp_send_rsp
             (
                 avrcp_handle,
                 param,
                 paramlen,
                 AVCTP_CHANNEL_TYPE_BROWSING
             );

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}


/**
 *  \fn BT_avrcp_al_set_browsed_player
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param player_id
 *
 *  \return None
 */
API_RESULT BT_avrcp_al_set_browsed_player
           (
               /* IN */ AVRCP_HANDLE * avrcp_handle,
               /* IN */ UINT16         player_id
           )
{
    API_RESULT  retval;
    UCHAR       param[2];
    UINT16      paramlen;

    retval = API_FAILURE;

    /* Lock */
    avrcp_al_lock();

    /* player_id length */
    paramlen = 2;

    avrcp_pack_2_byte_metadata_param
    (
        param,
        &player_id
    );

    retval = BT_avrcp_al_send_browsing_pdu
             (
                 avrcp_handle,
                 AVRCP_PDU_ID_SET_BROWSED_PLAYER,
                 param,
                 paramlen,
                 0x00
             );

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}

/**
 *  \fn BT_avrcp_al_get_items_attribute
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param folder_items_params
 *
 *  \return None
 */
API_RESULT BT_avrcp_al_get_items_attribute
           (
               /* IN */ AVRCP_HANDLE                         * avrcp_handle,
               /* IN */ AVRCP_AL_GET_ITEM_ATTRIBUTE_PARAMS   * item_attr_params
           )
{
    API_RESULT  retval;
    UCHAR     * param, attr_count;
    UINT16      paramlen;
    UINT16      i, offset;

    retval = API_SUCCESS;

    /* Validate the params */
    if ((NULL == avrcp_handle) ||
        (NULL == item_attr_params))
    {
        /* Log the Error */

        return API_FAILURE;
    }

    /* Lock */
    avrcp_al_lock();

    attr_count = item_attr_params->attr_count;

    /* Scope + UID + UID Counter + Num Attr Ids + Attributes size  */
    paramlen = 1 + 8 + 2 + 1 + 4 * attr_count;

    /* Allocate memory for param */
    param = BT_alloc_mem (paramlen);
    if (NULL == param)
    {
        /* Unlock */
        avrcp_al_unlock();

        return API_FAILURE;
    }

    offset = 0;

    /* Pack the Scope */
    param [offset] = item_attr_params->scope;
    offset += 1;

    /* Pack the MSB of uid */
    avrcp_pack_4_byte_metadata_param
    (
        &param[offset],
        &item_attr_params->uid_msb
    );
    offset += 4;

    /* Pack the LSB of uid */
    avrcp_pack_4_byte_metadata_param
    (
        &param[offset],
        &item_attr_params->uid_lsb
    );
    offset += 4;

    /* Pack the UID counter */
    avrcp_pack_2_byte_metadata_param
    (
        &param[offset],
        &item_attr_params->uid_counter
    );
    offset += 2;

    /* Pack number of attibutes */
    param[offset] = attr_count;
    offset += 1;

    /*  Pack the attribute ID's */
    for (i = 0; i < attr_count; i++)
    {
        avrcp_pack_4_byte_metadata_param
        (
            &param[offset],
            &item_attr_params->attr_list[i]
        );

        offset += 4;
    }

    /* Send the command */
    retval = BT_avrcp_al_send_browsing_pdu
             (
                 avrcp_handle,
                 AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES,
                 param,
                 paramlen,
                 0x00
             );

    /* Free the param memory */
    BT_free_mem (param);

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}


/**
 *  \fn BT_avrcp_al_get_folder_items
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param folder_items_params
 *
 *  \return None
 */
API_RESULT BT_avrcp_al_get_folder_items
           (
               /* IN */ AVRCP_HANDLE                    * avrcp_handle,
               /* IN */ AVRCP_AL_GET_FOLDER_ITEMS_PARAM * folder_items_params
           )
{
    API_RESULT  retval;
    UCHAR     * param, attr_count;
    UINT16      paramlen;
    UINT16      i, offset;


    /* Validate input params */
    if ((NULL == avrcp_handle) ||
        (NULL == folder_items_params))
    {
        /* Log Error */

        return API_FAILURE;
    }

    /* Lock */
    avrcp_al_lock();

    /* scope + start_item + end_item + attr_count */
    paramlen = 1 + 4 + 4 + 1;

    attr_count = folder_items_params->attr_count;

    if ((0x00 != attr_count) &&
        (0xFF != attr_count))
    {
        paramlen += (attr_count * 4);
    }

    /* Allocate memory for parameter */
    param = BT_alloc_mem (paramlen);
    if (NULL == param)
    {
        /* Unlock */
        avrcp_al_unlock();

        return API_FAILURE;
    }

    offset = 0;

    /* Pack Scope  */
    param[offset++] = folder_items_params->scope;


    /* Pack Start Item  */
    avrcp_pack_4_byte_metadata_param
    (
        &param[offset],
        &folder_items_params->start_item
    );
    offset += 4;

    /* Pack End Item  */
    avrcp_pack_4_byte_metadata_param
    (
        &param[offset],
        &folder_items_params->end_item
    );

    offset += 4;

    /* Attribute count */
    param[offset++] = attr_count;

    /* Pack attributes */
    if ((0x00 != attr_count) &&
        (0xFF != attr_count))
    {
        for (i = 0; i < attr_count; i++)
        {
            avrcp_pack_4_byte_metadata_param
            (
                &param[offset],
                &folder_items_params->attr_list[i]
            );

            offset += 4;
        }
    }

    /* Send the command */
    retval = BT_avrcp_al_send_browsing_pdu
             (
                 avrcp_handle,
                 AVRCP_PDU_ID_GET_FOLDER_ITEMS,
                 param,
                 paramlen,
                 0x00
             );

    /* Free the param */
    BT_free_mem (param);

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}


/**
 *  \fn BT_avrcp_al_change_path
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param uid_counter
 *  \param path_dir
 *  \param folder_uid
 *
 *  \return None
 */
API_RESULT BT_avrcp_al_change_path
           (
               /* IN */ AVRCP_HANDLE        * avrcp_handle,
               /* IN */ UINT16                uid_counter,
               /* IN */ UCHAR                 path_dir,
               /* IN */ UCHAR               * folder_uid
           )
{
    API_RESULT  retval;
    UCHAR     * param;
    UINT16      paramlen, offset;

    retval = API_SUCCESS;

    if (NULL == avrcp_handle)
    {
        /* Log error */

        return API_FAILURE;
    }

    /* Lock */
    avrcp_al_lock();

    /* uid_counter + path dir + folder_uid */
    paramlen = 2 + 1 + 8;

    /* Allocate memory for parameter */
    param = BT_alloc_mem (paramlen);
    if (NULL == param)
    {
        /* Unlock */
        avrcp_al_unlock();

        return API_FAILURE;
    }

    offset = 0;

    /* Pack char counter */
    avrcp_pack_2_byte_metadata_param
    (
        &param[offset],
        &uid_counter
    );

    offset += 2;

    /* path direction  */
    param[offset++] = path_dir;

    /* Copy search string */
    BT_mem_copy (&param[offset], folder_uid, 8);

    /* Send the command */
    retval = BT_avrcp_al_send_browsing_pdu
             (
                 avrcp_handle,
                 AVRCP_PDU_ID_CHANGE_PATH,
                 param,
                 paramlen,
                 0x00
             );

    /* Free the param */
    BT_free_mem (param);

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}


/**
 *  \fn BT_avrcp_al_search
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param char_set
 *  \param search_str
 *  \param search_str_len
 *
 *  \return None
 */
API_RESULT BT_avrcp_al_search
           (
               /* IN */ AVRCP_HANDLE        * avrcp_handle,
               /* IN */ UINT16                char_set,
               /* IN */ UCHAR               * search_str,
               /* IN */ UINT16                search_str_len
           )
{
    API_RESULT      retval;
    UCHAR         * param;
    UINT16          paramlen, offset;

    retval = API_SUCCESS;

    /* Validate parameters */
    if ((NULL == avrcp_handle) ||
        (NULL == search_str))
    {
        /* Log error */

        return API_FAILURE;
    }

    /* Lock */
    avrcp_al_lock();

    paramlen = 2 + 2 + search_str_len;

    /* Allocate memory for parameter */
    param = BT_alloc_mem (paramlen);
    if (NULL == param)
    {
        /* Unlock */
        avrcp_al_unlock();

        return API_FAILURE;
    }

    offset = 0;

    /* Pack char set */
    avrcp_pack_2_byte_metadata_param
    (
        &param[offset],
        &char_set
    );

    offset += 2;

    /* Search str length */
    avrcp_pack_2_byte_metadata_param
    (
        &param[offset],
        &search_str_len
    );

    offset += 2;

    /* Copy search string */
    BT_mem_copy (&param[offset], search_str, search_str_len);

    /* Send the command */
    retval = BT_avrcp_al_send_browsing_pdu
             (
                 avrcp_handle,
                 AVRCP_PDU_ID_SEARCH,
                 param,
                 paramlen,
                 0x00
             );

    /* Free the param */
    BT_free_mem (param);

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}

#ifdef AVRCP_GET_NUM_OF_ITEMS

/**
 *  \fn BT_avrcp_al_get_total_number_of_items
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param avrcp_handle
 *  \param scope
 *
 *  \return None
 */
API_RESULT BT_avrcp_al_get_total_number_of_items
           (
               /* IN */ AVRCP_HANDLE        * avrcp_handle,
               /* IN */ UCHAR                 scope
           )
{
    API_RESULT      retval;
    UCHAR         * param;
    UINT16          paramlen;

    retval = API_SUCCESS;

    if (NULL == avrcp_handle)
    {
        /* Log Error */

        return API_FAILURE;
    }

    /* Lock */
    avrcp_al_lock();

    paramlen = 1;
    param    = &scope;

    /* Send the command */
    retval = BT_avrcp_al_send_browsing_pdu
             (
                 avrcp_handle,
                 AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS,
                 param,
                 paramlen,
                 0x00
             );

    /* Unlock */
    avrcp_al_unlock();

    return retval;
}

#endif /* AVRCP_GET_NUM_OF_ITEMS */

#endif /* ((defined AVRCP_TG) || (defined AVRCP_CT)) */
