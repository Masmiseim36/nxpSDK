/*
 * Copyright 2019-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexspi.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_otfad.h"
#if CACHE_MAINTAIN
#include "fsl_cache.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_plain_text[16];
static uint8_t s_cipher_text[16];
static uint8_t s_decrypted_text[16];

extern status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address);
extern status_t flexspi_nor_flash_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint32_t length);
extern status_t flexspi_nor_enable_quad_mode(FLEXSPI_Type *base);
extern status_t flexspi_nor_erase_chip(FLEXSPI_Type *base);
extern void flexspi_nor_flash_init(FLEXSPI_Type *base);
extern void flexspi_nor_disable_cache(flexspi_cache_status_t *cacheStatus);
extern void flexspi_nor_enable_cache(flexspi_cache_status_t cacheStatus);

void OTFAD_GetEncryptedCounter(uint8_t *counter);
void OTFAD_EncryptData(uint8_t *counter, uint8_t *plainText, uint8_t *cipherText);
/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    uint32_t i      = 0;
    status_t status = kStatus_Success;
    otfad_config_t config;
    uint8_t encryptedCounter[16];
    uint32_t sysAddress = EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE;
    otfad_encryption_config_t encryptionConfig;
    uint8_t contextHit;

    BOARD_InitHardware();

    /*
     * config.misc.interruptRequest    = false;
     * config.misc.forceError          = false;
     * config.misc.forceSVM            = false;
     * config.misc.forceLDM            = false;
     * config.misc.keyBlobScramble     = false;
     * config.misc.keyBlobProcess      = false;
     * config.misc.restrictedRegAccess = false;
     * config.startKeyBlobProcessing = false;
     * config.enableOTFAD = false;
     */
    OTFAD_GetDefaultConfig(&config);

    /* Global OTFAD enbale */
    config.enableOTFAD = true;
    /* Flexspi base address */
    config.flexspiBaseAddr = EXAMPLE_FLEXSPI;

    /* Initialize FlexSPI */
    flexspi_nor_flash_init(EXAMPLE_FLEXSPI);

    PRINTF("\r\nOTFAD example started!\r\n");

    /* Initialize OTFAD. */
    OTFAD_Init(EXAMPLE_OTFAD, &config);

    encryptionConfig.contextIndex  = 0U;
    encryptionConfig.valid         = true;
    encryptionConfig.AESdecryption = true;
    encryptionConfig.readOnly      = 0U;
    /*
     * start address = "0x60008000",
     * end address   = "0x60008fff",
     */
    encryptionConfig.startAddr = EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE;
    encryptionConfig.endAddr   = EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE + 0xfffU;
    /* Key: 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F */
    encryptionConfig.key[0] = 0x03020100;
    encryptionConfig.key[1] = 0x07060504;
    encryptionConfig.key[2] = 0x0B0A0908;
    encryptionConfig.key[3] = 0x0F0E0D0C;
    /* Counter: 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF */
    encryptionConfig.counter[0] = 0x67452301;
    encryptionConfig.counter[1] = 0xEFCDAB89;

    status = OTFAD_SetEncryptionConfig(EXAMPLE_OTFAD, &encryptionConfig);

    if (status != kStatus_Success)
    {
        PRINTF("AES encryption configuration failure\r\n");
        return -1;
    }

    status = OTFAD_HitDetermination(EXAMPLE_OTFAD, sysAddress, &contextHit);
    if (status != kStatus_Success)
    {
        return status;
    }
    else
    {
        PRINTF("Hitted context: 0x%x\r\n", contextHit);
    }

#if !(defined(XIP_EXTERNAL_FLASH))
    /* Erase whole chip . */
    PRINTF("Erasing whole chip over FlexSPI...\r\n");

    status = flexspi_nor_erase_chip(EXAMPLE_FLEXSPI);
    if (status != kStatus_Success)
    {
        return status;
    }
    else
    {
        PRINTF("Erase whole chip finished !\r\n");
    }
#endif

    /* Enter quad mode. */
    status = flexspi_nor_enable_quad_mode(EXAMPLE_FLEXSPI);
    if (status != kStatus_Success)
    {
        return status;
    }

    PRINTF("Erasing Serial NOR over FlexSPI...\r\n");

    /* Erase sectors. */
    status = flexspi_nor_flash_erase_sector(EXAMPLE_FLEXSPI, EXAMPLE_SECTOR * SECTOR_SIZE);

    if (status != kStatus_Success)
    {
        PRINTF("Erase sector failure !\r\n");
        return -1;
    }
    else
    {
        PRINTF("Erase sector finished !\r\n");
    }

    /* Get encrypted counter for encryption and decryption */
    OTFAD_GetEncryptedCounter(encryptedCounter);

    for (i = 0U; i < 16U; i++)
    {
        s_plain_text[i] = i;
    }

    /* Encrypt data with encrypted counter */
    OTFAD_EncryptData(encryptedCounter, s_plain_text, s_cipher_text);

    /* Write ciphertext into external memory */
    status = flexspi_nor_flash_program(EXAMPLE_FLEXSPI, EXAMPLE_SECTOR * SECTOR_SIZE, (void *)s_cipher_text,
                                       sizeof(s_cipher_text));

    if (status != kStatus_Success)
    {
        PRINTF("Ciphertext program failure !\r\n");
        return -1;
    }

#if CACHE_MAINTAIN
    /*
     * Invalidate the cache, so new read will read from memory directly.
     */
    DCACHE_InvalidateByRange(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE, FLASH_PAGE_SIZE);
#endif

    memcpy(s_decrypted_text, (void *)(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE),
           sizeof(s_decrypted_text));

    if (memcmp(s_decrypted_text, s_plain_text, sizeof(s_decrypted_text)) != 0)
    {
        PRINTF("Decrypted data -  Decryption error !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("Decrypted data - successfully. \r\n");
    }

    while (1)
    {
    }
}

void OTFAD_GetEncryptedCounter(uint8_t *counter)
{
    assert(counter != NULL);

    status_t status     = kStatus_Success;
    uint32_t sysAddress = EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE;
    uint8_t contextHit;

    uint8_t i;
    uint8_t plainText[16]     = {0x00U};
    uint8_t *encryptedCounter = counter;

    status = OTFAD_HitDetermination(EXAMPLE_OTFAD, sysAddress, &contextHit);

    if (status == kStatus_Success)
    {
        /* Erase sector */
        status = flexspi_nor_flash_erase_sector(EXAMPLE_FLEXSPI, EXAMPLE_SECTOR * SECTOR_SIZE);
    }

#if CACHE_MAINTAIN
    /*
     * Invalidate the cache, so new read will read from memory directly.
     */
    DCACHE_InvalidateByRange(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE, FLASH_PAGE_SIZE);
#endif

    if (kStatus_Success == status)
    {
        /* Read 16 bytes plain text */
        memcpy(plainText, (void *)(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE), sizeof(plainText));
    }

    for (i = 0U; i < 16U; i++)
    {
        /* Method:
         * cipherText = encryptedCounter ^ plainText = 0xFF
         * encryptedCounter = cipherText ^ 0xFF = plainText ^ 0xFF
         */
        encryptedCounter[i] = plainText[i] ^ 0xFF;
    }
}

void OTFAD_EncryptData(uint8_t *counter, uint8_t *plainText, uint8_t *cipherText)
{
    assert(counter != NULL);
    assert(plainText != NULL);
    assert(cipherText != NULL);

    uint8_t i;

    for (i = 0U; i < 16U; i++)
    {
        cipherText[i] = counter[i] ^ plainText[i];
    }
}
