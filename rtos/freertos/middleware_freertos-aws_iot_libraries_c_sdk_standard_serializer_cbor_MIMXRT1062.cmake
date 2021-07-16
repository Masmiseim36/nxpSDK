include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_c_sdk_standard_serializer_cbor component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/serializer/src/cbor/iot_serializer_tinycbor_decoder.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/serializer/src/cbor/iot_serializer_tinycbor_encoder.c
)


