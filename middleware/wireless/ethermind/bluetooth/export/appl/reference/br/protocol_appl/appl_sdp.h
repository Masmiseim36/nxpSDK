
/**
 *  \file appl_sdp.h
 *
 *  Header File for SDP Command Line Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_SDP_
#define _H_APPL_SDP_

/* ----------------------------------------- Header File Inclusion */
#include "BT_sdp_api.h"

#include "db_gen.h"

/* ----------------------------------------- Global Definitions */

/* ----------------------------------------- Structures/Data Types */


/* ----------------------------------------- Macros */
/* Maximum SDP UUIDs. Used in SS and SSA requests */
#define APPL_SDP_MAX_UUIDS              16U

/* Maximum SDP Record Handles */
#define APPL_SDP_MAX_REC_HANDLES        64U

/* Maximum SDP Attribute IDs and Ranges */
#define APPL_SDP_MAX_ATTR_IDS           16U
#define APPL_SDP_MAX_ATTR_ID_RANGES     8U

/* SDP Attribute Data Buffer Size */
#define APPL_SDP_ATTR_DATA_BUF_SIZE     4096U

/* Macros to set SDP application state and operation */
#define APPL_SET_SDP_STATE(state) appl_sdp_state = (state)
#define APPL_SET_SDP_OP(op) appl_sdp_operation = (op)

/*
 * Application State Table.
 *
 * current state -- event --> new state
 * ------------------------------------
 *
 * IDLE       -- SDP Connection Request     --> IN_CONN
 * IN_CONN    -- SDP Connection Complete    --> CONNECTED
 * CONNECTED  -- SS Request                 --> IN_SS_REQ
 * CONNECTED  -- SA Request                 --> IN_SA_REQ
 * CONNECTED  -- SSA Request                --> IN_SSA_REQ
 * IN_SS_REQ  -- SS Complete                --> CONNECTED
 * IN_SA_REQ  -- SA Complete                --> CONNECTED
 * IN_SSA_REQ -- SSA Complete               --> CONNECTED
 * CONNECTED  -- SDP Disconnection Request  --> IN_DISC
 * IN_DISC    -- SDP Disconnection Complete --> IDLE
 */
#define APPL_SDP_STATE_IDLE                  0U
#define APPL_SDP_STATE_IN_CONN               1U
#define APPL_SDP_STATE_CONNECTED             2U
#define APPL_SDP_STATE_IN_DISC               3U
#define APPL_SDP_STATE_IN_SS_REQ             4U
#define APPL_SDP_STATE_IN_SA_REQ             5U
#define APPL_SDP_STATE_IN_SSA_REQ            6U

/*
 * Application Operations:
 * None, Service Search, Service Attribute, Service Search Attribute requests,
 * Open and Close.
 */
#define APPL_SDP_OP_NONE                     0U
#define APPL_SDP_OP_SS_REQ                   1U
#define APPL_SDP_OP_SA_REQ                   2U
#define APPL_SDP_OP_SSA_REQ                  3U
#define APPL_SDP_OP_OPEN                     4U
#define APPL_SDP_OP_CLOSE                    5U

/* ----------------------------------------- Function Declarations */
void main_sdp_operations(void);
void main_dbase_operations(void);

/* SDP API Handlers */
void appl_sdp_open (void);
void appl_sdp_close (void);

void appl_ss_req(void);
void appl_sa_req(void);
void appl_ssa_req(void);

/* The SDP Callback Handler */
void appl_sdp_callback
     (
         UCHAR     command,
         UCHAR *   data,
         UINT16    length,
         UINT16    status
     );

/* Utility Functions */
void appl_sdp_get_uuids(void);
void appl_sdp_get_attr_ids(void);
void appl_sdp_print_record_handles
     (
         /* IN */ UCHAR  * buffer,
         /* IN */ UINT16   no_of_rec_hdl
     );

#ifdef SDP_SERVER_HAVE_CB_IND_SUPPORT
API_RESULT appl_sdp_server_ind
           (
               UCHAR    event_type,
               UCHAR *  event_data,
               UCHAR    event_datalen
           );
void appl_sdp_server_register_cb(void);
#endif /* SDP_SERVER_HAVE_CB_IND_SUPPORT */

API_RESULT appl_update_gatt_service_range_in_sdp_record
            (
                /* IN */ UCHAR  service_type,
                /* IN */ UINT16 start_handle,
                /* IN */ UINT16 end_handle
            );

#endif /* _H_APPL_SDP_ */

