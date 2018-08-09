var group___g_a_t_t___s_e_r_v_e_r =
[
    [ "gatt_server_interface.h", "gatt__server__interface_8h.html", null ],
    [ "gattServerMtuChangedEvent_t", "group___g_a_t_t___s_e_r_v_e_r.html#structgatt_server_mtu_changed_event__t", [
      [ "newMtu", "group___g_a_t_t___s_e_r_v_e_r.html#a8815b35ac5caa4292e16ed8c88a2b215", null ]
    ] ],
    [ "gattServerAttributeWrittenEvent_t", "group___g_a_t_t___s_e_r_v_e_r.html#structgatt_server_attribute_written_event__t", [
      [ "handle", "group___g_a_t_t___s_e_r_v_e_r.html#a93e1ffa17c219321bfa1eb0c298a5408", null ],
      [ "cValueLength", "group___g_a_t_t___s_e_r_v_e_r.html#a1c022a778a0caaa97dd1e20a8e4555fa", null ],
      [ "aValue", "group___g_a_t_t___s_e_r_v_e_r.html#a9e55fe1ddfe466123ac5cd89097bb709", null ]
    ] ],
    [ "gattServerLongCharacteristicWrittenEvent_t", "group___g_a_t_t___s_e_r_v_e_r.html#structgatt_server_long_characteristic_written_event__t", [
      [ "handle", "group___g_a_t_t___s_e_r_v_e_r.html#a3fb50f5ee4202379f3047dcf671a9578", null ],
      [ "cValueLength", "group___g_a_t_t___s_e_r_v_e_r.html#a27329a51988ee0ecd984402e1c39f213", null ],
      [ "aValue", "group___g_a_t_t___s_e_r_v_e_r.html#acb854d56696512dba5475ccfd2193bc0", null ]
    ] ],
    [ "gattServerCccdWrittenEvent_t", "group___g_a_t_t___s_e_r_v_e_r.html#structgatt_server_cccd_written_event__t", [
      [ "handle", "group___g_a_t_t___s_e_r_v_e_r.html#a137a1a1444be3a5cb8b69dff3ebaaba4", null ],
      [ "newCccd", "group___g_a_t_t___s_e_r_v_e_r.html#ab2ac535f522344467a3b6cdc70c8f82e", null ]
    ] ],
    [ "gattServerAttributeReadEvent_t", "group___g_a_t_t___s_e_r_v_e_r.html#structgatt_server_attribute_read_event__t", [
      [ "handle", "group___g_a_t_t___s_e_r_v_e_r.html#a6efd0ee404e39d5a0942360456f2a2a2", null ]
    ] ],
    [ "gattServerProcedureError_t", "group___g_a_t_t___s_e_r_v_e_r.html#structgatt_server_procedure_error__t", [
      [ "procedureType", "group___g_a_t_t___s_e_r_v_e_r.html#a6b963a5fabab2c2f3bce1cc56d56934b", null ],
      [ "error", "group___g_a_t_t___s_e_r_v_e_r.html#a379f02c1c44eccc16e863ecb589cabfa", null ]
    ] ],
    [ "gattServerEvent_t", "group___g_a_t_t___s_e_r_v_e_r.html#structgatt_server_event__t", [
      [ "eventType", "group___g_a_t_t___s_e_r_v_e_r.html#ac73dfc1f48b0faf433fa243ca692e19e", null ],
      [ "eventData", "group___g_a_t_t___s_e_r_v_e_r.html#a3d32c7201d768783865ca6b64a5aac50", null ]
    ] ],
    [ "gattServerEvent_t.eventData", "group___g_a_t_t___s_e_r_v_e_r.html#uniongatt_server_event__t_8event_data", [
      [ "mtuChangedEvent", "group___g_a_t_t___s_e_r_v_e_r.html#aecce122ddef9614e8211801e51faa8cd", null ],
      [ "attributeWrittenEvent", "group___g_a_t_t___s_e_r_v_e_r.html#a78ce19008e516cd3089fe530173db6de", null ],
      [ "charCccdWrittenEvent", "group___g_a_t_t___s_e_r_v_e_r.html#aec48c5fa1873b5039aba161a3a2242e9", null ],
      [ "procedureError", "group___g_a_t_t___s_e_r_v_e_r.html#a54cccb9a8c6a494359376fc73a50c183", null ],
      [ "longCharWrittenEvent", "group___g_a_t_t___s_e_r_v_e_r.html#a0a292b0f789cc87f71b41a6047fadcec", null ],
      [ "attributeReadEvent", "group___g_a_t_t___s_e_r_v_e_r.html#ac17c2cb0c8721cbbf4a3ec69b35ac52e", null ]
    ] ],
    [ "gattServerCallback_t", "group___g_a_t_t___s_e_r_v_e_r.html#ga7dcecbb0050759bed77827002516765b", null ],
    [ "gattServerEventType_t", "group___g_a_t_t___s_e_r_v_e_r.html#ga6806d32e99a3229ad636f89f3840fcc0", [
      [ "gEvtMtuChanged_c", "group___g_a_t_t___s_e_r_v_e_r.html#gga6806d32e99a3229ad636f89f3840fcc0a424f95d633b128801165a213cc992b14", null ],
      [ "gEvtHandleValueConfirmation_c", "group___g_a_t_t___s_e_r_v_e_r.html#gga6806d32e99a3229ad636f89f3840fcc0a55bf7fd0e99efab08fbc0bd62b45418b", null ],
      [ "gEvtAttributeWritten_c", "group___g_a_t_t___s_e_r_v_e_r.html#gga6806d32e99a3229ad636f89f3840fcc0a9fa95d29e2b12047caa7c1137bfae558", null ],
      [ "gEvtCharacteristicCccdWritten_c", "group___g_a_t_t___s_e_r_v_e_r.html#gga6806d32e99a3229ad636f89f3840fcc0a09564e0b3fd1c8d098b538a22603a84c", null ],
      [ "gEvtAttributeWrittenWithoutResponse_c", "group___g_a_t_t___s_e_r_v_e_r.html#gga6806d32e99a3229ad636f89f3840fcc0ab143d9a8a9614f31e233e86855f7c228", null ],
      [ "gEvtError_c", "group___g_a_t_t___s_e_r_v_e_r.html#gga6806d32e99a3229ad636f89f3840fcc0a4f56164bc06f949dac9ac840dcf8c762", null ],
      [ "gEvtLongCharacteristicWritten_c", "group___g_a_t_t___s_e_r_v_e_r.html#gga6806d32e99a3229ad636f89f3840fcc0afd8ce826086ef0ee6d90b134e255a9ac", null ],
      [ "gEvtAttributeRead_c", "group___g_a_t_t___s_e_r_v_e_r.html#gga6806d32e99a3229ad636f89f3840fcc0ab10fce66411958117986397522e4fcd1", null ]
    ] ],
    [ "gattServerProcedureType_t", "group___g_a_t_t___s_e_r_v_e_r.html#ga27c3a9d2dcf345e4b6a47d475d593535", [
      [ "gSendAttributeWrittenStatus_c", "group___g_a_t_t___s_e_r_v_e_r.html#gga27c3a9d2dcf345e4b6a47d475d593535a599e056d750aab4d49a4587e317db25f", null ],
      [ "gSendAttributeReadStatus_c", "group___g_a_t_t___s_e_r_v_e_r.html#gga27c3a9d2dcf345e4b6a47d475d593535a7ee38173dbd9bd0b9f5f1b76ee0a5eaf", null ],
      [ "gSendNotification_c", "group___g_a_t_t___s_e_r_v_e_r.html#gga27c3a9d2dcf345e4b6a47d475d593535a942e052d14f7528e00b57a2ef735c080", null ],
      [ "gSendIndication_c", "group___g_a_t_t___s_e_r_v_e_r.html#gga27c3a9d2dcf345e4b6a47d475d593535a6155665e4017de2f93802932f71f9568", null ]
    ] ],
    [ "GattServer_Init", "group___g_a_t_t___s_e_r_v_e_r.html#gaeb6079d09d95f9312dc2677a99ed2e45", null ],
    [ "GattServer_RegisterCallback", "group___g_a_t_t___s_e_r_v_e_r.html#ga2e7581307bb568f2c828dad62f2dad45", null ],
    [ "GattServer_RegisterHandlesForWriteNotifications", "group___g_a_t_t___s_e_r_v_e_r.html#ga6ff81cac0be312fd79698030d3fffb37", null ],
    [ "GattServer_SendAttributeWrittenStatus", "group___g_a_t_t___s_e_r_v_e_r.html#ga7c35c777ec60e926b77fb630a5dbc6c7", null ],
    [ "GattServer_RegisterHandlesForReadNotifications", "group___g_a_t_t___s_e_r_v_e_r.html#gaed1479c9cd83f284aae60490692afd22", null ],
    [ "GattServer_SendAttributeReadStatus", "group___g_a_t_t___s_e_r_v_e_r.html#gaba56ef1080a8f74496b533c6ce2fde1c", null ],
    [ "GattServer_SendNotification", "group___g_a_t_t___s_e_r_v_e_r.html#ga2ac10b90addee712e1ec029d8fabccdf", null ],
    [ "GattServer_SendIndication", "group___g_a_t_t___s_e_r_v_e_r.html#ga917975dbb53b33dabb255a969c844a78", null ],
    [ "GattServer_SendInstantValueNotification", "group___g_a_t_t___s_e_r_v_e_r.html#ga2b103e1b4c95d43573faa6112609674b", null ],
    [ "GattServer_SendInstantValueIndication", "group___g_a_t_t___s_e_r_v_e_r.html#gaa580ed955583ba39d1a0a354875e06b0", null ]
];