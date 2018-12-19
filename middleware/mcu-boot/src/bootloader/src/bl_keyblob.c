/*
 * Copyright (c) 2012-2015, Freescale Semiconductor, Inc.
 * Copyright (c) 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "bootloader_common.h"
#include <stdint.h>
#include <stdbool.h>
#include "fsl_common.h"
#include "bootloader/bl_keyblob.h"
#include "bootloader/bootloader.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static keyblob_context_t s_keyblob_context;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int32_t keyblob_update(keyblob_info_t *key_info)
{
    status_t status = kStatus_InvalidArgument;
    do
    {
        if (key_info == NULL)
        {
            break;
        }
        if (key_info->option.B.tag != kKeyBlobInfoOption_Tag)
        {
            break;
        }

        int32_t keyblob_info_type = key_info->option.B.type;
        if ((keyblob_info_type != kKeyBlobInfoType_Program) && (keyblob_info_type != kKeyBlobInfoType_Update))
        {
            break;
        }

        if (keyblob_info_type == kKeyBlobInfoType_Update)
        {
            uint32_t dek_size = 0;
            uint32_t dek_size_option = key_info->option.B.dek_size;
            switch (dek_size_option)
            {
                case kDekSize_128bits:
                    dek_size = 128;
                    break;
                case kDekSize_192bits:
                    dek_size = 192;
                    break;
                case kDekSize_256bits:
                    dek_size = 256;
                    break;
                default:
                    break;
            }
            if (dek_size < 128)
            {
                break;
            }
            dek_size /= 8; // Calculate key bytes
            if ((!is_valid_application_location(key_info->dek_addr)) ||
                (!is_valid_application_location(key_info->dek_addr + dek_size)))
            {
                break;
            }

            uint32_t bk_size = BL_FEATURE_KEYBLOB_BK_SIZE;
            s_keyblob_context.bk_size = bk_size;

            memcpy(s_keyblob_context.dek, (uint32_t *)key_info->dek_addr, dek_size);
            s_keyblob_context.dek_size = dek_size;
            // header size = 8, mac size = 16
            s_keyblob_context.keyblob_size = kKeyBlobMaxSize;

            status = kStatus_Success;
        }
    } while (0);

    return status;
}

int32_t keyblob_get(uint8_t **keyblob_start, uint32_t *keyblob_size)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if ((keyblob_start == NULL) || (keyblob_size == NULL))
        {
            break;
        }

        memset(&s_keyblob_context.keyblob, 0, sizeof(s_keyblob_context.keyblob));
        status = generate_key_blob(s_keyblob_context.dek, (uint8_t *)&s_keyblob_context.keyblob);
        if (status != kStatus_Success)
        {
            break;
        }

        *keyblob_start = (uint8_t *)&s_keyblob_context.keyblob[0];
        *keyblob_size = s_keyblob_context.keyblob_size;

    } while (0);

    return status;
}
