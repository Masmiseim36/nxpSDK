if(NOT MIDDLEWARE_SE_HOSTLIB_COMMONSE050_CURRENTAPPLET_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_SE_HOSTLIB_COMMONSE050_CURRENTAPPLET_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_se_hostlib_commonSe050_CurrentApplet component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/se05x_03_xx_xx/se05x_APDU.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/se05x_03_xx_xx
    )


endif()
