var a00038 =
[
    [ "i2c_slave_address_t", "a00038.html#a00373", [
      [ "address", "a00038.html#ad7af2a58faba6a178daa97bd06ebce36", null ],
      [ "addressDisable", "a00038.html#aca3dcdb3ab2710d991ada52d64bf102c", null ]
    ] ],
    [ "i2c_slave_config_t", "a00038.html#a00374", [
      [ "address0", "a00038.html#a4738c7cd55260f7e8a3825d0b2278a34", null ],
      [ "address1", "a00038.html#ae19c45c96699bb3a6821150ab820b029", null ],
      [ "address2", "a00038.html#ae855ba5c53f7e585c44eae8bada85e9d", null ],
      [ "address3", "a00038.html#a213d1737a633686701581a09859213a6", null ],
      [ "qualMode", "a00038.html#a51577d06d8812b0c999957898ec016b5", null ],
      [ "qualAddress", "a00038.html#a4c9d09c35c4641d38bf565de31b07320", null ],
      [ "busSpeed", "a00038.html#a14acc40a290c779fde0825f3a8bdbb25", null ],
      [ "enableSlave", "a00038.html#a92d20835618a946b8f7702455877ef7a", null ]
    ] ],
    [ "i2c_slave_transfer_t", "a00038.html#a00375", [
      [ "handle", "a00038.html#ab74516c1edb1424ddb1554de7cae69bc", null ],
      [ "event", "a00038.html#ad0a9e837e9df16d89bab98a78af5bd10", null ],
      [ "receivedAddress", "a00038.html#ad6f3b291ee81b69cf91c161ae26d65ae", null ],
      [ "eventMask", "a00038.html#a1be21257ec375e09106dfe242c02beea", null ],
      [ "rxData", "a00038.html#a3e497078c6038baa8748ce5ecebc7e3d", null ],
      [ "txData", "a00038.html#afaff4f7ef2ae4ed49eff607a58db18c2", null ],
      [ "txSize", "a00038.html#a3d2ef683b0439ce83e3d54e8823ebc38", null ],
      [ "rxSize", "a00038.html#a75e06358c0d5840a1dfc19a029e10ba8", null ],
      [ "transferredCount", "a00038.html#a0394563c8d0f9eeeecd242a65cee2ad4", null ],
      [ "completionStatus", "a00038.html#a35adbf64ca65dd2c1b52f9260f5b9e90", null ]
    ] ],
    [ "i2c_slave_handle_t", "a00038.html#a00297", [
      [ "transfer", "a00038.html#a9934cd1a4d4b4e02ac5856e28f86a229", null ],
      [ "isBusy", "a00038.html#a81ece18a362fc9779750be91f7cc6b30", null ],
      [ "slaveFsm", "a00038.html#a18589a5de9b06b2eaf7d4260eac03a58", null ],
      [ "callback", "a00038.html#a7229e894f762ead4bd08b4add49e6bc2", null ],
      [ "userData", "a00038.html#a98ea5e99278b386e2ddb99d45a9750ee", null ]
    ] ],
    [ "i2c_slave_transfer_callback_t", "a00038.html#ga0704c8f14da38feb11555e4127d86a18", null ],
    [ "flexcomm_i2c_master_irq_handler_t", "a00038.html#gac86dc9a3b232a90765a2eb7df0eb9217", null ],
    [ "flexcomm_i2c_slave_irq_handler_t", "a00038.html#gad3e88bbb01efa34d7df40e87734aa4f1", null ],
    [ "i2c_slave_address_register_t", "a00038.html#ga833a7311515f1a3bf5cb8da2355cc661", [
      [ "kI2C_SlaveAddressRegister0", "a00038.html#gga833a7311515f1a3bf5cb8da2355cc661a39e0e158674bd8a6e254a6a42b5ce50f", null ],
      [ "kI2C_SlaveAddressRegister1", "a00038.html#gga833a7311515f1a3bf5cb8da2355cc661a42a589b5e31f29ddc41088c3fb4bfcf1", null ],
      [ "kI2C_SlaveAddressRegister2", "a00038.html#gga833a7311515f1a3bf5cb8da2355cc661a8381dd229f545df5e6a32ee5f0aac871", null ],
      [ "kI2C_SlaveAddressRegister3", "a00038.html#gga833a7311515f1a3bf5cb8da2355cc661a9e73a2b09fe3943d51849a7d87fbe8a3", null ]
    ] ],
    [ "i2c_slave_address_qual_mode_t", "a00038.html#ga719dc02b99647eb8f08a05d4d6066c51", [
      [ "kI2C_QualModeMask", "a00038.html#gga719dc02b99647eb8f08a05d4d6066c51af542f0269c83549e7b492911e281a902", null ],
      [ "kI2C_QualModeExtend", "a00038.html#gga719dc02b99647eb8f08a05d4d6066c51a6b755a1d30a626380730ce947d09335f", null ]
    ] ],
    [ "i2c_slave_bus_speed_t", "a00038.html#ga5f368505586dd356fc680711023ace7f", null ],
    [ "i2c_slave_transfer_event_t", "a00038.html#gac53e5c96a2eed1b5a95b7d84be48f4ac", [
      [ "kI2C_SlaveAddressMatchEvent", "a00038.html#ggac53e5c96a2eed1b5a95b7d84be48f4acacb1ae174dd6798a6fd79800a9e39a3c6", null ],
      [ "kI2C_SlaveTransmitEvent", "a00038.html#ggac53e5c96a2eed1b5a95b7d84be48f4aca2f85039a57379838909876a1d509b7aa", null ],
      [ "kI2C_SlaveReceiveEvent", "a00038.html#ggac53e5c96a2eed1b5a95b7d84be48f4acaac8593e808a8137d1b5d3a51640779de", null ],
      [ "kI2C_SlaveCompletionEvent", "a00038.html#ggac53e5c96a2eed1b5a95b7d84be48f4aca38cca3cd668e73f51b8e574835d419df", null ],
      [ "kI2C_SlaveDeselectedEvent", "a00038.html#ggac53e5c96a2eed1b5a95b7d84be48f4aca5e765c1c316a51908b68f1f32a7b524d", null ],
      [ "kI2C_SlaveAllEvents", "a00038.html#ggac53e5c96a2eed1b5a95b7d84be48f4aca94b030ebdac378a84961893ae567bfbc", null ]
    ] ],
    [ "i2c_slave_fsm_t", "a00038.html#gad6564299304730416461f62255fcd37c", null ],
    [ "I2C_SlaveGetDefaultConfig", "a00038.html#ga7115f80e28b62bbbd3be1a2a918529ba", null ],
    [ "I2C_SlaveInit", "a00038.html#ga175a81b5ab74dd78469d6cdea690d8cc", null ],
    [ "I2C_SlaveSetAddress", "a00038.html#gacf721065f83982e3df651111183bf7b2", null ],
    [ "I2C_SlaveDeinit", "a00038.html#gadb50014db29dd75ea2001fb7bc837e77", null ],
    [ "I2C_SlaveEnable", "a00038.html#gacbb93d5ea4f4b40038f41e0e942a9ee1", null ],
    [ "I2C_SlaveClearStatusFlags", "a00038.html#ga545c1a43d8d49c4675de493d45a4063a", null ],
    [ "I2C_SlaveWriteBlocking", "a00038.html#ga280404c6b1ede542f2c1c20bd382869c", null ],
    [ "I2C_SlaveReadBlocking", "a00038.html#ga89baaf8bc82b19c837032665e89a1707", null ],
    [ "I2C_SlaveTransferCreateHandle", "a00038.html#gac9b90f575d92ff8ad3cc350a5c8ad1b9", null ],
    [ "I2C_SlaveTransferNonBlocking", "a00038.html#gaa387d81ea08a9d4b39cfeb3dfec505f7", null ],
    [ "I2C_SlaveSetSendBuffer", "a00038.html#gac7602b6897596348fe190e4fc63f1381", null ],
    [ "I2C_SlaveSetReceiveBuffer", "a00038.html#ga25b9c35089f0fc966facec3bf874c840", null ],
    [ "I2C_SlaveGetReceivedAddress", "a00038.html#ga4c9fc06eeb8a3beb024f6525e4d99a0b", null ],
    [ "I2C_SlaveTransferAbort", "a00038.html#ga5ae9f5e6e854d14d33ba3029128bd6f0", null ],
    [ "I2C_SlaveTransferGetCount", "a00038.html#gabf38a498bd3527ea0b7cc947ae71e17e", null ],
    [ "I2C_SlaveTransferHandleIRQ", "a00038.html#ga3bc939e5dbdc165169f58b8d47d1742d", null ]
];