if(NOT DRIVER_DISPLAY-COMMON_MIMX8QM6_cm4_core1_INCLUDED)
    
    set(DRIVER_DISPLAY-COMMON_MIMX8QM6_cm4_core1_INCLUDED true CACHE BOOL "driver_display-common component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(driver_video-common_MIMX8QM6_cm4_core1)

endif()
