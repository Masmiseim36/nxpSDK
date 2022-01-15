include_guard(GLOBAL)
message("middleware_se_hostlib_commonCloudDemos_evkmimxrt1170 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/ksdk/common/ledHandler.c
    ${CMAKE_CURRENT_LIST_DIR}/demos/ksdk/common/boards/evkmimxrt1170/se_hostlib_examples/cloud_demo/pin_mux.c
    ${CMAKE_CURRENT_LIST_DIR}/demos/ksdk/common/freertos/FreeRTOSConfig.c
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_demo_utils.c
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_demo_utils_rtos.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11/aws_pkcs11_pal.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11/pkcs11_mbedtls_utils.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/ksdk/common/freertos/boards/evkmimxrt1170
    ${CMAKE_CURRENT_LIST_DIR}/demos/ksdk/common
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/inc
    ${CMAKE_CURRENT_LIST_DIR}/demos/ksdk/common/boards/evkmimxrt1170/se_hostlib_examples/cloud_demo
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11
    ${CMAKE_CURRENT_LIST_DIR}/demos/ksdk/common/https_client
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/tstUtil
)


include(middleware_freertos-kernel_heap_4_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_tls_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_utils_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_secure_sockets_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_coremqtt_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_3rdparty_pkcs11_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_3rdparty_jsmn_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_logging_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_pkcs11_helpers_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_backoff_algorithm_MIMXRT1176_cm7)

