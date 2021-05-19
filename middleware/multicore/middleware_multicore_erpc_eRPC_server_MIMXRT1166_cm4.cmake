if(NOT MIDDLEWARE_MULTICORE_ERPC_ERPC_SERVER_MIMXRT1166_cm4_INCLUDED)
    
    set(MIDDLEWARE_MULTICORE_ERPC_ERPC_SERVER_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "middleware_multicore_erpc_eRPC_server component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra/erpc_simple_server.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra/erpc_server.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup/erpc_server_setup.cpp
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup
    )


    include(middleware_multicore_erpc_common_MIMXRT1166_cm4)

endif()
