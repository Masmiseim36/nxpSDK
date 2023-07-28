# Add set(CONFIG_USE_middleware_se_hostlib_commonSe050 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_se_hostlib_commonSe050_ksdk AND CONFIG_USE_middleware_se_hostlib_commonSe050smCom AND CONFIG_USE_middleware_se_hostlib_commonSe050infra AND CONFIG_USE_middleware_se_hostlib_commonSe050_sss_ex AND CONFIG_USE_middleware_se_hostlib_commonSe050_CurrentApplet AND CONFIG_USE_middleware_se_hostlib_commonSe050_Scp03 AND CONFIG_USE_middleware_se_hostlib_mwlog)

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

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/sss/port/ksdk
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/inc
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/se05x/src
  ${CMAKE_CURRENT_LIST_DIR}/sss/inc
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/platform
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DT1oI2C
    -DT1oI2C_UM11225
    -DSSS_USE_FTR_FILE
    -DSDK_DEBUGCONSOLE_UART
    -DNO_SECURE_CHANNEL_SUPPORT
  )

endif()

else()

message(SEND_ERROR "middleware_se_hostlib_commonSe050 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
