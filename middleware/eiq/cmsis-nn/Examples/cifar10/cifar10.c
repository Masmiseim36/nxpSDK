/* ----------------------------------------------------------------------
* Copyright (C) 2010-2018 Arm Limited. All rights reserved.
* Copyright 2018-2019 NXP. All rights reserved.
*
*
* Project:       CMSIS NN Library
* Title:         arm_nnexamples_cifar10.cpp
*
* Description:   Convolutional Neural Network Example
*
* Target Processor: Cortex-M4/Cortex-M7
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*   - Neither the name of Arm LIMITED nor the names of its contributors
*     may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------- */

/* File modified by NXP. Changes are described in file
   /middleware/eiq/cmsis-nn/readme.txt in section "Release notes" */

#include <stdio.h>

#include <cmsis_compiler.h>

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"

#include "arm_nnfunctions.h"
#include "board_init.h"
#include "demo_config.h"
#include "image.h"
#include "labels.h"
#include "parameter.h"
#include "timer.h"
#include "weights.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define IMAGE_WIDTH 32
#define IMAGE_HEIGHT 32
#define IMAGE_CHANNELS 3

#define INPUT_MEAN_SHIFT {125,123,114}
#define INPUT_RIGHT_SHIFT {8,8,8}

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint8_t s_imageData[IMAGE_WIDTH * IMAGE_HEIGHT * IMAGE_CHANNELS];

/*******************************************************************************
 * Code
 ******************************************************************************/

/* conv1_wt, conv2_wt, conv3_wt are convolution layer weight matrices */
/* conv1_bias, conv2_bias, conv3_bias are convolution layer bias arrays */
static const q7_t __ALIGNED(4) conv1_wt[CONV1_IN_CH * CONV1_KER_DIM * CONV1_KER_DIM * CONV1_OUT_CH] = CONV1_WT;
static const q7_t __ALIGNED(4) conv1_bias[CONV1_OUT_CH] = CONV1_BIAS;

static const q7_t __ALIGNED(4) conv2_wt[CONV2_IN_CH * CONV2_KER_DIM * CONV2_KER_DIM * CONV2_OUT_CH] = CONV2_WT;
static const q7_t __ALIGNED(4) conv2_bias[CONV2_OUT_CH] = CONV2_BIAS;

static const q7_t __ALIGNED(4) conv3_wt[CONV3_IN_CH * CONV3_KER_DIM * CONV3_KER_DIM * CONV3_OUT_CH] = CONV3_WT;
static const q7_t __ALIGNED(4) conv3_bias[CONV3_OUT_CH] = CONV3_BIAS;

/* ip1_wt, ip1_bias point to fully-connected layer weights and biases */
static const q7_t __ALIGNED(4) ip1_wt[IP1_IN_DIM * IP1_OUT_DIM] = IP1_WT;
static const q7_t __ALIGNED(4) ip1_bias[IP1_OUT_DIM] = IP1_BIAS;

/* output_data points to the classification output */
static q7_t __ALIGNED(4) output_data[IP1_OUT_DIM];

/* col_buffer is a buffer to store the im2col output */
static q7_t __ALIGNED(4) col_buffer[6400];

/* scratch_buffer is used to store the activation data (intermediate layer outputs) */
static q7_t __ALIGNED(4) scratch_buffer[40960];

