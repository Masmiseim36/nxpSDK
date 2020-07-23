/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
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
#include "fsl_lptmr.h"
#include "board.h"
#include "clock_config.h"
#include "DCDC.h"
#include "Keyboard.h"
#include "SerialManager.h"
#include "NVM_Interface.h"
//#include "gap_interface.h"   // for Gap_GetBondedDevicesCount() for debugging

#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1)
#include "controller_interface.h"
#include "TMR_Adapter.h"
#endif

#include "nxp2p4_xcvr.h"

/*****************************************************************************
 *                             PRIVATE MACROS                                *
 *---------------------------------------------------------------------------*
 * Add to this section all the access macros, registers mappings, bit access *
 * macros, masks, flags etc ...                                              *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#define mBLETimoutMaxMs_c       40959U
#define mPWR_DSM_TimerMask_c    (0xffffffU)
#define mPWR_DSM_InvalidTime_c  (0xffffffffU)
#define mPWR_DSM_MaxTimeCount_c (0x1000000U)

#ifndef VECTOR_PENDING
#define VECTOR_PENDING (((*((volatile uint32_t*)0xE000ED04U))>>12)&0x3fU)
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

#ifndef DUMP_EVERY_X_TIMES
#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
#define DUMP_EVERY_X_TIMES      1
#else
#define DUMP_EVERY_X_TIMES      3
#endif
#endif

#if (!defined(GCOV_DO_COVERAGE) || (GCOV_DO_COVERAGE == 0))
#define PWR_NOP    PWR_ExecuteNop()
#define PWR_STATIC static
#define PWR_PUBLIC
#else
#define PWR_NOP    break
#define PWR_STATIC __WEAK
#define PWR_PUBLIC __WEAK
#endif


#if (cPWR_BLE_LL_Enable)
/* LLS3 + BLE DSM wakeup on the first next instant by BLE timer */
#define BLE_LL_CLOCK_CONFIG_LP_MOD_1   (BLE_LL_CLOCK_CONFIG_DSM_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_DSM_AUTO_WKUP_MASK | \
                                       BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK)

/* LLS3 + BLE idle wakeup on deepSleepTimeInMs by LPTMR */
#define BLE_LL_CLOCK_CONFIG_LP_MOD_2_3 (BLE_LL_CLOCK_CONFIG_DSM_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_DSM_INT_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK)

/* VLLS0 + BLE idle wakeup by keyboard */
#define BLE_LL_CLOCK_CONFIG_LP_MOD_4   (BLE_LL_CLOCK_CONFIG_DSM_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_DSM_INT_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK)

/* VLLS2 + BLE idle wakeup by keyboard */
#define BLE_LL_CLOCK_CONFIG_LP_MOD_5   (BLE_LL_CLOCK_CONFIG_DSM_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_DSM_INT_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK)

/* STOP wakeup on deepSleepTimeInMs by LPTMR */
#define BLE_LL_CLOCK_CONFIG_LP_MOD_6   (BLE_LL_CLOCK_CONFIG_DSM_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK)

/*  LLS3  + GENFSK_LL in DSM. */
#define BLE_LL_CLOCK_CONFIG_LP_MOD_7   (BLE_LL_CLOCK_CONFIG_DSM_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK)

/* VLLS2/3 + BLE LL in DSM shutdown, wakeup on the first next instant by BLE timer */
#define BLE_LL_CLOCK_CONFIG_LP_MOD_8   (BLE_LL_CLOCK_CONFIG_DSM_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_DSM_AUTO_WKUP_MASK | \
                                       BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK)

/* Default clock config used when going into Sleep mode */
#define BLE_LL_CLOCK_CONFIG_DEFAULT    (BLE_LL_CLOCK_CONFIG_DSM_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_SYS_CLK_GATE_EN_MASK | \
                                       BLE_LL_CLOCK_CONFIG_LPO_FREQ_SEL_MASK)

#endif /* (cPWR_BLE_LL_Enable) */

#define OFFSET_BEFORE_DSM_EXIT(__wakeup_offset, __osc_startup_delay)    ((uint16_t)(\
                   (__wakeup_offset)                                               \
                 + (((osc_startup_delay) & BLE_LL_WAKEUP_CONFIG_OSC_DELAY_SLOT_MASK) >> BLE_LL_WAKEUP_CONFIG_OSC_DELAY_SLOT_SHIFT) \
                 + 1U))

#define WAKEUP_CFG_OSC_STARTUP_DELAY(__wakeup_offset, __osc_startup_delay) ((uint16_t)(\
                 (((__wakeup_offset) << BLE_LL_WAKEUP_CONFIG_DSM_OFFSET_SHIFT) & BLE_LL_WAKEUP_CONFIG_DSM_OFFSET_MASK) |        \
                 (((__osc_startup_delay) << BLE_LL_WAKEUP_CONFIG_OSC_DELAY_SHIFT)& BLE_LL_WAKEUP_CONFIG_OSC_DELAY_MASK)         \
                   ))

#define PWR_SERIAL_IF_INVALID          0xFFU
#define PWR_NEXT_INSTANT_INVALID       0xFFFFU

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
#if (cPWR_EnableDeepSleepMode_1 || cPWR_EnableDeepSleepMode_2 || cPWR_EnableDeepSleepMode_3)
static void PWR_HandleDeepSleepMode_1_2_3(void);
#endif
#if cPWR_EnableDeepSleepMode_4
static void PWR_HandleDeepSleepMode_4(void);
#endif
#if cPWR_EnableDeepSleepMode_6
static void PWR_HandleDeepSleepMode_6(void);
#endif
#if cPWR_EnableDeepSleepMode_7
static void PWR_HandleDeepSleepMode_7(void);
#endif
#if (cPWR_EnableDeepSleepMode_8 || cPWR_EnableDeepSleepMode_5)
static void PWR_HandleDeepSleepMode_5_8(void);
#endif
#if (cPWR_EnableDeepSleepMode_9)
static void PWR_HandleDeepSleepMode_9(void);
static void PWR_BLE_LL_ResetRestartAdv(void);
#if defined(gPWR_RamOffRestoreBonding_d) && (gPWR_RamOffRestoreBonding_d == 1)
static void PWR_RestoreBondingFromRamOff(void);
#endif
#endif

#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1)
static bool_t PWR_CheckBLE_LL_state(uint16_t * nextInstant, bool_t * requireDsmEntry, uint16_t offsetBeforeDsmExit);
#endif

static uint32_t PWR_ProgramDeepSleepTime(void);

#if (gTMR_EnableLowPowerTimers_d)
static void PWR_SyncTimers(uint32_t notCountedTicksBeforeSleep, uint32_t lptmrFreq);
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
    PWR_Run = 77U,
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
static uint8_t mLPMFlag = gAllowDeviceToSleep_c;
static uint64_t mLpmTotalSleepDuration;

#if (cPWR_UsePowerDownMode)
static uint8_t  mLpmXcvrDisallowCnt      = 0;

static const uint8_t pwr_app_state_to_lowpower_state[] =
{
#if (cPWR_EnableDeepSleepMode_8) || (cPWR_EnableDeepSleepMode_9)

    PWR_LOWPOWER_VLLS0_1_PSWITCH_MODE,       /* PWR_APP_STATE_NO_ACTIVITY     */
#if (cPWR_EnableDeepSleepMode_5)
    PWR_LOWPOWER_VLLS2_3_NO_DSM_MODE,        /* PWR_APP_STATE_NO_ACTIVITY_RAM_RET */
#else
    PWR_LOWPOWER_LLS2_3_NO_DSM_MODE,         /* PWR_APP_STATE_NO_ACTIVITY_RAM_RET */
#endif
#if (cPWR_EnableDeepSleepMode_9)
    PWR_LOWPOWER_VLLS2_3_LL_RESET_ADV_MODE,  /* PWR_APP_STATE_ADV   : Mode 9 only used in ADV */
#else
    PWR_LOWPOWER_VLLS2_3_LL_DSM3_MODE,       /* PWR_APP_STATE_ADV   : Use PWR_LOWPOWER_LLS2_3_LL_DSM_MODE or PWR_LOWPOWER_VLLS2_3_LL_DSM3_MODE */
#endif
    PWR_LOWPOWER_LLS2_3_LL_DSM_MODE,         /* PWR_APP_STATE_SCAN  : Use only PWR_LOWPOWER_LLS2_3_LL_DSM_MODE */
#if defined(cPWR_UseVllsInConnectedMode) && (cPWR_UseVllsInConnectedMode == 1)
    PWR_LOWPOWER_VLLS2_3_LL_DSM3_MODE,       /* PWR_APP_STATE_CONN */
#else
    PWR_LOWPOWER_LLS2_3_LL_DSM_MODE,         /* PWR_APP_STATE_CONN */
#endif

#else   // (cPWR_EnableDeepSleepMode_8) || (cPWR_EnableDeepSleepMode_9)

    PWR_LOWPOWER_VLLS0_1_PSWITCH_MODE,       /* PWR_APP_STATE_NO_ACTIVITY     */
#if (cPWR_EnableDeepSleepMode_5)
    PWR_LOWPOWER_VLLS2_3_NO_DSM_MODE,        /* PWR_APP_STATE_NO_ACTIVITY_RAM_RET */
#else
    PWR_LOWPOWER_LLS2_3_NO_DSM_MODE,         /* PWR_APP_STATE_NO_ACTIVITY_RAM_RET */
#endif
    PWR_LOWPOWER_LLS2_3_LL_DSM_MODE,         /* PWR_APP_STATE_ADV   : Use only PWR_LOWPOWER_LLS2_3_LL_DSM_MODE */
    PWR_LOWPOWER_LLS2_3_LL_DSM_MODE,         /* PWR_APP_STATE_SCAN  : Use only PWR_LOWPOWER_LLS2_3_LL_DSM_MODE */
    PWR_LOWPOWER_LLS2_3_LL_DSM_MODE,         /* PWR_APP_STATE_CONN  : Use only PWR_LOWPOWER_LLS2_3_LL_DSM_MODE */
#endif
};

