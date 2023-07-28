
/**
 *  \file appl_avrcp_tg_virtual_media_player.h
 *
 *  Header file for AVRCP TG Virtual Media Player Test Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_AVRCP_VIRTUAL_MEDIA_PLAYER_
#define _H_APPL_AVRCP_VIRTUAL_MEDIA_PLAYER_
/* ----------------------------------------- Header File Inclusion */
#include "BT_os.h"
#include "BT_error.h"
#include "BT_fops.h"

/* ----------------------------------------- Global Definitions */

#define     APPL_AVRCP_MAX_COMPANY_ID_COUNT                 10U
#define     APPL_AVRCP_MAX_PLAYER_EVENT_COUNT               13U

#define     APPL_AVRCP_MAX_PLAYER_ATTR_COUNT                8U
#define     APPL_AVRCP_MAX_PLAYER_ATTR_VAL_COUNT            5U

#define     APPL_AVRCP_MAX_STR_LEN                          64U

#ifdef      AVRCP_COVER_ART
#define     APPL_AVRCP_MAX_ATTR_COUNT                       8U
#else
#define     APPL_AVRCP_MAX_ATTR_COUNT                       7U
#endif      /* AVRCP_COVER_ART */

#define     APPL_AVRCP_MAX_ATTR_ID_LEN                      512U /* 255U */

#define     APPL_AVRCP_MAX_NUM_PLAYERS                      3U

#define     APPL_AVRCP_MAX_VF_DEPTH                         2U
#define     APPL_AVRCP_MAX_FOLDER_ITEMS                     8U

#define     APPL_AVRCP_MAX_MEDIA_LIST_COUNT                 8U

#define     APPL_AVRCP_MAX_SEARCH_LIST_COUNT                8U

#define     APPL_AVRCP_DEFAULT_UID_COUNTER                  0x1234U

/* Browsable Item types */
#define     APPL_AVRCP_ITEM_TYPE_MEDIA_PLAYER               0x01U
#define     APPL_AVRCP_ITEM_TYPE_FOLDER                     0x02U
#define     APPL_AVRCP_ITEM_TYPE_MEDIA                      0x03U

/* Resourced file name constants */
#define APPL_AVRCP_BASE_FOLDER \
        BT_FOPS_PATH_JOIN(BT_FOPS_BASE, "data" BT_FOPS_PATH_SEP "avrcp")

#define APPL_AVRCP_TG_COMMAND_ID_FILE  \
        (UCHAR *)BT_FOPS_PATH_JOIN(APPL_AVRCP_BASE_FOLDER,"avrcp_cap_company_id_list.dat")

#define APPL_AVRCP_TG_EVENT_ID_FILE  \
        (UCHAR *)BT_FOPS_PATH_JOIN(APPL_AVRCP_BASE_FOLDER,"avrcp_cap_event_id_list.dat")

#define APPL_AVRCP_TG_PLAYER_APPL_ATTR_INFO_FILE  \
        (UCHAR *)BT_FOPS_PATH_JOIN(APPL_AVRCP_BASE_FOLDER,"avrcp_player_appl_attr_info.dat")

#define APPL_AVRCP_TG_MEDIA_ELEMENT_ATTR_INFO_FILE  \
        (UCHAR *)BT_FOPS_PATH_JOIN(APPL_AVRCP_BASE_FOLDER,"avrcp_media_element_attr_info.dat")

#define APPL_AVRCP_TG_MEDIA_ELEMENT_ATTR_INFO_FILE_1  \
        (UCHAR *)BT_FOPS_PATH_JOIN(APPL_AVRCP_BASE_FOLDER,"avrcp_media_element_attr_info_new_track.dat")

#define APPL_AVRCP_TG_MEDIA_ELEMENT_LARGE_ATTR_INFO_FILE  \
        (UCHAR *)BT_FOPS_PATH_JOIN(APPL_AVRCP_BASE_FOLDER,"avrcp_media_element_attr_info_large.dat")

#define APPL_AVRCP_TG_CUR_MEDIA_PLAY_STATUS_FILE  \
        (UCHAR *)BT_FOPS_PATH_JOIN(APPL_AVRCP_BASE_FOLDER,"avrcp_player_cur_media_play_status.dat")

