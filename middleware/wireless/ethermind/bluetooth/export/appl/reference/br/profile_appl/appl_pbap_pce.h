
/**
 *  \file appl_pbap_pce.h
 *
 *  Header file for PBAP_PCE Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_PBAP_PCE_
#define _H_APPL_PBAP_PCE_

/* ----------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "BT_pbap_api.h"
#include "pbap_pl.h"
#include "BT_fops.h"

/* ----------------------------------------- Global Definitions */
#define APPL_PBAP_PCE_MAX_SIZE         600U
#define APPL_PBAP_SUPPORTED_FEATURES   0x03FFU


/* ----------------------------------------- Macros */
#define PBAP_PCE_NOT_CONNECTED         0x00U
#define PBAP_PCE_CONNECTED             0x01U


/* ----------------------------------------- Structures/ Data Types */

typedef struct _APPL_PBAP_PCE_STRUCT
{
    PBAP_HANDLE   pbap_handle;

    /* ACL Connection handle */
    UINT16  acl_connect_handle;

    /* Connect structure */
    PBAP_CONNECT_STRUCT  connect_info;

    UINT8   state;

}APPL_PBAP_PCE_STRUCT;


/* User Id and associated password */
typedef struct _APPL_PBAP_PCE_USER_INFO
{
    UCHAR *user_id;

    UCHAR *passwd;
}APPL_PBAP_PCE_USER_INFO;

/* ----------------------------------------- Function Declarations */

void main_pbap_pce_operations (void);

#if 0
void appl_pbap_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     );
#endif /* 0 */

API_RESULT appl_pbap_pce_event_callback
           (
               /* IN */  UINT8            event_type,
               /* IN */  UINT16           event_result,
               /* IN */  PBAP_HANDLE      pbap_handle,
               /* IN */  PBAP_HEADERS   * pbap_headers,
               /* IN */  UINT8            num_headers
           );

void appl_pbap_pce_sdp_callback
     (
         UCHAR command,
         UCHAR * data,
         UINT16 length,
         UINT16 status
     );

UINT16 appl_pbap_pce_read_string
       (
           UCHAR *str,
           UINT16 max_len
       );

void appl_replace_special_chars
     (
         UCHAR* str
     );

void print_pbap_event_result
     (
         UINT16 event_result
     );

#endif /* _H_APPL_PBAP_PCE_ */


