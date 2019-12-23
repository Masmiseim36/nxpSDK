/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FLASH_H__
#define __FLASH_H__

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
/*! @brief Hal flash status. */
typedef enum _hal_flash_status
{
    kStatus_HAL_Flash_Success         = kStatus_Success,                        /*!< flash operation is successful*/
    kStatus_HAL_Flash_Fail            = MAKE_STATUS(kStatusGroup_HAL_FLASH, 1), /*!< flash operation is not successful*/
    kStatus_HAL_Flash_InvalidArgument = MAKE_STATUS(kStatusGroup_HAL_FLASH, 2), /*!< Invalid argument */
    kStatus_HAL_Flash_AlignmentError  = MAKE_STATUS(kStatusGroup_HAL_FLASH, 3), /*!< Alignment Error */
    kStatus_HAL_Flash_EccError        = MAKE_STATUS(kStatusGroup_HAL_FLASH, 4), /*!< ECC error detected */
    kStatus_HAL_Flash_Error           = MAKE_STATUS(kStatusGroup_HAL_FLASH, 5), /*!< Illegal command */
    kStatus_HAL_Flash_NotSupport      = MAKE_STATUS(kStatusGroup_HAL_FLASH, 6), /*!< Not support */
} hal_flash_status_t;

/*! @brief Hal flash property. */
typedef enum _hal_flash_property_tag
{
    kHAL_Flash_PropertyPflashSectorSize         = 0x00U, /*!< Pflash sector size property.*/
    kHAL_Flash_PropertyPflashTotalSize          = 0x01U, /*!< Pflash total size property.*/
    kHAL_Flash_PropertyPflashBlockSize          = 0x02U, /*!< Pflash block size property.*/
    kHAL_Flash_PropertyPflashBlockCount         = 0x03U, /*!< Pflash block count property.*/
    kHAL_Flash_PropertyPflashBlockBaseAddr      = 0x04U, /*!< Pflash block base address property.*/
    kHAL_Flash_PropertyPflashFacSupport         = 0x05U, /*!< Pflash fac support property.*/
    kHAL_Flash_PropertyPflashAccessSegmentSize  = 0x06U, /*!< Pflash access segment size property.*/
    kHAL_Flash_PropertyPflashAccessSegmentCount = 0x07U, /*!< Pflash access segment count property.*/
    kHAL_Flash_PropertyFlexRamBlockBaseAddr     = 0x08U, /*!< FlexRam block base address property.*/
    kHAL_Flash_PropertyFlexRamTotalSize         = 0x09U, /*!< FlexRam total size property.*/
    kHAL_Flash_PropertyDflashSectorSize         = 0x10U, /*!< Dflash sector size property.*/
    kHAL_Flash_PropertyDflashTotalSize          = 0x11U, /*!< Dflash total size property.*/
    kHAL_Flash_PropertyDflashBlockSize          = 0x12U, /*!< Dflash block size property.*/
    kHAL_Flash_PropertyDflashBlockCount         = 0x13U, /*!< Dflash block count property.*/
    kHAL_Flash_PropertyDflashBlockBaseAddr      = 0x14U, /*!< Dflash block base address property.*/
    kHAL_Flash_PropertyEepromTotalSize          = 0x15U, /*!< EEPROM total size property.*/
    kHAL_Flash_PropertyFlashMemoryIndex         = 0x20U  /*!< Flash memory index property.*/
} hal_flash_property_tag_t;

/*! @brief Hal flash security state. */
typedef enum _hal_flash_security_state
{
    kHAL_Flash_SecurityStateNotSecure        = 0xc33cc33cU, /*!< Flash is not secure.*/
    kHAL_Flash_SecurityStateBackdoorEnabled  = 0x5aa55aa5U, /*!< Flash backdoor is enabled.*/
    kHAL_Flash_SecurityStateBackdoorDisabled = 0x5ac33ca5U  /*!< Flash backdoor is disabled.*/
} hal_flash_security_state_t;

#define HAL_FLASH_HANDLE_SIZE (104U)

/*! @brief Hal flash handle. */
typedef void *hal_flash_handle_t;

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */

