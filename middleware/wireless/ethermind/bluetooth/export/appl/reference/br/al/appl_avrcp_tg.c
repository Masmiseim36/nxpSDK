
/**
 *  \file appl_avrcp_tg.c
 *
 *  Source file for AVRCP Target Source Command Line Test Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_avrcp.h"
#include "appl_avrcp_tg.h"

#include "appl_utils.h"

#ifdef AVRCP_TG
/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */

/* Main AVRCP Target Application Menu */
static const char avrcp_tg_main_menu[] = " \n\
--------------- AVRCP Target Menu ---------------------- \n\n\
  0.  Exit. \n\
  1.  Refresh. \n\
  \n\
  2.  Initialize Virtual Media Player \n\
  3.  Update Virtual Media Player Setting. \n\
  4.  Display  Virtual Media Player Setting. \n\
  \n\
 10.  AVCRP CoverArt Set number of wait stages. \n\
  \n\
 20.  AVCRP Generate Player Event. \n\
 \n\
 30.  Cover Art Transport Close. \n\
  \n\
Your Option -> ";

#ifdef AVRCP_1_4
static UCHAR   tg_additional_protocol_descriptor_list[] =
               {
                   0x35U, 0x12U, 0x35U, 0x10U, 0x35U, 0x06U,
                   0x19U, 0x01U, 0x00U, 0x09U, 0x00U, 0x1BU,
                   0x35U, 0x06U, 0x19U, 0x00U, 0x17U, 0x09U,
                   0x01U, 0x03U
               };
#endif /* AVRCP_1_4 */

#ifdef AVRCP_COVER_ART_RESPONDER

static UCHAR   avrcp16_tg_additional_protocol_desc_list[] =
               {
                   0x35U, 0x21U, 0x35U, 0x10U, 0x35U, 0x06U, 0x19U, 0x01U, 0x00U, 0x09U,
                   0x00U, 0x1BU, 0x35U, 0x06U, 0x19U, 0x00U, 0x17U, 0x09U, 0x01U, 0x04U,
                   0x35U, 0x0DU, 0x35U, 0x06U, 0x19U, 0x01U, 0x00U, 0x09U, (UCHAR)(COVER_ART_GOEP_L2CAP_PSM >> 8U),
                   (UCHAR)COVER_ART_GOEP_L2CAP_PSM, 0x35U, 0x03U, 0x19U, 0x00U, 0x08U
               };

static AVRCP_CA_HANDLE                     appl_avrcp_tg_ca_handles[AVRCP_CAR_NUM_ENTITIES];

static UCHAR                               obex_connect_state;

static UCHAR                               avrcp_tg_file_object[AVRCP_TG_MAX_FILE_OBJ_NAME_LEN];

/* Cover Art object exchange size */
static UINT16                              cover_art_xchg_size;
static UINT16                              sent;
static UINT16                              image_prop_obj_size;
static UINT32                              remaining;
static BT_fops_file_handle                 image_thmbnail_fp;
static UINT32                              image_thmbnail_file_size;
static BT_fops_file_handle                 image_fp;
static UINT32                              image_file_size;
#ifdef HAVE_OBJECT_DUMP
static BT_fops_file_handle                 ca_img_descriptor_fp;
static UCHAR                               cover_art_file_name[36U];
static UINT16                              img_descriptor_count;
#endif /* HAVE_OBJECT_DUMP */
static AVRCP_CA_IMAGE_DESCRIPTOR           image_params;
static UCHAR                               image_file_name[32U];
static UCHAR                               img_properties[APPL_MAX_CA_IMAGE_PROPERTIES_LEN];
static UCHAR                               appl_ca_get_img_reject_rsp;

static UCHAR wait_count;

#endif /* AVRCP_COVER_ART_RESPONDER */

