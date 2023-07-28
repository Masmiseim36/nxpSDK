
/**
 *  \file GA_debug.h
 *
 *  This Header File contains the mapping to generic OSAL APIs.
 */

/*
 *  Copyright (C) 2015. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_DEBUG_API_
#define _H_GA_DEBUG_API_

/* --------------------------------------------- Header File Inclusion */
#include "EM_debug.h"

/* --------------------------------------------- Global Definitions */
#define GA_debug_error          EM_debug_error
#define GA_debug_trace          EM_debug_trace
#define GA_debug_info           EM_debug_info
#define GA_debug_dump           EM_debug_dump
#define GA_debug_direct         EM_debug_trace

#define GA_debug_null           EM_debug_null

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */
#define GA_debug_dump_bytes(m, b, l)           EM_debug_dump_bytes((m), (b), (l))
#define GA_debug_dump_decimal(m, b, l)         EM_debug_dump_decimal((m), (b), (l))

#ifdef GA_ENABLE_DISABLE_RUNTIME_DEBUG
#define GA_set_debug_level(l)                           EM_set_debug_level(l)
#define GA_update_module_debug_flag(module_id, flag)    EM_update_module_debug_flag((module_id), (flag))
#define GA_enable_module_debug_flag                     EM_enable_module_debug_flag
#define GA_disable_module_debug_flag                    EM_disable_module_debug_flag

#endif /* GA_ENABLE_DISABLE_RUNTIME_DEBUG */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

#endif /* _H_GA_DEBUG_API_ */

