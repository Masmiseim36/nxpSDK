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


project(fmstr_example_net)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME fmstr_example_net.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../source/main.c"
"${ProjDirPath}/../source/board/clock_config.c"
"${ProjDirPath}/../source/board/clock_config.h"
"${ProjDirPath}/../source/board/pin_mux.c"
"${ProjDirPath}/../source/board/pin_mux.h"
"${ProjDirPath}/../source/freemaster_cfg.h"
"${ProjDirPath}/../source/freemaster_example.c"
"${ProjDirPath}/../source/freemaster_example.h"
"${ProjDirPath}/../source/network.c"
"${ProjDirPath}/../source/network.h"
"${ProjDirPath}/../source/FreeRTOSConfig.h"
"${ProjDirPath}/../source/board/FreeRTOSConfigBoard.h"
"${ProjDirPath}/../source/lwipopts.h"
"${ProjDirPath}/../source/network_enet.c"
"${ProjDirPath}/../source/board/board.c"
"${ProjDirPath}/../source/board/board.h"
"${ProjDirPath}/../source/example_data/data_example_pmp.h"
"${ProjDirPath}/../source/example_data/data_img_logo.h"
"${ProjDirPath}/../source/example_data/data_img_file.h"
"${ProjDirPath}/../source/example_data/data_img_fmstrlogo.h"
"${ProjDirPath}/../source/example_data/data_html_index.h"
"${ProjDirPath}/../source/example_data/example_data.h"
"${ProjDirPath}/../source/board/dcd.c"
"${ProjDirPath}/../source/board/dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../source
    ${ProjDirPath}/../source/board
    ${ProjDirPath}/../source/example_data
)

set_source_files_properties("${ProjDirPath}/../source/FreeRTOSConfig.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_template")
set_source_files_properties("${ProjDirPath}/../source/lwipopts.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_lwip_template")

include(${SdkRootDirPath}/devices/MIMXRT1062/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map")