/* ----------------------------------------- Functions */
void main_avrcp_tg_operations (void)
{
    int         choice, menu_choice;
#ifdef AVRCP_COVER_ART_RESPONDER
    API_RESULT  retval;
#endif /* AVRCP_COVER_ART_RESPONDER */
    UCHAR       index;

    index = 0x00U;

    BT_LOOP_FOREVER()
    {
        printf ("%s", avrcp_tg_main_menu); fflush(stdin);
        scanf("%d", &choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            /* Refresh */
            break;

        case 2:
            (BT_IGNORE_RETURN_VALUE) appl_mp_init ();
            break;

        case 3:
            appl_mp_update_media_player_setting ();
            break;

        case 4:
            appl_mp_display_media_player_setting ();
            break;

        case 10:
#ifdef AVRCP_COVER_ART_RESPONDER
            LOG_DEBUG("Enter number of wait stages: ");
            scanf("%d", &choice);
            wait_count = (UCHAR )choice;
#endif /* AVRCP_COVER_ART_RESPONDER */
            break;

        case 20:
            appl_avrcp_tg_notify_change_rsp(0x00U);
            break;

        case 30:
            appl_avrcp_choose_handle(&index);

#ifdef AVRCP_COVER_ART_RESPONDER
            LOG_DEBUG ("Sending AVRCP_TG Cover Art Transport Close..\n");
            retval = BT_avrcp_car_transport_close
                     (
                         &appl_avrcp_tg_ca_handles[index]
                     );
            LOG_DEBUG ("Retval - 0x%04X\n", retval);
#else
            LOG_DEBUG ("AVRCP_COVER_ART_RESPONDER flag is not defined\n");
#endif /* AVRCP_COVER_ART_RESPONDER */

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

void appl_avrcp_tg_init (void)
{
    API_RESULT retval;
    UCHAR      i;

    retval = API_SUCCESS;
    i      = 0U;

#ifdef AVRCP_COVER_ART_RESPONDER
    LOG_DEBUG ("Initializing AVRCP Cover Art Responder...\n");
    retval = BT_avrcp_car_init();
    LOG_DEBUG ("Retval - 0x%04X\n", retval);

    for (i = 0U; i < AVRCP_CAR_NUM_ENTITIES; i++)
    {
        appl_avrcp_tg_ca_handles[i] = AVRCP_CA_ENTITY_INVALID;
    }
#endif /* AVRCP_COVER_ART_RESPONDER */

    /* Init Media Player */
    retval = appl_mp_init ();
    if (API_SUCCESS == retval)
    {
        LOG_DEBUG ("Media Player Init Success\n");
    }
    else
    {
        LOG_DEBUG ("Media Player Init Failed\n");
    }

    return;
}

void appl_avrcp_tg_shutdown (void)
{
    API_RESULT retval;

    retval = API_SUCCESS;

#ifdef AVRCP_COVER_ART_RESPONDER
    LOG_DEBUG ("Shutting down AVRCP  Cover Art Responder...\n");
    retval = BT_avrcp_car_shutdown();
    LOG_DEBUG ("Retval - 0x%04X\n", retval);
#endif /* AVRCP_COVER_ART_RESPONDER */

    return;
}

void appl_avrcp_tg_start (UCHAR index)
{
    API_RESULT retval;
    UINT8     *phandle;
    UINT32     avrcp_record_handle;

    phandle = NULL;
    retval  = API_SUCCESS;
    avrcp_record_handle = 0x00U;
    BT_IGNORE_UNUSED_PARAM(index);

#ifdef AVRCP_1_4

    /* Update the additional protocol desc list */
    (BT_IGNORE_RETURN_VALUE) BT_dbase_get_record_handle
    (
        DB_RECORD_AVRCP_TARGET,
        0U,
        &avrcp_record_handle
    );

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value
    (
        avrcp_record_handle,
        ADDITIONAL_PROT_DESC_LIST_ID,
        tg_additional_protocol_descriptor_list,
        sizeof(tg_additional_protocol_descriptor_list)
    );
#endif /* AVRCP_1_4 */

#ifdef AVRCP_COVER_ART_RESPONDER

    /**
     *
     * Note:
     * PTS is not parsing  "additional_protocol_desc_list" correctly which is sent from the generated
     * sdp record. Need to check, why.
     *
     * Updating the additional protocol desc list with known PSM for Cover Art Service.
     *
     */
    (BT_IGNORE_RETURN_VALUE) BT_dbase_get_record_handle
    (
        DB_RECORD_AVRCP_TARGET,
        0U,
        &avrcp_record_handle
    );

    (BT_IGNORE_RETURN_VALUE) BT_dbase_change_attr_value
    (
        avrcp_record_handle,
        ADDITIONAL_PROT_DESC_LIST_ID,
        avrcp16_tg_additional_protocol_desc_list,
        sizeof(avrcp16_tg_additional_protocol_desc_list)
    );

    /* Set the OBEX connection state to FALSE */
    obex_connect_state  = BT_FALSE;

    phandle = &appl_avrcp_tg_ca_handles[index];

    LOG_DEBUG ("Starting AVRCP Cover Art Responder Instance...\n");

    retval = BT_avrcp_car_start
             (
                 phandle,
                 appl_avrcp_car_callback
             );

    if (API_SUCCESS != retval)
    {
        *phandle = AVRCP_CA_ENTITY_INVALID;
    }

    LOG_DEBUG ("Retval - 0x%04X\n", retval);

#endif /* AVRCP_COVER_ART_RESPONDER */

    return;
}

void appl_avrcp_tg_stop (UCHAR index)
{
    API_RESULT    retval;
    UINT8        *phandle;

    phandle = NULL;
    retval  = API_SUCCESS;
    BT_IGNORE_UNUSED_PARAM(index);

#ifdef AVRCP_COVER_ART_RESPONDER
    phandle = &appl_avrcp_tg_ca_handles[index];
    if (AVRCP_CA_ENTITY_INVALID == *phandle)
    {
        LOG_DEBUG ("Invalid Cover Art Handle to Stop. \n");
        /* return; */
    }
    else
    {
        LOG_DEBUG ("Stopping AVRCP Cover Art Responder Instance...\n");
        retval = BT_avrcp_car_stop
                 (
                     phandle
                 );
        if (API_SUCCESS == retval)
        {
            *phandle = AVRCP_CA_ENTITY_INVALID;
        }
        LOG_DEBUG ("Retval - 0x%04X\n", retval);
    }
#endif /* AVRCP_COVER_ART_RESPONDER */

    return;
}

void appl_avrcp_tg_handle_message_cmd
     (
         UCHAR inst,
         UCHAR *event_data,
         UINT16 event_datalen
     )
{
    API_RESULT      retval;
    UCHAR           avctp_tl_msg_type;
    UCHAR           op_id;
    UCHAR           op_id_state;
    UINT16          vu_op_id;

    AVRCP_AL_CT_CMD_INFO  *avrcp_cmd_info;

    BT_IGNORE_UNUSED_PARAM(event_datalen);

    retval = API_SUCCESS;

    avrcp_cmd_info  = ( AVRCP_AL_CT_CMD_INFO  *)event_data;

    appl_avrcp_print_opcode(avrcp_cmd_info->cmd_opcode);

    /* Update Transaction Label and Message Type */
    avctp_tl_msg_type = avrcp_cmd_info->cmd_tl | AVCTP_CTR_MSG_RESPONSE;

    switch(avrcp_cmd_info->cmd_opcode)
    {
    case AVRCP_OPCODE_UNIT_INFO:

        /* Send Unit Info response */
        retval = BT_avrcp_al_send_info_rsp
                 (
                     &appl_avrcp_handle[inst],
                     avctp_tl_msg_type,
                     AVRCP_OPCODE_UNIT_INFO,
                     AVRCP_SUBUNIT_TYPE_PANEL
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("Failed to send AVRCP_OPCODE_UNIT_INFO: Error Code = 0x%04X\n", retval);
        }

        break;

    case AVRCP_OPCODE_SUBUNIT_INFO:

        /* Send Subunit Info response */
        retval = BT_avrcp_al_send_info_rsp
                 (
                     &appl_avrcp_handle[inst],
                     avctp_tl_msg_type,
                     AVRCP_OPCODE_SUBUNIT_INFO,
                     AVRCP_SUBUNIT_TYPE_PANEL
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("Failed to send AVRCP_OPCODE_SUBUNIT_INFO: Error Code = 0x%04X\n", retval);
        }

        break;

    case AVRCP_OPCODE_PASS_THROUGH:

        AVRCP_GET_MSG_OPERATION_INFO((UCHAR *)avrcp_cmd_info->cmd_info, op_id, op_id_state);

        appl_avrcp_print_operation_id(op_id);
        LOG_DEBUG("    State = ");
        if (0x00U == op_id_state)
        {
            LOG_DEBUG ("Pressed\n");
        }
        else
        {
            LOG_DEBUG ("Released\n");
        }

        vu_op_id = 0U;

        if (AVRCP_OPERATION_ID_VENDOR_UNIQUE == op_id)
        {
            /* Extract Vendor Unique operation id */
            avrcp_extract_2_byte_metadata_param
            (
                &((UCHAR *)avrcp_cmd_info->cmd_info)[9U],
                &vu_op_id
            );

            LOG_DEBUG ("    Vendor Unique Op Id: 0x%04x\n", vu_op_id);
        }

        /* Accepting passthrought command */
        retval = BT_avrcp_al_send_passthrough_cmd_rsp
                 (
                     &appl_avrcp_handle[inst],
                     avctp_tl_msg_type,
                     AVRCP_RESPONSE_TYPE_ACCEPTED,
                     op_id,
                     (op_id_state >> 7U),
                     vu_op_id
                 );

        break;

    default:
        LOG_DEBUG ("Invalid opcode \n");
        break;
    }

}

#ifdef AVRCP_1_3

void appl_avrcp_tg_handle_vendor_dependent_cmd
     (
         UCHAR       inst,
         void      * event_data,
         UINT16      event_datalen
     )
{
    AVRCP_AL_VD_CMD_INFO  * vd_cmd_info;
    UCHAR                 * cmd_info;
    UINT16                  param_offset, param_len;
    API_RESULT              retval;
    UCHAR                   param;
    AVRCP_AL_VD_CMD_INFO    rsp_pdu_info;


    BT_IGNORE_UNUSED_PARAM(event_datalen);

    param_offset    = 0U;
    vd_cmd_info     = (AVRCP_AL_VD_CMD_INFO *)event_data;
    cmd_info        = (UCHAR *)vd_cmd_info->vd_cmd_data;

    /* Command Type */
    LOG_DEBUG ("    Command Type - ");
    switch(vd_cmd_info->cmd_type)
    {
    case AVRCP_COMMAND_TYPE_CONTROL:
        LOG_DEBUG ("CONTROL\n");
        break;

    case AVRCP_COMMAND_TYPE_STATUS:
        LOG_DEBUG ("STATUS\n");
        break;

    case AVRCP_COMMAND_TYPE_NOTIFY:
        LOG_DEBUG ("NOTIFY\n");
        break;

    default:
        LOG_DEBUG ("UNKNOWN\n");
        break;
    }

    /* offset till cmd hdr */
    param_offset = 6U + 1U;

    /* skip pdu id + packet type +  parameter length */
    param_offset += (1U + 1U + 2U);

    /* Extract param length */
    avrcp_extract_2_byte_metadata_param
    (
        &cmd_info[9U],
        &param_len
    );

    LOG_DEBUG ("    Param Length: 0x%04x\n", param_len);

    LOG_DEBUG ("\n");
    switch(vd_cmd_info->pdu_id)
    {
    case AVRCP_METADATA_PDU_ID_GET_CAPABILITY:
        {
            UCHAR    cap_id;

            cap_id = ((UCHAR *)vd_cmd_info->vd_cmd_data)[param_offset];

            LOG_DEBUG ("    PDU-ID -> Get Capability <0x%x>\n", vd_cmd_info->pdu_id);
            LOG_DEBUG ("     - Capability ID  = 0x%02x\n", cap_id);

            /* Send Get Capability response */
            appl_avrcp_tg_send_get_capability_cmd_rsp
            (
                cap_id,
                inst,
                0x00U
            );
        }

        break;

    case AVRCP_METADATA_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR:    /* Fall through */
    case AVRCP_METADATA_PDU_ID_LIST_PLAYER_APP_SETTING_VAL:     /* Fall through */
    case AVRCP_METADATA_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL:  /* Fall through */
    case AVRCP_METADATA_PDU_ID_SET_PLAYER_APP_SETTING_VAL:      /* Fall through */
    case AVRCP_METADATA_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT: /* Fall through */
    case AVRCP_METADATA_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT:  /* Fall through */
    case AVRCP_METADATA_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET:     /* Fall through */
    case AVRCP_METADATA_PDU_ID_INFORM_BATTERY_STATUS:

        appl_avrcp_tg_send_player_appl_setting_cmd_rsp
        (
            0x00U,
            vd_cmd_info
        );
        break;

    case AVRCP_METADATA_PDU_ID_GET_ELEMENT_ATTRIBUTE:
        appl_avrcp_tg_send_get_element_attributes_cmd_rsp
        (
            0U,
            vd_cmd_info
        );
        break;

    case AVRCP_METADATA_PDU_ID_GET_PLAY_STATUS:
        appl_avrcp_tg_send_get_play_status_cmd_rsp
        (
            0U,
            vd_cmd_info
        );
        break;

    case AVRCP_METADATA_PDU_ID_REGISTER_NOTIFICATION:
        LOG_DEBUG ("    PDU-ID -> Register Notification<0x%x>.\n", vd_cmd_info->pdu_id);
        appl_avrcp_tg_send_notify_cmd_interim_rsp
        (
            0U,
            vd_cmd_info
        );
        break;

    case AVRCP_METADATA_PDU_ID_REQUEST_CONTINUING_RESPONSE:
        appl_avrcp_tg_handle_request_continue_response
        (
            0U,
            vd_cmd_info
        );
        break;

    case AVRCP_METADATA_PDU_ID_ABORT_CONTINUING_RESPONSE:
        appl_avrcp_tg_handle_abort_continue_response
        (
            0U,
            vd_cmd_info
        );

        break;

    case AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME:
        appl_avrcp_tg_send_set_absolute_volume_cmd_rsp
        (
            0U,
            vd_cmd_info
        );
        break;

    case AVRCP_PDU_ID_SET_ADDRESSED_PLAYER:
        appl_avrcp_tg_send_set_addressed_player_cmd_rsp
        (
            0U,
            vd_cmd_info
        );

        break;

    case AVRCP_PDU_ID_PLAY_ITEMS:          /* Fall Through */
    case AVRCP_PDU_ID_ADD_TO_NOW_PLAYING:
        appl_avrcp_tg_send_play_item_rsp
        (
            0U,
            vd_cmd_info
        );

        break;

    default:
        LOG_DEBUG ("Unknown PDU-ID <0x%x>\n", vd_cmd_info->pdu_id);

        param     = AVRCP_METADATA_ERROR_INVALID_COMMAND;
        param_len = 1U;

        /* Update PDU parameters */
        rsp_pdu_info.tl              = vd_cmd_info->tl;
        rsp_pdu_info.cmd_type        = AVRCP_RESPONSE_TYPE_REJECTED;
        rsp_pdu_info.packet_type     = AVRCP_METADATA_PACKET_TYPE_SINGLE;
        rsp_pdu_info.pdu_id          = vd_cmd_info->pdu_id;
        rsp_pdu_info.vd_cmd_data     = &param;
        rsp_pdu_info.vd_cmd_datalen  = param_len;

        LOG_DEBUG ("Sending Reject Respone\n");
        retval = BT_avrcp_al_send_metadata_pdu
                 (
                     &appl_avrcp_handle[inst],
                     &rsp_pdu_info,
                     0x01U
                 );
        break;
    }

    BT_IGNORE_UNUSED_PARAM(retval);
}

void appl_avrcp_tg_send_get_capability_cmd_rsp
     (
         UCHAR       cap_id,
         UCHAR       inst,
         UCHAR       tl
     )
{
    API_RESULT             retval;
    UCHAR                 *param;
    UINT16                 paramlen, offset, i;
    AVRCP_AL_VD_CMD_INFO   vd_rsp_info;

    /* Init */
    vd_rsp_info.tl          = tl;
    vd_rsp_info.pdu_id      = AVRCP_METADATA_PDU_ID_GET_CAPABILITY;
    vd_rsp_info.packet_type = AVRCP_METADATA_PACKET_TYPE_SINGLE;
    vd_rsp_info.cmd_type    = AVRCP_RESPONSE_TYPE_STABLE;

    retval = API_SUCCESS;
    offset = 0U;
    paramlen = 0U;
    param    = NULL;
    if(AVRCP_CAP_ID_COMPANY_ID == cap_id)
    {
        paramlen = 1U + 1U + (UINT16)(company_id_list.no_ids) * 3U;

        param = BT_alloc_mem(paramlen);
        if (NULL == param)
        {
            LOG_DEBUG ("Failed to allocate memory\n");
            retval = API_FAILURE; /* return; */
        }
        else
        {
            param[offset++] = cap_id;
            param[offset++] = company_id_list.no_ids;

            for (i = 0U; i < company_id_list.no_ids; i++)
            {
                avrcp_pack_3_byte_metadata_param
                (
                    &param[offset],
                    &company_id_list.c_id_list[i]
                );

                offset += 3U;
            }

            vd_rsp_info.vd_cmd_data = param;
            vd_rsp_info.vd_cmd_datalen = paramlen;
        }

    }
    else if (AVRCP_CAP_ID_EVENTS_SUPPORTED == cap_id)
    {
        paramlen = 1U + 1U + (UINT16)event_id_list.no_ids;

        param = BT_alloc_mem(paramlen);
        if (NULL == param)
        {
            LOG_DEBUG ("Failed to allocate memory\n");
            retval = API_FAILURE; /* return; */
        }
        else
        {
            param[offset++] = cap_id;
            param[offset++] = event_id_list.no_ids;

            for (i = 0U; i < event_id_list.no_ids; i++)
            {
                param[offset++] = event_id_list.e_id_list[i];
            }

            vd_rsp_info.vd_cmd_data = param;
            vd_rsp_info.vd_cmd_datalen = paramlen;
        }

    }
    else
    {
        /* Unknow Capability ID */
        vd_rsp_info.cmd_type    = AVRCP_RESPONSE_TYPE_REJECTED;

        paramlen = 1U;

        param = BT_alloc_mem(paramlen);
        if (NULL == param)
        {
            LOG_DEBUG ("Failed to allocate memory\n");
            retval = API_FAILURE; /* return; */
        }
        else
        {
            param[offset] = AVRCP_METADATA_ERROR_INVALID_PARAMETER;

            vd_rsp_info.vd_cmd_data = param;
            vd_rsp_info.vd_cmd_datalen = paramlen;
        }
    }

    if (API_SUCCESS == retval)
    {
        retval = BT_avrcp_al_send_metadata_pdu
                 (
                     &appl_avrcp_handle[inst],
                     &vd_rsp_info,
                     0x01U
                 );

        /* Free the memory */
        if (NULL != param)
        {
            BT_free_mem(param);

            param = NULL;
        }
    }

    BT_IGNORE_UNUSED_PARAM(retval);
    return;
}

void appl_avrcp_tg_send_player_appl_setting_cmd_rsp
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     )
{
    UCHAR                  *param;
    UCHAR                   status_error_code;
    UINT16                  paramlen, offset, i;
    API_RESULT              retval;
    AVRCP_AL_VD_CMD_INFO    rsp_pdu_info;

    /* Init */
    paramlen            = 0U;
    offset              = 0U;
    status_error_code   = 0U;
    param               = NULL;
    retval = API_SUCCESS;

    if (NULL == vd_cmd_info)
    {
        LOG_DEBUG ("NULL vd_cmd_info\n");
        retval = API_FAILURE; /* return; */
    }
    else
    {
        rsp_pdu_info.tl             = vd_cmd_info->tl;
        rsp_pdu_info.packet_type    = AVRCP_METADATA_PACKET_TYPE_SINGLE;
        rsp_pdu_info.cmd_type       = AVRCP_RESPONSE_TYPE_STABLE;
        rsp_pdu_info.vd_cmd_data    = NULL;
        rsp_pdu_info.vd_cmd_datalen = 0U;

        switch (vd_cmd_info->pdu_id)
        {
        case AVRCP_METADATA_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR:
            LOG_DEBUG ("    PDU-ID -> List Player Appl. Setting Attributes<0x%x>.\n", vd_cmd_info->pdu_id);

            rsp_pdu_info.pdu_id = AVRCP_METADATA_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR;

            paramlen = 1U + (UINT16)player_appl_info.no_attr;

            param = BT_alloc_mem(paramlen);
            if (NULL == param)
            {
                LOG_DEBUG ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = player_appl_info.no_attr;

                for (i = 0U; i < player_appl_info.no_attr; i++)
                {
                    param[offset++] = player_appl_info.attr_id_info[i].attr_id;
                }

                rsp_pdu_info.vd_cmd_data = param;
                rsp_pdu_info.vd_cmd_datalen = paramlen;
            }

            break;

        case AVRCP_METADATA_PDU_ID_LIST_PLAYER_APP_SETTING_VAL:
            {
                UCHAR attr_id, j;

                attr_id = ((UCHAR *)vd_cmd_info->vd_cmd_data)[11U];
                rsp_pdu_info.pdu_id = AVRCP_METADATA_PDU_ID_LIST_PLAYER_APP_SETTING_VAL;

                LOG_DEBUG ("    PDU-ID -> List Player Appl. Setting Values<0x%x>.\n", vd_cmd_info->pdu_id);
                LOG_DEBUG ("    Attribute ID  = 0x%02x\n", attr_id);

                /* Validate Attribute ID */
                if (attr_id > AVRCP_PLAYER_APP_ATTR_ID_SCAN)
                {
                    /* Send reject response */
                    rsp_pdu_info.cmd_type       = AVRCP_RESPONSE_TYPE_REJECTED;
                    status_error_code           = AVRCP_METADATA_ERROR_INVALID_PARAMETER;

                    goto SEND_METADATA_PDU_RSP;
                }

                for (i = 0U; i < player_appl_info.no_attr; i++)
                {
                    if (attr_id == player_appl_info.attr_id_info[i].attr_id)
                    {
                        paramlen = 1U + (UINT16)player_appl_info.attr_id_info[i].attr_id_val_count;

                        param = BT_alloc_mem(paramlen);
                        if (NULL == param)
                        {
                            LOG_DEBUG ("Failed to allocate memory\n");
                            retval = API_FAILURE; /* return; */
                        }
                        else
                        {
                            param[offset++] = player_appl_info.attr_id_info[i].attr_id_val_count;

                            for (j = 0U; j < player_appl_info.attr_id_info[i].attr_id_val_count; j++)
                            {
                                param[offset++] = player_appl_info.attr_id_info[i].attr_id_val[j].val;
                            }

                            rsp_pdu_info.vd_cmd_data    = param;
                            rsp_pdu_info.vd_cmd_datalen = paramlen;
                        }

                        break;
                    }
                }
            }

            break;

        case AVRCP_METADATA_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL:
            {
                UCHAR num_attr, j;
                UCHAR attr_id[APPL_AVRCP_MAX_PLAYER_ATTR_COUNT];
                UINT16 param_offset;

                param_offset = 11U;
                rsp_pdu_info.pdu_id = AVRCP_METADATA_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL;

                LOG_DEBUG ("    PDU-ID -> Get Current Player Appl. Setting Value<0x%x>.\n", vd_cmd_info->pdu_id);

                num_attr = ((UCHAR *)vd_cmd_info->vd_cmd_data)[param_offset++];

                LOG_DEBUG ("    No. of Attrbutes: %d\n", num_attr);

                /**
                 * Validate attribute count
                 */
                if (num_attr > 4U)
                {
                    /* Send reject response */
                    rsp_pdu_info.cmd_type       = AVRCP_RESPONSE_TYPE_REJECTED;
                    status_error_code           = AVRCP_METADATA_ERROR_INVALID_PARAMETER;

                    goto SEND_METADATA_PDU_RSP;
                }

                for (i = 0U; i < num_attr; i++)
                {
                    attr_id[i] = ((UCHAR *)vd_cmd_info->vd_cmd_data)[param_offset++];

                    /* Validate Attribute ID */
                    if (attr_id[i] > AVRCP_PLAYER_APP_ATTR_ID_SCAN)
                    {
                        /* Send reject response */
                        rsp_pdu_info.cmd_type       = AVRCP_RESPONSE_TYPE_REJECTED;
                        status_error_code           = AVRCP_METADATA_ERROR_INVALID_PARAMETER;

                        goto SEND_METADATA_PDU_RSP;
                    }

                    LOG_DEBUG ("\t - 0x%x\n", attr_id[i]);
                }

                if (num_attr > player_appl_info.no_attr)
                {
                    num_attr = player_appl_info.no_attr;
                }

                /* No. Attr_id + (attr_id + attr_val) */
                paramlen = 1U + 2U * (UINT16)(num_attr);

                param = BT_alloc_mem(paramlen);
                if (NULL == param)
                {
                    LOG_DEBUG ("Failed to allocate memory\n");
                    retval = API_FAILURE; /* return; */
                }
                else
                {
                    param[offset++] = num_attr;

                    for (j = 0U; j < num_attr; j++)
                    {
                        for (i = 0U; i < player_appl_info.no_attr; i++)
                        {
                            if (attr_id[j] == player_appl_info.attr_id_info[i].attr_id)
                            {
                                param[offset++] = player_appl_info.attr_id_info[i].attr_id;
                                param[offset++] = player_appl_info.attr_id_info[i].cur_attr_val.val;

                                break;
                            }
                        }
                    }

                    rsp_pdu_info.vd_cmd_data    = param;
                    rsp_pdu_info.vd_cmd_datalen = paramlen;
                }

                break;
            }

        case AVRCP_METADATA_PDU_ID_SET_PLAYER_APP_SETTING_VAL:
            {
                UCHAR   num_attr, j, attr_id, attr_id_val;
                UINT16  param_offset;

                param_offset = 11U;
                rsp_pdu_info.pdu_id = AVRCP_METADATA_PDU_ID_SET_PLAYER_APP_SETTING_VAL;

                LOG_DEBUG ("    PDU-ID -> Set Player Appl. Setting Value<0x%x>.\n", vd_cmd_info->pdu_id);

                num_attr = ((UCHAR *)vd_cmd_info->vd_cmd_data)[param_offset++];

                LOG_DEBUG ("    No. of Attrbutes: %d\n", num_attr);

                /**
                 * Validate attribute count
                 */
                if (num_attr > 4U)
                {
                    /* Send reject response */
                    rsp_pdu_info.cmd_type       = AVRCP_RESPONSE_TYPE_REJECTED;
                    status_error_code           = AVRCP_METADATA_ERROR_INVALID_PARAMETER;

                    goto SEND_METADATA_PDU_RSP;
                }

                for (i = 0U; i < num_attr; i++)
                {
                    attr_id     = ((UCHAR *)vd_cmd_info->vd_cmd_data)[param_offset++];
                    attr_id_val = ((UCHAR *)vd_cmd_info->vd_cmd_data)[param_offset++];

                    LOG_DEBUG ("\tAttr. ID %d: 0x%x\n", (i+1U), attr_id);
                    LOG_DEBUG ("\t   - Value: 0x%x\n", attr_id_val );

                    /* Validate Attribute ID & attr_id_val range */
                    if (((AVRCP_PLAYER_APP_ATTR_ID_EQUALIZER == attr_id) &&
                        (attr_id_val > 2U)) ||
                        ((AVRCP_PLAYER_APP_ATTR_ID_REPEAT == attr_id) &&
                        (attr_id_val > 4U)) ||
                         ((AVRCP_PLAYER_APP_ATTR_ID_SHUFFLE == attr_id) &&
                        (attr_id_val > 3U)) ||
                         ((AVRCP_PLAYER_APP_ATTR_ID_SCAN == attr_id) &&
                        (attr_id_val > 3U)))
                    {
                        /* Send reject response */
                        rsp_pdu_info.cmd_type       = AVRCP_RESPONSE_TYPE_REJECTED;
                        status_error_code           = AVRCP_METADATA_ERROR_INVALID_PARAMETER;

                        goto SEND_METADATA_PDU_RSP;
                    }

                    /* Update the attribute values */
                    for (j = 0U; j < player_appl_info.no_attr; j++)
                    {
                        if (attr_id == player_appl_info.attr_id_info[j].attr_id)
                        {
                            player_appl_info.attr_id_info[j].cur_attr_val.val = attr_id_val;
                        }
                    }
                }
            }

            /* No parameters in response */

            break;

        case AVRCP_METADATA_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT:
            {
                UCHAR       attr_id_len;
                UINT16      char_set_id;
                UINT16      param_offset;
                UCHAR       num_attr, j, attr_id, attr_ids[4U];

                param_offset = 11U;
                rsp_pdu_info.pdu_id = AVRCP_METADATA_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT;

                num_attr = ((UCHAR *)vd_cmd_info->vd_cmd_data)[param_offset++];

                /**
                 * Validate attribute count
                 */
                if (num_attr > 4U)
                {
                    /* Send reject response */
                    rsp_pdu_info.cmd_type       = AVRCP_RESPONSE_TYPE_REJECTED;
                    status_error_code           = AVRCP_METADATA_ERROR_INVALID_PARAMETER;

                    goto SEND_METADATA_PDU_RSP;
                }

                LOG_DEBUG ("    PDU-ID -> Get Player Appl. Setting Attr. Text<0x%x>.\n", vd_cmd_info->pdu_id);
                LOG_DEBUG ("    No. of Attrbutes: %d\n", num_attr);
                for (i = 0U; i < num_attr; i++)
                {
                    attr_id = ((UCHAR *)vd_cmd_info->vd_cmd_data)[param_offset++];
                    LOG_DEBUG ("        - 0x%02x\n", attr_id);

                    /* Validate Attribute ID */
                    if (attr_id > AVRCP_PLAYER_APP_ATTR_ID_SCAN)
                    {
                        /* Send reject response */
                        rsp_pdu_info.cmd_type       = AVRCP_RESPONSE_TYPE_REJECTED;
                        status_error_code           = AVRCP_METADATA_ERROR_INVALID_PARAMETER;

                        goto SEND_METADATA_PDU_RSP;
                    }

                    attr_ids[i] = attr_id;
                }

                /* num_attr = player_appl_info.no_attr; */

                paramlen = 1U;

                for (i = 0U; i < num_attr; i++)
                {
                    for (j = 0U; j < player_appl_info.no_attr; j++)
                    {
                        if (attr_ids[i] == player_appl_info.attr_id_info[j].attr_id)
                        {
                            paramlen +=
                                (UINT16)(1U + 2U + 1U + BT_str_len(player_appl_info.attr_id_info[j].attr_id_text));
                            break;
                        }
                    }
                }

                param = BT_alloc_mem(paramlen);
                if (NULL == param)
                {
                    LOG_DEBUG ("Failed to allocate memory\n");
                    retval = API_FAILURE; /* return; */
                }
                else
                {
                    char_set_id = 0x006AU;

                    param[offset++] = num_attr;
                    for (i = 0U; i < num_attr; i++)
                    {
                        for (j = 0U; j < player_appl_info.no_attr; j++)
                        {
                            if (attr_ids[i] == player_appl_info.attr_id_info[j].attr_id)
                            {
                                param[offset++] = player_appl_info.attr_id_info[j].attr_id;

                                avrcp_pack_2_byte_metadata_param
                                (
                                    &param[offset],
                                    &char_set_id
                                );

                                offset += 2U;

                                attr_id_len = (UCHAR)BT_str_len(player_appl_info.attr_id_info[j].attr_id_text);

                                param[offset++] = attr_id_len;

                                BT_mem_copy
                                (
                                    &param[offset],
                                    player_appl_info.attr_id_info[j].attr_id_text,
                                    attr_id_len
                                );

                                offset += attr_id_len;

                                break;
                            }
                        }
                    }

                    rsp_pdu_info.vd_cmd_data    = param;
                    rsp_pdu_info.vd_cmd_datalen = paramlen;
                }
            }

            break;

        case AVRCP_METADATA_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT:
            {
                UCHAR   attr_id, num_attr, num_attr_val, attr_val_len;
                UINT16  char_set_id;
                UINT16  param_offset;
                UCHAR   attr_id_val, j, k;
                UCHAR   attr_id_values[APPL_AVRCP_MAX_PLAYER_ATTR_VAL_COUNT];

                param_offset = 11U;
                rsp_pdu_info.pdu_id = AVRCP_METADATA_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT;

                attr_id         = ((UCHAR *)vd_cmd_info->vd_cmd_data)[param_offset++];
                num_attr_val    = ((UCHAR *)vd_cmd_info->vd_cmd_data)[param_offset++];

                LOG_DEBUG ("    PDU-ID -> Get Player Appl. Setting Value Text<0x%x>.\n", vd_cmd_info->pdu_id);
                LOG_DEBUG ("    Attribute ID  = 0x%02x\n", attr_id);
                LOG_DEBUG ("    No. of Attr. Values  = 0x%02x\n", num_attr_val);

                /* Validate Attribute ID */
                if (attr_id > AVRCP_PLAYER_APP_ATTR_ID_SCAN)
                {
                    /* Send reject response */
                    rsp_pdu_info.cmd_type       = AVRCP_RESPONSE_TYPE_REJECTED;
                    status_error_code           = AVRCP_METADATA_ERROR_INVALID_PARAMETER;

                    goto SEND_METADATA_PDU_RSP;
                }

                /**
                 * Validate attribute val count
                 */
                if (num_attr_val > 4U)
                {
                    /* Send reject response */
                    rsp_pdu_info.cmd_type       = AVRCP_RESPONSE_TYPE_REJECTED;
                    status_error_code           = AVRCP_METADATA_ERROR_INVALID_PARAMETER;

                    goto SEND_METADATA_PDU_RSP;
                }

                for (i = 0U; i < num_attr_val; i++)
                {
                    attr_id_val = ((UCHAR *)vd_cmd_info->vd_cmd_data)[param_offset++];

                    LOG_DEBUG ("        - 0x%02x\n", attr_id_val);

                    /* Validate Attribute ID & attr_id_val range */
                    if (((AVRCP_PLAYER_APP_ATTR_ID_EQUALIZER == attr_id) &&
                        (attr_id_val > 2U)) ||
                        ((AVRCP_PLAYER_APP_ATTR_ID_REPEAT == attr_id) &&
                        (attr_id_val > 4U)) ||
                         ((AVRCP_PLAYER_APP_ATTR_ID_SHUFFLE == attr_id) &&
                        (attr_id_val > 3U)) ||
                         ((AVRCP_PLAYER_APP_ATTR_ID_SCAN == attr_id) &&
                        (attr_id_val > 3U)))
                    {
                        /* Send reject response */
                        rsp_pdu_info.cmd_type       = AVRCP_RESPONSE_TYPE_REJECTED;
                        status_error_code           = AVRCP_METADATA_ERROR_INVALID_PARAMETER;

                        goto SEND_METADATA_PDU_RSP;
                    }

                   attr_id_values[i] = attr_id_val;
                }

                num_attr = player_appl_info.no_attr;

                paramlen = 1U;

                for (i = 0U; i < num_attr; i++)
                {
                    if (attr_id == player_appl_info.attr_id_info[i].attr_id)
                    {
                        break;
                    }
                }

                if (num_attr_val > player_appl_info.attr_id_info[i].attr_id_val_count)
                {
                    num_attr_val = player_appl_info.attr_id_info[i].attr_id_val_count;
                }

                for (j = 0U; j < num_attr_val; j++)
                {
                    for (k = 0U; k < player_appl_info.attr_id_info[i].attr_id_val_count; k++)
                    {
                        if (attr_id_values[j] == player_appl_info.attr_id_info[i].attr_id_val[k].val)
                        {
                            paramlen +=
                                (UINT16)(1U + 2U + 1U + BT_str_len(player_appl_info.attr_id_info[i].attr_id_val[k].val_text));
                            break;
                        }
                    }
                }

                param = BT_alloc_mem(paramlen);
                if (NULL == param)
                {
                    LOG_DEBUG ("Failed to allocate memory\n");
                    retval = API_FAILURE; /* return; */
                }
                else
                {
                    char_set_id = 0x006AU;

                    for (i = 0U; i < num_attr; i++)
                    {
                        if (attr_id == player_appl_info.attr_id_info[i].attr_id)
                        {
                            break;
                        }
                    }

                    param[offset++] = num_attr_val;

                    for (j = 0U; j < num_attr_val; j++)
                    {
                        for (k = 0U; k < player_appl_info.attr_id_info[i].attr_id_val_count; k++)
                        {
                            if (attr_id_values[j] == player_appl_info.attr_id_info[i].attr_id_val[k].val)
                            {
                                param[offset++] = player_appl_info.attr_id_info[i].attr_id_val[k].val;

                                avrcp_pack_2_byte_metadata_param
                                (
                                    &param[offset],
                                    &char_set_id
                                );

                                offset += 2U;

                                attr_val_len = (UCHAR )BT_str_len(player_appl_info.attr_id_info[i].attr_id_val[k].val_text);

                                param[offset++]  = attr_val_len;

                                BT_mem_copy
                                (
                                    &param[offset],
                                    player_appl_info.attr_id_info[i].attr_id_val[k].val_text,
                                    attr_val_len
                                );

                                offset += attr_val_len;

                                break;
                            }
                        }
                    }

                    rsp_pdu_info.vd_cmd_data    = param;
                    rsp_pdu_info.vd_cmd_datalen = paramlen;
                }
            }

            break;

        case AVRCP_METADATA_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET:
            {
                UCHAR       num_char_set;
                UINT16      char_set_id;
                UINT16      param_offset;

                param_offset = 11U;
                rsp_pdu_info.pdu_id = AVRCP_METADATA_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET;

                num_char_set = ((UCHAR *)vd_cmd_info->vd_cmd_data)[param_offset++];

                LOG_DEBUG ("    PDU-ID -> Inform Displayable Char Set<0x%x>.\n", vd_cmd_info->pdu_id);
                LOG_DEBUG ("    No. of Char Set = 0x%02x\n", num_char_set);

                for (i = 0U; i < num_char_set; i++)
                {
                    /* Extract the player ID */
                    avrcp_extract_2_byte_metadata_param
                    (
                        &((UCHAR *)vd_cmd_info->vd_cmd_data)[param_offset],
                        &char_set_id
                    );

                    param_offset += 2U;

                    LOG_DEBUG ("        - 0x%04x\n", char_set_id);
                }

                /* No parameters in response */
            }

            break;

        case AVRCP_METADATA_PDU_ID_INFORM_BATTERY_STATUS:
            {
                UINT16   param_offset;

                param_offset = 11U;
                rsp_pdu_info.pdu_id = AVRCP_METADATA_PDU_ID_INFORM_BATTERY_STATUS;

                battery_status = ((UCHAR *)vd_cmd_info->vd_cmd_data)[param_offset];

                LOG_DEBUG ("    PDU-ID -> Inform Battery Status<0x%x>.\n", vd_cmd_info->pdu_id);
                LOG_DEBUG ("    Battery Status = 0x%02x\n", battery_status);

                /* No parameters in response */
            }

            break;

        default:
            LOG_DEBUG (" Invalid PDU-ID. \n");
            break;
        }

SEND_METADATA_PDU_RSP:
        if (API_SUCCESS == retval)
        {
            if (AVRCP_RESPONSE_TYPE_REJECTED == rsp_pdu_info.cmd_type)
            {
                LOG_DEBUG ("    Sending Reject Response\n");
                rsp_pdu_info.vd_cmd_data    = &status_error_code;
                rsp_pdu_info.vd_cmd_datalen = 1U;
            }

            retval = BT_avrcp_al_send_metadata_pdu
                     (
                        &appl_avrcp_handle[inst],
                        &rsp_pdu_info,
                        0x01U
                     );

            /* Free the memory */
            if (NULL != param)
            {
                BT_free_mem(param);

                param = NULL;
            }
        }
    }

    BT_IGNORE_UNUSED_PARAM(retval);
    return;
}

void appl_avrcp_tg_send_get_element_attributes_cmd_rsp
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     )
{
    API_RESULT  retval;
    UCHAR       *param;
    UCHAR       *vd_cmd_data;
    UCHAR       num_attr;
    UINT16      paramlen, offset, i, j;
    UINT16      param_offset;
    UINT32      attr_ids[APPL_AVRCP_MAX_ATTR_COUNT];
    UINT32      uid[2U], attr_id;
    AVRCP_AL_VD_CMD_INFO rsp_pdu_info;

    /* Init */
    paramlen = 0U;
    offset   = 0U;
    param    = NULL;
    retval = API_SUCCESS;
    vd_cmd_data = NULL;

    if (NULL == vd_cmd_info)
    {
        LOG_DEBUG ("NULL vd_cmd_info\n");
        retval = API_FAILURE; /* return; */
    }
    else
    {
        vd_cmd_data = vd_cmd_info->vd_cmd_data;
        param_offset = 11U;

        BT_mem_set (attr_ids, 0, (sizeof(UINT32)*APPL_AVRCP_MAX_ATTR_COUNT));

        LOG_DEBUG ("    PDU-ID -> Get Element Attributes<0x%x>.\n", vd_cmd_info->pdu_id);

        /* Extract UID */
        avrcp_extract_4_byte_metadata_param
        (
            &vd_cmd_data[param_offset],
            &uid [0U]
        );

        param_offset += 4U;

        avrcp_extract_4_byte_metadata_param
        (
            &vd_cmd_data[param_offset],
            &uid [1U]
        );

        param_offset += 4U;

        LOG_DEBUG ("    UID: 0x%04x%04x\n", uid [0U], uid [1U]);

        /* No. of Attributes */
        num_attr = vd_cmd_data[param_offset++];

        LOG_DEBUG ("    No. of Attributes: %d\n", num_attr);

        if (num_attr > APPL_AVRCP_MAX_ATTR_COUNT)
        {
            LOG_DEBUG ("No. of Attributes > APPL_AVRCP_MAX_ATTR_COUNT\n");
            num_attr = APPL_AVRCP_MAX_ATTR_COUNT;
        }

        /* Print Attribues */
        if (0U != num_attr)
        {
            for (i = 0U; i < num_attr; i++)
            {
                avrcp_extract_4_byte_metadata_param
                (
                    &vd_cmd_data[param_offset],
                    &attr_id
                );

                param_offset += 4U;

                attr_ids[i] = attr_id;

                LOG_DEBUG ("       - 0x%04x\n", attr_id);
            }

#ifdef AVRCP_COVER_ART
            /**
             * To skip Cover Art Handle in the response.
             * Few PTS testcases require not sent CoverArt Handles
             */
            for (i = 0U; i < num_attr; i++)
            {
                if ((0x00U != skip_cover_art_handle_get_folder_items_rsp) &&
                    (AVRCP_MEDIA_ATTR_ID_DEFAULT_COVER_ART == attr_ids[i]))
                {
                    attr_ids[i] = 0x00U;
                    num_attr -= 1U;
                    break;
                }
            }
#endif /* AVRCP_COVER_ART */
        }
        else
        {
            LOG_DEBUG ("    Requested ALL Attributes\n");

            num_attr = media_element_info.num_attr;

            for (i = 0U; i < num_attr; i++)
            {
                attr_ids[i] = media_element_info.attr_id_info[i].attr_id;
            }

            /**
             * To skip Cover Art Handle in the response.
             * Few PTS testcases require not sent CoverArt Handles
             */
             if (0x00U != skip_cover_art_handle_get_folder_items_rsp)
             {
                 attr_ids[7U] = 0x00U;
                 num_attr -= 1U;
            }
        }

#ifdef AVRCP_COVER_ART_RESPONDER
        /**
         * As per spec. CoverArt Attribute ID should be sent only when OBEX connection is
         * Active
         */
        retval = BT_avrcp_car_get_obex_connection_status(0U, &obex_connect_state);
        if (BT_TRUE != obex_connect_state)
        {
            for (i = 0U; i < num_attr; i++)
            {
                if (AVRCP_MEDIA_ATTR_ID_DEFAULT_COVER_ART == attr_ids[i])
                {
                    attr_ids[i] = 0x00U;
                    num_attr --;
                }
            }
        }
#endif /* AVRCP_COVER_ART_RESPONDER */

        rsp_pdu_info.tl          = vd_cmd_info->tl;
        rsp_pdu_info.pdu_id      = AVRCP_METADATA_PDU_ID_GET_ELEMENT_ATTRIBUTE;
        rsp_pdu_info.packet_type = AVRCP_METADATA_PACKET_TYPE_SINGLE;

        if ((0x00U == uid [0U]) &&
            (0x00U == uid [1U]))
        {
            rsp_pdu_info.cmd_type    = AVRCP_RESPONSE_TYPE_STABLE;

#ifdef AVRCP_COVER_ART_RESPONDER
            /**
             * Disable to send Reject response for Get Image Request.
             */
            appl_ca_get_img_reject_rsp = BT_FALSE;
#endif /* AVRCP_COVER_ART_RESPONDER */
        }
        else
        {
            rsp_pdu_info.cmd_type    = AVRCP_RESPONSE_TYPE_REJECTED;
        }

        /* Calculate parameter length */
        paramlen = 1U;
        if (AVRCP_RESPONSE_TYPE_STABLE == rsp_pdu_info.cmd_type)
        {
            for (i = 0U; i < num_attr; i++)
            {
                for (j = 0U; j < media_element_info.num_attr; j++)
                {
                    if (attr_ids[i] == media_element_info.attr_id_info[j].attr_id)
                    {
                        paramlen += 4U + 2U + 2U + media_element_info.attr_id_info[j].attr_len;
                        break;
                    }
                }
            }
        }

        /* Allocate memory */
        param = BT_alloc_mem(paramlen);
        if (NULL == param)
        {
            LOG_DEBUG ("Failed to allocate memory\n");
            retval = API_FAILURE; /* return; */
        }
        else
        {
            if (AVRCP_RESPONSE_TYPE_REJECTED == rsp_pdu_info.cmd_type)
            {
                param[0U] = AVRCP_METADATA_ERROR_INVALID_PARAMETER;

                goto SEND_RSP;
            }

            /* pack parameters */
            param[offset++] = num_attr;
            for (i = 0U; i < num_attr; i++)
            {
                for (j = 0U; j < media_element_info.num_attr; j++)
                {
                    if (attr_ids[i] == media_element_info.attr_id_info[j].attr_id)
                    {
                        /* attribute id */
                        avrcp_pack_4_byte_metadata_param
                        (
                            &param[offset],
                            &media_element_info.attr_id_info[j].attr_id
                        );

                        offset += 4U;

                        /* char set */
                        avrcp_pack_2_byte_metadata_param
                        (
                            &param[offset],
                            &media_element_info.attr_id_info[j].attr_char_set
                        );

                        offset += 2U;

                        /* attr_str len */
                        avrcp_pack_2_byte_metadata_param
                        (
                            &param[offset],
                            &media_element_info.attr_id_info[j].attr_len
                        );

                        offset += 2U;

                        /* attr. value */
                        BT_mem_copy
                        (
                            &param[offset],
                            media_element_info.attr_id_info[j].attr_str,
                            media_element_info.attr_id_info[j].attr_len
                        );

                        offset += media_element_info.attr_id_info[j].attr_len;

                        break;
                    }
                }
            }
        }
    }

SEND_RSP:

    if (API_SUCCESS == retval)
    {
        rsp_pdu_info.vd_cmd_data = param;
        rsp_pdu_info.vd_cmd_datalen = paramlen;

        retval = BT_avrcp_al_send_metadata_pdu
                 (
                     &appl_avrcp_handle[inst],
                     &rsp_pdu_info,
                     0x01U
                 );

        /* Free the memory */
        if (NULL != param)
        {
            BT_free_mem(param);
            param = NULL;
        }
    }

    BT_IGNORE_UNUSED_PARAM(retval);
    return;
}

void appl_avrcp_tg_send_get_play_status_cmd_rsp
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     )
{
    API_RESULT  retval;
    UCHAR       *param;
    /* UCHAR       *vd_cmd_data; */
    UINT16      paramlen, offset;
    AVRCP_AL_VD_CMD_INFO rsp_pdu_info;

    paramlen = 0U;
    offset   = 0U;
    param    = NULL;

    if (NULL == vd_cmd_info)
    {
        LOG_DEBUG ("NULL vd_cmd_info\n");
        retval = API_FAILURE; /* return; */
    }
    else
    {
        /* vd_cmd_data = vd_cmd_info->vd_cmd_data; */

        LOG_DEBUG ("    PDU-ID -> Get Play Status<0x%x>.\n", vd_cmd_info->pdu_id);

        rsp_pdu_info.tl          = vd_cmd_info->tl;
        rsp_pdu_info.pdu_id      = AVRCP_METADATA_PDU_ID_GET_PLAY_STATUS;
        rsp_pdu_info.packet_type = AVRCP_METADATA_PACKET_TYPE_SINGLE;
        rsp_pdu_info.cmd_type    = AVRCP_RESPONSE_TYPE_STABLE;

        /* Calculate parameter length */
        paramlen = 4U + 4U + 1U;

        /* Allocate memory */
        param = BT_alloc_mem(paramlen);
        if (NULL == param)
        {
            LOG_DEBUG ("Failed to allocate memory\n");
            /* return; */
        }
        else
        {
            /* pack parameters */
            avrcp_pack_4_byte_metadata_param
            (
                &param[offset],
                &cur_song_play_status.song_total_len
            );

            offset += 4U;

            avrcp_pack_4_byte_metadata_param
            (
                &param[offset],
                &cur_song_play_status.song_cur_pos
            );

            offset += 4U;

            param[offset] = cur_song_play_status.song_play_status;

            rsp_pdu_info.vd_cmd_data    = param;
            rsp_pdu_info.vd_cmd_datalen = paramlen;

            retval = BT_avrcp_al_send_metadata_pdu
                     (
                         &appl_avrcp_handle[inst],
                         &rsp_pdu_info,
                         0x01U
                     );

            /* Free the memory */
            if (NULL != param)
            {
                BT_free_mem(param);
                param = NULL;
            }
        }
    }

    BT_IGNORE_UNUSED_PARAM(retval);
    return;
}

