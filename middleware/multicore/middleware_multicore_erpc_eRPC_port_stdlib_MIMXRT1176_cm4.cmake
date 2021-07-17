include_guard(GLOBAL)
message("middleware_multicore_erpc_eRPC_port_stdlib component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/port/erpc_port_stdlib.cpp
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/port
)


include(middleware_multicore_erpc_common_MIMXRT1176_cm4)

