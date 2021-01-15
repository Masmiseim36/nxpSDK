
/**
 *  \file obex_pl.h
 *
 *  Header file for all platform specific implementation & extensions for
 *  OBEX module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_OBEX_PL_
#define _H_OBEX_PL_

/* ----------------------------------------- Header File Inclusion */
/* EtherMind Header Files */
#include "BT_obex_api.h"

/* ----------------------------------------- OBEX PL Debug */
#ifndef OBEX_PL_NO_DEBUG
#define OBEX_PL_ERR(...)   BT_debug_error(BT_MODULE_ID_OBEX, __VA_ARGS__)
#else  /* OBEX_PL NO_DEBUG */
#define OBEX_PL_ERR        BT_debug_null
#endif /* OBEX_PL_NO_DEBUG */

#ifdef OBEX_PL_DEBUG

#define OBEX_PL_TRC(...)   BT_debug_trace(BT_MODULE_ID_OBEX, __VA_ARGS__)
#define OBEX_PL_INF(...)   BT_debug_info(BT_MODULE_ID_OBEX, __VA_ARGS__)

#else  /* OBEX_PL_DEBUG */

#define OBEX_PL_TRC        BT_debug_null
#define OBEX_PL_INF        BT_debug_null

#endif /* OBEX_PL_DEBUG */

/* ----------------------------------------- Global Definitions */


/* ----------------------------------------- Structures/Data Types */


/* ----------------------------------------- Macros */


/* ----------------------------------------- API Declarations */


/* ----------------------------------------- Function Declarations */

#endif /* _H_OBEX_PL_ */

