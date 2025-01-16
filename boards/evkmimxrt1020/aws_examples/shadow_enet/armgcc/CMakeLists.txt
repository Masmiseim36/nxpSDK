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


project(aws_shadow_enet)

enable_language(ASM)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_debug flexspi_nor_sdram_release)

set(MCUX_SDK_PROJECT_NAME aws_shadow_enet.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../app.h"
"${ProjDirPath}/../hardware_init.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../aws_mbedtls_config.h"
"${ProjDirPath}/../aws_clientcredential.h"
"${ProjDirPath}/../aws_clientcredential_keys.h"
"${ProjDirPath}/../core_mqtt_config.h"
"${ProjDirPath}/../core_pkcs11_config.h"
"${ProjDirPath}/../demo_config.h"
"${ProjDirPath}/../freertos_tasks_c_additions.h"
"${ProjDirPath}/../kvstore_config.h"
"${ProjDirPath}/../shadow_config.h"
"${ProjDirPath}/../aws_dev_mode_key_provisioning.h"
"${ProjDirPath}/../aws_dev_mode_key_provisioning.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../iot_pkcs11_pal.c"
"${ProjDirPath}/../shadow_device_task.c"
"${ProjDirPath}/../shadow_update_task.c"
"${ProjDirPath}/../main_enet.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../FreeRTOSConfig_Gen.h"
"${ProjDirPath}/../mcux_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set_source_files_properties("${ProjDirPath}/../FreeRTOSConfig_Gen.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_config")

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE $<$<COMPILE_LANGUAGE:C>:MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h">)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE $<$<COMPILE_LANGUAGE:C>:MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h">)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE $<$<COMPILE_LANGUAGE:CXX>:MBEDTLS_CONFIG_FILE="ksdk_mbedtls_config.h">)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE $<$<COMPILE_LANGUAGE:CXX>:MBEDTLS_CONFIG_FILE="ksdk_mbedtls_config.h">)
endif()

include(${SdkRootDirPath}/devices/MIMXRT1021/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/aws_shadow_enet.bin)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map;${EXECUTABLE_OUTPUT_PATH}/aws_shadow_enet.bin")

# wrap all libraries with -Wl,--start-group -Wl,--end-group to prevent link order issue
group_link_libraries()

