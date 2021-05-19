if(NOT DRIVER_GT911_MIMXRT1166_cm4_INCLUDED)
    
    set(DRIVER_GT911_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "driver_gt911 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_gt911.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1166_cm4)

endif()
