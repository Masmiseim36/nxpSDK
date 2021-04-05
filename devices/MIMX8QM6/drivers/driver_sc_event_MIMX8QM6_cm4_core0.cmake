if(NOT DRIVER_SC_EVENT_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(DRIVER_SC_EVENT_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "driver_sc_event component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_sc_event.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_mu_MIMX8QM6_cm4_core0)

    include(driver_scfw_api_MIMX8QM6_cm4_core0)

    include(driver_common_MIMX8QM6_cm4_core0)

endif()
