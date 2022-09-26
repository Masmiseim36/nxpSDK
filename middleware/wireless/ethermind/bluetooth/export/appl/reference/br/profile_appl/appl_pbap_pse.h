
/**
 *  \file appl_pbap_pse.h
 *
 *  Header file for PBAP_PSE Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_PBAP_PSE_
#define _H_APPL_PBAP_PSE_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_api.h"
#include "BT_pbap_api.h"
#include "pbap_pl.h"
#include "object_parser_api.h"

/* ----------------------------------------- Global Definitions */


/* ----------------------------------------- Macros */
#define APPL_PBAP_PSE_NOT_CONNECTED          0x00U
#define APPL_PBAP_PSE_CONNECTED              0x01U

#define APPL_PBAP_PSE_MAX_SIZE               OBEX_MAX_PACKET_LENGTH

#define APPL_PBAP_PSE_MAX_PATH_LEN           100U
#define APPL_PBAP_PSE_MAX_SEARCH_VALUE_LEN   60U


/* ----------------------------------------- Structures/ Data Types */

typedef struct _APPL_PBAP_PSE_STRUCT
{
    PBAP_HANDLE   pbap_handle;

    /* ACL Connection handle */
    UINT16  acl_connect_handle;

    /* Connect structure */
    PBAP_CONNECT_STRUCT  connect_info;

    /* Instance current folder path */
    UCHAR path[APPL_PBAP_PSE_MAX_PATH_LEN];

    /* Instance current folder path */
    UCHAR tpath[APPL_PBAP_PSE_MAX_PATH_LEN];

    /* Root depth of the instance */
    INT8 root_depth;

    /* Root depth of the instance */
    INT8 troot_depth;

}APPL_PBAP_PSE_STRUCT;


/* User Id and associated password */
typedef struct _APPL_PBAP_PSE_USER_INFO
{
    UCHAR *user_id;

    UCHAR *passwd;
}APPL_PBAP_PSE_USER_INFO;

/* ----------------------------------------- Function Declarations */

void main_pbap_pse_operations (void);

API_RESULT appl_pbap_pse_event_callback
           (
               /* IN */  UINT8            event_type,
               /* IN */  UINT16           event_result,
               /* IN */  PBAP_HANDLE      pbap_handle,
               /* IN */  PBAP_HEADERS   * pbap_headers,
               /* IN */  UINT8            num_headers
           );

/* To read the user input string of length "max_len" */
UINT16 appl_pbap_pse_read_string
       (
           UCHAR *str,
           UINT16 max_len
       );

API_RESULT appl_pbap_pse_handle_set_phonebook_req
           (
               /* IN */  UINT16           event_result,
               /* IN */  PBAP_HANDLE      pbap_handle,
               /* IN */  PBAP_HEADERS   * pbap_headers,
               /* IN */  UINT8            num_headers
           );

API_RESULT appl_pbap_pse_handle_get_vcard_listing_req
           (
               /* IN */  UINT16           event_result,
               /* IN */  PBAP_HANDLE      pbap_handle,
               /* IN */  PBAP_HEADERS   * pbap_headers,
               /* IN */  UINT8            num_headers
           );

API_RESULT appl_pbap_pse_handle_get_vcard_req
           (
               /* IN */  UINT16           event_result,
               /* IN */  PBAP_HANDLE      pbap_handle,
               /* IN */  PBAP_HEADERS   * pbap_headers,
               /* IN */  UINT8            num_headers
           );

API_RESULT appl_pbap_pse_handle_get_phonebook_req
           (
               /* IN */  UINT16           event_result,
               /* IN */  PBAP_HANDLE      pbap_handle,
               /* IN */  PBAP_HEADERS   * pbap_headers,
               /* IN */  UINT8            num_headers
           );

API_RESULT appl_pbab_set_to_get_phonebook_req_path
            (
               /* INOUT */ APPL_PBAP_PSE_STRUCT  * pb_appl_inst,
               /* IN    */ UCHAR *pb_file_name,
               /* OUT   */ UCHAR *pb_object
            );

void appl_pbab_vcard_parser_cb
     (
         /* IN */ OBJECT_PARSER_CB_PARAM cb_param,
         /* IN */ OBJECT_PARSER_CONTEXT * context
     );

void appl_pbab_vcard_list_parser_cb
     (
         /* IN */ OBJECT_PARSER_CB_PARAM cb_param,
         /* IN */ OBJECT_PARSER_CONTEXT * context
     );

#endif /* _H_APPL_PBAP_PSE_ */


