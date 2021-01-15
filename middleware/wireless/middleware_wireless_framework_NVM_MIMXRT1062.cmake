if(NOT MIDDLEWARE_WIRELESS_FRAMEWORK_NVM_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_WIRELESS_FRAMEWORK_NVM_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_wireless_framework_NVM component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/framework/NVM/Source/NV_Flash.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/framework/NVM/Interface
        ${CMAKE_CURRENT_LIST_DIR}/framework/NVM/Source
    )


endif()
