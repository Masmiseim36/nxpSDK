/*
 * The Clear BSD License
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
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
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "spi_flash_driver.h"
#include "fsl_common.h"
#include "fsl_dspi.h"
#include "fsl_debug_console.h"

#include <stdbool.h>
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_DSPI_MASTER_PCS_NUMBER (4U)
#define VERIFY_PATTERN 0x5A
#define DSPI_TRANSFER_CTAR (kDSPI_Ctar0)  /* The CTAR to describle the transfer attribute */
#define DSPI_TRANSFER_BAUDRATE (1000000U) /* Transfer baudrate - 1M */
#define DSPI_MASTER_CLK_SRC (DSPI0_CLK_SRC)
#define DSPI_MASTER_CLK_FREQ CLOCK_GetFreq((DSPI0_CLK_SRC))

/*******************************************************************************
* Prototypes
******************************************************************************/

/*!
 * @brief Initializes the SPI instance used by the demo.
 */
void SPI_Init(void);
/*!
 * @brief Shows the error logs.
 */
void ErrorTrap(void);
void DSPI_MasterUserCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData);

/*!
 * @brief Show the menu for user.
 *
 */
static uint8_t show_menu(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

dspi_master_handle_t g_m_handle;
volatile bool isDspiTransferCompleted = false;

static char s_StrInputAddress[] = "\r\nInput the %s address(HEX), 0x";
static char s_StrInputLength[] = "\r\nInput the %s length:";
static char s_StrInvalid[] = "Invalid input!\r\n";
static char s_StrEraseFail[] = "Erase failed!\r\n";
static char s_StrEraseOk[] = "Erase OK!\r\n";

/*******************************************************************************
 * Code
 ******************************************************************************/
void DSPI_MasterUserCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData)
{
    isDspiTransferCompleted = true;
}

static uint8_t show_menu(void)
{
    char input;

    PRINTF("\r\n1 - Erase entire chip\r\n");
    PRINTF("2 - Erase sectors\r\n");
    PRINTF("3 - Erase block\r\n");
    PRINTF("4 - Program one page with pattern (0x%x) and verify\r\n", VERIFY_PATTERN);
    PRINTF("5 - Read byte\r\n");
    PRINTF("Please enter your choice (1-5): ");

    input = GETCHAR();
    PRINTF("%c\r\n", input);

    return input - '0';
}

/*Error trap function*/
void ErrorTrap(void)
{
    PRINTF("\n\rError Occured. Please check the configurations.\n\r");
    while (1)
    {
        ;
    }
}

void SPI_Init(void)
{
    uint32_t sourceClock;

    dspi_master_config_t masterConfig;

    /* Get default Master configuration */
    /*
     * masterConfig.whichPcs = kDSPI_Pcs0;
     * masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;
     * masterConfig.enableContinuousSCK = false;
     * masterConfig.enableRxFifoOverWrite = false;
     * masterConfig.enableModifiedTimingFormat = false;
     * masterConfig.samplePoint = kDSPI_SckToSin0Clock;
     */
    DSPI_MasterGetDefaultConfig(&masterConfig);
    sourceClock = DSPI_MASTER_CLK_FREQ;
    DSPI_MasterInit(EXAMPLE_DSPI_MASTER_BASE, &masterConfig, sourceClock);

    /* Set up master transfer */
    DSPI_MasterTransferCreateHandle(EXAMPLE_DSPI_MASTER_BASE, &g_m_handle, DSPI_MasterUserCallback, NULL);
}

/*!
 * @brief Main function
 */
