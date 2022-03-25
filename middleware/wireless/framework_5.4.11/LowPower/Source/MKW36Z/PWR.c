/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017, 2022 NXP
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
#define mPWR_InvalidDSMode_c    (0xff)
#if(cPWR_WaitFor32kOscStart && (cPWR_EnableDeepSleepMode_1 || cPWR_EnableDeepSleepMode_8) && cPWR_BLE_LL_Enable)
#define mPWR_WaitFor32kOscStart_c (1)
#else
#define mPWR_WaitFor32kOscStart_c (0)
#endif
#if (defined(__ICCARM__))
#define AT_WARMBOOTCONFIG_SECTION(var) var @"WarmbootConfig"
#elif(defined(__CC_ARM))
#define AT_WARMBOOTCONFIG_SECTION(var) __attribute__((section("WarmbootConfig"), zero_init)) var
#elif(defined(__GNUC__))
#define AT_WARMBOOTCONFIG_SECTION(var) __attribute__((section("WarmbootConfig"))) var
#else
#error Toolchain not supported.
#endif

#define mPWR_WaitForLlDSMandRfOscOffInLoopCounts_c     ((BOARD_BOOTCLOCKRUN_CORE_CLOCK * cPWR_WaitForLlDSMandRfOscOffInUs)/(1000000U* 5U)) 
#define mPWR_WaitForRfOscOffInLoopCounts_c             ((BOARD_BOOTCLOCKRUN_CORE_CLOCK * cPWR_WaitForRfOscOffInUs)/(1000000U* 5U))     
#define mPWR_WaitForLlDSMandRfOscOffIn32kTicks_c       ((BOARD_XTAL32K_CLK_HZ *cPWR_WaitForLlDSMandRfOscOffInUs)/1000000U + 1U)
#define mPWR_WaitForRfOscOffIn32kTicks_c               ((BOARD_XTAL32K_CLK_HZ * cPWR_WaitForRfOscOffInUs)/1000000U + 1U)

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
#if cPWR_EnableDeepSleepMode_8
static void PWR_HandleDeepSleepMode_8(void);
#endif

#if cPWR_BLE_LL_Enable
static void PWR_CheckBLE_LL_state(bool_t * enterLowPower, bool_t * dsmExit);
#endif

#if (cPWR_UsePowerDownMode) && (gTMR_EnableLowPowerTimers_d)
#if cPWR_EnableTimerAccuracyOptimization
static bool_t PWR_SyncTimersTmrAccuracy(uint32_t notCountedTicksBeforeSleep, uint32_t lptmrFreq);
#else  /* cPWR_EnableTimerAccuracyOptimization */
static void PWR_SyncTimers(uint32_t notCountedTicksBeforeSleep, uint32_t lptmrFreq);
#endif /* cPWR_EnableTimerAccuracyOptimization */
#endif /*(cPWR_UsePowerDownMode) && (gTMR_EnableLowPowerTimers_d)*/

#if cPWR_EnableTimerAccuracyOptimization
static uint32_t PWR_ProgramDeepSleepTimeTmrAccuracy(uint32_t adjustTicks, uint8_t* pClkMode, uint32_t* pLptmrTicks, bool_t* pEnterLowPower );
#else  /* cPWR_EnableTimerAccuracyOptimization */
static uint32_t PWR_ProgramDeepSleepTime(void);
#endif /* cPWR_EnableTimerAccuracyOptimization */

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

typedef struct _warmboot_config
{
    uint32_t doWarmbootSequence;
    uint32_t useWarmbootSp;
} warmboot_config_t;

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
#if (gTMR_EnableLowPowerTimers_d)
static bool_t mUnlockTMRThread = FALSE;
#endif
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
    PWR_HandleDeepSleepMode_7,
#else
    (pfHandleDeepSleepFunc_t)0,
#endif
#if cPWR_EnableDeepSleepMode_8
    PWR_HandleDeepSleepMode_8,
#else
    (pfHandleDeepSleepFunc_t)0,
#endif
};

static pfPWRCallBack_t gpfPWR_LowPowerEnterCb;
static pfPWRCallBack_t gpfPWR_LowPowerExitCb;

