/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_common.h"
#include "fsl_power.h"
#include "pmic_support.h"
#include "power_demo_config.h"
#include "fsl_lpuart.h"
#include "fsl_gpio.h"
#include "fsl_irtc.h"
#include "fsl_power.h"
#include "fsl_pvts.h"
#include "pmic_support.h"
#include "fsl_utick.h"
#include "lpm.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "coremark.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*
 * Task priorities.
 * PVT Should be highest priority so it can increase VDDCORE ASAP if the PVT interrupt triggers.
 */
/*Workload task is a simulation of workload, pvt task is for reduce vddcore voltage*/
#define workload_task_PRIORITY (configMAX_PRIORITIES - 2)
#define pvt_task_PRIORITY      (configMAX_PRIORITIES - 1)

#define WORKLOAD_TIMES (5U)

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint32_t delay;
TaskHandle_t pvts_task_handle;
volatile uint32_t cur_voltage           = MAX_VDDCORE;
volatile static uint32_t workload_index = 0;

/* Used for pvt_task to make sure that when pvt_task is running, even if there is a vtaskDelay, CPU won't go to
 * deepsleep mode.*/
volatile bool adjusting = false;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void pvt_task(void *pvParameters);
static void workload_task(void *pvParameters);

void PVTS0_CPU0_DriverIRQHandler(void);
void PVTS0_HIFI4_DriverIRQHandler(void);

static void pvt_wait_timer_callback(void);
static bool adjust_vddcore(void);
static void config_pvts(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

void LPM_WaitForInterrupt(void)
{
#if defined(DEMO_PVT_ON_CPU_DSP) && (DEMO_PVT_ON_CPU_DSP != 0U) /* When DSP used, only enter sleep mode. */
    BOARD_EnterSleep();
#else                                                           /* Only run PVT on CPU. */
    BOARD_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP);
#endif
}

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    uint64_t xOstimerStartValue = 0;
    uint32_t irqMask;
    TickType_t timeOutTicks;

    if (xExpectedIdleTime == 0)
    {
        return;
    }

    irqMask = DisableGlobalIRQ();

    if (eTaskConfirmSleepModeStatus() != eAbortSleep)
    {
        if (!adjusting)
        {
            timeOutTicks = LPM_EnterTicklessIdle(xExpectedIdleTime, &xOstimerStartValue);

            if (timeOutTicks != 0U)
            {
                LPM_WaitForInterrupt();
                LPM_ExitTicklessIdle(timeOutTicks, xOstimerStartValue);
            }
        }
        else
        {
            __DSB();
            __ISB();
            __WFI();
        }
    }

    EnableGlobalIRQ(irqMask);
}

