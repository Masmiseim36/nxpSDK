if(NOT DRIVER_DISPLAY-COMMON_MIMXRT1176_cm7_INCLUDED)
    
    set(DRIVER_DISPLAY-COMMON_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "driver_display-common component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(driver_video-common_MIMXRT1176_cm7)

endif()
