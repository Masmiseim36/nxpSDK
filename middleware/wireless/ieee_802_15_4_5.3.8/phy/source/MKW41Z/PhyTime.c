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
#include "Phy.h"
#include "EmbeddedTypes.h"
#include "FunctionLib.h"

#include "fsl_os_abstraction.h"
#include "fsl_device_registers.h"

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#define gPhyTimeMinSetupTime_c (4) /* symbols */

#define BM_ZLL_IRQSTS_TMRxMSK (ZLL_IRQSTS_TMR1MSK_MASK | \
                               ZLL_IRQSTS_TMR2MSK_MASK | \
                               ZLL_IRQSTS_TMR3MSK_MASK | \
                               ZLL_IRQSTS_TMR4MSK_MASK )

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
void (*gpfPhyTimeNotify)(void) = NULL;


/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
static phyTimeEvent_t  mPhyTimers[gMaxPhyTimers_c];
static phyTimeEvent_t *pNextEvent;
volatile uint64_t      gPhyTimerOverflow;
static uint8_t         mPhyActiveTimers;


/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
static void PhyTime_OverflowCB( uint32_t param );
static phyTimeEvent_t* PhyTime_GetNextEvent( void );


/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief  Sets the start time of a sequence
*
* \param[in]  startTime  the start time for a sequence
*
********************************************************************************** */
void PhyTimeSetEventTrigger
(
phyTime_t startTime
)
{
    uint32_t irqSts;

    OSA_InterruptDisable();

    /* disable TMR2 compare */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TMR2CMP_EN_MASK;

    ZLL->T2PRIMECMP = startTime;

    /* unmask TMR2 interrupt (do not change other IRQ status) */
    irqSts  = ZLL->IRQSTS & BM_ZLL_IRQSTS_TMRxMSK;
    irqSts &= ~(ZLL_IRQSTS_TMR2MSK_MASK);
    /* aknowledge TMR2 IRQ */
    irqSts |= ZLL_IRQSTS_TMR2IRQ_MASK;
    ZLL->IRQSTS = irqSts;

    /* TC2PRIME_EN must be enabled
       enable TMR2 compare, enable autosequence start by TC2 match */
    ZLL->PHY_CTRL |= ZLL_PHY_CTRL_TMR2CMP_EN_MASK | ZLL_PHY_CTRL_TMRTRIGEN_MASK;

    OSA_InterruptEnable();
}

/*! *********************************************************************************
* \brief  Disable the time trigger for a sequence.
*
* \remarks The sequence will start asap
*
********************************************************************************** */
void PhyTimeDisableEventTrigger
(
void
)
{
    uint32_t irqSts;

    OSA_InterruptDisable();
    /* disable autosequence start by TC2 match */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TMRTRIGEN_MASK;
    /* disable TMR2 compare */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TMR2CMP_EN_MASK;
    /* mask TMR2 interrupt (do not change other IRQ status) */
    irqSts  = ZLL->IRQSTS & BM_ZLL_IRQSTS_TMRxMSK;
    irqSts |= ZLL_IRQSTS_TMR2MSK_MASK;
    /* aknowledge TMR2 IRQ */
    irqSts |= ZLL_IRQSTS_TMR2IRQ_MASK;
    ZLL->IRQSTS = irqSts;
    OSA_InterruptEnable();
}

/*! *********************************************************************************
* \brief  Sets the timeout value for a sequence
*
* \param[in]  pEndTime the absolute time when a sequence should terminate
*
* \remarks If the sequence does not finish until the timeout, it will be aborted
*
********************************************************************************** */
void PhyTimeSetEventTimeout
(
phyTime_t *pEndTime
)
{
    uint32_t irqSts;

#ifdef PHY_PARAMETERS_VALIDATION
    if(NULL == pEndTime)
    {
        return;
    }
#endif /* PHY_PARAMETERS_VALIDATION */

    OSA_InterruptDisable();

    /* disable TMR3 compare */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TMR3CMP_EN_MASK;

    ZLL->T3CMP = *pEndTime & 0x00FFFFFF;

    /* aknowledge TMR3 IRQ */
    irqSts  = ZLL->IRQSTS & BM_ZLL_IRQSTS_TMRxMSK;
    irqSts |= ZLL_IRQSTS_TMR3IRQ_MASK;
    ZLL->IRQSTS = irqSts;
    /* enable TMR3 compare */
    ZLL->PHY_CTRL |= ZLL_PHY_CTRL_TMR3CMP_EN_MASK;
    /* enable autosequence stop by TC3 match */
    ZLL->PHY_CTRL |= ZLL_PHY_CTRL_TC3TMOUT_MASK;

    OSA_InterruptEnable();
}

