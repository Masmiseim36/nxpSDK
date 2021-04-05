if(NOT DRIVER_TSTMR_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(DRIVER_TSTMR_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "driver_tstmr component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(driver_common_MIMX8QM6_cm4_core0)

endif()
