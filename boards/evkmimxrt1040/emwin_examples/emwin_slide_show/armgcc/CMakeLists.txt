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


project(emwin_slide_show)

enable_language(ASM)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_debug flexspi_nor_sdram_release)

set(MCUX_SDK_PROJECT_NAME emwin_slide_show.elf)

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
"${ProjDirPath}/../emwin_slide_show.c"
"${ProjDirPath}/../Alarm.c"
"${ProjDirPath}/../Info.c"
"${ProjDirPath}/../Weather.c"
"${ProjDirPath}/../Resource.h"
"${ProjDirPath}/../bmNXP_110x44.c"
"${ProjDirPath}/../bmSeggerLogo_110x55.c"
"${ProjDirPath}/../bmBell_40x40.c"
"${ProjDirPath}/../bmBell_60x60.c"
"${ProjDirPath}/../bmArrowLeft_23x24.c"
"${ProjDirPath}/../bmArrowRight_23x24.c"
"${ProjDirPath}/../bmRainy_45x34.c"
"${ProjDirPath}/../bmStormy_45x34.c"
"${ProjDirPath}/../bmSunny_45x34.c"
"${ProjDirPath}/../AA4_B_20.c"
"${ProjDirPath}/../AA4_B_40.c"
"${ProjDirPath}/../AA4_B_100.c"
"${ProjDirPath}/../Font_13_R.c"
"${ProjDirPath}/../emwin_support.c"
"${ProjDirPath}/../emwin_support.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../hardware_init.c"
"${ProjDirPath}/../app.h"
"${ProjDirPath}/../mcux_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)


include(${SdkRootDirPath}/devices/MIMXRT1042/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/emwin_slide_show.bin)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map;${EXECUTABLE_OUTPUT_PATH}/emwin_slide_show.bin")

# wrap all libraries with -Wl,--start-group -Wl,--end-group to prevent link order issue
group_link_libraries()

