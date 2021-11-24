/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief sleep mode implementation.
 */

#include <FreeRTOS.h>
#include "board_define.h"
#include "fsl_gpc.h"
#include "fsl_gpio.h"

#include "fwk_log.h"
#include "fwk_lpm_manager.h"
#include "hal_lpm_dev.h"

#include "board.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
void board_elcdif_rk024hh298_backlight_control(bool on);
#if defined(__cplusplus)
}
#endif /* __cplusplus */

#define SLN_WAKEUP_GPIO     BOARD_USER_BUTTON_GPIO
#define SLN_WAKEUP_GPIO_PIN BOARD_USER_BUTTON_GPIO_PIN
#define SLN_WAKEUP_GPIO_IRQ BOARD_USER_BUTTON_IRQ

static void _GPC_EnableWakeupSource(uint32_t irq)
{
    GPC_CM_EnableIrqWakeup(GPC_CPU_MODE_CTRL_0, irq, true);
}

static void _GPC_DisableWakeupSource(uint32_t irq)
{
    GPC_CM_EnableIrqWakeup(GPC_CPU_MODE_CTRL_0, irq, false);
}

static void _GPC_DisableAllWakeupSource(GPC_CPU_MODE_CTRL_Type *base)
{
    uint8_t i;

    for (i = 0; i < GPC_CPU_MODE_CTRL_CM_IRQ_WAKEUP_MASK_COUNT; i++)
    {
        base->CM_IRQ_WAKEUP_MASK[i] |= 0xFFFFFFFF;
    }
}

static void _SetWakeupConfig(void)
{
    gpio_pin_config_t wakeupPinConfig;

    memset(&wakeupPinConfig, 0, sizeof(wakeupPinConfig));

    wakeupPinConfig.direction     = kGPIO_DigitalInput;
    wakeupPinConfig.interruptMode = kGPIO_IntRisingEdge;
    GPIO_PinInit(SLN_WAKEUP_GPIO, SLN_WAKEUP_GPIO_PIN, &wakeupPinConfig);

    /* Clear wake port all pins interrupt flags */
    GPIO_ClearPinsInterruptFlags(SLN_WAKEUP_GPIO, ~0);

    /* Disable wake port all pins interrupt flags */
    GPIO_PortDisableInterrupts(SLN_WAKEUP_GPIO, ~0);

    /* Enable wake pin interrupt */
    GPIO_EnableInterrupts(SLN_WAKEUP_GPIO, 1U << SLN_WAKEUP_GPIO_PIN);

    /* Enable the interrupt */
    EnableIRQ(SLN_WAKEUP_GPIO_IRQ);

    /* Mask all interrupt first */
    _GPC_DisableAllWakeupSource(GPC_CPU_MODE_CTRL_0);

    /* Enable GPC interrupt */
    _GPC_EnableWakeupSource(SLN_WAKEUP_GPIO_IRQ);
}

static void _EnterSnvsSleepMode(void)
{
    /* PMIC_STBY_REQ = 0 => disable VDD_2V8 and VDD_1V2 */
    GPIO_PinWrite(GPIO13, 2, 0U);

    /* turn off LCD and Backlight */
    GPIO_PinWrite(BOARD_ELCDIF_DISPLAY_RESET_GPIO, BOARD_ELCDIF_DISPLAY_RESET_PIN, 0U);
    board_elcdif_rk024hh298_backlight_control(0);
    _SetWakeupConfig();
    SNVS->LPCR |= SNVS_LPCR_TOP_MASK;
}

static void HAL_LpmDev_TimerCallback(TimerHandle_t handle)
{
    if (handle == NULL)
    {
        LOGE("Lpm dev is NULL");
        return;
    }

    lpm_dev_t *pDev = (lpm_dev_t *)pvTimerGetTimerID(handle);
    if (pDev->callback != NULL)
    {
        pDev->callback(pDev);
    }
}