#if (mPWR_WaitFor32kOscStart_c && cPWR_32kOscStabilizationDelayInMs)
static uint8_t mPendingDSMode = mPWR_InvalidDSMode_c;
#endif
AT_WARMBOOTCONFIG_SECTION(warmboot_config_t warmbootConfig);
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
    durationInDsmTicks = (dsmDuration << 15U)/1000U;
    if(durationInDsmTicks > mPWR_DSM_TimerMask_c)
    {
        durationInDsmTicks = mPWR_DSM_TimerMask_c;
    }
    dsmOscOffset = 1024U << READ_REGISTER_FIELD(RSIM->RF_OSC_CTRL,RSIM_RF_OSC_CTRL_BB_XTAL_READY_COUNT_SEL);
    dsmOscOffset = (dsmOscOffset << 15U)/BOARD_BOOTCLOCKRUN_CORE_CLOCK;
    if(durationInDsmTicks > dsmOscOffset)
    {
        uint32_t dsmSleep;
        RSIM->DSM_OSC_OFFSET = dsmOscOffset;  /* Set the DSM oscillator offset. */
        OSA_InterruptDisable();
        dsmSleep = (RSIM->DSM_TIMER + 5U) & mPWR_DSM_TimerMask_c;  /* Compute the sleep time. No fewer than 4 clocks greater than DSM_TIMER. */
        RSIM->GEN_SLEEP = dsmSleep;
        mPWR_AbsoluteWakeupTimeInDsmTicks = (dsmSleep + durationInDsmTicks) & mPWR_DSM_TimerMask_c;  /* Compute wake-up time. */
        RSIM->GEN_WAKE = mPWR_AbsoluteWakeupTimeInDsmTicks;
        GENFSK->DSM_CTRL = GENFSK_DSM_CTRL_GEN_SLEEP_REQUEST_MASK;  /* Enter DSM */
        OSA_InterruptEnable();
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
    uint64_t durationInDsmTicks = 0;
    bool_t retVal = FALSE;
    /* Get the current DSM time. */
    currentDsmTime = RSIM->DSM_TIMER;  
    /* Compute the remaining sleep period. */
    if (mPWR_AbsoluteWakeupTimeInDsmTicks != mPWR_DSM_InvalidTime_c)
    {
        durationInDsmTicks = (mPWR_AbsoluteWakeupTimeInDsmTicks - currentDsmTime - 5U)&mPWR_DSM_TimerMask_c;
    }
    else
    {
        durationInDsmTicks = mPWR_DeepSleepTimeMs;
        durationInDsmTicks = (durationInDsmTicks << 15U)/1000U;
    }
    
    dsmOscOffset = 1024U << READ_REGISTER_FIELD(RSIM->RF_OSC_CTRL,RSIM_RF_OSC_CTRL_BB_XTAL_READY_COUNT_SEL);
    dsmOscOffset = (dsmOscOffset << 15U)/BOARD_BOOTCLOCKRUN_CORE_CLOCK;
    
    if(durationInDsmTicks > dsmOscOffset)
    {
        uint32_t dsmSleep;
        RSIM->DSM_OSC_OFFSET = dsmOscOffset;
        dsmSleep = (RSIM->DSM_TIMER + 5U) & mPWR_DSM_TimerMask_c;
        RSIM->GEN_SLEEP = dsmSleep;
        RSIM->GEN_WAKE = dsmSleep + durationInDsmTicks;
        GENFSK->DSM_CTRL = GENFSK_DSM_CTRL_GEN_SLEEP_REQUEST_MASK;
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
        uint32_t dsmOscOffset;
        uint32_t dsmTimer, dsmSleep, dsmWake, dsmDuration;
        dsmOscOffset = 1024U << READ_REGISTER_FIELD(RSIM->RF_OSC_CTRL,RSIM_RF_OSC_CTRL_BB_XTAL_READY_COUNT_SEL);
        dsmOscOffset = (dsmOscOffset << 15U)/BOARD_BOOTCLOCKRUN_CORE_CLOCK;
        dsmTimer = RSIM->DSM_TIMER;
        remainTicksToSleep = RSIM->GEN_WAKE;
        remainTicksToSleep = (remainTicksToSleep - dsmTimer) & mPWR_DSM_TimerMask_c;
        dsmWake = (dsmTimer + 5U) & mPWR_DSM_TimerMask_c;
        dsmSleep = RSIM->GEN_SLEEP;
        dsmDuration  = (dsmWake-dsmSleep)& mPWR_DSM_TimerMask_c;
        if(dsmDuration < dsmOscOffset + 1U)
        {
            dsmWake = (dsmWake + dsmOscOffset - dsmDuration + 1U)&mPWR_DSM_TimerMask_c;
        }
        if(remainTicksToSleep > ((dsmWake - dsmTimer)&mPWR_DSM_TimerMask_c) )
        {
            RSIM->GEN_WAKE = dsmWake;
            remainTicksToSleep = (dsmWake - dsmTimer)&mPWR_DSM_TimerMask_c;
        }
        else
        {
            dsmWake = RSIM->GEN_WAKE;
        }
        while(remainTicksToSleep)
        {
            dsmTimer = RSIM->DSM_TIMER;
            dsmDuration = (dsmWake - dsmTimer)&mPWR_DSM_TimerMask_c;
            if((dsmDuration == 0U) || (dsmDuration > remainTicksToSleep))
            {
                remainTicksToSleep = 0U;
            }
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
#if cPWR_EnableTimerAccuracyOptimization
static void PWR_HandleDeepSleepMode_1(void)
{
#if cPWR_BLE_LL_Enable
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep = 0;
#endif /* (gTMR_EnableLowPowerTimers_d) */
    uint32_t lptmrFreq;
    uint8_t clkMode;
    uint32_t lptmrTicks;
    uint32_t sysTickCtrl;
    bool_t enterLowPower = !RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT);
    bool_t dsmExit = FALSE;
    uint32_t lptmrLPEnter;
    uint32_t lptmrLPExit;    
    static uint32_t adjustLptmrMaxFreqTicks = 0;
#if mPWR_WaitFor32kOscStart_c    
    osc32KStatus_t osc32KStatus = PWRLib_Get32KHzOscStatus();
    if(osc32KStatus == gOsc32K_NotStarted_c)
    {
        osc32KStatus = PWRLib_Check32KHzOscStatus();
    }
    if(osc32KStatus != gOsc32K_Ready_c)
    {
        enterLowPower = FALSE;
    }
#endif   
    PWRLib_MCU_WakeupReason.AllBits = 0;
#if (gTMR_EnableLowPowerTimers_d)    
    mUnlockTMRThread = FALSE;
#endif    
    if(enterLowPower)
    {
        /* Disable SysTick counter and interrupt */
        sysTickCtrl = SysTick->CTRL & (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
        SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
        lptmrFreq = PWR_ProgramDeepSleepTimeTmrAccuracy(adjustLptmrMaxFreqTicks, &clkMode, &lptmrTicks, &enterLowPower );
        if(enterLowPower)
        {
            PWR_CheckBLE_LL_state(&enterLowPower, &dsmExit);
            if(enterLowPower)
            {
                uint32_t rfOscEn;
                bool_t lowpower_skipped;
                PWRLib_LPTMR_ClockStart(clkMode,lptmrTicks);
#if (gTMR_EnableLowPowerTimers_d)
                /* If more low power timers are running, stop the hardware timer
                * and save the spend time in ticks that wasn't counted.
                */
                notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif /* (gTMR_EnableLowPowerTimers_d) */          
                if(gpfPWR_LowPowerEnterCb != NULL)
                {
                    gpfPWR_LowPowerEnterCb();
                }
                BOARD_BLPEtoBLPI();
                rfOscEn =  RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
                RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0);
                lptmrLPEnter = PWRLib_LPTMR_ClockCheckLight();
                lowpower_skipped = PWRLib_MCU_Enter_LLS3();
                if(lowpower_skipped)
                {
                    lptmrLPExit = PWRLib_LPTMR_ClockCheck();
                    PWRLib_DelayLoopRemainingTicks(mPWR_WaitForLlDSMandRfOscOffIn32kTicks_c, lptmrLPExit - lptmrLPEnter, lptmrFreq);
                }
                RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);
                PWRLib_WaitFor32MhzReady();
                BOARD_BLPItoBLPE();
                if(gpfPWR_LowPowerExitCb != NULL)
                {
                    gpfPWR_LowPowerExitCb();
                }
                PWRLib_LLWU_UpdateWakeupReason();
#if (gTMR_EnableLowPowerTimers_d)
                /* Sync. the low power timers */
                mUnlockTMRThread = PWR_SyncTimersTmrAccuracy(notCountedTicksBeforeSleep, lptmrFreq);
#endif /* (gTMR_EnableLowPowerTimers_d) */     
            }
            if(dsmExit)
            {
                if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_INT) == 0)
                {
                    RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT, 1);
                }
                while((BLE_LL_REG(EVENT_STATUS) & BLE_LL_EVENT_STATUS_DSM_INT_MASK) == 0)
                {
                }
            }
        }
        
        SysTick->CTRL |= sysTickCtrl;
        if(enterLowPower )
        {
            PWRLib_LPTMR_ClockStop(); 
            if(PWRLib_MCU_WakeupReason.Bits.FromLPTMR == 1U)
            {
                uint32_t lptmrAdj =  PWRLib_LPTMR_ClockCheck();
                lptmrAdj = PWRLib_LPTMR_GetMaxFreqTicks(lptmrAdj - lptmrTicks, lptmrFreq);
                if(lptmrAdj > adjustLptmrMaxFreqTicks)
                {
                    adjustLptmrMaxFreqTicks = lptmrAdj;
                }
            }
        }
    }
#else
    PWRLib_MCU_WakeupReason.AllBits = 0;
#endif /* cPWR_BLE_LL_Enable */
}
#else /* cPWR_EnableTimerAccuracyOptimization */
static void PWR_HandleDeepSleepMode_1(void)
{
#if cPWR_BLE_LL_Enable
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep = 0;
#endif /* (gTMR_EnableLowPowerTimers_d) */
    uint32_t lptmrFreq;
    uint32_t sysTickCtrl;
    bool_t enterLowPower = !RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT);
    bool_t dsmExit = FALSE;
    uint32_t lptmrLPEnter;
    uint32_t lptmrLPExit;    
#if mPWR_WaitFor32kOscStart_c    
    osc32KStatus_t osc32KStatus = PWRLib_Get32KHzOscStatus();
    if(osc32KStatus == gOsc32K_NotStarted_c)
    {
        osc32KStatus = PWRLib_Check32KHzOscStatus();
    }
    if(osc32KStatus != gOsc32K_Ready_c)
    {
        enterLowPower = FALSE;
    }
