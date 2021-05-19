if(NOT MIDDLEWARE_MULTICORE_ERPC_ERPC_MU_C_WRAPPER_MIMXRT1166_cm7_INCLUDED)
    
    set(MIDDLEWARE_MULTICORE_ERPC_ERPC_MU_C_WRAPPER_MIMXRT1166_cm7_INCLUDED true CACHE BOOL "middleware_multicore_erpc_eRPC_mu_c_wrapper component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup/erpc_setup_mu.cpp
    )


    include(middleware_multicore_erpc_common_MIMXRT1166_cm7)

endif()
