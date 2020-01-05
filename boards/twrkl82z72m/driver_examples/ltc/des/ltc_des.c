/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

#include "fsl_ltc.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define OUTPUT_ARRAY_LEN 512
#define DES_BLOCK_LENGTH 8
#define DES_KEY_LENGTH 8

/* LTC peripheral base address */
LTC_Type *base = LTC0;

/*8-byte multiple*/
static const uint8_t des_test_full[] =
    "Once upon a midnight dreary, while I pondered weak and weary,"
    "Over many a quaint and curious volume of forgotten lore,"
    "While I nodded, nearly napping, suddenly there came a tapping,"
    "As of some one gently rapping, rapping at my chamber door"
    "Its some visitor, I muttered, tapping at my chamber door"
    "Only this, and nothing more.";

static uint8_t output[OUTPUT_ARRAY_LEN];
static uint8_t cipher[OUTPUT_ARRAY_LEN];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void ltc_print_msg(const uint8_t *data, uint32_t length);
static void ltc_example_task(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Function ltc_print_msg prints data into console.
 */
static void ltc_print_msg(const uint8_t *data, uint32_t length)
{
    uint32_t i;

    PRINTF("          ");
    for (i = 0; i < length; i++)
    {
        PUTCHAR(data[i]);
        if (data[i] == ',')
        {
            PRINTF("\r\n          ");
        }
    }
    PRINTF("\r\n");
}

/*!
 * @brief Function ltc_example_task demonstrates use of LTC driver
 * encryption/decryption functions on short sample text.
 */
static void ltc_example_task(void)
{
    unsigned int length;
    status_t status;

    /*8 bytes key: "password"*/
    uint8_t key[DES_KEY_LENGTH] = {0x70, 0x61, 0x73, 0x73, 0x77, 0x6F, 0x72, 0x64};
    /*initialization vector: 8 bytes: "mysecret"*/
    uint8_t ive[DES_BLOCK_LENGTH] = {0x6d, 0x79, 0x73, 0x65, 0x63, 0x72, 0x65, 0x74};
    /* DES3 keys */
    /*24 bytes key: "verynicepassword12345678"*/
    uint8_t key1[DES_KEY_LENGTH] = {0x76, 0x65, 0x72, 0x79, 0x6e, 0x69, 0x63, 0x65};
    uint8_t key2[DES_KEY_LENGTH] = {0x70, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64};
    uint8_t key3[DES_KEY_LENGTH] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};

    /* Initialize LTC driver.
     * This enables clocking and resets the module to a known state. */
    LTC_Init(base);

    /* Print welcome string */
    PRINTF("............................. LTC (DES)  DRIVER  EXAMPLE ............................. \r\n\r\n");

    /******* FIRST PART USING DES-CBC method ********/
    memset(&output[0], 0, OUTPUT_ARRAY_LEN);
    memset(&cipher[0], 0, OUTPUT_ARRAY_LEN);
    length = sizeof(des_test_full) - 1u;

    PRINTF("Testing input string: \r\n");
    ltc_print_msg(&des_test_full[0], length);
    /* Format console output */
    PRINTF("\r\n");

    /******* FIRST PART USING DES-CBC method ********/
    PRINTF("----------------------------------- DES-CBC method --------------------------------------\r\n");
    /*   ENCRYPTION   */
    PRINTF("DES CBC Encryption of %u bytes.\r\n", length);

    /* Call DES_cbc encryption */
    status = LTC_DES_EncryptCbc(base, des_test_full, cipher, length, ive, key);
    if (status != kStatus_Success)
    {
        PRINTF("DES CBC encryption failed !\r\n");
        return;
    }

    /* Result message */
    PRINTF("DES CBC encryption finished.\r\n\r\n");

    /*   DECRYPTION   */
    PRINTF("DES CBC Decryption of %u bytes.\r\n", length);

    /* Call DES_cbc decryption */
    status = LTC_DES_DecryptCbc(base, cipher, output, length, ive, key);
    if (status != kStatus_Success)
    {
        PRINTF("DES CBC decryption failed !\r\n");
        return;
    }

    /* Result message */
    PRINTF("DES CBC decryption finished.\r\n");
    /* Print decrypted string */
    PRINTF("Decrypted string :\r\n");
    ltc_print_msg(output, length);
    /* Format console output */
    PRINTF("\r\n");

    /******* SECOND PART USING DES3-CBC method ********/
    PRINTF("----------------------------------- DES3-CBC method --------------------------------------\r\n");
    length = sizeof(des_test_full) - 1u;
    /*   ENCRYPTION   */
    PRINTF("DES3 CBC Encryption of %u bytes.\r\n", length);

    /* Call DES3_cbc encryption */
    status = LTC_DES3_EncryptCbc(base, des_test_full, cipher, length, ive, key1, key2, key3);
    if (status != kStatus_Success)
    {
        PRINTF("DES3 CBC encryption failed !\r\n");
        return;
    }

    /* Result message */
    PRINTF("DES3 CBC encryption finished.\r\n\r\n");

    /*   DECRYPTION   */
    PRINTF("DES3 CBC decryption of %u bytes.\r\n", length);

    /* Call DES3_cbc decryption */
    status = LTC_DES3_DecryptCbc(base, cipher, output, length, ive, key1, key2, key3);
    if (status != kStatus_Success)
    {
        PRINTF("DES3 CBC decryption failed !\r\n");
        return;
    }

    /* Result message */
    PRINTF("DES3 CBC decryption finished.\r\n");
    /* Print decrypted string */
    PRINTF("Decrypted string :\r\n");
    ltc_print_msg(output, length);
    PRINTF("\r\n");

    /* Goodbye message */
    PRINTF(".............. THE  END  OF  THE  LTC (DES)  DRIVER  EXAMPLE ................................");
}

int main(void)
{
    /* Init hardware*/
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Call example task */
    ltc_example_task();

    while (1)
    {
    }
}
