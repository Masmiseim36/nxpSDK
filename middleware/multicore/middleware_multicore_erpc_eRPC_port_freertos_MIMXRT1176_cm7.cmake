include_guard(GLOBAL)
message("middleware_multicore_erpc_eRPC_port_freertos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/port/erpc_port_freertos.cpp
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/port/erpc_threading_freertos.cpp
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/port/erpc_setup_extensions_freertos.cpp
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/port
)


include(middleware_multicore_erpc_common_MIMXRT1176_cm7)

