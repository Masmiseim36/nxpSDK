/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_debug_console.h"
#include "fsl_dspi.h"
#include "fsl_sdspi.h"
#include "fsl_gpio.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
* Definitions
******************************************************************************/
#define DSPI_MASTER_CLK_SRC DSPI1_CLK_SRC
#define DSPI_MASTER_PCS_CONFIG kDSPI_Pcs0
#define DSPI_MASTER_PCS_TRANSFER kDSPI_MasterPcs0
#define EXAMPLE_DSPI_MASTER_BASEADDR ((SPI_Type *)BOARD_SDSPI_SPI_BASE)

#define DSPI_MASTER_CTAR (kDSPI_Ctar0) /* The CTAR to describle the transfer attribute */
#define DSPI_BUS_BAUDRATE (500000U)    /* Transfer baudrate - 500k */

/* The block of the card to be accessed. */
#define DATA_BLOCK_COUNT (3U)
#define DATA_BLOCK_START (4U)

/* The value used to initialize the write buffer. */
#define INITIAL_DATA_VALUE_WRITE (0x17U)
/* The value used to initialize the read buffer. */
#define INITIAL_DATA_VALUE_READ (0U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Delay some time.
 *
 * @param milliseconds Some time united in milliseconds.
 */
void Delay(uint32_t milliseconds);

/*!
 * @brief Initialize the SPI interface.
 */
void InitSpi(void);

/*!
 * @brief Set the SPI bus frequency.
 *
 * @param frequency The frequency to set.
 * @retval kStatus_Success Success.
 * @retval kStatus_Fail Failed.
 */
status_t SetFrequency(uint32_t frequency);

/*!
 * @brief Transfer data over SPI bus in full-duplex way.
 *
 * @param in The buffer to save the data to be sent.
 * @param out The buffer to save the data to be read.
 * @param size The transfer data size.
 * @return The status of the function DSPI_MasterTransferPolling().
 */
status_t ExchangeData(uint8_t *in, uint8_t *out, uint32_t size);

/*!
 * @brief Initialize the timer to generator 1ms interrupt used to get current time in milliseconds.
 */
void InitTimer(void);

/*!
 * @brief Get current time in milliseconds.
 *
 * @return Current time in milliseconds.
 */
uint32_t GetCurrentMilliseconds(void);

/*!
 * @brief Initialize the host descriptor.
 */
void InitHost(void);

/*******************************************************************************
* Variables
******************************************************************************/
/* Save current time in milliseconds */
volatile uint32_t g_milliseconds;

/* SDSPI driver state. */
sdspi_host_t g_host;
sdspi_card_t g_card;

#define DATA_BUFFER_SIZE (FSL_SDSPI_DEFAULT_BLOCK_SIZE * DATA_BLOCK_COUNT)
uint8_t g_dataWrite[DATA_BUFFER_SIZE]; /* Data written to the card */
uint8_t g_dataRead[DATA_BUFFER_SIZE];  /* Data read from the card */

/*******************************************************************************
* Code
******************************************************************************/
/* Delay some time united in milliseconds. */
void Delay(uint32_t milliseconds)
{
    uint32_t i;
    uint32_t j;

    for (i = 0; i < milliseconds; i++)
    {
        for (j = 0; j < 20000U; j++)
        {
            __asm("NOP");
        }
    }
}

void InitSpi(void)
{
    uint32_t sourceClock;

    dspi_master_config_t masterConfig;

    /*Master config*/
    masterConfig.whichCtar = DSPI_MASTER_CTAR;
    masterConfig.ctarConfig.baudRate = DSPI_BUS_BAUDRATE;
    masterConfig.ctarConfig.bitsPerFrame = 8;
    masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
    masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
    masterConfig.ctarConfig.direction = kDSPI_MsbFirst;
    masterConfig.ctarConfig.pcsToSckDelayInNanoSec = 0;
    masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 0;
    masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 0;

    masterConfig.whichPcs = DSPI_MASTER_PCS_CONFIG;
    masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

    masterConfig.enableContinuousSCK = false;
    masterConfig.enableRxFifoOverWrite = false;
    masterConfig.enableModifiedTimingFormat = false;
    masterConfig.samplePoint = kDSPI_SckToSin0Clock;

    sourceClock = CLOCK_GetFreq(DSPI_MASTER_CLK_SRC);
    DSPI_MasterInit(EXAMPLE_DSPI_MASTER_BASEADDR, &masterConfig, sourceClock);
}

status_t SetFrequency(uint32_t frequency)
{
    uint32_t sourceClock;

    sourceClock = CLOCK_GetFreq(DSPI_MASTER_CLK_SRC);
    /* If returns 0, indicates failed. */
    if (DSPI_MasterSetBaudRate(EXAMPLE_DSPI_MASTER_BASEADDR, DSPI_MASTER_CTAR, frequency, sourceClock))
    {
        return kStatus_Success;
    }

    return kStatus_Fail;
}

