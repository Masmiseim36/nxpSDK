if(NOT DRIVER_LPADC_MIMX8QX6_INCLUDED)
    
    set(DRIVER_LPADC_MIMX8QX6_INCLUDED true CACHE BOOL "driver_lpadc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpadc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8QX6)

endif()
