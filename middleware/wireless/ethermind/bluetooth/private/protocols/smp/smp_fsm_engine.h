
/**
 *  \file smp_fsm_engine.h
 *
 *  This file defines state and events related to SMP FSM.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_SMP_FSM_ENGINE_
#define _H_SMP_FSM_ENGINE_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "fsm_defines.h"

/* --------------------------------------------- Global Definitions */
/* Number of SMP events at the FSM */
#define SMP_MAX_NUM_EVENTS     0x11

/* SMP Local and Remote event identifier */
#define SMP_FSM_LOCAL_EVENT    0x00
#define SMP_FSM_REMOTE_EVENT   0x01

/* --------------------------------------------- External Global Variables */
/* SMP FSM tables */
extern DECL_CONST EVENT_HANDLER_TABLE smp_event_handler_table[];
extern DECL_CONST STATE_EVENT_TABLE smp_state_event_table[];


/* --------------------------------------------- Structures/Data Types */

/*
 * Event Handler Table.
 * Current State, handler(if any), num_of_retval, retval_table_start_index,
 * num_of_substates handles this event,
 * start_index_of_those_handler_in_this_same_table
 */

typedef struct _SMP_INSTANCE_INFO
{
    UCHAR ci;

    UCHAR di;

    void * device;

    void * context;

} SMP_INSTANCE_INFO;

typedef struct _SMP_EVENT_INFO
{
    /* Event Data  */
    void     * data_param;

    /* SMP Context Handle */
    SMP_INSTANCE_INFO   * instance;

    /* Event Data Length */
    UINT16     datalen;

    /* Event */
    EVENT_T    event_type;

    /* Additional information on event */
    UCHAR      event_info;

} SMP_EVENT_INFO;

/* void smp_fsm_update_state (UCHAR instance, RETVAL_T retval); */

API_RESULT smp_fsm_post_event (UCHAR context, SMP_EVENT_INFO * smp_event);
API_RESULT smp_access_state_handler (void * param, STATE_T * state);

#endif /* _H_SMP_FSM_ENGINE_ */

