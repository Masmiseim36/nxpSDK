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


project(freedom_bootloader)

set(MCUX_SDK_PROJECT_NAME freedom_bootloader.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../usb_device_hid.c"
"${ProjDirPath}/../usb_device_hid.h"
"${ProjDirPath}/../usb_device_msc.c"
"${ProjDirPath}/../usb_device_msc.h"
"${ProjDirPath}/../usb_device_msc_ufi.c"
"${ProjDirPath}/../usb_device_msc_ufi.h"
"${ProjDirPath}/../usb_device_ch9.c"
"${ProjDirPath}/../usb_device_ch9.h"
"${ProjDirPath}/../usb_device_class.c"
"${ProjDirPath}/../usb_device_class.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/K32L2A41A/drivers
    ${ProjDirPath}/../../../../../devices/K32L2A41A
    ${ProjDirPath}/../../../../../middleware
    ${ProjDirPath}/../../../../../middleware/usb
    ${ProjDirPath}/../../../../../components/osa
    ${ProjDirPath}/../../../../../middleware/mcu-boot
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../components/lists
)

# include modules
include(driver_clock_K32L2A41A)

include(driver_flash_K32L2A41A)

include(device_K32L2A41A_CMSIS_K32L2A41A)

include(driver_lpuart_K32L2A41A)

include(driver_gpio_K32L2A41A)

include(driver_lpi2c_K32L2A41A)

include(driver_port_K32L2A41A)

include(driver_lpspi_K32L2A41A)

include(driver_crc_K32L2A41A)

include(driver_intmux_K32L2A41A)

include(driver_smc_K32L2A41A)

include(driver_wdog32_K32L2A41A)

include(middleware_baremetal_K32L2A41A)

include(middleware_usb_device_controller_driver_K32L2A41A)

include(component_osa_K32L2A41A)

include(middleware_usb_device_khci_K32L2A41A)

include(middleware_mcu-boot_K32L2A41A_startup_K32L2A41A)

include(middleware_mcu-boot_bootsources_K32L2A41A)

include(middleware_mcu-boot_bootsources_kinetis_K32L2A41A)

include(middleware_mcu-boot_K32L2A41A_sources_K32L2A41A)

include(middleware_mcu-boot_src_bm_usb_K32L2A41A)

include(middleware_mcu-boot_drv_mmcau_K32L2A41A)

include(middleware_mcu-boot_src_security_K32L2A41A)

include(middleware_mcu-boot_src_startup_K32L2A41A)

include(middleware_mcu-boot_drv_microseconds_K32L2A41A)

include(driver_common_K32L2A41A)

include(CMSIS_Include_core_cm0plus_K32L2A41A)

include(CMSIS_Include_common_K32L2A41A)

include(CMSIS_Include_dsp_K32L2A41A)

include(component_lists_K32L2A41A)

include(middleware_usb_device_common_header_K32L2A41A)

include(middleware_usb_common_header_K32L2A41A)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


