if(NOT DRIVER_TSTMR_K32L3A60_cm4_INCLUDED)
    
    set(DRIVER_TSTMR_K32L3A60_cm4_INCLUDED true CACHE BOOL "driver_tstmr component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(driver_common_K32L3A60_cm4)

endif()
