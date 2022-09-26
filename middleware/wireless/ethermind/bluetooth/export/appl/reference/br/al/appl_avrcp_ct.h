/**
 *  \file appl_avrcp_ct.h
 *
 *  Header file for AVRCP Controller Command Line Test Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */


#ifndef _H_APPL_AVRCP_CT_
#define _H_APPL_AVRCP_CT_

/* ----------------------------------------- Header File Inclusion */
#include "BT_sdp_api.h"
#include "appl_utils.h"
#ifdef AVRCP_COVER_ART
#include "BT_avrcp_ca_api.h"
#endif /* AVRCP_COVER_ART */

#include "BT_avrcp_al_api.h"
#include "appl_avrcp_tg_virtual_media_player.h"

/* ----------------------------------------- External Global Variables */
/* Image Descriptor with pixels attribute described as a range */
#define IMAGE_DESCRIPTOR_DEFAULT "<image-descriptor version=\"1.0\" > \n\
<pixel = \"0*0-65535*65535\" /> \n\
</image-descriptor>"

/* Max. file object name length */
#define AVRCP_CT_MAX_FILE_OBJ_NAME_LEN             190U

/* Resourced file name constants */
#define APPL_AVRCP_CT_BASE_FOLDER \
        BT_FOPS_PATH_JOIN(BT_FOPS_BASE, "data" BT_FOPS_PATH_SEP "avrcp" BT_FOPS_PATH_SEP "avrcp_ct")

#define  APPL_AVRCP_CT_IMG_PRPRTS_FILE      "ca_ct_image_properties.txt"

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */

/* ----------------------------------------- Functions */

/* AVRCP Controller Application Menu */
void main_avrcp_ct_operations (void);

void appl_avrcp_ct_init (void);

void appl_avrcp_ct_shutdown (void);

void appl_avrcp_ct_start (UCHAR index);

void appl_avrcp_ct_stop (UCHAR index);

void appl_avrcp_ct_send_unit_info_cmd (UCHAR index);

void appl_avrcp_ct_handle_message_cnf
     (
         UCHAR *    event_data,
         UINT16     event_datalen
     );

void appl_avrcp_ct_handle_vendor_dependent_cmd_rsp
     (
         UCHAR       inst,
         void      * event_data,
         UINT16      event_datalen
     );

void appl_avrcp_ct_send_passthrough_cmd
     (
         UCHAR index,
         UCHAR button_press_count
     );

void appl_avrcp_ct_send_get_capability_cmd (UCHAR index);

void appl_avrcp_ct_send_player_appl_setting_cmd (UCHAR index);

void appl_avrcp_ct_send_get_element_attibutes_cmd (UCHAR index);

void appl_avrcp_ct_send_notification_cmd (UCHAR index);

void appl_avrcp_ct_send_continuation_cmd (UCHAR index);

void appl_avrcp_ct_send_set_absolute_volume_cmd (UCHAR index);

void appl_avrcp_ct_send_set_addressed_player_cmd (UCHAR index);

void appl_avrcp_ct_send_item_operations (UCHAR   index);

void appl_avrcp_ct_send_browsing_cmd (UCHAR index);

void appl_avrcp_ct_send_set_browsed_player (UCHAR index);

void appl_avrcp_ct_handle_brow_cmd_rsp
     (
         UCHAR       inst,
         void      * event_data,
         UINT16      event_datalen
     );


void appl_avrcp_ct_handle_get_folder_items_rsp
     (
            AVRCP_AL_BROW_RSP_INFO  *brow_rsp_info
     );

void appl_avrcp_ct_handle_get_media_player_list_rsp
     (
         UCHAR    *mp_list_info,
         UINT16    mp_list_param_len
     );

void appl_avrcp_ct_display_media_player_list (void);

void appl_avrcp_ct_print_error (UCHAR status_code);

void appl_avrcp_ct_handle_set_addresss_player_rsp
     (
         AVRCP_AL_VD_CMD_INFO  *rsp_info
     );

void appl_avrcp_ct_display_vd_cmd_rsp_info
     (
         AVRCP_AL_VD_CMD_INFO  *rsp_info
     );

void appl_avrcp_ct_display_notify_rsp
     (
         UCHAR      event_id,
         UCHAR     *notify_param,
         UINT16     notify_param_len
     );

API_RESULT appl_avrcp_ct_reset_virtual_folder_items (void);

void appl_avrcp_ct_handle_get_virtual_filesytem_rsp
     (
         UCHAR    *vf_info,
         UINT16    vf_param_len
     );

API_RESULT appl_avrcp_ct_unpack_folder_item_info
           (
               UCHAR *data,
               APPL_AVRCP_FOLDER_ITEM_TYPE_INFO *info
           );

API_RESULT appl_avrcp_ct_unpack_media_element_item_info
           (
               UCHAR *data,
               APPL_AVRCP_MEDIA_ELEMENT_ITEM_TYPE_INFO *info
           );

API_RESULT appl_avrcp_ct_display_virtual_filesystem_items (void);

void appl_avrcp_ct_handle_get_media_element_list_rsp
     (
         UCHAR    *media_list_info,
         UINT16    media_list_info_len
     );

void appl_avrcp_ct_reset_media_element_list (void);

API_RESULT appl_avrcp_ct_display_media_element_list (void);

void appl_avrcp_ct_handle_set_brow_player_rsp
     (
            AVRCP_AL_BROW_RSP_INFO  *brow_rsp_info
     );

void appl_avrcp_ct_handle_get_item_attributes_rsp
     (
            AVRCP_AL_BROW_RSP_INFO  *brow_rsp_info
     );

#ifdef AVRCP_COVER_ART_INITIATOR

void appl_avrcp_cai_operation (void);

void appl_avrcp_cai_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     );

API_RESULT appl_avrcp_cai_callback
           (
               AVRCP_CA_HANDLE     *avrcp_ca_handle,
               UINT8                event_type,
               UINT16               event_result,
               AVRCP_CA_HEADERS   * avrcp_ca_headers,
               UINT16               num_headers
           );

#endif /* AVRCP_COVER_ART_INITIATOR */

#endif /* _H_APPL_AVRCP_CT_ */
