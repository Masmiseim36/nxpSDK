/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "power_mode_switch.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "lpm.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "fsl_gpio.h"

#include "pin_mux.h"
#include "fsl_gpt.h"
#include "fsl_lpuart.h"
#include "fsl_iomuxc.h"
#include "fsl_tickless_generic.h"
#include "fsl_semc.h"

#include "fsl_gpio.h"
#include "clock_config.h"
#include "fsl_dcdc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_SDRAM0_BASE_ADDRESS 0x80000000U

/*******************************************************************************
* Prototypes
******************************************************************************/
#if defined(__GNUC__)
void Board_CopyToRam();
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_wakeupTimeout;            /* Wakeup timeout. (Unit: Second) */
static app_wakeup_source_t s_wakeupSource; /* Wakeup source.                 */

static lpm_power_mode_t s_targetPowerMode;
static lpm_power_mode_t s_curRunMode = LPM_PowerModeOverRun;
static SemaphoreHandle_t s_wakeupSig;

static const char *s_modeNames[] = {"Over RUN",    "Full Run",       "Low Speed Run", "Low Power Run",
                                    "System Idle", "Low Power Idle", "Suspend",       "SNVS"};

int32_t is_suspend_reset = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_SetLPClockGate(void)
{
    CCM->CCGR0 = 0x014000C5U;
    CCM->CCGR1 = 0x541c0000U;
    CCM->CCGR2 = 0x00150010U;
    CCM->CCGR3 = 0x50040110U;

    CCM->CCGR4 = 0x00005514U;
    CCM->CCGR5 = 0x51001105U;
    /* We can enable DCDC when need to config it and close it after configuration */
    CCM->CCGR6 = 0x00540540U;
}

#if defined(__MCUXPRESSO) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
#elif defined(__GNUC__) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
extern uint32_t __ram_function_flash_start[];
#define __RAM_FUNCTION_FLASH_START __ram_function_flash_start
extern uint32_t __ram_function_ram_start[];
#define __RAM_FUNCTION_RAM_START __ram_function_ram_start
extern uint32_t __ram_function_size[];
#define __RAM_FUNCTION_SIZE __ram_function_size
void Board_CopyToRam()
{
    unsigned char *source;
    unsigned char *destiny;
    unsigned int size;

    source = (unsigned char *)(__RAM_FUNCTION_FLASH_START);
    destiny = (unsigned char *)(__RAM_FUNCTION_RAM_START);
    size = (unsigned long)(__RAM_FUNCTION_SIZE);

    while (size--)
    {
        *destiny++ = *source++;
    }
}
#endif

static void Board_SdramInitSequence(uint32_t bl, uint32_t cl)
{
    SEMC_SendIPCommand(SEMC, kSEMC_MemType_SDRAM, BOARD_SDRAM0_BASE_ADDRESS, kSEMC_SDRAMCM_Prechargeall, 0, NULL);
    SEMC_SendIPCommand(SEMC, kSEMC_MemType_SDRAM, BOARD_SDRAM0_BASE_ADDRESS, kSEMC_SDRAMCM_AutoRefresh, 0, NULL);
    SEMC_SendIPCommand(SEMC, kSEMC_MemType_SDRAM, BOARD_SDRAM0_BASE_ADDRESS, kSEMC_SDRAMCM_AutoRefresh, 0, NULL);
    SEMC_SendIPCommand(SEMC, kSEMC_MemType_SDRAM, BOARD_SDRAM0_BASE_ADDRESS, kSEMC_SDRAMCM_Modeset, bl | (cl << 4),
                       NULL);
}

static void Board_SdramInit(uint32_t bl, uint32_t cl)
{
    CLOCK_EnableClock(kCLOCK_Semc);

    SEMC->MCR &= ~SEMC_MCR_MDIS_MASK;

    SEMC->MCR = 0x10000004;

    SEMC->BMCR0 = 0x00030524;

    SEMC->BMCR1 = 0x06030524;

    SEMC->BR[0] = (BOARD_SDRAM0_BASE_ADDRESS & 0xfffff000) | (SEMC_BR_MS(0xD)) | (SEMC_BR_VLD(0x1));
    SEMC->IOCR |= SEMC_IOCR_MUX_CSX0(1); // config SEMC_CCSX0 as SDRAM_CS1

    SEMC->SDRAMCR0 &= (~SEMC_SDRAMCR0_BL_MASK) | (~SEMC_SDRAMCR0_CL_MASK);
    SEMC->SDRAMCR0 = SEMC_SDRAMCR0_BL(bl) | SEMC_SDRAMCR0_CL(cl) | 0x301;
    SEMC->SDRAMCR1 = 0x652922;
    SEMC->SDRAMCR2 = 0x10920;
    SEMC->SDRAMCR3 = 0x50210A08;

    SEMC->DBICR0 = 0x00000021;
    SEMC->DBICR1 = 0x00888888;
    SEMC->IPCR1 = 0x2;
    SEMC->IPCR2 = 0;

    Board_SdramInitSequence(bl, cl);
}

