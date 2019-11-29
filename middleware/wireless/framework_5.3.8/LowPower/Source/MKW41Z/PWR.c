/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/*****************************************************************************
*                               INCLUDED HEADERS                            *
*---------------------------------------------------------------------------*
* Add to this section all the headers that this module needs to include.    *
*---------------------------------------------------------------------------*
*****************************************************************************/
#include "EmbeddedTypes.h"
#include "PWR_Configuration.h"
#include "PWRLib.h"
#include "PWR_Interface.h"
#include "TimersManager.h"
#include "fsl_device_registers.h"
#include "fsl_os_abstraction.h"
#include "board.h"
#include "clock_config.h"
#if (cPWR_BLE_LL_Enable)
#include "PWR_BLE.h"
#endif

/*****************************************************************************
*                             PRIVATE MACROS                                *
*---------------------------------------------------------------------------*
* Add to this section all the access macros, registers mappings, bit access *
* macros, masks, flags etc ...                                              *
*---------------------------------------------------------------------------*
*****************************************************************************/
#define mBLETimoutMaxMs_c       40959
#define mPWR_DSM_TimerMask_c    (0xffffff)
#define mPWR_DSM_InvalidTime_c  (0xffffffff)
#define mPWR_DSM_MaxTimeCount_c (0x1000000)
#define VECTOR_PENDING (((*((volatile uint32_t*)0xE000ED04))>>12)&0x3f)
/*****************************************************************************
*                               PUBLIC VARIABLES                            *
*---------------------------------------------------------------------------*
* Add to this section all the variables and constants that have global      *
* (project) scope.                                                          *
* These variables / constants can be accessed outside this module.          *
* These variables / constants shall be preceded by the 'extern' keyword in  *
* the interface header.                                                     *
*---------------------------------------------------------------------------*
*****************************************************************************/

/*****************************************************************************
*                           PRIVATE FUNCTIONS PROTOTYPES                    *
*---------------------------------------------------------------------------*
* Add to this section all the functions prototypes that have local (file)   *
* scope.                                                                    *
* These functions cannot be accessed outside this module.                   *
* These declarations shall be preceded by the 'static' keyword.             *
*---------------------------------------------------------------------------*
*****************************************************************************/
#if (cPWR_UsePowerDownMode)
#if cPWR_EnableDeepSleepMode_1
static void PWR_HandleDeepSleepMode_1(void);
#endif
#if (cPWR_EnableDeepSleepMode_2 || cPWR_EnableDeepSleepMode_3)
static void PWR_HandleDeepSleepMode_2_3(void);
#endif
#if cPWR_EnableDeepSleepMode_4
static void PWR_HandleDeepSleepMode_4(void);
#endif
#if cPWR_EnableDeepSleepMode_5
static void PWR_HandleDeepSleepMode_5(void);   
#endif
#if cPWR_EnableDeepSleepMode_6
static void PWR_HandleDeepSleepMode_6(void);      
#endif
#if cPWR_EnableDeepSleepMode_7
static void PWR_HandleDeepSleepMode_7(void);      
#endif
#endif /* (cPWR_UsePowerDownMode) */

/*****************************************************************************
*                        PUBLIC TYPE DEFINITIONS                            *
*---------------------------------------------------------------------------*
* Add to this section all the data types definitions: stuctures, unions,    *
* enumerations, typedefs ...                                                *
*---------------------------------------------------------------------------*
*****************************************************************************/
typedef enum 
{
    PWR_Run = 77,
    PWR_Sleep,
    PWR_DeepSleep,
    PWR_Reset,
    PWR_OFF
} PWR_CheckForAndEnterNewPowerState_t;

typedef  void (*pfHandleDeepSleepFunc_t)(void);

/*****************************************************************************
*                               PRIVATE VARIABLES                           *
*---------------------------------------------------------------------------*
* Add to this section all the variables and constants that have local       *
* (file) scope.                                                             *
* Each of this declarations shall be preceded by the 'static' keyword.      *
* These variables / constants cannot be accessed outside this module.       *
*---------------------------------------------------------------------------*
*****************************************************************************/
uint8_t mLPMFlag = gAllowDeviceToSleep_c;
uint8_t mLpmXcvrDisallowCnt = 0;
uint64_t mLpmTotalSleepDuration;

#if (cPWR_UsePowerDownMode)
static uint32_t mPWR_DeepSleepTimeMs = cPWR_DeepSleepDurationMs;

#if (cPWR_GENFSK_LL_Enable)
static uint32_t mPWR_AbsoluteWakeupTimeInDsmTicks = mPWR_DSM_InvalidTime_c;
#endif

const pfHandleDeepSleepFunc_t maHandleDeepSleep[] =
{
#if cPWR_EnableDeepSleepMode_1  
    PWR_HandleDeepSleepMode_1,
#else
    (pfHandleDeepSleepFunc_t)0,
#endif  
#if cPWR_EnableDeepSleepMode_2    
    PWR_HandleDeepSleepMode_2_3,
#else
    (pfHandleDeepSleepFunc_t)0,
#endif  
#if cPWR_EnableDeepSleepMode_3                                                    
    PWR_HandleDeepSleepMode_2_3,
#else
    (pfHandleDeepSleepFunc_t)0,
#endif  
#if cPWR_EnableDeepSleepMode_4  
    PWR_HandleDeepSleepMode_4,
#else
    (pfHandleDeepSleepFunc_t)0,
#endif                          
#if cPWR_EnableDeepSleepMode_5    
    PWR_HandleDeepSleepMode_5,
#else
    (pfHandleDeepSleepFunc_t)0,
#endif                          
#if cPWR_EnableDeepSleepMode_6      
    PWR_HandleDeepSleepMode_6,
#else
    (pfHandleDeepSleepFunc_t)0,
#endif
#if cPWR_EnableDeepSleepMode_7
    PWR_HandleDeepSleepMode_7
#else
    (pfHandleDeepSleepFunc_t)0,
#endif
};

