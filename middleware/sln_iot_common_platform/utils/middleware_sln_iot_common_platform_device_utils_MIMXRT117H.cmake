if(NOT MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_DEVICE_UTILS_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_DEVICE_UTILS_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_sln_iot_common_platform_device_utils component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/device_utils.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(middleware_mbedtls_MIMXRT117H)

endif()