void appl_avrcp_tg_send_notify_cmd_interim_rsp
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     )
{
    API_RESULT  retval;
    UCHAR       *param;
    UCHAR       event_id;
    UCHAR       rsp_status;
    UCHAR       *vd_cmd_data;
    UINT16      paramlen, offset, i;
    UINT16      param_offset;
    AVRCP_AL_VD_CMD_INFO rsp_pdu_info;

    /* Init */
    paramlen = 0U;
    offset = 0U;
    param = NULL;
    rsp_status = 0x00U;
    retval = API_SUCCESS;
    vd_cmd_data = NULL;
    event_id = 0xFFU;

    if (NULL == vd_cmd_info)
    {
        LOG_DEBUG ("NULL vd_cmd_info\n");
        retval = API_FAILURE; /* return; */
    }
    else
    {
        vd_cmd_data = vd_cmd_info->vd_cmd_data;
        param_offset = 11U;

        event_id = vd_cmd_data[param_offset];

        switch(event_id)
        {
        case AVRCP_EVENT_PLAYBACK_STATUS_CHANGED:
            LOG_DEBUG ("\n    Event-ID -> AVRCP_EVENT_PLAYBACK_STATUS_CHANGED<0x%x>.\n", event_id);

            /* Calculate parameter length */
            paramlen = 2U;

            /* Allocate memory */
            param = BT_alloc_mem(paramlen);
            if (NULL == param)
            {
                LOG_DEBUG ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {

                param[offset++] = event_id;
                param[offset] = cur_song_play_status.song_play_status;

                /* register */
                (BT_IGNORE_RETURN_VALUE)appl_mp_register_player_event
                (
                    event_id,
                    vd_cmd_info->tl
                );
            }

            break;

        case AVRCP_EVENT_TRACK_CHANGED:
            LOG_DEBUG ("\n    Event-ID -> AVRCP_EVENT_TRACK_CHANGED<0x%x>.\n", event_id);

            /* Calculate parameter length */
            paramlen = 9U;

            /* Allocate memory */
            param = BT_alloc_mem(paramlen);
            if (NULL == param)
            {
                LOG_DEBUG ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = event_id;

                if ((0U != track_select_status) &&
                    (0xFFFFFFFFU != track_select_status))
                {
                    /* Sending UID of first element in media_list_info[] */
                    avrcp_pack_4_byte_metadata_param
                    (
                        &param[offset],
                        &media_list_info[0U].item_uid_msb
                    );

                    offset += 4U;

                    avrcp_pack_4_byte_metadata_param
                    (
                        &param[offset],
                        &media_list_info[0U].item_uid_lsb
                    );

                    offset += 4U;
                }
                else
                {
                    avrcp_pack_4_byte_metadata_param
                    (
                        &param[offset],
                        &track_select_status
                    );

                    offset += 4U;

                    avrcp_pack_4_byte_metadata_param
                    (
                        &param[offset],
                        &track_select_status
                    );

                    offset += 4U;
                }

                /* register */
                (BT_IGNORE_RETURN_VALUE) appl_mp_register_player_event
                (
                    event_id,
                    vd_cmd_info->tl
                );
            }

            break;

        case AVRCP_EVENT_TRACK_REACHED_END:  /* Fall Through */
        case AVRCP_EVENT_TRACK_REACHED_START:
            LOG_DEBUG ("\n    Event-ID -> %s<0x%x>.\n", event_id == AVRCP_EVENT_TRACK_REACHED_END ?
                    "AVRCP_EVENT_TRACK_REACHED_END" : "AVRCP_EVENT_TRACK_REACHED_START", event_id);

            /* Calculate parameter length */
            paramlen = 1U;

            /* Allocate memory */
            param = BT_alloc_mem(paramlen);
            if (NULL == param)
            {
                LOG_DEBUG ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset] = event_id;

                /* register */
                (BT_IGNORE_RETURN_VALUE)appl_mp_register_player_event
                (
                    event_id,
                    vd_cmd_info->tl
                );
            }

            break;

        case AVRCP_EVENT_PLAYBACK_POS_CHANGED:
            LOG_DEBUG ("\n    Event-ID -> AVRCP_EVENT_PLAYBACK_POS_CHANGED<0x%x>.\n", event_id);

            /* Calculate parameter length */
            paramlen = 5U;

            /* Allocate memory */
            param = BT_alloc_mem(paramlen);
            if (NULL == param)
            {
                LOG_DEBUG ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = event_id;

                avrcp_pack_4_byte_metadata_param
                (
                    &param[offset],
                    &cur_song_play_status.song_cur_pos
                );

                offset += 4U;

                /* register */
                (BT_IGNORE_RETURN_VALUE) appl_mp_register_player_event
                (
                    event_id,
                    vd_cmd_info->tl
                );
            }

            break;

        case AVRCP_EVENT_BATT_STATUS_CHANGED:
            LOG_DEBUG ("\n    Event-ID -> AVRCP_EVENT_BATT_STATUS_CHANGED<0x%x>.\n", event_id);

            /* Calculate parameter length */
            paramlen = 2U;

            /* Allocate memory */
            param = BT_alloc_mem(paramlen);
            if (NULL == param)
            {
                LOG_DEBUG ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = event_id;
                param[offset] = battery_status;

                /* register */
                (BT_IGNORE_RETURN_VALUE)appl_mp_register_player_event
                (
                    event_id,
                    vd_cmd_info->tl
                );
            }

            break;

        case AVRCP_EVENT_SYSTEM_STATUS_CHANGED:
             LOG_DEBUG ("\n    Event-ID -> AVRCP_EVENT_SYSTEM_STATUS_CHANGED<0x%x>.\n", event_id);

            /* Calculate parameter length */
            paramlen = 2U;

            /* Allocate memory */
            param = BT_alloc_mem(paramlen);
            if (NULL == param)
            {
                LOG_DEBUG ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = event_id;
                param[offset] = system_status;

                /* register */
                (BT_IGNORE_RETURN_VALUE)appl_mp_register_player_event
                (
                    event_id,
                    vd_cmd_info->tl
                );
            }

            break;

        case AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED:
             LOG_DEBUG ("\n    Event-ID -> AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED<0x%x>.\n", event_id);

            /* No. Attr_id + (attr_id + attr_val) */
            paramlen = 1U + 1U + 2U * (UINT16)(player_appl_info.no_attr);

            /* Allocate memory */
            param = BT_alloc_mem(paramlen);
            if (NULL == param)
            {
                LOG_DEBUG ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = event_id;

                param[offset++] = player_appl_info.no_attr;

                for (i = 0U; i < player_appl_info.no_attr; i++)
                {
                    param[offset++] = player_appl_info.attr_id_info[i].attr_id;
                    param[offset++] = player_appl_info.attr_id_info[i].cur_attr_val.val;
                }

                /* register */
                (BT_IGNORE_RETURN_VALUE)appl_mp_register_player_event
                (
                    event_id,
                    vd_cmd_info->tl
                );
            }

            break;

#ifdef AVRCP_1_4
        case AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED:
            LOG_DEBUG ("\n    Event-ID -> AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED<0x%x>.\n", event_id);

            /* Calculate parameter length */
            paramlen = 1U;

            /* Allocate memory */
            param = BT_alloc_mem(paramlen);
            if (NULL == param)
            {
                LOG_DEBUG ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset] = event_id;

                /* register */
                (BT_IGNORE_RETURN_VALUE)appl_mp_register_player_event
                (
                    event_id,
                    vd_cmd_info->tl
                );
            }

            break;

        case AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED:
            LOG_DEBUG ("\n    Event-ID -> AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED<0x%x>.\n", event_id);

            /* Calculate parameter length */
            paramlen = 1U;

            /* Allocate memory */
            param = BT_alloc_mem(paramlen);
            if (NULL == param)
            {
                LOG_DEBUG ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset] = event_id;

                /* register */
                (BT_IGNORE_RETURN_VALUE)appl_mp_register_player_event
                (
                    event_id,
                    vd_cmd_info->tl
                );
            }

            break;

        case AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED:
            LOG_DEBUG ("\n    Event-ID -> AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED<0x%x>.\n", event_id);

            /* Calculate parameter length */
            paramlen = 5U;

            /* Allocate memory */
            param = BT_alloc_mem(paramlen);
            if (NULL == param)
            {
                LOG_DEBUG ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = event_id;

                if (current_player_id == addressed_player_id)
                {
                    current_player_id = browsed_player_id;
                }
                else
                {
                    current_player_id = addressed_player_id;
                }

                avrcp_pack_2_byte_metadata_param
                (
                    &param[offset],
                    &current_player_id
                );

                offset += 2U;

                avrcp_pack_2_byte_metadata_param
                (
                    &param[offset],
                    &global_uid_counter
                );

                offset += 2U;

                /* register */
                (BT_IGNORE_RETURN_VALUE) appl_mp_register_player_event
                (
                    event_id,
                    vd_cmd_info->tl
                );
            }
            break;

        case AVRCP_EVENT_UIDS_CHANGED:
            LOG_DEBUG ("\n    Event-ID -> AVRCP_EVENT_UIDS_CHANGED<0x%x>.\n", event_id);

            /* Calculate parameter length */
            paramlen = 3U;

            /* Allocate memory */
            param = BT_alloc_mem(paramlen);
            if (NULL == param)
            {
                LOG_DEBUG ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = event_id;

                avrcp_pack_2_byte_metadata_param
                (
                    &param[offset],
                    &global_uid_counter
                );

                offset += 2U;

                /* register */
                (BT_IGNORE_RETURN_VALUE) appl_mp_register_player_event
                (
                    event_id,
                    vd_cmd_info->tl
                );
            }
            break;

        case AVRCP_EVENT_VOLUME_CHANGED:
            LOG_DEBUG ("\n    Event-ID -> AVRCP_EVENT_VOLUME_CHANGED<0x%x>.\n", event_id);

            /* Calculate parameter length */
            paramlen = 2U;

            /* Allocate memory */
            param = BT_alloc_mem(paramlen);
            if (NULL == param)
            {
                LOG_DEBUG ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {

                param[offset++] = event_id;

                param[offset] = system_volume;

                /* register */
                (BT_IGNORE_RETURN_VALUE)appl_mp_register_player_event
                (
                    event_id,
                    vd_cmd_info->tl
                );
            }
            break;;

#endif /* AVRCP_1_4 */
        default:
            LOG_DEBUG ("\n    Event-ID -> ??? \n");
            retval = API_FAILURE;
            break;
        }
    }

    if (NULL != vd_cmd_data)
    {
        /* Check Event ID */
        if(event_id > AVRCP_EVENT_VOLUME_CHANGED)
        {
            rsp_status = AVRCP_METADATA_ERROR_INVALID_PARAMETER;

            rsp_pdu_info.cmd_type    = AVRCP_RESPONSE_TYPE_REJECTED;

            rsp_pdu_info.vd_cmd_data    = &rsp_status;
            rsp_pdu_info.vd_cmd_datalen = 1U;
        }
        else
        {
            rsp_pdu_info.cmd_type    = AVRCP_RESPONSE_TYPE_INTERIM;

            if (API_FAILURE == retval)
            {
                /**
                 * Ideally, should not reach here.
                 */
                paramlen = 0U;
            }

            rsp_pdu_info.vd_cmd_data    = param;
            rsp_pdu_info.vd_cmd_datalen = paramlen;
        }

        rsp_pdu_info.tl          = vd_cmd_info->tl;
        rsp_pdu_info.pdu_id      = AVRCP_METADATA_PDU_ID_REGISTER_NOTIFICATION;
        rsp_pdu_info.packet_type = AVRCP_METADATA_PACKET_TYPE_SINGLE;


        retval = BT_avrcp_al_send_metadata_pdu
                 (
                     &appl_avrcp_handle[inst],
                     &rsp_pdu_info,
                     0x01U
                 );

        /* Free the memory */
        if (NULL != param)
        {
            BT_free_mem(param);
            param = NULL;
        }
    }

    BT_IGNORE_UNUSED_PARAM(retval);
    return;
}

void appl_avrcp_tg_handle_request_continue_response
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     )
{
    API_RESULT      retval;
    UINT16          param_offset;
    UCHAR          *vd_cmd_data;
    UCHAR           pdu_id;
    AVRCP_AL_VD_CMD_INFO rsp_pdu_info;

    if (NULL == vd_cmd_info)
    {
        LOG_DEBUG ("NULL vd_cmd_info\n");
        /* return; */
    }
    else
    {
        vd_cmd_data = vd_cmd_info->vd_cmd_data;
        param_offset = 11U;

        pdu_id = vd_cmd_data[param_offset];

        LOG_DEBUG ("    PDU-ID -> Request Continue Response<0x%x>.\n", vd_cmd_info->pdu_id);
        LOG_DEBUG ("    Continue PDU ID: 0x%02x\n", pdu_id);

        rsp_pdu_info.tl             = vd_cmd_info->tl;
        rsp_pdu_info.cmd_type       = AVRCP_RESPONSE_TYPE_STABLE;
        rsp_pdu_info.packet_type    = AVRCP_METADATA_PACKET_TYPE_CONTINUE;
        rsp_pdu_info.pdu_id         = pdu_id;
        rsp_pdu_info.vd_cmd_data    = NULL;
        rsp_pdu_info.vd_cmd_datalen = 0x00U;

        retval = BT_avrcp_al_send_metadata_pdu
                 (
                     &appl_avrcp_handle[inst],
                     &rsp_pdu_info,
                     0x01U
                 );
    }

    BT_IGNORE_UNUSED_PARAM(retval);
    return;
}

void appl_avrcp_tg_handle_abort_continue_response
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     )
{
    API_RESULT      retval;
    UINT16          param_offset;
    UCHAR          *vd_cmd_data;
    UCHAR           pdu_id;
    AVRCP_AL_VD_CMD_INFO rsp_pdu_info;

    if (NULL == vd_cmd_info)
    {
        LOG_DEBUG ("NULL vd_cmd_info\n");
        /* return; */
    }
    else
    {
        vd_cmd_data = vd_cmd_info->vd_cmd_data;
        param_offset = 11U;

        pdu_id = vd_cmd_data[param_offset];

        LOG_DEBUG ("    PDU-ID -> Abort Continue Response<0x%x>.\n", vd_cmd_info->pdu_id);
        LOG_DEBUG ("    Abort PDU ID: 0x%02x\n", pdu_id);

        rsp_pdu_info.tl             = vd_cmd_info->tl;
        rsp_pdu_info.cmd_type       = AVRCP_RESPONSE_TYPE_ACCEPTED;
        rsp_pdu_info.packet_type = AVRCP_METADATA_PACKET_TYPE_SINGLE;

        /**
         * PDU ID in the response has to AVRCP_METADATA_PDU_ID_ABORT_CONTINUING_RESPONSE
         * instead of pdu_id which was received in the request.
         */
        rsp_pdu_info.pdu_id = AVRCP_METADATA_PDU_ID_ABORT_CONTINUING_RESPONSE;
        rsp_pdu_info.vd_cmd_data    = NULL;
        rsp_pdu_info.vd_cmd_datalen = 0x00U;

        retval = BT_avrcp_al_send_metadata_pdu
                 (
                     &appl_avrcp_handle[inst],
                     &rsp_pdu_info,
                     0x01U
                 );
    }

    BT_IGNORE_UNUSED_PARAM(retval);
    return;
}


