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


project(se05x_cloud_aws)

enable_language(ASM)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME se05x_cloud_aws.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../readme.rst"
"${ProjDirPath}/../FreeRTOSConfigBoard.h"
"${ProjDirPath}/../fsl_sss_ftr.h"
"${ProjDirPath}/../se_board_config.c"
"${ProjDirPath}/../se_board_config.h"
"${ProjDirPath}/../aws_secure_sockets_config.h"
"${ProjDirPath}/../aws_mqtt_config.h"
"${ProjDirPath}/../aws_bufferpool_config.h"
"${ProjDirPath}/../iot_mqtt_agent_config.h"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../aws_demo.h"
"${ProjDirPath}/../aws_iot_config.h"
"${ProjDirPath}/../aws_client_credential_keys.h"
"${ProjDirPath}/../aws_jitr_demo_Main_lwip.c"
"${ProjDirPath}/../aws_jitr_task_lwip.c"
"${ProjDirPath}/../common_cloud_config.h"
"${ProjDirPath}/../core_mqtt_config.h"
"${ProjDirPath}/../core_pkcs11_config.h"
"${ProjDirPath}/../FreeRTOSConfig.c"
"${ProjDirPath}/../iot_default_root_certificates.h"
"${ProjDirPath}/../ledHandler.c"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../FreeRTOSIPConfig.h"
"${ProjDirPath}/../iot_config_common.h"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../FreeRTOSConfig_Gen.h"
"${ProjDirPath}/../fsl_debug_console_conf.h"
"${ProjDirPath}/../mcux_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set_source_files_properties("${ProjDirPath}/../FreeRTOSConfig.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_config")
set_source_files_properties("${ProjDirPath}/../FreeRTOSConfig_Gen.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_config")

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE $<$<COMPILE_LANGUAGE:C>:MBEDTLS_CONFIG_FILE="ksdk_mbedtls_config.h" MBEDTLS_USER_CONFIG_FILE="sss_ksdk_mbedtls_config.h">)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE $<$<COMPILE_LANGUAGE:C>:MBEDTLS_CONFIG_FILE="ksdk_mbedtls_config.h" MBEDTLS_USER_CONFIG_FILE="sss_ksdk_mbedtls_config.h">)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE $<$<COMPILE_LANGUAGE:CXX>:MBEDTLS_CONFIG_FILE="ksdk_mbedtls_config.h">)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE $<$<COMPILE_LANGUAGE:CXX>:MBEDTLS_CONFIG_FILE="ksdk_mbedtls_config.h">)
endif()

include(${SdkRootDirPath}/devices/MIMXRT1062/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/se05x_cloud_aws.bin)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map;${EXECUTABLE_OUTPUT_PATH}/se05x_cloud_aws.bin")

# wrap all libraries with -Wl,--start-group -Wl,--end-group to prevent link order issue
group_link_libraries()

