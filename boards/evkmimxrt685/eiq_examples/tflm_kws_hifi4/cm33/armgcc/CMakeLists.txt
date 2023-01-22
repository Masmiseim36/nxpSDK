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


project(tflm_kws_hifi4_cm33)

set(MCUX_BUILD_TYPES flash_debug flash_release)

set(MCUX_SDK_PROJECT_NAME tflm_kws_hifi4_cm33.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${SdkRootDirPath}/boards/evkmimxrt685/eiq_examples/tflm_kws_hifi4/hifi4/binary/dsp_text_release.bin"
"${SdkRootDirPath}/boards/evkmimxrt685/eiq_examples/tflm_kws_hifi4/hifi4/binary/dsp_data_release.bin"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../board_init.h"
"${ProjDirPath}/../incbin.S"
"${ProjDirPath}/../dsp_config.h"
"${ProjDirPath}/../dsp_support.c"
"${ProjDirPath}/../dsp_support.h"
"${ProjDirPath}/../pmic_support.c"
"${ProjDirPath}/../pmic_support.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board_init.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${SdkRootDirPath}/boards/evkmimxrt685/eiq_examples/tflm_kws_hifi4/hifi4/binary
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/components/codec/i2c
    ${SdkRootDirPath}/devices/MIMXRT685S/drivers
    ${SdkRootDirPath}/components/i2c
    ${SdkRootDirPath}/components/codec
    ${SdkRootDirPath}/components/codec/
    ${SdkRootDirPath}/components/codec/wm8904
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/devices/MIMXRT685S
    ${SdkRootDirPath}/boards/evkmimxrt685/flash_config
    ${SdkRootDirPath}/components/pca9420
    ${SdkRootDirPath}/devices/MIMXRT685S/utilities
    ${SdkRootDirPath}/devices/MIMXRT685S/utilities/debug_console_lite
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(component_codec_i2c_MIMXRT685S_cm33)

include(driver_common_MIMXRT685S_cm33)

include(component_i3c_adapter_MIMXRT685S_cm33)

include(driver_codec_MIMXRT685S_cm33)

include(component_wm8904_adapter_MIMXRT685S_cm33)

include(driver_flexcomm_i2c_MIMXRT685S_cm33)

include(driver_i3c_MIMXRT685S_cm33)

include(driver_wm8904_MIMXRT685S_cm33)

include(component_lists_MIMXRT685S_cm33)

include(component_usart_adapter_MIMXRT685S_cm33)

include(driver_flexcomm_usart_MIMXRT685S_cm33)

include(device_MIMXRT685S_CMSIS_MIMXRT685S_cm33)

include(device_MIMXRT685S_startup_MIMXRT685S_cm33)

include(driver_cache_cache64_MIMXRT685S_cm33)

include(driver_clock_MIMXRT685S_cm33)

include(driver_dsp_MIMXRT685S_cm33)

include(driver_flash_config_evkmimxrt685_MIMXRT685S_cm33)

include(driver_flexcomm_MIMXRT685S_cm33)

include(driver_flexspi_MIMXRT685S_cm33)

include(driver_inputmux_MIMXRT685S_cm33)

include(driver_lpc_gpio_MIMXRT685S_cm33)

include(driver_lpc_iopctl_MIMXRT685S_cm33)

include(driver_pca9420_MIMXRT685S_cm33)

include(driver_power_MIMXRT685S_cm33)

include(driver_reset_MIMXRT685S_cm33)

include(utility_debug_console_lite_MIMXRT685S_cm33)

include(utility_assert_lite_MIMXRT685S_cm33)

include(CMSIS_Include_core_cm_MIMXRT685S_cm33)

include(utilities_misc_utilities_MIMXRT685S_cm33)

include(driver_inputmux_connections_MIMXRT685S_cm33)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


