include_guard(GLOBAL)
message("middleware_tfm_ns component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/uart_stdout.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/CMSIS_Driver/Driver_USART.c
    ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_ns_interface.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/include
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/driver
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/lpcxpresso55s06/Device/Include
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/lpcxpresso55s06
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/lpcxpresso55s06/partition
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/lpcxpresso55s06/Device/Config
    ${CMAKE_CURRENT_LIST_DIR}/interface/include
    ${CMAKE_CURRENT_LIST_DIR}/interface/include/os_wrapper
    ${CMAKE_CURRENT_LIST_DIR}/interface/include/psa
    ${CMAKE_CURRENT_LIST_DIR}/bl2/include
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/include/tfm
)


include(middleware_tfmtests_ns_LPC55S06)

