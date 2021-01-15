if(NOT DRIVER_SCTIMER_MIMXRT685S_cm33_INCLUDED)
    
    set(DRIVER_SCTIMER_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "driver_sctimer component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_sctimer.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT685S_cm33)

endif()
