/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
