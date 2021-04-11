if(NOT DRIVER_COP_K32L2B31A_INCLUDED)
    
    set(DRIVER_COP_K32L2B31A_INCLUDED true CACHE BOOL "driver_cop component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_cop.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_K32L2B31A)

endif()
