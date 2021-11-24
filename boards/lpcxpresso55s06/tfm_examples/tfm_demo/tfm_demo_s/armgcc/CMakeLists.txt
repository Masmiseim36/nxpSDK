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


project(tfm_demo_s)

set(MCUX_SDK_PROJECT_NAME tfm_demo_s.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/lpcxpresso55s06/Device/Source/armgcc/startup_LPC55S06_s.S"
"${ProjDirPath}/../../../../../../devices/LPC55S06/system_LPC55S06.c"
"${ProjDirPath}/../../../../../../devices/LPC55S06/system_LPC55S06.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../hardware_init.c"
"${ProjDirPath}/../app.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../../../../../devices/LPC55S06
    ${ProjDirPath}/..
)

if(CMAKE_BUILD_TYPE STREQUAL debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="lpcxpresso55s06_mbedcrypto_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="lpcxpresso55s06_mbedcrypto_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL release)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} PRE_BUILD COMMAND
${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc -DTFM_LVL=2 -DTFM_PSA_API -DTFM_PARTITION_CRYPTO -DTFM_PARTITION_INITIAL_ATTESTATION -DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE -DTFM_PARTITION_PROTECTED_STORAGE -DTFM_PARTITION_PLATFORM -E -P -xc -I${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/lpcxpresso55s06/partition ${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/common/armgcc/tfm_common_s.ld -o ${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/common/armgcc/tfm_common_s_pre.ld
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../devices/LPC55S06/drivers
    ${ProjDirPath}/../../../../../../middleware/tfm/tf-m
    ${ProjDirPath}/../../../../../../middleware/tfm/mbed-crypto
    ${ProjDirPath}/../../../../../../devices/LPC55S06
    ${ProjDirPath}/../../../../../../devices/LPC55S06/utilities
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../CMSIS/Core/Include
)

# include modules
include(driver_casper_LPC55S06)

include(driver_hashcrypt_LPC55S06)

include(driver_rng_1_LPC55S06)

include(driver_ctimer_LPC55S06)

include(middleware_tfm_flash_iap1_LPC55S06)

include(driver_lpc_iocon_LPC55S06)

include(middleware_mbedcrypto_LPC55S06)

include(middleware_tfm_s_LPC55S06)

include(middleware_tfm_s_ipc_api_LPC55S06)

include(middleware_tfm_s_crypto_LPC55S06)

include(middleware_tfm_s_initial_attestation_LPC55S06)

include(middleware_tfm_s_its_LPC55S06)

include(middleware_tfm_s_platform_LPC55S06)

include(driver_common_LPC55S06)

include(driver_clock_LPC55S06)

include(driver_power_LPC55S06)

include(driver_reset_LPC55S06)

include(device_LPC55S06_CMSIS_LPC55S06)

include(driver_flexcomm_usart_LPC55S06)

include(driver_flexcomm_LPC55S06)

include(driver_lpc_gpio_LPC55S06)

include(utility_assert_LPC55S06)

include(utility_debug_console_LPC55S06)

include(component_usart_adapter_LPC55S06)

include(component_serial_manager_LPC55S06)

include(component_lists_LPC55S06)

include(component_serial_manager_uart_LPC55S06)

include(CMSIS_Include_core_cm_LPC55S06)

include(driver_iap1_LPC55S06)

include(middleware_tfm_memory_utils_LPC55S06)

include(utilities_misc_utilities_LPC55S06)

include(device_LPC55S06_system_LPC55S06)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