/*! *********************************************************************************
* \brief  Return the timeout value for the current sequence
*
* \return  uint32_t the timeout value
*
********************************************************************************** */
phyTime_t PhyTimeGetEventTimeout( void )
{
    return ZLL->T3CMP;
}

/*! *********************************************************************************
* \brief  Disables the sequence timeout
*
********************************************************************************** */
void PhyTimeDisableEventTimeout
(
void
)
{
    uint32_t irqSts;

    OSA_InterruptDisable();
    /* disable TMR3 compare */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TMR3CMP_EN_MASK;
    /* disable autosequence stop by TC3 match */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TC3TMOUT_MASK;
    /* mask TMR3 interrupt (do not change other IRQ status) */
    irqSts  = ZLL->IRQSTS & BM_ZLL_IRQSTS_TMRxMSK;
    irqSts |= ZLL_IRQSTS_TMR3MSK_MASK;
    /* aknowledge TMR3 IRQ */
    irqSts |= ZLL_IRQSTS_TMR3IRQ_MASK;
    ZLL->IRQSTS = irqSts;
    OSA_InterruptEnable();
}

/*! *********************************************************************************
* \brief  Reads the absolute clock from the radio
*
* \param[out]  pRetClk pointer to a location where the current clock will be stored
*
********************************************************************************** */
void PhyTimeReadClock
(
phyTime_t *pRetClk
)
{
#ifdef PHY_PARAMETERS_VALIDATION
    if(NULL == pRetClk)
    {
        return;
    }
#endif /* PHY_PARAMETERS_VALIDATION */

    *pRetClk = ZLL->EVENT_TMR >> ZLL_EVENT_TMR_EVENT_TMR_SHIFT;
}

/*! *********************************************************************************
* \brief  Initialize the Event Timer
*
* \param[in]  pAbsTime  pointer to the location where the new time is stored
*
********************************************************************************** */
void PhyTimeInitEventTimer
(
uint32_t *pAbsTime
)
{
#ifdef PHY_PARAMETERS_VALIDATION
    if(NULL == pAbsTime)
    {
        return;
    }
#endif /* PHY_PARAMETERS_VALIDATION */

    OSA_InterruptDisable();

    ZLL->EVENT_TMR = (*pAbsTime  << ZLL_EVENT_TMR_EVENT_TMR_SHIFT) | ZLL_EVENT_TMR_EVENT_TMR_LD_MASK;

    OSA_InterruptEnable();
}

/*! *********************************************************************************
* \brief  Set TMR1 timeout value
*
* \param[in]  pWaitTimeout the timeout value
*
********************************************************************************** */
void PhyTimeSetWaitTimeout
(
phyTime_t *pWaitTimeout
)
{
    uint32_t irqSts;

    OSA_InterruptDisable();

    /* disable TMR1 compare */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TMR1CMP_EN_MASK;

    ZLL->T1CMP = *pWaitTimeout;

    /* unmask TMR1 interrupt (do not change other IRQ status) */
    irqSts  = ZLL->IRQSTS & BM_ZLL_IRQSTS_TMRxMSK;
    irqSts &= ~(ZLL_IRQSTS_TMR1MSK_MASK);
    /* aknowledge TMR1 IRQ */
    irqSts |= ZLL_IRQSTS_TMR1IRQ_MASK;
    ZLL->IRQSTS = irqSts;
    /* enable TMR1 compare */
    ZLL->PHY_CTRL |= ZLL_PHY_CTRL_TMR1CMP_EN_MASK;

    OSA_InterruptEnable();
}

