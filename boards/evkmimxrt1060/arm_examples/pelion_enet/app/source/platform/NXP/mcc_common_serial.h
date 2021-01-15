// ----------------------------------------------------------------------------
// Copyright 2018-2020 ARM Ltd.
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

#ifndef MCC_COMMON_SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*uartParser)(int16_t);    /*! @brief   Prototype Serial parser function */

/*!
 * @brief SerialBytesTest - This function parses the input from the serial RX and display it on the terminal
 * @param uartParser Func - the call back function for parsing the input
 * @return void
 */
void SerialBytesTest(uartParser Func);

#ifdef __cplusplus
}
#endif

#endif // #ifndef MCC_COMMON_SERIAL_H
