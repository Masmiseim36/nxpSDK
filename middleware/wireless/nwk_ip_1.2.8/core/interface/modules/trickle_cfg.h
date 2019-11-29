/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _TRICKLE_CFG_H
#define _TRICKLE_CFG_H

/*!=================================================================================================
\file       trickle_cfg.h
\brief      This is a configuration header file for the Trickle module.

\details    This file contains the folowing configuration options:

            TRICKLE_ENABLED             0 | 1   (default is 1)
            TRICKLE_INSTANCE_SET_SIZE   0 | 255 (default is 5)
            TRICKLE_LIST_SIZE           0 | 255 (default is 20)
            TRICKLE_DEBUG               0 | 1   (default is 0)
==================================================================================================*/



/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "app_stack_config.h"

/*==================================================================================================
Public macros
==================================================================================================*/
/* Default Values */

/*! Maximum number of Trickle instances */
#ifndef TRICKLE_INSTANCE_SET_SIZE
    #define TRICKLE_INSTANCE_SET_SIZE       2
#endif

/*! Maximum number of trickle timers */
#ifndef TRICKLE_LIST_SIZE
  #define TRICKLE_LIST_SIZE                 5
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

#endif  /*_TRICKLE_CFG_H */
