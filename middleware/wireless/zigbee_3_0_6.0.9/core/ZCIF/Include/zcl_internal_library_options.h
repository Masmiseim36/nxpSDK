/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef  ZCL_LIBRARY_OPTIONS_H_INCLUDED
#define  ZCL_LIBRARY_OPTIONS_H_INCLUDED

/*!
\file       zcl_library_options.h
\brief      Internal functions that are needed to build zcl_library_options.h
*/

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include "jendefs.h"
#include "dlist.h"
#include "zcl.h"
#include "zcl_heap.h"
#include "zcl_customcommand.h"

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void vZCL_HandleAttributesReadRequest(
                    ZPS_tsAfEvent              *pZPSevent,
                    tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                    tsZCL_ClusterInstance      *psClusterInstance);

PUBLIC void vZCL_HandleAttributesReadResponse(
                    ZPS_tsAfEvent              *pZPSevent,
                    tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                    tsZCL_ClusterInstance      *psClusterInstance);

PUBLIC void vZCL_HandleAttributesWriteRequest(
                    ZPS_tsAfEvent              *pZPSevent,
                    tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                    tsZCL_ClusterInstance      *psClusterInstance,
                    bool_t                      bIsUndivided,
                    bool_t                      bNoResponse);
PUBLIC void vZCL_HandleAttributesWriteResponse(
                    ZPS_tsAfEvent              *pZPSevent,
                    tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                    tsZCL_ClusterInstance      *psClusterInstance);

PUBLIC void vZCL_HandleConfigureReportingCommand(
                    ZPS_tsAfEvent              *pZPSevent,
                    tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                    tsZCL_ClusterInstance      *psClusterInstance);

PUBLIC void vZCL_HandleConfigureReportingResponse(
                    ZPS_tsAfEvent              *pZPSevent,
                    tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                    tsZCL_ClusterInstance      *psClusterInstance);

PUBLIC void vZCL_HandleReadReportingConfigurationCommand(
                   ZPS_tsAfEvent              *pZPSevent,
                   tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                   tsZCL_ClusterInstance      *psClusterInstance);

PUBLIC void vZCL_HandleReadReportingConfigurationResponse(
                   ZPS_tsAfEvent              *pZPSevent,
                   tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                   tsZCL_ClusterInstance      *psClusterInstance);

PUBLIC void vZCL_HandleAttributesDiscoverRequest(
                    ZPS_tsAfEvent              *pZPSevent,
                    tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                    tsZCL_ClusterInstance      *psClusterInstance);

PUBLIC void vZCL_HandleDiscoverAttributesResponse(
                    ZPS_tsAfEvent              *pZPSevent,
                    tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                    tsZCL_ClusterInstance      *psClusterInstance);

PUBLIC teZCL_Status eZCL_CreateReportManager(
                    uint8                       u8NumberOfReports,
                    uint16                      u16SystemMinimumReportingInterval,
                    uint16                      u16SystemMaximumReportingInterval);

PUBLIC  void vZCL_HandleDiscoverCommandsReceivedRequest(
                                             ZPS_tsAfEvent              *pZPSevent,
                                             tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                                             tsZCL_ClusterInstance      *psClusterInstance);

PUBLIC  void vZCL_HandleDiscoverCommandsGeneratedRequest(
                                             ZPS_tsAfEvent              *pZPSevent,
                                             tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                                             tsZCL_ClusterInstance      *psClusterInstance);

PUBLIC void vZCL_HandleDiscoverCommandsReceivedResponse(
                                             ZPS_tsAfEvent              *pZPSevent,
                                             tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                                             tsZCL_ClusterInstance      *psClusterInstance);

PUBLIC void vZCL_HandleDiscoverCommandsGeneratedResponse(
                                             ZPS_tsAfEvent              *pZPSevent,
                                             tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                                             tsZCL_ClusterInstance      *psClusterInstance);                                             
                    
PUBLIC void vZCL_HandleAttributesDiscoverExtendedRequest(
                    ZPS_tsAfEvent              *pZPSevent,
                    tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                    tsZCL_ClusterInstance      *psClusterInstance);

PUBLIC void vZCL_HandleDiscoverAttributesExtendedResponse(
                    ZPS_tsAfEvent              *pZPSevent,
                    tsZCL_EndPointDefinition   *psZCL_EndPointDefinition,
                    tsZCL_ClusterInstance      *psClusterInstance);    

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
                    
#if defined __cplusplus
}
#endif

#endif  /* ZCL_LIBRARY_OPTIONS_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
