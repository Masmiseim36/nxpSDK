include_guard(GLOBAL)
message("middleware_tfmtests_ns component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/app/os_wrapper_cmsis_rtos_v2.c
    ${CMAKE_CURRENT_LIST_DIR}/app/main_ns.c
    ${CMAKE_CURRENT_LIST_DIR}/app/psa_api_test.c
    ${CMAKE_CURRENT_LIST_DIR}/log/tfm_log_raw.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/app
    ${CMAKE_CURRENT_LIST_DIR}/log
)


