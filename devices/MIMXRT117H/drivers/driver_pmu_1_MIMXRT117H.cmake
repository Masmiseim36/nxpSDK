if(NOT DRIVER_PMU_1_MIMXRT117H_INCLUDED)
    
    set(DRIVER_PMU_1_MIMXRT117H_INCLUDED true CACHE BOOL "driver_pmu_1 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_pmu.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT117H)

    include(driver_anatop_ai_MIMXRT117H)

endif()
