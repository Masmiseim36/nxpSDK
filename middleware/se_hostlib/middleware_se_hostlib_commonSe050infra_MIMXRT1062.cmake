if(NOT MIDDLEWARE_SE_HOSTLIB_COMMONSE050INFRA_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_SE_HOSTLIB_COMMONSE050INFRA_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_se_hostlib_commonSe050infra component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/global_platf.c
        ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_connect.c
        ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/nxLog.c
        ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_apdu.c
        ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_errors.c
        ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_printf.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra
    )


endif()
