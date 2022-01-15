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


project(wifi_serial_iperf3)

set(MCUX_SDK_PROJECT_NAME wifi_serial_iperf3.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../iperf_mwm.c"
"${ProjDirPath}/../iperf_api.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../usb_device_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../../common
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/serial_mwm
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../middleware/usb
    ${ProjDirPath}/../../../../../components/osa
    ${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel
    ${ProjDirPath}/../../../../../devices/MIMXRT1011
    ${ProjDirPath}/../../../../../devices/MIMXRT1011/utilities
    ${ProjDirPath}/../../../../../devices/MIMXRT1011/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
)

# include modules
include(component_serial_mwm_MIMXRT1011)

include(component_serial_mwm_lpuart_MIMXRT1011)

include(component_serial_manager_MIMXRT1011)

include(driver_common_MIMXRT1011)

include(component_lists_MIMXRT1011)

include(component_serial_manager_usb_cdc_MIMXRT1011)

include(middleware_usb_device_controller_driver_MIMXRT1011)

include(component_osa_MIMXRT1011)

include(middleware_usb_device_ehci_MIMXRT1011)

include(driver_cache_armv7_m7_MIMXRT1011)

include(middleware_freertos-kernel_heap_4_MIMXRT1011)

include(middleware_freertos-kernel_MIMXRT1011)

include(driver_clock_MIMXRT1011)

include(device_MIMXRT1011_CMSIS_MIMXRT1011)

include(utility_debug_console_MIMXRT1011)

include(driver_lpuart_MIMXRT1011)

include(device_MIMXRT1011_startup_MIMXRT1011)

include(driver_iomuxc_MIMXRT1011)

include(utility_assert_MIMXRT1011)

include(driver_igpio_MIMXRT1011)

include(driver_xip_device_MIMXRT1011)

include(driver_xip_board_evkmimxrt1010_MIMXRT1011)

include(driver_lpuart_freertos_MIMXRT1011)

include(CMSIS_Include_core_cm_MIMXRT1011)

include(middleware_freertos-kernel_extension_MIMXRT1011)

include(middleware_usb_device_cdc_external_MIMXRT1011)

include(middleware_usb_device_stack_external_MIMXRT1011)

include(middleware_usb_phy_MIMXRT1011)

include(middleware_usb_common_header_MIMXRT1011)

include(middleware_usb_device_common_header_MIMXRT1011)

include(utilities_misc_utilities_MIMXRT1011)

include(device_MIMXRT1011_system_MIMXRT1011)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


