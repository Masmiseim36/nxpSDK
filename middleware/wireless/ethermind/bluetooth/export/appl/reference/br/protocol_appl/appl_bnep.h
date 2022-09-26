
/**
 *  \file appl_bnep.h
 *
 *  Header File for BNEP Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_BNEP_
#define _H_APPL_BNEP_

#include "BT_hci_api.h"
#include "BT_bnep_api.h"

#ifdef BNEP_TEST_MODE_RAW
#include "bnep.h"
#endif /* BNEP_TEST_MODE_RAW */


typedef struct
{
    UCHAR bd_addr [BT_BD_ADDR_SIZE];
    UCHAR allocated;
    UCHAR local_service;
    UCHAR remote_service;

} APPL_BNEP_HANDLE;

/* Callback Handler for BNEP_EVENT_IND_CB */
API_RESULT appl_bnep_event_ind
           (
               UCHAR      event_type,
               UCHAR *    bd_addr,
               UINT16     event_result,
               void *     event_data,
               UINT16     event_data_len
           );

/* Callback Handler for BNEP_DATA_IND_CB */
API_RESULT appl_bnep_read_ind
           (
               UCHAR *    bd_addr,
               UCHAR *    eth_header,
               UCHAR *    eth_payload,
               UINT16     eth_payload_len
           );

API_RESULT appl_bnep_handle_data
           (
               UCHAR *    bd_addr,
               UCHAR *    eth_header,
               UCHAR *    eth_payload,
               UINT16     eth_payload_len
           );

#ifdef BNEP_TEST_MODE_RAW
/* Callback Handler for BNEP_DATA_IND_CB (Raw Reception Mode) */
API_RESULT appl_bnep_read_ind_raw
           (
               UCHAR *    bd_addr,
               UCHAR *    eth_payload,
               UINT16     eth_payload_len
           );
void appl_bnep_send_raw_packet (int indx, UCHAR * packet);
void appl_bnep_write_raw (void);
void appl_bnep_test_1 (void);
void appl_bnep_test_2 (void);
void appl_bnep_test_3 (void);
void appl_bnep_test_4 (void);
void appl_bnep_test_5 (void);
void appl_bnep_test_6 (void);
void appl_bnep_test_7 (void);
void appl_bnep_test_8 (void);
void appl_bnep_test_9 (void);
#endif /* BNEP_TEST_MODE_RAW */

void main_bnep_operations(void);

void appl_bnep_registration (void);
void appl_bnep_set_policy (void);

void appl_bnep_accept (void);
void appl_bnep_connect (void);
void appl_bnep_setup (void);
void appl_bnep_filter (void);
void appl_bnep_disconnect (void);

void appl_bnep_write_data ( void );
void appl_bnep_raw_write_data ( void );

API_RESULT appl_bnep_alloc_bd_addr (int * indx);
API_RESULT appl_bnep_search_bd_addr (UCHAR * bd_addr, int * indx);

void appl_bnep_get_handle ( int * indx );
void appl_bnep_display_handles ( void );

API_RESULT appl_bnep_choose_acl_connection (UINT16 * indx);

void appl_bnep_wait (void);

#endif  /* _H_APPL_BNEP_ */

