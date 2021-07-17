include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_3rdparty_pkcs11 component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/pkcs11
)

