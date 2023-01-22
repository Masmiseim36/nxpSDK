include_guard()
message("middleware_se_hostlib_commonCloudDemos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_demo_utils.c
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/infra/sm_demo_utils_rtos.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11/sss_pkcs11_pal_core.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11/sss_pkcs11_pal_helpers.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11/sss_pkcs11_pal_object.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11/sss_pkcs11_pal_asymm.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11/sss_pkcs11_pal_symm.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11/pkcs11_mbedtls_utils.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/inc
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/pkcs11
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/tstUtil
)


include(middleware_freertos-kernel_heap_4_MIMXRT1062)

include(middleware_freertos_coremqtt_MIMXRT1062)

include(middleware_pkcs11_MIMXRT1062)

include(middleware_freertos_backoffalgorithm_MIMXRT1062)

