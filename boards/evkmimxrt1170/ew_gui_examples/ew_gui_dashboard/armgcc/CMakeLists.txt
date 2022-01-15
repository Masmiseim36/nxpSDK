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
"${ProjDirPath}/../display_support.h"
"${ProjDirPath}/../display_support.c"
"${ProjDirPath}/../../../vglite_examples/vglite_support/vglite_support.h"
"${ProjDirPath}/../../../vglite_examples/vglite_support/vglite_support.c"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../ewmain.c"
"${ProjDirPath}/../ewconfig.h"
"${ProjDirPath}/../ewdef.h"
"${ProjDirPath}/../ewmain.h"
"${ProjDirPath}/../DeviceDriver.c"
"${ProjDirPath}/../DeviceDriver.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../cm7/FreeRTOSConfig.h"
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
"${ProjDirPath}/../GeneratedCode/_CoreLayoutLineContext.h"
"${ProjDirPath}/../GeneratedCode/_CoreLayoutQuadContext.h"
"${ProjDirPath}/../GeneratedCode/_CoreLineView.h"
"${ProjDirPath}/../GeneratedCode/_CoreModalContext.h"
"${ProjDirPath}/../GeneratedCode/_CoreOutline.h"
"${ProjDirPath}/../GeneratedCode/_CoreQuadView.h"
"${ProjDirPath}/../GeneratedCode/_CoreRectView.h"
"${ProjDirPath}/../GeneratedCode/_CoreResource.h"
"${ProjDirPath}/../GeneratedCode/_CoreRoot.h"
"${ProjDirPath}/../GeneratedCode/_CoreSimpleTouchHandler.h"
"${ProjDirPath}/../GeneratedCode/_CoreTask.h"
"${ProjDirPath}/../GeneratedCode/_CoreTaskQueue.h"
"${ProjDirPath}/../GeneratedCode/_CoreTime.h"
"${ProjDirPath}/../GeneratedCode/_CoreTimer.h"
"${ProjDirPath}/../GeneratedCode/_CoreView.h"
"${ProjDirPath}/../GeneratedCode/_DashboardAnimationClass.h"
"${ProjDirPath}/../GeneratedCode/_DashboardArrows.h"
"${ProjDirPath}/../GeneratedCode/_DashboardBattery.h"
"${ProjDirPath}/../GeneratedCode/_DashboardBatteryEnergy.h"
"${ProjDirPath}/../GeneratedCode/_DashboardBatteryFrame.h"
"${ProjDirPath}/../GeneratedCode/_DashboardCircle0.h"
"${ProjDirPath}/../GeneratedCode/_DashboardCircle1.h"
"${ProjDirPath}/../GeneratedCode/_DashboardCircle2.h"
"${ProjDirPath}/../GeneratedCode/_DashboardCircle3.h"
"${ProjDirPath}/../GeneratedCode/_DashboardCircle4.h"
"${ProjDirPath}/../GeneratedCode/_DashboardCircle5.h"
"${ProjDirPath}/../GeneratedCode/_DashboardClock.h"
"${ProjDirPath}/../GeneratedCode/_DashboardColorsClass.h"
"${ProjDirPath}/../GeneratedCode/_DashboardDashboard.h"
"${ProjDirPath}/../GeneratedCode/_DashboardDashboardV1.h"
"${ProjDirPath}/../GeneratedCode/_DashboardDashboardV2.h"
"${ProjDirPath}/../GeneratedCode/_DashboardDemoMode.h"
"${ProjDirPath}/../GeneratedCode/_DashboardDeviceClass.h"
"${ProjDirPath}/../GeneratedCode/_DashboardFontL.h"
"${ProjDirPath}/../GeneratedCode/_DashboardFontLightL.h"
"${ProjDirPath}/../GeneratedCode/_DashboardFontLightM.h"
"${ProjDirPath}/../GeneratedCode/_DashboardFontLightXL.h"
"${ProjDirPath}/../GeneratedCode/_DashboardFontXL.h"
"${ProjDirPath}/../GeneratedCode/_DashboardFontXXL.h"
"${ProjDirPath}/../GeneratedCode/_DashboardGauge.h"
"${ProjDirPath}/../GeneratedCode/_DashboardGaugeDisplay.h"
"${ProjDirPath}/../GeneratedCode/_DashboardGaugeGlowSegment.h"
"${ProjDirPath}/../GeneratedCode/_DashboardGaugeIndicator.h"
"${ProjDirPath}/../GeneratedCode/_DashboardGaugeLabels.h"
"${ProjDirPath}/../GeneratedCode/_DashboardGaugeNeedle.h"
"${ProjDirPath}/../GeneratedCode/_DashboardIcons.h"
"${ProjDirPath}/../GeneratedCode/_DashboardLeftBottomGloomV1.h"
"${ProjDirPath}/../GeneratedCode/_DashboardLeftGlowV2.h"
"${ProjDirPath}/../GeneratedCode/_DashboardLeftWing2V2.h"
"${ProjDirPath}/../GeneratedCode/_DashboardLeftWingV2.h"
"${ProjDirPath}/../GeneratedCode/_DashboardLogo.h"
"${ProjDirPath}/../GeneratedCode/_DashboardMap.h"
"${ProjDirPath}/../GeneratedCode/_DashboardMapFading.h"
"${ProjDirPath}/../GeneratedCode/_DashboardNavigation.h"
"${ProjDirPath}/../GeneratedCode/_DashboardNavigationHints.h"
"${ProjDirPath}/../GeneratedCode/_DashboardOffIcon.h"
"${ProjDirPath}/../GeneratedCode/_DashboardPosition.h"
"${ProjDirPath}/../GeneratedCode/_DashboardRightBottomGloomV1.h"
"${ProjDirPath}/../GeneratedCode/_DashboardRightGlowV2.h"
"${ProjDirPath}/../GeneratedCode/_DashboardRightWing2V2.h"
"${ProjDirPath}/../GeneratedCode/_DashboardRightWingV2.h"
"${ProjDirPath}/../GeneratedCode/_DashboardStartEngine.h"
"${ProjDirPath}/../GeneratedCode/_DashboardStartEngineGloom.h"
"${ProjDirPath}/../GeneratedCode/_DashboardStartEngineStates.h"
"${ProjDirPath}/../GeneratedCode/_DashboardStartScreen.h"
"${ProjDirPath}/../GeneratedCode/_DashboardSwitchIcon.h"
"${ProjDirPath}/../GeneratedCode/_DashboardTopGloomV1.h"
"${ProjDirPath}/../GeneratedCode/_DashboardVertValueBar.h"
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
"${ProjDirPath}/../GeneratedCode/_GraphicsArcPath.h"
"${ProjDirPath}/../GeneratedCode/_GraphicsCanvas.h"
"${ProjDirPath}/../GeneratedCode/_GraphicsPath.h"
"${ProjDirPath}/../GeneratedCode/_GraphicsWarpMatrix.h"
"${ProjDirPath}/../GeneratedCode/_ResourcesBitmap.h"
"${ProjDirPath}/../GeneratedCode/_ResourcesDefaultFont.h"
"${ProjDirPath}/../GeneratedCode/_ResourcesFont.h"
"${ProjDirPath}/../GeneratedCode/_ResourcesWhiteBitmapStripe.h"
"${ProjDirPath}/../GeneratedCode/_TemplatesHorizontalValueBar.h"
"${ProjDirPath}/../GeneratedCode/_TemplatesVerticalValueBar.h"
"${ProjDirPath}/../GeneratedCode/_ViewsFillPath.h"
"${ProjDirPath}/../GeneratedCode/_ViewsFrame.h"
"${ProjDirPath}/../GeneratedCode/_ViewsImage.h"
"${ProjDirPath}/../GeneratedCode/_ViewsLine.h"
"${ProjDirPath}/../GeneratedCode/_ViewsRectangle.h"
"${ProjDirPath}/../GeneratedCode/_ViewsStrokePath.h"
"${ProjDirPath}/../GeneratedCode/_ViewsText.h"
"${ProjDirPath}/../GeneratedCode/_ViewsWallpaper.h"
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
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../evkmimxrt1170_connect_cm4_cm7side.jlinkscript"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../cm7
    ${ProjDirPath}/../../../vglite_examples/vglite_support
    ${ProjDirPath}/..
    ${ProjDirPath}/../GeneratedCode
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/drivers
    ${ProjDirPath}/../../../../../components/ft5406_rt
    ${ProjDirPath}/../../../../../components/video
    ${ProjDirPath}/../../../../../components/video/display
    ${ProjDirPath}/../../../../../components/video/display/rm68191
    ${ProjDirPath}/../../../../../components/video/display/mipi_dsi_cmd
    ${ProjDirPath}/../../../../../components/video/display/rm68200
    ${ProjDirPath}/../../../../../components/video/display/dc/lcdifv2
    ${ProjDirPath}/../../../../../components/gt911
    ${ProjDirPath}/../../../../../components/video/display/dc/elcdif
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../components/video/display/fbdev
    ${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel
    ${ProjDirPath}/../../../../../middleware/vglite
    ${ProjDirPath}/../../../../../middleware/EmbeddedWizard
    ${ProjDirPath}/../../../../../components/video/display/hx8394
    ${ProjDirPath}/../../../../../devices/MIMXRT1176
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/drivers/cm7
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../components/video/display/dc
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/utilities
)