void appl_avrcp_tg_send_set_addressed_player_cmd_rsp
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     )
{
    API_RESULT  retval;
    UINT16      param_offset;
    UCHAR       *vd_cmd_data;
    UINT16      player_id;
    UCHAR       i;
    UCHAR       rsp_type;
    UCHAR       rsp_status;

    if (NULL == vd_cmd_info)
    {
        LOG_DEBUG ("NULL vd_cmd_info\n");
        /* return; */
    }
    else
    {
        vd_cmd_data = vd_cmd_info->vd_cmd_data;
        param_offset = 11U;

        /* Extract player id */
        avrcp_extract_2_byte_metadata_param
        (
            &vd_cmd_data[param_offset],
            &player_id
        );

        for (i = 0U; i < num_players; i++)
        {
            if (player_id == media_player_list_info[i].player_id)
            {
                break;
            }
        }

        if (i == num_players)
        {
            /* Send Reject Response */
            rsp_type = AVRCP_RESPONSE_TYPE_REJECTED;
            rsp_status = AVRCP_BOW_ERROR_INVALID_PLAYER_ID;
        }
        else
        {
            rsp_type = AVRCP_RESPONSE_TYPE_ACCEPTED;
            rsp_status = AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
        }

        LOG_DEBUG ("    PDU-ID -> Set Addressed Player<0x%x>.\n", vd_cmd_info->pdu_id);
        LOG_DEBUG ("    Player ID: 0x%04x\n", player_id);

        /* Send the response */
        retval = BT_avrcp_al_send_set_addressed_player_rsp
                 (
                     &appl_avrcp_handle[inst],
                     vd_cmd_info->tl,
                     rsp_type,
                     rsp_status
                 );
    }

    BT_IGNORE_UNUSED_PARAM(retval);
    return;
}

void appl_avrcp_tg_send_set_absolute_volume_cmd_rsp
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     )
{
    API_RESULT  retval;
    UINT16      param_offset;
    UCHAR      *vd_cmd_data;
    UCHAR       volume;
    UCHAR       param;
    UINT16      paramlen;
    AVRCP_AL_VD_CMD_INFO rsp_pdu_info;

    if (NULL == vd_cmd_info)
    {
        LOG_DEBUG ("NULL vd_cmd_info\n");
        /* return; */
    }
    else
    {
        vd_cmd_data = vd_cmd_info->vd_cmd_data;
        param_offset = 11U;

        /* Check param length */
        if (0x00U == vd_cmd_info->param_len)
        {
            /* Send reject response */
            rsp_pdu_info.cmd_type  = AVRCP_RESPONSE_TYPE_REJECTED;
            param                  = AVRCP_METADATA_ERROR_INVALID_PARAMETER;
            paramlen               = 1U;

            goto SEND_SET_VOLUME_RSP;
        }
        else
        {
            rsp_pdu_info.cmd_type   = AVRCP_RESPONSE_TYPE_ACCEPTED;
        }

        volume = vd_cmd_data[param_offset];

        /* Update System volume */
        system_volume = volume;

        LOG_DEBUG ("    PDU-ID -> Set Absolute Volume<0x%x>.\n", vd_cmd_info->pdu_id);
        LOG_DEBUG ("    Volume: 0x%02x\n", volume);

        /* Update parameters */
        param = (volume & 0x7FU);
        paramlen = 1U;

SEND_SET_VOLUME_RSP:

        /* Update PDU parameters */
        rsp_pdu_info.tl              = vd_cmd_info->tl;
        rsp_pdu_info.packet_type     = AVRCP_METADATA_PACKET_TYPE_SINGLE;
        rsp_pdu_info.pdu_id          = AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME;
        rsp_pdu_info.vd_cmd_data     = &param;
        rsp_pdu_info.vd_cmd_datalen  = paramlen;

        retval = BT_avrcp_al_send_metadata_pdu
                 (
                     &appl_avrcp_handle[inst],
                     &rsp_pdu_info,
                     0x01U
                 );
    }

    BT_IGNORE_UNUSED_PARAM(retval);
    return;
}

void appl_avrcp_tg_send_play_item_rsp
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     )
{
    API_RESULT  retval;
    UINT16      param_offset;
    UCHAR       *vd_cmd_data;
    UCHAR       scope;
    UINT32      uid[2U], attr_id;
    UINT16      uid_counter;
    UCHAR       param;
    UINT16      paramlen;
    UCHAR       reject, rsp_type;
    UCHAR       i = 0;
    UCHAR		j = 0;
    UCHAR	    item_found;
    AVRCP_AL_VD_CMD_INFO rsp_pdu_info;
    APPL_AVRCP_MEDIA_ELEMENT_ITEM_TYPE_INFO  *media_item_ptr;

    if (NULL == vd_cmd_info)
    {
        LOG_DEBUG ("NULL vd_cmd_info\n");
        /* return; */
    }
    else
    {
        media_item_ptr  = NULL;
        reject = 0U;
        item_found = 0x00U;

        vd_cmd_data = vd_cmd_info->vd_cmd_data;
        param_offset = 11U;

        if (AVRCP_PDU_ID_PLAY_ITEMS == vd_cmd_info->pdu_id)
        {
            LOG_DEBUG ("    PDU-ID -> Play Item<0x%x>.\n", vd_cmd_info->pdu_id);
        }
        else
        {
            LOG_DEBUG ("    PDU-ID -> Add To Now Playing<0x%x>.\n", vd_cmd_info->pdu_id);
        }

        /* Scope */
        scope = vd_cmd_data[param_offset++];

        if (AVRCP_SCOPE_MEDIA_PLAYER_LIST == scope)
        {
            /* Reject */
            reject = 0x01U;
            rsp_type = AVRCP_RESPONSE_TYPE_REJECTED;
            param = AVRCP_BOW_ERROR_INVALID_SCOPE;
            paramlen = 1U;
            goto SEND_RSP;
        }

        /* UID */
        /* MSB UID */
        avrcp_extract_4_byte_metadata_param
        (
            &vd_cmd_data[param_offset],
            &uid[0U]
        );
        param_offset += 4U;

        /* LSB UID */
        avrcp_extract_4_byte_metadata_param
        (
            &vd_cmd_data[param_offset],
            &uid[1U]
        );

        param_offset += 4U;

        /* UID Counter */
        avrcp_extract_2_byte_metadata_param
        (
            &vd_cmd_data[param_offset],
            &uid_counter
        );

        /* MISRA C - 2012 Rule 2.2 */
        /* param_offset += 2U; */

        if ((0xFFFFFFFFU == uid[0U]) &&
            (0xFFFFFFFFU == uid[1U]))
        {
            /* Reject */
            reject   = 0x01U;
            rsp_type = AVRCP_RESPONSE_TYPE_REJECTED;
            param    = AVRCP_BOW_ERROR_DOES_NOT_EXIST;
            paramlen = 1U;
            goto SEND_RSP;
        }

        /* Display */
        LOG_DEBUG ("     - Scope : %d\n", scope);
        LOG_DEBUG ("     - UID   : 0x0%04x%04x\n", uid[0U], uid[1U]);
        LOG_DEBUG ("     - UID counter : 0x%04x\n", uid_counter);

        /* Update parameters */
        rsp_type    = AVRCP_RESPONSE_TYPE_ACCEPTED;
        param       = AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
        paramlen    = 1U;

        /**
         * Adding the media item to media_list_info only if scope is AVRCP_SCOPE_VIRTUAL_FILESYSTEM.
         * For AVRCP_SCOPE_SEARCH, AVRCP_SCOPE_NOW_PLAYING scope, we use media_list_info data.
         */
        if (AVRCP_SCOPE_VIRTUAL_FILESYSTEM == scope)
        {
            for (i = 0U; i < APPL_AVRCP_MAX_VF_DEPTH; i++)
            {
                for (j = 0U; j < APPL_AVRCP_MAX_FOLDER_ITEMS; j++)
                {
                    if ((APPL_AVRCP_ITEM_TYPE_MEDIA == vf_list_info[i][j].item_type) &&
                        (uid[0U] == vf_list_info[i][j].item_uid_msb) &&
                        (uid[1U] == vf_list_info[i][j].item_uid_lsb))
                    {
                        item_found = 0x01U;
                        break;
                    }
                }

                if (0x01U == item_found)
                {
                    break;
                }
            }
        }
        else if (AVRCP_SCOPE_SEARCH == scope)
        {
            /* Find the Media Item */
            for (i = 0U; i < APPL_AVRCP_MAX_SEARCH_LIST_COUNT; i++)
            {
                if ((uid[0U] == media_search_list_info[i].item_uid_msb) &&
                   (uid[1U] == media_search_list_info[i].item_uid_lsb))
                {
                    item_found = 0x01U;
                    break;
                }
            }

            if (0x01U != item_found)
            {
                /* Reject */
                reject   = 0x01U;
                rsp_type = AVRCP_RESPONSE_TYPE_REJECTED;
                param    = AVRCP_BOW_ERROR_DOES_NOT_EXIST;
                paramlen = 1U;
                goto SEND_RSP;
            }
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        if (((media_list_count + 1U) < APPL_AVRCP_MAX_MEDIA_LIST_COUNT) &&
             (0x01U == item_found))
        {
            if (AVRCP_SCOPE_VIRTUAL_FILESYSTEM == scope)
            {
                /*  Assign the Media Item info pointer */
                media_item_ptr = &vf_list_info[i][j].media_item_info;
            }
            else if (AVRCP_SCOPE_SEARCH == scope)
            {
                media_item_ptr = &media_search_list_info[i].media_item_info;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            /* === Copy the item to Media Item List === */
            media_list_info[media_list_count].item_type = APPL_AVRCP_ITEM_TYPE_MEDIA;

            media_list_info[media_list_count].item_uid_lsb = uid[1U];
            media_list_info[media_list_count].item_uid_msb = uid[0U];

            media_list_info[media_list_count].media_item_info.char_set = media_item_ptr->char_set;
            media_list_info[media_list_count].media_item_info.name_len = media_item_ptr->name_len;

            BT_mem_copy
            (
                media_list_info[media_list_count].media_item_info.name,
                media_item_ptr->name,
                media_item_ptr->name_len
            );

            media_list_info[media_list_count].media_item_info.num_attr = media_item_ptr->num_attr;

            if (0x00U != media_item_ptr->num_attr)
            {
                 /* Copy attributes id and values */
                for (i = 0U;  i < APPL_AVRCP_MAX_ATTR_COUNT; i++)
                {
                    attr_id = media_item_ptr->attr_info[i].attr_id;

                    if (0x00U != attr_id)
                    {
                        media_list_info[media_list_count].media_item_info.attr_info[attr_id-1U].attr_id =
                                    media_item_ptr->attr_info[attr_id-1U].attr_id;

                        media_list_info[media_list_count].media_item_info.attr_info[attr_id-1U].char_set =
                                    media_item_ptr->attr_info[attr_id-1U].char_set;


                        media_list_info[media_list_count].media_item_info.attr_info[attr_id-1U].att_val_len =
                                    media_item_ptr->attr_info[attr_id-1U].att_val_len;

                        BT_mem_copy
                        (
                            media_list_info[media_list_count].media_item_info.attr_info[attr_id-1U].att_val,
                            media_item_ptr->attr_info[attr_id-1U].att_val,
                            media_item_ptr->attr_info[attr_id-1U].att_val_len
                        );
                    }
                }
            }

            media_list_count ++;

            /* Increment UID counter */
            global_uid_counter ++;
        }
        if (AVRCP_SCOPE_NOW_PLAYING == scope)
        {
            /* Find the Media Item */
            for (i = 0U; i < APPL_AVRCP_MAX_MEDIA_LIST_COUNT; i++)
            {
                if ((uid[0U] == media_list_info[i].item_uid_msb) &&
                        (uid[1U] == media_list_info[i].item_uid_lsb))
                {
                    item_found = 0x01U;
                    break;
                }
            }

            if (0x01U != item_found)
            {
                /* Reject */
                reject   = 0x01U;
                rsp_type = AVRCP_RESPONSE_TYPE_REJECTED;
                param    = AVRCP_BOW_ERROR_DOES_NOT_EXIST;
                paramlen = 1U;
                goto SEND_RSP;
            }
        }

        SEND_RSP:

        /* Update PDU parameters */
        rsp_pdu_info.tl              = vd_cmd_info->tl;
        rsp_pdu_info.cmd_type        = rsp_type;
        rsp_pdu_info.packet_type     = AVRCP_METADATA_PACKET_TYPE_SINGLE;
        rsp_pdu_info.pdu_id          = vd_cmd_info->pdu_id;
        rsp_pdu_info.vd_cmd_data     = &param;
        rsp_pdu_info.vd_cmd_datalen  = paramlen;

        retval = BT_avrcp_al_send_metadata_pdu
                 (
                     &appl_avrcp_handle[inst],
                     &rsp_pdu_info,
                     0x01U
                 );
    }

    BT_IGNORE_UNUSED_PARAM(retval);
    BT_IGNORE_UNUSED_PARAM(reject);

    return;
}

#endif /* AVRCP_1_3 */

#ifdef AVRCP_1_4

void appl_avrcp_tg_handle_browsing_cmd
     (
         UCHAR * event_data,
         UINT16   event_datalen
     )
{
    API_RESULT           retval;
    UCHAR               *cmd_info;
    UINT16               brow_cmd_len;
    /* UINT16               param_offset; */
    AVRCP_AL_BROW_CMD_INFO *brow_cmd_info;
    AVRCP_AL_BROW_RSP_INFO  rsp_info;
    UCHAR                rsp_status;

    BT_IGNORE_UNUSED_PARAM(event_datalen);

    retval = API_SUCCESS;

    brow_cmd_info = (AVRCP_AL_BROW_CMD_INFO *)event_data;
    cmd_info      = (UCHAR *)brow_cmd_info->brow_cmd_info;

    /* Extract parameter length */
    avrcp_extract_2_byte_metadata_param
    (
        &cmd_info[2U],
        &brow_cmd_len
    );

    /* offset till parameter length */
    /* param_offset = 4; */

    LOG_DEBUG ("\n    Parameters Length = 0x%04x\n", brow_cmd_len);

    LOG_DEBUG ("\n");
    switch(brow_cmd_info->pdu_id)
    {
    case AVRCP_PDU_ID_SET_BROWSED_PLAYER:
        appl_avrcp_tg_handle_set_browsed_player
        (
            0U,
            brow_cmd_info
        );
        break;

    case AVRCP_PDU_ID_GET_FOLDER_ITEMS:
        appl_avrcp_tg_handle_get_folder_items_req
        (
            0U,
            brow_cmd_info
        );
        break;

    case AVRCP_PDU_ID_CHANGE_PATH:
        appl_avrcp_tg_handle_setpath
        (
            0U,
            brow_cmd_info
        );
        break;

    case AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES:
        appl_avrcp_tg_handle_get_item_attributes
        (
            0U,
            brow_cmd_info
        );
        break;

    case AVRCP_PDU_ID_SEARCH:
        appl_avrcp_tg_handle_search
        (
            0U,
            brow_cmd_info
        );
        break;

#ifdef AVRCP_GET_NUM_OF_ITEMS
    case AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS:
        appl_avrcp_tg_handle_get_total_number_of_items
        (
            0U,
            brow_cmd_info
        );
        break;
#endif /* AVRCP_GET_NUM_OF_ITEMS */

    default:
        printf ("Unknown Browsing PDU-ID<0x%02x>\n", brow_cmd_info->pdu_id);

        rsp_status = AVRCP_METADATA_ERROR_INVALID_COMMAND;
        rsp_info.param_info     = &rsp_status;
        rsp_info.param_info_len = 1U;

        rsp_info.pdu_id    = AVRCP_PDU_ID_GENERAL_REJECT;
        rsp_info.tl        = brow_cmd_info->tl;

        LOG_DEBUG ("    Sending Reject Response.\n");
        retval = BT_avrcp_al_send_browsing_cmd_rsp
                (
                    &appl_avrcp_handle[0U],
                    &rsp_info
                );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
        }

        break;
    }

    return;
}

void appl_avrcp_tg_handle_get_folder_items_req
     (
         UCHAR               inst,
         AVRCP_AL_BROW_CMD_INFO * brow_cmd_info
     )
{
    API_RESULT   retval;
    UCHAR       *cmd_info;
    UINT16       param_offset;
    UCHAR        scope, attr_count, i;
    UINT32       start_item_idx, end_item_idx;
    UINT32       attr_ids[APPL_AVRCP_MAX_ATTR_COUNT], attr_id;
    UCHAR        reject, rsp_status;
    AVRCP_AL_BROW_RSP_INFO    rsp_info;

    if (NULL == brow_cmd_info)
    {
        LOG_DEBUG ("NULL brow_cmd_info\n");
        /* return; */
    }
    else
    {
        cmd_info      =  (UCHAR *)brow_cmd_info->brow_cmd_info;

        /* offset till parameter length */
        param_offset = 4U;

        /* flag to indicate reject */
        reject = 0x00U;
        rsp_status = 0x00U;

        /* Init */
        BT_mem_set(attr_ids, 0, sizeof(attr_ids));

        LOG_DEBUG ("    PDU ID - Get Folder Items(0x%x).\n", brow_cmd_info->pdu_id);

        scope = cmd_info[param_offset++];

        /* Validate the scope */
        if (scope > AVRCP_SCOPE_NOW_PLAYING)
        {
            /* reject */
            reject = 0x01U;
            rsp_status = AVRCP_BOW_ERROR_INVALID_SCOPE;

            goto SEND_RSP;
        }

        /* Extract Start Item Index */
        avrcp_extract_4_byte_metadata_param
        (
            &cmd_info[param_offset],
            &start_item_idx
        );

        param_offset += 4U;

        /* Extract End Item Index */
        avrcp_extract_4_byte_metadata_param
        (
            &cmd_info[param_offset],
            &end_item_idx
        );

        param_offset += 4U;

        if (start_item_idx > end_item_idx)
        {
            /* reject */
            reject = 0x01U;
            rsp_status = AVRCP_BOW_ERROR_RANGE_OUT_OF_BOUNDS;

            goto SEND_RSP;
        }

        attr_count = cmd_info[param_offset++];

        LOG_DEBUG ("    Scope : %d\n", scope);
        LOG_DEBUG ("    Start Item Index : %d\n", start_item_idx);
        LOG_DEBUG ("    End Item Index : %d\n", end_item_idx);
        if (0x00U == attr_count)
        {
            LOG_DEBUG ("    Requested ALL Attributes\n");

            /* Fill all the supported attr ID's  */
            for (i = 0U; i < APPL_AVRCP_MAX_ATTR_COUNT; i++)
            {
                attr_ids[i] = media_attr_ids[i];
            }

            attr_count = APPL_AVRCP_MAX_ATTR_COUNT;

            /**
             * Note:
             * To skip Cover Art Handle in the response.
             * Few PTS testcases require not sent CoverArt Handles
             */
            if (0x00U != skip_cover_art_handle_get_folder_items_rsp)
            {
                attr_ids[7U] = 0x00U;
                attr_count -= 1U;
            }
        }
        else if (0xFFU == attr_count)
        {
            LOG_DEBUG ("    Requested NO Attributes\n");
        }
        else
        {
            /* Extract requested attibutes ID's */
            for (i = 0U; i < attr_count; i++)
            {
                /* Extract Attribute ID */
                avrcp_extract_4_byte_metadata_param
                (
                    &cmd_info[param_offset],
                    &attr_id
                );

                param_offset += 4U;

                LOG_DEBUG ("     - 0x%04x\n", attr_id);

                if ((attr_id < AVRCP_MEDIA_ATTR_ID_MEDIA_TITLE) ||
#ifdef AVRCP_COVER_ART_RESPONDER
                    (attr_id > AVRCP_MEDIA_ATTR_ID_DEFAULT_COVER_ART)
#else
                    (attr_id > AVRCP_MEDIA_ATTR_ID_PLAYING_TIME)
#endif /* AVRCP_COVER_ART_RESPONDER */
                    )
                {
                    /* reject */
                    reject = 0x01U;
                    rsp_status = AVRCP_METADATA_ERROR_INVALID_PARAMETER;
                    goto SEND_RSP;
                }
                else
                {
                    attr_ids[attr_id-1U] = attr_id;
                }
            }
        }

#ifdef AVRCP_COVER_ART_RESPONDER
        /**
         * As per spec. CoverArt Attribute ID should be sent only when OBEX connection is
         * Active
         */
        retval = BT_avrcp_car_get_obex_connection_status(0U, &obex_connect_state);
        if ((BT_TRUE != obex_connect_state) &&
            (AVRCP_MEDIA_ATTR_ID_DEFAULT_COVER_ART == attr_ids[7U]))
        {
            attr_ids[7U] = 0x00U;
            attr_count --;
        }
#endif /* AVRCP_COVER_ART_RESPONDER */

        /* Based on the type of scope process the command */
        switch(scope)
        {
        case AVRCP_SCOPE_MEDIA_PLAYER_LIST:

            /* Check if the range is beyond the total no of players */
            if (start_item_idx >= num_players)
            {
                /* reject */
                reject = 0x01U;
                rsp_status = AVRCP_BOW_ERROR_RANGE_OUT_OF_BOUNDS;

                goto SEND_RSP;
            }

            if (end_item_idx >= num_players)
            {
                end_item_idx = (UINT32)num_players - 1U;
            }

            appl_avrcp_tg_handle_get_media_player_list
            (
                inst,
                start_item_idx,
                end_item_idx,
                brow_cmd_info->tl
            );

            break;

        case AVRCP_SCOPE_VIRTUAL_FILESYSTEM:

            appl_avrcp_tg_handle_get_virtual_filesystem_list
            (
                inst,
                start_item_idx,
                end_item_idx,
                attr_count,
                attr_ids,
                brow_cmd_info->tl
            );

#ifdef AVRCP_COVER_ART_RESPONDER
            /**
             * Disable to send Reject response for Get Image Request.
             */
            appl_ca_get_img_reject_rsp = BT_FALSE;
#endif /* AVRCP_COVER_ART_RESPONDER */

            break;

        case AVRCP_SCOPE_SEARCH:

            if (0x00U == media_search_list_count)
            {
                /* reject */
                reject = 0x01U;
                rsp_status = AVRCP_BOW_ERROR_NO_VALID_SEARCH_RESULTS;

                goto SEND_RSP;
            }

            if (start_item_idx >= media_search_list_count)
            {
                /* reject */
                reject = 0x01U;
                rsp_status = AVRCP_BOW_ERROR_RANGE_OUT_OF_BOUNDS;

                goto SEND_RSP;
            }

            if (end_item_idx >= media_search_list_count)
            {
                end_item_idx = media_search_list_count - 1U;
            }

            appl_avrcp_tg_handle_get_search_list
            (
                inst,
                start_item_idx,
                end_item_idx,
                attr_count,
                attr_ids,
                brow_cmd_info->tl
            );

            break;

        case AVRCP_SCOPE_NOW_PLAYING:

            /* Check if the range is beyond the total no of items */
            if (start_item_idx >= media_list_count)
            {
                /* reject */
                reject = 0x01U;
                rsp_status = AVRCP_BOW_ERROR_RANGE_OUT_OF_BOUNDS;

                goto SEND_RSP;
            }

            if (end_item_idx >= media_list_count)
            {
                end_item_idx = (UINT32)media_list_count - 1U;
            }

            appl_avrcp_tg_handle_get_media_element_list
            (
                inst,
                start_item_idx,
                end_item_idx,
                attr_count,
                attr_ids,
                brow_cmd_info->tl
            );

#ifdef AVRCP_COVER_ART_RESPONDER
            /**
             * Disable to send Reject response for Get Image Request.
             */
            appl_ca_get_img_reject_rsp = BT_FALSE;
#endif /* AVRCP_COVER_ART_RESPONDER */

            break;

        default:
            LOG_DEBUG("Invalid Scope: %d\n", scope);
            break;
        }

SEND_RSP:

        if (0x00U != reject)
        {
            rsp_info.pdu_id    = brow_cmd_info->pdu_id;
            rsp_info.tl        = brow_cmd_info->tl;
            rsp_info.param_info = &rsp_status;
            rsp_info.param_info_len = 1U;

            retval = BT_avrcp_al_send_browsing_cmd_rsp
                     (
                         &appl_avrcp_handle[inst],
                         &rsp_info
                     );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("BT_avrcp_al_send_browsing_cmd_rsp failed - 0x%04X\n", retval);
            }
        }
    }

    BT_IGNORE_UNUSED_PARAM(retval);
    return;
}

