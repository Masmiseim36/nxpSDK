
/**
 *  \file map_pl.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_MAP_PL_
#define _H_MAP_PL_

/* --------------------------------------------- Header File Inclusion */
#include "BT_map_api.h"
#include "BT_fops.h"

/* --------------------------------------------- Global Definitions */
/* Max Folder Name length */
#define MAP_FOLDER_NAME_LEN              64U

/* File object name including path */
#define MAPC_MAX_FILE_OBJ_NAME_LEN       128U

/* MAP Conversation Listing file name */
#define APPL_MAPC_CONV_LISTING_FILE      "MAP_rx_conv_listing.xml"

/* MAP Folder Listing file name */
#define APPL_MAPC_FOLDER_LISTING_FILE    "MAP_rx_folder_listing.xml"

/* MAP Message Listing file name */
#define APPL_MAPC_MSG_LISTING_FILE       "MAP_rx_msg_listing.xml"

/* MAP Rx. Message file name */
#define APPL_MAPC_MSG_FILE               "map_rx_raw.msg"

/* MAP Rx. Message file name */
#define APPL_MAPC_EVENT_RPRT_FILE        "map_tx_event.txt"

#define   APPL_MAPS_EVENT_REPORT_FILE_NAME                    "MAP-event_report.xml"
#define   APPL_MAPS_CONV_LISTING_FILE_NAME                    "MAP-tx_conv_listing_file.xml"

/* Base folder for MAP objects */
#define MAP_ROOT_FOLDER_BASE \
    BT_FOPS_PATH_JOIN(BT_FOPS_BASE,"data" BT_FOPS_PATH_SEP "map")

/* --------------------------------------------- Structures/Data Types */
typedef struct _MAP_MSG_ATTR_PL
{
    CHAR handle[20];
    CHAR subject[20];
    CHAR datetime[20];
    CHAR sname[20];
    CHAR saddr[20];
    CHAR rname[20];
    CHAR raddr[20];
    CHAR type[12];
    CHAR size[12];
    CHAR rstatus[12];
    CHAR asize[12];
    CHAR read[4];
    CHAR priority[4];

    /* Message Type: Bit mask */
    UCHAR m_type;

    /* Read Status */
    UCHAR rd_status;
} MAP_MSG_ATTR_PL;

/* --------------------------------------------- Macros */
#ifdef MAP_PL_DEBUG
#define MAP_PL_TRC(...)                 BT_debug_trace(BT_MODULE_ID_MAP_PL, __VA_ARGS__)
#else /* MAP_PL_DEBUG */
#define MAP_PL_TRC                      BT_debug_null
#endif /* MAP_PL_DEBUG */

/* --------------------------------------------- Internal Functions */
API_RESULT map_get_message_attributes_pl
           (
               /* IN */  UCHAR * idir,
               /* IN */  CHAR * filename,
               /* OUT */ MAP_MSG_ATTR_PL * attr
           );

/* --------------------------------------------- API Declarations */
API_RESULT BT_map_create_xml_folder_listing_pl
           (
               /* IN */  UCHAR   *dir_entry,
               /* IN */  UCHAR   *listingfile,
               /* OUT */ UINT16  *num_entity
           );

API_RESULT BT_map_create_xml_messages_listing_pl
           (
               /* IN */  UCHAR   *dir_entry,
               /* IN */  UCHAR   *listingfile,
               /* IN */  MAP_APPL_PARAMS * appl_params,
               /* OUT */ UINT16  *num_entity
           );

API_RESULT BT_map_update_inbox_pl
           (
               /* IN */  UCHAR  *idir,
               /* IN */  UINT16  item_id
           );

API_RESULT BT_map_push_message_pl
           (
               /* IN */  UCHAR  *idir,
               /* IN */  UINT16  item_id,
               /* OUT */ UCHAR  *message_handle
           );

API_RESULT BT_map_set_message_status_pl
           (
               /* IN */ UCHAR * idir,
               /* IN */ CHAR * handle,
               /* IN */ UINT16 status
           );

API_RESULT BT_map_get_message_file_pl
           (
               /* IN */   UCHAR    *msg_handle,
               /* IN */   UCHAR    *idir,
               /* OUT */  UCHAR    *message_file
           );

#if 0
API_RESULT BT_map_create_object_name_pl
           (
               /* IN */  UCHAR * path,
               /* IN */  UCHAR * object,
               /* OUT */ UCHAR * obj_name
           );

API_RESULT BT_map_set_path_backward_pl
           (
               /* OUT */ UCHAR *path
           );

API_RESULT BT_map_set_path_forward_pl
           (
               /* INOUT */  UCHAR * path,
               /* IN */     UCHAR * folder
           );
#endif /* 0 */

API_RESULT BT_map_build_event_report_file_pl
           (
               /* IN */  UCHAR   *dir_entry,
               /* IN */  UCHAR   *event_file,
               /* IN */  UCHAR   *folder_type,
               /* IN */  UCHAR   *event_type,
               /* IN */  UCHAR   event_ver,
               /* IN */  UCHAR   *handle,
               /* IN */  UCHAR   *msg_type
           );

API_RESULT BT_map_add_participant_to_conversation_pl
           (
               /* IN */  UCHAR   *dir_entry,
               /* IN */  UCHAR   *convlist_file,
               /* IN */  UCHAR   *conv_id
           );

API_RESULT BT_map_check_conversation_id_pl
           (
               /* IN */  UCHAR   *dir_entry,
               /* IN */  UCHAR   *convlist_file,
               /* IN */  UCHAR   *conv_id_str
           );

#define    BT_map_set_default_converstation_list_pl(d_e, c_f)        \
           BT_map_add_participant_to_conversation_pl                \
           (                                                        \
               (d_e),                                               \
               (c_f),                                               \
               NULL                                                 \
           )

API_RESULT BT_map_update_conversation_participant_fields_pl
           (
               /* IN */  UCHAR   *dir_entry,
               /* IN */  UCHAR   *convlist_file,
               /* IN */  UCHAR   *conv_id
           );

#endif /* _H_MAP_PL_ */

