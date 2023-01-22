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


project(tfm_regression_s)

set(MCUX_BUILD_TYPES debug release)

set(MCUX_SDK_PROJECT_NAME tfm_regression_s.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/evkmimxrt595/Device/Source/armgcc/startup_MIMXRT595S_cm33_s.S"
"${SdkRootDirPath}/devices/MIMXRT595S/system_MIMXRT595S_cm33.c"
"${SdkRootDirPath}/devices/MIMXRT595S/system_MIMXRT595S_cm33.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../hardware_init.c"
"${ProjDirPath}/../app.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${SdkRootDirPath}/devices/MIMXRT595S
    ${ProjDirPath}/..
)

if(CMAKE_BUILD_TYPE STREQUAL debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="tfm_mbedcrypto_config_profile_medium.h" MBEDTLS_PSA_CRYPTO_CONFIG_FILE="crypto_config_profile_medium.h")
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="tfm_mbedcrypto_config_profile_medium.h" MBEDTLS_PSA_CRYPTO_CONFIG_FILE="crypto_config_profile_medium.h")
endif(CMAKE_BUILD_TYPE STREQUAL release)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} PRE_BUILD COMMAND
${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc -DTFM_LVL=2 -DTFM_PSA_API -DTFM_PARTITION_TEST_SECURE_SERVICES -DCONFIG_TFM_PARTITION_META -DENABLE_HEAP -D__ARM_ARCH_8M_MAIN__ -DCONFIG_TFM_USE_TRUSTZONE -E -P -xc -I${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/evkmimxrt595/partition ${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/common/armgcc/tfm_common_s.ld -o ${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/common/armgcc/tfm_common_s_pre.ld
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT595S/drivers
    ${SdkRootDirPath}/middleware/tfm/tf-m
    ${SdkRootDirPath}/middleware/tfm/mbed-crypto
    ${SdkRootDirPath}/middleware/tfm/tf-m-tests
    ${SdkRootDirPath}/devices/MIMXRT595S
    ${SdkRootDirPath}/devices/MIMXRT595S/utilities
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/boards/evkmimxrt595/flash_config
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(driver_casper_MIMXRT595S_cm33)

include(driver_hashcrypt_MIMXRT595S_cm33)

include(driver_trng_MIMXRT595S_cm33)

include(driver_ctimer_MIMXRT595S_cm33)

include(middleware_tfm_flash_iap_MIMXRT595S_cm33)

include(middleware_mbedcrypto_MIMXRT595S_cm33)

include(middleware_tfm_s_evkmimxrt595_MIMXRT595S_cm33)

include(middleware_tfm_s_its_MIMXRT595S_cm33)

include(middleware_tfm_s_crypto_MIMXRT595S_cm33)

include(middleware_tfm_s_initial_attestation_MIMXRT595S_cm33)

include(middleware_tfm_s_platform_MIMXRT595S_cm33)

include(middleware_tfm_s_test_MIMXRT595S_cm33)

include(middleware_tfm_s_test_its_MIMXRT595S_cm33)

include(middleware_tfm_s_test_attestation_MIMXRT595S_cm33)

include(middleware_tfm_s_test_crypto_MIMXRT595S_cm33)

include(middleware_tfm_s_test_platform_MIMXRT595S_cm33)

include(middleware_tfm_s_ipc_api_MIMXRT595S_cm33)

include(middleware_tfm_s_test_ipc_MIMXRT595S_cm33)

include(middleware_tfm_s_test_core_MIMXRT595S_cm33)

include(middleware_tfm_s_ps_MIMXRT595S_cm33)

include(middleware_tfm_s_test_ps_MIMXRT595S_cm33)

include(middleware_tfm_s_profile_medium_MIMXRT595S_cm33)

include(middleware_tfm_s_test_irq_slih_MIMXRT595S_cm33)

include(driver_common_MIMXRT595S_cm33)

include(driver_clock_MIMXRT595S_cm33)

include(driver_power_MIMXRT595S_cm33)

include(driver_reset_MIMXRT595S_cm33)

include(device_MIMXRT595S_CMSIS_MIMXRT595S_cm33)

include(utility_debug_console_MIMXRT595S_cm33)

include(utility_assert_MIMXRT595S_cm33)

include(component_usart_adapter_MIMXRT595S_cm33)

include(driver_flexspi_MIMXRT595S_cm33)

include(driver_cache_cache64_MIMXRT595S_cm33)

include(component_serial_manager_MIMXRT595S_cm33)

include(component_lists_MIMXRT595S_cm33)

include(component_serial_manager_uart_MIMXRT595S_cm33)

include(driver_flexcomm_usart_MIMXRT595S_cm33)

include(driver_flash_config_evkmimxrt595_MIMXRT595S_cm33)

include(driver_flexcomm_MIMXRT595S_cm33)

include(driver_lpc_iopctl_MIMXRT595S_cm33)

include(driver_lpc_gpio_MIMXRT595S_cm33)

include(CMSIS_Include_core_cm_MIMXRT595S_cm33)

include(driver_iap_MIMXRT595S_cm33)

include(middleware_tfm_s_MIMXRT595S_cm33)

include(middleware_tfm_t_cose_sign1_verify_MIMXRT595S_cm33)

include(middleware_tfm_platform_test_MIMXRT595S_cm33)

include(utilities_misc_utilities_MIMXRT595S_cm33)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

