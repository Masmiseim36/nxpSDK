if(NOT DRIVER_FMEAS_MIMXRT685S_cm33_INCLUDED)
    
    set(DRIVER_FMEAS_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "driver_fmeas component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_fmeas.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT685S_cm33)

endif()
