/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _RIPNG_CFG_H
#define _RIPNG_CFG_H

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "app_stack_config.h"

/*==================================================================================================
Private macros
==================================================================================================*/

#ifndef RIPNG_ENABLE
   #define RIPNG_ENABLE 0
#endif

#ifndef RIPNG_ROUTE_TIMEOUT_SEC
   #define RIPNG_ROUTE_TIMEOUT_SEC  180
#endif


#ifndef RIPNG_TRIGGERD_UPDATE_HOLDOFF_TIME_MIN_SEC
   #define RIPNG_TRIGGERD_UPDATE_HOLDOFF_TIME_MIN_SEC  1
#endif

#ifndef RIPNG_TRIGGERD_UPDATE_HOLDOFF_TIME_MAX_SEC
   #define RIPNG_TRIGGERD_UPDATE_HOLDOFF_TIME_MAX_SEC   5
#endif

#ifndef RIPNG_ROUTING_UPDATE_TIME_MIN_SEC
   #define RIPNG_ROUTING_UPDATE_TIME_MIN_SEC 15
#endif

#ifndef RIPNG_ROUTING_UPDATE_TIME_MAX_SEC
   #define RIPNG_ROUTING_UPDATE_TIME_MAX_SEC 45
#endif

#endif  /*_RIPNG_CFG_H */
