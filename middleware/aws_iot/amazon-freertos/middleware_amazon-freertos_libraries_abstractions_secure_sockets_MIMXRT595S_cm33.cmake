include_guard()
message("middleware_amazon-freertos_libraries_abstractions_secure_sockets component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/secure_sockets/include
)

include(middleware_amazon-freertos_libraries_c_sdk_standard_common_MIMXRT595S_cm33)

