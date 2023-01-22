/*
 * Copyright 2019-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 *      This code is auto-generated and shall not be modified!
 */

#include <stdio.h>

#include "config_nn.h"
#include "dsp_nn.h"
#include "srtm_config.h"
#include "xa_type_def.h"
#include "xa_nnlib_kernels_api.h"
#include "xa_nnlib_standards.h"
#include "dsp_resizenearest.h"
#if NN_ENABLE_xa_nn_inference == 1
#include "model.h"
#endif

#if (defined(CPU_MIMXRT595SFAWC_dsp) || defined(CPU_MIMXRT595SFFOC_dsp))
#include "fsl_memory.h"
#endif
#if (defined(CPU_MIMXRT685SEVKA_dsp) || defined(CPU_MIMXRT685SFVKB_dsp))
// ARM CM33 bus base addresses
#define ARM_BUS_CACHEABLE_NONSECURE     0x00000000
#define ARM_BUS_NON_CACHEABLE_NONSECURE 0x20000000

#define kMEMORY_Local2DMA ARM_BUS_NON_CACHEABLE_NONSECURE

// Macro function to modify address in order to access
// memory through a specific bus.
#define MEMORY_ConvertMemoryMapAddress(addr, bus) (((unsigned int)(addr)&0x0FFFFFFF) + (bus))
#endif

unsigned int xa_nn_get_version()
{
    return (unsigned int)(2 | (4 << 8) | (1 << 16));
}

