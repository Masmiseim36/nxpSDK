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

#ifndef _MPP_HEAP_H
#define _MPP_HEAP_H

#include "mpp_api_types.h"
#include "mpp_api_types_internal.h"

void mpp_heap_init(_mpp_t *prio_lst[]);
int mpp_heap_insert(_mpp_t *mpp, _mpp_t *prio_lst[]);
void mpp_heap_move(_mpp_t *mpp , _mpp_t *rio_lst[], unsigned int dst_prio);
void mpp_dump_heap(_mpp_t *prio_lst[]);
int mpp_memory_manage_heap(_mpp_t *prio_lst[]);
int mpp_memory_check_list(_mpp_t *prio_lst[]);

#endif
