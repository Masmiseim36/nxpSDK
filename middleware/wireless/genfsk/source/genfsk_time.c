/*

Copyright 2016-2019 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

#include "genfsk_interface.h"
#include "genfsk_ll.h"
#include "genfsk_utils.h"

#include "fsl_os_abstraction.h"

#include "FunctionLib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifndef gGENFSK_MaxAppTimers_c
#define gGENFSK_MaxAppTimers_c (0U)
#endif

/*! @brief Maximum number of GENFSK LL software timers. */
#define gGENFSK_MaxTimers_c (2U + gGENFSK_MaxAppTimers_c)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Sets GENFSK timer T2 timeout value.
 *
 * @param pWaitTimeout the timeout value.
 */ 
GENFSK_STATIC void GENFSK_TimeSetWaitTimeout(GENFSK_timestamp_t *pWaitTimeout); 

/*! @brief GENFSK Timer T2 overflow callback. */
static void GENFSK_TimeOverflowCb(void);

/*! @brief Runs the callback for the recently expired event. */
static void GENFSK_TimeRunCallback(void);

/*! @brief Checks for expired events too close to be scheduled and programs next event. */
GENFSK_STATIC void GENFSK_TimeMaintenance(void);

/*! @brief Search for the next event to be scheduled. */
static GENFSK_TimeEvent_t* GENFSK_TimeGetNextEvent(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Next event to be scheduled. */
static GENFSK_TimeEvent_t *pNextEvent;

/*! @brief GENFSK timer module internal structure. */
static GENFSK_TimeEvent_t mGenfskTimers[gGENFSK_MaxTimers_c];

/*! @brief GENFSK timer overflow value. */
volatile uint64_t gGenfskTimerOverflow = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

GENFSK_STATIC void GENFSK_TimeSetWaitTimeout(GENFSK_timestamp_t *pWaitTimeout)
{   
    /* Enter critical section. */
    OSA_InterruptDisable();
    
    GENFSK->T2_CMP &= ~GENFSK_T2_CMP_T2_CMP_EN_MASK;
    GENFSK->T2_CMP = (uint32_t) (*pWaitTimeout & GENFSK_T2_CMP_T2_CMP_MASK);
    
    GENFSK_EnableInterrupts(GENFSK_IRQ_CTRL_T2_IRQ_EN_MASK);
    
    GENFSK->T2_CMP |= GENFSK_T2_CMP_T2_CMP_EN_MASK;
    
    /* Exit critical section. */
    OSA_InterruptEnable();
}

static void GENFSK_TimeRunCallback(void)
{
    genfskTimeCallback_t cb = NULL;

    /* Enter critical section. */
    OSA_InterruptDisable();
    
    if (pNextEvent != NULL)
    {
        cb = pNextEvent->callback;
        pNextEvent->callback = NULL;
        pNextEvent = NULL;
    }

    /* Exit critical section. */
    OSA_InterruptEnable();

    if (cb != NULL)
    {
    	cb();
    }
}

GENFSK_STATIC void GENFSK_TimeMaintenance(void)
{
    GENFSK_timestamp_t currentTime;
    GENFSK_TimeEvent_t *pEvent;
    
    GENFSK_TimeDisableWaitTimeout();
    
    while (TRUE)
    {
        /* Enter critical section. */
        OSA_InterruptDisable();
        
        pEvent = GENFSK_TimeGetNextEvent();
        currentTime = GENFSK_GetTimestamp();
        
        /* Program next event if exists. */
        if (pEvent != NULL)
        {
            pNextEvent = pEvent;
            
            if (pNextEvent->timestamp > (currentTime + gGENFSK_MinSetupTime_c))
            {
                GENFSK_TimeSetWaitTimeout(&pEvent->timestamp);
                pEvent = NULL;
            }
        }
        
        /* Exit critical section. */
        OSA_InterruptEnable();
        
        if (pEvent == NULL)
        {
            break;
        }
        
        GENFSK_TimeRunCallback();
    }    
}

static void GENFSK_TimeOverflowCb(void)
{       
    /* Reprogram next overflow callback. */
    mGenfskTimers[0].callback = GENFSK_TimeOverflowCb;
    mGenfskTimers[0].timestamp = gGenfskTimerOverflow + gGENFSK_OverflowTimerUnit_c;
}

/* For coverage test only */
#if (defined(GCOV_DO_COVERAGE) && (GCOV_DO_COVERAGE == 1))
/* LCOV_EXCL_START */
extern void GENFSK_TimeSetCallback0 (bool_t is_restore_original_callback)
{
    if (is_restore_original_callback)
    {
        mGenfskTimers[0].callback = GENFSK_TimeOverflowCb;
    }
    else
    {
        mGenfskTimers[0].callback = NULL;
    }
}
/* LCOV_EXCL_STOP */
#endif /* (GCOV_DO_COVERAGE == 1) */

/*! @brief Search for the next event to be scheduled. */
static GENFSK_TimeEvent_t* GENFSK_TimeGetNextEvent(void)
{
    GENFSK_TimeEvent_t *pEvent = NULL;
    uint8_t i;
    
    for (i = 0; i < gGENFSK_MaxTimers_c; i++)
    {
        if (NULL != mGenfskTimers[i].callback)
        {
            /* Select the event which expires first. */
            if (NULL == pEvent)
            {
                pEvent = &mGenfskTimers[i];
            }
            else
            {
                if (mGenfskTimers[i].timestamp < pEvent->timestamp)
                {
                    pEvent = &mGenfskTimers[i];
                }
            }
        }
    }
    
    return pEvent;
}

void GENFSK_TimeInit(void)
{
    gGenfskTimerOverflow = 0;

    /* Clear timer structures array */
    FLib_MemSet (mGenfskTimers, 0, sizeof(mGenfskTimers));
    
    /* Schedule overflow callback. */
    pNextEvent = &mGenfskTimers[0];
    pNextEvent->callback = GENFSK_TimeOverflowCb;
    pNextEvent->timestamp = gGENFSK_OverflowTimerUnit_c;
    GENFSK_TimeSetWaitTimeout(&pNextEvent->timestamp);
}

GENFSK_timestamp_t GENFSK_TimeGetTimestamp(void)
{
    GENFSK_timestamp_t timestamp = 0;
    uint32_t temp;
    
    /* Enter critical section. */
    OSA_InterruptDisable();
    
    temp = GENFSK->EVENT_TMR & GENFSK_EVENT_TMR_EVENT_TMR_MASK;
    timestamp = (uint64_t) temp;
    timestamp |= gGenfskTimerOverflow;
    
    /* Check for overflow. */
    if (pNextEvent->callback == GENFSK_TimeOverflowCb)
    {
        if ((GENFSK->IRQ_CTRL & GENFSK_IRQ_CTRL_T2_IRQ_MASK) != 0U)
        {
            timestamp += gGENFSK_OverflowTimerUnit_c;
        }
    }
    
    /* Exit critical section. */
    OSA_InterruptEnable();
    
    return timestamp;
}

/*
 * Disable T2_CMP interrupts
 */
void GENFSK_TimeDisableWaitTimeout(void)
{   
    GENFSK->T2_CMP &= ~GENFSK_T2_CMP_T2_CMP_EN_MASK;
    
    GENFSK_DisableInterrupts(GENFSK_IRQ_CTRL_T2_IRQ_EN_MASK);
}

genfskTimerId_t GENFSK_TimeScheduleEvent(GENFSK_TimeEvent_t *pEvent)
{
    genfskTimerId_t tmr;

    /*
    * Add an event to the queue of Timer Events (mGenfskTimers)
    * For now, events are not ordered and simply added in an array as very few events (timers) are supported.
    */    
    if (NULL == pEvent->callback)
    {
        tmr = gGENFSK_InvalidTimerId_c;
    }
    else
    {
        /* Enter critical section. */
        OSA_InterruptDisable();
        
        for (tmr = 1; tmr < gGENFSK_MaxTimers_c; tmr++)
        {
            if (mGenfskTimers[tmr].callback == NULL)
            {
                mGenfskTimers[tmr].callback = pEvent->callback;
                mGenfskTimers[tmr].timestamp = pEvent->timestamp;
                break;
            }
        }
        
        /* Exit critical section. */
        OSA_InterruptEnable();
        
        if (tmr >= gGENFSK_MaxTimers_c)
        {
            tmr = gGENFSK_InvalidTimerId_c;            
        }
        else
        {
            if (NULL == pNextEvent)
            {
                GENFSK_TimeMaintenance();
            }
            else
            {
                if (mGenfskTimers[tmr].timestamp < pNextEvent->timestamp)
                {
                    GENFSK_TimeMaintenance();
                }
            }
        }
    }
    return tmr;
}

void GENFSK_TimeCancelEvent(genfskTimerId_t *pTimerId)
{
    /* Never cancel overflow event */
    if ((pTimerId == NULL) || (*pTimerId == 0U) || (*pTimerId >= gGENFSK_MaxTimers_c))
    {
        /* Timer not found. */
    }
    else if (NULL == mGenfskTimers[*pTimerId].callback)
    {
        *pTimerId = gGENFSK_InvalidTimerId_c;
    }
    else
    {
        /* Enter critical section. */
        OSA_InterruptDisable();
        
        if (pNextEvent == &mGenfskTimers[*pTimerId])
        {
            pNextEvent = NULL;
        }
            
        mGenfskTimers[*pTimerId].callback = NULL;

        *pTimerId = gGENFSK_InvalidTimerId_c;

        /* Exit critical section. */
        OSA_InterruptEnable();
    }
}

/* Interrupt service routine for GENFSK_IRQ_CTRL_T2_IRQ. */
void GENFSK_TimeISR(void)
{
    /* Increment global overflow counter in case event is GENFSK_TimeOverflowCb */
    if ((pNextEvent != NULL) && (pNextEvent->callback == GENFSK_TimeOverflowCb))
    {
        gGenfskTimerOverflow += gGENFSK_OverflowTimerUnit_c;
    }

    /* Run callback corresponding to event that just expired */
    GENFSK_TimeRunCallback();
    GENFSK_TimeMaintenance();
}
