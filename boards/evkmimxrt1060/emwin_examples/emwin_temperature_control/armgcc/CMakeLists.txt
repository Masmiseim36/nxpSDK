# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.1.1)

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


project(emwin_temperature_control)

set(MCUX_SDK_PROJECT_NAME emwin_temperature_control.elf)

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
"${ProjDirPath}/../AppWizard/Resource/Font/Roboto_24_Normal_EXT_AA4.c"
"${ProjDirPath}/../AppWizard/Resource/Font/NettoOT_24_Normal_EXT_AA4.c"
"${ProjDirPath}/../AppWizard/Resource/Font/Roboto_16_Normal_EXT_AA4.c"
"${ProjDirPath}/../AppWizard/Resource/Image/SeggerLogo_80x40.c"
"${ProjDirPath}/../AppWizard/Resource/Image/KNOB_106x106.c"
"${ProjDirPath}/../AppWizard/Resource/Image/GreenGlowMarker_16x9.c"
"${ProjDirPath}/../AppWizard/Resource/Image/ButtonRound_48x21.c"
"${ProjDirPath}/../AppWizard/Resource/Image/TemperatureControl_3_480x272.c"
"${ProjDirPath}/../AppWizard/Resource/Image/ButtonRoundPressed_48x21.c"
"${ProjDirPath}/../AppWizard/Resource/Text/APPW_Pattern.c"
"${ProjDirPath}/../AppWizard/Resource/Text/APPW_Language_0.c"
"${ProjDirPath}/../AppWizard/Source/APPW_MainTask.c"
"${ProjDirPath}/../AppWizard/Source/CustomCode/Application.h"
"${ProjDirPath}/../AppWizard/Source/CustomCode/ID_SCREEN_TEMP_CTRL_Slots.c"
"${ProjDirPath}/../AppWizard/Source/Generated/ID_SCREEN_TEMP_CTRL.c"
"${ProjDirPath}/../AppWizard/Source/Generated/Resource.h"
"${ProjDirPath}/../AppWizard/Source/Generated/APPWConf.c"
"${ProjDirPath}/../AppWizard/Source/Generated/Resource.c"
"${ProjDirPath}/../AppWizard/Source/Generated/ID_SCREEN_TEMP_CTRL.h"
"${ProjDirPath}/../AppWizard/APPW_X_NoFS.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers
    ${ProjDirPath}/../../../../../components/ft5406_rt
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/emwin
    ${ProjDirPath}/../../../../../devices/MIMXRT1062
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities
)

# include modules
include(driver_elcdif_MIMXRT1062)

include(driver_pxp_MIMXRT1062)

include(driver_lpi2c_MIMXRT1062)

include(driver_ft5406_rt_MIMXRT1062)

include(driver_gpt_MIMXRT1062)

include(utility_debug_console_lite_MIMXRT1062)

include(utility_assert_lite_MIMXRT1062)

include(middleware_emwin_MIMXRT1062)

include(driver_common_MIMXRT1062)

include(driver_clock_MIMXRT1062)

include(device_MIMXRT1062_CMSIS_MIMXRT1062)

include(component_lpuart_adapter_MIMXRT1062)

include(component_lists_MIMXRT1062)

include(driver_lpuart_MIMXRT1062)

include(device_MIMXRT1062_startup_MIMXRT1062)

include(driver_iomuxc_MIMXRT1062)

include(driver_igpio_MIMXRT1062)

include(driver_xip_device_MIMXRT1062)

include(driver_xip_board_MIMXRT1062)

include(CMSIS_Include_core_cm7_MIMXRT1062)

include(CMSIS_Include_common_MIMXRT1062)

include(CMSIS_Include_dsp_MIMXRT1062)

include(utilities_misc_utilities_MIMXRT1062)

include(device_MIMXRT1062_system_MIMXRT1062)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/emwin/emWin_library/ARMGCC/libemWin_M7.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/emwin/emWin_library/ARMGCC/libemWin_M7.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