#endif   
    if(enterLowPower)
    {
        /* Disable SysTick counter and interrupt */
        sysTickCtrl = SysTick->CTRL & (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
        SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
        lptmrFreq = PWR_ProgramDeepSleepTime();
        PWRLib_MCU_WakeupReason.AllBits = 0;
#if (gTMR_EnableLowPowerTimers_d)
        /* If more low power timers are running, stop the hardware timer
        * and save the spend time in ticks that wasn't counted.
        */
        notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif /* (gTMR_EnableLowPowerTimers_d) */
        PWR_CheckBLE_LL_state(&enterLowPower, &dsmExit);
        if(enterLowPower)
        {
            uint32_t rfOscEn;
            bool_t lowpower_skipped;
            if(gpfPWR_LowPowerEnterCb != NULL)
            {
                gpfPWR_LowPowerEnterCb();
            }
            BOARD_BLPEtoBLPI();
            rfOscEn =  RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
            RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0);
            lptmrLPEnter = PWRLib_LPTMR_ClockCheckLight();
            lowpower_skipped = PWRLib_MCU_Enter_LLS3();
            if(lowpower_skipped)
            {
                lptmrLPExit = PWRLib_LPTMR_ClockCheck();
                PWRLib_DelayLoopRemainingTicks(mPWR_WaitForLlDSMandRfOscOffIn32kTicks_c, lptmrLPExit - lptmrLPEnter, lptmrFreq);
            }
            RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);
            PWRLib_WaitFor32MhzReady();
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
        PWR_SyncTimers(notCountedTicksBeforeSleep, lptmrFreq);
#else 
        (void)lptmrFreq;
#endif /* (gTMR_EnableLowPowerTimers_d) */
        if(dsmExit)
        {
            if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_INT) == 0)
            {
                RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT, 1);
            }
            while((BLE_LL_REG(EVENT_STATUS) & BLE_LL_EVENT_STATUS_DSM_INT_MASK) == 0)
            {
            }
        }
        SysTick->CTRL |= sysTickCtrl;
    }
#else
    PWRLib_MCU_WakeupReason.AllBits = 0;
#endif /* cPWR_BLE_LL_Enable */
}
#endif /* cPWR_EnableTimerAccuracyOptimization */
#endif /* (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_1) */

/*---------------------------------------------------------------------------
 * Name: PWR_HandleDeepSleepMode_2_3
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode && (cPWR_EnableDeepSleepMode_2 || cPWR_EnableDeepSleepMode_3))
#if cPWR_EnableTimerAccuracyOptimization
static void PWR_HandleDeepSleepMode_2_3(void)
{
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep = 0;
#endif
    uint32_t lptmrFreq;
    uint8_t clkMode;
    uint32_t lptmrTicks;
    uint32_t lptmrLPEnter;
    uint32_t lptmrLPExit;
    static uint32_t adjustLptmrMaxFreqTicks = 0;
    bool_t enterLowPower;
#if cPWR_BLE_LL_Enable
    uint16_t bleEnabledInt;
#endif
#if PWR_EnableRtosSysticks       
    uint32_t sysTickCtrl;
#endif /* PWR_EnableRtosSysticks */
    PWRLib_MCU_WakeupReason.AllBits = 0;
#if (gTMR_EnableLowPowerTimers_d)    
    mUnlockTMRThread = FALSE;
#endif    
#if cPWR_BLE_LL_Enable
    if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT) == 0)  /* BLL in DSM */
    {
        return;
    }
#endif
#if PWR_EnableRtosSysticks      
    /* Disable SysTick counter and interrupt */
    sysTickCtrl = SysTick->CTRL & (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
#endif /* PWR_EnableRtosSysticks */
    lptmrFreq = PWR_ProgramDeepSleepTimeTmrAccuracy(adjustLptmrMaxFreqTicks, &clkMode, &lptmrTicks, &enterLowPower );
    if(enterLowPower)
    {
        enterLowPower = (VECTOR_PENDING == 0);
    }
    if(enterLowPower)
    {
        uint32_t rfOscEn;
        uint8_t lpMode;
        bool_t lowpower_skipped;
        PWRLib_LPTMR_ClockStart(clkMode,lptmrTicks);
#if (gTMR_EnableLowPowerTimers_d)
        /* if more low power timers are running, stop the hardware timer
        and save the spend time in ticks that wasn't counted.  */
        notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
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
        lpMode = PWRLib_GetDeepSleepMode();
        BOARD_BLPEtoBLPI();
        rfOscEn =  RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0);
        lptmrLPEnter = PWRLib_LPTMR_ClockCheckLight();
        if(lpMode == 2)
        {
            lowpower_skipped = PWRLib_MCU_Enter_LLS2();
        }
        else
        {
            lowpower_skipped = PWRLib_MCU_Enter_LLS3();
        }
        if(lowpower_skipped)
        {
            lptmrLPExit = PWRLib_LPTMR_ClockCheck();
            PWRLib_DelayLoopRemainingTicks(mPWR_WaitForRfOscOffIn32kTicks_c, lptmrLPExit - lptmrLPEnter, lptmrFreq);
        }
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);
        PWRLib_WaitFor32MhzReady();
        BOARD_BLPItoBLPE();
        if(gpfPWR_LowPowerExitCb != NULL)
        {
            gpfPWR_LowPowerExitCb();
        }
        PWRLib_LLWU_UpdateWakeupReason();
#if cPWR_BLE_LL_Enable
        PWRLib_BLL_EnableInterrupts(bleEnabledInt);
#endif         
        /* Sync. the low power timers */
#if (gTMR_EnableLowPowerTimers_d)
        mUnlockTMRThread = PWR_SyncTimersTmrAccuracy(notCountedTicksBeforeSleep, lptmrFreq);
#endif /* gTMR_EnableLowPowerTimers_d */
    }
#if PWR_EnableRtosSysticks       
    SysTick->CTRL |= sysTickCtrl;
#endif /* PWR_EnableRtosSysticks */ 
    if(enterLowPower )
    {
        PWRLib_LPTMR_ClockStop(); 
        if(PWRLib_MCU_WakeupReason.Bits.FromLPTMR == 1)
        {
            uint32_t lptmrAdj =  PWRLib_LPTMR_ClockCheck();
            lptmrAdj = PWRLib_LPTMR_GetMaxFreqTicks(lptmrAdj - lptmrTicks, lptmrFreq);
            if(lptmrAdj > adjustLptmrMaxFreqTicks)
            {
                adjustLptmrMaxFreqTicks = lptmrAdj;
            }
        }
    }   
}
#else  /* cPWR_EnableTimerAccuracyOptimization */
static void PWR_HandleDeepSleepMode_2_3(void)
{
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep = 0;
#endif
    uint32_t lptmrFreq;
    uint32_t lptmrLPEnter;
    uint32_t lptmrLPExit;
#if cPWR_BLE_LL_Enable
    uint16_t bleEnabledInt;
#endif
#if PWR_EnableRtosSysticks       
    uint32_t sysTickCtrl;
#endif /* PWR_EnableRtosSysticks */
    PWRLib_MCU_WakeupReason.AllBits = 0;

#if cPWR_BLE_LL_Enable
    if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT) == 0)  /* BLL in DSM */
    {
        return;
    }
#endif
#if PWR_EnableRtosSysticks      
    /* Disable SysTick counter and interrupt */
    sysTickCtrl = SysTick->CTRL & (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
#endif /* PWR_EnableRtosSysticks */
    lptmrFreq = PWR_ProgramDeepSleepTime();
#if (gTMR_EnableLowPowerTimers_d)
    /* if more low power timers are running, stop the hardware timer
    and save the spend time in ticks that wasn't counted.  */
    notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
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
    {
        uint32_t rfOscEn;
        uint8_t lpMode;
        bool_t lowpower_skipped;
        lpMode = PWRLib_GetDeepSleepMode();
        BOARD_BLPEtoBLPI();
        rfOscEn =  RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0);
        lptmrLPEnter = PWRLib_LPTMR_ClockCheckLight();
        if(lpMode == 2)
        {
            lowpower_skipped = PWRLib_MCU_Enter_LLS2();
        }
        else
        {
            lowpower_skipped = PWRLib_MCU_Enter_LLS3();
        }
        if(lowpower_skipped)
        {
            lptmrLPExit = PWRLib_LPTMR_ClockCheck();
            PWRLib_DelayLoopRemainingTicks(mPWR_WaitForRfOscOffIn32kTicks_c, lptmrLPExit - lptmrLPEnter, lptmrFreq);
        }
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);
        PWRLib_WaitFor32MhzReady();
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
#if PWR_EnableRtosSysticks       
    SysTick->CTRL |= sysTickCtrl;
