/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
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
 
/*!=================================================================================================
\file       app_border_router.c
\brief      This is a public source file for the Ethernet interface.

\=================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "EmbeddedTypes.h"

#include "app_stack_config.h"
#include "stack_config.h"

#include "app_border_router.h"

#include "app_ethernet.h"
#include "app_ethernet_usb.h"
#include "app_serial_tun.h"

#include "thread_utils.h"


/*==================================================================================================
Private macros
==================================================================================================*/



/*==================================================================================================
Private type definitions
==================================================================================================*/


/*==================================================================================================
Private prototypes
==================================================================================================*/



/*==================================================================================================
Private global variables declarations
==================================================================================================*/



/*==================================================================================================
Public global variables declarations
==================================================================================================*/



/*==================================================================================================
Public functions
==================================================================================================*/

/*!*************************************************************************************************
\fn     BR_Start
\brief  Use this function to start the border router.
\return void
***************************************************************************************************/
void BR_Start(taskMsgQueue_t *pTaskMsgQueue, instanceId_t thrInstanceID)
{
    BR_PrefixSetup(thrInstanceID);
  
#if THR_SERIAL_TUN_ROUTER
    Serialtun_Start(pTaskMsgQueue, thrInstanceID);
#endif

#if (ENET_ROUTER || ENET_HOST)
    IFENET_Start(pTaskMsgQueue, thrInstanceID);
#endif

#if  (USBENET_ROUTER || USBENET_HOST)
    IFUSBENET_Start(pTaskMsgQueue, thrInstanceID);
#endif
}

/*!*************************************************************************************************
\fn     void BR_ThreadStartedCb()
\brief  This is a public function used to start the Ethernet IP Media Interface.
\return void
***************************************************************************************************/
void BR_ThreadStartedCb
(
    instanceId_t thrInstanceID
)
{
#if THR_SERIAL_TUN_ROUTER
    Serialtun_ThreadStarted(thrInstanceID);
#endif
#if (ENET_ROUTER || ENET_HOST)
    IFENET_ThreadStarted(thrInstanceID);
#endif
#if (USBENET_ROUTER || USBENET_HOST)
    IFUSBENET_ThreadStarted(thrInstanceID);
#endif
}

/*!*************************************************************************************************
\fn     void BR_ExternalIfRouterSetup()
\brief  This is a public function used to configure the prefixes on the External IP Media Interface.
\return void
***************************************************************************************************/
void BR_PrefixSetup
(
    instanceId_t thrInstanceID
)
{
#if (ENET_ROUTER || USBENET_ROUTER || THR_SERIAL_TUN_ROUTER)
    /* On ND ROUTER use case, if the External IF Prefix is not set (all bytes are zeros),
     * generate the External Prefix using the first 6 bytes from the global on-mesh prefix,
     * concatenated with a random subnet mask (byte 6 and 7) */
    if(FLib_MemCmp(&THR_GetAttr_BrExternalIfPrefix(thrInstanceID).prefix,
                   (void*)&in6addr_any, sizeof(ipAddr_t)))
    {
        thrPrefixAttr_t externalPrefix;
        thrPrefixAttr_t onMeshPrefix = THR_GetAttr_BrGlobalOnMeshPrefix(thrInstanceID);
        uint8_t* pSubNet = &externalPrefix.prefix.addr8[6];

        FLib_MemSet(&externalPrefix, 0, sizeof(externalPrefix));
        externalPrefix.prefixLenBits = 64;
        /* Get the first 6 bytes from the global on-mesh prefix */
        FLib_MemCpy(&externalPrefix.prefix.addr8[0], onMeshPrefix.prefix.addr8, 6);

        /* Generate the random subnet mask. Make sure that is not zero, all ffs or is not
         * the same as the subnet mask of the global on-mesh prefix */
        do
        {
            NWKU_GenRand(pSubNet, 2);
        }while( (ntohas(pSubNet) == 0) ||
                (ntohas(pSubNet) == THR_ALL_FFs16) ||
                 FLib_MemCmp(pSubNet, &onMeshPrefix.prefix.addr8[6] , 2));
        /* Set the attribute */
        THR_SetAttr(thrInstanceID, gNwkAttrId_BrExternalIfPrefix_c, 0,
                    sizeof(thrPrefixAttr_t), &externalPrefix);
    }
#endif
}

/*==================================================================================================
Private functions
==================================================================================================*/

