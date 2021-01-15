// ----------------------------------------------------------------------------
// Copyright 2016-2019 ARM Ltd.
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


#ifndef APPLICATION_INIT_H
#define APPLICATION_INIT_H

#ifndef STARTUP_MAX_RANDOM_DELAY
#define STARTUP_MAX_RANDOM_DELAY 0
#endif

#ifndef STARTUP_MIN_RANDOM_DELAY
#define STARTUP_MIN_RANDOM_DELAY STARTUP_MAX_RANDOM_DELAY/4
#endif

/*
 * Initializes tracing library.
 */

bool application_init_mbed_trace(void);

/*
 * application_init() runs the following initializations:
 *  1. platform initialization
 *  2. print memory statistics if MBED_HEAP_STATS_ENABLED is defined
 *  3. FCC initialization.
 */

bool application_init(void);

/*
 * Prints the FCC status and corresponding error description, if any.
 */
void print_fcc_status(int fcc_status);

/*
 * Wait for random delay with maximum defined using STARTUP_MAX_RANDOM_DELAY.
 * Lower limit can be defined via STARTUP_MIN_RANDOM_DELAY. The lower limit defaults to
 * STARTUP_MAX_RANDOM_DELAY/4.
 * The delay is needed for devices connected to a large network with high latency
 * and constrained bandwidth.
 * For example in Wi-SUN, this stabilizes the early network formation when all
 * clients do not register simultaneously.
 */
void wait_application_startup_delay();


#endif //APPLICATION_INIT_H

