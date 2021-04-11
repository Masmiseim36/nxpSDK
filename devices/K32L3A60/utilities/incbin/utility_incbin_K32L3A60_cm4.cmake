if(NOT UTILITY_INCBIN_K32L3A60_cm4_INCLUDED)
    
    set(UTILITY_INCBIN_K32L3A60_cm4_INCLUDED true CACHE BOOL "utility_incbin component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_incbin.S
    )


endif()