void APP_WAKEUP_GPT_IRQn_HANDLER(void)
{
    GPT_ClearStatusFlags(APP_WAKEUP_GPT_BASE, kGPT_OutputCompare1Flag);
    GPT_StopTimer(APP_WAKEUP_GPT_BASE);
    LPM_DisableWakeupSource(APP_WAKEUP_GPT_IRQn);

    if (!is_suspend_reset)
    {
        xSemaphoreGiveFromISR(s_wakeupSig, NULL);
        portYIELD_FROM_ISR(pdTRUE);
    }
}

void APP_WAKEUP_BUTTON_IRQ_HANDLER(void)
{
    if ((1U << APP_WAKEUP_BUTTON_GPIO_PIN) & GPIO_GetPinsInterruptFlags(APP_WAKEUP_BUTTON_GPIO))
    {
        /* Disable interrupt. */
        GPIO_DisableInterrupts(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
        GPIO_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
        LPM_DisableWakeupSource(APP_WAKEUP_BUTTON_IRQ);
    }

    if (!is_suspend_reset)
    {
        xSemaphoreGiveFromISR(s_wakeupSig, NULL);
        portYIELD_FROM_ISR(pdTRUE);
    }
}

void APP_PowerPreSwitchHook(lpm_power_mode_t targetMode)
{
    if (targetMode == LPM_PowerModeSuspend || targetMode == LPM_PowerModeSNVS)
    {
        /* Wait for debug console output finished. */
        while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
        {
        }
        DbgConsole_Deinit();

        /*
         * Set pin for current leakage.
         * Debug console RX pin: Set to pinmux to GPIO input.
         * Debug console TX pin: Don't need to change.
         */
        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_GPIO1_IO13, 0);
        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_GPIO1_IO13, IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                                 IOMUXC_SW_PAD_CTL_PAD_PUS(2) |
                                                                 IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
    }
}

void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode)
{
    if (targetMode == LPM_PowerModeSuspend)
    {
        /*
         * Debug console RX pin is set to GPIO input, nee to re-configure pinmux.
         * Debug console TX pin: Don't need to change.
         */
        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPUART1_RX, 0);
        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_LPUART1_RX, IOMUXC_SW_PAD_CTL_PAD_SPEED(2));

        BOARD_InitDebugConsole();
    }
}

lpm_power_mode_t APP_GetLPMPowerMode(void)
{
    return s_targetPowerMode;
}

/*!
 * @brief Get input from user about wakeup timeout
 */
static uint8_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;

    while (1)
    {
        PRINTF("Select the wake up timeout in seconds.\r\n");
        PRINTF("The allowed range is 1s ~ 9s.\r\n");
        PRINTF("Eg. enter 5 to wake up in 5 seconds.\r\n");
        PRINTF("\r\nWaiting for input timeout value...\r\n\r\n");

        timeout = GETCHAR();
        PRINTF("%c\r\n", timeout);
        if ((timeout > '0') && (timeout <= '9'))
        {
            return timeout - '0';
        }
        PRINTF("Wrong value!\r\n");
    }
}

/* Get wakeup source by user input. */
static app_wakeup_source_t APP_GetWakeupSource(lpm_power_mode_t targetMode)
{
    uint8_t ch;

    while (1)
    {
        PRINTF("Select the wake up source:\r\n");
        PRINTF("Press T for GPT - GPT Timer\r\n");
        PRINTF("Press S for switch/button %s. \r\n", APP_WAKEUP_BUTTON_NAME);

        PRINTF("\r\nWaiting for key press..\r\n\r\n");

        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        if (ch == 'T')
        {
            return kAPP_WakeupSourceGPT;
        }
        else if (ch == 'S')
        {
            return kAPP_WakeupSourcePin;
        }
        else
        {
            PRINTF("Wrong value!\r\n");
        }
    }
}

