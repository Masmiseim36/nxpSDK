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


project(nand_flash_management_cm4)

set(MCUX_BUILD_TYPES debug release flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME nand_flash_management_cm4.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../nand_flash_management.c"
"${ProjDirPath}/../dhara_nand_device.c"
"${ProjDirPath}/../dhara_nand_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../ffconf.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../evkmimxrt1170_connect_cm4_cm4side.jlinkscript"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkmimxrt1170/demo_apps/nand_flash_management/semc
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/components/flash/nand/semc
    ${SdkRootDirPath}/middleware/dhara
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/middleware/fatfs
    ${SdkRootDirPath}/devices/MIMXRT1176/utilities
    ${SdkRootDirPath}/devices/MIMXRT1176/drivers
    ${SdkRootDirPath}/devices/MIMXRT1176
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1176/xip
    ${SdkRootDirPath}/boards/evkmimxrt1170/xip
    ${SdkRootDirPath}/devices/MIMXRT1176/drivers/cm4
    ${SdkRootDirPath}/components/flash/nand
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(driver_nand_flash-controller-semc_MIMXRT1176_cm4)

include(middleware_dhara_MIMXRT1176_cm4)

include(middleware_dhara_dhara_ecc_hamming_MIMXRT1176_cm4)

include(middleware_freertos-kernel_heap_4_MIMXRT1176_cm4)

include(middleware_fatfs_nand_MIMXRT1176_cm4)

include(middleware_fatfs_MIMXRT1176_cm4)

include(utility_shell_MIMXRT1176_cm4)

include(driver_clock_MIMXRT1176_cm4)

include(driver_common_MIMXRT1176_cm4)

include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm4)

include(utility_debug_console_MIMXRT1176_cm4)

include(component_lpuart_adapter_MIMXRT1176_cm4)

include(component_serial_manager_MIMXRT1176_cm4)

include(component_lists_MIMXRT1176_cm4)

include(component_serial_manager_uart_MIMXRT1176_cm4)

include(driver_lpuart_MIMXRT1176_cm4)

include(device_MIMXRT1176_startup_MIMXRT1176_cm4)

include(driver_iomuxc_MIMXRT1176_cm4)

include(utility_assert_MIMXRT1176_cm4)

include(driver_igpio_MIMXRT1176_cm4)

include(driver_xip_device_MIMXRT1176_cm4)

include(driver_xip_board_evkmimxrt1170_MIMXRT1176_cm4)

include(driver_pmu_1_MIMXRT1176_cm4)

include(driver_dcdc_soc_MIMXRT1176_cm4)

include(driver_cache_lmem_MIMXRT1176_cm4)

include(driver_nand_flash-common_MIMXRT1176_cm4)

include(driver_semc_MIMXRT1176_cm4)

include(driver_anatop_ai_MIMXRT1176_cm4)

include(CMSIS_Include_core_cm_MIMXRT1176_cm4)

include(middleware_freertos-kernel_MIMXRT1176_cm4)

include(middleware_freertos-kernel_extension_MIMXRT1176_cm4)

include(utilities_misc_utilities_MIMXRT1176_cm4)

include(device_MIMXRT1176_system_MIMXRT1176_cm4)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


