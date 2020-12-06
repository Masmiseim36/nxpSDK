/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FICA_DRIVER_H_
#define _FICA_DRIVER_H_

/*!
 * @addtogroup flash_ica
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

// #ifdef PRODUCTION_VERSION // Production version will jump to Factory Image instead of Application A

#include <stdbool.h>
#include "fica_definition.h"

// Uncomment to Blink progress on LEDs
#define BLINK_PROGRESS 1

#define TYPE_APP false
#define TYPE_IMG true

#define IMG_EXT_NO_ERROR 0
#define IMG_EXT_ERROR 1

#define EXT_FLASH_PROGRAM_PAGE 0x200 // 512B
#define EXT_FLASH_ERASE_PAGE 0x40000 // 256KB

#define SLN_FLASH_NO_ERROR 0
#define SLN_FLASH_ERROR -1

/*******************************************************************************
 * Externals
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the Application Program External Flash Interface
 *
 */
int32_t FICA_app_program_ext_init(int32_t newimgtype);

/*!
 * @brief Blocking image program to external flash
 * This is part of a blocking image program, but the actual small buffer flash write is blocking
 * Assumes all calls have imgaddr that is PAGE ALIGNED
 *
 */
int32_t FICA_app_program_ext_abs(uint32_t offset, uint8_t *pbuf, uint32_t len);

/*!
 * @brief Finalize the program by verifying the Signature and writing image info
 *
 */
int32_t FICA_app_program_ext_finalize();

/*!
 * @brief Sets the reset vector to the new application and sets the OTA bit indicating its done
 *
 */
int32_t FICA_app_program_ext_set_reset_vector();

/*!
 * @brief
 *
 */
int32_t FICA_app_program_ext_calculate_crc(int32_t imgtype, uint32_t *pcrc);

/*!
 * @brief
 *
 */
int32_t FICA_app_program_ext_program_crc(int32_t imgtype, uint32_t crc);

/*!
 * @brief Clears the buffer with the passed initialization value
 *
 */
int32_t FICA_clear_buf(uint8_t *pbuf, uint8_t initval, uint32_t len);

/*!
 * @brief Read the FICA database into a buffer.
 * Usually done just prior to modify, write
 *
 */
int32_t FICA_read_db(void);

/*!
 * @brief Write the FICA database from the buffer
 *
 */
int32_t FICA_write_db(void);

/*!
 * @brief Writes the passed buffer to external flash starting at address passed by offset
 *
 */
int32_t FICA_write_buf(uint32_t offset, uint32_t len, void *buf);

/*!
 * @brief Reads the external flash to the passed buffer starting at address passed by offset
 *
 */
int32_t FICA_read_buf(uint32_t offset, uint32_t len, void *buf);

/*!
 * @brief Get the field flash address based on the image type and field offset
 *
 */
int32_t FICA_get_field_addr(int32_t imgtype, uint32_t fieldoffset, uint32_t *pfieldaddr);

/*!
 * @brief Write a value to a specific image field, write it to the external flash
 *
 */
int32_t FICA_write_field(int32_t imgtype, uint32_t fieldoffset, uint32_t val);

/*!
 * @brief Write a value to a specific image field, writes to buffer only, not external flash
 *
 */
int32_t FICA_write_field_no_save(int32_t imgtype, uint32_t fieldoffset, uint32_t val);

/*!
 * @brief Read any field given the passed image type and field offset
 *
 */
int32_t FICA_read_field(int32_t imgtype, uint32_t field_offset, uint32_t *pfieldval);

/*!
 * @brief Get the new application type from FICA
 *
 */
int32_t FICA_get_new_app_img_type(int32_t *imgtype);

/*!
 * @brief Get the application external flash start address, given the passed image type
 *
 */
int32_t FICA_get_app_img_start_addr(int32_t imgtype, uint32_t *startaddr);

/*!
 * @brief Get the maximum size for each image type
 *
 */
int32_t FICA_get_app_img_max_size(int32_t imgtype, uint32_t *maximgsize);

/*!
 * @brief Get the Application Image Length
 *
 */
int32_t FICA_get_app_img_len(int32_t imgtype, uint32_t *plen);

/*!
 * @brief Write the image info to external memory
 *
 */
int32_t FICA_write_image_info(int32_t imgtype, uint32_t imgfmt, uint32_t len);

/*!
 * @brief Write the image info to external memory
 *
 */
int32_t FICA_write_image_record(int32_t imgtype, uint32_t imgaddr, uint32_t imglen, uint8_t *imgsig);

/*!
 * @brief Reads the ICA record from the buffer
 *
 */
int32_t FICA_read_record(int32_t imgtype, fica_record_t *pimgrec);

/*!
 * @brief Write an ICA record to the FICA buffer and flash
 *
 */
int32_t FICA_write_record(int32_t imgtype, fica_record_t *pimgrec);

