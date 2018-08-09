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
