/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "lcd.h"
#include "fsl_dspi.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_DSPI_MASTER_BASE (SPI0)
#define EXAMPLE_DSPI_MASTER_PCS_NUMBER (2U)
#define VERIFY_PATTERN (0x5A)
#define DSPI_TRANSFER_CTAR (kDSPI_Ctar0)  /* The CTAR to describle the transfer attribute */
#define DSPI_TRANSFER_BAUDRATE (1000000U) /* Transfer baudrate - 1M */
#define DSPI_MASTER_CLK_SRC (DSPI0_CLK_SRC)
#define DSPI_MASTER_CLK_FREQ CLOCK_GetFreq((DSPI0_CLK_SRC))
uint8_t g_msg[] =
    "DSPI_LCD demo\r\n"
    "Show freescale logo in LCD.\r\n";

extern const unsigned char Freescale_logo[];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void DSPI_Init(void);

/*******************************************************************************
* Variables
******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void DSPI_Init(void)
{
    uint32_t sourceClock;

    dspi_master_config_t masterConfig;
    memset((void *)&masterConfig, 0, sizeof(masterConfig));

    /*Master config*/
    masterConfig.whichCtar = kDSPI_Ctar0;
    masterConfig.ctarConfig.baudRate = DSPI_TRANSFER_BAUDRATE;
    masterConfig.ctarConfig.bitsPerFrame = 8;
    masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
    masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
    masterConfig.ctarConfig.direction = kDSPI_MsbFirst;
    masterConfig.ctarConfig.pcsToSckDelayInNanoSec = 0;
    masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 0;
    masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 0;
    masterConfig.whichPcs = kDSPI_Pcs2;
    masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;
    masterConfig.enableContinuousSCK = false;
    masterConfig.samplePoint = kDSPI_SckToSin0Clock;

    sourceClock = DSPI_MASTER_CLK_FREQ;
    DSPI_MasterInit(EXAMPLE_DSPI_MASTER_BASE, &masterConfig, sourceClock);
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();

    gpio_pin_config_t pin_config;
    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1;
    GPIO_PinInit(PTC, 1, &pin_config);

    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();

    PRINTF("%s", g_msg);

    DSPI_Init();
    LCD_Initialize();

    LCD_FillAll((unsigned char *)Freescale_logo);

    while (1)
    {
    }
}
