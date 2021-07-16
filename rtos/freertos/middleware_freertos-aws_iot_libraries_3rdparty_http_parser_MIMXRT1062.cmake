include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_3rdparty_http_parser component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreHTTP/source/dependency/3rdparty/http_parser/http_parser.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreHTTP/source/dependency/3rdparty/http_parser
)


