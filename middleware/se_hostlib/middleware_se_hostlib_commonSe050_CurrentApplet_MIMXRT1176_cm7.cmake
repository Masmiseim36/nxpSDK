include_guard(GLOBAL)
message("middleware_se_hostlib_commonSe050_CurrentApplet component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/se05x_03_xx_xx/se05x_APDU.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/se05x_03_xx_xx
)


