if(NOT DRIVER_DISPLAY-RM68200_MIMXRT595S_cm33_INCLUDED)
    
    set(DRIVER_DISPLAY-RM68200_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "driver_display-rm68200 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_rm68200.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_display-common_MIMXRT595S_cm33)

    include(driver_display-mipi-dsi-cmd_MIMXRT595S_cm33)

endif()
