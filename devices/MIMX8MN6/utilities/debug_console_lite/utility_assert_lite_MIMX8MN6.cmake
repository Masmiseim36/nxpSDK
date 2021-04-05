if(NOT UTILITY_ASSERT_LITE_MIMX8MN6_INCLUDED)
    
    set(UTILITY_ASSERT_LITE_MIMX8MN6_INCLUDED true CACHE BOOL "utility_assert_lite component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_assert.c
    )


    include(utility_debug_console_lite_MIMX8MN6)

endif()
