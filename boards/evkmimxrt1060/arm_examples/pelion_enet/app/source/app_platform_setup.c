/*
 * Copyright (c) 2015-2019 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef MBED_CLOUD_CLIENT_USER_CONFIG_FILE
#include MBED_CLOUD_CLIENT_USER_CONFIG_FILE
#endif

#include <stdio.h>
#include "app_platform_setup.h"
#include "mcc_common_setup.h"
#include "mcc_common_config.h"
#include "factory_configurator_client.h"
#include "pal.h"

#if MBED_CONF_APP_DEVELOPER_MODE == 1
#ifdef PAL_USER_DEFINED_CONFIGURATION
    #include PAL_USER_DEFINED_CONFIGURATION
#endif
#endif // #if MBED_CONF_APP_DEVELOPER_MODE == 1

// Include this only for Developer mode and device which doesn't have in-built TRNG support
#if MBED_CONF_APP_DEVELOPER_MODE == 1
#ifdef PAL_USER_DEFINED_CONFIGURATION
#define FCC_ROT_SIZE                       16
const uint8_t MBED_CLOUD_DEV_ROT[FCC_ROT_SIZE] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16 };
#if !PAL_USE_HW_TRNG
#define FCC_ENTROPY_SIZE                   48
const uint8_t MBED_CLOUD_DEV_ENTROPY[FCC_ENTROPY_SIZE] = { 0xf6, 0xd6, 0xc0, 0x09, 0x9e, 0x6e, 0xf2, 0x37, 0xdc, 0x29, 0x88, 0xf1, 0x57, 0x32, 0x7d, 0xde, 0xac, 0xb3, 0x99, 0x8c, 0xb9, 0x11, 0x35, 0x18, 0xeb, 0x48, 0x29, 0x03, 0x6a, 0x94, 0x6d, 0xe8, 0x40, 0xc0, 0x28, 0xcc, 0xe4, 0x04, 0xc3, 0x1f, 0x4b, 0xc2, 0xe0, 0x68, 0xa0, 0x93, 0xe6, 0x3a };
#endif // PAL_USE_HW_TRNG = 0
#endif // PAL_USER_DEFINED_CONFIGURATION
#endif // #if MBED_CONF_APP_DEVELOPER_MODE == 1

int mcc_platform_reset_storage(void)
{
#if MBED_CONF_APP_DEVELOPER_MODE == 1
    printf("Resets storage to an empty state.\r\n");
    int status = fcc_storage_delete();
    if (status != FCC_STATUS_SUCCESS) {
        printf("Failed to delete storage - %d\r\n", status);
    }
    return status;
#else
    return FCC_STATUS_SUCCESS;
#endif
}

int mcc_platform_fcc_init(void)
{
#if MBED_CONF_APP_DEVELOPER_MODE == 1
    int status = fcc_init();
    // Ignore pre-existing RoT/Entropy in SOTP
    if (status != FCC_STATUS_SUCCESS && status != FCC_STATUS_ENTROPY_ERROR && status != FCC_STATUS_ROT_ERROR) {
        printf("fcc_init failed with status %d! - exit\r\n", status);
        return status;
    }
    status = mcc_platform_sotp_init();
    if (status != FCC_STATUS_SUCCESS) {
        printf("fcc_init failed with status %d! - exit\r\n", status);
        mcc_platform_fcc_finalize();
    } else {
        // We can return SUCCESS here as preexisting RoT/Entropy is expected flow.
        status = FCC_STATUS_SUCCESS;
    }
    return status;
#else
    return FCC_STATUS_SUCCESS;
#endif
}

int mcc_platform_sotp_init(void)
{
    int status = FCC_STATUS_SUCCESS;
// Include this only for Developer mode and a device which doesn't have in-built TRNG support.
#if MBED_CONF_APP_DEVELOPER_MODE == 1
#ifdef PAL_USER_DEFINED_CONFIGURATION
#if !PAL_USE_HW_TRNG
    status = fcc_entropy_set(MBED_CLOUD_DEV_ENTROPY, FCC_ENTROPY_SIZE);

    if (status != FCC_STATUS_SUCCESS && status != FCC_STATUS_ENTROPY_ERROR) {
        printf("fcc_entropy_set failed with status %d! - exit\r\n", status);
        mcc_platform_fcc_finalize();
        return status;
    }
#endif // PAL_USE_HW_TRNG = 0
/* Include this only for Developer mode. The application will use fixed RoT to simplify user-experience with the application.
 * With this change the application be reflashed/SOTP can be erased safely without invalidating the application credentials.
 */
    status = fcc_rot_set(MBED_CLOUD_DEV_ROT, FCC_ROT_SIZE);

    if (status != FCC_STATUS_SUCCESS && status != FCC_STATUS_ROT_ERROR) {
        printf("fcc_rot_set failed with status %d! - exit\r\n", status);
        mcc_platform_fcc_finalize();
    } else {
        // We can return SUCCESS here as preexisting RoT/Entropy is expected flow.
        printf("Using hardcoded Root of Trust, not suitable for production use.\r\n");
        status = FCC_STATUS_SUCCESS;
    }
#endif // PAL_USER_DEFINED_CONFIGURATION
#endif // #if MBED_CONF_APP_DEVELOPER_MODE == 1
    return status;
}

void mcc_platform_fcc_finalize(void)
{
#if MBED_CONF_APP_DEVELOPER_MODE == 1
    (void)fcc_finalize();
#endif
}
