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


project(lpspi_edma_b2b_transfer_slave)

set(MCUX_SDK_PROJECT_NAME lpspi_edma_b2b_transfer_slave.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../lpspi_edma_b2b_transfer_slave.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../../devices/MKE17Z7/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../../../devices/MKE17Z7/drivers
    ${ProjDirPath}/../../../../../../../devices/MKE17Z7
    ${ProjDirPath}/../../../../../../../components/uart
    ${ProjDirPath}/../../../../../../../components/lists
    ${ProjDirPath}/../../../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../../../devices/MKE17Z7/utilities
)

# include modules
include(utility_debug_console_lite_MKE17Z7)

include(utility_assert_lite_MKE17Z7)

include(driver_port_MKE17Z7)

include(driver_lpspi_MKE17Z7)

include(driver_lpspi_edma_MKE17Z7)

include(driver_edma_MKE17Z7)

include(driver_dmamux_MKE17Z7)

include(driver_clock_MKE17Z7)

include(driver_common_MKE17Z7)

include(device_MKE17Z7_CMSIS_MKE17Z7)

include(component_lpuart_adapter_MKE17Z7)

include(component_lists_MKE17Z7)

include(driver_lpuart_MKE17Z7)

include(device_MKE17Z7_startup_MKE17Z7)

include(driver_smc_MKE17Z7)

include(driver_gpio_MKE17Z7)

include(CMSIS_Include_core_cm_MKE17Z7)

include(utilities_misc_utilities_MKE17Z7)

include(device_MKE17Z7_system_MKE17Z7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


