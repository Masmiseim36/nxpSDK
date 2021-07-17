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


project(tensorflow_lite_micro_cifar10_cm33)

set(MCUX_SDK_PROJECT_NAME tensorflow_lite_micro_cifar10_cm33.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../middleware/eiq/common/demo_info.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/image/image.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/image/image_decode_raw.c"
"${ProjDirPath}/../../../../../middleware/eiq/common/image/image_utils.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/get_top_n.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/get_top_n.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/model.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/output_postproc.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/model/output_postproc.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/cifar10/demo_config.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/demo_info.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/cifar10/image_data.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/cifar10/labels.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/cifar10/model_cifarnet_ops_micro.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/model.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/image/main.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/cifar10/pcq/model_data.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/timer.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/timer.c"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/image/image_load.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../board_init.c"
"${ProjDirPath}/../board_init.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/cifar10
    ${ProjDirPath}/../../../../../middleware/eiq/common
    ${ProjDirPath}/../../../../../middleware/eiq/common/image
    ${ProjDirPath}/../../../../../middleware/eiq/common/model
    ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/cifar10/pcq
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT595S/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/eiq
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT595S/drivers
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../devices/MIMXRT595S
    ${ProjDirPath}/../../../flash_config
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT595S/utilities
    ${ProjDirPath}/../../../../../CMSIS/DSP
)

# include modules
include(utility_debug_console_lite_MIMXRT595S_cm33)

include(utility_assert_lite_MIMXRT595S_cm33)

include(middleware_eiq_tensorflow_lite_micro_MIMXRT595S_cm33)

include(component_lists_MIMXRT595S_cm33)

include(driver_common_MIMXRT595S_cm33)

include(component_usart_adapter_MIMXRT595S_cm33)

include(driver_flexcomm_usart_MIMXRT595S_cm33)

include(device_MIMXRT595S_CMSIS_MIMXRT595S_cm33)

include(device_MIMXRT595S_startup_MIMXRT595S_cm33)

include(driver_cache_cache64_MIMXRT595S_cm33)

include(driver_clock_MIMXRT595S_cm33)

include(driver_flash_config_MIMXRT595S_cm33)

include(driver_flexcomm_MIMXRT595S_cm33)

include(driver_flexspi_MIMXRT595S_cm33)

include(driver_lpc_gpio_MIMXRT595S_cm33)

include(driver_lpc_iopctl_MIMXRT595S_cm33)

include(driver_power_MIMXRT595S_cm33)

include(driver_reset_MIMXRT595S_cm33)

include(middleware_eiq_tensorflow_lite_micro_cmsis_nn_MIMXRT595S_cm33)

include(CMSIS_Include_core_cm_MIMXRT595S_cm33)

include(middleware_eiq_tensorflow_lite_micro_third_party_flatbuffers_MIMXRT595S_cm33)

include(middleware_eiq_tensorflow_lite_micro_third_party_gemmlowp_MIMXRT595S_cm33)

include(middleware_eiq_tensorflow_lite_micro_third_party_ruy_MIMXRT595S_cm33)

include(utilities_misc_utilities_MIMXRT595S_cm33)

include(driver_iap_MIMXRT595S_cm33)

include(middleware_eiq_tensorflow_lite_micro_third_party_cmsis_nn_MIMXRT595S_cm33)

include(CMSIS_DSP_Library_MIMXRT595S_cm33)


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


