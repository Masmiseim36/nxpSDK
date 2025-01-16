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


project(static_image_nanodet_view)

enable_language(ASM)

set(MCUX_BUILD_TYPES flash_debug flash_release)

set(MCUX_SDK_PROJECT_NAME static_image_nanodet_view.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../static_image_nanodet_view.c"
"${ProjDirPath}/../mpp_config.h"
"${ProjDirPath}/../skigirl_COCO_320_320_bgra.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../hardware_init.c"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../utick_config.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../app.h"
"${ProjDirPath}/../utick_config.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../hal_mimxrt700evk.c"
"${ProjDirPath}/../get_top_n.cpp"
"${ProjDirPath}/../utils.cpp"
"${ProjDirPath}/../nanodet_m_ops_micro_tflite.cpp"
"${ProjDirPath}/../nanodet_m_output_postproc.cpp"
"${ProjDirPath}/../get_top_n.h"
"${ProjDirPath}/../utils.h"
"${ProjDirPath}/../nanodet_labels.h"
"${ProjDirPath}/../nanodet_m_0.5x_nhwc_nopermute_npu64_tflite.h"
"${ProjDirPath}/../nanodet_m_0.5x_nhwc_nopermute_npu64_tflite_info.h"
"${ProjDirPath}/../nanodet_m_0.5x_nhwc_tflite.h"
"${ProjDirPath}/../nanodet_m_0.5x_nhwc_tflite_info.h"
"${ProjDirPath}/../nanodet_m_glow_cm7.h"
"${ProjDirPath}/../nanodet_m_output_postproc.h"
"${ProjDirPath}/../nanodet_m_weights_glow_cm7.h"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../pmic_support.c"
"${ProjDirPath}/../pmic_support.h"
"${ProjDirPath}/../display_support.h"
"${ProjDirPath}/../display_support.c"
"${ProjDirPath}/../vglite_support.h"
"${ProjDirPath}/../vglite_support.c"
"${ProjDirPath}/../mcux_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)


include(${SdkRootDirPath}/devices/MIMXRT798S/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/static_image_nanodet_view.bin)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map;${EXECUTABLE_OUTPUT_PATH}/static_image_nanodet_view.bin")

# wrap all libraries with -Wl,--start-group -Wl,--end-group to prevent link order issue
group_link_libraries()

