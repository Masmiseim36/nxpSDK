/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "sensor/sensor_raw.h"
#include <stdio.h>

#include "demo_config.h"
#include "fsl_common.h"
#include "model.h"
#include "sensor.h"
#include "sensor_data.h"

status_t SENSOR_LoadStaticData(fxas21002_gyrodata_t *rawDataGyro, fxos8700_accelmagdata_t *rawDataAccel);

typedef status_t (*SENSOR_UpdateData_t)(fxas21002_gyrodata_t *rawDataGyro, fxos8700_accelmagdata_t *rawDataAccel);

static SENSOR_UpdateData_t SENSOR_updatePtr;
static int s_staticCount = 0;

status_t SENSOR_GetData(uint8_t *dstData, tensor_type_t inputType, bool *skip)
{
    static int16_t s_sensorData[PATCH_SIZE * NUM_CHANELS]     = {0};
    static int16_t s_sensorDataDiff[PATCH_SIZE * NUM_CHANELS] = {0};
    status_t status                                           = kStatus_Success;
    s_staticCount++;
    /* Switch to camera image capture after one static image. */
    switch (s_staticCount)
    {
        case 1:
            printf(EOL "Static sensor data processing:" EOL);
            SENSOR_updatePtr = &SENSOR_LoadStaticData;
            *skip            = true;
            break;
        case 2:
            *skip = false;
            break;
        case 3:
            break;
        case 4:
            SENSOR_Init();
            break;
        case 5:
            printf(EOL "Sensors data processing:" EOL);
            SENSOR_updatePtr = &SENSOR_Run;
            *skip            = true;
            break;
        default:
            s_staticCount = 5;
            *skip         = false;
            break;
    }

    status = SENSOR_UpdateData(s_sensorDataDiff, s_sensorData);
    if (status != kStatus_Success)
    {
        return status;
    }

    printf("Data (from %s) for inference are ready" EOL, SENSOR_GetSenzorDataName());

    SENSOR_PreprocessSample(dstData, s_sensorDataDiff, inputType);

    return status;
}

status_t SENSOR_UpdateData(int16_t *sensorDiff, int16_t *sensorData)
{
    fxas21002_gyrodata_t rawDataGyro;
    fxos8700_accelmagdata_t rawDataAccel;
    status_t status = kStatus_Success;

    for (int j = 0; j < PATCH_SIZE; j++)
    {
        status = SENSOR_updatePtr(&rawDataGyro, &rawDataAccel);
        if (status != kStatus_Success)
        {
            return status;
        }

        sensorDiff[j * NUM_CHANELS + 0] = sensorData[j * NUM_CHANELS + 0] - rawDataGyro.gyro[0];
        sensorDiff[j * NUM_CHANELS + 1] = sensorData[j * NUM_CHANELS + 1] - rawDataGyro.gyro[1];
        sensorDiff[j * NUM_CHANELS + 2] = sensorData[j * NUM_CHANELS + 2] - rawDataGyro.gyro[2];
        sensorDiff[j * NUM_CHANELS + 3] = sensorData[j * NUM_CHANELS + 3] - rawDataAccel.accel[0];
        sensorDiff[j * NUM_CHANELS + 4] = sensorData[j * NUM_CHANELS + 4] - rawDataAccel.accel[1];
        sensorDiff[j * NUM_CHANELS + 5] = sensorData[j * NUM_CHANELS + 5] - rawDataAccel.accel[2];

        sensorData[j * NUM_CHANELS + 0] = rawDataGyro.gyro[0];
        sensorData[j * NUM_CHANELS + 1] = rawDataGyro.gyro[1];
        sensorData[j * NUM_CHANELS + 2] = rawDataGyro.gyro[2];
        sensorData[j * NUM_CHANELS + 3] = rawDataAccel.accel[0];
        sensorData[j * NUM_CHANELS + 4] = rawDataAccel.accel[1];
        sensorData[j * NUM_CHANELS + 5] = rawDataAccel.accel[2];
    }
    return status;
}
void SENSOR_PreprocessSample(uint8_t *dstData, const int16_t *srcData, tensor_type_t inputType)
{
    /* Load input data */
    for (int i = 0; i < PATCH_SIZE * NUM_CHANELS; i += 6)
    {
        for (int ch = 0; ch < NUM_CHANELS; ch++)
        {
            float scaled = (ch == 3 || ch == 4 || ch == 5) ? srcData[i + ch] / 10 : srcData[i + ch];
            if (inputType == kTensorType_UINT8)
            {
                dstData[i + ch] = (uint8_t)(MIN(MAX((scaled + 1) * 127.5, 0.0), 255.0));
            }
            else
            {
                ((float *)dstData)[i + ch] = scaled;
            }
        }
    }
}

status_t SENSOR_CollectData(void)
{
    fxas21002_gyrodata_t rawDataGyro;
    fxos8700_accelmagdata_t rawDataAccel;
    status_t status = kStatus_Success;

    status = SENSOR_Init();
    if (status != kStatus_Success)
    {
        return status;
    }

    printf("\rtime,wx,wy,wz,ax,ay,az,Bx,By,Bz\r");

    for (int sample_num = 0; sample_num < SAMPLE_NUM; ++sample_num)
    {
        status = SENSOR_Run(&rawDataGyro, &rawDataAccel);
        if (status != kStatus_Success)
        {
            return status;
        }

        printf("%s%d, %d, %d, %d, %d, %d, %d, %d, %d, %d", EOL, sample_num, rawDataGyro.gyro[0], rawDataGyro.gyro[1],
               rawDataGyro.gyro[2], rawDataAccel.accel[0], rawDataAccel.accel[1], rawDataAccel.accel[2],
               rawDataAccel.mag[0], rawDataAccel.mag[1], rawDataAccel.mag[2]);
    }
    printf("\r\nAll samples are collected.\r\n");

    return status;
}

status_t SENSOR_LoadStaticData(fxas21002_gyrodata_t *rawDataGyro, fxos8700_accelmagdata_t *rawDataAccel)
{
    static int idx     = 0;
    const int16_t *ptr = no_detect_sample_data;

    if (s_staticCount == 3)
    {
        idx = 0;
        ptr = detect_sample_data;
    }

    rawDataGyro->gyro[0]   = ptr[idx * 9 + 0];
    rawDataGyro->gyro[1]   = ptr[idx * 9 + 1];
    rawDataGyro->gyro[2]   = ptr[idx * 9 + 2];
    rawDataAccel->accel[0] = ptr[idx * 9 + 3];
    rawDataAccel->accel[1] = ptr[idx * 9 + 4];
    rawDataAccel->accel[2] = ptr[idx * 9 + 5];
    rawDataAccel->mag[0]   = ptr[idx * 9 + 6];
    rawDataAccel->mag[1]   = ptr[idx * 9 + 7];
    rawDataAccel->mag[2]   = ptr[idx * 9 + 8];

    ++idx;
    return kStatus_Success;
}

const char *SENSOR_GetSenzorDataName(void)
{
    switch (s_staticCount)
    {
        case 1:
        case 2:
            return NO_DETECTED_SAMPLE_NAME;
        case 3:
        case 4:
            return DETECTED_SAMPLE_NAME;
    }
    return "sensor";
}
