include_guard(GLOBAL)
message("middleware_azure_iot_parson component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/deps/parson/parson.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/deps/parson
)


include(middleware_azure_iot_MIMXRT1052)

