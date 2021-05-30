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
    ${ProjDirPath}/../../../../../devices/MK66F18/drivers
    ${ProjDirPath}/../../../../../devices/MK66F18
    ${ProjDirPath}/../../../../../middleware
    ${ProjDirPath}/../../../../../middleware/usb
    ${ProjDirPath}/../../../../../components/osa
    ${ProjDirPath}/../../../../../middleware/mcu-boot
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../components/lists
)

# include modules
include(driver_clock_MK66F18)

include(driver_flash_MK66F18)

include(device_MK66F18_CMSIS_MK66F18)

include(driver_uart_MK66F18)

include(driver_gpio_MK66F18)

include(driver_i2c_MK66F18)

include(driver_port_MK66F18)

include(driver_dspi_MK66F18)

include(driver_crc_MK66F18)

include(middleware_baremetal_MK66F18)

include(middleware_usb_device_controller_driver_MK66F18)

include(component_osa_MK66F18)

include(middleware_usb_device_khci_MK66F18)

include(middleware_usb_device_ehci_MK66F18)

include(middleware_mcu-boot_MK66F18_startup_MK66F18)

include(middleware_mcu-boot_bootsources_MK66F18)

include(middleware_mcu-boot_bootsources_kinetis_MK66F18)

include(middleware_mcu-boot_MK66F18_sources_MK66F18)

include(middleware_mcu-boot_src_bm_usb_MK66F18)

include(middleware_mcu-boot_drv_mmcau_MK66F18)

include(middleware_mcu-boot_src_security_MK66F18)

include(middleware_mcu-boot_src_startup_MK66F18)

include(middleware_mcu-boot_drv_microseconds_pit_MK66F18)

include(driver_common_MK66F18)

include(CMSIS_Include_core_cm4_MK66F18)

include(CMSIS_Include_common_MK66F18)

include(CMSIS_Include_dsp_MK66F18)

include(component_lists_MK66F18)

include(middleware_usb_device_common_header_MK66F18)

include(middleware_usb_common_header_MK66F18)

include(middleware_usb_phy_MK66F18)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


