# Add set(CONFIG_USE_middleware_mmcau_mmcau_files true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_CORE STREQUAL cm0p)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/asm-cm0p/src/mmcau_aes_functions.s
      ${CMAKE_CURRENT_LIST_DIR}/asm-cm0p/src/mmcau_des_functions.s
      ${CMAKE_CURRENT_LIST_DIR}/asm-cm0p/src/mmcau_md5_functions.s
      ${CMAKE_CURRENT_LIST_DIR}/asm-cm0p/src/mmcau_sha1_functions.s
      ${CMAKE_CURRENT_LIST_DIR}/asm-cm0p/src/mmcau_sha256_functions.s
  )
endif()

if((CONFIG_CORE STREQUAL cm4f OR CONFIG_CORE STREQUAL cm7f))
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/asm-cm4-cm7/src/mmcau_aes_functions.s
      ${CMAKE_CURRENT_LIST_DIR}/asm-cm4-cm7/src/mmcau_des_functions.s
      ${CMAKE_CURRENT_LIST_DIR}/asm-cm4-cm7/src/mmcau_md5_functions.s
      ${CMAKE_CURRENT_LIST_DIR}/asm-cm4-cm7/src/mmcau_sha1_functions.s
      ${CMAKE_CURRENT_LIST_DIR}/asm-cm4-cm7/src/mmcau_sha256_functions.s
  )
endif()

