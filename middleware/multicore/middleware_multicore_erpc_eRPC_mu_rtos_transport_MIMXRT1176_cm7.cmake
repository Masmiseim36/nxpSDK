include_guard(GLOBAL)
message("middleware_multicore_erpc_eRPC_mu_rtos_transport component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/transports/erpc_mu_transport.cpp
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/port/erpc_threading_freertos.cpp
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup/erpc_setup_mbf_dynamic.cpp
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/transports
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/port
)


include(middleware_multicore_erpc_common_MIMXRT1176_cm7)

