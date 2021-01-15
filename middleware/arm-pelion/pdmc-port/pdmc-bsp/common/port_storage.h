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

#ifndef PORT_STORAGE_H
#define PORT_STORAGE_H

#include <inttypes.h>

#define MBED_FLASH_INVALID_SIZE     0xFFFFFFFF

#ifdef __cplusplus
extern "C" {
#endif

int32_t port_storage_init(void);

int32_t port_storage_deinit(void);

int32_t port_storage_read(uint32_t address, uint8_t* data, uint32_t size);

int32_t port_storage_program_page(uint32_t address, const uint8_t* data, uint32_t size);

int32_t port_storage_erase_sector(uint32_t address);

uint32_t port_storage_get_sector_size(uint32_t address);

uint32_t port_storage_get_page_size(uint32_t address);

uint32_t port_storage_get_start_address(void);

uint32_t port_storage_get_size(void);

uint8_t port_storage_get_erase_value(void);

#ifdef __cplusplus
}
#endif

#endif // PORT_STORAGE_H
