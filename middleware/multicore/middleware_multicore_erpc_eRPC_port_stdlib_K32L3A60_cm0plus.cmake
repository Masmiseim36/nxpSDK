if(NOT MIDDLEWARE_MULTICORE_ERPC_ERPC_PORT_STDLIB_K32L3A60_cm0plus_INCLUDED)
    
    set(MIDDLEWARE_MULTICORE_ERPC_ERPC_PORT_STDLIB_K32L3A60_cm0plus_INCLUDED true CACHE BOOL "middleware_multicore_erpc_eRPC_port_stdlib component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/port/erpc_port_stdlib.cpp
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/port
    )


    include(middleware_multicore_erpc_common_K32L3A60_cm0plus)

endif()
