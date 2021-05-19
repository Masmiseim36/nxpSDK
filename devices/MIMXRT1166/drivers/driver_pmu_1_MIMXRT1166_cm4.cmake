if(NOT DRIVER_PMU_1_MIMXRT1166_cm4_INCLUDED)
    
    set(DRIVER_PMU_1_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "driver_pmu_1 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_pmu.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1166_cm4)

    include(driver_anatop_ai_MIMXRT1166_cm4)

endif()
