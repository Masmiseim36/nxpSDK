if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_ABSTRACTIONS_SECURE_SOCKETS_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_ABSTRACTIONS_SECURE_SOCKETS_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_libraries_abstractions_secure_sockets component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/secure_sockets/include
    )

    include(middleware_freertos-aws_iot_common_MIMXRT1062)

endif()
