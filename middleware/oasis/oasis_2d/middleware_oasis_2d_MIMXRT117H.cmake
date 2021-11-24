if(NOT MIDDLEWARE_OASIS_2D_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_OASIS_2D_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_oasis_2d component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

endif()
