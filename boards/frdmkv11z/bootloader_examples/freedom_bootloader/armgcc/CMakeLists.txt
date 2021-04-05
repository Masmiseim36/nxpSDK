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

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MKV11Z7/drivers
    ${ProjDirPath}/../../../../../devices/MKV11Z7
    ${ProjDirPath}/../../../../../middleware/mcu-boot
    ${ProjDirPath}/../../../../../components/osa
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../components/lists
)

# include modules
include(driver_clock_MKV11Z7)

include(driver_flash_MKV11Z7)

include(device_MKV11Z7_CMSIS_MKV11Z7)

include(driver_uart_MKV11Z7)

include(driver_gpio_MKV11Z7)

include(driver_i2c_MKV11Z7)

include(driver_port_MKV11Z7)

include(driver_dspi_MKV11Z7)

include(driver_crc_MKV11Z7)

include(driver_flexcan_MKV11Z7)

include(middleware_mcu-boot_MKV11Z7_startup_MKV11Z7)

include(middleware_mcu-boot_bootsources_MKV11Z7)

include(middleware_mcu-boot_bootsources_kinetis_MKV11Z7)

include(middleware_mcu-boot_MKV11Z7_sources_MKV11Z7)

include(middleware_mcu-boot_src_startup_MKV11Z7)

include(middleware_mcu-boot_drv_microseconds_MKV11Z7)

include(component_osa_bm_MKV11Z7)

include(driver_common_MKV11Z7)

include(CMSIS_Include_core_cm0plus_MKV11Z7)

include(CMSIS_Include_common_MKV11Z7)

include(CMSIS_Include_dsp_MKV11Z7)

include(component_lists_MKV11Z7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


