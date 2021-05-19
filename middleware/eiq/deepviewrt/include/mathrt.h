/**
 * Copyright 2018 by Au-Zone Technologies.  All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential.
 *
 * Authorization of this file is not implied by any DeepViewRT license
 * agreements unless explicitly stated.
 */

#ifndef __MATHRT_H__
#define __MATHRT_H__

#include <stdint.h>

/**
 * Activation functions for fused kernels.  Values must match definitions in
 * deepview_ops.h.
 */
#define MATHRT_RELU 1
#define MATHRT_RELU6 2
#define MATHRT_SIGMOID 3
#define MATHRT_SIGMOID_FAST 4
#define MATHRT_TANH 5

extern void
mathrt_activation(float* out, int32_t len, const float* in, int32_t activation);

extern void
mathrt_relu(float* out, int32_t len, const float* in);

extern void
mathrt_relu6(float* out, int32_t len, const float* in);

extern void
mathrt_prelu(float*       out,
             int32_t      len,
             const float* in,
             int32_t      n_weights,
             const float* weights);

extern void
mathrt_sigmoid(float* out, int32_t len, const float* in);

extern void
mathrt_sigmoid_fast(float* out, int32_t len, const float* in);

extern float
mathrt_expf(float x);

extern void
mathrt_tanh(float* out, int32_t len, const float* in);

extern void
mathrt_add_f32(float* out, int len, const float* a, float b);

extern void
mathrt_sub_f32(float* out, int len, const float* a, float b);

extern void
mathrt_mul_f32(float* out, int len, const float* a, float b);

extern void
mathrt_div_f32(float* out, int len, const float* a, float b);

extern void
mathrt_vadd_f32(float* out, int len, const float* a, const float* b);

extern void
mathrt_vsub_f32(float* out, int len, const float* a, const float* b);

extern void
mathrt_vmul_f32(float* out, int len, const float* a, const float* b);

extern void
mathrt_vdiv_f32(float* out, int len, const float* a, const float* b);

extern void
mathrt_mac_f32(float* out, int len, const float* a, float b);

extern void
mathrt_vmac_f32(float* out, int len, const float* a, const float* b);

extern void
mathrt_reduce_sum_f32(float*         out,
                      int            len,
                      const int32_t* strides,
                      const float*   in,
                      const int32_t* window_shape);

extern void
mathrt_reduce_min_f32(float*         out,
                      int            len,
                      const int32_t* strides,
                      const float*   in,
                      const int32_t* window_shape);

extern void
mathrt_reduce_max_f32(float*         out,
                      int            len,
                      const int32_t* strides,
                      const float*   in,
                      const int32_t* window_shape);

extern void
mathrt_reduce_mean_f32(float*         out,
                       int            len,
                       const int32_t* strides,
                       const float*   in,
                       const int32_t* window_shape);

extern void
mathrt_reduce_product_f32(float*         out,
                          int            len,
                          const int32_t* strides,
                          const float*   in,
                          const int32_t* window_shape);

extern void
mathrt_maxpool2d_f32(int32_t      output_width,
                     int32_t      output_depth,
                     float*       output,
                     int32_t      input_width,
                     int32_t      input_stride,
                     const float* input,
                     int32_t      pad_left,
                     int32_t      pad_right,
                     int32_t      window_height,
                     int32_t      window_width);

extern void
mathrt_avgpool2d_f32(int32_t      output_width,
                     int32_t      output_depth,
                     float*       output,
                     int32_t      input_width,
                     int32_t      input_stride,
                     const float* input,
                     int32_t      pad_left,
                     int32_t      pad_right,
                     int32_t      window_height,
                     int32_t      window_width);

extern void
mathrt_conv2d_f32(int32_t      output_width,
                  int32_t      output_depth,
                  float*       output,
                  int32_t      input_width,
                  int32_t      input_depth,
                  int32_t      input_stride,
                  const float* input,
                  int32_t      pad_left,
                  int32_t      pad_right,
                  int32_t      kernel_height,
                  int32_t      kernel_width,
                  const float* kernel);

extern void
mathrt_dwconv2d_f32(int32_t      output_width,
                    int32_t      output_depth,
                    float*       output,
                    int32_t      input_width,
                    int32_t      input_stride,
                    const float* input,
                    int32_t      pad_left,
                    int32_t      pad_right,
                    int32_t      filter_width,
                    const float* filter);

void
mathrt_dwconv2d_3x3s1_f32(int32_t       output_width,
                          int32_t       output_depth,
                          float*        output,
                          int32_t       input_width,
                          const int32_t pad_left,
                          const int32_t pad_right,
                          const int32_t window_height,
                          const int32_t truncated_height,
                          const float*  input,
                          const float*  filter,
                          const float*  bias,
                          int32_t       activation);

void
mathrt_dwdwconv2d_3x3s2_f32(int32_t      output_width,
                            int32_t      output_depth,
                            float*       output,
                            int32_t      input_width,
                            const float* input,
                            const float* filter,
                            const float* bias,
                            int32_t      activation);

extern void
mathrt_gervm_f32(float*       C,
                 const float* A,
                 const float* B,
                 const float* bias,
                 int32_t      activation,
                 int32_t      n,
                 int32_t      k);

extern void
mathrt_gemm_f32(float*       C,
                const float* A,
                char         order_a,
                const float* B,
                char         order_b,
                const float* bias,
                int32_t      activation,
                int32_t      m,
                int32_t      n,
                int32_t      k);

extern int32_t
mathrt_gemm_panel_sizes(const int32_t** sizes);

#endif /* __MATHRT_H__ */
