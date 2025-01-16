/*
 * Copyright 2022-2024 NXP
 * All rights reserved.
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "mpp_config.h"
#if (HAL_ENABLE_INFERENCE_TFLITE == 1)

#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#if defined(APP_USE_NEUTRON16_MODEL) || defined(APP_USE_NEUTRON64_MODEL)
#include "tensorflow/lite/micro/kernels/neutron/neutron.h"
#endif

tflite::MicroOpResolver __attribute__((weak)) &MODEL_GetOpsResolver()
{
    static tflite::MicroMutableOpResolver<114> s_microOpResolver;
    if (s_microOpResolver.FindOp(tflite::BuiltinOperator_ZEROS_LIKE) != nullptr){
        return s_microOpResolver;
    }

    s_microOpResolver.AddAbs();
    s_microOpResolver.AddAdd();
    s_microOpResolver.AddAddN();
    s_microOpResolver.AddArgMax();
    s_microOpResolver.AddArgMin();
    s_microOpResolver.AddAssignVariable();
    s_microOpResolver.AddAveragePool2D();
    s_microOpResolver.AddBatchMatMul();
    s_microOpResolver.AddBatchToSpaceNd();
    s_microOpResolver.AddBroadcastArgs();
    s_microOpResolver.AddBroadcastTo();
    s_microOpResolver.AddCallOnce();
    s_microOpResolver.AddCast();
    s_microOpResolver.AddCeil();
    s_microOpResolver.AddCircularBuffer();
    s_microOpResolver.AddConcatenation();
    s_microOpResolver.AddConv2D();
    s_microOpResolver.AddCos();
    s_microOpResolver.AddCumSum();
    s_microOpResolver.AddDelay();
    s_microOpResolver.AddDepthToSpace();
    s_microOpResolver.AddDepthwiseConv2D();
    s_microOpResolver.AddDequantize();
    s_microOpResolver.AddDetectionPostprocess();
    s_microOpResolver.AddDiv();
    s_microOpResolver.AddEmbeddingLookup();
    s_microOpResolver.AddEnergy();
    s_microOpResolver.AddElu();
    s_microOpResolver.AddEqual();
    s_microOpResolver.AddEthosU();
    s_microOpResolver.AddExp();
    s_microOpResolver.AddExpandDims();
    s_microOpResolver.AddFftAutoScale();
    s_microOpResolver.AddFill();
    s_microOpResolver.AddFilterBank();
    s_microOpResolver.AddFilterBankLog();
    s_microOpResolver.AddFilterBankSquareRoot();
    s_microOpResolver.AddFilterBankSpectralSubtraction();
    s_microOpResolver.AddFloor();
    s_microOpResolver.AddFloorDiv();
    s_microOpResolver.AddFloorMod();
    s_microOpResolver.AddFramer();
    s_microOpResolver.AddFullyConnected();
    s_microOpResolver.AddGather();
    s_microOpResolver.AddGatherNd();
    s_microOpResolver.AddGreater();
    s_microOpResolver.AddGreaterEqual();
    s_microOpResolver.AddHardSwish();
    s_microOpResolver.AddIf();
    s_microOpResolver.AddIrfft();
    s_microOpResolver.AddL2Normalization();
    s_microOpResolver.AddL2Pool2D();
    s_microOpResolver.AddLeakyRelu();
    s_microOpResolver.AddLess();
    s_microOpResolver.AddLessEqual();
    s_microOpResolver.AddLog();
    s_microOpResolver.AddLogicalAnd();
    s_microOpResolver.AddLogicalNot();
    s_microOpResolver.AddLogicalOr();
    s_microOpResolver.AddLogistic();
    s_microOpResolver.AddLogSoftmax();
    s_microOpResolver.AddMaximum();
    s_microOpResolver.AddMaxPool2D();
    s_microOpResolver.AddMirrorPad();
    s_microOpResolver.AddMean();
    s_microOpResolver.AddMinimum();
    s_microOpResolver.AddMul();
    s_microOpResolver.AddNeg();
    s_microOpResolver.AddNotEqual();
    s_microOpResolver.AddOverlapAdd();
    s_microOpResolver.AddPack();
    s_microOpResolver.AddPad();
    s_microOpResolver.AddPadV2();
    s_microOpResolver.AddPCAN();
    s_microOpResolver.AddPrelu();
    s_microOpResolver.AddQuantize();
    s_microOpResolver.AddReadVariable();
    s_microOpResolver.AddReduceMax();
    s_microOpResolver.AddRelu();
    s_microOpResolver.AddRelu6();
    s_microOpResolver.AddReshape();
    s_microOpResolver.AddResizeBilinear();
    s_microOpResolver.AddResizeNearestNeighbor();
    s_microOpResolver.AddRfft();
    s_microOpResolver.AddRound();
    s_microOpResolver.AddRsqrt();
    s_microOpResolver.AddSelectV2();
    s_microOpResolver.AddShape();
    s_microOpResolver.AddSin();
    s_microOpResolver.AddSlice();
    s_microOpResolver.AddSoftmax();
    s_microOpResolver.AddSpaceToBatchNd();
    s_microOpResolver.AddSpaceToDepth();
    s_microOpResolver.AddSplit();
    s_microOpResolver.AddSplitV();
    s_microOpResolver.AddSqueeze();
    s_microOpResolver.AddSqrt();
    s_microOpResolver.AddSquare();
    s_microOpResolver.AddSquaredDifference();
    s_microOpResolver.AddStridedSlice();
    s_microOpResolver.AddStacker();
    s_microOpResolver.AddSub();
    s_microOpResolver.AddSum();
    s_microOpResolver.AddSvdf();
    s_microOpResolver.AddTanh();
    s_microOpResolver.AddTransposeConv();
    s_microOpResolver.AddTranspose();
    s_microOpResolver.AddUnpack();
    s_microOpResolver.AddUnidirectionalSequenceLSTM();
    s_microOpResolver.AddVarHandle();
    s_microOpResolver.AddWhile();
    s_microOpResolver.AddWindow();
    s_microOpResolver.AddZerosLike();
#if defined(APP_USE_NEUTRON16_MODEL) || defined(APP_USE_NEUTRON64_MODEL)
    s_microOpResolver.AddCustom(tflite::GetString_NEUTRON_GRAPH(),
        tflite::Register_NEUTRON_GRAPH());
#endif
    return s_microOpResolver;
}

#endif // (HAL_ENABLE_INFERENCE_TFLITE == 1)
