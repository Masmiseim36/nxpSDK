include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_abstractions_backoff_algorithm component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/backoff_algorithm/source/backoff_algorithm.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/backoff_algorithm/source/include
)


