if(NOT MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_ILI9341_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_ILI9341_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_sln_iot_common_platform_sln_ili9341 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sln_ili9341.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


endif()
