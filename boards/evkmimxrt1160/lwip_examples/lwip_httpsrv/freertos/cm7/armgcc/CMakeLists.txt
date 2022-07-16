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


project(lwip_httpsrv_freertos_cm7)

set(MCUX_SDK_PROJECT_NAME lwip_httpsrv_freertos_cm7.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../lwip_httpsrv_freertos.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../httpsrv_fs_data.c"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../evkmimxrt1160_connect_cm4_cm7side.jlinkscript"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../../../../..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../../components/phy/mdio/enet
    ${ProjDirPath}/../../../../../../../components/phy/device/phyksz8081
    ${ProjDirPath}/../../../../../../../components/phy/device/phyrtl8211f
    ${ProjDirPath}/../../../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../../../components/phy
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1166/drivers
    ${ProjDirPath}/../../../../../../../rtos/freertos/freertos_kernel
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1166
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1166/utilities
    ${ProjDirPath}/../../../../../../../components/uart
    ${ProjDirPath}/../../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../../components/lists
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1166/xip
    ${ProjDirPath}/../../../../../xip
    ${ProjDirPath}/../../../../../../../devices/MIMXRT1166/drivers/cm7
    ${ProjDirPath}/../../../../../../../components/silicon_id
    ${ProjDirPath}/../../../../../../../CMSIS/Core/Include
)

# include modules
include(driver_mdio-enet_MIMXRT1166_cm7)

include(driver_phy-device-ksz8081_MIMXRT1166_cm7)

include(driver_phy-device-rtl8211f_MIMXRT1166_cm7)

include(middleware_lwip_apps_httpsrv_MIMXRT1166_cm7)

include(middleware_lwip_apps_mdns_MIMXRT1166_cm7)

include(middleware_lwip_MIMXRT1166_cm7)

include(middleware_lwip_enet_ethernetif_MIMXRT1166_cm7)

include(driver_phy-common_MIMXRT1166_cm7)

include(driver_enet_MIMXRT1166_cm7)

include(driver_clock_MIMXRT1166_cm7)

include(middleware_freertos-kernel_heap_3_MIMXRT1166_cm7)

include(driver_common_MIMXRT1166_cm7)

include(device_MIMXRT1166_CMSIS_MIMXRT1166_cm7)

include(utility_debug_console_MIMXRT1166_cm7)

include(component_lpuart_adapter_MIMXRT1166_cm7)

include(component_serial_manager_MIMXRT1166_cm7)

include(component_lists_MIMXRT1166_cm7)

include(component_serial_manager_uart_MIMXRT1166_cm7)

include(driver_lpuart_MIMXRT1166_cm7)

include(device_MIMXRT1166_startup_MIMXRT1166_cm7)

include(driver_iomuxc_MIMXRT1166_cm7)

include(utility_assert_MIMXRT1166_cm7)

include(driver_igpio_MIMXRT1166_cm7)

include(driver_xip_device_MIMXRT1166_cm7)

include(driver_xip_board_evkmimxrt1160_MIMXRT1166_cm7)

include(driver_pmu_1_MIMXRT1166_cm7)

include(driver_dcdc_soc_MIMXRT1166_cm7)

include(driver_cache_armv7_m7_MIMXRT1166_cm7)

include(component_silicon_id_MIMXRT1166_cm7)

include(driver_mdio-common_MIMXRT1166_cm7)

include(driver_anatop_ai_MIMXRT1166_cm7)

include(CMSIS_Include_core_cm_MIMXRT1166_cm7)

include(driver_memory_MIMXRT1166_cm7)

include(middleware_freertos-kernel_MIMXRT1166_cm7)

include(middleware_freertos-kernel_extension_MIMXRT1166_cm7)

include(utilities_misc_utilities_MIMXRT1166_cm7)

include(device_MIMXRT1166_system_MIMXRT1166_cm7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


