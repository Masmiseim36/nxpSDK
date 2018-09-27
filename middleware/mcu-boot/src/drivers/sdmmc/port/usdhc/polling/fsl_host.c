/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_host.h"
//#include "board.h"
#include "gpio/fsl_gpio.h"
//#include "event.h"
#ifdef BOARD_USDHC_CD_PORT_BASE
#include "fsl_port.h"
#endif
#include "microseconds/microseconds.h"
/*******************************************************************************
* Definitions
******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief host controller error recovery.
 * @param host base address.
 */
static void Host_ErrorRecovery(HOST_TYPE *hostBase);
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* DMA descriptor should allocate at non-cached memory */
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t g_usdhcAdma2Table[USDHC_ADMA_TABLE_WORDS], USDHC_ADMA2_ADDR_ALIGN);
extern volatile uint32_t g_timeMilliseconds;
static volatile bool g_sdInsertedFlag;
/*******************************************************************************
 * Code
 ******************************************************************************/
/* Delay some time united in milliseconds. */
// static void Delay(uint32_t milliseconds)
//{
//    uint32_t startTime = g_timeMilliseconds;
//    uint32_t periodTime = 0;
//    while (periodTime < milliseconds)
//    {
//        periodTime = g_timeMilliseconds - startTime;
//    }
//}

static void DetectCardByGpio(void)
{
    if (HOST_CARD_DETECT_STATUS() != HOST_CARD_INSERT_CD_LEVEL)
    {
        g_sdInsertedFlag = false;
    }
    else
    {
        g_sdInsertedFlag = true;
    }
}

/* Card detect. */
status_t CardInsertDetect(HOST_TYPE *hostBase)
{
    //    if (!EVENT_Create(kEVENT_CardDetect))
    //    {
    //        return kStatus_Fail;
    //    }

    HOST_CARD_DETECT_INIT();
    HOST_ENABLE_SD_POWER(false);
    /* Open card detection pin NVIC. */
    //    EnableIRQ(HOST_CARD_DETECT_IRQ);

    microseconds_set_delay(EVENT_TIMEOUT_CARD_DETECT);

    do
    {
        DetectCardByGpio();
        if (microseconds_timeout())
        {
            // Final check when timeout reaches.
            if (!g_sdInsertedFlag)
            {
                return kStatus_Fail;
            }
        }
    } while (!g_sdInsertedFlag);

    //    EVENT_Delete(kEVENT_CardDetect);

    /* Delay some time to make card stable. */
    microseconds_delay(500U);
    /* card power on */
    HOST_ENABLE_SD_POWER(true);
    /* Delay some time to make card stable. */
    microseconds_delay(500U);

    return kStatus_Success;
}

/* Card detect pin port interrupt handler. */
// void HOST_CARD_DETECT_INTERRUPT_HANDLER(void)
//{
//    if (HOST_CARD_DETECT_INTERRUPT_STATUS() & (1U << BOARD_USDHC_CD_GPIO_PIN))
//    {
//        DetectCardByGpio();
//    }
//    /* Clear interrupt flag.*/
//    HOST_CARD_DETECT_INTERRUPT_CLEAR(~0U);
//    EVENT_Notify(kEVENT_CardDetect);
//}

/* User defined transfer function. */
static status_t USDHC_TransferFunction(USDHC_Type *base, usdhc_transfer_t *content)
{
    usdhc_adma_config_t dmaConfig;
    status_t error = kStatus_Success;

    if (content->data != NULL)
    {
        memset(&dmaConfig, 0, sizeof(usdhc_adma_config_t));
        /* config adma */
        dmaConfig.dmaMode = USDHC_DMA_MODE;
        dmaConfig.burstLen = kUSDHC_EnBurstLenForINCR;
        dmaConfig.admaTable = g_usdhcAdma2Table;
        dmaConfig.admaTableWords = USDHC_ADMA_TABLE_WORDS;
    }

    error = USDHC_TransferBlocking(base, &dmaConfig, content);

    if (error == kStatus_Fail)
    {
        /* host error recovery */
        Host_ErrorRecovery(base);
    }

    return error;
}

static void Host_ErrorRecovery(HOST_TYPE *hostBase)
{
    uint32_t status = 0U;
    /* get host present status */
    status = USDHC_GetPresentStatusFlags(hostBase);
    /* check command inhibit status flag */
    if ((status & kUSDHC_CommandInhibitFlag) != 0U)
    {
        /* reset command line */
        USDHC_Reset(hostBase, kUSDHC_ResetCommand, 1000U);
    }
    /* check data inhibit status flag */
    if ((status & kUSDHC_DataInhibitFlag) != 0U)
    {
        /* reset data line */
        USDHC_Reset(hostBase, kUSDHC_ResetData, 1000U);
    }
}

status_t HOST_Init(void *host)
{
    usdhc_host_t *usdhcHost = (usdhc_host_t *)host;

    /* init card power control */
    // HOST_INIT_SD_POWER();
    // HOST_INIT_MMC_POWER();

    /* Initializes SDHC. */
    usdhcHost->config.dataTimeout = USDHC_DATA_TIMEOUT;
    usdhcHost->config.endianMode = USDHC_ENDIAN_MODE;
    usdhcHost->config.readWatermarkLevel = USDHC_READ_WATERMARK_LEVEL;
    usdhcHost->config.writeWatermarkLevel = USDHC_WRITE_WATERMARK_LEVEL;
    usdhcHost->config.readBurstLen = USDHC_READ_BURST_LEN;
    usdhcHost->config.writeBurstLen = USDHC_WRITE_BURST_LEN;

    USDHC_Init(usdhcHost->base, &(usdhcHost->config));

    /* Define transfer function. */
    usdhcHost->transfer = USDHC_TransferFunction;
    /* event init timer */
    // EVENT_InitTimer();

    return kStatus_Success;
}

void HOST_Reset(HOST_TYPE *hostBase)
{
    /* voltage switch to normal but not 1.8V */
    HOST_SWITCH_VOLTAGE180V(hostBase, false);
    /* Disable DDR mode */
    HOST_ENABLE_DDR_MODE(hostBase, false);
    /* disable tuning */
    HOST_EXECUTE_STANDARD_TUNING_ENABLE(hostBase, false);
    /* Disable HS400 mode */
    HOST_ENABLE_HS400_MODE(hostBase, false);
    /* Disable DLL */
    HOST_ENABLE_STROBE_DLL(hostBase, false);
}

void HOST_Deinit(void *host)
{
    usdhc_host_t *usdhcHost = (usdhc_host_t *)host;
    USDHC_Deinit(usdhcHost->base);
}
