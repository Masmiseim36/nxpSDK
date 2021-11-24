if(NOT MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_CAMERA_ADAPTER_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_CAMERA_ADAPTER_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_sln_iot_common_platform_sln_camera_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sln_flexio_camera_adapter.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


endif()
