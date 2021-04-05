if(NOT DRIVER_ISI_MIMX8QX6_INCLUDED)
    
    set(DRIVER_ISI_MIMX8QX6_INCLUDED true CACHE BOOL "driver_isi component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_isi.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8QX6)

endif()
