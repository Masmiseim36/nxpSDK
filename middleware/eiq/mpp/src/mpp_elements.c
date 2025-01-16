/*
 * Copyright 2020-2022,2024 NXP.
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

/* forward declaration for TEST element setup function */
unsigned int elem_test_setup(_elem_t *elem);
unsigned int elem_lbl_rct_setup(_elem_t *elem);
unsigned int elem_convert_setup(_elem_t *elem);
unsigned int elem_inference_setup(_elem_t *elem);

typedef struct _elem_func_id_pair {
    mpp_element_id_t id;
    elem_setup_func_t func;
} elem_func_id_pair;

/* registered elements setup functions */
elem_func_id_pair elem_setup_funcs[] =
{
//    {MPP_ELEMENT_COMPOSE, elem_compose_setup},
    {MPP_ELEMENT_INFERENCE, elem_inference_setup},
    {MPP_ELEMENT_LABELED_RECTANGLE, elem_lbl_rct_setup},
    {MPP_ELEMENT_CONVERT, elem_convert_setup},
#ifdef EMULATOR
    {MPP_ELEMENT_TEST, elem_test_setup},
#endif
    {MPP_ELEMENT_NUM, NULL}   /* keep this last */
};

elem_setup_func_t get_setup_function(mpp_element_id_t id) {
    int idx = 0;
    mpp_element_id_t refid = MPP_ELEMENT_NUM;
    elem_setup_func_t fp = NULL;
    do {
        refid = elem_setup_funcs[idx].id;
        if (refid == id) {
            fp = elem_setup_funcs[idx].func;
            break;  /* id found */
        }
        idx++;
    }
    while(refid != MPP_ELEMENT_NUM);

   return fp;
}
