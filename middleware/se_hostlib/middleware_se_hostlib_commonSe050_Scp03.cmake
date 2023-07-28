# Add set(CONFIG_USE_middleware_se_hostlib_commonSe050_Scp03 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/nxScp/nxScp03_Com.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/scp/scp.c
)

