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


project(tflm_kws_cm7)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_release flexspi_nor_sdram_debug)

set(MCUX_SDK_PROJECT_NAME tflm_kws_cm7.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
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
"${SdkRootDirPath}/middleware/eiq/common/audio/audio_capture.cpp"
"${SdkRootDirPath}/middleware/eiq/common/timer.h"
"${SdkRootDirPath}/middleware/eiq/common/timer.c"
"${ProjDirPath}/../board_init.c"
"${ProjDirPath}/../board_init.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/kws
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/common/audio
    ${SdkRootDirPath}/middleware/eiq/common/audio
    ${SdkRootDirPath}/middleware/eiq/common/model
    ${SdkRootDirPath}/middleware/eiq/common
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/components/codec/wm8960
    ${SdkRootDirPath}/components/codec
    ${SdkRootDirPath}/devices/MIMXRT1176/drivers
    ${SdkRootDirPath}/components/codec/
    ${SdkRootDirPath}/components/codec/i2c
    ${SdkRootDirPath}/components/i2c
    ${SdkRootDirPath}/devices/MIMXRT1176/utilities
    ${SdkRootDirPath}/devices/MIMXRT1176/utilities/debug_console_lite
    ${SdkRootDirPath}/middleware/eiq
    ${SdkRootDirPath}/CMSIS/DSP
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/devices/MIMXRT1176
    ${SdkRootDirPath}/devices/MIMXRT1176/xip
    ${SdkRootDirPath}/boards/evkmimxrt1170/xip
    ${SdkRootDirPath}/boards/evkmimxrt1170/xmcd
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite/third_party/cmsis
)

# include modules
include(driver_wm8960_MIMXRT1176_cm7)

include(driver_codec_MIMXRT1176_cm7)

include(driver_common_MIMXRT1176_cm7)

include(component_wm8960_adapter_MIMXRT1176_cm7)

include(component_codec_i2c_MIMXRT1176_cm7)

include(component_lpi2c_adapter_MIMXRT1176_cm7)

include(driver_sai_MIMXRT1176_cm7)

include(driver_sai_edma_MIMXRT1176_cm7)

include(driver_dmamux_MIMXRT1176_cm7)

include(utility_debug_console_lite_MIMXRT1176_cm7)

include(utility_assert_lite_MIMXRT1176_cm7)

include(middleware_eiq_worker_audio_MIMXRT1176_cm7)

include(CMSIS_DSP_Source_MIMXRT1176_cm7)

include(middleware_eiq_tensorflow_lite_micro_MIMXRT1176_cm7)

include(component_lists_MIMXRT1176_cm7)

include(component_lpuart_adapter_MIMXRT1176_cm7)

include(driver_lpuart_MIMXRT1176_cm7)

include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm7)

include(device_MIMXRT1176_startup_MIMXRT1176_cm7)

include(driver_dcdc_soc_MIMXRT1176_cm7)

include(driver_pmu_1_MIMXRT1176_cm7)

include(driver_clock_MIMXRT1176_cm7)

include(driver_igpio_MIMXRT1176_cm7)

include(driver_iomuxc_MIMXRT1176_cm7)

include(driver_lpi2c_MIMXRT1176_cm7)

include(driver_xip_device_MIMXRT1176_cm7)

include(driver_xip_board_evkmimxrt1170_MIMXRT1176_cm7)

include(middleware_eiq_tensorflow_lite_micro_cmsis_nn_MIMXRT1176_cm7)

include(driver_xmcd_evkmimxrt1170_MIMXRT1176_cm7)

include(driver_anatop_ai_MIMXRT1176_cm7)

include(CMSIS_Include_core_cm_MIMXRT1176_cm7)

include(driver_edma_MIMXRT1176_cm7)

include(driver_memory_MIMXRT1176_cm7)

include(middleware_eiq_worker_MIMXRT1176_cm7)

include(middleware_eiq_tensorflow_lite_micro_third_party_flatbuffers_MIMXRT1176_cm7)

include(middleware_eiq_tensorflow_lite_micro_third_party_gemmlowp_MIMXRT1176_cm7)

include(middleware_eiq_tensorflow_lite_micro_third_party_ruy_MIMXRT1176_cm7)

include(utilities_misc_utilities_MIMXRT1176_cm7)

include(device_MIMXRT1176_system_MIMXRT1176_cm7)

include(middleware_eiq_tensorflow_lite_micro_third_party_cmsis_nn_MIMXRT1176_cm7)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


