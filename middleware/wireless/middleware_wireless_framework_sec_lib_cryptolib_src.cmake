# Add set(CONFIG_USE_middleware_wireless_framework_sec_lib_cryptolib_src true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/framework/SecLib/SecLib.c
  ${CMAKE_CURRENT_LIST_DIR}/framework/SecLib/build/CryptoLib/src/SW_AES128.c
  ${CMAKE_CURRENT_LIST_DIR}/framework/SecLib/build/CryptoLib/src/SW_RNG.c
  ${CMAKE_CURRENT_LIST_DIR}/framework/SecLib/build/CryptoLib/src/SW_SHA1.c
  ${CMAKE_CURRENT_LIST_DIR}/framework/SecLib/build/CryptoLib/src/SW_SHA256.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/framework/SecLib
  ${CMAKE_CURRENT_LIST_DIR}/framework/SecLib/build/CryptoLib/src
)

