if(NOT DRIVER_TRGMUX_MCIMX7U5_INCLUDED)
    
    set(DRIVER_TRGMUX_MCIMX7U5_INCLUDED true CACHE BOOL "driver_trgmux component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_trgmux.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MCIMX7U5)

endif()
