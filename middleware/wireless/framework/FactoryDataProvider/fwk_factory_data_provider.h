/* -------------------------------------------------------------------------- */
/*                             Copyright 2023 NXP                             */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

#ifndef _FWK_FACTORY_DATA_STORAGE_H_
#define _FWK_FACTORY_DATA_STORAGE_H_

/*!
 * \file fwk_factory_data_provider.h
 * \brief Factory Data Provider module for Matter
 *
 * This module provides an interface to retrieve factory data used in Matter context.
 * Currently, the module assumes the factory data is placed in flash at the address __FACTORY_DATA_START_OFFSET
 * The data can be encrypted in AES 128 ECB.
 *
 * At module initialization (FDP_Init), the data is loaded in a RAM buffer and optionally decrypted if a AES128 key is
 * provided by the user.
 *
 * Once the initialization is done, the data can be retrieved by the user using FDP_SearchForId.
 *
 * Currently, the factory data must comply with a specific structure and it is recommended to generate it by following
 * the instructions:
 * https://github.com/project-chip/connectedhomeip/blob/master/examples/platform/nxp/doc/manufacturing_flow.md\
 *
 * Note: This module requires mbedtls, FreeRTOS and is validated on ARMGCC and IAR toolchains.
 *
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <stdint.h>

/* -------------------------------------------------------------------------- */
/*                               Public macros                                */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                Public types                                */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                Public memory                               */
/* -------------------------------------------------------------------------- */

/*!
 * \brief The following symbols give information on the factory data location and must be provided by the linker
 *        __FACTORY_DATA_START_OFFSET: Offset address of the factory data relative to the start address of the flash
 *        __FACTORY_DATA_SIZE: Size of the whole factory data section, including the header
 *
 */
extern uint32_t __FACTORY_DATA_START_OFFSET[];
extern uint32_t __FACTORY_DATA_SIZE[];

/* -------------------------------------------------------------------------- */
/*                             Public prototypes                              */
/* -------------------------------------------------------------------------- */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * \brief Initialize the Factory Data Provider module
 *
 * \return int >=0 for success, <0 for errors
 */
int FDP_Init(const uint8_t *aes128Key);

/*!
 * \brief Browse the Factory Data Provider for a specific id and provides a pointer to the data in RAM.
 *        Optionally copies the same data to a buffer provided by the user.
 *
 * \param[in] id id to search for in the Factory Data
 * \param[in] pBuffer pointer to a buffer to copy the data to. Ignored in case it's NULL.
 * \param[in] bufferLength size of the buffer the data will be copied to. Ignored in case pBuffer is NULL.
 * \param[out] dataLen actual size of the content read from the Factory Data. 0 is the data is not found.
 * \return uint8_t* pointer to the data in RAM if the data is found. NULL if not found.
 */
uint8_t *FDP_SearchForId(uint8_t id, uint8_t *pBuffer, uint32_t bufferLength, uint32_t *dataLen);

// TODO: Import the following functions from the Matter FactoryDataProvider module
// This will be needed to improve the security of factory data, because in the current implementation,
// SignWithDeviceAttestationKey reads the private key with FDS_ReadData so the private key is loaded in RAM in
// plained text.
// The idea would be to separate the private key from the rest of the factory data and use our secure enclaves to
// handle the signing procedure
// GetDeviceAttestationCert
// GetProductAttestationIntermediateCert
// SignWithDeviceAttestationKey
// GetSetupDiscriminator
// GetSpake2pIterationCount
// GetSpake2pSalt
// GetSpake2pVerifier

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _FWK_FACTORY_DATA_STORAGE_H_ */
