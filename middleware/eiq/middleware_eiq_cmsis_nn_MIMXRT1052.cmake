if(NOT MIDDLEWARE_EIQ_CMSIS_NN_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_EIQ_CMSIS_NN_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_eiq_cmsis_nn component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ActivationFunctions/arm_relu_q15.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ActivationFunctions/arm_relu_q7.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ActivationFunctions/arm_nn_activations_q7.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ActivationFunctions/arm_nn_activations_q15.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q7_basic.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15_reordered.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q7_basic_nonsquare.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q15_fast_nonsquare.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast_nonsquare.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_1x1_HWC_q7_fast_nonsquare.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q15_fast.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7_nonsquare.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q15_basic.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ConvolutionFunctions/arm_depthwise_conv_u8_basic_ver1.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/FullyConnectedFunctions/arm_fully_connected_q7.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/FullyConnectedFunctions/arm_fully_connected_q15_opt.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/FullyConnectedFunctions/arm_fully_connected_q15.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15_opt.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/NNSupportFunctions/arm_nn_mult_q15.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/NNSupportFunctions/arm_nntables.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/NNSupportFunctions/arm_nn_mult_q7.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/NNSupportFunctions/arm_q7_to_q15_reordered_no_shift.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/NNSupportFunctions/arm_q7_to_q15_no_shift.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/PoolingFunctions/arm_pool_q7_HWC.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/SoftmaxFunctions/arm_softmax_q7.c
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Source/SoftmaxFunctions/arm_softmax_q15.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/cmsis-nn/Include
    )


    include(CMSIS_DSP_Lib_arm_cortexM7lfdp_math_MIMXRT1052)

endif()