# include modules
include(driver_elcdif_MIMXRT1176_cm7)

include(driver_pxp_MIMXRT1176_cm7)

include(driver_lpi2c_MIMXRT1176_cm7)

include(driver_ft5406_rt_MIMXRT1176_cm7)

include(driver_video-common_MIMXRT1176_cm7)

include(driver_display-common_MIMXRT1176_cm7)

include(driver_display-rm68191_MIMXRT1176_cm7)

include(driver_mipi_dsi_split_MIMXRT1176_cm7)

include(driver_display-mipi-dsi-cmd_MIMXRT1176_cm7)

include(driver_common_MIMXRT1176_cm7)

include(driver_display-rm68200_MIMXRT1176_cm7)

include(driver_dc-fb-lcdifv2_MIMXRT1176_cm7)

include(driver_gt911_MIMXRT1176_cm7)

include(driver_soc_src_MIMXRT1176_cm7)

include(driver_dc-fb-elcdif_MIMXRT1176_cm7)

include(utility_debug_console_lite_MIMXRT1176_cm7)

include(driver_snvs_hp_MIMXRT1176_cm7)

include(driver_fbdev_MIMXRT1176_cm7)

include(middleware_freertos-kernel_heap_4_MIMXRT1176_cm7)

include(middleware_vglite_MIMXRT1176_cm7)

