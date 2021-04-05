if(NOT MIDDLEWARE_ISSDK_DRIVERS_SYSTICK_UTILS_MEKMIMX8QM_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(MIDDLEWARE_ISSDK_DRIVERS_SYSTICK_UTILS_MEKMIMX8QM_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "middleware_issdk_drivers_systick_utils_mekmimx8qm component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/drivers/systick/systick_utils.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/drivers/systick
    )


endif()
