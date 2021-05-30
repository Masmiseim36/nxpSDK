if(NOT DRIVER_PMC_MK66F18_INCLUDED)
    
    set(DRIVER_PMC_MK66F18_INCLUDED true CACHE BOOL "driver_pmc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_pmc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MK66F18)

endif()
