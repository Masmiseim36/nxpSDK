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


project(audio_profile_cm7)

enable_language(ASM)

set(MCUX_BUILD_TYPES flexspi_nor_release flexspi_nor_debug)

set(MCUX_SDK_PROJECT_NAME audio_profile_cm7.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../edgefast_bluetooth_app.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../gradle.xml"
"${ProjDirPath}/../misc.xml"
"${ProjDirPath}/../modules.xml"
"${ProjDirPath}/../runConfigurations.xml"
"${ProjDirPath}/../AwsMusicControl.apk"
"${ProjDirPath}/../AwsMusicControlPreferences.properties"
"${ProjDirPath}/../build.gradle"
"${ProjDirPath}/../proguard-rules.pro"
"${ProjDirPath}/../AndroidManifest.xml"
"${ProjDirPath}/../ic_launcher-web.png"
"${ProjDirPath}/../ic_launcher_round-web.png"
"${ProjDirPath}/../AboutActivity.java"
"${ProjDirPath}/../MainActivity.java"
"${ProjDirPath}/../AwsConstants.java"
"${ProjDirPath}/../AwsShadow.java"
"${ProjDirPath}/../IPreferencesListener.java"
"${ProjDirPath}/../RgbLed.java"
"${ProjDirPath}/../Connection.java"
"${ProjDirPath}/../ic_action_refresh.png"
"${ProjDirPath}/../ic_led.png"
"${ProjDirPath}/../ic_led_filled.png"
"${ProjDirPath}/../nxp.png"
"${ProjDirPath}/../activity_about.xml"
"${ProjDirPath}/../activity_main.xml"
"${ProjDirPath}/../content_about.xml"
"${ProjDirPath}/../content_main.xml"
"${ProjDirPath}/../music_info.xml"
"${ProjDirPath}/../main.xml"
"${ProjDirPath}/../ic_launcher.png"
"${ProjDirPath}/../ic_launcher_round.png"
"${ProjDirPath}/../colors.xml"
"${ProjDirPath}/../dimens.xml"
"${ProjDirPath}/../drawables.xml"
"${ProjDirPath}/../strings.xml"
"${ProjDirPath}/../styles.xml"
"${ProjDirPath}/../gradle.properties"
"${ProjDirPath}/../gradle-wrapper.jar"
"${ProjDirPath}/../gradle-wrapper.properties"
"${ProjDirPath}/../gradlew"
"${ProjDirPath}/../gradlew.bat"
"${ProjDirPath}/../settings.gradle"
"${ProjDirPath}/../app_a2dp.c"
"${ProjDirPath}/../app_a2dp.h"
"${ProjDirPath}/../app_common.c"
"${ProjDirPath}/../app_common.h"
"${ProjDirPath}/../app_music_control.c"
"${ProjDirPath}/../app_music_control.h"
"${ProjDirPath}/../app_streamer.c"
"${ProjDirPath}/../app_streamer.h"
"${ProjDirPath}/../cmd.c"
"${ProjDirPath}/../cmd.h"
"${ProjDirPath}/../ffconf.h"
"${ProjDirPath}/../aws_clientcredential.h"
"${ProjDirPath}/../aws_clientcredential_keys.h"
"${ProjDirPath}/../core_mqtt_config.h"
"${ProjDirPath}/../core_pkcs11_config.h"
"${ProjDirPath}/../demo_config.h"
"${ProjDirPath}/../kvstore_config.h"
"${ProjDirPath}/../shadow_config.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../shadow_device_task.c"
"${ProjDirPath}/../shadow_update_task.c"
"${ProjDirPath}/../streamer_pcm.c"
"${ProjDirPath}/../streamer_pcm.h"
"${ProjDirPath}/../usb_host_fatfs.c"
"${ProjDirPath}/../usb_host_fatfs.h"
"${ProjDirPath}/../wifi_bt_module_config.h"
"${ProjDirPath}/../FreeRTOSConfigBoard.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../wifi_bt_config.c"
"${ProjDirPath}/../wifi_bt_config.h"
"${ProjDirPath}/../aws_mbedtls_config.h"
"${ProjDirPath}/../wifi_config.h"
"${ProjDirPath}/../aws_dev_mode_key_provisioning.h"
"${ProjDirPath}/../aws_dev_mode_key_provisioning.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../iot_pkcs11_pal.c"
"${ProjDirPath}/../hardware_init.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../app_config.h"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../FreeRTOSConfig_Gen.h"
"${ProjDirPath}/../edgefast_bluetooth_audio_config_Gen.h"
"${ProjDirPath}/../edgefast_bluetooth_config_Gen.h"
"${ProjDirPath}/../edgefast_bluetooth_debug_config_Gen.h"
"${ProjDirPath}/../edgefast_bluetooth_extension_config_Gen.h"
"${ProjDirPath}/../fsl_debug_console_conf.h"
"${ProjDirPath}/../fsl_os_abstraction_config.h"
"${ProjDirPath}/../mcux_config.h"
"${ProjDirPath}/../usb_host_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set_source_files_properties("${ProjDirPath}/../FreeRTOSConfig.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_config")
set_source_files_properties("${ProjDirPath}/../sdmmc_config.c" PROPERTIES COMPONENT_CONFIG_FILE "middleware_sdmmc_usdhc_template")
set_source_files_properties("${ProjDirPath}/../sdmmc_config.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_sdmmc_usdhc_template")
set_source_files_properties("${ProjDirPath}/../wifi_bt_config.c" PROPERTIES COMPONENT_CONFIG_FILE "middleware_wifi_wifi_bt_config_template")
set_source_files_properties("${ProjDirPath}/../wifi_bt_config.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_wifi_wifi_bt_config_template")
set_source_files_properties("${ProjDirPath}/../FreeRTOSConfig_Gen.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_config")
set_source_files_properties("${ProjDirPath}/../fsl_os_abstraction_config.h" PROPERTIES COMPONENT_CONFIG_FILE "component_osa_template_config")

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE $<$<COMPILE_LANGUAGE:C>:MBEDTLS_USER_CONFIG_FILE="aws_mbedtls_config.h" MBEDTLS_CONFIG_FILE="ksdk_mbedtls_config.h">)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE $<$<COMPILE_LANGUAGE:C>:MBEDTLS_USER_CONFIG_FILE="aws_mbedtls_config.h" MBEDTLS_CONFIG_FILE="ksdk_mbedtls_config.h">)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE $<$<COMPILE_LANGUAGE:CXX>:MBEDTLS_CONFIG_FILE="ksdk_mbedtls_config.h">)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE $<$<COMPILE_LANGUAGE:CXX>:MBEDTLS_CONFIG_FILE="ksdk_mbedtls_config.h">)
endif()

include(${SdkRootDirPath}/devices/MIMXRT1176/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/audio_profile.bin)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map;${EXECUTABLE_OUTPUT_PATH}/audio_profile.bin")

# wrap all libraries with -Wl,--start-group -Wl,--end-group to prevent link order issue
group_link_libraries()

