/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_hashcrypt.h"

#include <string.h>

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define TEST_ASSERT(a)       \
    if (!(a))                \
    {                        \
        PRINTF("error\r\n"); \
        do                   \
        {                    \
        } while (1);         \
    }

#define CRYPTO_TEST_OUT_ARRAY_LEN 0x100
#define CRYPTO_SHA1_OUT_LEN 20
#define CRYPTO_SHA256_OUT_LEN 32

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void TestAesEcb(void)
{
    static const uint8_t keyAes128[] __attribute__((aligned)) = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                                                                 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    static const uint8_t plainAes128[]                        = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
                                          0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
    static const uint8_t cipherAes128[]                       = {0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60,
                                           0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97};
    uint8_t cipher[16];
    uint8_t output[16];
    status_t status;

    hashcrypt_handle_t m_handle;

    m_handle.keyType = kHASHCRYPT_UserKey;

    status = HASHCRYPT_AES_SetKey(HASHCRYPT, &m_handle, keyAes128, 16);

    TEST_ASSERT(kStatus_Success == status);

    status = HASHCRYPT_AES_EncryptEcb(HASHCRYPT, &m_handle, plainAes128, cipher, 16);
    TEST_ASSERT(kStatus_Success == status);
    TEST_ASSERT(memcmp(cipher, cipherAes128, 16) == 0);

    status = HASHCRYPT_AES_DecryptEcb(HASHCRYPT, &m_handle, cipher, output, 16);
    TEST_ASSERT(kStatus_Success == status);
    TEST_ASSERT(memcmp(output, plainAes128, 16) == 0);

    PRINTF("AES ECB Test pass\r\n");
}

void TestAesCbc(void)
{
    static const uint8_t keyAes128[] __attribute__((aligned)) = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                                                                 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    static const uint8_t plainAes128[]                        = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
                                          0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
    static const uint8_t ive[]                                = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

    static const uint8_t cipherAes128[] = {0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46,
                                           0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d};

    uint8_t cipher[16];
    uint8_t output[16];
    status_t status;

    hashcrypt_handle_t m_handle;

    m_handle.keySize = kHASHCRYPT_Aes128;
    m_handle.keyType = kHASHCRYPT_UserKey;

    status = HASHCRYPT_AES_SetKey(HASHCRYPT, &m_handle, keyAes128, 16);
    TEST_ASSERT(kStatus_Success == status);

    status = HASHCRYPT_AES_EncryptCbc(HASHCRYPT, &m_handle, plainAes128, cipher, 16, ive);
    TEST_ASSERT(kStatus_Success == status);
    TEST_ASSERT(memcmp(cipher, cipherAes128, 16) == 0);

    status = HASHCRYPT_AES_DecryptCbc(HASHCRYPT, &m_handle, cipher, output, 16, ive);
    TEST_ASSERT(kStatus_Success == status);
    TEST_ASSERT(memcmp(output, plainAes128, 16) == 0);

    PRINTF("AES CBC Test pass\r\n");
}

void TestAesCtr(void)
{
    static const uint8_t aes_ctr_test01_key[] __attribute__((aligned)) = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    static uint8_t aes_ctr_test01_counter_1[16]      = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
                                                   0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};
    static uint8_t aes_ctr_test01_counter_2[16]      = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
                                                   0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};
    static uint8_t aes_ctr_test01_plaintext[]        = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
                                                 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
    static const uint8_t aes_ctr_test01_ciphertext[] = {0x87, 0x4d, 0x61, 0x91, 0xb6, 0x20, 0xe3, 0x26,
                                                        0x1b, 0xef, 0x68, 0x64, 0x99, 0x0d, 0xb6, 0xce};
    uint8_t cipher[16]                               = {0};
    uint8_t output[16]                               = {0};
    status_t status;

    hashcrypt_handle_t m_handle;

    m_handle.keySize = kHASHCRYPT_Aes128;
    m_handle.keyType = kHASHCRYPT_UserKey;

    status = HASHCRYPT_AES_SetKey(HASHCRYPT, &m_handle, aes_ctr_test01_key, 16);
    TEST_ASSERT(kStatus_Success == status);

    /* Encrypt */
    status = HASHCRYPT_AES_CryptCtr(HASHCRYPT, &m_handle, aes_ctr_test01_plaintext, cipher, 16,
                                    aes_ctr_test01_counter_1, NULL, NULL);
    TEST_ASSERT(kStatus_Success == status);
    TEST_ASSERT(memcmp(cipher, aes_ctr_test01_ciphertext, 16) == 0);

    /* Decrypt */
    status = HASHCRYPT_AES_CryptCtr(HASHCRYPT, &m_handle, cipher, output, 16, aes_ctr_test01_counter_2, NULL, NULL);
    TEST_ASSERT(kStatus_Success == status);
    TEST_ASSERT(memcmp(output, aes_ctr_test01_plaintext, sizeof(aes_ctr_test01_plaintext)) == 0);

    PRINTF("AES CTR Test pass\r\n");
}

