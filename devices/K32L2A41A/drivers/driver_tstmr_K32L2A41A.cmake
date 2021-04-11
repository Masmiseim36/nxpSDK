if(NOT DRIVER_TSTMR_K32L2A41A_INCLUDED)
    
    set(DRIVER_TSTMR_K32L2A41A_INCLUDED true CACHE BOOL "driver_tstmr component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(driver_common_K32L2A41A)

endif()
