if(NOT MIDDLEWARE_SIGFOX_K32L2B31A_INCLUDED)
    
    set(MIDDLEWARE_SIGFOX_K32L2B31A_INCLUDED true CACHE BOOL "middleware_sigfox component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sf.c
        ${CMAKE_CURRENT_LIST_DIR}/sf_setup.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(middleware_aml_K32L2B31A)

endif()
