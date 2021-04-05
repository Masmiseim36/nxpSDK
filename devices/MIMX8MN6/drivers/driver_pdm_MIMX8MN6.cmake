if(NOT DRIVER_PDM_MIMX8MN6_INCLUDED)
    
    set(DRIVER_PDM_MIMX8MN6_INCLUDED true CACHE BOOL "driver_pdm component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_pdm.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8MN6)

endif()
