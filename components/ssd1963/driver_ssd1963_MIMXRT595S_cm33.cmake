if(NOT DRIVER_SSD1963_MIMXRT595S_cm33_INCLUDED)
    
    set(DRIVER_SSD1963_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "driver_ssd1963 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_ssd1963.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_dbi_MIMXRT595S_cm33)

endif()
