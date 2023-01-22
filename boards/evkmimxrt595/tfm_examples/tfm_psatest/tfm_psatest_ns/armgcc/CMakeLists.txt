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


project(tfm_psatest_ns)

set(MCUX_BUILD_TYPES debug release)

set(MCUX_SDK_PROJECT_NAME tfm_psatest_ns.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/evkmimxrt595/Device/Source/armgcc/startup_MIMXRT595S_cm33_ns.S"
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

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} PRE_BUILD COMMAND
${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc -E -P -xc -I${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/evkmimxrt595/partition ${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/common/armgcc/tfm_common_ns.ld -o ${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/common/armgcc/tfm_common_ns_pre.ld
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/middleware/tfm/tf-m
    ${SdkRootDirPath}/middleware/tfm/psa-arch-tests/api-tests
    ${SdkRootDirPath}/devices/MIMXRT595S/drivers
    ${SdkRootDirPath}/devices/MIMXRT595S
    ${SdkRootDirPath}/devices/MIMXRT595S/utilities
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/boards/evkmimxrt595/flash_config
    ${SdkRootDirPath}/middleware/tfm/tf-m-tests
    ${SdkRootDirPath}/CMSIS/RTOS2/RTX/Library
    ${SdkRootDirPath}/CMSIS/RTOS2
    ${SdkRootDirPath}/CMSIS/RTOS2/Include
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(middleware_tfm_ns_evkmimxrt595_MIMXRT595S_cm33)

include(middleware_tfm_ns_ipc_api_MIMXRT595S_cm33)

include(middleware_psatest_ns_MIMXRT595S_cm33)

include(middleware_psatest_ns_initial_attestation_1_MIMXRT595S_cm33)

include(middleware_psatest_ns_initial_attestation_2_MIMXRT595S_cm33)

include(middleware_psatest_ns_crypto_MIMXRT595S_cm33)

include(middleware_psatest_ns_internal_trusted_storage_MIMXRT595S_cm33)

include(middleware_psatest_ns_protected_storage_MIMXRT595S_cm33)

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

include(middleware_tfm_ns_MIMXRT595S_cm33)

include(middleware_tfmtests_ns_MIMXRT595S_cm33)

include(CMSIS_RTOS2_NonSecure_MIMXRT595S_cm33)

include(CMSIS_RTOS2_Common_MIMXRT595S_cm33)

include(CMSIS_Device_API_OSTick_MIMXRT595S_cm33)

include(CMSIS_Device_API_RTOS2_MIMXRT595S_cm33)

include(CMSIS_Include_core_cm_MIMXRT595S_cm33)

include(utilities_misc_utilities_MIMXRT595S_cm33)

include(driver_iap_MIMXRT595S_cm33)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../tfm_psatest_s/armgcc/${CMAKE_BUILD_TYPE}/tfm_psatest_s_CMSE_lib.o)
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../CMSIS/RTOS2/RTX/Library/GCC/libRTX_V8MMFN.a)
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../tfm_psatest_s/armgcc/${CMAKE_BUILD_TYPE}/tfm_psatest_s_CMSE_lib.o)
endif(CMAKE_BUILD_TYPE STREQUAL release)

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../CMSIS/RTOS2/RTX/Library/GCC/libRTX_V8MMFN.a)
endif(CMAKE_BUILD_TYPE STREQUAL release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

