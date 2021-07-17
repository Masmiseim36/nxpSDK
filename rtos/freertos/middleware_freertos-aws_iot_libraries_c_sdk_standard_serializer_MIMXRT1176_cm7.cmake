include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_c_sdk_standard_serializer component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/serializer/src/iot_json_utils.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/serializer/src/iot_serializer_static_memory.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/serializer/src/json/iot_serializer_json_decoder.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/serializer/src/json/iot_serializer_json_encoder.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/serializer/include
)


include(middleware_freertos-aws_iot_libraries_c_sdk_standard_common_MIMXRT1176_cm7)

include(middleware_mbedtls_MIMXRT1176_cm7)

