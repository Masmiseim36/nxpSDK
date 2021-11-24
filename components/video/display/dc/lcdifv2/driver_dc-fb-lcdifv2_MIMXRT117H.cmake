if(NOT DRIVER_DC-FB-LCDIFV2_MIMXRT117H_INCLUDED)
    
    set(DRIVER_DC-FB-LCDIFV2_MIMXRT117H_INCLUDED true CACHE BOOL "driver_dc-fb-lcdifv2 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_dc_fb_lcdifv2.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_dc-fb-common_MIMXRT117H)

    include(driver_display-common_MIMXRT117H)

    include(driver_lcdifv2_MIMXRT117H)

endif()
