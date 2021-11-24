if(NOT MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_ICONS_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_ICONS_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_sln_iot_common_platform_icons component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

endif()
