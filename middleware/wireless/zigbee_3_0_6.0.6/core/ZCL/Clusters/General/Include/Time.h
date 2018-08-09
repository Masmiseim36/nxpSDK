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


#ifndef TIME_H
#define TIME_H

/*!
\file       Time.h
\brief      Header for Time Cluster
*/
/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/     
#include <jendefs.h>
#include "zcl.h"
#include "zcl_options.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/*             Time Cluster - Optional Attributes                           */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the time cluster.                                          */
/****************************************************************************/

/* Enable the optional Time Zone attribute */
//#define CLD_TIME_ATTR_TIME_ZONE

/* Enable the optional DST Start attribute */
//#define CLD_TIME_ATTR_DST_START

/* Enable the optional DST End attribute */
//#define CLD_TIME_ATTR_DST_END

/* Enable the optional DST Shift attribute */
//#define CLD_TIME_ATTR_DST_SHIFT

/* Enable the optional Standard Time attribute */
//#define CLD_TIME_ATTR_STANDARD_TIME

/* Enable the optional Local Time attribute */
//#define CLD_TIME_ATTR_LOCAL_TIME

/* End of optional attributes */


/* Cluster ID's */
#define GENERAL_CLUSTER_ID_TIME                            0x000a


/* Time Status  - bit definitions and bit masks */
#define CLD_TM_TIME_STATUS_MASTER_BIT                 0
#define CLD_TM_TIME_STATUS_SYNCHRONIZED_BIT           1
#define CLD_TM_TIME_STATUS_MASTER_ZONE_DST_BIT        2
#define CLD_TM_TIME_STATUS_SUPERSEDING_BIT            3

#define CLD_TM_TIME_STATUS_MASTER_MASK                (1 << CLD_TM_TIME_STATUS_MASTER_BIT)
#define CLD_TM_TIME_STATUS_SYNCHRONIZED_MASK          (1 << CLD_TM_TIME_STATUS_SYNCHRONIZED_BIT)
#define CLD_TM_TIME_STATUS_MASTER_ZONE_DST_MASK       (1 << CLD_TM_TIME_STATUS_MASTER_ZONE_DST_BIT)
#define CLD_TM_TIME_STATUS_SUPERSEDING_MASK           (1 << CLD_TM_TIME_STATUS_SUPERSEDING_BIT)


#define CLD_TIME_NUMBER_OF_MANDATORY_ATTRIBUTE           2  //utctTime, u8TimeStatus

#ifdef CLD_TIME_ATTR_TIME_ZONE
    #define ATTR_TIME_ZONE                               1
#else
    #define ATTR_TIME_ZONE                               0
#endif

#ifdef CLD_TIME_ATTR_DST_START
    #define ATTR_DST_START                               1
#else
    #define ATTR_DST_START                               0
#endif

#ifdef CLD_TIME_ATTR_DST_END
    #define ATTR_DST_END                                 1
#else
    #define ATTR_DST_END                                 0
#endif

#ifdef CLD_TIME_ATTR_DST_SHIFT
    #define ATTR_DST_SHIFT                               1
#else
    #define ATTR_DST_SHIFT                               0
#endif
#ifdef CLD_TIME_ATTR_STANDARD_TIME
    #define ATTR_STANDARD_TIME                           1
#else
    #define ATTR_STANDARD_TIME                           0
#endif
#ifdef CLD_TIME_ATTR_LOCAL_TIME
    #define ATTR_LOCAL_TIME                              1
#else
    #define ATTR_LOCAL_TIME                              0
#endif
#ifdef CLD_TIME_ATTR_LAST_SET_TIME
    #define ATTR_LAST_SET_TIME                           1
#else
    #define ATTR_LAST_SET_TIME                           0
#endif
#ifdef CLD_TIME_ATTR_VALID_UNTIL_TIME
    #define ATTR_VALID_UNTIL_TIME                        1
#else
    #define ATTR_VALID_UNTIL_TIME                        0
#endif

#ifndef CLD_TIME_CLUSTER_REVISION
    #define CLD_TIME_CLUSTER_REVISION                     1
#endif    

#define CLD_TIME_NUMBER_OF_OPTIONAL_ATTRIBUTE  \
    ATTR_TIME_ZONE           +                 \
    ATTR_DST_START           +                 \
    ATTR_DST_END             +                 \
    ATTR_DST_SHIFT           +                 \
    ATTR_STANDARD_TIME       +                 \
    ATTR_LOCAL_TIME          +                 \
    ATTR_LAST_SET_TIME       +                 \
    ATTR_VALID_UNTIL_TIME

#define CLD_TIME_MAX_NUMBER_OF_ATTRIBUTE       \
    CLD_TIME_NUMBER_OF_OPTIONAL_ATTRIBUTE +    \
    CLD_TIME_NUMBER_OF_MANDATORY_ATTRIBUTE

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum 
{
    /* Time attribute set attribute ID's (3.12.2.2) */
    E_CLD_TIME_ATTR_ID_TIME                = 0x0000,  /* Mandatory */
    E_CLD_TIME_ATTR_ID_TIME_STATUS,                   /* Mandatory */
    E_CLD_TIME_ATTR_ID_TIME_ZONE,
    E_CLD_TIME_ATTR_ID_DST_START,
    E_CLD_TIME_ATTR_ID_DST_END,
    E_CLD_TIME_ATTR_ID_DST_SHIFT,
    E_CLD_TIME_ATTR_ID_STANDARD_TIME,
    E_CLD_TIME_ATTR_ID_LOCAL_TIME,
    E_CLD_TIME_ATTR_ID_LAST_SET_TIME,
    E_CLD_TIME_ATTR_ID_VALID_UNTIL_TIME,
} teCLD_TM_AttributeID;

/* Time Cluster */
typedef struct
{
#ifdef TIME_SERVER    
    zutctime                utctTime;

    zbmap8                  u8TimeStatus;

#ifdef CLD_TIME_ATTR_TIME_ZONE
    zint32                 i32TimeZone;
#endif

#ifdef CLD_TIME_ATTR_DST_START
    zuint32                u32DstStart;
#endif

#ifdef CLD_TIME_ATTR_DST_END
    zuint32                u32DstEnd;
#endif

#ifdef CLD_TIME_ATTR_DST_SHIFT
    zint32                 i32DstShift;
#endif

#ifdef CLD_TIME_ATTR_STANDARD_TIME
    zuint32                u32StandardTime;
#endif

#ifdef CLD_TIME_ATTR_LOCAL_TIME
    zuint32                u32LocalTime;
#endif

#ifdef CLD_TIME_ATTR_LAST_SET_TIME
    zutctime                u32LastSetTime;
#endif

#ifdef CLD_TIME_ATTR_VALID_UNTIL_TIME
    zutctime                u32ValidUntilTime;
#endif
#endif
    zuint16                 u16ClusterRevision;
} tsCLD_Time;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_TimeCreateTime(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8                              *pu8AttributeControlBits);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern tsZCL_ClusterDefinition sCLD_Time;
extern const tsZCL_AttributeDefinition asCLD_TimeClusterAttributeDefinitions[];
extern uint8 au8TimeClusterAttributeControlBits[];

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* TIME_H */
