/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sdmmc_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
/*!brief sdmmc dma buffer */
SDK_ALIGN(static uint32_t s_sdmmcHostDmaBuffer[BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE],
          SDMMCHOST_DMA_DESCRIPTOR_BUFFER_ALIGN_SIZE);
#if defined(SDIO_ENABLED) || defined(SD_ENABLED)
static sd_detect_card_t s_cd;
#endif
static sdmmchost_t s_host;
OSA_EVENT_HANDLE_DEFINE(s_event);
/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(SDIO_ENABLED) || defined(SD_ENABLED)
bool BOARD_SDCardGetDetectStatus(void)
{
    return GPIO_PinRead(BOARD_SDMMC_SD_CD_GPIO_BASE, BOARD_SDMMC_SD_CD_GPIO_PIN) == BOARD_SDMMC_SD_CD_INSERT_LEVEL;
}

void BOARD_SDMMC_SD_CD_PORT_IRQ_HANDLER(void)
{
    if (PORT_GetPinsInterruptFlags(BOARD_SDMMC_SD_CD_PORT_BASE) & (1U << BOARD_SDMMC_SD_CD_GPIO_PIN))
    {
        if (s_cd.callback != NULL)
        {
            s_cd.callback(BOARD_SDCardGetDetectStatus(), s_cd.userData);
        }
    }
    /* Clear interrupt flag.*/
    PORT_ClearPinsInterruptFlags(BOARD_SDMMC_SD_CD_PORT_BASE, ~0U);
}

void BOARD_SDCardDetectInit(sd_cd_t cd, void *userData)
{
    /* install card detect callback */
    s_cd.cdDebounce_ms = BOARD_SDMMC_SD_CARD_DETECT_DEBOUNCE_DELAY_MS;
    s_cd.type          = BOARD_SDMMC_SD_CD_TYPE;
    s_cd.cardDetected  = BOARD_SDCardGetDetectStatus;
    s_cd.callback      = cd;
    s_cd.userData      = userData;
    /* Card detection pin will generate interrupt on either eage */
    PORT_SetPinInterruptConfig(BOARD_SDMMC_SD_CD_PORT_BASE, BOARD_SDMMC_SD_CD_GPIO_PIN,
                               BOARD_SDMMC_SD_CD_INTTERUPT_TYPE);
    /* set IRQ priority */
    NVIC_SetPriority(BOARD_SDMMC_SD_CD_PORT_IRQ, BOARD_SDMMC_SD_CD_IRQ_PRIORITY);
    /* Open card detection pin NVIC. */
    EnableIRQ(BOARD_SDMMC_SD_CD_PORT_IRQ);

    if (GPIO_PinRead(BOARD_SDMMC_SD_CD_GPIO_BASE, BOARD_SDMMC_SD_CD_GPIO_PIN) == BOARD_SDMMC_SD_CD_INSERT_LEVEL)
    {
        if (cd != NULL)
        {
            cd(true, userData);
        }
    }
}
#endif

#ifdef SD_ENABLED
void BOARD_SD_Config(void *card, sd_cd_t cd, uint32_t hostIRQPriority, void *userData)
{
    assert(card);

    s_host.dmaDesBuffer                                      = s_sdmmcHostDmaBuffer;
    s_host.dmaDesBufferWordsNum                              = BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE;
    ((sd_card_t *)card)->host                                = &s_host;
    ((sd_card_t *)card)->host->hostController.base           = BOARD_SDMMC_SD_HOST_BASEADDR;
    ((sd_card_t *)card)->host->hostController.sourceClock_Hz = CLOCK_GetFreq(kCLOCK_CoreSysClk);

    ((sd_card_t *)card)->host->hostEvent = &s_event;
    ((sd_card_t *)card)->usrParam.cd     = &s_cd;

    BOARD_SDCardDetectInit(cd, userData);

    NVIC_SetPriority(BOARD_SDMMC_SD_HOST_IRQ, hostIRQPriority);
}
#endif