#define APPL_AVRCP_TG_MEDIA_PLAYER_LIST_FILE  \
        (UCHAR *)BT_FOPS_PATH_JOIN(APPL_AVRCP_BASE_FOLDER,"avrcp_media_player_list_info.dat")

#define APPL_AVRCP_TG_ROOT_FOLDER_ITEMS_LIST_FILE  \
        (UCHAR *)BT_FOPS_PATH_JOIN(APPL_AVRCP_BASE_FOLDER,"avrcp_root_folder_items_list.dat")

#define APPL_AVRCP_TG_FOLDER_DEPTH_1_ITEM_LIST_FILE  \
        (UCHAR *)BT_FOPS_PATH_JOIN(APPL_AVRCP_BASE_FOLDER,"avrcp_folder_depth_1_items_list.dat")

#define APPL_AVRCP_TG_MEDIA_ELEMENT_LIST_FILE  \
        (UCHAR *)BT_FOPS_PATH_JOIN(APPL_AVRCP_BASE_FOLDER,"avrcp_media_element_list_file.dat")

#define APPL_AVRCP_TG_NEW_MEDIA_ELEMENT_FILE  \
        (UCHAR *)BT_FOPS_PATH_JOIN(APPL_AVRCP_BASE_FOLDER,"avrcp_new_media_element_file.dat")

/* Cover Art Image File Names */
#define     IMAGE_THUMBNAIL_FILE                (UCHAR *)"Image_75_75.jpg"
#define     IMAGE_640_480_JPEG                  (UCHAR *)"Image_640_480.jpg"
#define     IMAGE_640_480_GIF                   (UCHAR *)"Image_640_480.gif"
#define     IMAGE_200_200_JPEG                  (UCHAR *)"Image_200_200.jpg"
#define     IMAGE_75_75_JPEG                    (UCHAR *)"Image_75_75.jpg"

/* ----------------------------------------- Structures/Data Types */

typedef struct _APPL_AVRCP_COMPANY_ID_LIST
{
    /* No. of IDs */
    UCHAR       no_ids;

    /* Company id list */
    UINT32      c_id_list[APPL_AVRCP_MAX_COMPANY_ID_COUNT];

}APPL_AVRCP_COMPANY_ID_LIST;


typedef struct _APPL_AVRCP_EVENT_ID_LIST
{
    /* No. of IDs */
    UCHAR       no_ids;

    /* Event ID list */
    UCHAR      e_id_list[APPL_AVRCP_MAX_PLAYER_EVENT_COUNT];

}APPL_AVRCP_EVENT_ID_LIST;

/* Attibute id value description */
typedef struct _APPL_AVRCP_ATTR_ID_VAL_DESC
{
    /* Attribute value */
    UCHAR val;

    /* Attibute value in text form */
    UCHAR val_text[32];

}APPL_AVRCP_ATTR_ID_VAL_DESC;

/* Media player attribute id information */
typedef struct _APPL_PLAYER_ATTR_ID_INFO
{
    /* Attribute ID */
    UCHAR   attr_id;

    /* Attribute ID in text form  */
    UCHAR   attr_id_text[32];

    /* Attribute id value count */
    UCHAR   attr_id_val_count;

    /* Attribute id value info. */
    APPL_AVRCP_ATTR_ID_VAL_DESC attr_id_val[APPL_AVRCP_MAX_PLAYER_ATTR_VAL_COUNT];

    /* Current Attibute ID value */
    APPL_AVRCP_ATTR_ID_VAL_DESC cur_attr_val;

}APPL_PLAYER_ATTR_ID_INFO;

/* Media Player Application Setting information */
typedef struct _APPL_AVRCP_PLAYER_APPL_SETTING_INFO
{
    /* No. of attributes */
    UCHAR               no_attr;

    /* Player attribute ID info. */
    APPL_PLAYER_ATTR_ID_INFO attr_id_info[APPL_AVRCP_MAX_PLAYER_ATTR_COUNT];

}APPL_AVRCP_PLAYER_APPL_SETTING_INFO;

