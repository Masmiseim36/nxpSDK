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


project(tensorflow_lite_micro_adt)

set(MCUX_SDK_PROJECT_NAME tensorflow_lite_micro_adt.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../frdm_stbc_agm01_shield.h"
"${ProjDirPath}/../issdk_hal.h"
"${ProjDirPath}/../RTE_Device.h"
"${ProjDirPath}/../evkmimxrt1060.c"
"${ProjDirPath}/../evkmimxrt1060.h"
"${ProjDirPath}/../../../../../middleware/issdk/drivers/systick/systick_utils.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/adt/main.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/adt/model_data.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/adt/sensor_data.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/adt/output_postproc.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/adt/output_postproc.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/model.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/model.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/adt/model_adt_ops_micro.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/common/demo_info.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/demo_info.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/adt/demo_config.h"
"${ProjDirPath}/../systick_utils.c"
"${ProjDirPath}/../systick_utils.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/timer.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/timer.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../board_init.c"
"${ProjDirPath}/../board_init.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/adt
    ${ProjDirPath}/../../../../../middleware/eiq/common/model
    ${ProjDirPath}/../../../../../middleware/eiq/common
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../middleware/issdk
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/cmsis_drivers
    ${ProjDirPath}/../../../../../CMSIS/Driver/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/eiq
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../devices/MIMXRT1062
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/xip
    ${ProjDirPath}/../../../../evkmimxrt1060/xip
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities
    ${ProjDirPath}/../../../../../CMSIS/DSP
)

# include modules
include(middleware_issdk_drivers_gpio_imxrt_MIMXRT1062)

include(middleware_issdk_sensor_interface_common_MIMXRT1062)

include(middleware_issdk_sensor_fxas21002_MIMXRT1062)

include(middleware_issdk_sensor_fxos8700_MIMXRT1062)

include(driver_edma_MIMXRT1062)

include(driver_dmamux_MIMXRT1062)

include(driver_lpi2c_edma_MIMXRT1062)

include(driver_cmsis_lpi2c_MIMXRT1062)

include(driver_lpspi_MIMXRT1062)

include(driver_lpspi_edma_MIMXRT1062)

include(driver_cmsis_lpspi_MIMXRT1062)

include(driver_lpuart_MIMXRT1062)

include(driver_lpuart_edma_MIMXRT1062)

include(driver_cmsis_lpuart_MIMXRT1062)

include(CMSIS_Driver_Include_Common_MIMXRT1062)

include(CMSIS_Driver_Include_SPI_MIMXRT1062)

include(CMSIS_Driver_Include_USART_MIMXRT1062)

include(CMSIS_Driver_Include_I2C_MIMXRT1062)

include(utility_debug_console_lite_MIMXRT1062)

include(utility_assert_lite_MIMXRT1062)

include(middleware_eiq_worker_sensor_MIMXRT1062)

include(middleware_eiq_tensorflow_lite_micro_MIMXRT1062)

include(component_lists_MIMXRT1062)

include(component_lpuart_adapter_MIMXRT1062)

include(driver_common_MIMXRT1062)

include(device_MIMXRT1062_CMSIS_MIMXRT1062)

include(device_MIMXRT1062_startup_MIMXRT1062)

include(driver_clock_MIMXRT1062)

include(driver_igpio_MIMXRT1062)

include(driver_iomuxc_MIMXRT1062)

include(driver_lpi2c_MIMXRT1062)

include(driver_xip_device_MIMXRT1062)

include(driver_xip_board_evkmimxrt1060_MIMXRT1062)

include(middleware_eiq_tensorflow_lite_micro_cmsis_nn_MIMXRT1062)

include(CMSIS_Include_core_cm_MIMXRT1062)

include(middleware_eiq_tensorflow_lite_micro_third_party_flatbuffers_MIMXRT1062)

include(middleware_eiq_tensorflow_lite_micro_third_party_gemmlowp_MIMXRT1062)

include(middleware_eiq_tensorflow_lite_micro_third_party_ruy_MIMXRT1062)

include(utilities_misc_utilities_MIMXRT1062)

include(device_MIMXRT1062_system_MIMXRT1062)

include(middleware_eiq_tensorflow_lite_micro_third_party_cmsis_nn_MIMXRT1062)

include(CMSIS_DSP_Library_MIMXRT1062)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfdp_math.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfdp_math.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