static pfPWRCallBack_t gpfPWR_LowPowerEnterCb;
static pfPWRCallBack_t gpfPWR_LowPowerExitCb;
#endif /* (cPWR_UsePowerDownMode) */

/*****************************************************************************
*                             PRIVATE FUNCTIONS                             *
*---------------------------------------------------------------------------*
* Add to this section all the functions that have local (file) scope.       *
* These functions cannot be accessed outside this module.                   *
* These definitions shall be preceded by the 'static' keyword.              *
*---------------------------------------------------------------------------*
*****************************************************************************/
/*---------------------------------------------------------------------------
* Name: PWR_GenfskGetDsmDurationMs
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
#if defined(__IAR_SYSTEMS_ICC__)
__weak uint32_t PWR_GenfskGetDsmDurationMs(void)
#elif defined(__GNUC__)
__attribute__((weak)) uint32_t PWR_GenfskGetDsmDurationMs(void)
#endif
{
    return mPWR_DeepSleepTimeMs >> 3;
}
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */

/*---------------------------------------------------------------------------
* Name: PWR_GENFSK_EnterDSM
* Description: - 
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
bool_t PWR_GENFSK_EnterDSM(uint32_t dsmDuration)
{
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
    uint32_t dsmOscOffset;
    uint64_t durationInDsmTicks;

    /* Compute the duration in 32.768KHz ticks. */
    durationInDsmTicks = (dsmDuration << 15)/1000;
        
    if(durationInDsmTicks > mPWR_DSM_TimerMask_c)
    {
        durationInDsmTicks = mPWR_DSM_TimerMask_c;
    }
    
    dsmOscOffset = 1024 << READ_REGISTER_FIELD(RSIM->RF_OSC_CTRL,RSIM_RF_OSC_CTRL_BB_XTAL_READY_COUNT_SEL);
    dsmOscOffset = (dsmOscOffset << 15)/BOARD_BOOTCLOCKRUN_CORE_CLOCK;
    
    if(durationInDsmTicks > dsmOscOffset)
    {
        uint32_t dsmSleep;
        
        RSIM->DSM_OSC_OFFSET = dsmOscOffset;  /* Set the DSM oscillator offset. */
        dsmSleep = (RSIM->DSM_TIMER + 5) & mPWR_DSM_TimerMask_c;  /* Compute the sleep time. No fewer than 4 clocks greater than DSM_TIMER. */
        RSIM->GEN_SLEEP = dsmSleep;  
        mPWR_AbsoluteWakeupTimeInDsmTicks = (dsmSleep + durationInDsmTicks) & mPWR_DSM_TimerMask_c;  /* Compute wake-up time. */
        RSIM->GEN_WAKE = mPWR_AbsoluteWakeupTimeInDsmTicks;
        GENFSK->DSM_CTRL = GENFSK_DSM_CTRL_GENERIC_FSK_SLEEP_EN_MASK;  /* Enter DSM */
    }
    else
    {
        mPWR_AbsoluteWakeupTimeInDsmTicks = mPWR_DSM_InvalidTime_c;
        
        return FALSE;
    }
    
    return TRUE;
#else
    return FALSE;
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
}

/*---------------------------------------------------------------------------
* Name: PWR_HandleGenfskDsmEnter
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
static bool_t PWR_HandleGenfskDsmEnter(void)
{
    uint32_t currentDsmTime;
    uint32_t dsmOscOffset;
    uint32_t durationInDsmTicks = 0;
    bool_t retVal = FALSE;
    
    currentDsmTime = RSIM->DSM_TIMER;  /* Get the current DSM time. */
    
    /* Compute the remaining sleep period. */
    if (mPWR_AbsoluteWakeupTimeInDsmTicks != mPWR_DSM_InvalidTime_c)
    {
        if (currentDsmTime < mPWR_AbsoluteWakeupTimeInDsmTicks)
        {        
            durationInDsmTicks = mPWR_AbsoluteWakeupTimeInDsmTicks - currentDsmTime;
        }
        else
        {
            durationInDsmTicks = mPWR_DSM_MaxTimeCount_c - currentDsmTime + mPWR_AbsoluteWakeupTimeInDsmTicks;
        }
    }
    
    dsmOscOffset = 1024 << READ_REGISTER_FIELD(RSIM->RF_OSC_CTRL,RSIM_RF_OSC_CTRL_BB_XTAL_READY_COUNT_SEL);
    dsmOscOffset = (dsmOscOffset << 15)/BOARD_BOOTCLOCKRUN_CORE_CLOCK;
    
    if(durationInDsmTicks > dsmOscOffset)
    {
        uint32_t dsmSleep;
        
        RSIM->DSM_OSC_OFFSET = dsmOscOffset;
        dsmSleep = (RSIM->DSM_TIMER + 5) & mPWR_DSM_TimerMask_c;
        RSIM->GEN_SLEEP = dsmSleep;
        RSIM->GEN_WAKE = mPWR_AbsoluteWakeupTimeInDsmTicks;
        GENFSK->DSM_CTRL = GENFSK_DSM_CTRL_GENERIC_FSK_SLEEP_EN_MASK;
        retVal = TRUE;
    }
    
    return retVal;
}
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */

/*---------------------------------------------------------------------------
* Name: PWR_HandleGenfskDsmExit
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
static void PWR_HandleGenfskDsmExit(void)
{
    if(READ_REGISTER_FIELD(RSIM->DSM_CONTROL,RSIM_DSM_CONTROL_GEN_SLEEP_REQUEST))
    {
        uint32_t remainTicksToSleep;
        uint32_t dsmTimer = RSIM->DSM_TIMER;
        
        remainTicksToSleep = RSIM->GEN_WAKE;
        remainTicksToSleep = (remainTicksToSleep - dsmTimer) & mPWR_DSM_TimerMask_c;
        
        if(remainTicksToSleep > 5)
        {
            RSIM->GEN_WAKE = (dsmTimer + 5) & mPWR_DSM_TimerMask_c;
        }
    }
}
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */

/*---------------------------------------------------------------------------
* Name: PWR_HandleDeepSleepMode_1
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_1)
static void PWR_HandleDeepSleepMode_1(void)
{
#if cPWR_BLE_LL_Enable
#if (gTMR_EnableLowPowerTimers_d)   
    uint32_t notCountedTicksBeforeSleep = 0;
#endif /* (gTMR_EnableLowPowerTimers_d) */
    uint8_t clkMode;
    uint32_t lptmrTicks;
    uint32_t lptmrFreq;
    uint32_t sleepTimeMs;
    uint32_t sysTickCtrl;
    bool_t enterLowPower = !RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT);
    bool_t dsmExit = FALSE;
    
    if(enterLowPower)
    {
        /* Disable SysTick counter and interrupt */
        sysTickCtrl = SysTick->CTRL & (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
        SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);        
#if (gTMR_EnableLowPowerTimers_d) && (cPWR_CheckLowPowerTimers)
            /* Get the expire time of the first programmed Low Power Timer */
            sleepTimeMs = TMR_GetFirstExpireTime(gTmrLowPowerTimer_c);
            
            if(mPWR_DeepSleepTimeMs < sleepTimeMs)
#endif /* (gTMR_EnableLowPowerTimers_d) && (cPWR_CheckLowPowerTimers) */
            {
                sleepTimeMs = mPWR_DeepSleepTimeMs;
            }
            PWRLib_MCU_WakeupReason.AllBits = 0;  
            PWRLib_LPTMR_GetTimeSettings(sleepTimeMs ,&clkMode ,&lptmrTicks, &lptmrFreq);
            PWRLib_LPTMR_ClockStart(clkMode,lptmrTicks);
            
#if (gTMR_EnableLowPowerTimers_d)
            /* If more low power timers are running, stop the hardware timer
            * and save the spend time in ticks that wasn't counted.
            */
            notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif /* (gTMR_EnableLowPowerTimers_d) */        
        
        
        if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT)) /* If false it means that BLE_LL is already in DSM */
        {
            uint16_t distanceToNextInstant;
            uint16_t nextInstant;
            uint16_t offsetBeforeDsmExit;   
            
            if(BLE_LL_REG(CLOCK_CONFIG) & BLE_LL_CLOCK_CONFIG_LLH_IDLE_MASK)
            {                  
                distanceToNextInstant = PWR_BLE_GetNearestInstant(&nextInstant);
                offsetBeforeDsmExit = PWRLib_BLL_GetOffsetSlotsBeforeDsmExit();
                
                if(distanceToNextInstant > offsetBeforeDsmExit + 1)
                {
                    if(VECTOR_PENDING == 0)
                    {
                        PWRLib_BLL_SetWakeupInstant(nextInstant);
                        PWRLib_BLL_EnterDSM();
                        dsmExit = TRUE;                    
                    }
                    else
                    {
                        enterLowPower = FALSE;
                    }
                    
                }  
                else
                {
                    enterLowPower = FALSE;
                }
            }
            else
            {
                enterLowPower = FALSE;
            }
        }
        else
        {
            enterLowPower = (VECTOR_PENDING == 0);
        }
        
        if(enterLowPower)
        {
            uint32_t rfOscEn;
            
            if(gpfPWR_LowPowerEnterCb != NULL)
            {
                gpfPWR_LowPowerEnterCb();
            }
            
            BOARD_BLPEtoBLPI();
            rfOscEn =  RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
            RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0);        
            PWRLib_MCU_Enter_LLS3();
            RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);    
            BOARD_BLPItoBLPE();
            
            if(gpfPWR_LowPowerExitCb != NULL)
            {
                gpfPWR_LowPowerExitCb();
            }
            
            PWRLib_LLWU_UpdateWakeupReason();
        }
        
        PWRLib_LPTMR_ClockStop();
        
        /* Sync. the low power timers */
#if (gTMR_EnableLowPowerTimers_d)
        {
            uint64_t timerTicks = PWRLib_LPTMR_ClockCheck();
            mLpmTotalSleepDuration += timerTicks * 1000000 / lptmrFreq;
            timerTicks = (timerTicks * TMR_GetTimerFreq()) / lptmrFreq;
            timerTicks += notCountedTicksBeforeSleep;
            TMR_SyncLpmTimers((uint32_t)timerTicks);
        }
#endif /* (gTMR_EnableLowPowerTimers_d) */    
        if(dsmExit)
        {
            if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_INT) == 0)
            {
                RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT, 1);
            }
        }    
        
        SysTick->CTRL |= sysTickCtrl;
    }

#else
    PWRLib_MCU_WakeupReason.AllBits = 0;  
#endif /* cPWR_BLE_LL_Enable */
}
#endif /* (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_1) */

/*---------------------------------------------------------------------------
* Name: PWR_HandleDeepSleepMode_2_3
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode && (cPWR_EnableDeepSleepMode_2 || cPWR_EnableDeepSleepMode_3))
static void PWR_HandleDeepSleepMode_2_3(void)
{
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep = 0;
#endif  
    uint8_t clkMode;
    uint32_t lptmrTicks;
    uint32_t lptmrFreq;
    uint32_t sleepTimeMs;
#if cPWR_BLE_LL_Enable  
    uint16_t bleEnabledInt;
#endif  
    
    PWRLib_MCU_WakeupReason.AllBits = 0; 
    
#if cPWR_BLE_LL_Enable   
    if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT) == 0)  /* BLL in DSM */
    {
        return;
    }
