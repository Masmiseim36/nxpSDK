/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_smc.h"
#include "fsl_pmc.h"
#include "board.h"
#include <stdlib.h>

#include "pin_mux.h"
#include "clock_config.h"
#include "peripherals.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ADC16_RESULT_REG_ADDR (uint32_t)(&ADC0->R[0]) /* Get ADC16 result register address */

#define DEMO_DMA_IRQ_HANDLER_FUNC DMA0_IRQHandler

#define LED_INIT() LED_RED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE() LED_RED_TOGGLE()

#define DEMO_ADC16_SAMPLE_COUNT 16U /* The ADC16 sample count */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_ConfigTriggerSource(void);
/*!
 * @brief Process ADC values.
 *
 */
static void ProcessSampleData(void);

/*!
 * @brief Initialize the DMA for async mode.
 *
 */
static void DMA_Configuration(void);

/*!
 * @brief Wait clock stable.
 *
 */
static void APP_WaitClockStable(void);

/*!
 * @brief Delay a bit.
 *
 */
static void Delay(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint32_t g_adc16SampleDataArray[DEMO_ADC16_SAMPLE_COUNT]; /* ADC value array */
static uint32_t g_avgADCValue = 0U;                              /* Average ADC value */
dma_transfer_config_t g_transferConfig;                          /* Dma transfer config */
const uint32_t g_Adc16_16bitFullRange = 65536U;

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_ConfigTriggerSource(void)
{
    /* Configure SIM for ADC hw trigger source selection */
    SIM->SOPT7 |= SIM_SOPT7_ADC0TRGSEL(14) | SIM_SOPT7_ADC0ALTTRGEN(1);
}
static void DMA_Configuration(void)
{
    DMA_PrepareTransfer(&g_transferConfig, (void *)ADC16_RESULT_REG_ADDR, sizeof(uint32_t),
                        (void *)g_adc16SampleDataArray, sizeof(uint32_t), sizeof(g_adc16SampleDataArray),
                        kDMA_PeripheralToMemory);
    /* Setup transfer */
    DMA_SetTransferConfig(DEMO_DMA_DMA_BASEADDR, DEMO_DMA_ADC0_DMA_CHANNEL, &g_transferConfig);
    /* Enable transfer. */
    DMA_StartTransfer(&DEMO_DMA_ADC0_Handle);
}

static void ProcessSampleData(void)
{
    uint32_t i = 0U;

    g_avgADCValue = 0;
    /* Get average adc value */
    for (i = 0; i < DEMO_ADC16_SAMPLE_COUNT; i++)
    {
        g_avgADCValue += g_adc16SampleDataArray[i];
    }
    g_avgADCValue = g_avgADCValue / DEMO_ADC16_SAMPLE_COUNT;

    /* Reset old value */
    for (i = 0; i < DEMO_ADC16_SAMPLE_COUNT; i++)
    {
        g_adc16SampleDataArray[i] = 0U;
    }
}

static void APP_WaitClockStable(void)
{
    /* Delay a bit */
    Delay();
#if defined(FSL_FEATURE_MCG_HAS_PLL) && (FSL_FEATURE_MCG_HAS_PLL)
    /* Set to PEE mode clock */
    if (kMCG_ModePBE == CLOCK_GetMode())
    {
        /* Wait for PLL lock. */
        while (!(kMCG_Pll0LockFlag & CLOCK_GetStatusFlags()))
        {
        }
        CLOCK_SetPeeMode();
    }
#endif
}

static void Delay(void)
{
    uint32_t i = 0;

    for (i = 0; i < 6000; i++)
    {
        __NOP();
    }
}

void DEMO_DMA_IRQ_HANDLER_FUNC(void)
{
    /* Stop trigger */
    LPTMR_StopTimer(DEMO_LPTMR_PERIPHERAL);
    /* Clear transaction done interrupt flag */
    DMA_ClearChannelStatusFlags(DEMO_DMA_DMA_BASEADDR, DEMO_DMA_ADC0_DMA_CHANNEL, kDMA_TransactionsDoneFlag);
    /* Setup transfer */
    DMA_PrepareTransfer(&g_transferConfig, (void *)ADC16_RESULT_REG_ADDR, sizeof(uint32_t),
                        (void *)g_adc16SampleDataArray, sizeof(uint32_t), sizeof(g_adc16SampleDataArray),
                        kDMA_PeripheralToMemory);
    DMA_SetTransferConfig(DEMO_DMA_DMA_BASEADDR, DEMO_DMA_ADC0_DMA_CHANNEL, &g_transferConfig);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

int main(void)
{
    /* Initialize hardware */
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitPeripherals();
    BOARD_InitDebugConsole();
    /* Initialize Led */
    LED_INIT();

    PRINTF("ADC LOW POWER ASYNC DMA PERIPHERAL DEMO\r\n");

    /* Set to allow entering vlps mode */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeVlp);
    /* Initialize DMA transfer */
    DMA_Configuration();
    /* Initialize SIM for ADC hw trigger source selection */
    BOARD_ConfigTriggerSource();

    PRINTF("ADC Full Range: %d\r\n", g_Adc16_16bitFullRange);
    while (1)
    {
        /* Start low power timer */
        LPTMR_StartTimer(DEMO_LPTMR_PERIPHERAL);
        /* Enter to Very Low Power Stop Mode */
        SMC_SetPowerModeVlps(SMC);
        /* Wait clock stable after wake up */
        APP_WaitClockStable();
        /* Deinit debug console */
        DbgConsole_Deinit();
        /* Init debug console after wake up */
        BOARD_InitDebugConsole();
        /* Toggle led */
        LED_TOGGLE();
        /* Process ADC value */
        ProcessSampleData();

        PRINTF("ADC value: %d\r\n", g_avgADCValue);

        /* Wait for data in uart fifo flushed */
        Delay();
    }
}
