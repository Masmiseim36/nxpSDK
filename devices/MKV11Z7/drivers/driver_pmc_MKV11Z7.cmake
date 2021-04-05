if(NOT DRIVER_PMC_MKV11Z7_INCLUDED)
    
    set(DRIVER_PMC_MKV11Z7_INCLUDED true CACHE BOOL "driver_pmc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_pmc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MKV11Z7)

endif()
