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


project(flashloader_cm4)

set(MCUX_SDK_PROJECT_NAME flashloader_cm4.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../usb_device_hid.c"
"${ProjDirPath}/../usb_device_hid.h"
"${ProjDirPath}/../usb_device_ch9.c"
"${ProjDirPath}/../usb_device_ch9.h"
"${ProjDirPath}/../usb_device_class.c"
"${ProjDirPath}/../usb_device_class.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/drivers
    ${ProjDirPath}/../../../../../../middleware/mcu-boot
    ${ProjDirPath}/../../../../../../middleware
    ${ProjDirPath}/../../../../../../middleware/usb
    ${ProjDirPath}/../../../../../../components/osa
    ${ProjDirPath}/../../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../../components/lists
)

# include modules
include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm4)

include(driver_clock_MIMXRT1176_cm4)

include(driver_pmu_1_MIMXRT1176_cm4)

include(driver_igpio_MIMXRT1176_cm4)

include(driver_lpuart_MIMXRT1176_cm4)

include(driver_iomuxc_MIMXRT1176_cm4)

include(middleware_mcu-boot_MIMXRT1176_startup_MIMXRT1176_cm4)

include(middleware_mcu-boot_src_bm_usb_hid_MIMXRT1176_cm4)

include(middleware_mcu-boot_bootsources_MIMXRT1176_cm4)

include(middleware_mcu-boot_mem_sdmmc_MIMXRT1176_cm4)

include(middleware_mcu-boot_MIMXRT1176_sources_MIMXRT1176_cm4)

include(middleware_baremetal_MIMXRT1176_cm4)

include(middleware_usb_device_controller_driver_MIMXRT1176_cm4)

include(component_osa_MIMXRT1176_cm4)

include(middleware_usb_device_ehci_MIMXRT1176_cm4)

include(middleware_mcu-boot_drv_semc_nand_MIMXRT1176_cm4)

include(middleware_mcu-boot_src_startup_MIMXRT1176_cm4)

include(middleware_mcu-boot_drv_microseconds_pit_MIMXRT1176_cm4)

include(middleware_mcu-boot_mem_flexspi_nor_MIMXRT1176_cm4)

include(middleware_mcu-boot_drv_flexspi_nand_MIMXRT1176_cm4)

include(middleware_mcu-boot_mem_spi_nand_MIMXRT1176_cm4)

include(middleware_mcu-boot_drv_ocotp_MIMXRT1176_cm4)

include(middleware_mcu-boot_mem_spi_nor_eeprom_MIMXRT1176_cm4)

include(CMSIS_Include_core_cm_MIMXRT1176_cm4)

include(driver_common_MIMXRT1176_cm4)

include(driver_anatop_ai_MIMXRT1176_cm4)

include(middleware_mcu-boot_drv_sdmmc_MIMXRT1176_cm4)

include(component_lists_MIMXRT1176_cm4)

include(middleware_usb_phy_MIMXRT1176_cm4)

include(middleware_usb_common_header_MIMXRT1176_cm4)

include(middleware_usb_device_common_header_MIMXRT1176_cm4)

include(middleware_mcu-boot_drv_flexspi_nor_MIMXRT1176_cm4)

include(middleware_mcu-boot_drv_serial_nor_eeprom_MIMXRT1176_cm4)

include(driver_lpspi_MIMXRT1176_cm4)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Osrec ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/flashloader.srec)

