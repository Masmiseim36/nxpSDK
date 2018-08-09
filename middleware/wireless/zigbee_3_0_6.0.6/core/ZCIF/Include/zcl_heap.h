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

#ifndef  ZCL_HEAP_H_INCLUDED
#define  ZCL_HEAP_H_INCLUDED

/*!
\file       zcl_heap.h
\brief      Manages the heap used to allocate memory to components. Simple design:
            doesn't allow the memory to be returned.
*/

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "dlist.h"
#include "zcl.h"
#include "zcl_customcommand.h"

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* ZCL_INTERNAL_TIMERS is 1 because the ZCL needs a timer of its own for reporting.
   u8NumberOfTimers in the heap size macro is the number of timers required outside of the ZCL.*/
#define ZCL_INTERNAL_TIMERS (1)

#ifndef ZCL_ATTRIBUTE_REPORT_STRING_MAXIMUM_SIZE
    #define ZCL_ATTRIBUTE_REPORT_STRING_MAXIMUM_SIZE        32
#endif    

typedef struct
{
    bool_t                    bRegistered;
    tsZCL_EndPointDefinition *psEndPointDefinition;
} tsZCL_EndPointRecord;

typedef struct {
    DNODE                       dllTimeNode;
    teZCL_TimerMode             eTimerMode;
    uint32                      u32UTCTime;
    tfpZCL_ZCLCallBackFunction  pfZCLCallBackFunction;
} tsZCL_TimerRecord;

typedef struct
{
    DNODE                       dllReportNode;
    tsZCL_EndPointDefinition    *psEndPointDefinition;
    tsZCL_ClusterInstance       *psClusterInstance;
    tsZCL_AttributeDefinition   *psAttributeDefinition;
    tuZCL_AttributeStorage       uAttributeStorage;
    uint32                       u32LastFiredUTCTime;
    tsZCL_AttributeReportingConfigurationRecord sAttributeReportingConfigurationRecord;
} tsZCL_ReportRecord;

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
// ZCL_INTERNAL_TIMERS is 1 because the ZCL now needs a timer of its own for reporting.  u8NumberOfTimers in the heap size macro is the number of timers required outside of the ZCL.
#define ZCL_HEAP_SIZE(u8NumberOfEndpoints, u8NumberOfTimers, u8NumberOfReports , u8NumberOfStringReports) (ENDPOINT_SIZE_WORDS(u8NumberOfEndpoints) + TIMER_SIZE_WORDS((u8NumberOfTimers+ZCL_INTERNAL_TIMERS)) + REPORT_SIZE_WORDS(u8NumberOfReports) + STRING_REPORT_SIZE_WORDS(u8NumberOfStringReports))

#define ENDPOINT_SIZE(u8NumberOfEndpoints)                ((sizeof(tsZCL_EndPointRecord)+ 4) * u8NumberOfEndpoints)
#define ENDPOINT_SIZE_WORDS(u8NumberOfEndpoints)          ((ENDPOINT_SIZE(u8NumberOfEndpoints) + 4)/4)

#define TIMER_SIZE(u8NumberOfTimers)                      ((sizeof(tsZCL_TimerRecord)+ 4) * u8NumberOfTimers)
#define TIMER_SIZE_WORDS(u8NumberOfTimers)                ((TIMER_SIZE(u8NumberOfTimers) + 4)/4)

#define REPORT_SIZE(u8NumberOfReports)                    ((sizeof(tsZCL_ReportRecord)+ 4) * u8NumberOfReports)
#define REPORT_SIZE_WORDS(u8NumberOfReports)              ((REPORT_SIZE(u8NumberOfReports) + 4)/4)

#define STRING_REPORT_SIZE(u8NumberOfStringReports)       ((ZCL_ATTRIBUTE_REPORT_STRING_MAXIMUM_SIZE + 4) * u8NumberOfStringReports)
#define STRING_REPORT_SIZE_WORDS(u8NumberOfStringReports) ((STRING_REPORT_SIZE(u8NumberOfStringReports) + 4)/4)
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
/***        Imported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* ZCL_HEAP_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
