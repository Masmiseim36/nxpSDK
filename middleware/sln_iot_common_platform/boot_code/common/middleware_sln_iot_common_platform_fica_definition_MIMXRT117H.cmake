if(NOT MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_FICA_DEFINITION_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_FICA_DEFINITION_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_sln_iot_common_platform_fica_definition component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(middleware_sln_iot_common_platform_sln_flash_MIMXRT117H)

endif()
