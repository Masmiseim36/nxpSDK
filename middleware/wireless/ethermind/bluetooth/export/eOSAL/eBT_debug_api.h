
/**
 *  \file eBT_debug_api.h
 *
 *  This Header File contains the APIs and the ADTs exported by the
 *  EtherMind Debug Library for QNX.
 */

/*
 *  Copyright (C) 2015. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_EBT_DEBUG_API_
#define _H_EBT_DEBUG_API_

/* --------------------------------------------- Header File Inclusion */
#include "EM_debug.h"

/* --------------------------------------------- Global Definitions */
#define BT_debug_error          EM_debug_error
#define BT_debug_trace          EM_debug_trace
#define BT_debug_info           EM_debug_info
#define BT_debug_dump           EM_debug_dump
#define BT_debug_direct         EM_debug_trace

#define BT_debug_null           EM_debug_null

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */
#define BT_debug_dump_bytes(m, b, l)           EM_debug_dump_bytes((m), (b), (l))
#define BT_debug_dump_decimal(m, b, l)         EM_debug_dump_decimal((m), (b), (l))

#ifdef BT_ENABLE_DISABLE_RUNTIME_DEBUG
#define BT_set_debug_level(l) EM_set_debug_level(l)

#define BT_update_module_debug_flag(module_id, flag) EM_update_module_debug_flag((module_id), (flag))

#define BT_enable_module_debug_flag   EM_enable_module_debug_flag
#define BT_disable_module_debug_flag  EM_disable_module_debug_flag

#endif /* BT_ENABLE_DISABLE_RUNTIME_DEBUG */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

#endif /* _H_EBT_DEBUG_API_ */

