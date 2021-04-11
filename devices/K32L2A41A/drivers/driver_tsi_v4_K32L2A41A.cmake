if(NOT DRIVER_TSI_V4_K32L2A41A_INCLUDED)
    
    set(DRIVER_TSI_V4_K32L2A41A_INCLUDED true CACHE BOOL "driver_tsi_v4 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_tsi_v4.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_K32L2A41A)

endif()
