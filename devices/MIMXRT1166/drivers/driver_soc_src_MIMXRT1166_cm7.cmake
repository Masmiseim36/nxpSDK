if(NOT DRIVER_SOC_SRC_MIMXRT1166_cm7_INCLUDED)
    
    set(DRIVER_SOC_SRC_MIMXRT1166_cm7_INCLUDED true CACHE BOOL "driver_soc_src component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_soc_src.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1166_cm7)

endif()
