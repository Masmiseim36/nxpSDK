var a00030 =
[
    [ "FLEXIO_I2C_Type", "a00030.html#a00404", [
      [ "flexioBase", "a00030.html#a85065e51da3c0158d4b1d9254d691d26", null ],
      [ "SDAPinIndex", "a00030.html#acf0bab2399a7d7013f3708feb4d74ff6", null ],
      [ "SCLPinIndex", "a00030.html#a8ffb4cb7eac22b561f34ebcf4c309db9", null ],
      [ "shifterIndex", "a00030.html#ab1a64e477a45a6f98cd835644c6699ec", null ],
      [ "timerIndex", "a00030.html#ab468253ce0595401dac6cbe2ede4c82a", null ],
      [ "baudrate", "a00030.html#a34f7664f9c39134c78a668c718040c54", null ]
    ] ],
    [ "flexio_i2c_master_config_t", "a00030.html#a00402", [
      [ "enableMaster", "a00030.html#a4dcbed2930b467df1978f9bf2b71c128", null ],
      [ "enableInDoze", "a00030.html#ad73b88c0700d82721ccbf0744d1b6780", null ],
      [ "enableInDebug", "a00030.html#acf98ed2c52a014d2ed825a38293001d4", null ],
      [ "enableFastAccess", "a00030.html#a89a0daa456c7d99a7144d5245f422bbe", null ],
      [ "baudRate_Bps", "a00030.html#a446dee2e270776aa6d462710e85b7454", null ]
    ] ],
    [ "flexio_i2c_master_transfer_t", "a00030.html#a00403", [
      [ "flags", "a00030.html#a2695cc7566c8a4cbbcf837d1f7209b6a", null ],
      [ "slaveAddress", "a00030.html#ad9a4b7694d338eaa4d7b6b2e8ebcf6c5", null ],
      [ "direction", "a00030.html#a756cb7ae9db1832fcffcd736a7039369", null ],
      [ "subaddress", "a00030.html#a60552386bd16b5d17197660571638898", null ],
      [ "subaddressSize", "a00030.html#a17d04e1156e9dd99026a4b527e487a96", null ],
      [ "data", "a00030.html#a94b0a6518c96392dd0497350af675ada", null ],
      [ "dataSize", "a00030.html#a5bd368a720ef8ed7df1601d7cac19ae2", null ]
    ] ],
    [ "flexio_i2c_master_handle_t", "a00030.html#a00306", [
      [ "transfer", "a00030.html#a60bd61f7bad1102b8ae17c147ebe7562", null ],
      [ "transferSize", "a00030.html#a48d8d3dd2b37359e54ee1ac10d68bfa5", null ],
      [ "state", "a00030.html#abdeb9df90f61127f95734a9ad2226de8", null ],
      [ "completionCallback", "a00030.html#a1b3ef650c54e91d7ae83998a7a3cb061", null ],
      [ "userData", "a00030.html#affef8570c9455a63c5b0d67595f1dc6a", null ],
      [ "needRestart", "a00030.html#aaf7dbf9de526b48d31035afdf18a2d8d", null ]
    ] ],
    [ "I2C_RETRY_TIMES", "a00030.html#gaac288786aaa7d0cb947c91591baf42d7", null ],
    [ "flexio_i2c_master_transfer_callback_t", "a00030.html#ga4a92b2aafd0ea1e7746dc23a7b0f0691", [
      [ "kStatus_FLEXIO_I2C_Busy", "a00030.html#ggae4d5251432e1a9e6803c0240cc492e18a8ab8145bc386f3630f13b87c38496331", null ],
      [ "kStatus_FLEXIO_I2C_Idle", "a00030.html#ggae4d5251432e1a9e6803c0240cc492e18a2f105638fc06422cc54cf5337d507873", null ],
      [ "kStatus_FLEXIO_I2C_Nak", "a00030.html#ggae4d5251432e1a9e6803c0240cc492e18af5e4a932f6f9e6ae30a42dc31fab8bd5", null ],
      [ "kStatus_FLEXIO_I2C_Timeout", "a00030.html#ggae4d5251432e1a9e6803c0240cc492e18a3d8bcc4ffd332948cb8b28f7982ac4d1", null ]
    ] ],
    [ "_flexio_i2c_master_interrupt", "a00030.html#gaa4597b532bb2a564c329076dc5da8155", [
      [ "kFLEXIO_I2C_TxEmptyInterruptEnable", "a00030.html#ggaa4597b532bb2a564c329076dc5da8155ac34fefcbb2d45dd7bfd385586439923b", null ],
      [ "kFLEXIO_I2C_RxFullInterruptEnable", "a00030.html#ggaa4597b532bb2a564c329076dc5da8155a46a53a6077c43dfb924d46e9a218eb6b", null ]
    ] ],
    [ "_flexio_i2c_master_status_flags", "a00030.html#gafc92480bf28b1161b62a1f10a23e7458", [
      [ "kFLEXIO_I2C_TxEmptyFlag", "a00030.html#ggafc92480bf28b1161b62a1f10a23e7458a5eb41467be294db469f4604a010405e3", null ],
      [ "kFLEXIO_I2C_RxFullFlag", "a00030.html#ggafc92480bf28b1161b62a1f10a23e7458a4eba9f70c326fcea72a255cba80dc42d", null ],
      [ "kFLEXIO_I2C_ReceiveNakFlag", "a00030.html#ggafc92480bf28b1161b62a1f10a23e7458a8774a1fedf2a596d2647864bd4f27664", null ]
    ] ],
    [ "flexio_i2c_direction_t", "a00030.html#gad7cd7cc57788188d0f804b5e3a549303", [
      [ "kFLEXIO_I2C_Write", "a00030.html#ggad7cd7cc57788188d0f804b5e3a549303a666a4c181486b773264ce1e038e14850", null ],
      [ "kFLEXIO_I2C_Read", "a00030.html#ggad7cd7cc57788188d0f804b5e3a549303aeac2a6bd710680e5f644763dea0c89c4", null ]
    ] ],
    [ "FLEXIO_I2C_CheckForBusyBus", "a00030.html#ga2ded19f42c2b7e09764e6e926c501b67", null ],
    [ "FLEXIO_I2C_MasterInit", "a00030.html#gaaacd8cca7de1ff3a8c33c86d4f7f88fd", null ],
    [ "FLEXIO_I2C_MasterDeinit", "a00030.html#ga2da731bfa4e1e36b5d6d0235898b26dd", null ],
    [ "FLEXIO_I2C_MasterGetDefaultConfig", "a00030.html#gaa85599771e53091102f0823217ad0bcc", null ],
    [ "FLEXIO_I2C_MasterEnable", "a00030.html#ga287dc7896229cd705bb8abbf51ca63a7", null ],
    [ "FLEXIO_I2C_MasterGetStatusFlags", "a00030.html#ga5ac748fc7ff593cfa7a9b9a64cd479fa", null ],
    [ "FLEXIO_I2C_MasterClearStatusFlags", "a00030.html#gad7509c6f4a8885650c313bb0b56e6bfc", null ],
    [ "FLEXIO_I2C_MasterEnableInterrupts", "a00030.html#ga07198112aef949ac73874e7d446f7d38", null ],
    [ "FLEXIO_I2C_MasterDisableInterrupts", "a00030.html#ga3fcce197e268d6e7310a0087d9288b08", null ],
    [ "FLEXIO_I2C_MasterSetBaudRate", "a00030.html#gac1cb6e0f52c2e9e2b4373415dff1f24c", null ],
    [ "FLEXIO_I2C_MasterStart", "a00030.html#ga6f9def4b064e4747675dfc16e9fdde00", null ],
    [ "FLEXIO_I2C_MasterStop", "a00030.html#ga6b921453ee8e263f9a2a198b497c88ec", null ],
    [ "FLEXIO_I2C_MasterRepeatedStart", "a00030.html#ga8c2b3776c1630e0bf8e64927a0012b2f", null ],
    [ "FLEXIO_I2C_MasterAbortStop", "a00030.html#ga20efb0f28522d945694d509b5e4bf52d", null ],
    [ "FLEXIO_I2C_MasterEnableAck", "a00030.html#gaca2e797a84bc5b68a75ba10cf2750fc0", null ],
    [ "FLEXIO_I2C_MasterSetTransferCount", "a00030.html#ga6239ded74ab146f5f04c6355edf6e64a", null ],
    [ "FLEXIO_I2C_MasterWriteByte", "a00030.html#ga7e82edf243f26522ea3ecedddb4760ba", null ],
    [ "FLEXIO_I2C_MasterReadByte", "a00030.html#ga2a790d99cbabf8bf7df3216b6da904e5", null ],
    [ "FLEXIO_I2C_MasterWriteBlocking", "a00030.html#ga13c8be777cb7336878e161ab32fba000", null ],
    [ "FLEXIO_I2C_MasterReadBlocking", "a00030.html#ga5713e8dca1e7e896c3aed89e8961547a", null ],
    [ "FLEXIO_I2C_MasterTransferBlocking", "a00030.html#ga7b4b3c280a860777e98b5065225a3150", null ],
    [ "FLEXIO_I2C_MasterTransferCreateHandle", "a00030.html#ga2d26c2cb21b034487ba8b8d4c5da2ef5", null ],
    [ "FLEXIO_I2C_MasterTransferNonBlocking", "a00030.html#ga4e21323685831e0ee3f88d50e4c8872c", null ],
    [ "FLEXIO_I2C_MasterTransferGetCount", "a00030.html#ga79fed99f5f4524619e2c2e28ad037544", null ],
    [ "FLEXIO_I2C_MasterTransferAbort", "a00030.html#ga75ecfe909fcd0ae47303eb90af39dea2", null ],
    [ "FLEXIO_I2C_MasterTransferHandleIRQ", "a00030.html#ga29129d78b9166b7bcb585e336746bec6", null ]
];