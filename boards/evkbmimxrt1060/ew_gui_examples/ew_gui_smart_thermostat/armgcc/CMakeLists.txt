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


project(ew_gui_smart_thermostat)

set(MCUX_SDK_PROJECT_NAME ew_gui_smart_thermostat.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../ewmain.c"
"${ProjDirPath}/../ewconfig.h"
"${ProjDirPath}/../ewdef.h"
"${ProjDirPath}/../ewmain.h"
"${ProjDirPath}/../DeviceDriver.c"
"${ProjDirPath}/../DeviceDriver.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Application.c"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Core.c"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Effects.c"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Graphics.c"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Oximeter.c"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Resources.c"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Templates.c"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Views.c"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/WidgetSet.c"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Application.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Core.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Effects.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Graphics.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Oximeter.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Resources.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Templates.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/Views.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/WidgetSet.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_ApplicationApplication.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreCursorEvent.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreCursorGrabEvent.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreCursorHit.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreDialogContext.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreDragEvent.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreEvent.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreGroup.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreKeyEvent.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreKeyPressHandler.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreLayoutContext.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreLayoutLineContext.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreLayoutQuadContext.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreLineView.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreModalContext.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreOutline.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CorePropertyObserver.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreQuadView.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreRectView.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreResource.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreRoot.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreRotateTouchHandler.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreSimpleTouchHandler.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreSlideTouchHandler.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreTask.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreTaskQueue.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreTimer.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_CoreView.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_EffectsEffect.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_EffectsEffectTimerClass.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_EffectsFloatEffect.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_EffectsInt32Effect.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_EffectsRectEffect.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_EffectsTimingList.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_GraphicsArcPath.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_GraphicsCanvas.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_GraphicsPath.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_GraphicsWarpMatrix.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterAnalogDisplay.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterBackground.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterBattery.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterDataStorage.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterDeviceClass.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterDigitsLarge.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterDigitsSmall.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterFontCaption.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterFontL.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterFontM.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterGlow.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterGraph.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterGraphPanel.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterHeartLarge.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterIconButton.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterLungLarge.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterMainMenu.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterMenuButton.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterMenuIcons.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterNumberDisplay.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterOximeter.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterOxygenGradient.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterOxygenPanel.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterOxygenSettings.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterPlotter.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterPulseGradient.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterPulsePanel.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterPulseSettings.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterPushButton.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterRotaryBackground.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterRotaryBackgroundFlow.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterRotaryBackgroundOxygen.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterRotaryThumbMax.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterRotaryThumbMin.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterThumb.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterToggleButton.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterTrack.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_OximeterValueDisplay.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_ResourcesBitmap.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_ResourcesFont.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_ResourcesWhiteBitmapStripe.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_TemplatesDeviceClass.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_ViewsFillPath.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_ViewsFrame.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_ViewsImage.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_ViewsLine.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_ViewsRectangle.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_ViewsStrokePath.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_ViewsText.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_ViewsWallpaper.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_ViewsWarpImage.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_ViewsWarpView.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetHorizontalSlider.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetHorizontalSliderConfig.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetPushButton.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetPushButtonConfig.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetRotaryKnob.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetRotaryKnobConfig.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetToggleButton.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetToggleButtonConfig.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetWidgetConfig.h"
"${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode/ewlocale.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../../ew_gui_pulse_oximeter/GeneratedCode
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers
    ${ProjDirPath}/../../../../../components/ft5406_rt
    ${ProjDirPath}/../../../../../components/gt911
    ${ProjDirPath}/../../../../../components/video
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/EmbeddedWizard
    ${ProjDirPath}/../../../../../devices/MIMXRT1062
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities
)

# include modules
include(driver_cache_armv7_m7_MIMXRT1062)

include(driver_elcdif_MIMXRT1062)

include(driver_snvs_hp_MIMXRT1062)

include(driver_pxp_MIMXRT1062)

include(driver_lpi2c_MIMXRT1062)

include(driver_ft5406_rt_MIMXRT1062)

include(driver_gt911_MIMXRT1062)

include(driver_video-common_MIMXRT1062)

include(utility_debug_console_lite_MIMXRT1062)

include(utility_assert_lite_MIMXRT1062)

include(middleware_embedded_wizard_lib_evkmimxrt10xx_MIMXRT1062)

include(driver_common_MIMXRT1062)

include(driver_clock_MIMXRT1062)

include(middleware_embedded_wizard_evkbmimxrt1060_MIMXRT1062)

include(device_MIMXRT1062_CMSIS_MIMXRT1062)

include(component_lpuart_adapter_MIMXRT1062)

include(component_lists_MIMXRT1062)

include(driver_lpuart_MIMXRT1062)

include(device_MIMXRT1062_startup_MIMXRT1062)

include(driver_iomuxc_MIMXRT1062)

include(driver_igpio_MIMXRT1062)

include(driver_xip_device_MIMXRT1062)

include(driver_xip_board_evkbmimxrt1060_MIMXRT1062)

include(CMSIS_Include_core_cm_MIMXRT1062)

include(utilities_misc_utilities_MIMXRT1062)

include(device_MIMXRT1062_system_MIMXRT1062)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/EmbeddedWizard/PlatformPackage/evkmimxrt1010/RTE/libewrte-m7-gcc.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/EmbeddedWizard/PlatformPackage/evkmimxrt1010/RGB565/libewgfx-m7-gcc.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/EmbeddedWizard/PlatformPackage/evkmimxrt1010/RTE/libewrte-m7-gcc.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/EmbeddedWizard/PlatformPackage/evkmimxrt1010/RGB565/libewgfx-m7-gcc.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


