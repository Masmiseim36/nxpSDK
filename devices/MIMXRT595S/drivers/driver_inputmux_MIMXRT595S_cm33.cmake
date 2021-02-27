if(NOT DRIVER_INPUTMUX_MIMXRT595S_cm33_INCLUDED)
    
    set(DRIVER_INPUTMUX_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "driver_inputmux component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_inputmux.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT595S_cm33)

    include(driver_inputmux_connections_MIMXRT595S_cm33)

endif()
