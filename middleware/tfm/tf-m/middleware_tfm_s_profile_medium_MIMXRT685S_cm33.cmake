include_guard()
message("middleware_tfm_s_profile_medium component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/mbedcrypto/mbedcrypto_config
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
)

