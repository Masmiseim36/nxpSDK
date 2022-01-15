include_guard(GLOBAL)
message("middleware_se_hostlib_commonSe050 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/se05x/src/se05x_ECC_curves.c
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/se05x/src/se05x_mw.c
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/se05x/src/se05x_tlv.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/src/fsl_sss_apis.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/src/fsl_sss_util_asn1_der.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/src/fsl_sss_util_rsa_sign_utils.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/src/se05x/fsl_sss_se05x_apis.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/src/se05x/fsl_sss_se05x_eckey.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/src/se05x/fsl_sss_se05x_mw.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/src/se05x/fsl_sss_se05x_policy.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/src/se05x/fsl_sss_se05x_scp03.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/sss/port/ksdk
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/inc
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/se05x/src
    ${CMAKE_CURRENT_LIST_DIR}/sss/inc
    ${CMAKE_CURRENT_LIST_DIR}/.
    ${CMAKE_CURRENT_LIST_DIR}/platform
)


include(middleware_se_hostlib_commonSe050_ksdk_MIMXRT1176_cm7)

include(middleware_se_hostlib_commonSe050smCom_MIMXRT1176_cm7)

include(middleware_se_hostlib_commonSe050infra_MIMXRT1176_cm7)

include(middleware_se_hostlib_commonSe050_sss_ex_MIMXRT1176_cm7)

include(middleware_se_hostlib_commonSe050_CurrentApplet_MIMXRT1176_cm7)

include(middleware_se_hostlib_commonSe050_Scp03_MIMXRT1176_cm7)

include(middleware_se_hostlib_mwlog_MIMXRT1176_cm7)

