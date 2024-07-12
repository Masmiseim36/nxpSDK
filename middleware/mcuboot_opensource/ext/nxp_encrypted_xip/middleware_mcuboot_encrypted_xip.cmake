# Add set(CONFIG_USE_middleware_mcuboot_encrypted_xip true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/mcuboot_enc_support.c
  ${CMAKE_CURRENT_LIST_DIR}/src/platform_enc_bee.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
)

