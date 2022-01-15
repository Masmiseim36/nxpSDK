include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_corejason component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreJSON/source/core_json.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreJSON/source/include
)


