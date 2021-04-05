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


project(asrc_p2p_out_sdma)

set(MCUX_SDK_PROJECT_NAME asrc_p2p_out_sdma.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../asrc_p2p_out_sdma.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../music.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../fsl_iomuxc.h"
"${ProjDirPath}/../empty_rsc_table.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../components/codec/wm8524
    ${ProjDirPath}/../../../../../../devices/MIMX8MN6/drivers
    ${ProjDirPath}/../../../../../../components/codec
    ${ProjDirPath}/../../../../../../components/codec/port/wm8524
    ${ProjDirPath}/../../../../../../devices/MIMX8MN6/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../../devices/MIMX8MN6
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../devices/MIMX8MN6/utilities
)

# include modules
include(driver_wm8524_MIMX8MN6)

include(driver_igpio_MIMX8MN6)

include(driver_codec_MIMX8MN6)

include(driver_common_MIMX8MN6)

include(component_wm8524_adapter_MIMX8MN6)

include(utility_debug_console_lite_MIMX8MN6)

include(utility_assert_lite_MIMX8MN6)

include(driver_clock_MIMX8MN6)

include(driver_sai_MIMX8MN6)

include(driver_sai_sdma_MIMX8MN6)

include(driver_sdma_MIMX8MN6)

include(driver_easrc_MIMX8MN6)

include(driver_easrc_sdma_MIMX8MN6)

include(driver_rdc_MIMX8MN6)

include(device_MIMX8MN6_CMSIS_MIMX8MN6)

include(component_iuart_adapter_MIMX8MN6)

include(component_lists_MIMX8MN6)

include(driver_iuart_MIMX8MN6)

include(device_MIMX8MN6_startup_MIMX8MN6)

include(CMSIS_Include_core_cm7_MIMX8MN6)

include(CMSIS_Include_common_MIMX8MN6)

include(CMSIS_Include_dsp_MIMX8MN6)

include(driver_memory_MIMX8MN6)

include(utilities_misc_utilities_MIMX8MN6)

include(device_MIMX8MN6_system_MIMX8MN6)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/easrc_p2p_out_sdma.bin)

