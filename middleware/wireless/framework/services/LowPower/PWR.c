/*
 * Copyright 2021-2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************************
 * Include
 ************************************************************************************/

#include "fsl_common.h"
#include "PWR_Interface.h"
#include "fwk_platform_lowpower.h"
#include "fwk_platform_lowpower_timer.h"
#include "fsl_pm_core.h"
#include "fwk_debug.h"

/************************************************************************************
 * Private type definitions and macros
 ************************************************************************************/

//#define PWR_DBG_DISABLE_LOWPOWER

/************************************************************************************
 * Private functions prototypes
 ************************************************************************************/

__WEAK uint32_t PWR_GetRadioNextEventUs(void);

/************************************************************************************
 * Private memory declarations
 ************************************************************************************/

static bool        initialized = false;
static pm_handle_t pm_hdl;
static uint8_t     lpDisallowCount = 0U;

/************************************************************************************
 * Private functions definitions
 ************************************************************************************/

/*!
 * \brief Callback registered to SDK Power Manager to get notified of entry/exit of low power modes
 *
 * \param[in] eventType event specifying if we entered or exited from low power mode
 * \param[in] powerState low power mode used during low power period
 * \param[in] data Optional data passed when the callback got registered (not used currently)
 * \return status_t
 */

static uint64_t PWR_GetMinValue(uint64_t osTime, uint64_t radioTime)
{
    uint64_t minTime;

    /* 0 is considered as no timeout, so this is not the minimal value */
    if (osTime == 0U)
    {
        /* if osTime argument is 0 it means infinite time so radioTime argument is not greater */
        minTime = radioTime;
    }
    else if (radioTime == 0U)
    {
        /* Likewise if radioTime argument is 0 it means infinite time so osTime argument is not greater */
        minTime = osTime;
    }
    else
    {
        /* delta between the 2 values in 64 bit arithmetic */
        minTime = MIN(osTime, radioTime);
    }
    return minTime;
}

/************************************************************************************
 * Public functions
 ************************************************************************************/
/* Weak version of the systick functions to have a definition even in baremetal, as they are declared in
 * PWR_Interface.h, those functions are not supposed to be called in baremetal*/
__attribute__((weak)) bool PWR_SysticksPreProcess(uint32_t xExpectedIdleTime, uint64_t *expectedIdleTimeUs)
{
    (void)xExpectedIdleTime;
    (void)expectedIdleTimeUs;
    assert(0);
    return false;
}
__attribute__((weak)) void PWR_SysticksPostProcess(uint64_t expectedIdleTimeUs, uint64_t actualIdleTimeUs)
{
    (void)expectedIdleTimeUs;
    (void)actualIdleTimeUs;
    assert(0);
}
void PWR_Init(void)
{
    // BOARD_DbgResetCheck();
    if (initialized == false)
    {
        PM_CreateHandle(&pm_hdl);
        PM_EnablePowerManager(true);

        /* Initialize platform specific ressources for low power support */
        PLATFORM_LowPowerInit();
        PLATFORM_InitWakeUpTimer();
        // BOARD_DbgCheckIrqPending(FALSE);

        initialized = true;
    }

    // PWR_DBG_LOG("<--", 0);
}

