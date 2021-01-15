if(NOT MIDDLEWARE_SE_HOSTLIB_COMMONSE050_SSS_EX_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_SE_HOSTLIB_COMMONSE050_SSS_EX_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_se_hostlib_commonSe050_sss_ex component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sss/ex/src/ex_sss_boot.c
        ${CMAKE_CURRENT_LIST_DIR}/sss/ex/src/ex_sss_boot_connectstring.c
        ${CMAKE_CURRENT_LIST_DIR}/sss/ex/src/ex_sss_se05x.c
        ${CMAKE_CURRENT_LIST_DIR}/sss/ex/src/ex_sss_se05x_auth.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sss/ex/inc
        ${CMAKE_CURRENT_LIST_DIR}/sss/ex/src
    )


endif()