#if (cPWR_GENFSK_LL_Enable)
static uint32_t mPWR_AbsoluteWakeupTimeInDsmTicks = mPWR_DSM_InvalidTime_c;
#endif

static const pfHandleDeepSleepFunc_t maHandleDeepSleep[] =
{
#if cPWR_EnableDeepSleepMode_1
    PWR_HandleDeepSleepMode_1_2_3,
#else
    (pfHandleDeepSleepFunc_t)0,
#endif
#if cPWR_EnableDeepSleepMode_2 && cPWR_EnableDeepSleepMode_1
    PWR_HandleDeepSleepMode_1_2_3,
#else
    (pfHandleDeepSleepFunc_t)0,
#endif
#if cPWR_EnableDeepSleepMode_3 && cPWR_EnableDeepSleepMode_1
    PWR_HandleDeepSleepMode_1_2_3,     /* Mode 3 is now handled in mode 1 handler */
#else
    (pfHandleDeepSleepFunc_t)0,
#endif
#if cPWR_EnableDeepSleepMode_4
    PWR_HandleDeepSleepMode_4,
#else
    (pfHandleDeepSleepFunc_t)0,
#endif
#if (cPWR_EnableDeepSleepMode_5)
    PWR_HandleDeepSleepMode_5_8,     /* Mode 5 is now handled in mode 8 handler */
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
    PWR_HandleDeepSleepMode_5_8,
#else
    (pfHandleDeepSleepFunc_t)0,
#endif
#if cPWR_EnableDeepSleepMode_9
    PWR_HandleDeepSleepMode_9,
#else
    (pfHandleDeepSleepFunc_t)0,
#endif
};

static volatile AT_WARMBOOTCONFIG_SECTION(warmboot_config_t warmbootConfig);

#if (defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0))
static uint8_t  pwr_serial_interface          = PWR_SERIAL_IF_INVALID;
uint32_t        pwr_llwu_serial_bitmap        = 0U;
#endif

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
* Name: PWR_ExecuteNop
* Description: Executes NOP instruction, used mainly for Misra
*---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode)
PWR_STATIC void PWR_ExecuteNop(void)
{
    /* Putting ASM instruction in a static function to isolate the code
       and fix related MISRA issue */
    asm volatile("NOP");
}
#endif /* cPWR_UsePowerDownMode */

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
        GENFSK->DSM_CTRL = GENFSK_DSM_CTRL_GEN_SLEEP_REQUEST_MASK;  /* Enter DSM */
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
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable) && (cPWR_EnableDeepSleepMode_7))
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
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable) && (cPWR_EnableDeepSleepMode_7))
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
 * Name: PWR_ProcessWakeupReason
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode)
static void PWR_ProcessWakeupReason(void)
{
#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
    if(PWRLib_MCU_WakeupReason.Bits.FromSerial != 0U)
    {
        assert( pwr_serial_interface != PWR_SERIAL_IF_INVALID );

        SerialInterface_SerialIsWakeupSrc( pwr_serial_interface );
        PWRLib_MCU_WakeupReason.Bits.FromSerial = 0U;
    }
#endif

#if (gKBD_KeysCount_c > 0)
    /* Woke up on Keyboard Press */
    if(PWRLib_MCU_WakeupReason.Bits.FromKeyBoard != 0U)
    {
#if defined(cPWR_BleAppHandleKeyDirectCall_d) && ( cPWR_BleAppHandleKeyDirectCall_d == 1)
        /* Skip over the key scan timer to improve low power consumption. */
        /* Warning : we assume that the pin wakeup is PB2 */
extern void BleApp_HandleKeys(key_event_t events);
        BleApp_HandleKeys(gKBD_EventPressPB2_c);
#else
        KBD_SwitchPressedOnWakeUp();
#endif
        PWRLib_MCU_WakeupReason.Bits.FromKeyBoard = 0U;
    }
#endif
}

/*---------------------------------------------------------------------------
 * Name: PWR_WaitFor32MhzReady
 *---------------------------------------------------------------------------*/
static void PWR_WaitFor32MhzReady(void)
{
    BOARD_DBGLPIOSET(1,0);
    while(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RF_OSC_READY) == 0U)
    {
        BOARD_DBGLPIOSET(1,1);
        PWR_NOP;
        BOARD_DBGLPIOSET(1,0);
    }
    BOARD_DBGLPIOSET(1,1);
}
#endif

/*---------------------------------------------------------------------------
 * Name: PWR_HandleDeepSleepMode
 * Description: - Generic Lowpower handler to LLS disregarding the GenFSK or Link layer state
 *      The function will overide the stop acknowledge from the radio
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_HandleDeepSleepMode(void)
{
#if (cPWR_UsePowerDownMode)
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep = 0U;
#endif /* (gTMR_EnableLowPowerTimers_d) */

    uint32_t lptmrFreq;

    BOARD_DBGLPIOSET(2,0);
    BOARD_DBGLPIOSET(2,1);
    BOARD_DBGLPIOSET(2,0);
    BOARD_DBGLPIOSET(2,1);

    PWR_DBG_LOG("");

#if defined(PWR_EnableRtosSysticks) && (PWR_EnableRtosSysticks == 1)
    /* Disable SysTick counter and interrupt */
    uint32_t sysTickCtrl = SysTick->CTRL & (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
#endif

    lptmrFreq = PWR_ProgramDeepSleepTime();

#if (gTMR_EnableLowPowerTimers_d)
    /* If more low power timers are running, stop the hardware timer
    * and save the spend time in ticks that wasn't counted.    */
    notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif /* (gTMR_EnableLowPowerTimers_d) */

    PWR_DBG_LOG("");
    DBG_LOG_DUMP(FALSE);

    BOARD_DBGLPIOSET(0,0);
    BOARD_EnterLowPowerCb();
    BOARD_DBGLPIOSET(0,1);

     /* set LLWU wakeup sources in addition to IO wakeup sources (set during Init) */
    LLWU->ME = gPWRLib_LLWU_WakeupModuleEnable_LPTMR_c;

    /* set RSIM for low power mode : Disregard the stop acknowledge from the radio */
    RSIM->CONTROL &= ~(RSIM_CONTROL_RF_OSC_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_EN_MASK);
    RSIM->CONTROL |= RSIM_CONTROL_RSIM_STOP_ACK_OVRD_EN_MASK | RSIM_CONTROL_RSIM_STOP_ACK_OVRD_MASK | \
                     RSIM_CONTROL_BLE_RF_OSC_REQ_INT_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_MASK;

    PWRLib_MCU_Enter_LLSx();

    /* reenable 32Mhz oscillator */
    RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 1U);

#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1) && \
    defined(gPWR_UsePswitchMode_d) && (gPWR_UsePswitchMode_d == 1)
    /* If RAM is saved in Flash, then PWR_HandleDeepSleepMode is used to LLS
        until 32KHz is ready for proper DCDC shutdown  */
    if ( gPWRLib_RamSavedStatus == PWRLIB_RAM_SAVED )
    {
        DCDC_ShutDown();
    }
#endif

    BOARD_DBGLPIOSET(0,0);
    BOARD_ExitLowPowerCb();
    BOARD_DBGLPIOSET(0,1);

    PWRLib_LLWU_UpdateWakeupReason();
    PWRLib_LPTMR_ClockStop();

    /* Sync. the low power timers */
#if (gTMR_EnableLowPowerTimers_d)
    PWR_SyncTimers(notCountedTicksBeforeSleep, lptmrFreq);
#else
    NOT_USED(lptmrFreq);
#endif /* (gTMR_EnableLowPowerTimers_d) */

#if defined(PWR_EnableRtosSysticks) && (PWR_EnableRtosSysticks == 1)
    SysTick->CTRL |= sysTickCtrl;
