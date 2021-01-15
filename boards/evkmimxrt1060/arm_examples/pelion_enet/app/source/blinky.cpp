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

#include "blinky.h"

#include "sal-stack-nanostack-eventloop/nanostack-event-loop/eventOS_event.h"
#include "sal-stack-nanostack-eventloop/nanostack-event-loop/eventOS_event_timer.h"
#include "ns-hal-pal/ns_hal_init.h"
#include "mbed-trace/mbed_trace.h"

#include "mcc_common_button_and_led.h"
#include "simplem2mclient.h"
#include "m2mresource.h"

#include <assert.h>
#include <string.h>

#define TRACE_GROUP "blky"

#define BLINKY_TASKLET_LOOP_INIT_EVENT 0
#define BLINKY_TASKLET_PATTERN_INIT_EVENT 1
#define BLINKY_TASKLET_PATTERN_TIMER 2
#define BLINKY_TASKLET_LOOP_TIMER 3
#define BLINKY_TASKLET_AUTOMATIC_INCREMENT_TIMER 4

#define BUTTON_POLL_INTERVAL_MS 100
#define AUTOMATIC_INCREMENT_INTERVAL_MS 5000

int8_t Blinky::_tasklet = -1;

extern "C" {

static void blinky_event_handler_wrapper(arm_event_s *event)
{
    assert(event);
    if (event->event_type != BLINKY_TASKLET_LOOP_INIT_EVENT) {
        // the init event will not contain instance pointer
        Blinky *instance = (Blinky *)event->data_ptr;
        if(instance) {
            instance->event_handler(*event);
        }
    }
}

}

Blinky::Blinky()
: _pattern(NULL),
  _curr_pattern(NULL),
  _client(NULL),
  _button_resource(NULL),
  _state(STATE_IDLE),
  _restart(false)
{
    _button_count = 0;
}

Blinky::~Blinky()
{
    // stop will free the pattern buffer if any is allocated
    stop();
}

void Blinky::create_tasklet()
{
    if (_tasklet < 0) {
        _tasklet = eventOS_event_handler_create(blinky_event_handler_wrapper, BLINKY_TASKLET_LOOP_INIT_EVENT);
        assert(_tasklet >= 0);
    }
}

// use references to encourage caller to pass this existing object
void Blinky::init(SimpleM2MClient &client, M2MResource *resource)
{
    // Do not start if resource has not been allocated.
    if (!resource) {
        return;
    }

    _client = &client;
    _button_resource = resource;

    // create the tasklet, if not done already
    create_tasklet();
}

bool Blinky::start(const char* pattern, size_t length, bool pattern_restart)
{
    assert(pattern);

    // create the tasklet, if not done already
    create_tasklet();

    _restart = pattern_restart;

    // allow one to start multiple times before previous sequence has completed
    stop();

    _pattern = (char*)malloc(length+1);
    if (_pattern == NULL) {
        return false;
    }

    memcpy(_pattern, pattern, length);
    _pattern[length] = '\0';

    _curr_pattern = _pattern;

    return run_step();
}

void Blinky::stop()
{
    free(_pattern);
    _pattern = NULL;
    _curr_pattern = NULL;
    _state = STATE_IDLE;
}

int Blinky::get_next_int()
{
    int result = -1;

    char *endptr;

    int conv_result  = strtol(_curr_pattern, &endptr, 10);

    if (*_curr_pattern != '\0') {
        // ints are separated with ':', which we will skip on next time
        if (*endptr == ':') {
            endptr += 1;
            result = conv_result;
        } else if (*endptr == '\0') { // end of
            result = conv_result;
        } else {
            tr_debug("invalid char %c", *endptr);
        }
    }

    _curr_pattern = endptr;

    return result;
}

bool Blinky::run_step()
{
    int32_t delay = get_next_int();

    // tr_debug("patt: %s, curr: %s, delay: %d", _pattern, _curr_pattern, delay);

    if (delay < 0) {
        _state = STATE_IDLE;
        return false;
    }

    if (request_timed_event(BLINKY_TASKLET_PATTERN_TIMER, ARM_LIB_MED_PRIORITY_EVENT, delay) == false) {
        _state = STATE_IDLE;
        assert(false);
        return false;
    }

    mcc_platform_toggle_led();

    _state = STATE_STARTED;

    return true;
}

void Blinky::event_handler(const arm_event_s &event)
{
    switch (event.event_type) {
        case BLINKY_TASKLET_PATTERN_TIMER:
            handle_pattern_event();
            break;
        case BLINKY_TASKLET_LOOP_TIMER:
            handle_buttons();
            break;
        case BLINKY_TASKLET_AUTOMATIC_INCREMENT_TIMER:
            handle_automatic_increment();
            break;
        case BLINKY_TASKLET_PATTERN_INIT_EVENT:
        default:
            break;
    }
}

void Blinky::handle_pattern_event()
{
    bool success = run_step();

    if ((!success) && (_restart)) {
        // tr_debug("Blinky restart pattern");
        _curr_pattern = _pattern;
        run_step();
    }
}

void Blinky::request_next_loop_event()
{
    request_timed_event(BLINKY_TASKLET_LOOP_TIMER, ARM_LIB_LOW_PRIORITY_EVENT, BUTTON_POLL_INTERVAL_MS);
}

void Blinky::request_automatic_increment_event()
{
    request_timed_event(BLINKY_TASKLET_AUTOMATIC_INCREMENT_TIMER, ARM_LIB_LOW_PRIORITY_EVENT, AUTOMATIC_INCREMENT_INTERVAL_MS);
}

// helper for requesting a event by given type after given delay (ms)
bool Blinky::request_timed_event(uint8_t event_type, arm_library_event_priority_e priority, int32_t delay)
{
    assert(_tasklet >= 0);

    arm_event_t event = { 0 };

    event.event_type = event_type;
    event.receiver = _tasklet;
    event.sender =  _tasklet;
    event.data_ptr = this;
    event.priority = priority;

    const int32_t delay_ticks = eventOS_event_timer_ms_to_ticks(delay);

    if (eventOS_event_send_after(&event, delay_ticks) == NULL) {
        return false;
    } else {
        return true;
    }
}

void Blinky::handle_buttons()
{
    assert(_client);
    assert(_button_resource);

    // this might be stopped now, but the loop should then be restarted after re-registration
    request_next_loop_event();

    if (_client->is_client_registered()) {
        if (mcc_platform_button_clicked()) {
            _button_count = _button_resource->get_value_int() + 1;
            _button_resource->set_value(_button_count);
            printf("Button resource manually updated. Value %d\r\n", _button_count);
        }
    }
}

void Blinky::handle_automatic_increment()
{
    assert(_client);
    assert(_button_resource);

    // this might be stopped now, but the loop should then be restarted after re-registration
    request_automatic_increment_event();

    if (_client->is_client_registered()) {
        _button_count = _button_resource->get_value_int() + 1;
        _button_resource->set_value(_button_count);
        printf("Button resource automatically updated. Value %d\r\n", _button_count);
    }
}
