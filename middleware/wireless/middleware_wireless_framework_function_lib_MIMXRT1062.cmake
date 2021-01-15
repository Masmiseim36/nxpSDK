if(NOT MIDDLEWARE_WIRELESS_FRAMEWORK_FUNCTION_LIB_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_WIRELESS_FRAMEWORK_FUNCTION_LIB_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_wireless_framework_function_lib component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/framework/FunctionLib/FunctionLib.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/framework/FunctionLib
    )


endif()
