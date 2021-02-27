if(NOT DRIVER_FT3267_MIMXRT595S_cm33_INCLUDED)
    
    set(DRIVER_FT3267_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "driver_ft3267 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_ft3267.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT595S_cm33)

endif()