/* Media item attributes information */
typedef struct _APPL_AVRCP_ATTRIBUTES_INFO
{
    /* attribute id */
    UINT32  attr_id;

    /* attibute character set id */
    UINT16  attr_char_set;

    /* attribute length */
    UINT16  attr_len;

    /* attibute value */
    UCHAR   attr_str[APPL_AVRCP_MAX_ATTR_ID_LEN];

}APPL_AVRCP_ATTRIBUTES_INFO;

/* Media Element Attibute information */
typedef struct _APPL_AVRCP_MEDIA_ELEMENT_ATTRIBUTES
{
    /* No. of attibutes */
    UCHAR               num_attr;

    /* Media Item attibutes information */
    APPL_AVRCP_ATTRIBUTES_INFO     attr_id_info[APPL_AVRCP_MAX_ATTR_COUNT];

}APPL_AVRCP_MEDIA_ELEMENT_ATTRIBUTES;

/* Media Item playing status information */
typedef struct _APPL_AVRCP_MEDIA_PLAY_STATUS
{
    /* Item total length(in msec) */
    UINT32      song_total_len;

    /* Item current position(in msec) */
    UINT32      song_cur_pos;

    /**
     * Item current status:
     * 0x00 -> STOPPED
     * 0x01 -> PLAYING
     * 0x02 -> PAUSED
     * 0x03 -> FWD_SEEK
     * 0x04 -> REV_SEEK
     * 0xFF -> ERROR
     */
    UCHAR       song_play_status;

}APPL_AVRCP_MEDIA_PLAY_STATUS;

/* Media Player Item fields */
typedef struct _APPL_AVRCP_MEDIA_PLAYER_LIST_INFO
{
    /**
     * Item type:
     *  0x01 -> Media Player
     */
    UCHAR       item_type;

    /* Media player item length */
    UINT16      item_len;

    /* Player ID */
    UINT16      player_id;

    /**
     * Media player Major Type (bit mask):
     * Bit 0(0x01) -> Audio
     * Bit 1(0x02) -> Video
     * Bit 2(0x04) -> Broadcasting Audio
     * Bit 3(0x08) -> Broadcasting Video
     * Bit 4- 7    -> Reserved
     */
    UCHAR       major_player_type;

    /**
     * Media Player Sub Type (bit mask)
     * Bit 0(0x00000001) -> Audio Book
     * Bit 1(0x00000002) -> Podcast
     * Bit 2-31          -> Reserved
     */
    UINT32      player_sub_type;

    /**
     * Media Player Status:
     *
     * 0x00 -> Stopped
     * 0x01 -> Playing
     * 0x02 -> Paused
     * 0x03 -> Fwd Seek
     * 0x04 -> Rev Seek
     * 0xFF -> Error
     * 0x05 - 0xFE -> Reserved
     */
    UCHAR       play_status;

    /**
     * Media player feature bit mask(16 octets)
     * Refer AVRCP specification
     */
    UINT32      features_bit_mask[4];

    /**
     * Charact set ID to be displayed on CT
     */
    UINT16      char_set;

    /* Media Player Displayable Name length */
    UINT16      name_len;

    /* Media Player Displayable Name */
    UCHAR       name[APPL_AVRCP_MAX_STR_LEN];

}APPL_AVRCP_MEDIA_PLAYER_LIST_INFO;

/* Folder Item type information */
typedef struct  _APPL_AVRCP_FOLDER_ITEM_TYPE_INFO
{
    /**
     * Folder Type:
     * 0x00  -> Mixed
     * 0x01  -> Titles
     * 0x02  -> Albums
     * 0x03  -> Artists
     * 0x04  -> Genres
     * 0x05  -> Playlists
     * 0x06  -> Years
     * 0x07 - 0xFF -> Reserved.
     */
    UCHAR       folder_type;

    /**
     * Item playable?
     * 0x00 -> Not playable
     * 0x01 -> Playable
     * 0x02 - 0xFF -> Reserved
     */
    UCHAR       is_playable;

    /**
     * Charact set ID to be displayed on CT
     */
    UINT16      char_set;

    /* Folder Displayable Name length */
    UINT16      name_len;

    /* Folder Displayable Name */
    UCHAR       name[APPL_AVRCP_MAX_STR_LEN];

}APPL_AVRCP_FOLDER_ITEM_TYPE_INFO;

