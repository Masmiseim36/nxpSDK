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


project(arm_pelion_enet)

set(MCUX_SDK_PROJECT_NAME arm_pelion_enet.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../middleware/arm-pelion/pdmc-port/pdmc-bsp/common/flash_api.c"
"${ProjDirPath}/../../../../../middleware/arm-pelion/pdmc-port/pdmc-bsp/target/nxp/mimxrt1060/flexspi_nor_flash_ops.c"
"${ProjDirPath}/../../../../../middleware/arm-pelion/pdmc-port/pdmc-bsp/target/nxp/mimxrt1060/rt1060_flexspi_storage.c"
"${ProjDirPath}/../../../../../middleware/arm-pelion/pdmc-port/pdmc-bsp/target/nxp/mimxrt1060/flexspi_nor_flash_ops.h"
"${ProjDirPath}/../../../../../middleware/arm-pelion/mbed-cloud-client/mbed-client-pal/Source/Port/Reference-Impl/OS_Specific/NXP/Board_Specific/TARGET_RT1060/pal_plat_RT1060.c"
"${ProjDirPath}/../rtos_startup.c"
"${ProjDirPath}/../app/main.cpp"
"${ProjDirPath}/../app/mbed_cloud_dev_credentials.c"
"${ProjDirPath}/../app/update_ui_example.h"
"${ProjDirPath}/../app/update_ui_example.cpp"
"${ProjDirPath}/../app/update_default_resources.c"
"${ProjDirPath}/../app/mbed_cloud_client_user_config.h"
"${ProjDirPath}/../app/pal_config_NXP.h"
"${ProjDirPath}/../app/source/resource.cpp"
"${ProjDirPath}/../app/source/resource.h"
"${ProjDirPath}/../app/source/simplem2mclient.h"
"${ProjDirPath}/../app/source/memory_tests.cpp"
"${ProjDirPath}/../app/source/memory_tests.h"
"${ProjDirPath}/../app/source/certificate_enrollment_user_cb.cpp"
"${ProjDirPath}/../app/source/certificate_enrollment_user_cb.h"
"${ProjDirPath}/../app/source/blinky.cpp"
"${ProjDirPath}/../app/source/blinky.h"
"${ProjDirPath}/../app/source/application_init.cpp"
"${ProjDirPath}/../app/source/application_init.h"
"${ProjDirPath}/../app/source/app_platform_setup.c"
"${ProjDirPath}/../app/source/include/app_platform_setup.h"
"${ProjDirPath}/../app/source/platform/NXP/mcc_common_setup.c"
"${ProjDirPath}/../app/source/platform/NXP/mcc_common_serial.h"
"${ProjDirPath}/../app/source/platform/NXP/mcc_common_button_and_led.c"
"${ProjDirPath}/../app/source/platform/NXP/event_platform.c"
"${ProjDirPath}/../app/source/platform/pal_plat_rot_insecure.c"
"${ProjDirPath}/../app/source/platform/include/mcc_common_button_and_led.h"
"${ProjDirPath}/../app/source/platform/include/mcc_common_config.h"
"${ProjDirPath}/../app/source/platform/include/mcc_common_setup.h"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../../../../../middleware/arm-pelion/tools/combine_bootloader_with_app.py"
"${ProjDirPath}/../mbed_bootloader.bin"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/.. flexspi_nor_debug
    ${ProjDirPath}/../../../../../middleware/arm-pelion/pdmc-port/pdmc-bsp/target/nxp/mimxrt1060
    ${ProjDirPath}/../app
    ${ProjDirPath}/../app/source
    ${ProjDirPath}/../app/source/include
    ${ProjDirPath}/../app/source/platform/NXP
    ${ProjDirPath}/../app/source/platform/include
    ${ProjDirPath}/..
)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBED_CLIENT_USER_CONFIG_FILE="mbed_cloud_client_user_config.h" MBED_CLOUD_CLIENT_USER_CONFIG_FILE="mbed_cloud_client_user_config.h" PAL_USER_DEFINED_CONFIGURATION="pal_config_NXP.h" MBEDTLS_CONFIG_FILE="mbedTLSConfig_NXP.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBED_CLIENT_USER_CONFIG_FILE="mbed_cloud_client_user_config.h" MBED_CLOUD_CLIENT_USER_CONFIG_FILE="mbed_cloud_client_user_config.h" PAL_USER_DEFINED_CONFIGURATION="pal_config_NXP.h" MBEDTLS_CONFIG_FILE="mbedTLSConfig_NXP.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/phy/mdio/enet
    ${ProjDirPath}/../../../../../components/phy/device/phyksz8081
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers
    ${ProjDirPath}/../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../components/phy
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities
    ${ProjDirPath}/../../../../../middleware/mbedtls
    ${ProjDirPath}/../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../middleware/arm-pelion
    ${ProjDirPath}/../../../../../devices/MIMXRT1062
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Include
)

# include modules
include(driver_mdio-enet_MIMXRT1062)

include(driver_phy-device-ksz8081_MIMXRT1062)

include(driver_trng_MIMXRT1062)

include(driver_dcp_MIMXRT1062)

include(driver_cache_armv7_m7_MIMXRT1062)

include(driver_flexspi_MIMXRT1062)

include(driver_pit_MIMXRT1062)

include(middleware_lwip_enet_ethernetif_MIMXRT1062)

include(middleware_lwip_MIMXRT1062)

include(driver_phy-common_MIMXRT1062)

include(driver_enet_MIMXRT1062)

include(utility_debug_console_MIMXRT1062)

include(middleware_mbedtls_port_ksdk_MIMXRT1062)

include(middleware_mbedtls_rt_MIMXRT1062)

include(middleware_freertos-kernel_heap_3_MIMXRT1062)

include(middleware_mbedtls_MIMXRT1062)

include(middleware_arm_pelion_MIMXRT1062)

include(driver_clock_MIMXRT1062)

include(driver_common_MIMXRT1062)

include(device_MIMXRT1062_CMSIS_MIMXRT1062)

include(component_lpuart_adapter_MIMXRT1062)

include(component_serial_manager_MIMXRT1062)

include(component_lists_MIMXRT1062)

include(component_serial_manager_uart_MIMXRT1062)

include(driver_lpuart_MIMXRT1062)

include(device_MIMXRT1062_startup_MIMXRT1062)

include(driver_iomuxc_MIMXRT1062)

include(utility_assert_MIMXRT1062)

include(driver_igpio_MIMXRT1062)

include(driver_xip_device_MIMXRT1062)

include(driver_xip_board_MIMXRT1062)

include(driver_mdio-common_MIMXRT1062)

include(CMSIS_Include_core_cm7_MIMXRT1062)

include(CMSIS_Include_common_MIMXRT1062)

include(CMSIS_Include_dsp_MIMXRT1062)

include(middleware_freertos-kernel_MIMXRT1062)

include(middleware_freertos-kernel_extension_MIMXRT1062)

include(utilities_misc_utilities_MIMXRT1062)

include(device_MIMXRT1062_system_MIMXRT1062)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/arm_pelion_enet.bin)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND
python ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../middleware/arm-pelion/tools/combine_bootloader_with_app.py -b ${CMAKE_CURRENT_SOURCE_DIR}/../mbed_bootloader.bin -a ${EXECUTABLE_OUTPUT_PATH}/arm_pelion_enet.bin -o ${EXECUTABLE_OUTPUT_PATH}/arm_pelion_combined.bin -c 0x10400 -d 0x10000
)

