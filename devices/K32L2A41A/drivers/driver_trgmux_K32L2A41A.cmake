if(NOT DRIVER_TRGMUX_K32L2A41A_INCLUDED)
    
    set(DRIVER_TRGMUX_K32L2A41A_INCLUDED true CACHE BOOL "driver_trgmux component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_trgmux.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_K32L2A41A)

endif()
