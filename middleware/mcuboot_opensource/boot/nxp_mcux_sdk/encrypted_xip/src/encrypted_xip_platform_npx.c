/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sblconfig.h"
#if defined(ENCRYPTED_XIP_NPX) && defined(CONFIG_ENCRYPT_XIP_EXT_ENABLE)
#include <ctype.h>
#include "fsl_debug_console.h"
#include "flash_map.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*
 * @brief Helper to assert function return status.
 *
 * @details Print finshed message and trap forever upon failure.
 */
#define ASSERT_NPX(expected, actual, ...) \
    do                                    \
    {                                     \
        if (expected != actual)           \
        {                                 \
            PRINTF("Assertion failed: ");   \
            PRINTF(__VA_ARGS__);          \
            while (1)                     \
                ;                         \
        }                                 \
    } while (0);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Static
 ******************************************************************************/

/*******************************************************************************
 * Externs
 ******************************************************************************/
status_t platform_enc_init(void)
{
    /* Nothing to do here */
    return kStatus_Success;
}

size_t platform_enc_cfg_getSize(void)
{
    /* Nothing to do here */
    return 0;
}

status_t platform_enc_cfg_write(struct flash_area *fa_meta, uint32_t region_start, uint32_t img_sz)
{
    /* Nothing to do here */
    return kStatus_Success;
}

status_t platform_enc_cfg_initEncryption(struct flash_area *fa_meta)
{
    /* Nothing to do here */
    return kStatus_Success;
}

bool platform_enc_cfg_isPresent(uint32_t addr)
{
    /* Nothing to do here */
    return kStatus_Success;
}

status_t platform_enc_cfg_getNonce(struct flash_area *fa_meta, uint8_t *nonce)
{
    /* Nonce is not needed in this case as NPX encryption is done on-the-fly */
    return kStatus_Success;
}

status_t platform_enc_finish(void)
{
    /* Nothing to do here */
    return kStatus_Success;
}

status_t platform_enc_encrypt_data(uint32_t flash_addr, uint8_t *nonce,
                                   uint8_t *input, uint8_t *output, uint32_t len)
{
    /* Nothing to do here */
    return kStatus_Success;
}

status_t platform_enc_flash_write(const struct flash_area *area, uint32_t off, const void *src, uint32_t len)
{
    /* Nothing to do here */
    return kStatus_Success;
}

#endif /* ENCRYPTED_XIP_NPX && CONFIG_ENCRYPT_XIP_EXT_ENABLE */
