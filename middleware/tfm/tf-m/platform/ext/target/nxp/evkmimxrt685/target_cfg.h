/*
 * Copyright (c) 2018-2020 Arm Limited
 * Copyright 2019-2023 NXP. All rights reserved.
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

#ifndef __TARGET_CFG_H__
#define __TARGET_CFG_H__

#include "target_cfg_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TFM_DRIVER_STDIO    Driver_USART
#define NS_DRIVER_STDIO     Driver_USART

/* Predefined NOR flash config options (Macronix Octal SPI Flash MX25UM51345G flash, connected to FlexSPI Port B) [UM11147, Table 1007] */
#define NOR_FLASH_INSTANCE      0U    /* This the instance number of FLEXSPI, only support 0 */
#define NOR_FLASH_OPTION0       0xC1500051U /* In SDK it was 0xC1503051U */
#define NOR_FLASH_OPTION1       0x20000008U /* In SDK it was 0x20000014U */
#define FLASH_NEED_RESET        1

#define FLEXSPI_FLASH_CTX       (SYSCTL0->FLEXSPI_BOOTROM_SCRATCH0)

#define SEC_VIO_IRQn            SECUREVIOLATION_IRQn       /* Security Violation Interrupt number, synonim */
#define SEC_VIO_IRQHandler      SECUREVIOLATION_IRQHandler /* Security Violation Interrupt handler, synonim */

/**
 * \brief Holds the data necessary to do isolation for a specific peripheral.
 */
struct platform_data_t
{
    uint32_t periph_start;
    uint32_t periph_limit;
    volatile uint32_t *periph_ppc_bank; /* Secure control register address */
    uint32_t periph_ppc_loc;            /* Position in the secure control register */
};

#ifdef __cplusplus
}
#endif

#endif /* __TARGET_CFG_H__ */
