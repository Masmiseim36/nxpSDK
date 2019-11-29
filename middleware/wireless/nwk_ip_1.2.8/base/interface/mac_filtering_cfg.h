/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*! *********************************************************************************
* \defgroup Mac Filtering Interface
* @{
 ***********************************************************************************/

#ifndef _MAC_FILTERING_CFG_H
#define _MAC_FILTERING_CFG_H

/*!
\file       mac_filtering_cfg.h
\brief      This is a configuration header file for the mac filtering module.
\details    This file contains the folowing configuration options:
            MAC_FILTERING_TABLE_SIZE            0 | 255 (default is 5)
*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "app_stack_config.h"
#include "stack_config.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*! Description of the MAC_FILTERING_TABLE_SIZE configuration option: maximum number of neighbors
    that can be added using MacFiltering_AddNeighbor function */
#ifndef MAC_FILTERING_TABLE_SIZE
    #define MAC_FILTERING_TABLE_SIZE            5
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
#endif  /*_MAC_FILTERING_CFG_H */
