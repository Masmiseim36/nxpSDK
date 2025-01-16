/*
 * Copyright 2022-2024 NXP.
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

#include "mpp_api.h"
#include "mpp_api_types_internal.h"

int mpp_nullsink_add(mpp_t mpp)
{
    int ret = MPP_SUCCESS;
    if (!mpp)
        return MPP_INVALID_PARAM;
    _mpp_t *_mpp = (_mpp_t *)mpp;
    if (_mpp->status != MPP_OPENED)
        return MPP_ERROR;

    _elem_t *elem;
    mpp_create_elem(_mpp, &elem);

    elem->io.in_buf[0] = elem->prev->io.out_buf[0];
    elem->io.inplace = false;
    elem->io.mem_policy = HAL_MEM_ALLOC_NONE;
    elem->io.nb_in_buf = 1;
    elem->io.nb_out_buf = 0;

    /* store sink info */
    elem->type = MPP_TYPE_SINK;
    elem->sink_typ = MPP_SINK_NULL;

    /* pipeline has been closed */
    _mpp->status = MPP_CLOSED;

    return ret;
}
