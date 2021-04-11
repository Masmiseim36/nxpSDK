if(NOT MIDDLEWARE_MULTICORE_ERPC_ERPC_UART_CMSIS_TRANSPORT_K32L2B31A_INCLUDED)
    
    set(MIDDLEWARE_MULTICORE_ERPC_ERPC_UART_CMSIS_TRANSPORT_K32L2B31A_INCLUDED true CACHE BOOL "middleware_multicore_erpc_eRPC_uart_cmsis_transport component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/transports/erpc_uart_cmsis_transport.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra/erpc_framed_transport.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup/erpc_setup_uart_cmsis.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup/erpc_setup_mbf_dynamic.cpp
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/transports
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra
    )


    include(middleware_multicore_erpc_common_K32L2B31A)

endif()
