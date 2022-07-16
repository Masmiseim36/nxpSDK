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


project(flexio_spi_edma_lpspi_transfer_slave_cm7)

set(MCUX_SDK_PROJECT_NAME flexio_spi_edma_lpspi_transfer_slave_cm7.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../flexio_spi_edma_lpspi_transfer_slave.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
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
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../../../../devices/MIMXRT1166/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../../../../../devices/MIMXRT1166/drivers
    ${ProjDirPath}/../../../../../../../../../devices/MIMXRT1166
    ${ProjDirPath}/../../../../../../../../../components/uart
    ${ProjDirPath}/../../../../../../../../../components/lists
    ${ProjDirPath}/../../../../../../../../../devices/MIMXRT1166/xip
    ${ProjDirPath}/../../../../../../../xip
    ${ProjDirPath}/../../../../../../../../../devices/MIMXRT1166/drivers/cm7
    ${ProjDirPath}/../../../../../../../../../components/silicon_id
    ${ProjDirPath}/../../../../../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../../../../../devices/MIMXRT1166/utilities
)

# include modules
include(utility_debug_console_lite_MIMXRT1166_cm7)

include(utility_assert_lite_MIMXRT1166_cm7)

include(driver_dmamux_MIMXRT1166_cm7)

include(driver_edma_MIMXRT1166_cm7)

include(driver_clock_MIMXRT1166_cm7)

include(driver_flexio_MIMXRT1166_cm7)

include(driver_flexio_spi_MIMXRT1166_cm7)

include(driver_flexio_spi_edma_MIMXRT1166_cm7)

include(driver_lpspi_MIMXRT1166_cm7)

include(driver_common_MIMXRT1166_cm7)

include(device_MIMXRT1166_CMSIS_MIMXRT1166_cm7)

include(component_lpuart_adapter_MIMXRT1166_cm7)

include(component_lists_MIMXRT1166_cm7)

include(driver_lpuart_MIMXRT1166_cm7)

include(device_MIMXRT1166_startup_MIMXRT1166_cm7)

include(driver_iomuxc_MIMXRT1166_cm7)

include(driver_igpio_MIMXRT1166_cm7)

include(driver_xip_device_MIMXRT1166_cm7)

include(driver_xip_board_evkmimxrt1160_MIMXRT1166_cm7)

include(driver_pmu_1_MIMXRT1166_cm7)

include(driver_dcdc_soc_MIMXRT1166_cm7)

include(driver_cache_armv7_m7_MIMXRT1166_cm7)

include(component_silicon_id_MIMXRT1166_cm7)

include(driver_anatop_ai_MIMXRT1166_cm7)

include(CMSIS_Include_core_cm_MIMXRT1166_cm7)

include(driver_memory_MIMXRT1166_cm7)

include(utilities_misc_utilities_MIMXRT1166_cm7)

include(device_MIMXRT1166_system_MIMXRT1166_cm7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


