/*
* The Clear BSD License
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


#ifndef COMBOMIN_H
#define COMBOMIN_H

/*!
\file       gp.h
\brief      The API for the Green Power End Point Registration
*/
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/    
#include <jendefs.h>
#include "zcl.h"
#include "zcl_options.h"
#include "GreenPower.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifndef GP_MANUFACTURER_CODE
#define GP_MANUFACTURER_CODE                                        (0)
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

    /* Holds cluster instances */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma pack(push, 4)
typedef struct
{
#ifdef GP_COMBO_BASIC_DEVICE
    tsZCL_ClusterInstance                  sGreenPowerServer;
#endif
    tsZCL_ClusterInstance                  sGreenPowerClient;
} tsGP_GreenPowerClusterInstances;
#pragma pack(pop)
#else  
typedef struct
{
#ifdef GP_COMBO_BASIC_DEVICE
    tsZCL_ClusterInstance                  sGreenPowerServer;
#endif
    tsZCL_ClusterInstance                  sGreenPowerClient;
} tsGP_GreenPowerClusterInstances __attribute__ ((aligned(4)));
#endif


/* Holds everything required to create an instance of an Green Power device */
typedef struct
{
    tsZCL_EndPointDefinition               sEndPoint;

    /* Cluster instances */
    tsGP_GreenPowerClusterInstances        sClusterInstance;
    /* Holds the attributes for the Green Power cluster server */
#ifdef GP_COMBO_BASIC_DEVICE
    tsCLD_GreenPower                       sServerGreenPowerCluster;
#endif
   /* Holds the attributes for the Green Power cluster Client */
    tsCLD_GreenPower                       sClientGreenPowerCluster;

    /*Event Address, Custom call back event, Custom call back message*/
    tsGP_GreenPowerCustomData              sGreenPowerCustomDataStruct;
} tsGP_GreenPowerDevice;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC teZCL_Status eGP_RegisterComboBasicEndPoint(
                        uint8                               u8EndPointIdentifier,
                        tfpZCL_ZCLCallBackFunction          cbCallBack,
                        tsGP_GreenPowerDevice               *psDeviceInfo,
                        uint16                              u16ProfileId,
                        tsGP_TranslationTableEntry          *psTranslationTableEntry);

PUBLIC teZCL_Status eGP_RegisterProxyBasicEndPoint(
                        uint8                               u8EndPointIdentifier,
                        tfpZCL_ZCLCallBackFunction          cbCallBack,
                        tsGP_GreenPowerDevice               *psDeviceInfo);
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern const tsZCL_AttributeDefinition  asCLD_GreenPowerClusterAttributeDefinitionsClient[];
extern tsZCL_ClusterDefinition          sCLD_GreenPowerClient;
extern uint8                            au8ClientGreenPowerClusterAttributeControlBits[];

#ifdef GP_COMBO_BASIC_DEVICE
extern const tsZCL_AttributeDefinition  asCLD_GreenPowerClusterAttributeDefinitionsServer[];
extern tsZCL_ClusterDefinition          sCLD_GreenPowerServer;

extern uint8                            au8ServerGreenPowerClusterAttributeControlBits[];

#endif
/****************************************************************************/
#if defined __cplusplus
}
#endif

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* COMBOMIN_H */
