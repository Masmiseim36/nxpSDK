if(NOT MIDDLEWARE_MULTICORE_ERPC_ERPC_RPMSG_TTY_RTOS_REMOTE_C_WRAPPER_MCIMX7U5_INCLUDED)
    
    set(MIDDLEWARE_MULTICORE_ERPC_ERPC_RPMSG_TTY_RTOS_REMOTE_C_WRAPPER_MCIMX7U5_INCLUDED true CACHE BOOL "middleware_multicore_erpc_eRPC_rpmsg_tty_rtos_remote_c_wrapper component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup/erpc_setup_rpmsg_tty_rtos_remote.cpp
    )


    include(middleware_multicore_erpc_common_MCIMX7U5)

endif()
