include_guard(GLOBAL)
message("middleware_multicore_erpc_eRPC_client component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra/erpc_client_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup/erpc_client_setup.cpp
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup
)


include(middleware_multicore_erpc_common_MIMXRT1176_cm7)

