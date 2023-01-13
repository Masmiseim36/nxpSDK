
/**
 *  \file eBT_timer.h
 *
 *  This Header File contains the APIs and the ADTs exported by the
 *  EtherMind Timer Library for QNX.
 */

/*
 *  Copyright (C) 2015. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_EBT_TIMER_
#define _H_EBT_TIMER_

/* --------------------------------------------- Header File Inclusion */
#include "EM_timer.h"

/* --------------------------------------------- Global Definitions */
#define BT_TIMER_HANDLE_INIT_VAL            EM_TIMER_HANDLE_INIT_VAL
#define BT_TIMEOUT_MILLISEC                 EM_TIMEOUT_MILLISEC

/* --------------------------------------------- Structures/Data Types */
#define BT_timer_handle                     EM_timer_handle

/* --------------------------------------------- Macros */
#define em_timer_init()
#define em_timer_shutdown()
#define timer_bt_init()
#define timer_bt_shutdown()

#define BT_start_timer(h, t, c, a, s)       EM_start_timer((h), (t), (c), (a), (s))
#define BT_restart_timer(h, t)              EM_restart_timer((h), (t))
#define BT_stop_timer(h)                    EM_stop_timer(h)
#define BT_is_active_timer(h)               EM_is_active_timer(h)

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

#endif /* _H_EBT_TIMER_ */