#endif

    PWR_ProcessWakeupReason();

    /* Make sure 32MHz osc is ready before going further.
       The oscillator could be needed right after then */
    PWR_WaitFor32MhzReady();

    BOARD_DBGLPIOSET(0,0);
    BOARD_DBGLPIOSET(2,1);
    BOARD_DBGLPIOSET(2,0);
    BOARD_DBGLPIOSET(2,1);

#endif   /* #if (cPWR_UsePowerDownMode) */
    return;
}


/*---------------------------------------------------------------------------
 * Name: && PWR_HandleDeepSleepMode_1_2_3
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_1)
static void PWR_HandleDeepSleepMode_1_2_3(void)
{
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep = 0U;
    uint32_t lptmrFreq;
#endif /* (gTMR_EnableLowPowerTimers_d) */

#if cPWR_BLE_LL_Enable
    bool_t   enterLowPower;
    bool_t   isLinkLayerAwake   = FALSE;
    uint32_t wakeup_offset      = cPWR_BLE_LL_OffsetToWakeupInstant;
    uint32_t osc_startup_delay  = cPWR_BLE_LL_OscStartupDelayLls;
    uint16_t nextInstant        = PWR_NEXT_INSTANT_INVALID;
    uint16_t offsetBeforeDsmExit;
#endif

    BOARD_DBGLPIOSET(2,0);

#ifdef BOARD_DBG_ENABLE
    static int count = DUMP_EVERY_X_TIMES;
    if (--count==0)
    {
        DBG_LOG_DUMP(FALSE);
        count = DUMP_EVERY_X_TIMES;
    }
#endif

    BOARD_DBGLPIOSET(2,1);
    BOARD_DBGLPIOSET(2,0);
    BOARD_DBGLPIOSET(2,1);
    BOARD_DBGLPIOSET(2,0);

#if cPWR_BLE_LL_Enable
    offsetBeforeDsmExit = OFFSET_BEFORE_DSM_EXIT( wakeup_offset, osc_startup_delay);

    enterLowPower = PWR_CheckBLE_LL_state(&nextInstant, &isLinkLayerAwake, offsetBeforeDsmExit);
    if(enterLowPower)
#endif
    {
        uint32_t rfOscEn, rsimCtrl;

#if cPWR_BLE_LL_Enable
        PWR_DBG_LOG("[%d] %d", mPWRLib_DeepSleepMode, nextInstant);

        /* For now isLinkLayerAwake shall always be set */
        assert(isLinkLayerAwake);

#if defined(cPWR_UseDefaultClockInLLS) && (cPWR_UseDefaultClockInLLS == 1)
        /* Restore default Core Clock
           Active consumption is reduced but active time is increased, leading
           to no gain in overall power consumption */
        BOARD_ResetCoreClock();
#endif

        /* if LL is awake, turn it into DSM mode */
        if ( isLinkLayerAwake == TRUE )
        {
            uint16_t clk_cfg = BLE_LL_CLOCK_CONFIG_LP_MOD_1;

            /* Optimization */
            if(PWRLib_AppState == PWR_APP_STATE_ADV)
            {
                /* shorten a bit the Osc startup time - the Timebase does not need to be so accurate in ADV */
                osc_startup_delay = cPWR_BLE_LL_OscStartupDelayLlsAdv;
            }

            /* set DSM configuration */
            BLE_LL_REG(WAKEUP_CONFIG) = ( WAKEUP_CFG_OSC_STARTUP_DELAY(wakeup_offset, osc_startup_delay) |
                                        BLE_LL_WAKEUP_CONFIG_CON_RAM_DSM_RET);

            if ( (mPWRLib_DeepSleepMode == PWR_LOWPOWER_LLS2_3_NO_DSM_MODE) )
            {
                clk_cfg = BLE_LL_CLOCK_CONFIG_LP_MOD_2_3;
                DBG_LOG_DUMP(FALSE);
            }
            BLE_LL_REG(CLOCK_CONFIG)  = clk_cfg;

            /* Set next instant */
            PWRLib_BLL_SetWakeupInstant(nextInstant);

            /* Time for LL to enter DSM now since all LL registers have been set up */
            PWRLib_BLL_EnterDSM();
        }
#else
        PWR_DBG_LOG("[%d]", mPWRLib_DeepSleepMode);
#endif

        BOARD_EnterLowPowerCb();

#if defined(PWR_EnableRtosSysticks) && (PWR_EnableRtosSysticks == 1)
        /* Disable SysTick counter and interrupt */
        uint32_t sysTickCtrl = SysTick->CTRL & (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
        SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
#endif

#if (gTMR_EnableLowPowerTimers_d)
        lptmrFreq = PWR_ProgramDeepSleepTime();
        /* If more low power timers are running, stop the hardware timer
        * and save the spend time in ticks that wasn't counted.
        */
        notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif /* (gTMR_EnableLowPowerTimers_d) */

        /* set LLWU wakeup sources in addition to IO wakeup sources (set during Init) */
        LLWU->ME = gPWRLib_LLWU_WakeupModuleEnable_Radio_c | gPWRLib_LLWU_WakeupModuleEnable_LPTMR_c;

        /* set RSIM for low power mode */
        rsimCtrl = RSIM->CONTROL;
        rfOscEn =  (rsimCtrl & RSIM_CONTROL_RF_OSC_EN_MASK) >> RSIM_CONTROL_RF_OSC_EN_SHIFT;
        rsimCtrl &= ~(RSIM_CONTROL_RSIM_STOP_ACK_OVRD_EN_MASK | RSIM_CONTROL_RF_OSC_EN_MASK);
        RSIM->CONTROL = rsimCtrl | RSIM_CONTROL_BLE_RF_OSC_REQ_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_MASK;

        PWRLib_MCU_Enter_LLSx();

        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);

#if cPWR_BLE_LL_Enable
        /* check if BLE LL exits DSM - Otherwise , force a wakeup now, this will take a while */
        if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_INT) == 0U)
        {
            BOARD_DBGLPIOSET(5,0);
            RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT, 1U);
            BOARD_DBGLPIOSET(5,1);
        }
#endif

        BOARD_DBGLPIOSET(3,0);
        BOARD_ExitLowPowerCb();
        BOARD_DBGLPIOSET(3,1);

        PWRLib_LLWU_UpdateWakeupReason();

        /* Sync. the low power timers */
#if (gTMR_EnableLowPowerTimers_d)
        PWRLib_LPTMR_ClockStop();
        PWR_SyncTimers(notCountedTicksBeforeSleep, lptmrFreq);
#endif /* (gTMR_EnableLowPowerTimers_d) */

        PWR_WaitFor32MhzReady();

        PWRLib_BLPItoBPE();

#if cPWR_BLE_LL_Enable
        BOARD_DBGLPIOSET(4,0);
        BOARD_DBGLPIOSET(0,0);
        PWRLib_BLL_WaitForDSMExit();
        BOARD_DBGLPIOSET(0,1);

        PWRLib_ClearRSIMInt();

        /* reset DSM_EXIT flag */
        //RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT, 0U);

        /* Reset LL DSM timer */
        RSIM->DSM_CONTROL |= RSIM_DSM_CONTROL_DSM_TIMER_CLR_MASK;
        BOARD_DBGLPIOSET(4,1);
#endif

#if defined(PWR_EnableRtosSysticks) && (PWR_EnableRtosSysticks == 1)
        SysTick->CTRL |= sysTickCtrl;
#endif

        PWR_ProcessWakeupReason();

        /* can be used to measure vbat at wake up */
        DCDC_ExitLowPowerPostCb();

        PWRLib_BPEtoBLPE();
    }

    BOARD_DBGLPIOSET(0,0);
    BOARD_DBGLPIOSET(2,1);
    BOARD_DBGLPIOSET(2,0);
    BOARD_DBGLPIOSET(2,1);

}
#endif /* (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_1) */

/*---------------------------------------------------------------------------
 * Name: PWR_HandleDeepSleepMode_4
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_4)
static void PWR_HandleDeepSleepMode_4(void)
{
    /* Make sure NVM module has completed all pending operations before
           shutting down the RAM
        Use the Unsafe function as Interrupt already masked (requires with RTOS
            in tickless mode) */
    NvCompletePendingOperationsUnsafe();

#if defined(gPWR_UsePswitchMode_d) && (gPWR_UsePswitchMode_d == 1)
    PWR_DBG_LOG("");
    DBG_LOG_DUMP(FALSE);

    /* Switch OFF DCDC immediatly */
    DCDC_ShutDown();
#else
    uint8_t  mode;

#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1)
    uint16_t bleEnabledInt;
#endif

    PWR_DBG_LOG("");
    DBG_LOG_DUMP(FALSE);

    BOARD_EnterLowPowerCb();

#if (gTMR_EnableLowPowerTimers_d)

    if(gTmrInvalidSleepTime == TMR_GetFirstExpireTime(gTmrLowPowerTimer_c))
    {
        /* If no LPTMR are running we can disable 32k osc to save power */
        LPTMR_Deinit(LPTMR0);
        BOARD_RTC_Deinit();
    }
    else
    {
        /* if a lowpower timer is active, the device will wake up from RAMOFF after first timer expiration */
        (void)PWR_ProgramDeepSleepTime();
    }
