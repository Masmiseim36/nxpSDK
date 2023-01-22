include_guard()
message("middleware_aws_iot_device_shadow component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/shadow.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/source/include
    ${CMAKE_CURRENT_LIST_DIR}/source/interface
)


