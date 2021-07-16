/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <stdint.h>

#include "fsl_common.h"
#include "model.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

status_t SENSOR_GetData(uint8_t *dstData, tensor_type_t inputType, bool *skip);
status_t SENSOR_UpdateData(int16_t *sensorDiff, int16_t *sensorData);
void SENSOR_PreprocessSample(uint8_t *dstData, const int16_t *srcData, tensor_type_t inputType);
status_t SENSOR_CollectData(void);
const char *SENSOR_GetSenzorDataName(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _AUDIO_H_ */