#else
    /* If LPTMR are not enabled, we can always shutdown the 32khz osc */
    BOARD_RTC_Deinit();
#endif /* (gTMR_EnableLowPowerTimers_d) */

#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1)
    bleEnabledInt = PWRLib_BLL_GetIntEn();
    PWRLib_BLL_ClearInterrupts(bleEnabledInt);
    PWRLib_BLL_DisableInterrupts(bleEnabledInt);
#endif

    /* RF DeInit */
    XCVR_Deinit();

    /* set LLWU wakeup sources */
    LLWU->ME = (gPWRLib_LLWU_WakeupModuleEnable_DCDC_c  | gPWRLib_LLWU_WakeupModuleEnable_LPTMR_c);

    /* set RSIM for low power mode */
    RSIM->CONTROL &= ~( RSIM_CONTROL_BLE_RF_OSC_REQ_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_EN_MASK |
                       RSIM_CONTROL_RF_OSC_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_MASK);
    RSIM->CONTROL |= RSIM_CONTROL_RSIM_STOP_ACK_OVRD_EN_MASK | RSIM_CONTROL_RSIM_STOP_ACK_OVRD_MASK;

#if (APP_DCDC_MODE == gDCDC_Bypass_c)
    mode = PWRLIB_VLLS_0;
#else
    mode = PWRLIB_VLLS_1;
#endif

    PWRLib_MCU_Enter_VLLSx(mode);

    PWR_DBG_LOG("Incorrect entering in Lowpower: %d", mode);
    DBG_LOG_DUMP(TRUE);
    assert(0);
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
    uint32_t rfOscEn, rsimCtrl;
#if defined(PWR_EnableRtosSysticks) && (PWR_EnableRtosSysticks == 1) && (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep = 0U;
    uint32_t lptmrFreq;
#endif /* (gTMR_EnableLowPowerTimers_d) */

    BOARD_DBGLPIOSET(3,0);

#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1)
    PWRLib_BLL_EnableInterrupts(BLE_LL_EVENT_ENABLE_LLH_IDLE_INT_MASK);
#endif

#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1)
    BLE_LL_REG(CLOCK_CONFIG) = BLE_LL_CLOCK_CONFIG_LP_MOD_6;
#endif

#if defined(PWR_EnableRtosSysticks) && (PWR_EnableRtosSysticks == 1)
    /* Disable SysTick counter and interrupt */
    uint32_t sysTickCtrl = SysTick->CTRL & (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);

    /* In stop mode, Lowpower timers are only required when PWR_EnableRtosSysticks is set
          in order to count the time spent in stop mode and compensate the tick counts on wakeup   */
#if (gTMR_EnableLowPowerTimers_d)
    lptmrFreq = PWR_ProgramDeepSleepTime();
    /* If more low power timers are running, stop the hardware timer
    * and save the spend time in ticks that wasn't counted.    */
    notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif /* (gTMR_EnableLowPowerTimers_d) */
#endif

    /* set LLWU wakeup sources */
    LLWU->ME = 0;

    /* set RSIM for low power mode */
    rsimCtrl = RSIM->CONTROL;
    rfOscEn =  (rsimCtrl & RSIM_CONTROL_RF_OSC_EN_MASK) >> RSIM_CONTROL_RF_OSC_EN_SHIFT;
    rsimCtrl = (((RSIM->CONTROL) & (~(RSIM_CONTROL_RF_OSC_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_MASK))) | RSIM_CONTROL_RF_OSC_EN(0x3));
    RSIM->CONTROL = rsimCtrl | RSIM_CONTROL_BLE_RF_OSC_REQ_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_EN_MASK | RSIM_CONTROL_RSIM_STOP_ACK_OVRD_EN_MASK | RSIM_CONTROL_RSIM_STOP_ACK_OVRD_MASK;

    SIM->FCFG1 |= SIM_FCFG1_FLASHDOZE_MASK;

    BOARD_DBGLPIOSET(3,1);
    PWRLib_MCU_Enter_Stop();
    BOARD_DBGLPIOSET(3,0);

    SIM->FCFG1 &= ~SIM_FCFG1_FLASHDOZE_MASK;
    RSIM->CONTROL &= ~RSIM_CONTROL_RSIM_STOP_ACK_OVRD_EN_MASK;

    PWR_WaitFor32MhzReady();

    RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);

#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1)
    /* check if STOP mode exit was on LL interrupt */
    if( (BLE_LL_REG_EVENT_STATUS & BLE_LL_EVENT_ENABLE_LLH_IDLE_INT_MASK) != 0U )
    {
        PWRLib_BLL_ClearInterrupts(BLE_LL_EVENT_ENABLE_LLH_IDLE_INT_MASK);
    }
#endif

#if defined(PWR_EnableRtosSysticks) && (PWR_EnableRtosSysticks == 1)
    /* Restore the state of SysTick */
    SysTick->CTRL |= sysTickCtrl;

    /* Sync. the low power timers */
#if (gTMR_EnableLowPowerTimers_d)
    PWRLib_LPTMR_ClockStop();
    PWR_SyncTimers(notCountedTicksBeforeSleep, lptmrFreq);
#endif /* (gTMR_EnableLowPowerTimers_d) */
#endif

#if cPWR_BLE_LL_Enable
    PWRLib_BLL_DisableInterrupts(BLE_LL_EVENT_ENABLE_LLH_IDLE_INT_MASK);
#endif

    BOARD_DBGLPIOSET(3,1);
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
    uint32_t lptmrFreq;
    bool_t enterLowPower = TRUE;

    BOARD_DBGLPIOSET(4,0);

#ifdef BOARD_DBG_ENABLE
    static int count = DUMP_EVERY_X_TIMES;
    if (--count==0)
    {
        DBG_LOG_DUMP(FALSE);
        count = DUMP_EVERY_X_TIMES;
    }
#endif

    BOARD_DBGLPIOSET(4,1);
    BOARD_DBGLPIOSET(4,0);

    lptmrFreq = PWR_ProgramDeepSleepTime();

#if (gTMR_EnableLowPowerTimers_d)
    /* If more low power timers are running, stop the hardware timer
     * and save the spend time in ticks that wasn't counted.
     */
    notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif

    if(enterLowPower && (READ_REGISTER_FIELD(RSIM->DSM_CONTROL,RSIM_DSM_CONTROL_GEN_SLEEP_REQUEST) == 0U))
    {
        enterLowPower &= PWR_HandleGenfskDsmEnter();
    }

    PWR_DBG_LOG("%d", enterLowPower);

    if(enterLowPower)
    {
        uint32_t rfOscEn, rsimCtrl;

        /* set DSM configuration */
        BLE_LL_REG(WAKEUP_CONFIG) = (((cPWR_BLE_LL_OffsetToWakeupInstant << BLE_LL_WAKEUP_CONFIG_DSM_OFFSET_SHIFT) & BLE_LL_WAKEUP_CONFIG_DSM_OFFSET_MASK) |
                                    ((cPWR_BLE_LL_OscStartupDelayLls << BLE_LL_WAKEUP_CONFIG_OSC_DELAY_SHIFT)& BLE_LL_WAKEUP_CONFIG_OSC_DELAY_MASK) |
                                    BLE_LL_WAKEUP_CONFIG_CON_RAM_DSM_RET);
        BLE_LL_REG(CLOCK_CONFIG) = BLE_LL_CLOCK_CONFIG_LP_MOD_7;

        BOARD_EnterLowPowerCb();

        /* set LLWU wakeup sources */
        LLWU->ME = gPWRLib_LLWU_WakeupModuleEnable_Radio_c | gPWRLib_LLWU_WakeupModuleEnable_LPTMR_c;

        /* set RSIM for low power mode */
        rsimCtrl = RSIM->CONTROL;
        rfOscEn =  (rsimCtrl & RSIM_CONTROL_RF_OSC_EN_MASK) >> RSIM_CONTROL_RF_OSC_EN_SHIFT;
        rsimCtrl &= ~(RSIM_CONTROL_RF_OSC_EN_MASK | RSIM_CONTROL_RSIM_STOP_ACK_OVRD_EN_MASK);
        RSIM->CONTROL = rsimCtrl | RSIM_CONTROL_BLE_RF_OSC_REQ_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_MASK ;

        PWRLib_MCU_Enter_LLSx();

        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, rfOscEn);

        PWR_WaitFor32MhzReady();

        BOARD_ExitLowPowerCb();

        PWRLib_LLWU_UpdateWakeupReason();

        PWR_HandleGenfskDsmExit();

        PWR_ProcessWakeupReason();
    }

    PWRLib_LPTMR_ClockStop();

    /* Sync. the low power timers */
#if (gTMR_EnableLowPowerTimers_d)
    PWR_SyncTimers(notCountedTicksBeforeSleep, lptmrFreq);
#endif

    BOARD_DBGLPIOSET(4,1);

#else
    PWRLib_MCU_WakeupReason.AllBits = 0;
