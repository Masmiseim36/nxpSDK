/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"
#include "fsl_qtmr.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* The QTMR instance/channel used for board */
#define BOARD_QTMR_BASEADDR TMR3
#define BOARD_QTMR_INPUT_CAPTURE_CHANNEL kQTMR_Channel_0
#define QTMR_CounterInputPin kQTMR_Counter0InputPin
#define BOARD_QTMR_PWM_CHANNEL kQTMR_Channel_1
#define QTMR_CounterInputPin kQTMR_Counter0InputPin

/* Get source clock for QTMR driver */
#define QTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_IpgClk)

#define EXAMPLE_QTMR_DMA_MUX (DMAMUX)
#define EXAMPLE_QTMR_DMA (DMA0)

#define QTMR_EDMA_REQUEST_CAPT_SOURCE kDmaRequestMuxQTIMER3CaptTimer0Cmpld1Timer0Cmpld2Timer1
#define QTMR_EDMA_REQUEST_CMPLD_SOURCE kDmaRequestMuxQTIMER3CaptTimer0Cmpld1Timer0Cmpld2Timer1


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
edma_handle_t g_EDMA_Handle;
volatile bool g_Transfer_Done = false;
AT_NONCACHEABLE_SECTION_INIT(volatile uint16_t g_Cmpld1Value) = 0U;
AT_NONCACHEABLE_SECTION_INIT(volatile uint16_t g_Cmpld2Value) = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* User callback function for EDMA transfer. */
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
    }
}

status_t QTMR_SetCmpldValue(uint32_t pwmFreqHz, uint8_t dutyCyclePercent, int32_t srcClock_Hz)
{
    uint32_t periodCount, highCount, lowCount;
    periodCount = (srcClock_Hz / pwmFreqHz);
    if (dutyCyclePercent > 100)
    {
        /* Invalid dutycycle */
        return kStatus_Fail;
    }
    highCount = (periodCount * dutyCyclePercent) / 100;
    lowCount = periodCount - highCount;
    g_Cmpld1Value = lowCount;
    g_Cmpld2Value = highCount;
    return kStatus_Success;
}

AT_NONCACHEABLE_SECTION_INIT(uint16_t captValue) = 0;

/*!
 * @brief Main function
 */
