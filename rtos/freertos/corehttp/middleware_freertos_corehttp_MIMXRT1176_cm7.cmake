include_guard()
message("middleware_freertos_corehttp component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/core_http_client.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/source/include
    ${CMAKE_CURRENT_LIST_DIR}/source/interface
)


include(middleware_llhttp_MIMXRT1176_cm7)

