/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"
#include "TimersManager.h"
#include "SerialManager.h"
#include "Panic.h"

#if defined(FWK_SMALL_RAM_CONFIG)

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#if gTmrTaskStackSize_c > gSerialTaskStackSize_c
#define gFwkCommonStackSize_c gTmrTaskStackSize_c
#else
#define gFwkCommonStackSize_c gSerialTaskStackSize_c
#endif


/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
extern void SerialManagerTask(osaTaskParam_t argument);
extern void TMR_Task(osaTaskParam_t param);
static void FwkTask(osaTaskParam_t argument);
/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
OSA_TASK_DEFINE(FwkTask, gSerialTaskPriority_c, 1, gFwkCommonStackSize_c, FALSE);
osaEventId_t  gFwkCommonEventId;
osaTaskId_t gFwkCommonTaskId = 0;

extern const uint8_t gUseRtos_c;

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */
void FwkInit(void)
{
        
    if( gFwkCommonTaskId )
        return;

    gFwkCommonEventId = OSA_EventCreate(TRUE);
    if( gFwkCommonEventId == NULL )
    {
        panic(0,0,0,0);
        return;
    }

    gFwkCommonTaskId = OSA_TaskCreate(OSA_TASK(FwkTask), (osaTaskParam_t)NULL);
    if( gFwkCommonTaskId == NULL )
    {
        panic(0,0,0,0);
        return;
    }
}


static void FwkTask(osaTaskParam_t argument)
{
    while(1)
    {
        osaEventFlags_t  ev = 0;
        (void)OSA_EventWait(gFwkCommonEventId, osaEventFlagsAll_c, FALSE, osaWaitForever_c ,&ev);

#if gTMR_Enabled_d
        if(ev >> 16)
        {
            TMR_Task(0);
        }
#endif

#if gSerialManagerMaxInterfaces_c
        ev |= 0x0000FFFF;
        
        if( ev )
        {
            SerialManagerTask(0);
        }
#endif

        /* For BareMetal break the while(1) after 1 run */
        if (gUseRtos_c == 0)
        {
            break;
        }
    }
}

#endif /* FWK_SMALL_RAM_CONFIG */