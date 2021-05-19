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


project(tensorflow_lite_kws_cm7)

set(MCUX_SDK_PROJECT_NAME tensorflow_lite_kws_cm7.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../middleware/eiq/common/board_init.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/demo_info.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/timer.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/timer.c"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/get_top_n.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/get_top_n.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/model.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/output_postproc.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/audio/audio_capture.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/audio/kws_mfcc.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/audio/kws_mfcc.hpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/audio/mfcc.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/audio/mfcc.hpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/model/model.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/model/output_postproc.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws/audio_data.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws/demo_config.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws/demo_info.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws/labels.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws/main.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws/model_data.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws/model_ds_cnn_ops.cpp"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../board_init.c"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/kws
    ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/audio
    ${ProjDirPath}/../../../../../middleware/eiq/common/model
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/codec/wm8960
    ${ProjDirPath}/../../../../../components/codec
    ${ProjDirPath}/../../../../../devices/MIMXRT1166/drivers
    ${ProjDirPath}/../../../../../components/codec/port/wm8960
    ${ProjDirPath}/../../../../../components/codec/i2c
    ${ProjDirPath}/../../../../../components/i2c
    ${ProjDirPath}/../../../../../CMSIS/DSP/Lib
    ${ProjDirPath}/../../../../../devices/MIMXRT1166/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/eiq
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../devices/MIMXRT1166
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT1166/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../devices/MIMXRT1166/utilities
)

# include modules
include(driver_wm8960_MIMXRT1166_cm7)

include(driver_codec_MIMXRT1166_cm7)

include(driver_common_MIMXRT1166_cm7)

include(component_wm8960_adapter_MIMXRT1166_cm7)

include(component_codec_i2c_MIMXRT1166_cm7)

include(component_lpi2c_adapter_MIMXRT1166_cm7)

include(driver_sai_MIMXRT1166_cm7)

include(CMSIS_DSP_Lib_arm_cortexM7lfdp_math_MIMXRT1166_cm7)

include(utility_debug_console_lite_MIMXRT1166_cm7)

include(utility_assert_lite_MIMXRT1166_cm7)

include(middleware_eiq_worker_audio_MIMXRT1166_cm7)

include(middleware_eiq_tensorflow_lite_binary_MIMXRT1166_cm7)

include(component_lists_MIMXRT1166_cm7)

include(component_lpuart_adapter_MIMXRT1166_cm7)

include(driver_lpuart_MIMXRT1166_cm7)

include(device_MIMXRT1166_CMSIS_MIMXRT1166_cm7)

include(CMSIS_Include_core_cm7_MIMXRT1166_cm7)

include(device_MIMXRT1166_startup_MIMXRT1166_cm7)

include(driver_dcdc_soc_MIMXRT1166_cm7)

include(driver_pmu_1_MIMXRT1166_cm7)

include(driver_clock_MIMXRT1166_cm7)

include(driver_igpio_MIMXRT1166_cm7)

include(driver_iomuxc_MIMXRT1166_cm7)

include(driver_lpi2c_MIMXRT1166_cm7)

include(driver_xip_device_MIMXRT1166_cm7)

include(driver_xip_board_MIMXRT1166_cm7)

include(driver_anatop_ai_MIMXRT1166_cm7)

include(middleware_eiq_worker_MIMXRT1166_cm7)

include(utilities_misc_utilities_MIMXRT1166_cm7)

include(CMSIS_Include_common_MIMXRT1166_cm7)

include(CMSIS_Include_dsp_MIMXRT1166_cm7)

include(device_MIMXRT1166_system_MIMXRT1166_cm7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfdp_math.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/lib/cm7/gcc/libtensorflow-lite.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfdp_math.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/lib/cm7/gcc/libtensorflow-lite.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


