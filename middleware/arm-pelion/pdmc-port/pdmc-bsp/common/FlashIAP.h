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

#ifndef MBED_FLASHIAP_H
#define MBED_FLASHIAP_H

#include "flash_api.h"

namespace mbed {

/** \addtogroup drivers-public-api */
/** @{*/

/**
 * \defgroup drivers_FlashIAP FlashIAP class
 * @{
 */

/** Flash IAP driver. It invokes flash HAL functions.
 *
 * @note Synchronization level: Thread safe
 */
class FlashIAP {
public:

    /** Initialize a flash IAP device
     *
     *  Should be called once per lifetime of the object.
     *  @return 0 on success or a negative error code on failure
     */
    int init();

    /** Deinitialize a flash IAP device
     *
     *  @return 0 on success or a negative error code on failure
     */
    int deinit();

    /** Read data from a flash device.
     *
     *  This method invokes memcpy - reads number of bytes from the address
     *
     *  @param buffer Buffer to write to
     *  @param addr   Flash address to begin reading from
     *  @param size   Size to read in bytes
     *  @return       0 on success, negative error code on failure
     */
    int read(void *buffer, uint32_t addr, uint32_t size);

    /** Program data to pages
     *
     *  The sectors must have been erased prior to being programmed
     *
     *  @param buffer Buffer of data to be written
     *  @param addr   Address of a page to begin writing to
     *  @param size   Size to write in bytes, must be a multiple of program size
     *  @return       0 on success, negative error code on failure
     */
    int program(const void *buffer, uint32_t addr, uint32_t size);

    /** Erase sectors
     *
     *  The state of an erased sector is undefined until it has been programmed
     *
     *  @param addr Address of a sector to begin erasing, must be a multiple of the sector size
     *  @param size Size to erase in bytes, must be a multiple of the sector size
     *  @return     0 on success, negative error code on failure
     */
    int erase(uint32_t addr, uint32_t size);

    /** Get the sector size at the defined address
     *
     *  Sector size might differ at address ranges.
     *  An example <0-0x1000, sector size=1024; 0x10000-0x20000, size=2048>
     *
     *  @param addr Address of or inside the sector to query
     *  @return Size of a sector in bytes or MBED_FLASH_INVALID_SIZE if not mapped
     */
    uint32_t get_sector_size(uint32_t addr) const;

    /** Get the flash start address
     *
     *  @return Flash start address
     */
    uint32_t get_flash_start() const;

    /** Get the flash size
     *
     *  @return Flash size
     */
    uint32_t get_flash_size() const;

    /** Get the program page size
     *
     *  The page size defines the writable page size
     *  @return Size of a program page in bytes
     */
    uint32_t get_page_size() const;

    /** Get the flash erase value
     *
     *  Get the value we read after erase operation
     *  @return flash erase value
     */
    uint8_t get_erase_value() const;

private:

    /* Check if address and size are aligned to a sector
     *
     *  @param addr Address of block to check for alignment
     *  @param size Size of block to check for alignment
     *  @return true if the block is sector aligned, false otherwise
     */
    bool is_aligned_to_sector(uint32_t addr, uint32_t size);
};

} /* namespace mbed */

#endif  /* MBED_FLASHIAP_H */