/*!
 * @brief Is Flash Image Config Area Initialized ?
 *
 */
bool is_FICA_initialized(void);

/*!
 * @brief Check if FICA is initialized, initialize it if needed
 *
 */
int32_t FICA_initialize(void);

/*!
 * @brief Initialize External Flash driver
 *
 */
int32_t FICA_verify_ext_flash(void);

/*!
 * @brief
 *
 */
uint16_t FICA_compute_chunk_CRC(uint8_t *pData, uint16_t lenData, uint16_t crcValueOld);

/*!
 * @brief Gets the Processor Communication Flag
 *
 */
int32_t FICA_get_comm_flag(uint32_t comflag, bool *flagstate);

/*!
 * @brief Sets the Processor Communication Flag
 *
 */
int32_t FICA_set_comm_flag(uint32_t comflag);

/*!
 * @brief Clears the Processor Communication Flag
 *
 */
int32_t FICA_clr_comm_flag(uint32_t comflag);

/*!
 * @brief Resets this MCU
 *
 */
void reset_mcu(void);

/*!
 * @brief Reads the flash at the address and compares it with the RAM s_appImgBuffer
 * This function returns an error in case the content in the flash doesn't match the s_appImgBuffer
 *
 */
int32_t FICA_read_for_debug(uint32_t addr, uint32_t len);

/*!
 * @brief Reads out the FICA table from NVM
 *
 */
int32_t FICA_GetCurBootStartAddr(uint32_t *paddr);

/*!
 * @brief Reads the address of the current application programmed in flash
 *
 */
int32_t FICA_GetCurAppStartType(int32_t *pimgtype);

/*!
 * @brief Reads the new application start address
 *
 */
int32_t FICA_GetNewAppStartAddr(uint32_t *paddr);

/*!
 * @brief Writes the current start image type
 *
 */
int32_t FICA_SetCurAppStartType(int32_t imgtype);

/*!
 * @brief Writes the current boot image type
 *
 */
int32_t FICA_SetCurBootType(int32_t imgtype);

/*!
 * @brief Reads the hyperflash ID
 *
 */
int32_t FICA_read_hyper_flash_id(uint8_t *pid);

/*!
 * @brief Reads the current image type
 *
 */
int32_t FICA_GetImgTypeFromAddr(uint32_t appaddr, int32_t *imgtype);

/*!
 * @brief Reads the current image type based on ResetISR address
 *
 */
int32_t FICA_GetImgTypeFromResetISRAddr(int32_t *imgType);

/*!
 * @brief Verifies the image certificate
 *
 */
int32_t FICA_Verify_Certificate(int32_t imgType);

/*!
 * @brief Saves the image certificate to flash
 *
 */
int32_t FICA_Save_Certificate(int32_t imgType);

/*!
 * @brief Invalidates the image certificate
 *
 */
int32_t FICA_Invalidate_Certificate(int32_t imgType);

/*!
 * @brief Erases the image certificate from flash
 *
 */
int32_t FICA_Erase_Certificate(int32_t imgType);

/*!
 * @brief Saves the image certificate to flash
 *
 */
int32_t FICA_Save_Signature(uint8_t *msgSig);

/*!
 * @brief Verifies the image certificate
 *
 */
int32_t FICA_Verify_Signature(int32_t imgType);

/*!
 * @brief Verifies the OTA image entry point
 *
 */
int32_t FICA_Verify_OTA_Image_Entry_Point(int32_t imgType);

/*!
 * @brief Check if the OTA bit is set in the flash, main app will set if OTA is desired
 *
 */
bool FICA_is_OTA_FlashBitCleared(void);

/*!
 * @brief Set OTA bit in the flash, not used in bootloader, here for reference
 *
 */
int32_t FICA_Set_OTA_FlashBit(void);

/*!
 * @brief Clear OTA bit in the flash, not used in bootloader, here for reference
 *
 */
int32_t FICA_Clear_OTA_FlashBit(void);

#if (defined(ENABLE_UNSIGNED_USB_MSD) || ENABLE_UNSIGNED_USB_MSD == 1)
// NOTE: To facilitate ease of use in SLN-ALEXA-IOT kits
/*!
 * @brief Sets the USB mode
 *
 */
int32_t FICA_set_usb_mode(void);

/*!
 * @brief Clears the USB mode
 *
 */
int32_t FICA_clr_usb_mode(void);

/*!
 * @brief Gets the USB mode
 *
 */
int32_t FICA_get_usb_mode(bool *isUSB);
#endif /* ENABLE_UNSIGNED_USB_MSD */

/*!
 * @brief Erases a bank a flash from the address
 *
 */
int32_t FICA_Erase_Bank(uint32_t startaddr, uint32_t banksize);

/*!
 * @brief Sets the reset vector
 *
 */
int32_t FICA_app_program_ext_set_reset_vector(void);

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FICA_DRIVER_H_ */
