include_guard()
message("middleware_multicore_erpc_eRPC_mu_c_wrapper component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup/erpc_setup_mu.cpp
)


include(middleware_multicore_erpc_common_MIMXRT1176_cm7)

