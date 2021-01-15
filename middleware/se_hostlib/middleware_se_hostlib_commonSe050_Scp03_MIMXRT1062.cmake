if(NOT MIDDLEWARE_SE_HOSTLIB_COMMONSE050_SCP03_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_SE_HOSTLIB_COMMONSE050_SCP03_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_se_hostlib_commonSe050_Scp03 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/nxScp/nxScp03_Com.c
        ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/scp/scp.c
    )


endif()
