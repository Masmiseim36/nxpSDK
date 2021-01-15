if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_ABSTRACTIONS_BLE_HAL_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_ABSTRACTIONS_BLE_HAL_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_libraries_abstractions_ble_hal component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/ble_hal/include
    )

endif()
