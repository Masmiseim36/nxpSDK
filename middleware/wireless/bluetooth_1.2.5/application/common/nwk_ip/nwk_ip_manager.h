/*! *********************************************************************************
 * \defgroup NWK_IP
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the Network IP Manager
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
