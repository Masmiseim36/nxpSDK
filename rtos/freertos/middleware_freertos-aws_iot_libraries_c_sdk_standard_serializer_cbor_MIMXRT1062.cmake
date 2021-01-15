if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_C_SDK_STANDARD_SERIALIZER_CBOR_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_C_SDK_STANDARD_SERIALIZER_CBOR_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_libraries_c_sdk_standard_serializer_cbor component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/serializer/src/cbor/iot_serializer_tinycbor_decoder.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/serializer/src/cbor/iot_serializer_tinycbor_encoder.c
    )


endif()
