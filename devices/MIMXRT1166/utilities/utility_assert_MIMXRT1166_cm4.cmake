if(NOT UTILITY_ASSERT_MIMXRT1166_cm4_INCLUDED)
    
    set(UTILITY_ASSERT_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "utility_assert component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_assert.c
    )


    include(utility_debug_console_MIMXRT1166_cm4)

endif()