#endif
    
#if cPWR_BLE_LL_Enable 
    /* At this point the BLE_LL should be in idle state. BLE stack should assure that 
    before allowing device to sleep */
    bleEnabledInt = PWRLib_BLL_GetIntEn();
    PWRLib_BLL_ClearInterrupts(bleEnabledInt);    
    PWRLib_BLL_DisableInterrupts(bleEnabledInt);
#endif  
    
    if(gpfPWR_LowPowerEnterCb != NULL)
    {
        gpfPWR_LowPowerEnterCb();
    }
    
#if (gTMR_EnableLowPowerTimers_d) && (cPWR_CheckLowPowerTimers)
    /* Get the expire time of the first programmed Low Power Timer */
    sleepTimeMs = TMR_GetFirstExpireTime(gTmrLowPowerTimer_c);
    
    if(mPWR_DeepSleepTimeMs < sleepTimeMs)
#endif
    {
        sleepTimeMs = mPWR_DeepSleepTimeMs;
    }
    
    PWRLib_LPTMR_GetTimeSettings(sleepTimeMs ,&clkMode ,&lptmrTicks, &lptmrFreq);
    PWRLib_LPTMR_ClockStart(clkMode,lptmrTicks);
    
#if (gTMR_EnableLowPowerTimers_d)
    /* if more low power timers are running, stop the hardware timer
    and save the spend time in ticks that wasn't counted.  */
    notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif
    
    {
        uint32_t rfOscEn;
        uint8_t lpMode;
        
        lpMode = PWRLib_GetDeepSleepMode();
        
        BOARD_BLPEtoBLPI();
        
        rfOscEn =  RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0);
        
        if(lpMode == 2)
        {
            PWRLib_MCU_Enter_LLS2();          
        }
        else
        {
            PWRLib_MCU_Enter_LLS3();          
        }
        
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);    
        BOARD_BLPItoBLPE();
    }
    
    if(gpfPWR_LowPowerExitCb != NULL)
    {
        gpfPWR_LowPowerExitCb();
    }
    
    PWRLib_LLWU_UpdateWakeupReason();
#if cPWR_BLE_LL_Enable        
    PWRLib_BLL_EnableInterrupts(bleEnabledInt); 
#endif       
    PWRLib_LPTMR_ClockStop();
    
    /* Sync. the low power timers */
#if (gTMR_EnableLowPowerTimers_d)
    {
        uint64_t timerTicks = PWRLib_LPTMR_ClockCheck();
        mLpmTotalSleepDuration += timerTicks * 1000000 / lptmrFreq;
        timerTicks = (timerTicks * TMR_GetTimerFreq()) / lptmrFreq;
        timerTicks += notCountedTicksBeforeSleep;
        TMR_SyncLpmTimers((uint32_t)timerTicks);
    }
#endif     
}
#endif

/*---------------------------------------------------------------------------
* Name: PWR_HandleDeepSleepMode_4
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_4)
static void PWR_HandleDeepSleepMode_4(void)
{
#if cPWR_BLE_LL_Enable
    uint16_t bleEnabledInt;
#endif  
    PWRLib_MCU_WakeupReason.AllBits = 0;  
    
#if cPWR_BLE_LL_Enable      
    if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT) == 0)  /* BLL in DSM */
    {
        return;
    }
    
    bleEnabledInt = PWRLib_BLL_GetIntEn();
    PWRLib_BLL_ClearInterrupts(bleEnabledInt);    
    PWRLib_BLL_DisableInterrupts(bleEnabledInt);
#endif  
    
    if(gpfPWR_LowPowerEnterCb != NULL)
    {
        gpfPWR_LowPowerEnterCb();
    }
    
    {
        uint32_t rfOscEn;
        
        BOARD_BLPEtoBLPI();
        
        rfOscEn =  RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0);
        
#if cPWR_DCDC_InBypass  
        PWRLib_MCU_Enter_VLLS0();
#else
        PWRLib_MCU_Enter_VLLS1();
#endif
        
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);    
        BOARD_BLPItoBLPE();
    }
    
    if(gpfPWR_LowPowerExitCb != NULL)
    {
        gpfPWR_LowPowerExitCb();
    }
    
#if cPWR_BLE_LL_Enable  
    PWRLib_BLL_EnableInterrupts(bleEnabledInt);        
#endif  
}
#endif

/*---------------------------------------------------------------------------
* Name: PWR_HandleDeepSleepMode_5
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_5)
static void PWR_HandleDeepSleepMode_5(void)
{
#if cPWR_BLE_LL_Enable
    uint16_t bleEnabledInt;
#endif
    PWRLib_MCU_WakeupReason.AllBits = 0;
    
#if cPWR_BLE_LL_Enable  
    if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT) == 0)  /* BLL in DSM */
    {
        return;
    }
    
    bleEnabledInt = PWRLib_BLL_GetIntEn();
    PWRLib_BLL_ClearInterrupts(bleEnabledInt);    
    PWRLib_BLL_DisableInterrupts(bleEnabledInt);
#endif  
    
    if(gpfPWR_LowPowerEnterCb != NULL)
    {
        gpfPWR_LowPowerEnterCb();
    }
    
    {
        uint32_t rfOscEn;
        
        BOARD_BLPEtoBLPI();
        
        rfOscEn =  RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0);
        
        PWRLib_MCU_Enter_VLLS2();  
        
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);    
        
        BOARD_BLPItoBLPE();
    }    
    
    if(gpfPWR_LowPowerExitCb != NULL)
    {
        gpfPWR_LowPowerExitCb();
    }
    