uint64_t PWR_EnterLowPower(uint64_t timeoutUs)
{
    uint64_t lowPowerDurationUs = 0U;
    uint32_t intMask            = DisableGlobalIRQ();

    // PWR_DBG_LOG("osTimeoutUs=%d %d", (uint32_t)(osTimeoutUs >> 32), (uint32_t)osTimeoutUs);

    /* identification for PWR_EnterLowPower() entry */
    BOARD_DBGLPIOSET(3, 1);
    BOARD_DBGLPIOSET(3, 0);
    BOARD_DBGLPIOSET(3, 1);

    if ((initialized == true) && (lpDisallowCount == 0U))
    {
        uint64_t lowPowerEntryTimestamp = 0U;
        uint64_t lowPowerExitTimestamp  = 0U;

        uint64_t minTimeoutUs;
        uint64_t nextRadioEvtUs;

        bool timerStarted = false;

        /* getting Next radio event is only usefull for the SDK power manager to estimate the best low power mode
            it can get into depending of the low power mode exit latency. if the next Radio event time is too short,
            a lighter low power mode will be chosen */
        nextRadioEvtUs = (uint64_t)PWR_GetRadioNextEventUs();

        /* Get Min value considering 0 is no timeout */
        minTimeoutUs = PWR_GetMinValue(timeoutUs, nextRadioEvtUs);

        if (minTimeoutUs == 0U)
        {
            /* no next event schedule */
            PWR_DBG_LOG("no next event schedule");
        }
        else if (minTimeoutUs == timeoutUs)
        {
            /* BLE Link layer has its own wakeup capability so start timer only if the next event if for os */
            PLATFORM_StartWakeUpTimer(timeoutUs);
            timerStarted = true;
            PWR_DBG_LOG("Timer started: %d ", (uint32_t)minTimeoutUs);
        }
        else
        {
            /* next event is Link layer, This last will wake up the device from BLE LL timer, not SOC timer */
            PWR_DBG_LOG("nxt evt is LL: %d ", (uint32_t)nextRadioEvtUs);
        }

        lowPowerEntryTimestamp = PLATFORM_GetLowPowerTimestampUs();

        BOARD_DBGLPIOSET(3, 0);
        BOARD_DBGLPIOSET(3, 1);

#ifdef PWR_DBG_DISABLE_LOWPOWER
        SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
        __WFI();
#else
        /* MinTimeoutUs is only given to estimate the best low power mode in case it is too short
            0 means no timeout*/
        PM_EnterLowPower(minTimeoutUs);
#endif
        BOARD_DBGLPIOSET(3, 0);
        BOARD_DBGLPIOSET(3, 1);

        if (timerStarted)
        {
            /* disable in case it was started for os event */
            PLATFORM_StopWakeUpTimer();
        }

        /* get time the device was in low power */
        lowPowerExitTimestamp = PLATFORM_GetLowPowerTimestampUs();
        lowPowerDurationUs    = PLATFORM_GetLowPowerDurationUs(lowPowerEntryTimestamp, lowPowerExitTimestamp);

        // PWR_DBG_LOG("%d %d %d", (uint32_t)lowPowerEntryTimestamp, (uint32_t)lowPowerExitTimestamp,
        // (uint32_t)lowPowerDurationUs);
    }

    BOARD_DBGLPIOSET(3, 0);

    EnableGlobalIRQ(intMask);

    return lowPowerDurationUs;
}

PWR_ReturnStatus_t PWR_SetLowPowerModeConstraint(PWR_LowpowerMode_t mode)
{
    PWR_ReturnStatus_t ret = PWR_Success;
    status_t           st  = kStatus_Success;

    // PWR_DBG_LOG("-->mode=%d, deepest lp mode=%d", mode, PM_GetAllowedLowestPowerMode());

    switch (mode)
    {
        case PWR_WFI:
            st = PM_SetConstraints(PLATFORM_WFI_STATE, PLATFORM_WFI_CONSTRAINTS);
            break;

        case PWR_Sleep:
            st = PM_SetConstraints(PLATFORM_SLEEP_STATE, PLATFORM_SLEEP_CONSTRAINTS);
            break;

        case PWR_DeepSleep:
            st = PM_SetConstraints(PLATFORM_DEEP_SLEEP_STATE, PLATFORM_DEEP_SLEEP_CONSTRAINTS);
            break;

        case PWR_PowerDown:
            st = PM_SetConstraints(PLATFORM_POWER_DOWN_STATE, PLATFORM_POWER_DOWN_CONSTRAINTS);
            break;

        case PWR_DeepPowerDown:
            st = PM_SetConstraints(PLATFORM_DEEP_POWER_DOWN_STATE, PLATFORM_DEEP_POWER_DOWN_CONSTRAINTS);
            break;

        default:
            ret = PWR_ErrorNotSupported;
            break;
    }

    if (st != kStatus_Success)
    {
        ret = PWR_Error;
    }

    // PWR_DBG_LOG("<--deepest lp mode=%d", PM_GetAllowedLowestPowerMode());

    return ret;
}

