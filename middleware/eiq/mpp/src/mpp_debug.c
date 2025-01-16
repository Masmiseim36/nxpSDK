/*
 * Copyright 2020-2024 NXP.
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "hal_os.h"
#include <sys/time.h>
#include "mpp_api_types.h"

int tick_check_rate(uint32_t *last, int *curr, int max)
{

    /*
     * Reset the last time and counter if this is the first call
     * or more than a second has passed since the last update of
     * lasttime.
     */

    uint32_t now = hal_get_ostick();
    if (!*last || (unsigned int)(now - *last) >= hal_get_tick_rate_hz()) {
        *last= now;
        *curr = 1;
        return (max != 0);
    } else {
        (*curr)++;            /* NB: ignore overflow */
        return (max < 0 || *curr < max);
    }
}

/* get string from element id */
char * elem_name(mpp_element_id_t id)
{
    char *str = NULL;
    switch(id)
    {
    case MPP_ELEMENT_COMPOSE:
        str = "COMPOSE";
        break;
    case MPP_ELEMENT_CONVERT:
        str = "CONVERT";
        break;
    case MPP_ELEMENT_INFERENCE:
        str = "INFERENCE";
        break;
    case MPP_ELEMENT_INVALID:
        str = "INVALID";
        break;
    case MPP_ELEMENT_LABELED_RECTANGLE:
        str = "LABELED_RECTANGLE";
        break;
    case MPP_ELEMENT_TEST:
        str = "TEST";
        break;
    case MPP_ELEMENT_NUM:
    default:
        str = "***BAD ELEMENT!***";
        break;
    }
    return str;
}