#endif /* PWR_EnableRtosSysticks */ 
    /* Sync. the low power timers */
#if (gTMR_EnableLowPowerTimers_d)
    PWR_SyncTimers(notCountedTicksBeforeSleep, lptmrFreq);
#else 
    (void)lptmrFreq;
#endif /* gTMR_EnableLowPowerTimers_d */
}
#endif /* cPWR_EnableTimerAccuracyOptimization */
#endif /* (cPWR_UsePowerDownMode && (cPWR_EnableDeepSleepMode_2 || cPWR_EnableDeepSleepMode_3)) */

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
        (void)PWRLib_MCU_Enter_VLLSx(0);
#else
        (void)PWRLib_MCU_Enter_VLLSx(1);
#endif
        PWRLib_DelayLoop(mPWR_WaitForRfOscOffInLoopCounts_c);
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);
        PWRLib_WaitFor32MhzReady();
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
#if cPWR_EnableTimerAccuracyOptimization
static void PWR_HandleDeepSleepMode_5(void)
{
#if cPWR_BLE_LL_Enable
    uint16_t bleEnabledInt;
#endif
#if PWR_EnableRtosSysticks    
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep = 0;
#endif /* (gTMR_EnableLowPowerTimers_d) */
    uint32_t lptmrFreq;
    uint8_t clkMode;
    uint32_t lptmrTicks;
    uint32_t lptmrLPEnter;
    uint32_t lptmrLPExit;    
    uint32_t sysTickCtrl;
    static uint32_t adjustLptmrMaxFreqTicks = 0;
#endif /* PWR_EnableRtosSysticks */
    bool_t enterLowPower = TRUE;
    PWRLib_MCU_WakeupReason.AllBits = 0;
#if PWR_EnableRtosSysticks && gTMR_EnableLowPowerTimers_d   
    mUnlockTMRThread = FALSE;
#endif
#if cPWR_BLE_LL_Enable
    if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT) == 0)  /* BLL in DSM */
    {
        return;
    }
    
#endif
#if PWR_EnableRtosSysticks      
    /* Disable SysTick counter and interrupt */
    sysTickCtrl = SysTick->CTRL & (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    lptmrFreq = PWR_ProgramDeepSleepTimeTmrAccuracy(adjustLptmrMaxFreqTicks, &clkMode, &lptmrTicks, &enterLowPower );
#endif /* PWR_EnableRtosSysticks */  
    if(enterLowPower)
    {
        enterLowPower = (VECTOR_PENDING == 0);
    }      
    if(enterLowPower)
    {
        uint32_t rfOscEn;
        bool_t lowpower_skipped;
#if cPWR_BLE_LL_Enable
        bleEnabledInt = PWRLib_BLL_GetIntEn();
        PWRLib_BLL_ClearInterrupts(bleEnabledInt);
        PWRLib_BLL_DisableInterrupts(bleEnabledInt);
#endif       
#if PWR_EnableRtosSysticks              
        PWRLib_LPTMR_ClockStart(clkMode,lptmrTicks);
#if gTMR_EnableLowPowerTimers_d
        /* if more low power timers are running, stop the hardware timer
        and save the spend time in ticks that wasn't counted.  */
        notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif /* gTMR_EnableLowPowerTimers_d */      
#endif /* PWR_EnableRtosSysticks */   
        if(gpfPWR_LowPowerEnterCb != NULL)
        {
            gpfPWR_LowPowerEnterCb();
        }
        BOARD_BLPEtoBLPI();
        rfOscEn =  RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0);
#if PWR_EnableRtosSysticks            
        lptmrLPEnter = PWRLib_LPTMR_ClockCheckLight();
#endif           
#if (cPWR_RamRetentionInVLLS > 2)
        lowpower_skipped = PWRLib_MCU_Enter_VLLSx(3);
#else
        lowpower_skipped = PWRLib_MCU_Enter_VLLSx(2);
#endif
        if(lowpower_skipped)
        {
#if PWR_EnableRtosSysticks                
            lptmrLPExit = PWRLib_LPTMR_ClockCheck();
            PWRLib_DelayLoopRemainingTicks(mPWR_WaitForRfOscOffIn32kTicks_c, lptmrLPExit - lptmrLPEnter, lptmrFreq);
#else
            PWRLib_DelayLoop(mPWR_WaitForRfOscOffInLoopCounts_c);
#endif            
        }
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);
        PWRLib_WaitFor32MhzReady();
        BOARD_BLPItoBLPE();
        if(gpfPWR_LowPowerExitCb != NULL)
        {
            gpfPWR_LowPowerExitCb();
        }
        PWRLib_LLWU_UpdateWakeupReason();
#if PWR_EnableRtosSysticks && gTMR_EnableLowPowerTimers_d     
        /* Sync. the low power timers */
        mUnlockTMRThread = PWR_SyncTimersTmrAccuracy(notCountedTicksBeforeSleep, lptmrFreq);
#endif  /* PWR_EnableRtosSysticks && gTMR_EnableLowPowerTimers_d */
#if cPWR_BLE_LL_Enable
        PWRLib_BLL_EnableInterrupts(bleEnabledInt);
#endif
    }    
#if PWR_EnableRtosSysticks       
    SysTick->CTRL |= sysTickCtrl;
    /* Sync. the low power timers */
    if(enterLowPower )
    {
        PWRLib_LPTMR_ClockStop(); 
        if(PWRLib_MCU_WakeupReason.Bits.FromLPTMR == 1)
        {
            uint32_t lptmrAdj =  PWRLib_LPTMR_ClockCheck();
            lptmrAdj = PWRLib_LPTMR_GetMaxFreqTicks(lptmrAdj - lptmrTicks, lptmrFreq);
            if(lptmrAdj > adjustLptmrMaxFreqTicks)
            {
                adjustLptmrMaxFreqTicks = lptmrAdj;
            }
        }
    }   
#endif /* PWR_EnableRtosSysticks */
}
#else /* cPWR_EnableTimerAccuracyOptimization */
static void PWR_HandleDeepSleepMode_5(void)
{
#if cPWR_BLE_LL_Enable
    uint16_t bleEnabledInt;
#endif
#if PWR_EnableRtosSysticks    
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep = 0;
#endif /* (gTMR_EnableLowPowerTimers_d) */
    uint32_t lptmrFreq;
    uint32_t lptmrLPEnter;
    uint32_t lptmrLPExit;    
    uint32_t sysTickCtrl;
#endif /* PWR_EnableRtosSysticks */
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
#if PWR_EnableRtosSysticks      
    /* Disable SysTick counter and interrupt */
    sysTickCtrl = SysTick->CTRL & (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    lptmrFreq = PWR_ProgramDeepSleepTime();
#if (gTMR_EnableLowPowerTimers_d)
    /* if more low power timers are running, stop the hardware timer
    and save the spend time in ticks that wasn't counted.  */
    notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif /* (gTMR_EnableLowPowerTimers_d) */
#endif /* PWR_EnableRtosSysticks */  
    if(gpfPWR_LowPowerEnterCb != NULL)
    {
        gpfPWR_LowPowerEnterCb();
    }
    {
        uint32_t rfOscEn;
        bool_t lowpower_skipped;
        BOARD_BLPEtoBLPI();
        rfOscEn =  RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0);
#if PWR_EnableRtosSysticks            
        lptmrLPEnter = PWRLib_LPTMR_ClockCheckLight();
#endif           
#if (cPWR_RamRetentionInVLLS > 2)
        lowpower_skipped = PWRLib_MCU_Enter_VLLSx(3);
#else
        lowpower_skipped = PWRLib_MCU_Enter_VLLSx(2);
#endif
        if(lowpower_skipped)
        {
#if PWR_EnableRtosSysticks                
            lptmrLPExit = PWRLib_LPTMR_ClockCheck();
            PWRLib_DelayLoopRemainingTicks(mPWR_WaitForRfOscOffIn32kTicks_c, lptmrLPExit - lptmrLPEnter, lptmrFreq);
#else
            PWRLib_DelayLoop(mPWR_WaitForRfOscOffInLoopCounts_c);
#endif            
        }
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);
        PWRLib_WaitFor32MhzReady();
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
#if PWR_EnableRtosSysticks       
    PWRLib_LPTMR_ClockStop();
    SysTick->CTRL |= sysTickCtrl;
    /* Sync. the low power timers */
#if (gTMR_EnableLowPowerTimers_d)
    PWR_SyncTimers(notCountedTicksBeforeSleep, lptmrFreq);
#endif  /* (gTMR_EnableLowPowerTimers_d) */
#endif /* PWR_EnableRtosSysticks */
}
#endif /* cPWR_EnableTimerAccuracyOptimization */
#endif /* (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_5) */

