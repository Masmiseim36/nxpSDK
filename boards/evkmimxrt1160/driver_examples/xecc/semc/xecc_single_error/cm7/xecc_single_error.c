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
static volatile bool s_xecc_single_error = false;
volatile uint32_t sdram_writeBuffer[SEMC_EXAMPLE_DATALEN];
volatile uint32_t sdram_readBuffer[SEMC_EXAMPLE_DATALEN];

/*******************************************************************************
 * Code
 ******************************************************************************/
void EXAMPLE_XECC_IRQ_HANDLER(void)
{
    uint32_t intStatus;

    intStatus = XECC_GetStatusFlags(EXAMPLE_XECC);
    XECC_ClearStatusFlags(EXAMPLE_XECC, intStatus);

    if ((intStatus & (uint32_t)kXECC_SingleErrorInterruptFlag) != 0x00U)
    {
        s_xecc_single_error = true;
    }
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t singleErrorData;
    uint32_t correctedData;
    uint32_t uncorrectedData;
    uint32_t bitPostion;
    uint32_t sdramAddress    = EXAMPLE_SEMC_START_ADDRESS;
    volatile uint32_t *sdram = (uint32_t *)EXAMPLE_SEMC_START_ADDRESS; /* SDRAM start address. */
    bool errorFlag           = false;
    xecc_config_t config;
    xecc_single_error_info_t info;

    /* Hardware initialize. */
    BOARD_InitHardware();

    XECC_Deinit(EXAMPLE_XECC);

    PRINTF("XECC Single Error Example Start!\r\n");
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
    (void)EnableIRQ(EXAMPLE_XECC_IRQ);
    /* Enable XECC single error interrupt */
    XECC_EnableInterrupts(EXAMPLE_XECC, kXECC_SingleErrorInterruptEnable);

    /* Single error injection. */
    singleErrorData = 0x04;
    XECC_ErrorInjection(EXAMPLE_XECC, singleErrorData, 0);

    sdram_writeBuffer[0] = 0xDDCCBBAAU;

    /* Back up correct/uncorrect data. */
    correctedData   = sdram_writeBuffer[0];
    uncorrectedData = 0xDDCCBBAEU;

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

    /* Waiting for single error interrupt */
    while (!s_xecc_single_error)
    {
    }

    /* Calculate single error bit position */
    bitPostion = singleErrorData;

    /* Verify single error with injection operation. */
    XECC_GetSingleErrorInfo(EXAMPLE_XECC, &info);

    if ((sdramAddress == info.singleErrorAddress) && (uncorrectedData == info.singleErrorData) &&
        (bitPostion == info.singleErrorBitPos))
    {
        PRINTF("Correcdted read data: 0x%x \r\n", correctedData);
        PRINTF("Uncorrecdted read data: 0x%x \r\n", uncorrectedData);
        PRINTF("Single error address: 0x%x \r\n", info.singleErrorAddress);
        PRINTF("Single error read data: 0x%x \r\n", info.singleErrorData);
        PRINTF("Single error ECC code: 0x%x \r\n", info.singleErrorEccCode);
        PRINTF("Single error bit postion: 0x%x \r\n", info.singleErrorBitPos);
        PRINTF("Single error bit field: 0x%x \r\n", info.singleErrorBitField);
    }
    else
    {
        errorFlag = true;
    }

    if (errorFlag)
    {
        PRINTF("XECC single error example failed!\r\n");
    }
    else
    {
        PRINTF("XECC single error example successfully!\r\n");
    }

    while (1)
    {
    }
}
