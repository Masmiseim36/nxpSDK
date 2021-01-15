if(NOT MIDDLEWARE_AZURE_IOT_UMOCK_C_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_AZURE_IOT_UMOCK_C_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_azure_iot_umock_c component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/deps/umock-c/inc
    )

    include(middleware_azure_iot_MIMXRT1052)

endif()
