// ----------------------------------------------------------------------------
// Copyright 2016-2017 ARM Ltd.
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

#ifndef __MEMORY_TESTS_H__
#define __MEMORY_TESTS_H__

#include "mbed-client/m2minterface.h"
#include "mbed.h"

//FORWARD DECLARATION
class M2MObject;
namespace m2m {
    template<class ObjectTemplate> class Vector;
}

using namespace m2m;

typedef Vector<M2MObject *> M2MObjectList;

// A function for creating a batch of resources for memory consumption purposes.
void create_m2mobject_test_set(M2MObjectList& object_list);

// Print into serial the m2m object sizes and heap allocation sizes.
void print_m2mobject_stats();

void print_heap_stats();

void print_stack_statistics();

#endif // !__MEMORY_TESTS_H__
