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


project(ncp_host)

enable_language(ASM)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME ncp_host.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../ncp_host_app.h"
"${ProjDirPath}/../ncp_host_app.c"
"${ProjDirPath}/../ncp_host_command.h"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../lwippools.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../ncp_host_app_system.c"
"${ProjDirPath}/../ncp_host_command_system.c"
"${ProjDirPath}/../ncp_host_app_wifi.c"
"${ProjDirPath}/../ncp_host_command_wifi.h"
"${ProjDirPath}/../ncp_host_command_wifi.c"
"${ProjDirPath}/../ncp_host_utils_wifi.h"
"${ProjDirPath}/../ble/ncp_host_app_ble.c"
"${ProjDirPath}/../ble/ncp_host_command_ble.h"
"${ProjDirPath}/../ble/ncp_host_command_ble.c"
"${ProjDirPath}/../ble/ble_service/service.h"
"${ProjDirPath}/../ble/ble_service/ht.h"
"${ProjDirPath}/../ble/ble_service/htc.c"
"${ProjDirPath}/../ble/ble_service/hts.c"
"${ProjDirPath}/../ble/ble_service/hr.h"
"${ProjDirPath}/../ble/ble_service/hrc.c"
"${ProjDirPath}/../ble/ble_service/hrs.c"
"${ProjDirPath}/../ble/ble_service/bas.h"
"${ProjDirPath}/../ble/ble_service/bas.c"
"${ProjDirPath}/../ncp_host_app_ot.c"
"${ProjDirPath}/../ncp_host_command_ot.c"
"${ProjDirPath}/../otopcode.c"
"${ProjDirPath}/../otopcode.h"
"${ProjDirPath}/../otopcode_private.h"
"${ProjDirPath}/../ncp_cmd_common.h"
"${ProjDirPath}/../ncp_intf_usb_host_cdc.c"
"${ProjDirPath}/../ncp_intf_usb_host_cdc.h"
"${ProjDirPath}/../ncp_usb_host_cdc.c"
"${ProjDirPath}/../ncp_usb_host_cdc.h"
"${ProjDirPath}/../usb_host_cdc_app.c"
"${ProjDirPath}/../usb_host_cdc_app.h"
"${ProjDirPath}/../usb_host_config.h"
"${ProjDirPath}/../ncp_intf_sdio_host.c"
"${ProjDirPath}/../ncp_intf_sdio_host.h"
"${ProjDirPath}/../ncp_intf_spi_master.c"
"${ProjDirPath}/../ncp_intf_spi_master.h"
"${ProjDirPath}/../crc.c"
"${ProjDirPath}/../crc.h"
"${ProjDirPath}/../ncp_tlv_adapter.c"
"${ProjDirPath}/../ncp_tlv_adapter.h"
"${ProjDirPath}/../ncp_adapter.h"
"${ProjDirPath}/../ncp_debug.h"
"${ProjDirPath}/../ncp_cmd_system.h"
"${ProjDirPath}/../ncp_cmd_wifi.h"
"${ProjDirPath}/../ncp_cmd_ble.h"
"${ProjDirPath}/../ncp_intf_pm.c"
"${ProjDirPath}/../ncp_intf_pm.h"
"${ProjDirPath}/../ncp_intf_uart.c"
"${ProjDirPath}/../ncp_intf_uart.h"
"${ProjDirPath}/../FreeRTOSConfigBoard.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../app_config.h"
"${ProjDirPath}/../ncp_host_config.h"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../hardware_init.c"
"${ProjDirPath}/../app.h"
"${ProjDirPath}/../FreeRTOSConfig_Gen.h"
"${ProjDirPath}/../fsl_os_abstraction_config.h"
"${ProjDirPath}/../mcux_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../ble
    ${ProjDirPath}/../ble/ble_service
)

set_source_files_properties("${ProjDirPath}/../FreeRTOSConfig.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_config")
set_source_files_properties("${ProjDirPath}/../sdmmc_config.c" PROPERTIES COMPONENT_CONFIG_FILE "middleware_sdmmc_usdhc_template")
set_source_files_properties("${ProjDirPath}/../sdmmc_config.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_sdmmc_usdhc_template")
set_source_files_properties("${ProjDirPath}/../FreeRTOSConfig_Gen.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_config")
set_source_files_properties("${ProjDirPath}/../fsl_os_abstraction_config.h" PROPERTIES COMPONENT_CONFIG_FILE "component_osa_template_config")

include(${SdkRootDirPath}/devices/MIMXRT1062/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/ncp_host.bin)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map;${EXECUTABLE_OUTPUT_PATH}/ncp_host.bin")

# wrap all libraries with -Wl,--start-group -Wl,--end-group to prevent link order issue
group_link_libraries()