PWR_ReturnStatus_t PWR_ReleaseLowPowerModeConstraint(PWR_LowpowerMode_t mode)
{
    PWR_ReturnStatus_t ret = PWR_Success;
    status_t           st  = kStatus_Success;

    // PWR_DBG_LOG("-->mode=%d, deepest lp mode=%d", mode, PM_GetAllowedLowestPowerMode());

    switch (mode)
    {
        case PWR_WFI:
            st = PM_ReleaseConstraints(PLATFORM_WFI_STATE, PLATFORM_WFI_CONSTRAINTS);
            break;

        case PWR_Sleep:
            st = PM_ReleaseConstraints(PLATFORM_SLEEP_STATE, PLATFORM_SLEEP_CONSTRAINTS);
            break;

        case PWR_DeepSleep:
            st = PM_ReleaseConstraints(PLATFORM_DEEP_SLEEP_STATE, PLATFORM_DEEP_SLEEP_CONSTRAINTS);
            break;

        case PWR_PowerDown:
            st = PM_ReleaseConstraints(PLATFORM_POWER_DOWN_STATE, PLATFORM_POWER_DOWN_CONSTRAINTS);
            break;

        case PWR_DeepPowerDown:
            st = PM_ReleaseConstraints(PLATFORM_DEEP_POWER_DOWN_STATE, PLATFORM_DEEP_POWER_DOWN_CONSTRAINTS);
            break;

        default:
            ret = PWR_ErrorNotSupported;
            break;
    }

    if (st != kStatus_Success)
    {
        ret = PWR_Error;
    }

    // PWR_DBG_LOG("-->mode=%d, deepest lp mode=%d", mode, PM_GetAllowedLowestPowerMode());

    return ret;
}

void PWR_AllowDeviceToSleep(void)
{
    /* Is it not allowed to call this function without calling PWR_DisallowDeviceToSleep first */
    uint32_t intMask = DisableGlobalIRQ();
    if (lpDisallowCount > 0U)
    {
        lpDisallowCount--;
        EnableGlobalIRQ(intMask);
    }
    else
    {
        EnableGlobalIRQ(intMask);
        assert(0);
    }
}

void PWR_DisallowDeviceToSleep(void)
{
    /* Do not allow overflow because it would desync the counter with PWR_AllowDeviceToSleep */
    uint32_t intMask = DisableGlobalIRQ();
    if (lpDisallowCount != (uint8_t)UINT8_MAX)
    {
        lpDisallowCount++;
        EnableGlobalIRQ(intMask);
    }
    else
    {
        EnableGlobalIRQ(intMask);
        assert(0);
    }
}

uint8_t PWR_IsDeviceAllowedToSleep(void)
{
    return lpDisallowCount;
}

int32_t PWR_LowPowerEnterCritical(int32_t power_mode)
{
    // PWR_DBG_LOG("");
    return ((int32_t)PWR_SetLowPowerModeConstraint((PWR_LowpowerMode_t)power_mode));
}

int32_t PWR_LowPowerExitCritical(int32_t power_mode)
{
    // PWR_DBG_LOG("");
    return ((int32_t)PWR_ReleaseLowPowerModeConstraint((PWR_LowpowerMode_t)power_mode));
}

__WEAK uint32_t PWR_GetRadioNextEventUs(void)
{
    return 0u; // no next event
}

/*
 * WEAK version of the systick LP init function to have a definition, even if
 * OSA Timer base is not SysTick interrupt or FreeRTOS is used. This is needed
 * since it is declared in PWR_Interface.h
 */
__attribute__((weak)) void PWR_SysTicksLowPowerInit(void)
{
    assert(0);
}
