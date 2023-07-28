# Add set(CONFIG_USE_middleware_tfm_qcbor true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/ieee754.c
  ${CMAKE_CURRENT_LIST_DIR}/src/qcbor_decode.c
  ${CMAKE_CURRENT_LIST_DIR}/src/qcbor_encode.c
  ${CMAKE_CURRENT_LIST_DIR}/src/UsefulBuf.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src
  ${CMAKE_CURRENT_LIST_DIR}/inc
  ${CMAKE_CURRENT_LIST_DIR}/inc/qcbor
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DQCBOR_DISABLE_FLOAT_HW_USE
    -DUSEFULBUF_DISABLE_ALL_FLOAT
    -DQCBOR_DISABLE_PREFERRED_FLOAT
  )

endif()

