if(NOT DRIVER_FT5406_MIMXRT595S_cm33_INCLUDED)
    
    set(DRIVER_FT5406_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "driver_ft5406 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_ft5406.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT595S_cm33)

endif()
