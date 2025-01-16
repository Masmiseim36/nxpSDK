/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_semc.h"
#include "fsl_xecc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SEMC_EXAMPLE_DATALEN (0x1000U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern status_t BOARD_InitSEMC(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool s_xecc_multi_error = false;
static volatile bool s_bus_fault_flag   = false;
volatile uint32_t sdram_writeBuffer[SEMC_EXAMPLE_DATALEN];
volatile uint32_t sdram_readBuffer[SEMC_EXAMPLE_DATALEN];

/*******************************************************************************
 * Code
 ******************************************************************************/
void EXAMPLE_XECC_BUSFAULT_HANDLER(void)
{
    /* BusFault is active and XECC multiple error interrupt is pending. */
    s_bus_fault_flag = true;
}

void EXAMPLE_XECC_IRQ_HANDLER(void)
{
    uint32_t intStatus;

    intStatus = XECC_GetStatusFlags(EXAMPLE_XECC);
    XECC_ClearStatusFlags(EXAMPLE_XECC, intStatus);

    if ((intStatus & (uint32_t)kXECC_MultiErrorInterruptFlag) != 0x00U)
    {
        s_xecc_multi_error = true;
    }
    SDK_DelayAtLeastUs(100000, SystemCoreClock);

    /* Avoid to loop between multiple error interrupt and BusFault infinitely*/
    XECC_Deinit(EXAMPLE_XECC);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t multiErrorData;
    uint32_t uncorrectedData;
    uint32_t sdramAddress    = EXAMPLE_SEMC_START_ADDRESS;
    volatile uint32_t *sdram = (uint32_t *)EXAMPLE_SEMC_START_ADDRESS; /* SDRAM start address. */
    bool errorFlag           = false;
    xecc_config_t config;
    xecc_multi_error_info_t info;

    /* Hardware initialize. */
    BOARD_InitHardware();

    XECC_Deinit(EXAMPLE_XECC);

    PRINTF("XECC Multiple Errors Example Start!\r\n");
    if (BOARD_InitSEMC() != kStatus_Success)
    {
        PRINTF("\r\n SEMC SDRAM Init Failed\r\n");
    }

    /* Get default configuration */
    XECC_GetDefaultConfig(&config);
    /* Enable ECC function for write and read */
    config.enableXECC     = true;
    config.enableWriteECC = true;
    config.enableReadECC  = true;

    /* Set ECC regions */
    config.Region0BaseAddress = EXAMPLE_SEMC_START_ADDRESS;
    config.Region0EndAddress  = EXAMPLE_SEMC_START_ADDRESS + EXAMPLE_XECC_AREA_SIZE;
    config.Region1BaseAddress = EXAMPLE_SEMC_START_ADDRESS + 0X2000U;
    config.Region1EndAddress  = EXAMPLE_SEMC_START_ADDRESS + 0X2000U + EXAMPLE_XECC_AREA_SIZE;
    config.Region2BaseAddress = EXAMPLE_SEMC_START_ADDRESS + 0x4000U;
    config.Region2EndAddress  = EXAMPLE_SEMC_START_ADDRESS + 0x4000U + EXAMPLE_XECC_AREA_SIZE;
    config.Region3BaseAddress = EXAMPLE_SEMC_START_ADDRESS + 0x6000U;
    config.Region3EndAddress  = EXAMPLE_SEMC_START_ADDRESS + 0x6000U + EXAMPLE_XECC_AREA_SIZE;

    /* Initialize XECC */
    XECC_Init(EXAMPLE_XECC, &config);
    /* Enable IRQ */
    EnableIRQ(EXAMPLE_XECC_IRQ);

    /*
     * When multiple bits error is occurred for read operation on xecc block, the xecc will generate
     * a fatal interrupt if xecc interrupt enable is active. At the same time, xecc will also output
     * an read response error signal on axi bus to the core. If the BusFault interrupt is enabled in NVIC,
     * then the core will also receive an bus fault interrupt (interrupt_number=5), which is more higher priority
     * than xecc fatal error interrupt. The core will response the BusFault interrupt firstly, then the xecc interrupt
     * ISR.
     */
    SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;

    /* Enable XECC multiple error interrupt */
    XECC_EnableInterrupts(EXAMPLE_XECC, kXECC_MultiErrorInterruptStatusEnable);

    multiErrorData       = 0x03;
    sdram_writeBuffer[0] = 0xDDCCBBAAU;
    uncorrectedData      = 0xDDCCBBA9U;

    /* Multiple error injection. */
    XECC_ErrorInjection(EXAMPLE_XECC, multiErrorData, 0);

    /* Write data into SDRAM. */
    sdram[0] = sdram_writeBuffer[0];

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    /* Disable D cache to avoid cache pre-fetch more data from external memory, which include ECC data.
       Otherwise, XECC will decode ECC data itself and generate another error interrupt. */
#if (defined __CORTEX_M) && (__CORTEX_M == 7U)
    SCB_DisableDCache();
#elif (defined __CORTEX_M) && (__CORTEX_M == 4U)
    L1CACHE_DisableSystemCache();
#endif
#endif

    /* Read data from SDRAM. */
    sdram_readBuffer[0] = sdram[0];

    /* Waiting for multiple error interrupt */
    while (!s_xecc_multi_error)
    {
    }

    if (s_bus_fault_flag)
    {
        PRINTF("First level BusFault interrupt finished.\r\n");
    }
    else
    {
        PRINTF("First level BusFault interrupt failed.\r\n");
    }

    /* Verify multiple error with injection operation. */
    (void)XECC_GetMultiErrorInfo(EXAMPLE_XECC, &info);

    if ((sdramAddress == info.multiErrorAddress) && (uncorrectedData == info.multiErrorData))
    {
        PRINTF("Uncorrecdted read data: 0x%x \r\n", uncorrectedData);
        PRINTF("Multiple error address: 0x%x \r\n", info.multiErrorAddress);
        PRINTF("Multiple error read data: 0x%x \r\n", info.multiErrorData);
        PRINTF("Multiple error ECC code: 0x%x \r\n", info.multiErrorEccCode);
        PRINTF("Multiple error bit field: 0x%x \r\n", info.multiErrorBitField);
    }
    else
    {
        errorFlag = true;
    }

    if (errorFlag)
    {
        PRINTF("XECC multiple errors example failed!\r\n");
    }
    else
    {
        PRINTF("XECC multiple errors example successfully!\r\n");
    }

    while (1)
    {
    }
}
