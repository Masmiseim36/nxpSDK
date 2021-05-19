if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_3RDPARTY_PKCS11_MIMXRT1166_cm7_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_3RDPARTY_PKCS11_MIMXRT1166_cm7_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_libraries_3rdparty_pkcs11 component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/pkcs11
    )

endif()
