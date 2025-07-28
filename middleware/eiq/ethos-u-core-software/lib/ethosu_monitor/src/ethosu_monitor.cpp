/*
 * Copyright (c) 2021 Arm Limited. All rights reserved.
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

#include "ethosu_monitor.hpp"
#include "ethosu_log.h"
#include <inttypes.h>
#include <stdio.h>

EthosUMonitor::EthosUMonitor(uint32_t _pmuCycleCounterEnable) :
    pmuCycleCounterEnable(_pmuCycleCounterEnable), numEvents(0) {}

void EthosUMonitor::monitorSample(ethosu_driver *drv) {
    // Fetch events
    for (size_t i = 0; i < numEvents; i++) {
        eventCount[i] = ETHOSU_PMU_Get_EVCNTR(drv, i);
    }

    if(pmuCycleCounterEnable != 0) {
        pmuCycleCounterCount = ETHOSU_PMU_Get_CCNTR(drv);
    }
}

void EthosUMonitor::monitorSample(ethosu_driver *drv, EthosuQreadEvent *event) {
    if (event == nullptr)
        return;

    // Fetch events
    for (size_t i = 0; i < numEvents; i++) {
        event->pmu[i].eventConfig = ethosuEventIds[i];
        event->pmu[i].eventCount = ETHOSU_PMU_Get_EVCNTR(drv, i);
    }

    event->cycleCount = ETHOSU_PMU_Get_CCNTR(drv);
    event->qread = ETHOSU_PMU_Get_QREAD(drv);
    event->status = ETHOSU_PMU_Get_STATUS(drv);
}

void EthosUMonitor::release(ethosu_driver *drv) {
    ETHOSU_PMU_Disable(drv);
}
