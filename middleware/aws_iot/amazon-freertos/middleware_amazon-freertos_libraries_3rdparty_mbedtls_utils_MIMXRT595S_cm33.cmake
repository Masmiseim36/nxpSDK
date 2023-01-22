include_guard()
message("middleware_amazon-freertos_libraries_3rdparty_mbedtls_utils component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/mbedtls_utils/mbedtls_error.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/mbedtls_utils/mbedtls_utils.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/libraries/3rdparty/mbedtls_utils
)


include(middleware_mbedtls_MIMXRT595S_cm33)

