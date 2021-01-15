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


project(dev_cdc_vnic_freertos)

set(MCUX_SDK_PROJECT_NAME dev_cdc_vnic_freertos.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../virtual_nic_enet_adapter.c"
"${ProjDirPath}/../virtual_nic_enet_adapter.h"
"${ProjDirPath}/../virtual_nic_enetif.c"
"${ProjDirPath}/../virtual_nic_enetif.h"
"${ProjDirPath}/../usb_device_descriptor.c"
"${ProjDirPath}/../usb_device_descriptor.h"
"${ProjDirPath}/../virtual_nic.c"
"${ProjDirPath}/../virtual_nic.h"
"${ProjDirPath}/../usb_device_config.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../usb_device_ch9.c"
"${ProjDirPath}/../usb_device_ch9.h"
"${ProjDirPath}/../usb_device_class.c"
"${ProjDirPath}/../usb_device_class.h"
"${ProjDirPath}/../usb_device_cdc_acm.c"
"${ProjDirPath}/../usb_device_cdc_acm.h"
"${ProjDirPath}/../usb_device_cdc_rndis.c"
"${ProjDirPath}/../usb_device_cdc_rndis.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/.. sdram_debug
    ${ProjDirPath}/../../../.. sdram_debug
    ${ProjDirPath}/..
    ${ProjDirPath}/../../../..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../middleware/usb
    ${ProjDirPath}/../../../../../../devices/MIMXRT1064/drivers
    ${ProjDirPath}/../../../../../../components/phyksz8081
    ${ProjDirPath}/../../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../../devices/MIMXRT1064
    ${ProjDirPath}/../../../../../../devices/MIMXRT1064/utilities
    ${ProjDirPath}/../../../../../../devices/MIMXRT1064/xip
    ${ProjDirPath}/../../../../xip
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../components/osa
    ${ProjDirPath}/../../../../../../CMSIS/Include
)

# include modules
include(middleware_usb_device_ehci_MIMXRT1064)

include(middleware_usb_phy_MIMXRT1064)

include(driver_cache_armv7_m7_MIMXRT1064)

include(driver_phyksz8081_MIMXRT1064)

include(driver_igpio_MIMXRT1064)

include(driver_enet_MIMXRT1064)

include(middleware_freertos-kernel_heap_4_MIMXRT1064)

include(middleware_freertos-kernel_MIMXRT1064)

include(driver_clock_MIMXRT1064)

include(driver_common_MIMXRT1064)

include(device_MIMXRT1064_CMSIS_MIMXRT1064)

include(device_MIMXRT1064_startup_MIMXRT1064)

include(driver_lpuart_MIMXRT1064)

include(driver_iomuxc_MIMXRT1064)

include(utility_assert_MIMXRT1064)

include(driver_xip_device_MIMXRT1064)

include(driver_xip_board_MIMXRT1064)

include(utility_debug_console_MIMXRT1064)

include(component_lpuart_adapter_MIMXRT1064)

include(component_serial_manager_MIMXRT1064)

include(component_lists_MIMXRT1064)

include(component_serial_manager_uart_MIMXRT1064)

include(middleware_usb_device_controller_driver_MIMXRT1064)

include(component_osa_MIMXRT1064)

include(middleware_usb_common_header_MIMXRT1064)

include(CMSIS_Include_core_cm7_MIMXRT1064)

include(CMSIS_Include_common_MIMXRT1064)

include(CMSIS_Include_dsp_MIMXRT1064)

include(middleware_usb_device_common_header_MIMXRT1064)

include(middleware_freertos-kernel_extension_MIMXRT1064)

include(utilities_misc_utilities_MIMXRT1064)

include(device_MIMXRT1064_system_MIMXRT1064)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


