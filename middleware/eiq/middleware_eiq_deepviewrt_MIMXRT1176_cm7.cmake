if(NOT MIDDLEWARE_EIQ_DEEPVIEWRT_MIMXRT1176_cm7_INCLUDED)
    
    set(MIDDLEWARE_EIQ_DEEPVIEWRT_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "middleware_eiq_deepviewrt component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/include
    )

endif()
