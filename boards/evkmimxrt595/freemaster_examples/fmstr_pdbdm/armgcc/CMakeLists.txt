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


project(fmstr_example_pdbdm)

set(MCUX_BUILD_TYPES debug release flash_debug flash_release)

set(MCUX_SDK_PROJECT_NAME fmstr_example_pdbdm.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../freemaster_cfg.h"
"${SdkRootDirPath}/boards/evkmimxrt595/freemaster_examples/common/fmstr_example/freemaster_example.c"
"${SdkRootDirPath}/boards/evkmimxrt595/freemaster_examples/common/fmstr_example/freemaster_example.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${SdkRootDirPath}/boards/evkmimxrt595/freemaster_examples/common/fmstr_example/data_example_pmp.h"
"${SdkRootDirPath}/boards/evkmimxrt595/freemaster_examples/common/fmstr_example/data_img_logo.h"
"${SdkRootDirPath}/boards/evkmimxrt595/freemaster_examples/common/fmstr_example/data_img_file.h"
"${SdkRootDirPath}/boards/evkmimxrt595/freemaster_examples/common/fmstr_example/data_img_fmstrlogo.h"
"${SdkRootDirPath}/boards/evkmimxrt595/freemaster_examples/common/fmstr_example/data_html_index.h"
"${SdkRootDirPath}/boards/evkmimxrt595/freemaster_examples/common/fmstr_example/example_data.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkmimxrt595/freemaster_examples/common/fmstr_example
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/middleware/freemaster
    ${SdkRootDirPath}/devices/MIMXRT595S/utilities
    ${SdkRootDirPath}/devices/MIMXRT595S/utilities/debug_console_lite
    ${SdkRootDirPath}/devices/MIMXRT595S/drivers
    ${SdkRootDirPath}/devices/MIMXRT595S
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/boards/evkmimxrt595/flash_config
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(middleware_fmstr_MIMXRT595S_cm33)

include(utility_debug_console_lite_MIMXRT595S_cm33)

include(utility_assert_lite_MIMXRT595S_cm33)

include(driver_clock_MIMXRT595S_cm33)

include(driver_common_MIMXRT595S_cm33)

include(device_MIMXRT595S_CMSIS_MIMXRT595S_cm33)

include(component_usart_adapter_MIMXRT595S_cm33)

include(driver_power_MIMXRT595S_cm33)

include(driver_flexspi_MIMXRT595S_cm33)

include(driver_cache_cache64_MIMXRT595S_cm33)

include(component_lists_MIMXRT595S_cm33)

include(driver_flexcomm_usart_MIMXRT595S_cm33)

include(driver_flash_config_evkmimxrt595_MIMXRT595S_cm33)

include(device_MIMXRT595S_startup_MIMXRT595S_cm33)

include(driver_flexcomm_MIMXRT595S_cm33)

include(driver_lpc_iopctl_MIMXRT595S_cm33)

include(driver_lpc_gpio_MIMXRT595S_cm33)

include(driver_reset_MIMXRT595S_cm33)

include(middleware_fmstr_platform_gen32le_MIMXRT595S_cm33)

include(CMSIS_Include_core_cm_MIMXRT595S_cm33)

include(utilities_misc_utilities_MIMXRT595S_cm33)

include(driver_iap_MIMXRT595S_cm33)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


