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


project(h264_freertos_cm7)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME h264_freertos_cm7.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../h264_freertos.c"
"${ProjDirPath}/../ffconf.h"
"${ProjDirPath}/../FreeRTOS_POSIX_portable.h"
"${ProjDirPath}/../display.c"
"${ProjDirPath}/../display.h"
"${ProjDirPath}/../h264_dec.h"
"${ProjDirPath}/../h264_dec.cpp"
"${ProjDirPath}/../sdcard.c"
"${ProjDirPath}/../sdcard.h"
"${ProjDirPath}/../display_support.h"
"${ProjDirPath}/../display_support.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../evkmimxrt1170_connect_cm4_cm7side.jlinkscript"
"${ProjDirPath}/../freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/errno.h"
"${ProjDirPath}/../freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/fcntl.h"
"${ProjDirPath}/../freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/mqueue.h"
"${ProjDirPath}/../freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/pthread.h"
"${ProjDirPath}/../freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/sched.h"
"${ProjDirPath}/../freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/semaphore.h"
"${ProjDirPath}/../freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/signal.h"
"${ProjDirPath}/../freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/sys/types.h"
"${ProjDirPath}/../freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/time.h"
"${ProjDirPath}/../freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/unistd.h"
"${ProjDirPath}/../freertos_libraries/abstractions/posix/include/FreeRTOS_POSIX/utils.h"
"${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/include/FreeRTOS_POSIX.h"
"${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/include/FreeRTOS_POSIX_internal.h"
"${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/include/FreeRTOS_POSIX_portable_default.h"
"${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/include/FreeRTOS_POSIX_types.h"
"${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_clock.c"
"${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_mqueue.c"
"${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_pthread.c"
"${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_pthread_barrier.c"
"${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_pthread_cond.c"
"${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_pthread_mutex.c"
"${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_sched.c"
"${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_semaphore.c"
"${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_timer.c"
"${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_unistd.c"
"${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/source/FreeRTOS_POSIX_utils.c"
"${ProjDirPath}/../freertos_libraries/c_sdk/standard/common/include/private/iot_doubly_linked_list.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${ProjDirPath}/../freertos_libraries/abstractions/posix/include
    ${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/include
    ${ProjDirPath}/../freertos_libraries/c_sdk/standard/common/include/private
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/components/video
    ${SdkRootDirPath}/components/video/display
    ${SdkRootDirPath}/components/video/display/rm68191
    ${SdkRootDirPath}/devices/MIMXRT1176/drivers
    ${SdkRootDirPath}/components/video/display/mipi_dsi_cmd
    ${SdkRootDirPath}/components/video/display/rm68200
    ${SdkRootDirPath}/components/video/display/dc/lcdifv2
    ${SdkRootDirPath}/components/gt911
    ${SdkRootDirPath}/components/video/display/dc/elcdif
    ${SdkRootDirPath}/middleware/sdmmc
    ${SdkRootDirPath}/devices/MIMXRT1176/utilities
    ${SdkRootDirPath}/devices/MIMXRT1176/utilities/debug_console_lite
    ${SdkRootDirPath}/components/video/display/hx8394
    ${SdkRootDirPath}/components/gpio
    ${SdkRootDirPath}/middleware/fatfs
    ${SdkRootDirPath}/components/video/display/fbdev
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/middleware/openh264
    ${SdkRootDirPath}/devices/MIMXRT1176
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1176/xip
    ${SdkRootDirPath}/boards/evkmimxrt1170/xip
    ${SdkRootDirPath}/devices/MIMXRT1176/drivers/cm7
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/components/video/display/dc
    ${SdkRootDirPath}/components/osa
)

# include modules
include(driver_video-common_MIMXRT1176_cm7)

include(driver_display-common_MIMXRT1176_cm7)

include(driver_display-rm68191_MIMXRT1176_cm7)

include(driver_mipi_dsi_split_MIMXRT1176_cm7)

include(driver_display-mipi-dsi-cmd_MIMXRT1176_cm7)

include(driver_common_MIMXRT1176_cm7)

include(driver_display-rm68200_MIMXRT1176_cm7)

include(driver_dc-fb-lcdifv2_MIMXRT1176_cm7)

include(driver_gt911_MIMXRT1176_cm7)

include(driver_lpi2c_MIMXRT1176_cm7)

include(driver_soc_src_MIMXRT1176_cm7)

include(driver_pxp_MIMXRT1176_cm7)

include(driver_elcdif_MIMXRT1176_cm7)

include(driver_dc-fb-elcdif_MIMXRT1176_cm7)

include(driver_usdhc_MIMXRT1176_cm7)

include(middleware_sdmmc_sd_MIMXRT1176_cm7)

include(middleware_sdmmc_common_MIMXRT1176_cm7)

include(middleware_sdmmc_host_usdhc_MIMXRT1176_cm7)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT1176_cm7)

include(utility_debug_console_lite_MIMXRT1176_cm7)

include(utility_assert_lite_MIMXRT1176_cm7)

include(driver_display-hx8394_MIMXRT1176_cm7)

include(component_igpio_adapter_MIMXRT1176_cm7)

include(middleware_fatfs_MIMXRT1176_cm7)

include(middleware_fatfs_sd_MIMXRT1176_cm7)

include(driver_fbdev_MIMXRT1176_cm7)

include(middleware_freertos-kernel_heap_4_MIMXRT1176_cm7)

include(middleware_openh264_decoder_MIMXRT1176_cm7)

include(driver_clock_MIMXRT1176_cm7)

include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm7)

include(component_lpuart_adapter_MIMXRT1176_cm7)

include(component_lists_MIMXRT1176_cm7)

include(driver_lpuart_MIMXRT1176_cm7)

include(device_MIMXRT1176_startup_MIMXRT1176_cm7)

include(driver_iomuxc_MIMXRT1176_cm7)

include(driver_igpio_MIMXRT1176_cm7)

include(driver_xip_device_MIMXRT1176_cm7)

include(driver_xip_board_evkmimxrt1170_MIMXRT1176_cm7)

include(driver_pmu_1_MIMXRT1176_cm7)

include(driver_dcdc_soc_MIMXRT1176_cm7)

include(driver_cache_armv7_m7_MIMXRT1176_cm7)

include(driver_anatop_ai_MIMXRT1176_cm7)

include(CMSIS_Include_core_cm_MIMXRT1176_cm7)

include(driver_soc_mipi_csi2rx_MIMXRT1176_cm7)

include(driver_dc-fb-common_MIMXRT1176_cm7)

include(driver_lcdifv2_MIMXRT1176_cm7)

include(driver_memory_MIMXRT1176_cm7)

include(middleware_sdmmc_osa_freertos_MIMXRT1176_cm7)

include(component_osa_free_rtos_MIMXRT1176_cm7)

include(middleware_freertos-kernel_MIMXRT1176_cm7)

include(middleware_freertos-kernel_extension_MIMXRT1176_cm7)

include(middleware_openh264_api_MIMXRT1176_cm7)

include(middleware_openh264_common_MIMXRT1176_cm7)

include(utilities_misc_utilities_MIMXRT1176_cm7)

include(device_MIMXRT1176_system_MIMXRT1176_cm7)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


