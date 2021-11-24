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

set(MCUX_SDK_PROJECT_NAME tfm_psatest_ns.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/lpcxpresso55s06/Device/Source/armgcc/startup_LPC55S06_ns.S"
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

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} PRE_BUILD COMMAND
${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc -E -P -xc -I${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/lpcxpresso55s06/partition ${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/common/armgcc/tfm_common_ns.ld -o ${ProjDirPath}/../../../../../../middleware/tfm/tf-m/platform/ext/target/nxp/common/armgcc/tfm_common_ns_pre.ld
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../devices/LPC55S06/drivers
    ${ProjDirPath}/../../../../../../middleware/tfm/tf-m
    ${ProjDirPath}/../../../../../../middleware/tfm/psa-arch-tests/api-tests
    ${ProjDirPath}/../../../../../../devices/LPC55S06
    ${ProjDirPath}/../../../../../../devices/LPC55S06/utilities
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../../middleware/tfm/tf-m-tests
    ${ProjDirPath}/../../../../../../CMSIS/RTOS2/RTX/Library
    ${ProjDirPath}/../../../../../../CMSIS/RTOS2
    ${ProjDirPath}/../../../../../../CMSIS/RTOS2/Include
)

# include modules
include(driver_lpc_iocon_LPC55S06)

include(middleware_tfm_ns_LPC55S06)

include(middleware_tfm_ns_rtx_LPC55S06)

include(middleware_tfm_ns_ipc_api_LPC55S06)

include(middleware_psatest_ns_LPC55S06)

include(middleware_psatest_ns_initial_attestation_1_LPC55S06)

include(middleware_psatest_ns_initial_attestation_2_LPC55S06)

include(middleware_psatest_ns_crypto_LPC55S06)

include(middleware_psatest_ns_internal_trusted_storage_LPC55S06)

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

include(middleware_tfmtests_ns_LPC55S06)

include(CMSIS_RTOS2_NonSecure_LPC55S06)

include(CMSIS_RTOS2_Common_LPC55S06)

include(CMSIS_Device_API_OSTick_LPC55S06)

include(CMSIS_Device_API_RTOS2_LPC55S06)

include(utilities_misc_utilities_LPC55S06)

include(device_LPC55S06_system_LPC55S06)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

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