/* Get wakeup timeout and wakeup source. */
static void APP_GetWakeupConfig(lpm_power_mode_t targetMode)
{
    /* Get wakeup source by user input. */
    s_wakeupSource = APP_GetWakeupSource(targetMode);

    if (kAPP_WakeupSourceGPT == s_wakeupSource)
    {
        /* Wakeup source is GPT, user should input wakeup timeout value. */
        s_wakeupTimeout = APP_GetWakeupTimeout();
        PRINTF("Will wakeup in %d seconds.\r\n", s_wakeupTimeout);
    }
    else
    {
        PRINTF("Switch %s from off to on to wake up.\r\n", APP_WAKEUP_BUTTON_NAME);
    }
}

static void APP_SetWakeupConfig(lpm_power_mode_t targetMode)
{
    /* Set GPT timeout value. */
    if (kAPP_WakeupSourceGPT == s_wakeupSource)
    {
        GPT_StopTimer(APP_WAKEUP_GPT_BASE);
        /* Update compare channel1 value will reset counter */
        GPT_SetOutputCompareValue(APP_WAKEUP_GPT_BASE, kGPT_OutputCompare_Channel1,
                                  (CLOCK_GetRtcFreq() * s_wakeupTimeout) - 1U);

        /* Enable GPT Output Compare1 interrupt */
        GPT_EnableInterrupts(APP_WAKEUP_GPT_BASE, kGPT_OutputCompare1InterruptEnable);
        NVIC_SetPriority(APP_WAKEUP_GPT_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY + 2);
        EnableIRQ(APP_WAKEUP_GPT_IRQn);

        /* Restart timer */
        GPT_StartTimer(APP_WAKEUP_GPT_BASE);

        LPM_EnableWakeupSource(APP_WAKEUP_GPT_IRQn);
    }
    else
    {
        GPIO_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
        /* Enable GPIO pin interrupt */
        GPIO_EnableInterrupts(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);
        NVIC_SetPriority(APP_WAKEUP_BUTTON_IRQ, configMAX_SYSCALL_INTERRUPT_PRIORITY + 2);
        /* Enable the Interrupt */
        EnableIRQ(APP_WAKEUP_BUTTON_IRQ);
        /* Enable GPC interrupt */
        LPM_EnableWakeupSource(APP_WAKEUP_BUTTON_IRQ);
    }
    is_suspend_reset = 0;
}

