
/**
 *  \file brr_internal.h
 *
 *  \brief This file defines the internal data structures, function declarations,
 *         macros and constants used in Bearer.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BRR_INTERNAL_
#define _H_BRR_INTERNAL_


/* --------------------------------------------- Header File Inclusion */
#include "GA_bearer_api.h"

/* --------------------------------------------- Debug Mapping */
#ifndef BRR_NO_DEBUG
#define BRR_ERR(...)                                GA_debug_error(GA_MODULE_ID_BRR, __VA_ARGS__)
#else  /* BRR_NO_DEBUG */
#define BRR_ERR                                     GA_debug_null
#endif /* BRR_NO_DEBUG */

#ifdef BRR_DEBUG

#define BRR_TRC(...)                                GA_debug_trace(GA_MODULE_ID_BRR, __VA_ARGS__)
#define BRR_INF(...)                                GA_debug_info(GA_MODULE_ID_BRR, __VA_ARGS__)

#define BRR_debug_dump_bytes(data, datalen) GA_debug_dump_bytes(GA_MODULE_ID_BRR, (data), (datalen))

#else /* BRR_DEBUG */

#define BRR_TRC                                     GA_debug_null
#define BRR_INF                                     GA_debug_null

#define BRR_debug_dump_bytes(data, datalen)

#endif /* BRR_DEBUG */

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- Functions */

#endif /* _H_BRR_INTERNAL_ */




