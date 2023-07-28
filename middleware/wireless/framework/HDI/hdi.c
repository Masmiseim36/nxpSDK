/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *********************************************************************************
 * Include
 ********************************************************************************** */

#include "fsl_common.h"
#include "EmbeddedTypes.h"
#include "fsl_lpspi.h"
#include "hdi.h"
#include "board.h"

extern uint32_t BOARD_GetSpiClock(uint32_t instance);
/*! *********************************************************************************
 * Public macros
 ********************************************************************************** */

/*! *********************************************************************************
 * Private macros
 ********************************************************************************** */

#define HDI_SPI_BASE     LPSPI0
#define HDI_SPI_IRQn     LPSPI0_IRQn
#define HDI_SPI_PRIO     (0x40u)
#define HDI_SPI_BAUDRATE (100000)

#define SPI_CMD_DUMMY (0x0u)

#define HDI_CHAN_CMD_MASK  (0x80U)
#define HDI_CHAN_VAL_MASK  (0x7FU)
#define HDI_CHAN_CMD_SHIFT (0U)
#define HDI_BUILD_CHAN_CMD(x) \
    ((((uint32_t)(((uint32_t)(x)) << HDI_CHAN_CMD_SHIFT)) & HDI_CHAN_VAL_MASK) | HDI_CHAN_CMD_MASK)
#define HDI_READ_CHAN_VAL(x) (((uint32_t)(((uint32_t)(x)) & HDI_CHAN_VAL_MASK)) >> HDI_CHAN_CMD_SHIFT)
#define HDI_IS_CHAN_CMD(x)   ((uint32_t)(((uint32_t)(x)) & HDI_CHAN_CMD_MASK))

#define HDI_CFG_CMD_MASK  (0x400U)
#define HDI_CFG_VAL_MASK  (0x300U)
#define HDI_CFG_CMD_SHIFT (8U)
#define HDI_BUILD_CFG_CMD(x) \
    ((((uint32_t)(((uint32_t)(x)) << HDI_CFG_CMD_SHIFT)) & HDI_CFG_VAL_MASK) | HDI_CFG_CMD_MASK)
#define HDI_READ_CFG_VAL(x) (((uint32_t)(((uint32_t)(x)) & HDI_CFG_VAL_MASK)) >> HDI_CFG_CMD_SHIFT)
#define HDI_IS_CFG_CMD(x)   ((uint32_t)(((uint32_t)(x)) & HDI_CFG_CMD_MASK))

/*! *********************************************************************************
 * Private prototypes
 ********************************************************************************** */
static lpspi_master_config_t HdiSpiConfig;
/*! *********************************************************************************
 * Private functions
 ********************************************************************************** */

/*! *********************************************************************************
 * Public type definitions
 ********************************************************************************** */

/*! *********************************************************************************
 * Private type definitions
 ********************************************************************************** */

/*! *********************************************************************************
 * Public memory declarations
 ********************************************************************************** */

/*! *********************************************************************************
 * Public functions
 ********************************************************************************** */
#define HDI_LOGSIZE 64

#define HDI_CS_LESS 0

extern uint32_t   BleTimerTimestamp(void);
volatile uint32_t fifo_size;

void HDI_Init(void)
{
    LPSPI_MasterGetDefaultConfig(&HdiSpiConfig);

    // HdiSpiConfig.dataOutConfig = kLpspiDataOutTristate;
    HdiSpiConfig.baudRate     = HDI_SPI_BAUDRATE;
    HdiSpiConfig.bitsPerFrame = 11;
    /* Define CS even if not used externaly */
    HdiSpiConfig.whichPcs           = kLPSPI_Pcs0;
    HdiSpiConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

    /* CPOL = 0 CPHA=1 for CSless operation */
    HdiSpiConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
    HdiSpiConfig.cpha = kLPSPI_ClockPhaseSecondEdge;

    HdiSpiConfig.pcsToSckDelayInNanoSec        = 1000000000 / HdiSpiConfig.baudRate;
    HdiSpiConfig.lastSckToPcsDelayInNanoSec    = 1000000000 / HdiSpiConfig.baudRate;
    HdiSpiConfig.betweenTransferDelayInNanoSec = 1000000000 / HdiSpiConfig.baudRate;

    LPSPI_MasterInit(HDI_SPI_BASE, &HdiSpiConfig, BOARD_GetSpiClock(0));
    LPSPI_FlushFifo(HDI_SPI_BASE, true, true);
    LPSPI_DisableInterrupts(HDI_SPI_BASE, kLPSPI_AllInterruptEnable);
    fifo_size = LPSPI_GetTxFifoSize(HDI_SPI_BASE);
}

static void send_to_spi(uint32_t word)
{
    /* Wait if TX fifo is full */
    while (LPSPI_GetTxFifoCount(HDI_SPI_BASE) == fifo_size)
        __asm("nop");
#if HDI_CS_LESS
    /* Dummy word to make sure CSless operation is ok */
    LPSPI_WriteData(HDI_SPI_BASE, 0x55);
#endif
    while (LPSPI_GetTxFifoCount(HDI_SPI_BASE) == fifo_size)
        __asm("nop");
    LPSPI_WriteData(HDI_SPI_BASE, word);
    /* Flush RX fifo to avoid overrun */
    LPSPI_FlushFifo(HDI_SPI_BASE, FALSE, TRUE);
}

void HDI_SendChannelSwitchCmd(uint32_t channel)
{
    uint32_t cmd;

    cmd = HDI_BUILD_CHAN_CMD(channel);
    send_to_spi(cmd);
}

void HDI_SendRadioConfigCmd(hdi_radio_mode_t mode)
{
    uint32_t cmd;

    cmd = HDI_BUILD_CFG_CMD(mode);
    send_to_spi(cmd);
}

void HDI_IpcInterface(void *buffer, uint32_t size)
{
    assert(size <= 4U);
    assert(buffer != NULL);
    HDI_SendChannelSwitchCmd(*(uint32_t *)buffer);
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
