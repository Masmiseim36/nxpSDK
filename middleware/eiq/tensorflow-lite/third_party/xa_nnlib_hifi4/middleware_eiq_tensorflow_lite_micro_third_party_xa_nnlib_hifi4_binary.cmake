# Add set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_xa_nnlib_hifi4_binary true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(((CONFIG_DEVICE_ID STREQUAL MIMXRT685S) OR (CONFIG_CORE STREQUAL dsp)))

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/include/nnlib
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DNNLIB_V2
    -DMODEL_INT16
    -Dhifi4
  )

endif()

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
  -Wl,--start-group
  ${CMAKE_CURRENT_LIST_DIR}/lib/mimxrt685s/libxa_nnlib_hifi4.a
  -Wl,--end-group
)

else()

message(SEND_ERROR "middleware_eiq_tensorflow_lite_micro_third_party_xa_nnlib_hifi4_binary dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
