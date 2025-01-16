/*! *********************************************************************************
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * \file
 *
 * This is the header file for the OTW: Over The Wire module
 *
 ** SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */
#ifndef _FWK_OTW_H_
#define _FWK_OTW_H_

/*!
 * @addtogroup OTW
 * Over The Wire (OTW) module.
 *
 * OTW module provides services to read/write data from a host device
 * to a remote device connected via a serial link.
 *
 * INFORMATION
 *
 * Specifically this module could provide the following services:
 * - Link init: Initialize the serial link and the exchange protocol
 *   between the host and the remote device.
 * - firmware download from the host device to a remote device.
 * - flash erase of the remote device.
 * - write data to the remote device.
 * - compare/read data gotten from the remote device.
 *
 * Example:
 * A dedicated implementation of this module allows a host device to access/read
 * data to a remote k32w0 device connected to a host over UART.
 * The implementation could be found in k32w0_transceiver/fwk_otw.c
 *
 * @{
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* -------------------------------------------------------------------------- */
/*                                Public types                                */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Enumerated data type definition for OTW error code/status
 */
typedef enum
{
    E_OTW_OK,
    E_OTW_FAILURE,
    E_OTW_GPIO_FAILURE,
    E_OTW_BUSY,
    E_OTW_UART_FAILURE,
    E_OTW_CRC_FAILURE,
    E_OTW_FILE_SIZE,
    E_OTW_ARG_FAILURE,
    E_OTW_WRITE_FAILURE,
    E_OTW_READ_COMPARE_FAILURE,
    E_OTW_MODULE_NOT_INITIALIZED,
    E_OTW_FIRMWARE_UPDATE_REQUIRED,
    E_OTW_FIRMWARE_UPDATE_NOT_REQUIRED,
} eOtwStatus;

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Initializes the OTW module and initilize the serial link.
 *        After calling this function the device should be able to
 *        do any operation on the remote device.
 *
 * \return eOtwStatus E_OTW_OK for success, or any other error value
 */
eOtwStatus Otw_Init(void);

/*!
 * \brief De-Initiliazes the OTW module and shutdown the serial link.
 *
 * \return eOtwStatus E_OTW_OK for success, or any other error value
 */
eOtwStatus Otw_DeInit(void);

/*!
 * \brief Download the firmware given to the remote device.
 *        Notes: This function will initialize the OTW module if not already done.
 *               Also at the end of the function the OTW module will be de-initialized.
 * \param[in] firmwareData : address of the firmware to load
 * \param[in] firmwareDataLen : size of the firmware to load
 * \param[in] verifyAfterWrite : verify if the firmware has been correclty loaded after the write process
 * \return eOtwStatus E_OTW_OK for success, or any other error value
 */
eOtwStatus Otw_FirmwareDownload(const uint8_t *firmwareData, uint32_t firmwareDataLen, bool verifyAfterWrite);

/*!
 * \brief Check if the remote device requires a firmware update or not
 *        Prerequisite: OTW module must be initialized
 *
 * \param[in] firmwareData : firmware address to compare
 * \param[in] firmwareDataLen : firmware length to compare
 * \return eOtwStatus E_OTW_FIRMWARE_UPDATE_REQUIRED or E_OTW_FIRMWARE_UPDATE_NOT_REQUIRED, or any other error value
 */
eOtwStatus Otw_IsUpdateRequired(const uint8_t *firmwareData, uint32_t firmwareDataLen);

/*!
 * \brief Erase the whole flash of the remote device
 *        Prerequisite: OTW module must be initialized
 * \return eOtwStatus E_OTW_OK for success, or any other error value
 */
eOtwStatus Otw_EraseAllFlashProcedure(void);

/*!
 * \brief Write the given data to the remote device at a specific offset
 *        Prerequisite: OTW module must be initialized
 * \param[in] pData
 * \param[in] dataLen
 * \param[in] offset to start to write
 * \return eOtwStatus E_OTW_OK for success, or any other error value
 */
eOtwStatus Otw_WriteToFlash(const uint8_t *pData, uint32_t dataLen, uint32_t offset);

/*!
 * \brief Compare the data given with the data read in the remote device at a specific offset
 *        Prerequisite: OTW module must be initialized
 * \param[in] pData reference address data
 * \param[in] dataLen reference address data length
 * \param[in] offset to start to read
 * \return eOtwStatus E_OTW_OK for success, or any other error value
 */
eOtwStatus Otw_ReadFromFlashAndCompare(const uint8_t *pData, uint32_t dataLen, uint32_t offset);

/*!
 * \brief Reset the remote device
 *
 * \return eOtwStatus E_OTW_OK for success, or any other error value
 */
eOtwStatus Otw_Reset(void);

/*!
 * @}  end of OTW addtogroup
 */

#ifdef __cplusplus
}
#endif

#endif /* _FWK_OTW_H_ */