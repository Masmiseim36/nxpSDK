if(NOT UTILITIES_MISC_UTILITIES_MIMX8MQ6_INCLUDED)
    
    set(UTILITIES_MISC_UTILITIES_MIMX8MQ6_INCLUDED true CACHE BOOL "utilities_misc_utilities component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_sbrk.c
    )


endif()
