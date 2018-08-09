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
     
/*!=============================================================================
\file       zcl_library_options.c
\brief      Core ZCL functionality that is controllable from zcl_options.h
            This file is built with user application code. 
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <string.h>

#include "zcl.h"
#include "zcl_customcommand.h"

#include "pdum_apl.h"
#include "zps_apl.h"
#include "zps_apl_af.h"
#include "zps_nwk_pub.h"
#include "zcl_options.h"
#include "zcl_internal_library_options.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifdef ZCL_CONFIGURE_ATTRIBUTE_REPORTING_SERVER_SUPPORTED
#ifndef ZCL_ATTRIBUTE_REPORTING_SERVER_SUPPORTED
#error "Must define ZCL_ATTRIBUTE_REPORTING_SERVER_SUPPORTED when ZCL_CONFIGURE_ATTRIBUTE_REPORTING_SERVER_SUPPORTED is defined"
#endif
#endif

#ifdef ZCL_READ_ATTRIBUTE_REPORTING_CONFIGURATION_SERVER_SUPPORTED
#ifndef ZCL_ATTRIBUTE_REPORTING_SERVER_SUPPORTED
#error "Must define ZCL_ATTRIBUTE_REPORTING_SERVER_SUPPORTED when ZCL_READ_ATTRIBUTE_REPORTING_CONFIGURATION_SERVER_SUPPORTED is defined"
#endif
#endif


#ifdef ZCL_CONFIGURE_ATTRIBUTE_REPORTING_CLIENT_SUPPORTED
#ifndef ZCL_ATTRIBUTE_REPORTING_CLIENT_SUPPORTED
#error "Must define ZCL_ATTRIBUTE_REPORTING_CLIENT_SUPPORTED when ZCL_CONFIGURE_ATTRIBUTE_REPORTING_CLIENT_SUPPORTED is defined"
#endif
#endif

#ifdef ZCL_READ_ATTRIBUTE_REPORTING_CONFIGURATION_CLIENT_SUPPORTED
#ifndef ZCL_ATTRIBUTE_REPORTING_CLIENT_SUPPORTED
#error "Must define ZCL_ATTRIBUTE_REPORTING_CLIENT_SUPPORTED when ZCL_READ_ATTRIBUTE_REPORTING_CONFIGURATION_CLIENT_SUPPORTED is defined"
#endif
#endif

#if ( (defined ZCL_COMMAND_GENERATED_DISCOVERY_CLIENT_SUPPORTED) || (defined ZCL_COMMAND_GENERATED_DISCOVERY_SERVER_SUPPORTED) || \
      (defined ZCL_COMMAND_RECEIVED_DISCOVERY_CLIENT_SUPPORTED ) || (defined ZCL_COMMAND_RECEIVED_DISCOVERY_SERVER_SUPPORTED) )
#ifndef ZCL_COMMAND_DISCOVERY_SUPPORTED
#error "Must Define ZCL_COMMAND_DISCOVERY_SUPPORTED in your zcl_options.h File"
#endif
#endif    




/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Public Functions                                              ***/
/****************************************************************************/


/****************************************************************************
 **
 ** NAME:       vZCL_HandleEntireProfileCommand
 **
 ** DESCRIPTION:
 ** Handles commands that act accross an entire profile. Built with application so that
 ** app options in zcl_options.h control the build.
 **
 ** PARAMETERS:               Name                    Usage
 ** uint8                     u8CommandIdentifier     ZCL command ID
 ** ZPS_tsAfEvent             *pZPSevent              Event to process
 ** tsZCL_EndPointDefinition  *psZCL_EndPointDefinition  Context of the endpoint
 ** tsZCL_ClusterInstance     *psClusterInstance      Context of the cluster
 ** RETURN:
 ** None
 **
 ****************************************************************************/

PUBLIC void vZCL_HandleEntireProfileCommand(uint8                       u8CommandIdentifier,
                                            ZPS_tsAfEvent               *pZPSevent,
                                            tsZCL_EndPointDefinition    *psZCL_EndPointDefinition,
                                            tsZCL_ClusterInstance       *psClusterInstance)

