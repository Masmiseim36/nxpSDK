if(NOT DRIVER_LPSPI_MIMXRT117H_INCLUDED)
    
    set(DRIVER_LPSPI_MIMXRT117H_INCLUDED true CACHE BOOL "driver_lpspi component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpspi.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT117H)

endif()
