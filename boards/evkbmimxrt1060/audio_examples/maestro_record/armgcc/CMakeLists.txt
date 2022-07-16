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


project(maestro_record)

set(MCUX_SDK_PROJECT_NAME maestro_record.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../opusmem2mem_file.h"
"${ProjDirPath}/../app_streamer.c"
"${ProjDirPath}/../app_streamer.h"
"${ProjDirPath}/../cmd.c"
"${ProjDirPath}/../cmd.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../main.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../ffconf.h"
"${ProjDirPath}/../streamer_pcm.c"
"${ProjDirPath}/../streamer_pcm_app.h"
"${ProjDirPath}/../startup_armgcc.c"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../app_definitions.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../../..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers
    ${ProjDirPath}/../../../../../components/codec/wm8960
    ${ProjDirPath}/../../../../../components/codec
    ${ProjDirPath}/../../../../../components/codec/i2c
    ${ProjDirPath}/../../../../../components/i2c
    ${ProjDirPath}/../../../../../middleware/sdmmc
    ${ProjDirPath}/../../../../../middleware/vit
    ${ProjDirPath}/../../../../../CMSIS/DSP
    ${ProjDirPath}/../../../../../components/gpio
    ${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel
    ${ProjDirPath}/../../../../../middleware/maestro
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities
    ${ProjDirPath}/../../../../../middleware/fatfs
    ${ProjDirPath}/../../../../../middleware/maestro/mcu-audio/opus
    ${ProjDirPath}/../../../../../devices/MIMXRT1062
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../components/silicon_id
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../components/codec/cs42448
    ${ProjDirPath}/../../../../../components/osa
    ${ProjDirPath}/../../../../../middleware/maestro/streamer
)

# include modules
include(driver_lpi2c_MIMXRT1062)

include(driver_sai_MIMXRT1062)

include(driver_wm8960_MIMXRT1062)

include(driver_codec_MIMXRT1062)

include(driver_common_MIMXRT1062)

include(component_codec_adapters_MIMXRT1062)

include(component_wm8960_adapter_MIMXRT1062)

include(component_cs42448_adapter_MIMXRT1062)

include(component_codec_i2c_MIMXRT1062)

include(component_lpi2c_adapter_MIMXRT1062)

include(driver_sai_edma_MIMXRT1062)

include(driver_dmamux_MIMXRT1062)

include(driver_edma_MIMXRT1062)

include(middleware_sdmmc_sd_MIMXRT1062)

include(middleware_sdmmc_common_MIMXRT1062)

include(middleware_sdmmc_host_usdhc_MIMXRT1062)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT1062)

include(driver_cache_armv7_m7_MIMXRT1062)

include(middleware_vit_cm7_RT106x_MIMXRT1062)

include(CMSIS_DSP_Source_MIMXRT1062)

include(component_igpio_adapter_MIMXRT1062)

include(middleware_freertos-kernel_heap_4_MIMXRT1062)

include(middleware_maestro_framework_MIMXRT1062)

include(utility_shell_MIMXRT1062)

include(middleware_fatfs_MIMXRT1062)

include(middleware_fatfs_sd_MIMXRT1062)

include(middleware_maestro_framework_opus_MIMXRT1062)

include(driver_clock_MIMXRT1062)

include(device_MIMXRT1062_CMSIS_MIMXRT1062)

include(utility_debug_console_MIMXRT1062)

include(component_lpuart_adapter_MIMXRT1062)

include(component_serial_manager_MIMXRT1062)

include(component_lists_MIMXRT1062)

include(component_serial_manager_uart_MIMXRT1062)

include(driver_lpuart_MIMXRT1062)

include(device_MIMXRT1062_startup_MIMXRT1062)

include(driver_iomuxc_MIMXRT1062)

include(utility_assert_MIMXRT1062)

include(driver_igpio_MIMXRT1062)

include(driver_xip_device_MIMXRT1062)

include(driver_xip_board_evkbmimxrt1060_MIMXRT1062)

include(component_silicon_id_MIMXRT1062)

include(CMSIS_Include_core_cm_MIMXRT1062)

include(driver_cs42448_MIMXRT1062)

include(middleware_sdmmc_osa_freertos_MIMXRT1062)

include(component_osa_free_rtos_MIMXRT1062)

include(middleware_freertos-kernel_MIMXRT1062)

include(middleware_freertos-kernel_extension_MIMXRT1062)

include(driver_usdhc_MIMXRT1062)

include(middleware_maestro_framework_doc_MIMXRT1062)

include(middleware_maestro_framework_streamer_MIMXRT1062)

include(utilities_misc_utilities_MIMXRT1062)

include(device_MIMXRT1062_system_MIMXRT1062)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/maestro/libs/cm7f/armgcc/release/libmp3.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/maestro/libs/cm7f/armgcc/release/libwav.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/maestro/libs/cm7f/armgcc/release/libmp3.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/maestro/libs/cm7f/armgcc/release/libwav.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


