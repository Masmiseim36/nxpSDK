
/**
 *  \file GA_timer.h
 *
 *  This Header File contains the mapping to generic OSAL APIs.
 */

/*
 *  Copyright (C) 2015. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_TIMER_
#define _H_GA_TIMER_

/* --------------------------------------------- Header File Inclusion */
#include "EM_timer.h"

/* --------------------------------------------- Global Definitions */
#define GA_TIMER_HANDLE_INIT_VAL            EM_TIMER_HANDLE_INIT_VAL

/* --------------------------------------------- Structures/Data Types */
#define GA_timer_handle                     EM_timer_handle

/* --------------------------------------------- Macros */
#define GA_start_timer(h, t, c, a, s)       EM_start_timer((h), (t), (c), (a), (s))
#define GA_restart_timer(h, t)              EM_restart_timer((h), (t))
#define GA_stop_timer(h)                    EM_stop_timer(h)
#define GA_is_active_timer(h)               EM_is_active_timer(h)

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

#endif /* _H_GA_TIMER_ */

