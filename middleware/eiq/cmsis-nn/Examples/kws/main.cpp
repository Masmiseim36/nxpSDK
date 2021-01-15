/* ----------------------------------------------------------------------
 * Copyright (C) 2018 Arm Limited or its affiliates. All rights reserved.
 * Copyright 2018-2020 NXP. All rights reserved.
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
 * ----------------------------------------------------------------------*/

/* File modified by NXP. Changes are described in file
   /middleware/eiq/cmsis-nn/readme.txt in section "Release notes" */

/*
 * Description: Example code for running keyword spotting on Cortex-M boards
 */
#include <iostream>
#include <iomanip>

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "audio_data.h"
#include "eiq_audio_worker.h"
#include "kws_ds_cnn.h"
#include "timer.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DETECTION_TRESHOLD 25

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Runs inference input buffer and print result to console
 *
 * @param pointer to kws
 * @param pointer to input data buffer
 * @param pointer to input data buffer
 * @param output class names array
 */
void run_inference(KWS *kws, int16_t* buf, const char* output_class[12])
{
  int detection_threshold = DETECTION_TRESHOLD; //in percent
  kws->audio_buffer = buf;
  kws->extract_features(); //extract mfcc features
  int start = TIMER_GetTimeInUS();
  kws->classify(); //classify using dnn
  kws->average_predictions();
  int end = TIMER_GetTimeInUS();
  int max_ind = kws->get_top_class(kws->output);
  if (kws->averaged_output[max_ind] > detection_threshold * 128 / 100)
  {
    std::cout << "----------------------------------------\r\n"
    << "     Inference time: " << (end - start) / 1000 << " ms\r\n"
    << "     Detected: " << std::setw(10) << output_class[max_ind] 
    << " (" << ((int)kws->averaged_output[max_ind] * 100 / 128) << "%)\r\n"
    << "----------------------------------------\r\n\r\n";
  }
}

/*!
 * @brief Main function
 */
int main(void)
{
  /* (recording_win x frame_shift) is the actual recording window size. */
  int recording_win = 49;
  /* Averaging window for smoothing out the output predictions. */
  int averaging_window_len = 1;

  /* Create new instance for static audio files with averaging window len = 1. */
  KWS_DS_CNN *kws = new KWS_DS_CNN(recording_win, averaging_window_len);

  const char* output_class[12] = {"Silence", "Unknown", "yes", "no", "up", "down", "left", "right", "on", "off", "stop", "go"};

  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  TIMER_Init();

  std::cout << "Keyword spotting example using CMSIS-NN.\r\n";
  std::cout << "Detection threshold: " << DETECTION_TRESHOLD << "\r\n";

  std::cout << "\r\nStatic data processing:\r\n";

  run_inference(kws, (int16_t*)off_sample_data, output_class);
  run_inference(kws, (int16_t*)right_sample_data, output_class);

  delete kws;

  std::cout << "\r\nMicrophone data processing:\r\n";

  EIQ_AudioWorker_t* worker = EIQ_AudioWorkerInit();

  /* Averaging window for smoothing out the output predictions. */
  averaging_window_len = 1;
  /* Create new instance for static audio files with averaging window len = 1. */
  kws = new KWS_DS_CNN(recording_win, averaging_window_len);

  worker->base.start();
  while (1)
  {
    if(worker->base.isReady()){
      run_inference(kws, (int16_t *)worker->base.getData(), output_class);
      std::cout << "Data for inference are ready\r\n";
    }
  }
}