static void APP_PrintRunFrequency(int32_t run_freq_only)
{
    PRINTF("\r\n");
    PRINTF("***********************************************************\r\n");
    PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
    PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
    PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
    PRINTF("IPG:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_IpgClk));
    PRINTF("OSC:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_OscClk));
    PRINTF("RTC:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_RtcClk));
    PRINTF("ARMPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_ArmPllClk));
    if (!run_freq_only)
    {
        PRINTF("USB1PLL:         %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllClk));
        PRINTF("USB1PLLPFD0:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk));
        PRINTF("USB1PLLPFD1:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk));
        PRINTF("USB1PLLPFD2:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd2Clk));
        PRINTF("USB1PLLPFD3:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd3Clk));
        PRINTF("USB2PLL:         %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb2PllClk));
        PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
        PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
        PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
        PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
        PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
        PRINTF("ENETPLL0:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_EnetPll0Clk));
        PRINTF("ENETPLL1:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_EnetPll1Clk));
        PRINTF("AUDIOPLL:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AudioPllClk));
        PRINTF("VIDEOPLL:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_VideoPllClk));
    }
    PRINTF("***********************************************************\r\n");
    PRINTF("\r\n");
}

static void APP_ShowPowerMode(lpm_power_mode_t powerMode)
{
    if (powerMode <= LPM_PowerModeEnd)
    {
        PRINTF("    Power mode: %s\r\n", s_modeNames[powerMode]);
        APP_PrintRunFrequency(1);
    }
    else
    {
        assert(0);
    }
}

/* Called in idle task */
static bool APP_LpmListener(lpm_power_mode_t curMode, lpm_power_mode_t newMode, void *data)
{
    PRINTF("WorkingTask %d: Transfer from %s to %s\r\n", (uint32_t)data, s_modeNames[curMode], s_modeNames[newMode]);

    /* Do necessary preparation for this mode change */

    return true; /* allow this switch */
}

/*!
 * @brief main task.
 */
static void PowerModeSwitchTask(void *pvParameters)
{
    uint8_t ch;
    uint32_t freq;
    gpt_config_t gptConfig;
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t swConfig = {
        kGPIO_DigitalInput, 0, kGPIO_IntRisingEdge,
    };

    /* Init GPT for wakeup as FreeRTOS tell us */
    GPT_GetDefaultConfig(&gptConfig);
    gptConfig.clockSource = kGPT_ClockSource_LowFreq; /* 32K RTC OSC */
    // gptConfig.enableMode = false;                     /* Keep counter when stop */
    gptConfig.enableMode = true; /* Don't keep counter when stop */
    gptConfig.enableRunInDoze = true;
    /* Initialize GPT module */
    GPT_Init(APP_WAKEUP_GPT_BASE, &gptConfig);
    GPT_SetClockDivider(APP_WAKEUP_GPT_BASE, 1);

    /* Init input switch GPIO. */
    GPIO_PinInit(APP_WAKEUP_BUTTON_GPIO, APP_WAKEUP_BUTTON_GPIO_PIN, &swConfig);
    GPIO_EnableInterrupts(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);

    while (1)
    {
        freq = CLOCK_GetFreq(kCLOCK_CpuClk);

        PRINTF("\r\n########## Power Mode Switch Demo (build %s) ###########\n\r\n", __DATE__);
        PRINTF("    Core Clock = %dHz \r\n", freq);

        APP_ShowPowerMode(s_curRunMode);

        PRINTF("\r\nSelect the desired operation \n\r\n");
        PRINTF("Press  %c for enter: Over RUN       - System Over Run mode (600MHz)\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeOverRun);
        PRINTF("Press  %c for enter: Full RUN       - System Full Run mode (528MHz)\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeFullRun);
        PRINTF("Press  %c for enter: Low Speed RUN  - System Low Speed Run mode (132MHz)\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeLowSpeedRun);
        PRINTF("Press  %c for enter: Low Power RUN  - System Low Power Run mode (24MHz)\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeLowPowerRun);
        PRINTF("Press  %c for enter: System Idle    - System Wait mode\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeSysIdle);
        PRINTF("Press  %c for enter: Low Power Idle - Low Power Idle mode\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeLPIdle);
        PRINTF("Press  %c for enter: Suspend        - Suspend mode\r\n", (uint8_t)'A' + (uint8_t)LPM_PowerModeSuspend);
        PRINTF("Press  %c for enter: SNVS           - Shutdown the system\r\n",
               (uint8_t)'A' + (uint8_t)LPM_PowerModeSNVS);

        PRINTF("\r\nWaiting for power mode select..\r\n\r\n");

        /* Wait for user response */
        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        s_targetPowerMode = (lpm_power_mode_t)(ch - 'A');

        if (s_targetPowerMode <= LPM_PowerModeSNVS)
        {
            if (!LPM_SetPowerMode(s_targetPowerMode))
            {
                PRINTF("Some task doesn't allow to enter mode %s\r\n", s_modeNames[s_targetPowerMode]);
            }
            else
            {
                if (s_targetPowerMode <= LPM_PowerModeRunEnd)
                {
                    switch (s_targetPowerMode)
                    {
                        case LPM_PowerModeOverRun:
                            LPM_SystemOverRun();
                            break;
                        case LPM_PowerModeFullRun:
                            LPM_SystemFullRun();
                            break;
                        case LPM_PowerModeLowSpeedRun:
                            LPM_SystemLowSpeedRun();
                            break;
                        case LPM_PowerModeLowPowerRun:
                            LPM_SystemLowPowerRun();
                            break;
                        default:
                            break;
                    }
                    s_curRunMode = s_targetPowerMode;
                    continue;
                }
                else if (LPM_PowerModeSNVS == s_targetPowerMode)
                {
                    PRINTF("Now shutting down the system...\r\n");
                    APP_PowerPreSwitchHook(s_targetPowerMode);
                    CLOCK_EnableClock(kCLOCK_SnvsHp);
                    SNVS->LPCR |= SNVS_LPCR_DP_EN_MASK;
                    SNVS->LPCR |= SNVS_LPCR_TOP_MASK;
                    while (1) /* Shutdown */
                    {
                    }
                }
                else
                {
                    APP_GetWakeupConfig(s_targetPowerMode);
                    APP_SetWakeupConfig(s_targetPowerMode);
                    xSemaphoreTake(s_wakeupSig, portMAX_DELAY);
                }
                LPM_SetPowerMode(s_curRunMode);
                s_targetPowerMode = s_curRunMode;
            }
        }
        PRINTF("\r\nNext loop\r\n");
    }
}

/*!
 * @brief simulating working task.
 */
static void WorkingTask(void *pvParameters)
{
    LPM_RegisterPowerListener(APP_LpmListener, pvParameters);

    for (;;)
    {
        /* Use App task logic to replace vTaskDelay */
        PRINTF("Task %d is working now\r\n", (uint32_t)pvParameters);
        vTaskDelay(portMAX_DELAY);
    }
}

/*!
 * @brief main demo function.
 */
int main(void)
{
    BaseType_t xReturn;

    if (PGC->CPU_SR & PGC_CPU_SR_PSR_MASK)
    {
        /* MPU config */
        BOARD_ConfigMPU();

/* For ARM GCC, need to copy text from flash to RAM. This also can be done in start up code. */
#if defined(__MCUXPRESSO) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
#elif defined(__GNUC__) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
        Board_CopyToRam();
#endif

        /* Boot ROM did initialize the XTAL, here we only sets external XTAL OSC freq */
        CLOCK_SetXtalFreq(BOARD_XTAL0_CLK_HZ);
        CLOCK_SetRtcXtalFreq(BOARD_XTAL32K_CLK_HZ);

        /* Restore when wakeup from suspend reset */
        LPM_SystemResumeDsm();

        /* Recover handshaking */
        IOMUXC_GPR->GPR4 = 0x00000000;
        IOMUXC_GPR->GPR7 = 0x00000000;
        IOMUXC_GPR->GPR8 = 0x00000000;
        IOMUXC_GPR->GPR12 = 0x00000000;

        CCM->CCR &= ~CCM_CCR_REG_BYPASS_COUNT_MASK;

        /* Set the flag to mark system reeset from SUSPEND */
        is_suspend_reset = 1;

        EnableIRQ(APP_WAKEUP_BUTTON_IRQ);
        EnableIRQ(APP_WAKEUP_GPT_IRQn);

        APP_PowerPostSwitchHook(LPM_PowerModeSuspend);

        PRINTF("\r\nWakeup from suspend reset!\r\n");

        /* Recover to Over Run after suspend reset */
        LPM_SystemOverRun();
    }
    else
    {
        /* Init board hardware. */
    BOARD_ConfigMPU();
#if defined(__MCUXPRESSO) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
#elif defined(__GNUC__) && defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    Board_CopyToRam();
#endif
    BOARD_InitPins();
    BOARD_BootClockRUN();
    Board_SdramInit(3, 3);

    /* Configure UART divider to default */
    CLOCK_SetMux(kCLOCK_UartMux, 1); /* Set UART source to OSC 24M */
    CLOCK_SetDiv(kCLOCK_UartDiv, 0); /* Set UART divider to 1 */

    /* Select OSC as PIT clock source */
    CLOCK_SetMux(kCLOCK_PerclkMux, 1); /* Set PIT clock source to OSC 24M */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0); /* Set PIT clock divider to 1 */

#if !(defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
    /* In low power, better to use core pll for flexspi
       Use AXI clock for flexspi, AXI clock is from SEMC clock, 100MHz */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x0);
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 0x0);

    /* In low power, better to use core pll for semc */
    CLOCK_SetMux(kCLOCK_SemcMux, 0x0); /* Use periph clock as semc clock source */
    CLOCK_SetDiv(kCLOCK_SemcDiv, 0x5); /* Semc 100MHz */
#endif

    BOARD_InitDebugConsole();
    }

    PRINTF("\r\nCPU wakeup source 0x%x...\r\n", SRC->SRSR);

    PRINTF("\r\n***********************************************************\r\n");
    PRINTF("\tPower Mode Switch Demo for iMXRT1064\r\n");
    PRINTF("***********************************************************\r\n");
    APP_PrintRunFrequency(0);

    if (true != LPM_Init(s_curRunMode))
    {
        PRINTF("LPM Init Failed!\r\n");
        return -1;
    }

    s_wakeupSig = xSemaphoreCreateBinary();
    /* Make current resource count 0 for signal purpose */
    xSemaphoreTake(s_wakeupSig, 0);

    xReturn = xTaskCreate(PowerModeSwitchTask, "Power Mode Switch Task", configMINIMAL_STACK_SIZE + 512, NULL,
                          tskIDLE_PRIORITY + 1U, NULL);
    configASSERT(xReturn);

    xReturn =
        xTaskCreate(WorkingTask, "Working Task1", configMINIMAL_STACK_SIZE, (void *)1, tskIDLE_PRIORITY + 2U, NULL);
    configASSERT(xReturn);

    xReturn =
        xTaskCreate(WorkingTask, "Working Task2", configMINIMAL_STACK_SIZE, (void *)2, tskIDLE_PRIORITY + 3U, NULL);
    configASSERT(xReturn);

    vTaskStartScheduler();

    /* Application should never reach this point. */
    for (;;)
    {
    }
}
