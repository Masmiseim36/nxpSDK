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

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME ew_gui_smart_thermostat.elf)

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
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Application.c"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Core.c"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Effects.c"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Graphics.c"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Oximeter.c"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Resources.c"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Templates.c"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Views.c"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/WidgetSet.c"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Application.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Core.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Effects.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Graphics.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Oximeter.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Resources.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Templates.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/Views.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/WidgetSet.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_ApplicationApplication.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreCursorEvent.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreCursorGrabEvent.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreCursorHit.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreDialogContext.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreDragEvent.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreEvent.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreGroup.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreKeyEvent.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreKeyPressHandler.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreLayoutContext.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreLayoutLineContext.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreLayoutQuadContext.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreLineView.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreModalContext.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreOutline.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CorePropertyObserver.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreQuadView.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreRectView.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreResource.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreRoot.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreRotateTouchHandler.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreSimpleTouchHandler.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreSlideTouchHandler.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreTask.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreTaskQueue.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreTimer.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_CoreView.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_EffectsEffect.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_EffectsEffectTimerClass.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_EffectsFloatEffect.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_EffectsInt32Effect.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_EffectsRectEffect.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_EffectsTimingList.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_GraphicsArcPath.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_GraphicsCanvas.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_GraphicsPath.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_GraphicsWarpMatrix.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterAnalogDisplay.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterBackground.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterBattery.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterDataStorage.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterDeviceClass.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterDigitsLarge.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterDigitsSmall.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterFontCaption.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterFontL.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterFontM.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterGlow.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterGraph.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterGraphPanel.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterHeartLarge.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterIconButton.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterLungLarge.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterMainMenu.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterMenuButton.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterMenuIcons.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterNumberDisplay.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterOximeter.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterOxygenGradient.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterOxygenPanel.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterOxygenSettings.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterPlotter.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterPulseGradient.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterPulsePanel.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterPulseSettings.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterPushButton.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterRotaryBackground.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterRotaryBackgroundFlow.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterRotaryBackgroundOxygen.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterRotaryThumbMax.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterRotaryThumbMin.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterThumb.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterToggleButton.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterTrack.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_OximeterValueDisplay.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_ResourcesBitmap.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_ResourcesFont.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_ResourcesWhiteBitmapStripe.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_TemplatesDeviceClass.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_ViewsFillPath.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_ViewsFrame.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_ViewsImage.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_ViewsLine.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_ViewsRectangle.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_ViewsStrokePath.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_ViewsText.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_ViewsWallpaper.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_ViewsWarpImage.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_ViewsWarpView.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetHorizontalSlider.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetHorizontalSliderConfig.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetPushButton.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetPushButtonConfig.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetRotaryKnob.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetRotaryKnobConfig.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetToggleButton.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetToggleButtonConfig.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/_WidgetSetWidgetConfig.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode/ewlocale.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkbmimxrt1060/ew_gui_examples/ew_gui_pulse_oximeter/GeneratedCode
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT1062/drivers
    ${SdkRootDirPath}/components/ft5406_rt
    ${SdkRootDirPath}/components/gt911
    ${SdkRootDirPath}/components/video
    ${SdkRootDirPath}/devices/MIMXRT1062/utilities
    ${SdkRootDirPath}/devices/MIMXRT1062/utilities/debug_console_lite
    ${SdkRootDirPath}/middleware/EmbeddedWizard
    ${SdkRootDirPath}/devices/MIMXRT1062
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1062/xip
    ${SdkRootDirPath}/boards/evkbmimxrt1060/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
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


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

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


