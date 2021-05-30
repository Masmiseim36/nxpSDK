if(NOT DRIVER_SIM_MK22F51212_INCLUDED)
    
    set(DRIVER_SIM_MK22F51212_INCLUDED true CACHE BOOL "driver_sim component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_sim.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MK22F51212)

endif()
