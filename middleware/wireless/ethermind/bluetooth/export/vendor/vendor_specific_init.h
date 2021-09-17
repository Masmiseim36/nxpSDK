/**
 *  \file vendor_specific_init.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_VENDOR_SPECIFIC_INIT_
#define _H_VENDOR_SPECIFIC_INIT_

/** ============================================= Header File Inclusion */
#include "BT_common.h"

/** ============================================= Global Definitions */

/** ============================================= Structures/Data Types */

/** ============================================= Macros */

/* #define BT_VENDOR_SPECIFIC_INIT */

#define V_INIT_FALSE                  0
#define V_INIT_IN_PROGRESS            1
#define V_INIT_WAIT_FOR_COMPLETION    2
#define V_INIT_COMPLETE               3


/** ============================================= Internal Functions */

/** ============================================= API Declarations */

#ifdef VS_INIT_DEBUG

#define VS_INIT_TRC(...) BT_debug_trace(0, __VA_ARGS__)
#define VS_INIT_INF(...) BT_debug_info(0, __VA_ARGS__)
#define VS_INIT_ERR(...) BT_debug_error(0, __VA_ARGS__)

#else

#define VS_INIT_TRC BT_debug_null
#define VS_INIT_INF BT_debug_null
#define VS_INIT_ERR BT_debug_null

#endif /* VS_INIT_DEBUG */

void app_vendor_specific_init(void ((*callback) (void)));

#endif /* _H_VENDOR_SPECIFIC_INIT_ */

