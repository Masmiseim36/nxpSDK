if(NOT DRIVER_TMU_MIMX8MQ6_INCLUDED)
    
    set(DRIVER_TMU_MIMX8MQ6_INCLUDED true CACHE BOOL "driver_tmu component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_tmu.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8MQ6)

endif()
