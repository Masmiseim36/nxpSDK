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

#include <string.h>

static inline unsigned int getXYZW(
    const unsigned int *dims, unsigned int x, unsigned int y, unsigned int z, unsigned int w)
{
    return (x * dims[1] * dims[2] * dims[3]) + (y * dims[2] * dims[3]) + (z * dims[3]) + w;
}

static inline unsigned int min(unsigned int a, unsigned int b)
{
    if (a < b)
        return a;
    return b;
}

void resizenearest_f32(float *dst,
                       const float *src,
                       const float scaleH,
                       const float scaleW,
                       const unsigned int *inWdims,
                       const unsigned int *outWdims)
{
    for (unsigned int ob = 0; ob < outWdims[0]; ++ob)
    {
        unsigned int ib = ob;
        for (unsigned int oh = 0; oh < outWdims[1]; ++oh)
        {
            unsigned int ih = min((unsigned int)(oh / (scaleH)), inWdims[1] - 1);
            for (unsigned int ow = 0; ow < outWdims[2]; ++ow)
            {
                unsigned int iw             = min((unsigned int)(ow / (scaleW)), inWdims[2] - 1);
                const unsigned int inIndex  = getXYZW(inWdims, ob, ih, iw, 0);
                const unsigned int outIndex = getXYZW(outWdims, ib, oh, ow, 0);
                memcpy(&dst[outIndex], &src[inIndex], sizeof(float) * outWdims[3]);
            }
        }
    }
}

void resizenearest_i8(char *dst,
                      const char *src,
                      const float scaleH,
                      const float scaleW,
                      const unsigned int *inWdims,
                      const unsigned int *outWdims)
{
    for (unsigned int ob = 0; ob < outWdims[0]; ++ob)
    {
        unsigned int ib = ob;
        for (unsigned int oh = 0; oh < outWdims[1]; ++oh)
        {
            unsigned int ih = min((unsigned int)(oh / (scaleH)), inWdims[1] - 1);
            for (unsigned int ow = 0; ow < outWdims[2]; ++ow)
            {
                unsigned int iw             = min((unsigned int)(ow / (scaleW)), inWdims[2] - 1);
                const unsigned int inIndex  = getXYZW(inWdims, ob, ih, iw, 0);
                const unsigned int outIndex = getXYZW(outWdims, ib, oh, ow, 0);
                memcpy(&dst[outIndex], &src[inIndex], sizeof(char) * outWdims[3]);
            }
        }
    }
}
