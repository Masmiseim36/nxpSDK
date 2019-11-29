/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _MPL_CFG_H
#define _MPL_CFG_H

/*!
\file       mpl_cfg.h
\brief      This is a configuration header file for the MPL module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "app_stack_config.h"
/*==================================================================================================
Private macros
==================================================================================================*/
/*! Maximum number of MPL instances */
#ifndef MPL_INSTANCE_SET_SIZE
  #define MPL_INSTANCE_SET_SIZE         2
#endif

/*! Maximum number of seeds that can be supported simultaneously */
#ifndef MPL_SEED_SET_SIZE
  #define MPL_SEED_SET_SIZE             20
#endif

/*! Maximum number of MPL messages that can be supported simultaneously */
#ifndef MPL_BUFFERED_MESSAGE_SET_SIZE
  #define MPL_BUFFERED_MESSAGE_SET_SIZE 2
#endif

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*================================================================================================*/

#endif  /*_MPL_CFG_H */
