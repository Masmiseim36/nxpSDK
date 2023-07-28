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


project(emwin_temperature_control)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_debug flexspi_nor_sdram_release)

set(MCUX_SDK_PROJECT_NAME emwin_temperature_control.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../emwin_temperature_control.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../emwin_support.c"
"${ProjDirPath}/../emwin_support.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../AppWizard/APPW_X_NoFS.c"
"${ProjDirPath}/../AppWizard/Resource/Font/NettoOT_24_Normal_EXT_AA4.c"
"${ProjDirPath}/../AppWizard/Resource/Font/Roboto_16_Normal_EXT_AA4.c"
"${ProjDirPath}/../AppWizard/Resource/Font/Roboto_24_Normal_EXT_AA4.c"
"${ProjDirPath}/../AppWizard/Resource/Image/ButtonRoundPressed_48x21.c"
"${ProjDirPath}/../AppWizard/Resource/Image/ButtonRound_48x21.c"
"${ProjDirPath}/../AppWizard/Resource/Image/GreenGlowMarker_16x9.c"
"${ProjDirPath}/../AppWizard/Resource/Image/KNOB_106x106.c"
"${ProjDirPath}/../AppWizard/Resource/Image/SeggerLogo_80x40.c"
"${ProjDirPath}/../AppWizard/Resource/Image/TemperatureControl_3_480x272.c"
"${ProjDirPath}/../AppWizard/Resource/Text/APPW_Language_0.c"
"${ProjDirPath}/../AppWizard/Resource/Text/APPW_Pattern.c"
"${ProjDirPath}/../AppWizard/Source/APPW_MainTask.c"
"${ProjDirPath}/../AppWizard/Source/CustomCode/Application.h"
"${ProjDirPath}/../AppWizard/Source/CustomCode/ID_SCREEN_TEMP_CTRL_Slots.c"
"${ProjDirPath}/../AppWizard/Source/Generated/APPWConf.c"
"${ProjDirPath}/../AppWizard/Source/Generated/ID_SCREEN_TEMP_CTRL.c"
"${ProjDirPath}/../AppWizard/Source/Generated/ID_SCREEN_TEMP_CTRL.h"
"${ProjDirPath}/../AppWizard/Source/Generated/Resource.c"
"${ProjDirPath}/../AppWizard/Source/Generated/Resource.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkbmimxrt1060/emwin_examples/common
)

set_source_files_properties("${ProjDirPath}/../emwin_support.c" PROPERTIES COMPONENT_CONFIG_FILE "middleware_emwin_template")
set_source_files_properties("${ProjDirPath}/../emwin_support.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_emwin_template")

include(${SdkRootDirPath}/devices/MIMXRT1062/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map")

