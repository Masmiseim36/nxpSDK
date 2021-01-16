/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#ifndef MBED_SPIF_BLOCK_DEVICE_H
#define MBED_SPIF_BLOCK_DEVICE_H

#include "BlockDevice.h"

/** Enum spif standard error codes
 *
 *  @enum spif_bd_error
 */
enum spif_bd_error {
    SPIF_BD_ERROR_OK                    = 0,     /*!< no error */
    SPIF_BD_ERROR_DEVICE_ERROR          = BD_ERROR_DEVICE_ERROR, /*!< device specific error -4001 */
    SPIF_BD_ERROR_PARSING_FAILED        = -4002, /* SFDP Parsing failed */
    SPIF_BD_ERROR_READY_FAILED          = -4003, /* Wait for Memory Ready failed */
    SPIF_BD_ERROR_WREN_FAILED           = -4004, /* Write Enable Failed */
    SPIF_BD_ERROR_INVALID_ERASE_PARAMS  = -4005, /* Erase command not on sector aligned addresses or exceeds device size */
};


#define SPIF_MAX_REGIONS    10
#define MAX_NUM_OF_ERASE_TYPES 4

/** BlockDevice for SFDP based flash devices over SPI bus
 *
 *  @code
 *  // An example using SPI flash device on K82F target
 *  #include "mbed.h"
 *  #include "SPIFBlockDevice.h"
 *
 *  // Create flash device on SPI bus with PTE5 as chip select
 *  SPIFBlockDevice spif(PTE2, PTE4, PTE1, PTE5);
 *
 *  int main() {
 *      printf("spif test\n");
 *
 *      // Initialize the SPI flash device and print the memory layout
 *      spif.init();
 *      printf("spif size: %llu\n",         spif.size());
 *      printf("spif read size: %llu\n",    spif.get_read_size());
 *      printf("spif program size: %llu\n", spif.get_program_size());
 *      printf("spif erase size: %llu\n",   spif.get_erase_size());
 *
 *      // Write "Hello World!" to the first block
 *      char *buffer = (char*)malloc(spif.get_erase_size());
 *      sprintf(buffer, "Hello World!\n");
 *      spif.erase(0, spif.get_erase_size());
 *      spif.program(buffer, 0, spif.get_erase_size());
 *
 *      // Read back what was stored
 *      spif.read(buffer, 0, spif.get_erase_size());
 *      printf("%s", buffer);
 *
 *      // Deinitialize the device
 *      spif.deinit();
 *  }
 *  @endcode
 */
class SPIFBlockDevice : public mbed::BlockDevice {
public:
    /** Creates a SPIFBlockDevice on a SPI bus specified by pins
     *
     *  @param mosi     SPI master out, slave in pin
     *  @param miso     SPI master in, slave out pin
     *  @param sclk     SPI clock pin
     *  @param csel     SPI chip select pin
     *  @param freq     Clock speed of the SPI bus (defaults to 40MHz)
     */
    SPIFBlockDevice(PinName mosi, PinName miso, PinName sclk, PinName csel, int freq = 40000000);

    /** Initialize a block device
     *
     *  @return         SPIF_BD_ERROR_OK(0) - success
     *                  SPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     *                  SPIF_BD_ERROR_READY_FAILED - Waiting for Memory ready failed or timed out
     *                  SPIF_BD_ERROR_PARSING_FAILED - unexpected format or values in one of the SFDP tables
     */
    virtual int init();

    /** Deinitialize a block device
     *
     *  @return         SPIF_BD_ERROR_OK(0) - success
     */
    virtual int deinit();

    /** Desctruct SPIFBlockDevice
      */
    ~SPIFBlockDevice()
    {
        deinit();
    }

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to write blocks to
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes, must be a multiple of read block size
     *  @return         SPIF_BD_ERROR_OK(0) - success
     *                  SPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     */
    virtual int read(void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Program blocks to a block device
     *
     *  @note The blocks must have been erased prior to being programmed
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes, must be a multiple of program block size
     *  @return         SPIF_BD_ERROR_OK(0) - success
     *                  SPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     *                  SPIF_BD_ERROR_READY_FAILED - Waiting for Memory ready failed or timed out
     *                  SPIF_BD_ERROR_WREN_FAILED - Write Enable failed
     */
    virtual int program(const void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Erase blocks on a block device
     *
     *  @note The state of an erased block is undefined until it has been programmed
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes, must be a multiple of erase block size
     *  @return         SPIF_BD_ERROR_OK(0) - success
     *                  SPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     *                  SPIF_BD_ERROR_READY_FAILED - Waiting for Memory ready failed or timed out
     *                  SPIF_BD_ERROR_INVALID_ERASE_PARAMS - Trying to erase unaligned address or size
     */
    virtual int erase(mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */
    virtual mbed::bd_size_t get_read_size() const;

    /** Get the size of a programable block
     *
     *  @return         Size of a programable block in bytes
     *  @note Must be a multiple of the read size
     */
    virtual mbed::bd_size_t get_program_size() const;

    /** Get the size of an erasable block
     *
     *  @return         Size of an erasable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual mbed::bd_size_t get_erase_size() const;

    /** Get the size of minimal erasable sector size of given address
     *
     *  @param addr     Any address within block queried for erase sector size (can be any address within flash size offset)
     *  @return         Size of minimal erase sector size, in given address region, in bytes
     *  @note Must be a multiple of the program size
     */
    virtual mbed::bd_size_t get_erase_size(mbed::bd_addr_t addr) const;

    /** Get the value of storage byte after it was erased
     *
     *  If get_erase_value returns a non-negative byte value, the underlying
     *  storage is set to that value when erased, and storage containing
     *  that value can be programmed without another erase.
     *
     *  @return         The value of storage when erased, or -1 if you can't
     *                  rely on the value of erased storage
     */
    virtual int get_erase_value() const;

    /** Get the total size of the underlying device
     *
     *  @return         Size of the underlying device in bytes
     */
    virtual mbed::bd_size_t size() const;

    /** Get the BlockDevice class type.
     *
     *  @return         A string representation of the BlockDevice class type.
     */
    virtual const char *get_type() const;
};

#endif  /* MBED_SPIF_BLOCK_DEVICE_H */
