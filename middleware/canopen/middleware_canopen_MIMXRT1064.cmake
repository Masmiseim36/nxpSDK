if(NOT MIDDLEWARE_CANOPEN_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_CANOPEN_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_canopen component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/mco
    )

endif()