status_t ExchangeData(uint8_t *in, uint8_t *out, uint32_t size)
{
    dspi_transfer_t masterTransfer;

    masterTransfer.txData = in;
    masterTransfer.rxData = out;
    masterTransfer.dataSize = size;
    masterTransfer.configFlags = (kDSPI_MasterCtar0 | DSPI_MASTER_PCS_TRANSFER | kDSPI_MasterPcsContinuous);
    return DSPI_MasterTransferBlocking(EXAMPLE_DSPI_MASTER_BASEADDR, &masterTransfer);
}

void InitTimer(void)
{
    /* Set systick reload value to generate 1ms interrupt */
    SysTick_Config(CLOCK_GetFreq(kCLOCK_CoreSysClk) / 1000U);
}

void SysTick_Handler(void)
{
    g_milliseconds++;
}

uint32_t GetCurrentMilliseconds(void)
{
    return g_milliseconds;
}

void InitHost(void)
{
    /* Save host state and callback. */
    g_host.busBaudRate = DSPI_BUS_BAUDRATE;
    g_host.setFrequency = SetFrequency;
    g_host.exchange = ExchangeData;
    g_host.getCurrentMilliseconds = GetCurrentMilliseconds;

    /* Save card state. */
    g_card.host = &g_host;
}

/*!
 * @brief Main function
 */
int main(void)
{
    sdspi_card_t *card = &g_card;
    bool failedFlag = false;
    char ch = '0';

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("SD card over SPI example start.\r\n\r\nPlease insert the SD card\r\n");
/* Wait the card to be inserted. */
#if defined BOARD_SDSPI_CD_LOGIC_RISING
    while (!(GPIO_ReadPinInput(BOARD_SDSPI_CD_GPIO_BASE, BOARD_SDSPI_CD_PIN)))
    {
    }
#else
    while (GPIO_ReadPinInput(BOARD_SDSPI_CD_GPIO_BASE, BOARD_SDSPI_CD_PIN))
    {
    }
#endif
    PRINTF("Detected SD card inserted.\r\n\r\n");
    /* Delat some time to make card stable. */
    Delay(1000U);

    InitTimer();
    InitSpi();
    InitHost();

    /* Initialize card. */
    if (kStatus_Success != SDSPI_Init(card))
    {
        PRINTF("SDSPI_Init failed\r\n");
        DSPI_Deinit(EXAMPLE_DSPI_MASTER_BASEADDR);
        return -1;
    }

    /* Read/Write card */
    PRINTF("Read/write SD card continuously until encounters error.\r\n");
    memset(g_dataWrite, INITIAL_DATA_VALUE_WRITE, sizeof(g_dataWrite));
    while (true)
    {
        if (failedFlag || (ch == 'q'))
        {
            break;
        }

        PRINTF("\r\nWrite/read one data block......\r\n");
        if (kStatus_Success != SDSPI_WriteBlocks(card, g_dataWrite, DATA_BLOCK_START, 1))
        {
            PRINTF("Write one data block failed.\r\n");
        }
        memset(g_dataRead, INITIAL_DATA_VALUE_READ, sizeof(g_dataRead));
        if (kStatus_Success != SDSPI_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, 1))
        {
            PRINTF("Read one data block failed.\r\n");
        }
        PRINTF("Compare the read/write content......\r\n");
        if (memcmp(g_dataWrite, g_dataRead, FSL_SDSPI_DEFAULT_BLOCK_SIZE))
        {
            PRINTF("The read/write content isn't consistent.\r\n\r\n");
            failedFlag = true;
            continue;
        }
        PRINTF("The read/write content is consistent.\r\n\r\n");

        PRINTF("Write/read multiple data blocks......\r\n");
        if (kStatus_Success != SDSPI_WriteBlocks(card, g_dataWrite, DATA_BLOCK_START, DATA_BLOCK_COUNT))
        {
            PRINTF("Write multiple data blocks failed.\r\n");
        }
        memset(g_dataRead, INITIAL_DATA_VALUE_READ, sizeof(g_dataRead));
        if (kStatus_Success != SDSPI_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, DATA_BLOCK_COUNT))
        {
            PRINTF("Read multiple data blocks failed.\r\n");
        }
        PRINTF("Compare the read/write content......\r\n");
        if (memcmp(g_dataWrite, g_dataRead, (FSL_SDSPI_DEFAULT_BLOCK_SIZE * DATA_BLOCK_COUNT)))
        {
            PRINTF("The read/write content isn't consistent.\r\n\r\n");
            failedFlag = true;
            continue;
        }
        PRINTF("The read/write content is consistent.\r\n\r\n");

        PRINTF("Input 'q' to quit read/write process.\r\nInput other char to read/write data blocks again.\r\n");
        ch = GETCHAR();
        PUTCHAR(ch);
    }
    PRINTF("\r\nThe example will not read/write data blocks again.\r\n");

    while (true)
    {
    }
}
