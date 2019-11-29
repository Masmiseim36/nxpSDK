/*! *********************************************************************************
 * \defgroup NWK_IP
 * @{
 ********************************************************************************** */
/*!
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file
*
* This file is the interface file for the Network IP Manager
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _IP_MANAGER_H_
#define _IP_MANAGER_H_

#include "network_utils.h"
#include "ip_if_management.h"
#include "ble_abs_types.h"

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/


/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
/*! Nwk Instance */
typedef struct nwkInstance_tag
{
    /* Instance Bindings */
    instanceId_t                nwkInstId;
    ipIfUniqueId_t              ifUniqueId;
    ifHandle_t                  ifHandle;
    bleAbsRequests_t           *pBleAbsReq;
    instanceId_t                slwpInstId;
}nwkInstance_t;

extern nwkInstance_t* gpNwkStackInstance;

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
  
/*! *********************************************************************************
* \brief    Initializes the Network IP Manager modules
*
* \return  gNwkStatusSuccess_c or error.
********************************************************************************** */
nwkStatus_t NwkIpManager_Init(void);

/*! *********************************************************************************
* \brief    Configures and starts the modules for an IPv6 Node. It shall be called 
*           when an IPSP L2CAP connection is created
*
* \return  gNwkStatusSuccess_c or error.
********************************************************************************** */
nwkStatus_t NwkIpManager_StartNode(void);

/*! *********************************************************************************
* \brief    Configures and starts the modules for an IPv6 Router. It shall be called 
*           when the BLE Stack is initialized.
*
* \return  gNwkStatusSuccess_c or error.
********************************************************************************** */
nwkStatus_t NwkIpManager_StartRouter(void);

#ifdef __cplusplus
}
#endif 


#endif /* _APP_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
