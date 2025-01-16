# CROSS COMPILER SETTING
set(CMAKE_SYSTEM_NAME Generic)
cmake_minimum_required(VERSION 3.10.0)

# THE VERSION NUMBER
SET (MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION 2)
SET (MCUXPRESSO_CMAKE_FORMAT_MINOR_VERSION 0)

include(ide_overrides.cmake OPTIONAL)

if(CMAKE_SCRIPT_MODE_FILE)
  message("${MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION}")
  return()
endif()


set(CMAKE_STATIC_LIBRARY_PREFIX)
set(CMAKE_STATIC_LIBRARY_SUFFIX)

set(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
set(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
set(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(wifi_cli)

enable_language(ASM)

set(MCUX_BUILD_TYPES flexspi_nor_release flexspi_nor_debug)

set(MCUX_SDK_PROJECT_NAME wifi_cli.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../specific.c"
"${ProjDirPath}/../specific.h"
"${ProjDirPath}/../host_sleep.c"
"${ProjDirPath}/../host_sleep.h"
"${ProjDirPath}/../lpm.c"
"${ProjDirPath}/../lpm.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../lwippools.h"
"${ProjDirPath}/../lwiphooks.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../wifi_config.h"
"${ProjDirPath}/../hardware_init.c"
"${ProjDirPath}/../app.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../wifi_bt_config.c"
"${ProjDirPath}/../wifi_bt_config.h"
"${ProjDirPath}/../app_config.h"
"${ProjDirPath}/../FreeRTOSConfigBoard.h"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../fsl_debug_console_conf.h"
"${ProjDirPath}/../fsl_os_abstraction_config.h"
"${ProjDirPath}/../mcux_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set_source_files_properties("${ProjDirPath}/../FreeRTOSConfig.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_config")
set_source_files_properties("${ProjDirPath}/../sdmmc_config.c" PROPERTIES COMPONENT_CONFIG_FILE "middleware_sdmmc_usdhc_template")
set_source_files_properties("${ProjDirPath}/../sdmmc_config.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_sdmmc_usdhc_template")
set_source_files_properties("${ProjDirPath}/../fsl_os_abstraction_config.h" PROPERTIES COMPONENT_CONFIG_FILE "component_osa_template_config")

include(${SdkRootDirPath}/devices/MIMXRT1062/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/wifi_cli.bin)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map;${EXECUTABLE_OUTPUT_PATH}/wifi_cli.bin")

# wrap all libraries with -Wl,--start-group -Wl,--end-group to prevent link order issue
group_link_libraries()

