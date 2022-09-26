
/**
 *  \file appl_mcap.h
 *
 *  Header File for MCAP Command Line Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_MCAP_
#define _H_APPL_MCAP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "BT_mcap_api.h"


/* ----------------------------------------- Global Definitions */

#ifdef MCAP_CLOCK_SYNC_FEATURE
extern UINT32 bt_clk_value;

extern UCHAR read_clock_in_progress;

extern UINT16 l2cap_dt_acl_handle;
#endif /* MCAP_CLOCK_SYNC_FEATURE */

/* ----------------------------------------- Structures/Data Types */


/* ----------------------------------------- Macros */


/* ----------------------------------------- Function Declarations */
API_RESULT mcap_cb
           (
               MCAP_HANDLE    * handle,
               UCHAR          event,
               UINT16         response,
               void          * event_param,
               UINT16         event_len
           );


void appl_mcap_register (void);
void appl_mcap_mc_connect (void);
void appl_mcap_mc_disconnect (void);

void appl_mcap_md_create (void);
void appl_mcap_md_disconnect (void);
void appl_mcap_md_delete (void);
void appl_mcap_md_delete_all (void);
void appl_mcap_md_reconnect (void);
void appl_mcap_md_abort (void);
void appl_mcap_md_write (void);

#ifdef MCAP_STANDARD_COMMANDS
void appl_initialize_data_channel_options (void);
#endif /* MCAP_STANDARD_COMMANDS_SUPPORT */

void appl_mcap_send_invalid_cmd (void);
void appl_send_clk_sync_cmd (void);
void appl_config_cb
     (
         /* IN */  MCAP_HANDLE            *  handle,
         /* IN */  UCHAR                     channel_prf,
         /* OUT */ L2CAP_CONFIG_OPTION    ** config_option
     );

void appl_mcap_sync_cap_req (void);

void appl_mcap_sync_cap_rsp (void);

void appl_mcap_sync_set_req (void);

void appl_mcap_sync_set_rsp (void);

void appl_mcap_sync_info_ind (void);

void appl_hci_read_clk (void);

void main_mcap_operations (void);

#endif /* _H_APPL_MCAP_ */

