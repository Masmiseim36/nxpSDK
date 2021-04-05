if(NOT DRIVER_SNVS_LP_MCIMX7U5_INCLUDED)
    
    set(DRIVER_SNVS_LP_MCIMX7U5_INCLUDED true CACHE BOOL "driver_snvs_lp component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_snvs_lp.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MCIMX7U5)

endif()
