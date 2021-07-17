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


project(dsp_audio_demo_bm_cm33)

set(MCUX_SDK_PROJECT_NAME dsp_audio_demo_bm_cm33.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main_cm33.c"
"${ProjDirPath}/../dsp_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../incbin.S"
"${ProjDirPath}/../dsp_support.c"
"${ProjDirPath}/../dsp_support.h"
"${ProjDirPath}/../pmic_support.c"
"${ProjDirPath}/../pmic_support.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../dsp/binary/dsp_text_release.bin"
"${ProjDirPath}/../../dsp/binary/dsp_data_release.bin"
"${ProjDirPath}/../../dsp/binary/dsp_ncache_release.bin"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../dsp/binary
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../devices/MIMXRT685S/drivers
    ${ProjDirPath}/../../../../../../components/codec/wm8904
    ${ProjDirPath}/../../../../../../devices/MIMXRT685S/utilities
    ${ProjDirPath}/../../../../../../components/codec
    ${ProjDirPath}/../../../../../../components/codec/i2c
    ${ProjDirPath}/../../../../../../components/i2c
    ${ProjDirPath}/../../../../../../components/pca9420
    ${ProjDirPath}/../../../../../../devices/MIMXRT685S
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../flash_config
    ${ProjDirPath}/../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../CMSIS/Core/Include
)

# include modules
include(driver_clock_MIMXRT685S_cm33)

include(driver_flexcomm_i2c_MIMXRT685S_cm33)

include(driver_i3c_MIMXRT685S_cm33)

include(driver_wm8904_MIMXRT685S_cm33)

include(utility_shell_MIMXRT685S_cm33)

include(driver_codec_MIMXRT685S_cm33)

include(driver_common_MIMXRT685S_cm33)

include(component_wm8904_adapter_MIMXRT685S_cm33)

include(component_codec_i2c_MIMXRT685S_cm33)

include(component_i3c_adapter_MIMXRT685S_cm33)

include(driver_lpc_gpio_MIMXRT685S_cm33)

include(driver_power_MIMXRT685S_cm33)

include(driver_reset_MIMXRT685S_cm33)

include(driver_dsp_MIMXRT685S_cm33)

include(driver_inputmux_MIMXRT685S_cm33)

include(driver_pca9420_MIMXRT685S_cm33)

include(device_MIMXRT685S_CMSIS_MIMXRT685S_cm33)

include(utility_debug_console_MIMXRT685S_cm33)

include(utility_assert_MIMXRT685S_cm33)

include(component_usart_adapter_MIMXRT685S_cm33)

include(driver_flash_config_MIMXRT685S_cm33)

include(driver_flexspi_MIMXRT685S_cm33)

include(driver_cache_cache64_MIMXRT685S_cm33)

include(component_serial_manager_MIMXRT685S_cm33)

include(component_lists_MIMXRT685S_cm33)

include(component_serial_manager_uart_MIMXRT685S_cm33)

include(driver_flexcomm_usart_MIMXRT685S_cm33)

include(device_MIMXRT685S_startup_MIMXRT685S_cm33)

include(driver_flexcomm_MIMXRT685S_cm33)

include(driver_lpc_iopctl_MIMXRT685S_cm33)

include(CMSIS_Include_core_cm_MIMXRT685S_cm33)

include(driver_inputmux_connections_MIMXRT685S_cm33)

include(utilities_misc_utilities_MIMXRT685S_cm33)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

