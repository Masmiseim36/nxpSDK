if(NOT COMPONENT_OSA_BM_MKV11Z7_INCLUDED)
    
    set(COMPONENT_OSA_BM_MKV11Z7_INCLUDED true CACHE BOOL "component_osa_bm component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_os_abstraction_bm.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MKV11Z7)

    include(component_lists_MKV11Z7)

endif()
