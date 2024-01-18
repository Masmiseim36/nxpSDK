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


project(ew_gui_pulse_oximeter)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME ew_gui_pulse_oximeter.elf)

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
"${ProjDirPath}/../GeneratedCode/Application.c"
"${ProjDirPath}/../GeneratedCode/Core.c"
"${ProjDirPath}/../GeneratedCode/Effects.c"
"${ProjDirPath}/../GeneratedCode/Graphics.c"
"${ProjDirPath}/../GeneratedCode/Oximeter.c"
"${ProjDirPath}/../GeneratedCode/Resources.c"
"${ProjDirPath}/../GeneratedCode/Templates.c"
"${ProjDirPath}/../GeneratedCode/Views.c"
"${ProjDirPath}/../GeneratedCode/WidgetSet.c"
"${ProjDirPath}/../GeneratedCode/Application.h"
"${ProjDirPath}/../GeneratedCode/Core.h"
"${ProjDirPath}/../GeneratedCode/Effects.h"
"${ProjDirPath}/../GeneratedCode/Graphics.h"
"${ProjDirPath}/../GeneratedCode/Oximeter.h"
"${ProjDirPath}/../GeneratedCode/Resources.h"
"${ProjDirPath}/../GeneratedCode/Templates.h"
"${ProjDirPath}/../GeneratedCode/Views.h"
"${ProjDirPath}/../GeneratedCode/WidgetSet.h"
"${ProjDirPath}/../GeneratedCode/_ApplicationApplication.h"
"${ProjDirPath}/../GeneratedCode/_CoreCursorEvent.h"
"${ProjDirPath}/../GeneratedCode/_CoreCursorGrabEvent.h"
"${ProjDirPath}/../GeneratedCode/_CoreCursorHit.h"
"${ProjDirPath}/../GeneratedCode/_CoreDialogContext.h"
"${ProjDirPath}/../GeneratedCode/_CoreDragEvent.h"
"${ProjDirPath}/../GeneratedCode/_CoreEvent.h"
"${ProjDirPath}/../GeneratedCode/_CoreGroup.h"
"${ProjDirPath}/../GeneratedCode/_CoreKeyEvent.h"
"${ProjDirPath}/../GeneratedCode/_CoreKeyPressHandler.h"
"${ProjDirPath}/../GeneratedCode/_CoreLayoutContext.h"
"${ProjDirPath}/../GeneratedCode/_CoreLayoutLineContext.h"
"${ProjDirPath}/../GeneratedCode/_CoreLayoutQuadContext.h"
"${ProjDirPath}/../GeneratedCode/_CoreLineView.h"
"${ProjDirPath}/../GeneratedCode/_CoreModalContext.h"
"${ProjDirPath}/../GeneratedCode/_CoreOutline.h"
"${ProjDirPath}/../GeneratedCode/_CorePropertyObserver.h"
"${ProjDirPath}/../GeneratedCode/_CoreQuadView.h"
"${ProjDirPath}/../GeneratedCode/_CoreRectView.h"
"${ProjDirPath}/../GeneratedCode/_CoreResource.h"
"${ProjDirPath}/../GeneratedCode/_CoreRoot.h"
"${ProjDirPath}/../GeneratedCode/_CoreRotateTouchHandler.h"
"${ProjDirPath}/../GeneratedCode/_CoreSimpleTouchHandler.h"
"${ProjDirPath}/../GeneratedCode/_CoreSlideTouchHandler.h"
"${ProjDirPath}/../GeneratedCode/_CoreTask.h"
"${ProjDirPath}/../GeneratedCode/_CoreTaskQueue.h"
"${ProjDirPath}/../GeneratedCode/_CoreTimer.h"
"${ProjDirPath}/../GeneratedCode/_CoreView.h"
"${ProjDirPath}/../GeneratedCode/_EffectsEffect.h"
"${ProjDirPath}/../GeneratedCode/_EffectsEffectTimerClass.h"
"${ProjDirPath}/../GeneratedCode/_EffectsFloatEffect.h"
"${ProjDirPath}/../GeneratedCode/_EffectsInt32Effect.h"
"${ProjDirPath}/../GeneratedCode/_EffectsRectEffect.h"
"${ProjDirPath}/../GeneratedCode/_EffectsTimingList.h"
"${ProjDirPath}/../GeneratedCode/_GraphicsArcPath.h"
"${ProjDirPath}/../GeneratedCode/_GraphicsCanvas.h"
"${ProjDirPath}/../GeneratedCode/_GraphicsPath.h"
"${ProjDirPath}/../GeneratedCode/_GraphicsWarpMatrix.h"
"${ProjDirPath}/../GeneratedCode/_OximeterAnalogDisplay.h"
"${ProjDirPath}/../GeneratedCode/_OximeterBackground.h"
"${ProjDirPath}/../GeneratedCode/_OximeterBattery.h"
"${ProjDirPath}/../GeneratedCode/_OximeterDataStorage.h"
"${ProjDirPath}/../GeneratedCode/_OximeterDeviceClass.h"
"${ProjDirPath}/../GeneratedCode/_OximeterDigitsLarge.h"
"${ProjDirPath}/../GeneratedCode/_OximeterDigitsSmall.h"
"${ProjDirPath}/../GeneratedCode/_OximeterFontCaption.h"
"${ProjDirPath}/../GeneratedCode/_OximeterFontL.h"
"${ProjDirPath}/../GeneratedCode/_OximeterFontM.h"
"${ProjDirPath}/../GeneratedCode/_OximeterGlow.h"
"${ProjDirPath}/../GeneratedCode/_OximeterGraph.h"
"${ProjDirPath}/../GeneratedCode/_OximeterGraphPanel.h"
"${ProjDirPath}/../GeneratedCode/_OximeterHeartLarge.h"
"${ProjDirPath}/../GeneratedCode/_OximeterIconButton.h"
"${ProjDirPath}/../GeneratedCode/_OximeterLungLarge.h"
"${ProjDirPath}/../GeneratedCode/_OximeterMainMenu.h"
"${ProjDirPath}/../GeneratedCode/_OximeterMenuButton.h"
"${ProjDirPath}/../GeneratedCode/_OximeterMenuIcons.h"
"${ProjDirPath}/../GeneratedCode/_OximeterNumberDisplay.h"
"${ProjDirPath}/../GeneratedCode/_OximeterOximeter.h"
"${ProjDirPath}/../GeneratedCode/_OximeterOxygenGradient.h"
"${ProjDirPath}/../GeneratedCode/_OximeterOxygenPanel.h"
"${ProjDirPath}/../GeneratedCode/_OximeterOxygenSettings.h"
"${ProjDirPath}/../GeneratedCode/_OximeterPlotter.h"
"${ProjDirPath}/../GeneratedCode/_OximeterPulseGradient.h"
"${ProjDirPath}/../GeneratedCode/_OximeterPulsePanel.h"
"${ProjDirPath}/../GeneratedCode/_OximeterPulseSettings.h"
"${ProjDirPath}/../GeneratedCode/_OximeterPushButton.h"
"${ProjDirPath}/../GeneratedCode/_OximeterRotaryBackground.h"
"${ProjDirPath}/../GeneratedCode/_OximeterRotaryBackgroundFlow.h"
"${ProjDirPath}/../GeneratedCode/_OximeterRotaryBackgroundOxygen.h"
"${ProjDirPath}/../GeneratedCode/_OximeterRotaryThumbMax.h"
"${ProjDirPath}/../GeneratedCode/_OximeterRotaryThumbMin.h"
"${ProjDirPath}/../GeneratedCode/_OximeterThumb.h"
"${ProjDirPath}/../GeneratedCode/_OximeterToggleButton.h"
"${ProjDirPath}/../GeneratedCode/_OximeterTrack.h"
"${ProjDirPath}/../GeneratedCode/_OximeterValueDisplay.h"
"${ProjDirPath}/../GeneratedCode/_ResourcesBitmap.h"
"${ProjDirPath}/../GeneratedCode/_ResourcesFont.h"
"${ProjDirPath}/../GeneratedCode/_ResourcesWhiteBitmapStripe.h"
"${ProjDirPath}/../GeneratedCode/_TemplatesDeviceClass.h"
"${ProjDirPath}/../GeneratedCode/_ViewsFillPath.h"
"${ProjDirPath}/../GeneratedCode/_ViewsFrame.h"
"${ProjDirPath}/../GeneratedCode/_ViewsImage.h"
"${ProjDirPath}/../GeneratedCode/_ViewsLine.h"
"${ProjDirPath}/../GeneratedCode/_ViewsRectangle.h"
"${ProjDirPath}/../GeneratedCode/_ViewsStrokePath.h"
"${ProjDirPath}/../GeneratedCode/_ViewsText.h"
"${ProjDirPath}/../GeneratedCode/_ViewsWallpaper.h"
"${ProjDirPath}/../GeneratedCode/_ViewsWarpImage.h"
"${ProjDirPath}/../GeneratedCode/_ViewsWarpView.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetHorizontalSlider.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetHorizontalSliderConfig.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetPushButton.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetPushButtonConfig.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetRotaryKnob.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetRotaryKnobConfig.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetToggleButton.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetToggleButtonConfig.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetWidgetConfig.h"
"${ProjDirPath}/../GeneratedCode/ewlocale.h"
"${ProjDirPath}/../EmbeddedWizard/Application.ewu"
"${ProjDirPath}/../EmbeddedWizard/Oximeter.ewu"
"${ProjDirPath}/../EmbeddedWizard/PulseOximeter.ewp"
"${ProjDirPath}/../EmbeddedWizard/PulseOximeter.ews"
"${ProjDirPath}/../EmbeddedWizard/Res/Battery.png"
"${ProjDirPath}/../EmbeddedWizard/Res/DigitsLarge.png"
"${ProjDirPath}/../EmbeddedWizard/Res/DigitsSmall.png"
"${ProjDirPath}/../EmbeddedWizard/Res/Glow.png"
"${ProjDirPath}/../EmbeddedWizard/Res/HeartLarge.png"
"${ProjDirPath}/../EmbeddedWizard/Res/IconButton.png"
"${ProjDirPath}/../EmbeddedWizard/Res/LungLarge.png"
"${ProjDirPath}/../EmbeddedWizard/Res/MenuButton.png"
"${ProjDirPath}/../EmbeddedWizard/Res/MenuIcons.png"
"${ProjDirPath}/../EmbeddedWizard/Res/OxygenGradient.png"
"${ProjDirPath}/../EmbeddedWizard/Res/PulseGradient.png"
"${ProjDirPath}/../EmbeddedWizard/Res/PushButton.png"
"${ProjDirPath}/../EmbeddedWizard/Res/RotaryBackground.png"
"${ProjDirPath}/../EmbeddedWizard/Res/RotaryBackgroundFlow.png"
"${ProjDirPath}/../EmbeddedWizard/Res/RotaryBackgroundOxygen.png"
"${ProjDirPath}/../EmbeddedWizard/Res/RotaryThumbMax.png"
"${ProjDirPath}/../EmbeddedWizard/Res/RotaryThumbMin.png"
"${ProjDirPath}/../EmbeddedWizard/Res/Thumb.png"
"${ProjDirPath}/../EmbeddedWizard/Res/ToggleButton.png"
"${ProjDirPath}/../EmbeddedWizard/Res/Track.png"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../GeneratedCode
)


include(${SdkRootDirPath}/devices/MIMXRT1062/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map")

