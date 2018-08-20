/*!
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

#include "genfsk_interface.h"
#include "genfsk_ll.h"
#include "genfsk_utils.h"

#include "fsl_os_abstraction.h"
#include "fsl_xcvr.h"

#include "MemManager.h"
#include "FunctionLib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Maximum number of GENFSK LL software timers. */
#define gGENFSK_MaxTimers_c               (2)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Sets GENFSK timer T2 timeout value.
 *
 * @param pWaitTimeout the timeout value.
 */ 
static void GENFSK_TimeSetWaitTimeout(GENFSK_timestamp_t *pWaitTimeout); 

/*! @brief GENFSK Timer T2 overflow callback. */
static void GENFSK_TimeOverflowCb(void);

/*! @brief Runs the callback for the recently expired event. */
static void GENFSK_TimeRunCallback(void);

/*! @brief Checks for expired events too close to be scheduled and programs next event. */
static void GENFSK_TimeMaintenance(void);

/*! @brief Search for the next event to be scheduled. */
static GENFSK_TimeEvent_t* GENFSK_TimeGetNextEvent(void);

/*! @brief Initializes the GENFSK timer module. */
void GENFSK_TimeInit(void);

/*! @brief Disables GENFSK timer T2 timeout. */ 
void GENFSK_TimeDisableWaitTimeout(void);

/*! 
 * @brief Returns a 64bit timestamp value. 
 *
 * @retval GENFSK_timestamp_t timestamp.
 */
GENFSK_timestamp_t GENFSK_TimeGetTimestamp(void);

/*! 
 * @brief Schedules an event.
 *
 * @param pEvent event to be scheduled.
 *
 * @retval genfskTimerId_t the ID of the allocated timer.
 */
genfskTimerId_t GENFSK_TimeScheduleEvent(GENFSK_TimeEvent_t *pEvent);

/*! 
 * @brief Cancels an event.
 *
 * @param timerId the ID of the timer.
 */
void GENFSK_TimeCancelEvent(genfskTimerId_t timerId);

/*! @brief GENFSK time interrupt service routine. */ 
void GENFSK_TimeISR(void);

extern void GENFSK_EnableInterrupts(uint32_t mask);
extern void GENFSK_DisableInterrupts(uint32_t mask);

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

static void GENFSK_TimeSetWaitTimeout(GENFSK_timestamp_t *pWaitTimeout)
{   
    /* Enter critical section. */
    OSA_InterruptDisable();
    
    GENFSK->T2_CMP &= ~GENFSK_T2_CMP_T2_CMP_EN_MASK;
    GENFSK->T2_CMP = *pWaitTimeout & GENFSK_T2_CMP_T2_CMP_MASK;
    
    GENFSK_EnableInterrupts(GENFSK_IRQ_CTRL_T2_IRQ_EN_MASK);
    
    GENFSK->T2_CMP |= GENFSK_T2_CMP_T2_CMP_EN_MASK;
    
    /* Exit critical section. */
    OSA_InterruptEnable();
}

static void GENFSK_TimeRunCallback(void)
{
    genfskTimeCallback_t cb;
    
    if (pNextEvent)
    {
        /* Enter critical section. */
        OSA_InterruptDisable();
        
        cb = pNextEvent->callback;
        pNextEvent->callback = NULL;
        pNextEvent = NULL;
        
        /* Exit critical section. */
        OSA_InterruptEnable();
        
        cb();
    }
}

static void GENFSK_TimeMaintenance(void)
{
    GENFSK_timestamp_t currentTime;
    GENFSK_TimeEvent_t *pEvent;
    
    GENFSK_TimeDisableWaitTimeout();
    
    while(1)
    {
        /* Enter critical section. */
        OSA_InterruptDisable();
        
        pEvent = GENFSK_TimeGetNextEvent();
        currentTime = GENFSK_GetTimestamp();
        
        /* Program next event if exists. */
        if (pEvent)
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
        
        if (!pEvent)
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
    
    /* Enter critical section. */
    OSA_InterruptDisable();
    
    timestamp = (uint64_t)(GENFSK->EVENT_TMR & GENFSK_EVENT_TMR_EVENT_TMR_MASK);
    timestamp |= gGenfskTimerOverflow;
    /* Check for overflow. */
    if (pNextEvent->callback == GENFSK_TimeOverflowCb)
    {
        if (GENFSK->IRQ_CTRL & GENFSK_IRQ_CTRL_T2_IRQ_MASK)
        {
            timestamp += gGENFSK_OverflowTimerUnit_c;
        }
    }
    
    /* Exit critical section. */
    OSA_InterruptEnable();
    
    return timestamp;
}

void GENFSK_TimeDisableWaitTimeout(void)
{   
    GENFSK->T2_CMP &= ~GENFSK_T2_CMP_T2_CMP_EN_MASK;
    
    GENFSK_DisableInterrupts(GENFSK_IRQ_CTRL_T2_IRQ_EN_MASK);
}

genfskTimerId_t GENFSK_TimeScheduleEvent(GENFSK_TimeEvent_t *pEvent)
{
    genfskTimerId_t tmr;
    
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
            if ((NULL == pNextEvent) || ((NULL != pNextEvent) && (mGenfskTimers[tmr].timestamp < pNextEvent->timestamp)))
            {
                GENFSK_TimeMaintenance();
            }
        }
    }
    return tmr;
}

void GENFSK_TimeCancelEvent(genfskTimerId_t timerId)
{
    if ((timerId == 0) || (timerId >= gGENFSK_MaxTimers_c) || (NULL == mGenfskTimers[timerId].callback))
    {
        /* Timer not found. */
    }
    else
    {
        /* Enter critical section. */
        OSA_InterruptDisable();
        
        if (pNextEvent == &mGenfskTimers[timerId])
        {
            pNextEvent = NULL;
        }
            
        mGenfskTimers[timerId].callback = NULL;
        
        /* Exit critical section. */
        OSA_InterruptEnable();
    }
}

void GENFSK_TimeISR(void)
{
    if (pNextEvent->callback == GENFSK_TimeOverflowCb)
    {
        gGenfskTimerOverflow += gGENFSK_OverflowTimerUnit_c;
    }
    
    GENFSK_TimeRunCallback();
    GENFSK_TimeMaintenance();
}
