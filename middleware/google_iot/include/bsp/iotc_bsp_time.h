/* Copyright 2018-2019 Google LLC
 *
 * This is part of the Google Cloud IoT Device SDK for Embedded C.
 * It is licensed under the BSD 3-Clause license; you may not use this file
 * except in compliance with the License.
 *
 * You may obtain a copy of the License at:
 *  https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __IOTC_BSP_TIME_H__
#define __IOTC_BSP_TIME_H__

/**
 * @file iotc_bsp_time.h
 * @brief Implement time functions.
 *
 * The client application uses these functions to keep track of time. Time
 * functions implementations muyst determine the exact execution time of
 * scheduled tasks and the delayed reconnection logic.
 *
 * Use hardware or NTP servers to maintain an accurate clock.
 */

#include <iotc_time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @function
 * @brief Initialize the timekeeping system.
 *
 * On POSIX systems, this function is empty because POSIX systems already
 * include initialized timekeeping systems. On non-POSIX systems,
 * implementations of this function can connect to an NTP service or
 * initialize time-step functionality.
 */
void iotc_bsp_time_init( void );

/**
 * @function
 * @brief Return the seconds since Epoch.
 */
iotc_time_t iotc_bsp_time_getcurrenttime_seconds( void );

/**
 * @function
 * @brief Return the milliseconds since Epoch.
 */
iotc_time_t iotc_bsp_time_getcurrenttime_milliseconds( void );

/**
 * @function
 * @brief Return monotonically increasing time in milliseconds.
 */
iotc_time_t iotc_bsp_time_getmonotonictime_milliseconds( void );

#ifdef __cplusplus
}
#endif

#endif /* __IOTC_BSP_TIME_H__ */
