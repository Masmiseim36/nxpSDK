if(NOT DRIVER_IRQSTEER_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(DRIVER_IRQSTEER_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "driver_irqsteer component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_irqsteer.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8QM6_cm4_core0)

endif()