/*! *********************************************************************************
* \brief  Disable the TMR1 timeout
*
********************************************************************************** */
void PhyTimeDisableWaitTimeout
(
void
)
{
    uint32_t irqSts;

    OSA_InterruptDisable();
    /* disable TMR1 compare */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TMR1CMP_EN_MASK;
    /* mask TMR1 interrupt (do not change other IRQ status) */
    irqSts  = ZLL->IRQSTS & BM_ZLL_IRQSTS_TMRxMSK;
    irqSts |= ZLL_IRQSTS_TMR1MSK_MASK;
    /* aknowledge TMR1 IRQ */
    irqSts |= ZLL_IRQSTS_TMR1IRQ_MASK;
    ZLL->IRQSTS = irqSts;
    OSA_InterruptEnable();
}

/*! *********************************************************************************
* \brief  Set TMR4 timeout value
*
* \param[in]  pWakeUpTime  absolute time
*
********************************************************************************** */
void PhyTimeSetWakeUpTime
(
uint32_t *pWakeUpTime
)
{
    uint32_t irqSts;

    OSA_InterruptDisable();

    /* disable TMR4 compare */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TMR4CMP_EN_MASK;

    ZLL->T4CMP = *pWakeUpTime;
    
    /* unmask TMR4 interrupt (do not change other IRQ status) */
    irqSts  = ZLL->IRQSTS & BM_ZLL_IRQSTS_TMRxMSK;
    irqSts &= ~(ZLL_IRQSTS_TMR4MSK_MASK);
    /* aknowledge TMR4 IRQ */
    irqSts |= ZLL_IRQSTS_TMR4IRQ_MASK;
    ZLL->IRQSTS = irqSts;
    /* enable TMR4 compare */
    ZLL->PHY_CTRL |= ZLL_PHY_CTRL_TMR4CMP_EN_MASK;

    OSA_InterruptEnable();
}

/*! *********************************************************************************
* \brief  Check if TMR4 IRQ occured, and aknowledge it
*
* \return  TRUE if TMR4 IRQ occured
*
********************************************************************************** */
bool_t PhyTimeIsWakeUpTimeExpired
(
void
)
{
    bool_t wakeUpIrq = FALSE;
    uint32_t irqSts;

    OSA_InterruptDisable();
    
    irqSts = ZLL->IRQSTS;

    /* disable TMR4 compare */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TMR4CMP_EN_MASK;

    if( (irqSts & ZLL_IRQSTS_TMR4IRQ_MASK) == ZLL_IRQSTS_TMR4IRQ_MASK )
    {
        wakeUpIrq = TRUE;
    }

    /* unmask TMR4 interrupt (do not change other IRQ status) */
    irqSts &= BM_ZLL_IRQSTS_TMRxMSK;
    irqSts &= ~(ZLL_IRQSTS_TMR4MSK_MASK);
    /* aknowledge TMR4 IRQ */
    irqSts |= ZLL_IRQSTS_TMR4IRQ_MASK;
    ZLL->IRQSTS = irqSts;

    OSA_InterruptEnable();

    return wakeUpIrq;
}


/*! *********************************************************************************
* \brief  PHY Timer Interrupt Service Routine
*
********************************************************************************** */
void PhyTime_ISR(void)
{
    if( pNextEvent->callback == PhyTime_OverflowCB )
    {
        gPhyTimerOverflow += (uint64_t)(1 << gPhyTimeShift_c);
    }
    
    if( gpfPhyTimeNotify )
    {
        gpfPhyTimeNotify();
    }
    else
    {
        PhyTime_RunCallback();
        PhyTime_Maintenance();
    }
}

