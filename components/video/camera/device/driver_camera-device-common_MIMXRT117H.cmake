if(NOT DRIVER_CAMERA-DEVICE-COMMON_MIMXRT117H_INCLUDED)
    
    set(DRIVER_CAMERA-DEVICE-COMMON_MIMXRT117H_INCLUDED true CACHE BOOL "driver_camera-device-common component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(driver_camera-common_MIMXRT117H)

    include(driver_common_MIMXRT117H)

endif()
