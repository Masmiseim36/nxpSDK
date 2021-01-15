if(NOT MIDDLEWARE_WIRELESS_FRAMEWORK_COMMON_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_WIRELESS_FRAMEWORK_COMMON_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_wireless_framework_Common component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/framework/Common
    )

endif()
