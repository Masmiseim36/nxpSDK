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


project(cmsis_nn_kws_cm7)

set(MCUX_SDK_PROJECT_NAME cmsis_nn_kws_cm7.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/audio_data.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS/kws.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS/kws.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS/kws_dnn.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS/kws_dnn.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS/kws_ds_cnn.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS/kws_ds_cnn.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/dnn.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/dnn.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/dnn_weights.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/ds_cnn.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/ds_cnn.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/ds_cnn_weights.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/nn.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/nn.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/local_NN/arm_avepool_q7_HWC_nonsquare.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/local_NN/local_NN.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/MFCC/mfcc.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/MFCC/mfcc.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/timer.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/timer.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/main.cpp"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS
    ${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN
    ${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/local_NN
    ${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/MFCC
    ${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws
    ${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/common/source
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/codec/wm8960
    ${ProjDirPath}/../../../../../components/codec
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/drivers
    ${ProjDirPath}/../../../../../components/codec/port/wm8960
    ${ProjDirPath}/../../../../../components/codec/i2c
    ${ProjDirPath}/../../../../../components/i2c
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/eiq
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../devices/MIMXRT1176
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/DSP/Lib
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/utilities
)

# include modules
include(driver_wm8960_MIMXRT1176_cm7)

include(driver_codec_MIMXRT1176_cm7)

include(driver_common_MIMXRT1176_cm7)

include(component_wm8960_adapter_MIMXRT1176_cm7)

include(component_codec_i2c_MIMXRT1176_cm7)

include(component_lpi2c_adapter_MIMXRT1176_cm7)

include(driver_sai_MIMXRT1176_cm7)

include(utility_debug_console_lite_MIMXRT1176_cm7)

include(utility_assert_lite_MIMXRT1176_cm7)

include(middleware_eiq_worker_audio_MIMXRT1176_cm7)

include(middleware_eiq_cmsis_nn_MIMXRT1176_cm7)

include(component_lists_MIMXRT1176_cm7)

include(component_lpuart_adapter_MIMXRT1176_cm7)

include(driver_lpuart_MIMXRT1176_cm7)

include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm7)

include(CMSIS_Include_core_cm7_MIMXRT1176_cm7)

include(device_MIMXRT1176_startup_MIMXRT1176_cm7)

include(driver_dcdc_soc_MIMXRT1176_cm7)

include(driver_pmu_1_MIMXRT1176_cm7)

include(driver_clock_MIMXRT1176_cm7)

include(driver_igpio_MIMXRT1176_cm7)

include(driver_iomuxc_MIMXRT1176_cm7)

include(driver_lpi2c_MIMXRT1176_cm7)

include(driver_xip_device_MIMXRT1176_cm7)

include(driver_xip_board_MIMXRT1176_cm7)

include(CMSIS_DSP_Lib_arm_cortexM7lfdp_math_MIMXRT1176_cm7)

include(driver_anatop_ai_MIMXRT1176_cm7)

include(middleware_eiq_worker_MIMXRT1176_cm7)

include(utilities_misc_utilities_MIMXRT1176_cm7)

include(CMSIS_Include_common_MIMXRT1176_cm7)

include(CMSIS_Include_dsp_MIMXRT1176_cm7)

include(device_MIMXRT1176_system_MIMXRT1176_cm7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfdp_math.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfdp_math.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


