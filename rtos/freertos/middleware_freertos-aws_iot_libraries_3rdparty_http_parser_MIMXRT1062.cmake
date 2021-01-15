if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_3RDPARTY_HTTP_PARSER_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_3RDPARTY_HTTP_PARSER_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_libraries_3rdparty_http_parser component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/http_parser/http_parser.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/http_parser
    )


endif()
