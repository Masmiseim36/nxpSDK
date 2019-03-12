/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_dcp.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


#define DCP_TEST_USE_OTP_KEY 0 /* Set to 1 to select OTP key for AES encryption/decryption. */

#define TEST_ASSERT(a)       \
    if (!(a))                \
    {                        \
        PRINTF("error\r\n"); \
        do                   \
        {                    \
        } while (1);         \
    }

#if DCP_TEST_USE_OTP_KEY
typedef enum _dcp_otp_key_select
{
    kDCP_OTPMKKeyLow  = 1U, /* Use [127:0] from snvs key as dcp key */
    kDCP_OTPMKKeyHigh = 2U, /* Use [255:128] from snvs key as dcp key */
    kDCP_OCOTPKeyLow  = 3U, /* Use [127:0] from ocotp key as dcp key */
    kDCP_OCOTPKeyHigh = 4U  /* Use [255:128] from ocotp key as dcp key */
} dcp_otp_key_select;
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
#if DCP_TEST_USE_OTP_KEY
status_t DCP_OTPKeySelect(dcp_otp_key_select keySelect)
{
    if (keySelect == kDCP_OTPMKKeyLow)
    {
        IOMUXC_GPR->GPR3 &= ~(1 << IOMUXC_GPR_GPR3_DCP_KEY_SEL_SHIFT);
        IOMUXC_GPR->GPR10 &= ~(1 << IOMUXC_GPR_GPR10_DCPKEY_OCOTP_OR_KEYMUX_SHIFT);
    }

    else if (keySelect == kDCP_OTPMKKeyHigh)
    {
        IOMUXC_GPR->GPR3 |= (1 << IOMUXC_GPR_GPR3_DCP_KEY_SEL_SHIFT);
        IOMUXC_GPR->GPR10 &= ~(1 << IOMUXC_GPR_GPR10_DCPKEY_OCOTP_OR_KEYMUX_SHIFT);
    }

    else if (keySelect == kDCP_OCOTPKeyLow)
    {
        IOMUXC_GPR->GPR3 &= ~(1 << IOMUXC_GPR_GPR3_DCP_KEY_SEL_SHIFT);
        IOMUXC_GPR->GPR10 |= (1 << IOMUXC_GPR_GPR10_DCPKEY_OCOTP_OR_KEYMUX_SHIFT);
    }

    else if (keySelect == kDCP_OCOTPKeyHigh)
    {
        IOMUXC_GPR->GPR3 |= (1 << IOMUXC_GPR_GPR3_DCP_KEY_SEL_SHIFT);
        IOMUXC_GPR->GPR10 |= (1 << IOMUXC_GPR_GPR10_DCPKEY_OCOTP_OR_KEYMUX_SHIFT);
    }

    else
    {
        return kStatus_InvalidArgument;
    }

    return kStatus_Success;
}
#endif

