/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.
   Copyright 2019 NXP
   
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
#include "systick_utils.h"
#if defined(__cplusplus)
}
#endif

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/optional_debug_tools.h"
#include "tensorflow/lite/string_util.h"

#include "get_sensor_data.h"
#include "adt_model.h"
#include "parameters.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DATA_COLLECT false
#define SAMPLE_NUM 1000

#define LOG(x) std::cout
#define CHECK_STATUS(STATUS, MESSAGE)                             \
  if (STATUS != 0) {                                              \
    LOG(FATAL) << MESSAGE << " Error code: " << STATUS << "\r\n"; \
  }                                                               \
  do {                                                            \
    ;                                                             \
  } while (STATUS)

/*******************************************************************************
 * Code
 ******************************************************************************/
using namespace std;

/*!
 * brief Initialize parameters for inference
 *
 * param reference to flat buffer
 * param reference to interpreter
 * param pointer to storing input tensor address
 * @return It returns kTfLiteOk if success otherwise kTfLiteError
 */
TfLiteStatus InferenceInit(unique_ptr<tflite::FlatBufferModel>& model,
                           unique_ptr<tflite::Interpreter>& interpreter,
                           TfLiteTensor** input_tensor) {
  TfLiteStatus status = kTfLiteOk;

  model = tflite::FlatBufferModel::BuildFromBuffer(adt_model, adt_model_len);
  if (!model) {
    LOG(FATAL) << "Failed to load model\r\n";
    return kTfLiteError;
  }

  tflite::ops::builtin::BuiltinOpResolver resolver;

  tflite::InterpreterBuilder(*model, resolver)(&interpreter);
  if (!interpreter) {
    LOG(FATAL) << "Failed to construct interpreter\r\n ";
    return kTfLiteError;
  }

  int input = interpreter->inputs()[0];

  if ((status = interpreter->AllocateTensors()) != kTfLiteOk) {
    LOG(FATAL) << "Failed to allocate tensors!\r\n";
    return status;
  }

  /* Get input dimension from the input tensor metadata
     assuming one input only */
  *input_tensor = interpreter->tensor(input);

  return status;
}

/*!
 * brief Runs inference input buffer and print result to console
 *
 * param pointer to kws mfcc class
 * param reference to flat buffer model
 * param reference to interpreter
 * param pointer to input tensor
 * @return It returns kTfLiteOk if success otherwise kTfLiteError
 */
TfLiteStatus RunInference(int16_t* buf,
                          unique_ptr<tflite::FlatBufferModel>& model,
                          unique_ptr<tflite::Interpreter>& interpreter,
                          TfLiteTensor* input_tensor) {
  TfLiteStatus status = kTfLiteOk;

  /* Load input data */
  for (int i = 0; i < PATCH_SIZE * NUM_CHANELS; i += 6) {
    for (int ch = 0; ch < NUM_CHANELS; ch++) {
      float scaled =
          (ch == 3 || ch == 4 || ch == 5) ? buf[i + ch] / 10 : buf[i + ch];
      if (input_tensor->type == kTfLiteUInt8) {
        input_tensor->data.uint8[i + ch] = 
          static_cast<uint8_t>(min(max((scaled + 1) * 127.5, 0.0), 255.0));
      } else {
        input_tensor->data.f[i + ch] = scaled;
      }
    }
  }

  /* Run inference */
  int32_t timeStart;
  BOARD_SystickStart(&timeStart);

  if ((status = interpreter->Invoke()) != kTfLiteOk) {
    LOG(FATAL) << "Failed to invoke tflite!\r\n";
    return status;
  }
  uint32_t time = BOARD_SystickElapsedTime_us(&timeStart);

  /* Compute error */
  float err = 0.0;
  float f1;
  float f2;
  int output = interpreter->outputs()[0];
  TfLiteTensor* output_tensor = interpreter->tensor(output);
  for (int i = 0; i < PATCH_SIZE * NUM_CHANELS; i++) {
    if (input_tensor->type == kTfLiteUInt8) {
      f1 = static_cast<float>(input_tensor->data.uint8[i]);
      f2 = static_cast<float>(output_tensor->data.uint8[i]);
    } else {
      f1 = input_tensor->data.f[i];
      f2 = output_tensor->data.f[i];
    }
    err += pow(f1 - f2, 2);
  }
  err /= (PATCH_SIZE * NUM_CHANELS);

  LOG(INFO) << "(%" << static_cast<int>(time) << " us) "
            << static_cast<int>(err) << "."
            << (static_cast<int>(err * 100)) % 100;

  if (err > THRESHOLD) LOG(INFO) << " anomaly detected!!!\r\n";

  LOG(INFO) << "\r\n";

  return status;
}

