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

#ifndef __BLINKY_H__
#define __BLINKY_H__

#include "sal-stack-nanostack-eventloop/nanostack-event-loop/eventOS_event.h"

class SimpleM2MClient;
class M2MResource;

#include <stdint.h>

class Blinky
{
    typedef enum {
        STATE_IDLE,
        STATE_STARTED,
    } BlinkyState;

public:
    Blinky();

    ~Blinky();

    void init(SimpleM2MClient &client, M2MResource *resource);

    bool start(const char* pattern, size_t length, bool pattern_restart);

    void stop();

public:
    // This needs
    void event_handler(const arm_event_s &event);

    void request_next_loop_event();
    void request_automatic_increment_event();

private:
    void create_tasklet();
    void handle_pattern_event();
    bool request_timed_event(uint8_t event_type, arm_library_event_priority_e priority, int32_t delay);

    void handle_buttons();
    void handle_automatic_increment();

private:
    int get_next_int();
    bool run_step();

private:

    char *_pattern;
    const char *_curr_pattern;

    SimpleM2MClient *_client;

    M2MResource     *_button_resource;

    int              _button_count;

    BlinkyState _state;

    bool _restart;

    static int8_t _tasklet;

};
#endif /* __BLINKY_H__ */
