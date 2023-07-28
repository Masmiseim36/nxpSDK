
/**
 *  \file appl_pan.h
 *
 *  Header file for PAN Profile Application in Linux User Mode.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_PAN_
#define _H_APPL_PAN_

/* ---------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "BT_pan_api.h"

#include "db_gen.h"

/* ---------------------------------------- Global Definitions */
#define APPL_PAN_MAX_EXTN_HDRS    5

/* ---------------------------------------- Structures/Data Types */
typedef struct
{
    UCHAR bd_addr [BT_BD_ADDR_SIZE];
    UCHAR allocated;
    UCHAR local_service;
    UCHAR remote_service;

} APPL_PAN_HANDLE;


/* ---------------------------------------- Macros */
/* Executing 'system()' commands */
#define pan_appl_execute_command(cmd_str) \
    printf("Executing System Command: '%s'\n", (cmd_str)); \
    system ((cmd_str));

#define INVALID_VALUE                   0xFFU
/* ---------------------------------------- Function Declarations */
void main_pan_operations ( void );

/* Callback Handler for BNEP_EVENT_IND_CB */
API_RESULT appl_pan_event_ind
           (
               UCHAR      event_type,
               UCHAR *    bd_addr,
               UINT16     event_result,
               void *     event_data,
               UINT16     event_data_len
           );

API_RESULT appl_pan_read_ind
           (
               UCHAR      local_service,
               UCHAR *    bd_addr,
               UCHAR *    eth_header,
               UCHAR *    eth_payload,
               UINT16     eth_payload_len
           );

void appl_pan_registration (void);
void appl_pan_set_policy (void);

void appl_pan_accept (void);
void appl_pan_accept_cancel ( void );
void appl_pan_connect (void);
void appl_pan_setup (void);
void appl_pan_filter (void);
void appl_pan_disconnect (void);

void appl_pan_write_data ( void );
/* void appl_pan_raw_write_data ( void ); */
void appl_pan_write_data_from_file(void);

/* Enable/disable header compression */
void appl_pan_hdr_comression(void);

API_RESULT appl_pan_alloc_bd_addr (int * indx);
API_RESULT appl_pan_search_bd_addr (UCHAR * bd_addr, int * indx);

void appl_pan_get_handle ( int * indx );
void appl_pan_display_handles ( void );

API_RESULT appl_pan_choose_acl_connection (UINT16 * indx);

/* void appl_pan_wait (void); */

API_RESULT appl_pan_update_sdp_record
           (
               /* IN */  UCHAR    service_type,
               /* IN */  UCHAR    flag
           );

#endif /* _H_APPL_PAN_ */