/*!
 * brief Initilizes device and runs KWS application
 *
 */
int main(void) {
  /*
   *    SENSOR READING CODE HERE
   */
  /* Initialize the MCU hardware */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  BOARD_SystickEnable();
  fxas21002_gyrodata_t rawDataGyro;
  fxos8700_accelmagdata_t rawDataAccel;

  status_t status = kStatus_Success;
  TfLiteStatus tfStatus = kTfLiteOk;

  unique_ptr<tflite::FlatBufferModel> model;
  unique_ptr<tflite::Interpreter> interpreter;
  TfLiteTensor* input_tensor = 0;

  if (!DATA_COLLECT) {
    LOG(INFO) << "Anomaly Detection example using a TensorFlow Lite model.\r\n";
    LOG(INFO) << "Threshold value " << static_cast<int>(THRESHOLD) << "."
       << (static_cast<int>(THRESHOLD * 100)) % 100 << "\r\n";
    tfStatus = InferenceInit(model, interpreter, &input_tensor);
    CHECK_STATUS(tfStatus, "Inference failed.");
  }

  status = init_sensors();
  CHECK_STATUS(status, "Initialization Sensors failed.");

  int16_t sensor_data[PATCH_SIZE * NUM_CHANELS] = {0};
  int16_t sensor_diff[PATCH_SIZE * NUM_CHANELS] = {0};

  int sample_num = 0;
  if (DATA_COLLECT) {
    LOG(INFO) << "\rtime,wx,wy,wz,ax,ay,az,Bx,By,Bz\r";
  }
  
  while (true) {
    for (int j = 0; j < PATCH_SIZE; j++) {
      status = run_sensors(&rawDataGyro, &rawDataAccel);

      CHECK_STATUS(status, "Sensors exit with error.");

      sensor_diff[j * NUM_CHANELS + 0] = 
        sensor_data[j * NUM_CHANELS + 0] - rawDataGyro.gyro[0];
      sensor_diff[j * NUM_CHANELS + 1] = 
        sensor_data[j * NUM_CHANELS + 1] - rawDataGyro.gyro[1];
      sensor_diff[j * NUM_CHANELS + 2] = 
        sensor_data[j * NUM_CHANELS + 2] - rawDataGyro.gyro[2];
      sensor_diff[j * NUM_CHANELS + 3] = 
        sensor_data[j * NUM_CHANELS + 3] - rawDataAccel.accel[0];
      sensor_diff[j * NUM_CHANELS + 4] = 
        sensor_data[j * NUM_CHANELS + 4] - rawDataAccel.accel[1];
      sensor_diff[j * NUM_CHANELS + 5] = 
        sensor_data[j * NUM_CHANELS + 5] - rawDataAccel.accel[2];

      sensor_data[j * NUM_CHANELS + 0] = rawDataGyro.gyro[0];
      sensor_data[j * NUM_CHANELS + 1] = rawDataGyro.gyro[1];
      sensor_data[j * NUM_CHANELS + 2] = rawDataGyro.gyro[2];
      sensor_data[j * NUM_CHANELS + 3] = rawDataAccel.accel[0];
      sensor_data[j * NUM_CHANELS + 4] = rawDataAccel.accel[1];
      sensor_data[j * NUM_CHANELS + 5] = rawDataAccel.accel[2];

      if (DATA_COLLECT) {
        LOG(INFO) << "\r\n"
          << sample_num << "," << rawDataGyro.gyro[0] << ","
          << rawDataGyro.gyro[1] << "," << rawDataGyro.gyro[2] << ","
          << rawDataAccel.accel[0] << "," << rawDataAccel.accel[1] << ","
          << rawDataAccel.accel[2] << "," << rawDataAccel.mag[0] << ","
          << rawDataAccel.mag[1] << "," << rawDataAccel.mag[2] << "\r";

        if (sample_num++ > SAMPLE_NUM) {
          LOG(INFO) << "\r\nAll samples are collected.\r\n";
          while (true) {;}
        }
      }

    }
    if (!DATA_COLLECT) {
      tfStatus = RunInference(sensor_diff, model, interpreter, input_tensor);
      CHECK_STATUS(tfStatus, "Inference failed.");
    }

    LOG(INFO) << flush;
  }

}
