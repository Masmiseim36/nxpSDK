include_guard(GLOBAL)
message("middleware_freertos-aws_iot_tests component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/unity/extras/fixture/src/unity_fixture.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/unity/src/unity.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/pkcs11/test/iot_test_pkcs11.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/secure_sockets/test/iot_test_tcp.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/wifi/test/iot_test_wifi.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/test/iot_memory_leak.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/test/iot_tests_taskpool.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/aws/greengrass/test/aws_test_helper_secure_connect.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/crypto/test/iot_test_crypto.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/tls/test/iot_test_tls.c
    ${CMAKE_CURRENT_LIST_DIR}/tests/common/aws_test.c
    ${CMAKE_CURRENT_LIST_DIR}/tests/common/aws_test_framework.c
    ${CMAKE_CURRENT_LIST_DIR}/tests/common/aws_test_runner.c
    ${CMAKE_CURRENT_LIST_DIR}/tests/common/iot_test_freertos.c
    ${CMAKE_CURRENT_LIST_DIR}/tests/common/iot_tests_network.c
    ${CMAKE_CURRENT_LIST_DIR}/tests/integration_test/core_http_system_test.c
    ${CMAKE_CURRENT_LIST_DIR}/tests/integration_test/core_mqtt_system_test.c
    ${CMAKE_CURRENT_LIST_DIR}/tests/integration_test/shadow_system_test.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/unity/extras/fixture/src
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/unity/src
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/pkcs11/test
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/wifi/test
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/test/access
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/test/mock
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/aws/greengrass/test
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/tls/test
    ${CMAKE_CURRENT_LIST_DIR}/tests/include
)


include(middleware_freertos-aws_iot_libraries_abstractions_backoff_algorithm_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_device_shadow_for_aws_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_corehttp_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_freertos_plus_aws_greengrass_MIMXRT1176_cm7)