/*---------------------------------------------------------------------------
 * Name: PWR_HandleDeepSleepMode_6
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_6)
#if cPWR_EnableTimerAccuracyOptimization
static void PWR_HandleDeepSleepMode_6(void)
{
    uint32_t lptmrFreq;
    uint32_t sysTickCtrl;
    uint8_t clkMode;
    uint32_t lptmrTicks;
    bool_t enterLowPower;
    static uint32_t adjustLptmrMaxFreqTicks = 0;
    PWRLib_MCU_WakeupReason.AllBits = 0;
#if cPWR_BLE_LL_Enable
    if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT) == 0)  /* BLL in DSM */
    {
        return;
    }
#endif
    /* Disable SysTick counter and interrupt */
    sysTickCtrl = SysTick->CTRL & (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
#if (gTMR_EnableLowPowerTimers_d)
    mUnlockTMRThread = FALSE;
#endif
    lptmrFreq = PWR_ProgramDeepSleepTimeTmrAccuracy(adjustLptmrMaxFreqTicks, &clkMode, &lptmrTicks, &enterLowPower );
    if(enterLowPower)
    {
        enterLowPower = (VECTOR_PENDING == 0);
    }      
    if(enterLowPower)
    {
        PWRLib_LPTMR_ClockStart(clkMode,lptmrTicks);
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
        PWRLib_StopUpdateWakeupReason();
    }
    /* Restore the state of SysTick */
    SysTick->CTRL |= sysTickCtrl;
    if(enterLowPower )
    {
        PWRLib_LPTMR_ClockStop(); 
        if(PWRLib_MCU_WakeupReason.Bits.FromLPTMR == 1)
        {
            uint32_t lptmrAdj =  PWRLib_LPTMR_ClockCheck();
            lptmrAdj = PWRLib_LPTMR_GetMaxFreqTicks(lptmrAdj - lptmrTicks, lptmrFreq);
            if(lptmrAdj > adjustLptmrMaxFreqTicks)
            {
                adjustLptmrMaxFreqTicks = lptmrAdj;
            }
        }
    }   
}
#else /* cPWR_EnableTimerAccuracyOptimization */
static void PWR_HandleDeepSleepMode_6(void)
{
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep = 0;
#endif
    uint32_t lptmrFreq;
    uint32_t sysTickCtrl;
    PWRLib_MCU_WakeupReason.AllBits = 0;
#if cPWR_BLE_LL_Enable
    if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT) == 0)  /* BLL in DSM */
    {
        return;
    }
#endif
    lptmrFreq = PWR_ProgramDeepSleepTime();
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
    PWR_SyncTimers(notCountedTicksBeforeSleep, lptmrFreq);
#else 
    (void)lptmrFreq;
#endif /* (gTMR_EnableLowPowerTimers_d) */
}
#endif /* cPWR_EnableTimerAccuracyOptimization */
#endif /* if (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_6) */

/*---------------------------------------------------------------------------
 * Name: PWR_HandleDeepSleepMode_7
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_7)
#if cPWR_EnableTimerAccuracyOptimization
static void PWR_HandleDeepSleepMode_7(void)
{
#if (cPWR_GENFSK_LL_Enable)
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep= 0;
#endif
    uint32_t lptmrFreq;
    uint8_t clkMode;
    uint32_t lptmrTicks;
    uint32_t lptmrLPEnter;
    uint32_t lptmrLPExit;
    static uint32_t adjustLptmrMaxFreqTicks = 0;
    bool_t enterLowPower = (VECTOR_PENDING == 0);
    bool_t dsmExit = FALSE;
    PWRLib_MCU_WakeupReason.AllBits = 0;
#if (gTMR_EnableLowPowerTimers_d)    
    mUnlockTMRThread = FALSE;
#endif  
    if(enterLowPower && (READ_REGISTER_FIELD(RSIM->DSM_CONTROL,RSIM_DSM_CONTROL_GEN_SLEEP_REQUEST) == 0))
    {
        if(VECTOR_PENDING == 0)
        {
            enterLowPower = dsmExit = PWR_HandleGenfskDsmEnter();
        }
        else
        {
            enterLowPower = FALSE;
        }
    }
    if(enterLowPower)
    {
        lptmrFreq = PWR_ProgramDeepSleepTimeTmrAccuracy(adjustLptmrMaxFreqTicks, &clkMode, &lptmrTicks, &enterLowPower );
        if(enterLowPower)
        {
            enterLowPower = (VECTOR_PENDING == 0);
        }
    }    
    if(enterLowPower)
    {
        uint32_t rfOscEn;
        bool_t lowpower_skipped;
        PWRLib_LPTMR_ClockStart(clkMode,lptmrTicks);
#if (gTMR_EnableLowPowerTimers_d)
        /* If more low power timers are running, stop the hardware timer
        * and save the spend time in ticks that wasn't counted.
        */
        notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif        
        if(gpfPWR_LowPowerEnterCb != NULL)
        {
            gpfPWR_LowPowerEnterCb();
        }
        BOARD_BLPEtoBLPI();
        rfOscEn =  RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0);
        lptmrLPEnter = PWRLib_LPTMR_ClockCheckLight();
        lowpower_skipped = PWRLib_MCU_Enter_LLS3();
        if(lowpower_skipped)
        {
            lptmrLPExit = PWRLib_LPTMR_ClockCheck();
            PWRLib_DelayLoopRemainingTicks(mPWR_WaitForRfOscOffIn32kTicks_c, lptmrLPExit - lptmrLPEnter, lptmrFreq);
        }
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);
        PWRLib_WaitFor32MhzReady();
        BOARD_BLPItoBLPE();
        if(gpfPWR_LowPowerExitCb != NULL)
        {
            gpfPWR_LowPowerExitCb();
        }
        PWRLib_LLWU_UpdateWakeupReason();
        /* Sync. the low power timers */
#if (gTMR_EnableLowPowerTimers_d)
        mUnlockTMRThread = PWR_SyncTimersTmrAccuracy(notCountedTicksBeforeSleep, lptmrFreq);
#endif
    }
    if(dsmExit)
    {
        PWR_HandleGenfskDsmExit();    
    }
    if(enterLowPower)
    {
        PWRLib_LPTMR_ClockStop(); 
        if(PWRLib_MCU_WakeupReason.Bits.FromLPTMR == 1)
        {
            uint32_t lptmrAdj =  PWRLib_LPTMR_ClockCheck();
            lptmrAdj = PWRLib_LPTMR_GetMaxFreqTicks(lptmrAdj - lptmrTicks, lptmrFreq);
            if(lptmrAdj > adjustLptmrMaxFreqTicks)
            {
                adjustLptmrMaxFreqTicks = lptmrAdj;
            }
        }
    }
