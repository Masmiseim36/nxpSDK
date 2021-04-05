if(NOT DRIVER_EASRC_SDMA_MIMX8MN6_INCLUDED)
    
    set(DRIVER_EASRC_SDMA_MIMX8MN6_INCLUDED true CACHE BOOL "driver_easrc_sdma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_asrc_sdma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_easrc_MIMX8MN6)

    include(driver_sdma_MIMX8MN6)

    include(driver_common_MIMX8MN6)

endif()