#endif
}
#endif


/*---------------------------------------------------------------------------
 * Name: PWR_HandleDeepSleepMode_5_8
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode && (cPWR_EnableDeepSleepMode_8 || cPWR_EnableDeepSleepMode_5))
static void PWR_HandleDeepSleepMode_5_8(void)
{
#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1)
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep = 0U;
    uint32_t lptmrFreq;
#endif /* (gTMR_EnableLowPowerTimers_d) */

    bool_t   enterLowPower;
    bool_t   isLinkLayerAwake   = FALSE;
    uint16_t wakeup_offset      = cPWR_BLE_LL_OffsetToWakeupInstant + 1U;   /* wakeup 1 slot in advance to restore LLH */
    uint16_t osc_startup_delay  = cPWR_BLE_LL_OscStartupDelayVlls;
    uint16_t nextInstant        = PWR_NEXT_INSTANT_INVALID;
    uint16_t offsetBeforeDsmExit;

    BOARD_DBGLPIOSET(2,0);

    offsetBeforeDsmExit = OFFSET_BEFORE_DSM_EXIT( wakeup_offset, osc_startup_delay);

#ifdef BOARD_DBG_ENABLE
    static int count = DUMP_EVERY_X_TIMES;
    if (--count==0)
    {
        DBG_LOG_DUMP(FALSE);
        count = DUMP_EVERY_X_TIMES;
    }
#endif

    BOARD_DBGLPIOSET(2,1);
    BOARD_DBGLPIOSET(2,0);

    /* Clear LLWU events right now so we can catch new events later on after interrupts are discarded */
    PWRLib_LLWU_ClearWakeUpSources();

    enterLowPower = PWR_CheckBLE_LL_state(&nextInstant, &isLinkLayerAwake, offsetBeforeDsmExit);

    if(enterLowPower)
    {
        uint32_t rsimCtrl;

        /* For now, isLinkLayerAwake shall always be set */
        assert(isLinkLayerAwake);

        //PWR_DBG_LOG("[%d] %d", mPWRLib_DeepSleepMode, nextInstant);

        /* if LL is awake, turn it into DSM mode */
        if ( isLinkLayerAwake == TRUE )
        {
            uint16_t clk_cfg = BLE_LL_CLOCK_CONFIG_LP_MOD_8;

            /* Optimization */
            if(PWRLib_AppState == PWR_APP_STATE_ADV)
            {
                /* shorten a bit the Osc startup time - the Timebase does not need to be so accurate in ADV */
                osc_startup_delay = cPWR_BLE_LL_OscStartupDelayVllsAdv;
            }

            /* set DSM configuration */
            BLE_LL_REG(WAKEUP_CONFIG) = ( WAKEUP_CFG_OSC_STARTUP_DELAY(wakeup_offset, osc_startup_delay) |
                                            BLE_LL_WAKEUP_CONFIG_CON_RAM_DSM_RET);

            if (mPWRLib_DeepSleepMode == PWR_LOWPOWER_VLLS2_3_NO_DSM_MODE)
            {
                clk_cfg = BLE_LL_CLOCK_CONFIG_LP_MOD_5;
                DBG_LOG_DUMP(FALSE);
            }
            BLE_LL_REG(CLOCK_CONFIG)  = clk_cfg;

            /* Set next instant */
            PWRLib_BLL_SetWakeupInstant(nextInstant);

            /* Time for LL to enter DSM now since all LL registers have been set up */
            PWRLib_BLL_EnterDSM();
        }

        BOARD_EnterLowPowerCb();

#if defined(PWR_EnableRtosSysticks) && (PWR_EnableRtosSysticks == 1)
        /* Disable SysTick counter and interrupt */
        uint32_t sysTickCtrl = SysTick->CTRL & ( SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
        SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
#endif

#if (gTMR_EnableLowPowerTimers_d)
        lptmrFreq = PWR_ProgramDeepSleepTime();

        /* If more low power timers are running, stop the hardware timer
        * and save the spend time in ticks that wasn't counted.        */
        notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif /* (gTMR_EnableLowPowerTimers_d) */

        /* set LLWU wakeup sources */
        LLWU->ME = gPWRLib_LLWU_WakeupModuleEnable_Radio_c | gPWRLib_LLWU_WakeupModuleEnable_LPTMR_c;

        /* set RSIM for low power mode */
        rsimCtrl = RSIM->CONTROL;
        rsimCtrl &= ~( RSIM_CONTROL_RF_OSC_EN_MASK | RSIM_CONTROL_RSIM_STOP_ACK_OVRD_EN_MASK);
        RSIM->CONTROL = rsimCtrl | RSIM_CONTROL_BLE_RF_OSC_REQ_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_MASK ;

        PWRLib_MCU_Enter_vlls2_3();

        BOARD_SetCoreClock48Mhz();

        /* check if BLE LL exits DSM - Otherwise , force a wakeup now, this will take a while */
        if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_INT) == 0U)
        {
            BOARD_DBGLPIOSET(5,0);
            RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT, 1U);
            BOARD_DBGLPIOSET(5,1);
        }

        BOARD_DBGLPIOSET(3,0);
        PWRLib_LLWU_UpdateWakeupReason();

        BOARD_ExitLowPowerVllsCb();
        BOARD_ExitLowPowerCb();
        BOARD_DBGLPIOSET(3,1);

        BOARD_DBGLPIOSET(3,0);
#if (gTMR_Enabled_d)
        /* Initialize the timer */
        TMR_ExitLowPower();
#endif

#if (gTMR_EnableLowPowerTimers_d)
        PWRLib_LPTMR_ClockStop();

        /* Sync. the low power timers - this makes the Timer task in ready state*/
        PWR_SyncTimers(notCountedTicksBeforeSleep, lptmrFreq);
#endif /* (gTMR_EnableLowPowerTimers_d) */
        BOARD_DBGLPIOSET(3,1);

        BOARD_DBGLPIOSET(3,0);
        /* can be used to measure vbat at wake up */
        DCDC_ExitLowPowerPostCb();
        BOARD_DBGLPIOSET(3,1);

        /* there is no need to wait here, the Radio will wait for 32Mhz during reinit */
        //PWR_WaitFor32MhzReady();

        /* Init RF */
        BOARD_DBGLPIOSET(5,0);
        Controller_RadioInit();
        BOARD_DBGLPIOSET(5,1);


        BOARD_DBGLPIOSET(4,0);
        PWRLib_BLL_WaitForDSMExit();
        BOARD_DBGLPIOSET(4,1);

        /* Restore BLE LL context */
        BOARD_DBGLPIOSET(4,0);
        Controller_RestoreLlhState();
        Controller_RestoreLlhStateComplete();

        PWRLib_ClearRSIMInt();

        /* Reset LL DSM timer */
        RSIM->DSM_CONTROL |= RSIM_DSM_CONTROL_DSM_TIMER_CLR_MASK;;

        /* reset DSM_EXIT flag - DSM exit interrupt is non interruptable */
        //RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT, 0);
        BOARD_DBGLPIOSET(4,1);

        PWRLib_BLPItoBPE();

#if defined(PWR_EnableRtosSysticks) && (PWR_EnableRtosSysticks == 1)
        SysTick->CTRL |= sysTickCtrl;
#endif

        BOARD_DBGLPIOSET(3,0);
        BOARD_ExitLowPowerVllsPostCb();
        BOARD_DBGLPIOSET(3,1);

        PWRLib_BPEtoBLPE();

        PWR_ProcessWakeupReason();
    }

    BOARD_DBGLPIOSET(0,0);
    BOARD_DBGLPIOSET(2,1);
#else
    PWRLib_MCU_WakeupReason.AllBits = 0;
#endif /* cPWR_BLE_LL_Enable */
}
#endif /* (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_8) */

/*---------------------------------------------------------------------------
 * Name: PWR_HandleDeepSleepMode_9
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_9)
static void PWR_HandleDeepSleepMode_9(void)
{
#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1)
#if (gTMR_EnableLowPowerTimers_d)
    uint32_t notCountedTicksBeforeSleep = 0U;
    uint32_t lptmrFreq;
#endif /* (gTMR_EnableLowPowerTimers_d) */

    bool_t   enterLowPower;
    bool_t   isLinkLayerAwake   = FALSE;
    uint16_t wakeup_offset      = cPWR_BLE_LL_OffsetToWakeupInstant + 1U;   /* wakeup 1 slot in advance to restore LLH */
    uint16_t osc_startup_delay  = 0x1u;
    uint16_t nextInstant        = PWR_NEXT_INSTANT_INVALID;
    uint16_t offsetBeforeDsmExit;

    BOARD_DBGLPIOSET(2,0);

    /* mode 9 can only be used in ADV */
    assert(PWRLib_AppState == PWR_APP_STATE_ADV);

    offsetBeforeDsmExit = OFFSET_BEFORE_DSM_EXIT( wakeup_offset, osc_startup_delay);

