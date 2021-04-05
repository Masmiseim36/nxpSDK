if(NOT DRIVER_LPIT_MIMX8QM6_cm4_core1_INCLUDED)
    
    set(DRIVER_LPIT_MIMX8QM6_cm4_core1_INCLUDED true CACHE BOOL "driver_lpit component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpit.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8QM6_cm4_core1)

endif()
