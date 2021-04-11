if(NOT MIDDLEWARE_MULTICORE_MCMGR_K32L3A6_K32L3A60_cm0plus_INCLUDED)
    
    set(MIDDLEWARE_MULTICORE_MCMGR_K32L3A6_K32L3A60_cm0plus_INCLUDED true CACHE BOOL "middleware_multicore_mcmgr_k32l3a6 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/mcmgr/src/mcmgr_internal_core_api_k32l3a6.c
        ${CMAKE_CURRENT_LIST_DIR}/mcmgr/src/mcmgr_mu_internal.c
    )


    include(middleware_multicore_mcmgr_K32L3A60_cm0plus)

    include(driver_mu_K32L3A60_cm0plus)

endif()
