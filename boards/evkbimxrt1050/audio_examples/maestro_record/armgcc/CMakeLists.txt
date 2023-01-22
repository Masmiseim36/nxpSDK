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

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME maestro_record.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

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

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkbimxrt1050
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT1052/drivers
    ${SdkRootDirPath}/components/codec/wm8960
    ${SdkRootDirPath}/components/codec
    ${SdkRootDirPath}/components/codec/
    ${SdkRootDirPath}/components/codec/i2c
    ${SdkRootDirPath}/components/i2c
    ${SdkRootDirPath}/middleware/sdmmc
    ${SdkRootDirPath}/middleware/vit
    ${SdkRootDirPath}/CMSIS/DSP
    ${SdkRootDirPath}/components/gpio
    ${SdkRootDirPath}/middleware/fatfs
    ${SdkRootDirPath}/middleware/maestro/mcu-audio/opus
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/middleware/maestro
    ${SdkRootDirPath}/devices/MIMXRT1052/utilities
    ${SdkRootDirPath}/devices/MIMXRT1052
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1052/xip
    ${SdkRootDirPath}/boards/evkbimxrt1050/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/components/osa
    ${SdkRootDirPath}/middleware/maestro/streamer
)

# include modules
include(driver_lpi2c_MIMXRT1052)

include(driver_sai_MIMXRT1052)

include(driver_wm8960_MIMXRT1052)

include(driver_codec_MIMXRT1052)

include(driver_common_MIMXRT1052)

include(component_wm8960_adapter_MIMXRT1052)

include(component_codec_i2c_MIMXRT1052)

include(component_lpi2c_adapter_MIMXRT1052)

include(driver_sai_edma_MIMXRT1052)

include(driver_dmamux_MIMXRT1052)

include(driver_edma_MIMXRT1052)

include(middleware_sdmmc_sd_MIMXRT1052)

include(middleware_sdmmc_common_MIMXRT1052)

include(middleware_sdmmc_host_usdhc_MIMXRT1052)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT1052)

include(driver_cache_armv7_m7_MIMXRT1052)

include(middleware_vit_cm7_MIMXRT1052)

include(CMSIS_DSP_Source_MIMXRT1052)

include(driver_flexram_MIMXRT1052)

include(component_igpio_adapter_MIMXRT1052)

include(middleware_fatfs_MIMXRT1052)

include(middleware_fatfs_sd_MIMXRT1052)

include(middleware_maestro_framework_opus_MIMXRT1052)

include(middleware_freertos-kernel_heap_4_MIMXRT1052)

include(middleware_maestro_framework_MIMXRT1052)

include(utility_shell_MIMXRT1052)

include(driver_clock_MIMXRT1052)

include(device_MIMXRT1052_CMSIS_MIMXRT1052)

include(utility_debug_console_MIMXRT1052)

include(component_lpuart_adapter_MIMXRT1052)

include(component_serial_manager_MIMXRT1052)

include(component_lists_MIMXRT1052)

include(component_serial_manager_uart_MIMXRT1052)

include(driver_lpuart_MIMXRT1052)

include(device_MIMXRT1052_startup_MIMXRT1052)

include(driver_iomuxc_MIMXRT1052)

include(utility_assert_MIMXRT1052)

include(driver_igpio_MIMXRT1052)

include(driver_xip_device_MIMXRT1052)

include(driver_xip_board_evkbimxrt1050_MIMXRT1052)

include(CMSIS_Include_core_cm_MIMXRT1052)

include(middleware_sdmmc_osa_freertos_MIMXRT1052)

include(component_osa_free_rtos_MIMXRT1052)

include(middleware_freertos-kernel_MIMXRT1052)

include(middleware_freertos-kernel_extension_MIMXRT1052)

include(driver_usdhc_MIMXRT1052)

include(driver_soc_flexram_allocate_MIMXRT1052)

include(middleware_maestro_framework_doc_MIMXRT1052)

include(middleware_maestro_framework_streamer_MIMXRT1052)

include(utilities_misc_utilities_MIMXRT1052)

include(device_MIMXRT1052_system_MIMXRT1052)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/maestro/libs/cm7f/armgcc/release/libmp3.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/maestro/libs/cm7f/armgcc/release/libwav.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/maestro/libs/cm7f/armgcc/release/libaac.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/maestro/libs/cm7f/armgcc/release/libflac.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/maestro/libs/cm7f/armgcc/release/libmp3.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/maestro/libs/cm7f/armgcc/release/libwav.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/maestro/libs/cm7f/armgcc/release/libaac.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/maestro/libs/cm7f/armgcc/release/libflac.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


