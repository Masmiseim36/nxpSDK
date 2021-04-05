if(NOT DRIVER_II2C_MIMX8MN6_INCLUDED)
    
    set(DRIVER_II2C_MIMX8MN6_INCLUDED true CACHE BOOL "driver_ii2c component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_i2c.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


endif()
