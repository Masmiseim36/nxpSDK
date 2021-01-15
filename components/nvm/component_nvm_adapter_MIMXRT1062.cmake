if(NOT COMPONENT_NVM_ADAPTER_MIMXRT1062_INCLUDED)
    
    set(COMPONENT_NVM_ADAPTER_MIMXRT1062_INCLUDED true CACHE BOOL "component_nvm_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/nvm_adapter.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1062)

    include(middleware_wireless_framework_function_lib_MIMXRT1062)

    include(middleware_wireless_framework_NVM_MIMXRT1062)

    include(middleware_wireless_framework_Common_MIMXRT1062)

endif()