/*! @brief Hal flash margin value. */
typedef enum _hal_flash_margin_value
{
    kHAL_Flash_MarginValueNormal = 0, /*!< Use the 'normal' read level for 1s.*/
    kHAL_Flash_MarginValueUser,       /*!< Apply the 'User' margin to the normal read-1 level.*/
    kHAL_Flash_MarginValueFactory,    /*!< Apply the 'Factory' margin to the normal read-1 level.*/
    kHAL_Flash_MarginValueInvalid     /*!< Not real margin level, Used to determine the range of valid margin level. */
} hal_flash_margin_value_t;

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Initialization
 * @{
 */

/*!
 * @brief Initializes the flash hal adapter module.
 *
 * This function initializes the Flash module for the other Flash APIs.
 *  @code
 *   uint32_t halFlashHandleBuffer[((HAL_FLASH_HANDLE_SIZE + sizeof(uint32_t) - 1) / sizeof(uitn32_t))];
 *   hal_flash_handle_t halFlashHandle = (hal_flash_handle_t)&halFlashHandleBuffer[0];
 *   HAL_FlashInit(halFlashHandle);
 *  @endcode
 *
 * @param halFlashHandle  Hal flash adapter handle, the handle buffer with size #HAL_FLASH_HANDLE_SIZE
 * should be allocated at upper level.
 * The handle should be 4 byte aligned, because unaligned access does not support on some devices.
 * @retval #kStatus_HAL_Flash_Success API was executed successfully.
 * @retval #kStatus_HAL_Flash_InvalidArgument An invalid argument is provided.
 * @retval #kStatus_HAL_Flash_ExecuteInRamFunctionNotReady Execute-in-RAM function is not available.
 * @retval #kStatus_HAL_Flash_PartitionStatusUpdateFailure Failed to update the partition status.
 */
hal_flash_status_t HAL_FlashInit(hal_flash_handle_t halFlashHandle);

/*!
 * \brief  Write aligned data to FLASH
 *
 * @param halFlashHandle  Hal flash adapter handle
 * @note This API should be called after HAL_FlashEraseSector was called to make sure the flash was erased, and
 *       make sure the flash program address and flash program size is aligned with flash write unit size.
 *
 * @param dest            The address of the Flash location
 * @param size            The number of bytes to be programed
 * @param pData           Pointer to the data to be programmed to Flash
 *
 * @retval #kStatus_HAL_Flash_Success API was executed successfully.
 *
 */
hal_flash_status_t HAL_FlashProgram(hal_flash_handle_t halFlashHandle, uint32_t dest, uint32_t size, uint8_t *pData);
/*!
 * \brief  Write data to FLASH
 *
 * @param halFlashHandle  Hal flash adapter handle
 * @note This API should be called after HAL_FlashEraseSector was called to make sure the flash was erased.
 * @param dest        The address of the Flash location
 * @param size        The number of bytes to be programed
 * @param pData       Pointer to the data to be programmed to Flash
 *
 * @retval #kStatus_HAL_Flash_Success API was executed successfully.
 *
 */
hal_flash_status_t HAL_FlashProgramUnaligned(hal_flash_handle_t halFlashHandle,
                                             uint32_t dest,
                                             uint32_t size,
                                             uint8_t *pData);

/*!
 * \brief Erase the Flash memory which is specified by the parameter dest and parameter size, the content of the
 * specified flash memory would be 0xFF.
 *
 * @param halFlashHandle  Hal flash adapter handle
 * @param dest            The start address of the first sector to be erased
 * @param size            The number of bytes to be erase
 *
 * @retval #kStatus_HAL_Flash_Success API was executed successfully.
 *
 */
hal_flash_status_t HAL_FlashEraseSector(hal_flash_handle_t halFlashHandle, uint32_t dest, uint32_t size);

/*!
 * \brief  Verify erase data in Flash
 *
 * @param halFlashHandle  Hal flash adapter handle
 * @param start           The address of the Flash location
 * @param lengthInBytes   The number of bytes to be checked
 * @param margin          Flash margin value
 * @retval #kStatus_HAL_Flash_Success API was executed successfully.
 */
hal_flash_status_t HAL_FlashVerifyErase(hal_flash_handle_t halFlashHandle,
                                        uint32_t start,
                                        uint32_t lengthInBytes,
                                        hal_flash_margin_value_t margin);
/*!
 * \brief  Read data from FLASH
 *
 * @param halFlashHandle  Hal flash adapter handle
 *
 * @param scr             The address of the Flash location to be read
 * @param size            The number of bytes to be read
 * @param pData           Pointer to the data to be read from Flash
 *
 * @retval #kStatus_HAL_Flash_Success API was executed successfully.
 *
 */
hal_flash_status_t HAL_FlashRead(hal_flash_handle_t halFlashHandle, uint32_t src, uint32_t size, uint8_t *pData);

/*!
 * @brief Returns the desired hal flash property.
 *
 * @param halFlashHandle  Hal flash adapter handle
 * @param Property        The desired property from the list of properties in
 *                        enum hal_flash_property_tag_t
 * @param value           A pointer to the value returned for the desired flash property.
 *
 * @retval #kStatus_HAL_Flash_Success API was executed successfully.
 * @retval #kStatus_HAL_Flash_InvalidArgument An invalid argument is provided.
 * @retval #kStatus_HAL_Flash_NotSupport Flash currently not support.
 */
hal_flash_status_t HAL_FlashGetProperty(hal_flash_handle_t halFlashHandle,
                                        hal_flash_property_tag_t property,
                                        uint32_t *value);

/*!
 * @brief Set the desired hal flash property.
 *
 * @param halFlashHandle  Hal flash adapter handle
 * @param Property        The desired property from the list of properties in
 *                        enum hal_flash_property_tag_t
 * @param value           The value would be set to the desired flash property.
 *
 * @retval #kStatus_HAL_Flash_Success API was executed successfully.
 * @retval #kStatus_HAL_Flash_InvalidArgument An invalid argument is provided.
 * @retval #kStatus_HAL_Flash_NotSupport Flash currently not support.
 */
hal_flash_status_t HAL_FlashSetProperty(hal_flash_handle_t halFlashHandle,
                                        hal_flash_property_tag_t property,
                                        uint32_t value);

/*!
 * @brief Returns the security state via the pointer passed into the function.
 *
 * This function retrieves the current flash security status, including the
 * security enabling state and the backdoor key enabling state.
 *
 * @param halFlashHandle  Hal flash adapter handle
 * @param state           A pointer to the value returned for the current security status
 *
 * @retval #kStatus_HAL_Flash_Success API was executed successfully.
 * @retval #kStatus_HAL_Flash_InvalidArgument An invalid argument is provided.
 * @retval #kStatus_HAL_Flash_NotSupport Flash currently not support.
 */
hal_flash_status_t HAL_FlashGetSecurityState(hal_flash_handle_t halFlashHandle, hal_flash_security_state_t *state);

#if defined(__cplusplus)
}
#endif
#endif /* __FLASH_H__ */