int main(void)
{
    qtmr_config_t qtmrConfig;
    edma_config_t userConfig;
    edma_transfer_config_t transferConfig;
    uint8_t updatedDutycycle = 50U;
    uint8_t getCharValue = 0U;
    uint32_t time1 = 0, time2 = 0, counterClock;
    uint32_t timediff = 0;

    /* Board pin, clock, debug console init */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* DMAMUX init */
    DMAMUX_Init(EXAMPLE_QTMR_DMA_MUX);
    DMAMUX_SetSource(EXAMPLE_QTMR_DMA_MUX, 0, QTMR_EDMA_REQUEST_CAPT_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_QTMR_DMA_MUX, 0);

    /* EDMA init */
    /*
     * userConfig.enableRoundRobinArbitration = false;
     * userConfig.enableHaltOnError = true;
     * userConfig.enableContinuousLinkMode = false;
     * userConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(EXAMPLE_QTMR_DMA, &userConfig);
    EDMA_CreateHandle(&g_EDMA_Handle, EXAMPLE_QTMR_DMA, 0);
    EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);

    /*
     * qtmrConfig.debugMode = kQTMR_RunNormalInDebug;
     * qtmrConfig.enableExternalForce = false;
     * qtmrConfig.enableMasterMode = false;
     * qtmrConfig.faultFilterCount = 0;
     * qtmrConfig.faultFilterPeriod = 0;
     * qtmrConfig.primarySource = kQTMR_ClockDivide_2;
     * qtmrConfig.secondarySource = kQTMR_Counter0InputPin;
     */
    QTMR_GetDefaultConfig(&qtmrConfig);

    PRINTF("\r\n****Input capture dma example start.****\n");
    PRINTF("\r\n****Provide a signal input to the QTMR pin****\n");

    /* Init input capture channel to use the IP Bus clock div by 8 */
    qtmrConfig.primarySource = kQTMR_ClockDivide_8;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, &qtmrConfig);

    /* Setup the input capture */
    QTMR_SetupInputCapture(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, QTMR_CounterInputPin, false, true,
                           kQTMR_RisingEdge);

    /* Enable the input edge flag DMA*/
    QTMR_EnableDma(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_InputEdgeFlagDmaEnable);
    EDMA_PrepareTransfer(&transferConfig,
                         (uint16_t *)&BOARD_QTMR_BASEADDR->CHANNEL[BOARD_QTMR_INPUT_CAPTURE_CHANNEL].CAPT, 2,
                         &captValue, 2, 2, 2, kEDMA_PeripheralToMemory);
    EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
    /* Start the input capture channel to count on rising edge of the primary source clock */
    QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_PriSrcRiseEdge);

    /* Wait input Edge*/
    while (!(QTMR_GetStatus(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL) & kQTMR_EdgeFlag))
    {
        time1 = BOARD_QTMR_BASEADDR->CHANNEL[BOARD_QTMR_INPUT_CAPTURE_CHANNEL].CAPT;
    }
    QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_EdgeFlag);
    while (!(QTMR_GetStatus(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL) & kQTMR_EdgeFlag))
    {
        time2 = BOARD_QTMR_BASEADDR->CHANNEL[BOARD_QTMR_INPUT_CAPTURE_CHANNEL].CAPT;
    }
    if (time1 < time2)
    {
        timediff = time2 - time1;
    }
    else /* Consider counter overflow and wrap situation */
    {
        timediff = 65536 - time1 + time2;
    }

    counterClock = QTMR_SOURCE_CLOCK / 8000;

    EDMA_StartTransfer(&g_EDMA_Handle);

    /* Wait for EDMA transfer finish */
    while (g_Transfer_Done != true)
    {
    }
    PRINTF("\r\nCaptured Period time=%d us\n", timediff * 1000 / counterClock);
    QTMR_DisableDma(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_InputEdgeFlagDmaEnable);

    DMAMUX_DisableChannel(EXAMPLE_QTMR_DMA_MUX, 0);
    DMAMUX_SetSource(EXAMPLE_QTMR_DMA_MUX, 0, QTMR_EDMA_REQUEST_CMPLD_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_QTMR_DMA_MUX, 0);

    PRINTF("\r\n****Output pwm dma example.****\n");
    PRINTF("\r\n*********Make sure to connect an oscilloscope.*********\n");
    PRINTF("\r\n****A 50% duty cycle PWM wave is observed on an oscilloscope.****\n");

    qtmrConfig.primarySource = kQTMR_ClockDivide_8;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, &qtmrConfig);

    /* Generate a 50Khz PWM signal with 50% dutycycle */
    QTMR_SetupPwm(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, 50000, 50, false, QTMR_SOURCE_CLOCK / 8);
    /* Enable comparator preload register 1 DMA */
    QTMR_EnableDma(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, kQTMR_ComparatorPreload1DmaEnable);
    /* Enable comparator preload register 2 DMA */
    QTMR_EnableDma(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, kQTMR_ComparatorPreload2DmaEnable);
    /* Start the counter */
    QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, kQTMR_PriSrcRiseEdge);
    while (1)
    {
        g_Transfer_Done = false;
        do
        {
            PRINTF("\r\nPlease enter a value to update the Duty cycle:\r\n");
            PRINTF("Note: The range of value is 1 to 9.\r\n");
            PRINTF("For example: If enter '5', the duty cycle will be set to 50 percent.\r\n");
            PRINTF("Value:");
            getCharValue = GETCHAR() - 0x30U;
            PRINTF("%d", getCharValue);
            PRINTF("\r\n");
        } while ((getCharValue > 9U) || (getCharValue == 0U));

        updatedDutycycle = getCharValue * 10U;
        QTMR_SetCmpldValue(50000, updatedDutycycle, QTMR_SOURCE_CLOCK / 8);
        EDMA_PrepareTransfer(&transferConfig, (uint16_t *)&g_Cmpld1Value, 2,
                             (uint16_t *)&BOARD_QTMR_BASEADDR->CHANNEL[BOARD_QTMR_PWM_CHANNEL].CMPLD1, 2, 2, 2,
                             kEDMA_MemoryToPeripheral);
        EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
        EDMA_StartTransfer(&g_EDMA_Handle);
        /* Wait for EDMA transfer finish */
        while (g_Transfer_Done != true)
        {
        }
        g_Transfer_Done = false;
        EDMA_PrepareTransfer(&transferConfig, (uint16_t *)&g_Cmpld2Value, 2,
                             (uint16_t *)&BOARD_QTMR_BASEADDR->CHANNEL[BOARD_QTMR_PWM_CHANNEL].CMPLD2, 2, 2, 2,
                             kEDMA_MemoryToPeripheral);
        EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
        EDMA_StartTransfer(&g_EDMA_Handle);
        /* Wait for EDMA transfer finish */
        while (g_Transfer_Done != true)
        {
        }
        PRINTF("The duty cycle was successfully updated!\r\n");
    }
}