/*! *********************************************************************************
* \brief  Initialize the PHY Timer module
*
* \return  phyTimeStatus_t
*
********************************************************************************** */
phyTimeStatus_t PhyTime_TimerInit( void (*cb)(void) )
{
    phyTimeStatus_t status = gPhyTimeOk_c;
    if( gpfPhyTimeNotify )
    {
        status = gPhyTimeError_c;
    }
    else
    {
        gpfPhyTimeNotify = cb;
        gPhyTimerOverflow = 0;
        FLib_MemSet( mPhyTimers, 0, sizeof(mPhyTimers) );
        
        /* Schedule Overflow Calback */
        pNextEvent = &mPhyTimers[0];
        pNextEvent->callback = PhyTime_OverflowCB;
        pNextEvent->timestamp = (uint64_t)(1 << gPhyTimeShift_c);
        PhyTimeSetWaitTimeout( &pNextEvent->timestamp );
        mPhyActiveTimers = 1;
    }

    return status;
}

/*! *********************************************************************************
* \brief  Returns a 64bit timestamp value to be used by the MAC Layer
*
* \return  phyTime_t PHY timestamp
*
********************************************************************************** */
phyTime_t PhyTime_GetTimestamp(void)
{
    phyTime_t t = 0;

    OSA_InterruptDisable();
    PhyTimeReadClock( &t );
    t |= gPhyTimerOverflow;
    /* Check for overflow */
    if( pNextEvent->callback == PhyTime_OverflowCB )
    {
        if( ZLL->IRQSTS & ZLL_IRQSTS_TMR1IRQ_MASK )
        {
            t += (1 << gPhyTimeShift_c);
        }
    }
    OSA_InterruptEnable();

    return t;
}

/*! *********************************************************************************
* \brief  Schedules an event
*
* \param[in]  pEvent  event to be scheduled
*
* \return  phyTimeTimerId_t  the id of the alocated timer
*
********************************************************************************** */
phyTimeTimerId_t PhyTime_ScheduleEvent( phyTimeEvent_t *pEvent )
{
    phyTimeTimerId_t tmr;

    /* Parameter validation */
    if( NULL == pEvent->callback )
    {
        tmr = gInvalidTimerId_c;
    }
    else
    {
        /* Search for a free slot (slot 0 is reserved for the Overflow calback) */
        OSA_InterruptDisable();
        for( tmr=1; tmr<gMaxPhyTimers_c; tmr++ )
        {
            if( mPhyTimers[tmr].callback == NULL )
            {
                if( mPhyActiveTimers == 1 )
                {
                    PWR_DisallowXcvrToSleep();
                }

                mPhyActiveTimers++;
                mPhyTimers[tmr] = *pEvent;
                break;
            }
        }
        OSA_InterruptEnable();
        
        if( tmr >= gMaxPhyTimers_c )
        {
            tmr = gInvalidTimerId_c;
        }
        else
        {
            /* Program the next event */
            if((NULL == pNextEvent) ||
               ((NULL != pNextEvent)  && (mPhyTimers[tmr].timestamp < pNextEvent->timestamp)))
            {
                PhyTime_Maintenance();
            }
        }
    }
    return tmr;
}

/*! *********************************************************************************
* \brief  Cancel an event
*
* \param[in]  timerId  the Id of the timer
*
* \return  phyTimeStatus_t
*
********************************************************************************** */
phyTimeStatus_t PhyTime_CancelEvent( phyTimeTimerId_t timerId )
{
    phyTimeStatus_t status = gPhyTimeOk_c;

    if( (timerId == 0) || (timerId >= gMaxPhyTimers_c) || (NULL == mPhyTimers[timerId].callback) )
    {
        status = gPhyTimeNotFound_c;
    }
    else
    {
        OSA_InterruptDisable();
        if( pNextEvent == &mPhyTimers[timerId] )
        {
            pNextEvent = NULL;
        }
        
        mPhyTimers[timerId].callback = NULL;
        mPhyActiveTimers--;

        if( mPhyActiveTimers == 1 )
        {
            PWR_AllowXcvrToSleep();
        }

        OSA_InterruptEnable();
    }

    return status;
}

