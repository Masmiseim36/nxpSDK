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


project(ew_gui_dashboard)

set(MCUX_SDK_PROJECT_NAME ew_gui_dashboard.elf)

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
"${ProjDirPath}/../GeneratedCode/Dashboard.c"
"${ProjDirPath}/../GeneratedCode/Effects.c"
"${ProjDirPath}/../GeneratedCode/Graphics.c"
"${ProjDirPath}/../GeneratedCode/Resources.c"
"${ProjDirPath}/../GeneratedCode/Templates.c"
"${ProjDirPath}/../GeneratedCode/Views.c"
"${ProjDirPath}/../GeneratedCode/WidgetSet.c"
"${ProjDirPath}/../GeneratedCode/Application.h"
"${ProjDirPath}/../GeneratedCode/Core.h"
"${ProjDirPath}/../GeneratedCode/Dashboard.h"
"${ProjDirPath}/../GeneratedCode/Effects.h"
"${ProjDirPath}/../GeneratedCode/Graphics.h"
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
"${ProjDirPath}/../GeneratedCode/_CoreLayoutQuadContext.h"
"${ProjDirPath}/../GeneratedCode/_CoreModalContext.h"
"${ProjDirPath}/../GeneratedCode/_CoreOutline.h"
"${ProjDirPath}/../GeneratedCode/_CoreQuadView.h"
"${ProjDirPath}/../GeneratedCode/_CoreRectView.h"
"${ProjDirPath}/../GeneratedCode/_CoreResource.h"
"${ProjDirPath}/../GeneratedCode/_CoreRoot.h"
"${ProjDirPath}/../GeneratedCode/_CoreSimpleTouchHandler.h"
"${ProjDirPath}/../GeneratedCode/_CoreTask.h"
"${ProjDirPath}/../GeneratedCode/_CoreTaskQueue.h"
"${ProjDirPath}/../GeneratedCode/_CoreTimer.h"
"${ProjDirPath}/../GeneratedCode/_CoreView.h"
"${ProjDirPath}/../GeneratedCode/_DashboardAnimationClass.h"
"${ProjDirPath}/../GeneratedCode/_DashboardBattery.h"
"${ProjDirPath}/../GeneratedCode/_DashboardBatteryFrame.h"
"${ProjDirPath}/../GeneratedCode/_DashboardCenterGlowV2.h"
"${ProjDirPath}/../GeneratedCode/_DashboardCircle0.h"
"${ProjDirPath}/../GeneratedCode/_DashboardCircle2.h"
"${ProjDirPath}/../GeneratedCode/_DashboardCircle3.h"
"${ProjDirPath}/../GeneratedCode/_DashboardCircle4.h"
"${ProjDirPath}/../GeneratedCode/_DashboardCircle5.h"
"${ProjDirPath}/../GeneratedCode/_DashboardColorsClass.h"
"${ProjDirPath}/../GeneratedCode/_DashboardDashboard.h"
"${ProjDirPath}/../GeneratedCode/_DashboardDashboardV1.h"
"${ProjDirPath}/../GeneratedCode/_DashboardDashboardV2.h"
"${ProjDirPath}/../GeneratedCode/_DashboardDemoMode.h"
"${ProjDirPath}/../GeneratedCode/_DashboardDeviceClass.h"
"${ProjDirPath}/../GeneratedCode/_DashboardFontL.h"
"${ProjDirPath}/../GeneratedCode/_DashboardFontLightM.h"
"${ProjDirPath}/../GeneratedCode/_DashboardFontLightS.h"
"${ProjDirPath}/../GeneratedCode/_DashboardFontS.h"
"${ProjDirPath}/../GeneratedCode/_DashboardFontXL.h"
"${ProjDirPath}/../GeneratedCode/_DashboardFontXXL.h"
"${ProjDirPath}/../GeneratedCode/_DashboardGauge.h"
"${ProjDirPath}/../GeneratedCode/_DashboardGaugeDisplay.h"
"${ProjDirPath}/../GeneratedCode/_DashboardGaugeGlowSegment.h"
"${ProjDirPath}/../GeneratedCode/_DashboardGaugeIndicator.h"
"${ProjDirPath}/../GeneratedCode/_DashboardGaugeLabels.h"
"${ProjDirPath}/../GeneratedCode/_DashboardGaugeNeedle.h"
"${ProjDirPath}/../GeneratedCode/_DashboardHorizontalLine.h"
"${ProjDirPath}/../GeneratedCode/_DashboardIcons.h"
"${ProjDirPath}/../GeneratedCode/_DashboardKWIcon.h"
"${ProjDirPath}/../GeneratedCode/_DashboardLeftBottomBar.h"
"${ProjDirPath}/../GeneratedCode/_DashboardLeftBottomGlowV1.h"
"${ProjDirPath}/../GeneratedCode/_DashboardLeftBricksV2.h"
"${ProjDirPath}/../GeneratedCode/_DashboardLeftGlowV2.h"
"${ProjDirPath}/../GeneratedCode/_DashboardLeftMiddleGlowV1.h"
"${ProjDirPath}/../GeneratedCode/_DashboardLeftTopBar.h"
"${ProjDirPath}/../GeneratedCode/_DashboardLogo.h"
"${ProjDirPath}/../GeneratedCode/_DashboardOffIcon.h"
"${ProjDirPath}/../GeneratedCode/_DashboardPWRIcon.h"
"${ProjDirPath}/../GeneratedCode/_DashboardRightBottomGlowV1.h"
"${ProjDirPath}/../GeneratedCode/_DashboardRightBricksV2.h"
"${ProjDirPath}/../GeneratedCode/_DashboardRightGlowV2.h"
"${ProjDirPath}/../GeneratedCode/_DashboardRightMiddleGlowV1.h"
"${ProjDirPath}/../GeneratedCode/_DashboardRightValueBar.h"
"${ProjDirPath}/../GeneratedCode/_DashboardStartEngine.h"
"${ProjDirPath}/../GeneratedCode/_DashboardStartEngineGloom.h"
"${ProjDirPath}/../GeneratedCode/_DashboardStartEngineStates.h"
"${ProjDirPath}/../GeneratedCode/_DashboardStartScreen.h"
"${ProjDirPath}/../GeneratedCode/_DashboardSwitchIcon.h"
"${ProjDirPath}/../GeneratedCode/_DashboardTopGlowV1.h"
"${ProjDirPath}/../GeneratedCode/_EffectsEffect.h"
"${ProjDirPath}/../GeneratedCode/_EffectsEffectTimerClass.h"
"${ProjDirPath}/../GeneratedCode/_EffectsFadeInOutTransition.h"
"${ProjDirPath}/../GeneratedCode/_EffectsFader.h"
"${ProjDirPath}/../GeneratedCode/_EffectsFaderTask.h"
"${ProjDirPath}/../GeneratedCode/_EffectsFloatEffect.h"
"${ProjDirPath}/../GeneratedCode/_EffectsInt32Effect.h"
"${ProjDirPath}/../GeneratedCode/_EffectsOpacityFader.h"
"${ProjDirPath}/../GeneratedCode/_EffectsPointEffect.h"
"${ProjDirPath}/../GeneratedCode/_EffectsPositionFader.h"
"${ProjDirPath}/../GeneratedCode/_EffectsShowHideTransition.h"
"${ProjDirPath}/../GeneratedCode/_EffectsTimingList.h"
"${ProjDirPath}/../GeneratedCode/_EffectsTransition.h"
"${ProjDirPath}/../GeneratedCode/_EffectsVisibilityFader.h"
"${ProjDirPath}/../GeneratedCode/_GraphicsCanvas.h"
"${ProjDirPath}/../GeneratedCode/_GraphicsPath.h"
"${ProjDirPath}/../GeneratedCode/_GraphicsWarpMatrix.h"
"${ProjDirPath}/../GeneratedCode/_ResourcesBitmap.h"
"${ProjDirPath}/../GeneratedCode/_ResourcesDefaultFont.h"
"${ProjDirPath}/../GeneratedCode/_ResourcesFont.h"
"${ProjDirPath}/../GeneratedCode/_TemplatesHorizontalValueBar.h"
"${ProjDirPath}/../GeneratedCode/_TemplatesVerticalValueBar.h"
"${ProjDirPath}/../GeneratedCode/_ViewsFrame.h"
"${ProjDirPath}/../GeneratedCode/_ViewsImage.h"
"${ProjDirPath}/../GeneratedCode/_ViewsRectangle.h"
"${ProjDirPath}/../GeneratedCode/_ViewsStrokePath.h"
"${ProjDirPath}/../GeneratedCode/_ViewsText.h"
"${ProjDirPath}/../GeneratedCode/_ViewsWarpGroup.h"
"${ProjDirPath}/../GeneratedCode/_ViewsWarpImage.h"
"${ProjDirPath}/../GeneratedCode/_ViewsWarpView.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetPushButton.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetPushButtonConfig.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetToggleButton.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetToggleButtonConfig.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetValueDisplay.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetValueDisplayConfig.h"
"${ProjDirPath}/../GeneratedCode/_WidgetSetWidgetConfig.h"
"${ProjDirPath}/../GeneratedCode/ewlocale.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../GeneratedCode
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
    ${ProjDirPath}/../../../../../components/silicon_id
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

include(component_silicon_id_MIMXRT1062)

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


