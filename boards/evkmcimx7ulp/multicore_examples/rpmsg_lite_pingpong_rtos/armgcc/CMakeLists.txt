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


project(rpmsg_lite_pingpong_rtos_imxcm4)

set(MCUX_SDK_PROJECT_NAME rpmsg_lite_pingpong_rtos_imxcm4.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main_remote.c"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../rpmsg_config.h"
"${ProjDirPath}/../rsc_table.c"
"${ProjDirPath}/../rsc_table.h"
"${ProjDirPath}/../remoteproc.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../fsl_iomuxc.h"
"${ProjDirPath}/../app_srtm.c"
"${ProjDirPath}/../app_srtm.h"
"${ProjDirPath}/../srtm/channels/srtm_rpmsg_endpoint.h"
"${ProjDirPath}/../srtm/channels/srtm_rpmsg_endpoint.c"
"${ProjDirPath}/../srtm/services/srtm_lfcl_service.h"
"${ProjDirPath}/../srtm/services/srtm_lfcl_service.c"
"${ProjDirPath}/../srtm/services/srtm_pmic_service.h"
"${ProjDirPath}/../srtm/services/srtm_pmic_service.c"
"${ProjDirPath}/../srtm/services/srtm_pf1550_adapter.h"
"${ProjDirPath}/../srtm/services/srtm_pf1550_adapter.c"
"${ProjDirPath}/../srtm/services/srtm_audio_service.h"
"${ProjDirPath}/../srtm/services/srtm_audio_service.c"
"${ProjDirPath}/../srtm/services/srtm_sai_edma_adapter.h"
"${ProjDirPath}/../srtm/services/srtm_sai_edma_adapter.c"
"${ProjDirPath}/../srtm/services/srtm_i2c_codec_adapter.h"
"${ProjDirPath}/../srtm/services/srtm_i2c_codec_adapter.c"
"${ProjDirPath}/../srtm/services/srtm_io_service.h"
"${ProjDirPath}/../srtm/services/srtm_io_service.c"
"${ProjDirPath}/../srtm/services/srtm_keypad_service.h"
"${ProjDirPath}/../srtm/services/srtm_keypad_service.c"
"${ProjDirPath}/../srtm/services/srtm_rtc_service.h"
"${ProjDirPath}/../srtm/services/srtm_rtc_service.c"
"${ProjDirPath}/../srtm/services/srtm_snvs_lp_rtc_adapter.h"
"${ProjDirPath}/../srtm/services/srtm_snvs_lp_rtc_adapter.c"
"${ProjDirPath}/../srtm/services/srtm_sensor_service.h"
"${ProjDirPath}/../srtm/services/srtm_sensor_service.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/.. debug
    ${ProjDirPath}/.. debug
    ${ProjDirPath}/..
    ${ProjDirPath}/../srtm/channels
    ${ProjDirPath}/../srtm/services
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/codec/i2c
    ${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers
    ${ProjDirPath}/../../../../../components/i2c
    ${ProjDirPath}/../../../../../components/codec
    ${ProjDirPath}/../../../../../components/codec/port/wm8960
    ${ProjDirPath}/../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../devices/MCIMX7U5
    ${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../components/pf1550
    ${ProjDirPath}/../../../../../components/codec/wm8960
    ${ProjDirPath}/../../../../../components/srtm/port
    ${ProjDirPath}/../../../../../components/srtm
    ${ProjDirPath}/../../../../../middleware/multicore
    ${ProjDirPath}/../../../../../middleware/issdk
    ${ProjDirPath}/../../../../../CMSIS/Driver/Include
)

# include modules
include(component_codec_i2c_MCIMX7U5)

include(driver_common_MCIMX7U5)

include(component_lpi2c_adapter_MCIMX7U5)

include(driver_codec_MCIMX7U5)

include(component_wm8960_adapter_MCIMX7U5)

include(driver_gpio_MCIMX7U5)

include(driver_port_MCIMX7U5)

include(driver_clock_MCIMX7U5)

include(driver_mu_MCIMX7U5)

include(middleware_freertos-kernel_MCIMX7U5)

include(device_MCIMX7U5_CMSIS_MCIMX7U5)

include(utility_debug_console_MCIMX7U5)

include(component_lpuart_adapter_MCIMX7U5)

include(component_serial_manager_MCIMX7U5)

include(component_lists_MCIMX7U5)

include(component_serial_manager_uart_MCIMX7U5)

include(driver_lpuart_MCIMX7U5)

include(device_MCIMX7U5_startup_MCIMX7U5)

include(driver_msmc_MCIMX7U5)

include(utility_assert_MCIMX7U5)

include(driver_dmamux_MCIMX7U5)

include(CMSIS_Include_core_cm4_MCIMX7U5)

include(CMSIS_Include_common_MCIMX7U5)

include(CMSIS_Include_dsp_MCIMX7U5)

include(driver_edma_MCIMX7U5)

include(driver_lpi2c_MCIMX7U5)

include(driver_lpi2c_freertos_MCIMX7U5)

include(middleware_freertos-kernel_extension_MCIMX7U5)

include(driver_pmc0_MCIMX7U5)

include(driver_llwu_MCIMX7U5)

include(driver_snvs_lp_MCIMX7U5)

include(driver_pf1550_MCIMX7U5)

include(driver_sai_MCIMX7U5)

include(driver_sai_edma_MCIMX7U5)

include(driver_wm8960_MCIMX7U5)

include(driver_srtm_freertos_MCIMX7U5)

include(driver_srtm_MCIMX7U5)

include(middleware_multicore_rpmsg_lite_imx7ulp_m4_freertos_MCIMX7U5)

include(middleware_multicore_rpmsg_lite_MCIMX7U5)

include(middleware_freertos-kernel_heap_4_MCIMX7U5)

include(middleware_issdk_sensor_allregdefs_MCIMX7U5)

include(CMSIS_Driver_Include_I2C_MCIMX7U5)

include(CMSIS_Driver_Include_Common_MCIMX7U5)

include(CMSIS_Driver_Include_SPI_MCIMX7U5)

include(middleware_issdk_algorithms_pedometer_lib_cm4_MCIMX7U5)

include(middleware_issdk_algorithms_pedometer_common_MCIMX7U5)

include(utilities_misc_utilities_MCIMX7U5)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/lib/libpedometerm4.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/lib/libpedometerm4.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flash_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/lib/libpedometerm4.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flash_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/lib/libpedometerm4.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/rpmsg_lite_pingpong_rtos.bin)