#if cPWR_BLE_LL_Enable  
    PWRLib_BLL_EnableInterrupts(bleEnabledInt);        
#endif  
}
#endif

/*---------------------------------------------------------------------------
* Name: PWR_HandleDeepSleepMode_6
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_6)
static void PWR_HandleDeepSleepMode_6(void)
{
    
#if (gTMR_EnableLowPowerTimers_d)   
    uint32_t notCountedTicksBeforeSleep = 0;
#endif  
    uint8_t clkMode;
    uint32_t lptmrTicks;
    uint32_t lptmrFreq;
    uint32_t sysTickCtrl;
    uint32_t sleepTimeMs;
    
    PWRLib_MCU_WakeupReason.AllBits = 0;  
    
#if cPWR_BLE_LL_Enable   
    if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT) == 0)  /* BLL in DSM */
    {
        return;
    }
#endif  
    
#if (gTMR_EnableLowPowerTimers_d) && (cPWR_CheckLowPowerTimers)
    /* Get the expire time of the first programmed Low Power Timer */
    sleepTimeMs = TMR_GetFirstExpireTime(gTmrLowPowerTimer_c);
    
    if(mPWR_DeepSleepTimeMs < sleepTimeMs)
#endif
    {
        sleepTimeMs = mPWR_DeepSleepTimeMs;
    }
    
    PWRLib_LPTMR_GetTimeSettings(sleepTimeMs ,&clkMode ,&lptmrTicks, &lptmrFreq);
    PWRLib_LPTMR_ClockStart(clkMode,lptmrTicks);
    
#if (gTMR_EnableLowPowerTimers_d)
    /* If more low power timers are running, stop the hardware timer
    * and save the spend time in ticks that wasn't counted.  
    */
    notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif
    
    /* Disable SysTick counter and interrupt */
    sysTickCtrl = SysTick->CTRL & (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    
    {
        uint32_t rfOscEn = RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
        
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0x3);
        
        BOARD_BLPEtoBLPI();
        
        RSIM->CONTROL |= RSIM_CONTROL_RSIM_STOP_ACK_OVRD_EN_MASK | RSIM_CONTROL_RSIM_STOP_ACK_OVRD_MASK;
        SIM->FCFG1 |= SIM_FCFG1_FLASHDOZE_MASK;
        
        PWRLib_MCU_Enter_Stop();
        SIM->FCFG1 &= ~SIM_FCFG1_FLASHDOZE_MASK;  
        RSIM->CONTROL &= ~RSIM_CONTROL_RSIM_STOP_ACK_OVRD_EN_MASK; 
        BOARD_BLPItoBLPE();
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);
    }
    
    /* Restore the state of SysTick */
    SysTick->CTRL |= sysTickCtrl;
    /* Checks sources of wakeup */
    PWRLib_StopUpdateWakeupReason();
    PWRLib_LPTMR_ClockStop();
    
    /* Sync. the low power timers */
#if (gTMR_EnableLowPowerTimers_d)
    {
        uint64_t timerTicks = PWRLib_LPTMR_ClockCheck();
        mLpmTotalSleepDuration += timerTicks * 1000000 / lptmrFreq;
        timerTicks = (timerTicks * TMR_GetTimerFreq()) / lptmrFreq;
        timerTicks += notCountedTicksBeforeSleep;
        TMR_SyncLpmTimers((uint32_t)timerTicks);
    }
#endif
}
#endif

/*---------------------------------------------------------------------------
* Name: PWR_HandleDeepSleepMode_7
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_7)
static void PWR_HandleDeepSleepMode_7(void)
{
#if (cPWR_GENFSK_LL_Enable)
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep= 0;
#endif
    uint8_t clkMode;
    uint32_t lptmrTicks;
    uint32_t lptmrFreq;
    uint32_t sleepTimeMs;
    bool_t enterLowPower = TRUE;
    
    PWRLib_MCU_WakeupReason.AllBits = 0;
    
    if(enterLowPower && (READ_REGISTER_FIELD(RSIM->DSM_CONTROL,RSIM_DSM_CONTROL_GEN_SLEEP_REQUEST) == 0))
    {
        enterLowPower &= PWR_HandleGenfskDsmEnter();
    }
    
    if(enterLowPower)
    {
        uint32_t rfOscEn;
        
        if(gpfPWR_LowPowerEnterCb != NULL)
        {
            gpfPWR_LowPowerEnterCb();
        }
        
#if (gTMR_EnableLowPowerTimers_d) && (cPWR_CheckLowPowerTimers)
        /* Get the expire time of the first programmed Low Power Timer */
        sleepTimeMs = TMR_GetFirstExpireTime(gTmrLowPowerTimer_c);
        
        if(mPWR_DeepSleepTimeMs < sleepTimeMs)
#endif /* (gTMR_EnableLowPowerTimers_d) && (cPWR_CheckLowPowerTimers) */
        {
            sleepTimeMs = mPWR_DeepSleepTimeMs;
        }
        
        PWRLib_LPTMR_GetTimeSettings(sleepTimeMs ,&clkMode ,&lptmrTicks, &lptmrFreq);
        PWRLib_LPTMR_ClockStart(clkMode,lptmrTicks);
        
#if (gTMR_EnableLowPowerTimers_d)
        /* If more low power timers are running, stop the hardware timer
        * and save the spend time in ticks that wasn't counted.
        */
        notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif
        
        BOARD_BLPEtoBLPI();
        
        rfOscEn =  RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0);        
        
        PWRLib_MCU_Enter_LLS3();
        
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);    
        
        BOARD_BLPItoBLPE();
        
        if(gpfPWR_LowPowerExitCb != NULL)
        {
            gpfPWR_LowPowerExitCb();
        }
        
        PWRLib_LLWU_UpdateWakeupReason();
        
        PWR_HandleGenfskDsmExit();
        
        PWRLib_LPTMR_ClockStop();
        
        /* Sync. the low power timers */
