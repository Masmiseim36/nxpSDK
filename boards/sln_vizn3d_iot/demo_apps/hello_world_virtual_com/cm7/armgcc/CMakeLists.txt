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


project(hello_world_virtual_com_cm7)

set(MCUX_SDK_PROJECT_NAME hello_world_virtual_com_cm7.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../hello_world.c"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../usb_device_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../..
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H/drivers
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../middleware/usb
    ${ProjDirPath}/../../../../../../components/osa
    ${ProjDirPath}/../../../../../../middleware
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H
    ${ProjDirPath}/../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H/utilities
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H/xip
    ${ProjDirPath}/../../../../xip
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H/drivers/cm7
)

# include modules
include(component_serial_manager_MIMXRT117H)

include(driver_common_MIMXRT117H)

include(component_lists_MIMXRT117H)

include(component_serial_manager_usb_cdc_MIMXRT117H)

include(middleware_usb_phy_MIMXRT117H)

include(middleware_usb_device_controller_driver_MIMXRT117H)

include(component_osa_MIMXRT117H)

include(middleware_usb_device_ehci_MIMXRT117H)

include(middleware_baremetal_MIMXRT117H)

include(driver_clock_MIMXRT117H)

include(device_MIMXRT117H_CMSIS_MIMXRT117H)

include(CMSIS_Include_core_cm7_MIMXRT117H)

include(utility_debug_console_MIMXRT117H)

include(component_lpuart_adapter_MIMXRT117H)

include(component_serial_manager_uart_MIMXRT117H)

include(driver_lpuart_MIMXRT117H)

include(device_MIMXRT117H_startup_MIMXRT117H)

include(driver_iomuxc_MIMXRT117H)

include(utility_assert_MIMXRT117H)

include(driver_igpio_MIMXRT117H)

include(driver_xip_device_MIMXRT117H)

include(driver_xip_board_MIMXRT117H)

include(driver_pmu_1_MIMXRT117H)

include(driver_dcdc_soc_MIMXRT117H)

include(driver_cache_armv7_m7_MIMXRT117H)

include(driver_anatop_ai_MIMXRT117H)

include(middleware_usb_device_cdc_external_MIMXRT117H)

include(middleware_usb_device_stack_external_MIMXRT117H)

include(middleware_usb_common_header_MIMXRT117H)

include(middleware_usb_device_common_header_MIMXRT117H)

include(utilities_misc_utilities_MIMXRT117H)

include(CMSIS_Include_common_MIMXRT117H)

include(CMSIS_Include_dsp_MIMXRT117H)

include(device_MIMXRT117H_system_MIMXRT117H)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


