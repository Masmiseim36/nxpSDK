/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"

#include "board.h"
#include "pin_mux.h"
#include "app_srtm.h"
#include "fsl_gpc.h"
#include "sai_low_power_audio.h"
#include "lpm.h"
#include "fsl_debug_console.h"
#include "clock_config.h"
#include "fsl_rdc.h"
#include "fsl_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RDC_DISABLE_A53_ACCESS 0xFC
#define RDC_DISABLE_M4_ACCESS 0xF3
static LPM_POWER_STATUS_M4 m4_lpm_state = LPM_M4_STATE_RUN;
/* Using SRC_GPR9 register as the communication address with A core. */
#define ServiceFlagAddr SRC->GPR9
#define ServiceBusy (0x5555U)
#define ServiceIdle (0x0U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void Peripheral_RdcSetting(void)
{
    rdc_domain_assignment_t assignment = {0};
    rdc_periph_access_config_t periphConfig;

    assignment.domainId = BOARD_DOMAIN_ID;
    RDC_SetMasterDomainAssignment(RDC, kRDC_Master_SDMA3_PERIPH, &assignment);
    RDC_SetMasterDomainAssignment(RDC, kRDC_Master_SDMA3_BURST, &assignment);
    RDC_SetMasterDomainAssignment(RDC, kRDC_Master_SDMA3_SPBA2, &assignment);

    RDC_GetDefaultPeriphAccessConfig(&periphConfig);
    /* Do not allow the A53 domain(domain0) to access the following peripherals. */
    periphConfig.policy = RDC_DISABLE_A53_ACCESS;
    periphConfig.periph = kRDC_Periph_SAI1;
    RDC_SetPeriphAccessConfig(RDC, &periphConfig);
    periphConfig.periph = kRDC_Periph_UART4;
    RDC_SetPeriphAccessConfig(RDC, &periphConfig);
    periphConfig.periph = kRDC_Periph_I2C3;
    RDC_SetPeriphAccessConfig(RDC, &periphConfig);
    periphConfig.periph = kRDC_Periph_GPT1;
    RDC_SetPeriphAccessConfig(RDC, &periphConfig);
    /* For SAI3, both kRDC_Periph_SAI3_ACCESS and kRDC_Periph_SAI3_LPM registers need set.*/
    periphConfig.periph = kRDC_Periph_SAI3_ACCESS;
    RDC_SetPeriphAccessConfig(RDC, &periphConfig);
    periphConfig.periph = kRDC_Periph_SAI3_LPM;
    RDC_SetPeriphAccessConfig(RDC, &periphConfig);
}
/*
 * Give readable string of current M4 lpm state
 */
const char *LPM_MCORE_GetPowerStatusString(void)
{
    switch (m4_lpm_state)
    {
        case LPM_M4_STATE_RUN:
            return "RUN";
        case LPM_M4_STATE_WAIT:
            return "WAIT";
        case LPM_M4_STATE_STOP:
            return "STOP";
        default:
            return "UNKNOWN";
    }
}

void LPM_MCORE_ChangeM4Clock(LPM_M4_CLOCK_SPEED target)
{
    /* Change CCM Root to change M4 clock*/
    switch (target)
    {
        case LPM_M4_LOW_FREQ:
            if (CLOCK_GetRootMux(kCLOCK_RootM4) != kCLOCK_M4RootmuxOsc24M)
            {
                CLOCK_SetRootMux(kCLOCK_RootM4, kCLOCK_M4RootmuxOsc24M);
                CLOCK_SetRootDivider(kCLOCK_RootM4, 1U, 1U);
            }
            break;
        case LPM_M4_HIGH_FREQ:
            if (CLOCK_GetRootMux(kCLOCK_RootM4) != kCLOCK_M4RootmuxSysPll1)
            {
                CLOCK_SetRootDivider(kCLOCK_RootM4, 1U, 2U);
                CLOCK_SetRootMux(kCLOCK_RootM4, kCLOCK_M4RootmuxSysPll1); /* switch cortex-m4 to SYSTEM PLL1 */
            }
            break;
        default:
            break;
    }
}

void LPM_MCORE_SetPowerStatus(GPC_Type *base, LPM_POWER_STATUS_M4 targetPowerMode)
{
    gpc_lpm_config_t config;
    config.enCpuClk              = false;
    config.enFastWakeUp          = false;
    config.enDsmMask             = false;
    config.enWfiMask             = false;
    config.enVirtualPGCPowerdown = true;
    config.enVirtualPGCPowerup   = true;
    switch (targetPowerMode)
    {
        case LPM_M4_STATE_RUN:
            GPC->LPCR_M4 = GPC->LPCR_M4 & (~GPC_LPCR_M4_LPM0_MASK);
            break;
        case LPM_M4_STATE_WAIT:
            GPC_EnterWaitMode(GPC, &config);
            break;
        case LPM_M4_STATE_STOP:
            GPC_EnterStopMode(GPC, &config);
            break;
        default:
            break;
    }

    m4_lpm_state = targetPowerMode;
}
void PreSleepProcessing(void)
{
    APP_SRTM_Suspend();
    DbgConsole_Deinit();
}

