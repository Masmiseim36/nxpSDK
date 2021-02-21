if(NOT MIDDLEWARE_ISSDK_DRIVERS_SYSTICK_UTILS_MIMXRT1176_cm7_INCLUDED)
    
    set(MIDDLEWARE_ISSDK_DRIVERS_SYSTICK_UTILS_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "middleware_issdk_drivers_systick_utils component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/drivers/systick/systick_utils.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/drivers/systick
    )


endif()
