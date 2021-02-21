if(NOT DRIVER_CLOCK_MIMXRT1176_cm4_INCLUDED)
    
    set(DRIVER_CLOCK_MIMXRT1176_cm4_INCLUDED true CACHE BOOL "driver_clock component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_clock.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1176_cm4)

    include(driver_anatop_ai_MIMXRT1176_cm4)

endif()
