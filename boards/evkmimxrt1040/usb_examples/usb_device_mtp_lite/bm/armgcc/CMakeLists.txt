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


project(dev_mtp_lite_bm)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME dev_mtp_lite_bm.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../ffconf.h"
"${ProjDirPath}/../mtp.c"
"${ProjDirPath}/../mtp.h"
"${ProjDirPath}/../mtp_operation.c"
"${ProjDirPath}/../mtp_operation.h"
"${ProjDirPath}/../mtp_file_system_adapter.c"
"${ProjDirPath}/../mtp_file_system_adapter.h"
"${ProjDirPath}/../mtp_object_handle.c"
"${ProjDirPath}/../mtp_object_handle.h"
"${ProjDirPath}/../usb_device_descriptor.c"
"${ProjDirPath}/../usb_device_descriptor.h"
"${ProjDirPath}/../usb_device_ch9.c"
"${ProjDirPath}/../usb_device_ch9.h"
"${ProjDirPath}/../usb_device_mtp.c"
"${ProjDirPath}/../usb_device_mtp.h"
"${ProjDirPath}/../usb_device_mtp_operation.c"
"${ProjDirPath}/../usb_device_mtp_operation.h"
"${ProjDirPath}/../usb_device_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${SdkRootDirPath}/boards/evkmimxrt1040
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/middleware/usb
    ${SdkRootDirPath}/devices/MIMXRT1042/drivers
    ${SdkRootDirPath}/middleware/sdmmc
    ${SdkRootDirPath}/middleware/fatfs
    ${SdkRootDirPath}/components/gpio
    ${SdkRootDirPath}/middleware
    ${SdkRootDirPath}/devices/MIMXRT1042
    ${SdkRootDirPath}/devices/MIMXRT1042/utilities
    ${SdkRootDirPath}/devices/MIMXRT1042/xip
    ${SdkRootDirPath}/boards/evkmimxrt1040/xip
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/components/osa
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(middleware_usb_device_ehci_MIMXRT1042)

include(middleware_usb_phy_MIMXRT1042)

include(driver_cache_armv7_m7_MIMXRT1042)

include(middleware_sdmmc_sd_MIMXRT1042)

include(middleware_sdmmc_common_MIMXRT1042)

include(middleware_sdmmc_host_usdhc_MIMXRT1042)

include(middleware_sdmmc_host_usdhc_interrupt_MIMXRT1042)

include(middleware_fatfs_MIMXRT1042)

include(middleware_fatfs_sd_MIMXRT1042)

include(component_igpio_adapter_MIMXRT1042)

include(middleware_baremetal_MIMXRT1042)

include(driver_clock_MIMXRT1042)

include(driver_common_MIMXRT1042)

include(device_MIMXRT1042_CMSIS_MIMXRT1042)

include(device_MIMXRT1042_startup_MIMXRT1042)

include(driver_lpuart_MIMXRT1042)

include(driver_iomuxc_MIMXRT1042)

include(utility_assert_MIMXRT1042)

include(driver_igpio_MIMXRT1042)

include(driver_xip_device_MIMXRT1042)

include(driver_xip_board_evkmimxrt1040_MIMXRT1042)

include(utility_debug_console_MIMXRT1042)

include(component_lpuart_adapter_MIMXRT1042)

include(component_serial_manager_MIMXRT1042)

include(component_lists_MIMXRT1042)

include(component_serial_manager_uart_MIMXRT1042)

include(middleware_usb_device_controller_driver_MIMXRT1042)

include(component_osa_MIMXRT1042)

include(middleware_usb_common_header_MIMXRT1042)

include(CMSIS_Include_core_cm_MIMXRT1042)

include(middleware_usb_device_common_header_MIMXRT1042)

include(middleware_sdmmc_osa_bm_MIMXRT1042)

include(component_osa_bm_MIMXRT1042)

include(driver_usdhc_MIMXRT1042)

include(utilities_misc_utilities_MIMXRT1042)

include(device_MIMXRT1042_system_MIMXRT1042)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


