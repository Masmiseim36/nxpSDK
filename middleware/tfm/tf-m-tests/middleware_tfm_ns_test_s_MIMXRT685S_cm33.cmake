include_guard()
message("middleware_tfm_ns_test_s component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/common_test_services/tfm_secure_client_service/tfm_secure_client_service_api.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/common_test_services/tfm_secure_client_service
)


