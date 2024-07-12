# Add set(CONFIG_USE_middleware_psatest_ns true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_psatest_ns_tgt_dev_apis_nxp)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/val/nspe/val_attestation.c
  ${CMAKE_CURRENT_LIST_DIR}/val/nspe/val_crypto.c
  ${CMAKE_CURRENT_LIST_DIR}/val/nspe/val_entry.c
  ${CMAKE_CURRENT_LIST_DIR}/val/nspe/val_framework.c
  ${CMAKE_CURRENT_LIST_DIR}/val/nspe/val_dispatcher.c
  ${CMAKE_CURRENT_LIST_DIR}/val/nspe/val_interfaces.c
  ${CMAKE_CURRENT_LIST_DIR}/val/nspe/val_peripherals.c
  ${CMAKE_CURRENT_LIST_DIR}/val/nspe/val_platform.c
  ${CMAKE_CURRENT_LIST_DIR}/val/nspe/val_storage.c
  ${CMAKE_CURRENT_LIST_DIR}/val/common/val_target.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/nvmem/pal_nvmem.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/targets/common/nspe/pal_weak.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/val/nspe
  ${CMAKE_CURRENT_LIST_DIR}/val/common
  ${CMAKE_CURRENT_LIST_DIR}/platform/drivers/nvmem
  ${CMAKE_CURRENT_LIST_DIR}/platform/targets/common/nspe/crypto
  ${CMAKE_CURRENT_LIST_DIR}/platform/targets/common/nspe
  ${CMAKE_CURRENT_LIST_DIR}/val
)

else()

message(SEND_ERROR "middleware_psatest_ns dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
