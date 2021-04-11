if(NOT MIDDLEWARE_MULTICORE_ERPC_ERPC_CLIENT_K32L2B31A_INCLUDED)
    
    set(MIDDLEWARE_MULTICORE_ERPC_ERPC_CLIENT_K32L2B31A_INCLUDED true CACHE BOOL "middleware_multicore_erpc_eRPC_client component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra/erpc_client_manager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup/erpc_client_setup.cpp
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup
    )


    include(middleware_multicore_erpc_common_K32L2B31A)

endif()
