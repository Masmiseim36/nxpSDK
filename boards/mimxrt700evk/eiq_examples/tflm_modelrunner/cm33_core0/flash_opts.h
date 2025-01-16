/*
* Copyright 2024 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#include "fsl_xspi.h"
#include "app.h"

#include "board.h"
#define FLASH_MODEL_ADDR 0x0U
#define FLASH_BASE_ADDR 0x60000000U

typedef xspi_config_t FlashConfig;
#if defined(__cplusplus)
extern "C" {
#endif

status_t FlashInit(FlashConfig *config);
status_t FlashErase(FlashConfig *config, uint32_t start, uint32_t length);
status_t FlashProgram(FlashConfig *config, uint32_t start, uint32_t *src, uint32_t length);

#if defined(__cplusplus)
}
#endif