static void HAL_LpmDev_PreEnterSleepTimerCallback(TimerHandle_t handle)
{
    if (handle == NULL)
    {
        LOGE("Lpm dev is NULL");
        return;
    }

    lpm_dev_t *pDev = (lpm_dev_t *)pvTimerGetTimerID(handle);
    if (pDev->preEnterSleepCallback != NULL)
    {
        pDev->preEnterSleepCallback(pDev);
    }
}

hal_lpm_status_t HAL_LpmDev_Init(lpm_dev_t *dev,
                                lpm_manager_timer_callback_t callback,
                                lpm_manager_timer_callback_t preEnterSleepCallback)
{
    int ret = kStatus_HAL_LpmSuccess;

    dev->callback              = callback;
    dev->preEnterSleepCallback = preEnterSleepCallback;

    /* Disable wake pin interrupt */
    GPIO_DisableInterrupts(SLN_WAKEUP_GPIO, 1U << SLN_WAKEUP_GPIO_PIN);

    /* Clear wake pin interrupt flags */
    GPIO_ClearPinsInterruptFlags(SLN_WAKEUP_GPIO, 1U << SLN_WAKEUP_GPIO_PIN);

    dev->timer = xTimerCreate("LpmTimer", pdMS_TO_TICKS(1000), pdTRUE, (void *)dev, HAL_LpmDev_TimerCallback);
    if (dev->timer == NULL)
    {
        LOGE("Lpm Timer create failed");
        return kStatus_HAL_LpmTimerNull;
    }

    dev->preEnterSleepTimer = xTimerCreate("LpmPreEnterSleepTimer", pdMS_TO_TICKS(1500), pdTRUE, (void *)dev,
                                           HAL_LpmDev_PreEnterSleepTimerCallback);
    if (dev->preEnterSleepTimer == NULL)
    {
        LOGE("Lpm Pre-Enter Sleep Timer create failed");
        return kStatus_HAL_LpmTimerNull;
    }

    dev->lock = xSemaphoreCreateMutex();
    if (dev->lock == NULL)
    {
        LOGE("Create Lpm lock fail");
        return kStatus_HAL_LpmLockNull;
    }

    return ret;
}

hal_lpm_status_t HAL_LpmDev_Deinit(const lpm_dev_t *dev)
{
    int ret = kStatus_HAL_LpmSuccess;

    return ret;
}

hal_lpm_status_t HAL_LpmDev_OpenTimer(const lpm_dev_t *dev)
{
    int ret = kStatus_HAL_LpmSuccess;

    if (dev->timer == NULL)
    {
        LOGE("Lpm Timer is NULL");
        return kStatus_HAL_LpmTimerNull;
    }

    if (xTimerStart(dev->timer, 0) != pdPASS)
    {
        LOGE("Lpm Timer start fail");
        ret = kStatus_HAL_LpmTimerFail;
    }

    FWK_LpmManager_SetSleepMode(kLPMMode_SNVS);
    FWK_LpmManager_EnableSleepMode(kLPMManagerStatus_SleepDisable);

    return ret;
}

hal_lpm_status_t HAL_LpmDev_StopTimer(const lpm_dev_t *dev)
{
    int ret = kStatus_HAL_LpmSuccess;

    if (dev->timer == NULL)
    {
        LOGE("Lpm Timer is NULL");
        return kStatus_HAL_LpmTimerNull;
    }

    if (xTimerStop(dev->timer, 0) != pdPASS)
    {
        LOGD("Lpm Timer stop fail");
        ret = kStatus_HAL_LpmTimerFail;
    }

    return ret;
}

hal_lpm_status_t HAL_LpmDev_OpenPreEnterSleepTimer(const lpm_dev_t *dev)
{
    int ret = kStatus_HAL_LpmSuccess;

    if (dev->preEnterSleepTimer == NULL)
    {
        LOGE("Lpm Pre-Enter Sleep Timer is NULL");
        return kStatus_HAL_LpmTimerNull;
    }

    if (xTimerStart(dev->preEnterSleepTimer, 0) != pdPASS)
    {
        LOGE("Lpm Pre-Enter Sleep Timer start fail");
        ret = kStatus_HAL_LpmTimerFail;
    }

    return ret;
}

