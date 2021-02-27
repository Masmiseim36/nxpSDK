if(NOT DRIVER_I3C_MIMXRT595S_cm33_INCLUDED)
    
    set(DRIVER_I3C_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "driver_i3c component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_i3c.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT595S_cm33)

endif()
