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


project(lvgl_guider_bm)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_debug flexspi_nor_sdram_release)

set(MCUX_SDK_PROJECT_NAME lvgl_guider_bm.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../lvgl_guider_bm.c"
"${ProjDirPath}/../lv_conf.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../generated/images/images.c"
"${ProjDirPath}/../generated/guider_fonts/guider_fonts.h"
"${ProjDirPath}/../generated/guider_customer_fonts/guider_customer_fonts.h"
"${ProjDirPath}/../generated/gui_guider.c"
"${ProjDirPath}/../generated/gui_guider.h"
"${ProjDirPath}/../generated/events_init.c"
"${ProjDirPath}/../generated/events_init.h"
"${ProjDirPath}/../custom/custom.c"
"${ProjDirPath}/../custom/custom.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../lvgl_support.h"
"${ProjDirPath}/../lvgl_support.c"
"${SdkRootDirPath}/boards/evkbmimxrt1060/lvgl_examples/lvgl_demo_utils.h"
"${SdkRootDirPath}/boards/evkbmimxrt1060/lvgl_examples/lvgl_demo_utils.c"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../generated
    ${ProjDirPath}/../custom
    ${ProjDirPath}/../generated/guider_customer_fonts
    ${ProjDirPath}/../generated/guider_fonts
    ${SdkRootDirPath}/boards/evkbmimxrt1060/lvgl_examples
)

set_source_files_properties("${ProjDirPath}/../lv_conf.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_lvgl_template")
set_source_files_properties("${ProjDirPath}/../lvgl_support.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_lvgl_template")
set_source_files_properties("${ProjDirPath}/../lvgl_support.c" PROPERTIES COMPONENT_CONFIG_FILE "middleware_lvgl_template")

include(${SdkRootDirPath}/devices/MIMXRT1062/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map")

