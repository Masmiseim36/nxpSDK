include_guard()
message("middleware_amazon-freertos_libraries_3rdparty_jsmn component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/jsmn/jsmn.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/jsmn
)