int main(void)
{
    char input, buf[FLASH_PAGE_SIZE];
    uint32_t ret;
    uint32_t blkSize;
    uint8_t vendorId = 0U;
    uint8_t devId[2];
    uint32_t offset = 0x00U;
    uint32_t length = 0x00U;
    uint32_t i = 0U;

    BOARD_InitPins();
    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();

    /* Sets the FLASH PCS as low logic */
    gpio_pin_config_t csPinConfig = {
        kGPIO_DigitalOutput, 0,
    };
    GPIO_PinInit(GPIOC, 4U, &csPinConfig);

    PRINTF("\r\n***SPI Flash Demo***\r\n");

    SPI_Init();

    ret = spi_flash_readid(&vendorId, devId);

    if (ret)
    {
        PRINTF("Can not find any SPI Flash device!\r\n");
        ErrorTrap();
    }

    /* check if it's GD25Q16B chip or not */
    if (SPI_FLASH_WINBOND_VENDER_ID == vendorId)
    {
        if (spi_flash_init_winbond(FLASH_PAGE_SIZE, FLASH_SECTOR_SIZE, 256))
        {
            PRINTF("Failed to init SPI flash chip!\r\n");
            ErrorTrap();
        }
    }
    else
    {
        PRINTF("Not a WinBond SPI flash chip! VID:%x\r\n", vendorId);
        ErrorTrap();
    }

    while (1)
    {
        /* showing the UI */
        input = show_menu();

        offset = 0x00U;

        switch (input)
        {
            case 1:
                /* erase all chips */
                ret = spi_flash_erase_all();
                if (ret)
                {
                    PRINTF(s_StrEraseFail);
                }
                else
                {
                    PRINTF(s_StrEraseOk);
                }
                break;

            case 2:
                PRINTF(s_StrInputAddress, "sector");
                SCANF("%x", &offset);

                PRINTF(s_StrInputLength, "sector");
                SCANF("%d", &length);
                PRINTF("\r\n");

                if ((offset >= FLASH_TOTAL_SIZE) || (length <= 0))
                {
                    PRINTF(s_StrInvalid);
                    break;
                }

                /* erase sector */
                ret = spi_flash_erase_sector(offset, length);
                if (ret)
                {
                    PRINTF(s_StrEraseFail);
                }
                else
                {
                    PRINTF(s_StrEraseOk);
                }
                break;

            case 3:
                PRINTF(s_StrInputAddress, "block");
                SCANF("%x", &offset);
                PRINTF("\r\n1. 32K block size");
                PRINTF("\r\n2. 64K block size");
                PRINTF("\r\nSelect the block size:");
                blkSize = GETCHAR();
                blkSize -= '0';
                PRINTF("%d\r\n", blkSize);

                if ((offset >= FLASH_TOTAL_SIZE) || ((blkSize != 1) && (blkSize != 2)))
                {
                    PRINTF(s_StrInvalid);
                    break;
                }

                /* erase one block */
                ret = spi_flash_erase_block(offset, blkSize == 1 ? ERASE_32K_SIZE : ERASE_64K_SIZE);
                if (ret)
                {
                    PRINTF(s_StrEraseFail);
                }
                else
                {
                    PRINTF(s_StrEraseOk);
                }
                break;

            case 4:
                /* get the program address and length */
                PRINTF(s_StrInputAddress, "program");
                SCANF("%x", &offset);
                PRINTF("\r\n");

                if (offset > (FLASH_TOTAL_SIZE - FLASH_PAGE_SIZE))
                {
                    PRINTF(s_StrInvalid);
                    break;
                }

                /* erase sectors first */
                ret = spi_flash_erase_sector((offset & ~(FLASH_SECTOR_SIZE - 1)), FLASH_SECTOR_SIZE);
                if (ret)
                {
                    PRINTF(s_StrEraseFail);
                    break;
                }

                /* fill the write buffer with pattern */
                memset(buf, VERIFY_PATTERN, FLASH_PAGE_SIZE);
                ret = spi_flash_write(offset, FLASH_PAGE_SIZE, (void *)buf);
                if (ret)
                {
                    PRINTF("Program page failed!\r\n");
                    break;
                }

                /* read back and verify */
                memset(buf, 0, FLASH_PAGE_SIZE);
                ret = spi_flash_read(offset, FLASH_PAGE_SIZE, (void *)buf);
                if (ret)
                {
                    PRINTF("Read page failed!\r\n");
                    break;
                }

                /* compare */
                for (i = 0; i < FLASH_PAGE_SIZE; i++)
                {
                    if (buf[i] != VERIFY_PATTERN)
                    {
                        PRINTF("Program and verify failed!\r\n");
                        break;
                    }
                }

                PRINTF("Program and verify done!\r\n");
                break;

            case 5:
                /* get the read address */
                PRINTF(s_StrInputAddress, "read");
                SCANF("%x", &offset);
                PRINTF("\r\n");

                if (offset >= FLASH_TOTAL_SIZE)
                {
                    PRINTF(s_StrInvalid);
                    break;
                }

                ret = spi_flash_read(offset, 1, (void *)buf);
                if (ret)
                {
                    PRINTF("Read byte failed!\r\n");
                    break;
                }
                PRINTF("0x%x = 0x%x\r\n", offset, buf[0]);
                break;

            default:
                PRINTF(s_StrInvalid);
                break;
        }
    }
}