/* This is a reference implementaiton. */
/* media_player_list_info will contain all the parameters about the player list */
void appl_avrcp_tg_handle_get_media_player_list
     (
         UCHAR      inst,
         UINT32     start_idx,
         UINT32     end_idx,
         UCHAR      tl
     )
{
    UINT16          param_len;
    UCHAR          *data;
    UINT16          data_len;
    UINT16          param_offset;
    UCHAR           i;
    UINT16          count;
    AVRCP_AL_BROW_RSP_INFO    rsp_info;

    API_RESULT retval;

    /* Init */
    count = 0U;
    retval = API_SUCCESS;

    /**
     * This is an assumption.
     * Actual value will depend on the folder items to be sent.
     */
    param_len = APPL_AVRCP_MAX_BROW_PDU_PARAM_LEN;

    /* Data len */
    data_len = param_len;

    /* Allocate the memory */
    data = BT_alloc_mem(data_len);
    if (NULL == data)
    {
        LOG_DEBUG("Failed to Allocated memory for media_player_list\n");
        /* return; */
    }
    else
    {
        /* Reset */
        BT_mem_set(data, 0, data_len);

        param_offset = 0x00U;

        /* Pack the status of the command */
        data [param_offset++] = AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;

        /* Pack the UID counter */
        avrcp_pack_2_byte_metadata_param
        (
            &data [param_offset],
            &global_uid_counter
        );

        param_offset += 2U;

        count = (UINT16 )((end_idx - start_idx) + 1U);

        /* Pack number of players */
        avrcp_pack_2_byte_metadata_param
        (
            &data [param_offset],
            &count
        );

        param_offset += 2U;

        for (i = (UCHAR )start_idx; i < (end_idx + 1U); i++)
        {
            /* === pack player params === */
            /* Item type */
            data [param_offset++] = media_player_list_info[i].item_type;

            /* Item length */
            avrcp_pack_2_byte_metadata_param
            (
                &data [param_offset],
                &media_player_list_info[i].item_len
            );

            param_offset += 2U;

            /* player id */
            avrcp_pack_2_byte_metadata_param
            (
                &data [param_offset],
                &media_player_list_info[i].player_id
            );

            param_offset += 2U;

            /* Major payer type */
            data [param_offset++] = media_player_list_info[i].major_player_type;

            /* Player sub type */
            avrcp_pack_4_byte_metadata_param
            (
                &data [param_offset],
                &media_player_list_info[i].player_sub_type
            );

            param_offset += 4U;

            /* Play status */
            data [param_offset++] = media_player_list_info[i].play_status;

            /* feature bit mask */
            avrcp_pack_4_byte_metadata_param
            (
                &data [param_offset],
                &media_player_list_info[i].features_bit_mask[3U]
            );
            param_offset += 4U;

            avrcp_pack_4_byte_metadata_param
            (
                &data [param_offset],
                &media_player_list_info[i].features_bit_mask[2U]
            );
            param_offset += 4U;

            avrcp_pack_4_byte_metadata_param
            (
                &data [param_offset],
                &media_player_list_info[i].features_bit_mask[1U]
            );
            param_offset += 4U;

            avrcp_pack_4_byte_metadata_param
            (
                &data [param_offset],
                &media_player_list_info[i].features_bit_mask[0U]
            );
            param_offset += 4U;

            /* char set */
            avrcp_pack_2_byte_metadata_param
            (
                &data [param_offset],
                &media_player_list_info[i].char_set
            );
            param_offset += 2U;

            /* name len */
            avrcp_pack_2_byte_metadata_param
            (
                &data [param_offset],
                &media_player_list_info[i].name_len
            );
            param_offset += 2U;

            /* name */
            BT_mem_copy(&data[param_offset],
                        media_player_list_info[i].name,
                        media_player_list_info[i].name_len);

            param_offset += media_player_list_info[i].name_len;
        }

        rsp_info.pdu_id         = AVRCP_PDU_ID_GET_FOLDER_ITEMS;
        rsp_info.tl             = tl;
        rsp_info.param_info     = data;
        rsp_info.param_info_len = param_offset;

        retval = BT_avrcp_al_send_browsing_cmd_rsp
                 (
                     &appl_avrcp_handle[inst],
                     &rsp_info
                 );
        if (retval != API_SUCCESS)
        {
            LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
        }

        /* Free the memory */
        if (NULL != data)
        {
            BT_free_mem(data);
            data = NULL;
        }
    }

    return;
}

