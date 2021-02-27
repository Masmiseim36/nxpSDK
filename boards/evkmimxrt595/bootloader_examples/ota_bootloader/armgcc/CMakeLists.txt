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


project(ota_bootloader)

set(MCUX_SDK_PROJECT_NAME ota_bootloader.elf)

include(${ProjDirPath}/flags.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../middleware/mcu-boot
    ${ProjDirPath}/../../../../../devices/MIMXRT595S
    ${ProjDirPath}/../../../../../devices/MIMXRT595S/drivers
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../components/osa
    ${ProjDirPath}/../../../../../CMSIS/Include
)

# include modules
include(middleware_mcu-boot_reliable_update_MIMXRT595S_cm33)

include(device_MIMXRT595S_CMSIS_MIMXRT595S_cm33)

include(driver_clock_MIMXRT595S_cm33)

include(driver_lpc_crc_MIMXRT595S_cm33)

include(driver_flexcomm_usart_MIMXRT595S_cm33)

include(middleware_mcu-boot_MIMXRT595S_startup_MIMXRT595S_cm33)

include(middleware_mcu-boot_MIMXRT595S_sources_MIMXRT595S_cm33)

include(middleware_mcu-boot_drv_microseconds_MIMXRT595S_cm33)

include(middleware_mcu-boot_mem_flexspi_nor_MIMXRT595S_cm33)

include(component_lists_MIMXRT595S_cm33)

include(component_osa_bm_MIMXRT595S_cm33)

include(CMSIS_Include_core_cm33_MIMXRT595S_cm33)

include(CMSIS_Include_common_MIMXRT595S_cm33)

include(CMSIS_Include_dsp_MIMXRT595S_cm33)

include(driver_common_MIMXRT595S_cm33)

include(driver_reset_MIMXRT595S_cm33)

include(driver_flexcomm_MIMXRT595S_cm33)

include(middleware_mcu-boot_drv_flexspi_nor_MIMXRT595S_cm33)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Osrec ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/ota_bootloader.srec)

