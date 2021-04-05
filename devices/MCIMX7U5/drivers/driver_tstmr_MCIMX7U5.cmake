if(NOT DRIVER_TSTMR_MCIMX7U5_INCLUDED)
    
    set(DRIVER_TSTMR_MCIMX7U5_INCLUDED true CACHE BOOL "driver_tstmr component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(driver_common_MCIMX7U5)

endif()
