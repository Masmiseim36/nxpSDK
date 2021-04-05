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


project(fmstr_example_any)

set(MCUX_SDK_PROJECT_NAME fmstr_example_any.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../peripherals.c"
"${ProjDirPath}/../peripherals.h"
"${ProjDirPath}/../freemaster_cfg.h"
"${ProjDirPath}/../freemaster_democfg.h"
"${ProjDirPath}/../../common/fmstr_example/freemaster_example_per.c"
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
    ${ProjDirPath}/../../../../../devices/MKV11Z7/drivers
    ${ProjDirPath}/../../../../../devices/MKV11Z7/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/freemaster
    ${ProjDirPath}/../../../../../devices/MKV11Z7
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../devices/MKV11Z7/utilities
)

# include modules
include(driver_sim_MKV11Z7)

include(utility_debug_console_lite_MKV11Z7)

include(utility_assert_lite_MKV11Z7)

include(middleware_fmstr_MKV11Z7)

include(driver_clock_MKV11Z7)

include(driver_common_MKV11Z7)

include(device_MKV11Z7_CMSIS_MKV11Z7)

include(component_uart_adapter_MKV11Z7)

include(component_lists_MKV11Z7)

include(driver_uart_MKV11Z7)

include(device_MKV11Z7_startup_MKV11Z7)

include(driver_port_MKV11Z7)

include(driver_smc_MKV11Z7)

include(driver_gpio_MKV11Z7)

include(middleware_fmstr_platform_gen32le_MKV11Z7)

include(CMSIS_Include_core_cm0plus_MKV11Z7)

include(CMSIS_Include_common_MKV11Z7)

include(CMSIS_Include_dsp_MKV11Z7)

include(utilities_misc_utilities_MKV11Z7)

include(device_MKV11Z7_system_MKV11Z7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


