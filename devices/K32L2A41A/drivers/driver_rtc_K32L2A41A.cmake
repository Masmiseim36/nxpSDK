if(NOT DRIVER_RTC_K32L2A41A_INCLUDED)
    
    set(DRIVER_RTC_K32L2A41A_INCLUDED true CACHE BOOL "driver_rtc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_rtc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_K32L2A41A)

endif()
