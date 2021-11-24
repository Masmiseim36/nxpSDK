if(NOT MIDDLEWARE_ORBBEC_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_ORBBEC_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_orbbec component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
    )

endif()
