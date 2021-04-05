if(NOT UTILITY_ASSERT_MCIMX7U5_INCLUDED)
    
    set(UTILITY_ASSERT_MCIMX7U5_INCLUDED true CACHE BOOL "utility_assert component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_assert.c
    )


    include(utility_debug_console_MCIMX7U5)

endif()
