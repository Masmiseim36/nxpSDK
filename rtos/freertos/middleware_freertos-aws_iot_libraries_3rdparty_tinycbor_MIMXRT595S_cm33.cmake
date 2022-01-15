include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_3rdparty_tinycbor component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/tinycbor/src/cborencoder.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/tinycbor/src/cborencoder_close_container_checked.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/tinycbor/src/cborerrorstrings.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/tinycbor/src/cborparser.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/tinycbor/src/cborparser_dup_string.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/tinycbor/src/cborvalidation.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/tinycbor/src
)


