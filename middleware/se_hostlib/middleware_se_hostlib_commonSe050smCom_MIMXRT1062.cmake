include_guard(GLOBAL)
message("middleware_se_hostlib_commonSe050smCom component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/smCom/smCom.c
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/smCom/smComT1oI2C.c
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/smCom/T1oI2C/phNxpEsePal_i2c.c
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/smCom/T1oI2C/phNxpEseProto7816_3.c
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/smCom/T1oI2C/phNxpEse_Api.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/smCom
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/smCom/T1oI2C
)


