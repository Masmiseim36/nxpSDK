// ----------------------------------------------------------------------------
// Copyright 2016-2017 ARM Ltd.
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

#ifndef __FCC_STATS_H__
#define __FCC_STATS_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* Prints heap statistics results.
* - Usually user will call this function at program end.
*/
void fcc_stats_print_summary(void);

#ifndef FCC_MEM_STATS_ENABLED
#define fcc_stats_print_summary()
#endif

#ifdef __cplusplus
}
#endif

#endif