void PVTS0_CPU0_DriverIRQHandler(void)
{
#if defined(DEMO_PRINTF_INTERRUPT_STATUS) && (DEMO_PRINTF_INTERRUPT_STATUS == 1)
    PRINTF("PVT CPU INT\r\n");
#endif
    NVIC_ClearPendingIRQ((IRQn_Type)PVTS0_CPU0_IRQn);
    PVTS_Stop(kPVTS_Sensor0);

    /* Wake-up the PVT task to increase VDDCORE immediately. */
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(pvts_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

#if defined(DEMO_PVT_ON_CPU_DSP) && (DEMO_PVT_ON_CPU_DSP != 0U)
void PVTS0_HIFI4_DriverIRQHandler(void)
{
#if defined(DEMO_PRINTF_INTERRUPT_STATUS) && (DEMO_PRINTF_INTERRUPT_STATUS == 1)
    DEMO_LOG("PVT DSP INT\r\n");
#endif
    NVIC_ClearPendingIRQ((IRQn_Type)PVTS0_HIFI4_IRQn);
    PVTS_Stop(kPVTS_Sensor1);
    /* Wake-up the PVT task to increase VDDCORE immediately */
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(pvts_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
#endif

/*UTick timer callback*/
static void pvt_wait_timer_callback(void)
{
    /* Wake-up the PVT task to increase VDDCORE immediately */
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(pvts_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void config_pvts(void)
{
    PVTS_Init();

    /*Enable interupt*/
    NVIC_ClearPendingIRQ((IRQn_Type)PVTS0_CPU0_IRQn);
    NVIC_SetPriority((IRQn_Type)PVTS0_CPU0_IRQn, 2U);
    EnableIRQ((IRQn_Type)PVTS0_CPU0_IRQn);
    /* This is used to check if the PVT interrupt triggered when trying to optimize VDDCORE */
    PVTS_EnableAlertCount(kPVTS_Sensor0);
    /* Set the delay and start sensing */
    PVTS_SetDelay(kPVTS_Sensor0, PVTS_GET_DELAY0_FROM_FUSE_VALUE(delay));

    PVTS_Start(kPVTS_Sensor0);

#if defined(DEMO_PVT_ON_CPU_DSP) && (DEMO_PVT_ON_CPU_DSP != 0U)
    NVIC_ClearPendingIRQ(PVTS0_HIFI4_IRQn);
    NVIC_SetPriority(PVTS0_HIFI4_IRQn, 3U);
    EnableIRQ(PVTS0_HIFI4_IRQn);
    /* This is used to check if the PVT interrupt triggered when trying to optimize VDDCORE */
    PVTS_EnableAlertCount(kPVTS_Sensor1);
    /* Set the delay and start sensing */
    PVTS_SetDelay(kPVTS_Sensor1, PVTS_GET_DELAY1_FROM_FUSE_VALUE(delay));

    PVTS_Start(kPVTS_Sensor1);
#endif
}

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    status_t ret;
    /* Init board hardware. */
    BOARD_InitHardware();
    POWER_ClearEventFlags(0xFFFFFFFF);

    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };
    /*Add code to let DSP toggle blue led to show it work normally*/
    GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, &led_config);
    GPIO_EnablePinControlNonPrivilege(BOARD_LED_BLUE_GPIO, (1 << BOARD_LED_BLUE_GPIO_PIN));
    GPIO_EnablePinControlNonSecure(BOARD_LED_BLUE_GPIO, (1 << BOARD_LED_BLUE_GPIO_PIN));

    DEMO_LOG(
        "PVT Application Demo\r\n\n"
        "This application uses the PMIC for power supply.\r\n"
        "SILICON_REV_ID = %X%X\r\n"
        "Build Time: %s--%s \r\n",
        (SYSCON3->SILICONREV_ID & SYSCON3_SILICONREV_ID_MAJOR_MASK) >> SYSCON3_SILICONREV_ID_MAJOR_SHIFT,
        SYSCON3->SILICONREV_ID & SYSCON3_SILICONREV_ID_MINOR_MASK, __DATE__, __TIME__);

    ret = PVTS_ReadDelayFromOTP(false, kPVTS_Vdd2Com, DEMO_MAINCLK_FREQ, &delay);

    /* !!!NOTE!!!
     * Use typical value to run the demo if failed to read delay value from OTP.
     * This is only used to allow runing the demo on some early samples which without delay values programed in fuse
     * and can't be used in production. The function may not work for some samples and temperature. 
     */
    if (ret == kStatus_Fail)
    {
        DEMO_LOG("\r\nFailed to read PVTS delay, please check the sample!\r\n\n");
        delay = DEMO_TYPICAL_DELAY;
    }

    PRINTF("PVTS delay = 0x%x\r\n", delay);

    DEMO_LOG("Core Clock: %dHz \r\n", CLOCK_GetCoreSysClkFreq());
    DEMO_LOG("Input any key to start\r\n");
    (void)GETCHAR();

    /* Initialze power/clock configuration. */
    BOARD_InitPowerConfig();

    /* Boot and wait CPU1 booted. */
    APP_BootCore1();

    /* After the CPU1 booted, CPU0 relinquish the domain's control over the modules, and give the other domain exclusive
     * control. */
    BOARD_PowerConfigAfterCPU1Booted();
#if defined(DEMO_PVT_ON_CPU_DSP) && (DEMO_PVT_ON_CPU_DSP != 0U)
    BOARD_BootDSP();
#endif

    /*Create tickless task*/
    if (xTaskCreate(pvt_task, "PVT_task", configMINIMAL_STACK_SIZE + 500U, NULL, pvt_task_PRIORITY,
                    &pvts_task_handle) != pdPASS)
    {
        PRINTF("PVT Task creation failed!.\r\n");
        while (1)
        {
        }
    }

    if (xTaskCreate(workload_task, "Workload_task", configMINIMAL_STACK_SIZE + 1000U, NULL, workload_task_PRIORITY,
                    NULL) != pdPASS)
    {
        PRINTF("Workload Task creation failed!.\r\n");
        while (1)
        {
        }
    }

    /*Task Scheduler*/
    vTaskStartScheduler();
    for (;;)
        ;
}

/**
 * Increases VDDCORE if the PVT interrupt triggered, else it tries
 * to decrease VDDCORE as much as possible.
 *
 * Returns true if VDDCORE == MIN_VDDCORE, false otherwise.
 */
static bool adjust_vddcore(void)
{
    cur_voltage = BOARD_GetPmicVdd2Voltage();

    if ((PVTS_GetAlertCount(kPVTS_Sensor0) > 0) || (PVTS_GetAlertCount(kPVTS_Sensor1) > 0))
    {
        /* PVT interrupt fired, so we increase VDDCORE */
        cur_voltage = cur_voltage + DEMO_PMIC_ADJUST_STEP;
        BOARD_SetPmicVdd2Voltage(cur_voltage);
        pvt_wait_timer_start(PMIC_SETTLING_TIME_MS, pvt_wait_timer_callback);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (PVTS_GetAlertCount(kPVTS_Sensor0) > 0)
        {
            PVTS_ClearAlertCount(kPVTS_Sensor0);
            PVTS_Start(kPVTS_Sensor0);
        }
#if defined(DEMO_PVT_ON_CPU_DSP) && (DEMO_PVT_ON_CPU_DSP != 0U)
        if (PVTS_GetAlertCount(kPVTS_Sensor1) > 0)
        {
            PVTS_ClearAlertCount(kPVTS_Sensor1);
            PVTS_Start(kPVTS_Sensor1);
        }
#endif
        return false;
    }

    /* Try to decrease VDDCORE as much as possible until we reach MIN_VDDCORE or PVT interrupt fires */
    while (cur_voltage > MIN_VDDCORE)
    {
        cur_voltage = cur_voltage - DEMO_PMIC_ADJUST_STEP;
        BOARD_SetPmicVdd2Voltage(cur_voltage);
        pvt_wait_timer_start(PMIC_SETTLING_TIME_MS, pvt_wait_timer_callback);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if ((PVTS_GetAlertCount(kPVTS_Sensor0) > 0) || (PVTS_GetAlertCount(kPVTS_Sensor1) > 0))
        {
            cur_voltage = cur_voltage + DEMO_PMIC_ADJUST_STEP;
            BOARD_SetPmicVdd2Voltage(cur_voltage);
            pvt_wait_timer_start(PMIC_SETTLING_TIME_MS, pvt_wait_timer_callback);
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            if (PVTS_GetAlertCount(kPVTS_Sensor0) > 0)
            {
                PVTS_ClearAlertCount(kPVTS_Sensor0);
                PVTS_Start(kPVTS_Sensor0);
            }
#if defined(DEMO_PVT_ON_CPU_DSP) && (DEMO_PVT_ON_CPU_DSP != 0U)
            if (PVTS_GetAlertCount(kPVTS_Sensor1) > 0)
            {
                PVTS_ClearAlertCount(kPVTS_Sensor1);
                PVTS_Start(kPVTS_Sensor1);
            }
#endif
            break;
        }
    }

    return (cur_voltage <= MIN_VDDCORE);
}

/* pvt_task */
static void pvt_task(void *pvParameters)
{
    bool at_min_vddcore = false;

    /* Start the pvt sensor */
    config_pvts();

    /* Initialize timer for PVT task */
    pvt_wait_timer_init();

    for (;;)
    {
        /* Adjust the VDDCORE. */
        {
            adjusting      = true;
            at_min_vddcore = adjust_vddcore();
            /*clear the pin*/
            adjusting = false;
        }

        DEMO_LOG("PVT Task: Current VDDCORE = %f V\r\n", (double)cur_voltage / 1000000.0);

        if (!at_min_vddcore)
        {
            /*Delay 10s using utick and Notify*/
            /* Only enable periodic timer if it's possible for VDDCORE to go lower */
            pvt_wait_timer_start(PVT_TASK_WAIT_MS, pvt_wait_timer_callback);
        }

        /* Wait for UTICK interrupt notification */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }
}

/* workload_task */
static void workload_task(void *pvParameters)
{
    for (;;)
    {
        for (workload_index = 0; workload_index < WORKLOAD_TIMES; workload_index++)
        {
            /* Simulate workload. */
            coremark_main();
        }

        PRINTF(" Workload Task: Current VDDCORE = %f V\r\n", (double)cur_voltage / 1000000.0);

        vTaskDelay(pdMS_TO_TICKS(WORKLOAD_DELAY_MS));
    }
}
