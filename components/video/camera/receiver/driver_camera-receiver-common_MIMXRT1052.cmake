if(NOT DRIVER_CAMERA-RECEIVER-COMMON_MIMXRT1052_INCLUDED)
    
    set(DRIVER_CAMERA-RECEIVER-COMMON_MIMXRT1052_INCLUDED true CACHE BOOL "driver_camera-receiver-common component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(driver_video-common_MIMXRT1052)

    include(driver_camera-common_MIMXRT1052)

endif()
