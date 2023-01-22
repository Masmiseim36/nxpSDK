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


project(usb_pd_sink_battery_bm)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME usb_pd_sink_battery_bm.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../pd_board_config.h"
"${ProjDirPath}/../pd_app_demo.c"
"${ProjDirPath}/../pd_app.c"
"${ProjDirPath}/../pd_app.h"
"${ProjDirPath}/../pd_command_app.c"
"${ProjDirPath}/../pd_command_interface.c"
"${ProjDirPath}/../pd_command_interface.h"
"${ProjDirPath}/../pd_power_app.c"
"${ProjDirPath}/../pd_power_interface.c"
"${ProjDirPath}/../pd_power_interface.h"
"${ProjDirPath}/../usb_pd_config.h"
"${ProjDirPath}/../pd_power_nx20p3483.c"
"${ProjDirPath}/../pd_power_nx20p3483.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT1015/drivers
    ${SdkRootDirPath}/components/i2c
    ${SdkRootDirPath}/middleware/usb
    ${SdkRootDirPath}/devices/MIMXRT1015/utilities
    ${SdkRootDirPath}/devices/MIMXRT1015/utilities/debug_console_lite
    ${SdkRootDirPath}/middleware
    ${SdkRootDirPath}/components/osa
    ${SdkRootDirPath}/devices/MIMXRT1015
    ${SdkRootDirPath}/devices/MIMXRT1015/xip
    ${SdkRootDirPath}/boards/evkmimxrt1015/xip
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/components/gpio
    ${SdkRootDirPath}/components/timer
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(driver_igpio_MIMXRT1015)

include(driver_gpt_MIMXRT1015)

include(driver_lpi2c_MIMXRT1015)

include(driver_lpi2c_edma_MIMXRT1015)

include(driver_edma_MIMXRT1015)

include(driver_dmamux_MIMXRT1015)

include(component_lpi2c_adapter_MIMXRT1015)

include(middleware_usb_pd_phy_ptn5110_MIMXRT1015)

include(utility_debug_console_lite_MIMXRT1015)

include(utility_assert_lite_MIMXRT1015)

include(middleware_baremetal_MIMXRT1015)

include(middleware_usb_pd_MIMXRT1015)

include(component_osa_bm_MIMXRT1015)

include(driver_clock_MIMXRT1015)

include(driver_common_MIMXRT1015)

include(device_MIMXRT1015_CMSIS_MIMXRT1015)

include(device_MIMXRT1015_startup_MIMXRT1015)

include(driver_lpuart_MIMXRT1015)

include(driver_iomuxc_MIMXRT1015)

include(driver_xip_device_MIMXRT1015)

include(driver_xip_board_evkmimxrt1015_MIMXRT1015)

include(component_lpuart_adapter_MIMXRT1015)

include(component_lists_MIMXRT1015)

include(component_igpio_adapter_MIMXRT1015)

include(component_gpt_adapter_MIMXRT1015)

include(CMSIS_Include_core_cm_MIMXRT1015)

include(component_osa_MIMXRT1015)

include(middleware_usb_pd_common_header_MIMXRT1015)

include(utilities_misc_utilities_MIMXRT1015)

include(device_MIMXRT1015_system_MIMXRT1015)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


