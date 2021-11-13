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


project(flashloader)

set(MCUX_SDK_PROJECT_NAME flashloader.elf)

include(${ProjDirPath}/flags.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../middleware/mcu-boot
    ${ProjDirPath}/../../../../../devices/MKE17Z7/drivers
    ${ProjDirPath}/../../../../../devices/MKE17Z7
    ${ProjDirPath}/../../../../../components/osa
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../components/lists
)

# include modules
include(middleware_mcu-boot_postaction_MKE17Z7)

include(driver_clock_MKE17Z7)

include(driver_flash_MKE17Z7)

include(device_MKE17Z7_CMSIS_MKE17Z7)

include(driver_lpuart_MKE17Z7)

include(driver_gpio_MKE17Z7)

include(driver_lpi2c_MKE17Z7)

include(driver_port_MKE17Z7)

include(driver_lpspi_MKE17Z7)

include(driver_crc_MKE17Z7)

include(driver_smc_MKE17Z7)

include(middleware_mcu-boot_MKE17Z7_startup_MKE17Z7)

include(middleware_mcu-boot_bootsources_MKE17Z7)

include(middleware_mcu-boot_bootsources_kinetis_MKE17Z7)

include(middleware_mcu-boot_MKE17Z7_sources_MKE17Z7)

include(middleware_mcu-boot_src_startup_MKE17Z7)

include(middleware_mcu-boot_drv_microseconds_MKE17Z7)

include(component_osa_bm_MKE17Z7)

include(driver_common_MKE17Z7)

include(CMSIS_Include_core_cm_MKE17Z7)

include(component_lists_MKE17Z7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/flashloader.bin)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND
python ../../../../../middleware/mcu-boot/bin/create_fl_image_mcux.py ${EXECUTABLE_OUTPUT_PATH}/flashloader.elf ${EXECUTABLE_OUTPUT_PATH}/flashloader.bin ${EXECUTABLE_OUTPUT_PATH}/flashloader_image.c
)

