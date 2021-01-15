/* mbed Microcontroller Library
 * Copyright (c) 2018-2020 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "port_storage.h"

#include "flexspi_nor_flash_ops.h"

#include "fsl_flexspi.h"
#include "fsl_cache.h"
#include "fsl_iomuxc.h"
#include "fsl_common.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#if 0
#include "fsl_debug_console.h"
#define DEBUG_PRINT(...) PRINTF(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define CACHE_LINE_SIZE (0x32)
#define CACHE_LINE_MASK ~(CACHE_LINE_SIZE - 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int32_t port_storage_init(void)
{
    DEBUG_PRINT("port_storage_init\r\n");

    static int initialized = 0;

    if (!initialized) {
        initialized = 1;

        /* Configure pins */
        CLOCK_EnableClock(kCLOCK_Iomuxc);         /* iomuxc clock (iomuxc_clk_enable): 0x03u */

        IOMUXC_SetPinMux(
          IOMUXC_GPIO_SD_B1_05_FLEXSPIA_DQS,      /* GPIO_SD_B1_05 is configured as FLEXSPIA_DQS */
          1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_05 */
        IOMUXC_SetPinMux(
          IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B,    /* GPIO_SD_B1_06 is configured as FLEXSPIA_SS0_B */
          1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_06 */
        IOMUXC_SetPinMux(
          IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK,     /* GPIO_SD_B1_07 is configured as FLEXSPIA_SCLK */
          1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_07 */
        IOMUXC_SetPinMux(
          IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00,   /* GPIO_SD_B1_08 is configured as FLEXSPIA_DATA00 */
          1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_08 */
        IOMUXC_SetPinMux(
          IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01,   /* GPIO_SD_B1_09 is configured as FLEXSPIA_DATA01 */
          1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_09 */
        IOMUXC_SetPinMux(
          IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02,   /* GPIO_SD_B1_10 is configured as FLEXSPIA_DATA02 */
          1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_10 */
        IOMUXC_SetPinMux(
          IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03,   /* GPIO_SD_B1_11 is configured as FLEXSPIA_DATA03 */
          1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_11 */

        IOMUXC_SetPinConfig(
          IOMUXC_GPIO_SD_B1_05_FLEXSPIA_DQS,      /* GPIO_SD_B1_05 PAD functional properties : */
          0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                     Drive Strength Field: R0/6
                                                     Speed Field: max(200MHz)
                                                     Open Drain Enable Field: Open Drain Disabled
                                                     Pull / Keep Enable Field: Pull/Keeper Enabled
                                                     Pull / Keep Select Field: Keeper
                                                     Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                     Hyst. Enable Field: Hysteresis Disabled */
        IOMUXC_SetPinConfig(
          IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B,    /* GPIO_SD_B1_06 PAD functional properties : */
          0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                     Drive Strength Field: R0/6
                                                     Speed Field: max(200MHz)
                                                     Open Drain Enable Field: Open Drain Disabled
                                                     Pull / Keep Enable Field: Pull/Keeper Enabled
                                                     Pull / Keep Select Field: Keeper
                                                     Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                     Hyst. Enable Field: Hysteresis Disabled */
        IOMUXC_SetPinConfig(
          IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK,     /* GPIO_SD_B1_07 PAD functional properties : */
          0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                     Drive Strength Field: R0/6
                                                     Speed Field: max(200MHz)
                                                     Open Drain Enable Field: Open Drain Disabled
                                                     Pull / Keep Enable Field: Pull/Keeper Enabled
                                                     Pull / Keep Select Field: Keeper
                                                     Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                     Hyst. Enable Field: Hysteresis Disabled */
        IOMUXC_SetPinConfig(
          IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00,   /* GPIO_SD_B1_08 PAD functional properties : */
          0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                     Drive Strength Field: R0/6
                                                     Speed Field: max(200MHz)
                                                     Open Drain Enable Field: Open Drain Disabled
                                                     Pull / Keep Enable Field: Pull/Keeper Enabled
                                                     Pull / Keep Select Field: Keeper
                                                     Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                     Hyst. Enable Field: Hysteresis Disabled */
        IOMUXC_SetPinConfig(
          IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01,   /* GPIO_SD_B1_09 PAD functional properties : */
          0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                     Drive Strength Field: R0/6
                                                     Speed Field: max(200MHz)
                                                     Open Drain Enable Field: Open Drain Disabled
                                                     Pull / Keep Enable Field: Pull/Keeper Enabled
                                                     Pull / Keep Select Field: Keeper
                                                     Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                     Hyst. Enable Field: Hysteresis Disabled */
        IOMUXC_SetPinConfig(
          IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02,   /* GPIO_SD_B1_10 PAD functional properties : */
          0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                     Drive Strength Field: R0/6
                                                     Speed Field: max(200MHz)
                                                     Open Drain Enable Field: Open Drain Disabled
                                                     Pull / Keep Enable Field: Pull/Keeper Enabled
                                                     Pull / Keep Select Field: Keeper
                                                     Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                     Hyst. Enable Field: Hysteresis Disabled */
        IOMUXC_SetPinConfig(
          IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03,   /* GPIO_SD_B1_11 PAD functional properties : */
          0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                     Drive Strength Field: R0/6
                                                     Speed Field: max(200MHz)
                                                     Open Drain Enable Field: Open Drain Disabled
                                                     Pull / Keep Enable Field: Pull/Keeper Enabled
                                                     Pull / Keep Select Field: Keeper
                                                     Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                     Hyst. Enable Field: Hysteresis Disabled */

        /* Configure clock */
        /* Configure FlexSPI */
        /* Get FLEXSPI default settings and configure the flexspi. */
        /* Initialize FlexSPI driver */
        /* Initialize Flash component */
        uint32_t mask = DisableGlobalIRQ();
        flexspi_nor_flash_init(FLEXSPI_INSTANCE);
        flexspi_nor_enable_quad_mode(FLEXSPI_INSTANCE);
        EnableGlobalIRQ(mask);
    }

    return 0;
}

int32_t port_storage_deinit(void)
{
    return 0;
}

int32_t port_storage_erase_sector(uint32_t address)
{
    status_t status = kStatus_Fail;

    DEBUG_PRINT("port_storage_erase_sector: %X\r\n", address);

    /* Check address is within range before erasing */
    if ((address >= START_ADDRESS) && (address < (START_ADDRESS + FLASH_SIZE))) {

        uint32_t erase_address = address - START_ADDRESS;

        /* Make erase operation and cache invalidation atomic */
        uint32_t mask = DisableGlobalIRQ();

        /* Erase sector */
        status = flexspi_nor_flash_erase_sector(FLEXSPI_INSTANCE, erase_address);

        /* Invalidate cache for the page just written */
        DCACHE_InvalidateByRange(erase_address, SECTOR_SIZE);

        /* Enable interrupts again */
        EnableGlobalIRQ(mask);
    }

    return (status == kStatus_Success) ? 0 : -1;
}

int32_t port_storage_read(uint32_t address, uint8_t *data, uint32_t size)
{
    int32_t result = -1;

    DEBUG_PRINT("port_storage_read: %X %p %X\r\n", address, data, size);

    /* Check address is within range before reading. */
    if ((address >= START_ADDRESS) && (address < (START_ADDRESS + FLASH_SIZE))) {

        /**
         * Copy data to buffer using XIP.
         */
        memcpy(data, (void *) address, size);

        /* Set return value. */
        result = 0;
    }

    return result;
}

int32_t port_storage_program_page(uint32_t address, const uint8_t *data, uint32_t size)
{
    int32_t result = -1;

    DEBUG_PRINT("port_storage_program_page: %X %p %X\r\n", address, data, size);

    /* Check address is within range before writing */
    if ((address >= START_ADDRESS) && (address < (START_ADDRESS + FLASH_SIZE))) {

        /* Check for alignment */
        if (((address % FLASH_PAGE_SIZE) == 0) && ((size % FLASH_PAGE_SIZE) == 0)) {

            uint32_t write_address = address - START_ADDRESS;
            uint32_t offset = 0;
            status_t status = kStatus_Success;

            /* Program page */
            while ((offset < size) && (status == kStatus_Success))
            {
                uint32_t buffer[FLASH_PAGE_SIZE / sizeof(uint32_t)];
                const uint32_t* write_pointer = (const uint32_t*) buffer;
                const uint8_t* read_pointer = data + offset;

                /* Copy data to RAM buffer if source is in flash or buffer is not word aligned */
                if (((read_pointer >= (uint8_t*) START_ADDRESS) &&
                    (read_pointer < (uint8_t*) (START_ADDRESS + FLASH_SIZE))) ||
                    ((uint32_t) read_pointer % 4)) {

                    /* Copy un-aligned buffer one byte at a time */
                    uint8_t* copy_pointer = (uint8_t*) buffer;

                    for (size_t index = 0; index < FLASH_PAGE_SIZE; index++) {
                        copy_pointer[index] = read_pointer[index];
                    }

                /* Use input buffer as-is */
                } else {
                    write_pointer = (const uint32_t*) read_pointer;
                }

                /* Make page write and cache invalidation atomic */
                uint32_t mask = DisableGlobalIRQ();

                /* Write page */
                status = flexspi_nor_flash_page_program(FLEXSPI_INSTANCE, write_address, write_pointer);

                /* Invalidate cache for the page just written */
                DCACHE_InvalidateByRange(write_address, FLASH_PAGE_SIZE);

                /* Enable interrupts again */
                EnableGlobalIRQ(mask);

                /* Setup next page */
                write_address += FLASH_PAGE_SIZE;
                offset += FLASH_PAGE_SIZE;
            }

            result = (status == kStatus_Success) ? 0 : -1;
        }
    }

    return result;
}

uint32_t port_storage_get_sector_size(uint32_t address)
{
    uint32_t sectorsize = 0;

    DEBUG_PRINT("port_storage_get_sector_size: %X\r\n", address);

    if ((address >= START_ADDRESS) && (address < (START_ADDRESS + FLASH_SIZE))) {
        sectorsize = SECTOR_SIZE;
    }

    return sectorsize;
}

uint32_t port_storage_get_page_size(uint32_t address)
{
    uint32_t pagesize = 0;

    DEBUG_PRINT("port_storage_get_page_size: %X\r\n", address);

    if ((address >= START_ADDRESS) && (address < (START_ADDRESS + FLASH_SIZE))) {
        pagesize = FLASH_PAGE_SIZE;
    }

    return pagesize;
}

uint32_t port_storage_get_start_address(void)
{
    return START_ADDRESS;
}

uint32_t port_storage_get_size(void)
{
    return FLASH_SIZE;
}

uint8_t port_storage_get_erase_value(void)
{
    return ERASE_VALUE;
}
