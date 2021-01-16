if(NOT DRIVER_DC-FB-ELCDIF_MIMXRT1064_INCLUDED)
    
    set(DRIVER_DC-FB-ELCDIF_MIMXRT1064_INCLUDED true CACHE BOOL "driver_dc-fb-elcdif component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_dc_fb_elcdif.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_dc-fb-common_MIMXRT1064)

    include(driver_elcdif_MIMXRT1064)

endif()
