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


project(fmstr_example_uart)

set(MCUX_SDK_PROJECT_NAME fmstr_example_uart.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../freemaster_cfg.h"
"${ProjDirPath}/../../common/fmstr_example/freemaster_example.c"
"${ProjDirPath}/../../common/fmstr_example/freemaster_example.h"
"${ProjDirPath}/../../common/fmstr_example/data_example_pmp.h"
"${ProjDirPath}/../../common/fmstr_example/data_img_logo.h"
"${ProjDirPath}/../../common/fmstr_example/data_img_file.h"
"${ProjDirPath}/../../common/fmstr_example/data_img_fmstrlogo.h"
"${ProjDirPath}/../../common/fmstr_example/data_html_index.h"
"${ProjDirPath}/../../common/fmstr_example/example_data.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../../common/fmstr_example
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT595S/drivers
    ${ProjDirPath}/../../../../../middleware/freemaster
    ${ProjDirPath}/../../../../../devices/MIMXRT595S/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../devices/MIMXRT595S
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../flash_config
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT595S/utilities
)

# include modules
include(driver_flexcomm_usart_MIMXRT595S_cm33)

include(middleware_fmstr_MIMXRT595S_cm33)

include(utility_debug_console_lite_MIMXRT595S_cm33)

include(utility_assert_lite_MIMXRT595S_cm33)

include(driver_clock_MIMXRT595S_cm33)

include(driver_common_MIMXRT595S_cm33)

include(device_MIMXRT595S_CMSIS_MIMXRT595S_cm33)

include(component_usart_adapter_MIMXRT595S_cm33)

include(driver_power_MIMXRT595S_cm33)

include(driver_flexspi_MIMXRT595S_cm33)

include(driver_cache_cache64_MIMXRT595S_cm33)

include(component_serial_manager_MIMXRT595S_cm33)

include(component_lists_MIMXRT595S_cm33)

include(component_serial_manager_uart_MIMXRT595S_cm33)

include(driver_flash_config_MIMXRT595S_cm33)

include(device_MIMXRT595S_startup_MIMXRT595S_cm33)

include(driver_flexcomm_MIMXRT595S_cm33)

include(driver_lpc_iopctl_MIMXRT595S_cm33)

include(driver_lpc_gpio_MIMXRT595S_cm33)

include(driver_reset_MIMXRT595S_cm33)

include(middleware_fmstr_platform_gen32le_MIMXRT595S_cm33)

include(CMSIS_Include_core_cm_MIMXRT595S_cm33)

include(utilities_misc_utilities_MIMXRT595S_cm33)

include(driver_iap_MIMXRT595S_cm33)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


