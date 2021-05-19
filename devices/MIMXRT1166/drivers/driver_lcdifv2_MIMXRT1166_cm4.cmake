if(NOT DRIVER_LCDIFV2_MIMXRT1166_cm4_INCLUDED)
    
    set(DRIVER_LCDIFV2_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "driver_lcdifv2 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lcdifv2.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1166_cm4)

endif()
