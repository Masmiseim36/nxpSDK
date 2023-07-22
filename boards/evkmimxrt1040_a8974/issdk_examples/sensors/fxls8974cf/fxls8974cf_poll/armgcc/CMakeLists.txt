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


project(fxls8974cf_poll)

set(MCUX_BUILD_TYPES debug release sdram_debug sdram_release flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME fxls8974cf_poll.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${SdkRootDirPath}/boards/evkmimxrt1040_a8974/evkmimxrt1040.c"
"${SdkRootDirPath}/boards/evkmimxrt1040_a8974/evkmimxrt1040.h"
"${SdkRootDirPath}/boards/evkmimxrt1040_a8974/frdm_stbi_a8974_shield.h"
"${SdkRootDirPath}/boards/evkmimxrt1040_a8974/issdk_hal.h"
"${SdkRootDirPath}/boards/evkmimxrt1040_a8974/RTE_Device.h"
"${ProjDirPath}/../fxls8974cf_poll.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${SdkRootDirPath}/boards/evkmimxrt1040_a8974
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/middleware/issdk
    ${SdkRootDirPath}/devices/MIMXRT1042/drivers
    ${SdkRootDirPath}/devices/MIMXRT1042/cmsis_drivers
    ${SdkRootDirPath}/devices/MIMXRT1042
    ${SdkRootDirPath}/devices/MIMXRT1042/utilities
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1042/xip
    ${SdkRootDirPath}/boards/evkmimxrt1040/xip
    ${SdkRootDirPath}/CMSIS/Driver/Include
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(middleware_issdk_drivers_gpio_imx_MIMXRT1042)

include(driver_clock_MIMXRT1042)

include(driver_edma_MIMXRT1042)

include(driver_dmamux_MIMXRT1042)

include(driver_lpi2c_MIMXRT1042)

include(driver_lpi2c_edma_MIMXRT1042)

include(driver_cmsis_lpi2c_MIMXRT1042)

include(driver_lpspi_MIMXRT1042)

include(driver_lpspi_edma_MIMXRT1042)

include(driver_cmsis_lpspi_MIMXRT1042)

include(driver_lpuart_MIMXRT1042)

include(driver_lpuart_edma_MIMXRT1042)

include(driver_cmsis_lpuart_MIMXRT1042)

include(middleware_issdk_drivers_systick_utils_evkmimxrt1040_MIMXRT1042)

include(middleware_issdk_sensor_interface_common_MIMXRT1042)

include(middleware_issdk_sensor_fxls8974cf_MIMXRT1042)

include(driver_common_MIMXRT1042)

include(device_MIMXRT1042_CMSIS_MIMXRT1042)

include(utility_debug_console_MIMXRT1042)

include(component_lpuart_adapter_MIMXRT1042)

include(component_serial_manager_MIMXRT1042)

include(component_lists_MIMXRT1042)

include(component_serial_manager_uart_MIMXRT1042)

include(device_MIMXRT1042_startup_MIMXRT1042)

include(driver_iomuxc_MIMXRT1042)

include(utility_assert_MIMXRT1042)

include(driver_igpio_MIMXRT1042)

include(driver_xip_device_MIMXRT1042)

include(driver_xip_board_evkmimxrt1040_MIMXRT1042)

include(middleware_issdk_drivers_cmsis_drivers_MIMXRT1042)

include(CMSIS_Include_core_cm_MIMXRT1042)

include(CMSIS_Driver_Include_I2C_MIMXRT1042)

include(CMSIS_Driver_Include_Common_MIMXRT1042)

include(CMSIS_Driver_Include_SPI_MIMXRT1042)

include(CMSIS_Driver_Include_USART_MIMXRT1042)

include(utilities_misc_utilities_MIMXRT1042)

include(device_MIMXRT1042_system_MIMXRT1042)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/MIMXRT1040-EVK-A8974-POLL.bin)