#if (gTMR_EnableLowPowerTimers_d)
        {
            uint64_t timerTicks = PWRLib_LPTMR_ClockCheck();
            mLpmTotalSleepDuration += timerTicks * 1000000 / lptmrFreq;
            timerTicks = (timerTicks * TMR_GetTimerFreq()) / lptmrFreq;
            timerTicks += notCountedTicksBeforeSleep;
            TMR_SyncLpmTimers((uint32_t)timerTicks);
        }
#endif
    }
#else
    PWRLib_MCU_WakeupReason.AllBits = 0;
#endif
}
#endif

/*---------------------------------------------------------------------------
* Name: PWR_HandleDeepSleep
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode)
static PWRLib_WakeupReason_t PWR_HandleDeepSleep(void)
{
    PWRLib_MCU_WakeupReason.AllBits = 0;
    {
        uint8_t lpMode;
        
        lpMode = PWRLib_GetDeepSleepMode();
        
        if(lpMode)
        {
            if(maHandleDeepSleep[lpMode - 1])
            {
                maHandleDeepSleep[lpMode - 1]();
            }
        }
    }
    
    return PWRLib_MCU_WakeupReason;
}
#endif

/*---------------------------------------------------------------------------
* Name: PWR_HandleSleep
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode)
static PWRLib_WakeupReason_t PWR_HandleSleep(void)
{
    PWRLib_WakeupReason_t  Res;
    
    Res.AllBits = 0;
    
    /*---------------------------------------------------------------------------*/
#if (cPWR_SleepMode == 0)
    return Res;
    
    /*---------------------------------------------------------------------------*/
#elif (cPWR_SleepMode == 1)
    /* Radio in autodoze mode by default. MCU in wait mode */
    PWRLib_MCU_WakeupReason.AllBits = 0;
    PWRLib_MCU_Enter_Sleep();
    Res.Bits.SleepTimeout = 1;
    PWRLib_MCU_WakeupReason.Bits.SleepTimeout = 1;
    
    return Res;
    /*---------------------------------------------------------------------------*/
#else
#error "*** ERROR: Not a valid cPWR_SleepMode chosen"
#endif
}
#endif /* (cPWR_UsePowerDownMode) */

/*---------------------------------------------------------------------------
* Name: PWR_SleepAllowed
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode)
static bool_t PWR_SleepAllowed(void)
{
    bool_t state = TRUE;
    
#if cPWR_Zigbee_Enable
    if((PWRLib_GetCurrentZigbeeStackPowerState != StackPS_Sleep) &&  \
        (PWRLib_GetCurrentZigbeeStackPowerState != StackPS_DeepSleep))
    {
        state = FALSE;
    }
#endif
    return state;
}
#endif /* (cPWR_UsePowerDownMode) */

/*---------------------------------------------------------------------------
* Name: PWR_DeepSleepAllowed
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode)
static bool_t PWR_DeepSleepAllowed(void)
{
    bool_t state = TRUE;
    
    if((PWRLib_GetDeepSleepMode() != 6) && mLpmXcvrDisallowCnt)
    {
        state = FALSE;
    }
    
#if cPWR_Zigbee_Enable  
    if(state)
    {
        if (PWRLib_GetCurrentZigbeeStackPowerState != StackPS_DeepSleep)
        {
            state = FALSE;
        }
    }
    
#endif      
    return state;
}
#endif /* #if (cPWR_UsePowerDownMode)*/

/*---------------------------------------------------------------------------
* Name: PWR_CheckForAndEnterNewPowerState
* Description: - 
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
static PWRLib_WakeupReason_t PWR_CheckForAndEnterNewPowerState(PWR_CheckForAndEnterNewPowerState_t NewPowerState)
{
    PWRLib_WakeupReason_t ReturnValue;
    
    ReturnValue.AllBits = 0;
    
#if (cPWR_UsePowerDownMode)
    if (NewPowerState == PWR_Run)
    {
        /* ReturnValue = 0; */
    }
    else if(NewPowerState == PWR_OFF)
    {
        /* Disable all wake up sources */
#if cPWR_EnableDeepSleepMode_4    
        void PWRLib_LLWU_DisableAllWakeupSources();
        /* Configure MCU in VLLS0 mode */
        PWR_HandleDeepSleepMode_4();
#endif    
        /* Never returns */
        for(;;){}        
    }
    else if(NewPowerState == PWR_Reset)
    {
        /* Never returns */
        PWR_SystemReset();
    }
    
    else if(NewPowerState == PWR_DeepSleep)
    {
        if(PWR_CheckIfDeviceCanGoToSleep() && PWR_DeepSleepAllowed())
        {
            ReturnValue = PWR_HandleDeepSleep();
        }
    } 
    else if(NewPowerState == PWR_Sleep)
    {
        if(PWR_CheckIfDeviceCanGoToSleep() && PWR_SleepAllowed())
        {
            ReturnValue = PWR_HandleSleep();
        }
    }
    else
    {
        /* ReturnValue = FALSE; */
    }
    /* Clear wakeup reason */    
#else
    /* To remove warning for variabels in function call */
    (void)NewPowerState;
#endif /* #if (cPWR_UsePowerDownMode) */
    
    return ReturnValue;
}

/*****************************************************************************
*                             PUBLIC FUNCTIONS                              *
*---------------------------------------------------------------------------*
* Add to this section all the functions that have global (project) scope.   *
* These functions can be accessed outside this module.                      *
* These functions shall have their declarations (prototypes) within the     *
* interface header file and shall be preceded by the 'extern' keyword.      *
*---------------------------------------------------------------------------*
*****************************************************************************/

