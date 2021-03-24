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


project(ew_gui_brick_game)

set(MCUX_SDK_PROJECT_NAME ew_gui_brick_game.elf)

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
"${ProjDirPath}/../GeneratedCode/Effects.c"
"${ProjDirPath}/../GeneratedCode/Application.c"
"${ProjDirPath}/../GeneratedCode/Resources.c"
"${ProjDirPath}/../GeneratedCode/Core.c"
"${ProjDirPath}/../GeneratedCode/Templates.c"
"${ProjDirPath}/../GeneratedCode/Graphics.c"
"${ProjDirPath}/../GeneratedCode/BrickGame.c"
"${ProjDirPath}/../GeneratedCode/Views.c"
"${ProjDirPath}/../GeneratedCode/_CoreQuadView.h"
"${ProjDirPath}/../GeneratedCode/_BrickGameStars.h"
"${ProjDirPath}/../GeneratedCode/_ApplicationApplication.h"
"${ProjDirPath}/../GeneratedCode/_ViewsText.h"
"${ProjDirPath}/../GeneratedCode/_BrickGameDescrFont.h"
"${ProjDirPath}/../GeneratedCode/_BrickGameLcdDisplay.h"
"${ProjDirPath}/../GeneratedCode/_BrickGamePaddle.h"
"${ProjDirPath}/../GeneratedCode/_ResourcesFont.h"
"${ProjDirPath}/../GeneratedCode/Resources.h"
"${ProjDirPath}/../GeneratedCode/_BrickGameDataClass.h"
"${ProjDirPath}/../GeneratedCode/_CoreKeyEvent.h"
"${ProjDirPath}/../GeneratedCode/_BrickGameLandscape.h"
"${ProjDirPath}/../GeneratedCode/_ViewsRectangle.h"
"${ProjDirPath}/../GeneratedCode/_CoreTask.h"
"${ProjDirPath}/../GeneratedCode/_CoreTaskQueue.h"
"${ProjDirPath}/../GeneratedCode/_CoreTimer.h"
"${ProjDirPath}/../GeneratedCode/_CoreGroup.h"
"${ProjDirPath}/../GeneratedCode/_CoreOutline.h"
"${ProjDirPath}/../GeneratedCode/_BrickGameBricks.h"
"${ProjDirPath}/../GeneratedCode/_CoreCursorEvent.h"
"${ProjDirPath}/../GeneratedCode/Core.h"
"${ProjDirPath}/../GeneratedCode/_CoreRoot.h"
"${ProjDirPath}/../GeneratedCode/_TemplatesDeviceClass.h"
"${ProjDirPath}/../GeneratedCode/_CoreLayoutContext.h"
"${ProjDirPath}/../GeneratedCode/_CoreEvent.h"
"${ProjDirPath}/../GeneratedCode/_EffectsEffectTimerClass.h"
"${ProjDirPath}/../GeneratedCode/_CoreModalContext.h"
"${ProjDirPath}/../GeneratedCode/_BrickGameBallPosition.h"
"${ProjDirPath}/../GeneratedCode/_CoreResource.h"
"${ProjDirPath}/../GeneratedCode/_CoreCursorGrabEvent.h"
"${ProjDirPath}/../GeneratedCode/_ResourcesBitmap.h"
"${ProjDirPath}/../GeneratedCode/_CoreCursorHit.h"
"${ProjDirPath}/../GeneratedCode/Application.h"
"${ProjDirPath}/../GeneratedCode/Effects.h"
"${ProjDirPath}/../GeneratedCode/Templates.h"
"${ProjDirPath}/../GeneratedCode/_CoreRectView.h"
"${ProjDirPath}/../GeneratedCode/_BrickGameLcdNumbers.h"
"${ProjDirPath}/../GeneratedCode/_BrickGameBall.h"
"${ProjDirPath}/../GeneratedCode/_BrickGameBrickGame.h"
"${ProjDirPath}/../GeneratedCode/_ViewsImage.h"
"${ProjDirPath}/../GeneratedCode/ewlocale.h"
"${ProjDirPath}/../GeneratedCode/_CoreDialogContext.h"
"${ProjDirPath}/../GeneratedCode/_GraphicsCanvas.h"
"${ProjDirPath}/../GeneratedCode/_CoreDragEvent.h"
"${ProjDirPath}/../GeneratedCode/_BrickGameBrickRow.h"
"${ProjDirPath}/../GeneratedCode/Graphics.h"
"${ProjDirPath}/../GeneratedCode/Views.h"
"${ProjDirPath}/../GeneratedCode/_CoreKeyPressHandler.h"
"${ProjDirPath}/../GeneratedCode/_CoreView.h"
"${ProjDirPath}/../GeneratedCode/BrickGame.h"
"${ProjDirPath}/../GeneratedCode/_CoreSimpleTouchHandler.h"
"${ProjDirPath}/../GeneratedCode/_CoreLayoutQuadContext.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../GeneratedCode
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers
    ${ProjDirPath}/../../../../../components/ft5406_rt
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/EmbeddedWizard
    ${ProjDirPath}/../../../../../devices/MIMXRT1064
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities
)

# include modules
include(driver_cache_armv7_m7_MIMXRT1064)

include(driver_elcdif_MIMXRT1064)

include(driver_snvs_hp_MIMXRT1064)

include(driver_pxp_MIMXRT1064)

include(driver_lpi2c_MIMXRT1064)

include(driver_ft5406_rt_MIMXRT1064)

include(utility_debug_console_lite_MIMXRT1064)

include(utility_assert_lite_MIMXRT1064)

include(driver_common_MIMXRT1064)

include(driver_clock_MIMXRT1064)

include(middleware_embedded_wizard_MIMXRT1064)

include(device_MIMXRT1064_CMSIS_MIMXRT1064)

include(component_lpuart_adapter_MIMXRT1064)

include(component_lists_MIMXRT1064)

include(driver_lpuart_MIMXRT1064)

include(device_MIMXRT1064_startup_MIMXRT1064)

include(driver_iomuxc_MIMXRT1064)

include(driver_igpio_MIMXRT1064)

include(driver_xip_device_MIMXRT1064)

include(driver_xip_board_MIMXRT1064)

include(CMSIS_Include_core_cm7_MIMXRT1064)

include(CMSIS_Include_common_MIMXRT1064)

include(CMSIS_Include_dsp_MIMXRT1064)

include(utilities_misc_utilities_MIMXRT1064)

include(device_MIMXRT1064_system_MIMXRT1064)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/EmbeddedWizard/PlatformPackage/RT/RTE/libewrte-m7-gcc.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/EmbeddedWizard/PlatformPackage/RT/RGB565/libewgfx-m7-gcc.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/EmbeddedWizard/PlatformPackage/RT/RTE/libewrte-m7-gcc.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/EmbeddedWizard/PlatformPackage/RT/RGB565/libewgfx-m7-gcc.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


