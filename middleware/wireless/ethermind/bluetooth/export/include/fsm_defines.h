
/**
 *  \file fsm_defines.h
 *
 *  This file defines state and events related to FSM.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_FSM_DEFINES_
#define _H_FSM_DEFINES_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"

/* --------------------------------------------- Structures/Data Types */
/**
 * \defgroup fsm_module FSM (Finite State Machine)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Finite State Machine (FSM) module to the Application and other upper
 *  layers of the stack.
 */

/**
 * \defgroup fsm_defines Defines
 * \{
 * This section describes the defines for FSM
 */

/**
 * \defgroup fsm_structures Structures
 * \{
 *  This section describes the various constant defines used in FSM
 */
typedef UINT32 STATE_T;

typedef UCHAR EVENT_T;

typedef API_RESULT (*SE_HNDLR_T)(void * param);

typedef API_RESULT RETVAL_T;

typedef struct
{
    STATE_T    set_bits;
} FSM_STATE_MASK;

typedef struct
{
    RETVAL_T        handler_retval;
    FSM_STATE_MASK  next_state;

} RETVAL_TABLE_T;

/**
 * Event Handler Table.
 * Current State, handler(if any), num_of_retval, retval_table_start_index,
 * num_of_substates handles this event,
 * start_index_of_those_handler_in_this_same_table
 */

typedef struct
{
    STATE_T       current_state;
    SE_HNDLR_T    handler;

} EVENT_HANDLER_TABLE_T;

#define EVENT_HANDLER_TABLE     EVENT_HANDLER_TABLE_T

/**
 * State-Event Table.
 * Event, Default Handler, num_of_retval, retval_table_start_index,
 * num_of_substates handles this event,
 * start_index_of_those_handler_in_this_same_table
 */
typedef struct
{
    EVENT_T       event;
    UINT32        event_handler_table_end_index;
    UINT32        event_handler_table_start_index;

} STATE_EVENT_TABLE_T;

#define STATE_EVENT_TABLE STATE_EVENT_TABLE_T

/* State identifier */
typedef struct
{
    STATE_T state;
} STATE_TABLE_T;

typedef void (*FSM_STATE_CHANGE_HANDLER)(void * param, STATE_T  state) DECL_REENTRANT;

typedef API_RESULT (*FSM_STATE_ACCESS_HANDLER)(void *param, STATE_T * state) DECL_REENTRANT;

typedef UCHAR FSM_STATE_SIZE_T;

typedef struct fsm_module_t
{
    /** Function to Access State of Module */
    FSM_STATE_ACCESS_HANDLER     state_access_handler;

    /** State Change Function Registered with FSM */
    FSM_STATE_CHANGE_HANDLER     state_change_handler;

    /** Event Handler Table */
    DECL_CONST EVENT_HANDLER_TABLE         * event_table;

    /** State Event Table */
    DECL_CONST STATE_EVENT_TABLE           * state_event_table;

    /** State Event Table Size */
    FSM_STATE_SIZE_T            state_event_table_size;

}FSM_MODULE_TABLE_T;
/** \} */
/** \} */
/** \} */
#endif /* _H_FSM_DEFINES_ */

