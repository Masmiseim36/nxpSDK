if(NOT DRIVER_CAMERA-DEVICE-COMMON_MIMXRT1176_cm4_INCLUDED)
    
    set(DRIVER_CAMERA-DEVICE-COMMON_MIMXRT1176_cm4_INCLUDED true CACHE BOOL "driver_camera-device-common component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(driver_camera-common_MIMXRT1176_cm4)

    include(driver_common_MIMXRT1176_cm4)

endif()
