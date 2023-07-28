
/**
 *  \file nwsim.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_NWSIM_
#define _H_NWSIM_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */
typedef struct _NWSIM_CALLINFO
{
    /** Index */
    UCHAR idx;

    /**
     * Direction
     * 0: Outgoing
     * 1: Incoming
     */
    UCHAR dir;

    /**
     * Status
     * 0: Active
     * 1: Held
     * 2: Dialing
     * 3: Alerting
     * 4: Incoming
     * 5: Waiting
     * 6: Call held by Response & Hold
     */
    UCHAR status;

    /**
     * Mode
     * 0: Voice
     * 1: Data
     * 2: Fax
     */
    UCHAR mode;

    /**
     * Mpty
     * 0: NOT a member of conference call
     * 1: IS a member of conference call
     */
    UCHAR mpty;

    /** Number */
    CHAR num[16];

    /** Type */
    UCHAR type;

} NWSIM_CALLINFO;

/* --------------------------------------------- Macros */
#ifndef NWSIM_NO_DEBUG
#define NWSIM_ERR(...)             BT_debug_error(BT_MODULE_ID_GEN_PL, __VA_ARGS__)
#else  /* NWSIM_NO_DEBUG */
#define NWSIM_ERR                  BT_debug_null
#endif /* NWSIM_NO_DEBUG */

#ifdef NWSIM_DEBUG
    #define NWSIM_TRC(...)         BT_debug_trace(BT_MODULE_ID_GEN_PL, __VA_ARGS__)
    #define NWSIM_INF(...)         BT_debug_info (BT_MODULE_ID_GEN_PL, __VA_ARGS__)
#else /* NWSIM_DEBUG */
    #define NWSIM_TRC              BT_debug_null
    #define NWSIM_INF              BT_debug_null
#endif /* NWSIM_DEBUG */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
void nwsim_operations(void);
void nwsim_init
     (
         void (* read_cb)(UCHAR *rsp, UINT16 rsplen),
         void (* call_control_cb)(UCHAR status),
         UCHAR(* get_inbandring_status)(void)
     );
void nwsim_start (UINT16 sf);
void nwsim_stop(void);
void nwsim_command_send (UCHAR * cmd, UINT16 cmdlen);

void nwsim_send_rsp(UCHAR rsp_code, void * value);
API_RESULT nwsim_setup_incoming_call(CHAR * num);
API_RESULT nwsim_setup_outgoing_call(CHAR * num);
API_RESULT nwsim_setup_twc_incoming(CHAR * num);
API_RESULT nwsim_setup_twc_outgoing(CHAR * num);
API_RESULT nwsim_handle_call(UCHAR action);
API_RESULT nwsim_dial(CHAR * num);
API_RESULT nwsim_handle_twc(UCHAR action, UCHAR idx);
API_RESULT nwsim_end_call(void);
API_RESULT nwsim_handle_btrh(UCHAR option);

void nwsim_display_callinfo(void);
NWSIM_CALLINFO * nwsim_alloc_call_context(UINT8 * idx);

#endif /* _H_NWSIM_*/