/* Media Item Attribute information */
typedef struct _APPL_AVRCP_MEDIA_ITEM_ATTR_INFO
{
    /* Attribute ID */
    UINT32      attr_id;

    /**
     * Charact set ID to be displayed on CT
     */
    UINT16      char_set;

    /* Attribute value length */
    UINT16      att_val_len;

    /* Attribute value */
    UCHAR       att_val[APPL_AVRCP_MAX_STR_LEN];

}APPL_AVRCP_MEDIA_ITEM_ATTR_INFO;

/* Media Eelement Item type information */
typedef struct  _APPL_AVRCP_MEDIA_ELEMENT_ITEM_TYPE_INFO
{
    /**
     * Media Type:
     * 0x00  -> Audio
     * 0x01  -> Video
     * 0x02 - 0xFF -> Reserved
     */
    UCHAR       media_type;

    /**
     * Charact set ID to be displayed on CT
     */
    UINT16      char_set;

    /* Media Item Displayable Name length */
    UINT16      name_len;

    /* Media Item Displayable Name */
    UCHAR       name[APPL_AVRCP_MAX_STR_LEN];

    /* No. of Attributes of Media Item */
    UCHAR       num_attr;

    /* Media Item Attribute information */
    APPL_AVRCP_MEDIA_ITEM_ATTR_INFO attr_info[APPL_AVRCP_MAX_ATTR_COUNT];

}APPL_AVRCP_MEDIA_ELEMENT_ITEM_TYPE_INFO;

/* Virtual Folder Item information */
/* TODO: remove structs inside another structure */
typedef struct  _APPL_AVRCP_VIRTUAL_FOLDER_LIST_INFO
{
    /**
     * Item type:
     * 0x02 -> Folder Item
     * 0x03 -> Media Item
     */
    UCHAR       item_type;

    /* Folder Item length */
    UINT16      item_length;

    /* Folder or Media Item Unique ID(MSB) */
    UINT32      item_uid_msb;

    /* Folder or Media Item Unique ID (LSB) */
    UINT32      item_uid_lsb;

    /**
     * if (item_type == 0x02) then
     * Folder Item type information
     */
    APPL_AVRCP_FOLDER_ITEM_TYPE_INFO folder_item_info;

    /**
     * if (item_type == 0x03) then
     * Media Eelement Item type information
     */
    APPL_AVRCP_MEDIA_ELEMENT_ITEM_TYPE_INFO media_item_info;

}APPL_AVRCP_VIRTUAL_FOLDER_LIST_INFO;

typedef struct _APPL_AVRCP_MEDIA_LIST_INFO
{
    /**
     * Item type:
     * 0x03 -> Media Item
     */
    UCHAR   item_type;

    /* Media Item length */
    UINT16  item_length;

    /* Media Item Unique ID(MSB) */
    UINT32  item_uid_msb;

    /* Media Item Unique ID(LSB) */
    UINT32  item_uid_lsb;

    /* Media Eelement Item type information */
    APPL_AVRCP_MEDIA_ELEMENT_ITEM_TYPE_INFO media_item_info;
}APPL_AVRCP_MEDIA_LIST_INFO;

typedef struct _APPL_AVRCP_PLAYER_EVENT
{
    /* Event registration status */
    UCHAR reg_status;

    /* Transaction Label */
    UCHAR avrcp_tl;

}APPL_AVRCP_PLAYER_EVENT;

/* ----------------------------------------- External Global Variables */
extern APPL_AVRCP_COMPANY_ID_LIST             company_id_list;
extern APPL_AVRCP_EVENT_ID_LIST               event_id_list;
extern APPL_AVRCP_PLAYER_APPL_SETTING_INFO    player_appl_info;
extern APPL_AVRCP_MEDIA_ELEMENT_ATTRIBUTES    media_element_info;
extern APPL_AVRCP_MEDIA_PLAY_STATUS           cur_song_play_status;

