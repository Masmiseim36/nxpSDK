/** @file
 * Copyright (c) 2018-2019, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include "target_database.h"


/* Range of 1KB Non-volatile memory to preserve data over reset. Ex, NVRAM and FLASH */
/* Flash interface just emulated over SRAM, use simple memory write */
uint8_t target_nvram[1024]; /* NVRAM imulation in RAM */

target_database_t target_database =
{
     /* Target Configuration Header */
    .target_cfg_hdr = {
        /* PSA_CFG */
        //uint32_t  signature[2];
        /* 8 byte String describing the Target platform */
        //uint32_t  target_string[2];
        .version = 1, /* version = 1 for now */
        .size = sizeof(target_cfg_hdr_t), /* Header Size */
    },   
    .uart = {                                   /* Uart */
        .cfg_type = {
            .cfg_id = TARGET_CONFIG_CREATE_ID(GROUP_SOC_PERIPHERAL, SOC_PERIPHERAL_UART, 0),
            .size = sizeof(soc_peripheral_desc_t)/* size inclusive of this header */
        }
        /* Other field are optional, UART is initializaed by TFM.*/
    },
    .wd_timer = {                                 /* WD timer */
        .cfg_type = {
            .cfg_id = TARGET_CONFIG_CREATE_ID(GROUP_SOC_PERIPHERAL,SOC_PERIPHERAL_WATCHDOG, 0),
            .size = sizeof(soc_peripheral_desc_t)/* size inclusive of this header */
        }
        /* Other field are optional, WD is not used yet.*/
    },
    .memory = {                                  /* Memory region */
        .cfg_type = {
            .cfg_id = TARGET_CONFIG_CREATE_ID(GROUP_MEMORY, MEMORY_NVMEM, 0),
            .size = sizeof(memory_desc_t)/* size inclusive of this header */
        },
        .start = (addr_t)target_nvram,
        .end = (addr_t)(&target_nvram[sizeof(target_nvram)-1])
    },
    .type_end = {
        .cfg_id = VAL_TEST_INVALID_CFG_ID,
    }
};