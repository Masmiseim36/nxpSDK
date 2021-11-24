if(NOT DRIVER_CLOCK_MIMXRT117H_INCLUDED)
    
    set(DRIVER_CLOCK_MIMXRT117H_INCLUDED true CACHE BOOL "driver_clock component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_clock.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT117H)

    include(driver_anatop_ai_MIMXRT117H)

    include(driver_pmu_1_MIMXRT117H)

endif()