/* image_data - raw uint8 type RGB image in [R, G, B, R, G, B, R, G, B, ... R, G, B] format */
void run_nn(const uint8_t *image_data)
{
  q7_t *buffer1 = scratch_buffer;
  q7_t *buffer2 = buffer1 + 32 * 32 * 32;

  int mean_data[3] = INPUT_MEAN_SHIFT;
  unsigned int scale_data[3] = INPUT_RIGHT_SHIFT;

  /* Input pre-processing */
  for (int i = 0; i < 32 * 32 * 3; i += 3)
  {
    buffer2[i]   = (q7_t)__SSAT(((((int)image_data[i] - mean_data[0]) << 7)
      + (0x1 << (scale_data[0] - 1))) >> scale_data[0], 8);
    buffer2[i + 1] = (q7_t)__SSAT(((((int)image_data[i + 1] - mean_data[1]) << 7)
      + (0x1 << (scale_data[1] - 1))) >> scale_data[1], 8);
    buffer2[i+2] = (q7_t)__SSAT(((((int)image_data[i + 2] - mean_data[2]) << 7)
      + (0x1 << (scale_data[2] - 1))) >> scale_data[2], 8);
  }

  /* conv1 buffer2 -> buffer1 */
  arm_convolve_HWC_q7_RGB(buffer2, CONV1_IN_DIM, CONV1_IN_CH, conv1_wt,
    CONV1_OUT_CH, CONV1_KER_DIM, CONV1_PAD, CONV1_STRIDE, conv1_bias,
    CONV1_BIAS_LSHIFT, CONV1_OUT_RSHIFT, buffer1, CONV1_OUT_DIM,
    (q15_t *) col_buffer, NULL);

  arm_relu_q7(buffer1, CONV1_OUT_DIM * CONV1_OUT_DIM * CONV1_OUT_CH);

  /* pool1 buffer1 -> buffer2 */
  arm_maxpool_q7_HWC(buffer1, CONV1_OUT_DIM, CONV1_OUT_CH, POOL1_KER_DIM,
                     POOL1_PAD, POOL1_STRIDE, POOL1_OUT_DIM, NULL, buffer2);

  /* conv2 buffer2 -> buffer1 */
  arm_convolve_HWC_q7_fast(buffer2, CONV2_IN_DIM, CONV2_IN_CH, conv2_wt,
    CONV2_OUT_CH, CONV2_KER_DIM, CONV2_PAD, CONV2_STRIDE, conv2_bias,
    CONV2_BIAS_LSHIFT, CONV2_OUT_RSHIFT, buffer1, CONV2_OUT_DIM,
    (q15_t *) col_buffer, NULL);

  arm_relu_q7(buffer1, CONV2_OUT_DIM * CONV2_OUT_DIM * CONV2_OUT_CH);

  /* pool2 buffer1 -> buffer2 */
  arm_maxpool_q7_HWC(buffer1, CONV2_OUT_DIM, CONV2_OUT_CH, POOL2_KER_DIM,
                     POOL2_PAD, POOL2_STRIDE, POOL2_OUT_DIM, col_buffer, buffer2);

  /* conv3 buffer2 -> buffer1 */
  arm_convolve_HWC_q7_fast(buffer2, CONV3_IN_DIM, CONV3_IN_CH, conv3_wt,
    CONV3_OUT_CH, CONV3_KER_DIM, CONV3_PAD, CONV3_STRIDE, conv3_bias,
    CONV3_BIAS_LSHIFT, CONV3_OUT_RSHIFT, buffer1, CONV3_OUT_DIM,
    (q15_t *) col_buffer, NULL);

  arm_relu_q7(buffer1, CONV3_OUT_DIM * CONV3_OUT_DIM * CONV3_OUT_CH);

  /* pool3 buffer-> buffer2 */
  arm_maxpool_q7_HWC(buffer1, CONV3_OUT_DIM, CONV3_OUT_CH, POOL3_KER_DIM,
                     POOL3_PAD, POOL3_STRIDE, POOL3_OUT_DIM, col_buffer, buffer2);

  arm_fully_connected_q7_opt(buffer2, ip1_wt, IP1_IN_DIM, IP1_OUT_DIM,
    IP1_BIAS_LSHIFT, IP1_OUT_RSHIFT, ip1_bias, output_data, (q15_t *) buffer1);

  arm_softmax_q7(output_data, 10, output_data);
}

/*!
 * @brief  Run inference. It processed static image if static image is not NULL
 * otherwise camera input is processed.
 *
 * param staticImage pointer to address of static image. Use NULL when static image
 * is not required.
 * param staticImageLen size of static image.
 */
void run_inference(const uint8_t *image_data, const char* labels[])
{
  uint32_t start = 0U;
  uint32_t end = 0U;
  q7_t max_value;
  uint32_t max_index;

  start = TIMER_GetTimeInUS();
  run_nn(image_data);
  end = TIMER_GetTimeInUS();

  /* Find out the true item with the biggest value */
  arm_max_q7(output_data, 10, &max_value, &max_index);
  int score = (max_value * 100) / 128;

  if (score > DETECTION_TRESHOLD)
  {
    printf("----------------------------------------\r\n");
    printf("     Inference time : %ld ms    \r\n", (end - start) / 1000);
    printf("     Detected: %.10s (%d%%)\r\n", labels[max_index], (int)score);
    printf("----------------------------------------\r\n\r\n");
  }
}

/*!
 * @brief Main function
 */
int main(void)
{
  BOARD_Init();
  TIMER_Init();

  printf("CIFAR-10 object recognition example using CMSIS-NN.\r\n");
  printf("Detection threshold: %d%%\r\n", DETECTION_TRESHOLD);

  while (1)
  {
    if (IMAGE_GetImage(s_imageData, IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_CHANNELS) != kStatus_Success)
    {
      printf("Failed retrieving input image\r\n");
      for (;;) {}
    }

    run_inference(s_imageData, labels);
  }
}
