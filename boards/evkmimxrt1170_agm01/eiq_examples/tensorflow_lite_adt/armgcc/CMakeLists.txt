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


project(tensorflow_lite_adt_cm7)

set(MCUX_SDK_PROJECT_NAME tensorflow_lite_adt_cm7.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../frdm_stbc_agm01_shield.h"
"${ProjDirPath}/../issdk_hal.h"
"${ProjDirPath}/../RTE_Device.h"
"${ProjDirPath}/../evkmimxrt1170.c"
"${ProjDirPath}/../evkmimxrt1170.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/adt/adt.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/adt/adt_model.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/adt/parameters.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/adt/get_sensor_data.c"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/adt/get_sensor_data.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/adt
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../middleware/issdk
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/drivers
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/cmsis_drivers
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../CMSIS/Driver/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/eiq
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../devices/MIMXRT1176
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/xip
    ${ProjDirPath}/../../../../evkmimxrt1170/xip
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/utilities
)

# include modules
include(middleware_issdk_drivers_systick_utils_MIMXRT1176_cm7)

include(middleware_issdk_drivers_systick_utils_evkmimxrt1170_MIMXRT1176_cm7)

include(middleware_issdk_drivers_gpio_imxrt_MIMXRT1176_cm7)

include(middleware_issdk_sensor_interface_common_MIMXRT1176_cm7)

include(middleware_issdk_sensor_fxas21002_MIMXRT1176_cm7)

include(middleware_issdk_sensor_fxos8700_MIMXRT1176_cm7)

include(driver_edma_MIMXRT1176_cm7)

include(driver_dmamux_MIMXRT1176_cm7)

include(driver_lpi2c_edma_MIMXRT1176_cm7)

include(driver_cmsis_lpi2c_MIMXRT1176_cm7)

include(driver_lpspi_MIMXRT1176_cm7)

include(driver_lpspi_edma_MIMXRT1176_cm7)

include(driver_cmsis_lpspi_MIMXRT1176_cm7)

include(driver_lpuart_MIMXRT1176_cm7)

include(driver_lpuart_edma_MIMXRT1176_cm7)

include(driver_cmsis_lpuart_MIMXRT1176_cm7)

include(driver_gpc_3_MIMXRT1176_cm7)

include(CMSIS_Include_core_cm7_MIMXRT1176_cm7)

include(CMSIS_Include_common_MIMXRT1176_cm7)

include(CMSIS_Driver_Include_Common_MIMXRT1176_cm7)

include(CMSIS_Driver_Include_SPI_MIMXRT1176_cm7)

include(CMSIS_Driver_Include_USART_MIMXRT1176_cm7)

include(CMSIS_Driver_Include_I2C_MIMXRT1176_cm7)

include(utility_debug_console_lite_MIMXRT1176_cm7)

include(utility_assert_lite_MIMXRT1176_cm7)

include(middleware_eiq_tensorflow_lite_binary_MIMXRT1176_cm7)

include(component_lists_MIMXRT1176_cm7)

include(component_lpuart_adapter_MIMXRT1176_cm7)

include(driver_common_MIMXRT1176_cm7)

include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm7)

include(device_MIMXRT1176_startup_MIMXRT1176_cm7)

include(driver_dcdc_soc_MIMXRT1176_cm7)

include(driver_pmu_1_MIMXRT1176_cm7)

include(driver_clock_MIMXRT1176_cm7)

include(driver_igpio_MIMXRT1176_cm7)

include(driver_iomuxc_MIMXRT1176_cm7)

include(driver_lpi2c_MIMXRT1176_cm7)

include(driver_xip_device_MIMXRT1176_cm7)

include(driver_xip_board_MIMXRT1176_cm7)

include(driver_anatop_ai_MIMXRT1176_cm7)

include(CMSIS_Include_dsp_MIMXRT1176_cm7)

include(utilities_misc_utilities_MIMXRT1176_cm7)

include(device_MIMXRT1176_system_MIMXRT1176_cm7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/lib/cm7/gcc/libtensorflow-lite.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/lib/cm7/gcc/libtensorflow-lite.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


