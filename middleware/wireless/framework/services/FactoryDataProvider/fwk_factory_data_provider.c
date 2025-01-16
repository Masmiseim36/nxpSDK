/*
 * Copyright 2023,2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <stdbool.h>
#include <stdalign.h>

#include "fwk_factory_data_provider.h"

#include "fsl_common.h"
#include "mflash_drv.h"

/* FreeRTOS */
#include "FreeRTOS.h"
#include "portable.h"

/* mbedtls */
#include "mbedtls/aes.h"
#include "mbedtls/sha256.h"

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */

#define HASH_ID 0xCE47BA5E

/* -------------------------------------------------------------------------- */
/*                                Private types                               */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Describes the Factory Data stored in flash
 *
 * hashId: magic pattern to check if the structure is present in flash
 * factoryDataStorageSize: Size of the factory data located after the header
 * hash256: 4 first bytes of the hash256 computed on the plain text factory data
 *          When the data is loaded in RAM, a hash is computed and must be identical to this one to validate the data
 *          Only the 4 first bytes are stored in flash to limit the footprint on some constrained platforms
 * pFactoryData: Pointer to the buffer in RAM where the factory data is loaded
 */
typedef struct
{
    uint32_t hashId;
    uint32_t factoryDataStorageSize;
    uint8_t  hash256[4];
    uint8_t *pFactoryData;
} factory_data_storage_t;

typedef struct __attribute__((__packed__))
{
    uint8_t  id;
    uint16_t length;
} factory_data_header_t;

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

static int FDP_DecryptAes128Ecb(uint8_t *dest, uint8_t *source, const uint8_t *aes128Key);
static int FDP_Hash256(uint8_t *pInput, uint16_t inputLength, uint8_t *pOutput);

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

static alignas(4) factory_data_storage_t factoryDataStorage;

static const uint8_t *pAes128Key;

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

int FDP_Init(const uint8_t *aes128Key)
{
    int      status                    = 0;
    uint32_t expectedHashId            = HASH_ID;
    uint32_t factoryDataStorageAddress = (uint32_t)__FACTORY_DATA_START_OFFSET;
    uint32_t currentBlock[16 / sizeof(uint32_t)];
    uint8_t  currentHash256[32];

    do
    {
        /* Retrieve the size of the factory data from the linker and allocate a buffer of the exact same size */
        factoryDataStorage.factoryDataStorageSize = (uint32_t)__FACTORY_DATA_SIZE;
        /* a malloc allows to correctly fit the buffer to the size of the data */
        factoryDataStorage.pFactoryData = pvPortMalloc(factoryDataStorage.factoryDataStorageSize);

        pAes128Key = aes128Key;

        if (factoryDataStorage.pFactoryData == NULL)
        {
            status = -1;
            break;
        }

        /* TODO: should we abstract this API in a wrapper ? or a port layer ?
         * Should we initialize the driver here ? */
        if (mflash_drv_init() != kStatus_Success)
        {
            status = -2;
            break;
        }

        /* Read the whole factory data storage header oneshot from flash */
        if (mflash_drv_read(factoryDataStorageAddress, (uint32_t *)&factoryDataStorage,
                            offsetof(factory_data_storage_t, pFactoryData)) != kStatus_Success)
        {
            status = -3;
            break;
        }

        /* FDP hash must match expected one */
        if (memcmp(&factoryDataStorage.hashId, &expectedHashId, sizeof(factoryDataStorage.hashId)) != 0)
        {
            status = -4;
            break;
        }

        factoryDataStorageAddress += offsetof(factory_data_storage_t, pFactoryData);
        /* Load the buffer into RAM by reading each 16 bytes blocks */
        for (int i = 0; i < (factoryDataStorage.factoryDataStorageSize / 16); i++)
        {
            if (mflash_drv_read(factoryDataStorageAddress + (i * 16), currentBlock, sizeof(currentBlock)) !=
                kStatus_Success)
            {
                status = -7;
                break;
            }

            if (pAes128Key != NULL)
            {
                /* Decrypt data if a key has been set */
                if (FDP_DecryptAes128Ecb(&factoryDataStorage.pFactoryData[i * 16], (uint8_t *)&currentBlock[0],
                                         pAes128Key) != 0)
                {
                    status = -8;
                    break;
                }
            }
            else
            {
                /* No key was set, copy the data as is */
                memcpy(&factoryDataStorage.pFactoryData[i * 16], currentBlock, sizeof(currentBlock));
            }
        }

        /* Calculate SHA256 value over the factory data and compare with stored value */
        if (FDP_Hash256(&factoryDataStorage.pFactoryData[0], factoryDataStorage.factoryDataStorageSize,
                        &currentHash256[0]) != 0)
        {
            status = -8;
            break;
        }

        if (memcmp(&currentHash256[0], &factoryDataStorage.hash256[0], 4) != 0)
        {
            status = -9;
            break;
        }

    } while (false);

    if ((status != 0) && (factoryDataStorage.pFactoryData != NULL))
    {
        vPortFree(factoryDataStorage.pFactoryData);
        factoryDataStorage.pFactoryData = NULL;
    }

    return status;
}

