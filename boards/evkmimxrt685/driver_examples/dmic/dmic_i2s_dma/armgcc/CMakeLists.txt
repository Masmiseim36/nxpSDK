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


project(dmic_i2s_dma)

set(MCUX_BUILD_TYPES debug release flash_debug flash_release)

set(MCUX_SDK_PROJECT_NAME dmic_i2s_dma.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../dmic_i2s_dma.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT685S/drivers
    ${SdkRootDirPath}/components/codec/i2c
    ${SdkRootDirPath}/components/i2c
    ${SdkRootDirPath}/devices/MIMXRT685S/utilities
    ${SdkRootDirPath}/devices/MIMXRT685S/utilities/debug_console_lite
    ${SdkRootDirPath}/components/codec/wm8904
    ${SdkRootDirPath}/components/codec
    ${SdkRootDirPath}/components/codec/
    ${SdkRootDirPath}/devices/MIMXRT685S
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/boards/evkmimxrt685/flash_config
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(driver_i3c_MIMXRT685S_cm33)

include(component_codec_i2c_MIMXRT685S_cm33)

include(driver_common_MIMXRT685S_cm33)

include(component_i3c_adapter_MIMXRT685S_cm33)

include(utility_debug_console_lite_MIMXRT685S_cm33)

include(utility_assert_lite_MIMXRT685S_cm33)

include(driver_flexcomm_i2c_MIMXRT685S_cm33)

include(driver_flexcomm_i2s_MIMXRT685S_cm33)

include(driver_flexcomm_i2s_dma_MIMXRT685S_cm33)

include(driver_dmic_MIMXRT685S_cm33)

include(driver_dmic_dma_MIMXRT685S_cm33)

include(driver_lpc_dma_MIMXRT685S_cm33)

include(driver_flexcomm_usart_MIMXRT685S_cm33)

include(driver_wm8904_MIMXRT685S_cm33)

include(driver_codec_MIMXRT685S_cm33)

include(component_wm8904_adapter_MIMXRT685S_cm33)

include(driver_clock_MIMXRT685S_cm33)

include(driver_power_MIMXRT685S_cm33)

include(driver_lpc_gpio_MIMXRT685S_cm33)

include(driver_inputmux_MIMXRT685S_cm33)

include(device_MIMXRT685S_CMSIS_MIMXRT685S_cm33)

include(component_usart_adapter_MIMXRT685S_cm33)

include(driver_flash_config_evkmimxrt685_MIMXRT685S_cm33)

include(driver_flexspi_MIMXRT685S_cm33)

include(driver_cache_cache64_MIMXRT685S_cm33)

include(component_lists_MIMXRT685S_cm33)

include(device_MIMXRT685S_startup_MIMXRT685S_cm33)

include(driver_flexcomm_MIMXRT685S_cm33)

include(driver_lpc_iopctl_MIMXRT685S_cm33)

include(driver_reset_MIMXRT685S_cm33)

include(CMSIS_Include_core_cm_MIMXRT685S_cm33)

include(driver_inputmux_connections_MIMXRT685S_cm33)

include(utilities_misc_utilities_MIMXRT685S_cm33)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

