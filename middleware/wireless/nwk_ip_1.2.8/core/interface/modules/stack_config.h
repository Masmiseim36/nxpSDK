/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _STACK_CONFIG_H
#define _STACK_CONFIG_H
/*!=================================================================================================
\file       stack_config.h
\brief      This is a header file for stack configuration structures.
==================================================================================================*/

#ifndef  STACK_THREAD
    #define  STACK_THREAD 1
#endif

/*==================================================================================================
Include Files
==================================================================================================*/
#if STACK_THREAD
    #include "thread_stack_config.h"
#endif

#if STACK_BLE
    #include "ip6_ble_stack_config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _FILENAME_PUBLIC_H */
