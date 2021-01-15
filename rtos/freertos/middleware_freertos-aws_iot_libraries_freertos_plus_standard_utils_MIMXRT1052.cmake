if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_FREERTOS_PLUS_STANDARD_UTILS_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_FREERTOS_PLUS_STANDARD_UTILS_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_libraries_freertos_plus_standard_utils component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/utils/src/iot_pki_utils.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/utils/src/iot_system_init.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/utils/include
    )


    include(middleware_freertos-kernel_MIMXRT1052)

    include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto_MIMXRT1052)

    include(middleware_freertos-aws_iot_libraries_abstractions_secure_sockets_MIMXRT1052)

endif()
