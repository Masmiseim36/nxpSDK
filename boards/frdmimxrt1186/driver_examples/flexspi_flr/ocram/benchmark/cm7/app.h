/*
 * Copyright 2023, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "fsl_clock.h"
#include "fsl_flexspi_flr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/*! @brief Clock frequency enumeration of FLEXSPI FOLLOWER.*/
typedef enum _flexspi_slv_clock_freq
{
    kFLEXSPI_SLV_RootClock_50M  = 50,  /*!< 50MHz root clock */
    kFLEXSPI_SLV_RootClock_66M  = 66,  /*!< 66MHz root clock */
    kFLEXSPI_SLV_RootClock_80M  = 80,  /*!< 80MHz root clock */
    kFLEXSPI_SLV_RootClock_100M = 100, /*!< 100MHz root clock */
    kFLEXSPI_SLV_RootClock_133M = 133, /*!< 133MHz root clock */
    kFLEXSPI_SLV_RootClock_166M = 166, /*!< 166MHz root clock */
    kFLEXSPI_SLV_RootClock_200M = 200, /*!< 200MHz root clock */
    kFLEXSPI_SLV_RootClock_400M = 400  /*!< 400MHz root clock */
} flexspi_slv_clock_freq_t;

#define EXAMPLE_FLEXSPI                 FLEXSPI1
#define EXAMPLE_FLEXSPI_AMBA_BASE       FlexSPI1_AMBA_BASE
#define EXAMPLE_FLEXSPI_CLOCK           kCLOCK_Flexspi1
#define EXAMPLE_FLEXSPI_ROOT_CLOCK_SRC  kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc400M
#define EXAMPLE_FLEXSPI_ROOT_CLOCK_DIV  4
#define FLASH_PORT                      kFLEXSPI_PortB1
#define EXAMPLE_FLEXSPI_RX_SAMPLE_CLOCK kFLEXSPI_ReadSampleClkExternalInputFromDqsPad

#define EXAMPLE_FLEXSPI_SLV             FLEXSPI_SLV
#define EXAMPLE_FLEXSPI_SLV_MODE        kFLEXSPI_SLV_IOMODE_SDRx8
#define EXAMPLE_FLEXSPI_SLV_ROOT_CLOCK  kFLEXSPI_SLV_RootClock_400M
#define EXAMPLE_FLEXSPI_SLV_DUMMY_CYCLE 0x14
#define OCRAM_BASEADDR                  0x20484000
#define MEMORY_SIZE                     100 /* KByte */

#define READ_REG_STATUS 0
#define READ_MEMORY     1
#define SEND_MAILBOX    2
#define WRITE_MEMORY    3

#define FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(x) ((x) * 4 + READ_REG_STATUS)
#define FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(x)     ((x) * 4 + READ_MEMORY)
#define FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(x)    ((x) * 4 + SEND_MAILBOX)
#define FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(x)    ((x) * 4 + WRITE_MEMORY)

#define CUSTOM_LUT_LENGTH ((FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx8) + 1) * 4)

#define FOLLOWER_CMD_READ_STATUS  0x97
#define FOLLOWER_CMD_READ_MEMORY  0x1F
#define FOLLOWER_CMD_SEND_MAILBOX 0xA6
#define FOLLOWER_CMD_WRITE_MEMORY 0x5B

#define EXAMPLE_GPT          GPT1
#define EXAMPLE_GPT_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Gpt1)

/*
 * If cache is enabled, this example should maintain the cache to make sure
 * CPU core accesses the memory, not cache only.
 */
#define CACHE_MAINTAIN 1

/*${macro:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
typedef uint32_t TickType_t;

#if (defined CACHE_MAINTAIN) && (CACHE_MAINTAIN == 1)
typedef struct _flexspi_cache_status
{
    volatile bool DCacheEnableFlag;
    volatile bool ICacheEnableFlag;
    volatile bool codeCacheEnableFlag;
    volatile bool systemCacheEnableFlag;
} flexspi_cache_status_t;
#endif
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void setupTimerInterrupt(void);
uint32_t FLEXSPI_GetRootClockFreqHz(clock_root_mux_source_t clkName);
/*${prototype:end}*/

#endif /* _APP_H_ */
