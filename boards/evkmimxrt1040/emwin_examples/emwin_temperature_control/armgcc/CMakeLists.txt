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


project(emwin_temperature_control)

enable_language(ASM)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_debug flexspi_nor_sdram_release)

set(MCUX_SDK_PROJECT_NAME emwin_temperature_control.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../emwin_temperature_control.c"
"${ProjDirPath}/../emwin_support.c"
"${ProjDirPath}/../emwin_support.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../hardware_init.c"
"${ProjDirPath}/../app.h"
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
"${ProjDirPath}/../APPW_MainTask.c"
"${ProjDirPath}/../AppWizard/Source/CustomCode/ID_SCREEN_TEMP_CTRL_Slots.c"
"${ProjDirPath}/../AppWizard/Source/CustomCode/Application.h"
"${ProjDirPath}/../AppWizard/Source/Generated/APPWConf.c"
"${ProjDirPath}/../AppWizard/Source/Generated/ID_SCREEN_TEMP_CTRL.c"
"${ProjDirPath}/../AppWizard/Source/Generated/Resource.c"
"${ProjDirPath}/../AppWizard/Source/Generated/ID_SCREEN_TEMP_CTRL.h"
"${ProjDirPath}/../AppWizard/Source/Generated/Resource.h"
"${ProjDirPath}/../APPW_X_NoFS.c"
"${ProjDirPath}/../mcux_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../AppWizard/Source
)


include(${SdkRootDirPath}/devices/MIMXRT1042/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/emwin_temperature_control.bin)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map;${EXECUTABLE_OUTPUT_PATH}/emwin_temperature_control.bin")

# wrap all libraries with -Wl,--start-group -Wl,--end-group to prevent link order issue
group_link_libraries()

