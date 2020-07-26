/*
 * Copyright 2019-2020 NXP
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

/*******************************************************************************
 * Code
 ******************************************************************************/

int handleMSG_NN(srtm_message *msg)
{
    int rc;

    switch (msg->head.command)
    {
#if NN_ENABLE_xa_nn_matXvec_16x16_16 == 1
        case SRTM_Command_xa_nn_matXvec_16x16_16:
            rc = xa_nn_matXvec_16x16_16((signed short *)(msg->param[0]), // p_out
                                        (signed short *)(msg->param[1]), // p_mat1
                                        (signed short *)(msg->param[2]), // p_mat2
                                        (signed short *)(msg->param[3]), // p_vec1
                                        (signed short *)(msg->param[4]), // p_vec2
                                        (signed short *)(msg->param[5]), // p_bias
                                        (signed int)(msg->param[6]),     // rows
                                        (signed int)(msg->param[7]),     // cols1
                                        (signed int)(msg->param[8]),     // cols2
                                        (signed int)(msg->param[9]),     // row_stride1
                                        (signed int)(msg->param[10]),    // row_stride2
                                        (signed int)(msg->param[11]),    // acc_shift
                                        (signed int)(msg->param[12])     // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_16x16_32 == 1
        case SRTM_Command_xa_nn_matXvec_16x16_32:
            rc = xa_nn_matXvec_16x16_32((signed int *)(msg->param[0]),   // p_out
                                        (signed short *)(msg->param[1]), // p_mat1
                                        (signed short *)(msg->param[2]), // p_mat2
                                        (signed short *)(msg->param[3]), // p_vec1
                                        (signed short *)(msg->param[4]), // p_vec2
                                        (signed short *)(msg->param[5]), // p_bias
                                        (signed int)(msg->param[6]),     // rows
                                        (signed int)(msg->param[7]),     // cols1
                                        (signed int)(msg->param[8]),     // cols2
                                        (signed int)(msg->param[9]),     // row_stride1
                                        (signed int)(msg->param[10]),    // row_stride2
                                        (signed int)(msg->param[11]),    // acc_shift
                                        (signed int)(msg->param[12])     // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_16x16_64 == 1
        case SRTM_Command_xa_nn_matXvec_16x16_64:
            rc = xa_nn_matXvec_16x16_64((signed long long *)(msg->param[0]), // p_out
                                        (signed short *)(msg->param[1]),     // p_mat1
                                        (signed short *)(msg->param[2]),     // p_mat2
                                        (signed short *)(msg->param[3]),     // p_vec1
                                        (signed short *)(msg->param[4]),     // p_vec2
                                        (signed short *)(msg->param[5]),     // p_bias
                                        (signed int)(msg->param[6]),         // rows
                                        (signed int)(msg->param[7]),         // cols1
                                        (signed int)(msg->param[8]),         // cols2
                                        (signed int)(msg->param[9]),         // row_stride1
                                        (signed int)(msg->param[10]),        // row_stride2
                                        (signed int)(msg->param[11]),        // acc_shift
                                        (signed int)(msg->param[12])         // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_16x16_16_tanh == 1
        case SRTM_Command_xa_nn_matXvec_16x16_16_tanh:
            rc = xa_nn_matXvec_16x16_16_tanh((signed short *)(msg->param[0]), // p_out
                                             (signed short *)(msg->param[1]), // p_mat1
                                             (signed short *)(msg->param[2]), // p_mat2
                                             (signed short *)(msg->param[3]), // p_vec1
                                             (signed short *)(msg->param[4]), // p_vec2
                                             (void *)(msg->param[5]),         // p_bias
                                             (signed int)(msg->param[6]),     // rows
                                             (signed int)(msg->param[7]),     // cols1
                                             (signed int)(msg->param[8]),     // cols2
                                             (signed int)(msg->param[9]),     // row_stride1
                                             (signed int)(msg->param[10]),    // row_stride2
                                             (signed int)(msg->param[11]),    // acc_shift
                                             (signed int)(msg->param[12]),    // bias_shift
                                             (signed int)(msg->param[13]),    // bias_precision
                                             (void *)(msg->param[14])         // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_16x16_16_sigmoid == 1
        case SRTM_Command_xa_nn_matXvec_16x16_16_sigmoid:
            rc = xa_nn_matXvec_16x16_16_sigmoid((signed short *)(msg->param[0]), // p_out
                                                (signed short *)(msg->param[1]), // p_mat1
                                                (signed short *)(msg->param[2]), // p_mat2
                                                (signed short *)(msg->param[3]), // p_vec1
                                                (signed short *)(msg->param[4]), // p_vec2
                                                (void *)(msg->param[5]),         // p_bias
                                                (signed int)(msg->param[6]),     // rows
                                                (signed int)(msg->param[7]),     // cols1
                                                (signed int)(msg->param[8]),     // cols2
                                                (signed int)(msg->param[9]),     // row_stride1
                                                (signed int)(msg->param[10]),    // row_stride2
                                                (signed int)(msg->param[11]),    // acc_shift
                                                (signed int)(msg->param[12]),    // bias_shift
                                                (signed int)(msg->param[13]),    // bias_precision
                                                (void *)(msg->param[14])         // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_batch_16x16_64 == 1
        case SRTM_Command_xa_nn_matXvec_batch_16x16_64:
            rc = xa_nn_matXvec_batch_16x16_64((signed long long **)(msg->param[0]), // p_out
                                              (signed short *)(msg->param[1]),      // p_mat1
                                              (signed short **)(msg->param[2]),     // p_vec1
                                              (signed short *)(msg->param[3]),      // p_bias
                                              (signed int)(msg->param[4]),          // rows
                                              (signed int)(msg->param[5]),          // cols1
                                              (signed int)(msg->param[6]),          // row_stride1
                                              (signed int)(msg->param[7]),          // acc_shift
                                              (signed int)(msg->param[8]),          // bias_shift
                                              (signed int)(msg->param[9])           // vec_count
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x16_16 == 1
        case SRTM_Command_xa_nn_matXvec_8x16_16:
            rc = xa_nn_matXvec_8x16_16((signed short *)(msg->param[0]), // p_out
                                       (signed char *)(msg->param[1]),  // p_mat1
                                       (signed char *)(msg->param[2]),  // p_mat2
                                       (signed short *)(msg->param[3]), // p_vec1
                                       (signed short *)(msg->param[4]), // p_vec2
                                       (signed short *)(msg->param[5]), // p_bias
                                       (signed int)(msg->param[6]),     // rows
                                       (signed int)(msg->param[7]),     // cols1
                                       (signed int)(msg->param[8]),     // cols2
                                       (signed int)(msg->param[9]),     // row_stride1
                                       (signed int)(msg->param[10]),    // row_stride2
                                       (signed int)(msg->param[11]),    // acc_shift
                                       (signed int)(msg->param[12])     // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x16_32 == 1
        case SRTM_Command_xa_nn_matXvec_8x16_32:
            rc = xa_nn_matXvec_8x16_32((signed int *)(msg->param[0]),   // p_out
                                       (signed char *)(msg->param[1]),  // p_mat1
                                       (signed char *)(msg->param[2]),  // p_mat2
                                       (signed short *)(msg->param[3]), // p_vec1
                                       (signed short *)(msg->param[4]), // p_vec2
                                       (signed short *)(msg->param[5]), // p_bias
                                       (signed int)(msg->param[6]),     // rows
                                       (signed int)(msg->param[7]),     // cols1
                                       (signed int)(msg->param[8]),     // cols2
                                       (signed int)(msg->param[9]),     // row_stride1
                                       (signed int)(msg->param[10]),    // row_stride2
                                       (signed int)(msg->param[11]),    // acc_shift
                                       (signed int)(msg->param[12])     // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x16_64 == 1
        case SRTM_Command_xa_nn_matXvec_8x16_64:
            rc = xa_nn_matXvec_8x16_64((signed long long *)(msg->param[0]), // p_out
                                       (signed char *)(msg->param[1]),      // p_mat1
                                       (signed char *)(msg->param[2]),      // p_mat2
                                       (signed short *)(msg->param[3]),     // p_vec1
                                       (signed short *)(msg->param[4]),     // p_vec2
                                       (signed short *)(msg->param[5]),     // p_bias
                                       (signed int)(msg->param[6]),         // rows
                                       (signed int)(msg->param[7]),         // cols1
                                       (signed int)(msg->param[8]),         // cols2
                                       (signed int)(msg->param[9]),         // row_stride1
                                       (signed int)(msg->param[10]),        // row_stride2
                                       (signed int)(msg->param[11]),        // acc_shift
                                       (signed int)(msg->param[12])         // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x16_16_tanh == 1
        case SRTM_Command_xa_nn_matXvec_8x16_16_tanh:
            rc = xa_nn_matXvec_8x16_16_tanh((signed short *)(msg->param[0]), // p_out
                                            (signed char *)(msg->param[1]),  // p_mat1
                                            (signed char *)(msg->param[2]),  // p_mat2
                                            (signed short *)(msg->param[3]), // p_vec1
                                            (signed short *)(msg->param[4]), // p_vec2
                                            (void *)(msg->param[5]),         // p_bias
                                            (signed int)(msg->param[6]),     // rows
                                            (signed int)(msg->param[7]),     // cols1
                                            (signed int)(msg->param[8]),     // cols2
                                            (signed int)(msg->param[9]),     // row_stride1
                                            (signed int)(msg->param[10]),    // row_stride2
                                            (signed int)(msg->param[11]),    // acc_shift
                                            (signed int)(msg->param[12]),    // bias_shift
                                            (signed int)(msg->param[13]),    // bias_precision
                                            (void *)(msg->param[14])         // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x16_16_sigmoid == 1
        case SRTM_Command_xa_nn_matXvec_8x16_16_sigmoid:
            rc = xa_nn_matXvec_8x16_16_sigmoid((signed short *)(msg->param[0]), // p_out
                                               (signed char *)(msg->param[1]),  // p_mat1
                                               (signed char *)(msg->param[2]),  // p_mat2
                                               (signed short *)(msg->param[3]), // p_vec1
                                               (signed short *)(msg->param[4]), // p_vec2
                                               (void *)(msg->param[5]),         // p_bias
                                               (signed int)(msg->param[6]),     // rows
                                               (signed int)(msg->param[7]),     // cols1
                                               (signed int)(msg->param[8]),     // cols2
                                               (signed int)(msg->param[9]),     // row_stride1
                                               (signed int)(msg->param[10]),    // row_stride2
                                               (signed int)(msg->param[11]),    // acc_shift
                                               (signed int)(msg->param[12]),    // bias_shift
                                               (signed int)(msg->param[13]),    // bias_precision
                                               (void *)(msg->param[14])         // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_batch_8x16_64 == 1
        case SRTM_Command_xa_nn_matXvec_batch_8x16_64:
            rc = xa_nn_matXvec_batch_8x16_64((signed long long **)(msg->param[0]), // p_out
                                             (signed char *)(msg->param[1]),       // p_mat1
                                             (signed short **)(msg->param[2]),     // p_vec1
                                             (signed short *)(msg->param[3]),      // p_bias
                                             (signed int)(msg->param[4]),          // rows
                                             (signed int)(msg->param[5]),          // cols1
                                             (signed int)(msg->param[6]),          // row_stride1
                                             (signed int)(msg->param[7]),          // acc_shift
                                             (signed int)(msg->param[8]),          // bias_shift
                                             (signed int)(msg->param[9])           // vec_count
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x8_8 == 1
        case SRTM_Command_xa_nn_matXvec_8x8_8:
            rc = xa_nn_matXvec_8x8_8((signed char *)(msg->param[0]), // p_out
                                     (signed char *)(msg->param[1]), // p_mat1
                                     (signed char *)(msg->param[2]), // p_mat2
                                     (signed char *)(msg->param[3]), // p_vec1
                                     (signed char *)(msg->param[4]), // p_vec2
                                     (signed char *)(msg->param[5]), // p_bias
                                     (signed int)(msg->param[6]),    // rows
                                     (signed int)(msg->param[7]),    // cols1
                                     (signed int)(msg->param[8]),    // cols2
                                     (signed int)(msg->param[9]),    // row_stride1
                                     (signed int)(msg->param[10]),   // row_stride2
                                     (signed int)(msg->param[11]),   // acc_shift
                                     (signed int)(msg->param[12])    // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x8_16 == 1
        case SRTM_Command_xa_nn_matXvec_8x8_16:
            rc = xa_nn_matXvec_8x8_16((signed short *)(msg->param[0]), // p_out
                                      (signed char *)(msg->param[1]),  // p_mat1
                                      (signed char *)(msg->param[2]),  // p_mat2
                                      (signed char *)(msg->param[3]),  // p_vec1
                                      (signed char *)(msg->param[4]),  // p_vec2
                                      (signed char *)(msg->param[5]),  // p_bias
                                      (signed int)(msg->param[6]),     // rows
                                      (signed int)(msg->param[7]),     // cols1
                                      (signed int)(msg->param[8]),     // cols2
                                      (signed int)(msg->param[9]),     // row_stride1
                                      (signed int)(msg->param[10]),    // row_stride2
                                      (signed int)(msg->param[11]),    // acc_shift
                                      (signed int)(msg->param[12])     // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x8_32 == 1
        case SRTM_Command_xa_nn_matXvec_8x8_32:
            rc = xa_nn_matXvec_8x8_32((signed int *)(msg->param[0]),  // p_out
                                      (signed char *)(msg->param[1]), // p_mat1
                                      (signed char *)(msg->param[2]), // p_mat2
                                      (signed char *)(msg->param[3]), // p_vec1
                                      (signed char *)(msg->param[4]), // p_vec2
                                      (signed char *)(msg->param[5]), // p_bias
                                      (signed int)(msg->param[6]),    // rows
                                      (signed int)(msg->param[7]),    // cols1
                                      (signed int)(msg->param[8]),    // cols2
                                      (signed int)(msg->param[9]),    // row_stride1
                                      (signed int)(msg->param[10]),   // row_stride2
                                      (signed int)(msg->param[11]),   // acc_shift
                                      (signed int)(msg->param[12])    // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x8_8_tanh == 1
        case SRTM_Command_xa_nn_matXvec_8x8_8_tanh:
            rc = xa_nn_matXvec_8x8_8_tanh((signed char *)(msg->param[0]), // p_out
                                          (signed char *)(msg->param[1]), // p_mat1
                                          (signed char *)(msg->param[2]), // p_mat2
                                          (signed char *)(msg->param[3]), // p_vec1
                                          (signed char *)(msg->param[4]), // p_vec2
                                          (void *)(msg->param[5]),        // p_bias
                                          (signed int)(msg->param[6]),    // rows
                                          (signed int)(msg->param[7]),    // cols1
                                          (signed int)(msg->param[8]),    // cols2
                                          (signed int)(msg->param[9]),    // row_stride1
                                          (signed int)(msg->param[10]),   // row_stride2
                                          (signed int)(msg->param[11]),   // acc_shift
                                          (signed int)(msg->param[12]),   // bias_shift
                                          (signed int)(msg->param[13]),   // bias_precision
                                          (void *)(msg->param[14])        // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_8x8_8_sigmoid == 1
        case SRTM_Command_xa_nn_matXvec_8x8_8_sigmoid:
            rc = xa_nn_matXvec_8x8_8_sigmoid((signed char *)(msg->param[0]), // p_out
                                             (signed char *)(msg->param[1]), // p_mat1
                                             (signed char *)(msg->param[2]), // p_mat2
                                             (signed char *)(msg->param[3]), // p_vec1
                                             (signed char *)(msg->param[4]), // p_vec2
                                             (void *)(msg->param[5]),        // p_bias
                                             (signed int)(msg->param[6]),    // rows
                                             (signed int)(msg->param[7]),    // cols1
                                             (signed int)(msg->param[8]),    // cols2
                                             (signed int)(msg->param[9]),    // row_stride1
                                             (signed int)(msg->param[10]),   // row_stride2
                                             (signed int)(msg->param[11]),   // acc_shift
                                             (signed int)(msg->param[12]),   // bias_shift
                                             (signed int)(msg->param[13]),   // bias_precision
                                             (void *)(msg->param[14])        // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_batch_8x8_32 == 1
        case SRTM_Command_xa_nn_matXvec_batch_8x8_32:
            rc = xa_nn_matXvec_batch_8x8_32((signed int **)(msg->param[0]),  // p_out
                                            (signed char *)(msg->param[1]),  // p_mat1
                                            (signed char **)(msg->param[2]), // p_vec1
                                            (signed char *)(msg->param[3]),  // p_bias
                                            (signed int)(msg->param[4]),     // rows
                                            (signed int)(msg->param[5]),     // cols1
                                            (signed int)(msg->param[6]),     // row_stride1
                                            (signed int)(msg->param[7]),     // acc_shift
                                            (signed int)(msg->param[8]),     // bias_shift
                                            (signed int)(msg->param[9])      // vec_count
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_f32xf32_f32_sigmoid == 1
        case SRTM_Command_xa_nn_matXvec_f32xf32_f32_sigmoid:
            rc = xa_nn_matXvec_f32xf32_f32_sigmoid((float *)(msg->param[0]),     // p_out
                                                   (float *)(msg->param[1]),     // p_mat1
                                                   (float *)(msg->param[2]),     // p_mat2
                                                   (float *)(msg->param[3]),     // p_vec1
                                                   (float *)(msg->param[4]),     // p_vec2
                                                   (float *)(msg->param[5]),     // p_bias
                                                   (signed int)(msg->param[6]),  // rows
                                                   (signed int)(msg->param[7]),  // cols1
                                                   (signed int)(msg->param[8]),  // cols2
                                                   (signed int)(msg->param[9]),  // row_stride1
                                                   (signed int)(msg->param[10]), // row_stride2
                                                   (float *)(msg->param[11])     // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_f32xf32_f32_tanh == 1
        case SRTM_Command_xa_nn_matXvec_f32xf32_f32_tanh:
            rc = xa_nn_matXvec_f32xf32_f32_tanh((float *)(msg->param[0]),     // p_out
                                                (float *)(msg->param[1]),     // p_mat1
                                                (float *)(msg->param[2]),     // p_mat2
                                                (float *)(msg->param[3]),     // p_vec1
                                                (float *)(msg->param[4]),     // p_vec2
                                                (float *)(msg->param[5]),     // p_bias
                                                (signed int)(msg->param[6]),  // rows
                                                (signed int)(msg->param[7]),  // cols1
                                                (signed int)(msg->param[8]),  // cols2
                                                (signed int)(msg->param[9]),  // row_stride1
                                                (signed int)(msg->param[10]), // row_stride2
                                                (float *)(msg->param[11])     // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_matXvec_f32xf32_f32:
            rc = xa_nn_matXvec_f32xf32_f32((float *)(msg->param[0]),       // p_out
                                           (const float *)(msg->param[1]), // p_mat1
                                           (const float *)(msg->param[2]), // p_mat2
                                           (const float *)(msg->param[3]), // p_vec1
                                           (const float *)(msg->param[4]), // p_vec2
                                           (const float *)(msg->param[5]), // p_bias
                                           (signed int)(msg->param[6]),    // rows
                                           (signed int)(msg->param[7]),    // cols1
                                           (signed int)(msg->param[8]),    // cols2
                                           (signed int)(msg->param[9]),    // row_stride1
                                           (signed int)(msg->param[10])    // row_stride2
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_batch_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_matXvec_batch_f32xf32_f32:
            rc = xa_nn_matXvec_batch_f32xf32_f32((float **)(msg->param[0]),   // p_out
                                                 (float *)(msg->param[1]),    // p_mat1
                                                 (float **)(msg->param[2]),   // p_vec1
                                                 (float *)(msg->param[3]),    // p_bias
                                                 (signed int)(msg->param[4]), // rows
                                                 (signed int)(msg->param[5]), // cols1
                                                 (signed int)(msg->param[6]), // row_stride1
                                                 (signed int)(msg->param[7])  // vec_count
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_sigmoid_32_32 == 1
        case SRTM_Command_xa_nn_vec_sigmoid_32_32:
            rc = xa_nn_vec_sigmoid_32_32((signed int *)(msg->param[0]),       // p_out
                                         (const signed int *)(msg->param[1]), // p_vec
                                         (signed int)(msg->param[2])          // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_tanh_32_32 == 1
        case SRTM_Command_xa_nn_vec_tanh_32_32:
            rc = xa_nn_vec_tanh_32_32((signed int *)(msg->param[0]),       // p_out
                                      (const signed int *)(msg->param[1]), // p_vec
                                      (signed int)(msg->param[2])          // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu_32_32 == 1
        case SRTM_Command_xa_nn_vec_relu_32_32:
            rc = xa_nn_vec_relu_32_32((signed int *)(msg->param[0]),       // p_out
                                      (const signed int *)(msg->param[1]), // p_vec
                                      (signed int)(msg->param[2]),         // threshold
                                      (signed int)(msg->param[3])          // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu1_32_32 == 1
        case SRTM_Command_xa_nn_vec_relu1_32_32:
            rc = xa_nn_vec_relu1_32_32((signed int *)(msg->param[0]),       // p_out
                                       (const signed int *)(msg->param[1]), // p_vec
                                       (signed int)(msg->param[2])          // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu6_32_32 == 1
        case SRTM_Command_xa_nn_vec_relu6_32_32:
            rc = xa_nn_vec_relu6_32_32((signed int *)(msg->param[0]),       // p_out
                                       (const signed int *)(msg->param[1]), // p_vec
                                       (signed int)(msg->param[2])          // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_softmax_32_32 == 1
        case SRTM_Command_xa_nn_vec_softmax_32_32:
            rc = xa_nn_vec_softmax_32_32((signed int *)(msg->param[0]),       // p_out
                                         (const signed int *)(msg->param[1]), // p_vec
                                         (signed int)(msg->param[2])          // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_sigmoid_f32_f32 == 1
        case SRTM_Command_xa_nn_vec_sigmoid_f32_f32:
            rc = xa_nn_vec_sigmoid_f32_f32((float *)(msg->param[0]),       // p_out
                                           (const float *)(msg->param[1]), // p_vec
                                           (signed int)(msg->param[2])     // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_tanh_f32_f32 == 1
        case SRTM_Command_xa_nn_vec_tanh_f32_f32:
            rc = xa_nn_vec_tanh_f32_f32((float *)(msg->param[0]),       // p_out
                                        (const float *)(msg->param[1]), // p_vec
                                        (signed int)(msg->param[2])     // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu_f32_f32 == 1
        case SRTM_Command_xa_nn_vec_relu_f32_f32:
            rc = xa_nn_vec_relu_f32_f32((float *)(msg->param[0]),       // p_out
                                        (const float *)(msg->param[1]), // p_vec
                                        (float)(msg->param[2]),         // threshold
                                        (signed int)(msg->param[3])     // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu1_f32_f32 == 1
        case SRTM_Command_xa_nn_vec_relu1_f32_f32:
            rc = xa_nn_vec_relu1_f32_f32((float *)(msg->param[0]),       // p_out
                                         (const float *)(msg->param[1]), // p_vec
                                         (signed int)(msg->param[2])     // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_relu6_f32_f32 == 1
        case SRTM_Command_xa_nn_vec_relu6_f32_f32:
            rc = xa_nn_vec_relu6_f32_f32((float *)(msg->param[0]),       // p_out
                                         (const float *)(msg->param[1]), // p_vec
                                         (signed int)(msg->param[2])     // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_softmax_f32_f32 == 1
        case SRTM_Command_xa_nn_vec_softmax_f32_f32:
            rc = xa_nn_vec_softmax_f32_f32((float *)(msg->param[0]),       // p_out
                                           (const float *)(msg->param[1]), // p_vec
                                           (signed int)(msg->param[2])     // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_sigmoid_32_16 == 1
        case SRTM_Command_xa_nn_vec_sigmoid_32_16:
            rc = xa_nn_vec_sigmoid_32_16((signed short *)(msg->param[0]),     // p_out
                                         (const signed int *)(msg->param[1]), // p_vec
                                         (signed int)(msg->param[2])          // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_tanh_32_16 == 1
        case SRTM_Command_xa_nn_vec_tanh_32_16:
            rc = xa_nn_vec_tanh_32_16((signed short *)(msg->param[0]),     // p_out
                                      (const signed int *)(msg->param[1]), // p_vec
                                      (signed int)(msg->param[2])          // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_sigmoid_32_8 == 1
        case SRTM_Command_xa_nn_vec_sigmoid_32_8:
            rc = xa_nn_vec_sigmoid_32_8((signed char *)(msg->param[0]),      // p_out
                                        (const signed int *)(msg->param[1]), // p_vec
                                        (signed int)(msg->param[2])          // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_tanh_32_8 == 1
        case SRTM_Command_xa_nn_vec_tanh_32_8:
            rc = xa_nn_vec_tanh_32_8((signed char *)(msg->param[0]),      // p_out
                                     (const signed int *)(msg->param[1]), // p_vec
                                     (signed int)(msg->param[2])          // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_interpolation_q15 == 1
        case SRTM_Command_xa_nn_vec_interpolation_q15:
            rc = xa_nn_vec_interpolation_q15((signed short *)(msg->param[0]),       // p_out
                                             (const signed short *)(msg->param[1]), // p_ifact
                                             (const signed short *)(msg->param[2]), // p_inp1
                                             (const signed short *)(msg->param[3]), // p_inp2
                                             (signed int)(msg->param[4])            // num_elements
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
            rc = xa_nn_conv1d_std_8x16((signed short *)(msg->param[0]), // p_out
                                       (signed short *)(msg->param[1]), // p_inp
                                       (signed char *)(msg->param[2]),  // p_kernel
                                       (signed short *)(msg->param[3]), // p_bias
                                       (signed int)(msg->param[4]),     // input_height
                                       (signed int)(msg->param[5]),     // input_width
                                       (signed int)(msg->param[6]),     // input_channels
                                       (signed int)(msg->param[7]),     // kernel_height
                                       (signed int)(msg->param[8]),     // out_channels
                                       (signed int)(msg->param[9]),     // y_stride
                                       (signed int)(msg->param[10]),    // y_padding
                                       (signed int)(msg->param[11]),    // out_height
                                       (signed int)(msg->param[12]),    // bias_shift
                                       (signed int)(msg->param[13]),    // acc_shift
                                       (signed int)(msg->param[14]),    // out_data_format
                                       (void *)(msg->param[15])         // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv1d_std_8x8 == 1
        case SRTM_Command_xa_nn_conv1d_std_8x8:
            rc = xa_nn_conv1d_std_8x8((signed char *)(msg->param[0]), // p_out
                                      (signed char *)(msg->param[1]), // p_inp
                                      (signed char *)(msg->param[2]), // p_kernel
                                      (signed char *)(msg->param[3]), // p_bias
                                      (signed int)(msg->param[4]),    // input_height
                                      (signed int)(msg->param[5]),    // input_width
                                      (signed int)(msg->param[6]),    // input_channels
                                      (signed int)(msg->param[7]),    // kernel_height
                                      (signed int)(msg->param[8]),    // out_channels
                                      (signed int)(msg->param[9]),    // y_stride
                                      (signed int)(msg->param[10]),   // y_padding
                                      (signed int)(msg->param[11]),   // out_height
                                      (signed int)(msg->param[12]),   // bias_shift
                                      (signed int)(msg->param[13]),   // acc_shift
                                      (signed int)(msg->param[14]),   // out_data_format
                                      (void *)(msg->param[15])        // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv1d_std_16x16 == 1
        case SRTM_Command_xa_nn_conv1d_std_16x16:
            rc = xa_nn_conv1d_std_16x16((signed short *)(msg->param[0]), // p_out
                                        (signed short *)(msg->param[1]), // p_inp
                                        (signed short *)(msg->param[2]), // p_kernel
                                        (signed short *)(msg->param[3]), // p_bias
                                        (signed int)(msg->param[4]),     // input_height
                                        (signed int)(msg->param[5]),     // input_width
                                        (signed int)(msg->param[6]),     // input_channels
                                        (signed int)(msg->param[7]),     // kernel_height
                                        (signed int)(msg->param[8]),     // out_channels
                                        (signed int)(msg->param[9]),     // y_stride
                                        (signed int)(msg->param[10]),    // y_padding
                                        (signed int)(msg->param[11]),    // out_height
                                        (signed int)(msg->param[12]),    // bias_shift
                                        (signed int)(msg->param[13]),    // acc_shift
                                        (signed int)(msg->param[14]),    // out_data_format
                                        (void *)(msg->param[15])         // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv1d_std_f32 == 1
        case SRTM_Command_xa_nn_conv1d_std_f32:
            rc = xa_nn_conv1d_std_f32((float *)(msg->param[0]),     // p_out
                                      (float *)(msg->param[1]),     // p_inp
                                      (float *)(msg->param[2]),     // p_kernel
                                      (float *)(msg->param[3]),     // p_bias
                                      (signed int)(msg->param[4]),  // input_height
                                      (signed int)(msg->param[5]),  // input_width
                                      (signed int)(msg->param[6]),  // input_channels
                                      (signed int)(msg->param[7]),  // kernel_height
                                      (signed int)(msg->param[8]),  // out_channels
                                      (signed int)(msg->param[9]),  // y_stride
                                      (signed int)(msg->param[10]), // y_padding
                                      (signed int)(msg->param[11]), // out_height
                                      (signed int)(msg->param[12]), // out_data_format
                                      (void *)(msg->param[13])      // p_handle
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
                                          (signed int)(msg->param[7])  // input_precision
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_std_8x16 == 1
        case SRTM_Command_xa_nn_conv2d_std_8x16:
            rc = xa_nn_conv2d_std_8x16((signed short *)(msg->param[0]), // p_out
                                       (signed short *)(msg->param[1]), // p_inp
                                       (signed char *)(msg->param[2]),  // p_kernel
                                       (signed short *)(msg->param[3]), // p_bias
                                       (signed int)(msg->param[4]),     // input_height
                                       (signed int)(msg->param[5]),     // input_width
                                       (signed int)(msg->param[6]),     // input_channels
                                       (signed int)(msg->param[7]),     // kernel_height
                                       (signed int)(msg->param[8]),     // kernel_width
                                       (signed int)(msg->param[9]),     // out_channels
                                       (signed int)(msg->param[10]),    // x_stride
                                       (signed int)(msg->param[11]),    // y_stride
                                       (signed int)(msg->param[12]),    // x_padding
                                       (signed int)(msg->param[13]),    // y_padding
                                       (signed int)(msg->param[14]),    // out_height
                                       (signed int)(msg->param[15]),    // out_width
                                       (signed int)(msg->param[16]),    // bias_shift
                                       (signed int)(msg->param[17]),    // acc_shift
                                       (signed int)(msg->param[18]),    // out_data_format
                                       (void *)(msg->param[19])         // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_std_8x8 == 1
        case SRTM_Command_xa_nn_conv2d_std_8x8:
            rc = xa_nn_conv2d_std_8x8((signed char *)(msg->param[0]), // p_out
                                      (signed char *)(msg->param[1]), // p_inp
                                      (signed char *)(msg->param[2]), // p_kernel
                                      (signed char *)(msg->param[3]), // p_bias
                                      (signed int)(msg->param[4]),    // input_height
                                      (signed int)(msg->param[5]),    // input_width
                                      (signed int)(msg->param[6]),    // input_channels
                                      (signed int)(msg->param[7]),    // kernel_height
                                      (signed int)(msg->param[8]),    // kernel_width
                                      (signed int)(msg->param[9]),    // out_channels
                                      (signed int)(msg->param[10]),   // x_stride
                                      (signed int)(msg->param[11]),   // y_stride
                                      (signed int)(msg->param[12]),   // x_padding
                                      (signed int)(msg->param[13]),   // y_padding
                                      (signed int)(msg->param[14]),   // out_height
                                      (signed int)(msg->param[15]),   // out_width
                                      (signed int)(msg->param[16]),   // bias_shift
                                      (signed int)(msg->param[17]),   // acc_shift
                                      (signed int)(msg->param[18]),   // out_data_format
                                      (void *)(msg->param[19])        // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_std_16x16 == 1
        case SRTM_Command_xa_nn_conv2d_std_16x16:
            rc = xa_nn_conv2d_std_16x16((signed short *)(msg->param[0]), // p_out
                                        (signed short *)(msg->param[1]), // p_inp
                                        (signed short *)(msg->param[2]), // p_kernel
                                        (signed short *)(msg->param[3]), // p_bias
                                        (signed int)(msg->param[4]),     // input_height
                                        (signed int)(msg->param[5]),     // input_width
                                        (signed int)(msg->param[6]),     // input_channels
                                        (signed int)(msg->param[7]),     // kernel_height
                                        (signed int)(msg->param[8]),     // kernel_width
                                        (signed int)(msg->param[9]),     // out_channels
                                        (signed int)(msg->param[10]),    // x_stride
                                        (signed int)(msg->param[11]),    // y_stride
                                        (signed int)(msg->param[12]),    // x_padding
                                        (signed int)(msg->param[13]),    // y_padding
                                        (signed int)(msg->param[14]),    // out_height
                                        (signed int)(msg->param[15]),    // out_width
                                        (signed int)(msg->param[16]),    // bias_shift
                                        (signed int)(msg->param[17]),    // acc_shift
                                        (signed int)(msg->param[18]),    // out_data_format
                                        (void *)(msg->param[19])         // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_std_f32 == 1
        case SRTM_Command_xa_nn_conv2d_std_f32:
            rc = xa_nn_conv2d_std_f32((float *)(msg->param[0]),       // p_out
                                      (const float *)(msg->param[1]), // p_inp
                                      (const float *)(msg->param[2]), // p_kernel
                                      (const float *)(msg->param[3]), // p_bias
                                      (signed int)(msg->param[4]),    // input_height
                                      (signed int)(msg->param[5]),    // input_width
                                      (signed int)(msg->param[6]),    // input_channels
                                      (signed int)(msg->param[7]),    // kernel_height
                                      (signed int)(msg->param[8]),    // kernel_width
                                      (signed int)(msg->param[9]),    // out_channels
                                      (signed int)(msg->param[10]),   // x_stride
                                      (signed int)(msg->param[11]),   // y_stride
                                      (signed int)(msg->param[12]),   // x_padding
                                      (signed int)(msg->param[13]),   // y_padding
                                      (signed int)(msg->param[14]),   // out_height
                                      (signed int)(msg->param[15]),   // out_width
                                      (signed int)(msg->param[16]),   // out_data_format
                                      (void *)(msg->param[17])        // p_handle
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_pointwise_f32 == 1
        case SRTM_Command_xa_nn_conv2d_pointwise_f32:
            rc = xa_nn_conv2d_pointwise_f32((float *)(msg->param[0]),    // p_out
                                            (float *)(msg->param[1]),    // p_kernel
                                            (float *)(msg->param[2]),    // p_inp
                                            (float *)(msg->param[3]),    // p_bias
                                            (signed int)(msg->param[4]), // input_height
                                            (signed int)(msg->param[5]), // input_width
                                            (signed int)(msg->param[6]), // input_channels
                                            (signed int)(msg->param[7]), // out_channels
                                            (signed int)(msg->param[8])  // out_data_format
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_pointwise_8x16 == 1
        case SRTM_Command_xa_nn_conv2d_pointwise_8x16:
            rc = xa_nn_conv2d_pointwise_8x16((signed short *)(msg->param[0]), // p_out
                                             (signed char *)(msg->param[1]),  // p_kernel
                                             (signed short *)(msg->param[2]), // p_inp
                                             (signed short *)(msg->param[3]), // p_bias
                                             (signed int)(msg->param[4]),     // input_height
                                             (signed int)(msg->param[5]),     // input_width
                                             (signed int)(msg->param[6]),     // input_channels
                                             (signed int)(msg->param[7]),     // out_channels
                                             (signed int)(msg->param[8]),     // acc_shift
                                             (signed int)(msg->param[9]),     // bias_shift
                                             (signed int)(msg->param[10])     // out_data_format
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_pointwise_8x8 == 1
        case SRTM_Command_xa_nn_conv2d_pointwise_8x8:
            rc = xa_nn_conv2d_pointwise_8x8((signed char *)(msg->param[0]), // p_out
                                            (signed char *)(msg->param[1]), // p_kernel
                                            (signed char *)(msg->param[2]), // p_inp
                                            (signed char *)(msg->param[3]), // p_bias
                                            (signed int)(msg->param[4]),    // input_height
                                            (signed int)(msg->param[5]),    // input_width
                                            (signed int)(msg->param[6]),    // input_channels
                                            (signed int)(msg->param[7]),    // out_channels
                                            (signed int)(msg->param[8]),    // acc_shift
                                            (signed int)(msg->param[9]),    // bias_shift
                                            (signed int)(msg->param[10])    // out_data_format
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

#if NN_ENABLE_xa_nn_conv2d_depthwise_f32 == 1
        case SRTM_Command_xa_nn_conv2d_depthwise_f32:
            rc = xa_nn_conv2d_depthwise_f32((float *)(msg->param[0]),       // p_out
                                            (const float *)(msg->param[1]), // p_kernel
                                            (const float *)(msg->param[2]), // p_inp
                                            (const float *)(msg->param[3]), // p_bias
                                            (signed int)(msg->param[4]),    // input_height
                                            (signed int)(msg->param[5]),    // input_width
                                            (signed int)(msg->param[6]),    // input_channels
                                            (signed int)(msg->param[7]),    // kernel_height
                                            (signed int)(msg->param[8]),    // kernel_width
                                            (signed int)(msg->param[9]),    // channels_multiplier
                                            (signed int)(msg->param[10]),   // x_stride
                                            (signed int)(msg->param[11]),   // y_stride
                                            (signed int)(msg->param[12]),   // x_padding
                                            (signed int)(msg->param[13]),   // y_padding
                                            (signed int)(msg->param[14]),   // out_height
                                            (signed int)(msg->param[15]),   // out_width
                                            (signed int)(msg->param[16]),   // inp_data_format
                                            (signed int)(msg->param[17]),   // out_data_format
                                            (void *)(msg->param[18])        // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_depthwise_8x16 == 1
        case SRTM_Command_xa_nn_conv2d_depthwise_8x16:
            rc = xa_nn_conv2d_depthwise_8x16((signed short *)(msg->param[0]),       // p_out
                                             (const signed char *)(msg->param[1]),  // p_kernel
                                             (const signed short *)(msg->param[2]), // p_inp
                                             (const signed short *)(msg->param[3]), // p_bias
                                             (signed int)(msg->param[4]),           // input_height
                                             (signed int)(msg->param[5]),           // input_width
                                             (signed int)(msg->param[6]),           // input_channels
                                             (signed int)(msg->param[7]),           // kernel_height
                                             (signed int)(msg->param[8]),           // kernel_width
                                             (signed int)(msg->param[9]),           // channels_multiplier
                                             (signed int)(msg->param[10]),          // x_stride
                                             (signed int)(msg->param[11]),          // y_stride
                                             (signed int)(msg->param[12]),          // x_padding
                                             (signed int)(msg->param[13]),          // y_padding
                                             (signed int)(msg->param[14]),          // out_height
                                             (signed int)(msg->param[15]),          // out_width
                                             (signed int)(msg->param[16]),          // acc_shift
                                             (signed int)(msg->param[17]),          // bias_shift
                                             (signed int)(msg->param[18]),          // inp_data_format
                                             (signed int)(msg->param[19]),          // out_data_format
                                             (void *)(msg->param[20])               // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_depthwise_8x8 == 1
        case SRTM_Command_xa_nn_conv2d_depthwise_8x8:
            rc = xa_nn_conv2d_depthwise_8x8((signed char *)(msg->param[0]),       // p_out
                                            (const signed char *)(msg->param[1]), // p_kernel
                                            (const signed char *)(msg->param[2]), // p_inp
                                            (const signed char *)(msg->param[3]), // p_bias
                                            (signed int)(msg->param[4]),          // input_height
                                            (signed int)(msg->param[5]),          // input_width
                                            (signed int)(msg->param[6]),          // input_channels
                                            (signed int)(msg->param[7]),          // kernel_height
                                            (signed int)(msg->param[8]),          // kernel_width
                                            (signed int)(msg->param[9]),          // channels_multiplier
                                            (signed int)(msg->param[10]),         // x_stride
                                            (signed int)(msg->param[11]),         // y_stride
                                            (signed int)(msg->param[12]),         // x_padding
                                            (signed int)(msg->param[13]),         // y_padding
                                            (signed int)(msg->param[14]),         // out_height
                                            (signed int)(msg->param[15]),         // out_width
                                            (signed int)(msg->param[16]),         // acc_shift
                                            (signed int)(msg->param[17]),         // bias_shift
                                            (signed int)(msg->param[18]),         // inp_data_format
                                            (signed int)(msg->param[19]),         // out_data_format
                                            (void *)(msg->param[20])              // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_depthwise_16x16 == 1
        case SRTM_Command_xa_nn_conv2d_depthwise_16x16:
            rc = xa_nn_conv2d_depthwise_16x16((signed short *)(msg->param[0]),       // p_out
                                              (const signed short *)(msg->param[1]), // p_kernel
                                              (const signed short *)(msg->param[2]), // p_inp
                                              (const signed short *)(msg->param[3]), // p_bias
                                              (signed int)(msg->param[4]),           // input_height
                                              (signed int)(msg->param[5]),           // input_width
                                              (signed int)(msg->param[6]),           // input_channels
                                              (signed int)(msg->param[7]),           // kernel_height
                                              (signed int)(msg->param[8]),           // kernel_width
                                              (signed int)(msg->param[9]),           // channels_multiplier
                                              (signed int)(msg->param[10]),          // x_stride
                                              (signed int)(msg->param[11]),          // y_stride
                                              (signed int)(msg->param[12]),          // x_padding
                                              (signed int)(msg->param[13]),          // y_padding
                                              (signed int)(msg->param[14]),          // out_height
                                              (signed int)(msg->param[15]),          // out_width
                                              (signed int)(msg->param[16]),          // acc_shift
                                              (signed int)(msg->param[17]),          // bias_shift
                                              (signed int)(msg->param[18]),          // inp_data_format
                                              (signed int)(msg->param[19]),          // out_data_format
                                              (void *)(msg->param[20])               // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_pointwise_16x16 == 1
        case SRTM_Command_xa_nn_conv2d_pointwise_16x16:
            rc = xa_nn_conv2d_pointwise_16x16((signed short *)(msg->param[0]), // p_out
                                              (signed short *)(msg->param[1]), // p_kernel
                                              (signed short *)(msg->param[2]), // p_inp
                                              (signed short *)(msg->param[3]), // p_bias
                                              (signed int)(msg->param[4]),     // input_height
                                              (signed int)(msg->param[5]),     // input_width
                                              (signed int)(msg->param[6]),     // input_channels
                                              (signed int)(msg->param[7]),     // out_channels
                                              (signed int)(msg->param[8]),     // acc_shift
                                              (signed int)(msg->param[9]),     // bias_shift
                                              (signed int)(msg->param[10])     // out_data_format
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_avgpool_8 == 1
        case SRTM_Command_xa_nn_avgpool_8:
            rc = xa_nn_avgpool_8((signed char *)(msg->param[0]), // p_out
                                 (signed char *)(msg->param[1]), // p_inp
                                 (signed int)(msg->param[2]),    // input_height
                                 (signed int)(msg->param[3]),    // input_width
                                 (signed int)(msg->param[4]),    // input_channels
                                 (signed int)(msg->param[5]),    // kernel_height
                                 (signed int)(msg->param[6]),    // kernel_width
                                 (signed int)(msg->param[7]),    // x_stride
                                 (signed int)(msg->param[8]),    // y_stride
                                 (signed int)(msg->param[9]),    // x_padding
                                 (signed int)(msg->param[10]),   // y_padding
                                 (signed int)(msg->param[11]),   // out_height
                                 (signed int)(msg->param[12]),   // out_width
                                 (signed int)(msg->param[13]),   // inp_data_format
                                 (signed int)(msg->param[14]),   // out_data_format
                                 (void *)(msg->param[15])        // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_avgpool_16 == 1
        case SRTM_Command_xa_nn_avgpool_16:
            rc = xa_nn_avgpool_16((signed short *)(msg->param[0]), // p_out
                                  (signed short *)(msg->param[1]), // p_inp
                                  (signed int)(msg->param[2]),     // input_height
                                  (signed int)(msg->param[3]),     // input_width
                                  (signed int)(msg->param[4]),     // input_channels
                                  (signed int)(msg->param[5]),     // kernel_height
                                  (signed int)(msg->param[6]),     // kernel_width
                                  (signed int)(msg->param[7]),     // x_stride
                                  (signed int)(msg->param[8]),     // y_stride
                                  (signed int)(msg->param[9]),     // x_padding
                                  (signed int)(msg->param[10]),    // y_padding
                                  (signed int)(msg->param[11]),    // out_height
                                  (signed int)(msg->param[12]),    // out_width
                                  (signed int)(msg->param[13]),    // inp_data_format
                                  (signed int)(msg->param[14]),    // out_data_format
                                  (void *)(msg->param[15])         // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_avgpool_f32 == 1
        case SRTM_Command_xa_nn_avgpool_f32:
            rc = xa_nn_avgpool_f32((float *)(msg->param[0]),       // p_out
                                   (const float *)(msg->param[1]), // p_inp
                                   (signed int)(msg->param[2]),    // input_height
                                   (signed int)(msg->param[3]),    // input_width
                                   (signed int)(msg->param[4]),    // input_channels
                                   (signed int)(msg->param[5]),    // kernel_height
                                   (signed int)(msg->param[6]),    // kernel_width
                                   (signed int)(msg->param[7]),    // x_stride
                                   (signed int)(msg->param[8]),    // y_stride
                                   (signed int)(msg->param[9]),    // x_padding
                                   (signed int)(msg->param[10]),   // y_padding
                                   (signed int)(msg->param[11]),   // out_height
                                   (signed int)(msg->param[12]),   // out_width
                                   (signed int)(msg->param[13]),   // inp_data_format
                                   (signed int)(msg->param[14]),   // out_data_format
                                   (void *)(msg->param[15])        // p_scratch
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

#if NN_ENABLE_xa_nn_maxpool_f32 == 1
        case SRTM_Command_xa_nn_maxpool_f32:
            rc = xa_nn_maxpool_f32((float *)(msg->param[0]),       // p_out
                                   (const float *)(msg->param[1]), // p_inp
                                   (signed int)(msg->param[2]),    // input_height
                                   (signed int)(msg->param[3]),    // input_width
                                   (signed int)(msg->param[4]),    // input_channels
                                   (signed int)(msg->param[5]),    // kernel_height
                                   (signed int)(msg->param[6]),    // kernel_width
                                   (signed int)(msg->param[7]),    // x_stride
                                   (signed int)(msg->param[8]),    // y_stride
                                   (signed int)(msg->param[9]),    // x_padding
                                   (signed int)(msg->param[10]),   // y_padding
                                   (signed int)(msg->param[11]),   // out_height
                                   (signed int)(msg->param[12]),   // out_width
                                   (signed int)(msg->param[13]),   // inp_data_format
                                   (signed int)(msg->param[14]),   // out_data_format
                                   (void *)(msg->param[15])        // p_scratch
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

#if NN_ENABLE_xa_nn_maxpool_8 == 1
        case SRTM_Command_xa_nn_maxpool_8:
            rc = xa_nn_maxpool_8((signed char *)(msg->param[0]), // p_out
                                 (signed char *)(msg->param[1]), // p_inp
                                 (signed int)(msg->param[2]),    // input_height
                                 (signed int)(msg->param[3]),    // input_width
                                 (signed int)(msg->param[4]),    // input_channels
                                 (signed int)(msg->param[5]),    // kernel_height
                                 (signed int)(msg->param[6]),    // kernel_width
                                 (signed int)(msg->param[7]),    // x_stride
                                 (signed int)(msg->param[8]),    // y_stride
                                 (signed int)(msg->param[9]),    // x_padding
                                 (signed int)(msg->param[10]),   // y_padding
                                 (signed int)(msg->param[11]),   // out_height
                                 (signed int)(msg->param[12]),   // out_width
                                 (signed int)(msg->param[13]),   // inp_data_format
                                 (signed int)(msg->param[14]),   // out_data_format
                                 (void *)(msg->param[15])        // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_maxpool_16 == 1
        case SRTM_Command_xa_nn_maxpool_16:
            rc = xa_nn_maxpool_16((signed short *)(msg->param[0]), // p_out
                                  (signed short *)(msg->param[1]), // p_inp
                                  (signed int)(msg->param[2]),     // input_height
                                  (signed int)(msg->param[3]),     // input_width
                                  (signed int)(msg->param[4]),     // input_channels
                                  (signed int)(msg->param[5]),     // kernel_height
                                  (signed int)(msg->param[6]),     // kernel_width
                                  (signed int)(msg->param[7]),     // x_stride
                                  (signed int)(msg->param[8]),     // y_stride
                                  (signed int)(msg->param[9]),     // x_padding
                                  (signed int)(msg->param[10]),    // y_padding
                                  (signed int)(msg->param[11]),    // out_height
                                  (signed int)(msg->param[12]),    // out_width
                                  (signed int)(msg->param[13]),    // inp_data_format
                                  (signed int)(msg->param[14]),    // out_data_format
                                  (void *)(msg->param[15])         // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_fully_connected_f32 == 1
        case SRTM_Command_xa_nn_fully_connected_f32:
            rc = xa_nn_fully_connected_f32((float *)(msg->param[0]),       // p_out
                                           (const float *)(msg->param[1]), // p_weight
                                           (const float *)(msg->param[2]), // p_inp
                                           (const float *)(msg->param[3]), // p_bias
                                           (signed int)(msg->param[4]),    // weight_depth
                                           (signed int)(msg->param[5])     // out_depth
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_fully_connected_16x16_16 == 1
        case SRTM_Command_xa_nn_fully_connected_16x16_16:
            rc = xa_nn_fully_connected_16x16_16((signed short *)(msg->param[0]), // p_out
                                                (signed short *)(msg->param[1]), // p_weight
                                                (signed short *)(msg->param[2]), // p_inp
                                                (signed short *)(msg->param[3]), // p_bias
                                                (signed int)(msg->param[4]),     // weight_depth
                                                (signed int)(msg->param[5]),     // out_depth
                                                (signed int)(msg->param[6]),     // acc_shift
                                                (signed int)(msg->param[7])      // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_fully_connected_8x16_16 == 1
        case SRTM_Command_xa_nn_fully_connected_8x16_16:
            rc = xa_nn_fully_connected_8x16_16((signed short *)(msg->param[0]), // p_out
                                               (signed char *)(msg->param[1]),  // p_weight
                                               (signed short *)(msg->param[2]), // p_inp
                                               (signed short *)(msg->param[3]), // p_bias
                                               (signed int)(msg->param[4]),     // weight_depth
                                               (signed int)(msg->param[5]),     // out_depth
                                               (signed int)(msg->param[6]),     // acc_shift
                                               (signed int)(msg->param[7])      // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_fully_connected_8x8_8 == 1
        case SRTM_Command_xa_nn_fully_connected_8x8_8:
            rc = xa_nn_fully_connected_8x8_8((signed char *)(msg->param[0]), // p_out
                                             (signed char *)(msg->param[1]), // p_weight
                                             (signed char *)(msg->param[2]), // p_inp
                                             (signed char *)(msg->param[3]), // p_bias
                                             (signed int)(msg->param[4]),    // weight_depth
                                             (signed int)(msg->param[5]),    // out_depth
                                             (signed int)(msg->param[6]),    // acc_shift
                                             (signed int)(msg->param[7])     // bias_shift
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_activation_min_max_asym8_asym8 == 1
        case SRTM_Command_xa_nn_vec_activation_min_max_asym8_asym8:
            rc = xa_nn_vec_activation_min_max_asym8_asym8((unsigned char *)(msg->param[0]),       // p_out
                                                          (const unsigned char *)(msg->param[1]), // p_vec
                                                          (int)(msg->param[2]),                   // activation_min
                                                          (int)(msg->param[3]),                   // activation_max
                                                          (signed int)(msg->param[4])             // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv1d_std_asym8xasym8 == 1
        case SRTM_Command_xa_nn_conv1d_std_asym8xasym8:
            rc = xa_nn_conv1d_std_asym8xasym8((unsigned char *)(msg->param[0]), // p_out
                                              (unsigned char *)(msg->param[1]), // p_inp
                                              (unsigned char *)(msg->param[2]), // p_kernel
                                              (signed int *)(msg->param[3]),    // p_bias
                                              (signed int)(msg->param[4]),      // input_height
                                              (signed int)(msg->param[5]),      // input_width
                                              (signed int)(msg->param[6]),      // input_channels
                                              (signed int)(msg->param[7]),      // kernel_height
                                              (signed int)(msg->param[8]),      // out_channels
                                              (signed int)(msg->param[9]),      // y_stride
                                              (signed int)(msg->param[10]),     // y_padding
                                              (signed int)(msg->param[11]),     // out_height
                                              (signed int)(msg->param[12]),     // input_zero_bias
                                              (signed int)(msg->param[13]),     // kernel_zero_bias
                                              (signed int)(msg->param[14]),     // out_multiplier
                                              (signed int)(msg->param[15]),     // out_shift
                                              (signed int)(msg->param[16]),     // out_zero_bias
                                              (signed int)(msg->param[17]),     // out_data_format
                                              (void *)(msg->param[18])          // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_std_asym8xasym8 == 1
        case SRTM_Command_xa_nn_conv2d_std_asym8xasym8:
            rc = xa_nn_conv2d_std_asym8xasym8((unsigned char *)(msg->param[0]),       // p_out
                                              (const unsigned char *)(msg->param[1]), // p_inp
                                              (const unsigned char *)(msg->param[2]), // p_kernel
                                              (const signed int *)(msg->param[3]),    // p_bias
                                              (signed int)(msg->param[4]),            // input_height
                                              (signed int)(msg->param[5]),            // input_width
                                              (signed int)(msg->param[6]),            // input_channels
                                              (signed int)(msg->param[7]),            // kernel_height
                                              (signed int)(msg->param[8]),            // kernel_width
                                              (signed int)(msg->param[9]),            // out_channels
                                              (signed int)(msg->param[10]),           // x_stride
                                              (signed int)(msg->param[11]),           // y_stride
                                              (signed int)(msg->param[12]),           // x_padding
                                              (signed int)(msg->param[13]),           // y_padding
                                              (signed int)(msg->param[14]),           // out_height
                                              (signed int)(msg->param[15]),           // out_width
                                              (signed int)(msg->param[16]),           // input_zero_bias
                                              (signed int)(msg->param[17]),           // kernel_zero_bias
                                              (signed int)(msg->param[18]),           // out_multiplier
                                              (signed int)(msg->param[19]),           // out_shift
                                              (signed int)(msg->param[20]),           // out_zero_bias
                                              (signed int)(msg->param[21]),           // out_data_format
                                              (void *)(msg->param[22])                // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_asym8xasym8_asym8 == 1
        case SRTM_Command_xa_nn_matXvec_asym8xasym8_asym8:
            rc = xa_nn_matXvec_asym8xasym8_asym8((unsigned char *)(msg->param[0]),       // p_out
                                                 (const unsigned char *)(msg->param[1]), // p_mat1
                                                 (const unsigned char *)(msg->param[2]), // p_mat2
                                                 (const unsigned char *)(msg->param[3]), // p_vec1
                                                 (const unsigned char *)(msg->param[4]), // p_vec2
                                                 (const signed int *)(msg->param[5]),    // p_bias
                                                 (signed int)(msg->param[6]),            // rows
                                                 (signed int)(msg->param[7]),            // cols1
                                                 (signed int)(msg->param[8]),            // cols2
                                                 (signed int)(msg->param[9]),            // row_stride1
                                                 (signed int)(msg->param[10]),           // row_stride2
                                                 (signed int)(msg->param[11]),           // mat1_zero_bias
                                                 (signed int)(msg->param[12]),           // mat2_zero_bias
                                                 (signed int)(msg->param[13]),           // vec1_zero_bias
                                                 (signed int)(msg->param[14]),           // vec2_zero_bias
                                                 (signed int)(msg->param[15]),           // out_multiplier
                                                 (signed int)(msg->param[16]),           // out_shift
                                                 (signed int)(msg->param[17])            // out_zero_bias
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_matXvec_batch_asym8xasym8_asym8 == 1
        case SRTM_Command_xa_nn_matXvec_batch_asym8xasym8_asym8:
            rc = xa_nn_matXvec_batch_asym8xasym8_asym8((unsigned char **)(msg->param[0]), // p_out
                                                       (unsigned char *)(msg->param[1]),  // p_mat1
                                                       (unsigned char **)(msg->param[2]), // p_vec1
                                                       (signed int *)(msg->param[3]),     // p_bias
                                                       (signed int)(msg->param[4]),       // rows
                                                       (signed int)(msg->param[5]),       // cols1
                                                       (signed int)(msg->param[6]),       // row_stride1
                                                       (signed int)(msg->param[7]),       // vec_count
                                                       (signed int)(msg->param[8]),       // mat1_zero_bias
                                                       (signed int)(msg->param[9]),       // vec1_zero_bias
                                                       (signed int)(msg->param[10]),      // out_multiplier
                                                       (signed int)(msg->param[11]),      // out_shift
                                                       (signed int)(msg->param[12])       // out_zero_bias
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_depthwise_asym8xasym8 == 1
        case SRTM_Command_xa_nn_conv2d_depthwise_asym8xasym8:
            rc = xa_nn_conv2d_depthwise_asym8xasym8((punsigned char)(msg->param[0]),        // p_out
                                                    (const unsigned char *)(msg->param[1]), // p_kernel
                                                    (const unsigned char *)(msg->param[2]), // p_inp
                                                    (const signed int *)(msg->param[3]),    // p_bias
                                                    (signed int)(msg->param[4]),            // input_height
                                                    (signed int)(msg->param[5]),            // input_width
                                                    (signed int)(msg->param[6]),            // input_channels
                                                    (signed int)(msg->param[7]),            // kernel_height
                                                    (signed int)(msg->param[8]),            // kernel_width
                                                    (signed int)(msg->param[9]),            // channels_multiplier
                                                    (signed int)(msg->param[10]),           // x_stride
                                                    (signed int)(msg->param[11]),           // y_stride
                                                    (signed int)(msg->param[12]),           // x_padding
                                                    (signed int)(msg->param[13]),           // y_padding
                                                    (signed int)(msg->param[14]),           // out_height
                                                    (signed int)(msg->param[15]),           // out_width
                                                    (signed int)(msg->param[16]),           // input_zero_bias
                                                    (signed int)(msg->param[17]),           // kernel_zero_bias
                                                    (signed int)(msg->param[18]),           // out_multiplier
                                                    (signed int)(msg->param[19]),           // out_shift
                                                    (signed int)(msg->param[20]),           // out_zero_bias
                                                    (signed int)(msg->param[21]),           // inp_data_format
                                                    (signed int)(msg->param[22]),           // out_data_format
                                                    (void *)(msg->param[23])                // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_conv2d_pointwise_asym8xasym8 == 1
        case SRTM_Command_xa_nn_conv2d_pointwise_asym8xasym8:
            rc = xa_nn_conv2d_pointwise_asym8xasym8((punsigned char)(msg->param[0]), // p_out
                                                    (punsigned char)(msg->param[1]), // p_kernel
                                                    (punsigned char)(msg->param[2]), // p_inp
                                                    (signed int *)(msg->param[3]),   // p_bias
                                                    (signed int)(msg->param[4]),     // input_height
                                                    (signed int)(msg->param[5]),     // input_width
                                                    (signed int)(msg->param[6]),     // input_channels
                                                    (signed int)(msg->param[7]),     // out_channels
                                                    (signed int)(msg->param[8]),     // input_zero_bias
                                                    (signed int)(msg->param[9]),     // kernel_zero_bias
                                                    (signed int)(msg->param[10]),    // out_multiplier
                                                    (signed int)(msg->param[11]),    // out_shift
                                                    (signed int)(msg->param[12]),    // out_zero_bias
                                                    (signed int)(msg->param[13])     // out_data_format
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_activation_min_max_f32_f32 == 1
        case SRTM_Command_xa_nn_vec_activation_min_max_f32_f32:
            rc = xa_nn_vec_activation_min_max_f32_f32((float *)(msg->param[0]),       // p_out
                                                      (const float *)(msg->param[1]), // p_vec
                                                      (float)(msg->param[2]),         // activation_min
                                                      (float)(msg->param[3]),         // activation_max
                                                      (signed int)(msg->param[4])     // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_maxpool_asym8 == 1
        case SRTM_Command_xa_nn_maxpool_asym8:
            rc = xa_nn_maxpool_asym8((unsigned char *)(msg->param[0]),       // p_out
                                     (const unsigned char *)(msg->param[1]), // p_inp
                                     (signed int)(msg->param[2]),            // input_height
                                     (signed int)(msg->param[3]),            // input_width
                                     (signed int)(msg->param[4]),            // input_channels
                                     (signed int)(msg->param[5]),            // kernel_height
                                     (signed int)(msg->param[6]),            // kernel_width
                                     (signed int)(msg->param[7]),            // x_stride
                                     (signed int)(msg->param[8]),            // y_stride
                                     (signed int)(msg->param[9]),            // x_padding
                                     (signed int)(msg->param[10]),           // y_padding
                                     (signed int)(msg->param[11]),           // out_height
                                     (signed int)(msg->param[12]),           // out_width
                                     (signed int)(msg->param[13]),           // inp_data_format
                                     (signed int)(msg->param[14]),           // out_data_format
                                     (void *)(msg->param[15])                // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_avgpool_asym8 == 1
        case SRTM_Command_xa_nn_avgpool_asym8:
            rc = xa_nn_avgpool_asym8((unsigned char *)(msg->param[0]),       // p_out
                                     (const unsigned char *)(msg->param[1]), // p_inp
                                     (signed int)(msg->param[2]),            // input_height
                                     (signed int)(msg->param[3]),            // input_width
                                     (signed int)(msg->param[4]),            // input_channels
                                     (signed int)(msg->param[5]),            // kernel_height
                                     (signed int)(msg->param[6]),            // kernel_width
                                     (signed int)(msg->param[7]),            // x_stride
                                     (signed int)(msg->param[8]),            // y_stride
                                     (signed int)(msg->param[9]),            // x_padding
                                     (signed int)(msg->param[10]),           // y_padding
                                     (signed int)(msg->param[11]),           // out_height
                                     (signed int)(msg->param[12]),           // out_width
                                     (signed int)(msg->param[13]),           // inp_data_format
                                     (signed int)(msg->param[14]),           // out_data_format
                                     (void *)(msg->param[15])                // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_fully_connected_asym8xasym8_asym8 == 1
        case SRTM_Command_xa_nn_fully_connected_asym8xasym8_asym8:
            rc = xa_nn_fully_connected_asym8xasym8_asym8((punsigned char)(msg->param[0]),        // p_out
                                                         (const unsigned char *)(msg->param[1]), // p_weight
                                                         (const unsigned char *)(msg->param[2]), // p_inp
                                                         (const signed int *)(msg->param[3]),    // p_bias
                                                         (signed int)(msg->param[4]),            // weight_depth
                                                         (signed int)(msg->param[5]),            // out_depth
                                                         (signed int)(msg->param[6]),            // input_zero_bias
                                                         (signed int)(msg->param[7]),            // weight_zero_bias
                                                         (signed int)(msg->param[8]),            // out_multiplier
                                                         (signed int)(msg->param[9]),            // out_shift
                                                         (signed int)(msg->param[10])            // out_zero_bias
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_mul_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_elm_mul_f32xf32_f32:
            rc = xa_nn_elm_mul_f32xf32_f32((float *)(msg->param[0]),       // p_out
                                           (const float *)(msg->param[1]), // p_inp1
                                           (const float *)(msg->param[2]), // p_inp2
                                           (signed int)(msg->param[3])     // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_add_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_elm_add_f32xf32_f32:
            rc = xa_nn_elm_add_f32xf32_f32((float *)(msg->param[0]),       // p_out
                                           (const float *)(msg->param[1]), // p_inp1
                                           (const float *)(msg->param[2]), // p_inp2
                                           (signed int)(msg->param[3])     // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_mul_acc_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_elm_mul_acc_f32xf32_f32:
            rc = xa_nn_elm_mul_acc_f32xf32_f32((float *)(msg->param[0]),       // p_out
                                               (const float *)(msg->param[1]), // p_inp1
                                               (const float *)(msg->param[2]), // p_inp2
                                               (signed int)(msg->param[3])     // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_sub_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_elm_sub_f32xf32_f32:
            rc = xa_nn_elm_sub_f32xf32_f32((float *)(msg->param[0]),       // p_out
                                           (const float *)(msg->param[1]), // p_inp1
                                           (const float *)(msg->param[2]), // p_inp2
                                           (signed int)(msg->param[3])     // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_div_f32xf32_f32 == 1
        case SRTM_Command_xa_nn_elm_div_f32xf32_f32:
            rc = xa_nn_elm_div_f32xf32_f32((float *)(msg->param[0]),       // p_out
                                           (const float *)(msg->param[1]), // p_inp1
                                           (const float *)(msg->param[2]), // p_inp2
                                           (signed int)(msg->param[3])     // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_floor_f32_f32 == 1
        case SRTM_Command_xa_nn_elm_floor_f32_f32:
            rc = xa_nn_elm_floor_f32_f32((float *)(msg->param[0]),       // p_out
                                         (const float *)(msg->param[1]), // p_inp
                                         (signed int)(msg->param[2])     // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_add_asym8xasym8_asym8 == 1
        case SRTM_Command_xa_nn_elm_add_asym8xasym8_asym8:
            rc = xa_nn_elm_add_asym8xasym8_asym8((unsigned char *)(msg->param[0]),        // p_out
                                                 (signed int)(msg->param[1]),             // out_zero_bias
                                                 (signed int)(msg->param[2]),             // out_left_shift
                                                 (signed int)(msg->param[3]),             // out_multiplier
                                                 (signed int)(msg->param[4]),             // out_activation_min
                                                 (signed int)(msg->param[5]),             // out_activation_max
                                                 (const unsigned char *)(msg->param[6]),  // p_inp1
                                                 (signed int)(msg->param[7]),             // inp1_zero_bias
                                                 (signed int)(msg->param[8]),             // inp1_left_shift
                                                 (signed int)(msg->param[9]),             // inp1_multiplier
                                                 (const unsigned char *)(msg->param[10]), // p_inp2
                                                 (signed int)(msg->param[11]),            // inp2_zero_bias
                                                 (signed int)(msg->param[12]),            // inp2_left_shift
                                                 (signed int)(msg->param[13]),            // inp2_multiplier
                                                 (signed int)(msg->param[14]),            // left_shift
                                                 (signed int)(msg->param[15])             // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_elm_mul_asym8xasym8_asym8 == 1
        case SRTM_Command_xa_nn_elm_mul_asym8xasym8_asym8:
            rc = xa_nn_elm_mul_asym8xasym8_asym8((unsigned char *)(msg->param[0]),       // p_out
                                                 (signed int)(msg->param[1]),            // out_zero_bias
                                                 (signed int)(msg->param[2]),            // out_shift
                                                 (signed int)(msg->param[3]),            // out_multiplier
                                                 (signed int)(msg->param[4]),            // out_activation_min
                                                 (signed int)(msg->param[5]),            // out_activation_max
                                                 (const unsigned char *)(msg->param[6]), // p_inp1
                                                 (signed int)(msg->param[7]),            // inp1_zero_bias
                                                 (const unsigned char *)(msg->param[8]), // p_inp2
                                                 (signed int)(msg->param[9]),            // inp2_zero_bias
                                                 (signed int)(msg->param[10])            // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_softmax_asym8_asym8 == 1
        case SRTM_Command_xa_nn_vec_softmax_asym8_asym8:
            rc = xa_nn_vec_softmax_asym8_asym8((unsigned char *)(msg->param[0]),       // p_out
                                               (const unsigned char *)(msg->param[1]), // p_vec
                                               (signed int)(msg->param[2]),            // diffmin
                                               (signed int)(msg->param[3]),            // input_left_shift
                                               (signed int)(msg->param[4]),            // input_multiplier
                                               (signed int)(msg->param[5]),            // vec_length
                                               (void *)(msg->param[6])                 // p_scratch
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_vec_sigmoid_asym8_asym8 == 1
        case SRTM_Command_xa_nn_vec_sigmoid_asym8_asym8:
            rc = xa_nn_vec_sigmoid_asym8_asym8((unsigned char *)(msg->param[0]),       // p_out
                                               (const unsigned char *)(msg->param[1]), // p_vec
                                               (signed int)(msg->param[2]),            // zero_point
                                               (signed int)(msg->param[3]),            // input_range_radius
                                               (signed int)(msg->param[4]),            // input_multiplier
                                               (signed int)(msg->param[5]),            // input_left_shift
                                               (signed int)(msg->param[6])             // vec_length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_get_softmax_scratch_size == 1
        case SRTM_Command_get_softmax_scratch_size:
            rc = get_softmax_scratch_size((int)(msg->param[0]), // inp_precision
                                          (int)(msg->param[1]), // out_precision
                                          (int)(msg->param[2])  // length
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
            break;
#endif

#if NN_ENABLE_xa_nn_l2_norm_f32 == 1
        case SRTM_Command_xa_nn_l2_norm_f32:
            rc = xa_nn_l2_norm_f32((float *)(msg->param[0]),       // p_out
                                   (const float *)(msg->param[1]), // p_inp
                                   (signed int)(msg->param[2])     // num_elm
            );

            msg->error    = SRTM_Status_Success;
            msg->param[0] = (unsigned int)rc;
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