void TestSha1(void)
{
    status_t status;
    size_t outLength;
    unsigned int length;
    unsigned char output[20];

    static const uint8_t message[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";

    /* Expected SHA-1 for the message. */
    static const unsigned char sha1[] = {0x84, 0x98, 0x3e, 0x44, 0x1c, 0x3b, 0xd2, 0x6e, 0xba, 0xae,
                                         0x4a, 0xa1, 0xf9, 0x51, 0x29, 0xe5, 0xe5, 0x46, 0x70, 0xf1};

    length    = sizeof(message) - 1;
    outLength = sizeof(output);
    memset(&output, 0, outLength);

    /************************ SHA-1 **************************/
    status = HASHCRYPT_SHA(HASHCRYPT, kHASHCRYPT_Sha1, message, length, output, &outLength);
    TEST_ASSERT(kStatus_Success == status);
    TEST_ASSERT(outLength == 20u);
    TEST_ASSERT(memcmp(output, sha1, outLength) == 0);

    PRINTF("SHA-1 Test pass\r\n");
}

void TestSha256(void)
{
    status_t status;
    size_t outLength;
    unsigned int length;
    unsigned char output[32];

    static const uint8_t message[] =
        "Be that word our sign of parting, bird or fiend! I shrieked upstarting"
        "Get thee back into the tempest and the Nights Plutonian shore!"
        "Leave no black plume as a token of that lie thy soul hath spoken!"
        "Leave my loneliness unbroken! quit the bust above my door!"
        "Take thy beak from out my heart, and take thy form from off my door!"
        "Quoth the raven, Nevermore.  ";

    /* Expected SHA-256 for the message. */
    static const unsigned char sha256[] = {0x63, 0x76, 0xea, 0xcc, 0xc9, 0xa2, 0xc0, 0x43, 0xf4, 0xfb, 0x01,
                                           0x34, 0x69, 0xb3, 0x0c, 0xf5, 0x28, 0x63, 0x5c, 0xfa, 0xa5, 0x65,
                                           0x60, 0xef, 0x59, 0x7b, 0xd9, 0x1c, 0xac, 0xaa, 0x31, 0xf7};

    length    = sizeof(message) - 1;
    outLength = sizeof(output);
    memset(&output, 0, outLength);

    /************************ SHA-256 **************************/
    status = HASHCRYPT_SHA(HASHCRYPT, kHASHCRYPT_Sha256, message, length, output, &outLength);
    TEST_ASSERT(kStatus_Success == status);
    TEST_ASSERT(outLength == 32u);
    TEST_ASSERT(memcmp(output, sha256, outLength) == 0);

    PRINTF("SHA-256 Test pass\r\n");
}

/*!
 * @brief Main function.
 */
int main(void)
{
    /* Init hardware */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Initialize Hashcrypt */
    HASHCRYPT_Init(HASHCRYPT);

    /* Call HASH APIs */
    TestAesEcb();
    TestAesCbc();
    TestAesCtr();
    TestSha1();
    TestSha256();

    HASHCRYPT_Deinit(HASHCRYPT);

    while (1)
    {
    }
}
