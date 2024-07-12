# Add set(CONFIG_USE_middleware_mbedtls_3rdparty true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/3rdparty/everest/library/Hacl_Curve25519.c
  ${CMAKE_CURRENT_LIST_DIR}/3rdparty/everest/library/Hacl_Curve25519_joined.c
  ${CMAKE_CURRENT_LIST_DIR}/3rdparty/everest/library/everest.c
  ${CMAKE_CURRENT_LIST_DIR}/3rdparty/everest/library/kremlib/FStar_UInt128_extracted.c
  ${CMAKE_CURRENT_LIST_DIR}/3rdparty/everest/library/kremlib/FStar_UInt64_FStar_UInt32_FStar_UInt16_FStar_UInt8.c
  ${CMAKE_CURRENT_LIST_DIR}/3rdparty/everest/library/legacy/Hacl_Curve25519.c
  ${CMAKE_CURRENT_LIST_DIR}/3rdparty/everest/library/x25519.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/3rdparty/everest/include
  ${CMAKE_CURRENT_LIST_DIR}/3rdparty/everest/include/everest
  ${CMAKE_CURRENT_LIST_DIR}/3rdparty/everest/include/everest/kremlib
  ${CMAKE_CURRENT_LIST_DIR}/3rdparty/everest/include/everest/kremlin
  ${CMAKE_CURRENT_LIST_DIR}/3rdparty/everest/include/everest/kremlin/internal
)

