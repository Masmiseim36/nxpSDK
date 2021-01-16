/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
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

#ifndef M2M_TIMER_PIMPL_H__
#define M2M_TIMER_PIMPL_H__

#include "ns_types.h"
#include "eventOS_event.h"
#include "mbed-client/m2mtimerobserver.h"

class M2MTimerPimpl {
private:

    // Prevents the use of assignment operator
    M2MTimerPimpl& operator=(const M2MTimerPimpl& other);

    // Prevents the use of copy constructor
    M2MTimerPimpl(const M2MTimerPimpl& other);
public:

    /**
     * Constructor.
     */
    M2MTimerPimpl(M2MTimerObserver& _observer);

    /**
     * Destructor.
     */
    virtual ~M2MTimerPimpl();

    /**
     * Starts timer
     * @param interval Timer's interval in milliseconds
     * @param single_shot defines if timer is ticked
     * once or is it restarted everytime timer is expired.
     */
    void start_timer(uint64_t interval, M2MTimerObserver::Type type, bool single_shot = true);

    /**
     * @brief Starts timer in DTLS manner
     * @param intermediate_interval Intermediate interval to use, must be smaller than tiotal (usually 1/4 of total)
     * @param total_interval Total interval to use; This is the timeout value of a DTLS packet
     * @param type Type of the timer
     */
    void start_dtls_timer(uint64_t intermediate_interval, uint64_t total_interval, M2MTimerObserver::Type type);

    /**
     * Stops timer.
     * This cancels the ongoing timer.
     */
    void stop_timer();

    /**
     * @brief Checks if the intermediate interval has passed
     * @return true if interval has passed, false otherwise
     */
    bool is_intermediate_interval_passed() const;

    /**
     * @brief Checks if the total interval has passed
     * @return true if interval has passed, false otherwise
     */
    bool is_total_interval_passed() const;

    /**
     * @brief Get still left time
     * @return Time left in milliseconds
     */
    uint64_t get_still_left_time() const;

    /**
     * Tasklet's internal event handler, which needs to be public as it is used from C wrapper side.
     * This makes it possible to at least keep the member variables private.
     */
    void handle_timer_event(const arm_event_s &event);

private:

    /**
     * @brief Start long period timer
     */
    void start_still_left_timer();

    /**
     * Function handling the timer completion.
     */
    void timer_expired();

    void start();
    void cancel();

    /**
     * Internal helper to request a event after given amount of milliseconds.
     * @param delay_ms requested delay in milliseconds
     */
    void request_event_in(int32_t delay_ms);

private:
    M2MTimerObserver&   _observer;
    uint64_t            _interval;

    uint64_t            _intermediate_interval;
    uint64_t            _total_interval;
    uint64_t            _still_left;

    // pointer to the current timer event pending, NULL if none is in flight
    arm_event_storage_t *_timer_event;

    M2MTimerObserver::Type  _type : 4;

    unsigned int        _status : 2;

    bool                _dtls_type : 1;

    bool                _single_shot : 1;

    static int8_t       _tasklet_id;

    friend class M2MTimer;
    friend class Test_M2MTimerPimpl_classic;
};

#endif //M2M_TIMER_PIMPL_H__

