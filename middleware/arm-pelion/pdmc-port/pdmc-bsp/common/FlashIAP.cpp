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

#include "FlashIAP.h"

#include "flash_api.h"

#define __STDC_FORMAT_MACROS 1
#include <cinttypes>
#include <cstdio>

#if 0
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

namespace mbed {

static inline bool is_aligned(uint32_t number, uint32_t alignment)
{
    if ((number % alignment) != 0) {
        return false;
    } else {
        return true;
    }
}

int FlashIAP::init()
{
    int ret = 0;

    if (flash_init(NULL) != 0) {
        ret = -1;
    }

    return ret;
}

int FlashIAP::deinit()
{
    int ret = 0;

    if (flash_free(NULL) != 0) {
        ret = -1;
    }

    return ret;
}

int FlashIAP::read(void *buffer, uint32_t address, uint32_t size)
{
    DEBUG_PRINT("FlashIAP::read: %" PRIX32 " %" PRIX32 "\r\n", address, size);

    return flash_read(NULL, address, (uint8_t *) buffer, size);
}

int FlashIAP::program(const void *buffer, uint32_t address, uint32_t size)
{
    DEBUG_PRINT("FlashIAP::program: %" PRIX32 " %" PRIX32 "\r\n", address, size);

    return flash_program_page(NULL, address, (const uint8_t*) buffer, size);
}

bool FlashIAP::is_aligned_to_sector(uint32_t address, uint32_t size)
{
    uint32_t current_sector_size = flash_get_sector_size(NULL, address);
    if (!is_aligned(size, current_sector_size) ||
            !is_aligned(address, current_sector_size)) {
        return false;
    } else {
        return true;
    }
}

int FlashIAP::erase(uint32_t address, uint32_t size)
{
    DEBUG_PRINT("FlashIAP::erase: %" PRIX32 " %" PRIX32 "\r\n", address, size);

    uint32_t flash_size = flash_get_size(NULL);
    uint32_t flash_start_addr = flash_get_start_address(NULL);
    uint32_t flash_end_addr = flash_start_addr + flash_size;
    uint32_t erase_end_addr = address + size;

    if (erase_end_addr > flash_end_addr) {
        return -1;
    } else if (erase_end_addr < flash_end_addr) {
        uint32_t following_sector_size = flash_get_sector_size(NULL, erase_end_addr);
        if (!is_aligned(erase_end_addr, following_sector_size)) {
            return -1;
        }
    }

    int32_t ret = 0;

    while ((address < erase_end_addr) && (ret == 0)) {

        ret = flash_erase_sector(NULL, address);
        uint32_t current_sector_size = flash_get_sector_size(NULL, address);

        address += current_sector_size;
    }

    return ret;
}

uint32_t FlashIAP::get_page_size() const
{
    return flash_get_page_size(NULL);
}

uint32_t FlashIAP::get_sector_size(uint32_t address) const
{
    return flash_get_sector_size(NULL, address);
}

uint32_t FlashIAP::get_flash_start() const
{
    return flash_get_start_address(NULL);
}

uint32_t FlashIAP::get_flash_size() const
{
    return flash_get_size(NULL);
}

uint8_t FlashIAP::get_erase_value() const
{
    return flash_get_erase_value(NULL);
}

}