{
    // pass buffer to specific ZCL handlers
    switch(u8CommandIdentifier)
    {

#ifdef ZCL_ATTRIBUTE_READ_SERVER_SUPPORTED
        case(E_ZCL_READ_ATTRIBUTES):
        {
            vZCL_HandleAttributesReadRequest(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif

#ifdef ZCL_ATTRIBUTE_READ_CLIENT_SUPPORTED
        case(E_ZCL_READ_ATTRIBUTES_RESPONSE):
        {
            vZCL_HandleAttributesReadResponse(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif

#ifdef ZCL_ATTRIBUTE_REPORTING_CLIENT_SUPPORTED
        case(E_ZCL_REPORT_ATTRIBUTES):
        {
            vZCL_HandleAttributesReadResponse(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif

#ifdef ZCL_ATTRIBUTE_WRITE_SERVER_SUPPORTED
        case(E_ZCL_WRITE_ATTRIBUTES):
        {
            vZCL_HandleAttributesWriteRequest(pZPSevent, psZCL_EndPointDefinition, psClusterInstance, FALSE, FALSE);
            break;
        }
        case(E_ZCL_WRITE_ATTRIBUTES_UNDIVIDED):
        {
            vZCL_HandleAttributesWriteRequest(pZPSevent, psZCL_EndPointDefinition, psClusterInstance, TRUE, FALSE);
            break;
        }
        case(E_ZCL_WRITE_ATTRIBUTES_NO_RESPONSE):
        {
            vZCL_HandleAttributesWriteRequest(pZPSevent, psZCL_EndPointDefinition, psClusterInstance, FALSE, TRUE);
            break;
        }
#endif

#ifdef ZCL_ATTRIBUTE_WRITE_CLIENT_SUPPORTED
        case(E_ZCL_WRITE_ATTRIBUTES_RESPONSE):
        {
            vZCL_HandleAttributesWriteResponse(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif

#ifdef ZCL_CONFIGURE_ATTRIBUTE_REPORTING_SERVER_SUPPORTED
        case(E_ZCL_CONFIGURE_REPORTING):
        {
            vZCL_HandleConfigureReportingCommand(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif

#ifdef ZCL_CONFIGURE_ATTRIBUTE_REPORTING_CLIENT_SUPPORTED
        case(E_ZCL_CONFIGURE_REPORTING_RESPONSE):
        {
            vZCL_HandleConfigureReportingResponse(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif

#ifdef ZCL_READ_ATTRIBUTE_REPORTING_CONFIGURATION_SERVER_SUPPORTED
        case(E_ZCL_READ_REPORTING_CONFIGURATION):
        {
            vZCL_HandleReadReportingConfigurationCommand(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif

#ifdef ZCL_READ_ATTRIBUTE_REPORTING_CONFIGURATION_CLIENT_SUPPORTED
        case(E_ZCL_READ_REPORTING_CONFIGURATION_RESPONSE):
        {
            vZCL_HandleReadReportingConfigurationResponse(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif

#ifdef ZCL_ATTRIBUTE_DISCOVERY_SERVER_SUPPORTED

        case(E_ZCL_DISCOVER_ATTRIBUTES):
        {
            vZCL_HandleAttributesDiscoverRequest(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif

#ifdef ZCL_ATTRIBUTE_DISCOVERY_CLIENT_SUPPORTED

        case(E_ZCL_DISCOVER_ATTRIBUTES_RESPONSE):
        {
            vZCL_HandleDiscoverAttributesResponse(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif

#ifdef ZCL_COMMAND_RECEIVED_DISCOVERY_SERVER_SUPPORTED

        case(E_ZCL_DISCOVER_COMMANDS_RECEIVED):
        {
            vZCL_HandleDiscoverCommandsReceivedRequest(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif

#ifdef ZCL_COMMAND_RECEIVED_DISCOVERY_CLIENT_SUPPORTED

        case(E_ZCL_DISCOVER_COMMANDS_RECEIVED_RESPONSE):
        {
            vZCL_HandleDiscoverCommandsReceivedResponse(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif
#ifdef ZCL_COMMAND_GENERATED_DISCOVERY_SERVER_SUPPORTED

        case(E_ZCL_DISCOVER_COMMANDS_GENERATED):
        {
            vZCL_HandleDiscoverCommandsGeneratedRequest(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif
#ifdef ZCL_COMMAND_GENERATED_DISCOVERY_CLIENT_SUPPORTED

        case(E_ZCL_DISCOVER_COMMANDS_GENERATED_RESPONSE):
        {
            vZCL_HandleDiscoverCommandsGeneratedResponse(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif

#ifdef ZCL_ATTRIBUTE_DISCOVERY_EXTENDED_SERVER_SUPPORTED

        case(E_ZCL_DISCOVER_ATTRIBUTES_EXTENDED):
        {
            vZCL_HandleAttributesDiscoverExtendedRequest(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif
#ifdef ZCL_ATTRIBUTE_DISCOVERY_EXTENDED_CLIENT_SUPPORTED

        case(E_ZCL_DISCOVER_ATTRIBUTES_EXTENDED_RESPONSE):
        {
            vZCL_HandleDiscoverAttributesExtendedResponse(pZPSevent, psZCL_EndPointDefinition, psClusterInstance);
            break;
        }
#endif
        default:
        {
            // Unrecognised command that should act accross all profiles
            eZCL_SendDefaultResponse(pZPSevent, E_ZCL_CMDS_UNSUP_GENERAL_COMMAND);
        }
    }
}


/****************************************************************************
 **
 ** NAME:       eZCL_CreateOptionalManagers
 **
 ** DESCRIPTION:
 ** Creates managers that are controlled by the app options in zcl_options.h.
 ** If further optional managers are needed, their parameters can be added to the
 ** list of parameters for this function.
 **
 ** PARAMETERS:         Name                                    Usage
 ** uint8               u8NumberOfReports                       Number of reports
 ** uint16              u16SystemMinimumReportingInterval       Min System Reporting Interval
 ** uint16              u16SystemMaximumReportingInterval       Max System Reporting Interval
 **
 ** RETURN:
 ** teZCL_Status E_ZCL_SUCCESS if all managers created OK or a manager specific
 ** error on failulre.
 **
 ****************************************************************************/
PUBLIC teZCL_Status eZCL_CreateOptionalManagers(
            uint8       u8NumberOfReports,
            uint16      u16SystemMinimumReportingInterval,
            uint16      u16SystemMaximumReportingInterval)
{
    teZCL_Status eStatus = E_ZCL_SUCCESS;
#ifdef ZCL_ATTRIBUTE_REPORTING_SERVER_SUPPORTED
    // Create Reports
    eStatus  = eZCL_CreateReportManager(u8NumberOfReports, u16SystemMinimumReportingInterval, u16SystemMaximumReportingInterval);
    if(eStatus != E_ZCL_SUCCESS)
    {
        return eStatus;
    }
#endif
    return eStatus;
 }

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
