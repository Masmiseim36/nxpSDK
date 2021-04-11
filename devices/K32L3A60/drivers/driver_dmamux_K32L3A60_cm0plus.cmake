if(NOT DRIVER_DMAMUX_K32L3A60_cm0plus_INCLUDED)
    
    set(DRIVER_DMAMUX_K32L3A60_cm0plus_INCLUDED true CACHE BOOL "driver_dmamux component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_dmamux.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_K32L3A60_cm0plus)

endif()
