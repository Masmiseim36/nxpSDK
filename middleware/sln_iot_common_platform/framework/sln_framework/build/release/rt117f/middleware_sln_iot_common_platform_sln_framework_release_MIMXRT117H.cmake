if(NOT MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_FRAMEWORK_RELEASE_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_FRAMEWORK_RELEASE_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_sln_iot_common_platform_sln_framework_release component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
        ${CMAKE_CURRENT_LIST_DIR}/inc
        ${CMAKE_CURRENT_LIST_DIR}/hal_api
    )

endif()
