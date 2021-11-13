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


project(freedom_bootloader)

set(MCUX_SDK_PROJECT_NAME freedom_bootloader.elf)

include(${ProjDirPath}/flags.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MKE16Z4/drivers
    ${ProjDirPath}/../../../../../devices/MKE16Z4
    ${ProjDirPath}/../../../../../middleware/mcu-boot
    ${ProjDirPath}/../../../../../components/osa
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../components/lists
)

# include modules
include(driver_clock_MKE16Z4)

include(driver_flash_MKE16Z4)

include(device_MKE16Z4_CMSIS_MKE16Z4)

include(driver_lpuart_MKE16Z4)

include(driver_gpio_MKE16Z4)

include(driver_lpi2c_MKE16Z4)

include(driver_port_MKE16Z4)

include(driver_lpspi_MKE16Z4)

include(driver_mscan_MKE16Z4)

include(driver_crc_MKE16Z4)

include(driver_smc_MKE16Z4)

include(middleware_mcu-boot_MKE16Z4_startup_MKE16Z4)

include(middleware_mcu-boot_bootsources_MKE16Z4)

include(middleware_mcu-boot_bootsources_kinetis_MKE16Z4)

include(middleware_mcu-boot_MKE16Z4_sources_MKE16Z4)

include(middleware_mcu-boot_src_startup_MKE16Z4)

include(middleware_mcu-boot_drv_microseconds_MKE16Z4)

include(component_osa_bm_MKE16Z4)

include(driver_common_MKE16Z4)

include(CMSIS_Include_core_cm_MKE16Z4)

include(component_lists_MKE16Z4)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


