# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

# THE VERSION NUMBER
SET (MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION 2)
SET (MCUXPRESSO_CMAKE_FORMAT_MINOR_VERSION 0)

include(ide_overrides.cmake OPTIONAL)

if(CMAKE_SCRIPT_MODE_FILE)
  message("${MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION}")
  return()
endif()


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


project(h264_freertos)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_debug flexspi_nor_sdram_release)

set(MCUX_SDK_PROJECT_NAME h264_freertos.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
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

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../freertos_libraries/abstractions/posix/include
    ${ProjDirPath}/../freertos_libraries/freertos_plus/standard/freertos_plus_posix/include
    ${ProjDirPath}/../freertos_libraries/c_sdk/standard/common/include/private
)

set_source_files_properties("${ProjDirPath}/../ffconf.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_fatfs_template_sd")
set_source_files_properties("${ProjDirPath}/../sdmmc_config.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_sdmmc_usdhc_template")
set_source_files_properties("${ProjDirPath}/../sdmmc_config.c" PROPERTIES COMPONENT_CONFIG_FILE "middleware_sdmmc_usdhc_template")
set_source_files_properties("${ProjDirPath}/../FreeRTOSConfig.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_template")

include(${SdkRootDirPath}/devices/MIMXRT1062/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map")

