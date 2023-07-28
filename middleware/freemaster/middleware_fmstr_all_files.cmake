# Add set(CONFIG_USE_middleware_fmstr_all_files true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/common/freemaster_appcmd.c
  ${CMAKE_CURRENT_LIST_DIR}/src/common/freemaster_can.c
  ${CMAKE_CURRENT_LIST_DIR}/src/common/freemaster_net.c
  ${CMAKE_CURRENT_LIST_DIR}/src/common/freemaster_pdbdm.c
  ${CMAKE_CURRENT_LIST_DIR}/src/common/freemaster_pipes.c
  ${CMAKE_CURRENT_LIST_DIR}/src/common/freemaster_protocol.c
  ${CMAKE_CURRENT_LIST_DIR}/src/common/freemaster_rec.c
  ${CMAKE_CURRENT_LIST_DIR}/src/common/freemaster_scope.c
  ${CMAKE_CURRENT_LIST_DIR}/src/common/freemaster_serial.c
  ${CMAKE_CURRENT_LIST_DIR}/src/common/freemaster_sha.c
  ${CMAKE_CURRENT_LIST_DIR}/src/common/freemaster_tsa.c
  ${CMAKE_CURRENT_LIST_DIR}/src/common/freemaster_ures.c
  ${CMAKE_CURRENT_LIST_DIR}/src/common/freemaster_utils.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/DSC/freemaster_56f800e_eonce.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/DSC/freemaster_56f800e_flexcan.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/DSC/freemaster_56f800e_qsci.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/Kxx/freemaster_kxx_uart.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/MPC/freemaster_mpc574xp_linflex.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/S12Z/freemaster_s12z_mscan.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/S12Z/freemaster_s12z_sci.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/S32G/freemaster_s32g274a_linflexd.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/S32K/freemaster_s32k144_lpuart.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/S32K/freemaster_s32k3xx_lpuart.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/S32R/freemaster_s32r45_linflexd.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/S32S/freemaster_s32s247tv_linflexd.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/can/freemaster_flexcan.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/can/freemaster_flexcan_dsc.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/can/freemaster_mcan.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/can/freemaster_mscan.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/can/freemaster_mscan_dsc.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial/freemaster_56f800e_eonce.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial/freemaster_serial_lpsci.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial/freemaster_serial_lpuart.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial/freemaster_serial_miniusart.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial/freemaster_serial_qsci.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial/freemaster_serial_uart.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial/freemaster_serial_usart.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial/freemaster_serial_usb.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/ampxsdk/uart/S32K1x/freemaster_serial_lpuart.c
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/ampxsdk/can/S32K1x/freemaster_flexcan.c
  ${CMAKE_CURRENT_LIST_DIR}/src/support/mcuxsdk_usb/freemaster_usb.c
  ${CMAKE_CURRENT_LIST_DIR}/src/support/mcuxsdk_usb/freemaster_usb_device_descriptor.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src/common
  ${CMAKE_CURRENT_LIST_DIR}/src/platforms/56f800e
  ${CMAKE_CURRENT_LIST_DIR}/src/platforms/gen32be
  ${CMAKE_CURRENT_LIST_DIR}/src/platforms/gen32le
  ${CMAKE_CURRENT_LIST_DIR}/src/platforms/s12z
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/DSC
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/Kxx
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/MPC
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/S12Z
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/S32G
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/S32K
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/S32R
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/dreg/S32S
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/can
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/mcuxsdk/serial
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/ampxsdk/uart/S32K1x
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/ampxsdk/can/S32K1x
  ${CMAKE_CURRENT_LIST_DIR}/src/support/mcuxsdk_usb
)