void PostSleepProcessing(void)
{
    APP_SRTM_Resume();
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE,
                    BOARD_DEBUG_UART_CLK_FREQ);
}

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    uint32_t irqMask;
    uint64_t counter = 0;
    uint32_t timeoutTicks;
    uint32_t timeoutMilliSec = (uint64_t)1000 * xExpectedIdleTime / configTICK_RATE_HZ;

    irqMask = DisableGlobalIRQ();

    /* Only when no context switch is pending and no task is waiting for the scheduler
     * to be unsuspended then enter low power entry.
     */
    if (eTaskConfirmSleepModeStatus() != eAbortSleep)
    {
        timeoutTicks = LPM_EnterTicklessIdle(timeoutMilliSec, &counter);
        if (timeoutTicks)
        {
            if (APP_SRTM_ServiceIdle() && LPM_AllowSleep())
            {
                LPM_MCORE_ChangeM4Clock(LPM_M4_LOW_FREQ);
                LPM_MCORE_SetPowerStatus(BOARD_GPC_BASEADDR, LPM_M4_STATE_STOP);
                PRINTF("\r\nMode:%s\r\n", LPM_MCORE_GetPowerStatusString());
                PreSleepProcessing();
                ServiceFlagAddr = ServiceIdle;
                __DSB();
                __ISB();
                __WFI();
                ServiceFlagAddr = ServiceBusy;
                PostSleepProcessing();
                LPM_MCORE_ChangeM4Clock(LPM_M4_HIGH_FREQ);
                LPM_MCORE_SetPowerStatus(BOARD_GPC_BASEADDR, LPM_M4_STATE_RUN);
                PRINTF("\r\nMode:%s\r\n", LPM_MCORE_GetPowerStatusString());
            }
            else
            {
                __DSB();
                __ISB();
                __WFI();
            }
        }
        LPM_ExitTicklessIdle(timeoutTicks, counter);
    }

    EnableGlobalIRQ(irqMask);
}
void MainTask(void *pvParameters)
{
    uint8_t control_char;

    /* Treat M core as busy status by default.*/
    ServiceFlagAddr = ServiceBusy;

    /*
     * Wait For A core Side Become Ready
     */
    PRINTF("********************************\r\n");
    PRINTF("Please follow:\r\n");
    PRINTF("  (1) Boot Linux kernel first to create the link between M core and A core;\r\n");
    PRINTF(
        "  (2) If want to make the M core enter STOP mode when there is no audio playback, press \"s\" or \"S\" "
        "first;\r\n");
    PRINTF("  (3) Audio playback is allowed even skip the step 2 using the playback command.\r\n");
    PRINTF("********************************\r\n");

    for (;;)
    {
        control_char = GETCHAR();
        PRINTF("%c", control_char);
        if ((control_char == 's') || (control_char == 'S'))
        {
            break;
        }
    }
    PRINTF("\r\n");

    /* Configure GPC */
    GPC_Init(BOARD_GPC_BASEADDR, APP_PowerUpSlot, APP_PowerDnSlot);
    GPC_EnableIRQ(BOARD_GPC_BASEADDR, BOARD_MU_IRQ_NUM);
    GPC_EnableIRQ(BOARD_GPC_BASEADDR, SYSTICK_IRQn);
    GPC_EnableIRQ(BOARD_GPC_BASEADDR, I2C3_IRQn);
    while (true)
    {
        /* Use App task logic to replace vTaskDelay */
        PRINTF("\r\nTask %s is working now.\r\n", (char *)pvParameters);
        vTaskDelay(portMAX_DELAY);
    }
}
/*!
 * @brief Main function
 */
int main(void)
{
    char *taskID = "A";
    uint32_t i = 0;

    BOARD_RdcInit();
    Peripheral_RdcSetting();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitMemory();
    /*
     * In order to wakeup M4 from LPM, all PLLCTRLs need to be set to "NeededRun"
     */
    for (i = 0; i != 39; i++)
    {
        CCM->PLL_CTRL[i].PLL_CTRL = kCLOCK_ClockNeededRun;
    }
    CLOCK_SetRootMux(kCLOCK_RootSai1, kCLOCK_SaiRootmuxAudioPll1); /* Set SAI source to Audio PLL1 393216000HZ */
    CLOCK_SetRootDivider(kCLOCK_RootSai1, 1U, 8U);                 /* Set root clock to 393216000HZ / 8 = 49152000HZ */
    CLOCK_SetRootMux(kCLOCK_RootSai3, kCLOCK_SaiRootmuxAudioPll1); /* Set SAI source to Audio PLL1 393215996HZ */
    CLOCK_SetRootDivider(kCLOCK_RootSai3, 1U, 16U);                /* Set root clock to 393216000HZ / 16 = 27576000HZ */
    CLOCK_SetRootMux(kCLOCK_RootI2c3, kCLOCK_I2cRootmuxSysPll1Div5); /* Set I2C source to SysPLL1 Div5 160MHZ */
    CLOCK_SetRootDivider(kCLOCK_RootI2c3, 1U, 10U);                  /* Set root clock to 160MHZ / 10 = 16MHZ */
    CLOCK_SetRootMux(kCLOCK_RootGpt1, kCLOCK_GptRootmuxOsc24M);      /* Set GPT source to Osc24 MHZ */
    CLOCK_SetRootDivider(kCLOCK_RootGpt1, 1U, 1U);
#if APP_SRTM_CODEC_AK4497_USED
    APP_SRTM_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
#endif
    PRINTF("\r\n####################  LOW POWER AUDIO TASK ####################\n\r\n");
    PRINTF("    Build Time: %s--%s \r\n", __DATE__, __TIME__);

    APP_SRTM_Init();

    if (xTaskCreate(MainTask, "Main Task", 256U, (void *)taskID, tskIDLE_PRIORITY + 1U, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    /* Start FreeRTOS scheduler. */
    vTaskStartScheduler();

    /* Application should never reach this point. */
    for (;;)
    {
    }
}
void vApplicationMallocFailedHook(void)
{
    PRINTF("Malloc Failed!!!\r\n");
}
