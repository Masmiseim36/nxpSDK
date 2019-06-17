/*
 * Copyright (C) 2018 Arm Limited or its affiliates. All rights reserved.
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

#include "arm_nnsupportfunctions.h"
#include "arm_nn_tables.h"

#define USE_INTRINSIC

#ifdef __cplusplus
extern    "C"
{
#endif

void arm_avepool_q7_HWC_nonsquare (
        const q7_t * Im_in,         
        const uint16_t dim_im_in_x,   
        const uint16_t dim_im_in_y,   
        const uint16_t ch_im_in,    
        const uint16_t dim_kernel_x,  
        const uint16_t dim_kernel_y,  
        const uint16_t padding_x,     
        const uint16_t padding_y,     
        const uint16_t stride_x,      
        const uint16_t stride_y,      
        const uint16_t dim_im_out_x,  
        const uint16_t dim_im_out_y,  
        q7_t * bufferA,             
        q7_t * Im_out,
        const uint16_t out_lshift);


#ifdef __cplusplus
}
#endif