/*! *********************************************************************************
* \brief  Cancel all event with the specified paameter
*
* \param[in]  param  event parameter
*
* \return  phyTimeStatus_t
*
********************************************************************************** */
phyTimeStatus_t PhyTime_CancelEventsWithParam ( uint32_t param )
{
    uint32_t i;
    phyTimeStatus_t status = gPhyTimeNotFound_c;

    OSA_InterruptDisable();
    for( i=1; i<gMaxPhyTimers_c; i++ )
    {
        if( (NULL != mPhyTimers[i].callback) && (param == mPhyTimers[i].parameter) )
        {
            status = gPhyTimeOk_c;
            mPhyTimers[i].callback = NULL;
            mPhyActiveTimers--;

            if( pNextEvent == &mPhyTimers[i] )
            {
                pNextEvent = NULL;
            }
        }
    }

    if( mPhyActiveTimers == 1 )
    {
        PWR_AllowXcvrToSleep();
    }
    OSA_InterruptEnable();

    return status;
}

/*! *********************************************************************************
* \brief  Run the callback for the recently expired event
*
********************************************************************************** */
void PhyTime_RunCallback( void )
{
    uint32_t param;
    phyTimeCallback_t cb = NULL;

    OSA_InterruptDisable();

    if (pNextEvent)
    {
        param = pNextEvent->parameter;
        cb = pNextEvent->callback;
        pNextEvent->callback = NULL;
        pNextEvent = NULL;
        mPhyActiveTimers--;

        if( mPhyActiveTimers == 1 )
        {
            PWR_AllowXcvrToSleep();
        }
    }

    OSA_InterruptEnable();
    
    if (cb)
    {
        cb(param);
    }
}

/*! *********************************************************************************
* \brief  Expire events too close to be scheduled.
*         Program the next event
*
********************************************************************************** */
void PhyTime_Maintenance( void )
{
    static uint8_t busy = 0;
    phyTime_t currentTime;

    OSA_InterruptDisable();
    if (busy)
    {
        /* Already running. Just exit to avoid recursive calls */
        OSA_InterruptEnable();
    }
    else
    {
        busy = 1;
        OSA_InterruptEnable();
        
        PhyTimeDisableWaitTimeout();
        
        while(busy)
        {
            OSA_InterruptDisable();
            pNextEvent = PhyTime_GetNextEvent();
            currentTime = PhyTime_GetTimestamp();
            
            /* Program next event if exists */
            if( pNextEvent )
            {
                /* Check if the event has expired or is to close to schedule */
                if( pNextEvent->timestamp > (currentTime + gPhyTimeMinSetupTime_c) )
                {
                    PhyTimeSetWaitTimeout( &pNextEvent->timestamp );
                    busy = 0;
                }
            }
            else
            {
                busy = 0;
            }
            
            OSA_InterruptEnable();
            
            if( busy )
            {
                /* Event expired. Run Callback */
                PhyTime_RunCallback();
            }
        }
    }
}

/*! *********************************************************************************
* \brief  Timer Overflow callback
*
* \param[in]  param
*
********************************************************************************** */
static void PhyTime_OverflowCB( uint32_t param )
{
    param = param;

    /* Reprogram the next overflow callback */
    OSA_InterruptDisable();
    mPhyActiveTimers++;
    mPhyTimers[0].callback = PhyTime_OverflowCB;
    mPhyTimers[0].timestamp = gPhyTimerOverflow + (1 << gPhyTimeShift_c);
    OSA_InterruptEnable();
}

/*! *********************************************************************************
* \brief  Search for the next event to be scheduled
*
* \return phyTimeEvent_t pointer to the next event to be scheduled
*
********************************************************************************** */
static phyTimeEvent_t* PhyTime_GetNextEvent( void )
{
    phyTimeEvent_t *pEv = NULL;
    uint32_t i;

    /* Search for the next event to be serviced */
    for( i=0; i<gMaxPhyTimers_c; i++ )
    {
        if( NULL != mPhyTimers[i].callback )
        {
            if( NULL == pEv )
            {
                pEv = &mPhyTimers[i];
            }
            /* Check which event expires first */
            else
            {
                if( mPhyTimers[i].timestamp < pEv->timestamp )
                {
                    pEv = &mPhyTimers[i];
                }
            }
        }
    }

    return pEv;
}
