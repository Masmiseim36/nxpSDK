/**
 * Copyright (c) Glow Contributors. See CONTRIBUTORS file.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __DSP_RESIZENEAREST_H__
#define __DSP_RESIZENEAREST_H__

void resizenearest_f32(float *dst,
                       const float *src,
                       const float scaleH,
                       const float scaleW,
                       const unsigned int *inWdims,
                       const unsigned int *outWdims);

void resizenearest_i8(char *dst,
                      const char *src,
                      const float scaleH,
                      const float scaleW,
                      const unsigned int *inWdims,
                      const unsigned int *outWdims);

#endif // __DSP_RESIZENEAREST_H__
