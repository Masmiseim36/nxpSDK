if(NOT MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_MT9M114_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_MT9M114_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_sln_iot_common_platform_sln_mt9m114 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sln_mt9m114.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


endif()
