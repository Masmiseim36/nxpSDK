if(NOT DRIVER_EASRC_MIMX8MN6_INCLUDED)
    
    set(DRIVER_EASRC_MIMX8MN6_INCLUDED true CACHE BOOL "driver_easrc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_asrc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8MN6)

endif()
