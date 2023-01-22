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


project(emwin_gui_demo)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME emwin_gui_demo.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../RTE_Device.h"
"${ProjDirPath}/../emwin_gui_demo.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../emwin_support.c"
"${ProjDirPath}/../emwin_support.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkmimxrt1010/emwin_examples/common
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/components/ili9341
    ${SdkRootDirPath}/components/ft6x06
    ${SdkRootDirPath}/devices/MIMXRT1011/drivers
    ${SdkRootDirPath}/devices/MIMXRT1011/cmsis_drivers
    ${SdkRootDirPath}/devices/MIMXRT1011/utilities
    ${SdkRootDirPath}/devices/MIMXRT1011/utilities/debug_console_lite
    ${SdkRootDirPath}/middleware/emwin
    ${SdkRootDirPath}/devices/MIMXRT1011
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1011/xip
    ${SdkRootDirPath}/boards/evkmimxrt1010/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/CMSIS/Driver/Include
)

# include modules
include(driver_ili9341_MIMXRT1011)

include(driver_ft6x06_MIMXRT1011)

include(driver_clock_MIMXRT1011)

include(driver_lpuart_MIMXRT1011)

include(driver_cache_armv7_m7_MIMXRT1011)

include(driver_edma_MIMXRT1011)

include(driver_dmamux_MIMXRT1011)

include(driver_lpi2c_MIMXRT1011)

include(driver_lpi2c_edma_MIMXRT1011)

include(driver_lpspi_edma_MIMXRT1011)

include(driver_cmsis_lpspi_MIMXRT1011)

include(driver_cmsis_lpi2c_MIMXRT1011)

include(utility_debug_console_lite_MIMXRT1011)

include(utility_assert_lite_MIMXRT1011)

include(middleware_emwin_MIMXRT1011)

include(driver_common_MIMXRT1011)

include(device_MIMXRT1011_CMSIS_MIMXRT1011)

include(component_lpuart_adapter_MIMXRT1011)

include(component_lists_MIMXRT1011)

include(device_MIMXRT1011_startup_MIMXRT1011)

include(driver_iomuxc_MIMXRT1011)

include(driver_igpio_MIMXRT1011)

include(driver_xip_device_MIMXRT1011)

include(driver_xip_board_evkmimxrt1010_MIMXRT1011)

include(CMSIS_Include_core_cm_MIMXRT1011)

include(CMSIS_Driver_Include_I2C_MIMXRT1011)

include(CMSIS_Driver_Include_Common_MIMXRT1011)

include(driver_lpspi_MIMXRT1011)

include(CMSIS_Driver_Include_SPI_MIMXRT1011)

include(utilities_misc_utilities_MIMXRT1011)

include(device_MIMXRT1011_system_MIMXRT1011)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/emwin/emWin_library/ARMGCC/libemWin_M7.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/emwin/emWin_library/ARMGCC/libemWin_M7.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


