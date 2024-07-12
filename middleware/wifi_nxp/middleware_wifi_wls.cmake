# Add set(CONFIG_USE_middleware_wifi_wls true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_wifi AND CONFIG_USE_middleware_wifi_wifidriver)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/wls/range_kalman.c
  ${CMAKE_CURRENT_LIST_DIR}/wls/wls_api.c
  ${CMAKE_CURRENT_LIST_DIR}/wls/wls_processing.c
  ${CMAKE_CURRENT_LIST_DIR}/wls/wls_QR_algorithm.c
  ${CMAKE_CURRENT_LIST_DIR}/wls/wls_radix4Fft.c
  ${CMAKE_CURRENT_LIST_DIR}/wls/wls_subspace_processing.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/wls
)

else()

message(SEND_ERROR "middleware_wifi_wls dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