hal_lpm_status_t HAL_LpmDev_StopPreEnterSleepTimer(const lpm_dev_t *dev)
{
    int ret = kStatus_HAL_LpmSuccess;

    if (dev->preEnterSleepTimer == NULL)
    {
        LOGE("Lpm Pre-Enter Sleep Timer is NULL");
        return kStatus_HAL_LpmTimerNull;
    }

    if (xTimerStop(dev->preEnterSleepTimer, 0) != pdPASS)
    {
        LOGE("Lpm Pre-Enter Sleep Timer stop fail");
        ret = kStatus_HAL_LpmTimerFail;
    }

    return ret;
}

hal_lpm_status_t HAL_LpmDev_EnterSleep(const lpm_dev_t *dev, hal_lpm_mode_t mode)
{
    int ret = kStatus_HAL_LpmSuccess;
    LOGD("[HAL_LpmDev_EnterSleep] Mode %d", mode);
    switch (mode)
    {
        case kLPMMode_SNVS:
        {
            _EnterSnvsSleepMode();
        }
        break;

        default:
            break;
    }

    return ret;
}

hal_lpm_status_t HAL_LpmDev_Lock(const lpm_dev_t *dev)
{
    uint8_t fromISR = __get_IPSR();

    if (dev->lock == NULL)
    {
        LOGE("Lpm lock is null");
        return kStatus_HAL_LpmLockNull;
    }

    if (fromISR)
    {
        BaseType_t HigherPriorityTaskWoken = pdFALSE;
        if (xSemaphoreTakeFromISR(dev->lock, &HigherPriorityTaskWoken) != pdPASS)
        {
            LOGE("Lpm lock take error");
            return kStatus_HAL_LpmLockError;
        }
    }
    else
    {
        if (xSemaphoreTake(dev->lock, portMAX_DELAY) != pdPASS)
        {
            LOGE("Lpm lock take error");
            return kStatus_HAL_LpmLockError;
        }
    }

    return kStatus_HAL_LpmSuccess;
}

hal_lpm_status_t HAL_LpmDev_Unlock(const lpm_dev_t *dev)
{
    uint8_t fromISR = __get_IPSR();

    if (dev->lock == NULL)
    {
        LOGE("Lpm lock is null");
        return kStatus_HAL_LpmLockNull;
    }

    if (fromISR)
    {
        BaseType_t HigherPriorityTaskWoken = pdFALSE;
        if (xSemaphoreGiveFromISR(dev->lock, &HigherPriorityTaskWoken) != pdPASS)
        {
            LOGE("Lpm lock give error");
            return kStatus_HAL_LpmLockError;
        }
    }
    else
    {
        if (xSemaphoreGive(dev->lock) != pdPASS)
        {
            LOGE("Lpm lock give error");
            return kStatus_HAL_LpmLockError;
        }
    }

    return kStatus_HAL_LpmSuccess;
}

static lpm_dev_operator_t s_LpmDevOperators = {
    .init              = HAL_LpmDev_Init,
    .deinit            = HAL_LpmDev_Deinit,
    .openTimer         = HAL_LpmDev_OpenTimer,
    .stopTimer         = HAL_LpmDev_StopTimer,
    .openPreEnterTimer = HAL_LpmDev_OpenPreEnterSleepTimer,
    .stopPreEnterTimer = HAL_LpmDev_StopPreEnterSleepTimer,
    .enterSleep        = HAL_LpmDev_EnterSleep,
    .lock              = HAL_LpmDev_Lock,
    .unlock            = HAL_LpmDev_Unlock,
};

static lpm_dev_t s_LpmDev = {
    .id  = 0,
    .ops = &s_LpmDevOperators,
};

int HAL_LpmDev_Register()
{
    int ret = 0;

    FWK_LpmManager_DeviceRegister(&s_LpmDev);

    return ret;
}
