if(NOT DRIVER_FT5406_RT_MIMXRT1062_INCLUDED)
    
    set(DRIVER_FT5406_RT_MIMXRT1062_INCLUDED true CACHE BOOL "driver_ft5406_rt component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_ft5406_rt.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_lpi2c_MIMXRT1062)

endif()
