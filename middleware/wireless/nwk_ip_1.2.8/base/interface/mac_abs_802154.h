/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef _MAC_ABS_802154_H
#define _MAC_ABS_802154_H
/*!=================================================================================================
\file       mac_abs_802154.h
\brief      This is a header file for the MAC abstraction module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "mac_abs_types.h"

/*==================================================================================================
Public macros
==================================================================================================*/

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


/*!*************************************************************************************************
\fn    instanceId_t MAC_RegisterAbsCb_802154(instanceId_t macInstanceId)
\brief Public interface function for the MAC abstraction module. This function initializes the
       internal MAC callbacks structure and returns the pointer to the Mac Abstraction Requests
       structure.

\param[in]  nwkId  The instance of the upper layer ( NWK layer)
\param[out]  nwkId  The instance of the MAC layer (output param)

\retval      macAbsRequests_t       pointer address to the Mac Abstraction Requests
***************************************************************************************************/
macAbsRequests_t* MAC_RegisterAbsCb_802154
(
    instanceId_t nwkInstanceId, //IN
    instanceId_t* pMacInstanceId //OUT
);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _MAC_ABS_802154_H */
