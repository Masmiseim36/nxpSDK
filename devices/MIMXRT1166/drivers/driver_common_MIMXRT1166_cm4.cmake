if(NOT DRIVER_COMMON_MIMXRT1166_cm4_INCLUDED)
    
    set(DRIVER_COMMON_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "driver_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_clock_MIMXRT1166_cm4)

    include(device_MIMXRT1166_CMSIS_MIMXRT1166_cm4)

endif()