extern APPL_AVRCP_MEDIA_PLAYER_LIST_INFO      media_player_list_info [APPL_AVRCP_MAX_NUM_PLAYERS];
extern UINT16                                 num_players;

extern APPL_AVRCP_VIRTUAL_FOLDER_LIST_INFO    vf_list_info[APPL_AVRCP_MAX_VF_DEPTH][APPL_AVRCP_MAX_FOLDER_ITEMS];
extern UINT32                                 root_folder_item_count;
extern UINT32                                 child_folder_item_count;
extern UCHAR                                  curr_folder_depth;

extern APPL_AVRCP_MEDIA_LIST_INFO             media_search_list_info [APPL_AVRCP_MAX_SEARCH_LIST_COUNT];
extern UINT32                                 media_search_list_count;

extern APPL_AVRCP_MEDIA_LIST_INFO             media_list_info [APPL_AVRCP_MAX_MEDIA_LIST_COUNT];
extern UINT16                                 media_list_count;

extern UINT16                                 addressed_player_id;
extern UINT16                                 browsed_player_id;
extern UINT16                                 current_player_id;

extern UINT16                                 global_uid_counter;

extern UINT32                                 media_attr_ids [APPL_AVRCP_MAX_ATTR_COUNT];

extern UCHAR                                  battery_status;
extern UCHAR                                  system_status;
extern UCHAR                                  system_volume;
extern UINT32                                 track_select_status;

extern UCHAR                                  bow_player_set_rsp_err;
extern UCHAR                                  skip_cover_art_handle_get_folder_items_rsp;

extern APPL_AVRCP_PLAYER_EVENT                registered_event_list[APPL_AVRCP_MAX_PLAYER_EVENT_COUNT];

extern UCHAR                                  change_media_elem_attr_flag;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */

/* ----------------------------------------- Functions */

API_RESULT  appl_mp_init(void);

void appl_mp_update_media_player_setting (void);

void appl_mp_display_media_player_setting (void);

API_RESULT  appl_mp_read_company_id_list(void);

API_RESULT  appl_mp_display_company_id_list(void);

API_RESULT  appl_mp_read_event_id_list(void);

API_RESULT  appl_mp_read_player_appl_attr_info(void);

API_RESULT  appl_mp_read_media_element_attr_info(void);

API_RESULT  appl_mp_display_player_appl_attr_info(void);

API_RESULT  appl_mp_display_media_element_attr_info(void);

API_RESULT appl_mp_display_event_id_list(void);

API_RESULT  appl_mp_read_cur_media_play_status_info(void);

API_RESULT  appl_mp_display_cur_media_play_status_info(void);

API_RESULT  appl_mp_read_media_player_list(void);

API_RESULT  appl_mp_display_media_player_list(void);

API_RESULT  appl_mp_read_virtual_filesystem_items
            (
                UCHAR    folder_depth
            );

API_RESULT appl_mp_reset_virtual_folder_items
           (
               UCHAR    folder_depth
           );

void appl_mp_update_virtual_folder_media_item
     (
         APPL_AVRCP_VIRTUAL_FOLDER_LIST_INFO   *vf,
         UCHAR                                  item_index,
         UINT32                                 uid_offset,
         BT_fops_file_handle                    fp
     );

API_RESULT appl_mp_display_virtual_filesystem_items
           (
               UCHAR    folder_depth
           );

void appl_mp_reset_media_element_list (void);

void appl_mp_reset_search_media_list (void);

API_RESULT appl_mp_read_media_element_items (void);

API_RESULT appl_mp_display_media_element_list (void);

API_RESULT appl_mp_register_player_event
           (
               UCHAR event_id,
               UCHAR avrcp_tl
           );

UCHAR appl_mp_get_registered_player_event_tl (UCHAR event_id);
UCHAR appl_mp_get_registered_player_event_reg_status (UCHAR event_id);
void appl_mp_un_register_player_event_reg_status (UCHAR event_id);
API_RESULT appl_mp_add_media_item_to_virtual_filesystem
           (
               UCHAR   folder_depth
           );

#endif  /* _H_APPL_AVRCP_VIRTUAL_MEDIA_PLAYER_ */

