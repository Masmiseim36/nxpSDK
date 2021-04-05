if(NOT DRIVER_WDOG32_MIMX8QM6_cm4_core1_INCLUDED)
    
    set(DRIVER_WDOG32_MIMX8QM6_cm4_core1_INCLUDED true CACHE BOOL "driver_wdog32 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_wdog32.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8QM6_cm4_core1)

endif()