#ifdef BOARD_DBG_ENABLE
    static int count = DUMP_EVERY_X_TIMES;
    if (--count==0)
    {
        DBG_LOG_DUMP(FALSE);
        count = DUMP_EVERY_X_TIMES;
    }
#endif

    BOARD_DBGLPIOSET(2,1);
    BOARD_DBGLPIOSET(2,0);

    enterLowPower = PWR_CheckBLE_LL_state(&nextInstant, &isLinkLayerAwake, offsetBeforeDsmExit);

    if(enterLowPower)
    {
#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1) && \
    defined(gPWR_UsePswitchMode_d) && (gPWR_UsePswitchMode_d == 1)
        if(gPWRLib_RamSavedStatus == PWRLIB_RAM_SAVED)
        {
            mPWR_DeepSleepTimeMs = 10U;
            PWR_HandleDeepSleepMode();
        }
#endif

        uint32_t rsimCtrl;

        /* For now, isLinkLayerAwake shall always be set */
        assert(isLinkLayerAwake);

        //PWR_DBG_LOG("Sleep [%d] next=%d cur=%d", mPWRLib_DeepSleepMode, nextInstant, BLE_LL_REG(TIM_COUNTER_L));

        /* if LL is awake, turn it into DSM mode */
        if ( isLinkLayerAwake == TRUE )
        {
            /* set DSM configuration - No osc startup delay */
            BLE_LL_REG(WAKEUP_CONFIG) = ( WAKEUP_CFG_OSC_STARTUP_DELAY(0, osc_startup_delay) );

            /* No wakeup from LL - same as mode 5 */
            BLE_LL_REG(CLOCK_CONFIG)  = BLE_LL_CLOCK_CONFIG_LP_MOD_5;

            /* Time for LL to enter DSM now since all LL registers have been set up */
            PWRLib_BLL_EnterDSM();
        }
#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
        /* Make sure NVM module has completed all pending operations before
           shutting down the RAM */
        NvCompletePendingOperations();
        /* in case of RAM OFF, we need to dump the trace immediatly */
        DBG_LOG_DUMP(FALSE);
#endif

        BOARD_EnterLowPowerCb();

#if defined(PWR_EnableRtosSysticks) && (PWR_EnableRtosSysticks == 1)
        /* Disable SysTick counter and interrupt */
        uint32_t sysTickCtrl = SysTick->CTRL & ( SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
        SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
#endif

#if (gTMR_EnableLowPowerTimers_d)
        lptmrFreq = PWR_ProgramDeepSleepTime();

        /* If more low power timers are running, stop the hardware timer
        * and save the spend time in ticks that wasn't counted.        */
        notCountedTicksBeforeSleep = TMR_NotCountedTicksBeforeSleep();
#endif /* (gTMR_EnableLowPowerTimers_d) */

        /* set LLWU wakeup sources */
        LLWU->ME = gPWRLib_LLWU_WakeupModuleEnable_Radio_c | gPWRLib_LLWU_WakeupModuleEnable_LPTMR_c;

        /* set RSIM for low power mode */
        rsimCtrl = RSIM->CONTROL;
        rsimCtrl &= ~( RSIM_CONTROL_RF_OSC_EN_MASK | RSIM_CONTROL_RSIM_STOP_ACK_OVRD_EN_MASK);
        RSIM->CONTROL = rsimCtrl | RSIM_CONTROL_BLE_RF_OSC_REQ_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_EN_MASK | RSIM_CONTROL_BLE_RF_OSC_REQ_INT_MASK ;

        PWRLib_MCU_Enter_vlls2_3();

        BOARD_DBGLPIOSET(3,0);
        BOARD_ExitLowPowerVllsCb();
        BOARD_ExitLowPowerCb();
        BOARD_DBGLPIOSET(3,1);

#if (gTMR_Enabled_d)
        /* Initialize the timer */
        TMR_ExitLowPower();
#endif

#if (gTMR_EnableLowPowerTimers_d)
        PWRLib_LPTMR_ClockStop();

        /* Sync. the low power timers - this makes the Timer task in ready state*/
        PWR_SyncTimers(notCountedTicksBeforeSleep, lptmrFreq);
#endif /* (gTMR_EnableLowPowerTimers_d) */

        BOARD_DBGLPIOSET(3,0);
        /* can be used to measure vbat at wake up */
        DCDC_ExitLowPowerPostCb();
        BOARD_DBGLPIOSET(3,1);

        /* Note : 32MHz oscillator already been started in warm main */
#if !defined(gPWR_RamOffDuringAdv_d) || (gPWR_RamOffDuringAdv_d == 0)
        /* can't do a lot, wait now for 32MHz osc to be ready before initializing the radio */
        PWR_WaitFor32MhzReady();

        /* Increase the core frequency to speed up the ADV timming as the FLL already ON */
        BOARD_SetCoreClock48Mhz();
#endif
        /* Init RF */
        BOARD_DBGLPIOSET(0,0);
        Controller_RadioInit();
        BOARD_DBGLPIOSET(0,1);

        PWR_BLE_LL_ResetRestartAdv();

        PWRLib_BLPItoBPE();

        PWRLib_LLWU_UpdateWakeupReason();

        PWRLib_ClearRSIMInt();

        /* reset DSM_EXIT flag */
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT, 0);

#if defined(PWR_EnableRtosSysticks) && (PWR_EnableRtosSysticks == 1)
        SysTick->CTRL |= sysTickCtrl;
#endif

        BOARD_DBGLPIOSET(3,0);
        BOARD_ExitLowPowerVllsPostCb();
        BOARD_DBGLPIOSET(3,1);

        PWRLib_BPEtoBLPE();

        PWR_ProcessWakeupReason();

#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
        BOARD_DBGLPIOSET(2,0);
        /* refresh NVM Ram values from Flash */
        NvModuleReInit();

#if defined(gPWR_RamOffRestoreBonding_d) && (gPWR_RamOffRestoreBonding_d == 1)
        /* restore database */
        PWR_RestoreBondingFromRamOff();
        BOARD_DBGLPIOSET(2,1);
#endif
#endif
    }

    BOARD_DBGLPIOSET(0,0);
    BOARD_DBGLPIOSET(2,1);
#else
    PWRLib_MCU_WakeupReason.AllBits = 0;
#endif /* cPWR_BLE_LL_Enable */
}
#endif /* (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_9) */

#if (cPWR_UsePowerDownMode)
#if (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_9)
#if defined(gPWR_RamOffDuringAdv_d)             && (gPWR_RamOffDuringAdv_d == 1)
#if defined(gPWR_RamOffRestoreBonding_d)        && (gPWR_RamOffRestoreBonding_d == 1)
static void PWR_RestoreBondingFromRamOff(void)
{
    //uint8_t bonded_device_nb;

    /* Refresh bonding and database from NVM */
extern bleResult_t DevDb_Init(void);
    DevDb_Init();

    //Gap_GetBondedDevicesCount(&bonded_device_nb);
    //PWR_DBG_LOG("%d bonded devices\n", bonded_device_nb);
}
#endif
#endif

/*---------------------------------------------------------------------------
 * Name: PWR_BLE_LL_ResetRestartAdv
 *---------------------------------------------------------------------------*/
static void PWR_BLE_LL_ResetRestartAdv(void)
{
    /* No need to wakeup the LLH, reset it to save time */
    BOARD_DBGLPIOSET(1,0);
    Controller_ResetLlhState();
    BOARD_DBGLPIOSET(1,1);

    /* Restore BLE LL context */
    BOARD_DBGLPIOSET(0,0);
    Controller_RestoreLlhState();
    BOARD_DBGLPIOSET(0,1);

    /* program ADV for next slot */
    Controller_UpdateLlhAdvInstant(1u);

    /* Complete the LLH restore and update */
    Controller_RestoreLlhStateComplete();
}
#endif /* (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_9) */
#endif /* #if (cPWR_UsePowerDownMode) */

/*---------------------------------------------------------------------------
 * Name: PWR_ProgramDeepSleepTime
 * Description: Program the next DeepSleep wakaup time
 * Parameters: -
 * Return: - LPTMR frequency in Hz
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode)
static uint32_t PWR_ProgramDeepSleepTime(void)
{
    uint8_t  clkMode;
    uint32_t lptmrTicks;
    uint32_t lptmrFreq;
    uint32_t sleepTimeMs;

    /* Get the expired time of the first programmed Low Power Timer */
    sleepTimeMs = TMR_GetFirstExpireTime(gTmrLowPowerTimer_c);

    /* check if it overtakes the maximum allowed deep sleep time */
    if(mPWR_DeepSleepTimeMs < sleepTimeMs)
    {
        sleepTimeMs = mPWR_DeepSleepTimeMs;
    }

    PWRLib_LPTMR_GetTimeSettings(sleepTimeMs ,&clkMode ,&lptmrTicks, &lptmrFreq);
    PWRLib_LPTMR_ClockStart(clkMode,lptmrTicks);

    //PWR_DBG_LOG("%u", sleepTimeMs);

    return lptmrFreq;
}
#endif

