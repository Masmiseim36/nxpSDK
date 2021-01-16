// ----------------------------------------------------------------------------
// Copyright 2016-2017 ARM Ltd.
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

#include "platform/arm_hal_interrupt.h"

#include "arm_hal_interrupt_private.h"
#include "pal.h"

#include <assert.h>


static uint8_t sys_irq_disable_counter;

static palMutexID_t critical_mutex_id;

void platform_critical_init(void)
{
    palStatus_t status;
    status = pal_osMutexCreate(&critical_mutex_id);
    assert(PAL_SUCCESS == status);
    (void) status;
}

void platform_enter_critical(void)
{
    palStatus_t status;
    status = pal_osMutexWait(critical_mutex_id, UINT32_MAX);
    assert(PAL_SUCCESS == status);
    (void) status;
    sys_irq_disable_counter++;
}

void platform_exit_critical(void)
{
    palStatus_t status;
    --sys_irq_disable_counter;
    status = pal_osMutexRelease(critical_mutex_id);
    assert(PAL_SUCCESS == status);
    (void) status;
}