/* This is a reference implementaiton. */
/* vf_list_info will have all parameters for virtual filesytstem list */
void appl_avrcp_tg_handle_get_virtual_filesystem_list
     (
         UCHAR      inst,
         UINT32     start_idx,
         UINT32     end_idx,
         UCHAR      num_attr_ids,
         UINT32     *mediaattr_ids,
         UCHAR      tl
     )
{
    UINT16      num_items, total_items;
    UINT16      param_offset;
    UCHAR       *data;
    UINT16      data_len;
    UINT16      param_len;
    UCHAR       i /*, j */;
    UCHAR       item_type;
    UINT16      attribute_len, item_length;
    UINT16      item_len_index;
    API_RESULT  retval;
    UCHAR       reject, rsp_status;
    AVRCP_AL_BROW_RSP_INFO    rsp_info;

    /* Init */
    data        = NULL;
    data_len    = 0x00U;
    num_items   = 0U;
    retval = API_SUCCESS;

    /**
     * This is an assumption.
     * Actual value will depend on the  folder items to be sent.
     */
    param_len = APPL_AVRCP_MAX_BROW_PDU_PARAM_LEN;

    /* Set No. of items based on current folder depth */
    if (0x00U == curr_folder_depth)
    {
        num_items = (UINT16 )root_folder_item_count;
    }
    else if (0x01U == curr_folder_depth)
    {
        num_items = (UINT16 )child_folder_item_count;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    /**
     * Restricting the number of items, based on the configured
     * resource files, the length will not cross defined value of
     * 'APPL_AVRCP_MAX_BROW_PDU_PARAM_LEN'
     *
     * Using AVRCP it is possible to send packet of larger size,
     * but that can be taken care in a more complex product level
     * application.
     */
    if (num_items > 5U)
    {
        num_items = 5U;
    }

    reject = 0x00U;

    /* Check if the folder is empty or current folder depth */
    if ((0x00U == num_items) ||
        ((1U == curr_folder_depth) && (0U == child_folder_item_count)))
    {
        reject      = 0x01U;
        rsp_status  = AVRCP_BOW_ERROR_RANGE_OUT_OF_BOUNDS;
        goto SEND_RSP;
    }

    /* Check if the range is beyond the total no of items */
    if (start_idx >= num_items)
    {
        reject      = 0x01U;
        rsp_status  = AVRCP_BOW_ERROR_RANGE_OUT_OF_BOUNDS;
        goto SEND_RSP;
    }

    if (end_idx >= num_items)
    {
        end_idx = (UINT32)num_items - 1U;
    }

    total_items = (UINT16 )((end_idx - start_idx) + 1U);

    /**
     * data len:
     */
    data_len = param_len;

    /* Allocate a large chunk of data to form the response */
    data = (UCHAR *) BT_alloc_mem (data_len);
    if (NULL == data)
    {
        LOG_DEBUG ("Failed to Allocate the memory \n");
        retval = API_FAILURE; /* return; */
    }
    else
    {
        /* Reset */
        BT_mem_set(data, 0, data_len);

        param_offset = 0x00U;

        /* Pack the status */
        data[param_offset++] = AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;

        /* Pack the UID counter */
        avrcp_pack_2_byte_metadata_param
        (
            &data[param_offset],
            &global_uid_counter
        );

        param_offset += 2U;

        /* Update the number of items */
        avrcp_pack_2_byte_metadata_param
        (
            &data[param_offset],
            &total_items
        );

        param_offset += 2U;

        for (i = (UCHAR)start_idx; i < (end_idx + 1U); i++)
        {
            /* === packet virtual folder list params === */

            /* Reset  lengths */
            item_length = 0x00U;
            attribute_len = 0x00U;

            /* Item type */
            item_type = vf_list_info[curr_folder_depth][i].item_type;

            data[param_offset++] = item_type;

            /* Item length will be packed after calculating item length later below */
#if 0
            avrcp_pack_2_byte_metadata_param
            (
                &data[param_offset],
                &current_folder_info[i].item_length
            );
#endif /* 0 */

            /* Save item length field index for filling correct length later below */
            item_len_index = param_offset;

            /* For item length */
            param_offset += 2U;

            /* UID MSB */
            avrcp_pack_4_byte_metadata_param
            (
                &data[param_offset],
                &vf_list_info[curr_folder_depth][i].item_uid_msb
            );

            item_length += 4U;

            /* UID LSB */
            avrcp_pack_4_byte_metadata_param
            (
                &data[param_offset + item_length],
                &vf_list_info[curr_folder_depth][i].item_uid_lsb
            );

            item_length += 4U;

            /**
             * Item_type can be either a Folder or Media Element
             */
            if (APPL_AVRCP_ITEM_TYPE_FOLDER == item_type)
            {
                /* Pack Folder Item params. */
                attribute_len = appl_avrcp_tg_pack_folder_item_info
                (
                    &data[param_offset + item_length],
                    &vf_list_info[curr_folder_depth][i].folder_item_info
                );
            }
            else if (APPL_AVRCP_ITEM_TYPE_MEDIA == item_type)
            {
                /* Pack Media element params. */
                attribute_len = appl_avrcp_tg_pack_media_element_item_info
                (
                    &data[param_offset + item_length],
                    &vf_list_info[curr_folder_depth][i].media_item_info,
                    num_attr_ids,
                    mediaattr_ids
                );
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            /* Add params length of Folder or Media Element */
            item_length += attribute_len;

            param_offset += item_length;

            /* Pack Item length */
            avrcp_pack_2_byte_metadata_param
            (
                &data[item_len_index],
                &item_length
            );
        }

        /* Update rsp params and its length */
        rsp_info.param_info = data;
        rsp_info.param_info_len = param_offset;
    }

SEND_RSP:

    if (0x00U != reject)
    {
        rsp_info.param_info = &rsp_status;
        rsp_info.param_info_len = 1U;
    }

    rsp_info.pdu_id = AVRCP_PDU_ID_GET_FOLDER_ITEMS;
    rsp_info.tl = tl;

    if (API_SUCCESS == retval)
    {
        /* Send response */
        retval = BT_avrcp_al_send_browsing_cmd_rsp
                    (
                        &appl_avrcp_handle[inst],
                        &rsp_info
                    );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
        }

        /* Free the memory */
        if (NULL != data)
        {
            BT_free_mem(data);
            data = NULL;
        }
    }

    return;
}

API_RESULT appl_avrcp_tg_pack_folder_item_info
           (
               UCHAR *data,
               APPL_AVRCP_FOLDER_ITEM_TYPE_INFO *info
           )
{
    UINT16      offset;

    offset = 0U;

    /* Folder type */
    data [offset++] = info->folder_type;

    /* is playable */
    data [offset++] = info->is_playable;

    /* char set */
    avrcp_pack_2_byte_metadata_param
    (
        &data[offset],
        &info->char_set
    );
    offset += 2U;

    /* name len */
    avrcp_pack_2_byte_metadata_param
    (
        &data[offset],
        &info->name_len
    );
    offset += 2U;

    /* name */
    BT_mem_copy(&data[offset], info->name, info->name_len);

    offset += info->name_len;

    return offset;
}

/* Packing the attributes of media element */
API_RESULT appl_avrcp_tg_pack_media_element_item_info
           (
               UCHAR *data,
               APPL_AVRCP_MEDIA_ELEMENT_ITEM_TYPE_INFO *info,
               UCHAR num_attr_ids,
               UINT32 *attr_ids
           )
{
    UCHAR       j;
    /* UCHAR       num_attr; */
    UCHAR       attr_id_count;
    UINT16      offset, index;
    UINT32      attr_id;

    offset = 0U;

    /* Media type */
    data [offset++] = info->media_type;

    /* char set */
    avrcp_pack_2_byte_metadata_param
    (
        &data [offset],
        &info->char_set
    );
    offset += 2U;

    /*  name length */
    avrcp_pack_2_byte_metadata_param
    (
        &data[offset],
        &info->name_len
    );
    offset += 2U;

    /* name */
    BT_mem_copy (&data[offset], info->name, info->name_len);

    offset += info->name_len;

#if 0
    /* num attributes supported by the media item */
    num_attr = info->num_attr;

    /* if requested num_attr_ids is zero, then send supported attr ID's */
    if (0x00 == num_attr_ids)
    {
        num_attr_ids = num_attr;
    }
    else if (0xFF == num_attr_ids)
    {
        num_attr_ids = 0;
    }
    else if (num_attr_ids > APPL_AVRCP_MAX_ATTR_COUNT)
    {
        num_attr_ids = APPL_AVRCP_MAX_ATTR_COUNT;
    }
#endif /* 0 */

    index = offset;
    attr_id_count = 0x00U;

    data [offset++] = num_attr_ids;

    if (0x00U != num_attr_ids)
    {
        for(j = 0U; j < APPL_AVRCP_MAX_ATTR_COUNT; j++)
        {
            /* Get the attr ID requested */
            attr_id = attr_ids[j];

            /* Check if it in range */
            if ((attr_id < AVRCP_MEDIA_ATTR_ID_MEDIA_TITLE) ||
#ifdef AVRCP_COVER_ART_RESPONDER
                (attr_id > AVRCP_MEDIA_ATTR_ID_DEFAULT_COVER_ART)
#else
                (attr_id > AVRCP_MEDIA_ATTR_ID_PLAYING_TIME)
#endif /* AVRCP_COVER_ART_RESPONDER */
                )
            {
                /* Invalid attri_id */
                continue;
            }

            /* Check if it in range */
            if ((info->attr_info[attr_id-1U].attr_id < AVRCP_MEDIA_ATTR_ID_MEDIA_TITLE) ||
#ifdef AVRCP_COVER_ART_RESPONDER
                (info->attr_info[attr_id-1U].attr_id > AVRCP_MEDIA_ATTR_ID_DEFAULT_COVER_ART)
#else
                (info->attr_info[attr_id-1].attr_id > AVRCP_MEDIA_ATTR_ID_PLAYING_TIME)
#endif /* AVRCP_COVER_ART_RESPONDER */
                )
            {
                /* Invalid attri_id */
                continue;
            }
            else
            {
                attr_id_count++;
            }

           offset += appl_avrcp_tg_pack_attr_id_info
                     (
                         &data[offset],
                         attr_id,
                         &info->attr_info[attr_id-1U]
                     );
        }
    }

    data [index] = attr_id_count;

    return offset;
}

API_RESULT appl_avrcp_tg_pack_attr_id_info
           (
               UCHAR *data,
               UINT32 attr_id,
               APPL_AVRCP_MEDIA_ITEM_ATTR_INFO *info
           )
{
    UINT16 offset;

    BT_IGNORE_UNUSED_PARAM(attr_id);

    offset = 0U;

    /* Attr ID */
    avrcp_pack_4_byte_metadata_param
    (
        &data [offset],
        &info->attr_id
    );
    offset += 4U;

    /* Char set */
    avrcp_pack_2_byte_metadata_param
    (
        &data [offset],
        &info->char_set
    );
    offset += 2U;

    /* Value length */
    avrcp_pack_2_byte_metadata_param
    (
        &data [offset],
        &info->att_val_len
    );
    offset += 2U;

    /* Value */
    BT_mem_copy
    (
        &data[offset],
        info->att_val,
        info->att_val_len
    );

    offset += info->att_val_len;

    return offset;
}

void appl_avrcp_tg_handle_set_browsed_player
     (
         UCHAR                 inst,
         AVRCP_AL_BROW_CMD_INFO * brow_cmd_info
     )
{
    API_RESULT      retval;
    UCHAR          *cmd_info;
    UCHAR           i;
    UCHAR           param_offset;
    UINT16          player_id;
    UINT16          char_set;
    UCHAR          *data;
    UINT16          data_len;
    UINT16          param_len;
    UCHAR           rsp_status, reject;
    AVRCP_AL_BROW_RSP_INFO    rsp_info;

    /* Init */
    reject = 0x00U;
    char_set = 0x006AU;
    data = NULL;
    data_len = 0U;
    param_len = 0U;
    cmd_info = NULL;
    retval = API_SUCCESS;


    if (NULL == brow_cmd_info)
    {
        LOG_DEBUG ("NULL brow_cmd_info\n");
        /* return; */
    }
    else
    {
        cmd_info      =  (UCHAR *)brow_cmd_info->brow_cmd_info;

       /* offset till parameter length */
        param_offset = 4U;

        LOG_DEBUG ("    PDU ID - Set Browsed Player(0x%x).\n", brow_cmd_info->pdu_id);

        /* Extract the player ID */
        avrcp_extract_2_byte_metadata_param
        (
            &cmd_info[param_offset],
            &player_id
        );

        LOG_DEBUG ("    Player ID - 0x%04x\n", player_id);

        /* Validate player ID */
        for (i = 0U; i < num_players; i++)
        {
            if (player_id == media_player_list_info[i].player_id)
            {
                break;
            }
        }

        /**
         * Note:
         * To sent -ve response.
         */
        if (AVRCP_BOW_ERROR_PLAYER_NOT_ADDRESSED == bow_player_set_rsp_err)
        {
            /* Send Reject */
            reject = 0x01U;
            rsp_status = AVRCP_BOW_ERROR_PLAYER_NOT_ADDRESSED;
            goto SEND_RSP;
        }

        if (i  == num_players)
        {
            /* Send Reject */
            reject = 0x01U;
            rsp_status = AVRCP_BOW_ERROR_INVALID_PLAYER_ID;
            goto SEND_RSP;
        }

        /*
         * This is an assumption.
         * Actual value will depend on the  folder items to be sent.
         */
        param_len = APPL_AVRCP_MAX_BROW_PDU_PARAM_LEN;

        /*
         * data len:
         */
        data_len = param_len;

        /* Allocate a large chunk of data to form the response */
        data = (UCHAR *) BT_alloc_mem (data_len);
        if (NULL == data)
        {
            LOG_DEBUG ("Failed to Allocate the memory \n");
            retval = API_FAILURE; /* return; */
        }
        else
        {
            /* Reset */
            BT_mem_set(data, 0, data_len);

            param_offset = 0x00U;

            /* Update the status */
            data[param_offset++] = AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;

            /* Pack the UID counter */
            avrcp_pack_2_byte_metadata_param
            (
                &data[param_offset],
                &global_uid_counter
            );

            param_offset += 2U;

            /* No. of Items */
            avrcp_pack_4_byte_metadata_param
            (
                &data[param_offset],
                &root_folder_item_count
            );

            param_offset += 4U;

            /* Char set */
            avrcp_pack_2_byte_metadata_param
            (
                &data[param_offset],
                &char_set
            );

            param_offset += 2U;

            /* Folder depth */
            data[param_offset++] = 0x00U; /* Setting to ROOT folder */

            rsp_info.param_info     = data;
            rsp_info.param_info_len = param_offset;
        }

SEND_RSP:

        if (0x00U != reject)
        {
            rsp_info.param_info     = &rsp_status;
            rsp_info.param_info_len = 1U;
        }

        rsp_info.pdu_id    = AVRCP_PDU_ID_SET_BROWSED_PLAYER;
        rsp_info.tl        = brow_cmd_info->tl;

        if (API_SUCCESS == retval)
        {
            /* Send response */
            retval = BT_avrcp_al_send_browsing_cmd_rsp
                     (
                         &appl_avrcp_handle[inst],
                         &rsp_info
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
            }

            /* Free the memory */
            if (NULL != data)
            {
                BT_free_mem(data);
                data = NULL;
            }
        }
    }

    return;
}

void appl_avrcp_tg_handle_setpath
     (
         UCHAR                 inst,
         AVRCP_AL_BROW_CMD_INFO * brow_cmd_info
     )
{
    API_RESULT      retval;
    UCHAR           *cmd_info;
    UCHAR           param_offset;
    UCHAR           direction;
    UINT32          uid[2U];
    UINT16          uid_counter;
    UCHAR           rsp_data[5U], reject;
    AVRCP_AL_BROW_RSP_INFO    rsp_info;

    if (NULL == brow_cmd_info)
    {
        LOG_DEBUG ("NULL brow_cmd_info\n");
        /* return; */
    }
    else
    {
        reject = 0x00U;

        cmd_info      =  (UCHAR *)brow_cmd_info->brow_cmd_info;

       /* offset till parameter length */
        param_offset = 4U;

        LOG_DEBUG ("    PDU ID - Change Path (0x%x).\n", brow_cmd_info->pdu_id);

        /* Extract UID counter */
        avrcp_extract_2_byte_metadata_param
        (
            &cmd_info[param_offset],
            &uid_counter
        );
        param_offset += 2U;

        /* Set path direction: up or down */
        direction = cmd_info[param_offset++];

        /* Extract UID */
        avrcp_extract_4_byte_metadata_param
        (
            &cmd_info[param_offset],
            &uid[0U]
        );
        param_offset += 4U;

        avrcp_extract_4_byte_metadata_param
        (
            &cmd_info[param_offset],
            &uid[1U]
        );

        param_offset += 4U;

        LOG_DEBUG ("    UID Counter: 0x%04x\n", uid_counter);
        if (0x00U == direction)
        {
            LOG_DEBUG ("    Direction : Up(0x%0x)\n", direction);
        }
        else
        {
            LOG_DEBUG ("    Direction : Down(0x%0x)\n", direction);
        }

        LOG_DEBUG ("    UID: 0x%04x%04x\n", uid [0U], uid [1U]);

        /* ==== Pack the response === */

        /* Response status */
        rsp_data[0U] =  AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;

        if ((0xFFFFFFFFU == uid[0U]) &&
            (0xFFFFFFFFU == uid[1U]) &&
            (0x00U == curr_folder_depth))
        {
            reject = 0x01U;
            rsp_data[0U] = AVRCP_BOW_ERROR_NOT_A_DIRECTORY;
            goto SEND_RSP;
        }

        if ((AVRCP_VFS_CHANGE_PATH_DOWN == direction) &&
            (0U == curr_folder_depth))
        {
            curr_folder_depth = 1U;
        }
        else if ((AVRCP_VFS_CHANGE_PATH_UP == direction) &&
                 (1U == curr_folder_depth))
        {
            curr_folder_depth = 0U;
        }
        else
        {
            reject = 0x01U;
            rsp_data[0U] = AVRCP_BOW_ERROR_NOT_A_DIRECTORY;
            goto SEND_RSP;
        }

        /* Pack no. of items in current folder */
        if (1U == curr_folder_depth)
        {
            avrcp_pack_4_byte_metadata_param
            (
                &rsp_data[1U],
                &child_folder_item_count
            );
        }
        else
        {
            avrcp_pack_4_byte_metadata_param
            (
                &rsp_data[1U],
                &root_folder_item_count
            );
        }

        rsp_info.param_info_len = 5U;

SEND_RSP:

        rsp_info.param_info = rsp_data;
        if (0x00U != reject)
        {
            rsp_info.param_info_len = 1U;
        }

        rsp_info.pdu_id    = AVRCP_PDU_ID_CHANGE_PATH;
        rsp_info.tl        = brow_cmd_info->tl;

        retval = BT_avrcp_al_send_browsing_cmd_rsp
                 (
                     &appl_avrcp_handle[inst],
                     &rsp_info
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
        }
    }

    return;
}

#ifdef AVRCP_GET_NUM_OF_ITEMS

void appl_avrcp_tg_handle_get_total_number_of_items
     (
         UCHAR                 inst,
         AVRCP_AL_BROW_CMD_INFO * brow_cmd_info
     )
{
    API_RESULT      retval;
    UCHAR           *cmd_info;
    UCHAR           param_offset;
    UCHAR           scope;
    UINT32          num_items;
    UINT16          uid_counter;
    UCHAR           rsp_data[7U], reject;
    AVRCP_AL_BROW_RSP_INFO    rsp_info;

    if (NULL == brow_cmd_info)
    {
        LOG_DEBUG ("NULL brow_cmd_info\n");
        /* return; */
    }
    else
    {
        reject = 0x00U;
        num_items = 0U;

        cmd_info      =  (UCHAR *)brow_cmd_info->brow_cmd_info;

       /* offset till parameter length */
        param_offset = 4U;

        LOG_DEBUG ("    PDU ID - Get Total Number of Items (0x%x).\n", brow_cmd_info->pdu_id);

        /* Extract scope */
        scope = cmd_info[param_offset];

        LOG_DEBUG ("    Scope: ");
        switch(scope)
        {
        case AVRCP_SCOPE_MEDIA_PLAYER_LIST:
            LOG_DEBUG ("Media Player List\n");
            num_items   = num_players;

            break;

        case AVRCP_SCOPE_VIRTUAL_FILESYSTEM:
            LOG_DEBUG ("Virtual Filesystem\n");
            if (0x00U == curr_folder_depth)
            {
                num_items   = root_folder_item_count;
            }
            else if (0x01U == curr_folder_depth)
            {
                num_items   = child_folder_item_count;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            break;

        case AVRCP_SCOPE_SEARCH:
            LOG_DEBUG ("Search\n");
            num_items   = media_search_list_count;

            break;

        case AVRCP_SCOPE_NOW_PLAYING:
            LOG_DEBUG ("Now Playing\n");
            num_items   = media_list_count;
            break;

        default:
            reject = 0x01U;
            rsp_data[0U] = AVRCP_BOW_ERROR_INVALID_SCOPE;
            goto SEND_RSP;
        }

        uid_counter = global_uid_counter;

        /* Pack Response status */
        rsp_data[0U] =  AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;

        /* Pack UID counter */
        avrcp_pack_2_byte_metadata_param
        (
            &rsp_data[1U],
            &uid_counter
        );

        /* Pack No. of items in the requested scope */
        avrcp_pack_4_byte_metadata_param
        (
            &rsp_data[3U],
            &num_items
        );

        rsp_info.param_info_len = 7U;

SEND_RSP:

        rsp_info.param_info = rsp_data;
        if (0x00U != reject)
        {
            rsp_info.param_info_len = 1U;
        }

        rsp_info.pdu_id    = AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS;
        rsp_info.tl        = brow_cmd_info->tl;

        retval = BT_avrcp_al_send_browsing_cmd_rsp
                 (
                     &appl_avrcp_handle[inst],
                     &rsp_info
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
        }
    }

    return;
}
#endif /* AVRCP_GET_NUM_OF_ITEMS */

void appl_avrcp_tg_handle_search
     (
         UCHAR                 inst,
         AVRCP_AL_BROW_CMD_INFO * brow_cmd_info
     )
{
    API_RESULT      retval;
    UCHAR           *cmd_info;
    UCHAR           param_offset;
    UCHAR           i,j,num_items;
    UCHAR           rsp_data[7U], rsp_status, reject;
    UCHAR           search_str[APPL_AVRCP_MAX_STR_LEN];
    UINT16          char_set, search_strlen;
    AVRCP_AL_BROW_RSP_INFO    rsp_info;

    if (NULL == brow_cmd_info)
    {
        LOG_DEBUG ("NULL brow_cmd_info\n");
        /* return; */
    }
    else
    {
        reject = 0x00U;

        cmd_info      =  (UCHAR *)brow_cmd_info->brow_cmd_info;

        /* offset till parameter length */
        param_offset = 4U;

        LOG_DEBUG ("    PDU ID - Search (0x%x).\n", brow_cmd_info->pdu_id);

        /* Extract char set */
        avrcp_extract_2_byte_metadata_param
        (
            &cmd_info[param_offset],
            &char_set
        );

        param_offset += 2U;

        /* Extract the search string length */
        avrcp_extract_2_byte_metadata_param
        (
            &cmd_info[param_offset],
            &search_strlen
        );

        param_offset += 2U;

        /* Adjust the search_strlen */
        if (APPL_AVRCP_MAX_STR_LEN <= search_strlen)
        {
            search_strlen = (APPL_AVRCP_MAX_STR_LEN-1U);
        }

        /* Copy search string */
        BT_mem_copy(search_str, &cmd_info[param_offset], search_strlen);
        search_str[(search_strlen)] = (UCHAR)'\0';

        LOG_DEBUG ("     - Char Set : 0x%04x\n", char_set);
        LOG_DEBUG ("     - Search strlen : 0x%04x\n", search_strlen);
        LOG_DEBUG ("     - Search Str : %s\n", search_str);

        if (0x00U == curr_folder_depth)
        {
            num_items = (UCHAR )root_folder_item_count;
        }
        else if (0x01U == curr_folder_depth)
        {
            num_items = (UCHAR )root_folder_item_count;
        }
        else
        {
            /* reject */
            reject      = 0x01U;
            rsp_status  = AVRCP_BOW_ERROR_NO_VALID_SEARCH_RESULTS;
            goto SEND_RSP;
        }

        media_search_list_count = 0x00U;

        /* Search for string in vi_list_info */
        for (i = 0U; i < num_items; i++)
        {
            if (APPL_AVRCP_ITEM_TYPE_MEDIA == vf_list_info[curr_folder_depth][i].item_type)
            {
                if ((vf_list_info[curr_folder_depth][i].media_item_info.char_set == char_set) &&
                    (vf_list_info[curr_folder_depth][i].media_item_info.name_len >= search_strlen) &&
                    (0 == BT_mem_cmp
                          (
                              vf_list_info[curr_folder_depth][i].media_item_info.name,
                              search_str,
                              search_strlen
                          )))
                {
                    /* === Yes, found the required string == */

                    /* Copy the Media Element to Search List */
                    media_search_list_info[media_search_list_count].item_type  =
                                            vf_list_info[curr_folder_depth][i].item_type;
                    media_search_list_info[media_search_list_count].item_length  =
                                            vf_list_info[curr_folder_depth][i].item_length;
                    media_search_list_info[media_search_list_count].item_uid_msb  =
                                            vf_list_info[curr_folder_depth][i].item_uid_msb;
                    media_search_list_info[media_search_list_count].item_uid_lsb  =
                                            vf_list_info[curr_folder_depth][i].item_uid_lsb;

                    media_search_list_info[media_search_list_count].media_item_info.media_type =
                                            vf_list_info[curr_folder_depth][i].media_item_info.media_type;

                    media_search_list_info[media_search_list_count].media_item_info.char_set =
                                            vf_list_info[curr_folder_depth][i].media_item_info.char_set;

                    media_search_list_info[media_search_list_count].media_item_info.name_len  =
                                            vf_list_info[curr_folder_depth][i].media_item_info.name_len;

                    BT_mem_copy
                    (
                        media_search_list_info[media_search_list_count].media_item_info.name,
                        vf_list_info[curr_folder_depth][i].media_item_info.name,
                        vf_list_info[curr_folder_depth][i].media_item_info.name_len
                    );

                    media_search_list_info[media_search_list_count].media_item_info.num_attr  =
                                            vf_list_info[curr_folder_depth][i].media_item_info.num_attr;

                    /* Attributes IDs */
                    for (j = 0U; j < APPL_AVRCP_MAX_ATTR_COUNT; j++)
                    {
                        media_search_list_info[media_search_list_count].media_item_info.attr_info[j].attr_id  =
                                            vf_list_info[curr_folder_depth][i].media_item_info.attr_info[j].attr_id;

                        media_search_list_info[media_search_list_count].media_item_info.attr_info[j].char_set  =
                                            vf_list_info[curr_folder_depth][i].media_item_info.attr_info[j].char_set;

                        media_search_list_info[media_search_list_count].media_item_info.attr_info[j].att_val_len  =
                                            vf_list_info[curr_folder_depth][i].media_item_info.attr_info[j].att_val_len;

                        BT_mem_copy
                        (
                            media_search_list_info[media_search_list_count].media_item_info.attr_info[j].att_val,
                                            vf_list_info[curr_folder_depth][i].media_item_info.attr_info[j].att_val,
                                            vf_list_info[curr_folder_depth][i].media_item_info.attr_info[j].att_val_len
                        );
                    }

                    /* Increment the count */
                    media_search_list_count ++;
                }
            }
        }

        if (0x00U == media_search_list_count)
        {
            /* reject */
            reject      = 0x01U;
            rsp_status  = AVRCP_BOW_ERROR_NO_VALID_SEARCH_RESULTS;
            goto SEND_RSP;
        }

        /* Packing rsp data params */
        rsp_data[0U] = AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;

        avrcp_pack_2_byte_metadata_param
        (
            &rsp_data[1U],
            &global_uid_counter
        );

        avrcp_pack_4_byte_metadata_param
        (
            &rsp_data[3U],
            &media_search_list_count
        );

        rsp_info.param_info_len = 7U;

SEND_RSP:

        if (0x01U == reject)
        {
            rsp_data[0U] = rsp_status;
            rsp_info.param_info_len = 1U;
        }

        rsp_info.param_info     = rsp_data;
        rsp_info.pdu_id         = AVRCP_PDU_ID_SEARCH;
        rsp_info.tl             = brow_cmd_info->tl;

        retval = BT_avrcp_al_send_browsing_cmd_rsp
                 (
                     &appl_avrcp_handle[inst],
                     &rsp_info
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
        }
    }

    return;
}

/* This is a reference implementaiton. */
/* media_search_list_info will have all parameters of searched media list */
void appl_avrcp_tg_handle_get_search_list
     (
         UCHAR      inst,
         UINT32     start_idx,
         UINT32     end_idx,
         UCHAR      num_attr_ids,
         UINT32     *mediaattr_ids,
         UCHAR      tl
     )
{
    UINT16      total_items;
    UINT16      param_offset;
    UCHAR       *data;
    UINT16      data_len;
    UINT16      param_len;
    UCHAR       i;
    UINT16      attribute_len, item_len, item_len_index;
    API_RESULT  retval;
    AVRCP_AL_BROW_RSP_INFO    rsp_info;

    /* Init */
    data        = NULL;
    data_len    = 0x00U;
    total_items = 0U;

    /*
     * This is an assumption.
     * Actual value will depend on the  folder items to be sent.
     */
    param_len = APPL_AVRCP_MAX_BROW_PDU_PARAM_LEN;

    total_items = (UINT16 )((end_idx - start_idx) + 1U);

    if (total_items > media_search_list_count)
    {
        total_items = (UINT16 )media_search_list_count;
    }

    /*
     * data len:
     */
    data_len = param_len;

    /* Allocate a large chunk of data to form the response */
    data = (UCHAR *) BT_alloc_mem (data_len);
    if (NULL == data)
    {
        LOG_DEBUG ("Failed to Allocate the memory \n");
        /* return; */
    }
    else
    {
        /* Reset */
        BT_mem_set(data, 0, data_len);

        param_offset = 0x00U;

         /* Pack the status */
        data [param_offset++] = AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;

        /* Pack the UID counter */
        avrcp_pack_2_byte_metadata_param
        (
            &data [param_offset],
            &global_uid_counter
        );
        param_offset += 2U;

        /* Pack no. of items in search list */
        avrcp_pack_2_byte_metadata_param
        (
            &data [param_offset],
            &total_items
        );
        param_offset += 2U;

        for (i = (UCHAR )start_idx; i < (end_idx + 1U); i++)
        {
            /* Reset  length */
            item_len = 0U;
            item_len_index = 0U;
            attribute_len = 0x00U;

            /* === Pack search list params === */

            /* Item type */
            data [param_offset++] = media_search_list_info[i].item_type;

            /**
             * Item length will be packed after calculating item length
             * later below.
             */
#if 0
            avrcp_pack_2_byte_metadata_param
            (
                &data [param_offset],
                &media_search_list_info[i].item_length
            );

#endif /* 0 */

            /**
             * Save item length field index for filling correct
             * length later below
             */
            item_len_index = param_offset;

            /* For item length */
            param_offset += 2U;

            /* UID MSB */
            avrcp_pack_4_byte_metadata_param
            (
                &data [param_offset],
                &media_search_list_info[i].item_uid_msb
            );

            item_len += 4U;

            /* UID LSB */
            avrcp_pack_4_byte_metadata_param
            (
                &data [param_offset + item_len],
                &media_search_list_info[i].item_uid_lsb
            );

            item_len += 4U;

            attribute_len = appl_avrcp_tg_pack_media_element_item_info
                            (
                                &data [param_offset + item_len],
                                &media_search_list_info[i].media_item_info,
                                num_attr_ids,
                                mediaattr_ids
                            );

            /* Add item attribute length */
            item_len += attribute_len;

            /* Add item length */
            param_offset += item_len;

            /* Fill Item length */
            avrcp_pack_2_byte_metadata_param
            (
                &data [item_len_index],
                &item_len
            );
        }

        /* Update rsp params and its length */
        rsp_info.pdu_id         = AVRCP_PDU_ID_GET_FOLDER_ITEMS;
        rsp_info.tl             = tl;
        rsp_info.param_info     = data;
        rsp_info.param_info_len = param_offset;

        retval = BT_avrcp_al_send_browsing_cmd_rsp
                 (
                     &appl_avrcp_handle[inst],
                     &rsp_info
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
        }

         /* Free the memory */
        if (NULL != data)
        {
            BT_free_mem(data);
            data = NULL;
        }
    }

    return;
}


/* This is a reference implementaiton. */
/* media_list_info will have all parameters of now playing list */
void appl_avrcp_tg_handle_get_media_element_list
     (
         UCHAR      inst,
         UINT32     start_idx,
         UINT32     end_idx,
         UCHAR      num_attr_ids,
         UINT32     *mediaattr_ids,
         UCHAR      tl
     )
{
    UINT16      total_items;
    UINT16      param_offset;
    UCHAR       *data;
    UINT16      data_len;
    UINT16      param_len;
    UCHAR       i;
    UINT16      attribute_len, item_len, item_len_index;
    API_RESULT  retval;
    AVRCP_AL_BROW_RSP_INFO    rsp_info;

    /* Init */
    data        = NULL;
    data_len    = 0x00U;
    total_items = 0U;

    /**
     * This is an assumption.
     * Actual value will depend on the  folder items to be sent.
     */
    param_len = APPL_AVRCP_MAX_BROW_PDU_PARAM_LEN;

    total_items = (UINT16)((end_idx - start_idx) + 1U);

    /**
     * data len:
     */
    data_len = param_len;

    /* Allocate a large chunk of data to form the response */
    data = (UCHAR *) BT_alloc_mem (data_len);
    if (NULL == data)
    {
        LOG_DEBUG ("Failed to Allocate the memory \n");
        /* return; */
    }
    else
    {
        /* Reset */
        BT_mem_set(data, 0, data_len);

        param_offset = 0x00U;

         /* Pack the status */
        data [param_offset++] = AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;

        /* Pack the UID counter */
        avrcp_pack_2_byte_metadata_param
        (
            &data [param_offset],
            &global_uid_counter
        );
        param_offset += 2U;

        /* Pack no. of media element list */
        avrcp_pack_2_byte_metadata_param
        (
            &data [param_offset],
            &total_items
        );
        param_offset += 2U;

        for (i = (UCHAR )start_idx; i < (end_idx + 1U); i++)
        {
            /* Reset  length */
            item_len = 0U;
            item_len_index = 0U;
            attribute_len = 0x00U;

            /* === Pack Media Element List params === */

            /* Item type */
            data [param_offset++] = media_list_info[i].item_type;

            /**
             * Item length will be packed after calculating item length
             * later below.
             */
#if 0
            avrcp_pack_2_byte_metadata_param
            (
                &data [param_offset],
                &media_list_info[i].item_length
            );

#endif /* 0 */

            /**
             * Save item length field index for filling correct
             * length later below
             */
            item_len_index = param_offset;

            /* For item length */
            param_offset += 2U;

            /* UID MSB */
            avrcp_pack_4_byte_metadata_param
            (
                &data [param_offset],
                &media_list_info[i].item_uid_msb
            );

            item_len += 4U;


            /* UID LSB */
            avrcp_pack_4_byte_metadata_param
            (
                &data [param_offset + item_len],
                &media_list_info[i].item_uid_lsb
            );

            item_len += 4U;

            attribute_len = appl_avrcp_tg_pack_media_element_item_info
                            (
                                &data [param_offset + item_len],
                                &media_list_info[i].media_item_info,
                                num_attr_ids,
                                mediaattr_ids
                            );

            /* Add item attribute length */
            item_len += attribute_len;

            /* Add item length */
            param_offset += item_len;

            /* Fill Item length */
            avrcp_pack_2_byte_metadata_param
            (
                &data [item_len_index],
                &item_len
            );
        }

        /* Update rsp params and its length */
        rsp_info.pdu_id         = AVRCP_PDU_ID_GET_FOLDER_ITEMS;
        rsp_info.tl             = tl;
        rsp_info.param_info     = data;
        rsp_info.param_info_len = param_offset;

        retval = BT_avrcp_al_send_browsing_cmd_rsp
                 (
                     &appl_avrcp_handle[inst],
                     &rsp_info
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
        }

         /* Free the memory */
        if (NULL != data)
        {
            BT_free_mem(data);
            data = NULL;
        }
    }

    return;
}

void appl_avrcp_tg_handle_get_item_attributes
     (
         UCHAR                 inst,
         AVRCP_AL_BROW_CMD_INFO * brow_cmd_info
     )
{
    API_RESULT      retval;
    UCHAR           *cmd_info;
    UINT16          param_offset, attr_len;
    UCHAR           scope;
    UINT32          uid[2U];
    UINT16          uid_counter;
    UCHAR           rsp_status, reject, item_found;
    UCHAR           num_attrs, i, j, k;
    UINT32          attr_ids[APPL_AVRCP_MAX_ATTR_COUNT], attr_id;
    UINT16          param_len;
    UCHAR          *data;
    UINT16          data_len;
    AVRCP_AL_BROW_RSP_INFO    rsp_info;
    APPL_AVRCP_MEDIA_ELEMENT_ITEM_TYPE_INFO  *media_item_ptr;

    if (NULL == brow_cmd_info)
    {
        LOG_DEBUG ("NULL brow_cmd_info\n");
        /* return; */
    }
    else
    {
        /* Init */
        reject          = 0x00U;
        attr_len        = 0x00U;
        data            = NULL;
        media_item_ptr  = NULL;
        item_found      = 0x00U;
        data_len        = 0x00U;
        BT_mem_set (attr_ids, 0, (APPL_AVRCP_MAX_ATTR_COUNT * sizeof(UINT32)));
        retval = API_SUCCESS;

        cmd_info      =  (UCHAR *)brow_cmd_info->brow_cmd_info;

       /* offset till parameter length */
        param_offset = 4U;

        LOG_DEBUG ("    PDU ID - Get Item Attributes (0x%x).\n", brow_cmd_info->pdu_id);

        /* Extract Scope */
        scope = cmd_info[param_offset++];

        /* Extract UID */
        /* MSB UID */
        avrcp_extract_4_byte_metadata_param
        (
            &cmd_info[param_offset],
            &uid[0U]
        );
        param_offset += 4U;

        /* LSB UID */
        avrcp_extract_4_byte_metadata_param
        (
            &cmd_info[param_offset],
            &uid[1U]
        );

        param_offset += 4U;

        /* Extract UID Counter */
        avrcp_extract_2_byte_metadata_param
        (
            &cmd_info[param_offset],
            &uid_counter
        );
        param_offset += 2U;

        /* No. of Attributes */
        num_attrs = cmd_info[param_offset++];

        /* Extract the Attributes */
        for (i = 0U; i < num_attrs; i++)
        {
            avrcp_extract_4_byte_metadata_param
            (
                &cmd_info[param_offset],
                &attr_id
            );
            param_offset += 4U;

            if ((attr_id < AVRCP_MEDIA_ATTR_ID_MEDIA_TITLE) ||
#ifdef AVRCP_COVER_ART_RESPONDER
                (attr_id > AVRCP_MEDIA_ATTR_ID_DEFAULT_COVER_ART)
#else
                (attr_id > AVRCP_MEDIA_ATTR_ID_PLAYING_TIME)
#endif /* AVRCP_COVER_ART_RESPONDER */
                )
            {
                /* reject */
                reject = 0x01U;
                rsp_status = AVRCP_METADATA_ERROR_INVALID_PARAMETER;
                goto SEND_RSP;
            }
            else
            {
                attr_ids[attr_id-1U] = attr_id;
            }
        }

        if (0x00U == num_attrs)
        {
            /* Fill all the supported attr ID's  */
            for (i = 0U; i < APPL_AVRCP_MAX_ATTR_COUNT; i++)
            {
                attr_ids[i] = media_attr_ids[i];
            }

            num_attrs = APPL_AVRCP_MAX_ATTR_COUNT;

            /**
             *:
             * To skip Cover Art Handle in the response.
             * Few PTS testcases require not sent CoverArt Handles
             */
            if (0x00U != skip_cover_art_handle_get_folder_items_rsp)
            {
                attr_ids[7U] = 0x00U;
                num_attrs -= 1U;
            }
        }

#ifdef AVRCP_COVER_ART_RESPONDER
        /**
         * As per spec. CoverArt Attribute ID should be sent only when OBEX connection is
         * Active
         */
        retval = BT_avrcp_car_get_obex_connection_status(0U, &obex_connect_state);
        if ((API_SUCCESS == retval) && (BT_TRUE != obex_connect_state) &&
            (AVRCP_MEDIA_ATTR_ID_DEFAULT_COVER_ART == attr_ids[7U]))
        {
            attr_ids[7U] = 0x00U;
            num_attrs --;
        }
#endif /* AVRCP_COVER_ART_RESPONDER */

        /* Display */
        LOG_DEBUG ("     - Scope : %d\n", scope);
        LOG_DEBUG ("     - UID   : 0x0%04x%04x\n", uid[0U], uid[1U]);
        LOG_DEBUG ("     - UID counter : 0x%04x\n", uid_counter);
        LOG_DEBUG ("     - No. of Attributes: ");

        if (uid_counter != global_uid_counter)
        {
            LOG_DEBUG (" UID counter does not match\n");
            reject = 0x01U;
            rsp_status = AVRCP_METADATA_ERROR_UID_CHANGED;
            goto SEND_RSP;
        }

        if (0x00U == num_attrs)
        {
            LOG_DEBUG ("All Supported Attributes\n");
        }
        else if (0xFFU == num_attrs)
        {
            LOG_DEBUG ("Zero Atrributes\n");

            reject = 0x01U;
            rsp_status = AVRCP_METADATA_ERROR_INVALID_PARAMETER;
            goto SEND_RSP;
        }
        else if (num_attrs <= APPL_AVRCP_MAX_ATTR_COUNT)
        {
            LOG_DEBUG ("%d\n", num_attrs);

            for (i = 0U; i < APPL_AVRCP_MAX_ATTR_COUNT; i++)
            {
                if (0x00U != attr_ids[i])
                {
                    LOG_DEBUG ("      - %d\n", attr_ids[i]);
                }
            }

            /* To skip Cover Art Handle */
            for (i = 0U; i < APPL_AVRCP_MAX_ATTR_COUNT; i++)
            {
                if ((0x00U != skip_cover_art_handle_get_folder_items_rsp) &&
                    (AVRCP_MEDIA_ATTR_ID_DEFAULT_COVER_ART == attr_ids[i]))
                {
                    attr_ids[i] = 0x00U;
                    num_attrs -= 1U;
                    break;
                }
            }
        }
        else
        {
            LOG_DEBUG ("??\n");

            /* reject */
            reject = 0x01U;
            rsp_status = AVRCP_METADATA_ERROR_INVALID_PARAMETER;
            goto SEND_RSP;
        }

        if (scope > AVRCP_SCOPE_NOW_PLAYING)
        {
            /* reject */
            reject = 0x01U;
            rsp_status = AVRCP_BOW_ERROR_INVALID_SCOPE;
            goto SEND_RSP;
        }

        /**
         * This is an assumption.
         * Actual value will depend on the folder items to be sent.
         */
        param_len = APPL_AVRCP_MAX_BROW_PDU_PARAM_LEN;

        /**
         * data len:
         */
        data_len = param_len;

        /* Allocate a large chunk of data to form the response */
        data = (UCHAR *) BT_alloc_mem (data_len);
        if (NULL == data)
        {
            LOG_DEBUG ("Failed to Allocate the memory \n");
            retval = API_FAILURE; /* return */
        }
        else
        {
            /* Reset */
            BT_mem_set(data, 0, data_len);

            param_offset = 0x00U;

            /* === Pack the response === */

             /* Pack the status */
            data [param_offset++] = AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;

            /* Handling for AVRCP_SCOPE_VIRTUAL_FILESYSTEM, AVRCP_SCOPE_NOW_PLAYING */
            if (AVRCP_SCOPE_VIRTUAL_FILESYSTEM == scope)
            {
                for (i = 0U; i < APPL_AVRCP_MAX_VF_DEPTH; i++)
                {
                    for (j = 0U; j < APPL_AVRCP_MAX_FOLDER_ITEMS; j++)
                    {
                        if ((APPL_AVRCP_ITEM_TYPE_MEDIA == vf_list_info[i][j].item_type) &&
                            (uid[0U] == vf_list_info[i][j].item_uid_msb) &&
                            (uid[1U] == vf_list_info[i][j].item_uid_lsb))
                        {
                            item_found = 0x01U;
                            break;
                        }
                    }

                    if (0x01U == item_found)
                    {
                        break;
                    }
                }

                if (0x00U == item_found)
                {
                    /* Could not find the Media Item, send error */
                    reject = 0x01U;
                    rsp_status = AVRCP_BOW_ERROR_DOES_NOT_EXIST;
                    goto SEND_RSP;
                }

                /*  Assign the Media Item info pointer */
                media_item_ptr = &vf_list_info[i][j].media_item_info;
            }
            else if (AVRCP_SCOPE_NOW_PLAYING == scope)
            {
                /* Find the Media Item */
                for (i = 0U; i < APPL_AVRCP_MAX_MEDIA_LIST_COUNT; i++)
                {
                    if ((uid[0U] == media_list_info[i].item_uid_msb) &&
                        (uid[1U] == media_list_info[i].item_uid_lsb))
                    {
                        item_found = 0x01U;
                        break;
                    }
                }

                if (0x01U != item_found)
                {
                    /* Could not find the Media Item, send error */
                    reject = 0x01U;
                    rsp_status = AVRCP_BOW_ERROR_DOES_NOT_EXIST;
                    goto SEND_RSP;
                }

                /*  Assign the Media Item info pointer */
                media_item_ptr = &media_list_info[i].media_item_info;
            }
            else if (AVRCP_SCOPE_SEARCH == scope)
            {
                /* Find the Media Item */
                for (i = 0U; i < APPL_AVRCP_MAX_MEDIA_LIST_COUNT; i++)
                {
                    if ((uid[0U] == media_search_list_info[i].item_uid_msb) &&
                        (uid[1U] == media_search_list_info[i].item_uid_lsb))
                    {
                        item_found = 0x01U;
                        break;
                    }
                }

                if (0x01U != item_found)
                {
                    /* Could not find the Media Item, send error */
                    reject = 0x01U;
                    rsp_status = AVRCP_BOW_ERROR_DOES_NOT_EXIST;
                    goto SEND_RSP;
                }

                /*  Assign the Media Item info pointer */
                media_item_ptr = &media_search_list_info[i].media_item_info;
            }
            else
            {
                /* reject */
                reject = 0x01U;
                rsp_status = AVRCP_BOW_ERROR_INVALID_SCOPE;
                goto SEND_RSP;
            }

            if (0x00U == num_attrs)
            {
                data[param_offset++] = 0x00U;
            }
            else
            {
                /*
                 * Get No. of attributes matched
                 * between requested versus locally supported
                 */
                num_attrs = 0U;
                for (k = 0U; k < APPL_AVRCP_MAX_ATTR_COUNT; k++)
                {
                    for (j = 0U; j < APPL_AVRCP_MAX_ATTR_COUNT; j++)
                    {
                        if ((attr_ids[k] == media_item_ptr->attr_info[j].attr_id) &&
                            (0x00U != attr_ids[k]))
                            {
                                num_attrs++;
                                break;
                            }
                    }
                }

                data [param_offset++] = num_attrs;
            }

            /* Pack the attributes */
            for(j = 0U; j < APPL_AVRCP_MAX_ATTR_COUNT; j++)
            {
                for (k = 0U; k < APPL_AVRCP_MAX_ATTR_COUNT; k++)
                {
                    attr_len = 0U;
                    attr_id = attr_ids[j];
                    if ((media_item_ptr->attr_info[k].attr_id == attr_id) &&
                        (0x00U != attr_ids[j]))
                    {
                        attr_len = appl_avrcp_tg_pack_attr_id_info
                                   (
                                       &data[param_offset],
                                        attr_ids[j],
                                       &media_item_ptr->attr_info[attr_id-1U]
                                   );

                        param_offset += attr_len;

                        break;
                    }
                }
            }

            rsp_info.param_info     = data;
            rsp_info.param_info_len = param_offset;
        }

SEND_RSP:

        if (0x00U != reject)
        {
            rsp_info.param_info     = &rsp_status;
            rsp_info.param_info_len = 1U;
        }

        rsp_info.pdu_id    = AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES;
        rsp_info.tl        = brow_cmd_info->tl;

        if (API_SUCCESS == retval)
        {
            retval = BT_avrcp_al_send_browsing_cmd_rsp
                     (
                         &appl_avrcp_handle[inst],
                         &rsp_info
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
            }

            /* Free the memory */
            if (NULL != data)
            {
                BT_free_mem(data);
                data = NULL;
            }
        }
    }

    return;
}


#endif /* AVRCP_1_4 */


void appl_avrcp_tg_notify_change_rsp
     (
        UCHAR   inst
     )
{
    int                     choice, read_val;
    API_RESULT              retval;
    UCHAR                   play_status;
    UCHAR                   event_id, tl, i, reg_status;
    UCHAR                  *param;
    UCHAR                   rsp_status;
    UINT16                  param_len, offset;
    UINT32                  play_back_pos;
    AVRCP_AL_VD_CMD_INFO    rsp_pdu_info;

    LOG_DEBUG ("Select Event ID to be Generated\n");
    LOG_DEBUG ("    1. Playback Status Changed\n");
    LOG_DEBUG ("    2. Track Changed\n");
    LOG_DEBUG ("    3. Track Reached End\n");
    LOG_DEBUG ("    4. Track Reacged Start\n");
    LOG_DEBUG ("    5. Playback Pos Changed\n");
    LOG_DEBUG ("    6. Battery Status Changed\n");
    LOG_DEBUG ("    7. System Status Changed\n");
    LOG_DEBUG ("    8. Player Application Setting Changed\n");
    LOG_DEBUG ("    9. Now Playing Content Changed\n");
    LOG_DEBUG ("    a. Available Player Changed\n");
    LOG_DEBUG ("    b. Address Player Changed\n");
    LOG_DEBUG ("    c. UIDS Change\n");
    LOG_DEBUG ("    d. Volume Changed\n");
    LOG_DEBUG ("    e. Reject register notifications\n");

    fflush (stdout);
    /* Init */
    offset    = 0x00U;
    param     = NULL;
    param_len = 0U;
    tl        = 0U;
    retval = API_SUCCESS;

    scanf ("%x", &choice);
    event_id = (UCHAR)choice;
    reg_status = appl_mp_get_registered_player_event_reg_status(event_id);
    if (0x00U == reg_status)
    {
        printf("Event ID 0x%x is not registered for notification\n", event_id);
        choice = 0xFFU;
    }
    else
    {
        switch (event_id)
        {
        case AVRCP_EVENT_PLAYBACK_STATUS_CHANGED:
            printf ("   Enter the Play status\n");
            printf ("        0. Stopped\n");
            printf ("        1. Playing\n");
            printf ("        2. Paused\n");
            printf ("        3. FWD seek\n");
            printf ("        4. REV seek\n");
            printf ("        5. Error\n");
            printf ("    Enter you choice = ");
            fflush (stdout);
            scanf ("%d", &read_val);
            play_status = (UCHAR )read_val;

            param_len = 2U;
            tl = appl_mp_get_registered_player_event_tl (event_id);

            /* Allocate memory */
            param = BT_alloc_mem(param_len);
            if (NULL == param)
            {
                printf ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = event_id;
                param[offset] = play_status;

                /* Save the status */
                cur_song_play_status.song_play_status = play_status;
            }

            break;

        case AVRCP_EVENT_TRACK_CHANGED:

            /* Calculate parameter length */
            param_len = 9U;
            tl        = appl_mp_get_registered_player_event_tl (event_id);

            /* Allocate memory */
            param = BT_alloc_mem(param_len);
            if (NULL == param)
            {
                printf ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = event_id;

                /* Sending UID of 2nd element in media_list_info[] */
                avrcp_pack_4_byte_metadata_param
                (
                    &param[offset],
                    &media_list_info[1U].item_uid_msb
                );

                offset += 4U;

                avrcp_pack_4_byte_metadata_param
                (
                    &param[offset],
                    &media_list_info[1U].item_uid_lsb
                );

                offset += 4U;
            }

            break;

        case AVRCP_EVENT_TRACK_REACHED_END:
        case AVRCP_EVENT_TRACK_REACHED_START: /* Fall Through */

            /* Calculate parameter length */
            param_len = 1U;
            tl        = appl_mp_get_registered_player_event_tl (event_id);

            /* Allocate memory */
            param = BT_alloc_mem(param_len);
            if (NULL == param)
            {
                printf ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset] = event_id;
            }

            break;

        case AVRCP_EVENT_PLAYBACK_POS_CHANGED:
            printf ("    Enter the playback position = ");
            scanf ("%d", &read_val);
            play_back_pos = read_val;

            /* Calculate parameter length */
            param_len = 5U;
            tl        = appl_mp_get_registered_player_event_tl (event_id);

            /* Allocate memory */
            param = BT_alloc_mem(param_len);
            if (NULL == param)
            {
                printf ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = event_id;

                avrcp_pack_4_byte_metadata_param
                (
                    &param[offset],
                    &play_back_pos
                );

                offset += 4U;
            }

            break;

        case AVRCP_EVENT_BATT_STATUS_CHANGED:

            /* Calculate parameter length */
            param_len  = 2U;
            tl         = appl_mp_get_registered_player_event_tl (event_id);

            /* Allocate memory */
            param = BT_alloc_mem(param_len);
            if (NULL == param)
            {
                printf ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = event_id;
                param[offset] = battery_status;
            }

            break;

        case AVRCP_EVENT_SYSTEM_STATUS_CHANGED:

            /* Calculate parameter length */
            param_len  = 2U;
            tl         = appl_mp_get_registered_player_event_tl (event_id);

            /* Allocate memory */
            param = BT_alloc_mem(param_len);
            if (NULL == param)
            {
                printf ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = event_id;
                param[offset] = system_status;
            }

            break;

        case AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED:

            /* Note: Update player_appl_info using mplayer menu */

            /* No. Attr_id + (attr_id + attr_val) */
            param_len = 1U + 1U + 2U * (UINT16)(player_appl_info.no_attr);
            tl        = appl_mp_get_registered_player_event_tl (event_id);

            /* Allocate memory */
            param = BT_alloc_mem(param_len);
            if (NULL == param)
            {
                printf ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = event_id;
                param[offset++] = player_appl_info.no_attr;

                for (i = 0U; i < player_appl_info.no_attr; i++)
                {
                    param[offset++] = player_appl_info.attr_id_info[i].attr_id;
                    param[offset++] = player_appl_info.attr_id_info[i].cur_attr_val.val;
                }
            }

            break;

        case AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED:
            /* Calculate parameter length */
            param_len = 1U;
            tl        = appl_mp_get_registered_player_event_tl (event_id);

            /* Allocate memory */
            param = BT_alloc_mem(param_len);
            if (NULL == param)
            {
                printf ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset] = event_id;
            }

            break;

        case AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED:
            /* Calculate parameter length */
            param_len  = 1U;
            tl         = appl_mp_get_registered_player_event_tl (event_id);

            /* Allocate memory */
            param = BT_alloc_mem(param_len);
            if (NULL == param)
            {
                printf ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset] = event_id;
            }

            break;

        case AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED:
            /* Calculate parameter length */
            param_len = 5U;
            tl        = appl_mp_get_registered_player_event_tl (event_id);

            /* Allocate memory */
            param = BT_alloc_mem(param_len);
            if (NULL == param)
            {
                printf ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = event_id;

                if (current_player_id == addressed_player_id)
                {
                    current_player_id = browsed_player_id;
                }
                else
                {
                    current_player_id = addressed_player_id;
                }

                avrcp_pack_2_byte_metadata_param
                (
                    &param[offset],
                    &current_player_id
                );

                offset += 2U;

                avrcp_pack_2_byte_metadata_param
                (
                    &param[offset],
                    &global_uid_counter
                );

                offset += 2U;
            }

            break;

        case AVRCP_EVENT_UIDS_CHANGED:
            /* Calculate parameter length */
            param_len = 3U;
            tl        = appl_mp_get_registered_player_event_tl (event_id);

            /* Allocate memory */
            param = BT_alloc_mem(param_len);
            if (NULL == param)
            {
                printf ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {
                param[offset++] = event_id;

                /* Update the UID counter and send */
                global_uid_counter++;

                avrcp_pack_2_byte_metadata_param
                (
                    &param[offset],
                    &global_uid_counter
                );

                offset += 2U;
            }
            break;

        case AVRCP_EVENT_VOLUME_CHANGED:
            /* Calculate parameter length */
            param_len = 2U;
            tl        = appl_mp_get_registered_player_event_tl (event_id);

            /* Allocate memory */
            param = BT_alloc_mem(param_len);
            if (NULL == param)
            {
                printf ("Failed to allocate memory\n");
                retval = API_FAILURE; /* return; */
            }
            else
            {

                param[offset++] = event_id;
                param[offset] = system_volume;
            }

            break;

        case 0x0EU:

            choice = 0xFE;

            printf ("Select the Event ID to reject\n");
            printf ("    1. Playback Status Changed\n");
            printf ("    2. Track Changed\n");
            printf ("    3. Track Reached End\n");
            printf ("    4. Track Reacged Start\n");
            printf ("    5. Playback Pos Changed\n");
            printf ("    6. Battery Status Changed\n");
            printf ("    7. System Status Changed\n");
            printf ("    8. Player Application Setting Changed\n");
            printf ("    9. Now Playing Content Changed\n");
            printf ("    a. Available Player Changed\n");
            printf ("    b. Address Player Changed\n");
            printf ("    c. UIDS Change\n");
            printf ("    d. Volume Changed\n");

            fflush (stdout);
            scanf ("%x", &read_val);
            event_id = (UCHAR )read_val;

            tl        = appl_mp_get_registered_player_event_tl (event_id);

            break;

        default:
            printf ("Invalid Choice\n");
            choice = 0xFF;
            break;
        }
    }
    /**
     * For invalide choice, not sending pdu
     */
    if (0xFFU == choice)
    {
        retval = API_FAILURE;
        printf("Not sending pdu\n");
    }
    else if (0xFEU == choice)
    {
        rsp_status = AVRCP_BOW_ERROR_ADDR_PLAYER_CHANGED;

        rsp_pdu_info.cmd_type    = AVRCP_RESPONSE_TYPE_REJECTED;

        rsp_pdu_info.vd_cmd_data    = &rsp_status;
        rsp_pdu_info.vd_cmd_datalen = 1U;
    }
    else
    {
        rsp_pdu_info.cmd_type    = AVRCP_RESPONSE_TYPE_CHANGED;

        rsp_pdu_info.vd_cmd_data    = param;
        rsp_pdu_info.vd_cmd_datalen = param_len;
        appl_mp_un_register_player_event_reg_status(event_id);
    }

    if (API_SUCCESS == retval)
    {
        rsp_pdu_info.tl          = tl;
        rsp_pdu_info.pdu_id      = AVRCP_METADATA_PDU_ID_REGISTER_NOTIFICATION;
        rsp_pdu_info.packet_type = AVRCP_METADATA_PACKET_TYPE_SINGLE;

    
        retval = BT_avrcp_al_send_metadata_pdu
                 (
                     &appl_avrcp_handle[inst],
                     &rsp_pdu_info,
                     0x01U
                 );

        /* Free the memory */
        if (NULL != param)
        {
            BT_free_mem(param);
            param = NULL;
        }
    }

    BT_IGNORE_UNUSED_PARAM(retval);
    return;
}


