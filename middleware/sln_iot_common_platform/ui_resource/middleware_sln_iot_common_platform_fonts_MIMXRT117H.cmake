if(NOT MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_FONTS_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_FONTS_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_sln_iot_common_platform_fonts component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fonts/font.c
        ${CMAKE_CURRENT_LIST_DIR}/fonts/raleway.c
        ${CMAKE_CURRENT_LIST_DIR}/fonts/cambo.c
        ${CMAKE_CURRENT_LIST_DIR}/fonts/open_sans.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


endif()
