if(NOT MIDDLEWARE_AZURE_IOT_AZURE_MACRO_UTILS_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_AZURE_IOT_AZURE_MACRO_UTILS_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_azure_iot_azure_macro_utils component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/deps/azure-macro-utils-c/inc
    )

    include(middleware_azure_iot_MIMXRT1052)

endif()
