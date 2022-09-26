

/**
 *  \file appl_avrcp.h
 *
 *  Header file for AVRCP command line applicaiton for both CT & TG roles.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_AVRCP_
#define _H_APPL_AVRCP_

/* --------------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "BT_avrcp_al_api.h"


/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */

/* --------------------------------------------- External Global Variables */
/* AVRCP Handles */
extern AVRCP_HANDLE appl_avrcp_handle[AVRCP_MAX_PROFILES];

/* AVRCP Peer BD Address */
extern UCHAR appl_avrcp_bd_addr[BT_BD_ADDR_SIZE];


/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

void main_avrcp_operations (void);

void appl_avrcp_init_handles (void);

void appl_avrcp_init (void);

void appl_avrcp_shutdown (void);

void appl_avrcp_start (void);

void appl_avrcp_stop (void);

void appl_avrcp_choose_handle (UCHAR * retval);

void appl_avrcp_show_handles (void);

void appl_avrcp_ctl_ch_connect_req (void);

void appl_avrcp_ctl_ch_disconnect_req (void);

#ifdef AVRCP_1_4

void appl_avrcp_brow_ch_connect_req (void);

void appl_avrcp_brow_ch_disconnect_req (void);

#endif /* AVRCP_1_4 */

API_RESULT appl_avrcp_choose_acl_connection (UINT16 * indx);

API_RESULT appl_avrcp_ntf_cb
           (
               AVRCP_HANDLE * avrcp_handle,
               UCHAR          event_type,
               UINT16         event_result,
               void  *        event_data,
               UINT16         event_datalen
           );

void appl_avrcp_print_opcode (UCHAR opcode);

void appl_avrcp_print_response_type (UCHAR rsp_type);

void appl_avrcp_print_operation_id (UCHAR operation_id);

#endif /* _H_APPL_AVRCP_ */



