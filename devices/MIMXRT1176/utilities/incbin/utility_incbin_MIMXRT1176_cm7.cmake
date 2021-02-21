if(NOT UTILITY_INCBIN_MIMXRT1176_cm7_INCLUDED)
    
    set(UTILITY_INCBIN_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "utility_incbin component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_incbin.S
    )


endif()
