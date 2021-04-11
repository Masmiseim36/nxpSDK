if(NOT COMPONENT_OSA_BM_K32L2B31A_INCLUDED)
    
    set(COMPONENT_OSA_BM_K32L2B31A_INCLUDED true CACHE BOOL "component_osa_bm component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_os_abstraction_bm.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_K32L2B31A)

    include(component_lists_K32L2B31A)

endif()
