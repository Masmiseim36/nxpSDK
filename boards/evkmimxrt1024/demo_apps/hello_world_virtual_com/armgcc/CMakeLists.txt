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


project(hello_world_virtual_com)

set(MCUX_BUILD_TYPES debug release sdram_debug sdram_release flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME hello_world_virtual_com.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

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

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/devices/MIMXRT1024/drivers
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/middleware/usb
    ${SdkRootDirPath}/components/osa
    ${SdkRootDirPath}/middleware
    ${SdkRootDirPath}/devices/MIMXRT1024
    ${SdkRootDirPath}/devices/MIMXRT1024/utilities
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/devices/MIMXRT1024/xip
    ${SdkRootDirPath}/boards/evkmimxrt1024/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(component_serial_manager_MIMXRT1024)

include(driver_common_MIMXRT1024)

include(component_lists_MIMXRT1024)

include(component_serial_manager_usb_cdc_MIMXRT1024)

include(middleware_usb_device_controller_driver_MIMXRT1024)

include(component_osa_MIMXRT1024)

include(middleware_usb_device_ehci_MIMXRT1024)

include(driver_cache_armv7_m7_MIMXRT1024)

include(middleware_baremetal_MIMXRT1024)

include(driver_clock_MIMXRT1024)

include(device_MIMXRT1024_CMSIS_MIMXRT1024)

include(utility_debug_console_MIMXRT1024)

include(component_lpuart_adapter_MIMXRT1024)

include(component_serial_manager_uart_MIMXRT1024)

include(driver_lpuart_MIMXRT1024)

include(device_MIMXRT1024_startup_MIMXRT1024)

include(driver_iomuxc_MIMXRT1024)

include(utility_assert_MIMXRT1024)

include(driver_igpio_MIMXRT1024)

include(driver_xip_device_MIMXRT1024)

include(driver_xip_board_evkmimxrt1024_MIMXRT1024)

include(CMSIS_Include_core_cm_MIMXRT1024)

include(middleware_usb_device_cdc_external_MIMXRT1024)

include(middleware_usb_device_stack_external_MIMXRT1024)

include(middleware_usb_phy_MIMXRT1024)

include(middleware_usb_common_header_MIMXRT1024)

include(middleware_usb_device_common_header_MIMXRT1024)

include(utilities_misc_utilities_MIMXRT1024)

include(device_MIMXRT1024_system_MIMXRT1024)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


