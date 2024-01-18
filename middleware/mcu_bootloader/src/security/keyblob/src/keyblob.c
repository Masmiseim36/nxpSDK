/*
 * Copyright 2017 NXP
 *
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdbool.h>
#include <stdint.h>

#include "bootloader/bootloader.h"
#include "bootloader_common.h"
#include "keyblob.h"

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

status_t keyblob_update(keyblob_info_t *key_info)
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
            uint32_t dek_size = key_info->option.B.dek_size;
            switch (dek_size)
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
                case kDekSize_edgelockDUK:
                    dek_size = BL_FEATURE_KEYBLOB_OTFAD_KEY_DATA_SIZE * BL_FEATURE_KEYBLOB_OTFAD_KEY_NUM * 8;
                    break;
#ifdef MIMXRT1189_cm33_SERIES
                case kDekSize_edgelockIEE:
                    dek_size = BL_FEATURE_KEYBLOB_IEE_KEY_DATA_SIZE * 8;
                    break;
#endif
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

            // For current implementation, blob key size is fixed to use the max aes key. So this argument is not used
            // currently, and reserved for extension in the future.
            s_keyblob_context.bk_size = kKeyBlob_BK_MaxSize;

            memcpy(s_keyblob_context.dek, (uint32_t *)key_info->dek_addr, dek_size);
            s_keyblob_context.dek_size = dek_size;

            s_keyblob_context.bk_sel = key_info->option.B.bk_sel;

            status = kStatus_Success;
        }
    } while (0);

    return status;
}

status_t keyblob_get(uint8_t **keyblob_start, uint32_t *keyblob_size)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if ((keyblob_start == NULL) || (keyblob_size == NULL))
        {
            break;
        }

        memset(&s_keyblob_context.keyblob, 0, sizeof(s_keyblob_context.keyblob));
        status = generate_key_blob((const uint8_t *)&s_keyblob_context.dek[0], s_keyblob_context.dek_size,
                                   s_keyblob_context.bk_sel, (uint8_t *)&s_keyblob_context.keyblob[0], keyblob_size);
        if (status != kStatus_Success)
        {
            break;
        }

        *keyblob_start = (uint8_t *)&s_keyblob_context.keyblob[0];

    } while (0);

    return status;
}
