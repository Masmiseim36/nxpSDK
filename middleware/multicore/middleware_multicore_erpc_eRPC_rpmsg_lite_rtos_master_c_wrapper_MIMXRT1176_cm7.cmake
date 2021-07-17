include_guard(GLOBAL)
message("middleware_multicore_erpc_eRPC_rpmsg_lite_rtos_master_c_wrapper component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup/erpc_setup_rpmsg_lite_rtos_master.cpp
)


include(middleware_multicore_erpc_common_MIMXRT1176_cm7)

