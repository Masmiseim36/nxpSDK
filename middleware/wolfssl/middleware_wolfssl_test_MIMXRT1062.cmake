if(NOT MIDDLEWARE_WOLFSSL_TEST_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_WOLFSSL_TEST_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_wolfssl_test component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/test/test.c
    )


    include(middleware_wolfssl_MIMXRT1062)

endif()
