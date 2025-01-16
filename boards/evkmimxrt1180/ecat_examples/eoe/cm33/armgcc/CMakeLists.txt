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


project(ecat_eoe_cm33)

enable_language(ASM)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME ecat_eoe_cm33.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../eoe_freertos.c"
"${ProjDirPath}/../ecat_hw.c"
"${ProjDirPath}/../ecat_hw.h"
"${ProjDirPath}/../eoe_lwip_interface.c"
"${ProjDirPath}/../eoe_lwip_interface.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../httpsrv_freertos.c"
"${ProjDirPath}/../httpsrv_freertos.h"
"${ProjDirPath}/../httpsrv_fs_data.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../favicon.ico"
"${ProjDirPath}/../NXP_logo.png"
"${ProjDirPath}/../request.js"
"${ProjDirPath}/../ssi.shtml"
"${ProjDirPath}/../SSC/eoe_freertos.xlsx"
"${ProjDirPath}/../SSC/eoe.xml"
"${ProjDirPath}/../SSC/Src/applInterface.h"
"${ProjDirPath}/../SSC/Src/coeappl.c"
"${ProjDirPath}/../SSC/Src/coeappl.h"
"${ProjDirPath}/../SSC/Src/ecat_def.h"
"${ProjDirPath}/../SSC/Src/ecatappl.c"
"${ProjDirPath}/../SSC/Src/ecatappl.h"
"${ProjDirPath}/../SSC/Src/ecatcoe.c"
"${ProjDirPath}/../SSC/Src/ecatcoe.h"
"${ProjDirPath}/../SSC/Src/ecateoe.c"
"${ProjDirPath}/../SSC/Src/ecateoe.h"
"${ProjDirPath}/../SSC/Src/ecatslv.c"
"${ProjDirPath}/../SSC/Src/ecatslv.h"
"${ProjDirPath}/../SSC/Src/eoe_freertos.h"
"${ProjDirPath}/../SSC/Src/eoe_freertosObjects.h"
"${ProjDirPath}/../SSC/Src/eoeappl.c"
"${ProjDirPath}/../SSC/Src/eoeappl.h"
"${ProjDirPath}/../SSC/Src/esc.h"
"${ProjDirPath}/../SSC/Src/mailbox.c"
"${ProjDirPath}/../SSC/Src/mailbox.h"
"${ProjDirPath}/../SSC/Src/objdef.c"
"${ProjDirPath}/../SSC/Src/objdef.h"
"${ProjDirPath}/../SSC/Src/sdoserv.c"
"${ProjDirPath}/../SSC/Src/sdoserv.h"
"${ProjDirPath}/../FreeRTOSConfigBoard.h"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../hardware_init.c"
"${ProjDirPath}/../app.h"
"${ProjDirPath}/../FreeRTOSConfig_Gen.h"
"${ProjDirPath}/../mcux_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../SSC
    ${ProjDirPath}/../SSC/Src
)

set_source_files_properties("${ProjDirPath}/../FreeRTOSConfig.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_config")
set_source_files_properties("${ProjDirPath}/../FreeRTOSConfig_Gen.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_config")

include(${SdkRootDirPath}/devices/MIMXRT1189/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/ecat_eoe.bin)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map;${EXECUTABLE_OUTPUT_PATH}/ecat_eoe.bin")

# wrap all libraries with -Wl,--start-group -Wl,--end-group to prevent link order issue
group_link_libraries()

