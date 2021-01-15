/*
 * Copyright (c) 2015-2018 ARM Limited. All rights reserved.
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

#ifndef APP_PLATFORM_SETUP_H
#define APP_PLATFORM_SETUP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Erases client credentials and SOTP storage, will also reformat
// the external storage for Mbed OS if initial erase fail.
int mcc_platform_reset_storage(void);

// Initialize common details for fcc.
int mcc_platform_fcc_init(void);

// For developer-mode only, (re)initializes the RoT and for non-TRNG boards
// also the entropy.
int mcc_platform_sotp_init(void);

// Reverse the resource allocations done by mcc_platform_fcc_init().
void mcc_platform_fcc_finalize(void);

#ifdef __cplusplus
}
#endif

#endif // #ifndef APP_PLATFORM_SETUP_H
