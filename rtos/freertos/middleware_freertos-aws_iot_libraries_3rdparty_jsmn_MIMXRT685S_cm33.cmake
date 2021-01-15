if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_3RDPARTY_JSMN_MIMXRT685S_cm33_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_3RDPARTY_JSMN_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_libraries_3rdparty_jsmn component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/jsmn/jsmn.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/jsmn
    )


endif()
