if(NOT DRIVER_UTICK_MIMXRT685S_cm33_INCLUDED)
    
    set(DRIVER_UTICK_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "driver_utick component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_utick.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_power_MIMXRT685S_cm33)

    include(driver_common_MIMXRT685S_cm33)

endif()