int handleMSG_NN(srtm_message *msg)
{
    int rc;

    switch (msg->head.command)
    {
        case SRTM_Command_check_version:
            rc            = xa_nn_get_version();
            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;

#if NN_ENABLE_xa_nn_matXvec_16x16_16 == 1
        case SRTM_Command_xa_nn_matXvec_16x16_16:
            rc = xa_nn_matXvec_16x16_16(
                (signed short *)MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA), // p_out
                (signed short *)MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA), // p_mat1
                (signed short *)MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA), // p_mat2
                (signed short *)MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA), // p_vec1
                (signed short *)MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA), // p_vec2
                (signed short *)MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA), // p_bias
                (signed int)(msg->param[6]),                                                      // rows
                (signed int)(msg->param[7]),                                                      // cols1
                (signed int)(msg->param[8]),                                                      // cols2
                (signed int)(msg->param[9]),                                                      // row_stride1
                (signed int)(msg->param[10]),                                                     // row_stride2
                (signed int)(msg->param[11]),                                                     // acc_shift
                (signed int)(msg->param[12])                                                      // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_16x16_32 == 1
        case SRTM_Command_xa_nn_matXvec_16x16_32:
            rc = xa_nn_matXvec_16x16_32(
                (signed int *)MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA),   // p_out
                (signed short *)MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA), // p_mat1
                (signed short *)MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA), // p_mat2
                (signed short *)MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA), // p_vec1
                (signed short *)MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA), // p_vec2
                (signed short *)MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA), // p_bias
                (signed int)(msg->param[6]),                                                      // rows
                (signed int)(msg->param[7]),                                                      // cols1
                (signed int)(msg->param[8]),                                                      // cols2
                (signed int)(msg->param[9]),                                                      // row_stride1
                (signed int)(msg->param[10]),                                                     // row_stride2
                (signed int)(msg->param[11]),                                                     // acc_shift
                (signed int)(msg->param[12])                                                      // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_16x16_64 == 1
        case SRTM_Command_xa_nn_matXvec_16x16_64:
            rc = xa_nn_matXvec_16x16_64(
                (signed long long *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),     // p_mat1
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)),     // p_mat2
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),     // p_vec1
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)),     // p_vec2
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)),     // p_bias
                (signed int)(msg->param[6]),                                                            // rows
                (signed int)(msg->param[7]),                                                            // cols1
                (signed int)(msg->param[8]),                                                            // cols2
                (signed int)(msg->param[9]),                                                            // row_stride1
                (signed int)(msg->param[10]),                                                           // row_stride2
                (signed int)(msg->param[11]),                                                           // acc_shift
                (signed int)(msg->param[12])                                                            // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_16x16_16_tanh == 1
        case SRTM_Command_xa_nn_matXvec_16x16_16_tanh:
            rc = xa_nn_matXvec_16x16_16_tanh(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat2
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_vec1
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)), // p_vec2
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)),         // p_bias
                (signed int)(msg->param[6]),                                                        // rows
                (signed int)(msg->param[7]),                                                        // cols1
                (signed int)(msg->param[8]),                                                        // cols2
                (signed int)(msg->param[9]),                                                        // row_stride1
                (signed int)(msg->param[10]),                                                       // row_stride2
                (signed int)(msg->param[11]),                                                       // acc_shift
                (signed int)(msg->param[12]),                                                       // bias_shift
                (signed int)(msg->param[13]),                                                       // bias_precision
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[14], kMEMORY_Local2DMA))         // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_16x16_16_sigmoid == 1
        case SRTM_Command_xa_nn_matXvec_16x16_16_sigmoid:
            rc = xa_nn_matXvec_16x16_16_sigmoid(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat1
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_mat2
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)), // p_vec1
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)), // p_vec2
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[6], kMEMORY_Local2DMA)),         // p_bias
                (signed int)(msg->param[6]),                                                        // rows
                (signed int)(msg->param[7]),                                                        // cols1
                (signed int)(msg->param[8]),                                                        // cols2
                (signed int)(msg->param[9]),                                                        // row_stride1
                (signed int)(msg->param[10]),                                                       // row_stride2
                (signed int)(msg->param[11]),                                                       // acc_shift
                (signed int)(msg->param[12]),                                                       // bias_shift
                (signed int)(msg->param[13]),                                                       // bias_precision
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[14], kMEMORY_Local2DMA))         // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_batch_16x16_64 == 1
        case SRTM_Command_xa_nn_matXvec_batch_16x16_64:
            rc = xa_nn_matXvec_batch_16x16_64(
                (signed long long **)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),      // p_mat1
                (signed short **)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),     // p_vec1
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),      // p_bias
                (signed int)(msg->param[4]),                                                             // rows
                (signed int)(msg->param[5]),                                                             // cols1
                (signed int)(msg->param[6]),                                                             // row_stride1
                (signed int)(msg->param[7]),                                                             // acc_shift
                (signed int)(msg->param[8]),                                                             // bias_shift
                (signed int)(msg->param[9])                                                              // vec_count
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matmul_16x16_16 == 1
        case SRTM_Command_xa_nn_matmul_16x16_16:
            rc = xa_nn_matmul_16x16_16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat2
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                              // rows
                (signed int)(msg->param[5]),                                                              // cols
                (signed int)(msg->param[6]),                                                              // row_stride
                (signed int)(msg->param[7]),                                                              // acc_shift
                (signed int)(msg->param[8]),                                                              // bias_shift
                (signed int)(msg->param[9]),                                                              // vec_count
                (signed int)(msg->param[10]),                                                             // vec_offset
                (signed int)(msg->param[11]),                                                             // out_offset
                (signed int)(msg->param[12])                                                              // out_stride
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x16_16 == 1
        case SRTM_Command_xa_nn_matXvec_8x16_16:
            rc = xa_nn_matXvec_8x16_16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),  // p_mat1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)),  // p_mat2
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_vec1
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)), // p_vec2
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[6]),                                                        // rows
                (signed int)(msg->param[7]),                                                        // cols1
                (signed int)(msg->param[8]),                                                        // cols2
                (signed int)(msg->param[9]),                                                        // row_stride1
                (signed int)(msg->param[10]),                                                       // row_stride2
                (signed int)(msg->param[11]),                                                       // acc_shift
                (signed int)(msg->param[12])                                                        // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x16_32 == 1
        case SRTM_Command_xa_nn_matXvec_8x16_32:
            rc = xa_nn_matXvec_8x16_32(
                (signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),   // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),  // p_mat1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)),  // p_mat2
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_vec1
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)), // p_vec2
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[6]),                                                        // rows
                (signed int)(msg->param[7]),                                                        // cols1
                (signed int)(msg->param[8]),                                                        // cols2
                (signed int)(msg->param[9]),                                                        // row_stride1
                (signed int)(msg->param[10]),                                                       // row_stride2
                (signed int)(msg->param[11]),                                                       // acc_shift
                (signed int)(msg->param[12])                                                        // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x16_64 == 1
        case SRTM_Command_xa_nn_matXvec_8x16_64:
            rc = xa_nn_matXvec_8x16_64(
                (signed long long *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),      // p_mat1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)),      // p_mat2
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),     // p_vec1
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)),     // p_vec2
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)),     // p_bias
                (signed int)(msg->param[6]),                                                            // rows
                (signed int)(msg->param[7]),                                                            // cols1
                (signed int)(msg->param[8]),                                                            // cols2
                (signed int)(msg->param[9]),                                                            // row_stride1
                (signed int)(msg->param[10]),                                                           // row_stride2
                (signed int)(msg->param[11]),                                                           // acc_shift
                (signed int)(msg->param[12])                                                            // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x16_16_tanh == 1
        case SRTM_Command_xa_nn_matXvec_8x16_16_tanh:
            rc = xa_nn_matXvec_8x16_16_tanh(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),  // p_mat1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)),  // p_mat2
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_vec1
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)), // p_vec2
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)),         // p_bias
                (signed int)(msg->param[6]),                                                        // rows
                (signed int)(msg->param[7]),                                                        // cols1
                (signed int)(msg->param[8]),                                                        // cols2
                (signed int)(msg->param[9]),                                                        // row_stride1
                (signed int)(msg->param[10]),                                                       // row_stride2
                (signed int)(msg->param[11]),                                                       // acc_shift
                (signed int)(msg->param[12]),                                                       // bias_shift
                (signed int)(msg->param[13]),                                                       // bias_precision
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[14], kMEMORY_Local2DMA))         // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x16_16_sigmoid == 1
        case SRTM_Command_xa_nn_matXvec_8x16_16_sigmoid:
            rc = xa_nn_matXvec_8x16_16_sigmoid(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),  // p_mat1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)),  // p_mat2
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_vec1
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)), // p_vec2
                (void *)(msg->param[5]),                                                            // p_bias
                (signed int)(msg->param[6]),                                                        // rows
                (signed int)(msg->param[7]),                                                        // cols1
                (signed int)(msg->param[8]),                                                        // cols2
                (signed int)(msg->param[9]),                                                        // row_stride1
                (signed int)(msg->param[10]),                                                       // row_stride2
                (signed int)(msg->param[11]),                                                       // acc_shift
                (signed int)(msg->param[12]),                                                       // bias_shift
                (signed int)(msg->param[13]),                                                       // bias_precision
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[14], kMEMORY_Local2DMA))         // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_batch_8x16_64 == 1
        case SRTM_Command_xa_nn_matXvec_batch_8x16_64:
            rc = xa_nn_matXvec_batch_8x16_64(
                (signed long long **)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),       // p_mat1
                (signed short **)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)),     // p_vec1
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),      // p_bias
                (signed int)(msg->param[4]),                                                             // rows
                (signed int)(msg->param[5]),                                                             // cols1
                (signed int)(msg->param[6]),                                                             // row_stride1
                (signed int)(msg->param[7]),                                                             // acc_shift
                (signed int)(msg->param[8]),                                                             // bias_shift
                (signed int)(msg->param[9])                                                              // vec_count
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matmul_8x16_16 == 1
        case SRTM_Command_xa_nn_matmul_8x16_16:
            rc = xa_nn_matmul_8x16_16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),  // p_mat1
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat2
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                              // rows
                (signed int)(msg->param[5]),                                                              // cols
                (signed int)(msg->param[6]),                                                              // row_stride
                (signed int)(msg->param[7]),                                                              // acc_shift
                (signed int)(msg->param[8]),                                                              // bias_shift
                (signed int)(msg->param[9]),                                                              // vec_count
                (signed int)(msg->param[10]),                                                             // vec_offset
                (signed int)(msg->param[11]),                                                             // out_offset
                (signed int)(msg->param[12])                                                              // out_stride
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x8_8 == 1
        case SRTM_Command_xa_nn_matXvec_8x8_8:
            rc = xa_nn_matXvec_8x8_8(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat2
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_vec1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)), // p_vec2
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[6]),                                                       // rows
                (signed int)(msg->param[7]),                                                       // cols1
                (signed int)(msg->param[8]),                                                       // cols2
                (signed int)(msg->param[9]),                                                       // row_stride1
                (signed int)(msg->param[10]),                                                      // row_stride2
                (signed int)(msg->param[11]),                                                      // acc_shift
                (signed int)(msg->param[12])                                                       // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x8_16 == 1
        case SRTM_Command_xa_nn_matXvec_8x8_16:
            rc = xa_nn_matXvec_8x8_16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),  // p_mat1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)),  // p_mat2
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),  // p_vec1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)),  // p_vec2
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)),  // p_bias
                (signed int)(msg->param[6]),                                                        // rows
                (signed int)(msg->param[7]),                                                        // cols1
                (signed int)(msg->param[8]),                                                        // cols2
                (signed int)(msg->param[9]),                                                        // row_stride1
                (signed int)(msg->param[10]),                                                       // row_stride2
                (signed int)(msg->param[11]),                                                       // acc_shift
                (signed int)(msg->param[12])                                                        // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x8_32 == 1
        case SRTM_Command_xa_nn_matXvec_8x8_32:
            rc = xa_nn_matXvec_8x8_32(
                (signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),  // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat2
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_vec1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)), // p_vec2
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[6]),                                                       // rows
                (signed int)(msg->param[7]),                                                       // cols1
                (signed int)(msg->param[8]),                                                       // cols2
                (signed int)(msg->param[9]),                                                       // row_stride1
                (signed int)(msg->param[10]),                                                      // row_stride2
                (signed int)(msg->param[11]),                                                      // acc_shift
                (signed int)(msg->param[12])                                                       // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x8_8_tanh == 1
        case SRTM_Command_xa_nn_matXvec_8x8_8_tanh:
            rc = xa_nn_matXvec_8x8_8_tanh(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat2
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_vec1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)), // p_vec2
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)),        // p_bias
                (signed int)(msg->param[6]),                                                       // rows
                (signed int)(msg->param[7]),                                                       // cols1
                (signed int)(msg->param[8]),                                                       // cols2
                (signed int)(msg->param[9]),                                                       // row_stride1
                (signed int)(msg->param[10]),                                                      // row_stride2
                (signed int)(msg->param[11]),                                                      // acc_shift
                (signed int)(msg->param[12]),                                                      // bias_shift
                (signed int)(msg->param[13]),                                                      // bias_precision
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[14], kMEMORY_Local2DMA))        // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x8_8_sigmoid == 1
        case SRTM_Command_xa_nn_matXvec_8x8_8_sigmoid:
            rc = xa_nn_matXvec_8x8_8_sigmoid(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat2
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_vec1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)), // p_vec2
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)),        // p_bias
                (signed int)(msg->param[6]),                                                       // rows
                (signed int)(msg->param[7]),                                                       // cols1
                (signed int)(msg->param[8]),                                                       // cols2
                (signed int)(msg->param[9]),                                                       // row_stride1
                (signed int)(msg->param[10]),                                                      // row_stride2
                (signed int)(msg->param[11]),                                                      // acc_shift
                (signed int)(msg->param[12]),                                                      // bias_shift
                (signed int)(msg->param[13]),                                                      // bias_precision
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[14], kMEMORY_Local2DMA))        // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_batch_8x8_32 == 1
        case SRTM_Command_xa_nn_matXvec_batch_8x8_32:
            rc = xa_nn_matXvec_batch_8x8_32(
                (signed int **)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),  // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),  // p_mat1
                (signed char **)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_vec1
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),  // p_bias
                (signed int)(msg->param[4]),                                                        // rows
                (signed int)(msg->param[5]),                                                        // cols1
                (signed int)(msg->param[6]),                                                        // row_stride1
                (signed int)(msg->param[7]),                                                        // acc_shift
                (signed int)(msg->param[8]),                                                        // bias_shift
                (signed int)(msg->param[9])                                                         // vec_count
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matmul_8x8_8 == 1
        case SRTM_Command_xa_nn_matmul_8x8_8:
            rc = xa_nn_matmul_8x8_8(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat2
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                             // rows
                (signed int)(msg->param[5]),                                                             // cols
                (signed int)(msg->param[6]),                                                             // row_stride
                (signed int)(msg->param[7]),                                                             // acc_shift
                (signed int)(msg->param[8]),                                                             // bias_shift
                (signed int)(msg->param[9]),                                                             // vec_count
                (signed int)(msg->param[10]),                                                            // vec_offset
                (signed int)(msg->param[11]),                                                            // out_offset
                (signed int)(msg->param[12])                                                             // out_stride
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_f32xf32_f32_sigmoid == 1
        case SRTM_Command_xa_nn_matXvec_f32xf32_f32_sigmoid:
            rc = xa_nn_matXvec_f32xf32_f32_sigmoid(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat2
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_vec1
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)), // p_vec2
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[6]),                                                 // rows
                (signed int)(msg->param[7]),                                                 // cols1
                (signed int)(msg->param[8]),                                                 // cols2
                (signed int)(msg->param[9]),                                                 // row_stride1
                (signed int)(msg->param[10]),                                                // row_stride2
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[11], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_f32xf32_f32_tanh == 1
        case SRTM_Command_xa_nn_matXvec_f32xf32_f32_tanh:
            rc = xa_nn_matXvec_f32xf32_f32_tanh(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat2
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_vec1
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)), // p_vec2
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[6]),                                                 // rows
                (signed int)(msg->param[7]),                                                 // cols1
                (signed int)(msg->param[8]),                                                 // cols2
                (signed int)(msg->param[9]),                                                 // row_stride1
                (signed int)(msg->param[10]),                                                // row_stride2
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[11], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_matXvec_f32xf32_f32:
            rc = xa_nn_matXvec_f32xf32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat2
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_vec1
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)), // p_vec2
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[6]),                                                       // rows
                (signed int)(msg->param[7]),                                                       // cols1
                (signed int)(msg->param[8]),                                                       // cols2
                (signed int)(msg->param[9]),                                                       // row_stride1
                (signed int)(msg->param[10])                                                       // row_stride2
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_batch_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_matXvec_batch_f32xf32_f32:
            rc = xa_nn_matXvec_batch_f32xf32_f32(
                (float **)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),  // p_mat1
                (float **)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_vec1
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),  // p_bias
                (signed int)(msg->param[4]),                                                  // rows
                (signed int)(msg->param[5]),                                                  // cols1
                (signed int)(msg->param[6]),                                                  // row_stride1
                (signed int)(msg->param[7])                                                   // vec_count
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matmul_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_matmul_f32xf32_f32:
            rc = xa_nn_matmul_f32xf32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat2
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                       // rows
                (signed int)(msg->param[5]),                                                       // cols
                (signed int)(msg->param[6]),                                                       // row_stride
                (signed int)(msg->param[7]),                                                       // vec_count
                (signed int)(msg->param[8]),                                                       // vec_offset
                (signed int)(msg->param[9]),                                                       // out_offset
                (signed int)(msg->param[10])                                                       // out_stride
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_asym8uxasym8u_asym8u == 1
        case SRTM_Command_xa_nn_matXvec_asym8uxasym8u_asym8u:
            rc = xa_nn_matXvec_asym8uxasym8u_asym8u(
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat2
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_vec1
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)), // p_vec2
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)),    // p_bias
                (signed int)(msg->param[6]),                                                               // rows
                (signed int)(msg->param[7]),                                                               // cols1
                (signed int)(msg->param[8]),                                                               // cols2
                (signed int)(msg->param[9]),  // row_stride1
                (signed int)(msg->param[10]), // row_stride2
                (signed int)(msg->param[11]), // mat1_zero_bias
                (signed int)(msg->param[12]), // mat2_zero_bias
                (signed int)(msg->param[13]), // vec1_zero_bias
                (signed int)(msg->param[14]), // vec2_zero_bias
                (signed int)(msg->param[15]), // out_multiplier
                (signed int)(msg->param[16]), // out_shift
                (signed int)(msg->param[17])  // out_zero_bias
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_sym8sxasym8s_asym8s == 1
        case SRTM_Command_xa_nn_matXvec_sym8sxasym8s_asym8s:
            rc = xa_nn_matXvec_sym8sxasym8s_asym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat2
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_vec1
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)), // p_vec2
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)),  // p_bias
                (signed int)(msg->param[6]),                                                             // rows
                (signed int)(msg->param[7]),                                                             // cols1
                (signed int)(msg->param[8]),                                                             // cols2
                (signed int)(msg->param[9]),                                                             // row_stride1
                (signed int)(msg->param[10]),                                                            // row_stride2
                (signed int)(msg->param[11]), // vec1_zero_bias
                (signed int)(msg->param[12]), // vec2_zero_bias
                (signed int)(msg->param[13]), // out_multiplier
                (signed int)(msg->param[14]), // out_shift
                (signed int)(msg->param[15])  // out_zero_bias
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_out_stride_sym8sxasym8s_16 == 1
        case SRTM_Command_xa_nn_matXvec_out_stride_sym8sxasym8s_16:
            rc = xa_nn_matXvec_out_stride_sym8sxasym8s_16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),      // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_vec1
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),  // p_bias
                (signed int)(msg->param[4]),                                                             // rows
                (signed int)(msg->param[5]),                                                             // cols1
                (signed int)(msg->param[6]),                                                             // row_stride1
                (signed int)(msg->param[7]),                                                             // out_stride
                (signed int)(msg->param[8]), // vec1_zero_bias
                (signed int)(msg->param[9]), // out_multiplier
                (signed int)(msg->param[10]) // out_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_sigmoid_32_32 == 1
        case SRTM_Command_xa_nn_vec_sigmoid_32_32:
            rc = xa_nn_vec_sigmoid_32_32(
                (signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                             // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_tanh_32_32 == 1
        case SRTM_Command_xa_nn_vec_tanh_32_32:
            rc = xa_nn_vec_tanh_32_32(
                (signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                             // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu_std_32_32 == 1
        case SRTM_Command_xa_nn_vec_relu_std_32_32:
            rc = xa_nn_vec_relu_std_32_32(
                (signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                             // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu_32_32 == 1
        case SRTM_Command_xa_nn_vec_relu_32_32:
            rc = xa_nn_vec_relu_32_32(
                (signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2]),                                                            // threshold
                (signed int)(msg->param[3])                                                             // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu1_32_32 == 1
        case SRTM_Command_xa_nn_vec_relu1_32_32:
            rc = xa_nn_vec_relu1_32_32(
                (signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                             // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu6_32_32 == 1
        case SRTM_Command_xa_nn_vec_relu6_32_32:
            rc = xa_nn_vec_relu6_32_32(
                (signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                             // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_softmax_32_32 == 1
        case SRTM_Command_xa_nn_vec_softmax_32_32:
            rc = xa_nn_vec_softmax_32_32(
                (signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                             // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_sigmoid_f32_f32 == 1
        case SRTM_Command_xa_nn_vec_sigmoid_f32_f32:
            rc = xa_nn_vec_sigmoid_f32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                        // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_tanh_f32_f32 == 1
        case SRTM_Command_xa_nn_vec_tanh_f32_f32:
            rc = xa_nn_vec_tanh_f32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                        // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu_f32_f32 == 1
        case SRTM_Command_xa_nn_vec_relu_f32_f32:
            rc = xa_nn_vec_relu_f32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (float)(msg->param[2]),                                                            // threshold
                (signed int)(msg->param[3])                                                        // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu_std_f32_f32 == 1
        case SRTM_Command_xa_nn_vec_relu_std_f32_f32:
            rc = xa_nn_vec_relu_std_f32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                        // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu1_f32_f32 == 1
        case SRTM_Command_xa_nn_vec_relu1_f32_f32:
            rc = xa_nn_vec_relu1_f32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                        // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu6_f32_f32 == 1
        case SRTM_Command_xa_nn_vec_relu6_f32_f32:
            rc = xa_nn_vec_relu6_f32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                        // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_softmax_f32_f32 == 1
        case SRTM_Command_xa_nn_vec_softmax_f32_f32:
            rc = xa_nn_vec_softmax_f32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                        // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_sigmoid_32_16 == 1
        case SRTM_Command_xa_nn_vec_sigmoid_32_16:
            rc = xa_nn_vec_sigmoid_32_16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),     // p_out
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                             // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_tanh_32_16 == 1
        case SRTM_Command_xa_nn_vec_tanh_32_16:
            rc = xa_nn_vec_tanh_32_16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),     // p_out
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                             // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_sigmoid_32_8 == 1
        case SRTM_Command_xa_nn_vec_sigmoid_32_8:
            rc = xa_nn_vec_sigmoid_32_8(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),      // p_out
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                             // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_tanh_32_8 == 1
        case SRTM_Command_xa_nn_vec_tanh_32_8:
            rc = xa_nn_vec_tanh_32_8(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),      // p_out
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                             // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu_16_16 == 1
        case SRTM_Command_xa_nn_vec_relu_16_16:
            rc = xa_nn_vec_relu_16_16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed short)(msg->param[2]),                                                            // threshold
                (signed int)(msg->param[3])                                                               // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu_std_16_16 == 1
        case SRTM_Command_xa_nn_vec_relu_std_16_16:
            rc = xa_nn_vec_relu_std_16_16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                               // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu_8_8 == 1
        case SRTM_Command_xa_nn_vec_relu_8_8:
            rc = xa_nn_vec_relu_8_8(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed char)(msg->param[2]),                                                            // threshold
                (signed int)(msg->param[3])                                                              // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu_std_8_8 == 1
        case SRTM_Command_xa_nn_vec_relu_std_8_8:
            rc = xa_nn_vec_relu_std_8_8(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2])                                                              // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_interpolation_q15 == 1
        case SRTM_Command_xa_nn_vec_interpolation_q15:
            rc = xa_nn_vec_interpolation_q15(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_ifact
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp1
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[4]) // num_elements
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv1d_std_getsize == 1
        case SRTM_Command_xa_nn_conv1d_std_getsize:
            rc = xa_nn_conv1d_std_getsize((signed int)(msg->param[0]), // kernel_height
                                          (signed int)(msg->param[1]), // input_width
                                          (signed int)(msg->param[2]), // input_channels
                                          (signed int)(msg->param[3])  // input_precision
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv1d_std_8x16 == 1
        case SRTM_Command_xa_nn_conv1d_std_8x16:
            rc = xa_nn_conv1d_std_8x16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)),  // p_kernel
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                        // input_height
                (signed int)(msg->param[5]),                                                        // input_width
                (signed int)(msg->param[6]),                                                        // input_channels
                (signed int)(msg->param[7]),                                                        // kernel_height
                (signed int)(msg->param[8]),                                                        // out_channels
                (signed int)(msg->param[9]),                                                        // y_stride
                (signed int)(msg->param[10]),                                                       // y_padding
                (signed int)(msg->param[11]),                                                       // out_height
                (signed int)(msg->param[12]),                                                       // bias_shift
                (signed int)(msg->param[13]),                                                       // acc_shift
                (signed int)(msg->param[14]),                                                       // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[15], kMEMORY_Local2DMA))         // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv1d_std_8x8 == 1
        case SRTM_Command_xa_nn_conv1d_std_8x8:
            rc = xa_nn_conv1d_std_8x8(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_kernel
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                       // input_height
                (signed int)(msg->param[5]),                                                       // input_width
                (signed int)(msg->param[6]),                                                       // input_channels
                (signed int)(msg->param[7]),                                                       // kernel_height
                (signed int)(msg->param[8]),                                                       // out_channels
                (signed int)(msg->param[9]),                                                       // y_stride
                (signed int)(msg->param[10]),                                                      // y_padding
                (signed int)(msg->param[11]),                                                      // out_height
                (signed int)(msg->param[12]),                                                      // bias_shift
                (signed int)(msg->param[13]),                                                      // acc_shift
                (signed int)(msg->param[14]),                                                      // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[15], kMEMORY_Local2DMA))        // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv1d_std_16x16 == 1
        case SRTM_Command_xa_nn_conv1d_std_16x16:
            rc = xa_nn_conv1d_std_16x16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_kernel
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                        // input_height
                (signed int)(msg->param[5]),                                                        // input_width
                (signed int)(msg->param[6]),                                                        // input_channels
                (signed int)(msg->param[7]),                                                        // kernel_height
                (signed int)(msg->param[8]),                                                        // out_channels
                (signed int)(msg->param[9]),                                                        // y_stride
                (signed int)(msg->param[10]),                                                       // y_padding
                (signed int)(msg->param[11]),                                                       // out_height
                (signed int)(msg->param[12]),                                                       // bias_shift
                (signed int)(msg->param[13]),                                                       // acc_shift
                (signed int)(msg->param[14]),                                                       // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[15], kMEMORY_Local2DMA))         // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv1d_std_f32 == 1
        case SRTM_Command_xa_nn_conv1d_std_f32:
            rc = xa_nn_conv1d_std_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_kernel
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                 // input_height
                (signed int)(msg->param[5]),                                                 // input_width
                (signed int)(msg->param[6]),                                                 // input_channels
                (signed int)(msg->param[7]),                                                 // kernel_height
                (signed int)(msg->param[8]),                                                 // out_channels
                (signed int)(msg->param[9]),                                                 // y_stride
                (signed int)(msg->param[10]),                                                // y_padding
                (signed int)(msg->param[11]),                                                // out_height
                (signed int)(msg->param[12]),                                                // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[13], kMEMORY_Local2DMA))  // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_std_getsize == 1
        case SRTM_Command_xa_nn_conv2d_std_getsize:
            rc = xa_nn_conv2d_std_getsize((signed int)(msg->param[0]), // input_height
                                          (signed int)(msg->param[1]), // input_channels
                                          (signed int)(msg->param[2]), // kernel_height
                                          (signed int)(msg->param[3]), // kernel_width
                                          (signed int)(msg->param[4]), // y_stride
                                          (signed int)(msg->param[5]), // y_padding
                                          (signed int)(msg->param[6]), // out_height
                                          (signed int)(msg->param[7]), // out_channels
                                          (signed int)(msg->param[8])  // input_precision
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_std_8x16 == 1
        case SRTM_Command_xa_nn_conv2d_std_8x16:

#ifndef IAN_TEST
            rc = xa_nn_conv2d_std_8x16(
                (signed short *)MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA), // p_out
                (signed short *)MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA), // p_inp
                (signed char *)MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA),  // p_kernel
                (signed short *)MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA), // p_bias
                (signed int)(msg->param[4]),                                                      // input_height
                (signed int)(msg->param[5]),                                                      // input_width
                (signed int)(msg->param[6]),                                                      // input_channels
                (signed int)(msg->param[7]),                                                      // kernel_height
                (signed int)(msg->param[8]),                                                      // kernel_width
                (signed int)(msg->param[9]),                                                      // out_channels
                (signed int)(msg->param[10]),                                                     // x_stride
                (signed int)(msg->param[11]),                                                     // y_stride
                (signed int)(msg->param[12]),                                                     // x_padding
                (signed int)(msg->param[13]),                                                     // y_padding
                (signed int)(msg->param[14]),                                                     // out_height
                (signed int)(msg->param[15]),                                                     // out_width
                (signed int)(msg->param[16]),                                                     // bias_shift
                (signed int)(msg->param[17]),                                                     // acc_shift
                (signed int)(msg->param[18]),                                                     // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[19], kMEMORY_Local2DMA))       // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#else
            rc = xa_nn_conv2d_std_8x16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)),  // p_kernel
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                        // input_height
                (signed int)(msg->param[5]),                                                        // input_width
                (signed int)(msg->param[6]),                                                        // input_channels
                (signed int)(msg->param[7]),                                                        // kernel_height
                (signed int)(msg->param[8]),                                                        // kernel_width
                (signed int)(msg->param[9]),                                                        // out_channels
                (signed int)(msg->param[10]),                                                       // x_stride
                (signed int)(msg->param[11]),                                                       // y_stride
                (signed int)(msg->param[12]),                                                       // x_padding
                (signed int)(msg->param[13]),                                                       // y_padding
                (signed int)(msg->param[14]),                                                       // out_height
                (signed int)(msg->param[15]),                                                       // out_width
                (signed int)(msg->param[16]),                                                       // bias_shift
                (signed int)(msg->param[17]),                                                       // acc_shift
                (signed int)(msg->param[18]),                                                       // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[19], kMEMORY_Local2DMA))         // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif
#endif

#if NN_ENABLE_xa_nn_conv2d_std_8x8 == 1
        case SRTM_Command_xa_nn_conv2d_std_8x8:
            rc = xa_nn_conv2d_std_8x8(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_kernel
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                       // input_height
                (signed int)(msg->param[5]),                                                       // input_width
                (signed int)(msg->param[6]),                                                       // input_channels
                (signed int)(msg->param[7]),                                                       // kernel_height
                (signed int)(msg->param[8]),                                                       // kernel_width
                (signed int)(msg->param[9]),                                                       // out_channels
                (signed int)(msg->param[10]),                                                      // x_stride
                (signed int)(msg->param[11]),                                                      // y_stride
                (signed int)(msg->param[12]),                                                      // x_padding
                (signed int)(msg->param[13]),                                                      // y_padding
                (signed int)(msg->param[14]),                                                      // out_height
                (signed int)(msg->param[15]),                                                      // out_width
                (signed int)(msg->param[16]),                                                      // bias_shift
                (signed int)(msg->param[17]),                                                      // acc_shift
                (signed int)(msg->param[18]),                                                      // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[19], kMEMORY_Local2DMA))        // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_std_16x16 == 1
        case SRTM_Command_xa_nn_conv2d_std_16x16:
            rc = xa_nn_conv2d_std_16x16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_kernel
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                        // input_height
                (signed int)(msg->param[5]),                                                        // input_width
                (signed int)(msg->param[6]),                                                        // input_channels
                (signed int)(msg->param[7]),                                                        // kernel_height
                (signed int)(msg->param[8]),                                                        // kernel_width
                (signed int)(msg->param[9]),                                                        // out_channels
                (signed int)(msg->param[10]),                                                       // x_stride
                (signed int)(msg->param[11]),                                                       // y_stride
                (signed int)(msg->param[12]),                                                       // x_padding
                (signed int)(msg->param[13]),                                                       // y_padding
                (signed int)(msg->param[14]),                                                       // out_height
                (signed int)(msg->param[15]),                                                       // out_width
                (signed int)(msg->param[16]),                                                       // bias_shift
                (signed int)(msg->param[17]),                                                       // acc_shift
                (signed int)(msg->param[18]),                                                       // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[19], kMEMORY_Local2DMA))         // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_std_f32 == 1
        case SRTM_Command_xa_nn_conv2d_std_f32:
            rc = xa_nn_conv2d_std_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_kernel
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                       // input_height
                (signed int)(msg->param[5]),                                                       // input_width
                (signed int)(msg->param[6]),                                                       // input_channels
                (signed int)(msg->param[7]),                                                       // kernel_height
                (signed int)(msg->param[8]),                                                       // kernel_width
                (signed int)(msg->param[9]),                                                       // out_channels
                (signed int)(msg->param[10]),                                                      // x_stride
                (signed int)(msg->param[11]),                                                      // y_stride
                (signed int)(msg->param[12]),                                                      // x_padding
                (signed int)(msg->param[13]),                                                      // y_padding
                (signed int)(msg->param[14]),                                                      // out_height
                (signed int)(msg->param[15]),                                                      // out_width
                (signed int)(msg->param[16]),                                                      // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[17], kMEMORY_Local2DMA))        // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_pointwise_f32 == 1
        case SRTM_Command_xa_nn_conv2d_pointwise_f32:
            rc = xa_nn_conv2d_pointwise_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_kernel
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                 // input_height
                (signed int)(msg->param[5]),                                                 // input_width
                (signed int)(msg->param[6]),                                                 // input_channels
                (signed int)(msg->param[7]),                                                 // out_channels
                (signed int)(msg->param[8])                                                  // out_data_format
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_pointwise_8x16 == 1
        case SRTM_Command_xa_nn_conv2d_pointwise_8x16:
            rc = xa_nn_conv2d_pointwise_8x16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),  // p_kernel
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                        // input_height
                (signed int)(msg->param[5]),                                                        // input_width
                (signed int)(msg->param[6]),                                                        // input_channels
                (signed int)(msg->param[7]),                                                        // out_channels
                (signed int)(msg->param[8]),                                                        // acc_shift
                (signed int)(msg->param[9]),                                                        // bias_shift
                (signed int)(msg->param[10])                                                        // out_data_format
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_pointwise_8x8 == 1
        case SRTM_Command_xa_nn_conv2d_pointwise_8x8:
            rc = xa_nn_conv2d_pointwise_8x8(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_kernel
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                       // input_height
                (signed int)(msg->param[5]),                                                       // input_width
                (signed int)(msg->param[6]),                                                       // input_channels
                (signed int)(msg->param[7]),                                                       // out_channels
                (signed int)(msg->param[8]),                                                       // acc_shift
                (signed int)(msg->param[9]),                                                       // bias_shift
                (signed int)(msg->param[10])                                                       // out_data_format
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_depthwise_getsize == 1
        case SRTM_Command_xa_nn_conv2d_depthwise_getsize:
            rc = xa_nn_conv2d_depthwise_getsize((signed int)(msg->param[0]),  // input_height
                                                (signed int)(msg->param[1]),  // input_width
                                                (signed int)(msg->param[2]),  // input_channels
                                                (signed int)(msg->param[3]),  // kernel_height
                                                (signed int)(msg->param[4]),  // kernel_width
                                                (signed int)(msg->param[5]),  // channels_multiplier
                                                (signed int)(msg->param[6]),  // x_stride
                                                (signed int)(msg->param[7]),  // y_stride
                                                (signed int)(msg->param[8]),  // x_padding
                                                (signed int)(msg->param[9]),  // y_padding
                                                (signed int)(msg->param[10]), // output_height
                                                (signed int)(msg->param[11]), // output_width
                                                (signed int)(msg->param[12]), // circ_buf_precision
                                                (signed int)(msg->param[13])  // inp_data_format
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_depthwise_8x8 == 1
        case SRTM_Command_xa_nn_conv2d_depthwise_8x8:
            rc = xa_nn_conv2d_depthwise_8x8(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_kernel
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                             // input_height
                (signed int)(msg->param[5]),                                                             // input_width
                (signed int)(msg->param[6]),                                                // input_channels
                (signed int)(msg->param[7]),                                                // kernel_height
                (signed int)(msg->param[8]),                                                // kernel_width
                (signed int)(msg->param[9]),                                                // channels_multiplier
                (signed int)(msg->param[10]),                                               // x_stride
                (signed int)(msg->param[11]),                                               // y_stride
                (signed int)(msg->param[12]),                                               // x_padding
                (signed int)(msg->param[13]),                                               // y_padding
                (signed int)(msg->param[14]),                                               // out_height
                (signed int)(msg->param[15]),                                               // out_width
                (signed int)(msg->param[16]),                                               // acc_shift
                (signed int)(msg->param[17]),                                               // bias_shift
                (signed int)(msg->param[18]),                                               // inp_data_format
                (signed int)(msg->param[19]),                                               // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[20], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_depthwise_f32 == 1
        case SRTM_Command_xa_nn_conv2d_depthwise_f32:
            rc = xa_nn_conv2d_depthwise_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_kernel
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                       // input_height
                (signed int)(msg->param[5]),                                                       // input_width
                (signed int)(msg->param[6]),                                                       // input_channels
                (signed int)(msg->param[7]),                                                       // kernel_height
                (signed int)(msg->param[8]),                                                       // kernel_width
                (signed int)(msg->param[9]),                                                // channels_multiplier
                (signed int)(msg->param[10]),                                               // x_stride
                (signed int)(msg->param[11]),                                               // y_stride
                (signed int)(msg->param[12]),                                               // x_padding
                (signed int)(msg->param[13]),                                               // y_padding
                (signed int)(msg->param[14]),                                               // out_height
                (signed int)(msg->param[15]),                                               // out_width
                (signed int)(msg->param[16]),                                               // inp_data_format
                (signed int)(msg->param[17]),                                               // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[18], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_depthwise_8x16 == 1
        case SRTM_Command_xa_nn_conv2d_depthwise_8x16:
            rc = xa_nn_conv2d_depthwise_8x16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),  // p_kernel
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                // input_height
                (signed int)(msg->param[5]),                                                // input_width
                (signed int)(msg->param[6]),                                                // input_channels
                (signed int)(msg->param[7]),                                                // kernel_height
                (signed int)(msg->param[8]),                                                // kernel_width
                (signed int)(msg->param[9]),                                                // channels_multiplier
                (signed int)(msg->param[10]),                                               // x_stride
                (signed int)(msg->param[11]),                                               // y_stride
                (signed int)(msg->param[12]),                                               // x_padding
                (signed int)(msg->param[13]),                                               // y_padding
                (signed int)(msg->param[14]),                                               // out_height
                (signed int)(msg->param[15]),                                               // out_width
                (signed int)(msg->param[16]),                                               // acc_shift
                (signed int)(msg->param[17]),                                               // bias_shift
                (signed int)(msg->param[18]),                                               // inp_data_format
                (signed int)(msg->param[19]),                                               // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[20], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_depthwise_16x16 == 1
        case SRTM_Command_xa_nn_conv2d_depthwise_16x16:
            rc = xa_nn_conv2d_depthwise_16x16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_kernel
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                // input_height
                (signed int)(msg->param[5]),                                                // input_width
                (signed int)(msg->param[6]),                                                // input_channels
                (signed int)(msg->param[7]),                                                // kernel_height
                (signed int)(msg->param[8]),                                                // kernel_width
                (signed int)(msg->param[9]),                                                // channels_multiplier
                (signed int)(msg->param[10]),                                               // x_stride
                (signed int)(msg->param[11]),                                               // y_stride
                (signed int)(msg->param[12]),                                               // x_padding
                (signed int)(msg->param[13]),                                               // y_padding
                (signed int)(msg->param[14]),                                               // out_height
                (signed int)(msg->param[15]),                                               // out_width
                (signed int)(msg->param[16]),                                               // acc_shift
                (signed int)(msg->param[17]),                                               // bias_shift
                (signed int)(msg->param[18]),                                               // inp_data_format
                (signed int)(msg->param[19]),                                               // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[20], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_pointwise_16x16 == 1
        case SRTM_Command_xa_nn_conv2d_pointwise_16x16:
            rc = xa_nn_conv2d_pointwise_16x16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_kernel
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                        // input_height
                (signed int)(msg->param[5]),                                                        // input_width
                (signed int)(msg->param[6]),                                                        // input_channels
                (signed int)(msg->param[7]),                                                        // out_channels
                (signed int)(msg->param[8]),                                                        // acc_shift
                (signed int)(msg->param[9]),                                                        // bias_shift
                (signed int)(msg->param[10])                                                        // out_data_format
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_avgpool_8 == 1
        case SRTM_Command_xa_nn_avgpool_8:
            rc = xa_nn_avgpool_8(
                (signed char *)MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA),       // p_out
                (const signed char *)MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA), // p_inp
                (signed int)(msg->param[2]),                                                           // input_height
                (signed int)(msg->param[3]),                                                           // input_width
                (signed int)(msg->param[4]),                                                           // input_channels
                (signed int)(msg->param[5]),                                                           // kernel_height
                (signed int)(msg->param[6]),                                                           // kernel_width
                (signed int)(msg->param[7]),                                                           // x_stride
                (signed int)(msg->param[8]),                                                           // y_stride
                (signed int)(msg->param[9]),                                                           // x_padding
                (signed int)(msg->param[10]),                                                          // y_padding
                (signed int)(msg->param[11]),                                                          // out_height
                (signed int)(msg->param[12]),                                                          // out_width
                (signed int)(msg->param[13]),                                               // inp_data_format
                (signed int)(msg->param[14]),                                               // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[15], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_avgpool_16 == 1
        case SRTM_Command_xa_nn_avgpool_16:
            rc = xa_nn_avgpool_16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed int)(msg->param[2]),                                                // input_height
                (signed int)(msg->param[3]),                                                // input_width
                (signed int)(msg->param[4]),                                                // input_channels
                (signed int)(msg->param[5]),                                                // kernel_height
                (signed int)(msg->param[6]),                                                // kernel_width
                (signed int)(msg->param[7]),                                                // x_stride
                (signed int)(msg->param[8]),                                                // y_stride
                (signed int)(msg->param[9]),                                                // x_padding
                (signed int)(msg->param[10]),                                               // y_padding
                (signed int)(msg->param[11]),                                               // out_height
                (signed int)(msg->param[12]),                                               // out_width
                (signed int)(msg->param[13]),                                               // inp_data_format
                (signed int)(msg->param[14]),                                               // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[15], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_avgpool_f32 == 1
        case SRTM_Command_xa_nn_avgpool_f32:
            rc = xa_nn_avgpool_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed int)(msg->param[2]),                                                       // input_height
                (signed int)(msg->param[3]),                                                       // input_width
                (signed int)(msg->param[4]),                                                       // input_channels
                (signed int)(msg->param[5]),                                                       // kernel_height
                (signed int)(msg->param[6]),                                                       // kernel_width
                (signed int)(msg->param[7]),                                                       // x_stride
                (signed int)(msg->param[8]),                                                       // y_stride
                (signed int)(msg->param[9]),                                                       // x_padding
                (signed int)(msg->param[10]),                                                      // y_padding
                (signed int)(msg->param[11]),                                                      // out_height
                (signed int)(msg->param[12]),                                                      // out_width
                (signed int)(msg->param[13]),                                                      // inp_data_format
                (signed int)(msg->param[14]),                                                      // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[15], kMEMORY_Local2DMA))        // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_avgpool_asym8u == 1
        case SRTM_Command_xa_nn_avgpool_asym8u:
            rc = xa_nn_avgpool_asym8u(
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed int)(msg->param[2]),                                                // input_height
                (signed int)(msg->param[3]),                                                // input_width
                (signed int)(msg->param[4]),                                                // input_channels
                (signed int)(msg->param[5]),                                                // kernel_height
                (signed int)(msg->param[6]),                                                // kernel_width
                (signed int)(msg->param[7]),                                                // x_stride
                (signed int)(msg->param[8]),                                                // y_stride
                (signed int)(msg->param[9]),                                                // x_padding
                (signed int)(msg->param[10]),                                               // y_padding
                (signed int)(msg->param[11]),                                               // out_height
                (signed int)(msg->param[12]),                                               // out_width
                (signed int)(msg->param[13]),                                               // inp_data_format
                (signed int)(msg->param[14]),                                               // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[15], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_avgpool_getsize == 1
        case SRTM_Command_xa_nn_avgpool_getsize:
            rc = xa_nn_avgpool_getsize((signed int)(msg->param[0]),  // input_channels
                                       (signed int)(msg->param[1]),  // inp_precision
                                       (signed int)(msg->param[2]),  // out_precision
                                       (signed int)(msg->param[3]),  // input_height
                                       (signed int)(msg->param[4]),  // input_width
                                       (signed int)(msg->param[5]),  // kernel_height
                                       (signed int)(msg->param[6]),  // kernel_width
                                       (signed int)(msg->param[7]),  // x_stride
                                       (signed int)(msg->param[8]),  // y_stride
                                       (signed int)(msg->param[9]),  // x_padding
                                       (signed int)(msg->param[10]), // y_padding
                                       (signed int)(msg->param[11]), // out_height
                                       (signed int)(msg->param[12]), // out_width
                                       (signed int)(msg->param[13]), // inp_data_format
                                       (signed int)(msg->param[14])  // out_data_format
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_maxpool_8 == 1
        case SRTM_Command_xa_nn_maxpool_8:
            rc = xa_nn_maxpool_8(
                (signed char *)MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA),       // p_out
                (const signed char *)MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA), // p_inp
                (signed int)(msg->param[2]),                                                           // input_height
                (signed int)(msg->param[3]),                                                           // input_width
                (signed int)(msg->param[4]),                                                           // input_channels
                (signed int)(msg->param[5]),                                                           // kernel_height
                (signed int)(msg->param[6]),                                                           // kernel_width
                (signed int)(msg->param[7]),                                                           // x_stride
                (signed int)(msg->param[8]),                                                           // y_stride
                (signed int)(msg->param[9]),                                                           // x_padding
                (signed int)(msg->param[10]),                                                          // y_padding
                (signed int)(msg->param[11]),                                                          // out_height
                (signed int)(msg->param[12]),                                                          // out_width
                (signed int)(msg->param[13]),                                             // inp_data_format
                (signed int)(msg->param[14]),                                             // out_data_format
                (void *)MEMORY_ConvertMemoryMapAddress(msg->param[15], kMEMORY_Local2DMA) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_maxpool_16 == 1
        case SRTM_Command_xa_nn_maxpool_16:
            rc = xa_nn_maxpool_16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed int)(msg->param[2]),                                                // input_height
                (signed int)(msg->param[3]),                                                // input_width
                (signed int)(msg->param[4]),                                                // input_channels
                (signed int)(msg->param[5]),                                                // kernel_height
                (signed int)(msg->param[6]),                                                // kernel_width
                (signed int)(msg->param[7]),                                                // x_stride
                (signed int)(msg->param[8]),                                                // y_stride
                (signed int)(msg->param[9]),                                                // x_padding
                (signed int)(msg->param[10]),                                               // y_padding
                (signed int)(msg->param[11]),                                               // out_height
                (signed int)(msg->param[12]),                                               // out_width
                (signed int)(msg->param[13]),                                               // inp_data_format
                (signed int)(msg->param[14]),                                               // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[15], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_maxpool_f32 == 1
        case SRTM_Command_xa_nn_maxpool_f32:
            rc = xa_nn_maxpool_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed int)(msg->param[2]),                                                       // input_height
                (signed int)(msg->param[3]),                                                       // input_width
                (signed int)(msg->param[4]),                                                       // input_channels
                (signed int)(msg->param[5]),                                                       // kernel_height
                (signed int)(msg->param[6]),                                                       // kernel_width
                (signed int)(msg->param[7]),                                                       // x_stride
                (signed int)(msg->param[8]),                                                       // y_stride
                (signed int)(msg->param[9]),                                                       // x_padding
                (signed int)(msg->param[10]),                                                      // y_padding
                (signed int)(msg->param[11]),                                                      // out_height
                (signed int)(msg->param[12]),                                                      // out_width
                (signed int)(msg->param[13]),                                                      // inp_data_format
                (signed int)(msg->param[14]),                                                      // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[15], kMEMORY_Local2DMA))        // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_maxpool_asym8u == 1
        case SRTM_Command_xa_nn_maxpool_asym8u:
            rc = xa_nn_maxpool_asym8u(
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed int)(msg->param[2]),                                                // input_height
                (signed int)(msg->param[3]),                                                // input_width
                (signed int)(msg->param[4]),                                                // input_channels
                (signed int)(msg->param[5]),                                                // kernel_height
                (signed int)(msg->param[6]),                                                // kernel_width
                (signed int)(msg->param[7]),                                                // x_stride
                (signed int)(msg->param[8]),                                                // y_stride
                (signed int)(msg->param[9]),                                                // x_padding
                (signed int)(msg->param[10]),                                               // y_padding
                (signed int)(msg->param[11]),                                               // out_height
                (signed int)(msg->param[12]),                                               // out_width
                (signed int)(msg->param[13]),                                               // inp_data_format
                (signed int)(msg->param[14]),                                               // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[15], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_maxpool_getsize == 1
        case SRTM_Command_xa_nn_maxpool_getsize:
            rc = xa_nn_maxpool_getsize((signed int)(msg->param[0]),  // input_channels
                                       (signed int)(msg->param[1]),  // inp_precision
                                       (signed int)(msg->param[2]),  // out_precision
                                       (signed int)(msg->param[3]),  // input_height
                                       (signed int)(msg->param[4]),  // input_width
                                       (signed int)(msg->param[5]),  // kernel_height
                                       (signed int)(msg->param[6]),  // kernel_width
                                       (signed int)(msg->param[7]),  // x_stride
                                       (signed int)(msg->param[8]),  // y_stride
                                       (signed int)(msg->param[9]),  // x_padding
                                       (signed int)(msg->param[10]), // y_padding
                                       (signed int)(msg->param[11]), // out_height
                                       (signed int)(msg->param[12]), // out_width
                                       (signed int)(msg->param[13]), // inp_data_format
                                       (signed int)(msg->param[14])  // out_data_format
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_fully_connected_f32 == 1
        case SRTM_Command_xa_nn_fully_connected_f32:
            rc = xa_nn_fully_connected_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_weight
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                       // weight_depth
                (signed int)(msg->param[5])                                                        // out_depth
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_fully_connected_16x16_16 == 1
        case SRTM_Command_xa_nn_fully_connected_16x16_16:
            rc = xa_nn_fully_connected_16x16_16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_weight
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                        // weight_depth
                (signed int)(msg->param[5]),                                                        // out_depth
                (signed int)(msg->param[6]),                                                        // acc_shift
                (signed int)(msg->param[7])                                                         // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_fully_connected_8x16_16 == 1
        case SRTM_Command_xa_nn_fully_connected_8x16_16:
            rc = xa_nn_fully_connected_8x16_16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),  // p_weight
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                        // weight_depth
                (signed int)(msg->param[5]),                                                        // out_depth
                (signed int)(msg->param[6]),                                                        // acc_shift
                (signed int)(msg->param[7])                                                         // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_fully_connected_8x8_8 == 1
        case SRTM_Command_xa_nn_fully_connected_8x8_8:
            rc = xa_nn_fully_connected_8x8_8(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_weight
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_bias
                (signed int)(msg->param[4]),                                                       // weight_depth
                (signed int)(msg->param[5]),                                                       // out_depth
                (signed int)(msg->param[6]),                                                       // acc_shift
                (signed int)(msg->param[7])                                                        // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_fully_connected_asym8uxasym8u_asym8u == 1
        case SRTM_Command_xa_nn_fully_connected_asym8uxasym8u_asym8u:
            rc = xa_nn_fully_connected_asym8uxasym8u_asym8u(
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_weight
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),    // p_bias
                (signed int)(msg->param[4]), // weight_depth
                (signed int)(msg->param[5]), // out_depth
                (signed int)(msg->param[6]), // input_zero_bias
                (signed int)(msg->param[7]), // weight_zero_bias
                (signed int)(msg->param[8]), // out_multiplier
                (signed int)(msg->param[9]), // out_shift
                (signed int)(msg->param[10]) // out_zero_bias
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_fully_connected_sym8sxasym8s_asym8s == 1
        case SRTM_Command_xa_nn_fully_connected_sym8sxasym8s_asym8s:
            rc = xa_nn_fully_connected_sym8sxasym8s_asym8s(
                (signed char *)MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA),       // p_out
                (const signed char *)MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA), // p_weight
                (const signed char *)MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA), // p_inp
                (const signed int *)MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA),  // p_bias
                (signed int)(msg->param[4]),                                                           // weight_depth
                (signed int)(msg->param[5]),                                                           // out_depth
                (signed int)(msg->param[6]), // input_zero_bias
                (signed int)(msg->param[7]), // out_multiplier
                (signed int)(msg->param[8]), // out_shift
                (signed int)(msg->param[9])  // out_zero_bias
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_activation_min_max_asym8u_asym8u == 1
        case SRTM_Command_xa_nn_vec_activation_min_max_asym8u_asym8u:
            rc = xa_nn_vec_activation_min_max_asym8u_asym8u(
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2]), // activation_min
                (signed int)(msg->param[3]), // activation_max
                (signed int)(msg->param[4])  // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_activation_min_max_f32_f32 == 1
        case SRTM_Command_xa_nn_vec_activation_min_max_f32_f32:
            rc = xa_nn_vec_activation_min_max_f32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (float)(msg->param[2]),                                                            // activation_min
                (float)(msg->param[3]),                                                            // activation_max
                (signed int)(msg->param[4])                                                        // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_softmax_asym8u_asym8u == 1
        case SRTM_Command_xa_nn_vec_softmax_asym8u_asym8u:
            rc = xa_nn_vec_softmax_asym8u_asym8u(
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2]),                                                               // diffmin
                (signed int)(msg->param[3]),                                               // input_left_shift
                (signed int)(msg->param[4]),                                               // input_multiplier
                (signed int)(msg->param[5]),                                               // vec_length
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[6], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_softmax_asym8s_asym8s == 1
        case SRTM_Command_xa_nn_vec_softmax_asym8s_asym8s:
            rc = xa_nn_vec_softmax_asym8s_asym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2]),                                                             // diffmin
                (signed int)(msg->param[3]),                                               // input_beta_left_shift
                (signed int)(msg->param[4]),                                               // input_beta_multiplier
                (signed int)(msg->param[5]),                                               // vec_length
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[6], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_softmax_asym8s_16 == 1
        case SRTM_Command_xa_nn_vec_softmax_asym8s_16:
            rc = xa_nn_vec_softmax_asym8s_16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),      // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2]),                                                             // diffmin
                (signed int)(msg->param[3]),                                               // input_beta_left_shift
                (signed int)(msg->param[4]),                                               // input_beta_multiplier
                (signed int)(msg->param[5]),                                               // vec_length
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[6], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_sigmoid_asym8u_asym8u == 1
        case SRTM_Command_xa_nn_vec_sigmoid_asym8u_asym8u:
            rc = xa_nn_vec_sigmoid_asym8u_asym8u(
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2]),                                                               // zero_point
                (signed int)(msg->param[3]), // input_range_radius
                (signed int)(msg->param[4]), // input_multiplier
                (signed int)(msg->param[5]), // input_left_shift
                (signed int)(msg->param[6])  // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_sigmoid_asym8s_asym8s == 1
        case SRTM_Command_xa_nn_vec_sigmoid_asym8s_asym8s:
            rc = xa_nn_vec_sigmoid_asym8s_asym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2]),                                                             // zero_point
                (signed int)(msg->param[3]), // input_range_radius
                (signed int)(msg->param[4]), // input_multiplier
                (signed int)(msg->param[5]), // input_left_shift
                (signed int)(msg->param[6])  // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_get_softmax_scratch_size == 1
        case SRTM_Command_get_softmax_scratch_size:
            rc = get_softmax_scratch_size((signed int)(msg->param[0]), // inp_precision
                                          (signed int)(msg->param[1]), // out_precision
                                          (signed int)(msg->param[2])  // length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_activation_min_max_8_8 == 1
        case SRTM_Command_xa_nn_vec_activation_min_max_8_8:
            rc = xa_nn_vec_activation_min_max_8_8(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2]), // activation_min
                (signed int)(msg->param[3]), // activation_max
                (signed int)(msg->param[4])  // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_activation_min_max_16_16 == 1
        case SRTM_Command_xa_nn_vec_activation_min_max_16_16:
            rc = xa_nn_vec_activation_min_max_16_16(
                (signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2]), // activation_min
                (signed int)(msg->param[3]), // activation_max
                (signed int)(msg->param[4])  // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu_asym8u_asym8u == 1
        case SRTM_Command_xa_nn_vec_relu_asym8u_asym8u:
            rc = xa_nn_vec_relu_asym8u_asym8u(
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2]), // inp_zero_bias
                (signed int)(msg->param[3]), // out_multiplier
                (signed int)(msg->param[4]), // out_shift
                (signed int)(msg->param[5]), // out_zero_bias
                (signed int)(msg->param[6]), // quantized_activation_min
                (signed int)(msg->param[7]), // quantized_activation_max
                (signed int)(msg->param[8])  // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu_asym8s_asym8s == 1
        case SRTM_Command_xa_nn_vec_relu_asym8s_asym8s:
            rc = xa_nn_vec_relu_asym8s_asym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2]), // inp_zero_bias
                (signed int)(msg->param[3]), // out_multiplier
                (signed int)(msg->param[4]), // out_shift
                (signed int)(msg->param[5]), // out_zero_bias
                (signed int)(msg->param[6]), // quantized_activation_min
                (signed int)(msg->param[7]), // quantized_activation_max
                (signed int)(msg->param[8])  // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_prelu_asym8s_asym8s == 1
        case SRTM_Command_xa_nn_vec_prelu_asym8s_asym8s:
            rc = xa_nn_vec_prelu_asym8s_asym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_vec_alpha
                (signed int)(msg->param[3]), // inp_zero_bias
                (signed int)(msg->param[4]), // alpha_zero_bias
                (signed int)(msg->param[5]), // alpha_multiplier
                (signed int)(msg->param[6]), // alpha_shift
                (signed int)(msg->param[7]), // out_multiplier
                (signed int)(msg->param[8]), // out_shift
                (signed int)(msg->param[9]), // out_zero_bias
                (signed int)(msg->param[10]) // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_hard_swish_asym8s_asym8s == 1
        case SRTM_Command_xa_nn_vec_hard_swish_asym8s_asym8s:
            rc = xa_nn_vec_hard_swish_asym8s_asym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2]),   // inp_zero_bias
                (signed short)(msg->param[3]), // reluish_multiplier
                (signed int)(msg->param[4]),   // reluish_shift
                (signed short)(msg->param[5]), // out_multiplier
                (signed int)(msg->param[6]),   // out_shift
                (signed int)(msg->param[7]),   // out_zero_bias
                (signed int)(msg->param[8])    // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_tanh_asym8s_asym8s == 1
        case SRTM_Command_xa_nn_vec_tanh_asym8s_asym8s:
            rc = xa_nn_vec_tanh_asym8s_asym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_vec
                (signed int)(msg->param[2]),                                                             // zero_point
                (signed int)(msg->param[3]), // input_range_radius
                (signed int)(msg->param[4]), // input_multiplier
                (signed int)(msg->param[5]), // input_left_shift
                (signed int)(msg->param[6])  // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv1d_std_asym8uxasym8u == 1
        case SRTM_Command_xa_nn_conv1d_std_asym8uxasym8u:
            rc = xa_nn_conv1d_std_asym8uxasym8u(
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_kernel
                (signed int *)(msg->param[3]),                                                       // p_bias
                (signed int)(msg->param[4]),                                                         // input_height
                (signed int)(msg->param[5]),                                                         // input_width
                (signed int)(msg->param[6]),                                                         // input_channels
                (signed int)(msg->param[7]),                                                         // kernel_height
                (signed int)(msg->param[8]),                                                         // out_channels
                (signed int)(msg->param[9]),                                                         // y_stride
                (signed int)(msg->param[10]),                                                        // y_padding
                (signed int)(msg->param[11]),                                                        // out_height
                (signed int)(msg->param[12]),                                                        // input_zero_bias
                (signed int)(msg->param[13]),                                                        // kernel_zero_bias
                (signed int)(msg->param[14]),                                                        // out_multiplier
                (signed int)(msg->param[15]),                                                        // out_shift
                (signed int)(msg->param[16]),                                                        // out_zero_bias
                (signed int)(msg->param[17]),                                                        // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[18], kMEMORY_Local2DMA))          // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_std_asym8uxasym8u == 1
        case SRTM_Command_xa_nn_conv2d_std_asym8uxasym8u:
            rc = xa_nn_conv2d_std_asym8uxasym8u(
                (unsigned char *)MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA),       // p_out
                (const unsigned char *)MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA), // p_inp
                (const unsigned char *)MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA), // p_kernel
                (const signed int *)MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA),    // p_bias
                (signed int)(msg->param[4]),                                                             // input_height
                (signed int)(msg->param[5]),                                                             // input_width
                (signed int)(msg->param[6]),                                                // input_channels
                (signed int)(msg->param[7]),                                                // kernel_height
                (signed int)(msg->param[8]),                                                // kernel_width
                (signed int)(msg->param[9]),                                                // out_channels
                (signed int)(msg->param[10]),                                               // x_stride
                (signed int)(msg->param[11]),                                               // y_stride
                (signed int)(msg->param[12]),                                               // x_padding
                (signed int)(msg->param[13]),                                               // y_padding
                (signed int)(msg->param[14]),                                               // out_height
                (signed int)(msg->param[15]),                                               // out_width
                (signed int)(msg->param[16]),                                               // input_zero_bias
                (signed int)(msg->param[17]),                                               // kernel_zero_bias
                (signed int)(msg->param[18]),                                               // out_multiplier
                (signed int)(msg->param[19]),                                               // out_shift
                (signed int)(msg->param[20]),                                               // out_zero_bias
                (signed int)(msg->param[21]),                                               // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[22], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_std_per_chan_sym8sxasym8s == 1
        case SRTM_Command_xa_nn_conv2d_std_per_chan_sym8sxasym8s:
            rc = xa_nn_conv2d_std_per_chan_sym8sxasym8s(
                (signed char *)MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA),       // p_out
                (const signed char *)MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA), // p_inp
                (const signed char *)MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA), // p_kernel
                (const signed int *)MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA),  // p_bias
                (signed int)(msg->param[4]),                                                           // input_height
                (signed int)(msg->param[5]),                                                           // input_width
                (signed int)(msg->param[6]),                                                           // input_channels
                (signed int)(msg->param[7]),                                                           // kernel_height
                (signed int)(msg->param[8]),                                                           // kernel_width
                (signed int)(msg->param[9]),                                                           // out_channels
                (signed int)(msg->param[10]),                                                          // x_stride
                (signed int)(msg->param[11]),                                                          // y_stride
                (signed int)(msg->param[12]),                                                          // x_padding
                (signed int)(msg->param[13]),                                                          // y_padding
                (signed int)(msg->param[14]),                                                          // out_height
                (signed int)(msg->param[15]),                                                          // out_width
                (signed int)(msg->param[16]),                                                      // input_zero_bias
                (signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[17], kMEMORY_Local2DMA)), // p_out_multiplier
                (signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[18], kMEMORY_Local2DMA)), // p_out_shift
                (signed int)(msg->param[19]),                                                      // out_zero_bias
                (signed int)(msg->param[20]),                                                      // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[21], kMEMORY_Local2DMA))        // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_batch_asym8uxasym8u_asym8u == 1
        case SRTM_Command_xa_nn_matXvec_batch_asym8uxasym8u_asym8u:
            rc = xa_nn_matXvec_batch_asym8uxasym8u_asym8u(
                (unsigned char **)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),  // p_mat1
                (unsigned char **)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_vec1
                (signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),     // p_bias
                (signed int)(msg->param[4]),                                                          // rows
                (signed int)(msg->param[5]),                                                          // cols1
                (signed int)(msg->param[6]),                                                          // row_stride1
                (signed int)(msg->param[7]),                                                          // vec_count
                (signed int)(msg->param[8]),                                                          // mat1_zero_bias
                (signed int)(msg->param[9]),                                                          // vec1_zero_bias
                (signed int)(msg->param[10]),                                                         // out_multiplier
                (signed int)(msg->param[11]),                                                         // out_shift
                (signed int)(msg->param[12])                                                          // out_zero_bias
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matmul_asym8uxasym8u_asym8u == 1
        case SRTM_Command_xa_nn_matmul_asym8uxasym8u_asym8u:
            rc = xa_nn_matmul_asym8uxasym8u_asym8u(
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_mat2
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),    // p_bias
                (signed int)(msg->param[4]),                                                               // rows
                (signed int)(msg->param[5]),                                                               // cols
                (signed int)(msg->param[6]),                                                               // row_stride
                (signed int)(msg->param[7]),                                                               // vec_count
                (signed int)(msg->param[8]),                                                               // vec_offset
                (signed int)(msg->param[9]),                                                               // out_offset
                (signed int)(msg->param[10]),                                                              // out_stride
                (signed int)(msg->param[11]), // mat1_zero_bias
                (signed int)(msg->param[12]), // vec1_zero_bias
                (signed int)(msg->param[13]), // out_multiplier
                (signed int)(msg->param[14]), // out_shift
                (signed int)(msg->param[15])  // out_zero_bias
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matmul_per_chan_sym8sxasym8s_asym8s == 1
        case SRTM_Command_xa_nn_matmul_per_chan_sym8sxasym8s_asym8s:
            rc = xa_nn_matmul_per_chan_sym8sxasym8s_asym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_mat1
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_vec1
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),  // p_bias
                (signed int)(msg->param[4]),                                                             // rows
                (signed int)(msg->param[5]),                                                             // cols1
                (signed int)(msg->param[6]),                                                             // row_stride1
                (signed int)(msg->param[7]),                                                             // vec_count
                (signed int)(msg->param[8]),                                                             // vec_offset
                (signed int)(msg->param[9]),                                                             // out_offset
                (signed int)(msg->param[10]),                                                            // out_stride
                (signed int)(msg->param[11]),         // vec1_zero_bias
                (const signed int *)(msg->param[12]), // p_out_multiplier
                (const signed int *)(msg->param[13]), // p_out_shift
                (signed int)(msg->param[14])          // out_zero_bias
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_depthwise_asym8uxasym8u == 1
        case SRTM_Command_xa_nn_conv2d_depthwise_asym8uxasym8u:
            rc = xa_nn_conv2d_depthwise_asym8uxasym8u(
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_kernel
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),    // p_bias
                (signed int)(msg->param[4]),                                                // input_height
                (signed int)(msg->param[5]),                                                // input_width
                (signed int)(msg->param[6]),                                                // input_channels
                (signed int)(msg->param[7]),                                                // kernel_height
                (signed int)(msg->param[8]),                                                // kernel_width
                (signed int)(msg->param[9]),                                                // channels_multiplier
                (signed int)(msg->param[10]),                                               // x_stride
                (signed int)(msg->param[11]),                                               // y_stride
                (signed int)(msg->param[12]),                                               // x_padding
                (signed int)(msg->param[13]),                                               // y_padding
                (signed int)(msg->param[14]),                                               // out_height
                (signed int)(msg->param[15]),                                               // out_width
                (signed int)(msg->param[16]),                                               // input_zero_bias
                (signed int)(msg->param[17]),                                               // kernel_zero_bias
                (signed int)(msg->param[18]),                                               // out_multiplier
                (signed int)(msg->param[19]),                                               // out_shift
                (signed int)(msg->param[20]),                                               // out_zero_bias
                (signed int)(msg->param[21]),                                               // inp_data_format
                (signed int)(msg->param[22]),                                               // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[23], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_pointwise_asym8uxasym8u == 1
        case SRTM_Command_xa_nn_conv2d_pointwise_asym8uxasym8u:
            rc = xa_nn_conv2d_pointwise_asym8uxasym8u(
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_kernel
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),    // p_bias
                (signed int)(msg->param[4]),                                                         // input_height
                (signed int)(msg->param[5]),                                                         // input_width
                (signed int)(msg->param[6]),                                                         // input_channels
                (signed int)(msg->param[7]),                                                         // out_channels
                (signed int)(msg->param[8]),                                                         // input_zero_bias
                (signed int)(msg->param[9]),                                                         // kernel_zero_bias
                (signed int)(msg->param[10]),                                                        // out_multiplier
                (signed int)(msg->param[11]),                                                        // out_shift
                (signed int)(msg->param[12]),                                                        // out_zero_bias
                (signed int)(msg->param[13])                                                         // out_data_format
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_depthwise_per_chan_sym8sxasym8s == 1
        case SRTM_Command_xa_nn_conv2d_depthwise_per_chan_sym8sxasym8s:
            rc = xa_nn_conv2d_depthwise_per_chan_sym8sxasym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_kernel
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),  // p_bias
                (signed int)(msg->param[4]),                                                             // input_height
                (signed int)(msg->param[5]),                                                             // input_width
                (signed int)(msg->param[6]),                                                // input_channels
                (signed int)(msg->param[7]),                                                // kernel_height
                (signed int)(msg->param[8]),                                                // kernel_width
                (signed int)(msg->param[9]),                                                // channels_multiplier
                (signed int)(msg->param[10]),                                               // x_stride
                (signed int)(msg->param[11]),                                               // y_stride
                (signed int)(msg->param[12]),                                               // x_padding
                (signed int)(msg->param[13]),                                               // y_padding
                (signed int)(msg->param[14]),                                               // out_height
                (signed int)(msg->param[15]),                                               // out_width
                (signed int)(msg->param[16]),                                               // input_zero_bias
                (const signed int *)(msg->param[17]),                                       // p_out_multiplier
                (const signed int *)(msg->param[18]),                                       // p_out_shift
                (signed int)(msg->param[19]),                                               // out_zero_bias
                (signed int)(msg->param[20]),                                               // inp_data_format
                (signed int)(msg->param[21]),                                               // out_data_format
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[22], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_pointwise_per_chan_sym8sxasym8s == 1
        case SRTM_Command_xa_nn_conv2d_pointwise_per_chan_sym8sxasym8s:
            rc = xa_nn_conv2d_pointwise_per_chan_sym8sxasym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_kernel
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),  // p_bias
                (signed int)(msg->param[4]),                                                       // input_height
                (signed int)(msg->param[5]),                                                       // input_width
                (signed int)(msg->param[6]),                                                       // input_channels
                (signed int)(msg->param[7]),                                                       // out_channels
                (signed int)(msg->param[8]),                                                       // input_zero_bias
                (signed int *)(msg->param[9]),                                                     // p_out_multiplier
                (signed int *)(msg->param[10]),                                                    // p_out_shift
                (signed int)(msg->param[11]),                                                      // out_zero_bias
                (signed int)(msg->param[12])                                                       // out_data_format
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_mul_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_elm_mul_f32xf32_f32:
            rc = xa_nn_elm_mul_f32xf32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp1
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[3])                                                        // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_add_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_elm_add_f32xf32_f32:
            rc = xa_nn_elm_add_f32xf32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp1
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[3])                                                        // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_mul_acc_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_elm_mul_acc_f32xf32_f32:
            rc = xa_nn_elm_mul_acc_f32xf32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp1
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[3])                                                        // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_sub_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_elm_sub_f32xf32_f32:
            rc = xa_nn_elm_sub_f32xf32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp1
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[3])                                                        // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_div_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_elm_div_f32xf32_f32:
            rc = xa_nn_elm_div_f32xf32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp1
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[3])                                                        // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_floor_f32_f32 == 1
        case SRTM_Command_xa_nn_elm_floor_f32_f32:
            rc = xa_nn_elm_floor_f32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed int)(msg->param[2])                                                        // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_add_asym8uxasym8u_asym8u == 1
        case SRTM_Command_xa_nn_elm_add_asym8uxasym8u_asym8u:
            rc = xa_nn_elm_add_asym8uxasym8u_asym8u(
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed int)(msg->param[1]),                                                         // out_zero_bias
                (signed int)(msg->param[2]),                                                         // out_left_shift
                (signed int)(msg->param[3]),                                                         // out_multiplier
                (signed int)(msg->param[4]), // out_activation_min
                (signed int)(msg->param[5]), // out_activation_max
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[6], kMEMORY_Local2DMA)), // p_inp1
                (signed int)(msg->param[7]), // inp1_zero_bias
                (signed int)(msg->param[8]), // inp1_left_shift
                (signed int)(msg->param[9]), // inp1_multiplier
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[10], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[11]), // inp2_zero_bias
                (signed int)(msg->param[12]), // inp2_left_shift
                (signed int)(msg->param[13]), // inp2_multiplier
                (signed int)(msg->param[14]), // left_shift
                (signed int)(msg->param[15])  // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_add_asym8sxasym8s_asym8s == 1
        case SRTM_Command_xa_nn_elm_add_asym8sxasym8s_asym8s:
            rc = xa_nn_elm_add_asym8sxasym8s_asym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed int)(msg->param[1]),                                                       // out_zero_bias
                (signed int)(msg->param[2]),                                                       // out_left_shift
                (signed int)(msg->param[3]),                                                       // out_multiplier
                (signed int)(msg->param[4]),                                                       // out_activation_min
                (signed int)(msg->param[5]),                                                       // out_activation_max
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[6], kMEMORY_Local2DMA)), // p_inp1
                (signed int)(msg->param[7]), // inp1_zero_bias
                (signed int)(msg->param[8]), // inp1_left_shift
                (signed int)(msg->param[9]), // inp1_multiplier
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[10], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[11]), // inp2_zero_bias
                (signed int)(msg->param[12]), // inp2_left_shift
                (signed int)(msg->param[13]), // inp2_multiplier
                (signed int)(msg->param[14]), // left_shift
                (signed int)(msg->param[15])  // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_sub_asym8uxasym8u_asym8u == 1
        case SRTM_Command_xa_nn_elm_sub_asym8uxasym8u_asym8u:
            rc = xa_nn_elm_sub_asym8uxasym8u_asym8u(
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed int)(msg->param[1]),                                                         // out_zero_bias
                (signed int)(msg->param[2]),                                                         // out_left_shift
                (signed int)(msg->param[3]),                                                         // out_multiplier
                (signed int)(msg->param[4]), // out_activation_min
                (signed int)(msg->param[5]), // out_activation_max
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[6], kMEMORY_Local2DMA)), // p_inp1
                (signed int)(msg->param[7]), // inp1_zero_bias
                (signed int)(msg->param[8]), // inp1_left_shift
                (signed int)(msg->param[9]), // inp1_multiplier
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[10], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[11]), // inp2_zero_bias
                (signed int)(msg->param[12]), // inp2_left_shift
                (signed int)(msg->param[13]), // inp2_multiplier
                (signed int)(msg->param[14]), // left_shift
                (signed int)(msg->param[15])  // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_sub_asym8sxasym8s_asym8s == 1
        case SRTM_Command_xa_nn_elm_sub_asym8sxasym8s_asym8s:
            rc = xa_nn_elm_sub_asym8sxasym8s_asym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed int)(msg->param[1]),                                                       // out_zero_bias
                (signed int)(msg->param[2]),                                                       // out_left_shift
                (signed int)(msg->param[3]),                                                       // out_multiplier
                (signed int)(msg->param[4]),                                                       // out_activation_min
                (signed int)(msg->param[5]),                                                       // out_activation_max
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[6], kMEMORY_Local2DMA)), // p_inp1
                (signed int)(msg->param[7]), // inp1_zero_bias
                (signed int)(msg->param[8]), // inp1_left_shift
                (signed int)(msg->param[9]), // inp1_multiplier
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[10], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[11]), // inp2_zero_bias
                (signed int)(msg->param[12]), // inp2_left_shift
                (signed int)(msg->param[13]), // inp2_multiplier
                (signed int)(msg->param[14]), // left_shift
                (signed int)(msg->param[15])  // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_mul_asym8uxasym8u_asym8u == 1
        case SRTM_Command_xa_nn_elm_mul_asym8uxasym8u_asym8u:
            rc = xa_nn_elm_mul_asym8uxasym8u_asym8u(
                (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed int)(msg->param[1]),                                                         // out_zero_bias
                (signed int)(msg->param[2]),                                                         // out_shift
                (signed int)(msg->param[3]),                                                         // out_multiplier
                (signed int)(msg->param[4]), // out_activation_min
                (signed int)(msg->param[5]), // out_activation_max
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[6], kMEMORY_Local2DMA)), // p_inp1
                (signed int)(msg->param[7]), // inp1_zero_bias
                (const unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[8], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[9]), // inp2_zero_bias
                (signed int)(msg->param[10]) // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_mul_asym8sxasym8s_asym8s == 1
        case SRTM_Command_xa_nn_elm_mul_asym8sxasym8s_asym8s:
            rc = xa_nn_elm_mul_asym8sxasym8s_asym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (signed int)(msg->param[1]),                                                       // out_zero_bias
                (signed int)(msg->param[2]),                                                       // out_shift
                (signed int)(msg->param[3]),                                                       // out_multiplier
                (signed int)(msg->param[4]),                                                       // out_activation_min
                (signed int)(msg->param[5]),                                                       // out_activation_max
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[6], kMEMORY_Local2DMA)), // p_inp1
                (signed int)(msg->param[7]), // inp1_zero_bias
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[8], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[9]), // inp2_zero_bias
                (signed int)(msg->param[10]) // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_quantize_asym16s_asym8s == 1
        case SRTM_Command_xa_nn_elm_quantize_asym16s_asym8s:
            rc = xa_nn_elm_requantize_asym16s_asym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),        // p_out
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed int)(msg->param[2]), // inp_zero_bias
                (signed int)(msg->param[3]), // out_zero_bias
                (signed int)(msg->param[4]), // out_shift
                (signed int)(msg->param[5]), // out_multiplier
                (signed int)(msg->param[6])  // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_max_8x8_8 == 1
        case SRTM_Command_xa_nn_elm_max_8x8_8:
            rc = xa_nn_elm_max_8x8_8(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_in1
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_in2
                (signed int)(msg->param[3])                                                              // num_element
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_min_8x8_8 == 1
        case SRTM_Command_xa_nn_elm_min_8x8_8:
            rc = xa_nn_elm_min_8x8_8(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_in1
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_in2
                (signed int)(msg->param[3])                                                              // num_element
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_equal_asym8sxasym8s == 1
        case SRTM_Command_xa_nn_elm_equal_asym8sxasym8s:
            rc = xa_nn_elm_equal_asym8sxasym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp1
                (signed int)(msg->param[2]), // inp1_zero_bias
                (signed int)(msg->param[3]), // inp1_shift
                (signed int)(msg->param[4]), // inp1_multiplier
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[6]), // inp2_zero_bias
                (signed int)(msg->param[7]), // inp2_shift
                (signed int)(msg->param[8]), // inp2_multiplier
                (signed int)(msg->param[9]), // left_shift
                (signed int)(msg->param[10]) // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_notequal_asym8sxasym8s == 1
        case SRTM_Command_xa_nn_elm_notequal_asym8sxasym8s:
            rc = xa_nn_elm_notequal_asym8sxasym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp1
                (signed int)(msg->param[2]), // inp1_zero_bias
                (signed int)(msg->param[3]), // inp1_shift
                (signed int)(msg->param[4]), // inp1_multiplier
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[6]), // inp2_zero_bias
                (signed int)(msg->param[7]), // inp2_shift
                (signed int)(msg->param[8]), // inp2_multiplier
                (signed int)(msg->param[9]), // left_shift
                (signed int)(msg->param[10]) // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_greater_asym8sxasym8s == 1
        case SRTM_Command_xa_nn_elm_greater_asym8sxasym8s:
            rc = xa_nn_elm_greater_asym8sxasym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp1
                (signed int)(msg->param[2]), // inp1_zero_bias
                (signed int)(msg->param[3]), // inp1_shift
                (signed int)(msg->param[4]), // inp1_multiplier
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[6]), // inp2_zero_bias
                (signed int)(msg->param[7]), // inp2_shift
                (signed int)(msg->param[8]), // inp2_multiplier
                (signed int)(msg->param[9]), // left_shift
                (signed int)(msg->param[10]) // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_greaterequal_asym8sxasym8s == 1
        case SRTM_Command_xa_nn_elm_greaterequal_asym8sxasym8s:
            rc = xa_nn_elm_greaterequal_asym8sxasym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp1
                (signed int)(msg->param[2]), // inp1_zero_bias
                (signed int)(msg->param[3]), // inp1_shift
                (signed int)(msg->param[4]), // inp1_multiplier
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[6]), // inp2_zero_bias
                (signed int)(msg->param[7]), // inp2_shift
                (signed int)(msg->param[8]), // inp2_multiplier
                (signed int)(msg->param[9]), // left_shift
                (signed int)(msg->param[10]) // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_less_asym8sxasym8s == 1
        case SRTM_Command_xa_nn_elm_less_asym8sxasym8s:
            rc = xa_nn_elm_less_asym8sxasym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp1
                (signed int)(msg->param[2]), // inp1_zero_bias
                (signed int)(msg->param[3]), // inp1_shift
                (signed int)(msg->param[4]), // inp1_multiplier
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[6]), // inp2_zero_bias
                (signed int)(msg->param[7]), // inp2_shift
                (signed int)(msg->param[8]), // inp2_multiplier
                (signed int)(msg->param[9]), // left_shift
                (signed int)(msg->param[10]) // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_lessequal_asym8sxasym8s == 1
        case SRTM_Command_xa_nn_elm_lessequal_asym8sxasym8s:
            rc = xa_nn_elm_lessequal_asym8sxasym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp1
                (signed int)(msg->param[2]), // inp1_zero_bias
                (signed int)(msg->param[3]), // inp1_shift
                (signed int)(msg->param[4]), // inp1_multiplier
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[5], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[6]), // inp2_zero_bias
                (signed int)(msg->param[7]), // inp2_shift
                (signed int)(msg->param[8]), // inp2_multiplier
                (signed int)(msg->param[9]), // left_shift
                (signed int)(msg->param[10]) // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_reduce_max_getsize_nhwc == 1
        case SRTM_Command_xa_nn_reduce_max_getsize_nhwc:
            rc = xa_nn_reduce_getsize_nhwc(
                (signed int)(msg->param[0]),                                                            // inp_precision
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp_shape
                (signed int)(msg->param[2]),                                                            // num_inp_dims
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // p_axis
                (signed int)(msg->param[4]),                                                            // num_axis_dims
                REDUCE_MAX);

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_reduce_max_4D_asym8s_asym8s == 1
        case SRTM_Command_xa_nn_reduce_max_4D_asym8s_asym8s:
            rc = xa_nn_reduce_max_4D_asym8s_asym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)),  // p_out_shape
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)),  // p_inp_shape
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[4], kMEMORY_Local2DMA)),  // p_axis
                (signed int)(msg->param[5]),                                                             // num_out_dims
                (signed int)(msg->param[6]),                                                             // num_inp_dims
                (signed int)(msg->param[7]),                                               // num_axis_dims
                (void *)(MEMORY_ConvertMemoryMapAddress(msg->param[8], kMEMORY_Local2DMA)) // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_logicaland_boolxbool_bool == 1
        case SRTM_Command_xa_nn_elm_logicaland_boolxbool_bool:
            rc = xa_nn_elm_logicaland_boolxbool_bool(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp1
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[3])                                                              // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_logicalor_boolxbool_bool == 1
        case SRTM_Command_xa_nn_elm_logicalor_boolxbool_bool:
            rc = xa_nn_elm_logicalor_boolxbool_bool(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp1
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[3])                                                              // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_logicalnot_bool_bool == 1
        case SRTM_Command_xa_nn_elm_logicalnot_bool_bool:
            rc = xa_nn_elm_logicalnot_bool_bool(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed int)(msg->param[2])                                                              // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_l2_norm_f32 == 1
        case SRTM_Command_xa_nn_l2_norm_f32:
            rc = xa_nn_l2_norm_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed int)(msg->param[2])                                                        // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_l2_norm_asym8s_asym8s == 1
        case SRTM_Command_xa_nn_l2_norm_asym8s_asym8s:
            rc = xa_nn_l2_norm_asym8s_asym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp
                (signed int)(msg->param[2]),                                                             // zero_point
                (signed int)(msg->param[3])                                                              // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_dot_prod_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_dot_prod_f32xf32_f32:
            rc = xa_nn_dot_prod_f32xf32_f32(
                (float *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)),       // p_out
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // p_inp1
                (const float *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA)), // p_inp2
                (signed int)(msg->param[3]),                                                       // vec_length
                (signed int)(msg->param[4])                                                        // num_vecs
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_dot_prod_16x16_asym8s == 1
        case SRTM_Command_xa_nn_dot_prod_16x16_asym8s:
            rc = xa_nn_dot_prod_16x16_asym8s(
                (signed char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // p_out
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[1],
                                                                      kMEMORY_Local2DMA)), // p_inp1_start
                (const signed short *)(MEMORY_ConvertMemoryMapAddress(msg->param[2],
                                                                      kMEMORY_Local2DMA)),              // p_inp2_start
                (const signed int *)(MEMORY_ConvertMemoryMapAddress(msg->param[3], kMEMORY_Local2DMA)), // bias_ptr
                (signed int)(msg->param[4]),                                                            // vec_length
                (signed int)(msg->param[5]), // out_multiplier
                (signed int)(msg->param[6]), // out_shift
                (signed int)(msg->param[7]), // out_zero_bias
                (signed int)(msg->param[8])  // vec_count
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif
#if NN_ENABLE_resizenearest_f32 == 1
        case SRTM_Command_resizenearest_f32:
            resizenearest_f32((float *)(msg->param[0]),              // dst
                              (const float *)(msg->param[1]),        // src
                              (const float)(msg->param[2]),          // scaleH
                              (const float)(msg->param[3]),          // scaleW
                              (const unsigned int *)(msg->param[4]), // inWdims
                              (const unsigned int *)(msg->param[5])  // outWdims
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)0;
            break;

        case SRTM_Command_resizenearest_8:
            resizenearest_i8((char *)(msg->param[0]),               // dst
                             (const char *)(msg->param[1]),         // src
                             (const float)(msg->param[2]),          // scaleH
                             (const float)(msg->param[3]),          // scaleW
                             (const unsigned int *)(msg->param[4]), // inWdims
                             (const unsigned int *)(msg->param[5])  // outWdims
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)0;
            break;
#endif
#if NN_ENABLE_xa_nn_inference == 1
        case SRTM_Command_inference:
            model((unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA)), // constantWeight
                  (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[1], kMEMORY_Local2DMA)), // mutableWeight
                  (unsigned char *)(MEMORY_ConvertMemoryMapAddress(msg->param[2], kMEMORY_Local2DMA))  // activations
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)0;
            break;
#endif
        /* Unknown message. */
        default:
            msg->head.type = SRTM_MessageTypeNotification;
            msg->error     = SRTM_Status_InvalidMessage;
            rc             = -1;
    }

    return rc;
}