uint8_t *FDP_SearchForId(uint8_t id, uint8_t *pBuffer, uint32_t bufferLength, uint32_t *dataLen)
{
    uint32_t index     = 0;
    uint8_t *data      = NULL;
    uint16_t lenToCopy = 0;

    while (index < factoryDataStorage.factoryDataStorageSize)
    {
        factory_data_header_t *pDataHeader = (factory_data_header_t *)&factoryDataStorage.pFactoryData[index];

        /* Check if the type gotten is the expected one */
        if (id == pDataHeader->id)
        {
            data     = ((uint8_t *)pDataHeader + sizeof(factory_data_header_t));
            *dataLen = pDataHeader->length;

            /* If pBuffer is NULL, ignore the copy */
            if (pBuffer != NULL)
            {
                /* If the buffer given is too small, fill only the available space */
                if (bufferLength < pDataHeader->length)
                {
                    lenToCopy = bufferLength;
                }
                else
                {
                    lenToCopy = pDataHeader->length;
                }

                memcpy((uint8_t *)pBuffer, data, lenToCopy);
            }

            break;
        }
        else if (pDataHeader->id == 0)
        {
            /* No more type available , break the loop */
            break;
        }
        else
        {
            /* Jump to next data */
            index += sizeof(factory_data_header_t) + pDataHeader->length;
        }
    }

    return data;
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static int FDP_DecryptAes128Ecb(uint8_t *dest, uint8_t *source, const uint8_t *aes128Key)
{
    int                 status = 0;
    mbedtls_aes_context aesCtx;

    do
    {
        mbedtls_aes_init(&aesCtx);
        if (mbedtls_aes_setkey_dec(&aesCtx, aes128Key, 128U) != 0)
        {
            status = -1;
            break;
        }

        if (mbedtls_aes_crypt_ecb(&aesCtx, MBEDTLS_AES_DECRYPT, source, dest) != 0)
        {
            status = -2;
            break;
        }

        mbedtls_aes_free(&aesCtx);
    } while (false);

    return status;
}

static int FDP_Hash256(uint8_t *pInput, uint16_t inputLength, uint8_t *pOutput)
{
    int                    status = 0;
    mbedtls_sha256_context shaCtx;

    do
    {
        mbedtls_sha256_init(&shaCtx);
        if (mbedtls_sha256_starts_ret(&shaCtx, 0) != 0)
        {
            status = -1;
            break;
        }

        if (mbedtls_sha256_update_ret(&shaCtx, pInput, inputLength) != 0)
        {
            status = -2;
            break;
        }

        if (mbedtls_sha256_finish_ret(&shaCtx, pOutput) != 0)
        {
            status = -3;
            break;
        }
    } while (false);

    return status;
}
