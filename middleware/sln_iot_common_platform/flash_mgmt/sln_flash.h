/*
 * Copyright 2019-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software
 */

#ifndef _SLN_FLASH_H_
#define _SLN_FLASH_H_

#include <stdint.h>
#include "fsl_common.h"

#ifdef __REDLIB__
size_t safe_strlen(const char *ptr, size_t max);
#else
#define safe_strlen strnlen
#endif

/*!
 * @brief Initialize flash subsystem (FlexSPI software reset)
 *
 * @returns Void
 *
 */
void SLN_Flash_Init(void);

/*!
 * @brief Memset function executing from RAM
 *
 * @param dst  Pointer to destination memory
 * @param data Data to set in destination memory
 * @param len  Lentght of Data to written to destination memory
 */
void SLN_ram_memset(void *dst, uint8_t data, size_t len);

/*!
 * @brief Memcpy function executing from RAM
 *
 * @param dst Pointer to destination memory
 * @param src Pointer to source memory
 * @param len Length of Data to copied to destination memory
 */
void SLN_ram_memcpy(void *dst, void *src, size_t len);

/*!
 * @brief Disable IRQ executing from RAM
 *
 * @returns Primask value
 */
uint32_t SLN_ram_disable_irq(void);

/*!
 * @brief Enable IRQ executing from RAM
 *
 * @param priMask Primask value returned from previous SLN_ram_disable_irq call
 */
void SLN_ram_enable_irq(uint32_t priMask);

/*!
 * @brief Disable data cache function executing from RAM
 *
 */
void SLN_ram_disable_d_cache(void);

/*!
 * @brief Enable data cache function executing from RAM
 *
 */
void SLN_ram_enable_d_cache(void);

/*!
 * @brief Write a page (512 bytes) to flash
 *
 * @param address The offset from the start of the flash
 * @param data The buffer to write to flash
 * @param len Length of the buffer in bytes
 *
 * @returns Status of the write operation
 *     kStatus_Success in case of success or a negative value in case of error
 */
status_t SLN_Write_Flash_Page(uint32_t address, uint8_t *data, uint32_t len);

/*!
 * @brief Erase a flash sector (up to 256 Kbytes, depending on the flash)
 *
 * @param address The offset from the start of the flash
 *
 * @returns Status of the erase operation
 *     kStatus_Success in case of success or a negative value in case of error
 */
status_t SLN_Erase_Sector(uint32_t address);

/*!
 * @brief Write a buffer to Flash, buffer is written page by page [WARNING: erases sector before write]
 *
 * @param address The offset from the start of the flash
 * @param buf The buffer to write to flash
 * @param len Length of the buffer in bytes
 *
 * @returns Status of the write operation
 *     kStatus_Success in case of success or a negative value in case of error
 */
int32_t SLN_Write_Sector(uint32_t address, uint8_t *buf, uint32_t len);

/*!
 * @brief Write a buffer to flash
 *     SLN_Erase_Sector must be called prior to writing pages in a sector.
 *     Afterwards, multiple pages can be written in that sector.
 *
 * @param address The offset from the start of the flash
 * @param data The buffer to write to flash
 *
 * @returns Status of the write operation
 *     kStatus_Success in case of success or a negative value in case of error
 */
status_t SLN_Write_Flash_At_Address(uint32_t address, uint8_t *data);

/*!
 * @brief Read data from the flash into a buffer
 *
 * @param address The offset from the start of the flash
 * @param data The destination buffer
 * @param size Length of the buffer in bytes
 *
 * @returns Status of the write operation
 *     kStatus_Success in case of success or a negative value in case of error
 */
status_t SLN_Read_Flash_At_Address(uint32_t address, uint8_t *data, uint32_t size);

/*!
 * @brief Return the memory address used to directly access the flash data
 *
 * @param address The offset from the start of the flash
 *
 * @returns A 32-bit address used to directly access the flash data
 */
uint32_t SLN_Flash_Get_Read_Address(uint32_t address);

#endif /* _SLN_FLASH_H_ */