#else
    PWRLib_MCU_WakeupReason.AllBits = 0;
#endif
}
#else  /* cPWR_EnableTimerAccuracyOptimization */
static void PWR_HandleDeepSleepMode_7(void)
{
#if (cPWR_GENFSK_LL_Enable)
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep= 0;
#endif
    uint32_t lptmrFreq;
    uint32_t lptmrLPEnter;
    uint32_t lptmrLPExit;
    bool_t dsmExit = FALSE;
    bool_t enterLowPower = (VECTOR_PENDING == 0);
    
    PWRLib_MCU_WakeupReason.AllBits = 0;
    lptmrFreq = PWR_ProgramDeepSleepTime();
#if (gTMR_EnableLowPowerTimers_d)
    /* If more low power timers are running, stop the hardware timer
    * and save the spend time in ticks that wasn't counted.
    */
    notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif
    if(enterLowPower && (READ_REGISTER_FIELD(RSIM->DSM_CONTROL,RSIM_DSM_CONTROL_GEN_SLEEP_REQUEST) == 0))
    {
        if(VECTOR_PENDING == 0)
        {
            enterLowPower = dsmExit = PWR_HandleGenfskDsmEnter();
        }
        else
        {
            enterLowPower = FALSE;
        }
    }
    if(enterLowPower)
    {
        uint32_t rfOscEn;
        bool_t lowpower_skipped;
        if(gpfPWR_LowPowerEnterCb != NULL)
        {
            gpfPWR_LowPowerEnterCb();
        }
        BOARD_BLPEtoBLPI();
        rfOscEn =  RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0);
        lptmrLPEnter = PWRLib_LPTMR_ClockCheckLight();
        lowpower_skipped = PWRLib_MCU_Enter_LLS3();
        if(lowpower_skipped)
        {
            lptmrLPExit = PWRLib_LPTMR_ClockCheck();
            PWRLib_DelayLoopRemainingTicks(mPWR_WaitForRfOscOffIn32kTicks_c, lptmrLPExit - lptmrLPEnter, lptmrFreq);
        }
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);
        PWRLib_WaitFor32MhzReady();
        BOARD_BLPItoBLPE();
        if(gpfPWR_LowPowerExitCb != NULL)
        {
            gpfPWR_LowPowerExitCb();
        }
        PWRLib_LLWU_UpdateWakeupReason();
        if(dsmExit)
        {
            PWR_HandleGenfskDsmExit();
        }
    }
    PWRLib_LPTMR_ClockStop();
    /* Sync. the low power timers */
#if (gTMR_EnableLowPowerTimers_d)
    PWR_SyncTimers(notCountedTicksBeforeSleep, lptmrFreq);
#endif
#else
    PWRLib_MCU_WakeupReason.AllBits = 0;
#endif
}
#endif /* cPWR_EnableTimerAccuracyOptimization */
#endif /* (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_7) */

/*---------------------------------------------------------------------------
 * Name: PWR_HandleDeepSleepMode_8
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_8)
#if cPWR_EnableTimerAccuracyOptimization
static void PWR_HandleDeepSleepMode_8(void)
{
#if cPWR_BLE_LL_Enable
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep = 0;
#endif /* (gTMR_EnableLowPowerTimers_d) */
    uint32_t lptmrFreq;
    uint8_t clkMode;
    uint32_t lptmrTicks;
    uint32_t lptmrLPEnter;
    uint32_t lptmrLPExit;
    uint32_t sysTickCtrl;
    bool_t enterLowPower = !RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT);
    bool_t dsmExit = FALSE;
    static uint32_t adjustLptmrMaxFreqTicks = 0;
#if mPWR_WaitFor32kOscStart_c    
    osc32KStatus_t osc32KStatus = PWRLib_Get32KHzOscStatus();
    if(osc32KStatus == gOsc32K_NotStarted_c)
    {
        osc32KStatus = PWRLib_Check32KHzOscStatus();
    }
    if(osc32KStatus != gOsc32K_Ready_c)
    {
        enterLowPower = FALSE;
    }
#endif   
    PWRLib_MCU_WakeupReason.AllBits = 0;    
#if (gTMR_EnableLowPowerTimers_d)    
    mUnlockTMRThread = FALSE;
#endif   
    if(enterLowPower)
    {
        /* Disable SysTick counter and interrupt */
        sysTickCtrl = SysTick->CTRL & (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
        SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
        lptmrFreq = PWR_ProgramDeepSleepTimeTmrAccuracy(adjustLptmrMaxFreqTicks, &clkMode, &lptmrTicks, &enterLowPower );
        if(enterLowPower)
        {
            PWR_CheckBLE_LL_state(&enterLowPower, &dsmExit);
            if(enterLowPower)
            {
                uint32_t rfOscEn;
                bool_t lowpower_skipped;
                PWRLib_LPTMR_ClockStart(clkMode,lptmrTicks);
#if (gTMR_EnableLowPowerTimers_d)
                /* If more low power timers are running, stop the hardware timer
                * and save the spend time in ticks that wasn't counted.
                */
                notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif /* (gTMR_EnableLowPowerTimers_d) */             
                if(gpfPWR_LowPowerEnterCb != NULL)
                {
                    gpfPWR_LowPowerEnterCb();
                }
                BOARD_BLPEtoBLPI();
                rfOscEn =  RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
                RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0);
                lptmrLPEnter = PWRLib_LPTMR_ClockCheckLight();
#if (cPWR_RamRetentionInVLLS > 2)
                lowpower_skipped = PWRLib_MCU_Enter_VLLSx(3);
#else
                lowpower_skipped = PWRLib_MCU_Enter_VLLSx(2);
#endif
                if(lowpower_skipped)
                {
                    lptmrLPExit = PWRLib_LPTMR_ClockCheck();
                    PWRLib_DelayLoopRemainingTicks(mPWR_WaitForLlDSMandRfOscOffIn32kTicks_c, lptmrLPExit - lptmrLPEnter, lptmrFreq);
                }            
                RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);
                PWRLib_WaitFor32MhzReady();
                BOARD_BLPItoBLPE();
                if(gpfPWR_LowPowerExitCb != NULL)
                {
                    gpfPWR_LowPowerExitCb();
                }
                PWRLib_LLWU_UpdateWakeupReason();
                /* Sync. the low power timers */
#if (gTMR_EnableLowPowerTimers_d)
                mUnlockTMRThread = PWR_SyncTimersTmrAccuracy(notCountedTicksBeforeSleep, lptmrFreq);
#endif /* (gTMR_EnableLowPowerTimers_d) */                
            }
            if(dsmExit)
            {
                if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_INT) == 0)
                {
                    RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT, 1);
                }
                while((BLE_LL_REG(EVENT_STATUS) & BLE_LL_EVENT_STATUS_DSM_INT_MASK) == 0)
                {
                }
            }
        }
        SysTick->CTRL |= sysTickCtrl;
        if(enterLowPower )
        {
            PWRLib_LPTMR_ClockStop(); 
            if(PWRLib_MCU_WakeupReason.Bits.FromLPTMR == 1) 
            {
                uint32_t lptmrAdj =  PWRLib_LPTMR_ClockCheck();
                lptmrAdj = PWRLib_LPTMR_GetMaxFreqTicks(lptmrAdj - lptmrTicks, lptmrFreq);
                if(lptmrAdj > adjustLptmrMaxFreqTicks)
                {
                    adjustLptmrMaxFreqTicks = lptmrAdj;
                }
            }
        }
    }
#else
    PWRLib_MCU_WakeupReason.AllBits = 0;
#endif /* cPWR_BLE_LL_Enable */
}
#else /*cPWR_EnableTimerAccuracyOptimization*/
static void PWR_HandleDeepSleepMode_8(void)
{
#if cPWR_BLE_LL_Enable
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep = 0;
#endif /* (gTMR_EnableLowPowerTimers_d) */
    uint32_t lptmrFreq;
    uint32_t lptmrLPEnter;
    uint32_t lptmrLPExit;
    uint32_t sysTickCtrl;
    bool_t enterLowPower = !RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT);
    bool_t dsmExit = FALSE;
