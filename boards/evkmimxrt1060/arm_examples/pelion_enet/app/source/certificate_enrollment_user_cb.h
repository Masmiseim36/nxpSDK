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

///////////
// INCLUDES
///////////

#ifndef CERTIFICATE_ENROLLMENT_USER_CB_H
#define CERTIFICATE_ENROLLMENT_USER_CB_H

#include "ce_status.h"
#include "ce_defs.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void certificate_renewal_cb(const char *cert_name, ce_status_e status, ce_initiator_e initiator);

#ifdef __cplusplus
}
#endif
#endif // CERTIFICATE_ENROLLMENT_USER_CB_H

