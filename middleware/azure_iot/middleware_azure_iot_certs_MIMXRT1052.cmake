include_guard(GLOBAL)
message("middleware_azure_iot_certs component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/certs/certs_azure.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/certs
)


include(middleware_azure_iot_MIMXRT1052)

