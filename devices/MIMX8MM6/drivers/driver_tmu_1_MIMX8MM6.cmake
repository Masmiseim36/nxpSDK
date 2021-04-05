if(NOT DRIVER_TMU_1_MIMX8MM6_INCLUDED)
    
    set(DRIVER_TMU_1_MIMX8MM6_INCLUDED true CACHE BOOL "driver_tmu_1 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_tmu.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8MM6)

endif()
