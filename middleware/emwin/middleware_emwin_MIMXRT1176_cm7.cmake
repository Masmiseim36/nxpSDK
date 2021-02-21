if(NOT MIDDLEWARE_EMWIN_MIMXRT1176_cm7_INCLUDED)
    
    set(MIDDLEWARE_EMWIN_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "middleware_emwin component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/emWin_header
        ${CMAKE_CURRENT_LIST_DIR}/emWin_Config
    )

endif()
