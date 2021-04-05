if(NOT DRIVER_ASMC_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(DRIVER_ASMC_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "driver_asmc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_asmc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8QM6_cm4_core0)

endif()
