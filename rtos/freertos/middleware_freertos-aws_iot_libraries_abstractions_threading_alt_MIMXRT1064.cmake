if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_ABSTRACTIONS_THREADING_ALT_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_ABSTRACTIONS_THREADING_ALT_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_libraries_abstractions_threading_alt component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/pkcs11/mbedtls
    )

    include(middleware_freertos-kernel_MIMXRT1064)

endif()
