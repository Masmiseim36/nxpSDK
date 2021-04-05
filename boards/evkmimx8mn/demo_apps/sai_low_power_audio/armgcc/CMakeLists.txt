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


project(sai_low_power_audio)

set(MCUX_SDK_PROJECT_NAME sai_low_power_audio.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../rpmsg_config.h"
"${ProjDirPath}/../lpm.c"
"${ProjDirPath}/../lpm.h"
"${ProjDirPath}/../sai_low_power_audio.c"
"${ProjDirPath}/../sai_low_power_audio.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
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
"${ProjDirPath}/../../../../../components/srtm/channels/srtm_rpmsg_endpoint.h"
"${ProjDirPath}/../../../../../components/srtm/channels/srtm_rpmsg_endpoint.c"
"${ProjDirPath}/../../../../../components/srtm/services/srtm_audio_service.h"
"${ProjDirPath}/../../../../../components/srtm/services/srtm_audio_service.c"
"${ProjDirPath}/../../../../../components/srtm/services/srtm_sai_sdma_adapter.h"
"${ProjDirPath}/../../../../../components/srtm/services/srtm_sai_sdma_adapter.c"
"${ProjDirPath}/../../../../../components/srtm/services/srtm_wm8524_adapter.h"
"${ProjDirPath}/../../../../../components/srtm/services/srtm_wm8524_adapter.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/.. debug
    ${ProjDirPath}/../../.. debug
    ${ProjDirPath}/../../..
    ${ProjDirPath}/..
    ${ProjDirPath}/../../../../../components/srtm/channels
    ${ProjDirPath}/../../../../../components/srtm/services
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMX8MN6/drivers
    ${ProjDirPath}/../../../../../components/codec
    ${ProjDirPath}/../../../../../components/codec/port
    ${ProjDirPath}/../../../../../devices/MIMX8MN6
    ${ProjDirPath}/../../../../../devices/MIMX8MN6/utilities
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../components/codec/wm8524
    ${ProjDirPath}/../../../../../components/srtm/port
    ${ProjDirPath}/../../../../../components/srtm
    ${ProjDirPath}/../../../../../middleware/multicore
    ${ProjDirPath}/../../../../../rtos/freertos
)

# include modules
include(driver_igpio_MIMX8MN6)

include(driver_iuart_MIMX8MN6)

include(driver_gpc_2_MIMX8MN6)

include(driver_gpt_MIMX8MN6)

include(driver_codec_MIMX8MN6)

include(driver_common_MIMX8MN6)

include(component_codec_wm8524_adapter_MIMX8MN6)

include(driver_clock_MIMX8MN6)

include(driver_sai_MIMX8MN6)

include(driver_sai_sdma_MIMX8MN6)

include(driver_rdc_MIMX8MN6)

include(device_MIMX8MN6_CMSIS_MIMX8MN6)

include(utility_debug_console_MIMX8MN6)

include(component_iuart_adapter_MIMX8MN6)

include(component_serial_manager_MIMX8MN6)

include(component_lists_MIMX8MN6)

include(component_serial_manager_uart_MIMX8MN6)

include(device_MIMX8MN6_startup_MIMX8MN6)

include(utility_assert_MIMX8MN6)

include(CMSIS_Include_core_cm7_MIMX8MN6)

include(CMSIS_Include_common_MIMX8MN6)

include(CMSIS_Include_dsp_MIMX8MN6)

include(driver_sdma_MIMX8MN6)

include(driver_memory_MIMX8MN6)

include(driver_mu_MIMX8MN6)

include(driver_wm8524_MIMX8MN6)

include(driver_srtm_freertos_MIMX8MN6)

include(driver_srtm_MIMX8MN6)

include(middleware_multicore_rpmsg_lite_imx8mn_m7_freertos_MIMX8MN6)

include(middleware_multicore_rpmsg_lite_MIMX8MN6)

include(middleware_freertos-kernel_MIMX8MN6)

include(middleware_freertos-kernel_extension_MIMX8MN6)

include(middleware_freertos-kernel_heap_4_MIMX8MN6)

include(utilities_misc_utilities_MIMX8MN6)

include(device_MIMX8MN6_system_MIMX8MN6)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sai_low_power_audio.bin)

