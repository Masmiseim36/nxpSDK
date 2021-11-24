// Copyright (c) 2015-2021 Orbbec Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Be excellent to each other.

#ifndef __DEPTH_H__
#define __DEPTH_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define MODE_DEPTH 1
#define MODE_IR 2

typedef struct ShiftInitConfig
{
    float fZeroPlaneDistance;      // 40
    float fZeroPlanePixelSize;     // 0.037037f
    float fEmitterDCmosDistance;   // 4.0f
    uint32_t nDeviceMaxShiftValue; // 1024
    uint32_t nDeviceMaxDepthValue; // 2000 : 12bit ok, 10000, fail.

    uint32_t nConstShift;      // 200
    uint32_t nPixelSizeFactor; // 1
    uint32_t nParamCoeff;      // 4
    uint32_t nShiftScale;      // 10

    uint16_t nDepthMinCutOff; // 0
    uint16_t nDepthMaxCutOff; // 65534

    uint16_t w; // 480
    uint16_t h; // 640
} ShiftInitConfig;

int OrbbecDevice_init(ShiftInitConfig p);
 /**
 * Set Depth Image Resolution, accuracy args.
 * @param resolutionX  Depth Image width
 * @param resolutionY  Depth Image height
 * @param zFactor     1: 1mm,   100: 0.1mm
 * @return  0, success < 0 failed
 */
int OrbbecDevice_refreshWorldConversionCache(int resolutionX, int resolutionY, int zFactor);
  /**
 * convert shift data to depth data
 * @param [in] src 16bit unpack raw data valid data at bit11-bit0
 * @param [out] dst 16bit depth data
 * @param [in] w   width
 * @param [in] h   height
 * @return 0: success, < 0 failed
 */
int OrbbecDevice_shift9_2ToDepth(uint16_t *src, uint16_t *dst, int w, int h);
/**
* convert shift data to depth data
* @param [in] src 16bit unpack raw data valid data at bit13-bit2
* @param [out] dst 16bit depth data
* @param [in] w   width
* @param [in] h   height
* @return 0: success, < 0 failed
*/
int OrbbecDevice_shift13_2ToDepth(uint16_t *src, uint16_t *dst, int w, int h);
  /**
 * judge the frame is depth or ir.
 * @param  [pInput] src 12bit raw data valid bit11-bit0
 * @return 1 : MODE_DEPTH, 2 : MODE_IR
 */
int parseFirstLine(unsigned short *pInput);
/**
 *  judge the frame is depth or ir.valid bit13-bit2
 */
int parseshift13_2Line(unsigned short *pInput);
#ifdef __cplusplus
}
#endif

#endif
