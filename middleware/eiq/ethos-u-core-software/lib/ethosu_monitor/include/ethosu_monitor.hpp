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

#ifndef ETHOSU_MONITOR_H
#define ETHOSU_MONITOR_H


#include <algorithm>
#include <ethosu_driver.h>
#include <pmu_ethosu.h>
#include <stdint.h>
#include <vector>

typedef struct {
    uint64_t cycleCount;
    uint32_t qread;
    uint32_t status;
    struct {
        uint32_t eventConfig;
        uint32_t eventCount;
    } pmu[ETHOSU_PMU_NCOUNTERS];
} EthosuQreadEvent;

class EthosUMonitor {

public:

    EthosUMonitor(uint32_t pmuCycleCounterEnable);
    EthosUMonitor() : pmuCycleCounterEnable(0), numEvents(0){};

    template <typename T>
    void configure(ethosu_driver *drv, const T &eventIds) {
        // Set event ids
        numEvents = std::min(static_cast<size_t>(ETHOSU_PMU_NCOUNTERS), eventIds.size());
        for (size_t i = 0; i < numEvents; i++) {
            ethosuEventIds[i] = static_cast<ethosu_pmu_event_type>(eventIds[i]);
        }

        // config pmu and set driver
        ETHOSU_PMU_Enable(drv);

        for (size_t i = 0; i < numEvents; i++) {
            ETHOSU_PMU_Set_EVTYPER(drv, i, ethosuEventIds[i]);
            ETHOSU_PMU_CNTR_Enable(drv, 1 << i);
        }

        if (pmuCycleCounterEnable != 0) {
            ETHOSU_PMU_CNTR_Enable(drv, ETHOSU_PMU_CCNT_Msk);
            ETHOSU_PMU_CYCCNT_Reset(drv);
        }

        ETHOSU_PMU_EVCNTR_ALL_Reset(drv);
    }

    void release(ethosu_driver *drv);

    void monitorSample(ethosu_driver *drv);
    void monitorSample(ethosu_driver *drv, EthosuQreadEvent* event);

    ethosu_pmu_event_type ethosuEventIds[ETHOSU_PMU_NCOUNTERS];
    uint32_t eventCount[ETHOSU_PMU_NCOUNTERS];
    uint32_t pmuCycleCounterEnable;
    uint64_t pmuCycleCounterCount;
    size_t numEvents;
};

#endif
