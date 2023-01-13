/**
 * Copyright 2022 NXP.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NEUTRON_FIRMWARE_H
#define NEUTRON_FIRMWARE_H

int neutronFwInit();

/* neutronFwInterpreter - runs microcode loaded into the current config. */
int neutronFwInterpreter(const void *microcode, void **baseAddress, uint16_t baseAddressNo);

#endif // NEUTRON_FIRMWARE_H
