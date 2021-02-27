if(NOT DRIVER_DC-FB-LCDIF_MIMXRT595S_cm33_INCLUDED)
    
    set(DRIVER_DC-FB-LCDIF_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "driver_dc-fb-lcdif component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_dc_fb_lcdif.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_dc-fb-common_MIMXRT595S_cm33)

    include(driver_lcdif_MIMXRT595S_cm33)

endif()