/*---------------------------------------------------------------------------
* Name: PWR_Init
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void PWR_Init(void)
{
#if (cPWR_UsePowerDownMode)
    PWRLib_Init();
#endif /* (cPWR_UsePowerDownMode) */
}

/*---------------------------------------------------------------------------
* Name: PWR_GetSystemResetStatus
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
uint16_t PWR_GetSystemResetStatus(void)
{
    uint16_t resetStatus = 0;
    
    resetStatus = (uint16_t)(RCM->SRS0);
    resetStatus |= (uint16_t)(RCM->SRS1 << 8);
    
    return resetStatus;
}

/*---------------------------------------------------------------------------
* Name: PWR_SystemReset
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void PWR_SystemReset(void)
{
    SCB->AIRCR = ((uint32_t)0x5FA << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk;
    while(1);
}

/*---------------------------------------------------------------------------
* Name: PWR_SetDeepSleepTimeInMs
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void PWR_SetDeepSleepTimeInMs(uint32_t deepSleepTimeMs)
{
#if (cPWR_UsePowerDownMode)
    if(deepSleepTimeMs == 0) 
    {
        return;
    }
    
    mPWR_DeepSleepTimeMs = deepSleepTimeMs; 
#else
    (void) deepSleepTimeMs;
#endif /* (cPWR_UsePowerDownMode) */
}

/*---------------------------------------------------------------------------
* Name: PWR_SetDeepSleepTimeInSymbols
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void PWR_SetDeepSleepTimeInSymbols(uint32_t deepSleepTimeSymbols)
{
#if (cPWR_UsePowerDownMode)
    if(deepSleepTimeSymbols == 0) 
    {
        return;
    }
    mPWR_DeepSleepTimeMs = (deepSleepTimeSymbols * 16)/1000; 
#else
    (void) deepSleepTimeSymbols;
#endif /* (cPWR_UsePowerDownMode) */
}

/*---------------------------------------------------------------------------
* Name: PWR_AllowDeviceToSleep
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void PWR_AllowDeviceToSleep(void)
{
#if (cPWR_UsePowerDownMode)
    OSA_InterruptDisable();
    
    if(mLPMFlag != 0)
    {
        mLPMFlag--;
    }
    
    OSA_InterruptEnable();
#endif /* (cPWR_UsePowerDownMode) */
}

/*---------------------------------------------------------------------------
* Name: PWR_DisallowDeviceToSleep
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void PWR_DisallowDeviceToSleep(void)
{
#if (cPWR_UsePowerDownMode)
    uint8_t prot;
    
    OSA_InterruptDisable();
    
    prot = mLPMFlag + 1;
    
    if(prot != 0)
    {
        mLPMFlag++;
    }
    
    OSA_InterruptEnable();
#endif /* (cPWR_UsePowerDownMode) */
}

/*---------------------------------------------------------------------------
* Name: PWR_AllowXcvrToSleep
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void PWR_AllowXcvrToSleep(void)
{
#if (cPWR_UsePowerDownMode)
    OSA_InterruptDisable();
    
    if(mLpmXcvrDisallowCnt != 0)
    {
        mLpmXcvrDisallowCnt--;
    }
    
    OSA_InterruptEnable();
#endif /* (cPWR_UsePowerDownMode) */
}

/*---------------------------------------------------------------------------
* Name: PWR_DisallowXcvrToSleep
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void PWR_DisallowXcvrToSleep(void)
{
#if (cPWR_UsePowerDownMode)
    uint8_t prot;
    
    OSA_InterruptDisable();
    
    prot = mLpmXcvrDisallowCnt + 1;
    
    if(prot != 0)
    {
        mLpmXcvrDisallowCnt++;
    }
    
    OSA_InterruptEnable();
#endif /* (cPWR_UsePowerDownMode) */
}

/*---------------------------------------------------------------------------
* Name: PWR_CheckIfDeviceCanGoToSleep
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
bool_t PWR_CheckIfDeviceCanGoToSleep(void)
{
    bool_t returnValue;
    
    OSA_InterruptDisable();
    returnValue = mLPMFlag == 0 ? TRUE : FALSE;
    OSA_InterruptEnable();
    
    return returnValue;
}

/*---------------------------------------------------------------------------
* Name: PWR_ChangeDeepSleepMode
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
bool_t PWR_ChangeDeepSleepMode(uint8_t dsMode)
{
#if (cPWR_UsePowerDownMode)
    if(dsMode > 7)
    {
        return FALSE;
    }
    
    if(dsMode)
    {
        if(maHandleDeepSleep[dsMode - 1] == (pfHandleDeepSleepFunc_t)0)
        {
            return FALSE;
        } 
    }
    
    PWRLib_SetDeepSleepMode(dsMode);
    PWRLib_ConfigLLWU(dsMode);
    
#if (cPWR_BLE_LL_Enable)  
    PWRLib_BLL_ConfigDSM(dsMode);
#endif   /* (cPWR_BLE_LL_Enable) */
    
    PWRLib_ConfigRSIM(dsMode); 
    
    return TRUE;
#else
    return TRUE;
#endif /* (cPWR_UsePowerDownMode) */
} 

/*---------------------------------------------------------------------------
* Name: PWR_GetDeepSleepMode
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
uint8_t PWR_GetDeepSleepMode(void)
{
#if (cPWR_UsePowerDownMode)
    return PWRLib_GetDeepSleepMode();
#else
    return 0;
#endif /* (cPWR_UsePowerDownMode) */
}

