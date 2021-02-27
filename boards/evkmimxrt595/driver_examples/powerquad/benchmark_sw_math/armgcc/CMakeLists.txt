# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.1.1)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(powerquad_benchmark_sw_math)

set(MCUX_SDK_PROJECT_NAME powerquad_benchmark_sw_math.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_bitreversal.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_bitreversal2.c"
"${ProjDirPath}/../powerquad_benchmark_sw_math.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/BasicMathFunctions/arm_mult_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/BasicMathFunctions/arm_mult_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix2_init_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_cfft_radix4_init_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_add_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_add_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_add_f32.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_sub_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_sub_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_sub_f32.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_mult_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_mult_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_mult_f32.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_inverse_f32.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_trans_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_trans_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_trans_f32.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_scale_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_scale_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_scale_f32.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_init_f32.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_init_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/MatrixFunctions/arm_mat_init_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FilteringFunctions/arm_fir_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FilteringFunctions/arm_fir_init_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FilteringFunctions/arm_fir_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FilteringFunctions/arm_fir_init_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FilteringFunctions/arm_fir_f32.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FilteringFunctions/arm_fir_init_f32.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FilteringFunctions/arm_conv_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FilteringFunctions/arm_conv_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FilteringFunctions/arm_conv_f32.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FilteringFunctions/arm_correlate_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FilteringFunctions/arm_correlate_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FilteringFunctions/arm_correlate_f32.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FilteringFunctions/arm_biquad_cascade_df2T_init_f32.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/FilteringFunctions/arm_biquad_cascade_df2T_f32.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_rfft_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_rfft_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_rfft_init_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_cfft_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_cfft_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_dct4_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_q15.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_dct4_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/TransformFunctions/arm_dct4_init_q31.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/CommonTables/arm_common_tables.c"
"${ProjDirPath}/../../../../../../CMSIS/DSP/Source/CommonTables/arm_const_structs.c"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers
    ${ProjDirPath}/../../../../../../devices/MIMXRT595S
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../flash_config
    ${ProjDirPath}/../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities
)

# include modules
include(utility_debug_console_lite_MIMXRT595S_cm33)

include(utility_assert_lite_MIMXRT595S_cm33)

include(driver_clock_MIMXRT595S_cm33)

include(driver_common_MIMXRT595S_cm33)

include(driver_power_MIMXRT595S_cm33)

include(driver_lpc_gpio_MIMXRT595S_cm33)

include(driver_inputmux_MIMXRT595S_cm33)

include(device_MIMXRT595S_CMSIS_MIMXRT595S_cm33)

include(component_usart_adapter_MIMXRT595S_cm33)

include(driver_flexspi_MIMXRT595S_cm33)

include(driver_cache_cache64_MIMXRT595S_cm33)

include(component_lists_MIMXRT595S_cm33)

include(driver_flexcomm_usart_MIMXRT595S_cm33)

include(driver_flash_config_MIMXRT595S_cm33)

include(device_MIMXRT595S_startup_MIMXRT595S_cm33)

include(driver_flexcomm_MIMXRT595S_cm33)

include(driver_lpc_iopctl_MIMXRT595S_cm33)

include(driver_reset_MIMXRT595S_cm33)

include(CMSIS_Include_core_cm33_MIMXRT595S_cm33)

include(CMSIS_Include_common_MIMXRT595S_cm33)

include(CMSIS_Include_dsp_MIMXRT595S_cm33)

include(driver_inputmux_connections_MIMXRT595S_cm33)

include(utilities_misc_utilities_MIMXRT595S_cm33)

include(driver_iap_MIMXRT595S_cm33)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

