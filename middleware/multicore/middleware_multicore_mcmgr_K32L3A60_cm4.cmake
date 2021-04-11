if(NOT MIDDLEWARE_MULTICORE_MCMGR_K32L3A60_cm4_INCLUDED)
    
    set(MIDDLEWARE_MULTICORE_MCMGR_K32L3A60_cm4_INCLUDED true CACHE BOOL "middleware_multicore_mcmgr component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/mcmgr/src/mcmgr.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/mcmgr/src
    )


endif()