#if mPWR_WaitFor32kOscStart_c    
    osc32KStatus_t osc32KStatus = PWRLib_Get32KHzOscStatus();
    if(osc32KStatus == gOsc32K_NotStarted_c)
    {
        osc32KStatus = PWRLib_Check32KHzOscStatus();
    }
    if(osc32KStatus != gOsc32K_Ready_c)
    {
        enterLowPower = FALSE;
    }
#endif   
    if(enterLowPower)
    {
        /* Disable SysTick counter and interrupt */
        sysTickCtrl = SysTick->CTRL & (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
        SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
        PWRLib_MCU_WakeupReason.AllBits = 0;
        lptmrFreq = PWR_ProgramDeepSleepTime();
#if (gTMR_EnableLowPowerTimers_d)
        /* If more low power timers are running, stop the hardware timer
        * and save the spend time in ticks that wasn't counted.
        */
        notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif /* (gTMR_EnableLowPowerTimers_d) */
        PWR_CheckBLE_LL_state(&enterLowPower, &dsmExit);
        if(enterLowPower)
        {
            uint32_t rfOscEn;
            bool_t lowpower_skipped;
            if(gpfPWR_LowPowerEnterCb != NULL)
            {
                gpfPWR_LowPowerEnterCb();
            }
            BOARD_BLPEtoBLPI();
            rfOscEn =  RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_EN);
            RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 0);
            lptmrLPEnter = PWRLib_LPTMR_ClockCheckLight();
#if (cPWR_RamRetentionInVLLS > 2)
            lowpower_skipped = PWRLib_MCU_Enter_VLLSx(3);
#else
            lowpower_skipped = PWRLib_MCU_Enter_VLLSx(2);
#endif
            if(lowpower_skipped)
            {
                lptmrLPExit = PWRLib_LPTMR_ClockCheck();
                PWRLib_DelayLoopRemainingTicks(mPWR_WaitForLlDSMandRfOscOffIn32kTicks_c, lptmrLPExit - lptmrLPEnter, lptmrFreq);
            }            
            RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);
            PWRLib_WaitFor32MhzReady();
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
        PWR_SyncTimers(notCountedTicksBeforeSleep, lptmrFreq);
#else 
        (void)lptmrFreq;        
#endif /* (gTMR_EnableLowPowerTimers_d) */
        if(dsmExit)
        {
            if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_INT) == 0)
            {
                RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT, 1);
            }
            while((BLE_LL_REG(EVENT_STATUS) & BLE_LL_EVENT_STATUS_DSM_INT_MASK) == 0)
            {
            }
        }
        SysTick->CTRL |= sysTickCtrl;
    }
#else
    PWRLib_MCU_WakeupReason.AllBits = 0;
#endif /* cPWR_BLE_LL_Enable */
}
#endif /*cPWR_EnableTimerAccuracyOptimization*/
#endif /* (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_8) */
/*---------------------------------------------------------------------------
 * Name: PWR_ProgramDeepSleepTime
 * Description: Program the next DeepSleep wakeup time
 * Parameters: - 
 * Return: - LPTMR frequency in Hz
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode)
#if (cPWR_EnableTimerAccuracyOptimization == 0U)
static uint32_t PWR_ProgramDeepSleepTime(void)
{
    uint8_t clkMode;
    uint32_t lptmrTicks;
    uint32_t lptmrFreq;
    uint32_t sleepTimeMs;
    
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
    
    return lptmrFreq;
}
#else /* (cPWR_EnableTimerAccuracyOptimization == 0U) */ 
/*---------------------------------------------------------------------------
 * Name: PWR_ProgramDeepSleepTimeTmrAccuracy
 * Description: Program the next DeepSleep wakeup time
 * Parameters: [in] adjustTicks 
 * Parameters: [out] pClkMode, pLptmrTicks, pEnterLowPower
 * Return: - LPTMR frequency in Hz
 *---------------------------------------------------------------------------*/
static uint32_t PWR_ProgramDeepSleepTimeTmrAccuracy(uint32_t adjustTicks, uint8_t* pClkMode, uint32_t* pLptmrTicks, bool_t* pEnterLowPower )
{
    uint32_t lptmrFreq = 0;
    uint32_t sleepTimeMs;
    bool_t enterLowPower = TRUE;
    tmrTimerTicks64_t timerTicks;
#if (cPWR_BLE_LL_Enable  ||   cPWR_GENFSK_LL_Enable)
    uint8_t lpMode = PWRLib_GetDeepSleepMode();
#endif    
    sleepTimeMs = mPWR_DeepSleepTimeMs;
#if cPWR_BLE_LL_Enable
    if((lpMode == 1U) || (lpMode == 8U))
    {
        if(sleepTimeMs > gPWRLib_MaxLPTMRTimeoutForMaxFreqMs_c)
        {
            uint32_t distanceToNextInstantMs;
            uint32_t maxSleepTimeForDesiredClockAccuracyMs;
            uint16_t nextInstant = 0U;
            distanceToNextInstantMs = PWR_BLE_GetNearestInstant(&nextInstant);
            /* translate BT slots(0.625ms) to ms */
            distanceToNextInstantMs *= 5U;
            distanceToNextInstantMs >>= 3U;
            maxSleepTimeForDesiredClockAccuracyMs = PWRLib_LPTMR_GetMaxDurationForTheSameFreq(distanceToNextInstantMs);
            if(maxSleepTimeForDesiredClockAccuracyMs < sleepTimeMs)
            {
                sleepTimeMs = maxSleepTimeForDesiredClockAccuracyMs;
            }    
        }
    }
#endif /* cPWR_BLE_LL_Enable */
#if cPWR_GENFSK_LL_Enable
    if(lpMode == 7U)
    {
        uint32_t dsmDurationMs;
        uint32_t maxSleepTimeForDesiredClockAccuracyMs;
        uint32_t dsmTimerTicksToSleep = RSIM->GEN_WAKE; 
        dsmTimerTicksToSleep -=  RSIM->DSM_TIMER;
        dsmTimerTicksToSleep &= mPWR_DSM_TimerMask_c;
        /* Translate DSM Timer ticks to ms*/
        dsmDurationMs = (dsmTimerTicksToSleep * 1000U) >> 15U;
        maxSleepTimeForDesiredClockAccuracyMs = PWRLib_LPTMR_GetMaxDurationForTheSameFreq(dsmDurationMs); 
        if(maxSleepTimeForDesiredClockAccuracyMs < sleepTimeMs)
        {
            sleepTimeMs = maxSleepTimeForDesiredClockAccuracyMs;
        }   
    }
    
#endif /* cPWR_GENFSK_LL_Enable */   
    timerTicks = (sleepTimeMs * TMR_GetTimerFreq())/1000;
#if (gTMR_EnableLowPowerTimers_d) && (cPWR_CheckLowPowerTimers)
    /* Get the expire time of the first programmed Low Power Timer */
    {
        tmrTimerID_t tmrTimerID = TMR_LpmGetFirstLowPowerTimerToExpire();
        if(tmrTimerID != gTmrInvalidTimerID_c)
        {  
            tmrTimerTicks64_t adjustTimerTicks;
            tmrTimerTicks64_t firstTimeout = TMR_LpmGetRemainingTicks(tmrTimerID);
            adjustTimerTicks = adjustTicks;
            /* Translate LPTMR max frequency ticks to TPM ticks*/
            adjustTimerTicks = (adjustTimerTicks * TMR_GetTimerFreq() + (1U << 14U)) >> 15U;
            if(firstTimeout > adjustTimerTicks)
            {
                firstTimeout -= adjustTimerTicks;
                if(firstTimeout < timerTicks)
                {
                    timerTicks  =  firstTimeout;  
                }
            }
            else
            {
                enterLowPower = FALSE;
            }
        }
    }    
#endif /* (gTMR_EnableLowPowerTimers_d) && (cPWR_CheckLowPowerTimers) */    
    if(enterLowPower)
    {
        /* Translate TPM ticks to LPTMR max frequency ticks */  
        timerTicks =   (timerTicks << 15)/TMR_GetTimerFreq();
        PWRLib_LPTMR_GetTimeSettingsFromLptmrMaxFreqTicks(timerTicks, pClkMode ,pLptmrTicks, &lptmrFreq); 
    }
    *pEnterLowPower = enterLowPower;
    return lptmrFreq;
}
#endif /* (cPWR_EnableTimerAccuracyOptimization == 0U) */ 
#endif /* cPWR_UsePowerDownMode */

