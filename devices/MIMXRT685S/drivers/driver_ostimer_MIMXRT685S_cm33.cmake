if(NOT DRIVER_OSTIMER_MIMXRT685S_cm33_INCLUDED)
    
    set(DRIVER_OSTIMER_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "driver_ostimer component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_ostimer.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT685S_cm33)

endif()
