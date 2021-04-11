if(NOT MIDDLEWARE_MULTICORE_ERPC_ERPC_MU_C_WRAPPER_K32L3A60_cm4_INCLUDED)
    
    set(MIDDLEWARE_MULTICORE_ERPC_ERPC_MU_C_WRAPPER_K32L3A60_cm4_INCLUDED true CACHE BOOL "middleware_multicore_erpc_eRPC_mu_c_wrapper component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup/erpc_setup_mu.cpp
    )


    include(middleware_multicore_erpc_common_K32L3A60_cm4)

endif()