void TestAesEcb(void)
{
    static const uint8_t keyAes128[] __attribute__((aligned)) = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                                                                 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    static const uint8_t plainAes128[]                        = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
                                          0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
    static const uint8_t cipherAes128[]                       = {0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60,
                                           0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97};
#if DCP_TEST_USE_OTP_KEY
#warning Please update cipherAes128 variables to match expected AES ciphertext for your OTP key.
#endif

    uint8_t cipher[16];
    uint8_t output[16];
    status_t status;

    dcp_handle_t m_handle;

    m_handle.channel    = kDCP_Channel0;
    m_handle.swapConfig = kDCP_NoSwap;

#if DCP_TEST_USE_OTP_KEY
    m_handle.keySlot = kDCP_OtpKey;
#else
    m_handle.keySlot = kDCP_KeySlot0;
#endif

    status = DCP_AES_SetKey(DCP, &m_handle, keyAes128, 16);
    TEST_ASSERT(kStatus_Success == status);

    DCP_AES_EncryptEcb(DCP, &m_handle, plainAes128, cipher, 16);
    TEST_ASSERT(memcmp(cipher, cipherAes128, 16) == 0);

    DCP_AES_DecryptEcb(DCP, &m_handle, cipher, output, 16);
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
#if DCP_TEST_USE_OTP_KEY
#warning Please update cipherAes128 variables to match expected AES ciphertext for your OTP key.
#endif

    uint8_t cipher[16];
    uint8_t output[16];
    status_t status;

    dcp_handle_t m_handle;

    m_handle.channel    = kDCP_Channel0;
    m_handle.swapConfig = kDCP_NoSwap;

#if DCP_TEST_USE_OTP_KEY
    m_handle.keySlot = kDCP_OtpKey;
#else
    m_handle.keySlot = kDCP_KeySlot0;
#endif

    status = DCP_AES_SetKey(DCP, &m_handle, keyAes128, 16);
    TEST_ASSERT(kStatus_Success == status);

    DCP_AES_EncryptCbc(DCP, &m_handle, plainAes128, cipher, 16, ive);
    TEST_ASSERT(memcmp(cipher, cipherAes128, 16) == 0);

    DCP_AES_DecryptCbc(DCP, &m_handle, cipher, output, 16, ive);
    TEST_ASSERT(memcmp(output, plainAes128, 16) == 0);

    PRINTF("AES CBC Test pass\r\n");
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

    dcp_handle_t m_handle;

    m_handle.channel    = kDCP_Channel0;
    m_handle.keySlot    = kDCP_KeySlot0;
    m_handle.swapConfig = kDCP_NoSwap;

    length    = sizeof(message) - 1;
    outLength = sizeof(output);
    memset(&output, 0, outLength);

    /************************ SHA-1 **************************/
    status = DCP_HASH(DCP, &m_handle, kDCP_Sha1, message, length, output, &outLength);
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

    dcp_handle_t m_handle;

    m_handle.channel    = kDCP_Channel0;
    m_handle.keySlot    = kDCP_KeySlot0;
    m_handle.swapConfig = kDCP_NoSwap;

    length    = sizeof(message) - 1;
    outLength = sizeof(output);
    memset(&output, 0, outLength);

    /************************ SHA-256 **************************/
    status = DCP_HASH(DCP, &m_handle, kDCP_Sha256, message, length, output, &outLength);
    TEST_ASSERT(kStatus_Success == status);
    TEST_ASSERT(outLength == 32u);
    TEST_ASSERT(memcmp(output, sha256, outLength) == 0);

    PRINTF("SHA-256 Test pass\r\n");
}

void TestCrc32(void)
{
    status_t status;
    size_t outLength;
    unsigned int length;
    unsigned char output[4];

    static const uint8_t message[] = "abcdbcdecdefdefgefghfghighijhijk";

    /* Expected CRC-32 for the message.
     * CRC-32 params:
     * width=32 poly=0x04c11db7 init=0xffffffff refin=false refout=false xorout=0x00000000
     * http://reveng.sourceforge.net/crc-catalogue/
     */
    static const unsigned char crc32[] = {0x7f, 0x04, 0x6a, 0xdd};

    dcp_handle_t m_handle;

    m_handle.channel    = kDCP_Channel0;
    m_handle.keySlot    = kDCP_KeySlot0;
    m_handle.swapConfig = kDCP_NoSwap;

    length    = sizeof(message) - 1;
    outLength = sizeof(output);
    memset(&output, 0, outLength);

    /************************ CRC-32 **************************/
    status = DCP_HASH(DCP, &m_handle, kDCP_Crc32, message, length, output, &outLength);
    TEST_ASSERT(kStatus_Success == status);
    TEST_ASSERT(outLength == 4u);
    TEST_ASSERT(memcmp(output, crc32, outLength) == 0);

    PRINTF("CRC-32 Test pass\r\n");
}

/*!
 * @brief Main function
 */
int main(void)
{
    dcp_config_t dcpConfig;

    /* Init hardware*/
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Data cache must be temporarily disabled to be able to use sdram */
    SCB_DisableDCache();

    PRINTF("DCP Driver Example\r\n\r\n");

    /* Initialize DCP */
    DCP_GetDefaultConfig(&dcpConfig);

#if DCP_TEST_USE_OTP_KEY
    /* Set OTP key type in IOMUX registers before initializing DCP. */
    /* Software reset of DCP must be issued after changing the OTP key type. */
    DCP_OTPKeySelect(kDCP_OTPMKKeyLow);
#endif

    /* Reset and initialize DCP */
    DCP_Init(DCP, &dcpConfig);

    /* Call DCP APIs */
    TestAesEcb();
    TestAesCbc();
    TestSha1();
    TestSha256();
    TestCrc32();

    /* Deinitialize DCP */
    DCP_Deinit(DCP);

    while (1)
    {
    }
}
