include_guard()
message("middleware_tfm_ns component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/uart_stdout.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/CMSIS_Driver/Driver_USART.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/platform/include
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/driver
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/Device/Config
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/Device/Include
    ${CMAKE_CURRENT_LIST_DIR}/interface/include
    ${CMAKE_CURRENT_LIST_DIR}/interface/include/crypto_keys
    ${CMAKE_CURRENT_LIST_DIR}/interface/include/psa
    ${CMAKE_CURRENT_LIST_DIR}/bl2/include
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/include/tfm
)


include(middleware_tfmtests_ns_MIMXRT595S_cm33)

include(CMSIS_RTOS2_NonSecure_MIMXRT595S_cm33)

