if(NOT DRIVER_RCM_K32L2B31A_INCLUDED)
    
    set(DRIVER_RCM_K32L2B31A_INCLUDED true CACHE BOOL "driver_rcm component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_rcm.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_K32L2B31A)

endif()