/*---------------------------------------------------------------------------
 * Name: PWR_SyncTimers
 * Description: Synchronize the framework timers with the amount of time in LP
 * Parameters: - notCountedTicksBeforeSleep: 
 *             - lptmrFreq: 
 * Return: -
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode) && (gTMR_EnableLowPowerTimers_d)
#if (cPWR_EnableTimerAccuracyOptimization == 0U)
static void PWR_SyncTimers(uint32_t notCountedTicksBeforeSleep, uint32_t lptmrFreq)
{
    uint64_t timerTicks = PWRLib_LPTMR_ClockCheck();
    mLpmTotalSleepDuration += timerTicks * 1000000 / lptmrFreq;
    timerTicks = (timerTicks * TMR_GetTimerFreq()) / lptmrFreq;
    timerTicks += notCountedTicksBeforeSleep;
    TMR_SyncLpmTimers((uint32_t)timerTicks);
}
#else /* (cPWR_EnableTimerAccuracyOptimization == 0U) */ 
/*---------------------------------------------------------------------------
 * Name: PWR_SyncTimersTmrAccuracy
 * Description: Synchronize the framework timers with the amount of time in LP
 * Parameters: - [in] notCountedTicksBeforeSleep: 
 *             - [in] lptmrFreq: 
 * Return: whether the timer task needs to be scheduled or not
 *---------------------------------------------------------------------------*/
static bool_t PWR_SyncTimersTmrAccuracy(uint32_t notCountedTicksBeforeSleep, uint32_t lptmrFreq)
{
    static uint32_t lptmrLostTicks = 0;
    bool_t makeTmrTaskReady;
    uint64_t timerTicks;
    uint32_t lptmrTicksExit;
    uint32_t lptmrTicksEnter = PWRLib_LPTMR_ClockCheck();
    /*When the LPTMR is enabled, the first increment will take an additional one or two prescaler clock cycles due to synchronization logic.*/
    timerTicks = PWRLib_LPTMR_GetMaxFreqTicks(lptmrTicksEnter, lptmrFreq) + 1U;
    timerTicks += lptmrLostTicks;
    mLpmTotalSleepDuration += (timerTicks * 1000000U) >> 15U;
    timerTicks = (timerTicks * TMR_GetTimerFreq() + (1U << 14U)) >> 15U;
    timerTicks += notCountedTicksBeforeSleep;
    makeTmrTaskReady = TMR_SyncLpmTimersAccurate((uint32_t)timerTicks);
    lptmrTicksExit = PWRLib_LPTMR_ClockCheck();
    lptmrLostTicks = PWRLib_LPTMR_GetMaxFreqTicks((lptmrTicksExit - lptmrTicksEnter), lptmrFreq);
    return makeTmrTaskReady;
}
#endif /* (cPWR_EnableTimerAccuracyOptimization == 0U) */ 
#endif /* (cPWR_UsePowerDownMode) && (gTMR_EnableLowPowerTimers_d) */

/*---------------------------------------------------------------------------
 * Name: PWR_CheckBLE_LL_state
 * Description: Checks whether the BLE link layer allows to enter low power state
 * Parameters: - enterLowPower: TRUE if low power mode alowed
 *             - dsmExit: TRUE if manual DSM exit is required
 * Return: -
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode) && (cPWR_BLE_LL_Enable)
static void PWR_CheckBLE_LL_state(bool_t * enterLowPower, bool_t * dsmExit)
{
    if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT)) /* If false it means that BLE_LL is already in DSM */
    {
        uint16_t distanceToNextInstant;
        uint16_t nextInstant = 0;
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
                    *dsmExit = TRUE;
                }
                else
                {
                    *enterLowPower = FALSE;
                }
                
            }
            else
            {
                *enterLowPower = FALSE;
            }
        }
        else
        {
            *enterLowPower = FALSE;
        }
    }
    else
    {
        *enterLowPower = (VECTOR_PENDING == 0);
    }
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
        if(PWR_CheckIfDeviceCanGoToSleep())
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

#ifdef SUPPORT_WARMBOOT
#if cPWR_EnableDeepSleepMode_8 || cPWR_EnableDeepSleepMode_5
    warmbootConfig.doWarmbootSequence = WARMBOOT_SEQUENCE;
#else
    warmbootConfig.doWarmbootSequence = 0;
#endif
    warmbootConfig.useWarmbootSp      = USE_WARMBOOT_SP;
#endif

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
    if(dsMode > 8)
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
#if (mPWR_WaitFor32kOscStart_c && cPWR_32kOscStabilizationDelayInMs)
    if(gOsc32K_Unstable_c == PWRLib_Get32KHzOscStatus())
    {
        mPendingDSMode = dsMode;
        return TRUE;
    }
#endif    
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
    mUnlockTMRThread = FALSE;
#endif
    ReturnValue.AllBits = 0;
    if(PWR_LVD_ReportLevel() == PWR_LEVEL_CRITICAL)
    {
        /* Voltage <= 1.8V so enter power-off state - to disable false Tx'ing(void)*/
        ReturnValue = PWR_CheckForAndEnterNewPowerState(PWR_OFF);
    }
#if (mPWR_WaitFor32kOscStart_c && cPWR_32kOscStabilizationDelayInMs)
    if((mPendingDSMode != mPWR_InvalidDSMode_c) && (gOsc32K_Ready_c == PWRLib_Get32KHzOscStatus()))
    {
        (void)PWR_ChangeDeepSleepMode(mPendingDSMode);
        mPendingDSMode = mPWR_InvalidDSMode_c;
    }
#endif  
    OSA_InterruptDisable();
#if USE_RTOS == 0
    if(!OSA_TaskShouldYield())
    {  
#endif    
        if (TMR_AreAllTimersOff())  /*No timer running*/
        {
            /* If power lib is enabled */
#if (cPWR_UsePowerDownMode)
            /* If Low Power Capability is enabled */
#if (gTMR_EnableLowPowerTimers_d)
            /* If more low power timers are running, stop the hardware timer
            *and save the spend time in ticks that wasn't counted.
            */
            mUnlockTMRThread = TRUE;
#endif /* (gTMR_EnableLowPowerTimers_d) */
#endif /* (cPWR_UsePowerDownMode) */
            ReturnValue = PWR_CheckForAndEnterNewPowerState(PWR_DeepSleep);
        }
        else /* Timers are running */
        {
            ReturnValue = PWR_CheckForAndEnterNewPowerState(PWR_Sleep);
        }
#if USE_RTOS == 0
    }
#endif   
    OSA_InterruptEnable();
#if (gTMR_EnableLowPowerTimers_d)
    if(mUnlockTMRThread)
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

/*---------------------------------------------------------------------------
 * Name: PWR_ClearRSIMDsmInt
 * Description:
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
bool_t PWR_ClearRSIMDsmInt(void)
{
    return PWRLib_ClearRSIMDsmInt();
}
#endif

