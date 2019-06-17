/*
 * Copyright (C) 2018 Arm Limited or its affiliates. All rights reserved.
 * Copyright 2018 NXP. All rights reserved.
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
 */

/*
 * Description: Example code for running keyword spotting on Cortex-M boards
 */

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#include "timer.h"
#include "kws_ds_cnn.h"
#include "wav_data.h"

int16_t audio_buffer[16000] = WAVE_DATA;
const char* labels[] = {"silence", "unknown", "yes", "no", "up", "down", "left", "right", "on", "off", "stop", "go"};

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    init_timer();

    PRINTF("Keyword spotting example using convolutional neural network\r\n");
     
    KWS_DS_CNN kws(audio_buffer);

    int start_time, end_time;
    start_time = get_time_in_us();
    /* Extract MFCC features */
    kws.extract_features();
    /* Classify using DS-CNN */
    kws.classify();
    end_time = get_time_in_us();
 
    int max_index = kws.get_top_class(kws.output);

    PRINTF("Elapsed time: %d ms\r\n", (end_time - start_time) / 1000);
    PRINTF("Detected: %s (%d%% confidence)\r\n", labels[max_index], (int)kws.output[max_index] * 100 / 128);

    for (;;) {}
}