/*---------------------------------------------------------------------------
* Name: PWR_EnterPowerOff
* Description: - Radio on Reset, MCU on VLLS1
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void PWR_EnterPowerOff(void)
{
    OSA_InterruptDisable();
    (void)PWR_CheckForAndEnterNewPowerState(PWR_OFF);
    OSA_InterruptEnable();
}

/*---------------------------------------------------------------------------
* Name: PWR_BLE_EnterDSM
* Description: - 
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void PWR_BLE_EnterDSM(uint16_t wakeupInstant)
{
#if cPWR_BLE_LL_Enable
    uint8_t dsMode;
    
    dsMode = PWRLib_GetDeepSleepMode();
    
    if((dsMode == 1) || (dsMode == 2))
    {
        if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT))
        {
            RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT, 0);
            PWRLib_BLL_SetWakeupInstant(wakeupInstant);
            PWRLib_BLL_EnterDSM();
        }
    }
#endif /* cPWR_BLE_LL_Enable */
}
/*---------------------------------------------------------------------------
* Name: PWR_BLE_GetReferenceClock
* Description: - 
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
uint16_t PWR_BLE_GetReferenceClock(void)
{
#if cPWR_BLE_LL_Enable
    return PWRLib_BLL_GetInstantTimer();
#else
    return 0;  
#endif /* cPWR_BLE_LL_Enable */
}

/*---------------------------------------------------------------------------
* Name: PWR_BLE_ExitDSM
* Description: - 
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void PWR_BLE_ExitDSM(void)
{
#if cPWR_BLE_LL_Enable
    if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT) == 0)
    {
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT, 1);
    }
#endif /* cPWR_BLE_LL_Enable */
}

/*---------------------------------------------------------------------------
* Name: PWR_LVD_ReportLevel
* Description: - 
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
PWRLib_LVD_VoltageLevel_t PWR_LVD_ReportLevel(void)
{
    PWRLib_LVD_VoltageLevel_t Level;
    
#if ((cPWR_LVD_Enable == 0) || (cPWR_LVD_Enable == 3))
    Level = PWR_ABOVE_LEVEL_3_0V;
#elif (cPWR_LVD_Enable == 1)
    Level = PWRLib_LVD_CollectLevel();
#elif (cPWR_LVD_Enable == 2)
    Level = PWRLib_LVD_SavedLevel;
#else
#error "*** ERROR: Illegal value for cPWR_LVD_Enable"
#endif /* (cPWR_LVD_Enable) */
    return Level;
}

/*---------------------------------------------------------------------------
* Name: PWR_EnterLowPower
* Description: - 
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
PWRLib_WakeupReason_t PWR_EnterLowPower(void)
{
    PWRLib_WakeupReason_t ReturnValue;
    
#if (gTMR_EnableLowPowerTimers_d) 
    bool_t unlockTMRThread = FALSE;
#endif
    
    ReturnValue.AllBits = 0;
    
    if(PWR_LVD_ReportLevel() == PWR_LEVEL_CRITICAL)
    {
        /* Voltage <= 1.8V so enter power-off state - to disable false Tx'ing(void)*/
        ReturnValue = PWR_CheckForAndEnterNewPowerState(PWR_OFF);
    }
    
    OSA_InterruptDisable();
    
    if (TMR_AreAllTimersOff())  /*No timer running*/
    {
        /* If power lib is enabled */	
#if (cPWR_UsePowerDownMode)
        /* If Low Power Capability is enabled */
#if (gTMR_EnableLowPowerTimers_d) 
        /* If more low power timers are running, stop the hardware timer
        *and save the spend time in ticks that wasn't counted.
        */
        unlockTMRThread = TRUE;
#endif /* (gTMR_EnableLowPowerTimers_d) */
#endif /* (cPWR_UsePowerDownMode) */
        
        ReturnValue = PWR_CheckForAndEnterNewPowerState(PWR_DeepSleep);
    }
    else /* Timers are running */
    {
        ReturnValue = PWR_CheckForAndEnterNewPowerState(PWR_Sleep);
    }
    
    OSA_InterruptEnable();
    
#if (gTMR_EnableLowPowerTimers_d)
    if(unlockTMRThread)
    {
        TMR_MakeTMRThreadReady();
    }
    
#endif    
    return ReturnValue;
}

/*---------------------------------------------------------------------------
* Name: PWR_RegisterLowPowerEnterCallback
* Description: - 
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void PWR_RegisterLowPowerEnterCallback(pfPWRCallBack_t lowPowerEnterCallback)
{
#if (cPWR_UsePowerDownMode)
    gpfPWR_LowPowerEnterCb = lowPowerEnterCallback;
#endif /* (cPWR_UsePowerDownMode) */
}

/*---------------------------------------------------------------------------
* Name: PWR_RegisterLowPowerExitCallback
* Description: - 
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void PWR_RegisterLowPowerExitCallback(pfPWRCallBack_t lowPowerExitCallback)
{
#if (cPWR_UsePowerDownMode)
    gpfPWR_LowPowerExitCb = lowPowerExitCallback;
#endif /* (cPWR_UsePowerDownMode) */
}

/*---------------------------------------------------------------------------
* Name: PWR_EnterSleep
* Description: - 
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void PWR_EnterSleep(void)
{
    PWRLib_MCU_Enter_Sleep();
}

/*---------------------------------------------------------------------------
* Name: PWR_GetTotalSleepDurationMS
* Description: - 
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
uint32_t PWR_GetTotalSleepDurationMS(void)
{
    uint32_t time;
    
    OSA_InterruptDisable();
    time = mLpmTotalSleepDuration / 1000;
    OSA_InterruptEnable();
    
    return time;
}

/*---------------------------------------------------------------------------
* Name: PWR_ResetTotalSleepDuration
* Description: - 
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void PWR_ResetTotalSleepDuration(void)
{
    OSA_InterruptDisable();
    mLpmTotalSleepDuration = 0;
    OSA_InterruptEnable();
}
