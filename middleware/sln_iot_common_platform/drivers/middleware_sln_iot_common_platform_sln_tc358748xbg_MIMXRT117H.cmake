if(NOT MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_TC358748XBG_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_TC358748XBG_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_sln_iot_common_platform_sln_tc358748xbg component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sln_tc358748xbg.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


endif()