include(utility_assert_lite_MIMXRT1176_cm7)

include(middleware_embedded_wizard_lib_evkmimxrt1170_MIMXRT1176_cm7)

include(driver_display-hx8394_MIMXRT1176_cm7)

include(driver_clock_MIMXRT1176_cm7)

include(middleware_embedded_wizard_evkmimxrt1170_MIMXRT1176_cm7)

include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm7)

include(component_lpuart_adapter_MIMXRT1176_cm7)

include(component_lists_MIMXRT1176_cm7)

include(driver_lpuart_MIMXRT1176_cm7)

include(device_MIMXRT1176_startup_MIMXRT1176_cm7)

include(driver_iomuxc_MIMXRT1176_cm7)

include(driver_igpio_MIMXRT1176_cm7)

include(driver_xip_device_MIMXRT1176_cm7)

include(driver_xip_board_evkmimxrt1170_MIMXRT1176_cm7)

include(driver_pmu_1_MIMXRT1176_cm7)

include(driver_dcdc_soc_MIMXRT1176_cm7)

include(driver_cache_armv7_m7_MIMXRT1176_cm7)

include(driver_memory_MIMXRT1176_cm7)

include(driver_anatop_ai_MIMXRT1176_cm7)

include(CMSIS_Include_core_cm_MIMXRT1176_cm7)

include(driver_soc_mipi_csi2rx_MIMXRT1176_cm7)

include(driver_dc-fb-common_MIMXRT1176_cm7)

include(driver_lcdifv2_MIMXRT1176_cm7)

include(middleware_freertos-kernel_MIMXRT1176_cm7)

include(middleware_freertos-kernel_extension_MIMXRT1176_cm7)

include(utilities_misc_utilities_MIMXRT1176_cm7)

include(device_MIMXRT1176_system_MIMXRT1176_cm7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/EmbeddedWizard/PlatformPackage/evkmimxrt1170/RTE/libewrte-m7-gcc.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/EmbeddedWizard/PlatformPackage/evkmimxrt1170/RGB565/libewgfx-m7-r90-gcc.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/EmbeddedWizard/PlatformPackage/evkmimxrt1170/RTE/libewrte-m7-gcc.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/EmbeddedWizard/PlatformPackage/evkmimxrt1170/RGB565/libewgfx-m7-r90-gcc.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


