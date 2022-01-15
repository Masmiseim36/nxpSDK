# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

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


project(tensorflow_lite_micro_kws_cm33)

set(MCUX_SDK_PROJECT_NAME tensorflow_lite_micro_kws_cm33.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../middleware/eiq/common/demo_info.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/get_top_n.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/get_top_n.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/model.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/output_postproc.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/audio/audio.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/audio/kws_mfcc.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/audio/kws_mfcc.hpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/audio/mfcc.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/audio/mfcc.hpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/model.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws/output_postproc.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws/audio_data.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws/demo_config.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/demo_info.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws/labels.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws/main.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws/model_data.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws/model_ds_cnn_ops_micro.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/common/timer.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/timer.c"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../board_init.c"
"${ProjDirPath}/../board_init.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws
    ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/audio
    ${ProjDirPath}/../../../../../middleware/eiq/common/audio
    ${ProjDirPath}/../../../../../middleware/eiq/common/model
    ${ProjDirPath}/../../../../../middleware/eiq/common
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../components/codec/i2c
    ${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers
    ${ProjDirPath}/../../../../../components/i2c
    ${ProjDirPath}/../../../../../components/codec
    ${ProjDirPath}/../../../../../components/codec/wm8904
    ${ProjDirPath}/../../../../../middleware/eiq/common/audio
    ${ProjDirPath}/../../../../../CMSIS/DSP
    ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../devices/MIMXRT685S
    ${ProjDirPath}/../../../flash_config
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../middleware/eiq
    ${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities
    ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/cmsis
)

# include modules
include(utility_debug_console_lite_MIMXRT685S_cm33)

include(utility_assert_lite_MIMXRT685S_cm33)

include(component_codec_i2c_MIMXRT685S_cm33)

include(driver_common_MIMXRT685S_cm33)

include(component_i3c_adapter_MIMXRT685S_cm33)

include(driver_codec_MIMXRT685S_cm33)

include(component_wm8904_adapter_MIMXRT685S_cm33)

include(driver_dmic_MIMXRT685S_cm33)

include(driver_dmic_dma_MIMXRT685S_cm33)

include(driver_flexcomm_i2c_MIMXRT685S_cm33)

include(driver_flexcomm_i2s_MIMXRT685S_cm33)

include(driver_flexcomm_i2s_dma_MIMXRT685S_cm33)

include(driver_flexcomm_usart_MIMXRT685S_cm33)

include(driver_i3c_MIMXRT685S_cm33)

include(driver_inputmux_MIMXRT685S_cm33)

include(driver_inputmux_connections_MIMXRT685S_cm33)

include(driver_lpc_dma_MIMXRT685S_cm33)

include(driver_wm8904_MIMXRT685S_cm33)

include(middleware_eiq_audio_MIMXRT685S_cm33)

include(CMSIS_DSP_Library_MIMXRT685S_cm33)

include(middleware_eiq_tensorflow_lite_micro_MIMXRT685S_cm33)

include(component_lists_MIMXRT685S_cm33)

include(component_usart_adapter_MIMXRT685S_cm33)

include(device_MIMXRT685S_CMSIS_MIMXRT685S_cm33)

include(device_MIMXRT685S_startup_MIMXRT685S_cm33)

include(driver_cache_cache64_MIMXRT685S_cm33)

include(driver_clock_MIMXRT685S_cm33)

include(driver_flash_config_evkmimxrt685_MIMXRT685S_cm33)

include(driver_flexcomm_MIMXRT685S_cm33)

include(driver_flexspi_MIMXRT685S_cm33)

include(driver_lpc_gpio_MIMXRT685S_cm33)

include(driver_lpc_iopctl_MIMXRT685S_cm33)

include(driver_power_MIMXRT685S_cm33)

include(driver_reset_MIMXRT685S_cm33)

include(middleware_eiq_tensorflow_lite_micro_cmsis_nn_MIMXRT685S_cm33)

include(CMSIS_Include_core_cm_MIMXRT685S_cm33)

include(middleware_eiq_tensorflow_lite_micro_third_party_flatbuffers_MIMXRT685S_cm33)

include(middleware_eiq_tensorflow_lite_micro_third_party_gemmlowp_MIMXRT685S_cm33)

include(middleware_eiq_tensorflow_lite_micro_third_party_ruy_MIMXRT685S_cm33)

include(utilities_misc_utilities_MIMXRT685S_cm33)

include(middleware_eiq_tensorflow_lite_micro_third_party_cmsis_nn_MIMXRT685S_cm33)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flash_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_ARMv8MMLldfsp_math.a)
endif(CMAKE_BUILD_TYPE STREQUAL flash_release)

if(CMAKE_BUILD_TYPE STREQUAL flash_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_ARMv8MMLldfsp_math.a)
endif(CMAKE_BUILD_TYPE STREQUAL flash_debug)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


