// ----------------------------------------------------------------------------
// Copyright 2015-2019 ARM Ltd.
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------------------------------------------------------

///////////
// INCLUDES
///////////

#ifndef MCC_COMMON_BUTTON_AND_LED_H
#define MCC_COMMON_BUTTON_AND_LED_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Toggle led (if available)
void mcc_platform_toggle_led(void);

// Put led off (if available)
void mcc_platform_led_off(void);

// Check if button has been pressed (if available)
uint8_t mcc_platform_button_clicked(void);

uint8_t mcc_platform_init_button_and_led(void);

#ifdef __cplusplus
}
#endif
#endif // MCC_COMMON_BUTTON_AND_LED_H