#ifdef AVRCP_COVER_ART_RESPONDER
/* Cover Art application callback */
API_RESULT appl_avrcp_car_callback
            (
                AVRCP_CA_HANDLE     *avrcp_ca_handle,
                UINT8                event_type,
                UINT16               event_result,
                AVRCP_CA_HEADERS     *avrcp_ca_hdrs,
                UINT16               num_headers
            )
{
    API_RESULT retval;
    AVRCP_CA_HANDLE             handle;
    AVRCP_CA_HEADERS           *ca_headers;
    UCHAR                       send_response;
    UCHAR                       tx_response;
    UINT16                      i;
    AVRCP_CA_HEADERS            ca_rsp_hdrs;
    AVRCP_CA_RESPONSE_STRUCT    rsp_hdrs;
    AVRCP_CA_HEADER_STRUCT      ca_body_info;
    AVRCP_CA_HEADER_STRUCT      ca_length_info;
    UINT16                      actual;

    BT_IGNORE_UNUSED_PARAM(num_headers);

    retval = API_SUCCESS;

    ca_headers  = avrcp_ca_hdrs;
    handle      = *avrcp_ca_handle;

    ca_rsp_hdrs.ca_rsp_info = &rsp_hdrs;

    /* Init response headers */
    rsp_hdrs.body = &ca_body_info;
    rsp_hdrs.length = &ca_length_info;

    CA_INIT_HEADER_STRUCT(ca_body_info);
    CA_INIT_HEADER_STRUCT(ca_length_info);

    tx_response = (UCHAR) event_result;
    send_response = 0U;

    if (handle >= AVRCP_CAR_NUM_ENTITIES)
    {
        LOG_DEBUG ("AVRCP_TG Cover Art Handle value out of range\n");
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        switch(event_type)
        {
        case AVRCP_CAR_TRANSPORT_CLOSE_IND: /* Fall Through */
        case AVRCP_CAR_TRANSPORT_CLOSE_CNF:
            LOG_DEBUG ("Recvd %s - %04X\n", (event_type == AVRCP_CAR_TRANSPORT_CLOSE_CNF) ?
            "AVRCP_CAR_TRANSPORT_CLOSE_CNF":"AVRCP_CAR_TRANSPORT_CLOSE_IND", event_result);
            LOG_DEBUG ("AVRCP-TG Cover Art Handle - 0x%02X\n", handle);

            break;

        case AVRCP_CAR_CONNECT_IND:
            LOG_DEBUG ("Recvd AVRCP_CAR_CONNECT_IND - %04X\n", event_result);
            LOG_DEBUG ("AVRCP-TG Cover Art Handle - 0x%02X\n", handle);

            if ((AVRCP_CA_SUCCESS_RSP == event_result) &&
                (NULL == ca_headers->ca_connect_info))
            {
                tx_response = AVRCP_CA_NOT_ACCEPTABLE_RSP;
                send_response = 1U;
                break;
            }

            LOG_DEBUG("Peer Address " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (ca_headers->ca_connect_info->bd_addr));

            LOG_DEBUG ("MAX Exchange Size: %d\n",
                ca_headers->ca_connect_info->max_recv_size);

            cover_art_xchg_size = ca_headers->ca_connect_info->max_recv_size;

            /* Set response to be sent */
            send_response = 1U;
            tx_response = AVRCP_CA_SUCCESS_RSP;
            sent        = 0U;

            /**
             * Enable to send Reject response when peer does Get Image request without
             * getting the Image handle through browsing channel after OBEX connection.
             */
            appl_ca_get_img_reject_rsp = BT_TRUE;

            break;

        case AVRCP_CAR_DISCONNECT_IND:
            LOG_DEBUG ("Recvd AVRCP_CAR_DISCONNECT_IND - 0x%04X\n", event_result);
            LOG_DEBUG ("AVRCP-CT Cover Art Handle - 0x%02X\n", handle);

            break;

        case AVRCP_CAR_GET_IMAGE_PROPERTIES_IND:
            LOG_DEBUG ("Recvd AVRCP_CAR_GET_IMAGE_PROPERTIES_IND - %04X\n",
                event_result);
            LOG_DEBUG ("AVRCP-TG Cover Art Handle - 0x%02X\n", handle);

            /* Set response to be sent */
            send_response = 1U;
            BT_mem_set(img_properties, 0, sizeof(img_properties));

            if (sent == 0U)
            {
                if((NULL == ca_headers->ca_req_info->img_handle) ||
                   (NULL == ca_headers->ca_req_info->img_handle->value) ||
                   (0U  == ca_headers->ca_req_info->img_handle->length) ||
                   (8U < ca_headers->ca_req_info->img_handle->length))
                {
                    LOG_DEBUG ("Invalid Image Handle \n");

                    tx_response = AVRCP_CA_BAD_REQ_RSP;
                    break;
                }

                LOG_DEBUG ("Image Handle: %s\n",
                    ca_headers->ca_req_info->img_handle->value);

                /* TODO: Check if the image handle is valid */

                /* Update rsp status */
                tx_response = AVRCP_CA_SUCCESS_RSP;

                /* Form image properties object for the given image handle */
                BT_str_print((CHAR *)img_properties,
                    "<image-properties version=\"1.0\" handle="
                    "\"%s\"> \n"
                    "<native encoding=\"JPEG\" pixel=\"640*480\" size=\"10240\"/>\n"
                    "<variant encoding=\"GIF\" pixel=\"640*480\"/>\n"
                    "<variant encoding=\"JPEG\" pixel=\"200*200\"/>\n"
                    "<variant encoding=\"JPEG\" pixel=\"75*75\"/>\n"
                    "</image-properties>",
                    ca_headers->ca_req_info->img_handle->value);

                /* Get the length of image properties object */
                image_prop_obj_size = (UINT16)BT_str_n_len(img_properties, sizeof(img_properties)-1);

                remaining = image_prop_obj_size;
            }

            rsp_hdrs.body  = &ca_body_info;

            /* Get length */
            if (remaining > cover_art_xchg_size)
            {
                ca_body_info.length = cover_art_xchg_size;

                tx_response = AVRCP_CA_CONTINUE_RSP;
            }
            else if (remaining != 0U)
            {
                ca_body_info.length = (UINT16 )remaining;

                tx_response = AVRCP_CA_SUCCESS_RSP;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            /* Allocate Memory */
            if(0U != ca_body_info.length)
            {
                ca_body_info.value = BT_alloc_mem(ca_body_info.length);
                if (NULL != ca_body_info.value)
                {
                    /* Copy image properties object */
                    BT_mem_copy (ca_body_info.value, img_properties, ca_body_info.length);
                }
            }

            sent += ca_body_info.length;
            remaining = (UINT32)image_prop_obj_size - sent;

            if (remaining == 0U)
            {
                image_prop_obj_size = 0U;
                sent = 0U;
            }

            break;

        case AVRCP_CAR_GET_IMAGE_IND:

            LOG_DEBUG ("Recvd AVRCP_CAR_GET_IMAGE_IND - %04X\n",
                event_result);
            LOG_DEBUG ("AVRCP-TG Cover Art Handle - 0x%02X\n", handle);

            /* Set response to be sent */
            send_response = 1U;

            if (BT_TRUE == appl_ca_get_img_reject_rsp)
            {
                tx_response = AVRCP_CA_BAD_REQ_RSP;
                break;
            }

            if (sent == 0U)
            {
                if((NULL == ca_headers->ca_req_info->img_handle) ||
                   (NULL == ca_headers->ca_req_info->img_handle->value) ||
                   (0U    == ca_headers->ca_req_info->img_handle->length))
                {
                    LOG_DEBUG ("Invalid Image Handle \n");

                    tx_response = AVRCP_CA_BAD_REQ_RSP;
                    break;
                }

                LOG_DEBUG ("Image Handle: %s\n",
                    ca_headers->ca_req_info->img_handle->value);

                /* Native image to send */
                BT_str_copy(image_file_name, IMAGE_640_480_JPEG);

                /* Image Decriptor handle */
                if (NULL != ca_headers->ca_req_info->img_descriptor)
                {
                    if ((NULL != ca_headers->ca_req_info->img_descriptor->value) &&
                        (0U    != ca_headers->ca_req_info->img_descriptor->length))
                    {
                        UCHAR *data;
                        UINT16 len;

                        data = ca_headers->ca_req_info->img_descriptor->value;
                        len  = ca_headers->ca_req_info->img_descriptor->length;

                        LOG_DEBUG ("\n========== Cover Art Image Descriptor ==========\n");
                        for (i = 0U; i < len; i++)
                        {
                            LOG_DEBUG ("%c", data[i]);
                        }
                        LOG_DEBUG ("\n======================================\n");

#ifdef HAVE_OBJECT_DUMP
                        /* Open file to dump raw message */
                        if (NULL == ca_img_descriptor_fp)
                        {
                            img_descriptor_count++;
                            BT_str_print((CHAR *)cover_art_file_name, "ca_tg_image_descriptor_%d.txt", img_descriptor_count);

                            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                            (void)BT_vfops_create_object_name
                            (
                                (UCHAR *)APPL_AVRCP_BASE_FOLDER,
                                cover_art_file_name,
                                avrcp_tg_file_object
                            );

                            /* ca_img_descriptor_fp = fopen ((CHAR *)cover_art_file_name, "wb"); */
                            retval = BT_fops_file_open(avrcp_tg_file_object, (UCHAR *)"wb", &ca_img_descriptor_fp);
                        }

                        /* Dump the raw message */
                        if (NULL != ca_img_descriptor_fp)
                        {
                            /* fwrite (data, len, 1, ca_img_descriptor_fp); */
                            retval = BT_fops_file_write(data, len, ca_img_descriptor_fp, &actual);
                        }

                        /* Close the file */
                        if (NULL != ca_img_descriptor_fp)
                        {
                            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                            (void)BT_fops_file_close(ca_img_descriptor_fp);
                            ca_img_descriptor_fp = NULL;
                        }
#endif /* HAVE_OBJECT_DUMP */

                        /* Get Image Descriptor fields */
                        retval = appl_avrcp_car_parse_image_descriptor
                                 (
                                     data,
                                     len,
                                     &image_params
                                 );
                        if (retval == API_SUCCESS)
                        {
                            if ((COVER_ART_FORMAT_JPEG == image_params.format) &&
                                (200U == image_params.img_height) &&
                                (200U == image_params.img_width))
                            {
                                /* Image file to send */
                                BT_str_copy(image_file_name, IMAGE_200_200_JPEG);
                            }
                            else if ((COVER_ART_FORMAT_JPEG == image_params.format) &&
                                (480U == image_params.img_height) &&
                                (640U == image_params.img_width))
                            {
                                /* Image file to send */
                                BT_str_copy(image_file_name, IMAGE_640_480_JPEG);
                            }
                            else if ((COVER_ART_FORMAT_GIF == image_params.format) &&
                                (480U == image_params.img_height) &&
                                (640U == image_params.img_width))
                            {
                                /* Image file to send */
                                BT_str_copy(image_file_name, IMAGE_640_480_GIF);
                            }
                            else if ((COVER_ART_FORMAT_JPEG == image_params.format) &&
                                (75U ==  image_params.img_height) &&
                                (75U == image_params.img_width))
                            {
                                /* Image file to send */
                                BT_str_copy(image_file_name, IMAGE_75_75_JPEG);
                            }
                            else
                            {
                                /* Send Reject Response */
                               tx_response = AVRCP_CA_BAD_REQ_RSP;
                               break;
                            }
                        }
                        else
                        {
                            tx_response = AVRCP_CA_NOT_FOUND_RSP;
                            break;
                        }
                    }
                }

                /* Update rsp status */
                tx_response = AVRCP_CA_SUCCESS_RSP;

                if (NULL == image_fp)
                {
                    /* Open the file */
                    /* image_fp = fopen ((CHAR *)image_file_name, "rb"); */

                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)APPL_AVRCP_BASE_FOLDER,
                        image_file_name,
                        avrcp_tg_file_object
                    );

                    retval = BT_fops_file_open(avrcp_tg_file_object, (UCHAR *)"rb", &image_fp);

                    if (NULL != image_fp)
                    {

#if 0
                        /* Get the file size */
                        BT_fops_file_seek(image_fp, 0L, SEEK_END);
                        image_file_size = (UINT16)ftell(image_fp);
                        rewind (image_fp);
#else
                        (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(image_fp, &image_file_size);
#endif /* 0 */

                        remaining = image_file_size;

                        LOG_DEBUG("%s file open success\n", avrcp_tg_file_object);
                        LOG_DEBUG("File size: %d \n", image_file_size);
                    }
                    else
                    {
                        LOG_DEBUG("%s file open failed\n", avrcp_tg_file_object);
                        LOG_DEBUG("Sending OBEX_SERVER_ERR_RSP response\n");
                        tx_response = OBEX_SERVER_ERR_RSP;
                        break;
                    }
                }

                /* Send Length hdr only in first response */
                rsp_hdrs.body  = &ca_body_info;
                rsp_hdrs.length = &ca_length_info;

                /* Update the Length header */
                ca_length_info.length = 4U;
                ca_length_info.value = BT_alloc_mem(ca_length_info.length);
                if (NULL != ca_length_info.value)
                {
                    /* copy the data */
                    ca_length_info.value[0U] = (UCHAR )(image_file_size >> 24U);
                    ca_length_info.value[1U] = (UCHAR )(image_file_size >> 16U);
                    ca_length_info.value[2U] = (UCHAR )(image_file_size >> 8U);
                    ca_length_info.value[3U] = (UCHAR )image_file_size;
                }
            }

            /* Get length */
            if (remaining > cover_art_xchg_size)
            {
                ca_body_info.length = cover_art_xchg_size;

                tx_response = AVRCP_CA_CONTINUE_RSP;
            }
            else if (remaining != 0U)
            {
                ca_body_info.length = (UINT16 )remaining;

                tx_response = AVRCP_CA_SUCCESS_RSP;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            /* Allocate Memory */
            if(0U != ca_body_info.length)
            {
                ca_body_info.value = BT_alloc_mem(ca_body_info.length);
            }

            /* Read data */
            if(NULL != ca_body_info.value)
            {
                /* fread(ca_body_info.value, ca_body_info.length, 1, image_fp); */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read(ca_body_info.value, ca_body_info.length, image_fp, &actual);
            }

            sent += ca_body_info.length;
            remaining = image_file_size - sent;

            if (0U == remaining)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(image_fp);
                image_fp = NULL;

                image_file_size = 0U;
                sent = 0U;
            }
            break;

        case AVRCP_CAR_GET_LINKED_THUMBNAIL_IND:
            LOG_DEBUG ("Recvd AVRCP_CAR_GET_LINKED_THUMBNAIL_IND - %04X\n",
                event_result);
            LOG_DEBUG ("AVRCP-TG Cover Art Handle - 0x%02X\n", handle);

            /* Set response to be sent */
            send_response = 1U;

            if (sent == 0U)
            {
                if((NULL == ca_headers->ca_req_info->img_handle) ||
                   (NULL == ca_headers->ca_req_info->img_handle->value) ||
                   (0U    == ca_headers->ca_req_info->img_handle->length))
                {
                    LOG_DEBUG ("Invalid Image Handle \n");

                    tx_response = AVRCP_CA_BAD_REQ_RSP;
                    break;
                }

                LOG_DEBUG ("Image Handle: %s\n",
                    ca_headers->ca_req_info->img_handle->value);

                /* Update rsp status */
                tx_response = AVRCP_CA_SUCCESS_RSP;

                if (NULL == image_thmbnail_fp)
                {
                    /* Open the file */
                    /* image_thmbnail_fp = fopen (IMAGE_THUMBNAIL_FILE, "rb"); */

                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_vfops_create_object_name
                    (
                        (UCHAR *)APPL_AVRCP_BASE_FOLDER,
                        IMAGE_THUMBNAIL_FILE,
                        avrcp_tg_file_object
                    );

                    retval = BT_fops_file_open(avrcp_tg_file_object, (UCHAR *)"rb", &image_thmbnail_fp);

                    if (NULL != image_thmbnail_fp)
                    {

#if 0
                        /* Get the file size */
                        BT_fops_file_seek(image_thmbnail_fp, 0L, SEEK_END);
                        image_thmbnail_file_size = (UINT16)ftell(image_thmbnail_fp);
                        rewind (image_thmbnail_fp);
#else
                        (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(image_thmbnail_fp, &image_thmbnail_file_size);
#endif /* 0 */

                        remaining = image_thmbnail_file_size;

                        LOG_DEBUG("%s file open success\n", avrcp_tg_file_object);
                        LOG_DEBUG("File size: %d \n", image_thmbnail_file_size);
                    }
                    else
                    {
                        LOG_DEBUG("%s file open failed\n", avrcp_tg_file_object);
                        LOG_DEBUG("Sending OBEX_SERVER_ERR_RSP response\n");
                        tx_response = OBEX_SERVER_ERR_RSP;
                        break;
                    }
                }
            }

            rsp_hdrs.body  = &ca_body_info;

            /* Get length */
            if (remaining > cover_art_xchg_size)
            {
                ca_body_info.length = cover_art_xchg_size;

                tx_response = AVRCP_CA_CONTINUE_RSP;
            }
            else if (remaining != 0U)
            {
                ca_body_info.length = (UINT16 )remaining;

                tx_response = AVRCP_CA_SUCCESS_RSP;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            /* Allocate Memory */
            if(0U != ca_body_info.length)
            {
                ca_body_info.value = BT_alloc_mem(ca_body_info.length);
            }

            /* Read data */
            if(NULL != ca_body_info.value)
            {
                /* fread(ca_body_info.value, ca_body_info.length, 1, image_thmbnail_fp); */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read(ca_body_info.value, ca_body_info.length, image_thmbnail_fp, &actual);
            }

            sent += ca_body_info.length;
            remaining = image_thmbnail_file_size - sent;

            if (0U == remaining)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(image_thmbnail_fp);
                image_thmbnail_fp = NULL;

                image_thmbnail_file_size = 0U;
                sent = 0U;
            }

            break;

        case AVRCP_CAR_GET_ABORT_IND:

            LOG_DEBUG ("Recvd AVRCP_CAR_GET_ABORT_IND - %04X\n",
                     event_result);
            LOG_DEBUG ("AVRCP-TG Cover Art Handle - 0x%02X\n", handle);

            break;

        default:

            LOG_DEBUG ("Recvd UnKnown Cover Art Event - %02X\n",
            event_type);

            LOG_DEBUG ("AVRCP-TG Cover Art Handle - 0x%02X\n", handle);

            break;

        }

        /* Send response if required */
        if (0U != send_response)
        {
            if (0U < wait_count)
            {
                tx_response |= OBEX_WAIT_MASK;
                wait_count--;
            }

            retval = BT_avrcp_car_send_response
                     (
                         &handle,
                         event_type,
                         tx_response,
                         &ca_rsp_hdrs
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG ("Failed to send AVRCP_TG Cover Art Response - 0x%05X\n", retval);
            }

            if (NULL != ca_body_info.value)
            {
                BT_free_mem (ca_body_info.value);
                ca_body_info.value = NULL;

                /* MISRA C - 2012 Rule 2.2 */
                /* ca_body_info.length = 0U; */
            }

            if (NULL != ca_length_info.value)
            {
                BT_free_mem (ca_length_info.value);
                ca_length_info.value = NULL;

                /* MISRA C - 2012 Rule 2.2 */
                /* ca_length_info.length = 0U; */
            }
        }
    }

    return retval;
}

API_RESULT appl_avrcp_car_parse_image_descriptor
           (
               UCHAR *img_desc_str,
               UINT16 img_desc_str_len,
               AVRCP_CA_IMAGE_DESCRIPTOR *img_desc_obj
           )
{
    API_RESULT      retval;
    UCHAR           pixel[32U];
    UCHAR           height[10U];
    UCHAR           width[10U];
    UCHAR           height1[10U];
    UCHAR           width1[10U];
    UCHAR          *pch;
    UCHAR           flag, ch;
    UINT16          i, j;

    retval = API_SUCCESS;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(pixel, 0, sizeof(pixel));
    BT_mem_set(height, 0, sizeof(height));
    BT_mem_set(width, 0, sizeof(width));
    BT_mem_set(height1, 0, sizeof(height1));
    BT_mem_set(width1, 0, sizeof(width1));

    /* Validate the parameters */
    if ((NULL == img_desc_obj) ||
        (NULL == img_desc_str) ||
        (0U == img_desc_str_len))
    {
        LOG_DEBUG ("Invalid parameters \n");
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        img_desc_str[img_desc_str_len - 1U] = (UCHAR)'\0';

        /* Check for image format */
        if ((NULL != BT_str_str(img_desc_str, "JPEG")) ||
            (NULL != BT_str_str(img_desc_str, "jpeg")))
        {
            img_desc_obj->format = COVER_ART_FORMAT_JPEG;
        }
        else if ((NULL != BT_str_str(img_desc_str, "GIF")) ||
                 (NULL != BT_str_str(img_desc_str, "gif")))
        {
            img_desc_obj->format = COVER_ART_FORMAT_GIF;
        }
        else
        {
            img_desc_obj->format = COVER_ART_FORMAT_INVALID;

            /* Image format not found  */
            /* return API_FAILURE; */
        }

        /* Check for  "pixel" string */
        pch = (UCHAR *)BT_str_str((CHAR *)img_desc_str, "pixel");
        if (NULL != pch)
        {
            pch += 5U;
        }
        else
        {
            /* pixel string not found */
            retval = API_FAILURE; /* return API_FAILURE; */
        }

        if (API_SUCCESS == retval)
        {
            i = 0U;
            j = 0U;

            flag = 0U;

            /* Extract image dimensions to an array */
            while((pch[i] != '\0'))
            {
                ch = pch[i];

                if (ch == '"')
                {
                    i++;
                    while(pch[i] != '"')
                    {
                        pixel[j] = pch[i];

                        j++;
                        i++;
                    }

                    pixel[j] = (UCHAR)'\0';
                    flag = 0x1U;
                }

                if (flag != 0x00U)
                {
                    break;
                }

                i++;
            }

            /* LOG_DEBUG ("pixel: %s\n", pixel);    */

            i = 0U;

            /* Extract Image Length and Width from pixel array */

            /* Extract the width */
            for (j = 0U;
                ((pixel[j] != '*') &&
                 (pixel[j] != '\0'));
                    j++)
            {
                width[j] = pixel[j];
            }

            width[j++] = (UCHAR)'\0';

            /* Extract the height */
            for(;
                ((pixel[j] != '\0') &&
                 (pixel[j] != '*')  &&
                 (pixel[j] != '-'));
                j++)
            {
                height[i] = pixel[j];
                i++;
            }

            height[i] = (UCHAR)'\0';

            if (pixel[j] == '*')
            {
               /* for '*' */
                j++;
            }

            /* Check if pixel attribute range is requested */
            if (pixel[j] == '-')
            {
                /* Yes */

                /* for '-' */
                j++;

                /* Extract the width */
                i = 0U;
                for (;
                    ((pixel[j] != '*') &&
                     (pixel[j] != '\0'));
                    j++)
                {
                    width1[i] = pixel[j];
                    i++;
                }
                width1[i] = (UCHAR)'\0';

                j++;

                /* Extract the height */
                i = 0U;
                for (;
                    (pixel[j] != '\0');
                    j++)
                {
                    height1[i] = pixel[j];
                    i++;
                }
                height1[i] = (UCHAR)'\0';

#if 0
                /* Check width range */
                i = (UINT16)atoi ((CHAR *)width);
                j = (UINT16)atoi ((CHAR *)width1);
#else
                i = (UINT16)appl_str_to_num(width, (UINT16)BT_str_len(width));
                j = (UINT16)appl_str_to_num(width1, (UINT16)BT_str_len(width1));
#endif /* 0 8 */

                if (i > j)
                {
                    img_desc_obj->format = COVER_ART_FORMAT_INVALID;

                    /* return API_SUCCESS; */
                }
                else
                {
#if 0
                    /* Check height range */
                    i = (UINT16)atoi ((CHAR *)height);
                    j = (UINT16)atoi ((CHAR *)height1);
#else
                    i = (UINT16)appl_str_to_num(height, (UINT16)BT_str_len(height));
                    j = (UINT16)appl_str_to_num(height1, (UINT16)BT_str_len(height1));
#endif

                    if (i > j)
                    {
                        img_desc_obj->format = COVER_ART_FORMAT_INVALID;

                        /* return API_SUCCESS; */
                    }
                }

                if (COVER_ART_FORMAT_INVALID == img_desc_obj->format)
                {
                    img_desc_obj->format = COVER_ART_FORMAT_JPEG;
                }

                /* Set the default size */
                img_desc_obj->img_height = 480U;
                img_desc_obj->img_width = 640U;

                /* return API_SUCCESS; */
            }
            else
            {

#if 0
                /* Converting length and width to integers */
                /* TODO: Use abstracted API calls */
                img_desc_obj->img_height = (UINT16)atoi((CHAR *)height);
                img_desc_obj->img_width = (UINT16)atoi((CHAR *)width);
#else
                /* Converting length and width to integers */
                img_desc_obj->img_height = (UINT16)appl_str_to_num(height, (UINT16)BT_str_len(height));
                img_desc_obj->img_width = (UINT16)appl_str_to_num(width, (UINT16)BT_str_len(width));
#endif /* 0 */
            }
        }
    }

    return retval;
}

#endif /* AVRCP_COVER_ART_RESPONDER */

#endif /* AVRCP_TG */