/*---------------------------------------------------------------------------
 * Name: PWR_SyncTimers
 * Description: Synchronize the framework timers with the amount of time in LP
 * Parameters: - notCountedTicksBeforeSleep:
 *             - lptmrFreq:
 * Return: -
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode) && (gTMR_EnableLowPowerTimers_d)
static void PWR_SyncTimers(uint32_t notCountedTicksBeforeSleep, uint32_t lptmrFreq)
{
    uint32_t timerTicks;
    uint64_t timerTicks_new;
    uint64_t mLpmTotalSleepDuration_new;

    /* Update mLpmTotalSleepDuration for later usage by PWR_GetTotalSleepDurationMS() */
    //PWR_DBG_LOG("notCountedTicksBeforeSleep=%u lptmrFreq=%u", notCountedTicksBeforeSleep, lptmrFreq);

    timerTicks = PWRLib_LPTMR_ClockCheck();

    mLpmTotalSleepDuration_new = ((uint64_t)timerTicks) * 1000000U / lptmrFreq;
    mLpmTotalSleepDuration      += mLpmTotalSleepDuration_new;

    //PWR_DBG_LOG("mLpmTotalSleepDuration_new=%u", (uint32_t)mLpmTotalSleepDuration_new);
    //PWR_DBG_LOG("mLpmTotalSleepDuration=%u", (uint32_t)mLpmTotalSleepDuration);

    /* Calculate number of ticks for next wakeup */
    timerTicks_new = ((uint64_t)timerTicks * TMR_GetTimerFreq()) / lptmrFreq;
    //PWR_DBG_LOG("old=%d conv=%d", timerTicks, timerTicks_new);

#if !defined(PWRLib_LPTMR_KEEP_RUNNING)
    /* when lptmr keeps on running, there is no need to count missing ticks */
    timerTicks_new += notCountedTicksBeforeSleep;
#endif

    //PWR_DBG_LOG("timerTicks_new=%u ", timerTicks_new);

    TMR_SyncLpmTimers((uint32_t)timerTicks_new);
    TMR_MakeTMRThreadReady();
}
#endif

/*---------------------------------------------------------------------------
 * Name: PWR_CheckBLE_LL_state
 * Description: Checks whether the BLE link layer allows to enter low power state
 * Parameters: - nextInstant : time to next instant
 *             - requireDsmEntry: TRUE if manual LL DSM entry is required
 *             - offsetBeforeDsmExit
 * Return: - enterLowPower: TRUE if low power mode alowed
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode) && (cPWR_BLE_LL_Enable)
static bool_t PWR_CheckBLE_LL_state(uint16_t * nextInstant, bool_t * requireDsmEntry, uint16_t offsetBeforeDsmExit)
{
    bool_t enterLowPower = FALSE;
    *requireDsmEntry     = FALSE;

    if( RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT) != 0U) /* If false it means that BLE_LL is already in DSM */
    {
        if( (BLE_LL_REG(CLOCK_CONFIG) & BLE_LL_CLOCK_CONFIG_LLH_IDLE_MASK) != 0U)
        {
            uint16_t distanceToNextInstant;

            /* Get nextInstant and distanceToNextInstant from LL */
            distanceToNextInstant = Controller_GetNextInstance(nextInstant);
            //PWR_DBG_LOG("%d %d %d", distanceToNextInstant, *nextInstant, offsetBeforeDsmExit + 3);

            if(distanceToNextInstant > offsetBeforeDsmExit + 3U)
            {
                if(VECTOR_PENDING == 0U)
                {
                    /* can enter lowpower and require DSM exit - nextInstant is up to date */
                    *requireDsmEntry = TRUE;
                    enterLowPower   = TRUE;
                }
            }
        }
    }
    else
    {
        if (VECTOR_PENDING == 0U)
        {
            /* no interrupt pending, LL already in deep sleep, => can enter lowpower */
            enterLowPower        = TRUE;
        }
    }

    return enterLowPower;
}
#endif


/*---------------------------------------------------------------------------
 * Name: PWR_HandleDeepSleep
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode)
static void PWR_HandleDeepSleep(void)
{
    PWRLib_MCU_WakeupReason.AllBits = 0U;

    if(mPWRLib_DeepSleepMode != PWR_LOWPOWER_INVALID_MODE)
    {

#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1)
        uint8_t resLpMode = mPWRLib_DeepSleepMode;

        if ( ((PWRLib_AppState == PWR_APP_STATE_CONN) || (PWRLib_AppState == PWR_APP_STATE_INVALID))
                  && (mPWRLib_DeepSleepMode == PWR_LOWPOWER_VLLS2_3_LL_DSM3_MODE)  )
        {
            bool_t power_gating_allowed = Controller_IsPowerGatingAllowed();

            /* In VLLS connected mode, check if the LL allows to go to power gating mode */
            if ( power_gating_allowed == FALSE )
            {
                /* power gating disabled, => Switch to LLS */
                 mPWRLib_DeepSleepMode = PWR_LOWPOWER_LLS2_3_LL_DSM_MODE;
            }
        }
#endif

        assert( maHandleDeepSleep[mPWRLib_DeepSleepMode - 1U] != 0U);
        maHandleDeepSleep[mPWRLib_DeepSleepMode - 1U]();

#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1)
        /* restore to previous value if it was changed */
        mPWRLib_DeepSleepMode   = resLpMode;
#endif
    }
}
#endif

/*---------------------------------------------------------------------------
 * Name: PWR_HandleSleep
 *---------------------------------------------------------------------------*/
#if ( !defined(cPWR_EnableDeepSleepMode_6) || (cPWR_EnableDeepSleepMode_6 == 0) ) || \
    ( !defined(cPWR_UsePowerDownMode) || (cPWR_UsePowerDownMode == 0) )
static PWRLib_WakeupReason_t PWR_HandleSleep(void)
{
    PWRLib_WakeupReason_t  Res;

    Res.AllBits = 0U;
    PWRLib_MCU_WakeupReason.AllBits = 0U;

#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1) && \
    defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)

    /* We enable LL interrupts to leave sleep mode when LL idles so we can try to go
     * into deeper sleep mode */
    PWRLib_BLL_EnableInterrupts(BLE_LL_EVENT_ENABLE_LLH_IDLE_INT_MASK);
    BLE_LL_REG(CLOCK_CONFIG) = BLE_LL_CLOCK_CONFIG_DEFAULT;

#endif

    PWRLib_MCU_Enter_Sleep();

#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1) && \
    defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)

    /* check if Sleep mode exit was on LL interrupt */
    if(BLE_LL_REG_EVENT_STATUS & BLE_LL_EVENT_STATUS_LLH_IDLE_INT_MASK )
    {
        PWRLib_BLL_ClearInterrupts(BLE_LL_EVENT_ENABLE_LLH_IDLE_INT_MASK);
    }
    PWRLib_BLL_DisableInterrupts(BLE_LL_EVENT_ENABLE_LLH_IDLE_INT_MASK);

#endif

    Res.Bits.SleepTimeout = 1U;
    PWRLib_MCU_WakeupReason.Bits.SleepTimeout = 1U;

    return Res;
}
#endif

/*---------------------------------------------------------------------------
 * Name: PWR_DeepSleepAllowed
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode) && ( !defined(PWR_DisableDeepLowpower) || (PWR_DisableDeepLowpower == 0) )
static bool_t PWR_DeepSleepAllowed(void)
{
    bool_t state = TRUE;

    if(mLpmXcvrDisallowCnt != 0U)
    {
        state = FALSE;
    }

    return state;
}
#endif /* #if (cPWR_UsePowerDownMode)*/


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
#if defined(USE_RTOS) && (USE_RTOS)
#if !defined(PWR_EnableRtosSysticks) || (PWR_EnableRtosSysticks == 0)
    /* Disable systick */
    SysTick->CTRL = 0U;
#endif
#endif

#if cPWR_EnableDeepSleepMode_8 || cPWR_EnableDeepSleepMode_5 || cPWR_EnableDeepSleepMode_9
    warmbootConfig.doWarmbootSequence = WARMBOOT_SEQUENCE;
#else
    warmbootConfig.doWarmbootSequence = 0U;
#endif
    warmbootConfig.useWarmbootSp      = USE_WARMBOOT_SP;

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
    uint16_t resetStatus = 0U;

    resetStatus = (uint16_t)(RCM_REG->SRS0);
    resetStatus |= ((uint16_t)(RCM_REG->SRS1)) << 8;

    return resetStatus;
}

