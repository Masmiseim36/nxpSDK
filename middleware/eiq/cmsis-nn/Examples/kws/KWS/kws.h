/*
 * Copyright (C) 2018 Arm Limited or its affiliates. All rights reserved.
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

#ifndef __KWS_H__
#define __KWS_H__

#include "nn.h"
#include "mfcc.h"

class KWS{

public:
  ~KWS();
  void extract_features();
  void classify();
  void average_predictions();
  int get_top_class(q7_t* prediction);
  int16_t* audio_buffer;
  q7_t *mfcc_buffer;
  q7_t *output;
  q7_t *predictions;
  q7_t *averaged_output;
  int num_frames;
  int num_mfcc_features;
  int frame_len;
  int frame_shift;
  int num_out_classes;
  int audio_block_size;
  int audio_buffer_size;

protected:
  KWS();
  void init_kws();
  MFCC *mfcc;
  NN *nn;
  int mfcc_buffer_size;
  int recording_win;
  int sliding_window_len;
  
};

#endif
