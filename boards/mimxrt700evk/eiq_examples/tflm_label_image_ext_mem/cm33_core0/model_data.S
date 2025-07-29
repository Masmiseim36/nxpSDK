/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if defined(__GNUC__) || defined(__ARMCC_VERSION)
        .section .modeldata , "ax" @progbits @preinit_array
        .align 16
        .global model_data
        .global model_data_end

model_data:
        .incbin "mobilenet_v1_1.0_224_int8_npu.tflite"
model_data_end:
#endif
