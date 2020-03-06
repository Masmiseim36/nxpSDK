/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_cache.h"
/*${header:end}*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_FLEXSPI FLEXSPI
#define FLASH_SIZE 0x10000 /* 512Mb/KByte */
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI_AMBA_BASE
#define FLASH_PAGE_SIZE 256
#define EXAMPLE_SECTOR 1000
#define SECTOR_SIZE 0x1000 /* 4K */
#define EXAMPLE_FLEXSPI_CLOCK kCLOCK_Flexspi
#define FLASH_PORT kFLEXSPI_PortB1
#define EXAMPLE_FLEXSPI_RX_SAMPLE_CLOCK kFLEXSPI_ReadSampleClkLoopbackInternally

#define NOR_CMD_LUT_SEQ_IDX_READ 0
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS 1
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE 2
#define NOR_CMD_LUT_SEQ_IDX_READID_OPI 3
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI 4
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR 5
#define NOR_CMD_LUT_SEQ_IDX_CHIPERASE 6
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM 7
#define NOR_CMD_LUT_SEQ_IDX_ENTEROPI 8
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI 9

#define CUSTOM_LUT_LENGTH 60
#define FLASH_BUSY_STATUS_POL 1
#define FLASH_BUSY_STATUS_OFFSET 0
#define FLASH_ERROR_STATUS_MASK 0x0e
#define FLASH_ENABLE_OCTAL_CMD 0x02
#define CACHE_MAINTAIN 1
#define EXAMPLE_FLASH_RESET_CONFIG()                                      \
    do                                                                    \
    {                                                                     \
        GPIO->B[2][12] = 0;                                               \
        for (uint64_t i = 0; i < USEC_TO_COUNT(20, SystemCoreClock); i++) \
        {                                                                 \
            __NOP();                                                      \
        }                                                                 \
        GPIO->B[2][12] = 1;                                               \
        flexspi_nor_enable_octal_mode(base);                              \
    } while (0)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

// AT_QUICKACCESS_SECTION_CODE(static inline void flash_reset(void))
//{
//    GPIO->B[2][12] = 0;
//    for (uint64_t i = 0; i < USEC_TO_COUNT(20, SystemCoreClock); i++)
//    {
//        __NOP();
//    }
//    GPIO->B[2][12] = 1;
//}

/*${prototype:end}*/

#endif /* _APP_H_ */
