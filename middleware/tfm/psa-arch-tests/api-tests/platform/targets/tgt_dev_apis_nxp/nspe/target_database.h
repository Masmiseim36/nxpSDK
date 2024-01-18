/** @file
 * Copyright (c) 2020, Arm Limited or its affiliates. All rights reserved.
 * Copyright 2020 NXP
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include "val_target.h"
#include "cmsis_compiler.h"

typedef __PACKED_STRUCT target_database{
    target_cfg_hdr_t target_cfg_hdr;            /* Target Configuration Header */
    soc_peripheral_desc_t   uart;               /* Uart */
    soc_peripheral_desc_t   wd_timer;           /* WD timer */
    memory_desc_t           memory;             /* Memory region */
    cfg_type_t              type_end;           /* End */
} target_database_t;

extern target_database_t target_database;