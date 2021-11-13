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


project(fmstr_example_pdbdm)

set(MCUX_SDK_PROJECT_NAME fmstr_example_pdbdm.elf)

include(${ProjDirPath}/flags.cmake)

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
    ${ProjDirPath}/../../../../../middleware/freemaster
    ${ProjDirPath}/../../../../../devices/MKE16Z4/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../devices/MKE16Z4/drivers
    ${ProjDirPath}/../../../../../devices/MKE16Z4
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../devices/MKE16Z4/utilities
)

# include modules
include(middleware_fmstr_MKE16Z4)

include(utility_debug_console_lite_MKE16Z4)

include(utility_assert_lite_MKE16Z4)

include(driver_clock_MKE16Z4)

include(driver_common_MKE16Z4)

include(device_MKE16Z4_CMSIS_MKE16Z4)

include(component_lists_MKE16Z4)

include(device_MKE16Z4_startup_MKE16Z4)

include(driver_lpuart_MKE16Z4)

include(driver_port_MKE16Z4)

include(driver_smc_MKE16Z4)

include(driver_gpio_MKE16Z4)

include(middleware_fmstr_platform_gen32le_MKE16Z4)

include(component_lpuart_adapter_MKE16Z4)

include(CMSIS_Include_core_cm_MKE16Z4)

include(utilities_misc_utilities_MKE16Z4)

include(device_MKE16Z4_system_MKE16Z4)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


