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


project(tflm_kws_cm33)

set(MCUX_BUILD_TYPES flash_release flash_debug)

set(MCUX_SDK_PROJECT_NAME tflm_kws_cm33.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${SdkRootDirPath}/middleware/eiq/common/demo_info.h"
"${SdkRootDirPath}/middleware/eiq/common/model/get_top_n.cpp"
"${SdkRootDirPath}/middleware/eiq/common/model/get_top_n.h"
"${SdkRootDirPath}/middleware/eiq/common/model/model.h"
"${SdkRootDirPath}/middleware/eiq/common/model/output_postproc.h"
"${SdkRootDirPath}/middleware/eiq/common/audio/audio.h"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/common/audio/kws_mfcc.cpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/common/audio/kws_mfcc.hpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/common/audio/mfcc.cpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/common/audio/mfcc.hpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/model.cpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/kws/output_postproc.cpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/kws/audio_data.h"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/kws/demo_config.h"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/common/demo_info.cpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/kws/labels.h"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/kws/main.cpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/kws/model_data.h"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/kws/model_ds_cnn_ops_micro.cpp"
"${SdkRootDirPath}/middleware/eiq/common/timer.h"
"${SdkRootDirPath}/middleware/eiq/common/timer.c"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../board_init.c"
"${ProjDirPath}/../board_init.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/kws
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/common/audio
    ${SdkRootDirPath}/middleware/eiq/common/audio
    ${SdkRootDirPath}/middleware/eiq/common/model
    ${SdkRootDirPath}/middleware/eiq/common
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT685S/utilities
    ${SdkRootDirPath}/devices/MIMXRT685S/utilities/debug_console_lite
    ${SdkRootDirPath}/components/codec/i2c
    ${SdkRootDirPath}/devices/MIMXRT685S/drivers
    ${SdkRootDirPath}/components/i2c
    ${SdkRootDirPath}/components/codec
    ${SdkRootDirPath}/components/codec/
    ${SdkRootDirPath}/components/codec/wm8904
    ${SdkRootDirPath}/middleware/eiq/common/audio
    ${SdkRootDirPath}/CMSIS/DSP
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/devices/MIMXRT685S
    ${SdkRootDirPath}/boards/evkmimxrt685/flash_config
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/middleware/eiq
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite/third_party/cmsis
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

include(middleware_eiq_audio_evkmimxrt685_MIMXRT685S_cm33)

include(CMSIS_DSP_Source_MIMXRT685S_cm33)

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


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


