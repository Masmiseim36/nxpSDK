
/**
 *  \file appl_avrcp_tg.h
 *
 *  Header file for AVRCP Target Command Line Test Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */


#ifndef _H_APPL_AVRCP_TG_
#define _H_APPL_AVRCP_TG_

/* ----------------------------------------- Header File Inclusion */
#ifdef AVRCP_COVER_ART_RESPONDER
#include "BT_avrcp_ca_api.h"
#endif /* AVRCP_COVER_ART_RESPONDER */

#include "BT_avrcp_al_api.h"

#include "appl_avrcp_tg_virtual_media_player.h"


/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

#define AVRCP_TG_MAX_FILE_OBJ_NAME_LEN                 190U
#define APPL_AVRCP_MAX_BROW_PDU_PARAM_LEN              (AVCTP_DEFAULT_BROW_CH_L2CAP_MTU - 16U)
#define APPL_MAX_CA_IMAGE_PROPERTIES_LEN               256U

/* ----------------------------------------- Static Global Variables */

/* ----------------------------------------- Functions */
/* AVRCP Target Application Menu */
void main_avrcp_tg_operations (void);

void appl_avrcp_tg_init (void);

void appl_avrcp_tg_shutdown (void);

void appl_avrcp_tg_start (UCHAR index);

void appl_avrcp_tg_stop (UCHAR index);

void appl_avrcp_tg_handle_message_cmd
     (
         UCHAR inst,
         UCHAR *event_data,
         UINT16 event_datalen
     );

#ifdef AVRCP_1_3

void appl_avrcp_tg_handle_vendor_dependent_cmd
     (
         UCHAR       inst,
         void      * event_data,
         UINT16      event_datalens
     );

#endif /* AVRCP_1_3 */

#ifdef AVRCP_1_4

void appl_avrcp_tg_handle_browsing_cmd
     (
         UCHAR    * event_data,
         UINT16   event_datalen
     );

#endif /* AVRCP_1_4 */

void appl_avrcp_tg_send_get_capability_cmd_rsp
     (
         UCHAR       cap_id,
         UCHAR       inst,
         UCHAR       tl
     );

void appl_avrcp_tg_send_player_appl_setting_cmd_rsp
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     );

void appl_avrcp_tg_send_get_element_attributes_cmd_rsp
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     );

void appl_avrcp_tg_send_get_play_status_cmd_rsp
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     );

void appl_avrcp_tg_send_notify_cmd_interim_rsp
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     );

void appl_avrcp_tg_send_set_addressed_player_cmd_rsp
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     );

void appl_avrcp_tg_send_set_absolute_volume_cmd_rsp
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     );

void appl_avrcp_tg_send_play_item_rsp
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     );

void appl_avrcp_tg_handle_get_folder_items_req
     (
         UCHAR               inst,
         AVRCP_AL_BROW_CMD_INFO * brow_cmd_info
     );

void appl_avrcp_tg_handle_get_media_player_list
     (
        UCHAR      inst,
        UINT32     start_idx,
        UINT32     end_idx,
        UCHAR      tl
     );

void appl_avrcp_tg_handle_get_virtual_filesystem_list
     (
         UCHAR      inst,
         UINT32     start_idx,
         UINT32     end_idx,
         UCHAR      num_attr_ids,
         UINT32     *media_attr_ids,
         UCHAR      tl
     );

API_RESULT appl_avrcp_tg_pack_folder_item_info
           (
               UCHAR *data,
               APPL_AVRCP_FOLDER_ITEM_TYPE_INFO *info
           );

API_RESULT appl_avrcp_tg_pack_media_element_item_info
           (
               UCHAR *data,
               APPL_AVRCP_MEDIA_ELEMENT_ITEM_TYPE_INFO *info,
               UCHAR num_attr_ids,
               UINT32 *attr_ids
           );

API_RESULT appl_avrcp_tg_pack_attr_id_info
           (
               UCHAR *data,
               UINT32 attr_id,
               APPL_AVRCP_MEDIA_ITEM_ATTR_INFO *info
           );

void appl_avrcp_tg_handle_setpath
     (
         UCHAR                 inst,
         AVRCP_AL_BROW_CMD_INFO * brow_cmd_info
     );

void appl_avrcp_tg_handle_get_search_list
     (
         UCHAR      inst,
         UINT32     start_idx,
         UINT32     end_idx,
         UCHAR      num_attr_ids,
         UINT32     *media_attr_ids,
         UCHAR      tl
     );

void appl_avrcp_tg_handle_get_media_element_list
     (
         UCHAR      inst,
         UINT32     start_idx,
         UINT32     end_idx,
         UCHAR      num_attr_ids,
         UINT32     *media_attr_ids,
         UCHAR      tl
     );

void appl_avrcp_tg_handle_set_browsed_player
     (
         UCHAR                 inst,
         AVRCP_AL_BROW_CMD_INFO * brow_cmd_info
     );

#ifdef AVRCP_GET_NUM_OF_ITEMS
void appl_avrcp_tg_handle_get_total_number_of_items
     (
         UCHAR                 inst,
         AVRCP_AL_BROW_CMD_INFO * brow_cmd_info
     );
#endif /* AVRCP_GET_NUM_OF_ITEMS */

void appl_avrcp_tg_handle_get_item_attributes
     (
         UCHAR                 inst,
         AVRCP_AL_BROW_CMD_INFO * brow_cmd_info
     );

void appl_avrcp_tg_handle_search
     (
         UCHAR                 inst,
         AVRCP_AL_BROW_CMD_INFO * brow_cmd_info
     );

void appl_avrcp_tg_notify_change_rsp (UCHAR    inst);

void appl_avrcp_tg_handle_request_continue_response
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     );

void appl_avrcp_tg_handle_abort_continue_response
     (
         UCHAR                  inst,
         AVRCP_AL_VD_CMD_INFO * vd_cmd_info
     );

#ifdef AVRCP_COVER_ART_RESPONDER
API_RESULT appl_avrcp_car_callback
           (
               AVRCP_CA_HANDLE     *avrcp_ca_handle,
               UINT8                event_type,
               UINT16               event_result,
               AVRCP_CA_HEADERS    *avrcp_ca_hdrs,
               UINT16               num_headers
           );

API_RESULT appl_avrcp_car_parse_image_descriptor
           (
               UCHAR *img_desc_str,
               UINT16 img_desc_str_len,
               AVRCP_CA_IMAGE_DESCRIPTOR *img_desc_obj
           );
#endif /* AVRCP_COVER_ART_RESPONDER */

#endif /* _H_APPL_AVRCP_TG_ */