#if defined(PWR_FUNCTION_NOT_USED)
/*---------------------------------------------------------------------------
 * Name: PWR_SystemReset
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_SystemReset(void)
{
    SCB->AIRCR = (0x5FAUL << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk;
    while(0 == 0)
    {

    }
}
#endif

/*---------------------------------------------------------------------------
 * Name: PWR_SetDeepSleepTimeInMs
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_SetDeepSleepTimeInMs(uint32_t deepSleepTimeMs)
{
#if (cPWR_UsePowerDownMode) && (cPWR_BLE_LL_Enable)
    if(deepSleepTimeMs != 0U)
    {
        mPWR_DeepSleepTimeMs = deepSleepTimeMs;
    }
    else
    {
        (void)deepSleepTimeMs;
    }
#else
    (void)deepSleepTimeMs;
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
#if (cPWR_UsePowerDownMode) && (cPWR_BLE_LL_Enable)
    if(deepSleepTimeSymbols != 0U)
    {
        mPWR_DeepSleepTimeMs = (deepSleepTimeSymbols * 16U)/1000U;
    }
    else
    {
        (void)deepSleepTimeSymbols;
    }
#else
    (void)deepSleepTimeSymbols;
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

    if(mLPMFlag != 0U)
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
    OSA_InterruptDisable();

    mLPMFlag++;

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

    if(mLpmXcvrDisallowCnt != 0U)
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
    OSA_InterruptDisable();

    mLpmXcvrDisallowCnt++;

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

    returnValue = mLPMFlag == 0U ? TRUE : FALSE;

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
    assert( dsMode <= PWR_LOWPOWER_MODE_MAX );
    assert( dsMode );
    assert( maHandleDeepSleep[dsMode - 1] != (pfHandleDeepSleepFunc_t)0 );

    mPWRLib_DeepSleepMode = dsMode;
#endif /* (cPWR_UsePowerDownMode) */

    return TRUE;
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
    return mPWRLib_DeepSleepMode;
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
#if (cPWR_UsePowerDownMode && cPWR_EnableDeepSleepMode_4)
    OSA_InterruptDisable();

    /* Disable all wake up sources */
    PWRLib_LLWU_DisableAllWakeupSources();

    /* Configure MCU in VLLS0/1 mode */
    PWR_HandleDeepSleepMode_4();

    /* this function never returns */
#endif
}

/*---------------------------------------------------------------------------
 * Name: PWR_BLE_EnterDSM
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_BLE_EnterDSM(uint16_t wakeupInstant)
{
#if (cPWR_UsePowerDownMode) && (cPWR_BLE_LL_Enable)
    uint8_t dsMode;

    dsMode = mPWRLib_DeepSleepMode;

    if((dsMode == PWR_LOWPOWER_LLS2_3_LL_DSM_MODE) || (dsMode == 2U))
    {
        if( RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT) != 0U)
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
#if (cPWR_UsePowerDownMode) && (cPWR_BLE_LL_Enable)
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
    if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_STAT) == 0U)
    {
        RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT, 1U);
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
    ReturnValue.AllBits = 0;

#if (cPWR_UsePowerDownMode)

    BOARD_DBGLPIOSET(0,0);

    //DBG_LOG_DUMP(FALSE);

    OSA_InterruptDisable();

#if ((cPWR_LVD_Enable == 1) || (cPWR_LVD_Enable == 2))
    if(PWR_LVD_ReportLevel() == PWR_LEVEL_CRITICAL)
    {
        /* Voltage <= 1.8V so enter power-off state - to disable false Tx'ing(void)*/
        PWR_EnterPowerOff();
        /* if the above function returns, the function does nothing, and no lowpower was entered
           return 0 */
    }
#endif

#if !defined(PWR_DisableDeepLowpower) || (PWR_DisableDeepLowpower == 0)
    if ( TMR_AreAllTimersOff())  /*No timer running*/
    {
        if( PWR_DeepSleepAllowed())
        {
            PWR_HandleDeepSleep();
            ReturnValue = PWRLib_MCU_WakeupReason;
        }
    }
    else
    {
        PWR_DBG_LOG("Timer ON [%d]", mPWRLib_DeepSleepMode);
    }
#endif

    /* if no lowpower entered, go to Stop or Sleep mode */
    if ( ReturnValue.AllBits == 0U )
    {
#if (cPWR_EnableDeepSleepMode_6 == 1)
        uint8_t lpMode = mPWRLib_DeepSleepMode;
        mPWRLib_DeepSleepMode = 6U;
        PWR_HandleDeepSleep();
        mPWRLib_DeepSleepMode = lpMode;
#else
        ReturnValue = PWR_HandleSleep();
#endif
    }

    OSA_InterruptEnable();

    BOARD_DBGLPIOSET(0,1);

    return ReturnValue;
#else
    ReturnValue = PWR_HandleSleep();
    return ReturnValue;
#endif
}

/*---------------------------------------------------------------------------
 * Name: PWR_RegisterLowPowerEnterCallback
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_RegisterLowPowerEnterCallback(pfPWRCallBack_t lowPowerEnterCallback)
{
}

/*---------------------------------------------------------------------------
 * Name: PWR_RegisterLowPowerExitCallback
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_RegisterLowPowerExitCallback(pfPWRCallBack_t lowPowerExitCallback)
{
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
    time = (uint32_t) (mLpmTotalSleepDuration / 1000ULL);
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
 * Name: PWR_GetWakeupReason
 * Description: -
 * Parameters: -
 * Return: - Wakeup reason
 *---------------------------------------------------------------------------*/
PWRLib_WakeupReason_t PWR_GetWakeUpReason(void)
{
    return PWRLib_MCU_WakeupReason;
}

/*---------------------------------------------------------------------------
 * Name: PWR_GetAppState
 * Description: - Provide the application state
 * Parameters: -
 * Return: - application state
 *---------------------------------------------------------------------------*/
PWR_AppState PWR_GetAppState(void)
{
    return PWRLib_AppState;
}

/*---------------------------------------------------------------------------
 * Name: PWR_SetNewAppState
 * Description: - Provide the application lowpower mode so the lowpower module
 *                could match to the most appropriate lowpower mode
 * Parameters: -  app_state
 * Return: -
 *---------------------------------------------------------------------------*/
void PWR_SetNewAppState(PWR_AppState app_state)
{
#if (cPWR_UsePowerDownMode)
    uint8_t lpMode;

    if(app_state < PWR_APP_STATE_INVALID)
    {
        lpMode = pwr_app_state_to_lowpower_state[app_state];
        PWRLib_AppState = app_state;
        (void)PWR_ChangeDeepSleepMode(lpMode);
    }
    //PWR_DBG_LOG("%d", mPWRLib_DeepSleepMode);
#else
    (void)app_state;
#endif
}

/*---------------------------------------------------------------------------
 * Name: PWR_SetWakeupPins
 *---------------------------------------------------------------------------*/
void PWR_SetWakeupPins(uint32_t wkup_pin_set)
{
#if (cPWR_UsePowerDownMode)
    /* Set that the wakeup pins have been configured */
    PWRLib_wkup_pin_setup |= wkup_pin_set;

    PWRLib_SetWakeupPins(PWRLib_wkup_pin_setup);
#else
    (void)wkup_pin_set;
#endif
}

/*---------------------------------------------------------------------------
 * Name: PWR_UnSetWakeupPins
 *---------------------------------------------------------------------------*/
void PWR_UnSetWakeupPins(uint32_t wkup_pin_unset)
{
#if (cPWR_UsePowerDownMode)
    LLWU->PE1 &= ~( (uint8_t)(wkup_pin_unset    ) & 0xFFU);
    LLWU->PE2 &= ~( (uint8_t)(wkup_pin_unset>>8 ) & 0xFFU);
    LLWU->PE3 &= ~( (uint8_t)(wkup_pin_unset>>16) & 0xFFU);
    LLWU->PE4 &= ~( (uint8_t)(wkup_pin_unset>>24) & 0xFFU);

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
    for (uint32_t i = 0U; i < 32U; i += 2U)
    {
        /* need to shift the 16 wakepins 2bit field into a 16bit field to match the
           LLWU wakeup source detect register                          */
        if ( ((wkup_pin_unset >> i) & 0x3U) != 0U )
        {
            /* clear the corresponding bit in the bitmap */
            pwr_llwu_serial_bitmap &= ~((uint32_t)0x1U << (i>>1U));
        }
    }
#endif
#else
    (void)wkup_pin_unset;
#endif
}

/*---------------------------------------------------------------------------
 * Name: PWR_SetWakeupPinsSerial
 *---------------------------------------------------------------------------*/
void PWR_SetWakeupPinsSerial(uint32_t serial_wkup_pin_set, uint8_t serial_interface)
{
#if (cPWR_UsePowerDownMode) && defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
    assert(serial_interface < gSerialManagerMaxInterfaces_c);

    PWR_SetWakeupPins( serial_wkup_pin_set );

    pwr_serial_interface             = serial_interface;

    for (uint32_t i = 0U; i < 32U; i += 2U)
    {
        /* need to shift the 16 wakepins 2bit field into a 16bit field to match the
           LLWU wakeup source detect register                          */
        if ( ((serial_wkup_pin_set >> i) & 0x3U) != 0U )
        {
              pwr_llwu_serial_bitmap |=  (uint32_t)0x1U << (i>>1U);
        }
    }
#else
    (void)serial_wkup_pin_set;
    (void)serial_interface;
#endif
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


