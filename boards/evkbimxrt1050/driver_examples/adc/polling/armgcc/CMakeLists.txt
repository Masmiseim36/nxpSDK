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


project(adc_12b1msps_sar_polling)

set(MCUX_BUILD_TYPES debug release sdram_debug sdram_release flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME adc_12b1msps_sar_polling.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../adc_polling.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT1052/utilities
    ${SdkRootDirPath}/devices/MIMXRT1052/utilities/debug_console_lite
    ${SdkRootDirPath}/devices/MIMXRT1052/drivers
    ${SdkRootDirPath}/devices/MIMXRT1052
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1052/xip
    ${SdkRootDirPath}/boards/evkbimxrt1050/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(utility_debug_console_lite_MIMXRT1052)

include(utility_assert_lite_MIMXRT1052)

include(driver_adc_12b1msps_sar_MIMXRT1052)

include(driver_clock_MIMXRT1052)

include(driver_common_MIMXRT1052)

include(device_MIMXRT1052_CMSIS_MIMXRT1052)

include(component_lpuart_adapter_MIMXRT1052)

include(component_lists_MIMXRT1052)

include(driver_lpuart_MIMXRT1052)

include(device_MIMXRT1052_startup_MIMXRT1052)

include(driver_iomuxc_MIMXRT1052)

include(driver_igpio_MIMXRT1052)

include(driver_xip_device_MIMXRT1052)

include(driver_xip_board_evkbimxrt1050_MIMXRT1052)

include(CMSIS_Include_core_cm_MIMXRT1052)

include(utilities_misc_utilities_MIMXRT1052)

include(device_MIMXRT1052_system_MIMXRT1052)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


