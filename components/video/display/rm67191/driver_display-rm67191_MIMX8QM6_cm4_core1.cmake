if(NOT DRIVER_DISPLAY-RM67191_MIMX8QM6_cm4_core1_INCLUDED)
    
    set(DRIVER_DISPLAY-RM67191_MIMX8QM6_cm4_core1_INCLUDED true CACHE BOOL "driver_display-rm67191 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_rm67191.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_display-common_MIMX8QM6_cm4_core1)

    include(driver_display-mipi-dsi-cmd_MIMX8QM6_cm4_core1)

endif()
