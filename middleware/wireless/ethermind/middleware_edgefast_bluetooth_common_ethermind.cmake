# Add set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND CONFIG_USE_middleware_edgefast_bluetooth_extension_common_ethermind AND CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci AND CONFIG_USE_middleware_edgefast_bluetooth_config_ethermind AND CONFIG_USE_middleware_littlefs AND CONFIG_USE_middleware_fatfs AND CONFIG_USE_component_osa)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/export/vendor/vendor_specific_init.c
  ${CMAKE_CURRENT_LIST_DIR}/port/osal/src/freertos/EM_config.c
  ${CMAKE_CURRENT_LIST_DIR}/port/osal/src/freertos/EM_debug.c
  ${CMAKE_CURRENT_LIST_DIR}/port/osal/src/freertos/EM_fops.c
  ${CMAKE_CURRENT_LIST_DIR}/port/osal/src/freertos/EM_os.c
  ${CMAKE_CURRENT_LIST_DIR}/port/osal/src/freertos/EM_serial.c
  ${CMAKE_CURRENT_LIST_DIR}/port/osal/src/freertos/EM_timer.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/BT_status_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/BT_storage_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/btsnoop_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/sco_audio_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/ctn_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/ftp_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/ht_read_task_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/map_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/obex_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/pbap_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/write_task_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/littlefs_pl.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/fw_loader_uart.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/ethal/ethal_config.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/ethal/ethal_debug.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/ethal/ethal_fops.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/ethal/ethal_os.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/ethal/ethal_serial.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/ethal/ethal_timer.c
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/EM_platform.c
)

if(CONFIG_TOOLCHAIN STREQUAL mcux)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/toolspec/mcuxpresso/toolspec.c
  )
endif()

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/export/eOSAL
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/export/include
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/export/vendor
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/protocols/att
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/protocols/avctp
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/protocols/avdtp
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/protocols/bnep
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/protocols/dbase
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/protocols/hci_1.2
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/protocols/mcap
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/protocols/obex
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/protocols/rfcomm
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/protocols/sdp
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/protocols/sm
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/protocols/smp
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/utils/aes_cmac
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/utils/at_parser
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/utils/object_parser
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/utils/racp
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/utils/storage
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/utils/xml_parser
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/ethal
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/sbc
  ${CMAKE_CURRENT_LIST_DIR}/port/osal/src/freertos
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/utils/sbc
)

if(CONFIG_TOOLCHAIN STREQUAL mcux)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/toolspec/mcuxpresso
)
endif()

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DLFS_NO_INTRINSICS=1
    -DLFS_NO_ERROR=1
    -DFSL_OSA_TASK_ENABLE=1
    -DFSL_OSA_MAIN_FUNC_ENABLE=0
  )

endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_common_ethermind dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
