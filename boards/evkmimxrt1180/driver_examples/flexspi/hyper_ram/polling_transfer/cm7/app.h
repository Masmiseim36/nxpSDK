/*
 * Copyright 2024 NXP
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
#define EXAMPLE_FLEXSPI                 FLEXSPI2
#define EXAMPLE_FLEXSPI_AMBA_BASE       FlexSPI2_AMBA_BASE
#define EXAMPLE_FLEXSPI_PORT            kFLEXSPI_PortA1
#define EXAMPLE_FLEXSPI_RX_SAMPLE_CLOCK kFLEXSPI_ReadSampleClkExternalInputFromDqsPad

#define EXAMPLE_FLEXSPI_COMBINATION_MODE_ENABLE 1
#define EXAMPLE_FLEXSPI_SCKBDIFFOPT_ENABLE      1

#define HYPERRAM_CMD_LUT_SEQ_IDX_READDATA  0
#define HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA 1
#define HYPERRAM_CMD_LUT_SEQ_IDX_READREG   2
#define HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG  3

#define CUSTOM_LUT_LENGTH               20

#define HYRAM_SIZE 0x10000 /* 64Mb/KByte. */
#define TEST_SIZE  0x80000U /* hyperram test size range in demo. */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
